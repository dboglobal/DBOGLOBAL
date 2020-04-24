#include "Precomp_NtlPathEngine.h"
#include "NtlMeshFederationBuilder.h"

// Log
#include "NtlPathEngineLog.h"

// Util
#include "NtlFileOutputStream.h"
#include "NtlSimpleDOM.h"

// Federation
#include "NtlMeshFederationTileGeneratedCallBack.h"

// PathEngine
#include "NtlPathEngine.h"

// Helper
#include "NtlPathEngineHelper.h"

// STL
#include <sstream>
#include <string>
#include <fstream>

CNtlMeshFederationBuilder::CNtlMeshFederationBuilder( void )
{
	
}

CNtlMeshFederationBuilder::~CNtlMeshFederationBuilder( void )
{

}

/**
* \brief WorldMesh에서 Federation을 사용한다.
*/
void CNtlMeshFederationBuilder::BuildFederation_FromWorldMesh( const char* pWorldMeshName, long tileSize, long overlap )
{
	// Mesh 전체를 읽어들인다.
	iMesh* worldMesh;
	{
		char* buffer;
		unsigned long size;
		std::string meshPath = "";
		meshPath.append(pWorldMeshName);
		meshPath.append(NTLPE_DATA_EXTENSION);
		buffer = NTLPE_LoadBinary_AllocatedBuffer(meshPath.c_str(), size);
		worldMesh = GetNtlPathEngine()->GetPathEngine()->loadMeshFromBuffer(NTLPE_DATA_FORMAT, buffer, size, 0);
		NTLPE_FreeBuffer(buffer);
	}

	// Mesh Federation Tile Generated Callback 생성
	CNtlMeshFederationTileGeneratedCallBack callback( pWorldMeshName );

	// Federation 들을 생성한다. 생성되는 각 타일들은 callback 클래스에 의해서 제어된다.
	iMeshFederation* federation;
	federation = GetNtlPathEngine()->GetPathEngine()->buildMeshFederation_FromWorldMesh(
		worldMesh,
		tileSize, overlap,
		callback
		);

	// 생성된 Federation의 정보를 헤더로서 저장한다.
	std::ostringstream fileName;
	fileName << pWorldMeshName;
	fileName << NTLPE_FEDERATION_APPEND_HEADER;
	fileName << NTLPE_DATA_EXTENSION;
	CNtlFileOutputStream fos(fileName.str().c_str());
	federation->save(NTLPE_DATA_FORMAT, &fos);
	delete federation;
}

void CNtlMeshFederationBuilder::BuildFederation_FromWorldMesh( const char* pWorldMeshName, long tileSize, long overlap, bool bPreProcessCollision, bool bPreProcessPathFind, std::vector< int >& vecAgentSize )
{
	// Mesh 전체를 읽어들인다.
	iMesh* worldMesh;
	{
		char* buffer;
		unsigned long size;
		std::string meshPath = "";
		meshPath.append(pWorldMeshName);
		meshPath.append(NTLPE_DATA_EXTENSION);
		buffer = NTLPE_LoadBinary_AllocatedBuffer(meshPath.c_str(), size);
		worldMesh = GetNtlPathEngine()->GetPathEngine()->loadMeshFromBuffer(NTLPE_DATA_FORMAT, buffer, size, 0);
		NTLPE_FreeBuffer(buffer);
	}

	// Mesh Federation Tile Generated Callback 생성
	CNtlMeshFederationTileGeneratedCallBack callback( pWorldMeshName, bPreProcessCollision, bPreProcessPathFind, &vecAgentSize );

	// Federation 들을 생성한다. 생성되는 각 타일들은 callback 클래스에 의해서 제어된다.
	iMeshFederation* federation;
	federation = GetNtlPathEngine()->GetPathEngine()->buildMeshFederation_FromWorldMesh(
		worldMesh,
		tileSize, overlap,
		callback
		);

	// 생성된 Federation의 정보를 헤더로서 저장한다.
	std::ostringstream fileName;
	fileName << pWorldMeshName;
	fileName << NTLPE_FEDERATION_APPEND_HEADER;
	fileName << NTLPE_DATA_EXTENSION;
	CNtlFileOutputStream fos(fileName.str().c_str());
	federation->save(NTLPE_DATA_FORMAT, &fos);
	delete federation;
}
/**
* \brief placement 정보로 Federation을 사용한다.
*/
void CNtlMeshFederationBuilder::BuildFederation_TileByTile( const char* placementFilename, long tileSize, long overlap )
{
	// PlaceMent 정보가 담긴 text 파일을 파싱한다.
	CNtlSimpleDOM placementScript;
	{
		std::string fileName("");
		fileName.append(placementFilename);
		fileName.append(".txt");
		std::ifstream is(fileName.c_str());
		if(!is.good())
		{
			CNtlPathEngineLog::GetInstance()->Log("Fatal - Could not open content chunk placement script.");
			return;
		}

		CNtlSimpleDOM::LoadWhiteSpaceDelimited(is, placementScript);
	}

	std::vector<iContentChunk*> loadedChunks;
	std::vector<iContentChunkInstance*> placedInstances;

	// Chunk를 로드하고
	// Instance를 생성한다.
	LoadContentChunkPlacement(placementScript, loadedChunks, placedInstances, true);
	if(placedInstances.empty())
	{
		CNtlPathEngineLog::GetInstance()->handle("Fatal", "No content chunk instances placed.");
		return;
	}

	// WorldRange 를 설정해준다.
	
	cHorizontalRange worldRange;
	
	// Test용 코드 ( MultiField )
	worldRange.minX = -204800;
	worldRange.minY = -204800;
	worldRange.maxX = 204800;
	worldRange.maxY = 204800;
	
	///*GetWorldRange*/(placedInstances, worldRange);

	iMeshFederation* federation = GetNtlPathEngine()->GetPathEngine()->buildMeshFederation_TilingOnly(worldRange, tileSize, overlap);

	// Tile 들을 생성해준다.
	long i;
	for(i = 0; i != federation->size(); ++i)
	{
		BuildTile(placementFilename, federation, i, placedInstances);
	}

	// Header file 생성
	std::ostringstream fileName;
	fileName << placementFilename;
	fileName << NTLPE_DATA_EXTENSION;

	CNtlFileOutputStream fos(fileName.str().c_str());
	federation->save(NTLPE_DATA_FORMAT, &fos);
	delete federation;

	for(size_t i = 0; i != loadedChunks.size(); ++i)
	{
		delete loadedChunks[i];
	}
}

void CNtlMeshFederationBuilder::LoadContentChunkPlacement( const CNtlSimpleDOM& placementScript, std::vector<iContentChunk*>& loadedChunks, std::vector<iContentChunkInstance*>& placedInstances, bool setUniqueSectionIDs )
{
	//assertR(placementScript._name == "contentChunkPlacement");
	std::map<std::string, iContentChunk*> loadedChunksMap;
	long i;
	for(i = 0; i != placementScript._children.size(); ++i)
	{
		long nextIndex = static_cast<long>(placedInstances.size()) + 1; // zero is reserved for terrain stand-in

		// process the document element
		const CNtlSimpleDOM& instanceDOM = placementScript._children[i];
		if(instanceDOM._name == "chunk")
		{
			const std::string chunkName = instanceDOM.getAttribute("name");
			iContentChunk* chunk = loadedChunksMap[chunkName];
			if(chunk == 0)
			{
				chunk = LoadChunk(chunkName);
				loadedChunksMap[chunkName] = chunk;
				loadedChunks.push_back(chunk);
			}

			long sectionID = -1; // passing -1 tells iContentChunk::instantiate() not to override sectionID
			if(setUniqueSectionIDs)
			{
				sectionID = nextIndex;
			}

			iContentChunkInstance* instance;
			instance = chunk->instantiate(
				instanceDOM.attributeAsLong("rotation"),                
				instanceDOM.attributeAsLong("scale"),
				instanceDOM.attributeAsLong("x"),
				instanceDOM.attributeAsLong("y"),
				instanceDOM.attributeAsFloat("z"),
				sectionID
				);
			placedInstances.push_back(instance);
			continue;
		}

		if(instanceDOM._name == "edgeToEdgeConnection")
		{
			long from = instanceDOM.attributeAsLong("fromInstance");
			long fromEdgeIndex = instanceDOM.attributeAsLong("fromEdgeIndex");
			long to = instanceDOM.attributeAsLong("toInstance");
			long toEdgeIndex = instanceDOM.attributeAsLong("toEdgeIndex");
			if(from <= 0 || from >= nextIndex)
			{
				CNtlPathEngineLog::GetInstance()->handle("NonFatal", "Bad fromInstance value in edgeToEdgeConnection element.");
				continue;
			}
			if(to <= 0 || to >= nextIndex)
			{
				CNtlPathEngineLog::GetInstance()->handle("NonFatal", "Bad toInstance value in edgeToEdgeConnection element.");
				continue;
			}
			iContentChunkInstance* fromInstance = placedInstances[from - 1];
			iContentChunkInstance* toInstance = placedInstances[to - 1];
			if(fromEdgeIndex < 0 || fromEdgeIndex >= fromInstance->numberOfConnectingEdges())
			{
				CNtlPathEngineLog::GetInstance()->handle("NonFatal", "Bad fromEdgeIndex value in edgeToEdgeConnection element.");
				continue;
			}
			if(toEdgeIndex < 0 || toEdgeIndex >= toInstance->numberOfConnectingEdges())
			{
				CNtlPathEngineLog::GetInstance()->handle("NonFatal", "Bad toEdgeIndex value in edgeToEdgeConnection element.");
				continue;
			}
			if(!fromInstance->edgesCanConnect(fromEdgeIndex, toInstance, toEdgeIndex))
			{
				CNtlPathEngineLog::GetInstance()->handle("NonFatal", "An edge pair specified as connecting in the placement file cannot be connected.");
				continue;
			}

			fromInstance->makeEdgeConnection(fromEdgeIndex, toInstance, toEdgeIndex);
			continue;
		}
	}
}

iContentChunk* CNtlMeshFederationBuilder::LoadChunk( const std::string& name )
{
	char* buffer;
	unsigned long bufferSize;
	std::string fileName = "";
	fileName.append(name);
	fileName.append(NTLPE_DATA_EXTENSION);
	buffer = NTLPE_LoadBinary_AllocatedBuffer(fileName.c_str(), bufferSize);
	iContentChunk* result = GetNtlPathEngine()->GetPathEngine()->loadContentChunk(buffer, bufferSize);
	NTLPE_FreeBuffer(buffer);
	return result;
}

void CNtlMeshFederationBuilder::BuildTile( const char* tileName, iMeshFederation* federation, long tileIndex, const std::vector<iContentChunkInstance*>& placedInstances )
{
	std::vector<const iContentChunkInstance*> overlappingInstances;

	cHorizontalRange representedRegion;
	federation->getRepresentedRegion_World(tileIndex, representedRegion);

	long i;
	for(i = 0; i != static_cast<long>(placedInstances.size()); ++i)
	{
		cHorizontalRange instanceRange;
		placedInstances[i]->getHorizontalRange(instanceRange);
		if(NTLPE_RangesOverlap(instanceRange, representedRegion))
		{
			overlappingInstances.push_back(placedInstances[i]);
		}
	}

	const iContentChunkInstance** instancesBuffer = 0;
	if(!overlappingInstances.empty())
	{
		instancesBuffer = &overlappingInstances[0];
	}

	iOutputStream* snapshotOSPtr = 0;

	// the following lines can be uncommented to write a file containing a snapshot of the data being passed into content processing
	// this can be imported into 3DS Max, and is useful for troubleshooting any problems with the content processing
	//    std::ostringstream snapshotFileName;
	//    snapshotFileName << "..\\resource\\meshes\\federationTile" << tileIndex << ".sourceContent.tok";
	//    cFileOutputStream snapshotFOS(snapshotFileName.str().c_str());
	//    snapshotOSPtr = &snapshotFOS;

	iMesh* tileMesh = federation->buildTileMeshFromContent(
		tileIndex, 
		instancesBuffer, static_cast<long>(overlappingInstances.size()),
		0, // no additional options
		snapshotOSPtr
		);

	std::ostringstream fileName;
	fileName << tileName << tileIndex << NTLPE_DATA_EXTENSION;
	CNtlFileOutputStream fos(fileName.str().c_str());
	tileMesh->saveGround(NTLPE_DATA_FORMAT, false, &fos);
	delete tileMesh;
}
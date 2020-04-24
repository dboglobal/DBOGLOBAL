//**********************************************************************
//
// Copyright (c) 2002-2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/cMesh.h"
#include "AnchorsLoader.h"
#include "ObstructionsLoader.h"
#include "ConnectionsLoader.h"
#include "InformationChannels3DLoader.h"
#include "InformationChannelsOutputStream.h"
#include "Pool.h"
#include "PositionIsValid.h"
#include "SurfaceTypeCosts.h"
#include "RepresentedRegionOverlaps.h"
#include "interface/PathSegmentInterpolator.h"
#include "interface/Singletons.h"
#include "interface/cPathEngine.h"
#include "interface/cShape.h"
#include "interface/cPath.h"
#include "interface/cAgent.h"
#include "interface/cCollisionContext.h"
#include "interface/cObstacleSet.h"
#include "interface/cCollisionInfo.h"
#include "libs/Content_Processing/interface/Export.h"
#include "libs/PathEngine_Core/interface/OffMeshConnections.h"
#include "libs/PathEngine_Core/interface/CollisionQueries_NoPreprocess.h"
#include "libs/PathEngine_Core/interface/CornerProvider.h"
#include "libs/PathEngine_Core/interface/CollisionPreprocess.h"
#include "libs/PathEngine_Core/interface/ClosestUnobstructedPoint.h"
#include "libs/PathEngine_Core/interface/ObstacleInfo.h"
#include "libs/PathEngine_Core/interface/PathfindPreprocess.h"
#include "libs/PathEngine_Core/interface/QueryContext.h"
#include "libs/PathEngine_Core/interface/LocalisedConvexShape.h"
#include "libs/PathEngine_Core/interface/LocalisedConvexShapeSet.h"
#include "libs/PathEngine_Core/interface/CollisionAgainstShapeSet.h"
#include "libs/PathEngine_Core/interface/QueryContext.h"
#include "libs/PathEngine_Core/interface/ContextCollisionPreprocess.h"
#include "libs/PathEngine_Core/interface/CompoundContextCollisionPreprocess.h"
#include "libs/PathEngine_Core/interface/ContextPathfindPreprocess.h"
#include "libs/PathEngine_Core/interface/CombinedCollisionTesters.h"
#include "libs/PathEngine_Core/interface/CornerProvider.h"
#include "libs/PathEngine_Core/interface/Version.h"
#include "libs/PathEngine_Core/interface/ManagedSoftObstacleExpansions.h"
#include "libs/PathEngine_Core/interface/OverlappingPolygon.h" //... used just for generating old style base circuits
#include "libs/MeshPair/interface/SaveMappingTo2D.h"
#include "libs/MeshPair/interface/GenerateMapping.h"
#include "libs/MeshPair/interface/TranslationToOverlapped.h"
#include "libs/Mesh2D/interface/GeneratePartitioned.h"
#include "libs/Mesh2D/interface/MeshCheckSum.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Mesh2D/interface/MeshCut.h"
#include "libs/Mesh2D/interface/SaveCircuits_XML.h"
#include "libs/Mesh2D/interface/TranslateToPartitioned.h"
#include "libs/Mesh2D/interface/ValidateMesh2D.h"
#include "libs/Mesh2D/interface/RandomPosition.h"
#include "libs/Mesh3D/interface/Mesh3DPartitioning.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "libs/Mesh3D/interface/DrawLineOnGround.h"
#include "libs/Mesh3D/interface/SaveMesh3D.h"
#include "libs/Mesh3D/interface/ValidateMesh_Import.h"
#include "libs/Mesh3D/interface/GetClosestPointInFace.h"
#include "libs/Mesh3D/interface/GetPointInsideShapeEntry.h"
#include "libs/Mesh_Common/interface/PositionForPointInFace.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "libs/Mesh_Common/interface/FaceBounds.h"
#include "libs/Mesh_Common/interface/Triangulate.h"
#include "libs/Mesh_Common/interface/Delaunay.h"
#include "libs/Mesh_Common/interface/JoinFacesAtVertex.h"
#include "libs/Mesh_Common/interface/MeshWithTrackedTransform.h"
//#include "libs/Geometry/interface/ApproximatePointsAlongLine.h"
#include "libs/Geometry/interface/AxisFraction.h"
#include "libs/Geometry/interface/InfinitessimallyOffset_PointLike.h"
#include "libs/Geometry/interface/Intersection_PointLike.h"
#include "libs/ParseXML/interface/ParseXML.h"
#include "libs/ParseXML/interface/DocumentHandler.h"
#include "libs/ParseXML/interface/ParallelHandler.h"
#include "libs/ParseXML/interface/TokenisedXMLInputStream.h"
#include "libs/ParseXML/interface/StandardXMLOutputStream.h"
#include "libs/ParseXML/interface/TokenisedXMLOutputStream.h"
#include "libs/Instrumentation/interface/MemoryReport.h"
#include "libs/Instrumentation/interface/Profiler.h"
#include "common/Containers/SwapOut.h"
#include "common/interface/Assert.h"
#include "common/interface/Error.h"
#include "common/interface/iUserData.h"
#include "common/AttributesBuilder.h"
#include "common/CRC32Generator.h"
#include "common/TemplateMesh/External.h"
#include "common/MutexLock.h"
#include "common/interface/Attributes.h"
#include "common/interface/iOutputStream_Operators.h"
#include "platform_common/CoordinateRanges.h"
#include "common/STL_Helpers.h"
#include "common/STL/sstream.h"
#include "iAPIObjectWrapper.h"

class cRandomStream : public iRandomStream
{
public:
    tSigned32 random15()
    {
        return nSingletons::pathEngine().random15();
    }
};

static void
FindAdjacentPositionInFace(tFace f, const tPoint& p, cPosition& result)
{
    int x;
    for(x = 0; x < 3; ++x)
    {
        int y;
        for(y = 0; y < 3; ++y)
        {
            tPoint offset(x - 1, y - 1, 1);
            tPoint candidate = p + offset;
            if(PointIsInFace<tMesh>(f, candidate))
            {
                result.x = candidate.getX();
                result.y = candidate.getY();
                result.cell = f.index();
                return;
            }
        }
    }
    result.cell = -1;
}

static void
saveCollisionShape(const cConvexPoly& shape, iXMLOutputStream& os)
{
    os.openElement("shape");
    os.addAttribute("vertices", shape.sides());
    tSigned32 i;
    for(i = 0; i < shape.sides(); ++i)
    {
        os.openElement("vertex");
        os.addAttribute("x", shape.vertex(i).getX());
        os.addAttribute("y", shape.vertex(i).getY());
        os.closeElement("vertex");
    }
    os.closeElement("shape");
}
static bool
savedCollisionShapeMatches(const cConvexPoly& shape, iXMLInputStream& is)
{
    const char *const* attributes = is.openElement("shape");
    tSigned32 vertices = ValueAsLong(attributes, "vertices");
    if(vertices != shape.sides())
    {
        is.closeElement();
        return false;
    }
    tSigned32 i;
    for(i = 0; i < shape.sides(); ++i)
    {
        attributes = is.openElement("vertex");
        is.closeElement();
        if(ValueAsLong(attributes, "x") != shape.vertex(i).getX()
         || ValueAsLong(attributes, "y") != shape.vertex(i).getY())
        {
            is.closeElement();
            return false;
        }
    }
    is.closeElement();
    return true;
}

void
cMesh::addElementCountRequirement(tSigned32 elementCount)
{
    cMutexLock lock(_queryContextsMutex);
    if(elementCount > _maxElementCountRequirement)
    {
        _maxElementCountRequirement = elementCount;
    }
}

void
cMesh::makeCheckSumValid() const
{
    assertD(!_preprocessGenerationCompleted); // checksum includes base circuits
    if(_checkSumValid)
    {
        return;
    }
    _checkSum = 0;
    cCRC32Generator generator;
    GenerateMeshCheckSum(ref2D(), generator);
//    GenerateCircuitsCheckSum(*_baseCircuits, generator);
    tSigned32 i;
    for(i = 0; i < SizeL(_baseObstacles); ++i)
    {
        _baseObstacles[i].checksum(generator, _baseObstacleShapes[i]);
    }
    if(!_baseSurfaceTypeCosts.empty())
    {
        assertD(_surfaceTypes);
        generator << static_cast<tSigned32>(_surfaceTypes->size());
        for(i = 0; i < SizeL(*_surfaceTypes); ++i)
        {
            generator << (*_surfaceTypes)[i];
        }
    }
    for(i = 0; i < SizeL(_baseSurfaceTypeCosts); ++i)
    {
        _baseSurfaceTypeCosts[i]->checksum(generator);
    }
    _checkSum = generator.getResult();
    _checkSumValid = true;
}

void
cMesh::buildAnythingRequiredForPathfindPreprocess()
{
    if(_pathfindPreprocessHasBeenGenerated)
    {
        return;
    }
    _pathfindPreprocessHasBeenGenerated = true;
    _mesh3DCorners = new cCornerProvider(&ref2D());
    tSigned32 j;
    for(j = 0; j < SizeL(_baseSurfaceTypeCosts); ++j)
    {
        assertD(!_baseSurfaceTypeCosts[j]->empty());
        _mesh3DCostCalculators.push_back(0);
        _baseSurfaceTypeCosts[j]->buildCornersAndCostCalculator(*_mesh3D, *_translation, _mesh3DCostCalculators.back(), *_mesh3DCorners);
    }
    _mesh3DCorners->sort();
}

void
cMesh::releasePreprocessFor(tSigned32 i)
{
    assertD(i != -1);
    cMutexLock lock(_meshMutex);
    _collisionPreprocess.set(i, 0);
    _pathfindPreprocess.set(i, 0);
}


static void
SaveObstacles(
        const std::vector<cObstacleInfo>& infos,
        const std::vector<cConvexPoly>& collisionShapes,
        const std::vector<std::string>* ids,
        iXMLOutputStream& os
        )
{
    tSigned32 i;
    for(i = 0; i < SizeL(infos); ++i)
    {
        os.openElement("obstacle");
        if(ids)
        {
            os.addAttribute("id", (*ids)[i].c_str());
        }
        const cObstacleInfo& info = infos[i];
        {
            std::ostringstream oss;
            oss << info._position.f.index() << ":" << info._position.p.getX() << "," << info._position.p.getY();
            os.addAttribute("position", oss.str().c_str());
        }
        {
            {
                std::ostringstream oss;
//                const cConvexPoly& cs = *info._shape;
                const cConvexPoly& cs = collisionShapes[i];
                oss << cs.vertex(0).getX() << "," << cs.vertex(0).getY();
                tSigned32 j;
                for(j = 1; j < cs.sides(); j++)
                {
                    oss << "," << cs.vertex(j).getX() << "," << cs.vertex(j).getY();
                }
                os.addAttribute("vertices", oss.str().c_str());
            }
            if(info._costToTraverse != -1.f)
            {
                std::string persistent = FloatToPersistent(info._costToTraverse);
                os.addAttribute("costToTraverse", persistent.c_str());
            }
            if(info._directionForCostToTraverse.getX() != 0)
            {
                os.addAttribute("costDirectionX", info._directionForCostToTraverse.getX());
            }
            if(info._directionForCostToTraverse.getY() != 0)
            {
                os.addAttribute("costDirectionY", info._directionForCostToTraverse.getY());
            }
        }
        os.closeElement("obstacle");
    }
}

void
cMesh::doSaveGround(iXMLOutputStream& baseOS, bool include_mapping) const
{
    cInformationChannelsOutputStream os(baseOS, _surfaceTypes, _sectionIDs, _userData);
    os.openElement("mesh");
    os.addAttribute("majorRelease", MajorRelease());
    os.addAttribute("minorRelease", MinorRelease());

    os.openElement("mesh3D");
    SaveMesh3D(*_mesh3D, os);
    os.closeElement("mesh3D");

    if(include_mapping)
    {
        SaveMappingTo2D(_mesh3D->vertices(), ref2D(), os);
    }

    if(_representedRegionOverlaps)
    {
        os.openElement("representedRegionOverlaps");
        _representedRegionOverlaps->save(os);
        os.closeElement("representedRegionOverlaps");
    }

    if(!_offMeshConnections->empty())
    {
        os.openElement("offMeshConnections");
        _offMeshConnections->save(os);
        os.closeElement("offMeshConnections");
    }

    if(!_baseObstacles.empty())
    {
        os.openElement("baseObstacles");
        SaveObstacles(_baseObstacles, _baseObstacleShapes, 0, os);
        os.closeElement("baseObstacles");
    }
    if(!_baseSurfaceTypeCosts.empty())
    {
        os.openElement("baseSurfaceTypeCosts");
        tSigned32 i;
        for(i = 0; i < SizeL(_baseSurfaceTypeCosts); ++i)
        {
            os.openElement("costs");
            _baseSurfaceTypeCosts[i]->save(os);
            os.closeElement("costs");
        }
        os.closeElement("baseSurfaceTypeCosts");
    }
//    if(!_baseCircuits->empty())
//    {
//        SaveCircuits_XML(*_baseCircuits, os, false);
//    }
    if(!_anchorIDs.empty())
    {
        os.openElement("anchors");
        size_t i;
        for(i = 0; i < _anchorIDs.size(); i++)
        {
            os.openElement("anchor");
            os.addAttribute("id", _anchorIDs[i].c_str());
            std::ostringstream oss;
            oss << _anchorPositions[i].cell << ":" << _anchorPositions[i].x << "," << _anchorPositions[i].y;
            os.addAttribute("position", oss.str().c_str());
            if(_anchorOrientations[i])
            {
                os.addAttribute("orientation", _anchorOrientations[i]);
            }
            os.closeElement("anchor");
        }
        os.closeElement("anchors");
    }
    if(!_namedObstacleIDs.empty())
    {
        os.openElement("namedObstacles");
        SaveObstacles(_namedObstacles, _namedObstacleShapes, &_namedObstacleIDs, os);
        os.closeElement("namedObstacles");
    }
    os.closeElement("mesh");
}

void
cMesh::doSavePathfindPreprocess(iXMLOutputStream& os, cShape* shape) const
{
    tSigned32 i = shape->getPreprocessIndex();
    os.openElement("pathfindPreprocess");
    os.addAttribute("majorVersion", cPathfindPreprocess::majorVersion());
    os.addAttribute("minorVersion", cPathfindPreprocess::minorVersion());
    os.addAttribute("meshCheckSum", _checkSum);
    saveCollisionShape(shape->refCS(), os);
    os.openElement("attributes");
    os.addAttributes(_pathfindPreprocess.get(i)->getAttributes());
    os.closeElement("attributes");
    os.openElement("preprocess");
    _pathfindPreprocess.get(i)->save(os);
    os.closeElement("preprocess");
    os.closeElement("pathfindPreprocess");
}

static void
DoSaveCollisionPreprocess(cCollisionPreprocess& cp, tSigned32 meshCheckSum, iShape* shape, iXMLOutputStream& os)
{
    cShape* cs = static_cast<cShape*>(shape);
    tSigned32 i = cs->getPreprocessIndex();

    os.openElement("collisionPreprocess");
    os.addAttribute("majorVersion", cCollisionPreprocess::majorVersion());
    os.addAttribute("minorVersion", cCollisionPreprocess::minorVersion());
    os.addAttribute("meshCheckSum", meshCheckSum);

    saveCollisionShape(cs->refCS(), os);

    os.openElement("attributes");
    os.addAttributes(cp.getAttributes());
    os.closeElement("attributes");

    cp.save(os);

    os.closeElement("collisionPreprocess");
}

void
cMesh::swapInOrBuild2DMeshAndIndex(tMesh& mesh, const char *const* options)
{
    PROFILER_SCOPE("cMesh::swapInOrBuild2DMeshAndIndex");
    bool useIdentityMapping = ValueAsBoolWithDefault(options, "useIdentityMapping", false);
    if(!mesh.empty())
    {
        if(useIdentityMapping)
        {
            Error("Warning", "The 'useIdentityMapping' attribute has been set for mesh loading, but a mapping is already present. "
                "The attribute will be ignored.");
        }
        _mesh2D = new tMesh;
        _mesh2D->swap(mesh);
    }
    else
    {
        _mesh2D = GenerateIdentityMapping(*_mesh3D);
        if(!useIdentityMapping)
        {
            OptimiseMapping(*_mesh2D);
        }
    }

    PROFILER_CHANGES("cMesh::swapInOrBuild2DMeshAndIndex end section");

  // this ensures that external faces and edges occur in exactly the same order as when the mesh is subsequently saved and reloaded
    RemoveExternalFaces(*_mesh2D);
    AddExternalFaces(*_mesh2D);

    _mesh2D->consolidate();
    ValidateMesh2D(*_mesh2D);
    _mesh2DInternalFaces = CountInternalFaces(*_mesh2D);
}

void
cMesh::constructorCommon(const char *const* options)
{
    _testBedUserData = 0;
    _mesh3DCorners = 0;
    _pathfindPreprocessHasBeenGenerated = false;
    _wrapper = 0;
    _unexpandedBaseObstacles = 0;
    _allowCollisionWithoutPreprocess = ValueAsBoolWithDefault(options, "allowCollisionWithoutPreprocess", false);

    _hasInternalVertices = false;
    {
        tVertex v = _mesh2D->beginVertex();
        do
        {
            if(!VertexIsExternal(tMesh::firstEdge(v)))
            {
                _hasInternalVertices = true;
                break;
            }
            if(ExternalVertexIsInLine(ExternalEdgeForVertex(tMesh::firstEdge(v))))
            {
                _hasInternalVertices = true; //....... rename?
                break;
            }
        }
        while((++v).valid());
    }

    _mesh2D_NoInternalVertices = 0;
    if(_hasInternalVertices)
    {
        _mesh2D_NoInternalVertices = new cMeshWithTrackedTransform<tMesh>(_mesh2D);

        tVertex v = _mesh2D_NoInternalVertices->beginVertex();
        do
        {
            tVertex candidateV = v;
            ++v;
            if(VertexIsExternal(tMesh::firstEdge(candidateV)))
            {
                continue;
            }
            JoinFacesAtVertex_KeepConvex(*_mesh2D_NoInternalVertices, candidateV);
        }
        while(v.valid());

        RemoveInlineEdgeVertices(*_mesh2D_NoInternalVertices, cNullVertexFilter<tVertex>());
    }

    _topSectionID = -1;
    if(_sectionIDs)
    {
        tSigned32 i;
        for(i = 0; i < SizeL(*_sectionIDs); ++i)
        {
            if((*_sectionIDs)[i] > _topSectionID)
            {
                _topSectionID = (*_sectionIDs)[i];
            }
        }

//        for(i = 0; i < SizeL(*_sectionIDs); ++i)
//        {
//            tSigned32 sectionID = (*_sectionIDs)[i];
//            if(sectionID == -1)
//            {
//                continue;
//            }
//            tFace_3D f3D = _mesh3D->faceAt(i);
//            tPoint bottomLeft, topRight;
//
//            tSigned32 encounteredIndex;
//            for(encounteredIndex = 0; encounteredIndex != SizeL(_sectionsEncountered); ++encounteredIndex)
//            {
//                if(_sectionsEncountered[encounteredIndex] == sectionID)
//                {
//                    break;
//                }
//            }
//
//            GetFaceBounds<tMesh_3D>(f3D, bottomLeft, topRight);
//
//            if(encounteredIndex == SizeL(_sectionsEncountered))
//            {
//                _sectionsEncountered.push_back(sectionID);
//                _sectionBounds.push_back(bottomLeft);
//                _sectionBounds.push_back(topRight);
//            }
//            else
//            {
//                _sectionBounds[encounteredIndex * 2 + 0].updateAsMinimumBound(bottomLeft);
//                _sectionBounds[encounteredIndex * 2 + 1].updateAsMaximumBound(topRight);
//            }
//        }
    }

    _queryContexts.push_back(new cQueryContext(ref2D(), _mesh2DInternalFaces, _hasInternalVertices, nSingletons::pathEngine().refHeapPool()));
    _maxElementCountRequirement = 0;
    _checkSumValid = false;
    _preprocessGenerated = false;
    _preprocessGenerationCompleted = false;
    _partitioning = new cMesh3DPartitioning(ref3D());
    _partitioningResultsBuffer.resize(1 + _partitioning->maximumResultsSize());
    incRef();

    _topSurfaceType = -1;
    if(_surfaceTypes)
    {
        tSigned32 i;
        for(i = 0; i != SizeL(*_surfaceTypes); ++i)
        {
            if((*_surfaceTypes)[i] > _topSurfaceType)
            {
                _topSurfaceType = (*_surfaceTypes)[i];
            }
        }
    }
}

void
cMesh::shrinkStuffToFit()
{
    _mesh3D->shrinkToFit();
    _mesh2D->shrinkToFit();
    ShrinkToFit(_baseObstacleShapes);
    ShrinkToFit(_baseObstacles);
    ShrinkToFit(_anchorIDs);
    ShrinkToFit(_anchorPositions);
    ShrinkToFit(_anchorOrientations);
    ShrinkToFit(_namedObstacleIDs);
    ShrinkToFit(_namedObstacleShapes);
    ShrinkToFit(_namedObstacles);
    ShrinkToFit(_sectionBounds);
}

static void
CheckLoadedPositions(std::vector<cPosition>& loadedPositions, tMesh& mesh, const char* message)
{
    tSigned32 i;
    for(i = 0; i < SizeL(loadedPositions); ++i)
    {
        if(!PositionIsValid(mesh, loadedPositions[i]))
        {
            Error("NonFatal", message);
            loadedPositions[i].cell = -1;
        }
    }
}
static void
CheckLoadedAnchors(std::vector<cAnchorInfo>& infos, tMesh& mesh)
{
    tSigned32 i;
    for(i = 0; i < SizeL(infos); ++i)
    {
        if(!PositionIsValid(mesh, infos[i]._position))
        {
            Error("NonFatal", "The position for an anchor being loaded is not valid. The anchor will be skipped.");
            infos[i]._position.cell = -1;
        }
    }
}
static void
RemapLoadedPositions(std::vector<cPosition>& loadedPositions, tMesh& mesh, const cTranslationToOverlapped& oldTranslation, const cTranslationToOverlapped& newTranslation)
{
    tSigned32 i;
    for(i = 0; i != SizeL(loadedPositions); ++i)
    {
        cPosition& pos = loadedPositions[i];
        if(pos.cell == -1)
        {
            continue;
        }
        tPoint p = tPoint(pos.x, pos.y, WORLD_RANGE);
        tFace f = mesh.faceAt(pos.cell);
        tFace_3D f3D = oldTranslation.translate(f, p);
        f = newTranslation.translate(f3D, p);
        pos.cell = f.index();
    }
}
static void
RemapLoadedAnchors(std::vector<cAnchorInfo>& infos, tMesh& mesh, const cTranslationToOverlapped& oldTranslation, const cTranslationToOverlapped& newTranslation)
{
    tSigned32 i;
    for(i = 0; i != SizeL(infos); ++i)
    {
        cPosition& pos = infos[i]._position;
        if(pos.cell == -1)
        {
            continue;
        }
        tPoint p = tPoint(pos.x, pos.y, WORLD_RANGE);
        tFace f = mesh.faceAt(pos.cell);
        tFace_3D f3D = oldTranslation.translate(f, p);
        f = newTranslation.translate(f3D, p);
        pos.cell = f.index();
    }
}

cMesh::cMesh(
          tMesh_3D& baseMesh, tMesh& mappingAsMesh2D,
          const char* format, const char* dataBuffer, tUnsigned32 bufferSize,
          const char *const* options,
          cList_NoSize<cMesh*>::iterator heldNode
          )
{
    PROFILER_SCOPE("cMesh::cMesh (with buffer) top");
    assertD(!baseMesh.hasDeadEntries());
    _mesh3DInternalFaces = CountInternalFaces(baseMesh);
    _mesh3D = new tMesh_3D;
    _mesh3D->swap(baseMesh);

    _offMeshConnections = new cOffMeshConnections;

    _heldNode = heldNode;

    cInformationChannels3DLoader informationChannelsLoader(
            _mesh3DInternalFaces,
            _surfaceTypes,
            _sectionIDs,
            _userData
            );

    PROFILER_CHANGES("cMesh::cMesh (with buffer) swap in or build mesh 2d");
    swapInOrBuild2DMeshAndIndex(mappingAsMesh2D, options);

    PROFILER_CHANGES("cMesh::cMesh (with buffer) loading");

    cConnectionsLoader connectionsLoader(*_mesh2D, *_offMeshConnections);
    cNestedHandler nestedHandler5(connectionsLoader, "offMeshConnections");

    cAnchorsLoader anchorsLoader;
    cNestedHandler nestedHandler(anchorsLoader, "anchors");

    cObstructionsLoader baseObstaclesLoader;
    cNestedHandler nestedHandler2(baseObstaclesLoader, "baseObstacles");

    cSurfaceTypeCostsVectorLoader baseSurfaceTypeCostsLoader(_surfaceTypes, _baseSurfaceTypeCosts);
    cNestedHandler nestedHandler4(baseSurfaceTypeCostsLoader, "baseSurfaceTypeCosts");

    cObstructionsLoader namedObstaclesLoader;
    cNestedHandler nestedHandler3(namedObstaclesLoader, "namedObstacles");

    cRepresentedRegionOverlapsLoader representedRegionOverlapsLoader;
    cNestedHandler nestedHandler6(representedRegionOverlapsLoader, "representedRegionOverlaps");

    cParallelHandler parallelHandler;
    parallelHandler.addHandler(&nestedHandler);
    parallelHandler.addHandler(&nestedHandler2);
    parallelHandler.addHandler(&nestedHandler3);
    parallelHandler.addHandler(&nestedHandler4);
    parallelHandler.addHandler(&nestedHandler5);
    parallelHandler.addHandler(&nestedHandler6);
    parallelHandler.addHandler(&informationChannelsLoader);

    cDocumentHandler documentHandler(parallelHandler, "mesh");
    ParseXML(format, dataBuffer, bufferSize, documentHandler);

    _representedRegionOverlaps = representedRegionOverlapsLoader.takeResult();

    PROFILER_CHANGES("cMesh::cMesh (with buffer) translation to overlapped");
    {
        bool useBSPs = ValueAsBoolWithDefault(options, "partitionTranslationTo3D", false);
        _translation = new cTranslationToOverlapped(*_mesh3D, *_mesh2D, useBSPs);
    }

    PROFILER_CHANGES("cMesh::cMesh (with buffer) check loaded positions and anchors");

    CheckLoadedPositions(baseObstaclesLoader._loadedPositions, *_mesh2D, "The position for a base obstacle being loaded is not valid. The obstacle will be skipped.");
    CheckLoadedPositions(namedObstaclesLoader._loadedPositions, *_mesh2D, "The position for a named obstacle being loaded is not valid. The obstacle will be skipped.");
    CheckLoadedAnchors(anchorsLoader._infos, *_mesh2D);

    if(ValueAsBoolWithDefault(options, "partitionByObstacleCentres", false))
    {
        PROFILER_CHANGES("cMesh::cMesh (with buffer) partition by obstacle centres");
        std::vector<cPosition>& loadedPositions = baseObstaclesLoader._loadedPositions;
        std::vector<cInternalPosition> positions(loadedPositions.size());
        positions.clear();
        std::vector<tMesh::tVertexData> vertexData(loadedPositions.size());
        vertexData.clear();

        tSigned32 i;
        for(i = 0; i != SizeL(loadedPositions); ++i)
        {
            cPosition pos = loadedPositions[i];
            if(pos.cell == -1)
            {
                continue;
            }
            tFace f2D = _mesh2D->faceAt(pos.cell);
            tPoint p = tPoint(pos.x, pos.y, WORLD_RANGE);
            positions.push_back(cInternalPosition(f2D, p));
            tFace_3D f3D = _translation->translate(f2D, p);
            cMeshElement<tMesh_3D> el3D = ElementForPointInOverlappingFace<tMesh_3D>(f3D, p);
            vertexData.push_back(tMesh::tVertexData(p));
            vertexData.back().setElementOn3DIndex(IndexForElement(el3D));
        }


        tMesh* partitionedMesh;
        {
            cMeshWithTrackedTransform<tMesh>* partitioned = GeneratePartitioned(*_mesh2D, positions, vertexData);
            partitionedMesh = partitioned->extractMesh();
            delete partitioned;
        }
        {
            Triangulate(*partitionedMesh);
            cDelaunayFilter_NoEdgesOnExternalFaces<tMesh> filter;
            Delaunay(*partitionedMesh, filter);
            SimplifyMesh(*partitionedMesh, filter);    
        }
        
        // the following added after switch to cVectorMesh,
        // because the above manipulations can now put internal faces after external faces
        RemoveExternalFaces(*partitionedMesh);
        AddExternalFaces(*partitionedMesh);

        partitionedMesh->consolidate();
        ValidateMesh2D(*partitionedMesh);

        cTranslationToOverlapped* partitionedTranslation = new cTranslationToOverlapped(*_mesh3D, *partitionedMesh, false);

        RemapLoadedPositions(loadedPositions, *_mesh2D, *_translation, *partitionedTranslation);
        RemapLoadedPositions(namedObstaclesLoader._loadedPositions, *_mesh2D, *_translation, *partitionedTranslation);
        RemapLoadedAnchors(anchorsLoader._infos, *_mesh2D, *_translation, *partitionedTranslation);

        delete _mesh2D;
        delete _translation;
        _mesh2D = partitionedMesh;
        _translation = partitionedTranslation;
    }

    PROFILER_CHANGES("cMesh::cMesh (with buffer) constructor common");

    constructorCommon(options);

    PROFILER_CHANGES("cMesh::cMesh (with buffer) burn in base obstacles");
    {
        const std::vector<cObstacleInfo>& infos = baseObstaclesLoader._infos;
        const std::vector<cConvexPoly>& collisionShapes = baseObstaclesLoader._shapes;
        const std::vector<cPosition>& loadedPositions = baseObstaclesLoader._loadedPositions;
        iCollisionContext* baseObstaclesContext = newContext();
        tSigned32 i;
        for(i = 0; i < SizeL(infos); ++i)
        {
            if(loadedPositions[i].cell == -1)
            {
                continue;
            }
//            iShape* shape = nSingletons::pathEngine().newShape_Internal(*infos[i]._shape);
            iShape* shape = nSingletons::pathEngine().newShape_Internal(collisionShapes[i]);
            iAgent* agent = placeAgent(shape, loadedPositions[i]);
            agent->setTraverseCost(infos[i]._costToTraverse);
            tPoint d = infos[i]._directionForCostToTraverse;
            agent->setTraverseCostDirection(d.getX(), d.getY());
            baseObstaclesContext->addAgent(agent);
            delete shape;
        }
        burnContextIntoMesh(baseObstaclesContext);
        while(baseObstaclesContext->getNumberOfAgents())
        {
            delete baseObstaclesContext->getAgent(0);
        }
        delete baseObstaclesContext;
    }
    PROFILER_CHANGES("cMesh::cMesh (with buffer) store anchors and shapes");
    {
        const std::vector<std::string>& ids = namedObstaclesLoader._ids;
        const std::vector<cObstacleInfo>& infos = namedObstaclesLoader._infos;
        const std::vector<cConvexPoly>& collisionShapes = namedObstaclesLoader._shapes;
        const std::vector<cPosition>& loadedPositions = namedObstaclesLoader._loadedPositions;
        tSigned32 i;
        for(i = 0; i < SizeL(ids); ++i)
        {
            if(loadedPositions[i].cell == -1)
            {
                continue;
            }
//            iShape* shape = nSingletons::pathEngine().newShape_Internal(*infos[i]._shape);
            iShape* shape = nSingletons::pathEngine().newShape_Internal(collisionShapes[i]);
            iAgent* agent = placeAgent(shape, loadedPositions[i]);
            agent->setTraverseCost(infos[i]._costToTraverse);
            tPoint d = infos[i]._directionForCostToTraverse;
            agent->setTraverseCostDirection(d.getX(), d.getY());
            storeNamedObstacle(ids[i].c_str(), agent);
            delete agent;
            delete shape;
        }
    }
    {
        const std::vector<cAnchorInfo>& infos = anchorsLoader._infos;
        tSigned32 i;
        for(i = 0; i < SizeL(infos); ++i)
        {
            if(infos[i]._position.cell == -1)
            {
                continue;
            }
            storeAnchor(infos[i]._id.c_str(), infos[i]._position, infos[i]._orientation);
        }
    }

    PROFILER_CHANGES("cMesh::cMesh (with buffer) shrink to fit");
    shrinkStuffToFit();
}

cMesh::cMesh(
        tMesh_3D& baseMesh,
        std::vector<tSigned32>* surfaceTypes,
        std::vector<tSigned32>* sectionIDs,
        std::vector<tSigned32>* userData,
        const char *const* options,
        cList_NoSize<cMesh*>::iterator heldNode
        )
{
    assertD(!baseMesh.hasDeadEntries());
    _mesh3DInternalFaces = CountInternalFaces(baseMesh);
    _mesh3D = new tMesh_3D;
    _mesh3D->swap(baseMesh);

    _offMeshConnections = new cOffMeshConnections;
    _heldNode = heldNode;
    {
        tMesh nullMapping;
        swapInOrBuild2DMeshAndIndex(nullMapping, options);
        {
            bool useBSPs = ValueAsBoolWithDefault(options, "partitionTranslationTo3D", false);
            _translation = new cTranslationToOverlapped(*_mesh3D, *_mesh2D, useBSPs);
        }
    }

    _surfaceTypes = surfaceTypes;
    _sectionIDs = sectionIDs;
    _userData = userData;

    _representedRegionOverlaps = 0;

    constructorCommon(options);
    shrinkStuffToFit();
}

cMesh::~cMesh()
{
    assertD(!_heldNode.valid());
    {
        MEMORY_REPORT_SCOPE("cMesh - testBed user data");
        delete _testBedUserData;
    }
    {
        MEMORY_REPORT_SCOPE("cMesh - RepresentedRegionOverlaps");
        delete _representedRegionOverlaps;
    }
    {
        MEMORY_REPORT_SCOPE("cMesh - Mesh 3D partitioning");
        delete _partitioning;
    }
    tSigned32 i;
    {
        MEMORY_REPORT_SCOPE("cMesh - releasing preprocess");
        for(i = 0; i < nSingletons::pathEngine().numberOfPreprocessShapes(); ++i)
        {
            releasePreprocessFor(i);
        }
    }
    {
        MEMORY_REPORT_SCOPE("cMesh - held contexts");
        for(i = 0; i != _heldContexts.size(); ++i)
        {
            if(_heldContexts.indexIsInUse(i))
            {
                _heldContexts[i]->destroy();
            }
        }
    }
    {
        MEMORY_REPORT_SCOPE("cMesh - held obstacle sets");
        for(i = 0; i != _heldObstacleSets.size(); ++i)
        {
            if(_heldObstacleSets.indexIsInUse(i))
            {
                _heldObstacleSets[i]->destroy();
            }
        }
    }
    {
        MEMORY_REPORT_SCOPE("cMesh - held agents");
        for(i = 0; i != _heldAgents.size(); ++i)
        {
            if(_heldAgents.indexIsInUse(i))
            {
                _heldAgents[i]->destroy();
            }
        }
    }
    {
        MEMORY_REPORT_SCOPE("cMesh - held paths");
        while(!_heldPaths.empty())
        {
            _heldPaths.front()->destroy();
        }
    }
    {
        MEMORY_REPORT_SCOPE("cMesh - held collision infos");
        while(!_heldCollisionInfos.empty())
        {
            _heldCollisionInfos.front()->destroy();
        }
    }

    {
        MEMORY_REPORT_SCOPE("cMesh - mesh 2d no internal vertices");
        delete _mesh2D_NoInternalVertices;
    }
    {
        MEMORY_REPORT_SCOPE("cMesh - mesh 3D");
        delete _mesh3D;
    }
    {
        MEMORY_REPORT_SCOPE("cMesh - mesh 2D");
        delete _mesh2D;
    }
    {
        MEMORY_REPORT_SCOPE("cMesh - Translation to overlapped");
        delete _translation;
    }

    {
        MEMORY_REPORT_SCOPE("cMesh - query contexts");
        for(i = 0; i < SizeL(_queryContexts); ++i)
        {
            delete _queryContexts[i];
        }
    }
    {
        MEMORY_REPORT_SCOPE("cMesh - 3D face info channels");
        delete _surfaceTypes;
        delete _sectionIDs;
        delete _userData;
    }

    {
        MEMORY_REPORT_SCOPE("cMesh - base surface type costs");
        for(i = 0; i != SizeL(_baseSurfaceTypeCosts); ++i)
        {
            delete _baseSurfaceTypeCosts[i];
        }
    }

    {
        MEMORY_REPORT_SCOPE("cMesh - unexpanded base obstacles");
        if(_unexpandedBaseObstacles)
        {
            _unexpandedBaseObstacles->deleteAllAndClear();
            delete _unexpandedBaseObstacles;
        }
    }

    {
        MEMORY_REPORT_SCOPE("cMesh - off mesh connections");
        delete _offMeshConnections;
    }
    {
        MEMORY_REPORT_SCOPE("cMesh - mesh 3d corners");
        delete _mesh3DCorners;
    }


#ifdef MEMORY_REPORTS_ON
    {
        MEMORY_REPORT_SCOPE("cMesh - base obstacle infos");
        CompletelyClear(_baseObstacles);
    }
    {
        MEMORY_REPORT_SCOPE("cMesh - base obstacle shapes");
        CompletelyClear(_baseObstacleShapes);
    }
    {
        MEMORY_REPORT_SCOPE("cMesh - anchor info");
        CompletelyClear(_anchorIDs);
        CompletelyClear(_anchorPositions);
        CompletelyClear(_anchorOrientations);
    }
    {
        MEMORY_REPORT_SCOPE("cMesh - named obstacle info");
        CompletelyClear(_namedObstacleIDs);
        CompletelyClear(_namedObstacleShapes);
        CompletelyClear(_namedObstacles);
    }
    {
        MEMORY_REPORT_SCOPE("cMesh - partitioning results buffer");
        CompletelyClear(_partitioningResultsBuffer);
    }
    {
        MEMORY_REPORT_SCOPE("cMesh - section bounds");
        CompletelyClear(_sectionBounds);
    }
    {
        MEMORY_REPORT_SCOPE("cMesh - misc vectors");
        CompletelyClear(_mesh3DCostCalculators);
        CompletelyClear(_queryContexts);
        CompletelyClear(_terrainCallBacks);
        CompletelyClear(_baseSurfaceTypeCosts);
        _collisionPreprocess.completelyClear();
        _pathfindPreprocess.completelyClear();
        _heldContexts.completelyClear();
        _heldObstacleSets.completelyClear();
        _heldAgents.completelyClear();
    }
#endif
}

// iMesh

void cMesh::destroy()
{
    if(_wrapper) {_wrapper->destroy_APIObjectWrapper();}
    INTERFACE_ASSERT(_heldNode.valid());
    nSingletons::pathEngine().lockHeldMeshes();
    _heldNode.eraseAndSetInvalid();
    nSingletons::pathEngine().unlockHeldMeshes();
    decRef();
}

tSigned32
cMesh::getNumberOf3DFaces() const
{
    cMutexLock lock(_meshMutex);
    return _mesh3DInternalFaces;
}
tSigned32
cMesh::get3DFaceAtPosition(const cPosition& position) const
{
    cMutexLock lock(_meshMutex);
	INTERFACE_CHECK(positionIsValid(position), "iMesh::get3DFaceAtPosition() called with invalid position");
    cInternalPosition p;
    convertPosition(position, p);
    tFace_3D f = _translation->translate(p.f, p.p);
    return f.index();
}
void
cMesh::get3DFaceVertex(tSigned32 faceIndex, tSigned32 vertexIndexInFace, tSigned32& x, tSigned32& y, float& z) const
{
    cMutexLock lock(_meshMutex);
    INTERFACE_ASSERT(faceIndex >= 0 && faceIndex < getNumberOf3DFaces());
    INTERFACE_ASSERT(vertexIndexInFace >= 0 && vertexIndexInFace < 3);
    tFace_3D f = _mesh3D->faceAt(faceIndex);
    tEdge_3D e = tMesh_3D::beginEdge(f);
    tSigned32 i;
    for(i = 0; i < vertexIndexInFace; ++i)
    {
        ++e;
    }
    x = GetVertexPoint(e).getX();
    y = GetVertexPoint(e).getY();
    z = e->getZ();
}
tSigned32
cMesh::get3DFaceConnection(tSigned32 faceIndex, tSigned32 edgeIndexInFace) const
{
    cMutexLock lock(_meshMutex);
    INTERFACE_ASSERT(faceIndex >= 0 && faceIndex < getNumberOf3DFaces());
    INTERFACE_ASSERT(edgeIndexInFace >= 0 && edgeIndexInFace < 3);
    tFace_3D f = _mesh3D->faceAt(faceIndex);
    tEdge_3D e = tMesh_3D::beginEdge(f);
    tSigned32 i;
    for(i = 0; i < edgeIndexInFace; ++i)
    {
        ++e;
    }
    if(e.twin().face()->isExternal())
    {
        return -1;
    }
    return e.twin().face().index();
}

void
cMesh::get3DFaceNormal(tSigned32 faceIndex, float& resultX, float& resultY, float& resultZ) const
{
    cMutexLock lock(_meshMutex);
    tFace_3D f = _mesh3D->faceAt(faceIndex);
    f->getNormal(resultX, resultY, resultZ);
}

tSigned32
cMesh::get3DFaceAttribute(tSigned32 faceIndex, tSigned32 attributeIndex) const
{
    cMutexLock lock(_meshMutex);
    switch(attributeIndex)
    {
    default:
        Error("Warning", "cMesh::get3DFaceAttribute() - unknown attribute index.");
        return -1;
    case PE_FaceAttribute_SurfaceType:
        if(_surfaceTypes == 0)
        {
            return 0;
        }
        return (*_surfaceTypes)[faceIndex];
    case PE_FaceAttribute_SectionID:
        if(_sectionIDs == 0)
        {
            return 01;
        }
        return (*_sectionIDs)[faceIndex];
    case PE_FaceAttribute_MarksPortal:
    case PE_FaceAttribute_MarksTerrainPortal:
    case PE_FaceAttribute_MarksExternalShape:
    case PE_FaceAttribute_MarksConnectingEdge:
        Error("Warning", "cMesh::get3DFaceAttribute() - attribute used only during content processing.");
        return -1;
    case PE_FaceAttribute_UserData:
        if(_userData == 0)
        {
            return 0;
        }
        return (*_userData)[faceIndex];
    }
}

tSigned32
cMesh::getNumberOfSections() const
{
//    return SizeL(_terrainCallBacks);
    return _topSectionID;
}

tSigned32
cMesh::getSurfaceType(const cPosition& position) const
{
    cMutexLock lock(_meshMutex);
	INTERFACE_CHECK(positionIsValid(position), "iMesh::getSurfaceType() called with invalid position");
    if(_surfaceTypes == 0)
    {
        return 0;
    }
    tSigned32 f3DIndex = get3DFaceAtPosition(position);
    return (*_surfaceTypes)[f3DIndex];
}
tSigned32
cMesh::getSectionID(const cPosition& position) const
{
    cMutexLock lock(_meshMutex);
	INTERFACE_CHECK(positionIsValid(position), "iMesh::getSectionID() called with invalid position");
    if(_sectionIDs == 0)
    {
        return -1;
    }
    tSigned32 f3DIndex = get3DFaceAtPosition(position);
    return (*_sectionIDs)[f3DIndex];
}

void
cMesh::storeAnchor(const char* id, const cPosition& position, tSigned32 orientation)
{
    cMutexLock lock(_meshMutex);
	INTERFACE_CHECK(positionIsValid(position), "iMesh::storeAnchor() called with invalid position");
    INTERFACE_ASSERT(id);
    {
        cPosition retrieved;
        tSigned32 orientation_Ignored;
        retrieveAnchor(id, orientation_Ignored);
        if(retrieved.cell != -1)
        {
            Error("NonFatal", "iMesh::storeAnchor() - id already used", 0);
            return;
        }
    }
    _anchorIDs.push_back(id);
    _anchorPositions.push_back(position);
    _anchorOrientations.push_back(orientation);
}
cPosition
cMesh::retrieveAnchor(const char* id, tSigned32& orientation) const
{
    cMutexLock lock(_meshMutex);
    INTERFACE_ASSERT(id);
    size_t i;
    for(i = 0; i < _anchorIDs.size(); i++)
    {
        if(_anchorIDs[i] == id)
        {
            orientation = _anchorOrientations[i];
            return _anchorPositions[i];
        }
    }
    return cPosition();
}

tSigned32
cMesh::getNumberOfAnchors() const
{
    cMutexLock lock(_meshMutex);
    return static_cast<tSigned32>(_anchorIDs.size());
}
void
cMesh::retrieveAnchorByIndex(tSigned32 index, cPosition& position, tSigned32& orientation, const char*& id) const
{
    cMutexLock lock(_meshMutex);
    INTERFACE_ASSERT(index >= 0 && index < getNumberOfAnchors());
    position = _anchorPositions[index];
    orientation = _anchorOrientations[index];
    id = _anchorIDs[index].c_str();
}

void
cMesh::storeNamedObstacle(const char* id, iAgent* agent)
{
    cMutexLock lock(_meshMutex);
	INTERFACE_ASSERT(agent);
    INTERFACE_ASSERT(id);
    size_t i;
    for(i = 0; i < _namedObstacleIDs.size(); i++)
    {
        if(_namedObstacleIDs[i] == id)
        {
            break;
        }
    }
    if(i != _namedObstacleIDs.size())
    {
        Error("NonFatal", "iMesh::storeObstruction() - id already used", 0);
        return;
    }

    cAgent* agentC = FromI(agent);

    tSigned32 x, y;
    agent->getTraverseCostDirection(x, y);

    cObstacleInfo info(agentC->getInternalPosition(),
                       agent->getTraverseCost(),
                       tPoint(x, y, DIRECTION_VECTOR_RANGE)
                       );

    _namedObstacles.push_back(info);
    _namedObstacleIDs.push_back(id);
    if(agent->isMoveable())
    {
        _namedObstacleShapes.push_back(agentC->refShape().refCS());
    }
    else
    {
        cConvexPoly offsetShape = agentC->refPlacedShape();
        offsetShape -= agentC->getInternalPosition().p;
        _namedObstacleShapes.push_back(offsetShape);
    }
}
iAgent*
cMesh::retrieveNamedObstacle(const char* id) const
{
    cMutexLock lock(_meshMutex);
    INTERFACE_ASSERT(id);
    size_t i;
    for(i = 0; i < _namedObstacleIDs.size(); i++)
    {
        if(_namedObstacleIDs[i] == id)
        {
            cPosition p;
            convertPosition(_namedObstacles[i]._position, p);
            iAgent* result;
            if(_namedObstacleShapes[i].getRangeUsed() > SHAPE_RANGE)
            {
                cConvexPoly placedShape;
                _namedObstacleShapes[i].offset(_namedObstacles[i]._position.p, placedShape);
                placedShape.setRange(WORLD_RANGE);
                result = placeLargeStaticObstacle_Internal(placedShape, p);
            }
            else
            {
                result = placeAgent(_namedObstacleShapes[i], p);
            }
            result->setTraverseCost(_namedObstacles[i]._costToTraverse);
            tPoint v = _namedObstacles[i]._directionForCostToTraverse;
            result->setTraverseCostDirection(v.getX(), v.getY());
            return result;
        }
    }
    return 0;
}

tSigned32
cMesh::getNumberOfNamedObstacles() const
{
    cMutexLock lock(_meshMutex);
    return static_cast<tSigned32>(_namedObstacleIDs.size());
}
void
cMesh::retrieveNamedObstacleByIndex(tSigned32 index, iAgent*& agent, const char*& id) const
{
    cMutexLock lock(_meshMutex);
    INTERFACE_ASSERT(index >= 0 && index < getNumberOfNamedObstacles());
    id = _namedObstacleIDs[index].c_str();

    cPosition p;
    convertPosition(_namedObstacles[index]._position, p);

    if(_namedObstacleShapes[index].getRangeUsed() > SHAPE_RANGE)
    {
        cConvexPoly placedShape;
        _namedObstacleShapes[index].offset(_namedObstacles[index]._position.p, placedShape);
        placedShape.setRange(WORLD_RANGE);
        agent = placeLargeStaticObstacle_Internal(placedShape, p);
    }
    else
    {
        agent = placeAgent(_namedObstacleShapes[index], p);
    }

    agent->setTraverseCost(_namedObstacles[index]._costToTraverse);
    tPoint v = _namedObstacles[index]._directionForCostToTraverse;
    agent->setTraverseCostDirection(v.getX(), v.getY());
}

cPosition
cMesh::positionFor3DPoint(const tSigned32* point) const
{
    return positionInSectionFor3DPoint(-1, point);
}
cPosition cMesh::positionFor3DPointF(const float *point) const
{
    tSigned32 lpoint[3];
    lpoint[0]=static_cast<tSigned32>(point[0]);
    lpoint[1]=static_cast<tSigned32>(point[1]);
    lpoint[2]=static_cast<tSigned32>(point[2]);
    return positionFor3DPoint(lpoint);
}

cPosition
cMesh::positionNear3DPoint(const tSigned32* point, tSigned32 horizontalRange, tSigned32 verticalRange) const
{
    return positionInSectionNear3DPoint(-1, point, horizontalRange, verticalRange);
}

cPosition
cMesh::positionInSection(tSigned32 sectionID, tSigned32 x, tSigned32 y) const
{
    cMutexLock lock(_meshMutex);
    if(sectionID != -1 && !_sectionIDs)
    {
        return cPosition();
    }
    tPoint p(x, y, WORLD_RANGE);
    {
        tSigned32 segments = _partitioning->queryFacesPotentiallyStabbed(p, &_partitioningResultsBuffer.front());
        tSigned32 i;
        for(i = 0; i < segments; i++)
        {
            tSigned32 f3DIndex = _partitioningResultsBuffer[i];
            if(sectionID != -1 && (*_sectionIDs)[f3DIndex] != sectionID)
            {
                continue;
            }
            tFace_3D f3D = _mesh3D->faceAt(f3DIndex);
            if(!PointIsInFace<tMesh_3D>(f3D, p))
            {
                continue;
            }
            tFace f2D = _translation->translate(f3D, p);
            cPosition result;
            result.x = x;
            result.y = y;
            result.cell = f2D.index();
            return result;
        }
    }
    return cPosition();
}

cPosition
cMesh::positionInSectionFor3DPoint(tSigned32 sectionID, const tSigned32* point) const
{
    cMutexLock lock(_meshMutex);
    if(sectionID != -1 && !_sectionIDs)
    {
        return cPosition();
    }
    tPoint p(point[0], point[1], WORLD_RANGE);
    float bestHeight;
    tFace_3D bestFace;
    bestFace.clear();
    {
        tSigned32 segments = _partitioning->queryFacesPotentiallyStabbed(p, &_partitioningResultsBuffer.front());
        tSigned32 i;
        for(i = 0; i < segments; i++)
        {
            tSigned32 f3DIndex = _partitioningResultsBuffer[i];
            if(sectionID != -1 && (*_sectionIDs)[f3DIndex] != sectionID)
            {
                continue;
            }
            tFace_3D f3D = _mesh3D->faceAt(f3DIndex);
            if(!PointIsInFace<tMesh_3D>(f3D, p))
            {
                continue;
            }

            float groundHeight = heightAtPointIn3DFace(f3DIndex, p);

            if(groundHeight > static_cast<float>(point[2]))
            {
                continue;
            }
            if(bestFace.valid() && bestHeight >= groundHeight)
            {
                continue;
            }
            bestFace = f3D;
            bestHeight = groundHeight;
        }
    }
    if(!bestFace.valid())
    {
        return cPosition();
    }
    tFace f2D = _translation->translate(bestFace, p);

    cPosition result;
    result.x = point[0];
    result.y = point[1];
    result.cell = f2D.index();
    return result;
}
cPosition
cMesh::positionInSectionFor3DPointF(tSigned32 sectionID, const float* point) const
{
    tSigned32 lpoint[3];
    lpoint[0]=static_cast<tSigned32>(point[0]);
    lpoint[1]=static_cast<tSigned32>(point[1]);
    lpoint[2]=static_cast<tSigned32>(point[2]);
    return positionInSectionFor3DPoint(sectionID, lpoint);
}

cPosition
cMesh::positionInSectionNear3DPoint(tSigned32 sectionID, const tSigned32* point, tSigned32 horizontalRange, tSigned32 verticalRange) const
{
    INTERFACE_ASSERT(point[0] - horizontalRange >= -WORLD_RANGE);
    INTERFACE_ASSERT(point[1] - horizontalRange >= -WORLD_RANGE);
    INTERFACE_ASSERT(point[0] + horizontalRange <= WORLD_RANGE);
    INTERFACE_ASSERT(point[1] + horizontalRange <= WORLD_RANGE);
    cMutexLock lock(_meshMutex);
    if(sectionID != -1 && !_sectionIDs)
    {
        return cPosition();
    }
    tPoint p(point[0], point[1], WORLD_RANGE);
    tPoint bottomLeft(point[0] - horizontalRange, point[1] - horizontalRange, WORLD_RANGE);
    tPoint topRight(point[0] + horizontalRange, point[1] + horizontalRange, WORLD_RANGE);

    tPoint::tDistance bestDistance;
    tFace_3D bestFace;
    bestFace.clear();
    tPoint bestP;

    {
        tSigned32 i;
        tSigned32 faces = _partitioning->queryFacesPotentiallyOverlapped(
                        bottomLeft, topRight, &_partitioningResultsBuffer.front());
        for(i = 0; i < faces; i++)
        {
            tSigned32 f3DIndex = _partitioningResultsBuffer[i];
            if(sectionID != -1 && (*_sectionIDs)[f3DIndex] != sectionID)
            {
                continue;
            }
            tFace_3D f3D = _mesh3D->faceAt(f3DIndex);

            tPoint candidateP;
            if(PointIsInFace<tMesh_3D>(f3D, p))
            {
                candidateP = p;
            }
            else
            if(!GetClosestPointInFace(f3D, p, candidateP))
            {
                continue;
            }

            tSigned32 groundHeight = static_cast<tSigned32>(heightAtPointIn3DFace(f3DIndex, candidateP));

            if(groundHeight < point[2] - verticalRange || groundHeight > point[2] + verticalRange)
            {
                continue;
            }

            if(p == candidateP)
            {
                bestFace = f3D;
                bestP = p;
                break;
            }
            
            tPoint::tDistance candidateDistance;
            LengthSquared(candidateP - p, candidateDistance);
            if(bestFace.valid() && CompareDistances(bestDistance, candidateDistance) != -1)
            {
                continue;
            }

            bestFace = f3D;
            bestDistance = candidateDistance;
            bestP = candidateP;
        }
    }

    if(!bestFace.valid())
    {
        return cPosition();
    }

    tFace f2D = _translation->translate(bestFace, bestP);

    cPosition result;
    result.x = bestP.getX();
    result.y = bestP.getY();
    result.cell = f2D.index();
    return result;
}

cPosition
cMesh::positionFor3DPoint_ExcludeTerrain(const tSigned32* point, tSigned32 numberOfTerrainLayers) const
{
    cMutexLock lock(_meshMutex);
    tPoint p(point[0], point[1], WORLD_RANGE);
    float bestHeight;
    tFace_3D bestFace;
    bestFace.clear();
    {
        tSigned32 segments = _partitioning->queryFacesPotentiallyStabbed(p, &_partitioningResultsBuffer.front());
        tSigned32 i;
        for(i = 0; i < segments; i++)
        {
            tSigned32 f3DIndex = _partitioningResultsBuffer[i];
            if(_sectionIDs && (*_sectionIDs)[f3DIndex] >= 0 && (*_sectionIDs)[f3DIndex] < numberOfTerrainLayers)
            {
                continue;
            }
            tFace_3D f3D = _mesh3D->faceAt(f3DIndex);
            if(!PointIsInFace<tMesh_3D>(f3D, p))
            {
                continue;
            }

            float groundHeight = heightAtPointIn3DFace(f3DIndex, p);

            if(groundHeight > static_cast<float>(point[2]))
            {
                continue;
            }
            if(bestFace.valid() && bestHeight >= groundHeight)
            {
                continue;
            }
            bestFace = f3D;
            bestHeight = groundHeight;
        }
    }
    if(!bestFace.valid())
    {
        return cPosition();
    }
    tFace f2D = _translation->translate(bestFace, p);

    cPosition result;
    result.x = point[0];
    result.y = point[1];
    result.cell = f2D.index();
    return result;
}

cPosition
cMesh::generateRandomPosition() const
{
    cMutexLock lock(_meshMutex);
    iRandomSequence& randomSequence = nSingletons::pathEngine().lockRandomSequence();
    cInternalPosition ip = RandomPosition(ref2D(), _mesh2DInternalFaces, randomSequence);
    nSingletons::pathEngine().releaseRandomSequence();
    cPosition result;
    convertPosition(ip, result);
    return result;
}

cPosition
cMesh::generateRandomPositionLocally(const cPosition& centre, tSigned32 range) const
{
    cMutexLock lock(_meshMutex);
    cInternalPosition centreIP;
    convertPosition(centre, centreIP);
    cQueryContext& qc = enterQuery();

    INTERFACE_ASSERT(range > 0);
    cConvexPoly clippedBoundsShape(centreIP.p, range, WORLD_RANGE + 1);
    cLocalisedConvexShape bounds(qc, clippedBoundsShape, centreIP);
    tPoint boundsBottomLeft = centreIP.p - tPoint(range, range, WORLD_RANGE);
    tPoint boundsTopRight = centreIP.p + tPoint(range, range, WORLD_RANGE);

    exitQuery(qc);
    const std::vector<tSigned32>& facesOverlapped = bounds.refFacesOverlapped();
    assertD(!facesOverlapped.empty());
    while(1)
    {
        tSigned32 r = nSingletons::pathEngine().random31();
        tSigned32 rX = nSingletons::pathEngine().random31();
        tSigned32 rY = nSingletons::pathEngine().random31();

        tSigned32 faceIndex = facesOverlapped[r % facesOverlapped.size()];

        tFace f = ref2D().faceAt(faceIndex);

        tPoint bottomLeft, topRight;
        GetFaceBounds<tMesh>(f, bottomLeft, topRight);        

        bottomLeft.updateAsMaximumBound(boundsBottomLeft);
        topRight.updateAsMinimumBound(boundsTopRight);

        tPoint range = topRight - bottomLeft;

        if(range.getX() == 0 || range.getY() == 0)
        {
            continue;
        }

        tPoint p(bottomLeft.getX() + rX % range.getX(),
                 bottomLeft.getY() + rY % range.getY(),
                 WORLD_RANGE);

        if(PointIsInFace<tMesh>(f, p))
        {
            if(!bounds.poly().pointCollides2D_Inverted(p))
            {
                cPosition result;
                result.x = p.getX();
                result.y = p.getY();
                result.cell = f.index();
                return result;
            }
        }

    }
}

cPosition
cMesh::generateRandomPositionInSection(tSigned32 sectionID) const
{
    cMutexLock lock(_meshMutex);
  	INTERFACE_ASSERT(sectionID != -1);
    tSigned32 i;

// first, generate or retrieve cached bounds for this section

    for(i = 0; i != SizeL(_sectionsWithBounds); ++i)
    {
        if(_sectionsWithBounds[i] == sectionID)
        {
            break;
        }
    }
    if(i == SizeL(_sectionsWithBounds))
    {
      // not cached, so generate
        _sectionsWithBounds.push_back(sectionID);
        _sectionBoundsValid.push_back(false);
        _sectionBounds.push_back(tPoint());
        _sectionBounds.push_back(tPoint());
        for(tSigned32 j = 0; j < SizeL(*_sectionIDs); ++j)
        {
            tSigned32 sectionID = (*_sectionIDs)[j];
            if(sectionID == -1)
            {
                continue;
            }
            tFace_3D f3D = _mesh3D->faceAt(j);
            tPoint bottomLeft, topRight;
            GetFaceBounds<tMesh_3D>(f3D, bottomLeft, topRight);
            if(_sectionBoundsValid[i])
            {
                _sectionBounds[i * 2 + 0].updateAsMinimumBound(bottomLeft);
                _sectionBounds[i * 2 + 1].updateAsMaximumBound(topRight);
            }
            else
            {
                _sectionBounds[i * 2 + 0] = bottomLeft;
                _sectionBounds[i * 2 + 1] = topRight;
                _sectionBoundsValid.setBit(i, true);
            }
        }
    }

// --

    if(!_sectionBoundsValid[i])
    {
      // there are no faces in this mesh with the given section ID
        return cPosition();
    }

    tPoint bottomLeft = _sectionBounds[i * 2 + 0];
    tPoint topRight = _sectionBounds[i * 2 + 1];
    tPoint range = topRight - bottomLeft;
    cPosition result;
    for(i = 0; i < 10; i++)
    {
        tSigned32 rX = nSingletons::pathEngine().random31();
        tSigned32 rY = nSingletons::pathEngine().random31();
        result = positionInSection(sectionID,
                            bottomLeft.getX() + rX % range.getX(),
                            bottomLeft.getY() + rY % range.getY());
        if(result.cell != -1)
        {
            return result;
        }
    }
    return result;
}

tSigned32 cMesh::heightAtPosition(const cPosition& position) const
{
    cMutexLock lock(_meshMutex);
    return static_cast<tSigned32>(heightAtPositionF(position));
}

float
cMesh::heightAtPositionF(const cPosition& position) const
{
    cMutexLock lock(_meshMutex);
	INTERFACE_CHECK(positionIsValid(position), "iMesh::heightAtPositionF() called with invalid position");
    cInternalPosition p;
    convertPosition(position, p);
    tFace_3D f = _translation->translate(p.f, p.p);
    tSigned32 fIndex = f.index();
    if(_sectionIDs)
    {
        tSigned32 terrainLayer = (*_sectionIDs)[fIndex];
        if(terrainLayer >= 0 && terrainLayer < SizeL(_terrainCallBacks) && _terrainCallBacks[terrainLayer])
        {
            return _terrainCallBacks[terrainLayer]->getHeightAt(position.x, position.y);
        }
    }
    return f->heightForF(p.p);
}

float
cMesh::heightAtPositionWithPrecision(const cPosition& position, float precisionX, float precisionY) const
{
    cMutexLock lock(_meshMutex);
	INTERFACE_CHECK(positionIsValid(position), "iMesh::heightAtPositionWithPrecision() called with invalid position");
    cInternalPosition p;
    convertPosition(position, p);
    tFace_3D f = _translation->translate(p.f, p.p);
    tSigned32 fIndex = f.index();
    if(_sectionIDs)
    {
        tSigned32 terrainLayer = (*_sectionIDs)[fIndex];
        if(terrainLayer >= 0 && terrainLayer < SizeL(_terrainCallBacks) && _terrainCallBacks[terrainLayer])
        {
            return _terrainCallBacks[terrainLayer]->getHeightAtWithPrecision(position.x, position.y, precisionX, precisionY);
        }
    }
    return f->heightForF(p.p.getX() + precisionX, p.p.getY() + precisionY);
}

tSigned32
cMesh::getCellForEndOfLine(const cPosition& start, tSigned32 x, tSigned32 y) const
{
    cMutexLock lock(_meshMutex);
	INTERFACE_CHECK(positionIsValid(start), "iMesh::getCellForEndOfLine() called with invalid position");
    if(start.x == x && start.y == y)
    {
        return start.cell;
    }
    cInternalPosition p;
    convertPosition(start, p);

    tFace endF = p.f;
    tLine line(p.p, tPoint(x, y, WORLD_RANGE));
    tEdge crossed;
    while(TraverseTowardsTarget_SI<tMesh>(endF, line, line.end(), crossed))
    {
    }
    if(endF->isExternal())
    {
        return -1;
    }
    return endF.index();
}

bool
cMesh::positionIsValid(const cPosition& p) const
{
    cMutexLock lock(_meshMutex);
    return PositionIsValid(ref2D(), p);
}

void
cMesh::burnContextIntoMesh(iCollisionContext* contextI)
{
    cMutexLock lock(_meshMutex);
    INTERFACE_ASSERT(!_preprocessGenerated);
    INTERFACE_ASSERT(!_preprocessGenerationCompleted);
    if(!contextI)
    {
        return;
    }
    _checkSumValid = false;
    cCollisionContext* context = FromI(contextI);
    INTERFACE_ASSERT(&context->refMesh() == this);

    cQueryContext& qc = enterQuery();

    if(context->getNumberOfObstacleSets())
    {
        Error("Warning", "iMesh::burnContextIntoMesh called for a context with obstacle sets added. These obstacle sets will be ignored.");
    }

    tSigned32 i;
    for(i = 0; i < context->getNumberOfAgents(); ++i)
    {
        cAgent* agent = FromI(context->getAgent(i));

        tSigned32 x, y;
        agent->getTraverseCostDirection(x, y);

        cObstacleInfo info(agent->getInternalPosition(),
                           agent->getCostToTraverse(),
                           tPoint(x, y, DIRECTION_VECTOR_RANGE)
                           );

        if(agent->isMoveable())
        {
            _baseObstacleShapes.push_back(agent->refShape().refCS());
        }
        else
        {
            cConvexPoly offsetShape = agent->refPlacedShape();
            offsetShape -= agent->getInternalPosition().p;
            _baseObstacleShapes.push_back(offsetShape);
        }
        _baseObstacles.push_back(info);

        if(_allowCollisionWithoutPreprocess && agent->getCostToTraverse() == -1.f)
        {
            if(!_unexpandedBaseObstacles)
            {
                _unexpandedBaseObstacles = new cLocalisedConvexShapeSet(_mesh2DInternalFaces);
            }
            cLocalisedConvexShape* lcs;
            lcs = new cLocalisedConvexShape(qc, agent->refPlacedShape(), agent->getInternalPosition());
            _unexpandedBaseObstacles->push_back(lcs); // this shape must then be deleted on destruction of the set
        }
    }
    if(_unexpandedBaseObstacles)
    {
        _unexpandedBaseObstacles->setCompleted();
    }

    exitQuery(qc);

    if(!context->surfaceTypeCostsEmpty())
    {
        _baseSurfaceTypeCosts.push_back(context->cloneSurfaceTypeCosts());
    }
}

void
cMesh::saveGround(const char* format, bool include_mapping, iOutputStream* output_stream) const
{
    cMutexLock lock(_meshMutex);
	INTERFACE_CHECK(!_preprocessGenerationCompleted, "iMesh::saveGround() cannot be called after iMesh::preprocessGenerationCompleted(). Returning with no effect.");
    if(_preprocessGenerationCompleted)
    {
        return;
    }
    INTERFACE_ASSERT(output_stream);
    if(strcmp(format,"xml") == 0)
    {
        cStandardXMLOutputStream os(*output_stream);
        doSaveGround(os, include_mapping);
    }
    else
    {
        INTERFACE_ASSERT(strcmp(format,"tok") == 0);
        cElementAndAttributeEnumerator enumerator;
        doSaveGround(enumerator, include_mapping);
        enumerator.writeTo(*output_stream);
        cTokenisedXMLOutputStream os(enumerator, *output_stream);
        doSaveGround(os, include_mapping);
    }
}

void
cMesh::setTerrainCallBack(tSigned32 terrainLayer, iTerrainCallBack* callBack)
{
    cMutexLock lock(_meshMutex);
    INTERFACE_ASSERT(!_preprocessGenerated);
	INTERFACE_CHECK(terrainLayer >= 0 && terrainLayer <= _topSectionID, "iMesh::setTerrainCallBack() called with invalid terrain layer");
    if(terrainLayer >= SizeL(_terrainCallBacks))
    {
        _terrainCallBacks.resize(terrainLayer + 1, 0);
    }
    _terrainCallBacks[terrainLayer] = callBack;
}

tSigned32
cMesh::addEndPoint(const cPosition& position)
{
    cMutexLock lock(_meshMutex);
	INTERFACE_ASSERT(positionIsValid(position));
    INTERFACE_ASSERT(!_preprocessGenerated);
    INTERFACE_ASSERT(!_preprocessGenerationCompleted);
    cInternalPosition ip;
    convertPosition(position, ip);
    return _offMeshConnections->addEndPoint(ip);
}

tSigned32
cMesh::addOffMeshConnection(tSigned32 fromEndPoint, tSigned32 toEndPoint, tSigned32 penalty)
{
	INTERFACE_CHECK(penalty >= 0 && penalty <= 10000, "iMesh::addMeshConnection() - penalty must be in the range 0 to 10000.");
    cMutexLock lock(_meshMutex);
    INTERFACE_ASSERT(!_preprocessGenerated);
    INTERFACE_ASSERT(!_preprocessGenerationCompleted);
    INTERFACE_ASSERT(fromEndPoint >= 0 && fromEndPoint < _offMeshConnections->numberOfEndPoints());
    INTERFACE_ASSERT(toEndPoint >= 0 && toEndPoint < _offMeshConnections->numberOfEndPoints());
    return _offMeshConnections->addConnection(fromEndPoint, toEndPoint, penalty);
}

tSigned32
cMesh::getNumberOfEndPoints() const
{
    cMutexLock lock(_meshMutex);
    return _offMeshConnections->numberOfEndPoints();
}
cPosition
cMesh::getEndPoint(tSigned32 index) const
{
    cMutexLock lock(_meshMutex);
    cInternalPosition ip = _offMeshConnections->refEndPoint(index);
    cPosition result;
    convertPosition(ip, result);
    return result;
}
tSigned32
cMesh::getNumberOfOffMeshConnections() const
{
    cMutexLock lock(_meshMutex);
    return _offMeshConnections->numberOfConnections();
}
void
cMesh::getOffMeshConnectionInfo(tSigned32 index, tSigned32& fromEndPoint, tSigned32& toEndPoint, tSigned32& penalty) const
{
    cMutexLock lock(_meshMutex);
    fromEndPoint = _offMeshConnections->refConnection(index).fromEndPoint;
    toEndPoint = _offMeshConnections->refConnection(index).toEndPoint;
    penalty = _offMeshConnections->refConnection(index).penalty;
}

void
cMesh::clearOffMeshConnectionsAndEndPoints()
{
    INTERFACE_ASSERT(!_preprocessGenerated);
    INTERFACE_ASSERT(!_preprocessGenerationCompleted);
    _offMeshConnections->clear();
}

void
cMesh::generateCollisionPreprocessFor(iShape* shape, const char *const* attributes)
{
    cMutexLock lock(_meshMutex);
    INTERFACE_ASSERT(!_preprocessGenerationCompleted);
    INTERFACE_ASSERT(shapeCanCollide(shape) == false);
    _preprocessGenerated = true;
    cShape* cs = static_cast<cShape*>(shape);
    cs->declareWillBePreprocessed();
    {
        tSigned32 i = cs->getPreprocessIndex();
        cQueryContext& qc = enterQuery();
        cCollisionPreprocess* cp;
        cp = new cCollisionPreprocess(
                qc, 
                _mesh2D_NoInternalVertices,
                _baseObstacles, _baseObstacleShapes,
                cs->refCS(),
                attributes
                );
        _collisionPreprocess.set(i, cp);
        exitQuery(qc);
        addElementCountRequirement(_collisionPreprocess.get(i)->getElementCountRequirement());
    }
}

void
cMesh::generatePathfindPreprocessFor(iShape* shape, const char *const* attributes)
{
    cMutexLock lock(_meshMutex);
    INTERFACE_ASSERT(!_preprocessGenerationCompleted);
    INTERFACE_ASSERT(shapeCanCollide(shape) == true);
    INTERFACE_ASSERT(shapeCanPathfind(shape) == false);
    cShape* cs = static_cast<cShape*>(shape);
    tSigned32 i = cs->getPreprocessIndex();

    bool keepPartitioned =
        (GetValueForAttribute(attributes, "keep_silhouette_regions_partitioned") != 0);
    bool justGeneratePartitioned =
        (GetValueForAttribute(attributes, "just_generate_partitioned") != 0);

    cQueryContext& qc = enterQuery();

    cPathfindPreprocess* pp;

    buildAnythingRequiredForPathfindPreprocess();

    cHandle<cManagedSoftObstacleExpansions> softObstacleExpansions = new cManagedSoftObstacleExpansions();
    softObstacleExpansions->add(qc, _baseObstacles, _baseObstacleShapes, cs->refCS());
    softObstacleExpansions->setCompleted();

    pp = new cPathfindPreprocess(
                    qc,
//                    _collisionPreprocess.get(i),
                    _collisionPreprocess.get(i)->getEdgeExpansion(),
                    _collisionPreprocess.get(i)->getBaseObstacleExpansions(),
                    _collisionPreprocess.get(i)->getAttributes(),
                    *_offMeshConnections,
//                    _baseObstacles,
//                    cs->refCS(),
                    softObstacleExpansions,
                    *_mesh3DCorners,
                    _mesh3DCostCalculators,
                    attributes,
                    keepPartitioned,
                    justGeneratePartitioned
                    );

    _pathfindPreprocess.set(i, pp);

    exitQuery(qc);
//    addElementCountRequirement(_pathfindPreprocess.get(i)->getElementCountRequirement());
}

void
cMesh::releaseAllPreprocessFor(iShape* shape)
{
    cMutexLock lock(_meshMutex);
    if(!shapeCanCollide(shape))
    {
        return;
    }
    cShape* cs = static_cast<cShape*>(shape);
    tSigned32 i=cs->getPreprocessIndex();
    assertD(i != -1);
    releasePreprocessFor(i);
}

void
cMesh::releasePathfindPreprocessFor(iShape* shape)
{
    cMutexLock lock(_meshMutex);
    if(!shapeCanPathfind(shape))
    {
        return;
    }
    cShape* cs = static_cast<cShape*>(shape);
    tSigned32 i = cs->getPreprocessIndex();
    _pathfindPreprocess.set(i, 0);
}

void
cMesh::preprocessGenerationCompleted()
{
    cMutexLock lock(_meshMutex);
    INTERFACE_ASSERT(!_preprocessGenerationCompleted);
    _preprocessGenerationCompleted = true;
    delete _mesh2D_NoInternalVertices;
    _mesh2D_NoInternalVertices = 0;
}

bool
cMesh::shapeCanCollide(const iShape* shape) const
{
    cMutexLock lock(_meshMutex);
    const cShape* cs = static_cast<const cShape*>(shape);
    tSigned32 index = cs->getPreprocessIndex();
    if(index == -1)
    {
        return false;
    }
    return _collisionPreprocess.get(index).isSet();
}
bool
cMesh::shapeCanPathfind(const iShape* shape) const
{
    cMutexLock lock(_meshMutex);
    const cShape* cs = static_cast<const cShape*>(shape);
    tSigned32 index = cs->getPreprocessIndex();
    if(index == -1)
    {
        return false;
    }
    return _pathfindPreprocess.get(index).isSet();
}

const char *const*
cMesh::getCollisionPreprocessAttributes(iShape* shape) const
{
    cMutexLock lock(_meshMutex);
    INTERFACE_ASSERT(shapeCanCollide(shape));
    cShape* cs = static_cast<cShape*>(shape);
    tSigned32 i = cs->getPreprocessIndex();
    return _collisionPreprocess.get(i)->getAttributes();
}
const char *const*
cMesh::getPathfindPreprocessAttributes(iShape* shape) const
{
    INTERFACE_ASSERT(shapeCanPathfind(shape));
    return 0;
}

void
cMesh::saveCollisionPreprocessFor(iShape* shape, iOutputStream* os) const
{
    cMutexLock lock(_meshMutex);

	INTERFACE_CHECK(!_preprocessGenerationCompleted, "iMesh::saveCollisionPreprocessFor() cannot be called after iMesh::preprocessGenerationCompleted(). Returning with no effect.");
    if(_preprocessGenerationCompleted)
    {
        return;
    }
    INTERFACE_ASSERT(shapeCanCollide(shape));
    cShape* cs = static_cast<cShape*>(shape);
    tSigned32 i = cs->getPreprocessIndex();

    makeCheckSumValid();

    cElementAndAttributeEnumerator enumerator;
    
    DoSaveCollisionPreprocess(*_collisionPreprocess.get(i), _checkSum, shape, enumerator);

    enumerator.writeTo(*os);

    cTokenisedXMLOutputStream xos(enumerator, *os);
    
    DoSaveCollisionPreprocess(*_collisionPreprocess.get(i), _checkSum, shape, xos);
}
void
cMesh::savePathfindPreprocessFor(iShape* shape, iOutputStream* os) const
{
    cMutexLock lock(_meshMutex);

	INTERFACE_CHECK(!_preprocessGenerationCompleted, "iMesh::savePathfindPreprocessFor() cannot be called after iMesh::preprocessGenerationCompleted(). Returning with no effect.");
    if(_preprocessGenerationCompleted)
    {
        return;
    }

    INTERFACE_ASSERT(shapeCanPathfind(shape));
    cShape* cs = static_cast<cShape*>(shape);
    tSigned32 i = cs->getPreprocessIndex();

    makeCheckSumValid();

    cElementAndAttributeEnumerator enumerator;
    doSavePathfindPreprocess(enumerator, cs);
    enumerator.writeTo(*os);
    cTokenisedXMLOutputStream xos(enumerator, *os);
    doSavePathfindPreprocess(xos, cs);
}
void
cMesh::loadCollisionPreprocessFor(iShape* shape, const char* dataBuffer, tUnsigned32 bufferSize)
{
    cMutexLock lock(_meshMutex);
    INTERFACE_ASSERT(!_preprocessGenerationCompleted);
    INTERFACE_ASSERT(shapeCanCollide(shape) == false);
    _preprocessGenerated = true;
    cShape* cs = static_cast<cShape*>(shape);
    cs->declareWillBePreprocessed();
    tSigned32 i = cs->getPreprocessIndex();

    tSigned32 majorVersion, minorVersion, meshCheckSum;

    cTokenisedXMLInputStream is(dataBuffer, bufferSize);
    {
        const char *const* attributes;
        attributes = is.openElement("collisionPreprocess");
        majorVersion = ValueAsLong(attributes, "majorVersion");
        minorVersion = ValueAsLong(attributes, "minorVersion");
        meshCheckSum = ValueAsLong(attributes, "meshCheckSum");
    }

    makeCheckSumValid();
    if(meshCheckSum != _checkSum)
    {
        Error("Fatal", "cMesh::loadCollisionPreprocessFor() - mesh checksum does not match saved checksum.");
        PathEngine_Abort();
    }
    if(!savedCollisionShapeMatches(cs->refCS(), is))
    {
        Error("Fatal", "cMesh::loadCollisionPreprocessFor() - shape does not match saved shape.");
        PathEngine_Abort();
    }

    cAttributesBuilder attributesBuilder;
    {
        const char *const* attributes;
        attributes = is.openElement("attributes");
        attributesBuilder.addCopies(attributes);
        is.closeElement();
    }

    cQueryContext& qc = enterQuery();
    cCollisionPreprocess* cp;
    if(majorVersion != cCollisionPreprocess::majorVersion() || minorVersion < cCollisionPreprocess::minorVersion())
    {
        Error("Warning", "Collision preprocess persistent data version is incompatible. Building from scratch.");
        cp = new cCollisionPreprocess(
                qc, 
                _mesh2D_NoInternalVertices,
                _baseObstacles, _baseObstacleShapes,
                cs->refCS(),
                attributesBuilder.get());
    }
    else
    {
        cp = new cCollisionPreprocess(qc, is, _baseObstacles, _baseObstacleShapes, cs->refCS(), attributesBuilder.get());
    }
    _collisionPreprocess.set(i, cp);
    addElementCountRequirement(_collisionPreprocess.get(i)->getElementCountRequirement());
    exitQuery(qc);

    is.closeElement();
}
void
cMesh::loadPathfindPreprocessFor(iShape* shape, const char* dataBuffer, tUnsigned32 bufferSize)
{
    cMutexLock lock(_meshMutex);
    INTERFACE_ASSERT(!_preprocessGenerationCompleted);
    INTERFACE_ASSERT(shapeCanCollide(shape) == true);
    INTERFACE_ASSERT(shapeCanPathfind(shape) == false);
    cShape* cs = static_cast<cShape*>(shape);
    tSigned32 i = cs->getPreprocessIndex();

    cTokenisedXMLInputStream is(dataBuffer, bufferSize);
    const char *const* attributes = is.openElement("pathfindPreprocess");
    tSigned32 majorVersion = ValueAsLong(attributes, "majorVersion");
    tSigned32 minorVersion = ValueAsLong(attributes, "minorVersion");

    {
        tSigned32 meshCheckSum = ValueAsLong(attributes, "meshCheckSum");
        makeCheckSumValid();
        if(meshCheckSum != _checkSum)
        {
            Error("Fatal", "cMesh::loadPathfindPreprocessFor() - mesh checksum does not match saved checksum.");
            PathEngine_Abort();
        }
    }
    if(!savedCollisionShapeMatches(cs->refCS(), is))
    {
        Error("Fatal", "cMesh::loadPathfindPreprocessFor() - shape does not match saved shape.");
        PathEngine_Abort();
    }

    cQueryContext& qc = enterQuery();

    cPathfindPreprocess* pp;

    buildAnythingRequiredForPathfindPreprocess();

    cAttributesBuilder attributesBuilder;
    {
        const char* nextElement = is.peekNextElement();
        if(strcmp(nextElement, "attributes") == 0)
        {
            const char *const* attributes;
            attributes = is.openNextElement();
            attributesBuilder.addCopies(attributes);
            is.closeElement();
        }
    }

    cHandle<cManagedSoftObstacleExpansions> softObstacleExpansions = new cManagedSoftObstacleExpansions();
    softObstacleExpansions->add(qc, _baseObstacles, _baseObstacleShapes, cs->refCS());
    softObstacleExpansions->setCompleted();

    if(majorVersion != cPathfindPreprocess::majorVersion() || minorVersion < cPathfindPreprocess::minorVersion())
    {
        Error("Warning", "Pathfind preprocess persistent data version is incompatible. Building from scratch.");
        pp = new cPathfindPreprocess(
                        qc,
//                        _collisionPreprocess.get(i),
                        _collisionPreprocess.get(i)->getEdgeExpansion(),
                        _collisionPreprocess.get(i)->getBaseObstacleExpansions(),
                        _collisionPreprocess.get(i)->getAttributes(),
                        *_offMeshConnections,
//                        _baseObstacles,
//                        cs->refCS(),
                        softObstacleExpansions,
                        *_mesh3DCorners,
                        _mesh3DCostCalculators,
                        attributesBuilder.get(),
                        false);
    }
    else
    {
        is.openElement("preprocess");
        pp = new cPathfindPreprocess(qc,
//                        _collisionPreprocess.get(i),
                        _collisionPreprocess.get(i)->getEdgeExpansion(),
                        _collisionPreprocess.get(i)->getBaseObstacleExpansions(),
                        _collisionPreprocess.get(i)->getAttributes(),
                        *_offMeshConnections,
//                        _baseObstacles,
//                        cs->refCS(),
                        softObstacleExpansions,
                        *_mesh3DCorners,
                        _mesh3DCostCalculators,
                        is,
                        attributesBuilder.get()
                        );
        is.closeElement();
    }
    is.closeElement();

    _pathfindPreprocess.set(i, pp);

    exitQuery(qc);
}

iAgent*
cMesh::placeAgent(iShape* shape, const cPosition& position) const
{
	INTERFACE_CHECK(positionIsValid(position), "iMesh::placeAgent() called with invalid position");
    cShape* cs =static_cast<cShape*>(shape);
    cAgent* a = new cAgent(const_cast<cMesh*>(this), cs, position);
    cMutexLock lock(_heldAgentsMutex);
    a->setIndexInMesh(_heldAgents.reuseOrPushBack(a));
    return a;
}

iCollisionContext*
cMesh::newContext() const
{
    cMutexLock lock(_meshMutex);
    cCollisionContext* c = new cCollisionContext(const_cast<cMesh*>(this));
    c->setIndexInMesh(_heldContexts.reuseOrPushBack(c));
    return c;
}

iObstacleSet*
cMesh::newObstacleSet() const
{
    return newObstacleSet_WithAttributes(0);
}

bool
cMesh::testPointCollision(
        iShape* shape, 
        iCollisionContext* context, 
        const cPosition& position
        ) const
{
	INTERFACE_CHECK(positionIsValid(position), "iMesh::testPointCollision() - invalid position");
    return testPointCollision_IgnoreOneAgent(shape, context, position, 0);
}

bool
cMesh::testLineCollision(
                iShape* shape, 
                iCollisionContext* context, 
                const cPosition& start, const cPosition& end) const
{
	INTERFACE_CHECK(positionIsValid(start), "iMesh::testLineCollision() - invalid position");
	INTERFACE_CHECK(positionIsValid(end), "iMesh::testLineCollision() - invalid position");
    return testLineCollision_IgnoreOneAgent(shape, context, start, end, 0);
}
bool
cMesh::testLineCollision_XY(
                iShape* shape, 
                iCollisionContext* context, 
                const cPosition& start, tSigned32 x, tSigned32 y, tSigned32& cell) const
{
	INTERFACE_CHECK(positionIsValid(start), "iMesh::testLineCollision_XY() - invalid position");
    return testLineCollision_XY_IgnoreOneAgent(shape, context, start, x, y, cell, 0);
}

iCollisionInfo*
cMesh::firstCollision(
                iShape* shape, 
                iCollisionContext* context, 
                const cPosition& start, tSigned32 x, tSigned32 y, tSigned32& cell) const
{
	INTERFACE_CHECK(positionIsValid(start), "iMesh::firstCollision() - invalid position");
    INTERFACE_CHECK(shapeCanCollide(shape), "iMesh::firstCollision() - no preprocess for this shape");
    if(start.x == x && start.y == y)
    {
        cell=start.cell;
        return 0;
    }
    cShape* cs = static_cast<cShape*>(shape);
    tSigned32 index = cs->getPreprocessIndex();

    tFace startF = ref2D().faceAt(start.cell);

    bool collided = false;
    cAxisFraction distanceAlong;
    tLine collidingLine;

    tLine testLine(tPoint(start.x, start.y, WORLD_RANGE), tPoint(x, y, WORLD_RANGE));

    cQueryContext& qc = enterQuery();

    {
        cHandle<cCollisionPreprocess> cp = getCollisionPreprocessFor(index);
        cp->firstCollision(qc, startF, testLine, collided, distanceAlong, collidingLine);
    }

    iAgent* collidingAgent = 0;
    
    if(context)
    {
        cCollisionContext& cc = static_cast<cCollisionContext&>(*context);
        INTERFACE_ASSERT(&cc.refMesh() == this);
        cCompoundContextCollisionPreprocess cccp;
        cc.addCollisionPreprocessFor(qc, index, cccp);
        cAgent* collidingAgentC = 0;
        cccp.firstCollision(startF, testLine, collided, distanceAlong, collidingLine, collidingAgentC);
        if(collidingAgentC)
        {
            collidingAgent = collidingAgentC;
        }
    }

    exitQuery(qc);

    if(collided)
    {
        cMutexLock lock(_meshMutex);
        _heldCollisionInfos.push_front(0);
        cCollisionInfo* ci = nSingletons::pathEngine().refCollisionInfoPool().get();
        ci->init(const_cast<cMesh*>(this), collidingLine, collidingAgent, _heldCollisionInfos.begin());
        _heldCollisionInfos.front() = ci;
        return ci;
    }

    //.. optimise - use end_face
    cell = getCellForEndOfLine(start, x, y);
    return 0;
}

cPosition
cMesh::findClosestUnobstructedPosition(
        iShape* shape, 
        iCollisionContext* context,
        const cPosition& position,
        tSigned32 maximumDistance
        ) const
{
    cMutexLock lock(_meshMutex);
    INTERFACE_ASSERT(maximumDistance > 0);
	INTERFACE_CHECK(positionIsValid(position), "iMesh::findClosestUnobstructedPoint() - invalid position");
    INTERFACE_CHECK(shapeCanCollide(shape), "iMesh::findClosestUnobstructedPoint() - no preprocess for this shape");

    cInternalPosition centreP;
    convertPosition(position, centreP);

    cShape* cs = static_cast<cShape*>(shape);
    tSigned32 index = cs->getPreprocessIndex();

    cCollisionContext *cc = static_cast<cCollisionContext*>(context);

    cQueryContext& qc = enterQuery();

    cInternalPosition resultIP;
    if(context)
    {
        INTERFACE_ASSERT(&cc->refMesh() == this);
        cCompoundContextCollisionPreprocess cccp;
        cc->addCollisionPreprocessFor(qc, index, cccp);
        resultIP = nClosestUnobstructedPoint::find(
                                        qc,
                                        centreP, maximumDistance,
//                                        _collisionPreprocess.get(index)->refEdgeExpansionLookup(),
                                        *_collisionPreprocess.get(index)->getEdgeExpansion(),
                                        _collisionPreprocess.get(index)->refBaseObstacleExpansions(),
                                        &cccp
                                        );
    }
    else
    {
        resultIP = nClosestUnobstructedPoint::find(
                                        qc,
                                        centreP, maximumDistance,
//                                        _collisionPreprocess.get(index)->refEdgeExpansionLookup(),
                                        *_collisionPreprocess.get(index)->getEdgeExpansion(),
                                        _collisionPreprocess.get(index)->refBaseObstacleExpansions(),
                                        0
                                        );
    }
    exitQuery(qc);

    cPosition result;
    if(resultIP.f.valid())
    {
        convertPosition(resultIP, result);
    }
    return result;
}

void
cMesh::getAllAgentsOverlapped(iShape* shape, 
                    iCollisionContext* context,
                    const cPosition& position,
                    iAgent** resultsBuffer,
                    tSigned32& numberOverlapped
                    ) const
{
    cMutexLock lock(_meshMutex);
	INTERFACE_CHECK(positionIsValid(position), "iMesh::getAllAgentsOverlapped() - invalid position");
    INTERFACE_CHECK(shapeCanCollide(shape), "iMesh::getAllAgentsOverlapped() - no preprocess for this shape");
    cShape* cs = static_cast<cShape*>(shape);
    tSigned32 index = cs->getPreprocessIndex();
    cInternalPosition p;
    convertPosition(position, p);
    numberOverlapped = 0;
    if(context)
    {
        cCollisionContext& cc=static_cast<cCollisionContext&>(*context);
        INTERFACE_ASSERT(&cc.refMesh() == this);

        cMutexLock lock(_agentsAndObstacleSetsMutex);

        cQueryContext& qc = enterQuery();        
//        cHandle<cContextCollisionPreprocess> ccp = cc.getCollisionPreprocessFor(qc, index);
        cCompoundContextCollisionPreprocess cccp;
        cc.addCollisionPreprocessFor(qc, index, cccp);
        exitQuery(qc);

        //... hacky pointer casting to avoid allocating a separate buffer
        numberOverlapped = cccp.getAllAgentsOverlapped(p, (cAgent**)resultsBuffer);
        tSigned32 i;
        for(i = 0; i < numberOverlapped; ++i)
        {
            resultsBuffer[i] = static_cast<iAgent*>(((cAgent**)resultsBuffer)[i]);
        }
    }
}

iPath*
cMesh::findShortestPath(
        iShape* shape, 
        iCollisionContext* context, 
        const cPosition& start, const cPosition& end
        ) const
{
    return findShortestPath_WithQueryCallBack(shape, context, start, end, 0);
}

iPath*
cMesh::findShortestPath_WithQueryCallBack(
        iShape* shape, 
        iCollisionContext* context, 
        const cPosition& start, const cPosition& end,
        iQueryCallBack* queryCallBack
        ) const
{
    PROFILER_SCOPE("cMesh::findShortestPath");
	INTERFACE_CHECK(positionIsValid(start), "iMesh::findShortestPath() - invalid position");
	INTERFACE_CHECK(positionIsValid(end), "iMesh::findShortestPath() - invalid position");
    INTERFACE_CHECK(shapeCanPathfind(shape), "iMesh::findShortestPath() - no preprocess for this shape");
    
    std::vector<cInternalPosition> pathVector;
    std::vector<tSigned32>* connectionIndices = 0;
    
    {
        cShape* cs = static_cast<cShape*>(shape);

        cInternalPosition startIP, endIP;
        convertPosition(start, startIP);
        convertPosition(end, endIP);

        cQueryContext& qc = enterQuery();
        findShortestPath_Internal(
                qc, *cs,
                static_cast<cCollisionContext*>(context),
                startIP, endIP,
                pathVector, connectionIndices,
                queryCallBack);
        exitQuery(qc);
    }

    if(pathVector.empty())
    {
        return 0;
    }

    cPath* result;
    {
        cMutexLock lock(_meshMutex);
        result = nSingletons::pathEngine().refPathPool().get();
        _heldPaths.push_front(result);
        result->attachToMesh(const_cast<cMesh*>(this), _heldPaths.begin());
        result->init(pathVector, connectionIndices);
    }

    return result;
}

iPath*
cMesh::findPathAway(
        iShape* shape,
        iCollisionContext* context,
        const cPosition& start, const cPosition& awayFrom,
        tSigned32 distanceAway
        ) const
{
    return findPathAway_WithQueryCallBack(shape, context, start, awayFrom, distanceAway, 0);
}

iPath*
cMesh::findPathAway_WithQueryCallBack(
        iShape* shape,
        iCollisionContext* context,
        const cPosition& start, const cPosition& awayFrom,
        tSigned32 distanceAway,
        iQueryCallBack* queryCallBack
        ) const
{
    cMutexLock lock(_meshMutex);
	INTERFACE_CHECK(positionIsValid(start), "iMesh::findPathAway() - invalid position");
	INTERFACE_CHECK(positionIsValid(awayFrom), "iMesh::findPathAway() - invalid position");
    INTERFACE_CHECK(shapeCanPathfind(shape), "iMesh::findPathAway() - no preprocess for this shape");

    INTERFACE_ASSERT(distanceAway > 0);


    cShape* cs = static_cast<cShape*>(shape);
    tSigned32 index = cs->getPreprocessIndex();
    cInternalPosition startP, awayFromP;
    convertPosition(start, startP);
    convertPosition(awayFrom, awayFromP);

    cCollisionContext* cc = static_cast<cCollisionContext*>(context);

    cQueryContext& qc = enterQuery();

    std::vector<cInternalPosition> pathPoints;
    std::vector<tSigned32>* connectionIndices = 0;

    cHandle<cContextPathfindPreprocess> cpp;

    if(cc)
    {
        INTERFACE_ASSERT(&cc->refMesh() == this);
        cCompoundContextCollisionPreprocess cccp;
        cHandle<cPathfindPreprocess> pp;
        cc->getPathfindingPreprocessesFor(qc, index, cccp, pp, cpp);
        pp->findPathAway(
                        qc,
                        startP, awayFromP, distanceAway,
                        cccp, *cpp,
                        pathPoints, connectionIndices,
                        queryCallBack
                        );
    }
    else
    {
        _pathfindPreprocess.get(index)->findPathAway(
                        qc,
                        startP, awayFromP, distanceAway,
                        pathPoints, connectionIndices,
                        queryCallBack
                        );
    }

    if(pathPoints.size() > 1
     && _pathfindPreprocess.get(index)->testPointCollision_SmallConvex(qc, pathPoints.back().f, pathPoints.back().p)
     )
    {
    // shorten the path to before the last collision

        tFace sectionStartF;
        tSigned32 beforeLast = SizeL(pathPoints) - 2;
        sectionStartF = pathPoints[beforeLast].f;
        tLine sectionLine(pathPoints[beforeLast].p, pathPoints.back().p);

        bool collides = false;
        cAxisFraction distanceAlong;

        LastCollision_AgainstShapeSet(
            *_pathfindPreprocess.get(index)->getSmallConvexOnOriginal(),
            sectionStartF, sectionLine, collides, distanceAlong
            );
        assertD(collides);

        tPoint p1, p2;
        bool exactlyOnLine;
//        ApproximatePointsAlongLine(sectionLine, static_cast<float>(distanceAlong.asDistance(sectionLine)), exactlyOnLine, p1, p2);
        {
            cPathSegmentInterpolator segmentInterpolator;
            segmentInterpolator.initOrReinit(sectionLine.start(), sectionLine.end());
            tSigned32 iv = distanceAlong.multiplyLongByThis(segmentInterpolator.interpolatorMax());
            segmentInterpolator.interpolate(sectionLine.start(), iv, exactlyOnLine, p1, p2);
        }

        cInternalPosition sectionStart(sectionStartF, sectionLine.start());
        tFace sectionEndF;
        if(exactlyOnLine)
        {
            sectionEndF = getFaceForEndOfLine(sectionStart, p1);
        }
        else
        {
            bool collides = testLineCollision_XY_Internal_ExceptSmallConvex(qc, index, cc, sectionStart, p1, sectionEndF);
            if(collides)
            {
                p1 = p2;
                collides = testLineCollision_XY_Internal_ExceptSmallConvex(qc, index, cc, sectionStart, p1, sectionEndF);
                if(collides)
                {
                    sectionEndF = tMesh::endFace();
                }
            }
        }

        if(sectionEndF.valid())
        {
        // successfully shortened the path
            pathPoints.back() = cInternalPosition(sectionEndF, p1);
        }
        else
        {
        // pathological situation - just remove the whole last section
            pathPoints.pop_back();
            if(connectionIndices)
            {
                connectionIndices->pop_back();
            }
        }
    }

    std::vector<cInternalPosition> pushedPath;
    std::vector<tSigned32>* pushedIndices;
    if(_pathfindPreprocess.get(index)->smallConvexWereSplit())
    {
        pushedIndices = 0;
        if(connectionIndices)
        {
            pushedIndices = new std::vector<tSigned32>;
        }
        if(context)
        {
            _pathfindPreprocess.get(index)->pushPathAroundSmallConvex(pathPoints, connectionIndices, pushedPath, pushedIndices, cpp->refObstaclesAndSmallConvexOverlapped());
        }
        else
        {
            _pathfindPreprocess.get(index)->pushPathAroundSmallConvex(pathPoints, connectionIndices, pushedPath, pushedIndices);
        }
        delete connectionIndices;
    }
    else
    {
        pathPoints.swap(pushedPath);
        pushedIndices = connectionIndices;
    }

    cPath* result;
    if(pushedPath.empty())
    {
        result = 0;
    }
    else
    {
        result = nSingletons::pathEngine().refPathPool().get();
        _heldPaths.push_front(result);
        result->attachToMesh(const_cast<cMesh*>(this), _heldPaths.begin());
        result->init(pushedPath, pushedIndices);
    }

    exitQuery(qc);
    return result;
}

iPath*
cMesh::generateCurvedPath(
        iShape* shape, 
        iPath* basePath, 
        iCollisionContext* context,
        tSigned32 startVectorX, tSigned32 startVectorY,
        tSigned32 sectionLength,
        float turnRatio1, float turnRatio2
        ) const
{
    return generateCurvedPath_WithEndVector(shape, basePath, context, startVectorX, startVectorY, 0, 0, sectionLength, turnRatio2, turnRatio2);
}

iPath*
cMesh::constructPath(const cPosition* positionsBuffer, const tSigned32* connectionIndicesBuffer, tSigned32 pathLength) const
{
    INTERFACE_ASSERT(pathLength > 0);
    cPath* result = nSingletons::pathEngine().refPathPool().get();
    _heldPaths.push_front(result);
    result->attachToMesh(const_cast<cMesh*>(this), _heldPaths.begin());
    result->init(positionsBuffer, connectionIndicesBuffer, pathLength);
    return result;
}
iPath*
cMesh::constructPath_Reversed(const cPosition* positionsBuffer, const tSigned32* connectionIndicesBuffer, tSigned32 pathLength) const
{
    INTERFACE_ASSERT(pathLength > 0);
    cPath* result = nSingletons::pathEngine().refPathPool().get();
    _heldPaths.push_front(result);
    result->attachToMesh(const_cast<cMesh*>(this), _heldPaths.begin());
    result->init_Reversed(positionsBuffer, connectionIndicesBuffer, pathLength);
    return result;
}

void
cMesh::savePath(iPath* path, iOutputStream* os) const
{
    if(path == 0)
    {
        return;
    }
    cMutexLock lock(_meshMutex);
    INTERFACE_ASSERT(path);
    cPath* cp = static_cast<cPath*>(path);

    makeCheckSumValid();

    cElementAndAttributeEnumerator enumerator;

    enumerator.openElement("path");
    enumerator.addAttribute("majorVersion", cPath::majorVersion());
    enumerator.addAttribute("minorVersion", cPath::minorVersion());
    enumerator.addAttribute("meshCheckSum", _checkSum);
    cp->save(enumerator);
    enumerator.closeElement("path");

    enumerator.writeTo(*os);

    cTokenisedXMLOutputStream xos(enumerator, *os);

    xos.openElement("path");
    xos.addAttribute("majorVersion", cPath::majorVersion());
    xos.addAttribute("minorVersion", cPath::minorVersion());
    xos.addAttribute("meshCheckSum", _checkSum);
    cp->save(xos);
    xos.closeElement("path");
}
iPath*
cMesh::loadPath(const char* dataBuffer, tUnsigned32 bufferSize) const
{
    if(bufferSize == 0)
    {
        return 0;
    }
    cMutexLock lock(_meshMutex);

    cTokenisedXMLInputStream is(dataBuffer, bufferSize);
    const char *const* attributes = is.openElement("path");
    tSigned32 majorVersion = ValueAsLong(attributes, "majorVersion");
    tSigned32 minorVersion = ValueAsLong(attributes, "minorVersion");

    {
        tSigned32 meshCheckSum = ValueAsLong(attributes, "meshCheckSum");
        makeCheckSumValid();
        if(meshCheckSum != _checkSum)
        {
            Error("Fatal", "cMesh::loadPath() - mesh checksum does not match saved checksum.");
            PathEngine_Abort();
        }
    }

    cPath* result = nSingletons::pathEngine().refPathPool().get();
    _heldPaths.push_front(result);
    result->attachToMesh(const_cast<cMesh*>(this), _heldPaths.begin());

    if(majorVersion != cPath::majorVersion() || minorVersion < cPath::minorVersion())
    {
        Error("Warning", "Saved path data version is incompatible. Returning failed path.");
        std::vector<cInternalPosition> points;
        result->init(points, 0);
    }
    else
    {
        result->load(is, attributes);
    }
    is.closeElement();
    return result;
}

void
cMesh::renderLineOnGround(const cPosition& start, tSigned32 endX, tSigned32 endY, tSigned32 originX, tSigned32 originY, iRender3DLinesCallBack& callBack) const
{
    INTERFACE_ASSERT(positionIsValid(start));
    INTERFACE_ASSERT(endX >= -WORLD_RANGE && endY <= WORLD_RANGE);
    INTERFACE_ASSERT(originX >= -WORLD_RANGE && originY <= WORLD_RANGE);
    if(endX == start.x && endY == start.y)
    {
        return;
    }
    tPoint startP(start.x, start.y, WORLD_RANGE);
    tPoint endP(endX, endY, WORLD_RANGE);
    tPoint origin(originX, originY, WORLD_RANGE);

    cInternalPosition startIP;
    convertPosition(start, startIP);
    tFace_3D f3D = _translation->translate(startIP.f, startIP.p);

    DrawLineOnGround(f3D, tLine(startP, endP), SIDE_RIGHT, origin, false, callBack);
}

iObstacleSet*
cMesh::newObstacleSet_WithAttributes(const char *const* attributes) const
{
    cObstacleSet* result = new cObstacleSet(const_cast<cMesh*>(this), attributes);
    cMutexLock lock(_heldObstacleSetsMutex);
    result->setIndexInMesh(_heldObstacleSets.reuseOrPushBack(result));
    return result;
}

cPosition
cMesh::positionInSectionInShape(tSigned32 sectionID, tSigned32 shapeOriginX, tSigned32 shapeOriginY, tSigned32 numberOfPoints, const tSigned32* coordinateData) const
{
    cMutexLock lock(_meshMutex);
    INTERFACE_ASSERT(nSingletons::pathEngine().shapeIsValid(numberOfPoints, coordinateData));
    if(sectionID != -1 && !_sectionIDs)
    {
        return cPosition();
    }
    cPosition result = positionInSection(sectionID, shapeOriginX, shapeOriginY);
    if(result.cell != -1)
    {
        return result;
    }
    tPoint origin(shapeOriginX, shapeOriginY, WORLD_RANGE);

    tPoint* pointArray = new tPoint[numberOfPoints];
    tSigned32 i;
    for(i = 0; i != numberOfPoints; ++i)
    {
        pointArray[i] = tPoint(coordinateData[i * 2], coordinateData[i * 2 + 1], SHAPE_RANGE);
    }
    cConvexPoly cs(numberOfPoints, pointArray);

    tPoint bottomLeft, topRight;
    cs.getBounds(bottomLeft, topRight);
    bottomLeft += origin;
    topRight += origin;
    {
        tSigned32 faces = _partitioning->queryFacesPotentiallyOverlapped(
                        bottomLeft, topRight, &_partitioningResultsBuffer.front());
        for(i = 0; i < faces; i++)
        {
            tSigned32 f3DIndex = _partitioningResultsBuffer[i];
            if(sectionID != -1 && (*_sectionIDs)[f3DIndex] != sectionID)
            {
                continue;
            }
            tFace_3D f3D = _mesh3D->faceAt(f3DIndex);
            assertD(!f3D->isExternal());
            tEdge_3D e3D = tMesh_3D::beginEdge(f3D);
            do
            {
                tPoint p;
                if(GetPointInsideShapeEntry(e3D, origin, cs, p))
                {
                    tFace f2D = _translation->translate(f3D, p);
                    cPosition result;
                    result.x = p.getX();
                    result.y = p.getY();
                    result.cell = f2D.index();
                    return result;
                }
            }
            while(e3D.toNextInFace());
        }
    }
    return cPosition();
}

iPath*
cMesh::generateCurvedPath_WithEndVector(
        iShape* shape, 
        iPath* basePath, 
        iCollisionContext* context,
        tSigned32 startVectorX, tSigned32 startVectorY,
        tSigned32 endVectorX, tSigned32 endVectorY,
        tSigned32 sectionLength,
        float turnRatio1, float turnRatio2
        ) const
{
    if(basePath == 0)
    {
        return 0;
    }
    cPath* _path = FromI(basePath);
    cShape* _shape = FromI(shape);
    cCollisionContext* _context = FromI(context);
    cPath* result;
    {
        cMutexLock lock(_meshMutex);
        result = nSingletons::pathEngine().refPathPool().get();
        registerPath(result);
    }
    _path->curved(
        _shape,_context,
        startVectorX, startVectorY,
        endVectorX, endVectorY,
        sectionLength,
        turnRatio1, turnRatio2,
        result
        );
    return result;
}

iAgent*
cMesh::placeLargeStaticObstacle(tSigned32 numberOfPoints, const tSigned32* coordinateData, const cPosition& position) const
{
    INTERFACE_ASSERT(positionIsValid(position));
    INTERFACE_ASSERT(nSingletons::pathEngine().largeStaticObstacleShapeIsValid(numberOfPoints, coordinateData, position.x, position.y));
    tPoint* pointArray = new tPoint[numberOfPoints];
    tSigned32 i;
    for(i = 0; i != numberOfPoints; ++i)
    {
        pointArray[i] = tPoint(coordinateData[i * 2], coordinateData[i * 2 + 1], WORLD_RANGE);
    }
    cConvexPoly placedShape(numberOfPoints, pointArray);
    return placeLargeStaticObstacle_Internal(placedShape, position);
}

void
cMesh::addAnchorsAndShapes(const iAnchorsAndPinnedShapes* anchorsAndShapes, const char* idPrefix, tSigned32 numberOfTerrainLayers)
{
    INTERFACE_ASSERT(anchorsAndShapes);
    INTERFACE_ASSERT(numberOfTerrainLayers >= 0);
    AddAnchorsAndPinnedShapes(nSingletons::pathEngine(), this, *anchorsAndShapes, idPrefix, numberOfTerrainLayers);
}

//====================== not provided through iMesh

//iPath*
//cMesh::findShortestPath_Incremental(
//        iShape* shape, 
//        iCollisionContext* context, 
//        const cPosition& start, const cPosition& end
//        )
//{
//	INTERFACE_ASSERT(!testPointCollision(shape, context, start));
//	if(testPointCollision(shape, context, end))
//    {
//        std::vector<cInternalPosition> pathVector;
//        cPath* result;
//        {
//            cMutexLock lock(_meshMutex);
//            result = nSingletons::pathEngine().refPathPool().get();
//            _heldPaths.push_front(result);
//            result->attachToMesh(this, _heldPaths.begin());
//            result->init(pathVector);
//        }
//        return result;
//    }
//    if(start == end)
//    {
//        return findShortestPath(shape, 0, start, end);
//    }
//    iCollisionContext* pathfindContext = newContext();
//    while(1)
//    {
//        iPath* path = findShortestPath(shape, pathfindContext, start, end);
//        if(path->size() == 0)
//        {
//            return path;
//        }
//        const cPosition* positions = path->getPositionArray();
//        tSigned32 i;
//        for(i = 0; i + 1 != path->size(); ++i)
//        {
//            tSigned32 cell;
//            iCollisionInfo* info = firstCollision(
//                    shape, context,
//                    positions[i],
//                    positions[i + 1].x, positions[i + 1].y,
//                    cell
//                    );
//            if(info)
//            {
//                iAgent* agent = info->getCollidingAgent();
//                assertD(agent);
//                pathfindContext->addAgent(agent);
//                delete info;
//                break;
//            }
//            assertD(cell == positions[i + 1].cell);
//        }
//        if(i + 1 == path->size())
//        {
//            return path;
//        }
//        delete path;
//    }
//}

cPosition
cMesh::findClosestUnobstructedPoint_Reference(
                iShape* shape, 
                iCollisionContext* context,
                const cPosition& position,
                tSigned32 maximumDistance)
{
    cMutexLock lock(_meshMutex);
    INTERFACE_ASSERT(maximumDistance > 0);
	INTERFACE_ASSERT(positionIsValid(position));
    INTERFACE_ASSERT(shapeCanCollide(shape));

    cInternalPosition centreIP;
    convertPosition(position, centreIP);

    cShape* cs = static_cast<cShape*>(shape);
    tSigned32 index = cs->getPreprocessIndex();

    cCollisionContext *cc=static_cast<cCollisionContext*>(context);
    INTERFACE_ASSERT(&cc->refMesh() == this);

    cQueryContext& qc = enterQuery();

    cInternalPosition resultIP;
    if(context)
    {
        cCompoundContextCollisionPreprocess cccp;
        cc->addCollisionPreprocessFor(qc, index, cccp);
//        cHandle<cContextCollisionPreprocess> ccp = cc->getCollisionPreprocessFor(qc, index);
        resultIP = nClosestUnobstructedPoint::find_Reference(
                                    qc,
                                    centreIP, maximumDistance,
//                                    _collisionPreprocess.get(index)->refEdgeExpansionLookup(),
                                    *_collisionPreprocess.get(index)->getEdgeExpansion(),
                                    _collisionPreprocess.get(index)->refBaseObstacleExpansions(),
                                    &cccp
                                    );
    }
    else
    {
        resultIP = nClosestUnobstructedPoint::find_Reference(
                                    qc,
                                    centreIP, maximumDistance,
//                                    _collisionPreprocess.get(index)->refEdgeExpansionLookup(),
                                    *_collisionPreprocess.get(index)->getEdgeExpansion(),
                                    _collisionPreprocess.get(index)->refBaseObstacleExpansions(),
                                    0
                                    );
    }
    
    exitQuery(qc);

    cPosition result;
    convertPosition(resultIP, result);
    return result;
}


//void
//cMesh::extendPathEnd(cPath& path, cShape& shape, cCollisionContext* context, const cInternalPosition& newEndPosition)
//{
//    if(path.getNumberOfPositions() == 0)
//    {
//        return;
//    }
//    cInternalPosition oldEndPosition(path.getPositionInPath_Internal(path.getNumberOfPositions() - 1));
//    if(newEndPosition == oldEndPosition)
//    {
//        return;
//    }
//
//    tSigned32 shapeIndex = shape.getPreprocessIndex();
//
//#ifdef INTERFACE_CHECKING
//    {
//        bool collides = testLineCollision_Internal(shapeIndex, context, oldEndPosition, newEndPosition);
//        INTERFACE_ASSERT(!collides);
//    }
//#endif
//
//    if(path.getNumberOfPositions() == 1)
//    {
//    // path with single point indicates agent has reached end of path or no movement was required
//    // just add the new point at end to create a new single section path
//        path.append(newEndPosition);
//        return;
//    }
//    tSigned32 unwindTo = path.getIndexForEndUnwind(newEndPosition.p);
//
//#ifdef INTERFACE_CHECKING
//    {
//        tSigned32 i;
//        for(i = unwindTo; i + 1 < path.getNumberOfPositions(); ++i)
//        {
//            bool collides = testLineCollision_Internal(shapeIndex, context, path.getPositionInPath_Internal(i), path.getPositionInPath_Internal(i + 1));
//            INTERFACE_ASSERT(!collides);
//        }
//    }    
//#endif    
//
//    cInternalPosition unwindPointPosition(path.getPositionInPath_Internal(unwindTo));
//    eSide unwindDirection;
//    {
//        tLine lineAfterUnwindPoint(unwindPointPosition.p, path.getPositionInPath_Internal(unwindTo + 1).p);
//        unwindDirection = SideOfLine(lineAfterUnwindPoint, newEndPosition.p);
//    }
//    if(unwindDirection == SIDE_CENTRE)
//    {
//        unwindDirection = SIDE_LEFT; // chosen arbitrarily
//    }
//
//    std::list<std::pair<tFace, tPoint> > pathToAppend;
//    pathToAppend.push_back(std::pair<tFace, tPoint>(unwindPointPosition.f, unwindPointPosition.p));
//    pathToAppend.push_back(std::pair<tFace, tPoint>(newEndPosition.f, newEndPosition.p));
//
//    eSide pushDirection = OtherSide(unwindDirection);
//
//    if(context)
//    {
//        cContextPathfindPreprocess& cp = context->getPathfindPreprocessFor(shapeIndex);
//        _collisionPreprocess.get(shapeIndex)->pushPathSectionAroundAll(pathToAppend, pathToAppend.begin(),
//                                                                   pushDirection,
//                                                                   cp.refObstaclesAndSmallConvexOverlappedTester());
//    }
//    else
//    {
//        _collisionPreprocess.get(shapeIndex)->pushPathSectionAroundAll(pathToAppend, pathToAppend.begin(), pushDirection);
//    }
//
//    path.replaceEnd(unwindTo + 1, pathToAppend);
//}
//
//void
//cMesh::extendPathStart(cPath& path, cShape& shape, cCollisionContext* context, const cInternalPosition& newStartPosition)
//{
//    if(path.getNumberOfPositions() == 0)
//    {
//        return;
//    }
//    cInternalPosition oldStartPosition(path.getPositionInPath_Internal(0));
//    if(newStartPosition == oldStartPosition)
//    {
//        return;
//    }
//
//    tSigned32 shapeIndex = shape.getPreprocessIndex();
//
//#ifdef INTERFACE_CHECKING
//    {
//        bool collides = testLineCollision_Internal(shapeIndex, context, oldStartPosition, newStartPosition);
//        INTERFACE_ASSERT(!collides);
//    }
//#endif
//
//    if(path.getNumberOfPositions() == 1)
//    {
//    // path with single point indicates agent has reached end of path or no movement was required
//    // just add the new point at start to create a new single section path
//        std::vector<cInternalPosition> newPath(2);
//        newPath[0] = newStartPosition;
//        newPath[1] = oldStartPosition;
//        path.init(newPath);
//        return;
//    }
//    tSigned32 unwindTo = path.getIndexForStartUnwind(newStartPosition.p);
//
//#ifdef INTERFACE_CHECKING
//    {
//        tSigned32 i;
//        for(i = 0; i + 1 <= unwindTo; ++i)
//        {
//            bool collides = testLineCollision_Internal(shapeIndex, context, path.getPositionInPath_Internal(i), path.getPositionInPath_Internal(i + 1));
//            INTERFACE_ASSERT(!collides);
//        }
//    }    
//#endif    
//
//    cInternalPosition unwindPointPosition(path.getPositionInPath_Internal(unwindTo));
//    eSide unwindDirection;
//    {
//        tLine lineAfterUnwindPoint(unwindPointPosition.p, path.getPositionInPath_Internal(unwindTo - 1).p);
//        unwindDirection = SideOfLine(lineAfterUnwindPoint, newStartPosition.p);
//    }
//    if(unwindDirection == SIDE_CENTRE)
//    {
//        unwindDirection = SIDE_LEFT; // chosen arbitrarily
//    }
//
//    std::list<std::pair<tFace, tPoint> > pathToPrepend;
//    pathToPrepend.push_back(std::pair<tFace, tPoint>(newStartPosition.f, newStartPosition.p));
//    pathToPrepend.push_back(std::pair<tFace, tPoint>(unwindPointPosition.f, unwindPointPosition.p));
//
//    eSide pushDirection = unwindDirection;
//
//    if(context)
//    {
//        cContextPathfindPreprocess& cp = context->getPathfindPreprocessFor(shapeIndex);
//        _collisionPreprocess.get(shapeIndex)->pushPathSectionAroundAll(pathToPrepend, pathToPrepend.begin(),
//                                                                   pushDirection,
//                                                                   cp.refObstaclesAndSmallConvexOverlappedTester());
//    }
//    else
//    {
//        _collisionPreprocess.get(shapeIndex)->pushPathSectionAroundAll(pathToPrepend, pathToPrepend.begin(), pushDirection);
//    }
//
//    path.replaceStart(unwindTo - 1, pathToPrepend);
//}


//====================== internal

iCollisionInfo*
cMesh::firstCollision_Internal(
            iShape* shape, 
            iCollisionContext* context, 
            tFace startF,
            tLine testLine, const cAxisFraction& startFraction, const cAxisFraction& endFraction,
            tFace& endF
            ) const
{
    assertD(testLine.start() != testLine.end());
    cShape* cs = static_cast<cShape*>(shape);
    tSigned32 index = cs->getPreprocessIndex();

    bool collided = false;
    cAxisFraction distanceAlong;
    tLine collidingLine;

    cQueryContext& qc = enterQuery();

    //..... optimise - when a collision has already been found, dont need to test against both endFraction and distanceAlong

    {
        cHandle<cCollisionPreprocess> cp = getCollisionPreprocessFor(index);
        cp->firstCollision(qc, startF, testLine, startFraction, endFraction, collided, distanceAlong, collidingLine);
    }

    iAgent* collidingAgent = 0;
    
    if(context)
    {
        cCollisionContext& cc = static_cast<cCollisionContext&>(*context);

        cMutexLock lock(_agentsAndObstacleSetsMutex);

        cCompoundContextCollisionPreprocess cccp;
        cc.addCollisionPreprocessFor(qc, index, cccp);
        cAgent* collidingAgentC = 0;
        cccp.firstCollision(startF, testLine, startFraction, endFraction, collided, distanceAlong, collidingLine, collidingAgentC);
        if(collidingAgentC)
        {
            collidingAgent = collidingAgentC;
        }
    }

    exitQuery(qc);

    if(collided)
    {
        cMutexLock lock(_meshMutex);
        _heldCollisionInfos.push_front(0);
        cCollisionInfo* ci = nSingletons::pathEngine().refCollisionInfoPool().get();
        ci->init(const_cast<cMesh*>(this), collidingLine, collidingAgent, _heldCollisionInfos.begin());
        _heldCollisionInfos.front() = ci;
        return ci;
    }

// get the face at endFraction along

    tIntersection endIntersection(testLine, endFraction.n, endFraction.d);
    cIntersection_PointLike target(endIntersection);
    tEdge crossed;
    endF = startF;
    while(TraverseTowardsTarget_SI<tMesh>(endF, testLine, target, crossed))
    {
    }

    return 0;
}

bool
cMesh::testPointCollision_IgnoreOneAgent(
                iShape* shape, 
                iCollisionContext* context, 
                const cPosition& position,
                const cAgent* agentToIgnore
                ) const
{
    cShape* cs = static_cast<cShape*>(shape);
    cInternalPosition p;
    convertPosition(position, p);

    if(shapeCanCollide(shape))
    {
        tSigned32 index = cs->getPreprocessIndex();
        cHandle<cCollisionPreprocess> cp = getCollisionPreprocessFor(index);

        cQueryContext& qc = enterQuery();
        bool result = cp->refPointTester_All().testPointCollision(qc, p.f, p.p);
        if(!result && context)
        {
            cCollisionContext& cc = static_cast<cCollisionContext&>(*context);
            INTERFACE_ASSERT(&cc.refMesh() == this);
//            cHandle<cContextCollisionPreprocess> cp = cc.getCollisionPreprocessFor(qc, index);
            cCompoundContextCollisionPreprocess cccp;
            cc.addCollisionPreprocessFor(qc, index, cccp);
            exitQuery(qc);
            result = cccp.testPointCollision_IgnoringOneAgent(p, agentToIgnore);
//            result = PointCollision_AgainstShapeSet_IgnoringOneAgent(cp->refShapeSet(), p, agentToIgnore);
        }
        else
        {
            exitQuery(qc);
        }
        return result;
    }

    INTERFACE_ASSERT(_allowCollisionWithoutPreprocess);

//    cHandle<cContextCollisionPreprocess> ccp;
    if(context)
    {
        cCollisionContext& cc = static_cast<cCollisionContext&>(*context);
        INTERFACE_ASSERT(&cc.refMesh() == this);
        cQueryContext& qc = enterQuery();
//        ccp = cc.getUnexpandedPreprocess(qc);
        cCompoundContextCollisionPreprocess cccp;
        cc.addUnexpandedPreprocess(qc, cccp);
        cMutexLock lock(_meshMutex);
        bool result = TestPointCollision_NoPreprocess(qc, cs->refCS(), _unexpandedBaseObstacles, &cccp, p, agentToIgnore);
        exitQuery(qc);
        return result;
    }
    else
    {
        cMutexLock lock(_meshMutex);
        cQueryContext& qc = enterQuery();
        bool result = TestPointCollision_NoPreprocess(qc, cs->refCS(), _unexpandedBaseObstacles, 0, p, agentToIgnore);
        exitQuery(qc);
        return result;
    }
}

// if preprocess is present then this doesnt actually ignore the agent currently
// this doesnt matter for the case where the agent is at the start of the line
bool
cMesh::testLineCollision_IgnoreOneAgent(
            iShape* shape, 
            iCollisionContext* context, 
            const cPosition& start, const cPosition& end,
            const cAgent* agentToIgnore
            ) const
{
    cShape* cs = static_cast<cShape*>(shape);
    cInternalPosition startP, endP;
    convertPosition(start, startP);
    convertPosition(end, endP);

    if(shapeCanCollide(shape))
    {
        tSigned32 index = cs->getPreprocessIndex();
        cCollisionContext* cc = static_cast<cCollisionContext*>(context);
        cQueryContext& qc = enterQuery();
        bool result = testLineCollision_Internal(qc, index, cc, startP, endP);
        exitQuery(qc);
        return result;
    }

    INTERFACE_ASSERT(_allowCollisionWithoutPreprocess);

//    cHandle<cContextCollisionPreprocess> ccp;
    if(context)
    {
        cCollisionContext& cc = static_cast<cCollisionContext&>(*context);
        INTERFACE_ASSERT(&cc.refMesh() == this);
        cQueryContext& qc = enterQuery();
//        ccp = cc.getUnexpandedPreprocess(qc);
        cCompoundContextCollisionPreprocess cccp;
        cc.addUnexpandedPreprocess(qc, cccp);
        cMutexLock lock(_meshMutex);
        bool result = TestLineCollision_NoPreprocess(
                qc, cs->refCS(),
                _unexpandedBaseObstacles, &cccp,
                startP, endP,
                agentToIgnore
                );
        exitQuery(qc);
        return result;
    }
    else
    {
        cMutexLock lock(_meshMutex);
        cQueryContext& qc = enterQuery();
        bool result = TestLineCollision_NoPreprocess(
                qc, cs->refCS(),
                _unexpandedBaseObstacles, 0,
                startP, endP,
                agentToIgnore
                );
        exitQuery(qc);
        return result;
    }
}

// if preprocess is present then this doesnt actually ignore the agent currently
// this doesnt matter for the case where the agent is at the start of the line
bool
cMesh::testLineCollision_XY_IgnoreOneAgent(
            iShape* shape, 
            iCollisionContext* context, 
            const cPosition& start, tSigned32 x, tSigned32 y, tSigned32& cell,
            const cAgent* agentToIgnore
            ) const
{
    if(x == start.x && y == start.y)
    {
        cell = start.cell;
        return false;
    }
    cShape* cs = static_cast<cShape*>(shape);
    cInternalPosition startP;
    convertPosition(start, startP);
    cCollisionContext* cc = static_cast<cCollisionContext*>(context);
    tFace endF;

    if(shapeCanCollide(shape))
    {
        tSigned32 index = cs->getPreprocessIndex();
        cQueryContext& qc = enterQuery();
        bool result = testLineCollision_XY_Internal(qc, index, cc, startP, tPoint(x, y, WORLD_RANGE), endF);
        exitQuery(qc);
        if(!result)
        {
            cell = endF.index();
        }
        return result;
    }

    INTERFACE_ASSERT(_allowCollisionWithoutPreprocess);

//    cHandle<cContextCollisionPreprocess> ccp;
    if(context)
    {
        cCollisionContext& cc = static_cast<cCollisionContext&>(*context);
        INTERFACE_ASSERT(&cc.refMesh() == this);
        cQueryContext& qc = enterQuery();
//        ccp = cc.getUnexpandedPreprocess(qc);
        cCompoundContextCollisionPreprocess cccp;
        cc.addUnexpandedPreprocess(qc, cccp);
        cMutexLock lock(_meshMutex);
        bool result = TestLineCollision_XY_NoPreprocess(
                qc, cs->refCS(),
                _unexpandedBaseObstacles, &cccp,
                startP, tPoint(x, y, WORLD_RANGE), endF,
                agentToIgnore
                );
        exitQuery(qc);
        if(!result)
        {
            cell = endF.index();
        }
        return result;
    }
    else
    {
        cMutexLock lock(_meshMutex);
        cQueryContext& qc = enterQuery();
        bool result = TestLineCollision_XY_NoPreprocess(
                qc, cs->refCS(),
                _unexpandedBaseObstacles, 0,
                startP, tPoint(x, y, WORLD_RANGE), endF,
                agentToIgnore
                );
        exitQuery(qc);
        if(!result)
        {
            cell = endF.index();
        }
        return result;
    }
}


void
cMesh::lockAgentsAndObstacleSets() const
{
    _agentsAndObstacleSetsMutex.lock();
}
void
cMesh::unlockAgentsAndObstacleSets() const
{
    _agentsAndObstacleSetsMutex.unlock();
}

cQueryContext&
cMesh::enterQuery() const
{
    cMutexLock lock(_queryContextsMutex);
    tSigned32 i;
    for(i = 0; i < SizeL(_queryContexts); ++i)
    {
        if(!_queryContexts[i]->insideQuery())
        {
            break;
        }
    }
    if(i == _queryContexts.size())
    {
        _queryContexts.push_back(new cQueryContext(ref2D(), _mesh2DInternalFaces, _hasInternalVertices, nSingletons::pathEngine().refHeapPool()));
    }
    _queryContexts[i]->addElementCountRequirement(_maxElementCountRequirement);
    _queryContexts[i]->enter();
    return *_queryContexts[i];
}
void
cMesh::exitQuery(cQueryContext& qc) const
{
    cMutexLock lock(_queryContextsMutex);
    tSigned32 i;
    for(i = 0; i < SizeL(_queryContexts); ++i)
    {
        if(_queryContexts[i] == &qc)
        {
            break;
        }
    }
    assertR(i < SizeL(_queryContexts));
    _queryContexts[i]->exit();
}

void
cMesh::notifyContextDestroyed(tSigned32 index)
{
    cMutexLock lock(_heldContextsMutex);
    _heldContexts.release(index);
}
void
cMesh::notifyAgentDestroyed(tSigned32 index)
{
    cMutexLock lock(_heldAgentsMutex);
    _heldAgents.release(index);
}
void
cMesh::notifyObstacleSetDestroyed(tSigned32 index)
{
    cMutexLock lock(_heldObstacleSetsMutex);
    _heldObstacleSets[index] = 0;
    _heldObstacleSets.release(index);
}

bool
cMesh::shapeCanCollide(const cShape* cs) const
{
    tSigned32 index = cs->getPreprocessIndex();
    if(index==-1)
        return false;
    return _collisionPreprocess.get(index).isSet();
}
bool
cMesh::shapeCanPathfind(const cShape* cs) const
{
    tSigned32 index = cs->getPreprocessIndex();
    if(index==-1)
        return false;
    return _pathfindPreprocess.get(index).isSet();
}
bool
cMesh::shapeCanCollide(tSigned32 index) const
{
    assertD(index != -1);
    return _collisionPreprocess.get(index).isSet();
}
bool
cMesh::shapeCanPathfind(tSigned32 index) const
{
    assertD(index != -1);
    return _pathfindPreprocess.get(index).isSet();
}

void
cMesh::convertPosition(const cPosition& p, cInternalPosition& result) const
{
    if(p.cell >= 0)
    {
        result = cInternalPosition(ref2D().faceAt(p.cell), tPoint(p.x, p.y, WORLD_RANGE));   
    }
    else
    {
        result.f.clear();
    }
}
void
cMesh::convertPosition(const cInternalPosition& p, cPosition& result) const
{
    if(p.f.valid())
    {
        result.x = p.p.getX();
        result.y = p.p.getY();
        result.cell = p.cell();
    }
    else
    {
        result.cell = -1;
    }
}

tEdge cMesh::lookupEdge(tSigned32 index) const
{
    return ref2D().edgeAt(index);
}

const tMeshCutSequence&
cMesh::refEdgeExpansion(cShape* shape) const
{
    return _collisionPreprocess.get(shape->getPreprocessIndex())->refEdgeExpansion();
}

//const tMeshCircuits&
//cMesh::refSmallConvexExpansion(cShape* shape) const
//{
//    return _collisionPreprocess.get(shape->getPreprocessIndex())->refSmallConvexOnOriginalMesh_NotThreadSafe();
//}

tSigned32 cMesh::size() const
{
    return ref2D().faces();
}

cHandle<cCollisionPreprocess>
cMesh::getCollisionPreprocessFor(tSigned32 index) const
{
    cMutexLock lock(_meshMutex);
    assertD(index < _collisionPreprocess.size());
    assertD(_collisionPreprocess.get(index).isSet());
    return _collisionPreprocess.get(index);
}
cHandle<cPathfindPreprocess>
cMesh::getPathfindPreprocessFor(tSigned32 index) const
{
    cMutexLock lock(_meshMutex);
    assertD(index < _pathfindPreprocess.size());
    assertD(_pathfindPreprocess.get(index).isSet());
    return _pathfindPreprocess.get(index);
}

const cMeshWithTrackedTransform<tMesh>*
cMesh::getSilhouetteRegionPartitioned(const cShape& shape) const
{
    tSigned32 index = shape.getPreprocessIndex();
    return _pathfindPreprocess.get(index)->getPartitioned();
}
const cMeshWithTrackedTransform<tMesh>&
cMesh::refCircuitCombinationPartitioned(const cShape& shape)
{
    tSigned32 index = shape.getPreprocessIndex();
    return _pathfindPreprocess.get(index)->refCircuitCombinationPartitioned();
}
const cMeshWithTrackedTransform<tMesh>&
cMesh::refSmallConvexPartitioned(const cShape& shape) const
{
    tSigned32 index = shape.getPreprocessIndex();
    return *_pathfindPreprocess.get(index)->getSmallConvexPartitioned();
}

void
cMesh::registerPath(cPath* path) const
{
    cMutexLock lock(_meshMutex);
    _heldPaths.push_front(path);
    path->attachToMesh(const_cast<cMesh*>(this), _heldPaths.begin());
}
void
cMesh::detachPath(cList_NoSize<cPath*>::iterator heldNode) const
{
    cMutexLock lock(_meshMutex);
    heldNode.erase();
}
void
cMesh::detachCollisionInfo(cList_NoSize<cCollisionInfo*>::iterator heldNode) const
{
    cMutexLock lock(_meshMutex);
    heldNode.erase();
}


//bool
//cMesh::testPointCollision_Internal(
//                cQueryContext& qc,
//                iShape* shape, 
//                iCollisionContext* context, 
//                const cPosition &position,
//                tSigned32 agentToIgnore,
//                tSigned32& collidingAgent
//                )
//{
//    cShape* cs = static_cast<cShape*>(shape);
//    tSigned32 index = cs->getPreprocessIndex();
//    cInternalPosition p;
//    convertPosition(position, p);
//    if(_collisionPreprocess.get(index)->testPointCollision(p.f, p.p))
//    {
//        collidingAgent = -1;
//        return true;
//    }
//    if(_collisionPreprocess.get(index)->testPointCollision_SmallConvex(p.f, p.p))
//    {
//        collidingAgent = -1;
//        return true;
//    }
//
//    if(context)
//    {
//        cCollisionContext &cc=static_cast<cCollisionContext&>(*context);
//        cLocalisedConvexShapeSet &cp=cc.getCollisionPreprocessFor(qc, index);
//        if(agentToIgnore != -1)
//        {
//            return cp.testPointCollision_IgnoringOneAgent(p, agentToIgnore, collidingAgent);
//        }
//        else
//        {
//            return cp.testPointCollision(p, collidingAgent);
//        }
//    }
//    return false;
//}

bool
cMesh::testLineCollision_Internal(cQueryContext& qc, tSigned32 shapeIndex, 
                                    cCollisionContext* context, 
                                    const cInternalPosition& start, const cInternalPosition& end) const
{
    PROFILER_SCOPE("cMesh::testLineCollision_Internal");
    if(_collisionPreprocess.get(shapeIndex)->refLineTester_All().testCollision(qc, start, end))
    {
        return true;
    }
    if(context)
    {
        INTERFACE_ASSERT(&context->refMesh() == this);
//        cHandle<cContextCollisionPreprocess> ccp = context->getCollisionPreprocessFor(qc, shapeIndex);
        cCompoundContextCollisionPreprocess cccp;
        context->addCollisionPreprocessFor(qc, shapeIndex, cccp);
        if(cccp.testCollision(qc, start, end))
        {
            return true;
        }
    }
    return false;
}

bool
cMesh::testLineCollision_Internal_ExceptSmallConvex(cQueryContext& qc, tSigned32 shapeIndex, 
                                    cCollisionContext* context, 
                                    const cInternalPosition& start, const cInternalPosition& end) const
{
    if(_pathfindPreprocess.get(shapeIndex)->refLineTester_NonSmallConvex().testCollision(qc, start, end))
    {
        return true;
    }
    if(context)
    {
        INTERFACE_ASSERT(&context->refMesh() == this);
//        cHandle<cContextCollisionPreprocess> ccp = context->getCollisionPreprocessFor(qc, shapeIndex);
        cCompoundContextCollisionPreprocess cccp;
        context->addCollisionPreprocessFor(qc, shapeIndex, cccp);
        if(cccp.testCollision(qc, start, end))
        {
            return true;
        }
    }
    return false;
}

bool
cMesh::testLineCollision_XY_Internal(
                cQueryContext& qc,
                tSigned32 shapeIndex,
                cCollisionContext* context,
                const cInternalPosition& start, const tPoint& endP,
                tFace& endF) const
{
    endF = getFaceForEndOfLine(start, endP);
    if(!endF.valid())
    {
        return true;
    }
    cInternalPosition end(endF, endP);
    if(testLineCollision_Internal(qc, shapeIndex, context, start, end))
    {
        return true;
    }
    return false;
}
bool
cMesh::testLineCollision_XY_Internal_ExceptSmallConvex(
                cQueryContext& qc,
                tSigned32 shapeIndex,
                cCollisionContext* context,
                const cInternalPosition& start, const tPoint& endP,
                tFace& endF) const
{
    endF = getFaceForEndOfLine(start, endP);
    if(!endF.valid())
    {
        return true;
    }
    cInternalPosition end(endF, endP);
    if(testLineCollision_Internal_ExceptSmallConvex(qc, shapeIndex, context, start, end))
    {
        return true;
    }
    return false;
}

bool
cMesh::testForSnagsToSegmentEnd(
            cQueryContext& qc,
            tSigned32 shapeIndex,
            cCollisionContext* context,
            const tLine& segmentLine,
            const cInternalPosition& approximation
            ) const
{
    if(_pathfindPreprocess.get(shapeIndex)->refLineTester_NonSmallConvex().testForSnagsToSegmentEnd(qc, segmentLine, approximation))
    {
        return true;
    }
    if(context)
    {
        assertD(&context->refMesh() == this);
        cCompoundContextCollisionPreprocess cccp;
        context->addCollisionPreprocessFor(qc, shapeIndex, cccp);
        if(cccp.testForSnagsToSegmentEnd(qc, segmentLine, approximation))
        {
            return true;
        }
    }
    return false;
}

bool
cMesh::testForSnagsToSegmentEnd(
        iShape* shape,
        iCollisionContext* context,
        const tLine& segmentLine,
        const cPosition& approximation
        ) const
{
    tSigned32 shapeIndex = FromI(shape)->getPreprocessIndex();
    cInternalPosition ip;
    convertPosition(approximation, ip);
    cQueryContext& qc = enterQuery();
    bool result = testForSnagsToSegmentEnd(
            qc, shapeIndex,
            FromI(context),
            segmentLine,
            ip
            );
    exitQuery(qc);
    return result;
}

void
cMesh::findShortestPath_Internal(
        cQueryContext& qc,
        cShape& shape,
        cCollisionContext* context,
        const cInternalPosition& start,
        const cInternalPosition& goal,
        std::vector<cInternalPosition>& resultPath,
        std::vector<tSigned32>*& resultIndices,
        iQueryCallBack* queryCallBack
        ) const
{
    PROFILER_SCOPE("cMesh::findShortestPath_Internal");
    tSigned32 shapeIndex = shape.getPreprocessIndex();

    cCompoundContextCollisionPreprocess cccp;
    cHandle<cPathfindPreprocess> pp;
    cHandle<cContextPathfindPreprocess> cpp;

    if(context)
    {
        INTERFACE_ASSERT(&context->refMesh() == this);
        context->getPathfindingPreprocessesFor(qc, shapeIndex, cccp, pp, cpp);
    }
    else
    {
        pp = _pathfindPreprocess.get(shapeIndex);
    }

    if(pp->smallConvexWereSplit())
    {
        std::vector<cInternalPosition> beforePath;
        std::vector<tSigned32>* beforeIndices = 0;
        if(context)
        {
            pp->findShortestPath(qc, start, goal, cccp, *cpp, beforePath, beforeIndices, queryCallBack);
        }
        else
        {  
            pp->findShortestPath(qc, start, goal, beforePath, beforeIndices, queryCallBack);
        }

        if(beforePath.size() > 1 && 
            pp->testPointCollision_SmallConvex(qc, beforePath.back().f, beforePath.back().p)
            )
        {
            beforePath.clear();
            if(beforeIndices)
            {
                delete beforeIndices;
                beforeIndices = 0;
            }
        }

        resultIndices = 0;
        if(beforeIndices)
        {
            resultIndices = new std::vector<tSigned32>;
        }
        if(context)
        {
            pp->pushPathAroundSmallConvex(beforePath, beforeIndices, resultPath, resultIndices, cpp->refObstaclesAndSmallConvexOverlapped());
        }
        else
        {
            pp->pushPathAroundSmallConvex(beforePath, beforeIndices, resultPath, resultIndices);
        }
        delete beforeIndices;
    }
    else
    {
        if(context)
        {
            INTERFACE_ASSERT(&context->refMesh() == this);
            pp->findShortestPath(
                        qc,
                        start, goal,
                        cccp, *cpp,
                        resultPath,
                        resultIndices,
                        queryCallBack
                        );
        }
        else
        {
            pp->findShortestPath(qc, start, goal, resultPath, resultIndices, queryCallBack);
        }
    }
}

cCollisionInfo*
cMesh::newCollisionInfo(iAgent* collidingAgent) const
{
    cMutexLock lock(_meshMutex);
    _heldCollisionInfos.push_front(0);
    cCollisionInfo* info = nSingletons::pathEngine().refCollisionInfoPool().get();
    info->init(const_cast<cMesh*>(this), collidingAgent, _heldCollisionInfos.begin());
    _heldCollisionInfos.front() = info;
    return info;
}

float
cMesh::heightAtPointIn3DFace(tSigned32 f3DIndex, const tPoint& p) const
{
    if(_sectionIDs)
    {
        tSigned32 terrainLayer = (*_sectionIDs)[f3DIndex];
        if(terrainLayer >= 0 && terrainLayer < SizeL(_terrainCallBacks) && _terrainCallBacks[terrainLayer])
        {
            return _terrainCallBacks[terrainLayer]->getHeightAt(p.getX(), p.getY());
        }
    }
    tFace_3D f3D = _mesh3D->faceAt(f3DIndex);
    return f3D->heightForF(p);
}
float
cMesh::heightAtPointIn3DFace(tSigned32 f3DIndex, float x, float y) const
{
    if(_sectionIDs)
    {
        tSigned32 terrainLayer = (*_sectionIDs)[f3DIndex];
        if(terrainLayer >= 0 && terrainLayer < SizeL(_terrainCallBacks) && _terrainCallBacks[terrainLayer])
        {
            tSigned32 xRounded = static_cast<tSigned32>(x);
            tSigned32 yRounded = static_cast<tSigned32>(y);
            x -= static_cast<float>(xRounded);
            y -= static_cast<float>(yRounded);
            return _terrainCallBacks[terrainLayer]->getHeightAtWithPrecision(xRounded, yRounded, x, y);
        }
    }
    tFace_3D f3D = _mesh3D->faceAt(f3DIndex);
    return f3D->heightForF(x, y);
}


cHandle<tMeshCircuits>
cMesh::generateOldStyleBaseCircuitsForBaseObstacles() const
{
    cHandle<tMeshCircuits> result = new tMeshCircuits;
    tSigned32 i;
    for(i = 0; i != SizeL(_baseObstacles); ++i)
    {
        const cObstacleInfo& info = _baseObstacles[i];
        if(info._costToTraverse != -1.f)
        {
            continue;
        }
        std::list<tFace> containedExternalFaces;
//        CircuitsForOverlappingPolygon(info._position, *info._shape, containedExternalFaces, *result);
        cConvexPoly placedShape;
        _baseObstacleShapes[i].offset(info._position.p, placedShape);
        CircuitsForOverlappingPolygon(info._position.f, placedShape, containedExternalFaces, *result);
        assertD(containedExternalFaces.empty()); //..... should be able to follow through to support this now
    }
    return result;
}

const cCollisionPreprocess&
cMesh::refCollisionPreprocess(tSigned32 index) const
{
    return *_collisionPreprocess.get(index);
}
const cPathfindPreprocess&
cMesh::refPathfindPreprocess(tSigned32 index) const
{
    return *_pathfindPreprocess.get(index);
}

tFace
cMesh::getFaceForEndOfLine(const cInternalPosition& start, const tPoint& end) const
{
    tFace endF = start.f;
    tLine line(start.p, end);
    tEdge crossed;
    while(TraverseTowardsTarget_SI<tMesh>(endF, line, line.end(), crossed))
    {
    }
    if(endF->isExternal())
    {
        endF.clear();
    }
    return endF;
}

tFace_3D
cMesh::getOverlappingFaceOn3DJustAfterStartOfEdge(tEdge e) const
{
    cInfinitessimallyOffset_PointLike<tPoint> p(GetVertexPoint(e), GetEdgeAxis(e));
    cMeshElement<tMesh> on2D(e);
    cMeshElement<tMesh_3D> on3D = _translation->translateElement(on2D, p);
    return on3D.getOverlappingFace();
}

const cOffMeshConnections&
cMesh::refOffMeshConnections() const
{
    assertD(_offMeshConnections);
    return *_offMeshConnections;
}
tUnsigned32
cMesh::lookupPenaltyForOffMeshConnection(tSigned32 index) const
{
    assertD(_offMeshConnections);
    return _offMeshConnections->refConnection(index).penalty;
}


cPosition
cMesh::translateFrom3D(tSigned32 f3D, tSigned32 x, tSigned32 y) const
{
    tPoint p(x, y, WORLD_RANGE);
    tFace f2D = _translation->translate(_mesh3D->faceAt(f3D), p);
    cPosition result;
    result.x = x;
    result.y = y;
    result.cell = f2D.index();
    return result;
}

tSigned32
cMesh::getNumberOfInternalFacesIn2D() const
{
    cMutexLock lock(_meshMutex);
    return _mesh2DInternalFaces;
}

iAgent*
cMesh::placeAgent(const cConvexPoly& cs, const cPosition& position) const
{
    iShape* shape = nSingletons::pathEngine().newShape_Internal(cs);
    iAgent* result = placeAgent(shape, position);
    delete shape;
    return result;
}
iAgent*
cMesh::placeLargeStaticObstacle_Internal(const cConvexPoly& placedShape, const cPosition& position) const
{
	cConvexPoly normalised;
	placedShape.normalise(normalised);
    cAgent* a = new cAgent(const_cast<cMesh*>(this), normalised, position);
    cMutexLock lock(_heldAgentsMutex);
    a->setIndexInMesh(_heldAgents.reuseOrPushBack(a));
    return a;
}

void
cMesh::getRange(tPoint& bottomLeft, tPoint& topRight) const
{
    cMutexLock lock(_meshMutex);
    tVertex v = _mesh2D->beginVertex();
    bottomLeft = GetVertexPoint(v);
    topRight = bottomLeft;
    while((++v).valid())
    {
        tPoint p = GetVertexPoint(v);
        bottomLeft.updateAsMinimumBound(p);
        topRight.updateAsMaximumBound(p);
    }
}

void
cMesh::attachBaseObstacles(
        std::vector<cConvexPoly>& baseObstacleShapes,
        std::vector<cObstacleInfo>& baseObstacleInfos
        )
{
    assertD(_baseObstacleShapes.empty());
    assertD(_baseObstacles.empty());
    _baseObstacleShapes.swap(baseObstacleShapes);
    _baseObstacles.swap(baseObstacleInfos);
    ShrinkToFit(_baseObstacleShapes);
    ShrinkToFit(_baseObstacles);
}

void
cMesh::attachNamedObstacles(
        std::vector<std::string>& namedObstacleIDs,
        std::vector<cConvexPoly>& namedObstacleShapes,
        std::vector<cObstacleInfo>& namedObstacleInfos
        )
{
    assertD(_namedObstacleIDs.empty());
    assertD(_namedObstacleShapes.empty());
    assertD(_namedObstacles.empty());
    _namedObstacleIDs.swap(namedObstacleIDs);
    _namedObstacleShapes.swap(namedObstacleShapes);
    _namedObstacles.swap(namedObstacleInfos);
    ShrinkToFit(_namedObstacleIDs);
    ShrinkToFit(_namedObstacleShapes);
    ShrinkToFit(_namedObstacles);
}

void
cMesh::attachAnchors(
        std::vector<std::string> anchorIDs,
        std::vector<cPosition> anchorPositions,
        std::vector<tSigned32> anchorOrientations
        )
{
    assertD(_anchorIDs.empty());
    assertD(_anchorPositions.empty());
    assertD(_anchorOrientations.empty());
    _anchorIDs.swap(anchorIDs);
    _anchorPositions.swap(anchorPositions);
    _anchorOrientations.swap(anchorOrientations);
    ShrinkToFit(_anchorIDs);
    ShrinkToFit(_anchorPositions);
    ShrinkToFit(_anchorOrientations);
}

bool
cMesh::hasOffMeshConnections() const
{
    cMutexLock lock(_meshMutex);
    return !_offMeshConnections->empty();
}

bool
cMesh::hasBaseSurfaceTypeCosts() const
{
    cMutexLock lock(_meshMutex);
    return !_baseSurfaceTypeCosts.empty();
}

tSigned32
cMesh::lookup3DFaceInSectionWithCoords(tSigned32 sectionID, tSigned32* horizontalCoords, float* verticalCoords) const
{
    cMutexLock lock(_meshMutex);
    if(sectionID != -1 && !_sectionIDs)
    {
        return -1;
    }
    tPoint p0(horizontalCoords[0], horizontalCoords[1], WORLD_RANGE);
    {
        tSigned32 segments = _partitioning->queryFacesPotentiallyStabbed(p0, &_partitioningResultsBuffer.front());
        tSigned32 i;
        for(i = 0; i < segments; i++)
        {
            tSigned32 f3DIndex = _partitioningResultsBuffer[i];
            if(sectionID != -1 && (*_sectionIDs)[f3DIndex] != sectionID)
            {
                continue;
            }
            tFace_3D f3D = _mesh3D->faceAt(f3DIndex);
            tEdge_3D e = tMesh_3D::beginEdge(f3D);
            do
            {
                if(GetVertexPoint(e) != p0)
                {
                    continue;
                }
                if(e->_z != verticalCoords[0])
                {
                    continue;
                }
              // matched first vertex
              // check remaining two
                tPoint p1(horizontalCoords[2], horizontalCoords[3], WORLD_RANGE);
                ++e;
                if(GetVertexPoint(e) != p1)
                {
                    break;
                }                
                if(e->_z != verticalCoords[1])
                {
                    break;
                }
                tPoint p2(horizontalCoords[4], horizontalCoords[5], WORLD_RANGE);
                ++e;
                if(GetVertexPoint(e) != p2)
                {
                    break;
                }                
                if(e->_z != verticalCoords[2])
                {
                    break;
                }
              // found matching tri
                return f3DIndex;
            }
            while(e.toNextInFace());
        }
    }
    return -1;
}

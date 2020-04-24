#include "precomp_ntlpresentation.h"

#include "NtlPLResourcePack.h"
#include "NtlPLEvent.h"

#include "NtlPLSceneManager.h"
#include "NtlPLVisualManager.h"

#include "NtlWorldSector.h"
#include "NtlWorldField.h"

#include "NtlWorldSectorInfo.h"
#include "ntlworldinterface.h"
#include "NtlWorldMaterialPlugin.h"

#include "NtlWorldSectorManager.h"
#include "NtlWorldFieldManager.h"

#include "NtlPLEventGenerator.h"

#include "NtlPLWorldEntity.h"
#include "NtlPLObject.h"
#include "NtlPLWater.h"
#include "NtlWorldShadow.h"
#include "NtlPLSoftSpot.h"
#include "NtlPLShoreLine.h"
#include "NtlPLDecal.h"
#include "NtlPLWorldDecal.h"
#include "NtlPLPlant.h"
#include "NtlPLWorldLight_Base.h"
#include "NtlPLWorldLight_Sphere.h"
#include "NtlPLWorldLight_Box.h"
#include "NtlPLOccluder_Base.h"
#include "NtlPLOccluder_Quad.h"
#include "NtlPLHeatHaze.h"
#include "NtlPLLightObject.h"
#include "NtlPLDecalVolume.h"
#include "NtlPLGameProperty.h"
#include "NtlPLDojo.h"

#include "NtlWorldSchedule.h"

#define	dNTL_WORLD_SCHEDULE_PERFORMACE
#ifdef dNTL_WORLD_SCHEDULE_PERFORMACE
#include "PerformanceChecker.h"
CPerformanceChecker performaceChecker;
#endif

#ifdef dNTL_WORLD_SCHEDULE_PERFORMACE
#define dNTL_WORLD_SCHEDULE_PERFORMACE_RUN()		performaceChecker.Run();
#define dNTL_WORLD_SCHEDULE_PERFORMACE_STOP(msg)	performaceChecker.Stop(); performaceChecker.Print(msg);
#else
#define dNTL_WORLD_SCHEDULE_PERFORMACE_RUN()
#define dNTL_WORLD_SCHEDULE_PERFORMACE_STOP(msg)
#endif

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifdef dNTL_WORLD_FILE

RwFreeList* CNtlWorldSchedulerFreeList::m_pFreeList[ESCHEDULE_FREELIST_NUM];
RwInt32		CNtlWorldSchedulerFreeList::m_iRefCount = 0;

RwBool CNtlWorldSchedulerFreeList::AllocFreeList()
{
	if (m_iRefCount++ > 1)
	{
		return TRUE;
	}

	RwInt32 iSize[ESCHEDULE_FREELIST_NUM];
	iSize[ESCHEDULE_FREELIST_FIELD_DELETE]			= sizeof(CNtlWSEFieldDelete);
	iSize[ESCHEDULE_FREELIST_FIELD_CREATE]			= sizeof(CNtlWSEFieldCreate);
	iSize[ESCHEDULE_FREELIST_FIELD_CREATE_INDOOR]	= sizeof(CNtlWSEFieldCreateIndoor);
	iSize[ESCHEDULE_FREELIST_FIELD]					= sizeof(CNtlWSEField);
	iSize[ESCHEDULE_FREELIST_MHD]					= sizeof(CNtlWSEMHD);
	iSize[ESCHEDULE_FREELIST_MHD_INDOOR]			= sizeof(CNtlWSEMHDIndoor);
	iSize[ESCHEDULE_FREELIST_DOODADS]				= sizeof(CNtlWSEDoodads);
	iSize[ESCHEDULE_FREELIST_DOODADS_INDOOR]		= sizeof(CNtlWSEDoodadsIndoor);
	iSize[ESCHEDULE_FREELIST_WATER]					= sizeof(CNtlWSEWater);
	iSize[ESCHEDULE_FREELIST_SHADOW]				= sizeof(CNtlWSEShadow);
	iSize[ESCHEDULE_FREELIST_EFFECT]				= sizeof(CNtlWSEEffect);
	iSize[ESCHEDULE_FREELIST_SE]					= sizeof(CNtlWSESE);
	// CZ-SS
	//iSize[ESCHEDULE_FREELIST_SS]					= sizeof(CNtlWSESS);
	iSize[ESCHEDULE_FREELIST_SECTORCULL]			= sizeof(CNtlWSESectorCull);
	iSize[ESCHEDULE_FREELIST_TITETRANSPARENCY]		= sizeof(CNtlWSETileTransparency);
	iSize[ESCHEDULE_FREELIST_SHORELINE]				= sizeof(CNtlWSEShoreLine);
	iSize[ESCHEDULE_FREELIST_DECAL]					= sizeof(CNtlWSEDecal);
	iSize[ESCHEDULE_FREELIST_PLANT]					= sizeof(CNtlWSEPlant);
	iSize[ESCHEDULE_FREELIST_WORLDLIGHT]			= sizeof(CNtlWSEWorldLight);
	iSize[ESCHEDULE_FREELIST_OCCLUDER]				= sizeof(CNtlWSEOccluder);
	iSize[ESCHEDULE_FREELIST_HEATHAZEOBJECT]		= sizeof(CNtlWSEHeatHazeObject);
	iSize[ESCHEDULE_FREELIST_LIGHT_OBJECT]			= sizeof(CNtlWSELightObject);
	iSize[ESCHEDULE_FREELIST_DECAL_VOLUME]			= sizeof(CNtlWSEDecalVolume);
	iSize[ESCHEDULE_FREELIST_GAME_PROPERTY]			= sizeof(CNtlWSEGameProperty);
	iSize[ESCHEDULE_FREELIST_DOJO]					= sizeof(CNtlWSEDojo);

	RwInt32 iBlock[ESCHEDULE_FREELIST_NUM];
	iBlock[ESCHEDULE_FREELIST_FIELD_DELETE]			= 22;
	iBlock[ESCHEDULE_FREELIST_FIELD_CREATE]			= 22;
	iBlock[ESCHEDULE_FREELIST_FIELD_CREATE_INDOOR]	= 22;
	iBlock[ESCHEDULE_FREELIST_FIELD]				= 22;
	iBlock[ESCHEDULE_FREELIST_MHD]					= 88;
	iBlock[ESCHEDULE_FREELIST_MHD_INDOOR]			= 88;
	iBlock[ESCHEDULE_FREELIST_DOODADS]				= 88;
	iBlock[ESCHEDULE_FREELIST_DOODADS_INDOOR]		= 88;
	iBlock[ESCHEDULE_FREELIST_WATER]				= 88;
	iBlock[ESCHEDULE_FREELIST_SHADOW]				= 88;
	iBlock[ESCHEDULE_FREELIST_EFFECT]				= 88;
	iBlock[ESCHEDULE_FREELIST_SE]					= 88;
	// CZ-SS
	//iBlock[ESCHEDULE_FREELIST_SS]					= 88;
	iBlock[ESCHEDULE_FREELIST_SECTORCULL]			= 88;
	iBlock[ESCHEDULE_FREELIST_TITETRANSPARENCY]		= 88;
	iBlock[ESCHEDULE_FREELIST_SHORELINE]			= 88;
	iBlock[ESCHEDULE_FREELIST_DECAL]				= 88;
	iBlock[ESCHEDULE_FREELIST_PLANT]				= 88;
	iBlock[ESCHEDULE_FREELIST_WORLDLIGHT]			= 88;
	iBlock[ESCHEDULE_FREELIST_OCCLUDER]				= 88;
	iBlock[ESCHEDULE_FREELIST_HEATHAZEOBJECT]		= 88;	
	iBlock[ESCHEDULE_FREELIST_LIGHT_OBJECT]			= 88;	
	iBlock[ESCHEDULE_FREELIST_DECAL_VOLUME]			= 88;	
	iBlock[ESCHEDULE_FREELIST_GAME_PROPERTY]		= 88;	
	iBlock[ESCHEDULE_FREELIST_DOJO]					= 88;	

	for (int i = 0; i < ESCHEDULE_FREELIST_NUM; ++i)
	{
		//m_pFreeList[i] = RwFreeListCreateAndPreallocateSpace(iSize[i], 11, 16/*rwMALLOCALIGNMENT*/, 1, NULL, rwMEMHINTDUR_GLOBAL);
		m_pFreeList[i] = RwFreeListCreate(iSize[i], iBlock[i], 16, rwMEMHINTDUR_GLOBAL);
	}

	return TRUE;
}

void CNtlWorldSchedulerFreeList::DeAllocFreeList()
{
	if (m_iRefCount-- > 1)
	{
		NTL_ASSERTFAIL("CNtlWorldSchedulerFreeList::DeAllocFreeList, please call me(Cz)");
		return;
	}

	for (int i = 0; i < ESCHEDULE_FREELIST_NUM; ++i)
	{
		if (m_pFreeList[i] != NULL)
		{
			RwFreeListDestroy(m_pFreeList[i]);
			m_pFreeList[i] = NULL;
		}
	}
}

void* CNtlWorldSchedulerFreeList::Alloc(const ESCHEDULE_FREELIST_TYPE eType)
{
	return  RwFreeListAlloc(m_pFreeList[eType], rwMEMHINTDUR_GLOBAL);
}

void CNtlWorldSchedulerFreeList::Free(const ESCHEDULE_FREELIST_TYPE eType, void *p)
{
	RwFreeListFree(m_pFreeList[eType], p);
}



//////////////////////////////////////////////////////////////////////////

CNtlWorldScheduler::CNtlWorldScheduler()
{
	CNtlWorldSchedulerFreeList::AllocFreeList();

	m_vLastPos.x	= 0.0f;
	m_vLastPos.y	= 0.0f;
	m_vLastPos.z	= 0.0f;

	m_fElapsedTime	= 0.0f;
}

CNtlWorldScheduler::~CNtlWorldScheduler()
{
	CNtlWorldSchedulerFreeList::DeAllocFreeList();
}

void CNtlWorldScheduler::CreateField(RwInt32 iFieldIdx)
{
	// Create Schedule does not remove because it runs only when the Delete Schedule is empty.
	// Data can be added if you remove it.
	//m_WDS.RemoveSchedule(iFieldIdx);
	switch (GetSceneManager()->GetActiveWorldType())
	{
	case AW_HEGITHFIELD:
		m_WCS.AddSchedule(NTL_NEW CNtlWSEFieldCreate(iFieldIdx, 0.15f, iFieldIdx)); 
		break;
	case AW_RWWORLD:
		m_WCS.AddSchedule(NTL_NEW CNtlWSEFieldCreateIndoor(iFieldIdx, 0.15f, iFieldIdx)); 
		break;
	}
}

void CNtlWorldScheduler::DeleteField(RwInt32 iFieldIdx)
{
	m_WCS.RemoveSchedule(iFieldIdx);
	m_WDS.AddSchedule(NTL_NEW CNtlWSEFieldDelete(iFieldIdx, 0.15f, iFieldIdx));
}

void CNtlWorldScheduler::RemoveSchedule(RwInt32 iPrimaryKey)
{
	m_WCS.RemoveSchedule(iPrimaryKey);
	m_WDS.RemoveSchedule(iPrimaryKey);
}

void CNtlWorldScheduler::ClearSchedule()
{
	m_WDS.ClearSchedule();
	m_WCS.ClearSchedule();
}

RwBool CNtlWorldScheduler::Scheduling(RwReal fElapsedTime, RwV3d& vAvatarPos)
{
	if (!m_WDS.Scheduling(fElapsedTime))
	{
		RwReal fTemp = (m_vLastPos.x - vAvatarPos.x) * (m_vLastPos.x - vAvatarPos.x) + (m_vLastPos.z - vAvatarPos.z) * (m_vLastPos.z - vAvatarPos.z);
		if (fTemp == 0.0f)
		{
			fTemp = fElapsedTime;
		}
		else
		{
			fTemp = fElapsedTime + sqrtf(fTemp);
		}
		m_fElapsedTime = (m_fElapsedTime + fTemp) * 0.5f;
		if (!m_WCS.Scheduling(m_fElapsedTime))
		{
			m_vLastPos = vAvatarPos;
			return FALSE;
		}
	}

	m_vLastPos = vAvatarPos;
	return TRUE;
}

// CNtlWorldScheduler::CNtlWorldScheduler()
// {
// 	CNtlWorldSchedulerFreeList::AllocFreeList();
// 
// 	m_vLastPos.x	= 0.0f;
// 	m_vLastPos.y	= 0.0f;
// 	m_vLastPos.z	= 0.0f;
// 
// 	m_fElapsedTime	= 0.0f;
// }
// 
// CNtlWorldScheduler::~CNtlWorldScheduler()
// {
// 	CNtlWorldSchedulerFreeList::DeAllocFreeList();
// }
// 
// void CNtlWorldScheduler::CreateField(RwInt32 iFieldIdx)
// {
// 	// Create Schedule은 Delete Schedule이 비어 있을 경우만 실행하기 때문에 Remove 하지 않는다.
// 	// Remove 할 경우 데이터가 Add 될 수 있다.
// 	//m_WDS.RemoveSchedule(iFieldIdx);
// 	switch (GetSceneManager()->GetActiveWorldType())
// 	{
// 	case AW_HEGITHFIELD:
// 		m_NtlWorldFieldScheduler.AddSchedule(NTL_NEW CNtlWSEFieldCreate(iFieldIdx, 0.0f, iFieldIdx)); 
// 		break;
// 	case AW_RWWORLD:
// 		m_NtlWorldFieldScheduler.AddSchedule(NTL_NEW CNtlWSEFieldCreateIndoor(iFieldIdx, 0.0f, iFieldIdx)); 
// 		break;
// 	}
// }
// 
// void CNtlWorldScheduler::DeleteField(RwInt32 iFieldIdx)
// {	
// 	m_NtlWorldFieldScheduler.AddSchedule(NTL_NEW CNtlWSEFieldDelete(iFieldIdx, 0.5f, iFieldIdx));
// }
// 
// RwBool CNtlWorldScheduler::Scheduling(RwReal fElapsedTime, RwV3d& vAvatarPos)
// {
// 	RwReal fTemp = (m_vLastPos.x - vAvatarPos.x) * (m_vLastPos.x - vAvatarPos.x) + (m_vLastPos.z - vAvatarPos.z) * (m_vLastPos.z - vAvatarPos.z);
// 	if (fTemp <= 0.001f)
// 	{
// 		m_fElapsedTime = (m_fElapsedTime + fElapsedTime) * 0.5f;
// 	}
// 	else
// 	{
// 		m_fElapsedTime = fElapsedTime + sqrtf(fTemp);
// 	}
// 	
// 	m_vLastPos = vAvatarPos;
// 	if (!m_NtlWorldFieldScheduler.Scheduling(m_fElapsedTime))
// 	{		
// 		return FALSE;
// 	}
// 	return TRUE;
// }

//////////////////////////////////////////////////////////////////////////

RwBool CNtlWorldCreateScheduler::SchedulingProcess()
{
	if (CScheduler::SchedulingProcess())
	{
		CScheduleElement* pScheduleElement = GetFirstScedule();
		if (pScheduleElement)
		{			
			if (pScheduleElement->GetScheduleStep() == ESCHEDULE_STEP_SCHEDULING)
			{
				//CScheduleElement* pScheduleElement = m_listScheduleElement.front();

				CScheduler::m_listScheduleElement.pop_front();
				CScheduler::m_listScheduleElement.push_back(pScheduleElement);

				//NTL_DELETE(pScheduleElement);
			}			
		}
		return TRUE;
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEFieldDelete::CNtlWSEFieldDelete(RwInt32 iPrimaryKey, RwReal fUsedTime, RwInt32 iFieldIdx)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_iFieldIdx(iFieldIdx)
{
}

CNtlWSEFieldDelete::~CNtlWSEFieldDelete()
{
}

RwBool CNtlWSEFieldDelete::Begin()
{
	return TRUE;
}

RwBool CNtlWSEFieldDelete::Scheduling()
{
	dNTL_WORLD_SCHEDULE_PERFORMACE_RUN();

	CNtlWorldFieldManager* pWFManager = GetSceneManager()->GetWorld()->GetWorldFieldMgr();

	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("fields");

	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", m_iFieldIdx);

	_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

	pWFManager->DeleteFieldInMemory(m_iFieldIdx);

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	dNTL_WORLD_SCHEDULE_PERFORMACE_STOP("CNtlWSEFieldDelete");

	return FALSE;
}

RwBool CNtlWSEFieldDelete::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEFieldCreate::CNtlWSEFieldCreate(RwInt32 iPrimaryKey, RwReal fUsedTime, RwInt32 iFieldIdx)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_iFieldIdx(iFieldIdx)
,m_pFile(NULL)
{
}

CNtlWSEFieldCreate::~CNtlWSEFieldCreate()
{
	if (m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
}

RwBool CNtlWSEFieldCreate::Begin()
{
	dNTL_WORLD_SCHEDULE_PERFORMACE_RUN();

	if (m_iFieldIdx == -1)
	{
		return FALSE;
	}

	CNtlWorldFieldManager*	pWFManager	= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pWFManager->GetFields());

	RwV3d vSPos = pFields[m_iFieldIdx].GetSPos();
	if(GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_TERRAIN)
	{
		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", m_iFieldIdx);

		static RwChar chPackPatch[NTL_MAX_DIR_PATH];
		strcpy_s(chPackPatch, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldProjectFolderName);
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "fields");
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldChar64Buf);
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldChar64Buf);

		SPackResFileData sPackFileData;
		RwBool bPack = GetNtlResourcePackManager()->LoadTerrain(chPackPatch, sPackFileData);
		if(bPack)
		{
			_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			::fopen_s(&m_pFile, sPackFileData.strPackFileName.c_str(), "rb");
			if(m_pFile)
			{
				fseek(m_pFile, sPackFileData.uiOffset, SEEK_SET);
			}
		}
	}
	else
	{
		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", m_iFieldIdx);

		_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
		_chdir("fields");
		_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

		::fopen_s(&m_pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb");
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	}

	DBO_ASSERT(m_pFile, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
	
	if (m_pFile)
	{
		std::vector<CScheduleElement*>	vecScheduleElement[4];
		RwInt32 iIndex = 0;
		for(int l = (int)vSPos.z; l < (int)vSPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			for(int m = (int)vSPos.x; m < (int)vSPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
			{
			//	DBO_WARNING_MESSAGE("A");
				RwV3d vSectorPos;
				vSectorPos.x = (RwReal)m;
				vSectorPos.z = (RwReal)l;

				RwInt32				iSectorIdx		= pWFManager->GetSectorIdx(vSectorPos);
				CNtlWorldSector*	pNtlWorldSector = &pWFManager->m_pSectors[iSectorIdx];

				// Add Material HeightField Diffuse :: ::fseek(m_pFile, 0, SEEK_CUR);

				RwInt32				iFilePos			= 0;
				CScheduleElement*	pScheduleElement	= NULL;

				iFilePos			= ::ftell(m_pFile);

				RwInt32				iPrimaryKey = 0;


				// add material & height field & diffuse
				pScheduleElement	= NTL_NEW CNtlWSEMHD(++iPrimaryKey, 0.25f, pNtlWorldSector, m_pFile, iFilePos, &pFields[m_iFieldIdx].GetTexAttr());
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip material & height field & diffuse
				iFilePos = GetNtlWorldSectorInfo()->SectorMeshSkipToFile(m_pFile);

				// add doodads
				pScheduleElement	= NTL_NEW CNtlWSEDoodads(++iPrimaryKey, 0.5f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip doodads
				iFilePos = GetNtlWorldSectorInfo()->SectorObjectSkipToFile(m_pFile, AW_HEGITHFIELD);

				// add water
				pScheduleElement	= NTL_NEW CNtlWSEWater(++iPrimaryKey, 0.01f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip water
				iFilePos = GetNtlWorldSectorInfo()->SectorWaterSkipToFile(m_pFile);

				// add shadow
				pScheduleElement	= NTL_NEW CNtlWSEShadow(++iPrimaryKey, 0.25f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip shadow
				iFilePos = GetNtlWorldSectorInfo()->SectorShadowSkipToFile(m_pFile);

				// add effect
				pScheduleElement	= NTL_NEW CNtlWSEEffect(++iPrimaryKey, 0.25f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip effect
				iFilePos = GetNtlWorldSectorInfo()->SectorEffectSkipToFile(m_pFile);
;
				// add se
				pScheduleElement	= NTL_NEW CNtlWSESE(++iPrimaryKey, 0.1f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip se
				iFilePos = GetNtlWorldSectorInfo()->SectorSoundEffectSkipToFile(m_pFile);

				// add sectorcull
				pScheduleElement	= NTL_NEW CNtlWSESectorCull(++iPrimaryKey, 0.01f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip sectorcull
				iFilePos = GetNtlWorldSectorInfo()->SectorCullSkipToFile(m_pFile);

				// add transparency
				pScheduleElement	= NTL_NEW CNtlWSETileTransparency(++iPrimaryKey, 0.01f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip tile transparency
				iFilePos = GetNtlWorldSectorInfo()->SectorTileTransparencySkipToFile(m_pFile);

				// add shoreline
				pScheduleElement	= NTL_NEW CNtlWSEShoreLine(++iPrimaryKey, 0.1f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip shoreline
				iFilePos = GetNtlWorldSectorInfo()->SectorShoreLineSkipToFile(m_pFile);

				// add decal
				pScheduleElement	= NTL_NEW CNtlWSEDecal(++iPrimaryKey, 0.1f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip decal
				iFilePos = GetNtlWorldSectorInfo()->SectorDecalSkipToFile(m_pFile);

				// add plant
				pScheduleElement	= NTL_NEW CNtlWSEPlant(++iPrimaryKey, 0.1f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip plant
				iFilePos = GetNtlWorldSectorInfo()->SectorPlantSkipToFile(m_pFile);

				// add world light
				pScheduleElement	= NTL_NEW CNtlWSEWorldLight(++iPrimaryKey, 0.1f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip world light
				iFilePos = GetNtlWorldSectorInfo()->SectorWorldLightSkipToFile(m_pFile);

				// add occluder
				pScheduleElement	= NTL_NEW CNtlWSEOccluder(++iPrimaryKey, 0.1f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip occluder
				iFilePos = GetNtlWorldSectorInfo()->SectorOccluderSkipToFile(m_pFile);

				// add heathaze
				pScheduleElement	= NTL_NEW CNtlWSEHeatHazeObject(++iPrimaryKey, 0.1f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip heathaze
				iFilePos = GetNtlWorldSectorInfo()->SectorHeatHazeObjectSkipToFile(m_pFile);

				// add light object
				pScheduleElement	= NTL_NEW CNtlWSELightObject(++iPrimaryKey, 0.1f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip light object
				iFilePos = GetNtlWorldSectorInfo()->SectorLightObjectSkipToFile(m_pFile);

				// add dojo
				pScheduleElement	= NTL_NEW CNtlWSEDojo(++iPrimaryKey, 0.01f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip dojo
				iFilePos = GetNtlWorldSectorInfo()->SectorDojoSkipToFile(m_pFile);

				++iIndex;
			}
		}

		for (int i = 0; i < (int)vecScheduleElement[0].size(); ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				m_Scheduler.AddSchedule(vecScheduleElement[j].at(i));
			}
			if (i == 0)
			{
				m_Scheduler.AddSchedule(NTL_NEW CNtlWSEField(99, 0.01f, m_iFieldIdx));
			}
		}
	}

	dNTL_WORLD_SCHEDULE_PERFORMACE_STOP("CNtlWSEFieldCreate");

	return TRUE;
}

RwBool CNtlWSEFieldCreate::Scheduling()
{
	return m_Scheduler.SchedulingProcess();
}

RwBool CNtlWSEFieldCreate::End()
{
	CNtlWorldFieldManager* pWFManager = GetSceneManager()->GetWorld()->GetWorldFieldMgr();

	if (m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}

	return TRUE;
}

// CNtlWSEFieldCreate::CNtlWSEFieldCreate(RwInt32 iPrimaryKey, RwReal fUsedTime, RwInt32 iFieldIdx)
// :CScheduleElement(iPrimaryKey, fUsedTime)
// ,m_iFieldIdx(iFieldIdx)
// ,m_pFile(NULL)
// {
// }
// 
// CNtlWSEFieldCreate::~CNtlWSEFieldCreate()
// {
// 	if (m_pFile)
// 	{
// 		fclose(m_pFile);
// 		m_pFile = NULL;
// 	}
// }
// 
// RwBool CNtlWSEFieldCreate::Begin()
// {
//	dNTL_WORLD_SCHEDULE_PERFORMACE_STOP("CNtlWSEFieldCreate");
// 	if (m_iFieldIdx == -1)
// 	{
// 		return FALSE;
// 	}
// 
// 	CNtlWorldFieldManager*	pWFManager	= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
// 	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pWFManager->GetFields());
// 
// 	RwV3d vSPos = pFields[m_iFieldIdx].GetSPos();
// 	if(GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_TERRAIN)
// 	{
// 		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", m_iFieldIdx);
// 
// 		static RwChar chPackPatch[NTL_MAX_DIR_PATH];
// 		strcpy_s(chPackPatch, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldProjectFolderName);
// 		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");
// 		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "fields");
// 		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");
// 		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldChar64Buf);
// 		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");
// 		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldChar64Buf);
// 
// 		SPackResFileData sPackFileData;
// 		RwBool bPack = GetNtlResourcePackManager()->LoadTerrain(chPackPatch, sPackFileData);
// 		if(bPack)
// 		{
// 			_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
// 			::fopen_s(&m_pFile, sPackFileData.strPackFileName.c_str(), "rb");
// 			if(m_pFile)
// 			{
// 				fseek(m_pFile, sPackFileData.uiOffset, SEEK_SET);
// 			}
// 		}
// 	}
// 	else
// 	{
// 		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", m_iFieldIdx);
// 
// 		_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
// 		_chdir("fields");
// 		_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);
// 
// 		::fopen_s(&m_pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb");
// 	}
// 	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
// 
// 	DBO_ASSERT(m_pFile, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
// 
// 	if (m_pFile)
// 	{
// 		std::vector<CScheduleElement*>	vecScheduleElement[4];
// 		RwInt32							iIndex = 0;
// 		for(int l = (int)vSPos.z; l < (int)vSPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
// 		{
// 			for(int m = (int)vSPos.x; m < (int)vSPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
// 			{
// 				RwV3d vSectorPos;
// 				vSectorPos.x = (RwReal)m;
// 				vSectorPos.z = (RwReal)l;
// 
// 				RwInt32				iSectorIdx			= pWFManager->GetSectorIdx(vSectorPos);
// 				CNtlWorldSector*	pNtlWorldSector		= &pWFManager->m_pSectors[iSectorIdx];
// 
// 				RwInt32				iFilePos			= -1;
// 				CScheduleElement*	pScheduleElement	= NULL;
// 				RwInt32				iPrimaryKey			= 0;
// 
// 				// add material & height field & diffuse
// 				pScheduleElement	= NTL_NEW CNtlWSEMHD(++iPrimaryKey, 0.25f, pNtlWorldSector, m_pFile, iFilePos, &pFields[m_iFieldIdx].GetTexAttr());
// 				m_Scheduler.AddSchedule(pScheduleElement);
// 
// 				// add doodads
// 				pScheduleElement	= NTL_NEW CNtlWSEDoodads(++iPrimaryKey, 0.5f, pNtlWorldSector, m_pFile, iFilePos);
// 				m_Scheduler.AddSchedule(pScheduleElement);
// 
// 				// add water
// 				pScheduleElement	= NTL_NEW CNtlWSEWater(++iPrimaryKey, 0.01f, pNtlWorldSector, m_pFile, iFilePos);
// 				m_Scheduler.AddSchedule(pScheduleElement);
// 
// 				// add shadow
// 				pScheduleElement	= NTL_NEW CNtlWSEShadow(++iPrimaryKey, 0.25f, pNtlWorldSector, m_pFile, iFilePos);
// 				m_Scheduler.AddSchedule(pScheduleElement);
// 
// 				// add effect
// 				pScheduleElement	= NTL_NEW CNtlWSEEffect(++iPrimaryKey, 0.25f, pNtlWorldSector, m_pFile, iFilePos);
// 				m_Scheduler.AddSchedule(pScheduleElement);
// 
// 				// add se
// 				pScheduleElement	= NTL_NEW CNtlWSESE(++iPrimaryKey, 0.1f, pNtlWorldSector, m_pFile, iFilePos);
// 				m_Scheduler.AddSchedule(pScheduleElement);
// 
// 				// add sectorcull
// 				pScheduleElement	= NTL_NEW CNtlWSESectorCull(++iPrimaryKey, 0.01f, pNtlWorldSector, m_pFile, iFilePos);
// 				m_Scheduler.AddSchedule(pScheduleElement);
// 
// 				// add transparency
// 				pScheduleElement	= NTL_NEW CNtlWSETileTransparency(++iPrimaryKey, 0.01f, pNtlWorldSector, m_pFile, iFilePos);
// 				m_Scheduler.AddSchedule(pScheduleElement);
// 
// 				// add shoreline
// 				pScheduleElement	= NTL_NEW CNtlWSEShoreLine(++iPrimaryKey, 0.1f, pNtlWorldSector, m_pFile, iFilePos);
// 				m_Scheduler.AddSchedule(pScheduleElement);
// 
// 				// add decal
// 				pScheduleElement	= NTL_NEW CNtlWSEDecal(++iPrimaryKey, 0.1f, pNtlWorldSector, m_pFile, iFilePos);
// 				m_Scheduler.AddSchedule(pScheduleElement);
// 
// 				// add plant
// 				pScheduleElement	= NTL_NEW CNtlWSEPlant(++iPrimaryKey, 0.1f, pNtlWorldSector, m_pFile, iFilePos);
// 				m_Scheduler.AddSchedule(pScheduleElement);
// 
// 				// add world light
// 				pScheduleElement	= NTL_NEW CNtlWSEWorldLight(++iPrimaryKey, 0.1f, pNtlWorldSector, m_pFile, iFilePos);
// 				m_Scheduler.AddSchedule(pScheduleElement);
// 
// 				// add occluder
// 				pScheduleElement	= NTL_NEW CNtlWSEOccluder(++iPrimaryKey, 0.1f, pNtlWorldSector, m_pFile, iFilePos);
// 				m_Scheduler.AddSchedule(pScheduleElement);
// 
// 				// add heathaze
// 				pScheduleElement	= NTL_NEW CNtlWSEHeatHazeObject(++iPrimaryKey, 0.1f, pNtlWorldSector, m_pFile, iFilePos);
// 				m_Scheduler.AddSchedule(pScheduleElement);
// 
// 				// add light object
// 				pScheduleElement	= NTL_NEW CNtlWSELightObject(++iPrimaryKey, 0.1f, pNtlWorldSector, m_pFile, iFilePos);
// 				m_Scheduler.AddSchedule(pScheduleElement);
// 
// 				// add dojo
// 				pScheduleElement	= NTL_NEW CNtlWSEDojo(++iPrimaryKey, 0.01f, pNtlWorldSector, m_pFile, iFilePos);
// 				m_Scheduler.AddSchedule(pScheduleElement);
// 
// 				++iIndex;
// 			}
// 		}
// 	}
// 	m_Scheduler.AddSchedule(NTL_NEW CNtlWSEField(99, 0.01f, m_iFieldIdx));
// 
//	dNTL_WORLD_SCHEDULE_PERFORMACE_STOP("CNtlWSEFieldCreate");

// 	return TRUE;
// }
// 
// RwBool CNtlWSEFieldCreate::Scheduling()
// {
// 	return m_Scheduler.SchedulingProcess();
// }
// 
// RwBool CNtlWSEFieldCreate::End()
// {
// 	if (m_pFile)
// 	{
// 		fclose(m_pFile);
// 		m_pFile = NULL;
// 	}
// 
// 	return TRUE;
// }

//////////////////////////////////////////////////////////////////////////

CNtlWSEFieldCreateIndoor::CNtlWSEFieldCreateIndoor(RwInt32 iPrimaryKey, RwReal fUsedTime, RwInt32 iFieldIdx)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_iFieldIdx(iFieldIdx)
,m_pFile(NULL)
{
}

CNtlWSEFieldCreateIndoor::~CNtlWSEFieldCreateIndoor()
{
	if (m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
}

RwBool CNtlWSEFieldCreateIndoor::Begin()
{
	dNTL_WORLD_SCHEDULE_PERFORMACE_RUN();

	if (m_iFieldIdx == -1)
	{
		return FALSE;
	}

	CNtlWorldFieldManager*	pWFManager	= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pWFManager->GetFields());

	RwV3d vSPos = pFields[m_iFieldIdx].GetSPos();
	if(GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_TERRAIN)
	{
		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", m_iFieldIdx);

		static RwChar chPackPatch[NTL_MAX_DIR_PATH];
		strcpy_s(chPackPatch, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldProjectFolderName);
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "fields");
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldChar64Buf);
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldChar64Buf);

		SPackResFileData sPackFileData;
		RwBool bPack = GetNtlResourcePackManager()->LoadTerrain(chPackPatch, sPackFileData);
		if(bPack)
		{
			_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			::fopen_s(&m_pFile, sPackFileData.strPackFileName.c_str(), "rb");
			if(m_pFile)
			{
				fseek(m_pFile, sPackFileData.uiOffset, SEEK_SET);
			}
		}
	}
	else
	{
		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", m_iFieldIdx);

		_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
		_chdir("fields");
		_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

		::fopen_s(&m_pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb");
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	}

	DBO_ASSERT(m_pFile, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");

	if (m_pFile)
	{
		std::vector<CScheduleElement*>	vecScheduleElement[4];
		RwInt32							iIndex = 0;
		for(int l = (int)vSPos.z; l < (int)vSPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			for(int m = (int)vSPos.x; m < (int)vSPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
			{
				RwV3d vSectorPos;
				vSectorPos.x = (RwReal)m;
				vSectorPos.z = (RwReal)l;

				RwInt32				iSectorIdx		= pWFManager->GetSectorIdx(vSectorPos);
				CNtlWorldSector*	pNtlWorldSector = &pWFManager->m_pSectors[iSectorIdx];

				// Add Material HeightField Diffuse :: ::fseek(pFile, 0, SEEK_CUR);

				RwInt32				iFilePos			= 0;
				CScheduleElement*	pScheduleElement	= NULL;

				iFilePos			= ::ftell(m_pFile);

				RwInt32				iPrimaryKey = 0;

				// add material & height field & diffuse
				pScheduleElement	= NTL_NEW CNtlWSEMHDIndoor(++iPrimaryKey, 0.01f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// add doodads
				pScheduleElement	= NTL_NEW CNtlWSEDoodadsIndoor(++iPrimaryKey, 0.5f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip doodads
				iFilePos = GetNtlWorldSectorInfo()->SectorObjectSkipToFile(m_pFile, AW_RWWORLD);

				// add water
				pScheduleElement	= NTL_NEW CNtlWSEWater(++iPrimaryKey, 0.01f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip water
				iFilePos = GetNtlWorldSectorInfo()->SectorWaterSkipToFile(m_pFile);

				// add shadow
				pScheduleElement	= NTL_NEW CNtlWSEShadow(++iPrimaryKey, 0.25f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip shadow
				iFilePos = GetNtlWorldSectorInfo()->SectorShadowSkipToFile(m_pFile);

				// add effect
				pScheduleElement	= NTL_NEW CNtlWSEEffect(++iPrimaryKey, 0.25f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip effect
				iFilePos = GetNtlWorldSectorInfo()->SectorEffectSkipToFile(m_pFile);

				// add se
				pScheduleElement	= NTL_NEW CNtlWSESE(++iPrimaryKey, 0.1f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip se
				iFilePos = GetNtlWorldSectorInfo()->SectorSoundEffectSkipToFile(m_pFile);

				// add sectorcull
				pScheduleElement	= NTL_NEW CNtlWSESectorCull(++iPrimaryKey, 0.01f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip sectorcull
				iFilePos = GetNtlWorldSectorInfo()->SectorCullSkipToFile(m_pFile);

				// add transparency
				pScheduleElement	= NTL_NEW CNtlWSETileTransparency(++iPrimaryKey, 0.01f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip tile transparency
				iFilePos = GetNtlWorldSectorInfo()->SectorTileTransparencySkipToFile(m_pFile);

				// add shoreline
				pScheduleElement	= NTL_NEW CNtlWSEShoreLine(++iPrimaryKey, 0.1f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip shoreline
				iFilePos = GetNtlWorldSectorInfo()->SectorShoreLineSkipToFile(m_pFile);

				// add decal
				pScheduleElement	= NTL_NEW CNtlWSEDecal(++iPrimaryKey, 0.1f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip decal
				iFilePos = GetNtlWorldSectorInfo()->SectorDecalSkipToFile(m_pFile);

				// add plant
				pScheduleElement	= NTL_NEW CNtlWSEPlant(++iPrimaryKey, 0.1f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip plant
				iFilePos = GetNtlWorldSectorInfo()->SectorPlantSkipToFile(m_pFile);

				// add world light
				pScheduleElement	= NTL_NEW CNtlWSEWorldLight(++iPrimaryKey, 0.1f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip world light
				iFilePos = GetNtlWorldSectorInfo()->SectorWorldLightSkipToFile(m_pFile);

				// add occluder
				pScheduleElement	= NTL_NEW CNtlWSEOccluder(++iPrimaryKey, 0.1f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip occluder
				iFilePos = GetNtlWorldSectorInfo()->SectorOccluderSkipToFile(m_pFile);

				// add heathaze
				pScheduleElement	= NTL_NEW CNtlWSEHeatHazeObject(++iPrimaryKey, 0.1f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip heathaze
				iFilePos = GetNtlWorldSectorInfo()->SectorHeatHazeObjectSkipToFile(m_pFile);

				// add light object
				pScheduleElement	= NTL_NEW CNtlWSELightObject(++iPrimaryKey, 0.1f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip light object
				iFilePos = GetNtlWorldSectorInfo()->SectorLightObjectSkipToFile(m_pFile);

				// add decal volume
				pScheduleElement	= NTL_NEW CNtlWSEDecalVolume(++iPrimaryKey, 0.1f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip decal volume
				iFilePos = GetNtlWorldSectorInfo()->SectorDecalVolumeSkipToFile(m_pFile);

				// add game property
				pScheduleElement	= NTL_NEW CNtlWSEGameProperty(++iPrimaryKey, 0.1f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip game property
				iFilePos = GetNtlWorldSectorInfo()->SectorGamePropertySkipToFile(m_pFile);

				// add dojo
				pScheduleElement	= NTL_NEW CNtlWSEDojo(++iPrimaryKey, 0.01f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip dojo
				iFilePos = GetNtlWorldSectorInfo()->SectorDojoSkipToFile(m_pFile);

				++iIndex;
			}
		}

		for (int i = 0; i < (int)vecScheduleElement[0].size(); ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				m_Scheduler.AddSchedule(vecScheduleElement[j].at(i));
			}
		}
	}

	dNTL_WORLD_SCHEDULE_PERFORMACE_STOP("CNtlWSEFieldCreateIndoor");

	return TRUE;
}

RwBool CNtlWSEFieldCreateIndoor::Scheduling()
{
	return m_Scheduler.SchedulingProcess();
}

RwBool CNtlWSEFieldCreateIndoor::End()
{
	CNtlWorldFieldManager* pWFManager = GetSceneManager()->GetWorld()->GetWorldFieldMgr();

	if (m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEField::CNtlWSEField(RwInt32 iPrimaryKey, RwReal fUsedTime, RwInt32 iFieldIdx)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_iFieldIdx(iFieldIdx)
{
}

CNtlWSEField::~CNtlWSEField()
{
}

RwBool CNtlWSEField::Begin()
{	
	return TRUE;
}

RwBool CNtlWSEField::Scheduling()
{
	dNTL_WORLD_SCHEDULE_PERFORMACE_RUN();

	CNtlWorldFieldManager*	pWFManager	= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pWFManager->GetFields());

	if(!pWFManager->m_pFields[m_iFieldIdx].CreateFieldFromFile(m_iFieldIdx))
	{
		pWFManager->m_pFields[m_iFieldIdx].CreateFieldInMemory(m_iFieldIdx);
	}

	dNTL_WORLD_SCHEDULE_PERFORMACE_STOP("CNtlWSEField");

	return FALSE;
}

RwBool CNtlWSEField::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEMHD::CNtlWSEMHD(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos, sCUR_FIELD_TEX_INFO* pFieldTexInfo)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
,m_pFieldTexInfo(pFieldTexInfo)
{
}

CNtlWSEMHD::~CNtlWSEMHD()
{
}

RwBool CNtlWSEMHD::Begin()
{	
	return TRUE;
}

RwBool CNtlWSEMHD::Scheduling()
{
	dNTL_WORLD_SCHEDULE_PERFORMACE_RUN();

	dNTL_WORLD_LOCAL(m_pNtlWorldSector->m_pWorldSector, pNtlSector) = static_cast<sNtlWorldSector*>(RwFreeListAlloc(dNTL_WORLD_GLOBAL(pSectorFreelist), rwMEMHINTDUR_GLOBAL));
	sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(m_pNtlWorldSector->m_pWorldSector, pNtlSector);

	pNtlSector->pNtlWorldSector = m_pNtlWorldSector;
	pNtlSector->pNtlWorldSector->MaterialContructor();

	m_pNtlWorldSector->m_bLoaded = TRUE;

	// move file pos
	if (m_iFilePos != -1) 
		fseek(m_pFile, m_iFilePos, SEEK_SET);

	GetNtlWorldSectorInfo()->SectorMeshLoadFromFile(m_pFile, m_pNtlWorldSector, m_pFieldTexInfo);

	dNTL_WORLD_SCHEDULE_PERFORMACE_STOP("CNtlWSEMHD");

	return FALSE;
}

RwBool CNtlWSEMHD::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEMHDIndoor::CNtlWSEMHDIndoor(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSEMHDIndoor::~CNtlWSEMHDIndoor()
{
}

RwBool CNtlWSEMHDIndoor::Begin()
{	
	return TRUE;
}

RwBool CNtlWSEMHDIndoor::Scheduling()
{
	dNTL_WORLD_SCHEDULE_PERFORMACE_RUN();

	m_pNtlWorldSector->MaterialContructor();

	m_pNtlWorldSector->m_bLoaded = TRUE;

	dNTL_WORLD_SCHEDULE_PERFORMACE_STOP("CNtlWSEMHDIndoor");

	return FALSE;
}

RwBool CNtlWSEMHDIndoor::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEDoodads::CNtlWSEDoodads(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSEDoodads::~CNtlWSEDoodads()
{
}

RwBool CNtlWSEDoodads::Begin()
{	
	return TRUE;
}

RwBool CNtlWSEDoodads::Scheduling()
{
	dNTL_WORLD_SCHEDULE_PERFORMACE_RUN();

	// file pos move
	if (m_iFilePos != -1) fseek(m_pFile, m_iFilePos, SEEK_SET);

	GetNtlWorldSectorInfo()->SectorObjectLoadFromFile(m_pFile, m_pNtlWorldSector, AW_HEGITHFIELD);

	dNTL_WORLD_SCHEDULE_PERFORMACE_STOP("CNtlWSEDoodads");

	return FALSE;
}

RwBool CNtlWSEDoodads::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEDoodadsIndoor::CNtlWSEDoodadsIndoor(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSEDoodadsIndoor::~CNtlWSEDoodadsIndoor()
{
}

RwBool CNtlWSEDoodadsIndoor::Begin()
{	
	return TRUE;
}

RwBool CNtlWSEDoodadsIndoor::Scheduling()
{
	dNTL_WORLD_SCHEDULE_PERFORMACE_RUN();

	// file pos move
	if (m_iFilePos != -1) fseek(m_pFile, m_iFilePos, SEEK_SET);

	GetNtlWorldSectorInfo()->SectorObjectLoadFromFile(m_pFile, m_pNtlWorldSector, AW_RWWORLD);

	dNTL_WORLD_SCHEDULE_PERFORMACE_STOP("CNtlWSEDoodadsIndoor");

	return FALSE;
}

RwBool CNtlWSEDoodadsIndoor::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEWater::CNtlWSEWater(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSEWater::~CNtlWSEWater()
{
}

RwBool CNtlWSEWater::Begin()
{	
	return TRUE;
}

RwBool CNtlWSEWater::Scheduling()
{
	dNTL_WORLD_SCHEDULE_PERFORMACE_RUN();

	// file pos move
	if (m_iFilePos != -1) fseek(m_pFile, m_iFilePos, SEEK_SET);

	GetNtlWorldSectorInfo()->SectorWaterLoadFromFile(m_pFile, m_pNtlWorldSector);

	dNTL_WORLD_SCHEDULE_PERFORMACE_STOP("CNtlWSEWater");

	return FALSE;
}

RwBool CNtlWSEWater::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEShadow::CNtlWSEShadow(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSEShadow::~CNtlWSEShadow()
{
}

RwBool CNtlWSEShadow::Begin()
{	
	return TRUE;
}

RwBool CNtlWSEShadow::Scheduling()
{
	dNTL_WORLD_SCHEDULE_PERFORMACE_RUN();

	// file pos move
	if (m_iFilePos != -1) fseek(m_pFile, m_iFilePos, SEEK_SET);

	GetNtlWorldSectorInfo()->SectorShadowLoadFromFile(m_pFile, m_pNtlWorldSector);

	dNTL_WORLD_SCHEDULE_PERFORMACE_STOP("CNtlWSEShadow");

	return FALSE;
}

RwBool CNtlWSEShadow::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEEffect::CNtlWSEEffect(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSEEffect::~CNtlWSEEffect()
{
}

RwBool CNtlWSEEffect::Begin()
{	
	return TRUE;
}

RwBool CNtlWSEEffect::Scheduling()
{
	dNTL_WORLD_SCHEDULE_PERFORMACE_RUN();

	// file pos move
	if (m_iFilePos != -1) fseek(m_pFile, m_iFilePos, SEEK_SET);

	GetNtlWorldSectorInfo()->SectorEffectLoadFromFile(m_pFile, m_pNtlWorldSector);

	dNTL_WORLD_SCHEDULE_PERFORMACE_STOP("CNtlWSEEffect");

	return FALSE;
}

RwBool CNtlWSEEffect::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSESE::CNtlWSESE(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSESE::~CNtlWSESE()
{
}

RwBool CNtlWSESE::Begin()
{	
	return TRUE;
}

RwBool CNtlWSESE::Scheduling()
{
	dNTL_WORLD_SCHEDULE_PERFORMACE_RUN();

	// file pos move
	if (m_iFilePos != -1) fseek(m_pFile, m_iFilePos, SEEK_SET);

	GetNtlWorldSectorInfo()->SectorSoundEffectLoadFromFile(m_pFile, m_pNtlWorldSector);

	dNTL_WORLD_SCHEDULE_PERFORMACE_STOP("CNtlWSESE");

	return FALSE;
}

RwBool CNtlWSESE::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// CZ-SS
// CNtlWSESS::CNtlWSESS(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
// :CScheduleElement(iPrimaryKey, fUsedTime)
// ,m_pNtlWorldSector(pNtlWorldSector)
// ,m_pFile(pFile)
// ,m_iFilePos(iFilePos)
// {
// }
// 
// CNtlWSESS::~CNtlWSESS()
// {
// }
// 
// RwBool CNtlWSESS::Begin()
// {	
// 	return TRUE;
// }
// 
// RwBool CNtlWSESS::Scheduling()
// {
// 	// file pos move
// 	fseek(m_pFile, m_iFilePos, SEEK_SET);
// 
// 	// soft edged spot light
// 	RwV3d		SSPos;
// 	RwV3d		SSRot;
// 	RwInt32		SSNum = 0;
// 	RwRGBAReal	SSClr;
// 	RwReal		SSAng, SSRad;
// 
// 	fread(&SSNum, sizeof(RwInt32), 1, m_pFile);
// 
// 	for(int i = 0; i < SSNum; ++i)
// 	{
// 		fread(&SSPos, sizeof(RwV3d), 1, m_pFile);
// 		fread(&SSRot, sizeof(RwV3d), 1, m_pFile);
// 		fread(&SSClr, sizeof(RwRGBAReal), 1, m_pFile);
// 		fread(&SSAng, sizeof(RwReal), 1, m_pFile);
// 		fread(&SSRad, sizeof(RwReal), 1, m_pFile);
// 
// 		SPLEntityCreateParam CreateParam;
// 		CreateParam.pPos = &SSPos;
// 
// 		CNtlPLEntity* pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_SPOT_LIGHT, NTL_PLEN_SPOT_LIGHT, &CreateParam);
// 		DBO_ASSERT(pNtlPLEntity, "Entity create failed.");
// 
// 		CNtlPLSpot*	pSpotEntity	= static_cast<CNtlPLSpot*>(pNtlPLEntity);
// 
// 		pSpotEntity->SetColor(SSClr);
// 		pSpotEntity->SetAngle(SSAng);
// 		pSpotEntity->SetRadius(SSRad);
// 		pSpotEntity->SetPosition(&SSPos);
// 		pSpotEntity->SetRotate(SSRot.x, SSRot.y, SSRot.z);
// 
// 
// 		m_pNtlWorldSector->m_vecNtlPLEntitySS.push_back(pNtlPLEntity);
// 	}
// 
// 	return FALSE;
// }
// 
// RwBool CNtlWSESS::End()
// {
// 	return TRUE;
// }

//////////////////////////////////////////////////////////////////////////

CNtlWSESectorCull::CNtlWSESectorCull(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSESectorCull::~CNtlWSESectorCull()
{
}

RwBool CNtlWSESectorCull::Begin()
{	
	return TRUE;
}

RwBool CNtlWSESectorCull::Scheduling()
{
	dNTL_WORLD_SCHEDULE_PERFORMACE_RUN();

	// file pos move
	if (m_iFilePos != -1) fseek(m_pFile, m_iFilePos, SEEK_SET);

	GetNtlWorldSectorInfo()->SectorCullLoadFromFile(m_pFile, m_pNtlWorldSector);

	dNTL_WORLD_SCHEDULE_PERFORMACE_STOP("CNtlWSESectorCull");

	return FALSE;
}

RwBool CNtlWSESectorCull::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSETileTransparency::CNtlWSETileTransparency(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSETileTransparency::~CNtlWSETileTransparency()
{
}

RwBool CNtlWSETileTransparency::Begin()
{	
	return TRUE;
}

RwBool CNtlWSETileTransparency::Scheduling()
{
	dNTL_WORLD_SCHEDULE_PERFORMACE_RUN();

	// file pos move
	if (m_iFilePos != -1) fseek(m_pFile, m_iFilePos, SEEK_SET);

	GetNtlWorldSectorInfo()->SectorTileTransparencyLoadFromFile(m_pFile, m_pNtlWorldSector);

	dNTL_WORLD_SCHEDULE_PERFORMACE_STOP("CNtlWSETileTransparency");

	return FALSE;
}

RwBool CNtlWSETileTransparency::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEShoreLine::CNtlWSEShoreLine(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSEShoreLine::~CNtlWSEShoreLine()
{
}

RwBool CNtlWSEShoreLine::Begin()
{	
	return TRUE;
}

RwBool CNtlWSEShoreLine::Scheduling()
{
	dNTL_WORLD_SCHEDULE_PERFORMACE_RUN();

	// file pos move
	if (m_iFilePos != -1) fseek(m_pFile, m_iFilePos, SEEK_SET);

	GetNtlWorldSectorInfo()->SectorShoreLineLoadFromFile(m_pFile, m_pNtlWorldSector);

	dNTL_WORLD_SCHEDULE_PERFORMACE_STOP("CNtlWSEShoreLine");

	return FALSE;
}

RwBool CNtlWSEShoreLine::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEDecal::CNtlWSEDecal(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSEDecal::~CNtlWSEDecal()
{
}

RwBool CNtlWSEDecal::Begin()
{	
	return TRUE;
}

RwBool CNtlWSEDecal::Scheduling()
{
	dNTL_WORLD_SCHEDULE_PERFORMACE_RUN();

	// file pos move
	if (m_iFilePos != -1) fseek(m_pFile, m_iFilePos, SEEK_SET);

	GetNtlWorldSectorInfo()->SectorDecalLoadFromFile(m_pFile, m_pNtlWorldSector);

	dNTL_WORLD_SCHEDULE_PERFORMACE_STOP("CNtlWSEShoreLine");

	return FALSE;
}

RwBool CNtlWSEDecal::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEPlant::CNtlWSEPlant(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSEPlant::~CNtlWSEPlant()
{
}

RwBool CNtlWSEPlant::Begin()
{	
	return TRUE;
}

RwBool CNtlWSEPlant::Scheduling()
{
	dNTL_WORLD_SCHEDULE_PERFORMACE_RUN();

	// file pos move
	if (m_iFilePos != -1) fseek(m_pFile, m_iFilePos, SEEK_SET);

	GetNtlWorldSectorInfo()->SectorPlantLoadFromFile(m_pFile, m_pNtlWorldSector);

	dNTL_WORLD_SCHEDULE_PERFORMACE_STOP("CNtlWSEPlant");

	return FALSE;
}

RwBool CNtlWSEPlant::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEWorldLight::CNtlWSEWorldLight(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSEWorldLight::~CNtlWSEWorldLight()
{
}

RwBool CNtlWSEWorldLight::Begin()
{	
	return TRUE;
}

RwBool CNtlWSEWorldLight::Scheduling()
{
	dNTL_WORLD_SCHEDULE_PERFORMACE_RUN();

	// file pos move
	if (m_iFilePos != -1) fseek(m_pFile, m_iFilePos, SEEK_SET);

	GetNtlWorldSectorInfo()->SectorWorldLightLoadFromFile(m_pFile, m_pNtlWorldSector);

	dNTL_WORLD_SCHEDULE_PERFORMACE_STOP("CNtlWSEWorldLight");

	return FALSE;
}

RwBool CNtlWSEWorldLight::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEOccluder::CNtlWSEOccluder(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSEOccluder::~CNtlWSEOccluder()
{
}

RwBool CNtlWSEOccluder::Begin()
{	
	return TRUE;
}

RwBool CNtlWSEOccluder::Scheduling()
{
	dNTL_WORLD_SCHEDULE_PERFORMACE_RUN();

	// file pos move
	if (m_iFilePos != -1) fseek(m_pFile, m_iFilePos, SEEK_SET);

	GetNtlWorldSectorInfo()->SectorOccluderLoadFromFile(m_pFile, m_pNtlWorldSector);

	dNTL_WORLD_SCHEDULE_PERFORMACE_STOP("CNtlWSEOccluder");

	return FALSE;
}

RwBool CNtlWSEOccluder::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEHeatHazeObject::CNtlWSEHeatHazeObject(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSEHeatHazeObject::~CNtlWSEHeatHazeObject()
{
}

RwBool CNtlWSEHeatHazeObject::Begin()
{	
	return TRUE;
}

RwBool CNtlWSEHeatHazeObject::Scheduling()
{
	dNTL_WORLD_SCHEDULE_PERFORMACE_RUN();

	// file pos move
	if (m_iFilePos != -1) fseek(m_pFile, m_iFilePos, SEEK_SET);

	GetNtlWorldSectorInfo()->SectorHeatHazeObjectLoadFromFile(m_pFile, m_pNtlWorldSector);

	dNTL_WORLD_SCHEDULE_PERFORMACE_STOP("CNtlWSEHeatHazeObject");

	return FALSE;
}

RwBool CNtlWSEHeatHazeObject::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSELightObject::CNtlWSELightObject(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSELightObject::~CNtlWSELightObject()
{
}

RwBool CNtlWSELightObject::Begin()
{	
	return TRUE;
}

RwBool CNtlWSELightObject::Scheduling()
{
	dNTL_WORLD_SCHEDULE_PERFORMACE_RUN();

	// file pos move
	if (m_iFilePos != -1) fseek(m_pFile, m_iFilePos, SEEK_SET);

	GetNtlWorldSectorInfo()->SectorLightObjectLoadFromFile(m_pFile, m_pNtlWorldSector);

	dNTL_WORLD_SCHEDULE_PERFORMACE_STOP("CNtlWSELightObject");

	return FALSE;
}

RwBool CNtlWSELightObject::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEDecalVolume::CNtlWSEDecalVolume(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSEDecalVolume::~CNtlWSEDecalVolume()
{
}

RwBool CNtlWSEDecalVolume::Begin()
{	
	return TRUE;
}

RwBool CNtlWSEDecalVolume::Scheduling()
{
	dNTL_WORLD_SCHEDULE_PERFORMACE_RUN();

	// file pos move
	if (m_iFilePos != -1) fseek(m_pFile, m_iFilePos, SEEK_SET);

	GetNtlWorldSectorInfo()->SectorDecalVolumeLoadFromFile(m_pFile, m_pNtlWorldSector);

	dNTL_WORLD_SCHEDULE_PERFORMACE_STOP("CNtlWSEDecalVolume");

	return FALSE;
}

RwBool CNtlWSEDecalVolume::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEGameProperty::CNtlWSEGameProperty(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSEGameProperty::~CNtlWSEGameProperty()
{
}

RwBool CNtlWSEGameProperty::Begin()
{	
	return TRUE;
}

RwBool CNtlWSEGameProperty::Scheduling()
{
	dNTL_WORLD_SCHEDULE_PERFORMACE_RUN();

	// file pos move
	if (m_iFilePos != -1) fseek(m_pFile, m_iFilePos, SEEK_SET);

	GetNtlWorldSectorInfo()->SectorGamePropertyLoadFromFile(m_pFile, m_pNtlWorldSector);

	dNTL_WORLD_SCHEDULE_PERFORMACE_STOP("CNtlWSEGameProperty");

	return FALSE;
}

RwBool CNtlWSEGameProperty::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEDojo::CNtlWSEDojo(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSEDojo::~CNtlWSEDojo()
{
}

RwBool CNtlWSEDojo::Begin()
{	
	return TRUE;
}

RwBool CNtlWSEDojo::Scheduling()
{
	dNTL_WORLD_SCHEDULE_PERFORMACE_RUN();

	// file pos move
	if (m_iFilePos != -1) fseek(m_pFile, m_iFilePos, SEEK_SET);

	GetNtlWorldSectorInfo()->SectorDojoLoadFromFile(m_pFile, m_pNtlWorldSector);

	dNTL_WORLD_SCHEDULE_PERFORMACE_STOP("CNtlWSEDojo");

	return FALSE;
}

RwBool CNtlWSEDojo::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#else

RwFreeList* CNtlWorldSchedulerFreeList::m_pFreeList[ESCHEDULE_FREELIST_NUM];
RwInt32		CNtlWorldSchedulerFreeList::m_iRefCount = 0;

RwBool CNtlWorldSchedulerFreeList::AllocFreeList()
{
	if (m_iRefCount++ > 1)
	{
		return TRUE;
	}

	RwInt32 iSize[ESCHEDULE_FREELIST_NUM];
	iSize[ESCHEDULE_FREELIST_FIELD_DELETE]			= sizeof(CNtlWSEFieldDelete);
	iSize[ESCHEDULE_FREELIST_FIELD_CREATE]			= sizeof(CNtlWSEFieldCreate);
	iSize[ESCHEDULE_FREELIST_FIELD_CREATE_INDOOR]	= sizeof(CNtlWSEFieldCreateIndoor);
	iSize[ESCHEDULE_FREELIST_FIELD]					= sizeof(CNtlWSEField);
	iSize[ESCHEDULE_FREELIST_MHD]					= sizeof(CNtlWSEMHD);
	iSize[ESCHEDULE_FREELIST_MHD_INDOOR]			= sizeof(CNtlWSEMHDIndoor);
	iSize[ESCHEDULE_FREELIST_DOODADS]				= sizeof(CNtlWSEDoodads);
	iSize[ESCHEDULE_FREELIST_DOODADS_INDOOR]		= sizeof(CNtlWSEDoodadsIndoor);
	iSize[ESCHEDULE_FREELIST_WATER]					= sizeof(CNtlWSEWater);
	iSize[ESCHEDULE_FREELIST_SHADOW]				= sizeof(CNtlWSEShadow);
	iSize[ESCHEDULE_FREELIST_EFFECT]				= sizeof(CNtlWSEEffect);
	iSize[ESCHEDULE_FREELIST_SE]					= sizeof(CNtlWSESE);
	// CZ-SS
	//iSize[ESCHEDULE_FREELIST_SS]					= sizeof(CNtlWSESS);
	iSize[ESCHEDULE_FREELIST_SECTORCULL]			= sizeof(CNtlWSESectorCull);
	iSize[ESCHEDULE_FREELIST_TITETRANSPARENCY]		= sizeof(CNtlWSETileTransparency);
	iSize[ESCHEDULE_FREELIST_SHORELINE]				= sizeof(CNtlWSEShoreLine);
	iSize[ESCHEDULE_FREELIST_DECAL]					= sizeof(CNtlWSEDecal);
	iSize[ESCHEDULE_FREELIST_PLANT]					= sizeof(CNtlWSEPlant);
	iSize[ESCHEDULE_FREELIST_WORLDLIGHT]			= sizeof(CNtlWSEWorldLight);
	iSize[ESCHEDULE_FREELIST_OCCLUDER]				= sizeof(CNtlWSEOccluder);
	iSize[ESCHEDULE_FREELIST_HEATHAZEOBJECT]		= sizeof(CNtlWSEHeatHazeObject);
	iSize[ESCHEDULE_FREELIST_LIGHT_OBJECT]			= sizeof(CNtlWSELightObject);
	iSize[ESCHEDULE_FREELIST_DECAL_VOLUME]			= sizeof(CNtlWSEDecalVolume);
	iSize[ESCHEDULE_FREELIST_GAME_PROPERTY]			= sizeof(CNtlWSEGameProperty);
	iSize[ESCHEDULE_FREELIST_DOJO]					= sizeof(CNtlWSEDojo);

	RwInt32 iBlock[ESCHEDULE_FREELIST_NUM];
	iBlock[ESCHEDULE_FREELIST_FIELD_DELETE]			= 22;
	iBlock[ESCHEDULE_FREELIST_FIELD_CREATE]			= 22;
	iBlock[ESCHEDULE_FREELIST_FIELD_CREATE_INDOOR]	= 22;
	iBlock[ESCHEDULE_FREELIST_FIELD]				= 22;
	iBlock[ESCHEDULE_FREELIST_MHD]					= 88;
	iBlock[ESCHEDULE_FREELIST_MHD_INDOOR]			= 88;
	iBlock[ESCHEDULE_FREELIST_DOODADS]				= 88;
	iBlock[ESCHEDULE_FREELIST_DOODADS_INDOOR]		= 88;
	iBlock[ESCHEDULE_FREELIST_WATER]				= 88;
	iBlock[ESCHEDULE_FREELIST_SHADOW]				= 88;
	iBlock[ESCHEDULE_FREELIST_EFFECT]				= 88;
	iBlock[ESCHEDULE_FREELIST_SE]					= 88;
	// CZ-SS
	//iBlock[ESCHEDULE_FREELIST_SS]					= 88;
	iBlock[ESCHEDULE_FREELIST_SECTORCULL]			= 88;
	iBlock[ESCHEDULE_FREELIST_TITETRANSPARENCY]		= 88;
	iBlock[ESCHEDULE_FREELIST_SHORELINE]			= 88;
	iBlock[ESCHEDULE_FREELIST_DECAL]				= 88;
	iBlock[ESCHEDULE_FREELIST_PLANT]				= 88;
	iBlock[ESCHEDULE_FREELIST_WORLDLIGHT]			= 88;
	iBlock[ESCHEDULE_FREELIST_OCCLUDER]				= 88;
	iBlock[ESCHEDULE_FREELIST_HEATHAZEOBJECT]		= 88;	
	iBlock[ESCHEDULE_FREELIST_LIGHT_OBJECT]			= 88;	
	iBlock[ESCHEDULE_FREELIST_DECAL_VOLUME]			= 88;	
	iBlock[ESCHEDULE_FREELIST_GAME_PROPERTY]		= 88;	
	iBlock[ESCHEDULE_FREELIST_DOJO]					= 88;	

	for (int i = 0; i < ESCHEDULE_FREELIST_NUM; ++i)
	{
		//m_pFreeList[i] = RwFreeListCreateAndPreallocateSpace(iSize[i], 11, 16/*rwMALLOCALIGNMENT*/, 1, NULL, rwMEMHINTDUR_GLOBAL);
		m_pFreeList[i] = RwFreeListCreate(iSize[i], iBlock[i], 16, rwMEMHINTDUR_GLOBAL);
	}

	return TRUE;
}

void CNtlWorldSchedulerFreeList::DeAllocFreeList()
{
	if (m_iRefCount-- > 1)
	{
		NTL_ASSERTFAIL("CNtlWorldSchedulerFreeList::DeAllocFreeList, please call me(Cz)");
		return;
	}

	for (int i = 0; i < ESCHEDULE_FREELIST_NUM; ++i)
	{
		if (m_pFreeList[i] != NULL)
		{
			RwFreeListDestroy(m_pFreeList[i]);
			m_pFreeList[i] = NULL;
		}
	}
}

void* CNtlWorldSchedulerFreeList::Alloc(const ESCHEDULE_FREELIST_TYPE eType)
{
	return  RwFreeListAlloc(m_pFreeList[eType], rwMEMHINTDUR_GLOBAL);
}

void CNtlWorldSchedulerFreeList::Free(const ESCHEDULE_FREELIST_TYPE eType, void *p)
{
	RwFreeListFree(m_pFreeList[eType], p);
}



//////////////////////////////////////////////////////////////////////////

CNtlWorldScheduler::CNtlWorldScheduler()
{
	CNtlWorldSchedulerFreeList::AllocFreeList();

	m_vLastPos.x	= 0.0f;
	m_vLastPos.y	= 0.0f;
	m_vLastPos.z	= 0.0f;

	m_fElapsedTime	= 0.0f;
}

CNtlWorldScheduler::~CNtlWorldScheduler()
{
	CNtlWorldSchedulerFreeList::DeAllocFreeList();
}

void CNtlWorldScheduler::CreateField(RwInt32 iFieldIdx)
{
	// Create Schedule은 Delete Schedule이 비어 있을 경우만 실행하기 때문에 Remove 하지 않는다.
	// Remove 할 경우 데이터가 Add 될 수 있다.
	//m_WDS.RemoveSchedule(iFieldIdx);
	switch (GetSceneManager()->GetActiveWorldType())
	{
	case AW_HEGITHFIELD:
		m_WCS.AddSchedule(NTL_NEW CNtlWSEFieldCreate(iFieldIdx, 0.15f, iFieldIdx)); 
		break;
	case AW_RWWORLD:
		m_WCS.AddSchedule(NTL_NEW CNtlWSEFieldCreateIndoor(iFieldIdx, 0.15f, iFieldIdx)); 
		break;
	}
}

void CNtlWorldScheduler::DeleteField(RwInt32 iFieldIdx)
{
	m_WCS.RemoveSchedule(iFieldIdx);
	m_WDS.AddSchedule(NTL_NEW CNtlWSEFieldDelete(iFieldIdx, 0.15f, iFieldIdx));
}

void CNtlWorldScheduler::RemoveSchedule(RwInt32 iPrimaryKey)
{
	m_WCS.RemoveSchedule(iPrimaryKey);
	m_WDS.RemoveSchedule(iPrimaryKey);
}

void CNtlWorldScheduler::ClearSchedule()
{
	m_WDS.ClearSchedule();
	m_WCS.ClearSchedule();
}

RwBool CNtlWorldScheduler::Scheduling(RwReal fElapsedTime, RwV3d& vAvatarPos)
{
	if (!m_WDS.Scheduling(fElapsedTime))
	{
		RwReal fTemp = (m_vLastPos.x - vAvatarPos.x) * (m_vLastPos.x - vAvatarPos.x) + (m_vLastPos.z - vAvatarPos.z) * (m_vLastPos.z - vAvatarPos.z);
		if (fTemp == 0.0f)
		{
			fTemp = fElapsedTime;
		}
		else
		{
			fTemp = fElapsedTime + sqrtf(fTemp);
		}
		m_fElapsedTime = (m_fElapsedTime + fTemp) * 0.5f;
		if (!m_WCS.Scheduling(m_fElapsedTime))
		{
			m_vLastPos = vAvatarPos;
			return FALSE;
		}
	}

	m_vLastPos = vAvatarPos;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

RwBool CNtlWorldCreateScheduler::SchedulingProcess()
{
	if (CScheduler::SchedulingProcess())
	{
		CScheduleElement* pScheduleElement = GetFirstScedule();
		if (pScheduleElement)
		{			
			if (pScheduleElement->GetScheduleStep() == ESCHEDULE_STEP_SCHEDULING)
			{
				//CScheduleElement* pScheduleElement = m_listScheduleElement.front();

				CScheduler::m_listScheduleElement.pop_front();
				CScheduler::m_listScheduleElement.push_back(pScheduleElement);

				//NTL_DELETE(pScheduleElement);
			}			
		}
		return TRUE;
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEFieldDelete::CNtlWSEFieldDelete(RwInt32 iPrimaryKey, RwReal fUsedTime, RwInt32 iFieldIdx)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_iFieldIdx(iFieldIdx)
{
}

CNtlWSEFieldDelete::~CNtlWSEFieldDelete()
{
}

RwBool CNtlWSEFieldDelete::Begin()
{
	return TRUE;
}

RwBool CNtlWSEFieldDelete::Scheduling()
{
	CNtlWorldFieldManager* pWFManager = GetSceneManager()->GetWorld()->GetWorldFieldMgr();

	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("fields");

	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", m_iFieldIdx);

	_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

	pWFManager->DeleteFieldInMemory(m_iFieldIdx);

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return FALSE;
}

RwBool CNtlWSEFieldDelete::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEFieldCreate::CNtlWSEFieldCreate(RwInt32 iPrimaryKey, RwReal fUsedTime, RwInt32 iFieldIdx)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_iFieldIdx(iFieldIdx)
,m_pFile(NULL)
{
}

CNtlWSEFieldCreate::~CNtlWSEFieldCreate()
{
	if (m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
}

RwBool CNtlWSEFieldCreate::Begin()
{
	if (m_iFieldIdx == -1)
	{
		return FALSE;
	}

	CNtlWorldFieldManager*	pWFManager	= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pWFManager->GetFields());

	RwV3d vSPos = pFields[m_iFieldIdx].GetSPos();
	if(GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_TERRAIN)
	{
		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", m_iFieldIdx);

		static RwChar chPackPatch[NTL_MAX_DIR_PATH];
		strcpy_s(chPackPatch, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldProjectFolderName);
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "fields");
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldChar64Buf);
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldChar64Buf);

		SPackResFileData sPackFileData;
		RwBool bPack = GetNtlResourcePackManager()->LoadTerrain(chPackPatch, sPackFileData);
		if(bPack)
		{
			_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			::fopen_s(&m_pFile, sPackFileData.strPackFileName.c_str(), "rb");
			if(m_pFile)
			{
				fseek(m_pFile, sPackFileData.uiOffset, SEEK_SET);
			}
		}
	}
	else
	{
		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", m_iFieldIdx);

		_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
		_chdir("fields");
		_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

		::fopen_s(&m_pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb");
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	}

	DBO_ASSERT(m_pFile, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");

	if (m_pFile)
	{
		std::vector<CScheduleElement*>	vecScheduleElement[4];
		RwInt32							iIndex = 0;
		for(int l = (int)vSPos.z; l < (int)vSPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			for(int m = (int)vSPos.x; m < (int)vSPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
			{
				RwV3d vSectorPos;
				vSectorPos.x = (RwReal)m;
				vSectorPos.z = (RwReal)l;

				RwInt32				iSectorIdx		= pWFManager->GetSectorIdx(vSectorPos);
				CNtlWorldSector*	pNtlWorldSector = &pWFManager->m_pSectors[iSectorIdx];

				// Add Material HeightField Diffuse :: ::fseek(pFile, 0, SEEK_CUR);

				RwInt32				iFilePos			= 0;
				CScheduleElement*	pScheduleElement	= NULL;

				iFilePos			= ::ftell(m_pFile);


				// add material & height field & diffuse
				pScheduleElement	= NTL_NEW CNtlWSEMHD(0, 0.15f, pNtlWorldSector, m_pFile, iFilePos, &pFields[m_iFieldIdx].GetTexAttr());
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip material & height field & diffuse
				iFilePos			= g_FA.Skip_Material(m_pFile);
				iFilePos			= g_FA.Skip_Heightfield(m_pFile);
				iFilePos			= g_FA.Skip_Diffuse(m_pFile);

				RwInt32				iPrimaryKey = 0;

				// add doodads
				pScheduleElement	= NTL_NEW CNtlWSEDoodads(++iPrimaryKey, 0.15f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip doodads
				iFilePos			= g_FA.Skip_Doodads(m_pFile);

				// add water
				pScheduleElement	= NTL_NEW CNtlWSEWater(++iPrimaryKey, 0.05f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip water
				iFilePos			= g_FA.Skip_Water(m_pFile);

				// add shadow
				pScheduleElement	= NTL_NEW CNtlWSEShadow(++iPrimaryKey, 0.15f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip shadow
				iFilePos			= g_FA.Skip_Shadow(m_pFile);

				// add effect
				pScheduleElement	= NTL_NEW CNtlWSEEffect(++iPrimaryKey, 0.05f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip effect
				iFilePos			= g_FA.Skip_Effects(m_pFile);

				// add se
				pScheduleElement	= NTL_NEW CNtlWSESE(++iPrimaryKey, 0.05f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip se
				iFilePos			= g_FA.Skip_SE(m_pFile);

				// CZ-SS
				// add ss
				// 				pScheduleElement	= NTL_NEW CNtlWSESS(++iPrimaryKey, 0.05f, pNtlWorldSector, m_pFile, iFilePos);
				// 				vecScheduleElement[iIndex].push_back(pScheduleElement);
				// 
				// 				// skip ss
				// 				iFilePos			= g_FA.Skip_SS(m_pFile);

				// add sectorcull
				pScheduleElement	= NTL_NEW CNtlWSESectorCull(++iPrimaryKey, 0.00f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip sectorcull
				iFilePos			= g_FA.Skip_SectorCull(m_pFile);

				// add transparency
				pScheduleElement	= NTL_NEW CNtlWSETileTransparency(++iPrimaryKey, 0.00f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip tile transparency
				iFilePos			= g_FA.Skip_TileTransparency(m_pFile);

				// add shoreline
				pScheduleElement	= NTL_NEW CNtlWSEShoreLine(++iPrimaryKey, 0.05f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip shoreline
				iFilePos			= g_FA.Skip_Shoreline(m_pFile);

				// add decal
				pScheduleElement	= NTL_NEW CNtlWSEDecal(++iPrimaryKey, 0.05f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip decal
				iFilePos			= g_FA.Skip_Decal(m_pFile);

				// add plant
				pScheduleElement	= NTL_NEW CNtlWSEPlant(++iPrimaryKey, 0.05f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip plant
				iFilePos			= g_FA.Skip_Plant(m_pFile);

				// add world light
				pScheduleElement	= NTL_NEW CNtlWSEWorldLight(++iPrimaryKey, 0.05f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip world light
				iFilePos			= g_FA.Skip_WorldLight(m_pFile);

				// add occluder
				pScheduleElement	= NTL_NEW CNtlWSEOccluder(++iPrimaryKey, 0.05f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip occluder
				iFilePos			= g_FA.Skip_Occluder(m_pFile);

				// add heathaze
				pScheduleElement	= NTL_NEW CNtlWSEHeatHazeObject(++iPrimaryKey, 0.05f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip heathaze
				iFilePos			= g_FA.Skip_HeatHazeObject(m_pFile);

				// add light object
				pScheduleElement	= NTL_NEW CNtlWSELightObject(++iPrimaryKey, 0.05f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip light object
				iFilePos			= g_FA.Skip_LightObject(m_pFile);

				// add dojo
				pScheduleElement	= NTL_NEW CNtlWSEDojo(++iPrimaryKey, 0.05f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip dojo
				iFilePos			= g_FA.Skip_Dojo(m_pFile);


				++iIndex;
			}
		}

		for (int i = 0; i < (int)vecScheduleElement[0].size(); ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				m_Scheduler.AddSchedule(vecScheduleElement[j].at(i));
			}
			if (i == 0)
			{
				m_Scheduler.AddSchedule(NTL_NEW CNtlWSEField(99, 0.05f, m_iFieldIdx));
			}
		}
	}

	return TRUE;
}

RwBool CNtlWSEFieldCreate::Scheduling()
{
	return m_Scheduler.SchedulingProcess();
}

RwBool CNtlWSEFieldCreate::End()
{
	CNtlWorldFieldManager* pWFManager = GetSceneManager()->GetWorld()->GetWorldFieldMgr();

	if (m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEFieldCreateIndoor::CNtlWSEFieldCreateIndoor(RwInt32 iPrimaryKey, RwReal fUsedTime, RwInt32 iFieldIdx)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_iFieldIdx(iFieldIdx)
,m_pFile(NULL)
{
}

CNtlWSEFieldCreateIndoor::~CNtlWSEFieldCreateIndoor()
{
	if (m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
}

RwBool CNtlWSEFieldCreateIndoor::Begin()
{
	if (m_iFieldIdx == -1)
	{
		return FALSE;
	}

	CNtlWorldFieldManager*	pWFManager	= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pWFManager->GetFields());

	RwV3d vSPos = pFields[m_iFieldIdx].GetSPos();
	if(GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_TERRAIN)
	{
		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", m_iFieldIdx);

		static RwChar chPackPatch[NTL_MAX_DIR_PATH];
		strcpy_s(chPackPatch, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldProjectFolderName);
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "fields");
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldChar64Buf);
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, "\\");
		strcat_s(chPackPatch, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldChar64Buf);

		SPackResFileData sPackFileData;
		RwBool bPack = GetNtlResourcePackManager()->LoadTerrain(chPackPatch, sPackFileData);
		if(bPack)
		{
			_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			::fopen_s(&m_pFile, sPackFileData.strPackFileName.c_str(), "rb");
			if(m_pFile)
			{
				fseek(m_pFile, sPackFileData.uiOffset, SEEK_SET);
			}
		}
	}
	else
	{
		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", m_iFieldIdx);

		_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
		_chdir("fields");
		_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

		::fopen_s(&m_pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb");
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	}

	DBO_ASSERT(m_pFile, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");

	if (m_pFile)
	{
		std::vector<CScheduleElement*>	vecScheduleElement[4];
		RwInt32							iIndex = 0;
		for(int l = (int)vSPos.z; l < (int)vSPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			for(int m = (int)vSPos.x; m < (int)vSPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
			{
				RwV3d vSectorPos;
				vSectorPos.x = (RwReal)m;
				vSectorPos.z = (RwReal)l;

				RwInt32				iSectorIdx		= pWFManager->GetSectorIdx(vSectorPos);
				CNtlWorldSector*	pNtlWorldSector = &pWFManager->m_pSectors[iSectorIdx];

				// Add Material HeightField Diffuse :: ::fseek(pFile, 0, SEEK_CUR);

				RwInt32				iFilePos			= 0;
				CScheduleElement*	pScheduleElement	= NULL;

				iFilePos			= ::ftell(m_pFile);

				RwInt32				iPrimaryKey = 0;

				// add material & height field & diffuse
				pScheduleElement	= NTL_NEW CNtlWSEMHDIndoor(++iPrimaryKey, 0.15f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip material & height field & diffuse
				//iFilePos			= g_FA.Skip_Material(m_pFile);
				//iFilePos			= g_FA.Skip_Heightfield(m_pFile);
				//iFilePos			= g_FA.Skip_Diffuse(m_pFile);

				// add doodads
				pScheduleElement	= NTL_NEW CNtlWSEDoodadsIndoor(++iPrimaryKey, 0.15f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip doodads
				iFilePos			= g_FA.Skip_DoodadsIndoor(m_pFile);

				// add water
				pScheduleElement	= NTL_NEW CNtlWSEWater(++iPrimaryKey, 0.05f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip water
				iFilePos			= g_FA.Skip_Water(m_pFile);

				// add shadow
				pScheduleElement	= NTL_NEW CNtlWSEShadow(++iPrimaryKey, 0.15f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip shadow
				iFilePos			= g_FA.Skip_Shadow(m_pFile);

				// add effect
				pScheduleElement	= NTL_NEW CNtlWSEEffect(++iPrimaryKey, 0.05f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip effect
				iFilePos			= g_FA.Skip_Effects(m_pFile);

				// add se
				pScheduleElement	= NTL_NEW CNtlWSESE(++iPrimaryKey, 0.05f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip se
				iFilePos			= g_FA.Skip_SE(m_pFile);

				// CZ-SS
				// add ss
				// 				pScheduleElement	= NTL_NEW CNtlWSESS(++iPrimaryKey, 0.05f, pNtlWorldSector, m_pFile, iFilePos);
				// 				vecScheduleElement[iIndex].push_back(pScheduleElement);
				// 
				// 				// skip ss
				// 				iFilePos			= g_FA.Skip_SS(m_pFile);

				// add sectorcull
				pScheduleElement	= NTL_NEW CNtlWSESectorCull(++iPrimaryKey, 0.00f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip sectorcull
				iFilePos			= g_FA.Skip_SectorCull(m_pFile);

				// add transparency
				pScheduleElement	= NTL_NEW CNtlWSETileTransparency(++iPrimaryKey, 0.00f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip tile transparency
				iFilePos			= g_FA.Skip_TileTransparency(m_pFile);

				// add shoreline
				pScheduleElement	= NTL_NEW CNtlWSEShoreLine(++iPrimaryKey, 0.05f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip shoreline
				iFilePos			= g_FA.Skip_Shoreline(m_pFile);

				// add decal
				pScheduleElement	= NTL_NEW CNtlWSEDecal(++iPrimaryKey, 0.05f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip decal
				iFilePos			= g_FA.Skip_Decal(m_pFile);

				// add plant
				pScheduleElement	= NTL_NEW CNtlWSEPlant(++iPrimaryKey, 0.05f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip plant
				iFilePos			= g_FA.Skip_Plant(m_pFile);

				// add world light
				pScheduleElement	= NTL_NEW CNtlWSEWorldLight(++iPrimaryKey, 0.05f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip world light
				iFilePos			= g_FA.Skip_WorldLight(m_pFile);

				// add occluder
				pScheduleElement	= NTL_NEW CNtlWSEOccluder(++iPrimaryKey, 0.05f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip occluder
				iFilePos			= g_FA.Skip_Occluder(m_pFile);

				// add heathaze
				pScheduleElement	= NTL_NEW CNtlWSEHeatHazeObject(++iPrimaryKey, 0.05f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip heathaze
				iFilePos			= g_FA.Skip_HeatHazeObject(m_pFile);

				// add light object
				pScheduleElement	= NTL_NEW CNtlWSELightObject(++iPrimaryKey, 0.05f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip light object
				iFilePos			= g_FA.Skip_LightObject(m_pFile);

				// add decal volume
				pScheduleElement	= NTL_NEW CNtlWSEDecalVolume(++iPrimaryKey, 0.05f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip decal volume
				iFilePos			= g_FA.Skip_DecalVolume(m_pFile);

				// add game property
				pScheduleElement	= NTL_NEW CNtlWSEGameProperty(++iPrimaryKey, 0.05f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip game property
				iFilePos			= g_FA.Skip_GameProperty(m_pFile);

				// add dojo
				pScheduleElement	= NTL_NEW CNtlWSEDojo(++iPrimaryKey, 0.05f, pNtlWorldSector, m_pFile, iFilePos);
				vecScheduleElement[iIndex].push_back(pScheduleElement);

				// skip dojo
				iFilePos			= g_FA.Skip_Dojo(m_pFile);

				++iIndex;
			}
		}

		for (int i = 0; i < (int)vecScheduleElement[0].size(); ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				m_Scheduler.AddSchedule(vecScheduleElement[j].at(i));
			}
			// 			if (i == 0 && GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
			// 			{
			// 				m_Scheduler.AddSchedule(NTL_NEW CNtlWSEField(99, 0.05f, m_iFieldIdx));
			// 			}
		}
	}

	return TRUE;
}

RwBool CNtlWSEFieldCreateIndoor::Scheduling()
{
	return m_Scheduler.SchedulingProcess();
}

RwBool CNtlWSEFieldCreateIndoor::End()
{
	CNtlWorldFieldManager* pWFManager = GetSceneManager()->GetWorld()->GetWorldFieldMgr();

	if (m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEField::CNtlWSEField(RwInt32 iPrimaryKey, RwReal fUsedTime, RwInt32 iFieldIdx)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_iFieldIdx(iFieldIdx)
{
}

CNtlWSEField::~CNtlWSEField()
{
}

RwBool CNtlWSEField::Begin()
{	
	return TRUE;
}

RwBool CNtlWSEField::Scheduling()
{
	CNtlWorldFieldManager*	pWFManager	= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pWFManager->GetFields());

	if(!pWFManager->m_pFields[m_iFieldIdx].CreateFieldFromFile(m_iFieldIdx))
	{
		pWFManager->m_pFields[m_iFieldIdx].CreateFieldInMemory(m_iFieldIdx);
	}

	return FALSE;
}

RwBool CNtlWSEField::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEMHD::CNtlWSEMHD(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos, sCUR_FIELD_TEX_INFO* pFieldTexInfo)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
,m_pFieldTexInfo(pFieldTexInfo)
{
}

CNtlWSEMHD::~CNtlWSEMHD()
{
}

RwBool CNtlWSEMHD::Begin()
{	
	return TRUE;
}

RwBool CNtlWSEMHD::Scheduling()
{
	dNTL_WORLD_LOCAL(m_pNtlWorldSector->m_pWorldSector, pNtlSector) = static_cast<sNtlWorldSector*>(RwFreeListAlloc(dNTL_WORLD_GLOBAL(pSectorFreelist), rwMEMHINTDUR_GLOBAL));
	sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(m_pNtlWorldSector->m_pWorldSector, pNtlSector);

	pNtlSector->pNtlWorldSector = m_pNtlWorldSector;
	pNtlSector->pNtlWorldSector->MaterialContructor();

	m_pNtlWorldSector->m_bLoaded = TRUE;

	// move file pos
	fseek(m_pFile, m_iFilePos, SEEK_SET);

	RpGeometry*				pGeometry;
	RpMaterial*				pMaterial;
	RwSurfaceProperties		SurfProp;
	RpMorphTarget*			pMorphTarget;
	RwSphere				BoundingSphere;
	RwTexCoords*			pTexCoord;
	RwV3d*					pVList;
	RpTriangle*				pTList;
	RwRGBA*					pPrelights;

	pGeometry			= RpGeometryCreate(dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, dGET_WORLD_PARAM()->WorldSectorPolyNum, rpGEOMETRYPOSITIONS | rpGEOMETRYPRELIT | rpGEOMETRYTEXTURED);
	pMorphTarget		= RpGeometryGetMorphTarget(pGeometry, 0);
	pMaterial			= RpMaterialCreate();
	SurfProp.ambient	= 1.0f;
	SurfProp.diffuse	= 1.0f;
	SurfProp.specular	= 1.0f;
	RpMaterialSetSurfaceProperties(pMaterial, &SurfProp);

	pPrelights = RpGeometryGetPreLightColors(pGeometry);
	pVList = RpMorphTargetGetVertices(pMorphTarget);
	pTList = RpGeometryGetTriangles(pGeometry);
	pTexCoord = RpGeometryGetVertexTexCoords(pGeometry, rwTEXTURECOORDINATEINDEX0);

	_RpNtlWorldSectorMaterialExtensionRead(*m_pFieldTexInfo, m_pNtlWorldSector, pMaterial, m_pFile);

	// load heightfield info.
	fread(pVList ,sizeof(RwV3d) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, 1, m_pFile);

	// load prelit
	fread(pPrelights, sizeof(RwRGBA) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, 1, m_pFile);

	RwV2d SPos, EPos;
	SPos.x = m_pNtlWorldSector->m_pWorldSector->boundingBox.inf.x;
	SPos.y = m_pNtlWorldSector->m_pWorldSector->boundingBox.inf.z;
	EPos.x = m_pNtlWorldSector->m_pWorldSector->boundingBox.sup.x;
	EPos.y = m_pNtlWorldSector->m_pWorldSector->boundingBox.sup.z;

	// 4 lod indexing
	RwInt32 NPosSXBy0 = (RwInt32)(EPos.x + (float)dGET_WORLD_PARAM()->WorldSizeHalf);
	RwInt32 NPosEXBy0 = (RwInt32)(SPos.x + (float)dGET_WORLD_PARAM()->WorldSizeHalf);

	for(int i = (RwInt32)SPos.y; i <= (RwInt32)EPos.y; i += dGET_WORLD_PARAM()->WorldSectorTileSize)
	{
		for(int j = NPosSXBy0; j >= NPosEXBy0; j -= dGET_WORLD_PARAM()->WorldSectorTileSize)
		{
			RwInt32 m = (RwInt32)((pVList->z - SPos.y) / dGET_WORLD_PARAM()->WorldSectorTileSize);
			RwInt32 n = (RwInt32)((pVList->x - SPos.x) / dGET_WORLD_PARAM()->WorldSectorTileSize);
			pTexCoord->u = NtlCommonTexCoord[n + m * dGET_WORLD_PARAM()->WorldSectorVertNum].u;
			pTexCoord->v = NtlCommonTexCoord[n + m * dGET_WORLD_PARAM()->WorldSectorVertNum].v;

			pPrelights++;
			pVList++;
			pTexCoord++;
		}
	}

	RwInt16 CurPtIndices[4];
	for(int i = 0; i < dGET_WORLD_PARAM()->WorldSectorTileNum; ++i)
	{
		for(int j = 0; j < dGET_WORLD_PARAM()->WorldSectorTileNum; ++j)
		{
			CurPtIndices[0] = j + (i * dGET_WORLD_PARAM()->WorldSectorVertNum);
			CurPtIndices[1] = CurPtIndices[0] + 1;
			CurPtIndices[3] = CurPtIndices[0] + dGET_WORLD_PARAM()->WorldSectorVertNum;
			CurPtIndices[2] = CurPtIndices[3] + 1;

			RpGeometryTriangleSetVertexIndices(pGeometry, pTList, CurPtIndices[0], CurPtIndices[2], CurPtIndices[3]);
			RpGeometryTriangleSetMaterial(pGeometry, pTList++, pMaterial);
			RpGeometryTriangleSetVertexIndices(pGeometry, pTList, CurPtIndices[2], CurPtIndices[0], CurPtIndices[1]);
			RpGeometryTriangleSetMaterial(pGeometry, pTList++, pMaterial);
		}
	}

	RpMorphTargetCalcBoundingSphere(pMorphTarget, &BoundingSphere);
	RpMorphTargetSetBoundingSphere(pMorphTarget, &BoundingSphere);

	RpGeometryUnlock(pGeometry);

	m_pNtlWorldSector->m_pAtomic = RpAtomicCreate();

	RpAtomicSetPipeline(m_pNtlWorldSector->m_pAtomic, D3D9NtlWorldSectorAtomicPipeline);
	RpAtomicSetFrame(m_pNtlWorldSector->m_pAtomic, dNTL_WORLD_SECTOR_LOCAL(RpWorldSectorGetWorld(m_pNtlWorldSector->m_pWorldSector), pParentFrame));
	RpAtomicSetGeometry(m_pNtlWorldSector->m_pAtomic, pGeometry, 0);

	RpGeometryDestroy(pGeometry);
	RpMaterialDestroy(pMaterial);

	return FALSE;
}

RwBool CNtlWSEMHD::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEMHDIndoor::CNtlWSEMHDIndoor(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSEMHDIndoor::~CNtlWSEMHDIndoor()
{
}

RwBool CNtlWSEMHDIndoor::Begin()
{	
	return TRUE;
}

RwBool CNtlWSEMHDIndoor::Scheduling()
{
	m_pNtlWorldSector->MaterialContructor();

	m_pNtlWorldSector->m_bLoaded = TRUE;

	return FALSE;
}

RwBool CNtlWSEMHDIndoor::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEDoodads::CNtlWSEDoodads(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSEDoodads::~CNtlWSEDoodads()
{
}

RwBool CNtlWSEDoodads::Begin()
{	
	return TRUE;
}

RwBool CNtlWSEDoodads::Scheduling()
{
	// file pos move
	fseek(m_pFile, m_iFilePos, SEEK_SET);

	// map obj.; ornament
	RwInt32 NumMapObj;
	fread(&NumMapObj, sizeof(RwInt32), 1, m_pFile);

	RwV3d		SRT[3];
	RwChar		Name[128];
	RwBool		EnableFlag;
	RwUInt32	SID;
#ifdef dNTL_WORLD_FORMAT_CONVERT_COMPLTE
	RwUInt32	ObjectID;
#endif
	RwReal		VisibilityDist;
	RwUInt32	MilePostGUID = 0;

	SPLObjectCreateParam PLEntityCreateParam;
	CNtlPLEntity *pNtlPLEntity = NULL;
	m_pNtlWorldSector->m_vecNtlPLEntityOrnament.clear();

#ifndef dNTL_WORLD_TOOL_MODE

	sNTL_EVENT_TRI_DOODADS	NtlEventTriDoodads;

	if(NumMapObj)
	{
		NtlEventTriDoodads.FlgTriDoodads = 0;
		NtlEventTriDoodads.vecDat.reserve(10);
	}
#endif

	for(int i = 0; i < NumMapObj; ++i)
	{
		fread(Name, sizeof(RwChar) * 128, 1, m_pFile);
		fread(&SRT[0], sizeof(RwV3d), 1, m_pFile);
		fread(&SRT[1], sizeof(RwV3d), 1, m_pFile);
		fread(&SRT[2], sizeof(RwV3d), 1, m_pFile);

		RwUInt32 uiObjectType = EPL_OBJECT_TYPE_OUTDOOR_OBJECT;
		fread(&uiObjectType, sizeof(RwUInt32), 1, m_pFile);

		PLEntityCreateParam.pPos		= &SRT[2];
		PLEntityCreateParam.bLoadMap	= dGET_WORLD_PARAM()->Loading;

		RwBool bLoadScheduling = CNtlPLResourceManager::GetInstance()->IsLoadScheduling();
		if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
			uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
		{
			PLEntityCreateParam.bLoadMap = TRUE;
			CNtlPLResourceManager::GetInstance()->SetLoadScheduling(FALSE);
		}		

		pNtlPLEntity = static_cast<CNtlPLEntity*>(GetSceneManager()->CreateEntity(PLENTITY_OBJECT, Name, &PLEntityCreateParam));
		DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

		CNtlPLResourceManager::GetInstance()->SetLoadScheduling(bLoadScheduling);

		static_cast<CNtlPLObject*>(pNtlPLEntity)->SetObjectType(uiObjectType);
		if (static_cast<CNtlPLObject*>(pNtlPLEntity)->GetObjectTypePtr())
		{
			static_cast<CNtlPLObject*>(pNtlPLEntity)->GetObjectTypePtr()->LoadFromFile(m_pFile);
		}	

		if(static_cast<CNtlPLObject*>(pNtlPLEntity)->GetPlayAnim())
		{
			static_cast<CNtlPLObject*>(pNtlPLEntity)->SetPlayAnimTime(NtlRandomNumber(0.0f, 1.0f));
		}

#ifdef dNTL_WORLD_TOOL_MODE
		{
			// If current doodads visual mode is set
			//pNtlPLEntity->SetVisible(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_DOODADS));

			// doodads visibility distance culling mode setting
			static_cast<CNtlPLObject*>(pNtlPLEntity)->SetFadeEnable(TRUE);
		}
#endif

		static_cast<CNtlPLObject*>(pNtlPLEntity)->SetScale(&SRT[0]);
		static_cast<CNtlPLObject*>(pNtlPLEntity)->SetRotate(SRT[1].x, SRT[1].y, SRT[1].z);
		static_cast<CNtlPLObject*>(pNtlPLEntity)->SetPosition(&SRT[2]);

		// serial id
		fread(&SID, sizeof(RwUInt32), 1, m_pFile);
		pNtlPLEntity->SetSID(SID);

		// object id
#ifdef dNTL_WORLD_FORMAT_CONVERT_COMPLTE
		fread(&ObjectID, sizeof(RwUInt32), 1, m_pFile);
		static_cast<CNtlPLObject*>(pNtlPLEntity)->SetObjectSerialID(ObjectID);
#endif

		// object name index
		DWORD ObjNameIdx;
		fread(&ObjNameIdx, sizeof(DWORD), 1, m_pFile);
		pNtlPLEntity->SetObjNameIdx(ObjNameIdx);

		// visibility distance
		fread(&VisibilityDist, sizeof(RwReal), 1, m_pFile);
		pNtlPLEntity->SetVisibleCullingDistance(VisibilityDist);

		// enable trigger
		fread(&EnableFlag, sizeof(RwBool), 1, m_pFile);
		if(EnableFlag)
		{
			pNtlPLEntity->SetFlags(pNtlPLEntity->GetFlags() | NTL_PLEFLAG_TRIGGER);

#ifndef dNTL_WORLD_TOOL_MODE
			NtlEventTriDoodads.vecDat.push_back(pNtlPLEntity);
#endif
		}


		// enable selection
		fread(&EnableFlag, sizeof(RwBool), 1, m_pFile);
		if(EnableFlag)
		{
			pNtlPLEntity->SetFlags(pNtlPLEntity->GetFlags() | NTL_PLEFLAG_SELECTION);
		}

		m_pNtlWorldSector->m_vecNtlPLEntityOrnament.push_back(pNtlPLEntity);

		// enable alterable; dynamic obj.s.
		fread(&EnableFlag, sizeof(RwBool), 1, m_pFile);
		if(EnableFlag)
		{
			pNtlPLEntity->SetFlags(pNtlPLEntity->GetFlags() | NTL_PLEFLAG_ALTERABLE);
		}

		// enable portal
		fread(&EnableFlag, sizeof(RwBool), 1, m_pFile);
		if(EnableFlag)
		{
			pNtlPLEntity->SetFlags(pNtlPLEntity->GetFlags() | NTL_PLEFLAG_PORTAL);
		}

		// enable path engine collision
		fread(&EnableFlag, sizeof(RwBool), 1, m_pFile);
		if(EnableFlag)
		{
			pNtlPLEntity->SetFlags(pNtlPLEntity->GetFlags() | NTL_PLEFLAG_PECOLLISION);
		}

		// enable shadow prop
		fread(&EnableFlag, sizeof(RwBool), 1, m_pFile);
		if(EnableFlag)
		{
			pNtlPLEntity->SetFlags(pNtlPLEntity->GetFlags() | NTL_PLEFLAG_SHADOW_PROP);
		}

		// object PS map
		static_cast<CNtlPLObject*>(pNtlPLEntity)->LoadPSMap(m_pFile);

		if(!GetNtlResourceManager()->IsLoadScheduling())
		{
			static_cast<CNtlPLObject*>(pNtlPLEntity)->SetAtomicPSMap();
		}

		// milepost GUID
		fread(&EnableFlag, sizeof(RwBool), 1, m_pFile);
		if(EnableFlag)
		{
			fread(&MilePostGUID, sizeof(RwUInt32), 1, m_pFile);
			static_cast<CNtlPLObject*>(pNtlPLEntity)->SetMilepostID(MilePostGUID);
		}

		// attach lightojbect
		static_cast<CNtlPLObject*>(pNtlPLEntity)->LoadLightObjectAttachData(m_pFile);
	}
#ifdef dNTL_WORLD_TOOL_MODE
	CNtlPLObject::LoadSwapFile(m_pNtlWorldSector->DatumPoint.x, m_pNtlWorldSector->DatumPoint.y, m_pNtlWorldSector->DatumPoint.z);
#endif

#ifndef dNTL_WORLD_TOOL_MODE

	if(NtlEventTriDoodads.vecDat.size())
	{
		CNtlPLEventGenerator::OnTriDoodadsEvent(&NtlEventTriDoodads);
		NtlEventTriDoodads.vecDat.clear();
	}
#endif

	return FALSE;
}

RwBool CNtlWSEDoodads::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEDoodadsIndoor::CNtlWSEDoodadsIndoor(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSEDoodadsIndoor::~CNtlWSEDoodadsIndoor()
{
}

RwBool CNtlWSEDoodadsIndoor::Begin()
{	
	return TRUE;
}

RwBool CNtlWSEDoodadsIndoor::Scheduling()
{
	// file pos move
	fseek(m_pFile, m_iFilePos, SEEK_SET);

	// map obj.; ornament
	RwInt32 NumMapObj;
	fread(&NumMapObj, sizeof(RwInt32), 1, m_pFile);

	RwV3d		SRT[3];
	RwChar		Name[128];
	RwBool		EnableFlag;
	RwUInt32	SID;
#ifdef dNTL_WORLD_FORMAT_CONVERT_COMPLTE
	RwUInt32	ObjectID;
#endif
	RwReal		VisibilityDist;
	RwUInt32	MilePostGUID = 0;

	SPLObjectCreateParam PLEntityCreateParam;
	CNtlPLEntity *pNtlPLEntity = NULL;
	m_pNtlWorldSector->m_vecNtlPLEntityOrnament.clear();

#ifndef dNTL_WORLD_TOOL_MODE

	sNTL_EVENT_TRI_DOODADS	NtlEventTriDoodads;

	if(NumMapObj)
	{
		NtlEventTriDoodads.FlgTriDoodads = 0;
		NtlEventTriDoodads.vecDat.reserve(10);
	}
#endif

	for(int i = 0; i < NumMapObj; ++i)
	{
		fread(Name, sizeof(RwChar) * 128, 1, m_pFile);
		fread(&SRT[0], sizeof(RwV3d), 1, m_pFile);
		fread(&SRT[1], sizeof(RwV3d), 1, m_pFile);
		fread(&SRT[2], sizeof(RwV3d), 1, m_pFile);

		RwUInt32 uiObjectType = EPL_OBJECT_TYPE_OUTDOOR_OBJECT;
		fread(&uiObjectType, sizeof(RwUInt32), 1, m_pFile);

		PLEntityCreateParam.pPos		= &SRT[2];
		PLEntityCreateParam.bLoadMap	= dGET_WORLD_PARAM()->Loading;

		RwBool bLoadScheduling = CNtlPLResourceManager::GetInstance()->IsLoadScheduling();
		if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
			uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
		{
			PLEntityCreateParam.bLoadMap = TRUE;
			CNtlPLResourceManager::GetInstance()->SetLoadScheduling(FALSE);
		}		

		pNtlPLEntity = static_cast<CNtlPLEntity*>(GetSceneManager()->CreateEntity(PLENTITY_OBJECT, Name, &PLEntityCreateParam));
		DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

		CNtlPLResourceManager::GetInstance()->SetLoadScheduling(bLoadScheduling);

		static_cast<CNtlPLObject*>(pNtlPLEntity)->SetObjectType(uiObjectType);
		if (static_cast<CNtlPLObject*>(pNtlPLEntity)->GetObjectTypePtr())
		{
			static_cast<CNtlPLObject*>(pNtlPLEntity)->GetObjectTypePtr()->LoadFromFile(m_pFile);
		}	


		if(static_cast<CNtlPLObject*>(pNtlPLEntity)->GetPlayAnim())
		{
			static_cast<CNtlPLObject*>(pNtlPLEntity)->SetPlayAnimTime(NtlRandomNumber(0.0f, 1.0f));
		}

#ifdef dNTL_WORLD_TOOL_MODE
		{
			// If current doodads visual mode is set
			//pNtlPLEntity->SetVisible(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_DOODADS));

			// doodads visibility distance culling mode setting
			static_cast<CNtlPLObject*>(pNtlPLEntity)->SetFadeEnable(TRUE);
		}
#endif

		static_cast<CNtlPLObject*>(pNtlPLEntity)->SetScale(&SRT[0]);
		static_cast<CNtlPLObject*>(pNtlPLEntity)->SetRotate(SRT[1].x, SRT[1].y, SRT[1].z);
		static_cast<CNtlPLObject*>(pNtlPLEntity)->SetPosition(&SRT[2]);

		// serial id
		fread(&SID, sizeof(RwUInt32), 1, m_pFile);
		pNtlPLEntity->SetSID(SID);

		// object id
#ifdef dNTL_WORLD_FORMAT_CONVERT_COMPLTE
		fread(&ObjectID, sizeof(RwUInt32), 1, m_pFile);
		static_cast<CNtlPLObject*>(pNtlPLEntity)->SetObjectSerialID(ObjectID);
#endif

		// object name index
		DWORD ObjNameIdx;
		fread(&ObjNameIdx, sizeof(DWORD), 1, m_pFile);
		pNtlPLEntity->SetObjNameIdx(ObjNameIdx);

		// visibility distance
		fread(&VisibilityDist, sizeof(RwReal), 1, m_pFile);
		pNtlPLEntity->SetVisibleCullingDistance(VisibilityDist);

		// enable trigger
		fread(&EnableFlag, sizeof(RwBool), 1, m_pFile);
		if(EnableFlag)
		{
			pNtlPLEntity->SetFlags(pNtlPLEntity->GetFlags() | NTL_PLEFLAG_TRIGGER);

#ifndef dNTL_WORLD_TOOL_MODE
			NtlEventTriDoodads.vecDat.push_back(pNtlPLEntity);
#endif
		}


		// enable selection
		fread(&EnableFlag, sizeof(RwBool), 1, m_pFile);
		if(EnableFlag)
		{
			pNtlPLEntity->SetFlags(pNtlPLEntity->GetFlags() | NTL_PLEFLAG_SELECTION);
		}

		m_pNtlWorldSector->m_vecNtlPLEntityOrnament.push_back(pNtlPLEntity);

		// enable alterable; dynamic obj.s.
		fread(&EnableFlag, sizeof(RwBool), 1, m_pFile);
		if(EnableFlag)
		{
			pNtlPLEntity->SetFlags(pNtlPLEntity->GetFlags() | NTL_PLEFLAG_ALTERABLE);
		}

		// enable portal
		fread(&EnableFlag, sizeof(RwBool), 1, m_pFile);
		if(EnableFlag)
		{
			pNtlPLEntity->SetFlags(pNtlPLEntity->GetFlags() | NTL_PLEFLAG_PORTAL);
		}

		// enable path engine collision
		fread(&EnableFlag, sizeof(RwBool), 1, m_pFile);
		if(EnableFlag)
		{
			pNtlPLEntity->SetFlags(pNtlPLEntity->GetFlags() | NTL_PLEFLAG_PECOLLISION);
		}

		// enable shadow prop
		fread(&EnableFlag, sizeof(RwBool), 1, m_pFile);
		if(EnableFlag)
		{
			pNtlPLEntity->SetFlags(pNtlPLEntity->GetFlags() | NTL_PLEFLAG_SHADOW_PROP);
		}

		// object PS map
		static_cast<CNtlPLObject*>(pNtlPLEntity)->LoadPSMap(m_pFile);

		if(!GetNtlResourceManager()->IsLoadScheduling())
		{
			static_cast<CNtlPLObject*>(pNtlPLEntity)->SetAtomicPSMap();
		}

		// milepost GUID
		fread(&EnableFlag, sizeof(RwBool), 1, m_pFile);
		if(EnableFlag)
		{
			fread(&MilePostGUID, sizeof(RwUInt32), 1, m_pFile);
			static_cast<CNtlPLObject*>(pNtlPLEntity)->SetMilepostID(MilePostGUID);
		}

		// attach lightojbect
		static_cast<CNtlPLObject*>(pNtlPLEntity)->LoadLightObjectAttachData(m_pFile);

		// Illumination
		if(GetNtlResourceManager()->IsLoadScheduling())
			static_cast<CNtlPLObject*>(pNtlPLEntity)->LoadSchedulingLightmapProp(m_pFile);
		else
			static_cast<CNtlPLObject*>(pNtlPLEntity)->LoadLightmap(m_pFile);
	}
#ifdef dNTL_WORLD_TOOL_MODE
	CNtlPLObject::LoadSwapFile(m_pNtlWorldSector->DatumPoint.x, m_pNtlWorldSector->DatumPoint.y, m_pNtlWorldSector->DatumPoint.z);
#endif

#ifndef dNTL_WORLD_TOOL_MODE

	if(NtlEventTriDoodads.vecDat.size())
	{
		CNtlPLEventGenerator::OnTriDoodadsEvent(&NtlEventTriDoodads);
		NtlEventTriDoodads.vecDat.clear();
	}
#endif

	return FALSE;
}

RwBool CNtlWSEDoodadsIndoor::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEWater::CNtlWSEWater(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSEWater::~CNtlWSEWater()
{
}

RwBool CNtlWSEWater::Begin()
{	
	return TRUE;
}

RwBool CNtlWSEWater::Scheduling()
{
	// file pos move
	fseek(m_pFile, m_iFilePos, SEEK_SET);

	// water
	RwBool BooleanFlag;
	::fread(&BooleanFlag, sizeof(RwBool), 1, m_pFile);

	if(BooleanFlag)
	{
		sSECTOR_WATER_ATTR SectorWaterAttr;
		::fread(&SectorWaterAttr._Height,						sizeof(RwReal), 1, m_pFile);
		::fread(&SectorWaterAttr._RGBA,							sizeof(RwRGBA), 1, m_pFile);
		::fread(&SectorWaterAttr._RenderstateSrc,				sizeof(DWORD), 1, m_pFile);
		::fread(&SectorWaterAttr._RenderstateDst,				sizeof(DWORD), 1, m_pFile);
		::fread(&SectorWaterAttr._RotTrans,						sizeof(RwReal), 1, m_pFile);
		::fread(&SectorWaterAttr._SclTrans,						sizeof(RwReal), 1, m_pFile);
		::fread(&SectorWaterAttr._DepthMapSize,					sizeof(RwInt32), 1, m_pFile);
		::fread(&SectorWaterAttr._DepthMapHeightVariation[0],	sizeof(RwReal), 1, m_pFile);
		::fread(&SectorWaterAttr._DepthMapHeightVariation[1],	sizeof(RwReal), 1, m_pFile);
		::fread(&SectorWaterAttr._IdxSequence,					sizeof(RwInt32), 1, m_pFile);

		CNtlPLVisualManager* pSceneManager = static_cast<CNtlPLVisualManager*>(GetSceneManager());
		pSceneManager->GetWater()->OnCreate(m_pNtlWorldSector, SectorWaterAttr);

		::fread(&BooleanFlag, sizeof(RwBool), 1, m_pFile);
		if(BooleanFlag)
		{
			m_pNtlWorldSector->m_pWater->_pDepthMap = CNtlPLResourceManager::GetInstance()->CreateTexture("DepthMap",
				SectorWaterAttr._DepthMapSize, SectorWaterAttr._DepthMapSize, 32, rwRASTERFORMAT8888);

			RwTextureSetAddressing(m_pNtlWorldSector->m_pWater->_pDepthMap, rwTEXTUREADDRESSCLAMP);
			RwTextureSetFilterMode(m_pNtlWorldSector->m_pWater->_pDepthMap, rwFILTERLINEAR);

			BYTE* pTmp = NTL_NEW BYTE [SectorWaterAttr._DepthMapSize * SectorWaterAttr._DepthMapSize];
			::fread(pTmp, sizeof(BYTE) * SectorWaterAttr._DepthMapSize * SectorWaterAttr._DepthMapSize, 1, m_pFile);
			CNtlPLResourceManager::GetInstance()->LoadAlphaBits(m_pNtlWorldSector->m_pWater->_pDepthMap, pTmp);
			NTL_ARRAY_DELETE(pTmp);
		}
	}

	return FALSE;
}

RwBool CNtlWSEWater::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEShadow::CNtlWSEShadow(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSEShadow::~CNtlWSEShadow()
{
}

RwBool CNtlWSEShadow::Begin()
{	
	return TRUE;
}

RwBool CNtlWSEShadow::Scheduling()
{
	// file pos move
	fseek(m_pFile, m_iFilePos, SEEK_SET);

	// shadow
	RwBool BooleanFlag;

	::fread(&BooleanFlag, sizeof(RwBool), 1, m_pFile);
	if(BooleanFlag)
	{
		CNtlWorldShadow* pNtlWorldShadow = m_pNtlWorldSector->m_pNtlWorldShadow;

		::fread(&pNtlWorldShadow->m_rgbaShadow, sizeof(RwRGBA), 1, m_pFile);
		::fread(&pNtlWorldShadow->m_CamViewWindowX, sizeof(RwReal), 1, m_pFile);
		::fread(&pNtlWorldShadow->m_matLight, sizeof(RwMatrix), 1, m_pFile);

		RwInt32 Size;
		::fread(&Size, sizeof(RwInt32), 1, m_pFile);

		pNtlWorldShadow->m_pTexShadow = CNtlPLResourceManager::GetInstance()->CreateTexture("ShadowMap",
			Size,
			Size,
			32,
			rwRASTERFORMAT8888);

		RwTextureSetFilterMode(pNtlWorldShadow->m_pTexShadow, rwFILTERLINEAR);
		RwTextureSetAddressing(pNtlWorldShadow->m_pTexShadow, rwTEXTUREADDRESSCLAMP);

		CNtlPLResourceManager::GetInstance()->LoadTexture(pNtlWorldShadow->m_pTexShadow, m_pFile);
		DBO_ASSERT(pNtlWorldShadow->m_pTexShadow, "Texture load failed.");
	}	

	return FALSE;
}

RwBool CNtlWSEShadow::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEEffect::CNtlWSEEffect(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSEEffect::~CNtlWSEEffect()
{
}

RwBool CNtlWSEEffect::Begin()
{	
	return TRUE;
}

RwBool CNtlWSEEffect::Scheduling()
{
	// file pos move
	fseek(m_pFile, m_iFilePos, SEEK_SET);

	RwV3d		SRT[3];
	RwChar		Name[128];
	RwReal		VisibilityDist;
	RwUInt32	MilePostGUID = 0;

	SPLEntityCreateParam PLEntityCreateParam;
	CNtlPLEntity *pNtlPLEntity = NULL;

	// effects
	RwInt32 NumEff;
	fread(&NumEff, sizeof(RwInt32), 1, m_pFile);

	for(int i = 0; i < NumEff; ++i)
	{
		fread(Name, sizeof(RwChar) * 128, 1, m_pFile);
		fread(&SRT[0], sizeof(RwV3d), 1, m_pFile);
		fread(&SRT[1], sizeof(RwV3d), 1, m_pFile);
		fread(&SRT[2], sizeof(RwV3d), 1, m_pFile);

		PLEntityCreateParam.pPos = &SRT[2];
		pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, Name, &PLEntityCreateParam);
		DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

#ifdef dNTL_WORLD_TOOL_MODE

		pNtlPLEntity->SetVisible(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_EFFECTS));

		if(dGET_CUR_PALETTE() == ePM_EFFECT)
		{
			(static_cast<CNtlInstanceEffect*>(pNtlPLEntity))->SetVisibleBoundingVolume(TRUE);
		}
		else
		{
			(static_cast<CNtlInstanceEffect*>(pNtlPLEntity))->SetVisibleBoundingVolume(FALSE);
		}

#endif

		pNtlPLEntity->SetScale(SRT[0].x);
		pNtlPLEntity->SetRotate(SRT[1].x, SRT[1].y, SRT[1].z);
		pNtlPLEntity->SetPosition(&SRT[2]);

		// visibility distance
		fread(&VisibilityDist, sizeof(RwReal), 1, m_pFile);
		pNtlPLEntity->SetVisibleCullingDistance(VisibilityDist);

		m_pNtlWorldSector->m_vecNtlPLEntityEffect.push_back(pNtlPLEntity);
	}
#ifdef dNTL_WORLD_TOOL_MODE
	CNtlInstanceEffect::LoadSwapFile(m_pNtlWorldSector->DatumPoint.x, m_pNtlWorldSector->DatumPoint.y, m_pNtlWorldSector->DatumPoint.z);
#endif

	return FALSE;
}

RwBool CNtlWSEEffect::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSESE::CNtlWSESE(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSESE::~CNtlWSESE()
{
}

RwBool CNtlWSESE::Begin()
{	
	return TRUE;
}

RwBool CNtlWSESE::Scheduling()
{
	// file pos move
	fseek(m_pFile, m_iFilePos, SEEK_SET);

	SPLEntityCreateParam PLEntityCreateParam;

	// enviroment sound		
	RwInt32 NumSound;
	fread(&NumSound, sizeof(RwInt32), 1, m_pFile);

	for(int i = 0; i < NumSound; ++i)
	{
		RwV3d	Pos;		
		RwReal	Vol;
		RwReal	RadiusMin, RadiusMax;
		RwChar  NameSound[32] = {0,};

		fread(NameSound, sizeof(RwChar) * 32, 1, m_pFile);
		fread(&Vol, sizeof(RwReal), 1, m_pFile);
		fread(&RadiusMin, sizeof(RwReal), 1, m_pFile);
		fread(&RadiusMax, sizeof(RwReal), 1, m_pFile);
		fread(&Pos, sizeof(RwV3d), 1, m_pFile);

		PLEntityCreateParam.pPos = &Pos;

		CNtlPLEntity *pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_SOUND, "NULL", &PLEntityCreateParam);
		DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

		static_cast<CNtlPLSound*>(pNtlPLEntity)->GetVolume() = Vol;
		static_cast<CNtlPLSound*>(pNtlPLEntity)->GetRadius(TRUE) = RadiusMin;
		static_cast<CNtlPLSound*>(pNtlPLEntity)->GetRadius(FALSE) = RadiusMax;
		static_cast<CNtlPLSound*>(pNtlPLEntity)->PlaySound(NameSound);
		m_pNtlWorldSector->m_vecNtlPLEntitySound.push_back(pNtlPLEntity);
	}

	// bgm sound
	// CZ-BGM
	RwInt32 NumSoundBGM;
	fread(&NumSoundBGM, sizeof(RwInt32), 1, m_pFile);

	for(int i = 0; i < NumSoundBGM; ++i)
	{
		RwV3d	Pos;		
		//RwReal	Vol;
		RwReal	Radius;
		RwChar  NameSound[32] = {0,};

		fread(NameSound, sizeof(RwChar) * 32, 1, m_pFile);
		//fread(&Vol, sizeof(RwReal), 1, m_pFile);
		fread(&Radius, sizeof(RwReal), 1, m_pFile);
		fread(&Pos, sizeof(RwV3d), 1, m_pFile);

		PLEntityCreateParam.pPos = &Pos;

		CNtlPLEntity *pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_SOUND_BGM, "NULL", &PLEntityCreateParam);
		DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

		//static_cast<CNtlPLSoundBGM*>(pNtlPLEntity)->GetVolume() = Vol;
		static_cast<CNtlPLSoundBGM*>(pNtlPLEntity)->GetRadius() = Radius;
		static_cast<CNtlPLSoundBGM*>(pNtlPLEntity)->PlaySound(NameSound);
		m_pNtlWorldSector->m_vecNtlPLEntitySoundBGM.push_back(pNtlPLEntity);
	}

	return FALSE;
}

RwBool CNtlWSESE::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// CZ-SS
// CNtlWSESS::CNtlWSESS(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
// :CScheduleElement(iPrimaryKey, fUsedTime)
// ,m_pNtlWorldSector(pNtlWorldSector)
// ,m_pFile(pFile)
// ,m_iFilePos(iFilePos)
// {
// }
// 
// CNtlWSESS::~CNtlWSESS()
// {
// }
// 
// RwBool CNtlWSESS::Begin()
// {	
// 	return TRUE;
// }
// 
// RwBool CNtlWSESS::Scheduling()
// {
// 	// file pos move
// 	fseek(m_pFile, m_iFilePos, SEEK_SET);
// 
// 	// soft edged spot light
// 	RwV3d		SSPos;
// 	RwV3d		SSRot;
// 	RwInt32		SSNum = 0;
// 	RwRGBAReal	SSClr;
// 	RwReal		SSAng, SSRad;
// 
// 	fread(&SSNum, sizeof(RwInt32), 1, m_pFile);
// 
// 	for(int i = 0; i < SSNum; ++i)
// 	{
// 		fread(&SSPos, sizeof(RwV3d), 1, m_pFile);
// 		fread(&SSRot, sizeof(RwV3d), 1, m_pFile);
// 		fread(&SSClr, sizeof(RwRGBAReal), 1, m_pFile);
// 		fread(&SSAng, sizeof(RwReal), 1, m_pFile);
// 		fread(&SSRad, sizeof(RwReal), 1, m_pFile);
// 
// 		SPLEntityCreateParam CreateParam;
// 		CreateParam.pPos = &SSPos;
// 
// 		CNtlPLEntity* pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_SPOT_LIGHT, NTL_PLEN_SPOT_LIGHT, &CreateParam);
// 		DBO_ASSERT(pNtlPLEntity, "Entity create failed.");
// 
// 		CNtlPLSpot*	pSpotEntity	= static_cast<CNtlPLSpot*>(pNtlPLEntity);
// 
// 		pSpotEntity->SetColor(SSClr);
// 		pSpotEntity->SetAngle(SSAng);
// 		pSpotEntity->SetRadius(SSRad);
// 		pSpotEntity->SetPosition(&SSPos);
// 		pSpotEntity->SetRotate(SSRot.x, SSRot.y, SSRot.z);
// 
// 
// 		m_pNtlWorldSector->m_vecNtlPLEntitySS.push_back(pNtlPLEntity);
// 	}
// 
// 	return FALSE;
// }
// 
// RwBool CNtlWSESS::End()
// {
// 	return TRUE;
// }

//////////////////////////////////////////////////////////////////////////

CNtlWSESectorCull::CNtlWSESectorCull(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSESectorCull::~CNtlWSESectorCull()
{
}

RwBool CNtlWSESectorCull::Begin()
{	
	return TRUE;
}

RwBool CNtlWSESectorCull::Scheduling()
{
	// file pos move
	fseek(m_pFile, m_iFilePos, SEEK_SET);

	fread(&m_pNtlWorldSector->m_IsCulled, sizeof(RwBool), 1, m_pFile);

	return FALSE;
}

RwBool CNtlWSESectorCull::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSETileTransparency::CNtlWSETileTransparency(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSETileTransparency::~CNtlWSETileTransparency()
{
}

RwBool CNtlWSETileTransparency::Begin()
{	
	return TRUE;
}

RwBool CNtlWSETileTransparency::Scheduling()
{
	// file pos move
	fseek(m_pFile, m_iFilePos, SEEK_SET);

	fread(&m_pNtlWorldSector->m_IsTileTransparency, sizeof(RwBool), 1, m_pFile);

	return FALSE;
}

RwBool CNtlWSETileTransparency::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEShoreLine::CNtlWSEShoreLine(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSEShoreLine::~CNtlWSEShoreLine()
{
}

RwBool CNtlWSEShoreLine::Begin()
{	
	return TRUE;
}

RwBool CNtlWSEShoreLine::Scheduling()
{
	// file pos move
	fseek(m_pFile, m_iFilePos, SEEK_SET);

	// Shoreline
	RwBool IsThereShoreLine;
	fread(&IsThereShoreLine, sizeof(RwBool), 1, m_pFile);
	if(IsThereShoreLine)
	{
		SPLEntityCreateParam	PLEntityCreateParam;
		RwV3d					Pos;

		Pos.x						= m_pNtlWorldSector->DatumPoint.x;
		Pos.y						= m_pNtlWorldSector->DatumPoint.y;
		Pos.z						= m_pNtlWorldSector->DatumPoint.z;
		PLEntityCreateParam.pPos	= &Pos;

		m_pNtlWorldSector->m_pNtlPLEntityShoreLine = static_cast<CNtlPLShoreLine*>(GetSceneManager()->CreateEntity(PLENTITY_SHORELINE, NTL_PLEN_SHORELINE, &PLEntityCreateParam));			
		DBO_ASSERT(m_pNtlWorldSector->m_pNtlPLEntityShoreLine, "Entity create failed.");

		static_cast<CNtlPLShoreLine*>(m_pNtlWorldSector->m_pNtlPLEntityShoreLine)->LoadFromFile(m_pFile);
	}

	return FALSE;
}

RwBool CNtlWSEShoreLine::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEDecal::CNtlWSEDecal(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSEDecal::~CNtlWSEDecal()
{
}

RwBool CNtlWSEDecal::Begin()
{	
	return TRUE;
}

RwBool CNtlWSEDecal::Scheduling()
{
	// file pos move
	fseek(m_pFile, m_iFilePos, SEEK_SET);

	// Decals
	CNtlPLWorldDecal*	pNtlPLWorldDecal;
	RwInt32				EntityNum;
	sDECAL_PROP_PARAM	DecalPropParam;
	SPLDecalCreateParam CreateParam;

	fread(&EntityNum, sizeof(RwInt32), 1, m_pFile);

	for(int i = 0; i < EntityNum; ++i)
	{
		CNtlPLWorldDecal::Load(DecalPropParam, m_pFile);

		CreateParam.pTexName	= DecalPropParam._TexName;
		CreateParam.pTexPath	= ".\\Texture\\NtlWe\\Decal\\";

		RwReal Width	= DecalPropParam._SizeX;
		RwReal Height	= DecalPropParam._SizeZ;
		if(Width > Height)
		{
			CreateParam.fIntersectionRadius = Width;
		}
		else
		{
			CreateParam.fIntersectionRadius = Height;
		}

		CreateParam.vSize.x	= DecalPropParam._SizeX;
		CreateParam.vSize.z	= DecalPropParam._SizeZ;

		CreateParam.fScale				= DecalPropParam._TexScale;
		CreateParam.fVisibleSquaredDist	= DecalPropParam._VisibilityDist;

		string CurTexName = DecalPropParam._TexName;
		string W_Prefix("w_");
		string C_Prefix("c_");

		if(-1 == CurTexName.find_first_of(W_Prefix) && -1 == CurTexName.find_first_of(C_Prefix))
		{
			DBO_TRACE(FALSE, "Decal textures should've got prefix; w_, c_");
		}
		else
		{
			RwInt32 IdxWPrefix = CurTexName.find_first_of(W_Prefix);
			RwInt32 IdxCPrefix = CurTexName.find_first_of(C_Prefix);

			if(!IdxWPrefix)
			{
				CreateParam.rwWrapType = rwTEXTUREADDRESSWRAP;
			}
			else if(!IdxCPrefix)
			{
				CreateParam.rwWrapType = rwTEXTUREADDRESSCLAMP;
			}
			else
			{
				DBO_TRACE(FALSE, "Decal textures should've got prefix; w_, c_");
			}
		}

		if(DecalPropParam._DecalMode == 0)
		{
			CreateParam.nFlagEffect = E_FLAG_DECAL_RENDER_WORLD;
		}
		else if(DecalPropParam._DecalMode == 1)
		{
			CreateParam.nFlagEffect = E_FLAG_DECAL_RENDER_OBJECT;
		}
		else if(DecalPropParam._DecalMode == 2)
		{
			CreateParam.nFlagEffect = E_FLAG_DECAL_RENDER_WORLD | E_FLAG_DECAL_RENDER_OBJECT;
		}
		else if(DecalPropParam._DecalMode == 3)
		{
			CreateParam.nFlagEffect = E_FLAG_DECAL_RENDER_OBJECT | E_FLAG_DECAL_WATER;
		}

		CreateParam.eDecalType = DECAL_TERRAIN;

		pNtlPLWorldDecal = static_cast<CNtlPLWorldDecal*>(GetSceneManager()->CreateEntity(PLENTITY_DECAL, "NULL", &CreateParam));
		DBO_ASSERT(pNtlPLWorldDecal, "Entity create failed.");

		m_pNtlWorldSector->InsertIn2World(pNtlPLWorldDecal, 4);

		pNtlPLWorldDecal->SetRotate(DecalPropParam._TexAngle);
		pNtlPLWorldDecal->SetColor(DecalPropParam._RGBA.red, DecalPropParam._RGBA.green, DecalPropParam._RGBA.blue);
		pNtlPLWorldDecal->SetAlpha(DecalPropParam._RGBA.alpha);
		pNtlPLWorldDecal->SetAlphaBlend(TRUE, rwBLENDSRCALPHA, rwBLENDINVSRCALPHA);
		pNtlPLWorldDecal->SetPosition(&DecalPropParam._Pos);
		pNtlPLWorldDecal->SetName(DecalPropParam._TexName);

		if(DecalPropParam._UVAniCheck)
		{
			if(DecalPropParam._UVAniPatternIdx == 0)
			{
				SNtlPrtStdEmitterPrtUVAnim EmitterUVAnim;
				EmitterUVAnim.vDir.x		= 0.0f;
				EmitterUVAnim.vDir.y		= 0.0f;
				EmitterUVAnim.vDir.z		= 1.0f;
				EmitterUVAnim.fVelocity		= DecalPropParam._UVAniSpeed;
				EmitterUVAnim.fMaxOffset	= 0.0f;
				pNtlPLWorldDecal->SetUVAnimAction(EmitterUVAnim);
			}
			else if(DecalPropParam._UVAniPatternIdx == 1)
			{
				SNtlPrtStdEmitterPrtUVAnim EmitterUVAnim;
				EmitterUVAnim.vDir.x		= 0.0f;
				EmitterUVAnim.vDir.y		= 0.0f;
				EmitterUVAnim.vDir.z		= 1.0f;
				EmitterUVAnim.fVelocity		= DecalPropParam._UVAniSpeed;
				EmitterUVAnim.fMaxOffset	= DecalPropParam._UVAniOffset;
				pNtlPLWorldDecal->SetUVAnimAction(EmitterUVAnim);
			}
			else if(DecalPropParam._UVAniPatternIdx == 2)
			{
				pNtlPLWorldDecal->Set2DRoateAction(DecalPropParam._UVAniOffset, DecalPropParam._UVAniSpeed);
			}
			else if(DecalPropParam._UVAniPatternIdx == 3)
			{
				pNtlPLWorldDecal->SetSizeAction(DecalPropParam._TexScale, DecalPropParam._UVAniOffset, DecalPropParam._UVAniSpeed);
			}
		}
	}

	return FALSE;
}

RwBool CNtlWSEDecal::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEPlant::CNtlWSEPlant(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSEPlant::~CNtlWSEPlant()
{
}

RwBool CNtlWSEPlant::Begin()
{	
	return TRUE;
}

RwBool CNtlWSEPlant::Scheduling()
{
	// file pos move
	fseek(m_pFile, m_iFilePos, SEEK_SET);

	// plant
	RwInt32 iPlantSetNum;
	fread(&iPlantSetNum, sizeof(RwInt32), 1, m_pFile);
	for (int i = 0; i < iPlantSetNum; ++i)
	{
		sSECTOR_PLANT_SET_ATTR* pPlantSet = NTL_NEW sSECTOR_PLANT_SET_ATTR;
		RwInt32					iPlantObjNum;			

		fread(&pPlantSet->iSetIdx, sizeof(RwInt32), 1, m_pFile); // Set Index
		fread(&iPlantObjNum, sizeof(RwInt32), 1, m_pFile); // Obj Num

		for (int j = 0; j < iPlantObjNum; ++j)
		{
			sSECTOR_PLANT_SET_ATTR::sSECTOR_PLANT_OBJ_ATTR* pPlantObj = NTL_NEW sSECTOR_PLANT_SET_ATTR::sSECTOR_PLANT_OBJ_ATTR;

			fread(&pPlantObj->iTileIdx, sizeof(RwInt32), 1, m_pFile); // Tile Index

			pPlantSet->vecPlantObj.push_back(pPlantObj);
		}			
		m_pNtlWorldSector->m_vecPlantSet.push_back(pPlantSet);
	}

	GetSceneManager()->GetPlant()->OnLoad(m_pNtlWorldSector);

	return FALSE;
}

RwBool CNtlWSEPlant::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEWorldLight::CNtlWSEWorldLight(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSEWorldLight::~CNtlWSEWorldLight()
{
}

RwBool CNtlWSEWorldLight::Begin()
{	
	return TRUE;
}

RwBool CNtlWSEWorldLight::Scheduling()
{
	// file pos move
	fseek(m_pFile, m_iFilePos, SEEK_SET);

	RwInt32 iWLSphereNum;

	fread(&iWLSphereNum, sizeof(RwInt32), 1, m_pFile);

	for (int i = 0; i < iWLSphereNum; ++i)
	{
		RwV3d				vPos;
		RwRGBA				clr;
		RwReal				fRadius;
		EPLWorldLightType	eType = EPLWORLDLIGHT_SPHERE;

		fread(&vPos, sizeof(RwV3d), 1, m_pFile);
		fread(&clr, sizeof(RwRGBA), 1, m_pFile);
		fread(&fRadius, sizeof(RwReal), 1, m_pFile);

		SPLWorldLightCreateParam_Sphere CreateParam;
		CreateParam.pPos		= &vPos;
		CreateParam._pUserData	= &eType;
		CreateParam.uiRed		= clr.red;
		CreateParam.uiGreen		= clr.green;
		CreateParam.uiBlue		= clr.blue;
		CreateParam.fRadius		= fRadius;

		CNtlPLEntity* pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_WORLD_LIGHT, "NULL", &CreateParam);
		DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

		m_pNtlWorldSector->m_vecNtlPLEntityWorldLight_Sphere.push_back(pNtlPLEntity);
	}

	RwInt32 iWLBoxNum;

	fread(&iWLBoxNum, sizeof(RwInt32), 1, m_pFile);

	for (int i = 0; i < iWLBoxNum; ++i)
	{
		RwV3d				vPos;
		RwRGBA				clr;
		RwV3d				vRot;
		RwV3d				vScale;
		EPLWorldLightType	eType = EPLWORLDLIGHT_BOX;

		fread(&vPos, sizeof(RwV3d), 1, m_pFile);
		fread(&clr, sizeof(RwRGBA), 1, m_pFile);
		fread(&vRot, sizeof(RwV3d), 1, m_pFile);
		fread(&vScale, sizeof(RwV3d), 1, m_pFile);	

		SPLWorldLightCreateParam_Box CreateParam;
		CreateParam.pPos		= &vPos;
		CreateParam._pUserData	= &eType;
		CreateParam.uiRed		= clr.red;
		CreateParam.uiGreen		= clr.green;
		CreateParam.uiBlue		= clr.blue;
		CreateParam.vScale		= vScale;
		CreateParam.vRotate		= vRot;

		CNtlPLEntity* pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_WORLD_LIGHT, "NULL", &CreateParam);
		DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

		m_pNtlWorldSector->m_vecNtlPLEntityWorldLight_Box.push_back(pNtlPLEntity);
	}

	return FALSE;
}

RwBool CNtlWSEWorldLight::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEOccluder::CNtlWSEOccluder(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSEOccluder::~CNtlWSEOccluder()
{
}

RwBool CNtlWSEOccluder::Begin()
{	
	return TRUE;
}

RwBool CNtlWSEOccluder::Scheduling()
{
	// file pos move
	fseek(m_pFile, m_iFilePos, SEEK_SET);

	RwInt32 iOccluderQuadNum;

	fread(&iOccluderQuadNum, sizeof(RwInt32), 1, m_pFile);

	for (int i = 0; i < iOccluderQuadNum; ++i)
	{
		RwUInt32			eFuncFlag;
		RwV3d				vPos;
		RwInt32				iPlaneType;
		RwV3d				vVertex;
		EPLOccluderType		eType = EPLOCCLUDER_QUAD;
		RwChar				acTemp[128];

		fread(&eFuncFlag, sizeof(RwUInt32), 1, m_pFile);

#ifndef dNTL_WORLD_TOOL_MODE
		if (eFuncFlag & EPLOCCLUDER_FUNC_PVS)
		{
			fseek(m_pFile, sizeof(RwV3d), SEEK_CUR);
			fseek(m_pFile, sizeof(RwInt32), SEEK_CUR);
			fseek(m_pFile, sizeof(RwV3d), SEEK_CUR);
			fseek(m_pFile, sizeof(RwV3d), SEEK_CUR);
			fseek(m_pFile, sizeof(RwV3d), SEEK_CUR);
			fseek(m_pFile, sizeof(RwV3d), SEEK_CUR);

			continue;
		}
#endif
		fread(&vPos, sizeof(RwV3d), 1, m_pFile);
		fread(&iPlaneType, sizeof(RwInt32), 1, m_pFile);
		fread(acTemp, sizeof(RwChar) * 128, 1, m_pFile);

		SPLEntityCreateParam CreateParam;
		CreateParam.pPos		= &vPos;
		CreateParam._pUserData	= &eType;

		CNtlPLOccluder_Quad* pNtlPLOccluder = static_cast<CNtlPLOccluder_Quad*>(GetSceneManager()->CreateEntity(PLENTITY_OCCLUDER, "NULL", &CreateParam));
		DBO_ASSERT(pNtlPLOccluder, "Entity create failed.");

		for (int j = 0; j < 4; ++j)
		{
			fread(&vVertex, sizeof(RwV3d), 1, m_pFile);
			pNtlPLOccluder->SetVertexOriginal(j, &vVertex);
		}

		pNtlPLOccluder->SetOccluderFuncFlag(eFuncFlag);
		pNtlPLOccluder->SetOccluderPlaneType(iPlaneType);
		pNtlPLOccluder->SetTriggerName(acTemp);
		pNtlPLOccluder->CalcBoundingSphere();
		m_pNtlWorldSector->m_vecNtlPLEntityOccluder_Quad.push_back(pNtlPLOccluder);
	}
#ifdef dNTL_WORLD_TOOL_MODE
	CNtlPLOccluder_Quad::LoadSwapFile(m_pNtlWorldSector->DatumPoint.x, m_pNtlWorldSector->DatumPoint.y, m_pNtlWorldSector->DatumPoint.z);
#endif

	m_pNtlWorldSector->CreateOccluderProxy(1.0f, 1);

	return FALSE;
}

RwBool CNtlWSEOccluder::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEHeatHazeObject::CNtlWSEHeatHazeObject(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSEHeatHazeObject::~CNtlWSEHeatHazeObject()
{
}

RwBool CNtlWSEHeatHazeObject::Begin()
{	
	return TRUE;
}

RwBool CNtlWSEHeatHazeObject::Scheduling()
{
	// file pos move
	fseek(m_pFile, m_iFilePos, SEEK_SET);

	RwInt32 iHeatHazeNum;

	fread(&iHeatHazeNum, sizeof(RwInt32), 1, m_pFile);

	for (int i = 0; i < iHeatHazeNum; ++i)
	{
		SPLEntityCreateParam CreateParam; CreateParam.pPos = NULL;
		CNtlPLHeatHazeObject* pNtlPLHeatHaze = static_cast<CNtlPLHeatHazeObject*>(GetSceneManager()->CreateEntity(PLENTITY_HEATHAZE, "NULL", &CreateParam));
		DBO_ASSERT(pNtlPLHeatHaze, "Entity create failed.");

		RwV3d vVertex;
		for (int j = 0; j < 4; ++j)
		{
			fread(&vVertex, sizeof(RwV3d), 1, m_pFile);
			pNtlPLHeatHaze->SetVertex(j, &vVertex);
		}

		RwReal fDistance;
		fread(&fDistance, sizeof(RwReal), 1, m_pFile);
		pNtlPLHeatHaze->SetDistance(fDistance);

		fread(pNtlPLHeatHaze->HeatHazeData(), sizeof(sNTL_HEATHAZE), 1, m_pFile);
		pNtlPLHeatHaze->CalcBoundingSphere();

		m_pNtlWorldSector->m_vecNtlPLEntityHeatHaze.push_back(pNtlPLHeatHaze);
	}
#ifdef dNTL_WORLD_TOOL_MODE
	CNtlPLHeatHazeObject::LoadSwapFile(m_pNtlWorldSector->DatumPoint.x, m_pNtlWorldSector->DatumPoint.y, m_pNtlWorldSector->DatumPoint.z);
#endif
	return FALSE;
}

RwBool CNtlWSEHeatHazeObject::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSELightObject::CNtlWSELightObject(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSELightObject::~CNtlWSELightObject()
{
}

RwBool CNtlWSELightObject::Begin()
{	
	return TRUE;
}

RwBool CNtlWSELightObject::Scheduling()
{
	// file pos move
	fseek(m_pFile, m_iFilePos, SEEK_SET);

	RwInt32 iLightObjectNum;
	fread(&iLightObjectNum, sizeof(RwInt32), 1, m_pFile);

	for (int i = 0; i < iLightObjectNum; ++i)
	{
		SPLLightObjectCreateParam CreateParam;

		fread(&CreateParam.rpLightType, sizeof(RpLightType), 1, m_pFile);
		fread(&CreateParam.bRealLight, sizeof(RwBool), 1, m_pFile);

		RwV3d vPos;
		fread(&vPos, sizeof(RwV3d), 1, m_pFile);
		CreateParam.pPos = &vPos;

		fread(&CreateParam.vRotate, sizeof(RwV3d), 1, m_pFile);
		fread(&CreateParam.rgbafColor, sizeof(RwRGBAReal), 1, m_pFile);
		fread(&CreateParam.fRadius, sizeof(RwReal), 1, m_pFile);
		fread(&CreateParam.fAngle, sizeof(RwReal), 1, m_pFile);

		// CZ-LO
		fread(&CreateParam.bDecal, sizeof(RwBool), 1, m_pFile);
		fread(&CreateParam.bWorldLight, sizeof(RwBool), 1, m_pFile);

		CNtlPLLightObject* pNtlPLLightObject = static_cast<CNtlPLLightObject*>(GetSceneManager()->CreateEntity(PLENTITY_LIGHT_OBJECT, "NULL", &CreateParam));
		DBO_ASSERT(pNtlPLLightObject, "Entity create failed.");

		m_pNtlWorldSector->m_vecNtlPLEntityLightObject.push_back(pNtlPLLightObject);
	}

	return FALSE;
}

RwBool CNtlWSELightObject::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEDecalVolume::CNtlWSEDecalVolume(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSEDecalVolume::~CNtlWSEDecalVolume()
{
}

RwBool CNtlWSEDecalVolume::Begin()
{	
	return TRUE;
}

RwBool CNtlWSEDecalVolume::Scheduling()
{
	// file pos move
	fseek(m_pFile, m_iFilePos, SEEK_SET);

	RwInt32 iDecalVolumeNum;
	fread(&iDecalVolumeNum, sizeof(RwInt32), 1, m_pFile);
	for (int i = 0; i < iDecalVolumeNum; ++i)
	{
		SPLDecalVolumeCreateParam CreateParam;

		fread(CreateParam.acTextureName, sizeof(RwChar) * 128, 1, m_pFile);

		RwV3d vPos;
		fread(&vPos, sizeof(RwV3d), 1, m_pFile);
		CreateParam.pPos = &vPos;

		fread(&CreateParam.vRotation, sizeof(RwV3d), 1, m_pFile);
		fread(&CreateParam.vScale, sizeof(RwV3d), 1, m_pFile);
		fread(&CreateParam.clrDiffuse, sizeof(RwRGBA), 1, m_pFile);
		fread(&CreateParam.fAppendOffset, sizeof(RwReal), 1, m_pFile);
		fread(&CreateParam.fVisibleDist, sizeof(RwReal), 1, m_pFile);
		fread(&CreateParam.uiDecalVolumeFlags, sizeof(RwUInt32), 1, m_pFile);

		CNtlPLDecalVolume* pNtlPLDecalVolume = static_cast<CNtlPLDecalVolume*>(GetSceneManager()->CreateEntity(PLENTITY_DECAL_VOLUME, "NULL", &CreateParam));
		DBO_ASSERT(pNtlPLDecalVolume, "Entity create failed.");

		m_pNtlWorldSector->m_vecNtlPLEntityDecalVolume.push_back(pNtlPLDecalVolume);
	}

	return FALSE;
}

RwBool CNtlWSEDecalVolume::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEGameProperty::CNtlWSEGameProperty(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSEGameProperty::~CNtlWSEGameProperty()
{
}

RwBool CNtlWSEGameProperty::Begin()
{	
	return TRUE;
}

RwBool CNtlWSEGameProperty::Scheduling()
{
	// file pos move
	fseek(m_pFile, m_iFilePos, SEEK_SET);

	RwInt32 iGamePropertyNum;
	fread(&iGamePropertyNum, sizeof(RwInt32), 1, m_pFile);
	for (int i = 0; i < iGamePropertyNum; ++i)
	{
		SPLGamePropertyCreateParam CreateParam;

		fread(&CreateParam.eShapeType, sizeof(RwUInt32), 1, m_pFile);

		RwV3d vPos;
		fread(&vPos, sizeof(RwV3d), 1, m_pFile);
		CreateParam.pPos = &vPos;

		fread(&CreateParam.vSize, sizeof(RwV3d), 1, m_pFile);
		fread(&CreateParam.uiProperty, sizeof(RwUInt32), 1, m_pFile);

		CNtlPLGameProperty* pNtlPLGameProperty = static_cast<CNtlPLGameProperty*>(GetSceneManager()->CreateEntity(PLENTITY_GAME_PROPERTY, "NULL", &CreateParam));
		DBO_ASSERT(pNtlPLGameProperty, "Entity create failed.");

		m_pNtlWorldSector->m_vecNtlPLEntityGameProperty.push_back(pNtlPLGameProperty);
	}

	return FALSE;
}

RwBool CNtlWSEGameProperty::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

CNtlWSEDojo::CNtlWSEDojo(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos)
:CScheduleElement(iPrimaryKey, fUsedTime)
,m_pNtlWorldSector(pNtlWorldSector)
,m_pFile(pFile)
,m_iFilePos(iFilePos)
{
}

CNtlWSEDojo::~CNtlWSEDojo()
{
}

RwBool CNtlWSEDojo::Begin()
{	
	return TRUE;
}

RwBool CNtlWSEDojo::Scheduling()
{
	// file pos move
	fseek(m_pFile, m_iFilePos, SEEK_SET);

	RwInt32 iDojoNum;
	fread(&iDojoNum, sizeof(RwInt32), 1, m_pFile);
	for (int i = 0; i < iDojoNum; ++i)
	{
		SPLDojoCreateParam CreateParam;

		RwV3d vScale;
		RwV3d vRot;
		RwV3d vPos;

		fread(&vScale, sizeof(RwV3d), 1, m_pFile);
		fread(&vRot, sizeof(RwV3d), 1, m_pFile);
		fread(&vPos, sizeof(RwV3d), 1, m_pFile);

		CreateParam.pPos = &vPos;

		fread(&CreateParam.iDojoID, sizeof(RwInt32), 1, m_pFile);
		fread(&CreateParam.fFadeInTime, sizeof(RwReal), 1, m_pFile);
		fread(&CreateParam.fFadeOutTime, sizeof(RwReal), 1, m_pFile);

		for (int i = 0; i < dPL_DOJO_MAX_LEVEL; ++i)
		{				
			fread(CreateParam.acObjectName[i], sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN, 1, m_pFile);
			fread(CreateParam.acEffectNameUp[i], sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN, 1, m_pFile);
			fread(CreateParam.acEffectNameDown[i], sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN, 1, m_pFile);
		}

		CNtlPLDojo* pNtlPLDojo = static_cast<CNtlPLDojo*>(GetSceneManager()->CreateEntity(PLENTITY_DOJO, "NULL", &CreateParam));
		DBO_ASSERT(pNtlPLDojo, "Entity create failed.");

		pNtlPLDojo->SetPosition(&vPos);
		pNtlPLDojo->SetRotate(vRot.x, vRot.y, vRot.z);
		pNtlPLDojo->SetScaleVector(&vScale);

		m_pNtlWorldSector->m_vecNtlPLEntityDojo.push_back(pNtlPLDojo);
	}

	return FALSE;
}

RwBool CNtlWSEDojo::End()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#endif
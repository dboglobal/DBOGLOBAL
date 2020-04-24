#include "precomp_ntlpresentation.h"
#include "NtlPLResourceScheduling.h"

// core
#include "NtlDebug.h"
#include "NtlCoreUtil.h"

// framework
#include "NtlTimer.h"

// presentation
#include "NtlPLDef.h"
#include "NtlPLEntity.h"
#include "NtlPLResourceManager.h"

// Performance Check ( Develop )
//#define USE_RESOURCE_SCHEDULING_PERFORMANCE_CHECK

#define MAX_LOAD_CHARACTER_SCHEDULING_TIME			4.0f
#define MAX_LOAD_OBJECT_SCHEDULING_TIME				3.0f

#define MAX_DELETE_SCHEDULING_TIME					0.1f

// 이 시간을 넘기면 rest time 을 뻥튀기 한다.
#define MAX_LOAD_CHARACTER_OVER_WEIGHT				0.07f
#define MAX_LOAD_OBJECT_OVER_WEIGHT					0.05f

#define DEFAULT_LOAD_OBJECT_REST_TIME					0.1f
#define DEFAULT_LOAD_CHARACTER_REST_TIME				0.3f

RwBool	g_bLoadSeamless				= FALSE;
RwReal	g_fLoadObjectTime			= 0.02f;
RwReal	g_fLoadObjectSeamlessTime	= 0.01f;
RwReal	g_fLoadCharacterSeamlessTime = 0.001f;

/**
* \brief Construction
*/
CNtlResourceScheduleUnit::CNtlResourceScheduleUnit()
{
	m_byEntityType			= 0;

	m_fLoadCurrTime			= 0.0f;
	m_fLoadSchedulingTime	= g_fLoadObjectTime;
}

/**
* \brief Destruction
*/
CNtlResourceScheduleUnit::~CNtlResourceScheduleUnit()
{
	// Delete Schedule에 있는 clump 를 삭제한다.
	CNtlPLResource *pResource;
	ListScheduling::iterator itDelete;
	for(itDelete = m_listClumpDeleteSchedule.begin(); itDelete != m_listClumpDeleteSchedule.end(); itDelete++)
	{
		pResource = (*itDelete);
		GetNtlResourceManager()->UnLoad(pResource);
	}
	m_listClumpDeleteSchedule.clear();

	// Load Schedule에 있는 clump 리스트를 삭제한다.
	MapScheduling::iterator it;
	for(it = m_mapClumpLoadSchedule.begin(); it != m_mapClumpLoadSchedule.end(); it++)
	{
		CNtlResourceScheduleManager::FreeListFree((void*)(*it).second);
	}
	m_mapClumpLoadSchedule.clear();
}

/**
* \brief Obejct 관련 Clump들을 스케쥴링 로딩 ( 게임 스테이지로 진입 도중에 로딩한다. )
* \param fElapsed	(RwReal) 이전 업데이트에서 경과 시간
*/
void CNtlResourceScheduleUnit::UpdateObjectLoadScheduling(RwReal fElapsed)
{
	RwReal fSum = 0.0f;
	DWORD dwTime;
	
	SResourceScheduleNode *pNode;
	CNtlPLResource *pResource;
	CNtlPLEntity *pPLEntity;
	
	MapScheduling::iterator it;

#ifdef USE_RESOURCE_SCHEDULING_PERFORMANCE_CHECK
	DWORD dwCount = 0;
#endif
	
	// The map is rotated for a certain period of time and is loaded unconditionally. (Improving the loading speed of the data that was read.)
	for(it = m_mapClumpLoadSchedule.begin(); it != m_mapClumpLoadSchedule.end(); )
	{
		dwTime = GetTickCount();

		pNode = (*it).second;
		pPLEntity = (*it).first;
		if(pPLEntity)
			pPLEntity->CallPreSchedulingResource();

		pResource = GetNtlResourceManager()->LoadClump(pNode->chFileName, pNode->chResourcePath);

		if(pPLEntity)
		{	
			pPLEntity->CallSchedulingResource(pResource);
			pPLEntity->CallSchedulingResourceOnly();
		}

		it = m_mapClumpLoadSchedule.erase(it);
		CNtlResourceScheduleManager::FreeListFree((void*)pNode);

#ifdef USE_RESOURCE_SCHEDULING_PERFORMANCE_CHECK
		dwCount++;
#endif

		fSum += (RwReal)(GetTickCount() - dwTime)/1000.0f;
		if(fSum > g_fLoadObjectTime)
			break;
	}

#ifdef USE_RESOURCE_SCHEDULING_PERFORMANCE_CHECK
	RwReal fRealTime = fSum;
#endif
	
	if( fSum > MAX_LOAD_OBJECT_OVER_WEIGHT )
	{
		fSum *= 5.0f;
		if( fSum > MAX_LOAD_OBJECT_SCHEDULING_TIME )
			fSum = MAX_LOAD_OBJECT_SCHEDULING_TIME;
	}

	m_fLoadSchedulingTime = DEFAULT_LOAD_OBJECT_REST_TIME + fSum;

#ifdef USE_RESOURCE_SCHEDULING_PERFORMANCE_CHECK
	NtlLogFilePrintArgs( "[OBJECT] : %d loading for one tick, time is %3.3f sec and after rest time %3.3f sec", dwCount, fRealTime, m_fLoadSchedulingTime);
#endif
}

/**
* \brief Object 관련 clump들을 Seamless 로딩 ( 게임 도중 심리스 형식의 로딩 )
* \param fElapsed	(RwReal) 이전 업데이트에서 경과한 시간
*/
void CNtlResourceScheduleUnit::UpdateObjectLoadSeamlessScheduling(RwReal fElapsed)
{
	RwReal fSum = 0.0f;
	DWORD dwTime;
	
	SResourceScheduleNode *pNode;
	CNtlPLResource *pResource;
	CNtlPLEntity *pPLEntity;
	
	MapScheduling::iterator it;

#ifdef USE_RESOURCE_SCHEDULING_PERFORMANCE_CHECK
	DWORD dwCount = 0;
#endif
	
	for(it = m_mapClumpLoadSchedule.begin(); it != m_mapClumpLoadSchedule.end(); )
	{
		dwTime = GetTickCount();

		pNode = (*it).second;
		pPLEntity = (*it).first;
		if(pPLEntity)
			pPLEntity->CallPreSchedulingResource();

		pResource = GetNtlResourceManager()->LoadClump(pNode->chFileName, pNode->chResourcePath);

		if(pPLEntity)
		{	
			pPLEntity->CallSchedulingResource(pResource);
			pPLEntity->CallSchedulingResourceOnly();
		}

		it = m_mapClumpLoadSchedule.erase(it);
		CNtlResourceScheduleManager::FreeListFree((void*)pNode);

#ifdef USE_RESOURCE_SCHEDULING_PERFORMANCE_CHECK
		dwCount++;
#endif

		// 초당 20 frm
		fSum += (RwReal)(GetTickCount() - dwTime)/1000.0f;
		if(fSum > g_fLoadObjectSeamlessTime)
			break;
	}

#ifdef USE_RESOURCE_SCHEDULING_PERFORMANCE_CHECK
	RwReal fRealtime = fSum;
#endif

	if( fSum > MAX_LOAD_OBJECT_OVER_WEIGHT )
	{
		fSum *= 5.0f;
		if( fSum > MAX_LOAD_OBJECT_SCHEDULING_TIME )
			fSum = MAX_LOAD_OBJECT_SCHEDULING_TIME;
	}

	m_fLoadSchedulingTime = DEFAULT_LOAD_OBJECT_REST_TIME + fSum;

#ifdef USE_RESOURCE_SCHEDULING_PERFORMANCE_CHECK
	NtlLogFilePrintArgs( "[OBJECTSEAMLESS] : %d loading for one tick, time is %3.3f sec and after rest time %3.3f sec", dwCount, fRealtime, m_fLoadSchedulingTime);
#endif
}

/**
* \brief 스케쥴링 업데이트를 로딩한다.
* \param fElapsed	(RwReal) 이전 업데이트에서 경과된 시간
*/
void CNtlResourceScheduleUnit::UpdateLoadScheduling(RwReal fElapsed)
{
	m_fLoadCurrTime += fElapsed;
	if(m_fLoadCurrTime <= m_fLoadSchedulingTime)
		return;

	m_fLoadCurrTime -= m_fLoadSchedulingTime;

	if(m_mapClumpLoadSchedule.empty())
		return;

	// 오브젝트 만 업데이트 한다.
	UpdateObjectLoadScheduling(fElapsed);
}

/**
* \brief 심리스 방식으로 스케쥴러를 업데이트 한다.
*/
void CNtlResourceScheduleUnit::UpdateLoadSeamlessScheduling(RwReal fElapsed)
{
	if(m_mapClumpLoadSchedule.empty())
		return;
	
	UpdateObjectLoadSeamlessScheduling(fElapsed);
}

/**
* \brief 지워질 목록을 스케쥴링한다.
*/
void CNtlResourceScheduleUnit::UpdateDeleteScheduling(RwReal fElapsed)
{
	if(m_listClumpDeleteSchedule.empty())
		return;

	RwReal fSumTime = 0.0f;
	DWORD dwTime;
	RwReal fFps		= ( 1.0f / CNtlTimer::GetFps() ) * 0.1f;
	if(fFps > 0.1f)
		fFps = 0.1f;

	CNtlPLResource *pResource;
	ListScheduling::iterator it = m_listClumpDeleteSchedule.begin();

	dwTime = GetTickCount();
	
	while(1)
	{

		pResource = (*it);
		GetNtlResourceManager()->UnLoad(pResource);

		it = m_listClumpDeleteSchedule.erase(it);

		if(m_listClumpDeleteSchedule.empty())
		{
			break;
		}

		fSumTime = (GetTickCount() - dwTime)/1000.0f;

		if(fSumTime > fFps)
		{
			break;
		}
	}
}

/**
* \brief Unit이 스케쥴링 로드할 Entity의 Type을 셋팅
* \param byEntityType	(RwUInt8) 엔티티의 타입
*/
void CNtlResourceScheduleUnit::SetEntityType(RwUInt8 byEntityType)
{
	m_byEntityType = byEntityType;
}

/**
* \brief
*/
void CNtlResourceScheduleUnit::Update(RwReal fElapsed)
{
	if(g_bLoadSeamless)
		UpdateLoadSeamlessScheduling(fElapsed);
	else
		UpdateLoadScheduling(fElapsed);

	UpdateDeleteScheduling(fElapsed);
}

void CNtlResourceScheduleUnit::AddSchedulingNode(CNtlPLEntity *pPLEntity, SResourceScheduleNode *pNode)
{
	m_mapClumpLoadSchedule[pPLEntity] = pNode;
}

void CNtlResourceScheduleUnit::UnLoadScheduling(CNtlPLEntity *pPLEntity, CNtlPLResource *pResource)
{
	if(pResource == NULL)
	{
		MapScheduling::iterator it = m_mapClumpLoadSchedule.find(pPLEntity);
		if(it != m_mapClumpLoadSchedule.end())
		{
			CNtlResourceScheduleManager::FreeListFree((void*)(*it).second);
			m_mapClumpLoadSchedule.erase(it);
		}
	}
	else
	{
		m_listClumpDeleteSchedule.push_back(pResource);
	}
}

RwBool CNtlResourceScheduleUnit::IsEmptyLoadScheduling(void)
{
	if(m_mapClumpLoadSchedule.empty())
		return TRUE;

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* \brief Construction
*/
CNtlResourceScheduleCharUnit::CNtlResourceScheduleCharUnit() 
{
}

/**
* \brief Destruction
*/
CNtlResourceScheduleCharUnit::~CNtlResourceScheduleCharUnit() 
{
	ListCharScheduling::iterator it;
	for(it = m_listCharClumpLoadSchedule.begin(); it != m_listCharClumpLoadSchedule.end(); it++)
	{
		CNtlResourceScheduleManager::FreeListFree((void*)(*it).pNode);
	}
	m_listCharClumpLoadSchedule.clear();
}

/**
* \brief Update
* \param fElapsed	(RwReal) 이전 프레임에서의 업데이트 Time
*/
void CNtlResourceScheduleCharUnit::Update( RwReal fElapsed ) 
{
	//UpdateCharacterLoadScheduling( fElapsed );
	UpdateCharacterLoadSeamlessScheduling( fElapsed );

	CNtlResourceScheduleUnit::UpdateDeleteScheduling( fElapsed );
}

/**
* \brief 스케쥴링 로딩을 Node를 등록한다.
* \param pPLEntity	(CNtlPLEntity*) Entity의 주소
* \param pNode		(SResourceScheduleNode*) ResourceNode
*/
void CNtlResourceScheduleCharUnit::AddSchedulingNode(CNtlPLEntity *pPLEntity, SResourceScheduleNode *pNode ) 
{
	// 리스트에 등록될 구조체
	SResourceScheduleCharUnit sCharUnit;
	sCharUnit.pPLEntity = pPLEntity;
	sCharUnit.pNode = pNode;

	// 리스트에 추가한다.
	m_listCharClumpLoadSchedule.push_back( sCharUnit );
}

/**
* \brief 스케쥴링 로딩에서 Node를 삭제한다.
* \param pPLEntity	(CNtlPLEntity*) 삭제할 Entity의 포인터
* \param pResource	(CNtlPLResource*) 삭제할 리소스의 포인터
*/
void CNtlResourceScheduleCharUnit::UnLoadScheduling( CNtlPLEntity *pPLEntity, CNtlPLResource *pResource ) 
{
	// pResource가 NULL로 들어온다면 리스트에서 삭제해야 한다.
	// multimap으로 자료구조를 바꿔야함
	if(pResource == NULL)
	{
		ListCharScheduling::iterator it = m_listCharClumpLoadSchedule.begin();
		while(it != m_listCharClumpLoadSchedule.end())
		{
			// 리스트에서 검색하여 삭제
			if( (*it).pPLEntity == pPLEntity )
			{
				CNtlResourceScheduleManager::FreeListFree((void*)(*it).pNode);
				m_listCharClumpLoadSchedule.erase(it);
				return;
			}
			else
			{
				it++;
			}
		}
	}
	else
	{
		// 삭제할 리스트를 등록한다.
		m_listClumpDeleteSchedule.push_back(pResource);
	}
}

/**
* \brief 스케쥴링 로딩 할 리스트가 비어있는가?
* \returns 비어있는지 여부
*/
RwBool CNtlResourceScheduleCharUnit::IsEmptyLoadScheduling( void ) 
{
	if( m_listCharClumpLoadSchedule.empty() )
		return TRUE;

	return FALSE;
}

/**
* \brief 캐릭터의 스케쥴링에 들어 있는 clump를 로딩한다.
* \param fElapsed	(RwReal) 경과시간
*/
void CNtlResourceScheduleCharUnit::UpdateCharacterLoadScheduling(RwReal fElapsed)
{
	m_fLoadCurrTime += fElapsed;

	// 이전에 걸렸던 시간만큼 지나지 않았다면 리턴한다.
	if(m_fLoadCurrTime <= m_fLoadSchedulingTime)
		return;

	m_fLoadCurrTime = 0.0f;

	// 없다면 리턴
	if( IsEmptyLoadScheduling() )
		return;

	SResourceScheduleNode *pNode = NULL;
	CNtlPLResource *pResource = NULL;
	CNtlPLEntity *pPLEntity = NULL;

	ListCharScheduling::iterator it = m_listCharClumpLoadSchedule.begin(); 
		
	DWORD dwTime = GetTickCount();

	pPLEntity = (*it).pPLEntity;
	pNode = (*it).pNode;

	// Entity에게 Resource를 로딩하기 전에 알려준다.
	if(pPLEntity)
		pPLEntity->CallPreSchedulingResource();

	// 리소스를 가지고 와서
	pResource = GetNtlResourceManager()->LoadClump(pNode->chFileName, pNode->chResourcePath);

	// 리소스를 로딩
	if(pPLEntity)
	{	
		pPLEntity->CallSchedulingResource(pResource);
		pPLEntity->CallSchedulingResourceOnly();
	}

	m_listCharClumpLoadSchedule.erase(it);
	// 리소스가 로딩된후에는 반복자를 지워준다.
	CNtlResourceScheduleManager::FreeListFree((void*)pNode);

	dwTime = GetTickCount() - dwTime;

	m_fLoadSchedulingTime = g_fCharScheduleTime;

}

/**
* \brief Cash 된 데이터의 로딩 속도를 향상시키기 위한 업데이트 함수
* \param fElapsed	(RwReal) 이전 업데이트에서 경과된 시간
*/
void CNtlResourceScheduleCharUnit::UpdateCharacterLoadSeamlessScheduling( RwReal fElapsed ) 
{
	m_fLoadCurrTime += fElapsed;

	// 이전에 걸렸던 시간만큼 지나지 않았다면 리턴한다.
	if(m_fLoadCurrTime <= m_fLoadSchedulingTime)
		return;

	m_fLoadCurrTime = 0.0f;

	// 없다면 리턴
	if( IsEmptyLoadScheduling() )
		return;

	DWORD dwTime = 0;
	RwReal fSum = 0.0f;

	SResourceScheduleNode *pNode = NULL;
	CNtlPLResource *pResource = NULL;
	CNtlPLEntity *pPLEntity = NULL;

	ListCharScheduling::iterator it;

#ifdef USE_RESOURCE_SCHEDULING_PERFORMANCE_CHECK
	DWORD dwCount = 0;
#endif

	for( it = m_listCharClumpLoadSchedule.begin(); it != m_listCharClumpLoadSchedule.end(); )
	{
		dwTime = GetTickCount();

		pPLEntity = (*it).pPLEntity;
		pNode = (*it).pNode;

		// Entity에게 Resource를 로딩하기 전에 알려준다.
		if(pPLEntity)
			pPLEntity->CallPreSchedulingResource();

		// 리소스를 가지고 와서
		pResource = GetNtlResourceManager()->LoadClump(pNode->chFileName, pNode->chResourcePath);

		// 리소스를 로딩
		if(pPLEntity)
		{	
			pPLEntity->CallSchedulingResource(pResource);
			pPLEntity->CallSchedulingResourceOnly();
		}

		it = m_listCharClumpLoadSchedule.erase(it);
		CNtlResourceScheduleManager::FreeListFree((void*)pNode);

		dwTime = GetTickCount() - dwTime;

#ifdef USE_RESOURCE_SCHEDULING_PERFORMANCE_CHECK
		dwCount++;
#endif

		fSum += ((RwReal)dwTime/1000.0f);
		if( fSum > g_fLoadCharacterSeamlessTime )
			break;
	}

#ifdef USE_RESOURCE_SCHEDULING_PERFORMANCE_CHECK
	RwReal fRealTime = fSum;
#endif

	if( fSum > MAX_LOAD_CHARACTER_OVER_WEIGHT )
	{
		fSum *= 5.0f;
		if( fSum > MAX_LOAD_CHARACTER_SCHEDULING_TIME )
			fSum = MAX_LOAD_CHARACTER_SCHEDULING_TIME;
	}

	m_fLoadSchedulingTime = g_fCharScheduleTime + fSum;

#ifdef USE_RESOURCE_SCHEDULING_PERFORMANCE_CHECK
	NtlLogFilePrintArgs( "[CHARACTER] : %d loading for one tick, time is %3.3f sec and after rest time %3.3f sec", dwCount, fRealTime, m_fLoadSchedulingTime);
#endif
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

RwFreeList* CNtlResourceScheduleManager::m_pScheduleNodeFreeList = NULL;		/* scheduling node memory pool **/


RwBool CNtlResourceScheduleManager::AllocFreeList(void)
{
	NTL_FUNCTION("CNtlReourceScheduleManager::AllocFreeList");

	RwUInt32 iBlockSize = 500;
	m_pScheduleNodeFreeList = RwFreeListCreate(sizeof(SResourceScheduleNode), iBlockSize, 16, rwMEMHINTDUR_GLOBAL);
    RWS_ASSERT(m_pScheduleNodeFreeList, "CNtlReourceScheduleManager::AllocFreeList");

	NTL_RETURN(TRUE);
}

void CNtlResourceScheduleManager::DeAllocFreeList(void)
{
	NTL_FUNCTION("CNtlReourceScheduleManager::DeAllocFreeList");

	if (m_pScheduleNodeFreeList)
    {
        RwFreeListDestroy(m_pScheduleNodeFreeList);
        m_pScheduleNodeFreeList = 0;
    }

	NTL_RETURNVOID();
}

void CNtlResourceScheduleManager::FreeListFree(void *pData)
{
	RwFreeListFree(m_pScheduleNodeFreeList, pData);
}

CNtlResourceScheduleManager::~CNtlResourceScheduleManager()
{
	CNtlResourceScheduleUnit *pUnit;
	MapScheduleUnit::iterator it;

	for(it = m_mapUnit.begin(); it != m_mapUnit.end(); it++)
	{
		pUnit = (*it).second;
		NTL_DELETE(pUnit);
	}

	m_mapUnit.clear();
}

void CNtlResourceScheduleManager::Update(RwReal fElapsed)
{
	// Kell
	static RwBool bCz = FALSE;
	if (bCz)
	{
		return;
	}

	MapScheduleUnit::iterator it;
	for(it = m_mapUnit.begin(); it != m_mapUnit.end(); it++)
	{
		(*it).second->Update(fElapsed);
	}
}

CNtlResourceScheduleUnit* CNtlResourceScheduleManager::FindUnit(RwUInt8 byEntityType)
{
	MapScheduleUnit::iterator it = m_mapUnit.find(byEntityType);
	if(it == m_mapUnit.end())
		return NULL;

	return (*it).second;
}

void CNtlResourceScheduleManager::AddSchedulingClump(const char *pStrName, const char * pResourcePath, CNtlPLEntity *pEntity)
{
	SResourceScheduleNode *pNode = (SResourceScheduleNode*)RwFreeListAlloc(m_pScheduleNodeFreeList, rwMEMHINTDUR_GLOBAL);
	pNode->uiResType = rwID_CLUMP;
	strcpy_s(pNode->chResourcePath, pResourcePath);
	strcpy_s(pNode->chFileName, pStrName);

	RwUInt8 byEntityType = (RwUInt8)pEntity->GetClassType();

	// 확장 by Kell
	// 캐릭터와 아이템은 다른 방식의 스케쥴링 로딩을 사용한다.
	CNtlResourceScheduleUnit *pUnit = FindUnit(byEntityType);
	if(pUnit == NULL)
	{
		if( byEntityType == PLENTITY_CHARACTER || byEntityType == PLENTITY_ITEM )
		{
			pUnit = NTL_NEW CNtlResourceScheduleCharUnit;
		}
		else
		{
			pUnit = NTL_NEW CNtlResourceScheduleUnit;
		}

		pUnit->SetEntityType(byEntityType);
		m_mapUnit[byEntityType] = pUnit;
	}

	pUnit->AddSchedulingNode(pEntity, pNode);
}

void CNtlResourceScheduleManager::UnLoadScheduling(CNtlPLEntity *pPLEntity, CNtlPLResource *pResource)
{
	if(pPLEntity == NULL)
		return;

	CNtlResourceScheduleUnit *pUnit = FindUnit((RwUInt8)pPLEntity->GetClassType());

	if(pUnit == NULL)
		return;

	pUnit->UnLoadScheduling(pPLEntity, pResource);
}

RwBool CNtlResourceScheduleManager::IsEmptyLoadScheduling(RwUInt8 byEntityType)
{
	CNtlResourceScheduleUnit *pUnit = FindUnit(byEntityType);
	if(pUnit == NULL)
		return TRUE;

	return pUnit->IsEmptyLoadScheduling();
}

RwBool GetLoadObjectSeamlessScheduling(void)
{
	return g_bLoadSeamless;
}

void SetLoadObjectSeamlessScheduling(RwBool bEnable)
{
	g_bLoadSeamless = bEnable;
}

void SetLoadObjectSchedulingOneTickTime(RwReal fTime)
{
	g_fLoadObjectTime = fTime;
}

void SetLoadObjectSeamlessSchedulingOneTickTime(RwReal fTime)
{
	g_fLoadObjectSeamlessTime = fTime;
}
#include "precomp_ntlframework.h"
#include "NtlStageManager.h"
#include "NtlStage.h"
#include "NtlDebug.h"
#include "NtlProfiler.h"

/**
*
*  Default constructor for CNtlStageManager
*
*/
CNtlStageManager::CNtlStageManager()
{
}

/**
*
*  Destructor for CNtlStageManager
*
*/
CNtlStageManager::~CNtlStageManager()
{
}


/**
*
*  \param pStageName는 stage name이다.
*
*  \return stage name에 해당하는 stage를 pointer를 리턴한다. 없으면 NULL.
*/
CNtlStage* CNtlStageManager::FindActiveStage(const char *pStageName)
{
	NTL_FUNCTION("CNtlStageManager::FindActiveStage" );

	CNtlStage *pStage = NULL;
	std::list<CNtlStage*>::iterator it;

	for(it = m_listStage.begin(); it != m_listStage.end(); it++)
	{
		const char *pName = (*it)->GetName();
		if(pName == NULL || strcmp(pName, pStageName) != 0)
			continue;

		pStage = (*it);
		m_listStage.erase(it);

		break;
	}

	NTL_RETURN(pStage);
}

RwBool CNtlStageManager::ExistStage(const char *pStageName)
{
	NTL_FUNCTION("CNtlStageManager::ExistStage" );

	CNtlStage *pStage = NULL;
	std::list<CNtlStage*>::iterator it;

	for(it = m_listStage.begin(); it != m_listStage.end(); it++)
	{
		const char *pName = (*it)->GetName();
		if(pName == NULL || strcmp(pName, pStageName) != 0)
			continue;

		NTL_RETURN(TRUE);
	}

	NTL_RETURN(FALSE);
}

CNtlStage* CNtlStageManager::GetActiveStage(void)
{
	std::list<CNtlStage*>::iterator it = m_listStage.begin();
	if(it == m_listStage.end())
		return NULL;

	return (*it);
}


/**
*  stage를 destroy 시킨다.
*  \param pStage는 destroy시킬 stage pointer이다.
*/
void CNtlStageManager::DeleteStage(CNtlStage *pStage)
{
	NTL_FUNCTION("CNtlStageManager::DeleteStage" );

	if(pStage)
	{
		pStage->Destroy();
		NTL_DELETE(pStage);
	}

	NTL_RETURNVOID();
}

/**
*  이 함수는 stage manager를 instance를 생성한 다음 곧바로 호출한다.
*  이 함수의 기능은 stage manager가 처리한 event를 등록한다.
*  event에는 stage create 와 stage delete event가 존재한다.
*/

bool CNtlStageManager::Create(void)
{
	NTL_FUNCTION("CNtlStageManager::Create" );

	RegisterMsg (g_EventCreateStage, "g_EventCreateStageStr", "SNtlEventStageData");
	RegisterMsg (g_EventDeleteStage, "g_EventDeleteStageStr", "SNtlEventStageData");

	LinkMsg(g_EventCreateStage, 0);
	LinkMsg(g_EventDeleteStage, 0);

	NTL_RETURN(true);
}

/**
*  이 함수는 stage manager 객체를 삭제하기 전에 호출한다.
*  여기에는 등록된 event를 모두 해제한다.
*  관리하고 있는 active stage를 모두 삭제한다.
*/

void CNtlStageManager::Destroy(void)
{
	NTL_FUNCTION("CNtlStageManager::Destroy" );

	UnLinkMsg(g_EventCreateStage);
	UnLinkMsg(g_EventDeleteStage);

	UnRegisterMsg ( g_EventCreateStage );
	UnRegisterMsg ( g_EventDeleteStage );

	std::list<CNtlStage*>::reverse_iterator it;

	for(it = m_listStage.rbegin(); it != m_listStage.rend(); it++)
	{
		DeleteStage(*it);
	}

	m_listStage.clear();

	NTL_RETURNVOID();
}


void CNtlStageManager::PostRender(void)
{
	NTL_SPROFILE("CNtlStageManager::PostRender")

	std::list<CNtlStage*>::reverse_iterator it;

	for(it = m_listStage.rbegin(); it != m_listStage.rend(); it++)
	{
		(*it)->PostRender();
	}

	NTL_EPROFILE()
}


/**
*  Event Handler 함수.
*  \param pMsg는 event message이며, message안에 event id와 data 정보가 포함되어 있다.
*/

void CNtlStageManager::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlStageManager::HandleEvents");

	if(pMsg.Id == g_EventCreateStage)
	{
		SNtlEventStageData *pEventData = static_cast<SNtlEventStageData*>(pMsg.pData);
		CNtlStage *pNewStage = CreateStage(pEventData->chStageName); 
		if(pNewStage->Create()) 
			m_listStage.push_back(pNewStage); 
	}

	else if(pMsg.Id == g_EventDeleteStage)
	{
		SNtlEventStageData *pEventData = static_cast<SNtlEventStageData*>(pMsg.pData);
		DeleteStage(FindActiveStage(pEventData->chStageName)); 
	}

	NTL_RETURNVOID();
}
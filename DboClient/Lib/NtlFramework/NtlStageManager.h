/*****************************************************************************
 *
 * File			: NtlStageManager.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 7. 14	
 * Abstract		: Stage Manager.
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_STAGE_MANAGER__
#define __NTL_STAGE_MANAGER__

#include "ceventhandler.h"
#include "NtlSysEvent.h"

class CNtlStage;

/**
 * \ingroup Client
 * active된 stage를 관리하는 class이다.
 * stage manager class는 event를 받아서 처리하는데, 
 * event에는 stage 생성/삭제가 존재한다.
 * event id에는 g_EventCreateStage, g_EventDeleteStage가 쓰이며,
 * NtlGlobalEvent.h에 선언되어 있다.
  */

class CNtlStageManager : public RWS::CEventHandler
{
private:

	std::list<CNtlStage*> m_listStage;	 /**< active stage objectlist*/

protected:

	/**
	* 
    *  \return CNtlStageManager class를 상속받는 class에서 함수를 재정의 하여 stage name에 해당하는 stage를 생성하여
    *  리턴한다.
    *  생성된 stage는 active stage list에 등록된다.
	*/
	virtual CNtlStage* CreateStage(const char *pStageName) = 0;

	/**
	* 
    *  stage를 삭제하고, active stage list에서 제거한다.
	*/
	void DeleteStage(CNtlStage *pStage);

	/**
    *
    *  \return 현재 active된 stage중에서 stage name과 CNtlStage를 찾아서 리턴한다.
    */
	CNtlStage* FindActiveStage(const char *pStageName);


public:

	CNtlStageManager();
	virtual ~CNtlStageManager();

	virtual bool Create(void);
	virtual void Destroy(void);

	virtual void PostRender(void);

	RwBool ExistStage(const char *pStageName);

	CNtlStage* GetActiveStage(void);
	
	/**
    *
    *  event handler
	*/
	virtual void HandleEvents(RWS::CMsg &pMsg);
};

#endif
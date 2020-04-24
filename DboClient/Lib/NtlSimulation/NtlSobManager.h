/*****************************************************************************
 *
 * File			: NtlSobManager.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 8. 23	
 * Abstract		: Simulation entity manager class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_SOB_MANAGER_H__
#define __NTL_SOB_MANAGER_H__

#include "ceventhandler.h"
#include "NtlSLDef.h"

class CNtlSob;
class CNtlSobGroup;
class CNtlSobStatusAnimSyncManager;
class CNtlSobCharPerfController;
class CNtlFrameSkipScheduler;

class CNtlSobManager : public RWS::CEventHandler
{
public:

	enum ESobGroupVisibleFilter
	{
		E_SOB_GROUP_VISIBLE_FILTER_RANGE_OUT,
		E_SOB_GROUP_VISIBLE_FILTER_CINEMATIC,
		E_SOB_GROUP_VISIBLE_FILTER_NPC_SHOP,
		E_SOB_GROUP_VISIBLE_FILTER_PRIVATE_SHOP,
		E_SOB_GROUP_VISIBLE_FILTER_END
	};

private:

	RwBool	m_bActive;				
	static RwReal			m_fOptionOutRange;
	static CNtlSobManager *m_pInstance;

	typedef std::map<SERIAL_HANDLE, CNtlSob*> MapObject;
	typedef std::map<SERIAL_HANDLE, CNtlSobGroup*> MapObjectGroup;
	MapObject		m_mapObject;
	MapObject		m_mapUpdate;
	MapObjectGroup	m_mapGroup;

	typedef std::list<CNtlSob*> ListObject;
	typedef std::list<SERIAL_HANDLE> ListHandle;
	ListHandle m_listRemoveQueue;

	typedef std::map<RwUInt32, SERIAL_HANDLE> MapSerialTrigger;
	MapSerialTrigger m_mapTriggerNpc;
	MapSerialTrigger m_mapTriggerObject;

	typedef void (CNtlSobManager::*FuncEventHander)(RWS::CMsg &pMsg);
	typedef std::map<const RWS::CRegisteredMsgs*, FuncEventHander> MapFuncEventHandler;
	MapFuncEventHandler m_mapEventFunc;

	// 특정 group에 visual proxy 처리.
	typedef std::map<SERIAL_HANDLE, void*> MapVisible;
	MapVisible m_mapGroupVisible[E_SOB_GROUP_VISIBLE_FILTER_END];

	// Character 가시거리에 따른 visual proxy 처리.
	RwReal		m_fOptiionRangeTick;

	// status animation sync 처리.
	CNtlSobStatusAnimSyncManager *m_pAnimSyncManager;

	CNtlSobCharPerfController* m_pCharPerfCtrl;

	CNtlFrameSkipScheduler *m_pMoveFrameSkip;		

private:

	CNtlSob*		FindEntity(RwInt32 uiClassId, SERIAL_HANDLE hSerialId);
	CNtlSobGroup*	FindGroup(RwInt32 uiClassId);

	void			AddTriggerIdMapping(RwInt32 uiClassId, CNtlSob *pObj);
	void			RemoveTriggerIdMapping(RwInt32 uiClassId, CNtlSob *pObj);

	void			RemoveOptionRangeOut(SERIAL_HANDLE hSerialId);

	void			UpdateOptionRangeOut(RwReal fElapsed);

public:

	CNtlSobManager();
	~CNtlSobManager();

	static void SetOptionOutRange(RwReal fRange);
	static RwReal GetOptionOutRange();
	static CNtlSobManager* GetInstance(void);

	RwBool	Create(void);
	void	Destroy(void);

	void	Reset(void);

	void	SetActive(RwBool bActive);
	RwBool	GetActive(void) const;

	void	Update(RwReal fElapsed);    

	CNtlSob* CreateObject(RwUInt32 uiClassId, SERIAL_HANDLE hSerialId = INVALID_SLCLASS_ID);
	void DeleteObject(CNtlSob *pObj);
	void DeleteObjectGroup(RwInt32 uiClassId);

	void AddObject(RwInt32 uiClassId, CNtlSob *pObj);
	MapObject::iterator RemoveObject(RwInt32 uiClassId, CNtlSob *pObj);
	void RemoveAllObject();
	void RemoveWorld(void);

	void AddUpdate(CNtlSob *pSobObj);
	void RemoveUpdateQueue(CNtlSob *pSobObj);

	void SetFlagSobGroup(RwInt32 uiClassID, RwBool bEnable, RwUInt32 nFlag);

	void AddGroupVisible(ESobGroupVisibleFilter eFilter, SERIAL_HANDLE hSerialId);
	void RemoveGroupVisible(ESobGroupVisibleFilter eFilter, SERIAL_HANDLE hSerialId);
	void VisibleAllObject(ESobGroupVisibleFilter eFilter, RwUInt32 uiClassId, RwBool bVisible);
	
	CNtlSob* GetSobObject(SERIAL_HANDLE hSerialId);
	CNtlSobGroup* GetSobGroup(RwInt32 uiClassId);
	RwUInt32 GetSobObjectCount(void) const;
	RwUInt32 GetSobObjectCount(RwInt32 uiClassId);
	RwUInt32 GetSobObjectUpdateCount(void) const;
	CNtlSob* GetSobNearWorldItem(const RwV3d& vLoc);

	// serid form trigger id
	SERIAL_HANDLE GetNpcTriggerSerialFromId(RwUInt32 uiId);
	SERIAL_HANDLE GetObjectTriggerSerialFromId(RwUInt32 uiId);


	CNtlFrameSkipScheduler* GetMoveFrameSkipScheduler(void) { return m_pMoveFrameSkip; }
	CNtlSobStatusAnimSyncManager* GetAnimSyncManager(void) { return m_pAnimSyncManager; }

	/** 
	* simulation option flags
	*
	*/

	void GameOptionShadowOnOff(RwBool bShadowOnOff);

	/**
	*  simulation entity manager의 event handler
	*  \param pMsg는 event가 발생할 경우 넘어오는 message.
	*/
	virtual void HandleEvents(RWS::CMsg &pMsg);

	void UpdateTickEventHandler(RWS::CMsg &pMsg);
	void ActionMapSkillUseEventHandler(RWS::CMsg &pMsg);

    void SobEventHandler(RWS::CMsg& pMsg);
	void SobCreateEventHandler(RWS::CMsg &pMsg);
	void SobDeleteEventHandler(RWS::CMsg &pMsg);
    void SobShareTargetReleaseHandler(RWS::CMsg& pMsg);
	void SobPlayerChangeGuildEmblemHandler(RWS::CMsg &pMsg);
	void SobPrivateShopStateEventHandler(RWS::CMsg &pMsg);
    void SobCondConfusedEventHandler(RWS::CMsg& pMsg);
    void SobCondTerrorEventHandler(RWS::CMsg& pMsg);
	void SobGetFightingMode(RWS::CMsg &pMsg);
	void PlayerNameVisibleEventHandler(RWS::CMsg &pMsg);
	void AnimFootStepEventHandler(RWS::CMsg &pMsg);
	void AnimDirectEventHandler(RWS::CMsg& pMsg);
	void AlphaBlendEventHandler(RWS::CMsg &pMsg);
	void TriggerObjectEventHandler(RWS::CMsg &pMsg);
    void TMQEventHandler(RWS::CMsg &pMsg);
	void ExplosionHandler(RWS::CMsg &pMsg);
    void PostEffectEventHandler(RWS::CMsg& pMsg);
    void BotHelpMeNotifyHandler(RWS::CMsg &pMsg);

    // Event Object
    void EventObjectEventHandler(RWS::CMsg& pMsg);
	void EventObjectThreadLoadCompleteEventHandler(RWS::CMsg& pMsg);

    // World Create
    void EventCreateWorldHandler(RWS::CMsg& pMsg);                      ///< 월드가 생성/제거 될때의 이벤트

	// Naming
	void ChangeCharName(RWS::CMsg& pMsg);
	void ChangeNickName(RWS::CMsg& pMsg);
};


static CNtlSobManager* GetNtlSobManager(void)
{
	return CNtlSobManager::GetInstance();
}


#endif 

/*****************************************************************************
 *
 * File			: NtlSobAvatar.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 9. 02	
 * Abstract		: Simulation player object
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_SOB_AVATAR_H__
#define __NTL_SOB_AVATAR_H__

#include "NtlSharedType.h"
#include "NtlSobPlayer.h"
#include "NtlSkillContainer.h"
#include "NtlSLParty.h"
#include "NtlSLGuild.h"
#include "NtlWareHouse.h"
#include "NtlOtherParam.h"
#include "NtlSLPrivateShop.h"
#include "NtlSLTMQ.h"
#include "NtlSLCCBD.h"
#include "NtlSLMailSystem.h"
#include "NtlSLRankBattle.h"
#include "NtlSLFriendList.h"
#include "NtlSLBlackList.h"
#include "NtlGuildWarehouse.h"
#include "NtlSLHoipoiMix.h"
#include "NtlSLDojo.h"
#include "NtlSLTBudokai.h"
#include "DboTSCMain.h"

class CDboTSCQAgency;
class CDboTSCTAgency;
class CDboTSCObjAgency;


class CNtlSobAvatar : public CNtlSobPlayer
{
	DECLEAR_MEMORY_POOL(CNtlSobAvatar, NTL_DEFAULT_MEMORY_POOL)

private:

	CHARACTERID					m_charID;

	CNtlInventory				m_Inventory;
	CNtlWarehouse				m_WareHouse;
	CNtlQuestInventory			m_QuestInventory;
	CNtlAvatarSkillContainer	m_SkillContainer;
	CNtlParty					m_Party;
	CNtlGuild					m_Guild;
	CNtlOtherParam				m_OtherParam;
	CNtlPrivateShop				m_PrivateShop;
	CNtlTMQ						m_TMQ;
	CNtlCCBD					m_CCBD;
	CNtlMailSystem				m_MailSystem; 
	CNtlRankBattle				m_RankBattle;
    CNtlSLFriendList            m_FriendList;
    CNtlSLBlackList             m_BlackList;
	CNtlGuildWarehouse			m_GuildWarehouse;
	CNtlTBudokai				m_TBudokai;
	CNtlSLHoipoiMix				m_HoipoiMix;
	CNtlDojo					m_Dojo;

	RwBool						m_bValidAvatarData;

	CDboTSCQAgency				*m_pQAgency;
	CDboTSCTAgency				*m_pPCTAgency;
	CDboTSCObjAgency			*m_pObjAgency;

private:

	RwBool CreateContents(void);
	void DestroyContents(void);

	void CreateEvent(void);
	void DestroyEvent(void);
		
public:

	CNtlSobAvatar();
	virtual ~CNtlSobAvatar();

	virtual RwBool Create(void);

	virtual void Destroy(void);

	virtual void ServerChangeOut(void);

	virtual void ServerChangeIn(void);

	virtual void Update(RwReal fElapsed);

	virtual CNtlFSMLayer* CreateFSM(void);

	virtual void HandleEvents(RWS::CMsg &pMsg);

public:

	virtual CHARACTERID	GetCharID();

	CNtlInventory*		GetInventory(void);
	CNtlWarehouse*		GetWarehouse(void);
	CNtlQuestInventory*	GetQuestInventory(void);
	CNtlSkillContainer* GetSkillContainer(void);
	CNtlParty*			GetParty(void);
	CNtlGuild*			GetGuild(void);
	CNtlOtherParam*		GetOtherParam(void);
	CNtlPrivateShop*	GetPrivateShop(void);
	CNtlTMQ*			GetTMQ(void);
	CNtlCCBD*			GetCCBD(void);
	CNtlMailSystem*		GetMailSystem(void);
	CNtlRankBattle*		GetRankBattle(void);
    CNtlSLFriendList*   GetFriendList(void);
    CNtlSLBlackList*    GetBlackList(void);
	CNtlGuildWarehouse*	GetGuildWarehouse(void);
	CNtlTBudokai*		GetTenkaichiBudokai(void);
	CNtlSLHoipoiMix*		GetHoipoiMix(void);
	CNtlDojo*			GetDojo(void);

	CDboTSCQAgency*		GetQuestAgency( void );
	CDboTSCTAgency*		GetPCTriggerAgency( void );

	RwBool				IsAvatarReady(void);
	RwBool				IsValidAvatarData(void);	

	void				DeleteTSAgency(void);

	// avooo's position & direction test
	virtual void				SetPosition(const RwV3d *pPos);
	virtual void				SetDirection(const RwV3d *pDir);
};

inline CHARACTERID CNtlSobAvatar::GetCharID()
{
	return m_charID;
}

inline CNtlInventory* CNtlSobAvatar::GetInventory(void) 
{
	return &m_Inventory;
}

inline CNtlWarehouse* CNtlSobAvatar::GetWarehouse(void)
{
	return &m_WareHouse;
}

inline CNtlQuestInventory* CNtlSobAvatar::GetQuestInventory(void) 
{
	return &m_QuestInventory;
}

inline CNtlSkillContainer* CNtlSobAvatar::GetSkillContainer(void)
{
	return &m_SkillContainer;
}

inline CNtlParty* CNtlSobAvatar::GetParty(void)
{
	return &m_Party;
}

inline CNtlPrivateShop* CNtlSobAvatar::GetPrivateShop(void)
{
	return &m_PrivateShop;
}

inline CNtlGuild* CNtlSobAvatar::GetGuild(void)
{
	return &m_Guild;
}

inline CNtlOtherParam* CNtlSobAvatar::GetOtherParam(void)
{
	return &m_OtherParam;
}

inline CNtlTMQ* CNtlSobAvatar::GetTMQ()
{
	return &m_TMQ;
}

inline CNtlCCBD* CNtlSobAvatar::GetCCBD()
{
	return &m_CCBD;
}

inline CDboTSCQAgency* CNtlSobAvatar::GetQuestAgency( void )
{
	CDboTSCMain* pTSMain = GetTSCMain();
	if ( NULL == pTSMain || !pTSMain->IsCreated() ) return NULL;

	return m_pQAgency;
}

inline CDboTSCTAgency* CNtlSobAvatar::GetPCTriggerAgency( void )
{
	CDboTSCMain* pTSMain = GetTSCMain();
	if ( NULL == pTSMain || !pTSMain->IsCreated() ) return NULL;

	return m_pPCTAgency;
}

inline CNtlMailSystem* CNtlSobAvatar::GetMailSystem( void )
{
	return &m_MailSystem;
}

inline CNtlRankBattle* CNtlSobAvatar::GetRankBattle(void)
{
	return &m_RankBattle;
}

inline CNtlSLFriendList* CNtlSobAvatar::GetFriendList()
{
    return &m_FriendList;
}

inline CNtlSLBlackList* CNtlSobAvatar::GetBlackList()
{
    return &m_BlackList;
}

inline CNtlGuildWarehouse* CNtlSobAvatar::GetGuildWarehouse()
{
	return &m_GuildWarehouse;
}

inline CNtlTBudokai* CNtlSobAvatar::GetTenkaichiBudokai()
{
	return &m_TBudokai;
}

inline CNtlSLHoipoiMix* CNtlSobAvatar::GetHoipoiMix()
{
	return &m_HoipoiMix;
}

inline CNtlDojo* CNtlSobAvatar::GetDojo()
{
	return &m_Dojo;
}

inline RwBool CNtlSobAvatar::IsValidAvatarData()
{
	return m_bValidAvatarData;
}

#endif
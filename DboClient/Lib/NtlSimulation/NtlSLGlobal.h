/*****************************************************************************
 *
 * File			: NtlSobGlobal.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 8. 23	
 * Abstract		: Simulation global type
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_SLGLOBAL_H__
#define __NTL_SLGLOBAL_H__


class CNtlSobAvatar;
class CNtlSLPacketGenerator;
class CNtlNetSender;
class CNtlTriggerSyncManager;

#include "NtlCharacter.h"
#include "NtlItem.h"
#include "NtlMail.h"
#include "NtlSkill.h"
#include "NtlSobSkill.h"
#include "NtlWorld.h"
#include "NtlPacketLockManager.h"
#include "NtlQuest.h"
#include "NtlRankBattle.h"
#include "NtlSLTBudokai.h"
#include "NtlTeleport.h"
#include "NtlDojo.h"

/**
* \brief Avatar Info
*/
typedef struct _SAvatarInfo
{
	RwUInt32			uiSerialId;										// avatar serial id
	sPC_PROFILE			sCharPf;										// avatar profile
	sCHARSTATE			sCharState;										// avatar state
	RwUInt16			wItemCount;									// 유효한 item count	
	sITEM_PROFILE		sItemPf[NTL_MAX_COUNT_USER_HAVE_INVEN_ITEM];	// item profile
	RwUInt8				bySkillCount;									// 우효한 skill count
	sSKILL_INFO			sSkillPf[NTL_MAX_PC_HAVE_SKILL];				// skill profile
	RwUInt8				byHTBCount;										// 유효한 HTB count
	sHTB_SKILL_INFO		sHTBPf[NTL_HTB_MAX_PC_HAVE_HTB_SKILL];			// HTB profile
	RwUInt8				byBuffCount;
	sBUFF_INFO			sBuffPf[DBO_MAX_BUFF_CHARACTER_HAS];
	sWORLD_INFO			sOldWorldInfo;
	sWORLD_INFO			sWorldInfo;
	sQUEST_COMPLETE_INFO sQuestCompleteInfo;								// 퀘스트 완료 정보
	RwUInt8				byQuestProgressInfoCount;							// 퀘스트 진행 정보 개수
	sQUEST_PROGRESS_INFO sQuestProgressInfo[eMAX_CAN_PROGRESS_QUEST_NUM];	// 퀘스트 진행 정보
	sQUEST_INVENTORY_FULLDATA sQuestInventoryInfo;							// 퀘스트 인벤토리 정보
	RwUInt8				bySetQuickSlotInfo;									// 퀵슬롯 정보가 들어오면 TRUE;
	RwUInt8				byQuickSlotInfoCount;								// 퀵슬롯 정보 개수
	sQUICK_SLOT_PROFILE	sQuickSlotInfo[NTL_CHAR_QUICK_SLOT_MAX_COUNT];		// 퀵슬롯 정보
    RwUInt8             byMaxRpStock;                                       ///< 최대 RP Stock 개수
	
	RwUInt8				byMailCount;										// 메일 갯수
	RwUInt8				byUnreadMailCountNormal;							// 안 읽은 편지 : 일반 메일
	RwUInt8				byUnreadMailCountManager;							// 안 읽은 편지 : 매니져 메일

	char				acWarFogFlag[NTL_MAX_SIZE_WAR_FOG];					// 워포그 정보

	char				abyTitleIndexFlag[NTL_MAX_CHAR_TITLE_COUNT_IN_FLAG];

	sRANKBATTLE_SCORE_INFO sRankBattleScoreInfo;							// 랭크배틀 전적 정보

	RwUInt8				byDojoBriefCount;
	sDBO_DOJO_BRIEF		sDojoBrief[DBO_MAX_COUNT_DOJO_IN_WORLD];

	PORTALID			aPortalID[NTL_PORTAL_MAX_COUNT];	// portals we have registered

	bool				bCombatMode;

}SAvatarInfo;

/**
* \brief 소환수의 정보
*/
typedef struct _SSummonPetInfo
{
	RwUInt32				uiSerialId;			// avatar summon pet serial id
	sSUMMON_PET_PROFILE		sCharPf;			// avatar summon pet profile
	sCHARSTATE				sCharState;			// avatar summon pet character state
}SSummonPetInfo;

/**
* \brief 소환수 스킬의 정보
*/
typedef struct _SSummonPetSkillInfo
{
	RwUInt8				bySkillCount;						// avatar summon pet skill count
	sSKILL_INFO_PET		aSkillInfo[NTL_MAX_NPC_HAVE_SKILL];	// avatar summon pet skill info
}SSummonPetSkillInfo;

/**
* \brief 천하제일 무도회 상태에 대한 정보
*/
typedef struct _STenkaichiBudokaiInfo
{
	sBUDOKAI_UPDATE_STATE_INFO			sStateInfo;			///< 천하제일 무도회 메인 스테이트
	sBUDOKAI_UPDATE_MATCH_STATE_INFO	sMatchStateInfo[MAX_BUDOKAI_STATE];	///< 매치 상태
	sBUDOKAI_JOIN_INFO					sJoinInfo;							///< 플레이어의 신청 상태
} STenkaichiBudokaiInfo;

/**
* \brief	!! AdjustMoney !!
*			서버와 클라이언트 간의 제니 사용 타이밍을 맞춰주기 위하여 저장. (파티참,메일 등..)
*/
struct SAdjustZennyInfo
{
	enum ZENNYADJUSTTYPE { USER_TRADE, PARTY_CHARM, MAIL_BOX, PARTY_INVEN, NUM_TYPE };
	
	RwInt32	m_aiAdjustZenny[NUM_TYPE];	

	void	Init(void);
	void	SetAdjustZenny(RwInt32 eType, RwInt32 nValue);	
	RwInt32	GetAdjustZenny(RwInt32 eType);
};

struct SAvatarRecipeInfo
{
	RwUInt16			wCount;
	sRECIPE_DATA		asRecipeData[NTL_ITEM_RECIPE_MAX_COUNT];
};

/**
* \brief	!! PassiveSkillEffect !!
*			패시브 스킬에 따른 스킬 변화를 저장. 
*/
struct SPassiveSkillEffect
{
	RwUInt32				SkillTblidx;
	RwUInt8					bySlotidx;
	sDBO_UPDATED_SKILL_DATA	sUpdatedSkillData;
};

typedef std::list<SPassiveSkillEffect>				LIST_PASSIVE_SKILL_EFFECT;
typedef std::list<SPassiveSkillEffect>::iterator	LIST_PASSIVE_SKILL_EFFECT_ITER;	

/**
* \brief	GameManiaTime
* GMT 정보를 저장
*/
struct SGameManiaTimeInfo
{
	RwUInt8 byGMTResetCount;
	sGAME_MANIA_TIME_DATA sCurrent;
	sGAME_MANIA_TIME_DATA sNext;
};

/**
* \ingroup Simulation
* \brief Simulation layer에서 자유롭게 접근할 수 있는 전역 자료구조
*/
class CNtlSLGlobal
{
private:

	static CNtlSLGlobal		*m_pInstance;
	
	SAvatarInfo				m_sAvatarInfo;						///< 게임 진입시 avatar 정보를 담는 자료구조
	STenkaichiBudokaiInfo	m_sTenkaichiBudokaiInfo;			///< 게임 진입시 천하제일 무도회 정보를 담는 자료구조
	SSummonPetInfo			m_sSummonPetInfo;					///< avatar가 summon pet 소환시 보내주는 정보
	SSummonPetSkillInfo		m_sSummonPetSkillInfo;				///< avatar가 summon pet 소환시 보내주는 정보 (skill)
	SAdjustZennyInfo		m_sAdjustZennyInfo;					///< 서버,클라이언트간 제니사용타이밍이 다름으로 인해 생기는 것을 보정.
	SAvatarRecipeInfo		m_sAvatarRecipeInfo;				///< 게임 진입시 아바타의 레시피 데티타를 담는 자료구조
	SGameManiaTimeInfo		m_sGameManiaTimeInfo;
	LIST_PASSIVE_SKILL_EFFECT m_listPassveSkillEffectInfo;		///< 아바타 생성 이전에 날라오는 스킬별 패시브이펙트 적용 정보.

	CNtlSobAvatar			*m_pSobAvatar;						///< avatar 객체 instance pointer
	RwBool					m_bWorldUpdateAvaterPos;
	DBOTIME					m_ServerTimeOffset;					///< ( 서버타임 - 클라이언트 ), 클라이언트 타임에 더해서 사용.

	HOBJECT					m_hNpcFacingHandle;

public:

	static bool				m_bSelectSkillMode;
	static SERIAL_HANDLE	m_hSelectSkillModeHandle;
	
public:

	CNtlSLGlobal();
	~CNtlSLGlobal();

	static CNtlSLGlobal* GetInstance(void);

	RwBool Create(RwBool bVirtualNet);
	void Destroy(void);

	void			Reset(void);
	void			ResetTenkaichiBudokaiInfo(void);
	
	SAvatarInfo*	GetAvatarInfo(void);

	SSummonPetInfo*			GetAvatarSummonPetInfo(void);
	SSummonPetSkillInfo*	GetAvatarSummonPetSkillInfo(void);
	STenkaichiBudokaiInfo*	GetTBudokaiStateInfo(void);
	SAdjustZennyInfo*		GetAdjustZennyInfo(void);
	SAvatarRecipeInfo*		GetAvatarRecipeInfo(void);
	SGameManiaTimeInfo*		GetGameManiaTimeInfo(void);
	LIST_PASSIVE_SKILL_EFFECT* GetPassiveSkillEffectInfo(void);
	
	void			SetSobAvatar(CNtlSobAvatar *pSobAvatar);
	CNtlSobAvatar*	GetSobAvatar(void);

	bool			IsSkillSelectMode();
	SERIAL_HANDLE	GetSkillSelectModeHandle();
	
	void			SetWorldUpdateAvatarPosition(RwBool bAvatarPos);
	RwBool			IsWorldUpdateAvaterPosition(void);

	void			SetServerTime( DBOTIME serverTime );
	DBOTIME			GetServerTime(void);

	void			SetNpcFacingHandle(HOBJECT hHandle) { m_hNpcFacingHandle = hHandle; }
	HOBJECT			GetNpcFacingHandle() { return m_hNpcFacingHandle; }
};

static CNtlSLGlobal* GetNtlSLGlobal(void)
{
	return CNtlSLGlobal::GetInstance(); 
}

// Inline ----------
inline SAvatarInfo* CNtlSLGlobal::GetAvatarInfo(void)
{
	return &m_sAvatarInfo;
}

inline SSummonPetInfo* CNtlSLGlobal::GetAvatarSummonPetInfo(void)
{
	return &m_sSummonPetInfo;
}

inline SSummonPetSkillInfo* CNtlSLGlobal::GetAvatarSummonPetSkillInfo(void)
{
	return &m_sSummonPetSkillInfo;
}

inline SAvatarRecipeInfo* CNtlSLGlobal::GetAvatarRecipeInfo(void)
{
	return &m_sAvatarRecipeInfo;
}

inline void CNtlSLGlobal::SetSobAvatar(CNtlSobAvatar *pSobAvatar)
{
	m_pSobAvatar = pSobAvatar;
}

inline void	CNtlSLGlobal::SetWorldUpdateAvatarPosition(RwBool bAvatarPos)
{
	m_bWorldUpdateAvaterPos = bAvatarPos;
}

inline RwBool CNtlSLGlobal::IsWorldUpdateAvaterPosition(void)
{
	return m_bWorldUpdateAvaterPos;
}

inline STenkaichiBudokaiInfo* CNtlSLGlobal::GetTBudokaiStateInfo( void )
{
	return &m_sTenkaichiBudokaiInfo;
}

inline SAdjustZennyInfo* CNtlSLGlobal::GetAdjustZennyInfo( void )
{
	return &m_sAdjustZennyInfo;
}

inline LIST_PASSIVE_SKILL_EFFECT* CNtlSLGlobal::GetPassiveSkillEffectInfo( void )
{
	return &m_listPassveSkillEffectInfo;
}

inline SGameManiaTimeInfo* CNtlSLGlobal::GetGameManiaTimeInfo( void )
{
	return &m_sGameManiaTimeInfo;
}

inline void CNtlSLGlobal::SetServerTime( DBOTIME serverTime )
{
	m_ServerTimeOffset = serverTime - time(NULL);
}

inline DBOTIME CNtlSLGlobal::GetServerTime(void)
{
	return time(NULL) + m_ServerTimeOffset;
}


#endif
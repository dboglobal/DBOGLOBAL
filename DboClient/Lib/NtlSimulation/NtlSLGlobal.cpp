#include "precomp_ntlsimulation.h"
#include "NtlSLGlobal.h"
#include "NtlSLDef.h"
#include "TableContainer.h"
#include "NtlDebug.h"
#include "NtlSLPacketGenerator.h"
#include "NtlSobManager.h"
#include "NtlSobAvatar.h"
#include "NtlNetSender.h"
#include "NtlTriggerDirectSync.h"
#include "NtlSobCharProxy.h"
#include "NtlSLEventFunc.h"

CNtlSLGlobal* CNtlSLGlobal::m_pInstance = 0;
bool	CNtlSLGlobal::m_bSelectSkillMode = false;
SERIAL_HANDLE	CNtlSLGlobal::m_hSelectSkillModeHandle = INVALID_SERIAL_ID;

/**
* \brief Construction
*/
CNtlSLGlobal::CNtlSLGlobal()
{
	NTL_PRE(m_pInstance == 0);
	m_pInstance = this;

	Reset();
	memset(&m_sSummonPetSkillInfo, 0, sizeof(SSummonPetSkillInfo));

	m_pSobAvatar				= NULL;
	m_bWorldUpdateAvaterPos		= TRUE;

	m_hNpcFacingHandle = INVALID_HOBJECT;
}

/**
* \brief Destruction
*/
CNtlSLGlobal::~CNtlSLGlobal()
{
	m_pInstance = NULL;
}

/**
* \brief Get Instance
*/
CNtlSLGlobal* CNtlSLGlobal::GetInstance(void)
{
	return m_pInstance;
}

/**
* \brief Create
* \param bVirtualNet		(RwBool) 가상 Network에 접속했을 때 주는 변수
* \return 성공 실패 여부
*/
RwBool CNtlSLGlobal::Create(RwBool bVirtualNet)
{
	NTL_FUNCTION("CNtlSLGlobal::Create");

   
	NTL_RETURN(TRUE);
}

void CNtlSLGlobal::Destroy(void)
{
}

void CNtlSLGlobal::Reset(void)
{
	memset(&m_sAvatarInfo, 0, sizeof(SAvatarInfo));    

	m_pSobAvatar = NULL;

	m_sAdjustZennyInfo.Init();
	m_listPassveSkillEffectInfo.clear();
	memset( &m_sAvatarRecipeInfo, 0xFF, sizeof( sRECIPE_DATA ) * NTL_ITEM_RECIPE_MAX_COUNT );
	m_sAvatarRecipeInfo.wCount = 0;

	// Gmt Reset
	memset( &m_sGameManiaTimeInfo, 0xFF, sizeof( SGameManiaTimeInfo ) );

	ResetTenkaichiBudokaiInfo();

	m_ServerTimeOffset = 0;

	m_sAvatarInfo.bCombatMode = false;
}

CNtlSobAvatar* CNtlSLGlobal::GetSobAvatar(void)
{
	if(m_pSobAvatar == NULL)
		m_pSobAvatar = reinterpret_cast<CNtlSobAvatar*>(GetNtlSobManager()->GetSobObject(m_sAvatarInfo.uiSerialId));  

	return m_pSobAvatar;
}

bool CNtlSLGlobal::IsSkillSelectMode()
{
	return m_bSelectSkillMode;
}

SERIAL_HANDLE CNtlSLGlobal::GetSkillSelectModeHandle()
{
	return m_hSelectSkillModeHandle;
}

void CNtlSLGlobal::ResetTenkaichiBudokaiInfo( void )
{
	// 천하제일 무도회 리소스
	memset(&m_sTenkaichiBudokaiInfo, 0, sizeof(STenkaichiBudokaiInfo));
	m_sTenkaichiBudokaiInfo.sMatchStateInfo[BUDOKAI_MATCH_TYPE_INDIVIDIAUL].byState = INVALID_BUDOKAI_MATCH_TYPE;
	m_sTenkaichiBudokaiInfo.sMatchStateInfo[BUDOKAI_MATCH_TYPE_TEAM].byState = INVALID_BUDOKAI_MATCH_TYPE;
	m_sTenkaichiBudokaiInfo.sJoinInfo.byMatchType = INVALID_BUDOKAI_MATCH_TYPE;
	m_sTenkaichiBudokaiInfo.sJoinInfo.byJoinState = INVALID_BUDOKAI_JOIN_STATE;
	m_sTenkaichiBudokaiInfo.sJoinInfo.byJoinResult = INVALID_BUDOKAI_JOIN_RESULT;
}

/**
* \brief SAdjustZenny
*/

void SAdjustZennyInfo::Init(void)
{
	memset( m_aiAdjustZenny, 0, sizeof( m_aiAdjustZenny ) );	
}

void SAdjustZennyInfo::SetAdjustZenny( RwInt32 eType, RwInt32 nValue )
{
	NTL_ASSERT( eType >= 0 && eType < NUM_TYPE, "SAdjustZenny::SetAdjustZenny : Invalid Adjust Type" );
	m_aiAdjustZenny[eType] = nValue;	

	if( GetNtlSLGlobal()->GetSobAvatar() )
		CNtlSLEventGenerator::SobUpdate( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(), EVENT_AIUT_ZENNY, 0 );	
}

RwInt32 SAdjustZennyInfo::GetAdjustZenny( RwInt32 eType )
{
	NTL_ASSERT( eType >= 0 && eType < NUM_TYPE, "SAdjustZenny::GetAdjustZenny : Invalid Adjust Type" );
	return m_aiAdjustZenny[eType];	
}

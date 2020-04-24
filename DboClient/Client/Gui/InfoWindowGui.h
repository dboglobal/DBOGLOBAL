////////////////////////////////////////////////////////////////////////////////
// File: InfoWindowGui.h
// Desc: Skill및 Item의 정보를 보여주는 GUI
//
// 2006.05.22 Peessi@hitel.net   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __INFO_WINDOW_GUI_H__
#define __INFO_WIDOOW_GUI_H__

#include "ceventhandler.h"
#include "gui_precomp.h"
#include "NtlPLGui.h"
#include "DboDef.h"
#include "InfoWndManager.h"
#include "DialogDefine.h"

class CNtlSobItem;
class CNtlSobItemAttr;
class CNtlSobSkill;
class CNtlSobSkillAttr;
class CNtlSobHTBSkill;
class CNtlSobActionSkill;
class CNtlSobBuff;
class CNtlSobAvatar;
class CNtlSobAvatarAttr;
struct sSKILL_TBLDAT;
struct sITEM_TBLDAT;
struct sITEM_RECIPE_TBLDAT;
struct sQUESTITEM_TBLDAT;
struct sHTB_SET_TBLDAT;
struct sACTION_TBLDAT;
struct stINFOWND_REWARD;
struct sITEM_PROFILE;
struct sNtlFakeBuff;
struct sFriendMember;
struct stSkillUpgradeInfo;
class CNtlSobBuffAttr;
struct sCASHITEM;

class CInfoWindowGui : public CNtlPLGui
{
public:
	//! Contructor & Destructor
	CInfoWindowGui(VOID);
	CInfoWindowGui( const RwChar* pName );
	~CInfoWindowGui(VOID);

	//! Operation
	VOID	Init(VOID);
	RwBool	Create(VOID);
	VOID	Destroy(VOID);

	VOID	Show( bool bShow );

	VOID	SetItemInfo( CNtlSobItem* pItem );
	VOID	SetTableItemInfo( sITEM_TBLDAT* pData );
	VOID	SetProfileItemInfo( sITEM_PROFILE* pItemProfile );	
	VOID	SetItemInfo( gui::CMDStaticBox* pmdBox, CNtlSobItem* pItem, CNtlSobItemAttr* pItemAttr, RwBool bDoClear = TRUE );
	VOID	SetTableItemInfo( gui::CMDStaticBox* pmdBox, sITEM_TBLDAT* pData, RwBool bDoClear = TRUE );
	VOID	SetProfileItemInfo( gui::CMDStaticBox* pmdBox, sITEM_TBLDAT* pData, sITEM_PROFILE* pItemProfile, RwBool bDoClear = TRUE );	

	VOID	SetQuestItemInfo( sQUESTITEM_TBLDAT* pData );
	VOID	SetJustItemNameInfo( sITEM_TBLDAT* pData );
	VOID	SetUnIdentifiedInfo(VOID);
	VOID	SetSkillInfo( CNtlSobSkill* pSkill );
	VOID	SetNotLearnSkillInfo( sSKILL_TBLDAT* pData );
	VOID	SetSkillRQInfo( RwInt32 eRQState );
	VOID	SetBuffInfo( CNtlSobBuff* pBuff );
	VOID	SetBuffInfo( sSKILL_TBLDAT* pData );
	VOID	SetBuffInfo( sITEM_TBLDAT* pData );
	VOID	SetFakeBuffInfo( sNtlFakeBuff* pBuff );
	VOID	SetHTBSkillInfo( CNtlSobHTBSkill* pHTBSkill );
	VOID	SetNotLearnHTBSkillInfo( sHTB_SET_TBLDAT* pData );
	VOID	SetJustWTextInfo( WCHAR* szText );
	VOID	SetActionSkillInfo( CNtlSobActionSkill* pActionSkill );
	VOID	SetRewardInfo( stINFOWND_REWARD* pReward );
	VOID	SetMiniMapInfo(MINIMAPINFO_LIST* pMINIMAPINFO);
	VOID    SetPortalInfo(RwUInt32 nPortalID, RwInt32 ePortalIconType, DWORD dwZenny);
	VOID    SetFriendInfo(sFriendMember* pMember);                             // 친구 리스트에 등록된 친구의 세부정보 표시 툴팁
	VOID	SetUpgradeInfo( stINFOWND_UPGRADE* pUpgrade );
	VOID	SetBattleAttributeDisplay( stINFOWND_BATTLEATTR* pBattleAttr );
	VOID	SetQuestSearch( QUEST_SEARCH_LIST* pQuestInfo );
	VOID	SetQuestIndicateInfo( RwUInt32 uiQuestTitle, RwUInt32 uiQuestGoal );
	VOID	SetSkillUpgradeInfo( stSkillUpgradeInfo* pUpgradeInfo );
	VOID	SetRecipeInfo( sITEM_RECIPE_TBLDAT* pRecipeTblData );

	void	SetHlsItemInfo(sCASHITEM* pData);

	//! Position 
	VOID	CalcPos( RwInt32 nXPos, RwInt32 nYPos );

private:
	//! Implementation

	// Item /////////////////////////////////////////////////////////////////////////////////
	VOID	SetItemInfo_Name( gui::CMDStaticBox* pmdBox, sITEM_TBLDAT* pData, RwUInt8 ucGrade, RwUInt8 ucRank );
	VOID	SetItemInfo_CommonPointType(gui::CMDStaticBox* pmdBox, sITEM_TBLDAT* pData);
	VOID	SetItemInfo_Attr_Info( gui::CMDStaticBox* pmdBox, sITEM_TBLDAT* pData, RwUInt8 byBattleAttribute );
	VOID	SetItemInfo_EquipLimit( gui::CMDStaticBox* pmdBox, sITEM_TBLDAT* pData, CNtlSobAvatarAttr* pAvatarAttr );

	void	SetItemInfo_DogiBallEffect(gui::CMDStaticBox* pmdBox, sITEM_TBLDAT* pData, CNtlSobAvatarAttr* pAvatarAttr, CNtlSobItemAttr* pItemAttr);

	RwBool	SetItemInfo_SpecialClass( gui::CMDStaticBox* pmdBox, sITEM_TBLDAT* pData, CNtlSobAvatarAttr* pAvatarAttr );
	VOID	SetItemInfo_Parameter( gui::CMDStaticBox* pmdBox, sITEM_TBLDAT* pData, RwUInt8 byGrade );
	VOID	SetItemInfo_SpecialEffect( gui::CMDStaticBox* pmdBox );
	RwBool	SetItemInfo_Option( gui::CMDStaticBox* pmdBox, sITEM_TBLDAT* pData, RwUInt32 uiOptionTblIdx, RwBool bShowTitle, RwBool bBlankLine, RwInt32 nOrder = 0, RwUInt32 uiColor = INFOCOLOR_7 );
	RwBool	SetItemInfo_RandomOption(gui::CMDStaticBox* pmdBox, sITEM_TBLDAT* pData, sITEM_OPTION_SET* pItemOptionSet, RwBool bShowTitle, RwBool bBlankLine, RwInt32 nOrder = 0, RwUInt32 uiColor = INFOCOLOR_7);

	VOID	SetItemInfo_Durablity( gui::CMDStaticBox* pmdBox, RwUInt8 ucCurDur, RwUInt8 ucMaxDur, RwBool bJustMaxDurablity = false);
	VOID	SetItemInfo_SetItem( gui::CMDStaticBox* pmdBox, sITEM_TBLDAT* pData, CNtlSobAvatar* pAvatar );
	BOOL	SetItemInfo_LimitState( gui::CMDStaticBox* pmdBox, sITEM_TBLDAT* pData, CNtlSobItemAttr* pSobItemAttr = NULL, sITEM_PROFILE* pItemProfile = NULL );
	VOID	SetItemInfo_SellPrice( gui::CMDStaticBox* pmdBox, sITEM_TBLDAT* pData, RwUInt8 byStackCount );
	VOID	SetItemInfo_RepairCost( gui::CMDStaticBox* pmdBox, sITEM_TBLDAT* pData, RwUInt8 byCurDur, RwUInt8 byMaxDur );

	VOID	SetItemInfo_StackCount(gui::CMDStaticBox* pmdBox, RwUInt8 byStackCount, RwUInt8 byMaxStackCount, BOOL bNewLine);
	/////////////////////////////////////////////////////////////////////////////////////////

	// Skill ////////////////////////////////////////////////////////////////////////////////
	VOID	SetSkillInfo_NameGradeClass( sSKILL_TBLDAT* pData, RwBool bLearned );
	VOID	SetSkillInfo_LearnCondition( sSKILL_TBLDAT* pData, RwBool bGray );
	VOID	SetSkillInfo_UseCondition( sSKILL_TBLDAT* pData, sSKILL_TBLDAT* pBaseData, RwBool bGrayed = FALSE, CNtlSobSkill* pSkill = NULL ); // pSkill 적용시 RPBonusType관련 텍스트 출력 및 Required EP Factor 적용
	VOID	SetSkillInfo_EffectData( sSKILL_TBLDAT* pData, RwUInt32 uiColor, RwInt32 nOrder = 0,CNtlSobSkill* pSkill = NULL ); 
	VOID	SetSkillInfo_AppointApply( sSKILL_TBLDAT* pData, sSKILL_TBLDAT* pBaseData = NULL );
	VOID	SetSkillInfo_AppointApplySetString( std::string stringKey, RwUInt8 byRange1 = 0, RwUInt8 byRange2 = 0, BYTE byMaxTarget = 0, sSKILL_TBLDAT* pBaseData = NULL, RwInt32 i = 0 );
	VOID	SetSkillInfo_Parameter( CNtlSobSkill* pSkill, sSKILL_TBLDAT* pData, sSKILL_TBLDAT* pBaseData, RwBool bGrayed );

	VOID	SetSkillInfo_Usable(sSKILL_TBLDAT* pData);

	VOID	SetHTBInfo_NameGradeClass( sHTB_SET_TBLDAT* pData, RwBool bLearned );
	VOID	SetHTBInfo_LearnCondition( sHTB_SET_TBLDAT* pData );
	VOID	SetHTBInfo_UseCondition( sHTB_SET_TBLDAT* pData );
	VOID	SetHTBInfo_EffectData( sHTB_SET_TBLDAT* pData );
	VOID	SetHTBInfo_ApplyParameter( CNtlSobHTBSkill* pHTB, sHTB_SET_TBLDAT* pData );
	/////////////////////////////////////////////////////////////////////////////////////////

	// Buff /////////////////////////////////////////////////////////////////////////////////
	VOID	SetBuffInfo_Common( sSKILL_TBLDAT* pData, RwReal fKeepTime, sDBO_BUFF_PARAMETER* pParameter = NULL );
	VOID	SetBuffInfo_Common( sITEM_TBLDAT* pData );		
	VOID	SetBuffInfo_RemainTime( RwReal fMaxTime, RwReal fTime );
	VOID	SetBuffInfo_DropInfo(CNtlSobBuffAttr* pAttr);
	VOID	SetBuffInfo_Restriction(CNtlSobBuffAttr* pAttr);
	void	SetBuffInfo_AutoRecover(CNtlSobBuffAttr* pAttr);
	/////////////////////////////////////////////////////////////////////////////////////////

	// Reward ///////////////////////////////////////////////////////////////////////////////
	VOID	SetRewardInfo_Exp( RwInt32 nValue );
	VOID	SetRewardInfo_Zenny( RwInt32 nValue );
	VOID	SetRewardInfo_Class( const WCHAR* wchText );
	VOID	SetRewardInfo_Reputation( RwInt32 nValue );
	/////////////////////////////////////////////////////////////////////////////////////////

	// Battle Attr //////////////////////////////////////////////////////////////////////////
	VOID	SetBattleAttrInfo_Weapon( stINFOWND_BATTLEATTR* pAttr );
//	VOID	SetBattleAttrInfo_Armor( stINFOWND_BATTLEATTR* pAttr );
//	VOID	SetBattleAttrInfo_Attack_Rate( stINFOWND_BATTLEATTR* pAttr );
//	VOID	SetBattleAttrInfo_Defence_Rate( stINFOWND_BATTLEATTR* pAttr );
	VOID	SetBattleAttrInfo_Upgrade( stINFOWND_BATTLEATTR* pAttr );

	// Etc
	RwBool	GetSystemEffectData( RwUInt32 uiSystemEffectIdx, RwReal fValue, std::wstring& wstrText, RwUInt8 byPercentType, sSKILL_TBLDAT* pSkillData = NULL, RwUInt32* pColor = NULL, RwBool* pBuffShow = NULL );
	VOID	EquipInfoProc( sITEM_TBLDAT* pData );
	RwBool  SetEquipInfo( gui::CMDStaticBox* pmdBox, RwUInt32 hSerial );

	// Recipe ( Item Data의 툴팁과, 레시피의 툴팁 )
	VOID	SetItemInfo_RecipeInfo( gui::CMDStaticBox* pmdBox, sITEM_TBLDAT* pData );

	//! Variables
	gui::CMDStaticBox*	m_pmdBox;

	gui::CDialog*		m_pEpuipDlg[2];
	gui::CMDStaticBox*	m_pmdEquipInfo[2];	

	gui::CPanel*		m_ppnlBeadSlot[3];
	gui::CPanel*		m_ppnlBeadItem[3];
};


#endif//__INFO_WINDOW_GUI_H__
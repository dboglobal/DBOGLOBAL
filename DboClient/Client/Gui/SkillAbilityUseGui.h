/*****************************************************************************
*
* File			: SkillAbilityUseGui.h
* Author		: Cho HaeSung
* Copyright		: (аж)NTL
* Date			: 2008. 11. 17
* Abstract		: DBO Skill Ability Use
*****************************************************************************
* Desc          : 
*
*****************************************************************************/

#ifndef __SKILL_ABILITY_USE_GUI_H__
#define __SKILL_ABILITY_USE_GUI_H__

#define dSKILL_ABILITY_ELAPSED_TIME		2.0f
#define dSKILL_ABILITY_WIDTH_OFFSET		70
#define dSKILL_ABILITY_ICON_WIDTH		64
#define dSKILL_ABILITY_ICON_HEIGHT		64

#include "ceventhandler.h"
#include "NtlPLGui.h"
#include "SkillTable.h"
#include "NtlSobSkillIcon.h"

class CSkillAbilityUseGui;

class CSkillAbilityUseIcon
{
public:
	// Icon Pos
	enum eICONPOS
	{
		ICONPOS_1,
		ICONPOS_2,
		ICONPOS_3,
		ICONPOS_4,
		ICONPOS_5,
		ICONPOS_6,

		ICONPOS_NUMS,
		INVALID_ICONPOS = 0xFF
	};

	CSkillAbilityUseIcon();
	~CSkillAbilityUseIcon();

	VOID	Create( CSkillAbilityUseGui* pGui, gui::CFlash* pComponent, RwUInt8 byAbilityType );
	VOID	Destroy();

	VOID	InitIcon();
	VOID	SetIconPos( RwUInt8 byIndex );
	void	SetIconNumber();

	VOID	Update( RwReal fElapsed );

	VOID	GotoStartFrame();
	VOID	GotoEndFrame();
	VOID	GotoAutoFrame();
	VOID	GotoDisableFrame();

	VOID	OnFSCallBack( const char* pChar1, const char* pChar2 );
	VOID	OnMovieEnd( gui::CComponent* pComponent );
	void	OnMouseDown(const CKey& key);
	
	VOID	PlayMovie( RwBool bPlay );
	VOID	Show( RwBool bShow );

	VOID	Select( RwBool bSelect );
	
	VOID	Enable( RwBool bEnable );
	RwBool	IsEnable();

	RwUInt8	GetIconIndex();
	RwBool	IsVisible();

protected:
	gui::CFlash*		m_pFlaUseIcon;
	gui::CStaticBox*	m_pStbNumber;
	gui::CSlot			m_slotFSCallBack;
	gui::CSlot			m_slotMovieEnd;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseDown;
	
	RwUInt8			m_byAbilityType;
	RwUInt8			m_byIconIndex;

	RwBool			m_bSelected;
	RwBool			m_bEnable;

	CSkillAbilityUseGui* m_pSkillAbilityUseGui;
};

class CSkillAbilityUseGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CSkillAbilityUseGui( const RwChar* pName );
	~CSkillAbilityUseGui();

	RwBool	Create(VOID);
	VOID	Destroy(VOID);

	VOID	Update( RwReal fElapsed );
	VOID	HandleEvents( RWS::CMsg &msg );

	VOID	SetAbility( RwUInt8 byIconIndex, RwUInt8 byAbilityType, sSKILL_TBLDAT* pSkillTbl, RwBool bEnable = TRUE );

	RwInt32	SwitchDialog( bool bOpen );

	void	ShowAllIcon(RwBool bShow );
	VOID	SetActive( RwBool bActive );

	VOID	InitIcon();
	VOID	AddShowIcon( RwUInt8 byAddIconNum );

	VOID	SelectedSkillAbility( RwUInt8 byAbilityType );

	VOID	SetAdjustGui();

	VOID	OnKeyDown( gui::CComponent* pComponent, CInputDevice* pDevice, const CKey& key );

protected:
	CSkillAbilityUseIcon	m_UseIcon[DBO_RP_BONUS_TYPE_COUNT];
	CNtlSobSkillIcon*		m_pSkillIcon;
	gui::CSlot				m_slotKeyDown;

	RwReal					m_fElapsedTime;
	RwUInt8					m_byShowIconNum;

	RwBool					m_bActive;
	RwBool					m_bSelected;
	RwBool					m_bInput;
};


#endif//__SKILL_ABILITY_USE_GUI_H__
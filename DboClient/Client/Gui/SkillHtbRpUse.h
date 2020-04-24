/*****************************************************************************
*
* File			: SkillAbilityUseGui.h
* Author		: Cho HaeSung
* Copyright		: (주)NTL
* Date			: 2008. 11. 17
* Abstract		: DBO Skill Ability Use
*****************************************************************************
* Desc          : 
*
*****************************************************************************/

#ifndef __SKILL_HTB_RP_USE_H__
#define __SKILL_HTB_RP_USE_H__

#define dSKILL_HTB_RP_USE_TIME			3.0f

#define dSKILL_HTB_RP_HEIGHT			100

#define dSKILL_HTB_RP_WIDTH_OFFSET		50					///< 각 아이콘간의 간격
#define dSKILL_HTB_RP_ICON_WIDTH		64					///< ICON 넓이
#define dSKILL_HTB_RP_ICON_HEIGHT		64					///< ICON 높이
#define dSKILL_HTB_RP_ICON_NUMS			7					///< ICON 갯수
#define dSKILL_HTB_RP_FLASH_BALL		"HTB_ball.swf"
#define dSKILL_HTB_RP_FLASH_NONE		"HTB_cancel.swf"

#define dSKILL_HTP_RP_TIME_OFFSET_Y		75					///< PROGRESS BAR의 위치
#define dSKILL_HTB_RP_PGB_OFFSET_X		23
#define dSKILL_HTB_RP_PGB_OFFSET_Y		7

#include "ceventhandler.h"
#include "NtlPLGui.h"

class CSkillHtbRpUse;

/**
* \brief Rpball select icon
*/
class CSkillHtbRpUseIcon
{
public:
	// Construction & Destruction
	CSkillHtbRpUseIcon();
	~CSkillHtbRpUseIcon();

	// Create & Destroy
	VOID		Create( CSkillHtbRpUse* pGui, const RwChar* pFlashName, RwUInt8 byIndex );
	VOID		Destroy(VOID);

	VOID		Update( RwReal fElapsed );

	VOID		Show( RwBool bShow );
	RwBool		IsShow();

	VOID		SetPosition( RwInt32 nPosX, RwInt32 nPosY );

	VOID		GotoStartFrame();
	VOID		GotoEndFrame();
	VOID		GotoAutoFrame();
	VOID		GotoDisableFrame();

	VOID		OnFSCallBack( const char* pChar1, const char* pChar2 );
	VOID		OnMovieEnd( gui::CComponent* pComponent );

	VOID		PlayMovie( RwBool bPlay );
	VOID		SetText( const char* pChar );
	VOID		ClearText();

	VOID		Select( RwBool bSelect );
	VOID		Enable( RwBool bEnable );
	RwBool		IsEnable();

	RwUInt8		GetIndex();

protected:
	CSkillHtbRpUse*		m_pGui;
	gui::CFlash*		m_pFlaRpBall;
	gui::CStaticBox*	m_pStbRpBall;

	gui::CSlot			m_slotFSCallback;
	gui::CSlot			m_slotMovieEnd;

	RwUInt8		m_byIndex;

	RwBool		m_bShow;
	RwBool		m_bSelected;
	RwBool		m_bEnable;
};

/**
* \brief Rpball select for HTB Skill
*/
class CSkillHtbRpUse : public CNtlPLGui, public RWS::CEventHandler
{
public:
	/// Construction & Destruction
	CSkillHtbRpUse( const RwChar* pName );
	~CSkillHtbRpUse();

	// Create & Destroy
	RwBool		Create(VOID);
	VOID		Destroy();

	// Update
	VOID		Update( RwReal fElapsed );

	// SwitchDialog & HandleEvents
	RwInt32		SwitchDialog( bool bOpen );
	VOID		HandleEvents( RWS::CMsg& msg );

	// Signal callback
	VOID		OnKeyDown( gui::CComponent* pComponent, CInputDevice* pDevice, const CKey& key );
	
	// Implements
	VOID		SelectedHtpRpUse( RwUInt8 byIndex );
	VOID		InitAllIcon();
	VOID		ShowAllIcon( RwBool bShow );
	
protected:
	CSkillHtbRpUseIcon	m_aRpuseIcon[dSKILL_HTB_RP_ICON_NUMS];	///< rp ball(icon) flash component
	CSkillHtbRpUseIcon	m_RpUseIconNone;						///< rp ball(none) flash component

	gui::CPanel*		m_pPnlGaugeBack;
	gui::CProgressBar*	m_pPgbGaugeTime;						

	gui::CSlot			m_slotKeyDown;

	RwReal				m_fPlayTime;							///< play time

	RwBool				m_bActive;								///< is active gui?
	RwBool				m_bSelect;								///< is select rp ball?
	RwBool				m_bInput;
	
	RwUInt32			m_uiCurRpStock;							///< Active 일 때만 유효한 Rp
};


#endif//__SKILL_HTB_RP_USE_H__
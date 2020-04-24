/*****************************************************************************
* 
*  File			: TargetHpGui.h
*  Author		: HyungSuk, Jang
*  Copyright	: (аж)NTL
*  Date			: 2005. 12. 12	
*  Abstract		: DBO Target hp gui.
* ****************************************************************************
*  Desc         : 
* 
* ****************************************************************************/

#ifndef __TARGETHP_GUI_H__
#define __TARGETHP_GUI_H__


#include "HpGui.h"


class CNtlSob;
class CBuffDispGui;

class CTargetHpGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	enum TYPE_BATTLEATTR { TYPE_WEAPON, TYPE_ARMOR, TYPECOUNT, TYPE_MOB };

	CTargetHpGui();
	CTargetHpGui(const RwChar* pName);
	~CTargetHpGui();

	RwBool	Create(VOID);
	VOID	Destroy(VOID);

	VOID	Update( RwReal fElapsed );

	RwInt32 SwitchDialog(bool bOpen);

	virtual VOID HandleEvents(RWS::CMsg &pMsg);

private:
	// Implementation
	RwBool	TargetUpdateProc(VOID);

	VOID	SetRaceIcon( RwUInt8 byRace );
	VOID	SetTargetRaceIcon(RwUInt8 byRace);
	VOID	SetMobNpcTypeIcon( RwUInt8 byType );
	VOID	SetTargetMobNpcTypeIcon(RwUInt8 byType);
	VOID	SetMobGrade( RwUInt8 byMobGrade );
	VOID	SetNPCJobIcon( RwUInt8 byNPCJob );
	VOID	SetObjectIcon( RwUInt32 uiObjectFunction );

	VOID	OnMove( RwInt32 nX, RwInt32 nY );
	VOID	OnClickMenuBtn( gui::CComponent* pComponent );
	VOID	OnClickShareTargetBtn( gui::CComponent* pComponent );
	/*VOID	OnPowerPaint(VOID);
	VOID	OnPowerMove( RwInt32 nOldX, RwInt32 nOldY );*/

	VOID	OnMouseUpTargetsTarget(const CKey& key);
	VOID	OnMouseEnterTargetsTarget(gui::CComponent* pComponent);
	VOID	OnMouseLeaveTargetsTarget(gui::CComponent* pComponent);

	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotClickMenuBtn;
	gui::CSlot			m_slotClickShareTargetBtn;
	/*gui::CSlot		m_slotPowerPaint;
	gui::CSlot			m_slotPowerMove;*/
	gui::CSlot			m_slotMouseUpTargetsTarget;
	gui::CSlot			m_slotMouseEnterTargetsTarget;
	gui::CSlot			m_slotMouseLeaveTargetsTarget;

	gui::CStaticBox*	m_psttNickName;
	gui::CStaticBox*	m_psttJobName;
	gui::CPanel*		m_ppnlHp;
	gui::CProgressBar*	m_ppgbHp;
	gui::CStaticBox*	m_sttHp;
	gui::CPanel*		m_ppnlEp;
	gui::CProgressBar*	m_ppgbEp;
	gui::CStaticBox*	m_sttEp;
	gui::CButton*		m_pbtnMenu;
	gui::CButton*		m_pbtnShareTarget;
	
	gui::CPanel*		m_ppnlRaceIcon;
	gui::CPanel*		m_ppnlMobGrade;

	gui::CPanel*		m_ppnlNpcJob;

	gui::CPanel*		m_ppnlSelfattack;
	gui::CPanel*		m_ppnlfirstattack;
	gui::CStaticBox*	m_pstSelfattack;
	gui::CStaticBox*	m_pstfirstattack;

	gui::CPanel*		m_ppnlPlayerLevel;
	gui::CStaticBox*	m_pstPlayerLevel;

	CNtlSob*			m_pTargetSobObj;
	CNtlSob*			m_pTargetsTargetSobObj;

	gui::CStaticBox*	m_psttTargetName;
	gui::CPanel*		m_ppnlTargetIcon;
	gui::CPanel*		m_pbtTargetIcon;
	gui::CPanel*		m_ppnlTargetLpEdBarBack;
	gui::CProgressBar*	m_ppgbTargetAggroLpGauge;
	gui::CProgressBar*	m_ppgbTargetAggroEpGauge;

	// Buff
	CBuffDispGui*		m_pBuff;

protected:

	VOID	SetScrambleOwner(RwUInt8 bySealOwnerTeam);
};

#endif
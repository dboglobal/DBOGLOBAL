/******************************************************************************
* File			: ScrambleStatusGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2009. 4. 10
* Abstract		: 
*****************************************************************************
* Desc			: 도장전 진행상태를 표시한다.
*
*				  또한 도장전 말풍선 관리자의 생성, 소멸을 주관한다
*****************************************************************************/

#pragma once

// shared
#include "NtlSharedType.h"

// core
#include "ceventhandler.h"

// sound
#include "NtlSoundDefines.h"

// presetation
#include "NtlPLGui.h"

// simulation
#include "NtlSLDef.h"

// client
#include "SurfaceGui.h"
#include "RwTextureGui.h"

struct sEmblemFactor;
class ScrambleBalloonHelper;


class CScrambleStatusGui : public CNtlPLGui, public RWS::CEventHandler
{
#define dNUM_SEAL				(5)

	enum eINFO_WINDOW_TYPE
	{
		INFO_WINDOW_TYPE_DOJO_NAME_IN_UPPER_EMBLEM,
		INFO_WINDOW_TYPE_DOJO_NAME_IN_LOWER_EMBLEM,

		NUM_INFO_WINDOW_TYPE,

		INVALID_INFO_WINDOW_TYPE
	};

public:
	CScrambleStatusGui(const RwChar* pName);
	virtual ~CScrambleStatusGui();

	RwBool			Create();
	VOID			Update(RwReal fElapsed);
	VOID			Destroy();

	RwInt32			SwitchDialog(bool bOpen);

protected:
	CScrambleStatusGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			CheckInfoWindow();	

	VOID			SetComponent_of_Team();

	VOID			InitEmblem();
	VOID			SetEmblem(RwBool bEnemyTeamTeam, sEmblemFactor* pEmblemFactor);

	VOID			InitSealState();
	VOID			SetSealState(RwUInt8 byObjectIndex, eDBO_TEAM eDBOTeam);

	VOID			SetScrambleBGM(RwUInt8 byDojoState);

	VOID			CheckTeleportButton();

	VOID			OnClicked_MinimizeButton(gui::CComponent* pComponent);
	VOID			OnClicked_JoinButton(gui::CComponent* pComponent);

	VOID			OnMove( RwInt32 iOldX, RwInt32 iOldY );
	VOID			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID			OnMouseLeave(gui::CComponent* pComponent);
	VOID			OnPaint();

protected:
	gui::CSlot			m_slotMinimizeButton;
	gui::CSlot			m_slotJoinButton;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotPaint;

	gui::CButton*		m_pMinimizeButton;
	gui::CButton*		m_pJoinButton;

	gui::CStaticBox*	m_pDialogName;
	gui::CStaticBox*	m_pEnemyTeam_GuildName;
	gui::CStaticBox*	m_pEnemyTeam_PeopleCount;
	gui::CStaticBox*	m_pEnemyTeam_SealCount;
	gui::CStaticBox*	m_pEnemyTeam_Point;
	gui::CStaticBox*	m_pMyTeam_GuildName;
	gui::CStaticBox*	m_pMyTeam_PeopleCount;
	gui::CStaticBox*	m_pMyTeam_SealCount;
	gui::CStaticBox*	m_pMyTeam_Point;
	gui::CStaticBox*	m_pTimer;

	gui::CPanel*		m_pEnemyTeam_PeoplePanel;	
	gui::CPanel*		m_pEnemyTeam_SealPanel;
	gui::CPanel*		m_pStatus_Seal[dNUM_SEAL];			///< 인장 상태 표시
	gui::CPanel*		m_pMyTeam_PeoplePanel;
	gui::CPanel*		m_pMyTeam_SealPanel;

	gui::CProgressBar*	m_pEnemyTeam_PointGause;
	gui::CProgressBar*	m_pMyTeam_PointGause;

	CRwTextureGui*		m_pEnemyTeamEmblem;
	CRwTextureGui*		m_pMyTeamEmblem;
		
	CSurfaceGui			m_srfStatus_Background;
	CSurfaceGui			m_srfEnemyTeam_Background;
	CSurfaceGui			m_srfEnemyTeam_GauseBack;
	CSurfaceGui			m_srfEnemyTeam_NoEmblem;	
	CSurfaceGui			m_srfMyTeam_Background;
	CSurfaceGui			m_srfMyTeam_GauseBack;
	CSurfaceGui			m_srfMyTeam_NoEmblem;

	eINFO_WINDOW_TYPE	m_eInfoWindowType;
	SOUND_HANDLE		m_hBGMSound;

	ScrambleBalloonHelper*	m_pScrambleBalloonHelper;
};
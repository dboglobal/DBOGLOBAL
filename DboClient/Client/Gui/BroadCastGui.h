/******************************************************************************
* File			: BroadCastGui.h
* Author		: Haesung Cho
* Copyright		: (주)NTL
* Date			: 2008. 10. 27
* Abstract		: 
*****************************************************************************
* Desc			: 게임 TS message, Server emergency, Mini narration을 담당하는 GUI
*****************************************************************************/

#ifndef __BROAD_CAST_GUI_H__
#define __BROAD_CAST_GUI_H__

#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

// Client UI Config
#include "DBOUIConfig.h"

// BroadCast
#include "BroadCastUnit.h"
#include "NtlPLAccelController.h"

// Client gui
#include "SurfaceGui.h"

#define dBROAD_BALLOON_SHAPE_NUMS		3
#define dBROAD_BALLOON_BTS_DEFAULT_IMG	"BTS_Default.png"
#define dBROAD_BALLOON_BE_DEFAULT_IMG	"BE_Default.png"
#define dBROAD_HTML_TEXT_STANDARD		33
#define dBROAD_HTML_TEXT_LARGE_WIDTH	286
#define dBROAD_HTML_TEXT_SMALL_WIDTH	143
#define dBROAD_HTML_TEXT_HEIGHT			55
#define dBROAD_HTML_TEXT_LARGE_POSITION_X	-325
#define dBROAD_HTML_TEXT_SMALL_POSITION_X	-185
#define dBROAD_HTML_TEXT_POSITION_Y			60

// 전방 선언
class CNtlPLAccelController;
class CBroadCastUnit;

// unit deque define
typedef std::deque< CBroadCastUnit > deqBroadUnit;

/**
* \ingroup client
*/
class CBroadCastGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	// 브로드캐스트 GUI의 상태
	enum eBroadState
	{
		STATE_DIRECTION_IN_START,	// 나탄나는 연출 시작
		STATE_DIRECTION_IN,			// 나타나는 연출 중
		STATE_DIRECTION_IN_END,		// 나타나는 연출 끝			- 슬라이드의 경우 말풍선을 띄워준다.
		STATE_OPEN,					// 열린 상태
		STATE_DIRECTION_OUT_START,	// 사라지는 연출 시작		- 슬라이드의 경우 말풍선을 닫는다.
		STATE_DIRECTION_OUT,		// 사라지는 연출 중
		STATE_DIRECTION_OUT_END,	// 사라지는 연출 끝
		STATE_CLOSE,				// 닫힌 상태

		STATE_NUMS,
		INVALID_STATE = 0xFF
	};

	CBroadCastGui(const RwChar* pName);
	virtual ~CBroadCastGui();

	RwBool		Create();
	VOID		Destroy();

	// Updates
	VOID		Update( RwReal fElapsed );
	VOID		UpdateDirectionInStart( RwReal fElapsed );
	VOID		UpdateDirectionIn( RwReal fElapsed );
	VOID		UpdateDirectionInEnd( RwReal fElapsed );
	VOID		UpdateOpen( RwReal fElapsed );
	VOID		UpdateDirectionOutStart( RwReal fElapsed );
	VOID		UpdateDirectionOut( RwReal fElapsed );
	VOID		UpdateDirectionOutEnd( RwReal fElapsed );
	VOID		UpdateClose( RwReal fElapsed );

	// HandleEvents
	VOID		HandleEvents( RWS::CMsg &msg );
	RwInt32		SwitchDialog( bool bOpen );

	// Interfaces
	VOID		SetState( RwUInt8 byState );
	VOID		SetCurUnitData( RwUInt8 byMsgType, vecdef_BroadMsgDataList& vecList );
	VOID		SetCurMsgData( CBroadCastUnit& unit );
	VOID		SetHtmlString( const WCHAR* pString, RwInt32 nSize );
	VOID		SaveCurUnitData();
	VOID		DeleteDeqUnit( RwInt8 byMsgType );
	VOID		DeleteDeqUnitAll();
	VOID		ShowBalloon( RwUInt8 byBalloonShape, RwBool bSmall = FALSE, RwBool bShow = TRUE);
	VOID		HideAllBalloon();

	VOID		SetIllust( const RwChar* pIllustName );
	VOID		SetIllust( RwUInt32 uiOwnerTblIdx, RwUInt32 uiOwnerCondition );
	VOID		SetillustPos( RwInt32 nX, RwInt32 nY );

	VOID		Play( RwBool bPlay );
	RwBool		IsPlay();

	// Signal back
	VOID		OnPaint();
	VOID		OnMove( RwInt32 nOldX, RwInt32 nOldY );


	// Gui
protected:
	gui::CPanel*		m_pPnlBack;
	gui::CStaticBox*	m_pStbName;
	gui::CFlash*		m_paFlaBalloon[dBROAD_BALLOON_SHAPE_NUMS];
	gui::CFlash*		m_paFlaBalloonSmall[dBROAD_BALLOON_SHAPE_NUMS];
	gui::CHtmlBox*		m_pHtmlText;
	gui::CPanel*		m_pPnlIllust;
	CSurfaceGui			m_surIllust;

	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotMove;

protected:
	// 업데이트 함수의 함수 포인터
	typedef				VOID (CBroadCastGui::*UpdateState)( RwReal );
	UpdateState			m_aCallUpdateState[STATE_NUMS];	///< Update 함수 포인터

	RwUInt8				m_byState;			///< UI의 현재 상태
	deqBroadUnit		m_deqUnit;			///< Unit의 대기열
	
	CBroadCastUnit		m_CurUnit;			///< 현재 출력중인 Unit의 Data
	RwBool				m_bCurUnit;			///< 현재 출력중인 Unit의 데이타 유효/무효 여부

	sBROAD_MSG_DATA		m_sCurData;			///< 현재 출력되고 있는 Data ( Setting )	
	RwUInt8				m_uSlidingFadeAlpha;		///< 현재의 Alpha ( Sliding/Fade 일 때만 쓰임 )
	RwBool				m_bSmallText;		///< 현재 출력되고 있는 Text가 Large인지의 여부

	RwReal				m_fElapsed;			///< 현재 GUI에서 소모되고 있는 time

	RwReal				m_fShakeTime;		///< Illust Shake에 쓰이는 타임 변수
	CRectangle			m_rectBackIllust;	///< 일러스트의 흔들림 중심 좌표

	CNtlPLAccelController	m_conAccel;		///< AccelController
	CNtlPLAccelController	m_conAlpha;		///< Alpha AccelController

	RwBool				m_bPlay;			///< 현재 플레이 되고 있는가?

	SUIConfigBroadCast*	m_pBroadCastConfig;	///< Broad Cast 연출 관련 옵션들
};

#endif//__BROAD_CAST_GUI_H__
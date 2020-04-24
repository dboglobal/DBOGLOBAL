/*****************************************************************************
* File			: TBNewsGui.h
* Author		: Haesung, Cho
* Copyright		: (주)NTL
* Date			: 2008. 10. 21
* Abstract		: DBO 천하제일 무도회 소식지 리뉴얼
*****************************************************************************
* Desc         : 
*****************************************************************************/

#ifndef __TBNEWSGUI_H__
#define __TBNEWSGUI_H__

#pragma once

// 천하제일 무도회 GUI 스크립트 파일
#define dTBNEWS_SCRIPT_FILE_FOLDER		L"script\\tenkaichibudokai\\"
#define dTBNEWS_GUI_FRAMEWORK_FILE		"gui\\TBNewsGui.frm"
#define dTBNEWS_GUI_SURFACE_FILE		"gui\\TBNewsGui.srf"

// 천하제일 무도회의 스크롤 되는 스태틱 박스의 갯수
#define dTBNEWS_INFO_BOX_NUMS						2

// 천하제일 무도회 스크롤 박스의 최소 크기
#define dTBNEWS_INFO_BOX_MIN_WIDTH					245

// 천하제일 무도회 스크롤 박스의 간격
#define dTBNEWS_INFO_BOX_MARGIN						10

/**
* \ingroup client
* \brief 천하제일 무도회 소식지 GUI
*/
class CTBNewsGui : public CNtlPLGui, public RWS::CEventHandler
{
	enum eNewsType
	{
		TYPE_INDIVIDIAUL = 0,			///< 개인전 소식지
		TYPE_TEAM,					///< 팀전 소식지

		TYPE_NUMS,
		INVALID_TYPE = 0xFF
	};

	enum eStaticInfo
	{
		INFO_STATE = 0,					///< 개인의 참가 정보
		INFO_INDITIME,				///< 개인전 시간 정보
		INFO_TEAMTIME,				///< 파티전 시간 정보
		
		INFO_NUMS,
		INVALID_INFO = 0xFF
	};

	enum eTextLine
	{
		TL_1 = 0,
		TL_2,
		TL_3,
		TL_4,

		TL_NUMS
	};

	enum eNoticeType
	{
		NIT_STATE_GUIDE,
		NIT_STATE_END,
		
		NIT_NUMS,
		INVALID_NIT = 0xFF
	};

public:
	CTBNewsGui(const RwChar* pName);
	virtual ~CTBNewsGui();

	RwBool	Create();
	void	Destroy();

	void	Update(RwReal fElapsed);

	RwInt32	SwitchDialog(bool bOpen);
	void	HandleEvents(RWS::CMsg& msg);

public:
	void	UpdateInterface();
	void	SetNews( RwUInt8 byIndiMatchState, RwUInt8 byTeamMatchState );
	void	SetMatchNotice( RwUInt8 byIndiMatchState, RwUInt8 byTeamMatchState );
	void	SetPrizeText( eNoticeType eNitType, RwUInt8 byMatchType, gui::CStaticBox* pComponent, const WCHAR* awcWinner, const WCHAR* awcSecondWinner );
	void	UpdateNoticeImage( eNoticeType eNitType );
	void	UpdateButtons();

	void	OnClickedBtnClose( gui::CComponent* pComponent );
	void	OnClickedBtnSoloRequest( gui::CComponent* pComponent );
	void	OnClickedBtnPartyRequest( gui::CComponent* pComponent );
	void	OnClickedBtnLicense( gui::CComponent* pComponent );
	void	OnClickedBtnTournenment( gui::CComponent* pComponent );

	// Info
public:
	void	InitInfoBox();
	WCHAR*	GetNextStepString( RwUInt8 byMatchType );			///< 다음 상태에 대한 메시지 출력
	void	SetInfoBox( RwUInt8 byInfoBoxNum, gui::CStaticBox* pStbInfoBox );
	void	UpdateInstanceInfo();


protected:
	gui::CStaticBox*	m_pStbTitle;			///< 소식지의 이름
	gui::CStaticBox*	m_pStbHeadLine;			///< 소식지의 HeadLine
	gui::CStaticBox*	m_pStbNewsMark;			///< 소식지의 Mark

	gui::CPanel*		m_pPnlImage;			///< 소식지에 삽일된 이미지
	gui::CHtmlBox*		m_pHtmlNews;			///< 소식지가 출력될 Html 박스

	gui::CStaticBox*	m_pStbNoticeText[TL_NUMS];	///< 소식지의 출력될 텍스트들
		
	gui::CButton*	m_pBtnClose;			///< 닫기 버튼
	gui::CButton*	m_pBtnSoloRequest;		///< 개인전 신청하기 의 버튼
	gui::CButton*	m_pBtnPartyRequest;		///< 파티전 신청하기 의 버튼
	gui::CButton*	m_pBtnLicense;			///< 접수증의 버튼
	gui::CButton*	m_pBtnTournement;		///< 대진표의 버튼

	// Scroll infotext
	gui::CDialog*		m_pDlgInfo;			///< Clipping 되는 dialog
	gui::CStaticBox*	m_paStbInfoBox[dTBNEWS_INFO_BOX_NUMS];
	RwUInt8				m_aBoxPresentInfo[dTBNEWS_INFO_BOX_NUMS];
	RwReal				m_fBoxPresentPosX[dTBNEWS_INFO_BOX_NUMS];

	RwUInt8				m_byCurFirstBox;	///< 현재 앞서고 있는 박스의 번호

	// Slot
	gui::CSlot			m_slotClickedBtnClose;
	gui::CSlot			m_slotClickedBtnSoloRequest;
	gui::CSlot			m_slotClickedBtnPartyRequest;
	gui::CSlot			m_slotClickedBtnLicense;
	gui::CSlot			m_slotClickedBtnTournement;

	RwBool				m_bMudosaUpdate;
	RwUInt32			m_uiMudosaReaminTime;
};

#endif//__TBNEWS_GUI_H__
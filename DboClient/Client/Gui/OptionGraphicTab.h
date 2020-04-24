/******************************************************************************
* File			: OptionGraphicTab.h
* Author		: Hae sung, Cho
* Copyright		: (주)NTL
* Date			: 2009. 3. 24
* Abstract		: 
*****************************************************************************
* Desc			: Tab of graphics
*****************************************************************************/

#ifndef _OPTION_GRAPHIC_TAB_H_
#define _OPTION_GRAPHIC_TAB_H_

#pragma once

//// Video Mode 리스트
typedef struct _SVideoModeInfo
{
	RwInt32 iWidth;
	RwInt32 iHeight;
	RwInt32 iColorDepth;
}SVideoModeInfo;

typedef std::list<SVideoModeInfo> ListVideoMode;

/**
* \ingroup client
*/
class COptionGraphic : public COptionBase
{
public:
	enum eQuality
	{
		eQUALITY_LOW = 0,
		eQUALITY_MIDDLE,
		eQUALITY_HIGH,
		eQUALITY_USER,

		eQUALITY_NUMS,
		eQUALITY_INVALID = 0XFF
	};

public:
	COptionGraphic();
	virtual ~COptionGraphic();

	virtual RwBool		Create(COptionWindowGui* pOptionWindow);
	virtual void		Destroy();

	virtual void		Show();
	virtual void		Hide();

	virtual void		OnInit();
	virtual void		OnReset();
	virtual void		OnOk();
	virtual void		OnCancel();
	virtual void		OnHandleEvents(RWS::CMsg &pMsg);

	void				OnScrollBarChanged(RwInt32 nParam);
	void				OnAbsoluteMouseWheel( RwInt32 nFlag, RwInt16 sDelta, CPos& pos );
	void				OnListToggled(BOOL bToggled, gui::CComponent* pComponent);

	// Resolution & Gamma
public:
	void				SetComponentResolution(RwBool bDefault = FALSE);

	RwInt32				GetVideoMode(RwBool bDefault = FALSE);
	void				SetVideoMode(RwInt32 iIndex);
	std::wstring		GetVideoModeText();
	void				SetVideoModeText(const std::wstring& strText);

	RwInt32				GetGammaValue();
	void				SetGammaValue(RwInt32 iValue);
	void				OnGammaChanged(RwInt32 iParam);

	void				SetFpsValue(RwInt32 iValue);
	void				OnFpsSliderMoved(RwInt32 iValue);
	void				SetFpsToolTip(RwInt32 iValue);

	void				SelectVideoMode(RwInt32 iWidth, RwInt32 iHeight, RwInt32 iColorDepth); 
	void				SelectVideoModeWin(RwInt32 iWidth, RwInt32 iHeight);
	void				GetSelectVideoMode(SVideoModeInfo& sVideoModeInfo);
	ListVideoMode&		GetVideoModeList(RwBool bFullMode);

	// Preset
public:
	void				SetPreset( RwInt32 nLevel );
	
	void				OnOtherDataChanged( RwInt32 nParam );
	void				OnOtherDataToggled( gui::CComponent* pComponent, bool bToggle );

	// Handler
public:
	int					RestoreHandler();

protected:
	int					m_nCurrentQuality;	
	DEVICE_RESTORE_HANDLE m_hRestore;

protected:
	gui::CDialog*		m_pThis;			///< 현재 Dialog
	gui::CDialog*		m_pScrollDialog;	///< 스크롤 되는 Dialog

	gui::CSlot			m_slotAbsoluteMouseWheel;

	gui::CStaticBox*	m_pStbResolutionTitle;	///< 해상도 설정
	gui::CStaticBox*	m_pStbResolution;		///< 해상도
	gui::CComboBox*		m_pCbbResolution;		///< 해상도 설정 콤보박스
	gui::CSlot			m_slotListToggled;	

	gui::CStaticBox*	m_pStbGamma;			///< 밝기
	gui::CScrollBar*	m_pScbGamma;			///< 밝기 설정 스크롤바
	gui::CSlot			m_slotGammaSliderMoved;
	gui::CSlot			m_slotGammaValueChanged;

	gui::CStaticBox*	m_pStbFps;
	gui::CScrollBar*	m_pScbFps;
	gui::CSlot			m_slotFpsSliderMoved;
	gui::CSlot			m_slotFpsValueChanged;

	gui::CButton*		m_pBtnWindowMode;		///< 윈도우 모드 설정 토글바

	gui::CStaticBox*	m_pStbEffectTitle;		///< 효과 타이틀
	gui::CStaticBox*	m_pStbGraphicQuality;	///< 전체 그래픽 품질
	gui::CStaticBox*	m_pStbGraphicQualityLow;	///< 낮음
	gui::CStaticBox*	m_pStbGraphicQualityMiddle;	///< 보통
	gui::CStaticBox*	m_pStbGraphicQualityHigh;	///< 높음
	gui::CStaticBox*	m_pStbGraphicQualityUser;	///< 사용자 설정
	gui::CScrollBar*	m_pScbGraphicQuality;		///< 그래픽 품질 설정 스크롤바
	gui::CSlot			m_slotQualitySliderMoved;
	gui::CSlot			m_slotQualityValueChanged;

	gui::CStaticBox*	m_pStbTerrainDist;		///< 지형 시야거리
	gui::CScrollBar*	m_pScbTerrainDist;		///< 지형 시야거리 설정 스크롤 바
	gui::CSlot			m_slotTerrainSliderMoved;
	gui::CSlot			m_slotTerrainValueChanged;
	
	gui::CStaticBox*	m_pStbObjectDist;		///< 사물 시야거리
	gui::CScrollBar*	m_pScbObjectDist;		///< 사물 시야거리 설정 스크롤 바
	gui::CSlot			m_slotObjectSliderMoved;
	gui::CSlot			m_slotObjectValueChanged;

	gui::CButton*		m_pBtnTerrainShadow;	///< 지형 그림자
	gui::CSlot			m_slotTerrainShadowToggled;

	gui::CButton*		m_pBtnWaterEffect;		///< 물 효과
	gui::CSlot			m_slotWaterEffectToggled;

	/*gui::CStaticBox*	m_pStbCharacterDist;	///< 캐릭터 시야거리
	gui::CScrollBar*	m_pScbCharacterDist;	///< 캐릭터 시야거리 설정 스크롤 바
	gui::CSlot			m_slotCharacterDistSliderMoved;
	gui::CSlot			m_slotCharacterDistValueChanged;*/

	gui::CButton*		m_pBtnCharacterShadow;	///< 캐릭터 그림자
	gui::CSlot			m_slotCharacterShadowToggled;

	gui::CStaticBox*	m_pStbMagicEffect;		///< 마법 세부효과
	gui::CScrollBar*	m_pScbMagicEffect;		///< 마법 세부효과 설정 스크롤 바
	gui::CSlot			m_slotMagicEffectSliderMoved;
	gui::CSlot			m_slotMagicEffectValueChanged;

	gui::CStaticBox*	m_pStbWeatherEffect;	///< 날씨 효과
	gui::CScrollBar*	m_pScbWeatherEffect;	///< 날씨 효과 설정 스크롤 바
	gui::CSlot			m_slotWeatherEffectSliderMoved;
	gui::CSlot			m_slotWeatherEffectValueChanged;

	gui::CStaticBox*	m_pStbTextureQuality;	///< 텍스쳐 해상도
	gui::CScrollBar*	m_pScbTextureQuality;	///< 텍스쳐 해상도 설정 스크롤 바
	gui::CSlot			m_slotTextureQualitySliderMoved;
	gui::CSlot			m_slotTextureQualityValueChanged;

	gui::CStaticBox*	m_pStbCharacterEffectTitle;	///< 캐릭터 효과 타이틀
	gui::CButton*		m_pBtnCartoonEdge;			///< 카툰식 외곽선
	/*gui::CButton*		m_pBtnUpgradeEffect;		///< 업그레이드 효과*/
	gui::CSlot			m_slotCartoonEdgeToggled;
	/*gui::CSlot			m_slotUpgradeEffectToggled;*/

	gui::CStaticBox*	m_pStbEtcTitle;				
	gui::CButton*		m_pBtnHdrEffect;			///< HDR 효과
	gui::CSlot			m_slotHdrEffectToggled;
	
	gui::CScrollBar*	m_pThisScrollBar;			///< 스크롤 바
	gui::CSlot			m_slotScrollBarChanged;
	gui::CSlot			m_slotScrollBarSliderMoved;

	RwInt32				m_iGamma;
	ListVideoMode		m_listFullVideo;
	ListVideoMode		m_listWinVideo;
	SVideoModeInfo		m_sSelectMode;
};


#endif
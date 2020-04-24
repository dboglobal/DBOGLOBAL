#include "precomp_dboclient.h"
// core
#include "NtlDebug.h"
#include "CEventHandler.h"
#include "NtlSysEvent.h"
#include "NtlDeviceRestoreHandler.h"

// presentation
#include "NtlPLGui.h"
#include "NtlPLGuiManager.h"

// FrameWork
#include "NtlApplication.h"

// Simulation
#include "NtlStorageMTContainer.h"
#include "NtlStorageManager.h"

// dbo
#include "DialogManager.h"
#include "DisplayStringManager.h"

// Option
#include "OptionWindowGui.h"
#include "OptionBase.h"
#include "OptionGraphicTab.h"

COptionGraphic::COptionGraphic()
{

}

COptionGraphic::~COptionGraphic()
{

}

RwBool COptionGraphic::Create( COptionWindowGui* pOptionWindow )
{
	NTL_FUNCTION( "COptionGraphic::Create");
	COptionBase::Create(pOptionWindow);

	// Component
	m_pThis = (gui::CDialog*)GetComponent("dlgGraphic");
	m_pScrollDialog = (gui::CDialog*)GetComponent("dlgGraphicList");
	
	m_pThisScrollBar = (gui::CScrollBar*)GetComponent("scbGraphicScroll");
	CRectangle rtThis = m_pThis->GetScreenRect();
	CRectangle rtScroll = m_pScrollDialog->GetScreenRect();
	int nScrollRange = rtScroll.bottom - rtThis.bottom;
	m_pThisScrollBar->SetRange(0, nScrollRange );

	m_pStbResolutionTitle = (gui::CStaticBox*)GetComponent("stbResoulutionTitle");	///< 해상도 설정
	m_pStbResolutionTitle->SetText( GetDisplayStringManager()->GetString( "DST_OPTION_GRAPHIC_RESOLUTION_TITLE" ) );
	m_pStbResolution = (gui::CStaticBox*)GetComponent("stbResoulution");		///< 해상도
	m_pStbResolution->SetText( GetDisplayStringManager()->GetString( "DST_OPTION_GRAPHIC_RESOLUTION"  ) );
	m_pStbResolution->SetToolTip( GetDisplayStringManager()->GetString( "DST_OPTION_TOOLTIP_RESOLUTION" ) );
	m_pCbbResolution = (gui::CComboBox*)GetComponent("cbbGraphicResolution");		///< 해상도 설정 콤보박스
	m_pCbbResolution->GetButton()->SetClippingMode( TRUE );
	m_pCbbResolution->GetStaticBox()->SetClippingMode( TRUE );
	m_pCbbResolution->GetListBox()->SetClippingMode( TRUE );
	m_slotListToggled = m_pCbbResolution->SigListToggled().Connect( this, &COptionGraphic::OnListToggled );

	m_pStbGamma = (gui::CStaticBox*)GetComponent("stbGammaTitle");			///< 밝기
	m_pStbGamma->SetText( GetDisplayStringManager()->GetString( "DST_OPTION_GRAPHIC_GAMMA" ) );
	m_pStbGamma->SetToolTip( GetDisplayStringManager()->GetString( "DST_OPTION_TOOLTIP_GAMMA" ) );
	m_pScbGamma = (gui::CScrollBar*)GetComponent("scbGraphicGamma");		///< 밝기 설정 스크롤바
	m_pScbGamma->GetIncreaseButton()->SetClippingMode( TRUE );
	m_pScbGamma->GetDecreaseButton()->SetClippingMode( TRUE );
	m_pScbGamma->SetRange( 0, 100 );

	m_slotGammaSliderMoved = m_pScbGamma->SigSliderMoved().Connect( this, &COptionGraphic::OnGammaChanged );
	m_slotGammaValueChanged = m_pScbGamma->SigValueChanged().Connect( this, &COptionGraphic::OnGammaChanged );


	m_pStbFps = (gui::CStaticBox*)GetComponent("stbFpsTitle");
	m_pStbFps->SetText(L"FPS Limit");
	m_pStbFps->SetToolTip(L"FPS Limit");
	m_pScbFps = (gui::CScrollBar*)GetComponent("scbFps");
	m_pScbFps->GetIncreaseButton()->SetClippingMode(TRUE);
	m_pScbFps->GetDecreaseButton()->SetClippingMode(TRUE);
	m_pScbFps->SetRange(30, 240);
	m_pScbFps->SetValue(GetNtlStorageManager()->GetIntData(dSTORAGE_GRAPHIC_FPS)); // set default value
	SetFpsToolTip(m_pScbFps->GetValue());

	m_slotFpsSliderMoved = m_pScbFps->SigSliderMoved().Connect(this, &COptionGraphic::OnFpsSliderMoved);
	m_slotFpsValueChanged = m_pScbFps->SigValueChanged().Connect(this, &COptionGraphic::OnFpsSliderMoved);

	m_pBtnWindowMode = (gui::CButton*)GetComponent("btnWindowMode");		///< 윈도우 모드 토글
	m_pBtnWindowMode->SetText( GetDisplayStringManager()->GetString( "DST_OPTION_GRAPHIC_WINDOWMODE" ) );
	m_pBtnWindowMode->SetToolTip( GetDisplayStringManager()->GetString( "DST_OPTION_TOOLTIP_WINDOWMODE" ) );
	m_pBtnWindowMode->SetTextStyle( COMP_TEXT_LEFT );
	m_pBtnWindowMode->SetTextCoord( 17, 0 );

	m_pStbEffectTitle = (gui::CStaticBox*)GetComponent("stbEffectTitle");		///< 효과 타이틀
	m_pStbEffectTitle->SetText( GetDisplayStringManager()->GetString( "DST_OPTION_GRAPHIC_TITLE" ) );
	m_pStbGraphicQuality = (gui::CStaticBox*)GetComponent("stbGrahpicQualityText");	///< 전체 그래픽 품질
	m_pStbGraphicQuality->SetText( GetDisplayStringManager()->GetString( "DST_OPTION_GRAPHIC_QUALITY_TOTAL" ) );
	m_pStbGraphicQuality->SetToolTip( GetDisplayStringManager()->GetString( "DST_OPTION_TOOLTIP_GRAPHIC_QUALITY" ) );
	m_pStbGraphicQualityLow = (gui::CStaticBox*)GetComponent("stbGrahpicQualityLow");	///< 낮음
	m_pStbGraphicQualityLow->SetText( GetDisplayStringManager()->GetString( "DST_OPTION_GRAPHIC_QUALITY_LOW" ) );
	m_pStbGraphicQualityMiddle = (gui::CStaticBox*)GetComponent("stbGrahpicQualityMiddle");	///< 보통
	m_pStbGraphicQualityMiddle->SetText( GetDisplayStringManager()->GetString( "DST_OPTION_GRAPHIC_QUALITY_MIDDLE" ) );
	m_pStbGraphicQualityHigh = (gui::CStaticBox*)GetComponent("stbGrahpicQualityHigh");	///< 높음
	m_pStbGraphicQualityHigh->SetText( GetDisplayStringManager()->GetString( "DST_OPTION_GRAPHIC_QUALITY_HIGH" ) );
	m_pStbGraphicQualityUser = (gui::CStaticBox*)GetComponent("stbGrahpicQualityUser");	///< 사용자 설정
	m_pStbGraphicQualityUser->SetText( GetDisplayStringManager()->GetString( "DST_OPTION_GRAPHIC_QUALITY_USER" ) );
	m_pScbGraphicQuality = (gui::CScrollBar*)GetComponent("scbGraphicQuality");		///< 그래픽 품질 설정 스크롤바
	m_pScbGraphicQuality->GetIncreaseButton()->SetClippingMode( TRUE );
	m_pScbGraphicQuality->GetDecreaseButton()->SetClippingMode( TRUE );
	m_pScbGraphicQuality->SetRange( 0, 3 );

	m_slotQualitySliderMoved = m_pScbGraphicQuality->SigSliderMoved().Connect( this, &COptionGraphic::SetPreset );
	m_slotQualityValueChanged = m_pScbGraphicQuality->SigValueChanged().Connect( this, &COptionGraphic::SetPreset );

	m_pStbTerrainDist = (gui::CStaticBox*)GetComponent("stbTerrainDist");		///< 지형 시야거리
	m_pStbTerrainDist->SetText( GetDisplayStringManager()->GetString( "DST_OPTION_GRAPHIC_QUALITY_TERRAIN_RANGE" ) );
	m_pStbTerrainDist->SetToolTip( GetDisplayStringManager()->GetString( "DST_OPTION_TOOLTIP_TERRAIN_DISTANCE" ) );
	m_pScbTerrainDist = (gui::CScrollBar*)GetComponent("scbTerrainDist");		///< 지형 시야거리 설정 스크롤 바
	m_pScbTerrainDist->GetIncreaseButton()->SetClippingMode( TRUE );
	m_pScbTerrainDist->GetDecreaseButton()->SetClippingMode( TRUE );
	m_pScbTerrainDist->SetRange( 0, 3 );
	m_slotTerrainSliderMoved = m_pScbTerrainDist->SigSliderMoved().Connect( this, &COptionGraphic::OnOtherDataChanged );
	m_slotTerrainValueChanged = m_pScbTerrainDist->SigValueChanged().Connect( this, &COptionGraphic::OnOtherDataChanged );

	m_pStbObjectDist = (gui::CStaticBox*)GetComponent("stbObjectDist");		///< 사물 시야거리
	m_pStbObjectDist->SetText( GetDisplayStringManager()->GetString( "DST_OPTION_GRAPHIC_QUALITY_OBJECT_RANGE" ) );
	m_pStbObjectDist->SetToolTip( GetDisplayStringManager()->GetString( "DST_OPTION_TOOLTIP_OBJECT_DISTANCE" ) );
	m_pScbObjectDist = (gui::CScrollBar*)GetComponent("scbObjectDist");		///< 사물 시야거리 설정 스크롤 바
	m_pScbObjectDist->GetIncreaseButton()->SetClippingMode( TRUE );
	m_pScbObjectDist->GetDecreaseButton()->SetClippingMode( TRUE );
	m_pScbObjectDist->SetRange( 0, 3 );
	m_slotObjectSliderMoved = m_pScbObjectDist->SigSliderMoved().Connect( this, &COptionGraphic::OnOtherDataChanged );
	m_slotObjectValueChanged = m_pScbObjectDist->SigValueChanged().Connect( this, &COptionGraphic::OnOtherDataChanged );

	m_pBtnTerrainShadow = (gui::CButton*)GetComponent("btnTerrainShadow");	///< 지형 그림자
	m_pBtnTerrainShadow->SetText( GetDisplayStringManager()->GetString( "DST_OPTION_GRAPHIC_TERRAIN_SHADOW" ) );
	m_pBtnTerrainShadow->SetToolTip( GetDisplayStringManager()->GetString( "DST_OPTION_TOOLTIP_TERRAIN_SHADOW" ) );
	m_pBtnTerrainShadow->SetTextStyle( COMP_TEXT_LEFT );
	m_pBtnTerrainShadow->SetTextCoord( 17, 0 );
	m_slotTerrainShadowToggled = m_pBtnTerrainShadow->SigToggled().Connect( this, &COptionGraphic::OnOtherDataToggled );

	m_pBtnWaterEffect = (gui::CButton*)GetComponent("btnWaterEffect");		///< 물 효과
	m_pBtnWaterEffect->SetText( GetDisplayStringManager()->GetString( "DST_OPTION_GRAPHIC_WATER_EFFECT" ) );
	m_pBtnWaterEffect->SetToolTip( GetDisplayStringManager()->GetString( "DST_OPTION_TOOLTIP_WATER_EFFECT" ) );
	m_pBtnWaterEffect->SetTextStyle( COMP_TEXT_LEFT );
	m_pBtnWaterEffect->SetTextCoord( 17, 0 );
	m_slotWaterEffectToggled = m_pBtnWaterEffect->SigToggled().Connect( this, &COptionGraphic::OnOtherDataToggled );

	//m_pStbCharacterDist = (gui::CStaticBox*)GetComponent("stbCharacterDist");	///< 캐릭터 시야거리
	//
	//// Disable 처리를 위하여 회색으로 한다.
	//m_pStbCharacterDist->SetTextColor( RGB( 128, 128, 128 ) );
	//m_pStbCharacterDist->SetText( GetDisplayStringManager()->GetString( DST_OPTION_GRAPHIC_CHARACTER_RANGE ) );
	//m_pStbCharacterDist->SetToolTip( GetDisplayStringManager()->GetString( DST_OPTION_TOOLTIP_CHARACTER_DISTANCE ) );
	//m_pScbCharacterDist = (gui::CScrollBar*)GetComponent("scbCharacterDist");	///< 캐릭터 시야거리 설정 스크롤 바
	//m_pScbCharacterDist->GetIncreaseButton()->SetClippingMode( TRUE );
	//m_pScbCharacterDist->GetDecreaseButton()->SetClippingMode( TRUE );
	//m_pScbCharacterDist->SetRange( 0, 3 );
	//
	//// 캐릭터 시야거리는 Disable 시켜놓는다.
	//m_pScbCharacterDist->Enable( false );
	//m_pScbCharacterDist->GetDecreaseButton()->ClickEnable( FALSE );
	//m_pScbCharacterDist->GetIncreaseButton()->ClickEnable( FALSE );

	//m_slotCharacterDistSliderMoved = m_pScbCharacterDist->SigSliderMoved().Connect( this, &COptionGraphic::OnOtherDataChanged );
	//m_slotCharacterDistValueChanged = m_pScbCharacterDist->SigValueChanged().Connect( this, &COptionGraphic::OnOtherDataChanged );

	m_pBtnCharacterShadow = (gui::CButton*)GetComponent("btnCharacterShadow");
	m_pBtnCharacterShadow->SetText( GetDisplayStringManager()->GetString( "DST_OPTION_GRAPHIC_CHARACTER_SHADOW" ) );
	m_pBtnCharacterShadow->SetToolTip( GetDisplayStringManager()->GetString( "DST_OPTION_TOOLTIP_SHADOW" ) );
	m_pBtnCharacterShadow->SetTextStyle( COMP_TEXT_LEFT );
	m_pBtnCharacterShadow->SetTextCoord( 17, 0 );
	m_slotCharacterShadowToggled = m_pBtnCharacterShadow->SigToggled().Connect( this, &COptionGraphic::OnOtherDataToggled );

	m_pStbMagicEffect = (gui::CStaticBox*)GetComponent("stbMagicEffect");		///< 마법 세부효과
	m_pStbMagicEffect->SetText( GetDisplayStringManager()->GetString( "DST_OPTION_GRAPHIC_MAGIC_EFFECT" ) );
	m_pStbMagicEffect->SetToolTip( GetDisplayStringManager()->GetString( "DST_OPTION_TOOLTIP_MAGIC_DETAIL" ) );
	m_pScbMagicEffect = (gui::CScrollBar*)GetComponent("scbMagicEffect");		///< 마법 세부효과 설정 스크롤 바
	m_pScbMagicEffect->GetIncreaseButton()->SetClippingMode( TRUE );
	m_pScbMagicEffect->GetDecreaseButton()->SetClippingMode( TRUE );
	m_pScbMagicEffect->SetRange( 0, 3 );
	m_slotMagicEffectSliderMoved = m_pScbMagicEffect->SigSliderMoved().Connect( this, &COptionGraphic::OnOtherDataChanged );
	m_slotMagicEffectValueChanged = m_pScbMagicEffect->SigValueChanged().Connect( this, &COptionGraphic::OnOtherDataChanged );

	m_pStbWeatherEffect = (gui::CStaticBox*)GetComponent("stbWeatherEffect");	///< 날씨 효과
	m_pStbWeatherEffect->SetText( GetDisplayStringManager()->GetString( "DST_OPTION_GRAPHIC_WEATHER_EFFECT" ) );
	m_pStbWeatherEffect->SetToolTip( GetDisplayStringManager()->GetString( "DST_OPTION_TOOLTIP_WEATHER_EFFECT" ) );
	m_pScbWeatherEffect = (gui::CScrollBar*)GetComponent("scbWeatherEffect");	///< 날씨 효과 설정 스크롤 바
	m_pScbWeatherEffect->GetIncreaseButton()->SetClippingMode( TRUE );
	m_pScbWeatherEffect->GetDecreaseButton()->SetClippingMode( TRUE );
	m_pScbWeatherEffect->SetRange( 0, 1 );
	m_slotWeatherEffectSliderMoved = m_pScbWeatherEffect->SigSliderMoved().Connect( this, &COptionGraphic::OnOtherDataChanged );
	m_slotWeatherEffectValueChanged = m_pScbWeatherEffect->SigValueChanged().Connect( this, &COptionGraphic::OnOtherDataChanged );

	m_pStbTextureQuality = (gui::CStaticBox*)GetComponent("stbTextureQuality");	///< 텍스쳐 해상도
	/*m_pStbTextureQuality->SetTextColor( RGB( 128, 128, 128 ) );*/
	m_pStbTextureQuality->SetText( GetDisplayStringManager()->GetString( "DST_OPTION_GRAPHIC_TEXTURE_QUALITY" ) );
	m_pStbTextureQuality->SetToolTip( GetDisplayStringManager()->GetString( "DST_OPTION_TOOLTIP_TEXTURE_LEVEL" ) );
	m_pScbTextureQuality = (gui::CScrollBar*)GetComponent("scbTextureQuality");	///< 텍스쳐 해상도 설정 스크롤 바
	m_pScbTextureQuality->SetRange( 0, 2 );

	//m_pScbTextureQuality->Enable( false );
	m_pScbTextureQuality->GetIncreaseButton()->SetClippingMode( TRUE );
	m_pScbTextureQuality->GetDecreaseButton()->SetClippingMode( TRUE );
	m_slotTextureQualitySliderMoved = m_pScbTextureQuality->SigSliderMoved().Connect( this, &COptionGraphic::OnOtherDataChanged );
	m_slotTextureQualityValueChanged = m_pScbTextureQuality->SigValueChanged().Connect( this, &COptionGraphic::OnOtherDataChanged );

	m_pStbCharacterEffectTitle = (gui::CStaticBox*)GetComponent("stbCharacterEffectTitle");	///< 캐릭터 효과 타이틀
	m_pStbCharacterEffectTitle->SetText( GetDisplayStringManager()->GetString( "DST_OPTION_GRAPHIC_CHARACTER_EFFECT" ) );
	m_pBtnCartoonEdge = (gui::CButton*)GetComponent("btnCartoonEdge");			///< 카툰식 외곽선
	m_pBtnCartoonEdge->SetText( GetDisplayStringManager()->GetString( "DST_OPTION_GRAPHIC_CARTOON_EDGE" ) );
	m_pBtnCartoonEdge->SetToolTip( GetDisplayStringManager()->GetString( "DST_OPTION_TOOLTIP_CARTOON_EDGE" ) );
	m_pBtnCartoonEdge->SetTextStyle( COMP_TEXT_LEFT );
	m_pBtnCartoonEdge->SetTextCoord( 17, 0 );

	//m_pBtnUpgradeEffect = (gui::CButton*)GetComponent("btnUpgradeEffect");		///< 업그레이드 효과
	//m_pBtnUpgradeEffect->SetText( GetDisplayStringManager()->GetString( DST_OPTION_GRAPHIC_UPGRADE_EFFECT ) );
	//m_pBtnUpgradeEffect->SetToolTip( GetDisplayStringManager()->GetString( DST_OPTION_TOOLTIP_UPGRADE_EFFECT ) );
	//m_pBtnUpgradeEffect->SetTextStyle( COMP_TEXT_LEFT );
	//m_pBtnUpgradeEffect->SetTextCoord( 17, 0 );
	//m_pBtnUpgradeEffect->ClickEnable( FALSE );
	
	m_slotCartoonEdgeToggled = m_pBtnCartoonEdge->SigToggled().Connect( this, &COptionGraphic::OnOtherDataToggled );
	/*m_slotUpgradeEffectToggled = m_pBtnUpgradeEffect->SigToggled().Connect( this, &COptionGraphic::OnOtherDataToggled );*/
	
	m_pStbEtcTitle = (gui::CStaticBox*)GetComponent("stbEtcTitle");
	m_pStbEtcTitle->SetText( GetDisplayStringManager()->GetString( "DST_OPTION_GRAPHIC_ETC_TITLE" ) );
	m_pBtnHdrEffect = (gui::CButton*)GetComponent("btnHdrEffect");			///< HDR 효과
	m_pBtnHdrEffect->SetText( GetDisplayStringManager()->GetString( "DST_OPTION_GRAPHIC_HDR_EFFECT" ) );
	m_pBtnHdrEffect->SetToolTip( GetDisplayStringManager()->GetString( "DST_OPTION_GRAPHIC_HDR_EFFECT" ) );
	m_pBtnHdrEffect->SetTextStyle( COMP_TEXT_LEFT );
	m_pBtnHdrEffect->SetTextCoord( 17, 0 );
	m_slotHdrEffectToggled = m_pBtnHdrEffect->SigToggled().Connect( this, &COptionGraphic::OnOtherDataToggled );

	m_slotScrollBarChanged			= m_pThisScrollBar->SigValueChanged().Connect(this, &COptionGraphic::OnScrollBarChanged);
	m_slotScrollBarSliderMoved		= m_pThisScrollBar->SigSliderMoved().Connect(this, &COptionGraphic::OnScrollBarChanged);
	m_slotAbsoluteMouseWheel		= GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect( this, &COptionGraphic::OnAbsoluteMouseWheel );
	
	// Link Device Restore
	m_hRestore	= CNtlDeviceRestoreHandler::GetInstance()->LinkDeviceRestore(this, &COptionGraphic::RestoreHandler);

	NTL_RETURN(TRUE);
}

void COptionGraphic::Destroy()
{
	NTL_FUNCTION("COptionGraphic::Destroy");

	CNtlDeviceRestoreHandler::GetInstance()->UnLinkDeviceRestore( m_hRestore );

	NTL_RETURNVOID();
}

void COptionGraphic::Show()
{
	// COptionBase의 ScrollBar를 사용하지 않는다.
	m_pScrollBar->Show(false);

	m_pThis->Show( true );

	COptionBase::Show();
}

void COptionGraphic::Hide()
{
	// 복구
	m_pScrollBar->Show(true);

	m_pThis->Show( false );

	COptionBase::Hide();
}

void COptionGraphic::OnInit()
{
	// Open the NtlStorage
	SetComponentResolution();

	m_iGamma = GetGammaValue();
	m_pScbGamma->SetValue( m_iGamma );

	// FullScreen이 아닐 경우 사용하지 못하게 한다.
	if( !CNtlApplication::GetInstance()->IsFullScreen() )
	{
		m_pStbGamma->SetTextColor( RGB(128, 128, 128), TRUE );
		m_pScbGamma->Enable( false );
		m_pBtnWindowMode->SetDown( true );
	}
	else
	{
		m_pStbGamma->SetTextColor( RGB(255, 255, 255), TRUE );
		m_pScbGamma->Enable( true );
		m_pBtnWindowMode->SetDown( false );
	}
	
	int nGraphicLevel = GetNtlStorageManager()->GetIntData( dSTORAGE_GRAPHIC_PRESET );
	SetPreset( nGraphicLevel );

	if( nGraphicLevel == eQUALITY_USER )
	{
		m_pScbTerrainDist->SetValue( GetNtlStorageManager()->GetIntData( dSTORAGE_GRAPHIC_TERRAIN_RANGE ), FALSE );
		m_pScbObjectDist->SetValue( GetNtlStorageManager()->GetIntData( dSTORAGE_GRAPHIC_OBJECT_RANGE ), FALSE );
		m_pBtnTerrainShadow->SetDown( GetNtlStorageManager()->GetBoolData( dSTORAGE_GRAPHIC_TERRAIN_SHADOW ) );
		m_pBtnWaterEffect->SetDown( GetNtlStorageManager()->GetBoolData( dSTORAGE_GRAPHIC_WATER_EFFECT ) );
		//m_pScbCharacterDist->SetValue( GetNtlStorageManager()->GetIntData( dSTORAGE_GRAPHIC_CHARACTER_RANGE ), FALSE );
		m_pBtnCharacterShadow->SetDown( GetNtlStorageManager()->GetBoolData( dSTORAGE_GRAPHIC_CHARACTER_SHADOW ) );
		m_pScbMagicEffect->SetValue( GetNtlStorageManager()->GetIntData( dSTORAGE_GRAPHIC_MAGIC_EFFECT ), FALSE );
		m_pBtnHdrEffect->SetDown( GetNtlStorageManager()->GetBoolData( dSTORAGE_GRAPHIC_SHADER_HDR ) );
		m_pBtnCartoonEdge->SetDown( GetNtlStorageManager()->GetBoolData( dSTORAGE_GRAPHIC_CHARACTER_EDGE ) );
		/*m_pBtnUpgradeEffect->SetDown( GetNtlStorageManager()->GetBoolData( dSTORAGE_GRAPHIC_CHARACTER_EFFECT ) );*/
		m_pScbWeatherEffect->SetValue( GetNtlStorageManager()->GetIntData( dSTORAGE_GRAPHIC_WEATHER_EFFECT ), FALSE );
		m_pScbTextureQuality->SetValue( GetNtlStorageManager()->GetIntData( dSTORAGE_GRAPHIC_TEXTURE_LEVEL ), FALSE );
	}

	/*m_pScbFps->SetValue(GetNtlStorageMTContainer()->GetDefaultInt(dSTORAGE_GRAPHIC_FPS));
	SetFpsToolTip(m_pScbFps->GetValue());
	CNtlApplication::m_uiFrameRate = m_pScbFps->GetValue();*/

	// 잠시 임시 값으로 세팅한다.
	/*m_pScbTextureQuality->SetValue( 3 );*/
	//m_pScbCharacterDist->SetValue( 3 );
	/*m_pBtnUpgradeEffect->SetDown( true );*/
}

void COptionGraphic::OnReset()
{
	// Default
	SetComponentResolution(TRUE);

	if( CNtlApplication::GetInstance()->IsFullScreen() )
	{
		m_iGamma = (RwInt32)(GetNtlStorageMTContainer()->GetDefaultFloat( dSTORAGE_GRAPHIC_GAMMA ) * 25.0f);
		m_pScbGamma->SetValue(m_iGamma);
	}

	m_pBtnWindowMode->SetDown( GetNtlStorageMTContainer()->GetDefaultBool( dSTORAGE_GRAPHIC_WINDOW_MODE ) );
	
	SetPreset( GetNtlStorageMTContainer()->GetDefaultInt( dSTORAGE_GRAPHIC_PRESET ) );

	m_pScbFps->SetValue(GetNtlStorageManager()->GetIntData(dSTORAGE_GRAPHIC_FPS));
	SetFpsToolTip(m_pScbFps->GetValue());

}

void COptionGraphic::OnOk()
{
	// Apply
	SetGammaValue( m_pScbGamma->GetValue() );
	SetVideoMode( m_pCbbResolution->GetSelectedItemIndex() );
	SetFpsValue(m_pScbFps->GetValue());

	GetNtlStorageManager()->SetData( dSTORAGE_GRAPHIC_WINDOW_MODE, m_pBtnWindowMode->IsDown() );

	// Preset
	GetNtlStorageManager()->SetData( dSTORAGE_GRAPHIC_PRESET, m_pScbGraphicQuality->GetValue() );

	// 하위 옵션 저장
	GetNtlStorageManager()->SetData( dSTORAGE_GRAPHIC_TERRAIN_RANGE, m_pScbTerrainDist->GetValue() );
	GetNtlStorageManager()->SetData( dSTORAGE_GRAPHIC_OBJECT_RANGE, m_pScbObjectDist->GetValue() );
	GetNtlStorageManager()->SetData( dSTORAGE_GRAPHIC_TERRAIN_SHADOW, m_pBtnTerrainShadow->IsDown() );
	GetNtlStorageManager()->SetData( dSTORAGE_GRAPHIC_WATER_EFFECT, m_pBtnWaterEffect->IsDown() );
	//GetNtlStorageManager()->SetData( dSTORAGE_GRAPHIC_CHARACTER_RANGE, m_pScbCharacterDist->GetValue() );
	GetNtlStorageManager()->SetData( dSTORAGE_GRAPHIC_CHARACTER_SHADOW, m_pBtnCharacterShadow->IsDown() );
	GetNtlStorageManager()->SetData( dSTORAGE_GRAPHIC_MAGIC_EFFECT, m_pScbMagicEffect->GetValue() );
	GetNtlStorageManager()->SetData( dSTORAGE_GRAPHIC_SHADER_HDR, m_pBtnHdrEffect->IsDown() );
	GetNtlStorageManager()->SetData( dSTORAGE_GRAPHIC_CHARACTER_EDGE, m_pBtnCartoonEdge->IsDown() );
	/*GetNtlStorageManager()->SetData( dSTORAGE_GRAPHIC_CHARACTER_EFFECT, m_pBtnUpgradeEffect->IsDown() );*/
	GetNtlStorageManager()->SetData( dSTORAGE_GRAPHIC_WEATHER_EFFECT, m_pScbWeatherEffect->GetValue() );
	GetNtlStorageManager()->SetData( dSTORAGE_GRAPHIC_TEXTURE_LEVEL, m_pScbTextureQuality->GetValue() );

	// 적용
	GetNtlStorageManager()->ApplyStorageType( eNTL_STORAGE_GRAPHIC, eNTL_STORAGE_APPLY_ALL );
}

void COptionGraphic::OnCancel()
{
	CNtlApplication::GetInstance()->SetGammaRamp( GetNtlStorageManager()->GetFloatData(dSTORAGE_GRAPHIC_GAMMA) );

	m_pScbFps->SetValue(GetNtlStorageManager()->GetIntData(dSTORAGE_GRAPHIC_FPS));
	SetFpsToolTip(m_pScbFps->GetValue());
}

void COptionGraphic::OnHandleEvents( RWS::CMsg &pMsg )
{
	if( pMsg.Id == g_EventToggleFullScreen )
	{
		bool* pbFullScreen = reinterpret_cast<bool*>( pMsg.pData );

		if( !(*pbFullScreen) )
		{
			/*SetComponentResolution();*/

			m_pStbGamma->SetTextColor( RGB(128, 128, 128), TRUE );
			m_pScbGamma->Enable( false );
		}
		else
		{
			/*SetComponentResolution();*/

			m_pStbGamma->SetTextColor( RGB(255, 255, 255), TRUE );
			m_pScbGamma->Enable( true );
		}
	}
}

void COptionGraphic::OnScrollBarChanged( RwInt32 nParam )
{
	// Offset 은 갯수이기 때문에 Dialog의 크기만큼 Offset을 준다.
	/*CRectangle rtPos = m_pOptionWindow->GetPosition();*/
	m_pScrollDialog->SetPosition( 0 , -nParam );
}

void COptionGraphic::SetComponentResolution( RwBool bDefault /*= FALSE*/ )
{
	RwBool bFullScreen = CNtlApplication::GetInstance()->IsFullScreen();
	ListVideoMode& listVideoMode = GetVideoModeList(bFullScreen);

	wchar_t	buf[256];

	// If the application is toggled to full-screen <-> window mode
// Close the listbox if it is currently popped up.
	gui::CListBox* pListBox = m_pCbbResolution->GetListBox();
	if( pListBox->IsVisible() )
		m_pCbbResolution->ToggleList();

	m_pCbbResolution->ClearAllItems();

	if( bFullScreen )
	{
		for (ListVideoMode::iterator it = listVideoMode.begin(); it != listVideoMode.end(); ++it)
		{
			SVideoModeInfo& tempvideoModeInfo = *it;
			swprintf_s(buf, 256, GetDisplayStringManager()->GetString("DST_OPTION_RESOLUTION_FORMAT"), tempvideoModeInfo.iWidth, tempvideoModeInfo.iHeight, tempvideoModeInfo.iColorDepth);

			m_pCbbResolution->AddItem(buf);
		}
	}
	else
	{
		for (ListVideoMode::iterator it = listVideoMode.begin(); it != listVideoMode.end(); ++it)
		{
			SVideoModeInfo& tempvideoModeInfo = *it;
			swprintf_s(buf, 256, GetDisplayStringManager()->GetString("DST_OPTION_RESOLUTION_FORMAT_WINMODE"), tempvideoModeInfo.iWidth, tempvideoModeInfo.iHeight);

			m_pCbbResolution->AddItem(buf);
		}
	}

#ifdef dOPTION_RESOLUTION_TEXT
	m_pCbbResolution->SetText(GetVideoModeText().c_str());
#else
	/*RwVideoMode vm;
	RwEngineGetVideoModeInfo(&vm, RwEngineGetCurrentVideoMode());*/

	m_sSelectMode.iWidth = CNtlApplication::GetInstance()->GetUserWidth();
	m_sSelectMode.iHeight = CNtlApplication::GetInstance()->GetUserHeight();
	m_sSelectMode.iColorDepth = CNtlApplication::GetInstance()->GetBitDepth();

	RwInt32 nIndex = GetVideoMode( bDefault );

	/*if( bFullScreen )*/
		m_pCbbResolution->SelectItem(nIndex);
	/*else
		m_pCbbResolution->SelectItem(-1);*/
#endif

	/*m_pCbbResolution->Enable(B2b(bFullScreen));*/
}

RwInt32 COptionGraphic::GetVideoMode( RwBool bDefault /*= FALSE*/ )
{
	RwBool bFullScreen = CNtlApplication::GetInstance()->IsFullScreen();
	ListVideoMode&	listVideoMode = GetVideoModeList(bFullScreen);

	SVideoModeInfo	videoModeInfo;
	GetSelectVideoMode(videoModeInfo);

	int i = 0;

	if( bFullScreen )
	{
		for (ListVideoMode::iterator it = listVideoMode.begin(); it != listVideoMode.end(); ++it, ++i)
		{
			SVideoModeInfo& tempvideoModeInfo = *it;
			if( bDefault )
			{
				if (GetNtlStorageMTContainer()->GetDefaultInt( dSTORAGE_GRAPHIC_SELECT_VIDEODEPTH ) == tempvideoModeInfo.iColorDepth &&
					GetNtlStorageMTContainer()->GetDefaultInt( dSTORAGE_GRAPHIC_SELECT_VIDEOHEIGHT ) == tempvideoModeInfo.iHeight &&
					GetNtlStorageMTContainer()->GetDefaultInt( dSTORAGE_GRAPHIC_SELECT_VIDEOWIDTH ) == tempvideoModeInfo.iWidth)
				{
					return i;
				}

			}
			else
			{
				if (videoModeInfo.iColorDepth == tempvideoModeInfo.iColorDepth &&
					videoModeInfo.iHeight == tempvideoModeInfo.iHeight &&
					videoModeInfo.iWidth == tempvideoModeInfo.iWidth)
				{
					return i;
				}
			}
		}
	}
	else
	{
		for (ListVideoMode::iterator it = listVideoMode.begin(); it != listVideoMode.end(); ++it, ++i)
		{
			SVideoModeInfo& tempvideoModeInfo = *it;
			if( bDefault )
			{
				if (GetNtlStorageMTContainer()->GetDefaultInt( dSTORAGE_GRAPHIC_SELECT_VIDEOHEIGHT ) == tempvideoModeInfo.iHeight &&
					GetNtlStorageMTContainer()->GetDefaultInt( dSTORAGE_GRAPHIC_SELECT_VIDEOWIDTH ) == tempvideoModeInfo.iWidth)
				{
					return i;
				}

			}
			else
			{
				if (videoModeInfo.iHeight == tempvideoModeInfo.iHeight &&
					videoModeInfo.iWidth == tempvideoModeInfo.iWidth)
				{
					return i;
				}
			}
		}
	}

	return -1;
}

void COptionGraphic::SetVideoMode( RwInt32 iIndex )
{
	RwBool bFullScreen = CNtlApplication::GetInstance()->IsFullScreen();
	ListVideoMode& listVideoMode = GetVideoModeList(bFullScreen);
	int i = 0;

	if( bFullScreen )
	{
		for (ListVideoMode::iterator it = listVideoMode.begin(); it != listVideoMode.end(); ++it, ++i)
		{
			SVideoModeInfo& tempvideoModeInfo = *it;
			if (iIndex == i)
			{
				SelectVideoMode(tempvideoModeInfo.iWidth, tempvideoModeInfo.iHeight, tempvideoModeInfo.iColorDepth);				
			}
		}
	}
	else
	{
		for (ListVideoMode::iterator it = listVideoMode.begin(); it != listVideoMode.end(); ++it, ++i)
		{
			SVideoModeInfo& tempvideoModeInfo = *it;
			if (iIndex == i)
			{
				SelectVideoModeWin(tempvideoModeInfo.iWidth, tempvideoModeInfo.iHeight);
			}
		}
	}
}

std::wstring COptionGraphic::GetVideoModeText()
{
	SVideoModeInfo	tempvideoModeInfo;
	GetSelectVideoMode(tempvideoModeInfo);

	wchar_t	buf[256];
	swprintf_s(buf, 256, GetDisplayStringManager()->GetString("DST_OPTION_RESOLUTION_FORMAT"), tempvideoModeInfo.iWidth, tempvideoModeInfo.iHeight, tempvideoModeInfo.iColorDepth);

	return buf;
}

void COptionGraphic::SetVideoModeText( const std::wstring& strText )
{
	SVideoModeInfo tempvideoModeInfo;
	ZeroMemory(&tempvideoModeInfo, sizeof(tempvideoModeInfo));
	swscanf_s(strText.c_str(), GetDisplayStringManager()->GetString("DST_OPTION_RESOLUTION_FORMAT"), &tempvideoModeInfo.iWidth, &tempvideoModeInfo.iHeight, &tempvideoModeInfo.iColorDepth);
	SelectVideoMode(tempvideoModeInfo.iWidth, tempvideoModeInfo.iHeight, tempvideoModeInfo.iColorDepth);
}

RwInt32 COptionGraphic::GetGammaValue()
{
	return (RwInt32)(GetNtlStorageManager()->GetFloatData( dSTORAGE_GRAPHIC_GAMMA ) * 25.0f);  ;
}

void COptionGraphic::SetGammaValue( RwInt32 iValue )
{
	m_iGamma = iValue;
	GetNtlStorageManager()->SetData( dSTORAGE_GRAPHIC_GAMMA , (float)m_iGamma * 0.04f );
}

void COptionGraphic::OnGammaChanged( RwInt32 iParam )
{
	CNtlApplication::GetInstance()->SetGammaRamp( (float) iParam * 0.04f );
}

void COptionGraphic::SetFpsValue(RwInt32 iValue)
{
	GetNtlStorageManager()->SetData(dSTORAGE_GRAPHIC_FPS, iValue);

	CNtlApplication::m_uiFrameRate = iValue;
}

void COptionGraphic::OnFpsSliderMoved(RwInt32 iValue)
{
	SetFpsToolTip(iValue);
}

void COptionGraphic::SetFpsToolTip(RwInt32 iValue)
{
	char toolTipBuf[50];
	sprintf(toolTipBuf, "%d", iValue);
	m_pScbFps->SetToolTip(toolTipBuf);
}

void COptionGraphic::SelectVideoMode( RwInt32 iWidth, RwInt32 iHeight, RwInt32 iColorDepth )
{
	if(m_sSelectMode.iWidth == iWidth && m_sSelectMode.iHeight == iHeight &&
		m_sSelectMode.iColorDepth == iColorDepth)
		return;

	m_sSelectMode.iWidth		= iWidth;
	m_sSelectMode.iHeight		= iHeight;
	m_sSelectMode.iColorDepth	= iColorDepth;

	GetNtlStorageManager()->SetData( dSTORAGE_GRAPHIC_SELECT_VIDEOWIDTH, iWidth );
	GetNtlStorageManager()->SetData( dSTORAGE_GRAPHIC_SELECT_VIDEOHEIGHT, iHeight );
	GetNtlStorageManager()->SetData( dSTORAGE_GRAPHIC_SELECT_VIDEODEPTH, iColorDepth );

	/*CNtlApplication::GetInstance()->ChangeVideoMode( iWidth, iHeight, iColorDepth );*/
}

void COptionGraphic::SelectVideoModeWin( RwInt32 iWidth, RwInt32 iHeight )
{
	if(m_sSelectMode.iWidth == iWidth && m_sSelectMode.iHeight == iHeight )
		return;

	m_sSelectMode.iWidth = iWidth;
	m_sSelectMode.iHeight = iHeight;
	
	GetNtlStorageManager()->SetData( dSTORAGE_GRAPHIC_SELECT_VIDEOWIDTH, iWidth );
	GetNtlStorageManager()->SetData( dSTORAGE_GRAPHIC_SELECT_VIDEOHEIGHT, iHeight );
}

void COptionGraphic::GetSelectVideoMode( SVideoModeInfo& sVideoModeInfo )
{
	sVideoModeInfo = m_sSelectMode;
}

ListVideoMode& COptionGraphic::GetVideoModeList( RwBool bFullMode )
{
	RwInt32 numVidModes = RwEngineGetNumVideoModes();

	if(bFullMode)
	{
		if(m_listFullVideo.size() == 0)
		{
			RwVideoMode vidModemInfo;
			for (RwInt32 vidMode = 0; vidMode < numVidModes; vidMode++)
			{
				RwEngineGetVideoModeInfo(&vidModemInfo, vidMode);

				SVideoModeInfo sInfo;

				sInfo.iWidth		= vidModemInfo.width;
				sInfo.iHeight		= vidModemInfo.height;
				sInfo.iColorDepth	= vidModemInfo.depth;

				if(vidModemInfo.width > 800 && vidModemInfo.height > 600)
				{
					if (vidModemInfo.flags & rwVIDEOMODEEXCLUSIVE)
						m_listFullVideo.push_back(sInfo);
				}
			}
		}

		return m_listFullVideo;
	}
	else
	{
		if(m_listWinVideo.size() == 0)
		{
			RwVideoMode vidModemInfo;
			for (RwInt32 vidMode = 0; vidMode < numVidModes; vidMode++)
			{
				RwEngineGetVideoModeInfo(&vidModemInfo, vidMode);

				SVideoModeInfo sInfo;

				sInfo.iWidth		= vidModemInfo.width;
				sInfo.iHeight		= vidModemInfo.height;
				sInfo.iColorDepth	= vidModemInfo.depth;

				RwBool bUnique = TRUE;
				for each( SVideoModeInfo sWinInfo in m_listWinVideo )
				{
					if( sWinInfo.iWidth == sInfo.iWidth &&
						sWinInfo.iHeight == sInfo.iHeight )
						bUnique = FALSE;
				}

				if( bUnique )
				{
					if(vidModemInfo.width > 800 && vidModemInfo.height > 600)
					{
						if (vidModemInfo.flags & rwVIDEOMODEEXCLUSIVE)
							m_listWinVideo.push_back(sInfo);
					}
				}
			}
		}

		return m_listWinVideo;
	}
}

void COptionGraphic::SetPreset( int nLevel )
{
	switch( nLevel )
	{
	case eQUALITY_LOW:
		{
			m_pScbGraphicQuality->SetValue( 0 );
			
			m_pScbTerrainDist->SetValue( 0, FALSE );
			m_pScbObjectDist->SetValue( 0, FALSE );
			m_pBtnTerrainShadow->SetDown( FALSE );
			m_pBtnWaterEffect->SetDown( FALSE );
			//m_pScbCharacterDist->SetValue( 1, FALSE);
			m_pBtnCharacterShadow->SetDown( FALSE );
			m_pScbMagicEffect->SetValue( 1, FALSE );
			m_pBtnHdrEffect->SetDown( FALSE );
			m_pBtnCartoonEdge->SetDown( FALSE );
			/*m_pBtnUpgradeEffect->SetDown( FALSE );*/
			m_pScbWeatherEffect->SetValue( 0, FALSE );
			m_pScbTextureQuality->SetValue( 0, FALSE );
		}
		break;
	case eQUALITY_MIDDLE:
		{
			m_pScbGraphicQuality->SetValue( 1 );

			m_pScbTerrainDist->SetValue( 2, FALSE );
			m_pScbObjectDist->SetValue( 2, FALSE );
			m_pBtnTerrainShadow->SetDown( TRUE );
			m_pBtnWaterEffect->SetDown( FALSE );
			//m_pScbCharacterDist->SetValue( 1, FALSE );
			m_pBtnCharacterShadow->SetDown( TRUE );
			m_pScbMagicEffect->SetValue( 2, FALSE );
			m_pBtnHdrEffect->SetDown( FALSE );
			m_pBtnCartoonEdge->SetDown( TRUE );
			/*m_pBtnUpgradeEffect->SetDown( TRUE );*/
			m_pScbWeatherEffect->SetValue( 0, FALSE );
			m_pScbTextureQuality->SetValue( 1, FALSE );
		}
		break;
	case eQUALITY_HIGH:
		{
			m_pScbGraphicQuality->SetValue( 2 );

			m_pScbTerrainDist->SetValue( 3, FALSE );
			m_pScbObjectDist->SetValue( 3, FALSE );
			m_pBtnTerrainShadow->SetDown( TRUE );
			m_pBtnWaterEffect->SetDown( TRUE );
			//m_pScbCharacterDist->SetValue( 3, FALSE );
			m_pBtnCharacterShadow->SetDown( TRUE );
			m_pScbMagicEffect->SetValue( 3, FALSE );
			m_pBtnHdrEffect->SetDown( TRUE );
			m_pBtnCartoonEdge->SetDown( TRUE );
			/*m_pBtnUpgradeEffect->SetDown( TRUE );*/
			m_pScbWeatherEffect->SetValue( 1, FALSE );
			m_pScbTextureQuality->SetValue( 2, FALSE );
		}
		break;
	case eQUALITY_USER:
		{
			m_pScbGraphicQuality->SetValue( 3 );
		}	
		break;
	default:
		break;
	}
}

void COptionGraphic::OnOtherDataChanged( RwInt32 nParam )
{
	SetPreset( eQUALITY_USER );
}

void COptionGraphic::OnOtherDataToggled( gui::CComponent* pComponent, bool bToggle )
{
	SetPreset( eQUALITY_USER );
}

void COptionGraphic::OnAbsoluteMouseWheel( RwInt32 nFlag, RwInt16 sDelta, CPos& pos )
{
	// If the application is toggled to full-screen <-> window mode
// Close the listbox if it is currently popped up.
	gui::CListBox* pListBox = m_pCbbResolution->GetListBox();
	if( pListBox->IsVisible() )
		m_pCbbResolution->ToggleList();

	CAPTURE_MOUSEWHEELMOVE_SCROLLBAR( m_pThis->IsVisible() ,m_pThis, m_pThis, m_pThisScrollBar, sDelta * 12, pos );
}

void COptionGraphic::OnListToggled( BOOL bToggled, gui::CComponent* pComponent )
{
	if (bToggled)
	{
		pComponent->Raise();
	}
	else
	{
		pComponent->Lower();
	}
}

int COptionGraphic::RestoreHandler()
{
	SetComponentResolution();

	return TRUE;
}


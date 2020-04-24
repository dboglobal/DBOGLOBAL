// PropertiesPane.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "2DParticleEditor.h"
#include "PropertiesPane.h"

// Gui Library
#include "gui_particlepath.h"
#include "PathManager.h"
#include "GuideManager.h"

// CPropertiesPane

///< 프로퍼티를 ID와 함께 등록하기 위한 매크로
#define Mac_AddChildItem( pParent, cProperty, pItem, strName, initValue, nID )					\
	pItem		= (cProperty*)pParent->AddChildItem( new cProperty( strName , initValue ) );	\
	pItem->SetID( nID );

IMPLEMENT_DYNAMIC(CPropertiesPane, CWnd)

CPropertiesPane::CPropertiesPane()
{
	m_pActiveItem = NULL;
}

CPropertiesPane::~CPropertiesPane()
{
}

void CPropertiesPane::Show( BOOL bShow )
{
	m_pParticleItem->SetHidden( !bShow );
	m_pEmitterItem->SetHidden( !bShow );
	m_pUseAffectorItem->SetHidden( !bShow );
	m_pGravityItem->SetHidden( !bShow );
	m_pColorItem->SetHidden( !bShow );
	m_pPathItem->SetHidden( !bShow );
}

BEGIN_MESSAGE_MAP(CPropertiesPane, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, OnGridNotify)
END_MESSAGE_MAP()



// CPropertiesPane 메시지 처리기입니다.
int CPropertiesPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 정해진 프로퍼티 생성
	if( m_PropGrid.Create( CRect(0, 0, 0, 0), this, IDC_PROPERTY_GRID ) )
	{
		m_PropGrid.SetTheme( xtpGridThemeOffice2003 );
		m_PropGrid.SetVariableItemsHeight(TRUE);
		m_PropGrid.SetVariableHelpHeight(TRUE);
		m_PropGrid.ShowToolBar( TRUE);
		m_PropGrid.ShowHelp(TRUE);

		// Particle
		m_pParticleItem = (CXTPPropertyGridItemCategory*)m_PropGrid.AddCategory( "Particle Attributes" );
		m_pParticleItem->SetID( CATEGORY_COMMON );

		m_pParLifeTime	= (CXTPPropertyGridItemDouble*)m_pParticleItem->AddChildItem( new CXTPPropertyGridItemDouble(_T("Life"), 0.0f ) );
		m_pParLifeTime->SetID( ITEM_PARTICLE_LIFE );

		m_pParLimit		= (CXTPPropertyGridItemNumber*)m_pParticleItem->AddChildItem( new CXTPPropertyGridItemNumber( _T("Limit"), 0 ) );
		m_pParLimit->SetID( ITEM_PARTICLE_LIMIT );

		m_pParticleItem->SetHidden( FALSE );
		m_pParticleItem->Expand();

		// Emitter
		m_pEmitterItem	= (CXTPPropertyGridItemCategory*)m_PropGrid.AddCategory( "Emitter Attributs" );
		m_pEmitterItem->SetID( CATEGORY_EMITTER );

		Mac_AddChildItem( m_pEmitterItem, CXTPPropertyGridItemEnum, m_pEmitType, _T("Type\nSets the Type of the emitter.\n"), 0, ITEM_EMITTER_TYPE );
		m_pEmitType->GetConstraints()->AddConstraint(_T("Point"), 0);
		m_pEmitType->GetConstraints()->AddConstraint(_T("Box"), 1);
		m_pEmitType->SetEnum(0);
		Mac_AddChildItem( m_pEmitterItem, CXTPPropertyGridItemNumber, m_pEmitPosX, _T("X\nSets the X coordinate where the particle emitter will be located.\n"), 0, ITEM_EMITTER_POSX );
		Mac_AddChildItem( m_pEmitterItem, CXTPPropertyGridItemNumber, m_pEmitPosY, _T("Y\nSets the Y coordinate where the particle emitter will be located.\n"), 0, ITEM_EMITTER_POSY );
		Mac_AddChildItem( m_pEmitterItem, CXTPPropertyGridItemNumber, m_pEmitWidth, _T("Width\nSets the width of the particle emitter.\n"), 0, ITEM_EMITTER_WIDTH );
		Mac_AddChildItem( m_pEmitterItem, CXTPPropertyGridItemNumber, m_pEmitHeight, _T("Height\nThe particle emitter sets the height.\n"), 0, ITEM_EMITTER_HEIGHT );
		Mac_AddChildItem( m_pEmitterItem, CXTPPropertyGridItemNumber, m_pEmissivity, _T("Emissivity\nSets the emissivity per second of the emitter.\n1000 : 초당 1000개 발사"), 0, ITEM_EMITTER_EMISSIVITY );
		Mac_AddChildItem( m_pEmitterItem, CXTPPropertyGridItemDouble, m_pLifeMin, _T("Life[Min]\nThe emitter sets the minimum life of the particles to emit.\n5.5 : 5.5초"), 0.0f, ITEM_EMITTER_LIFEMIN );
		Mac_AddChildItem( m_pEmitterItem, CXTPPropertyGridItemDouble, m_pLifeMax, _T("Life[Max]\nThe emitter sets the maximum life of the particles to emit.\n5.5 : 5.5초"), 0.0f, ITEM_EMITTER_LIFEMAX );
		Mac_AddChildItem( m_pEmitterItem, CXTPPropertyGridItemDouble, m_pVelMin, _T("Velocity[Min]\nSets the minimum velocity of the particles that the emitter will emit.\n200 : Per second 200px move"), 0.0f, ITEM_EMITTER_VELMIN );
		Mac_AddChildItem( m_pEmitterItem, CXTPPropertyGridItemDouble, m_pVelMax, _T("Velocity[Max]\nSets the maximum velocity of the particles that the emitter will emit.\n200 : Per second 200px move"), 0.0f, ITEM_EMITTER_VELMAX );
		Mac_AddChildItem( m_pEmitterItem, CXTPPropertyGridItemDouble, m_pEmitDirX, _T("EmitDir X\nSets the X property of the direction vector emitted by the emitter.\n-1 is left, 1 is right"), 0.0f, ITEM_EMITTER_DIRX );
		Mac_AddChildItem( m_pEmitterItem, CXTPPropertyGridItemDouble, m_pEmitDirY, _T("EmitDir Y\nSets the Y property of the direction vector emitted by the emitter.\n-1 is the top, 1 is the bottom"), 0.0f, ITEM_EMITTER_DIRY );
		Mac_AddChildItem( m_pEmitterItem, CXTPPropertyGridItemDouble, m_pEmitAngle, _T("EmitAngle\nSets the angle of radiation emitted by the emitter..\n90 : A range of 45 degrees left and right based on the direction of radiation"), 0.0f, ITEM_EMITTER_ANGLE );
		Mac_AddChildItem( m_pEmitterItem, CXTPPropertyGridItemDouble, m_pRotRateMin, _T("Rot Rate[Min]\nSets the minimum per second rotation rate of the particles that the emitter will emit.\n-5 : Rotate left 5 times per second"), 0.0f, ITEM_EMITTER_ROTRATEMIN );
		Mac_AddChildItem( m_pEmitterItem, CXTPPropertyGridItemDouble, m_pRotRateMax, _T("Rot Rate[Max]\nSets the maximum per second rotation rate of the particles that the emitter will emit.\n5 : 5 rotations per second to the right"), 0.0f, ITEM_EMITTER_ROTRATEMAX );
		Mac_AddChildItem( m_pEmitterItem, CXTPPropertyGridItemDouble, m_pScaleMin, _T("Scale[Min]\nSets the minimum amount of zooming based on 1.\n0.5 : 50% size"), 0.0f, ITEM_EMITTER_SCALEMIN );
		Mac_AddChildItem( m_pEmitterItem, CXTPPropertyGridItemDouble, m_pScaleMax, _T("Scale[Max]\nSets the maximum amount of zooming based on 1.\n1.5 : 150% size"), 0.0f, ITEM_EMITTER_SCALEMAX );
		Mac_AddChildItem( m_pEmitterItem, CXTPPropertyGridItemColor, m_pColorMin, _T("Color[Min]\nMinimum RGB value of the particle emitted by the emitter"), RGB(255,255,255), ITEM_EMITTER_COLORMIN );
		Mac_AddChildItem( m_pEmitterItem, CXTPPropertyGridItemNumber, m_pAlphaMin, _T("Alpha[Min]\nMinimum transparency of particles emitted by the emitter\n0 : Fully transparent 255: fully opaque"), 255, ITEM_EMITTER_ALPHAMIN );
		Mac_AddChildItem( m_pEmitterItem, CXTPPropertyGridItemColor, m_pColorMax, _T("Color[Max]\nThe maximum RGB value of the particle emitted by the emitter."), RGB(255,255,255), ITEM_EMITTER_COLORMAX );
		Mac_AddChildItem( m_pEmitterItem, CXTPPropertyGridItemNumber, m_pAlphaMax, _T("Alpha[Max]\nMaximum transparency of particles emitted by the emitter \n0: Fully transparent 255: Fully opaque"), 255, ITEM_EMITTER_ALPHAMAX );
		

		m_pSurfaceFile	= (CXTPPropertyGridItem*)m_pEmitterItem->AddChildItem( new CXTPPropertyGridItem( _T("SurfaceFile\nThe name of the 2d surface script that the emitter uses to emit particles.") ) );
		m_pSurfaceFile->SetID( ITEM_EMITTER_SURFACEFILE );
		m_pSurfaceName	= (CXTPPropertyGridItem*)m_pEmitterItem->AddChildItem( new CXTPPropertyGridItem( _T("SurfaceName\nThe name of the 2d surface that the emitter uses to emit particles.") ) );
		m_pSurfaceName->SetID( ITEM_EMITTER_SURFACENAME );

		m_pEmitterItem->SetHidden( FALSE );
		m_pEmitterItem->Expand();

		// Use Affector
		m_pUseAffectorItem = (CXTPPropertyGridItemCategory*)m_PropGrid.AddCategory( "Use Affector Option" );
		m_pUseAffectorItem->SetID( CATEGORY_USEAFFECTOR );

		Mac_AddChildItem( m_pUseAffectorItem, CXTPPropertyGridItemBool, m_pUseGravity, _T("Use Gravity\nUse gravity effect.\n"), FALSE, ITEM_USEAFFECTOR_GRAVITY );
		Mac_AddChildItem( m_pUseAffectorItem, CXTPPropertyGridItemBool, m_pUseColor, _T("Use Color\nUse color change option.\n"), FALSE, ITEM_USEAFFECTOR_COLOR );
		Mac_AddChildItem( m_pUseAffectorItem, CXTPPropertyGridItemBool, m_pUsePath, _T("Use Path\nUse the spline path option.\n"), FALSE, ITEM_USEAFFECTOR_PATH );
		
		m_pUseAffectorItem->SetHidden( FALSE );
		m_pUseAffectorItem->Expand();

		// Affector Gravity
		m_pGravityItem	= (CXTPPropertyGridItemCategory*)m_PropGrid.AddCategory( "Gravity Affector Attributes" );
		m_pGravityItem->SetID( CATEGORY_GRAVITY );

		Mac_AddChildItem( m_pGravityItem, CXTPPropertyGridItemDouble, m_pGravityDirX, _T("GravityDir X\nSets the X property of the direction vector to which gravity factor gravity will act. \n-1 is left, 1 is right"), 0.0f, ITEM_GRAVITY_DIRX );
		Mac_AddChildItem( m_pGravityItem, CXTPPropertyGridItemDouble, m_pGravityDirY, _T("GravityDir Y\nSets the Y property of the direction vector that gravity factor gravity will act on. \n-1 is up, 1 is down"), 0.0f, ITEM_GRAVITY_DIRY );
		Mac_AddChildItem( m_pGravityItem, CXTPPropertyGridItemNumber, m_pGravityWeight, _T("Gravity Weight\nSets the force of gravity to act on. \n500: 500px per second affect particle properties.."), 0, ITEM_GRAVITY_WEIGHT );

		m_pGravityItem->SetHidden( FALSE );
		m_pGravityItem->Expand();

		// Affector Color
		m_pColorItem	= (CXTPPropertyGridItemCategory*)m_PropGrid.AddCategory( "Color Affector Attributes" );
		m_pColorItem->SetID( CATEGORY_COLOR );

		Mac_AddChildItem( m_pColorItem, CXTPPropertyGridItemNumber, m_pTransRed, _T("Trans RED\nRED attribute to change"), 255, ITEM_COLOR_TRANSRED );
		Mac_AddChildItem( m_pColorItem, CXTPPropertyGridItemBool, m_pIsTransRed, _T("Trans RED\nApplied / Not Applicable"), FALSE, ITEM_COLOR_ISTRANSRED );
		Mac_AddChildItem( m_pColorItem, CXTPPropertyGridItemNumber, m_pTransGreen, _T("Trans GREEN\nGREEN attribute to change"), 255, ITEM_COLOR_TRANSGREEN );
		Mac_AddChildItem( m_pColorItem, CXTPPropertyGridItemBool, m_pIsTransGreen, _T("Trans GREEN\nApplied / Not Applicable"), FALSE, ITEM_COLOR_ISTRANSGREEN );
		Mac_AddChildItem( m_pColorItem, CXTPPropertyGridItemNumber, m_pTransBlue, _T("Trans BLUE\nBLUE attribute to change"), 255, ITEM_COLOR_TRANSBLUE );
		Mac_AddChildItem( m_pColorItem, CXTPPropertyGridItemBool, m_pIsTransBlue, _T("Trans BLUE\nApplied / Not Applicable"), FALSE, ITEM_COLOR_ISTRANSBLUE );
		Mac_AddChildItem( m_pColorItem, CXTPPropertyGridItemNumber, m_pTransAlpha, _T("Trans ALPHA\nALPHA attribute to change"), 255, ITEM_COLOR_TRANSALPHA );
		Mac_AddChildItem( m_pColorItem, CXTPPropertyGridItemBool, m_pIsTransAlpha, _T("Trans ALPHA\nApplied / Not Applicable"), FALSE, ITEM_COLOR_ISTRANSALPHA );

		m_pColorItem->SetHidden( FALSE );
		m_pColorItem->Expand();

		// Path
		m_pPathItem = (CXTPPropertyGridItemCategory*)m_PropGrid.AddCategory( "Path Attributes" );
		m_pPathItem->SetID( CATEGORY_PATH );

		Mac_AddChildItem( m_pPathItem, CXTPPropertyGridItemEnum, m_pPathType, _T("Path type\nDetermines how the path is interpolated."), 0, ITEM_PATH_TYPE );
		m_pPathType->GetConstraints()->AddConstraint(_T("Line"), 0);
		m_pPathType->GetConstraints()->AddConstraint(_T("CatMullrom"), 1);
		m_pPathType->SetEnum(0);
		Mac_AddChildItem( m_pPathItem, CXTPPropertyGridItemDouble, m_pPathTime, _T("Total path time\nSets the total time for the path."), 0.0f, ITEM_PATH_TOTALTIME );
		Mac_AddChildItem( m_pPathItem, CXTPPropertyGridItemNumber, m_pPathCtrl, _T("Total control point\nSets the number of passes."), 0, ITEM_PATH_CTRLNUM );
		Mac_AddChildItem( m_pPathItem, CXTPPropertyGridItemNumber, m_pCurveSub, _T("Curve sub division point\nSets the number of split points that occur automatically in the middle of the path."), 0, ITEM_PATH_CURVESUBNUM );

		m_pPathItem->SetHidden( FALSE );
		m_pPathItem->Expand();
	}

	Show( FALSE );

	return 0;
}

void CPropertiesPane::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSize sz(0);

	if( m_PropGrid.GetSafeHwnd() )
	{
		m_PropGrid.MoveWindow(0, sz.cy, cx, cy - sz.cy );
	}
}

void CPropertiesPane::SetProperty()
{
	if( m_pActiveItem == NULL )
		return;

	// 파티클 카테고리
	m_pParticleItem->SetHidden( FALSE );
	m_pEmitterItem->SetHidden( FALSE );
	m_pUseAffectorItem->SetHidden( FALSE );

	// Common
	gui::CValueItem* pValueItem = m_pActiveItem->GetValueItem( gui::CParticleItem::ITEM_COMMON );

	gui::SNTL_COMMON sCommon;
	pValueItem->GetValue( "particle_life", sCommon.fLifeTime );
	pValueItem->GetValue( "particle_limit", sCommon.nLimit );
	
	m_pParLifeTime->SetDouble( sCommon.fLifeTime );
	m_pParLimit->SetNumber( sCommon.nLimit );

	// Emitter
	pValueItem = m_pActiveItem->GetValueItem( gui::CParticleItem::ITEM_EMITTER );
	if( !pValueItem->GetName().compare(_T("point")) )
	{
		gui::SNTL_EMITTER sEmitter;
		pValueItem->GetValue( "position", sEmitter.v2dPos );
		pValueItem->GetValue( "emissivity", sEmitter.fEmissivity );
		pValueItem->GetValue( "life", sEmitter.fLifeMin, sEmitter.fLifeMax );
		pValueItem->GetValue( "velocity", sEmitter.fVelMin, sEmitter.fVelMax );
		pValueItem->GetValue( "emit_dir", sEmitter.v2dEmitDir );
		pValueItem->GetValue( "emit_angle", sEmitter.fEmitAngle );
		pValueItem->GetValue( "rot_rate", sEmitter.fRotRateMin, sEmitter.fRotRateMax );
		pValueItem->GetValue( "scale_rate", sEmitter.fScaleMin, sEmitter.fScaleMax );
		pValueItem->GetValue( "rgba_min", sEmitter.rgbaMin );
		pValueItem->GetValue( "rgba_max", sEmitter.rgbaMax );
		pValueItem->GetValue( "surface_file", sEmitter.acSurfaceFile, sizeof( sEmitter.acSurfaceFile ) );
		pValueItem->GetValue( "surface_name", sEmitter.acSurfaceName, sizeof( sEmitter.acSurfaceFile ) );

		m_pEmitType->SetEnum(0);
		m_pEmitPosX->SetNumber( (int)sEmitter.v2dPos.x );
		m_pEmitPosY->SetNumber( (int)sEmitter.v2dPos.y );
		m_pEmitWidth->SetHidden( TRUE );
		m_pEmitHeight->SetHidden( TRUE );
		m_pEmissivity->SetNumber( (int)sEmitter.fEmissivity );
		m_pLifeMin->SetDouble( sEmitter.fLifeMin );
		m_pLifeMax->SetDouble( sEmitter.fLifeMax );
		m_pVelMin->SetDouble( sEmitter.fVelMin );
		m_pVelMax->SetDouble( sEmitter.fVelMax );
		m_pEmitDirX->SetDouble( sEmitter.v2dEmitDir.x );
		m_pEmitDirY->SetDouble( sEmitter.v2dEmitDir.y );
		m_pEmitAngle->SetDouble( sEmitter.fEmitAngle );
		m_pRotRateMin->SetDouble( sEmitter.fRotRateMin );
		m_pRotRateMax->SetDouble( sEmitter.fRotRateMax );
		m_pScaleMin->SetDouble( sEmitter.fScaleMin );
		m_pScaleMax->SetDouble( sEmitter.fScaleMax );
		m_pColorMin->SetColor( RGB( sEmitter.rgbaMin.red, sEmitter.rgbaMin.green, sEmitter.rgbaMin.blue ) );
		m_pAlphaMin->SetNumber( sEmitter.rgbaMin.alpha );
		m_pColorMax->SetColor( RGB( sEmitter.rgbaMax.red, sEmitter.rgbaMax.green, sEmitter.rgbaMax.blue ) );
		m_pAlphaMax->SetNumber( sEmitter.rgbaMax.alpha );
		m_pSurfaceFile->SetValue( sEmitter.acSurfaceFile );
		m_pSurfaceName->SetValue( sEmitter.acSurfaceName );
	}
	else if( !pValueItem->GetName().compare( _T("box")) )
	{
		gui::SNTL_EMITTER_BOX sEmitter;
		m_pEmitType->SetEnum(1);
		pValueItem->GetValue( "position", sEmitter.v2dPos );
		pValueItem->GetValue( "width", sEmitter.nWidth );
		pValueItem->GetValue( "height", sEmitter.nHeight );

		GetGuideManager()->SetBox( sEmitter.v2dPos.x, sEmitter.v2dPos.y, (float)sEmitter.nWidth, (float)sEmitter.nHeight );
		GetGuideManager()->ShowBox( TRUE );

		pValueItem->GetValue( "emissivity", sEmitter.fEmissivity );
		pValueItem->GetValue( "life", sEmitter.fLifeMin, sEmitter.fLifeMax );
		pValueItem->GetValue( "velocity", sEmitter.fVelMin, sEmitter.fVelMax );
		pValueItem->GetValue( "emit_dir", sEmitter.v2dEmitDir );
		pValueItem->GetValue( "emit_angle", sEmitter.fEmitAngle );
		pValueItem->GetValue( "rot_rate", sEmitter.fRotRateMin, sEmitter.fRotRateMax );
		pValueItem->GetValue( "scale_rate", sEmitter.fScaleMin, sEmitter.fScaleMax );
		pValueItem->GetValue( "rgba_min", sEmitter.rgbaMin );
		pValueItem->GetValue( "rgba_max", sEmitter.rgbaMax );
		pValueItem->GetValue( "surface_file", sEmitter.acSurfaceFile, sizeof( sEmitter.acSurfaceFile ) );
		pValueItem->GetValue( "surface_name", sEmitter.acSurfaceName, sizeof( sEmitter.acSurfaceFile ) );

		m_pEmitPosX->SetNumber( (int)sEmitter.v2dPos.x );
		m_pEmitPosY->SetNumber( (int)sEmitter.v2dPos.y );
		
		m_pEmitWidth->SetHidden( FALSE );
		m_pEmitWidth->SetNumber( sEmitter.nWidth );
		m_pEmitHeight->SetHidden( FALSE );
		m_pEmitHeight->SetNumber( sEmitter.nHeight );
		
		m_pEmissivity->SetNumber( (int)sEmitter.fEmissivity );
		m_pLifeMin->SetDouble( sEmitter.fLifeMin );
		m_pLifeMax->SetDouble( sEmitter.fLifeMax );
		m_pVelMin->SetDouble( sEmitter.fVelMin );
		m_pVelMax->SetDouble( sEmitter.fVelMax );
		m_pEmitDirX->SetDouble( sEmitter.v2dEmitDir.x );
		m_pEmitDirY->SetDouble( sEmitter.v2dEmitDir.y );
		m_pEmitAngle->SetDouble( sEmitter.fEmitAngle );
		m_pRotRateMin->SetDouble( sEmitter.fRotRateMin );
		m_pRotRateMax->SetDouble( sEmitter.fRotRateMax );
		m_pScaleMin->SetDouble( sEmitter.fScaleMin );
		m_pScaleMax->SetDouble( sEmitter.fScaleMax );
		m_pColorMin->SetColor( RGB( sEmitter.rgbaMin.red, sEmitter.rgbaMin.green, sEmitter.rgbaMin.blue ) );
		m_pAlphaMin->SetNumber( sEmitter.rgbaMin.alpha );
		m_pColorMax->SetColor( RGB( sEmitter.rgbaMax.red, sEmitter.rgbaMax.green, sEmitter.rgbaMax.blue ) );
		m_pAlphaMax->SetNumber( sEmitter.rgbaMax.alpha );
		m_pSurfaceFile->SetValue( sEmitter.acSurfaceFile );
		m_pSurfaceName->SetValue( sEmitter.acSurfaceName );
	}

	// Gravity
	pValueItem = m_pActiveItem->GetValueItem( gui::CParticleItem::ITEM_GRAVITY );

	if( pValueItem->GetSize() > 0 )
	{
		m_pUseGravity->SetBool( TRUE );
		m_pGravityItem->SetHidden( FALSE );
	}
	else
	{
		m_pUseGravity->SetBool( FALSE );
		m_pGravityItem->SetHidden( TRUE );
	}

	gui::SNTL_GRAVITY sGravity;

	pValueItem->GetValue( "gravity_dir", sGravity.v2dGravity );
	pValueItem->GetValue( "gravity_weight", sGravity.fWeight );

	m_pGravityDirX->SetDouble( sGravity.v2dGravity.x );
	m_pGravityDirY->SetDouble( sGravity.v2dGravity.y );
	m_pGravityWeight->SetNumber( (int)sGravity.fWeight );
	
	// Color
	pValueItem = m_pActiveItem->GetValueItem( gui::CParticleItem::ITEM_COLOR );

	if( pValueItem->GetSize() > 0 )
	{
		m_pUseColor->SetBool( TRUE );
		m_pColorItem->SetHidden( FALSE );
	}
	else
	{
		m_pUseColor->SetBool( FALSE );
		m_pColorItem->SetHidden( TRUE );
	}

	gui::SNTL_COLOR sColor;
	if ( pValueItem->GetValue( "red", sColor.uRed ) )
	{
		m_pTransRed->SetNumber( sColor.uRed );
		m_pIsTransRed->SetBool( TRUE );
	}
	else
	{
		m_pTransRed->SetNumber( 255 );
		m_pIsTransRed->SetBool( FALSE );
	}

	if( pValueItem->GetValue( "green", sColor.uGreen ) )
	{
		m_pTransGreen->SetNumber( sColor.uGreen );
		m_pIsTransGreen->SetBool( TRUE );
	}
	else
	{
		m_pTransGreen->SetNumber( 255 );
		m_pIsTransGreen->SetBool( FALSE );

	}

	if( pValueItem->GetValue( "blue", sColor.uBlue ) )
	{
		m_pTransBlue->SetNumber( sColor.uBlue );
		m_pIsTransBlue->SetBool( TRUE );
	}
	else
	{
		m_pTransBlue->SetNumber( 255 );
		m_pIsTransBlue->SetBool( FALSE );

	}

	if( pValueItem->GetValue( "alpha", sColor.uAlpha ) )
	{
		m_pTransAlpha->SetNumber( sColor.uAlpha );
		m_pIsTransAlpha->SetBool( TRUE );
	}
	else
	{
		m_pTransAlpha->SetNumber( 255 );
		m_pIsTransAlpha->SetBool( FALSE );
	}

	// Path
	pValueItem = m_pActiveItem->GetValueItem( gui::CParticleItem::ITEM_PATH );

	gui::SPathInfo sPathInfo;

	pValueItem->GetValue( "path_type", sPathInfo.nType );
	pValueItem->GetValue( "path_time", sPathInfo.fTime );
	pValueItem->GetValue( "path_ctrlnum", sPathInfo.nCtrlNum );
	pValueItem->GetValue( "path_curvesubnum", sPathInfo.nCurveSubNum );

	m_pPathType->SetEnum( sPathInfo.nType );
	m_pPathTime->SetDouble( sPathInfo.fTime );
	m_pPathCtrl->SetNumber( sPathInfo.nCtrlNum );
	m_pCurveSub->SetNumber( sPathInfo.nCurveSubNum );
	
	if( pValueItem->GetSize() > 0 )
	{
		m_pUsePath->SetBool( TRUE );
		m_pPathItem->SetHidden( FALSE );
		CPathManager::GetInstance()->SetActivePathItem( pValueItem );
	}
	else
	{
		m_pUsePath->SetBool( FALSE );
		m_pPathItem->SetHidden( TRUE );
	}
}


void CPropertiesPane::SetActiveProperty( gui::CParticleItem* pItem )
{
	DeActiveProperty();

	m_pActiveItem = pItem;
	SetProperty();
}

void CPropertiesPane::DeActiveProperty()
{
	m_pActiveItem = NULL;
	
	CPathManager::GetInstance()->DeActivePathItem();
	CGuideManager::GetInstance()->ShowBox( FALSE );

	Show( FALSE );
}

LRESULT CPropertiesPane::OnGridNotify( WPARAM wParam, LPARAM lParam )
{
	if( m_pActiveItem == NULL )
		return -1;

	if(wParam == XTP_PGN_ITEMVALUE_CHANGED)
	{
		CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;
		CXTPPropertyGridItem* pParentItem = pItem->GetParentItem();

		switch( pParentItem->GetID() )
		{
		case CATEGORY_COMMON:
			{
				gui::CValueItem* pValueItem = m_pActiveItem->GetValueItem( gui::CParticleItem::ITEM_COMMON );
				
				TCHAR tchar[256];
				switch( pItem->GetID() )
				{
				case ITEM_PARTICLE_LIFE:
					_stprintf_s( tchar, 256, _T("%f"), m_pParLifeTime->GetDouble() );
					pValueItem->AddValue( "particle_life", tchar );
					break;
				case ITEM_PARTICLE_LIMIT:
					_stprintf_s( tchar, 256, _T("%d"), m_pParLimit->GetNumber() );
					pValueItem->AddValue( "particle_limit", tchar );
					break;
				default:
					_stprintf_s( tchar, 256, _T("CPropertiesPane::OnGridNotify (FILE : %s), (LINE : %d)"), __FILE__, __LINE__ );
					AfxMessageBox( tchar, MB_OK );
					break;
				}
			}
			break;
		case CATEGORY_EMITTER:
			{
				gui::CValueItem* pValueItem = m_pActiveItem->GetValueItem( gui::CParticleItem::ITEM_EMITTER );

				TCHAR tchar[256];
				switch( pItem->GetID() )
				{
				case ITEM_EMITTER_TYPE:
					if( m_pEmitType->GetEnum() == 0 )	// POINT
					{
						m_pEmitWidth->SetHidden( TRUE );
						m_pEmitHeight->SetHidden( TRUE );
						pValueItem->SetName( "point" );

						GetGuideManager()->SetBox( (float)m_pEmitPosX->GetNumber(),
							(float)m_pEmitPosY->GetNumber(),
							0.0f,
							0.0f );
						GetGuideManager()->ShowBox( FALSE );
					}
					else if( m_pEmitType->GetEnum() == 1 )	// BOX
					{
						m_pEmitWidth->SetHidden( FALSE );
						m_pEmitHeight->SetHidden( FALSE );
						pValueItem->SetName( "box" );

						GetGuideManager()->SetBox( (float)m_pEmitPosX->GetNumber(),
							(float)m_pEmitPosY->GetNumber(),
							(float)m_pEmitWidth->GetNumber(),
							(float)m_pEmitHeight->GetNumber() );
						GetGuideManager()->ShowBox( TRUE );
					}
					break;
				case ITEM_EMITTER_POSX:
				case ITEM_EMITTER_POSY:
					_stprintf_s( tchar, 256, _T("(%d,%d)"), m_pEmitPosX->GetNumber(), m_pEmitPosY->GetNumber() );
					pValueItem->AddValue( "position", tchar );
					break;
				case ITEM_EMITTER_WIDTH:
				case ITEM_EMITTER_HEIGHT:
					_stprintf_s( tchar, 256, _T("%d"), m_pEmitWidth->GetNumber() );
					pValueItem->AddValue( "width", tchar );
								
					_stprintf_s( tchar, 256, _T("%d"), m_pEmitHeight->GetNumber() );
					pValueItem->AddValue( "height", tchar );

					GetGuideManager()->SetBox( (float)m_pEmitPosX->GetNumber(),
						(float)m_pEmitPosY->GetNumber(),
						(float)m_pEmitWidth->GetNumber(),
						(float)m_pEmitHeight->GetNumber() );

					break;
				case ITEM_EMITTER_EMISSIVITY:
					_stprintf_s( tchar, 256, _T("%d"), m_pEmissivity->GetNumber() );
					pValueItem->AddValue( "emissivity", tchar );
					break;
				case ITEM_EMITTER_LIFEMIN:
				case ITEM_EMITTER_LIFEMAX:
					_stprintf_s( tchar, 256, _T("(%f,%f)"), (float)m_pLifeMin->GetDouble(), (float)m_pLifeMax->GetDouble() );
					pValueItem->AddValue( "life", tchar );
					break;
				case ITEM_EMITTER_VELMIN:
				case ITEM_EMITTER_VELMAX:
					_stprintf_s( tchar, 256, _T("(%f,%f)"), (float)m_pVelMin->GetDouble(), (float)m_pVelMax->GetDouble() );
					pValueItem->AddValue( "velocity", tchar );
					break;
				case ITEM_EMITTER_DIRX:
				case ITEM_EMITTER_DIRY:
					_stprintf_s( tchar, 256, _T("(%f,%f)"), (float)m_pEmitDirX->GetDouble(), (float)m_pEmitDirY->GetDouble() );
					pValueItem->AddValue( "emit_dir", tchar );
					break;
				case ITEM_EMITTER_ANGLE:
					_stprintf_s( tchar, 256, _T("%f"), (float)m_pEmitAngle->GetDouble() );
					pValueItem->AddValue( "emit_angle", tchar );
					break;
				case ITEM_EMITTER_ROTRATEMIN:
				case ITEM_EMITTER_ROTRATEMAX:
					_stprintf_s( tchar, 256, _T("(%f,%f)"), (float)m_pRotRateMin->GetDouble(), (float)m_pRotRateMax->GetDouble() );
					pValueItem->AddValue( "rot_rate", tchar );
					break;
				case ITEM_EMITTER_SCALEMIN:
				case ITEM_EMITTER_SCALEMAX:
					_stprintf_s( tchar, 256, _T("(%f,%f)"), (float)m_pScaleMin->GetDouble(), (float)m_pScaleMax->GetDouble() );
					pValueItem->AddValue( "scale_rate", tchar );
					break;
				case ITEM_EMITTER_COLORMIN:
				case ITEM_EMITTER_ALPHAMIN:
					{
						RwRGBA rgbaMin;
						memset( &rgbaMin, 0xFF, sizeof( RwRGBA ) );
						COLORREF colorMin = m_pColorMin->GetColor();
						rgbaMin.red = GetRValue( colorMin );
						rgbaMin.green = GetGValue( colorMin );
						rgbaMin.blue = GetBValue( colorMin );
						rgbaMin.alpha = (unsigned char) m_pAlphaMin->GetNumber();

						_stprintf_s( tchar, 256, _T("(%d,%d,%d,%d)"), rgbaMin.red, rgbaMin.green, rgbaMin.blue, rgbaMin.alpha );
						pValueItem->AddValue( "rgba_min", tchar );
					}
					break;
				case ITEM_EMITTER_COLORMAX:
				case ITEM_EMITTER_ALPHAMAX:
					{
						RwRGBA rgbaMax;
						memset( &rgbaMax, 0xFF, sizeof( RwRGBA ) );
						COLORREF colorMax = m_pColorMax->GetColor();
						rgbaMax.red = GetRValue( colorMax );
						rgbaMax.green = GetGValue( colorMax );
						rgbaMax.blue = GetBValue( colorMax );
						rgbaMax.alpha = (unsigned char) m_pAlphaMax->GetNumber();

						_stprintf_s( tchar, 256, _T("(%d,%d,%d,%d)"), rgbaMax.red, rgbaMax.green, rgbaMax.blue, rgbaMax.alpha );
						pValueItem->AddValue( "rgba_max", tchar );
					}
					break;
				case ITEM_EMITTER_SURFACEFILE:
					pValueItem->AddValue( "surface_file", std::string( m_pSurfaceFile->GetValue() ) );
					break;
				case ITEM_EMITTER_SURFACENAME:
					pValueItem->AddValue( "surface_name", std::string( m_pSurfaceName->GetValue() ) );
					break;
				default:
					_stprintf_s( tchar, 256, _T("CPropertiesPane::OnGridNotify (FILE : %s), (LINE : %d)"), __FILE__, __LINE__ );
					AfxMessageBox( tchar, MB_OK );
					break;
				}
			}
			break;
		case CATEGORY_USEAFFECTOR:
			{
				switch( pItem->GetID() )
				{
				case ITEM_USEAFFECTOR_GRAVITY:
					{
						m_pGravityItem->SetHidden( !m_pUseGravity->GetBool() );

						if( !m_pUseGravity->GetBool() )
						{
							gui::CValueItem* pValueItem = m_pActiveItem->GetValueItem( gui::CParticleItem::ITEM_GRAVITY );
							pValueItem->Clear();
						}
					}
					break;
				case ITEM_USEAFFECTOR_COLOR:
					{
						m_pColorItem->SetHidden( !m_pUseColor->GetBool() );

						if( !m_pUseColor->GetBool() )
						{
							gui::CValueItem* pValueItem = m_pActiveItem->GetValueItem( gui::CParticleItem::ITEM_COLOR );
							pValueItem->Clear();
						}
					}
					break;
				case ITEM_USEAFFECTOR_PATH:
					{
						m_pPathItem->SetHidden( !m_pUsePath->GetBool() );
					
						if( !m_pUsePath->GetBool() )
						{
							gui::CValueItem* pValueItem = m_pActiveItem->GetValueItem( gui::CParticleItem::ITEM_PATH );
							pValueItem->Clear();

							CPathManager::GetInstance()->Destroy();
							CPathManager::GetInstance()->DeActivePathItem();
						}
						else
						{
							gui::CValueItem* pValueItem = m_pActiveItem->GetValueItem( gui::CParticleItem::ITEM_PATH );

							gui::SPathInfo sPathInfo;
							sPathInfo.nType = gui::PATH_LINE;
							sPathInfo.fTime = 10.0f;
							sPathInfo.nCtrlNum = 5;
							sPathInfo.nCurveSubNum = 5;
							
							pValueItem->AddValue( "path_type", sPathInfo.nType );
							pValueItem->AddValue( "path_time", sPathInfo.fTime );
							pValueItem->AddValue( "path_ctrlnum", sPathInfo.nCtrlNum );
							pValueItem->AddValue( "path_curvesubnum", sPathInfo.nCurveSubNum );


							m_pPathType->SetEnum( sPathInfo.nType );
							m_pPathTime->SetDouble( (float)sPathInfo.fTime );
							m_pPathCtrl->SetNumber( sPathInfo.nCtrlNum );
							m_pCurveSub->SetNumber( sPathInfo.nCurveSubNum );

							CHAR acBuffer[4096];

							CString strData;

							for( int i=0; i<sPathInfo.nCtrlNum; ++i )
							{
								sprintf_s( acBuffer, 4096, "(%d,%d,%0.3f)", 
									i * 20, 
									0, (float)i*2.5f );

								strData += acBuffer;

								if( i != sPathInfo.nCtrlNum-1 )
									strData += ",";
							}

							pValueItem->AddValue( "path_ctrlpoint", strData.GetBuffer() );

							CPathManager::GetInstance()->SetActivePathItem( pValueItem );
						}
					}
					break;
				}

			}
			break;
		case CATEGORY_GRAVITY:
			{
				gui::CValueItem* pValueItem = m_pActiveItem->GetValueItem( gui::CParticleItem::ITEM_GRAVITY );

				TCHAR tchar[256];
				switch( pItem->GetID() )
				{
				case ITEM_GRAVITY_DIRX:
				case ITEM_GRAVITY_DIRY:
					_stprintf_s( tchar, 256, _T("(%f,%f)"), (float)m_pGravityDirX->GetDouble(), (float)m_pGravityDirY->GetDouble() );
					pValueItem->AddValue( "gravity_dir", tchar );
					break;
				case ITEM_GRAVITY_WEIGHT:
					_stprintf_s( tchar, 256, _T("%d"), (int)m_pGravityWeight->GetNumber() );	
					pValueItem->AddValue( "gravity_weight", tchar );
					break;
				default:
					_stprintf_s( tchar, 256, _T("CPropertiesPane::OnGridNotify (FILE : %s), (LINE : %d)"), __FILE__, __LINE__ );
					AfxMessageBox( tchar, MB_OK );
					break;
				}
			}
			break;
		case CATEGORY_COLOR:
			{
				gui::CValueItem* pValueItem = m_pActiveItem->GetValueItem( gui::CParticleItem::ITEM_COLOR );

				TCHAR tchar[256];
				switch( pItem->GetID() )
				{
				case ITEM_COLOR_TRANSRED:
					{
						if( m_pIsTransRed->GetBool() )
						{
							_stprintf_s( tchar, 256, _T("%d"), (unsigned char)m_pTransRed->GetNumber() );
							pValueItem->AddValue( "red", tchar );
						}
					}
					break;
				case ITEM_COLOR_ISTRANSRED:
					{
						if( m_pIsTransRed->GetBool() )
						{
							_stprintf_s( tchar, 256, _T("%d"), (unsigned char)m_pTransRed->GetNumber() );
							pValueItem->AddValue( "red", tchar );
						}
						else
						{
							pValueItem->RemoveValue( "red" );
						}
					}
					break;

				case ITEM_COLOR_TRANSGREEN:
					{
						if( m_pIsTransGreen->GetBool() )
						{
							_stprintf_s( tchar, 256, _T("%d"), (unsigned char)m_pTransGreen->GetNumber() );
							pValueItem->AddValue( "green", tchar );
						}
					}
					break;
				case ITEM_COLOR_ISTRANSGREEN:
					{
						if( m_pIsTransGreen->GetBool() )
						{
							_stprintf_s( tchar, 256, _T("%d"), (unsigned char)m_pTransGreen->GetNumber() );
							pValueItem->AddValue( "green", tchar );
						}
						else
						{
							pValueItem->RemoveValue( "green" );
						}
					}
					break;

				case ITEM_COLOR_TRANSBLUE:
					{
						if( m_pIsTransBlue->GetBool() )
						{
							_stprintf_s( tchar, 256, _T("%d"), (unsigned char)m_pTransBlue->GetNumber() );
							pValueItem->AddValue( "blue", tchar );
						}
					}
					break;
				case ITEM_COLOR_ISTRANSBLUE:
					{
						if( m_pIsTransBlue->GetBool() )
						{
							_stprintf_s( tchar, 256, _T("%d"), (unsigned char)m_pTransBlue->GetNumber() );
							pValueItem->AddValue( "blue", tchar );
						}
						else
						{
							pValueItem->RemoveValue( "blue" );
						}
					}
					break;

				case ITEM_COLOR_TRANSALPHA:
					{
						if( m_pIsTransAlpha->GetBool() )
						{
							_stprintf_s( tchar, 256, _T("%d"), (unsigned char)m_pTransAlpha->GetNumber() );
							pValueItem->AddValue( "alpha", tchar );
						}
					}
					break;
				case ITEM_COLOR_ISTRANSALPHA:
					{
						if( m_pIsTransAlpha->GetBool() )
						{
							_stprintf_s( tchar, 256, _T("%d"), (unsigned char)m_pTransAlpha->GetNumber() );
							pValueItem->AddValue( "alpha", tchar );
						}
						else
						{
							pValueItem->RemoveValue( "alpha" );
						}
					}
					break;
				default:
					_stprintf_s( tchar, 256, _T("CPropertiesPane::OnGridNotify (FILE : %s), (LINE : %d)"), __FILE__, __LINE__ );
					AfxMessageBox( tchar, MB_OK );
					break;
				}
			}
			break;
		case CATEGORY_PATH:
			{
				gui::CValueItem* pValueItem = m_pActiveItem->GetValueItem( gui::CParticleItem::ITEM_PATH );

				TCHAR tchar[256];
				switch( pItem->GetID() )
				{
				case ITEM_PATH_TYPE:
					{
						_stprintf_s( tchar, 256, _T("%d"), m_pPathType->GetEnum() );
						pValueItem->AddValue( "path_type", tchar );

						CPathManager::GetInstance()->SetActivePathItem( pValueItem );
					}
					break;
				case ITEM_PATH_TOTALTIME:
					{
						_stprintf_s( tchar, 256, _T("%f"), (float)m_pPathTime->GetDouble() );
						pValueItem->AddValue( "path_time", tchar );

						// PathManager에서 수정
						CPathManager::GetInstance()->SetTotalTime( (float)m_pPathTime->GetDouble() );
						CPathManager::GetInstance()->UpdateCurrentPathData();
						CPathManager::GetInstance()->SaveCurrentPathData();
					}
					break;
				case ITEM_PATH_CTRLNUM:
					{
						int nCtrlNum = m_pPathCtrl->GetNumber();

						_stprintf_s( tchar, 256, _T("%d"), nCtrlNum );						
						pValueItem->AddValue( "path_ctrlnum", tchar );

						// PathManager에서 수정 ( Build 후에 )
						CPathManager::GetInstance()->SetActivePathItem( pValueItem );

						// PathManager에서 수정
						CPathManager::GetInstance()->SetTotalTime( (float)m_pPathTime->GetDouble() );
						CPathManager::GetInstance()->UpdateCurrentPathData();
						CPathManager::GetInstance()->SaveCurrentPathData();
					}
					break;
				case ITEM_PATH_CURVESUBNUM:
					{
						int nCurbeSubNum = m_pCurveSub->GetNumber();

						_stprintf_s( tchar, 256, _T("%d"), nCurbeSubNum );
						pValueItem->AddValue( "path_curvesubnum", tchar );
				
						CPathManager::GetInstance()->SetActivePathItem( pValueItem );
					}
					break;
				}
			}
			break;
		default:
			{
				TCHAR tchar[256];
				_stprintf_s( tchar, 256, _T("CPropertiesPane::OnGridNotify (FILE : %s), (LINE : %d)"), __FILE__, __LINE__ );
				AfxMessageBox( tchar, MB_OK );
			}
			break;
		}
	}

	return 0;
}
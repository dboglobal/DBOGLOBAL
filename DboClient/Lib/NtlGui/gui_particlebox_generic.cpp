#include "gui_precomp.h"
#include "gui_particleBox_generic.h"

#include "gui_renderer.h"
#include "gui_particlesystem.h"
#include "gui_particle_doc.h"
#include "gui_particlepath.h"

#include "GuiUtil.h"

START_GUI

CParticleBox_Generic::CParticleBox_Generic( CParticleBox* pSelf )
{
	m_pParticleBox = pSelf;
	m_pParticleBox->Enable( false );

	m_fPlayTime = 0.0f;
	m_fLifeTime = 5.0f;

	m_bPlay = FALSE;
	m_bRepeat = FALSE;

	m_SlotMove	= m_pParticleBox->SigMove().Connect( this, &CParticleBox_Generic::OnMove );
	m_SlotPaint	= m_pParticleBox->SigPaint().Connect( this, &CParticleBox_Generic::OnPaint );
	m_SlotSetOption = m_pParticleBox->SigSetOptions().Connect( this, &CParticleBox_Generic::OnSetOptions );

	m_pDoc = GetParticleDoc();
}

CParticleBox_Generic::~CParticleBox_Generic(VOID)
{
	LISTPSIT it = m_listParticle.begin();
	while( it != m_listParticle.end() )
	{
		NTL_DELETE( *it );
		++it;
	}
	m_listParticle.clear();
}

void CParticleBox_Generic::Play( BOOL bRepeat )
{
	m_bPlay = TRUE;
	m_bRepeat = bRepeat;
	m_fPlayTime = 0.0f;
	
	LISTPSIT it = m_listParticle.begin();
	while( it != m_listParticle.end() )
	{
		(*it)->Play( bRepeat );
		it++;
	}
}

void CParticleBox_Generic::Stop()
{
	m_bPlay = FALSE;
	m_fPlayTime = 0.0f;

	LISTPSIT it = m_listParticle.begin();
	while( it != m_listParticle.end() )
	{
		(*it)->Stop();
		it++;
	}
}

VOID CParticleBox_Generic::Update( FLOAT fElapsed )
{
	LISTPSIT it = m_listParticle.begin();
	while( it != m_listParticle.end() )
	{
		(*it)->Update( fElapsed );
		it++;
	}

	if( !m_bPlay )
		return;

	m_fPlayTime += fElapsed;
	if( m_fPlayTime > m_fLifeTime )
	{
		m_bPlay = FALSE;
		m_fPlayTime = 0.0f;
	}
}



void CParticleBox_Generic::SetDoc( CParticleDoc* pDoc )
{
	m_pDoc = pDoc;
}

void CParticleBox_Generic::Load( const char* pParticleName )
{
	if( m_pDoc == NULL )
		return;

	// 기존에 있던 파티클 시스템들을 삭제한다.
	LISTPSIT it = m_listParticle.begin();
	while( it != m_listParticle.end() )
	{
		NTL_DELETE( *it );

		++it;
	}
	m_listParticle.clear();

	// 파티클 그룹을 읽어온다.
	CParticleGroup* pGroup = m_pDoc->GetParticleGroup( pParticleName );
	if( pGroup == NULL )
		return;

	// 파티클 그룹에 있는 파티클들을 순회하며 각종 속성들을 읽어온다. :: 동작하는지 확인 해야함
	CParticleItem* pItem = pGroup->GetFirstParticle();
	while( pItem )
	{
		// 파티클 시스템을 생성
		CRectangle rect = m_pParticleBox->GetScreenRect();
		CParticleSystem* pParticle = NTL_NEW CParticleSystem( &rect, m_pParticleBox->GetSurfaceManager() );
		
		// 공통 속성 읽기
		CValueItem* pValueItem = pItem->GetValueItem( CParticleItem::ITEM_COMMON );

		SNTL_COMMON sCommon;

		pValueItem->GetValue( "particle_life", sCommon.fLifeTime );
		pValueItem->GetValue( "particle_limit", sCommon.nLimit );

		pParticle->SetLife( sCommon.fLifeTime );
		pParticle->SetLimit( sCommon.nLimit );
		
		// Emitter 속성 읽기
		pValueItem = pItem->GetValueItem( CParticleItem::ITEM_EMITTER );
		if( !pValueItem->GetName().compare( "point" ) )
		{
			if( pValueItem->GetSize() > 0 )
			{
				SNTL_EMITTER sEmitter;

				pValueItem->GetValue( "position",		sEmitter.v2dPos );
				pValueItem->GetValue( "emissivity",		sEmitter.fEmissivity );
				pValueItem->GetValue( "life",			sEmitter.fLifeMin, sEmitter.fLifeMax );
				pValueItem->GetValue( "velocity",		sEmitter.fVelMin, sEmitter.fVelMax );
				pValueItem->GetValue( "emit_dir",		sEmitter.v2dEmitDir );
				pValueItem->GetValue( "emit_angle",		sEmitter.fEmitAngle );
				pValueItem->GetValue( "rot_rate",		sEmitter.fRotRateMin, sEmitter.fRotRateMax );
				pValueItem->GetValue( "scale_rate",		sEmitter.fScaleMin, sEmitter.fScaleMax );
				pValueItem->GetValue( "rgba_min",		sEmitter.rgbaMin.red, sEmitter.rgbaMin.green
					, sEmitter.rgbaMin.blue, sEmitter.rgbaMin.alpha );
				pValueItem->GetValue( "rgba_max",		sEmitter.rgbaMax.red, sEmitter.rgbaMax.green
					, sEmitter.rgbaMax.blue, sEmitter.rgbaMax.alpha );
				pValueItem->GetValue( "surface_file",	sEmitter.acSurfaceFile, sizeof( sEmitter.acSurfaceFile ) );
				pValueItem->GetValue( "surface_name",	sEmitter.acSurfaceName, sizeof( sEmitter.acSurfaceName ) );

				pParticle->AddPointEmitter( rect.left + (int)sEmitter.v2dPos.x, rect.top + (int)sEmitter.v2dPos.y,
					sEmitter.fLifeMin, sEmitter.fLifeMax,
					sEmitter.fVelMin, sEmitter.fVelMax, sEmitter.fEmissivity,
					sEmitter.v2dEmitDir, sEmitter.fEmitAngle,
					sEmitter.fRotRateMin, sEmitter.fRotRateMax,
					sEmitter.fScaleMin, sEmitter.fScaleMax,
					sEmitter.rgbaMin.red, sEmitter.rgbaMax.red,
					sEmitter.rgbaMin.green, sEmitter.rgbaMax.green,
					sEmitter.rgbaMin.blue, sEmitter.rgbaMax.blue,
					sEmitter.rgbaMin.alpha, sEmitter.rgbaMax.alpha,
					sEmitter.acSurfaceFile, sEmitter.acSurfaceName );
			}
		}
		else if( !pValueItem->GetName().compare( "box" ) )
		{
			if( pValueItem->GetSize() > 0 )
			{
				SNTL_EMITTER_BOX sEmitter;

				pValueItem->GetValue( "position",		sEmitter.v2dPos );
				pValueItem->GetValue( "width",			sEmitter.nWidth );
				pValueItem->GetValue( "height",			sEmitter.nHeight );
				pValueItem->GetValue( "emissivity",		sEmitter.fEmissivity );
				pValueItem->GetValue( "life",			sEmitter.fLifeMin, sEmitter.fLifeMax );
				pValueItem->GetValue( "velocity",		sEmitter.fVelMin, sEmitter.fVelMax );
				pValueItem->GetValue( "emit_dir",		sEmitter.v2dEmitDir );
				pValueItem->GetValue( "emit_angle",		sEmitter.fEmitAngle );
				pValueItem->GetValue( "rot_rate",		sEmitter.fRotRateMin, sEmitter.fRotRateMax );
				pValueItem->GetValue( "scale_rate",		sEmitter.fScaleMin, sEmitter.fScaleMax );
				pValueItem->GetValue( "rgba_min",		sEmitter.rgbaMin.red, sEmitter.rgbaMin.green
					, sEmitter.rgbaMin.blue, sEmitter.rgbaMin.alpha );
				pValueItem->GetValue( "rgba_max",		sEmitter.rgbaMax.red, sEmitter.rgbaMax.green
					, sEmitter.rgbaMax.blue, sEmitter.rgbaMax.alpha );
				pValueItem->GetValue( "surface_file",	sEmitter.acSurfaceFile, sizeof( sEmitter.acSurfaceFile ) );
				pValueItem->GetValue( "surface_name",	sEmitter.acSurfaceName, sizeof( sEmitter.acSurfaceName ) );

				pParticle->AddBoxEmitter( rect.left + (int)sEmitter.v2dPos.x, rect.top + (int)sEmitter.v2dPos.y,
					sEmitter.nWidth, sEmitter.nHeight,
					sEmitter.fLifeMin, sEmitter.fLifeMax,
					sEmitter.fVelMin, sEmitter.fVelMax, sEmitter.fEmissivity,
					sEmitter.v2dEmitDir, sEmitter.fEmitAngle,
					sEmitter.fRotRateMin, sEmitter.fRotRateMax,
					sEmitter.fScaleMin, sEmitter.fScaleMax,
					sEmitter.rgbaMin.red, sEmitter.rgbaMax.red,
					sEmitter.rgbaMin.green, sEmitter.rgbaMax.green,
					sEmitter.rgbaMin.blue, sEmitter.rgbaMax.blue,
					sEmitter.rgbaMin.alpha, sEmitter.rgbaMax.alpha,
					sEmitter.acSurfaceFile, sEmitter.acSurfaceName );
			}
		}

		// Gravity 속성 읽기
		pValueItem = pItem->GetValueItem( CParticleItem::ITEM_GRAVITY );

		if( pValueItem->GetSize() > 0 )
		{
			SNTL_GRAVITY sGravity;
			memset( &sGravity, 0, sizeof( SNTL_GRAVITY ) );

			pValueItem->GetValue( "gravity_dir",		sGravity.v2dGravity );
			pValueItem->GetValue( "gravity_weight",	sGravity.fWeight );

			pParticle->AddGravityAffector( sGravity.v2dGravity, sGravity.fWeight );
		}

		// Color
		pValueItem = pItem->GetValueItem( CParticleItem::ITEM_COLOR );

		if( pValueItem->GetSize() > 0 )
		{
			SNTL_COLOR	sColor;
			memset( &sColor, 0, sizeof( SNTL_COLOR ) );

			if( pValueItem->GetValue( "red",			sColor.uRed ) )
				sColor.bRed = TRUE;

			if( pValueItem->GetValue( "green",		sColor.uGreen ) )
				sColor.bGreen = TRUE;

			if( pValueItem->GetValue( "blue",		sColor.uBlue ) )
				sColor.bBlue = TRUE;

			if( pValueItem->GetValue( "alpha",		sColor.uAlpha ) )
				sColor.bAlpha = TRUE;

			pParticle->AddColorAffector( sColor.uRed, sColor.uGreen, sColor.uBlue, sColor.uAlpha,
				sColor.bRed, sColor.bGreen, sColor.bBlue, sColor.bAlpha );
		}

		// Path
		pValueItem = pItem->GetValueItem( CParticleItem::ITEM_PATH );

		if( pValueItem->GetSize() > 0 )
		{
			SPathInfo sPathInfo;

			pValueItem->GetValue( "path_type", sPathInfo.nType );
			pValueItem->GetValue( "path_time", sPathInfo.fTime );
			pValueItem->GetValue( "path_ctrlnum", sPathInfo.nCtrlNum );
			pValueItem->GetValue( "path_curvesubnum", sPathInfo.nCurveSubNum );

			SCtrlPoint* pPointBuffer = NTL_NEW SCtrlPoint[sPathInfo.nCtrlNum];
			pValueItem->GetValue( "path_ctrlpoint", pPointBuffer, sPathInfo.nCtrlNum );

			pParticle->AddPath( sPathInfo, pPointBuffer, sPathInfo.nCtrlNum );

			NTL_ARRAY_DELETE( pPointBuffer );
		}

		// 파티클 시스템의 리스트에 넣어준다.
		m_listParticle.push_back( pParticle );

		pItem = pGroup->GetNextParticle();
	}
}

void CParticleBox_Generic::Load( CParticleDoc* doc )
{
	LISTPSIT itPsSystem = m_listParticle.begin();
	while( itPsSystem != m_listParticle.end() )
	{
		NTL_DELETE( *itPsSystem );
		++itPsSystem;
	}
	m_listParticle.clear();
}


VOID CParticleBox_Generic::OnSetOptions( const CComponentOptions& options )
{
	
}

VOID CParticleBox_Generic::OnPaint(VOID)
{
	LISTPSIT it = m_listParticle.begin();
	while( it != m_listParticle.end() )
	{
		(*it)->Render();

		it++;
	}
}

VOID CParticleBox_Generic::OnMove( INT nX,INT nY )
{
	CRectangle rect = m_pParticleBox->GetScreenRect();

	LISTPSIT itPsSystem = m_listParticle.begin();
	while( itPsSystem != m_listParticle.end() )
	{
		(*itPsSystem)->SetPosition( rect.left, rect.top );

		itPsSystem++;
	}
}


END_GUI
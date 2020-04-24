// Precompile
#include "stdafx.h"
#include "PathManager.h"

#include "position.h"

#include "pathpoint.h"

#include "2DParticleApp.h"

#define dSAVE_BUFFER_SIZE				4096
#define dPATH_MANAGER_TOOLTIP_WIDTH		100
#define dPATH_MANAGER_TOOLTIP_HEIGHT	50
#define dPATH_MANAGER_TOOLTIP_FONTSIZE	90
#define dPATH_MANAGER_TOOLTIP_OFFSET_X	20
#define dPATH_MANAGER_TOOLTIP_OFFSET_Y	20

CPathManager::CPathManager()
: m_pPath(NULL)
, m_pCtrlPoint(NULL)
, m_pActiveValueItem(NULL)
, m_pStbToolTip(NULL)
, m_nCount(0)
{
	::ZeroMemory( &m_sPathInfo, sizeof( gui::SPathInfo ) );
	m_pLinePath = NULL;
}

CPathManager::~CPathManager()
{
	Destroy();
}

void CPathManager::Create( int nCount )
{
	m_pPath = new CPathPoint[nCount];
	m_pCtrlPoint = new gui::SCtrlPoint[nCount];
	m_nCount = nCount;

	for( int i=0; i<m_nCount; ++i )
	{
		m_pPath[i].Create( 0, 0 );
		m_pCtrlPoint[i].fTime = 0.0f;
		m_pCtrlPoint[i].vPos.x = 0.0f;
		m_pCtrlPoint[i].vPos.y = 0.0f;
		m_pPath[i].SetNumber( i );
	}

	CRectangle rect(0, 0, dPATH_MANAGER_TOOLTIP_WIDTH, dPATH_MANAGER_TOOLTIP_HEIGHT);

	m_pStbToolTip = new gui::CStaticBox( &rect, g_pGuiMgr, NULL );
	m_pStbToolTip->CreateFontStd( DETAIL_FONT, dPATH_MANAGER_TOOLTIP_FONTSIZE, DEFAULT_FONT_ATTR );
	m_pStbToolTip->SetEffectMode(TE_SHADOW);
	m_pStbToolTip->SetEffectColor(DEFAULT_SHADOW_EFFECT_VALUE);
	m_pStbToolTip->Enable( false );
	m_pStbToolTip->Show( false );

	// Background color
	gui::CSurface surface;
	surface.m_Original.rtRect.SetRectWH( 0, 0, dPATH_MANAGER_TOOLTIP_WIDTH, dPATH_MANAGER_TOOLTIP_HEIGHT );
	surface.m_Original.uRed = 255;
	surface.m_Original.uGreen = 255;
	surface.m_Original.uBlue = 255;
	surface.m_Original.uAlpha = 255;
	surface.m_Original.uBlend = 0;
	surface.m_Original.UVs[0] = 0.0f;
	surface.m_Original.UVs[1] = 0.0f;
	surface.m_Original.UVs[2] = 1.0f;
	surface.m_Original.UVs[3] = 1.0f;

	surface.m_SnapShot.rtRect.SetRectWH( 0, 0, dPATH_MANAGER_TOOLTIP_WIDTH, dPATH_MANAGER_TOOLTIP_HEIGHT );
	surface.m_SnapShot.uRed = 0;
	surface.m_SnapShot.uGreen = 0;
	surface.m_SnapShot.uBlue = 0;
	surface.m_SnapShot.uAlpha = 50;
	surface.m_SnapShot.uBlend = 0;
	surface.m_SnapShot.UVs[0] = 0.0f;
	surface.m_SnapShot.UVs[1] = 0.0f;
	surface.m_SnapShot.UVs[2] = 1.0f;
	surface.m_SnapShot.UVs[3] = 1.0f;

	m_pStbToolTip->AddSurface( surface );

	// 라인패스를 생성
	m_pLinePath = new gui::CGuiLine();
}

void CPathManager::Destroy()
{
	if( m_pPath )
	{
		delete [] m_pPath;
		m_pPath = NULL;
	}

	if( m_pCtrlPoint )
	{
		delete [] m_pCtrlPoint;
		m_pCtrlPoint = NULL;
	}

	if( m_pStbToolTip )
	{
		delete m_pStbToolTip;
		m_pStbToolTip = NULL;
	}

	if( m_pLinePath )
	{
		delete m_pLinePath;
		m_pLinePath = NULL;
	}
}

void CPathManager::SetControlPoint( gui::SCtrlPoint* pCtrlPoint, int nCount )
{
	Destroy();
	Create( nCount );

	memcpy( m_pCtrlPoint, pCtrlPoint, sizeof( gui::SCtrlPoint ) * m_nCount );

	for( int i=0; i<m_nCount; ++i )
	{
		m_pPath[i].SetPosition( CPos( (int)m_pCtrlPoint[i].vPos.x + GetParticleApp()->GetPBOffsetX(),
			(int)m_pCtrlPoint[i].vPos.y + GetParticleApp()->GetPBOffsetY() ));
		m_pPath[i].SetTime( m_pCtrlPoint[i].fTime );
		m_pPath[i].SetNumber( i );
	}
}

void CPathManager::SetActivePathItem( gui::CValueItem* pValueItem )
{
	m_pActiveValueItem = pValueItem;

	BuildActivePathItem();
}

void CPathManager::DeActivePathItem()
{
	m_pActiveValueItem = NULL;

	if( m_pPath )
	{
		delete [] m_pPath;
		m_pPath = NULL;
	}
	
	if( m_pLinePath )
		m_pLinePath->Destroy();
}

void CPathManager::BuildActivePathItem()
{
	if( !m_pActiveValueItem )
		return;

	m_pActiveValueItem->GetValue( "path_type", m_sPathInfo.nType );
	m_pActiveValueItem->GetValue( "path_time", m_sPathInfo.fTime );
	m_pActiveValueItem->GetValue( "path_ctrlnum", m_sPathInfo.nCtrlNum );
	m_pActiveValueItem->GetValue( "path_curvesubnum", m_sPathInfo.nCurveSubNum );
	// 버퍼
	gui::SCtrlPoint* pCtrlPoint = new gui::SCtrlPoint[m_sPathInfo.nCtrlNum];

	m_pActiveValueItem->GetValue( "path_ctrlpoint", pCtrlPoint, m_sPathInfo.nCtrlNum );

	SetControlPoint( pCtrlPoint, m_sPathInfo.nCtrlNum );

	delete [] pCtrlPoint;

	CreateActivePathLine();
}

void CPathManager::UpdateCurrentPathData()
{
	if( m_pPath == NULL )
		return;

	// Destroy
	if( m_pCtrlPoint )
	{
		delete [] m_pCtrlPoint;
		m_pCtrlPoint = NULL;
	}

	// Create
	m_pCtrlPoint = new gui::SCtrlPoint[m_nCount];	

	for(int i=0; i<m_nCount; ++i )
	{
		m_pCtrlPoint[i].vPos.x = (float)m_pPath[i].GetPosition().x - GetParticleApp()->GetPBOffsetX();
		m_pCtrlPoint[i].vPos.y = (float)m_pPath[i].GetPosition().y - GetParticleApp()->GetPBOffsetY();
		m_pCtrlPoint[i].fTime = m_pPath[i].GetTime();
	}

	// Path Line을 Update한다.
	CreateActivePathLine();
}

void CPathManager::SaveCurrentPathData()
{
	// Data가 없다.
	if( m_pPath == NULL )
		return;

	CHAR acBuffer[dSAVE_BUFFER_SIZE];

	CString strData;

	for( int i=0; i<m_nCount; ++i )
	{
		sprintf_s( acBuffer, dSAVE_BUFFER_SIZE, "(%d,%d,%0.3f)", 
			m_pPath[i].GetPosition().x - GetParticleApp()->GetPBOffsetX(),
			m_pPath[i].GetPosition().y - GetParticleApp()->GetPBOffsetY(), m_pPath[i].GetTime() );

		strData += acBuffer;
		
		if( i != m_nCount-1 )
			strData += ",";
	}

	m_pActiveValueItem->AddValue( "path_ctrlpoint", strData.GetBuffer() );
}

void CPathManager::ShowToolTip( int nPosX, int nPosY, TCHAR* tcToolTip, BOOL bShow /*= TRUE */ )
{
	if( m_pStbToolTip == NULL )
		return;

	if( bShow )
	{
		m_pStbToolTip->SetPosition( nPosX+dPATH_MANAGER_TOOLTIP_OFFSET_X, nPosY+dPATH_MANAGER_TOOLTIP_OFFSET_Y );
		m_pStbToolTip->SetText( tcToolTip );
		m_pStbToolTip->Show( true );
	}
	else
	{
		m_pStbToolTip->Show( false );
	}
}

void CPathManager::SetTotalTime( float fTime )
{
	for( int i=0; i < m_nCount; ++i )
	{
		float fCurrentTime = ((float)i/ float(m_nCount-1) ) * fTime;

		m_pPath[i].SetTime( fCurrentTime );
	}
}

/**
* \brief Offset이 바뀌면 실제 데이터는 변경하지 않고 Rendering 되는 아이템들 속성만 변경한다.
*/
void CPathManager::RefreshOffsetData()
{
	if( m_pPath == NULL )
		return;

	// Control point
	for( int i=0; i<m_nCount; ++i )
	{
		int nNewX = (int)m_pCtrlPoint[i].vPos.x + GetParticleApp()->GetPBOffsetX();
		int nNewY = (int)m_pCtrlPoint[i].vPos.y + GetParticleApp()->GetPBOffsetY();

		m_pPath[i].SetPosition( CPos( nNewX, nNewY ));
		m_pPath[i].SetTime( m_pCtrlPoint[i].fTime );
		m_pPath[i].SetNumber( i );
	}

	// Curve Point
	int nCount = m_GuiPath.GetCurveCount();
	if( nCount > 0 )
	{
		gui::SCurvePoint* pCurve = m_GuiPath.GetCurvePoint();

		for( int i=0; i < nCount; ++i )
		{
			int nNewCurveX = (int)pCurve[i].vPos.x + GetParticleApp()->GetPBOffsetX();
			int nNewCurveY = (int)pCurve[i].vPos.y + GetParticleApp()->GetPBOffsetY();
			m_pLinePath->SetLineAt( i, (float)nNewCurveX, (float)nNewCurveY );
		}
	}
}

int CPathManager::GetCount()
{
	return m_nCount;
}

/**
* \brief 현재의 Active Item에 맞춘 Path의 Line을 세팅한다.
*/
void CPathManager::CreateActivePathLine()
{
	m_GuiPath.SetPath( m_sPathInfo, m_pCtrlPoint );

	int nCount = m_GuiPath.GetCurveCount();
	if( nCount > 0 )
	{
		gui::SCurvePoint* pCurve = m_GuiPath.GetCurvePoint();

		gui::SLineItem* pRenderItem = new gui::SLineItem[nCount];

		for( int i=0; i < nCount; ++i )
		{
			pRenderItem[i].x = pCurve[i].vPos.x + (float)GetParticleApp()->GetPBOffsetX();
			pRenderItem[i].y = pCurve[i].vPos.y + (float)GetParticleApp()->GetPBOffsetY();
		}

		m_pLinePath->SetLine( pRenderItem, nCount );

		delete [] pRenderItem;
	}

}

void CPathManager::DestroyActivePathLine()
{
	if( m_pLinePath )
		m_pLinePath->Destroy();
}

void CPathManager::Render()
{
	if( m_pLinePath )
		m_pLinePath->Render();
}
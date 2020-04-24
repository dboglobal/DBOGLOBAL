#include "StdAfx.h"
#include "GuideManager.h"

CGuideManager::CGuideManager(void)
{
	m_bLineBox = FALSE;
	m_lineCenter[eCENTER_LINE_HORIZONTAL].SetLine( -10000.0f, 0.0f, 10000.0f, 0.0f );
	m_lineCenter[eCENTER_LINE_HORIZONTAL].SetColor( RGB( 128, 128, 128 ) );
	m_lineCenter[eCENTER_LINE_VERTICAL].SetLine( 0.0f, -10000.0f, 0.0f, 10000.0f );
	m_lineCenter[eCENTER_LINE_VERTICAL].SetColor( RGB( 128, 128, 128 ) );

	m_fBoxPosX = 0.0f;
	m_fBoxPosY = 0.0f;
	m_fBoxWidth = 0.0f;
	m_fBoxHeight = 0.0f;
}

CGuideManager::~CGuideManager(void)
{
	for( int i=0; i<eCENTER_LINE_NUMS; ++i )
		m_lineCenter[i].Destroy();

	m_lineBox.Destroy();
	// Destroy
}

VOID CGuideManager::SetCenter( float fX, float fY )
{
	m_fCenterX = fX;
	m_fCenterY = fY;

	for( int i=0; i<eCENTER_LINE_NUMS; ++i )
	{
		m_lineCenter[eCENTER_LINE_HORIZONTAL].SetLine( -10000.0f, fY, 10000.0f, fY );
		m_lineCenter[eCENTER_LINE_HORIZONTAL].SetColor( RGB( 128, 128, 128 ) );
		m_lineCenter[eCENTER_LINE_VERTICAL].SetLine( fX, -10000.0f, fX, 10000.0f );
		m_lineCenter[eCENTER_LINE_VERTICAL].SetColor( RGB( 128, 128, 128 ) );
	}

	SetBox( m_fBoxPosX, m_fBoxPosY, m_fBoxWidth, m_fBoxHeight);
}

VOID CGuideManager::SetBox( float fX, float fY, float fWidth, float fHeight )
{
	m_fBoxPosX = fX;
	m_fBoxPosY = fY;
	m_fBoxWidth = fWidth;
	m_fBoxHeight = fHeight;

	float fOffsetX = fX + m_fCenterX;
	float fOffsetY = fY + m_fCenterY;

	gui::SLineItem sLine[5];
	sLine[0].x = fOffsetX;
	sLine[0].y = fOffsetY;

	sLine[1].x = fOffsetX;
	sLine[1].y = fOffsetY + fHeight;

	sLine[2].x = fOffsetX + fWidth;
	sLine[2].y = fOffsetY + fHeight;

	sLine[3].x = fOffsetX + fWidth;
	sLine[3].y = fOffsetY;

	sLine[4].x = fOffsetX;
	sLine[4].y = fOffsetY;

	m_lineBox.SetLine( sLine, 5 );
}

VOID CGuideManager::ShowBox( BOOL bShow )
{
	m_bLineBox = bShow;
}

VOID CGuideManager::Render( VOID )
{
	for( int i=0; i<eCENTER_LINE_NUMS; ++i )
		m_lineCenter[i].Render();

	if( m_bLineBox )
		m_lineBox.Render();
}
#ifndef _PATHPOINT_H_
#define _PATHPOINT_H_

// 패스 포인트의 Size
#define dPATH_POINT_WIDTH			15
#define dPATH_POINT_HEIGHT			15

#define dPATH_POINT_STB_WIDTH		20
#define dPATH_POINT_STB_HEIGHT		20

#define dPATH_POINT_FONT_SIZE		90

#define dPATH_POINT_TOOLTIP_SIZE	128

#pragma once

#include "gui_precomp.h"
#include "guicore_precomp.h"
#include "position.h"


class CPathPoint
{
public:
	enum ePOINT_STATE
	{
		STATE_NORMAL,
		STATE_FOCUS,
		STATE_DOWN,

		STATE_NUMS,
		INVALID_STATE = 0xFF
	};

	CPathPoint();
	CPathPoint(int nX, int nY);
	CPathPoint(CPos& pos);
	~CPathPoint();


	void				Create(int nX, int nY);
	void				Destroy();

	void				SetColor( unsigned char uRed, unsigned char uGreen, unsigned char uBlue );
	void				SetAlpha( unsigned char uAlpha );
	void				SetPosition( CPos pos );
	void				SetNumber( int nNumber );
	void				SetState( unsigned char byState );
	void				SetTime( float fTime );

	void				UpdateToolTip( BOOL bShow = TRUE );
	
	void				ClearNumber();
	
	CPos				GetPosition();
	float				GetTime();

	void				OnMouseEnter( gui::CComponent* pComponent );
	void				OnMouseLeave( gui::CComponent* pComponent );
	void				OnMouseUp( const CKey& key );
	void				OnMouseDown( const CKey& key );
	void				OnMouseMove( int nFlags, int nX, int nY );


protected:
	CPos				m_Pos;			///< 현재 점의 위치
	float				m_fTime;
	int					m_nIndex;

	gui::CPanel*		m_pPanPoint;	///< 점을 표현할 Panel
	gui::CStaticBox*	m_pStbNum;

	gui::CSlot			m_slotMouseEnter;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseMove;

	unsigned char		m_byState;

	TCHAR				m_atcToolTip[dPATH_POINT_TOOLTIP_SIZE];
};

#endif
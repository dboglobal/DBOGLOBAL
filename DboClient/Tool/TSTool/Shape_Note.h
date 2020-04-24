#pragma once


#include "Shape.h"


class CTSDoc;
class CTSView;


//////////////////////////////////////////////////////////////////////////
//
//	CShape_Note
//
//////////////////////////////////////////////////////////////////////////


class CShape_Note : public CShape
{
	DECLARE_SERIAL( CShape_Note )

// Member variables
protected:
	CString								m_strNote;

	int									m_nWidth;
	int									m_nHeight;
	int									m_nCornerWidth;
	int									m_nCornerHeight;

	CFont*								m_pTextFont;
	COLORREF							m_TextColor;
	COLORREF							m_FillColor;
	COLORREF							m_LineColor;
	COLORREF							m_ShadowColor;

// Constructions and Destructions
protected:
	CShape_Note( void )					{ SetupTheme(); }

public:
	CShape_Note( const CPoint &point );
	virtual ~CShape_Note( void )		{ return; }

// Methods
public:
	virtual CRect						GetBoundBox( void ) const;

	virtual int							GetHandlesCount( void ) const { return 4; }
	virtual CPoint						GetHandlePoint( HSHAPE handle ) const;

	virtual bool						Intersects( const CRect &rect ) const;

	virtual void						Activate( const CPoint &point );

	virtual void						Drag( const CPoint &point );

	virtual void						Render( CDC* pDC ) const;
	virtual void						RenderTracker( CDC* pDC ) const;

	virtual void						Serialize( CArchive &ar );

// Implementations
protected:
	virtual void						SetupTheme( void );

	virtual CRect						GetTextRect( void ) const;
	virtual void						EditText( void );
};

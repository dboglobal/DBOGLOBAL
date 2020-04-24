#pragma once


#include "TSToolDefine.h"


class CTSDoc;
class CTSView;


#define HANDLE_DRAG_TEXT -2


//////////////////////////////////////////////////////////////////////////
//
//	CShape
//
//////////////////////////////////////////////////////////////////////////


class CShape : public CObject
{
	DECLARE_SERIAL( CShape )

// Member variables
protected:
	// 자신의 영역
	CPoint								m_ptOrigin;

	// Dragging 관련 멤버 변수들
	bool								m_bDragging;
	CPoint								m_ptDragPoint;
	HSHAPE								m_hDragHandle;
	CPoint								m_ptLastPoint;

	// Zombie
	bool								m_bZombie;

// Constructions and Destructions
protected:
	CShape( void );

public:
	virtual ~CShape( void )			{ return; }

// Methods
public:
	virtual const CPoint&				GetCurPosition( void ) const { return m_ptOrigin; }
	virtual CRect						GetBoundBox( void ) const { return CRect(); }
	virtual CSize						GetMinSize( void ) const { return CSize(); }
	virtual CSize						GetMaxSize( void ) const { return CSize(5000, 5000); }
	virtual CSize						GetIdealSize( void ) const { return CSize(100, 100); }

	virtual HSHAPE						GetHandleAt( const CPoint &point ) const;
	virtual HCURSOR						GetHandleCursor( HSHAPE handle ) const;
	virtual int							GetHandlesCount( void ) const { return 0; }
	virtual CPoint						GetHandlePoint( HSHAPE handle ) const;

	virtual bool						Intersects( const CPoint &point ) const;
	virtual bool						Intersects( const CRect &rect ) const { return false; }

	virtual void						Activate( const CPoint &point ) { return; }
	virtual void						Update( void ) { return; }

	virtual void						BeginDrag( const CPoint &point, HSHAPE handle = -1 );
	virtual void						Drag( const CPoint &point );
	virtual void						EndDrag( void );

	virtual void						Render( CDC* pDC ) const { return; }
	virtual void						RenderTracker( CDC* pDC ) const;

	virtual void						MakeZombie( bool bTrue = true ) { m_bZombie = bTrue; }
	virtual bool						IsZombie( void ) const { return m_bZombie; }

	virtual void						OnMouseEnter( const CPoint &point, UINT nFlags ) { return; }
	virtual void						OnMouseLeave( const CPoint &point, UINT nFlags ) { return; }
	virtual HCURSOR						OnMouseMove( const CPoint &point, UINT nFlags );

	virtual void						Serialize( CArchive &ar );

// Implementations
protected:
	static CTSDoc*						GetDoc( void );
	static CTSView*						GetActiveView( void );

	virtual void						SetupTheme( void ) { return; }
};


//////////////////////////////////////////////////////////////////////////
//
//	CTypedPtrList
//
//////////////////////////////////////////////////////////////////////////


typedef CTypedPtrList<CObList, CShape*> ShapeList;

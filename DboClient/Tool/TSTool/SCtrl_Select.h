#pragma once


#include "SCtrl.h"


class CTSView;
class CShape;


/** 
	CSCtrl_Select
*/


class CSCtrl_Select : public CSCtrl
{
// Declarations
public:
	enum eSTATE { eSTATE_NONE, eSTATE_DRAG, eSTATE_DRAGHANDLE, eSTATE_NETSELECT };

// Member variables
protected:
	eSTATE								m_eState;
	CShape*								m_pShape;
	CPoint								m_ptDown;
	CPoint								m_ptLastDown;
	bool								m_bLock;

// Constructions and Destructions
public:
	CSCtrl_Select( CTSView* pView );
	virtual ~CSCtrl_Select( void )		{ return; }

// Methods
public:
	virtual void						OnLButtonDown( const CPoint& point, UINT nFlags );
	virtual void						OnLButtonUp( const CPoint& point, UINT nFlags );
	virtual void						OnMouseMove( const CPoint& point, UINT nFlags );
	virtual void						OnDoubleClick( const CPoint& point, UINT nFlags );
	virtual void						OnContextMenu( const CPoint& point );
	virtual void						OnDeactivate( void );

	virtual void						Lock( void ) { m_bLock = true; }
	virtual void						Unlock( void ) { m_bLock = false; }
	virtual bool						IsLocked() const;

// Implementation
protected:
	CPoint								CalMouseMoveClipper( const CPoint& point );
};


inline bool CSCtrl_Select::IsLocked() const
{
	return m_bLock;
}

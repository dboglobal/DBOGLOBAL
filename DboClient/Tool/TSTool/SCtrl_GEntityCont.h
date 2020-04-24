#pragma once


#include "SCtrl.h"


class CTSView;
class CShape_GEntityCont;


/** 
	CSCtrl_GEntityCont
*/


class CSCtrl_GEntityCont : public CSCtrl
{
// Member variables
protected:
	CShape_GEntityCont*					m_pShapeGEntityCont;

// Constructions and Destructions
public:
	CSCtrl_GEntityCont( CTSView* pView );
	virtual ~CSCtrl_GEntityCont( void )	{ return; }

// Methods
public:
	virtual void						OnLButtonDown( const CPoint &point, UINT nFlags );
	virtual void						OnLButtonUp( const CPoint &point, UINT nFlags );
	virtual void						OnMouseMove( const CPoint &point, UINT nFlags );
	virtual void						OnDoubleClick( const CPoint &point, UINT nFlags );
	virtual void						OnContextMenu( const CPoint &point );
	virtual void						OnDeactivate( void );

	virtual CShape_GEntityCont*			New( const CPoint& point );
};

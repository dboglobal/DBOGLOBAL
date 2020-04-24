#pragma once


#include "SCtrl.h"


class CTSView;
class CShape;
class CShape_Link;


/** 
	CSCtrl_Link
*/


class CSCtrl_Link : public CSCtrl
{
// Declarations
public:
	enum eTYPE { eTYPE_DIAGONAL, eTYPE_RECTANGULAR };

// Member variables
protected:
	eTYPE								m_eType;
	CShape_Link*						m_pShapeLink;

// Constructions and Destructions
public:
	CSCtrl_Link( CTSView* pView );
	virtual ~CSCtrl_Link( void )		{ return; }

// Methods
public:
	virtual void						OnLButtonDown( const CPoint &point, UINT nFlags );
	virtual void						OnLButtonUp( const CPoint &point, UINT nFlags );
	virtual void						OnMouseMove( const CPoint &point, UINT nFlags );
	virtual void						OnDoubleClick( const CPoint &point, UINT nFlags );
	virtual void						OnContextMenu( const CPoint &point );
	virtual void						OnDeactivate( void );

	virtual CShape_Link*				New( const CPoint& point, CShape* pShape );
};

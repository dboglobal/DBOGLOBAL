#pragma once


#include "SCtrl_Link.h"


class CTSView;


/** 
	CSCtrl_Link_Error
*/


class CSCtrl_Link_Error : public CSCtrl_Link
{
// Constructions and Destructions
public:
	CSCtrl_Link_Error( CTSView* pView );
	virtual ~CSCtrl_Link_Error( void )	{ return; }

// Methods
public:
	virtual void						OnLButtonDown( const CPoint &point, UINT nFlags );

	virtual CShape_Link*				New( const CPoint& point, CShape* pShape );
};

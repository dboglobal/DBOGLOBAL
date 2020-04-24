#pragma once


#include "SCtrl_Link.h"


class CTSView;


/** 
	CSCtrl_Link_Yes
*/


class CSCtrl_Link_Yes : public CSCtrl_Link
{
// Constructions and Destructions
public:
	CSCtrl_Link_Yes( CTSView* pView );
	virtual ~CSCtrl_Link_Yes( void )	{ return; }

// Methods
public:
	virtual void						OnLButtonDown( const CPoint &point, UINT nFlags );

	virtual CShape_Link*				New( const CPoint& point, CShape* pShape );
};

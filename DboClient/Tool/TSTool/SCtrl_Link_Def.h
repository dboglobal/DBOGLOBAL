#pragma once


#include "SCtrl_Link.h"


class CTSView;


/** 
	CSCtrl_Link_Def
*/


class CSCtrl_Link_Def : public CSCtrl_Link
{
// Constructions and Destructions
public:
	CSCtrl_Link_Def( CTSView* pView );
	virtual ~CSCtrl_Link_Def( void )	{ return; }

// Methods
public:
	virtual void						OnLButtonDown( const CPoint &point, UINT nFlags );

	virtual CShape_Link*				New( const CPoint& point, CShape* pShape );
};

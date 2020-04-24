#pragma once


#include "SCtrl_GEntityCont.h"


class CTSView;


/** 
	CSCtrl_Start
*/


class CSCtrl_Start : public CSCtrl_GEntityCont
{
// Constructions and Destructions
public:
	CSCtrl_Start( CTSView* pView );
	virtual ~CSCtrl_Start( void )	{ return; }

// Methods
public:
	virtual CShape_GEntityCont*			New( const CPoint& point );
};

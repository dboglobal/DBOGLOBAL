#pragma once


#include "SCtrl_GEntityCont.h"


class CTSView;


/** 
	CSCtrl_End
*/


class CSCtrl_End : public CSCtrl_GEntityCont
{
// Constructions and Destructions
public:
	CSCtrl_End( CTSView* pView );
	virtual ~CSCtrl_End( void )	{ return; }

// Methods
public:
	virtual CShape_GEntityCont*			New( const CPoint& point );
};

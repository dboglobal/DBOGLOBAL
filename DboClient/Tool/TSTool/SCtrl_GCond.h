#pragma once


#include "SCtrl_GEntityCont.h"


class CTSView;


/** 
	CSCtrl_GCond
*/


class CSCtrl_GCond : public CSCtrl_GEntityCont
{
// Constructions and Destructions
public:
	CSCtrl_GCond( CTSView* pView );
	virtual ~CSCtrl_GCond( void )	{ return; }

// Methods
public:
	virtual CShape_GEntityCont*			New( const CPoint& point );
};

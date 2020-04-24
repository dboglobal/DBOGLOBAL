#pragma once


#include "SCtrl_GEntityCont.h"


class CTSView;


/** 
	CSCtrl_Switch
*/


class CSCtrl_Switch : public CSCtrl_GEntityCont
{
// Constructions and Destructions
public:
	CSCtrl_Switch( CTSView* pView );
	virtual ~CSCtrl_Switch( void )	{ return; }

// Methods
public:
	virtual CShape_GEntityCont*			New( const CPoint& point );
};

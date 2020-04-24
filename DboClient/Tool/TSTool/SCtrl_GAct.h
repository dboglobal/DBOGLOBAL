#pragma once


#include "SCtrl_GEntityCont.h"


class CTSView;


/** 
	CSCtrl_GAct
*/


class CSCtrl_GAct : public CSCtrl_GEntityCont
{
// Constructions and Destructions
public:
	CSCtrl_GAct( CTSView* pView );
	virtual ~CSCtrl_GAct( void )	{ return; }

// Methods
public:
	virtual CShape_GEntityCont*			New( const CPoint& point );
};

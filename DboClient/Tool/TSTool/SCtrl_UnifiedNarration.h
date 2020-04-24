#pragma once


#include "SCtrl_GEntityCont.h"


class CTSView;


/** 
	CSCtrl_UnifiedNarration
*/


class CSCtrl_UnifiedNarration : public CSCtrl_GEntityCont
{
// Constructions and Destructions
public:
	CSCtrl_UnifiedNarration( CTSView* pView );
	virtual ~CSCtrl_UnifiedNarration( void )	{ return; }

// Methods
public:
	virtual CShape_GEntityCont*			New( const CPoint& point );
};

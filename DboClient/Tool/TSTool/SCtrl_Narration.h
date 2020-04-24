#pragma once


#include "SCtrl_GEntityCont.h"


class CTSView;


/** 
	CSCtrl_Narration
*/


class CSCtrl_Narration : public CSCtrl_GEntityCont
{
// Constructions and Destructions
public:
	CSCtrl_Narration( CTSView* pView );
	virtual ~CSCtrl_Narration( void )	{ return; }

// Methods
public:
	virtual CShape_GEntityCont*			New( const CPoint& point );
};

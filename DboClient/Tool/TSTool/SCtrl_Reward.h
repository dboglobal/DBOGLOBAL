#pragma once


#include "SCtrl_GEntityCont.h"


class CTSView;


/** 
	CSCtrl_Reward
*/


class CSCtrl_Reward : public CSCtrl_GEntityCont
{
// Constructions and Destructions
public:
	CSCtrl_Reward( CTSView* pView );
	virtual ~CSCtrl_Reward( void )	{ return; }

// Methods
public:
	virtual CShape_GEntityCont*			New( const CPoint& point );
};

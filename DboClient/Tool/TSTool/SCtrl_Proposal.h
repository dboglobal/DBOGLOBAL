#pragma once


#include "SCtrl_GEntityCont.h"


class CTSView;


/** 
	CSCtrl_Proposal
*/


class CSCtrl_Proposal : public CSCtrl_GEntityCont
{
// Constructions and Destructions
public:
	CSCtrl_Proposal( CTSView* pView );
	virtual ~CSCtrl_Proposal( void )	{ return; }

// Methods
public:
	virtual CShape_GEntityCont*			New( const CPoint& point );
};

#pragma once


#include "SCtrl_GEntityCont.h"


class CTSView;


/** 
	CSCtrl_UsrSel
*/


class CSCtrl_UsrSel : public CSCtrl_GEntityCont
{
// Constructions and Destructions
public:
	CSCtrl_UsrSel( CTSView* pView );
	virtual ~CSCtrl_UsrSel( void )	{ return; }

// Methods
public:
	virtual CShape_GEntityCont*			New( const CPoint& point );
};

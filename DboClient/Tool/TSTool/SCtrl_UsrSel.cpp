#include "stdafx.h"
#include "TSTool.h"
#include "SCtrl_UsrSel.h"
#include "Shape_UsrSel.h"


/** 
	CSCtrl_UsrSel
*/


CSCtrl_UsrSel::CSCtrl_UsrSel( CTSView* pView )
: CSCtrl_GEntityCont( pView )
{
}

CShape_GEntityCont* CSCtrl_UsrSel::New( const CPoint& point )
{
	return new CShape_UsrSel( point );
}

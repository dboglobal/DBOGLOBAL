#include "stdafx.h"
#include "TSTool.h"
#include "SCtrl_Switch.h"
#include "Shape_Switch.h"


/** 
	CSCtrl_Switch
*/


CSCtrl_Switch::CSCtrl_Switch( CTSView* pView )
: CSCtrl_GEntityCont( pView )
{
}

CShape_GEntityCont* CSCtrl_Switch::New( const CPoint& point )
{
	return new CShape_Switch( point );
}

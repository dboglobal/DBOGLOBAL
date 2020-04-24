#include "stdafx.h"
#include "TSTool.h"
#include "SCtrl_GCond.h"
#include "Shape_GCond.h"


/** 
	CSCtrl_GCond
*/


CSCtrl_GCond::CSCtrl_GCond( CTSView* pView )
: CSCtrl_GEntityCont( pView )
{
}

CShape_GEntityCont* CSCtrl_GCond::New( const CPoint& point )
{
	return new CShape_GCond( point );
}

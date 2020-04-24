#include "stdafx.h"
#include "TSTool.h"
#include "SCtrl_Start.h"
#include "Shape_Start.h"


/** 
	CSCtrl_Start
*/


CSCtrl_Start::CSCtrl_Start( CTSView* pView )
: CSCtrl_GEntityCont( pView )
{
}

CShape_GEntityCont* CSCtrl_Start::New( const CPoint& point )
{
	return new CShape_Start( point );
}

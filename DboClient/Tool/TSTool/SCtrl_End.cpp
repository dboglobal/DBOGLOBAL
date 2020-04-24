#include "stdafx.h"
#include "TSTool.h"
#include "SCtrl_End.h"
#include "Shape_End.h"


/** 
	CSCtrl_End
*/


CSCtrl_End::CSCtrl_End( CTSView* pView )
: CSCtrl_GEntityCont( pView )
{
}

CShape_GEntityCont* CSCtrl_End::New( const CPoint& point )
{
	return new CShape_End( point );
}

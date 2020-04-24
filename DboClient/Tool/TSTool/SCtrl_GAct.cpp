#include "stdafx.h"
#include "TSTool.h"
#include "SCtrl_GAct.h"
#include "Shape_GAct.h"


/** 
	CSCtrl_GAct
*/


CSCtrl_GAct::CSCtrl_GAct( CTSView* pView )
: CSCtrl_GEntityCont( pView )
{
}

CShape_GEntityCont* CSCtrl_GAct::New( const CPoint& point )
{
	return new CShape_GAct( point );
}

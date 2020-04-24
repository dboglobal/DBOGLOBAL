#include "stdafx.h"
#include "TSTool.h"
#include "SCtrl_Narration.h"
#include "Shape_Narration.h"


/** 
	CSCtrl_Narration
*/


CSCtrl_Narration::CSCtrl_Narration( CTSView* pView )
: CSCtrl_GEntityCont( pView )
{
}

CShape_GEntityCont* CSCtrl_Narration::New( const CPoint& point )
{
	return new CShape_Narration( point );
}

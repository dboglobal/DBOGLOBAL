#include "stdafx.h"
#include "TSTool.h"
#include "SCtrl_UnifiedNarration.h"
#include "Shape_UnifiedNarration.h"


/** 
	CSCtrl_UnifiedNarration
*/


CSCtrl_UnifiedNarration::CSCtrl_UnifiedNarration( CTSView* pView )
: CSCtrl_GEntityCont( pView )
{
}

CShape_GEntityCont* CSCtrl_UnifiedNarration::New( const CPoint& point )
{
	return new CShape_UnifiedNarration( point );
}

#include "stdafx.h"
#include "TSTool.h"
#include "SCtrl_Reward.h"
#include "Shape_Reward.h"


/** 
	CSCtrl_Reward
*/


CSCtrl_Reward::CSCtrl_Reward( CTSView* pView )
: CSCtrl_GEntityCont( pView )
{
}

CShape_GEntityCont* CSCtrl_Reward::New( const CPoint& point )
{
	return new CShape_Reward( point );
}

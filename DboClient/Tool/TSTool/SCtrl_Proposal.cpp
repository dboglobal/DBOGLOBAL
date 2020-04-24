#include "stdafx.h"
#include "TSTool.h"
#include "SCtrl_Proposal.h"
#include "Shape_Proposal.h"


/** 
	CSCtrl_Proposal
*/


CSCtrl_Proposal::CSCtrl_Proposal( CTSView* pView )
: CSCtrl_GEntityCont( pView )
{
}

CShape_GEntityCont* CSCtrl_Proposal::New( const CPoint& point )
{
	return new CShape_Proposal( point );
}

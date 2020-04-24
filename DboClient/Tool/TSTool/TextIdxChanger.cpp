#include "stdafx.h"
#include "TextIdxChanger.h"
#include "Attr_Page.h"


#define CONV_QUEST_TITLE( tid )			( ( tid * 100 ) + 2 )
#define CONV_QUEST_STARTDIAG( tid )		( ( tid * 100 ) + 7 )
#define CONV_QUEST_AREANAME( tid )		( ( tid * 100 ) + 1 )
#define CONV_QUEST_GOAL( tid )			( ( tid * 100 ) + 4 )
#define CONV_QUEST_SORT( tid )			( ( tid * 100 ) + 5 )
#define CONV_QUEST_CONTENTS( tid )		( ( tid * 100 ) + 8 )
#define CONV_QUEST_MARKTOOLTIP1( tid )	( ( tid * 100 ) + 15 )
#define CONV_QUEST_MARKTOOLTIP2( tid )	( ( tid * 100 ) + 16 )
#define CONV_QUEST_MARKTOOLTIP3( tid )	( ( tid * 100 ) + 17 )
#define CONV_QUEST_NPCDIAG( tid )		( ( tid * 100 ) + 9 )
#define CONV_QUEST_REWARDDIAG( tid )	( ( tid * 100 ) + 14 )


CTextIdxChanger::CTextIdxChanger( CTSTrigger* pTSTrigger )
: CChanger( pTSTrigger )
{
}

CTextIdxChanger::~CTextIdxChanger( void )
{
}

CString CTextIdxChanger::Trigger( NTL_TS_T_ID tID, CAttrData* pAttr )
{
	// Trigger
	pAttr->ChangeAttr( _T("title"), MakeString( CONV_QUEST_TITLE( tID ) ) );

	return pAttr->GetAttr();
}

CString CTextIdxChanger::Entity( NTL_TS_T_ID tID, NTL_TS_TG_ID tgID, NTL_TS_TC_ID tcID, CString strType, CAttrData* pAttr )
{
	if ( strType == _T("CT_Reward") )
	{
		pAttr->ChangeAttr( _T("desc"), MakeString( CONV_QUEST_REWARDDIAG( tID ) ) );
	}
	else if ( strType == _T("CT_Start") )
	{
		pAttr->ChangeAttr( _T("stdiag"), MakeString( CONV_QUEST_STARTDIAG( tID ) ) );
	}
	else if ( strType == _T("CT_Proposal") )
	{
		pAttr->ChangeAttr( _T("title"), MakeString( CONV_QUEST_TITLE( tID ) ) );
		pAttr->ChangeAttr( _T("area"), MakeString( CONV_QUEST_AREANAME( tID ) ) );
		pAttr->ChangeAttr( _T("goal"), MakeString( CONV_QUEST_GOAL( tID ) ) );
		pAttr->ChangeAttr( _T("sort"), MakeString( CONV_QUEST_SORT( tID ) ) );
		pAttr->ChangeAttr( _T("cont"), MakeString( CONV_QUEST_CONTENTS( tID ) ) );
	}
	else if ( strType == _T("A_NPCConv" ) )
	{
		pAttr->ChangeAttr( _T("conv"), MakeString( CONV_QUEST_NPCDIAG( tID ) ) );
	}
	else if ( strType == _T("A_RegQInfo" ) )
	{
		pAttr->ChangeAttr( _T("title"), MakeString( CONV_QUEST_TITLE( tID ) ) );
		pAttr->ChangeAttr( _T("area"), MakeString( CONV_QUEST_AREANAME( tID ) ) );
		pAttr->ChangeAttr( _T("goal"), MakeString( CONV_QUEST_GOAL( tID ) ) );
		pAttr->ChangeAttr( _T("sort"), MakeString( CONV_QUEST_SORT( tID ) ) );
		pAttr->ChangeAttr( _T("cont"), MakeString( CONV_QUEST_CONTENTS( tID ) ) );
		pAttr->ChangeAttr( _T("m0ttip"), MakeString( CONV_QUEST_MARKTOOLTIP1( tID ) ) );
		pAttr->ChangeAttr( _T("m1ttip"), MakeString( CONV_QUEST_MARKTOOLTIP2( tID ) ) );
		pAttr->ChangeAttr( _T("m2ttip"), MakeString( CONV_QUEST_MARKTOOLTIP3( tID ) ) );
	}

	return pAttr->GetAttr();
}

CString CTextIdxChanger::MakeString( unsigned int uiVal )
{
	CString strVal;
	strVal.Format( "%d", uiVal );
	return strVal;
}

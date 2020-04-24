#include "precomp_trigger.h"
#include "DboTSContProposal.h"


/**
	Proposal container
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSContProposal, CNtlTSCont )


CDboTSContProposal::CDboTSContProposal( void )
{
	m_bDspSummary = true;

	m_uiTitle = 0xffffffff;
	m_uiArea = 0xffffffff;
	m_uiGoal = 0xffffffff;
	m_uiGrade = 0xffffffff;
	m_eGradeType = eGRADE_TYPE_INVALID;
	m_uiSort = 0xffffffff;
	m_uiContents = 0xffffffff;
	m_tcReward = NTL_TS_TC_ID_INVALID;

	m_tcOk = NTL_TS_TC_ID_INVALID;
	m_tcCancel = NTL_TS_TC_ID_INVALID;
}

void CDboTSContProposal::SetDisplaySummary( bool bShow )
{
	m_bDspSummary = bShow;
}

void CDboTSContProposal::SetTitle( unsigned int uiTitle )
{
	m_uiTitle = uiTitle;
}

void CDboTSContProposal::SetArea( unsigned int uiArea )
{
	m_uiArea = uiArea;
}

void CDboTSContProposal::SetGoal( unsigned int uiGoal )
{
	m_uiGoal = uiGoal;
}

void CDboTSContProposal::SetGrade( unsigned int uiGrade )
{
	m_uiGrade = uiGrade;
}

void CDboTSContProposal::SetGradeType( eGRADE_TYPE eGradeType )
{
	m_eGradeType = eGradeType;
}

void CDboTSContProposal::SetSort( unsigned int uiSort )
{
	m_uiSort = uiSort;
}

void CDboTSContProposal::SetContents( unsigned int uiContents )
{
	m_uiContents = uiContents;
}

void CDboTSContProposal::SetReward( NTL_TS_TC_ID tcId )
{
	m_tcReward = tcId;
}

void CDboTSContProposal::SetOkLink( NTL_TS_TC_ID tcId )
{
	m_tcOk = tcId;
}

void CDboTSContProposal::SetCancelLink( NTL_TS_TC_ID tcId )
{
	m_tcCancel = tcId;
}

void CDboTSContProposal::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSCont::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "ds" ) )
	{
		SetDisplaySummary( clProperty.GetValueAsInt( "ds" ) ? true : false );
	}

	if ( clProperty.IsExist( "title" ) )
	{
		SetTitle( clProperty.GetValueAsInt( "title" ) );
	}

	if ( clProperty.IsExist( "area" ) )
	{
		SetArea( clProperty.GetValueAsInt( "area" ) );
	}

	if ( clProperty.IsExist( "goal" ) )
	{
		SetGoal( clProperty.GetValueAsInt( "goal" ) );
	}

	if ( clProperty.IsExist( "grade" ) )
	{
		SetGrade( clProperty.GetValueAsInt( "grade" ) );
	}

	if ( clProperty.IsExist( "gtype" ) )
	{
		SetGradeType( (eGRADE_TYPE)clProperty.GetValueAsInt( "gtype" ) );
	}

	if ( clProperty.IsExist( "sort" ) )
	{
		SetSort( clProperty.GetValueAsInt( "sort" ) );
	}

	if ( clProperty.IsExist( "cont" ) )
	{
		SetContents( clProperty.GetValueAsInt( "cont" ) );
	}

	if ( clProperty.IsExist( "rwd" ) )
	{
		SetReward( (NTL_TS_TC_ID)clProperty.GetValueAsInt( "rwd" ) );
	}

	if ( clProperty.IsExist( "oklnk" ) )
	{
		SetOkLink( (NTL_TS_TC_ID)clProperty.GetValueAsInt( "oklnk" ) );
	}
	if ( clProperty.IsExist( "cancellnk" ) )
	{
		SetCancelLink( (NTL_TS_TC_ID)clProperty.GetValueAsInt( "cancellnk" ) );
	}
}

void CDboTSContProposal::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSCont::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetDisplaySummary() ? 1 : 0 );
	clProperty.m_defProperty["ds"]			= g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetTitle() );
	clProperty.m_defProperty["title"]		= g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetArea() );
	clProperty.m_defProperty["area"]		= g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetGoal() );
	clProperty.m_defProperty["goal"]		= g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetGrade() );
	clProperty.m_defProperty["grade"]		= g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetGradeType() );
	clProperty.m_defProperty["gtype"]		= g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetSort() );
	clProperty.m_defProperty["sort"]		= g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetContents() );
	clProperty.m_defProperty["cont"]		= g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetReward() );
	clProperty.m_defProperty["rwd"]			= g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetOkLink() );
	clProperty.m_defProperty["oklnk"]		= g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetCancelLink() );
	clProperty.m_defProperty["cancellnk"]	= g_NtlTSString;
}
#include "precomp_trigger.h"
#include "DboTSActRegQInfo.h"


/**
	Register quest information action
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSActRegQInfo, CNtlTSAction )


CDboTSActRegQInfo::CDboTSActRegQInfo( void )
: m_uiTitle( 0xffffffff ),
  m_uiArea( 0xffffffff ),
  m_uiGoal( 0xffffffff ),
  m_uiGrade( 0xffffffff ),
  m_eGradeType( eGRADE_TYPE_INVALID ),
  m_eSort( eQUEST_SORT_TYPE_INVALID ),
  m_uiSort( 0xffffffff ),
  m_uiContents( 0xffffffff ),
  m_tcReward( NTL_TS_TC_ID_INVALID ),
  m_uim0pat( 0xffffffff ),
  m_uim1pat( 0xffffffff ),
  m_uim2pat( 0xffffffff ),
  m_uim3pat( 0xffffffff ),
  m_uim4pat( 0xffffffff ),
  m_uiscitem( 0xffffffff )
{
	memset( m_sQuestMarkInfo, 0xff, sizeof(m_sQuestMarkInfo) );
}

void CDboTSActRegQInfo::SetTitle( unsigned int uiTitle )
{
	m_uiTitle = uiTitle;
}

void CDboTSActRegQInfo::SetArea( unsigned int uiArea )
{
	m_uiArea = uiArea;
}

void CDboTSActRegQInfo::SetGoal( unsigned int uiGoal )
{
	m_uiGoal = uiGoal;
}

void CDboTSActRegQInfo::SetGrade( unsigned int uiGrade )
{
	m_uiGrade = uiGrade;
}

void CDboTSActRegQInfo::SetGradeType( eGRADE_TYPE eGradeType )
{
	m_eGradeType = eGradeType;
}

void CDboTSActRegQInfo::SetSortType( eQUEST_SORT_TYPE eQuestType )
{
	m_eSort = eQuestType;
}

void CDboTSActRegQInfo::SetSort( unsigned int uiSort )
{
	m_uiSort = uiSort;
}

void CDboTSActRegQInfo::SetContents( unsigned int uiContents )
{
	m_uiContents = uiContents;
}

void CDboTSActRegQInfo::SetReward( NTL_TS_TC_ID tcId )
{
	m_tcReward = tcId;
}

void CDboTSActRegQInfo::SetM0Pat( unsigned int uiM0Pat )
{
	m_uim0pat = uiM0Pat;
}
void CDboTSActRegQInfo::SetM1Pat( unsigned int uiM1Pat )
{
	m_uim1pat = uiM1Pat;
}
void CDboTSActRegQInfo::SetM2Pat( unsigned int uiM2Pat )
{
	m_uim2pat = uiM2Pat;
}
void CDboTSActRegQInfo::SetM3Pat( unsigned int uiM3Pat )
{
	m_uim3pat = uiM3Pat;
}
void CDboTSActRegQInfo::SetM4Pat( unsigned int uiM4Pat )
{
	m_uim4pat = uiM4Pat;
}
void CDboTSActRegQInfo::SetScItem( unsigned int uiScItem )
{
	m_uiscitem = uiScItem;
}

void CDboTSActRegQInfo::SetQuestMarkInfo( int nIdx, const SQUEST_MARK_INFO& sQuestMarkInfo )
{
	m_sQuestMarkInfo[nIdx] = sQuestMarkInfo;
}

void CDboTSActRegQInfo::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

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

	if ( clProperty.IsExist( "stype" ) )
	{
		// 스크립트에는 eQUEST_SORT_TYPE 타입들이 Bit flag로 통합되어 저장되므로
		// 분리해서 읽어들여야 한다.
		// 또한 eQUEST_SORT_TYPE 이 존재하지 않는 경우는 0 이다
		unsigned int uiSType = (unsigned int)clProperty.GetValueAsInt( "stype" );

		eQUEST_SORT_TYPE eQuestSortType = eQUEST_SORT_TYPE_INVALID;

		// 아래로 갈수록 우선 순위가 높아진다
		if ( uiSType & (1<<eQUEST_SORT_TYPE_TASK) )
		{
			eQuestSortType = eQUEST_SORT_TYPE_TASK;
		}
		if ( uiSType & (1<<eQUEST_SORT_TYPE_SCENARIO) )
		{
			eQuestSortType = eQUEST_SORT_TYPE_SCENARIO;
		}
		if ( uiSType & (1<<eQUEST_SORT_TYPE_TIMEMACHINE) )
		{
			eQuestSortType = eQUEST_SORT_TYPE_TIMEMACHINE;
		}
		if ( uiSType & (1<<eQUEST_SORT_TYPE_CHANGEJOB) )
		{
			eQuestSortType = eQUEST_SORT_TYPE_CHANGEJOB;
		}
		if ( uiSType & (1<<eQUEST_SORT_TYPE_DRAGONBALL) )
		{
			eQuestSortType = eQUEST_SORT_TYPE_DRAGONBALL;
		}
		if ( uiSType & (1<<eQUEST_SORT_TYPE_TIMELEAPQUEST) )
		{
			eQuestSortType = eQUEST_SORT_TYPE_TIMELEAPQUEST;
		}
		if ( uiSType & (1<<eQUEST_SORT_TYPE_SYSTEM) )
		{
			eQuestSortType = eQUEST_SORT_TYPE_SYSTEM;
		}
		if ( uiSType & (1<<eQUEST_SORT_TYPE_EVENT) )
		{
			eQuestSortType = eQUEST_SORT_TYPE_EVENT;
		}

		SetSortType( eQuestSortType );
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

	if ( clProperty.IsExist( "m0widx" ) )
	{
		m_sQuestMarkInfo[0].uiWorldTblIdx = clProperty.GetValueAsInt( "m0widx" );

		m_sQuestMarkInfo[0].fX = (float)atof( clProperty.GetValue( "m0fx" ).c_str() );
		m_sQuestMarkInfo[0].fY = (float)atof( clProperty.GetValue( "m0fy" ).c_str() );
		m_sQuestMarkInfo[0].fZ = (float)atof( clProperty.GetValue( "m0fz" ).c_str() );

		m_sQuestMarkInfo[0].uiTooltipIdx = clProperty.GetValueAsInt( "m0ttip" );
	}

	if ( clProperty.IsExist( "m1widx" ) )
	{
		m_sQuestMarkInfo[1].uiWorldTblIdx = clProperty.GetValueAsInt( "m1widx" );

		m_sQuestMarkInfo[1].fX = (float)atof( clProperty.GetValue( "m1fx" ).c_str() );
		m_sQuestMarkInfo[1].fY = (float)atof( clProperty.GetValue( "m1fy" ).c_str() );
		m_sQuestMarkInfo[1].fZ = (float)atof( clProperty.GetValue( "m1fz" ).c_str() );

		m_sQuestMarkInfo[1].uiTooltipIdx = clProperty.GetValueAsInt( "m1ttip" );
	}

	if ( clProperty.IsExist( "m2widx" ) )
	{
		m_sQuestMarkInfo[2].uiWorldTblIdx = clProperty.GetValueAsInt( "m2widx" );

		m_sQuestMarkInfo[2].fX = (float)atof( clProperty.GetValue( "m2fx" ).c_str() );
		m_sQuestMarkInfo[2].fY = (float)atof( clProperty.GetValue( "m2fy" ).c_str() );
		m_sQuestMarkInfo[2].fZ = (float)atof( clProperty.GetValue( "m2fz" ).c_str() );

		m_sQuestMarkInfo[2].uiTooltipIdx = clProperty.GetValueAsInt( "m2ttip" );
	}

	if ( clProperty.IsExist( "m3widx" ) )
	{
		m_sQuestMarkInfo[3].uiWorldTblIdx = clProperty.GetValueAsInt( "m3widx" );

		m_sQuestMarkInfo[3].fX = (float)atof( clProperty.GetValue( "m3fx" ).c_str() );
		m_sQuestMarkInfo[3].fY = (float)atof( clProperty.GetValue( "m3fy" ).c_str() );
		m_sQuestMarkInfo[3].fZ = (float)atof( clProperty.GetValue( "m3fz" ).c_str() );

		m_sQuestMarkInfo[3].uiTooltipIdx = clProperty.GetValueAsInt( "m3ttip" );
	}

	if ( clProperty.IsExist( "m4widx" ) )
	{
		m_sQuestMarkInfo[4].uiWorldTblIdx = clProperty.GetValueAsInt( "m4widx" );

		m_sQuestMarkInfo[4].fX = (float)atof( clProperty.GetValue( "m4fx" ).c_str() );
		m_sQuestMarkInfo[4].fY = (float)atof( clProperty.GetValue( "m4fy" ).c_str() );
		m_sQuestMarkInfo[4].fZ = (float)atof( clProperty.GetValue( "m4fz" ).c_str() );

		m_sQuestMarkInfo[4].uiTooltipIdx = clProperty.GetValueAsInt( "m4ttip" );
	}

	if ( clProperty.IsExist( "m0pat" ) )
	{
		SetM0Pat( clProperty.GetValueAsInt( "m0pat" ) );
	}
	if ( clProperty.IsExist( "m1pat" ) )
	{
		SetM1Pat( clProperty.GetValueAsInt( "m1pat" ) );
	}
	if ( clProperty.IsExist( "m2pat" ) )
	{
		SetM2Pat( clProperty.GetValueAsInt( "m2pat" ) );
	}
	if ( clProperty.IsExist( "m3pat" ) )
	{
		SetM3Pat( clProperty.GetValueAsInt( "m3pat" ) );
	}
	if ( clProperty.IsExist( "m4pat" ) )
	{
		SetM4Pat( clProperty.GetValueAsInt( "m4pat" ) );
	}
	if ( clProperty.IsExist( "scitem" ) )
	{
		SetScItem( clProperty.GetValueAsInt( "scitem" ) );
	}
}

void CDboTSActRegQInfo::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

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

	// 스크립트에는 eQUEST_SORT_TYPE 타입들이 Bit flag로 통합되어 저장되므로
	// 통합해서 저장해야한다.
	// 또한 eQUEST_SORT_TYPE 이 존재하지 않는 경우는 0 이다
	if ( GetSortType() == eQUEST_SORT_TYPE_INVALID )
	{
		sprintf_s( g_NtlTSString, "%d", 0 );
		clProperty.m_defProperty["stype"]	= g_NtlTSString;
	}
	else
	{
		sprintf_s( g_NtlTSString, "%d", (1<<GetSortType()) );
		clProperty.m_defProperty["stype"]	= g_NtlTSString;
	}

	sprintf_s( g_NtlTSString, "%d", GetSort() );
	clProperty.m_defProperty["sort"]		= g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetContents() );
	clProperty.m_defProperty["cont"]		= g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetReward() );
	clProperty.m_defProperty["rwd"]			= g_NtlTSString;

	if ( 0xffffffff != m_sQuestMarkInfo[0].uiWorldTblIdx )
	{
		sprintf_s( g_NtlTSString, "%d", m_sQuestMarkInfo[0].uiWorldTblIdx );
		clProperty.m_defProperty["m0widx"]	= g_NtlTSString;

		sprintf_s( g_NtlTSString, "%f", m_sQuestMarkInfo[0].fX );
		clProperty.m_defProperty["m0fx"]	= g_NtlTSString;

		sprintf_s( g_NtlTSString, "%f", m_sQuestMarkInfo[0].fY );
		clProperty.m_defProperty["m0fy"]	= g_NtlTSString;

		sprintf_s( g_NtlTSString, "%f", m_sQuestMarkInfo[0].fZ );
		clProperty.m_defProperty["m0fz"]	= g_NtlTSString;

		sprintf_s( g_NtlTSString, "%d", m_sQuestMarkInfo[0].uiTooltipIdx );
		clProperty.m_defProperty["m0ttip"]	= g_NtlTSString;
	}

	if ( 0xffffffff != m_sQuestMarkInfo[1].uiWorldTblIdx )
	{
		sprintf_s( g_NtlTSString, "%d", m_sQuestMarkInfo[1].uiWorldTblIdx );
		clProperty.m_defProperty["m1widx"]	= g_NtlTSString;

		sprintf_s( g_NtlTSString, "%f", m_sQuestMarkInfo[1].fX );
		clProperty.m_defProperty["m1fx"]	= g_NtlTSString;

		sprintf_s( g_NtlTSString, "%f", m_sQuestMarkInfo[1].fY );
		clProperty.m_defProperty["m1fy"]	= g_NtlTSString;

		sprintf_s( g_NtlTSString, "%f", m_sQuestMarkInfo[1].fZ );
		clProperty.m_defProperty["m1fz"]	= g_NtlTSString;

		sprintf_s( g_NtlTSString, "%d", m_sQuestMarkInfo[1].uiTooltipIdx );
		clProperty.m_defProperty["m1ttip"]	= g_NtlTSString;
	}

	if ( 0xffffffff != m_sQuestMarkInfo[2].uiWorldTblIdx )
	{
		sprintf_s( g_NtlTSString, "%d", m_sQuestMarkInfo[2].uiWorldTblIdx );
		clProperty.m_defProperty["m2widx"]	= g_NtlTSString;

		sprintf_s( g_NtlTSString, "%f", m_sQuestMarkInfo[2].fX );
		clProperty.m_defProperty["m2fx"]	= g_NtlTSString;

		sprintf_s( g_NtlTSString, "%f", m_sQuestMarkInfo[2].fY );
		clProperty.m_defProperty["m2fy"]	= g_NtlTSString;

		sprintf_s( g_NtlTSString, "%f", m_sQuestMarkInfo[2].fZ );
		clProperty.m_defProperty["m2fz"]	= g_NtlTSString;

		sprintf_s( g_NtlTSString, "%d", m_sQuestMarkInfo[2].uiTooltipIdx );
		clProperty.m_defProperty["m2ttip"]	= g_NtlTSString;
	}

	if ( 0xffffffff != m_sQuestMarkInfo[3].uiWorldTblIdx )
	{
		sprintf_s( g_NtlTSString, "%d", m_sQuestMarkInfo[3].uiWorldTblIdx );
		clProperty.m_defProperty["m3widx"]	= g_NtlTSString;

		sprintf_s( g_NtlTSString, "%f", m_sQuestMarkInfo[3].fX );
		clProperty.m_defProperty["m3fx"]	= g_NtlTSString;

		sprintf_s( g_NtlTSString, "%f", m_sQuestMarkInfo[3].fY );
		clProperty.m_defProperty["m3fy"]	= g_NtlTSString;

		sprintf_s( g_NtlTSString, "%f", m_sQuestMarkInfo[3].fZ );
		clProperty.m_defProperty["m3fz"]	= g_NtlTSString;

		sprintf_s( g_NtlTSString, "%d", m_sQuestMarkInfo[3].uiTooltipIdx );
		clProperty.m_defProperty["m3ttip"]	= g_NtlTSString;
	}

	if ( 0xffffffff != m_sQuestMarkInfo[4].uiWorldTblIdx )
	{
		sprintf_s( g_NtlTSString, "%d", m_sQuestMarkInfo[4].uiWorldTblIdx );
		clProperty.m_defProperty["m4widx"]	= g_NtlTSString;

		sprintf_s( g_NtlTSString, "%f", m_sQuestMarkInfo[4].fX );
		clProperty.m_defProperty["m4fx"]	= g_NtlTSString;

		sprintf_s( g_NtlTSString, "%f", m_sQuestMarkInfo[4].fY );
		clProperty.m_defProperty["m4fy"]	= g_NtlTSString;

		sprintf_s( g_NtlTSString, "%f", m_sQuestMarkInfo[4].fZ );
		clProperty.m_defProperty["m4fz"]	= g_NtlTSString;

		sprintf_s( g_NtlTSString, "%d", m_sQuestMarkInfo[4].uiTooltipIdx );
		clProperty.m_defProperty["m4ttip"]	= g_NtlTSString;
	}

	sprintf_s( g_NtlTSString, "%d", GetM0Pat() );
	clProperty.m_defProperty["m0pat"]			= g_NtlTSString;
	sprintf_s( g_NtlTSString, "%d", GetM1Pat() );
	clProperty.m_defProperty["m1pat"]			= g_NtlTSString;
	sprintf_s( g_NtlTSString, "%d", GetM2Pat() );
	clProperty.m_defProperty["m2pat"]			= g_NtlTSString;
	sprintf_s( g_NtlTSString, "%d", GetM3Pat() );
	clProperty.m_defProperty["m3pat"]			= g_NtlTSString;
	sprintf_s( g_NtlTSString, "%d", GetM4Pat() );
	clProperty.m_defProperty["m4pat"]			= g_NtlTSString;
	sprintf_s( g_NtlTSString, "%d", GetScItem() );
	clProperty.m_defProperty["scitem"]			= g_NtlTSString;
}

#include "precomp_trigger.h"
#include "DboTSActExcCGroup.h"


/**
	Execute Client Group
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSActExcCGroup, CNtlTSAction )


CDboTSActExcCGroup::CDboTSActExcCGroup( void )
: m_bReg( true ),
  m_tgExcCGID( NTL_TS_TG_ID_INVALID )
{
}

void CDboTSActExcCGroup::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "rtype" ) )
	{
		m_bReg = clProperty.GetValueAsInt( "rtype" ) ? true : false;
	}

	if ( clProperty.IsExist( "gid" ) )
	{
		m_tgExcCGID = (NTL_TS_TC_ID)clProperty.GetValueAsInt( "gid" );
	}
}

void CDboTSActExcCGroup::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", m_bReg ? 1 : 0 );
	clProperty.m_defProperty["rtype"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", m_tgExcCGID );
	clProperty.m_defProperty["gid"] = g_NtlTSString;
}

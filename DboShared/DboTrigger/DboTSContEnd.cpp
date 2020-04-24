#include "precomp_trigger.h"
#include "DboTSContEnd.h"


/**
	End container
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSContEnd, CDboTSContGAct )


CDboTSContEnd::CDboTSContEnd( void )
: m_eEndType( eEND_TYPE_NOT_PROGRESS )
{
}

void CDboTSContEnd::SetEndType( eEND_TYPE eEndType )
{
	m_eEndType = eEndType;
}

void CDboTSContEnd::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSCont::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "type" ) )
	{
		SetEndType( (eEND_TYPE)clProperty.GetValueAsInt( "type" ) );
	}
}

void CDboTSContEnd::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSCont::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetEndType() );
	clProperty.m_defProperty["type"] = g_NtlTSString;
}
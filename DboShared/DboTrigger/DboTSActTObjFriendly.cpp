#include "precomp_trigger.h"
#include "DboTSActTObjFriendly.h"


/**
	Trigger object friendly
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSActTObjFriendly, CNtlTSAction )


CDboTSActTObjFriendly::CDboTSActTObjFriendly( void )
: m_uiFriendlyFlags( 0 )
{
}

void CDboTSActTObjFriendly::SetFriendlyFlags( unsigned int uiFlags )
{
	m_uiFriendlyFlags = uiFlags;
}

void CDboTSActTObjFriendly::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "flgs" ) )
	{
		SetFriendlyFlags( (unsigned int)clProperty.GetValueAsInt( "flgs" ) );
	}
}

void CDboTSActTObjFriendly::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetFriendlyFlags() );
	clProperty.m_defProperty["flgs"] = g_NtlTSString;
}

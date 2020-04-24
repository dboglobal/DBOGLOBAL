#include "precomp_trigger.h"
#include "DboTSActMiniNarration.h"


/**
	Mini narration
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSActMiniNarration, CNtlTSAction )


CDboTSActMiniNarration::CDboTSActMiniNarration( void )
: m_uiNarrationTblIdx( 0xffffffff )
{
}

void CDboTSActMiniNarration::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "tidx" ) )
	{
		m_uiNarrationTblIdx = clProperty.GetValueAsInt( "tidx" );
	}
}

void CDboTSActMiniNarration::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", m_uiNarrationTblIdx );
	clProperty.m_defProperty["tidx"] = g_NtlTSString;
}

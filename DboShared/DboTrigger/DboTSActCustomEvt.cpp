#include "precomp_trigger.h"
#include "DboTSActCustomEvt.h"


/**
	Custom event action
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSActCustomEvt, CNtlTSAction )


CDboTSActCustomEvt::CDboTSActCustomEvt( void )
: m_uiCustomEvtID( 0xffffffff )
{
}

void CDboTSActCustomEvt::SetCustomEvtID( unsigned int uiCustomEvtID )
{
	m_uiCustomEvtID = uiCustomEvtID;
}

void CDboTSActCustomEvt::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "ceid" ) )
	{
		SetCustomEvtID( (unsigned int)clProperty.GetValueAsInt( "ceid" ) );
	}
}

void CDboTSActCustomEvt::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetCustomEvtID() );
	clProperty.m_defProperty["ceid"] = g_NtlTSString;
}

#include "precomp_trigger.h"
#include "DboTSClickSideIcon.h"


/**
	click sideicon event
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSClickSideIcon, CNtlTSEvent )


CDboTSClickSideIcon::CDboTSClickSideIcon( void )
: m_bySideIconType( 0xff )
{
}



void CDboTSClickSideIcon::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "sitype" ) )
	{
		m_bySideIconType = (unsigned char)clProperty.GetValueAsInt( "sitype" );
	}

}

void CDboTSClickSideIcon::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", m_bySideIconType );
	clProperty.m_defProperty["sitype"] = g_NtlTSString;
}


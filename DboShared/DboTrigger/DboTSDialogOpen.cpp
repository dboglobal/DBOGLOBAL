#include "precomp_trigger.h"
#include "DboTSDialogOpen.h"


/**
	DIALOG OPEN
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSDialogOpen, CNtlTSEvent )


CDboTSDialogOpen::CDboTSDialogOpen( void )
: m_byDialogOpenType( 0xff )
{
}



void CDboTSDialogOpen::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "dotype" ) )
	{
		m_byDialogOpenType = (unsigned char)clProperty.GetValueAsInt( "dotype" );
	}

}

void CDboTSDialogOpen::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", m_byDialogOpenType );
	clProperty.m_defProperty["dotype"] = g_NtlTSString;
}


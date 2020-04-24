#include "precomp_trigger.h"
#include "DboTSActOpenInventory.h"


NTL_TS_IMPLEMENT_RTTI( CDboTSActOpenInventory, CNtlTSAction )


CDboTSActOpenInventory::CDboTSActOpenInventory( void )
: m_bShow( true )
{
}

void CDboTSActOpenInventory::SetShow( bool bShow )
{
	m_bShow = bShow;
}

void CDboTSActOpenInventory::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "show" ) )
	{
		SetShow( (bool)clProperty.GetValueAsInt( "show" ) );
	}
}

void CDboTSActOpenInventory::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetShow() );
	clProperty.m_defProperty["show"] = g_NtlTSString;
}

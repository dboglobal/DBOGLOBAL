#include "precomp_trigger.h"
#include "DboTSSlotMachine.h"


/**
	Slot machine
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSSlotMachine, CNtlTSEvent )


CDboTSSlotMachine::CDboTSSlotMachine( void )
{
}

void CDboTSSlotMachine::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "st" ) )
	{
		m_iSt = clProperty.GetValueAsInt( "st" );
	}

}

void CDboTSSlotMachine::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", m_iSt );
	clProperty.m_defProperty["st"] = g_NtlTSString;
}

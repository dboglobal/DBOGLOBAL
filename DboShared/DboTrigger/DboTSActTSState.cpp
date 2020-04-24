#include "precomp_trigger.h"
#include "DboTSActTSState.h"


/**
	TS state update action
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSActTSState, CNtlTSAction )


CDboTSActTSState::CDboTSActTSState( void )
: m_eStateType( eTSSTATE_TYPE_ADD ),
  m_wTSState( 0 )
{
}

void CDboTSActTSState::SetType( eTSSTATE_TYPE eType )
{
	m_eStateType = eType;
}

void CDboTSActTSState::SetTSState( unsigned short wTSState )
{
	m_wTSState = wTSState;
}

void CDboTSActTSState::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "type" ) )
	{
		SetType( (eTSSTATE_TYPE)clProperty.GetValueAsInt( "type" ) );
	}

	if ( clProperty.IsExist( "state" ) )
	{
		SetTSState( (unsigned short)clProperty.GetValueAsInt( "state" ) );
	}
}

void CDboTSActTSState::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetType() );
	clProperty.m_defProperty["type"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetTSState() );
	clProperty.m_defProperty["state"] = g_NtlTSString;
}

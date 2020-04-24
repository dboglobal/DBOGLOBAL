#include "precomp_trigger.h"
#include "DboTSCheckObjState.h"


/**
	Check level condition
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSCheckObjState, CNtlTSCond )


CDboTSCheckObjState::CDboTSCheckObjState( void )
{
	m_uiWorldIdx = 0xffffffff;
	m_uiObjIdx = 0xffffffff;
	m_eObjMainState = eMAIN_STATE_IGNORE;
	m_eObjectShowHide = SUB_STATE_OBJECT_IGNORE;
}

void CDboTSCheckObjState::SetWorldIdx( unsigned int uiWorldIdx )
{
	m_uiWorldIdx = uiWorldIdx;
}

void CDboTSCheckObjState::SetObjectIdx( unsigned int uiObjectIdx )
{
	m_uiObjIdx = uiObjectIdx;
}

void CDboTSCheckObjState::SetObjectMainState( CDboTSCheckObjState::eMAIN_STATE eObjectMainState )
{
	m_eObjMainState = eObjectMainState;
}

void CDboTSCheckObjState::SetObjectShowHide( CDboTSCheckObjState::eSUB_STATE_OBJECT eObjectShowHide )
{
	m_eObjectShowHide = eObjectShowHide;
}

void CDboTSCheckObjState::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "widx" ) )
	{
		SetWorldIdx( (unsigned int)clProperty.GetValueAsInt( "widx" ) );
	}

	if ( clProperty.IsExist( "oidx" ) )
	{
		SetObjectIdx( (unsigned int)clProperty.GetValueAsInt( "oidx" ) );
	}

	if ( clProperty.IsExist( "mstate" ) )
	{
		SetObjectMainState( (eMAIN_STATE)clProperty.GetValueAsInt( "mstate" ) );
	}

	if ( clProperty.IsExist( "osh" ) )
	{
		SetObjectShowHide( (eSUB_STATE_OBJECT)clProperty.GetValueAsInt( "osh" ) );
	}
}

void CDboTSCheckObjState::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetWorldIdx() );
	clProperty.m_defProperty["widx"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetObjectIdx() );
	clProperty.m_defProperty["oidx"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetObjectMainState() );
	clProperty.m_defProperty["mstate"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetObjectShowHide() );
	clProperty.m_defProperty["osh"] = g_NtlTSString;
}

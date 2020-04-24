#include "precomp_trigger.h"
#include "DboTSActObjState.h"


/**
	Object state
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSActObjState, CNtlTSAction )


CDboTSActObjState::CDboTSActObjState( void )
{
	m_uiWorldIdx = 0xffffffff;
	m_uiObjIdx = 0xffffffff;
	m_eObjMainState = eMAIN_STATE_IGNORE;
	m_eObjectShowHide = SUB_STATE_OBJECT_SH_IGNORE;
	m_eObjectCheckUncheck = eSUB_STATE_OBJECT_UC_IGNORE;
}

void CDboTSActObjState::SetWorldIdx( unsigned int uiWorldIdx )
{
	m_uiWorldIdx = uiWorldIdx;
}

void CDboTSActObjState::SetObjectIdx( unsigned int uiObjectIdx )
{
	m_uiObjIdx = uiObjectIdx;
}

void CDboTSActObjState::SetObjectMainState( CDboTSActObjState::eMAIN_STATE eObjectMainState )
{
	m_eObjMainState = eObjectMainState;
}

void CDboTSActObjState::SetObjectShowHide( CDboTSActObjState::eSUB_STATE_OBJECT_SH eObjectShowHide )
{
	m_eObjectShowHide = eObjectShowHide;
}

void CDboTSActObjState::SetObjectCheckUncheck( CDboTSActObjState::eSUB_STATE_OBJECT_UC eObjectCheckUncheck )
{
	m_eObjectCheckUncheck = eObjectCheckUncheck;
}

void CDboTSActObjState::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

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
		unsigned char byVal = (unsigned char)clProperty.GetValueAsInt( "osh" );

		switch ( byVal )
		{
		case SUB_STATE_OBJECT_SH_HIDE:
			SetObjectShowHide( SUB_STATE_OBJECT_SH_HIDE );
			break;
		case SUB_STATE_OBJECT_SH_SHOW:
			SetObjectShowHide( SUB_STATE_OBJECT_SH_HIDE );
			break;
		case SUB_STATE_OBJECT_SH_IGNORE:
			SetObjectShowHide( SUB_STATE_OBJECT_SH_IGNORE );
			break;
		}

		SetObjectCheckUncheck( eSUB_STATE_OBJECT_UC_IGNORE );
	}

	if ( clProperty.IsExist( "osh_sh" ) )
	{
		SetObjectShowHide( (eSUB_STATE_OBJECT_SH)clProperty.GetValueAsInt( "osh_sh" ) );
	}

	if ( clProperty.IsExist( "osh_uc" ) )
	{
		SetObjectCheckUncheck( (eSUB_STATE_OBJECT_UC)clProperty.GetValueAsInt( "osh_uc" ) );
	}
}

void CDboTSActObjState::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetWorldIdx() );
	clProperty.m_defProperty["widx"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetObjectIdx() );
	clProperty.m_defProperty["oidx"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetObjectMainState() );
	clProperty.m_defProperty["mstate"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetObjectShowHide() );
	clProperty.m_defProperty["osh_sh"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetObjectCheckUncheck() );
	clProperty.m_defProperty["osh_uc"] = g_NtlTSString;
}

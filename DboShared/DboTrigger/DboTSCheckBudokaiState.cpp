#include "precomp_trigger.h"
#include "DboTSCheckBudokaiState.h"
#include "NtlBudokai.h"


/**
	Check Budokai state condition
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSCheckBudokaiState, CNtlTSCond )


CDboTSCheckBudokaiState::CDboTSCheckBudokaiState( void )
: m_eMode( eTS_BUDOKAI_MODE_ALL ),
  m_eState( eTS_BUDOKAI_STATE_OPEN )
{
}

void CDboTSCheckBudokaiState::SetMode( eTS_BUDOKAI_MODE eMode )
{
	m_eMode = eMode;
}

void CDboTSCheckBudokaiState::SetState( eTS_BUDOKAI_STATE eState )
{
	m_eState = eState;
}

bool CDboTSCheckBudokaiState::IsConditionYes( unsigned char byState /*eBUDOKAI_STATE*/ )
{
	if ( eTS_BUDOKAI_STATE_OPEN == m_eState )
	{
		if ( eTS_BUDOKAI_MODE_ALL == m_eMode )
		{
			if ( true == IsBudokaiOpen( byState) )
			{
				return true;
			}
		}
		else if ( eTS_BUDOKAI_MODE_ADULT == m_eMode )
		{
			if ( BUDOKAI_TYPE_ADULT == GetBudokaiType( byState ) &&
				 true == IsBudokaiOpen( byState) )
			{
				return true;
			}
		}
		else if ( eTS_BUDOKAI_MODE_CHILD == m_eMode )
		{
			if ( BUDOKAI_TYPE_JUNIOR == GetBudokaiType( byState ) &&
				 true == IsBudokaiOpen( byState) )
			{
				return true;
			}
		}
	}
	else if ( eTS_BUDOKAI_STATE_CLOSE == m_eState )
	{
		if ( eTS_BUDOKAI_MODE_ALL == m_eMode )
		{
			if ( false == IsBudokaiOpen( byState) )
			{
				return true;
			}
		}
		else if ( eTS_BUDOKAI_MODE_ADULT == m_eMode )
		{
			if ( BUDOKAI_TYPE_ADULT == GetBudokaiType( byState ) &&
				 false == IsBudokaiOpen( byState) )
			{
				return true;
			}
		}
		else if ( eTS_BUDOKAI_MODE_CHILD == m_eMode )
		{
			if ( BUDOKAI_TYPE_JUNIOR == GetBudokaiType( byState ) &&
				 false == IsBudokaiOpen( byState) )
			{
				return true;
			}
		}
	}

	return false;
}

void CDboTSCheckBudokaiState::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "md" ) )
	{
		SetMode( (eTS_BUDOKAI_MODE)clProperty.GetValueAsInt( "md" ) );
	}
	if ( clProperty.IsExist( "st" ) )
	{
		SetState( (eTS_BUDOKAI_STATE)clProperty.GetValueAsInt( "st" ) );
	}
}

void CDboTSCheckBudokaiState::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetMode() );
	clProperty.m_defProperty["md"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetState() );
	clProperty.m_defProperty["st"] = g_NtlTSString;
}

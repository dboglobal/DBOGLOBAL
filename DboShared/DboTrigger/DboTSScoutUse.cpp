#include "precomp_trigger.h"
#include "DboTSScoutUse.h"


NTL_TS_IMPLEMENT_RTTI( CDboTSScoutUse, CNtlTSEvent )


CDboTSScoutUse::CDboTSScoutUse( void )
: m_uiItemIdx( 0xffffffff ),
  m_eTargetType( eEVENT_SCOUTER_TARGET_TYPE_ALL ),
  m_uiTargetIdx( 0xffffffff )
{
}

void CDboTSScoutUse::SetItemIdx( unsigned int uiItemIdx )
{
	m_uiItemIdx = uiItemIdx;
}

void CDboTSScoutUse::SetTargetType( eEVENT_SCOUTER_TARGET_TYPE eTargetType )
{
	m_eTargetType = eTargetType;
}

void CDboTSScoutUse::SetTargetIdx( unsigned int uiTargetIdx )
{
	m_uiTargetIdx = uiTargetIdx;
}

void CDboTSScoutUse::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "idx" ) )
	{
		SetItemIdx( clProperty.GetValueAsInt( "idx" ) );
	}

	if ( clProperty.IsExist( "tt" ) )
	{
		SetTargetType( (eEVENT_SCOUTER_TARGET_TYPE)clProperty.GetValueAsInt( "tt" ) );
	}

	if ( clProperty.IsExist( "tidx" ) )
	{
		SetTargetIdx( clProperty.GetValueAsInt( "tidx" ) );
	}
}

void CDboTSScoutUse::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetItemIdx() );
	clProperty.m_defProperty["idx"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetTargetType() );
	clProperty.m_defProperty["tt"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetTargetIdx() );
	clProperty.m_defProperty["tidx"] = g_NtlTSString;
}

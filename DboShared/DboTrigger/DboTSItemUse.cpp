#include "precomp_trigger.h"
#include "DboTSItemUse.h"


NTL_TS_IMPLEMENT_RTTI( CDboTSItemUse, CNtlTSEvent )


CDboTSItemUse::CDboTSItemUse( void )
: m_uiItemIdx( 0xffffffff ),
  m_eItemTargetType( eEVENT_ITEM_TARGET_TYPE_ALL ),
  m_uiItemTargetIdx( 0xffffffff )
{
}

void CDboTSItemUse::SetItemIdx( unsigned int uiItemIdx )
{
	m_uiItemIdx = uiItemIdx;
}

void CDboTSItemUse::SetItemTargetType( eEVENT_ITEM_TARGET_TYPE eItemTargetType )
{
	m_eItemTargetType = eItemTargetType;
}

void CDboTSItemUse::SetItemTargetIdx( unsigned int uiItemTargetIdx )
{
	m_uiItemTargetIdx = uiItemTargetIdx;
}

void CDboTSItemUse::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "idx" ) )
	{
		SetItemIdx( clProperty.GetValueAsInt( "idx" ) );
	}

	if ( clProperty.IsExist( "ttype" ) )
	{
		SetItemTargetType( (eEVENT_ITEM_TARGET_TYPE)clProperty.GetValueAsInt( "ttype" ) );
	}

	if ( clProperty.IsExist( "tidx" ) )
	{
		SetItemTargetIdx( clProperty.GetValueAsInt( "tidx" ) );
	}
}

void CDboTSItemUse::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetItemIdx() );
	clProperty.m_defProperty["idx"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetItemTargetType() );
	clProperty.m_defProperty["ttype"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetItemTargetIdx() );
	clProperty.m_defProperty["tidx"] = g_NtlTSString;
}

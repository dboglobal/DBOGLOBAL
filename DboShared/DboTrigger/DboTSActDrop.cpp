#include "precomp_trigger.h"
#include "DboTSActDrop.h"


NTL_TS_IMPLEMENT_RTTI( CDboTSActDrop, CNtlTSAction )


CDboTSActDrop::CDboTSActDrop( void )
: m_ePermissionType( eQUEST_DROP_PERMISSION_TYPE_PARTY ),
  m_uiProbDropTblIdx( 0xffffffff )
{
}

void CDboTSActDrop::SetPermissionType( eQUEST_DROP_PERMISSION_TYPE eType )
{
	m_ePermissionType = eType;
}

void CDboTSActDrop::SetTableIndex( unsigned int uiProbDropTblIdx )
{
	m_uiProbDropTblIdx = uiProbDropTblIdx;
}

void CDboTSActDrop::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "ptype" ) )
	{
		SetPermissionType( (eQUEST_DROP_PERMISSION_TYPE)clProperty.GetValueAsInt( "ptype" ) );
	}

	if ( clProperty.IsExist( "tblidx" ) )
	{
		SetTableIndex( clProperty.GetValueAsInt( "tblidx" ) );
	}
}

void CDboTSActDrop::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetPermissionType() );
	clProperty.m_defProperty["ptype"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetTableIndex() );
	clProperty.m_defProperty["tblidx"] = g_NtlTSString;
}

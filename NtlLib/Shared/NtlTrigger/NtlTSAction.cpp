#include "precomp_trigger.h"
#include "NtlTSAction.h"
#include "NtlTSCoreStatic.h"

/**
	Action entity
*/


NTL_TS_IMPLEMENT_RTTI( CNtlTSAction, CNtlTSEntity)


void CNtlTSAction::SetActionId( NTL_TS_TA_ID taId )
{
	m_taId = taId;
}

void CNtlTSAction::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSEntity::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "taid" ) )
	{
		SetActionId( (NTL_TS_TA_ID)clProperty.GetValueAsInt( "taid" ) );
	}
}

void CNtlTSAction::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSEntity::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetActionId() );
	clProperty.m_defProperty["taid"] = g_NtlTSString;
}
#include "precomp_trigger.h"
#include "DboTSCheckObjItem.h"


/**
	Check obejct item condition
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSCheckObjItem, CNtlTSCond )


CDboTSCheckObjItem::CDboTSCheckObjItem( void )
{
	m_tQuestId = NTL_TS_T_ID_INVALID;
}

void CDboTSCheckObjItem::SetQuestId( NTL_TS_T_ID tId )
{
	m_tQuestId = tId;
}

void CDboTSCheckObjItem::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "tid" ) )
	{
		SetQuestId( (NTL_TS_T_ID)clProperty.GetValueAsInt( "tid" ) );
	}
}

void CDboTSCheckObjItem::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetQuestId() );
	clProperty.m_defProperty["tid"] = g_NtlTSString;
}
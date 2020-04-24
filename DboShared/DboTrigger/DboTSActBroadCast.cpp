#include "precomp_trigger.h"
#include "DboTSActBroadCast.h"


NTL_TS_IMPLEMENT_RTTI( CDboTSActBroadCast, CNtlTSAction )


CDboTSActBroadCast::CDboTSActBroadCast( void )
: m_uiNPCTblIdx( 0xffffffff ),
  m_byMessageType( 0xff ),
  m_uiSpeechTblIdx( 0xffffffff )
{
}

void CDboTSActBroadCast::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "npcidx" ) )
	{
		SetNPCTableIndex( clProperty.GetValueAsInt( "npcidx" ) );
	}

	if ( clProperty.IsExist( "msgtype" ) )
	{
		SetMessageType( clProperty.GetValueAsInt( "msgtype" ) );
	}

	if ( clProperty.IsExist( "speechidx" ) )
	{
		SetSpeechTableIndex( clProperty.GetValueAsInt( "speechidx" ) );
	}
}

void CDboTSActBroadCast::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetNPCTableIndex() );
	clProperty.m_defProperty["npcidx"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetMessageType() );
	clProperty.m_defProperty["msgtype"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetSpeechTableIndex() );
	clProperty.m_defProperty["speechidx"] = g_NtlTSString;
}

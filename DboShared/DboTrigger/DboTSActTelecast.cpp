#include "precomp_trigger.h"
#include "DboTSActTelecast.h"


NTL_TS_IMPLEMENT_RTTI( CDboTSActTelecast, CNtlTSAction )


CDboTSActTelecast::CDboTSActTelecast( void )
: m_eType( eTSTELECAST_TYPE_TIME ),
  m_uiNPCTblIdx( 0xffffffff ),
  m_byMessageType( 0xff ),
  m_uiSpeechTblIdx( 0xffffffff ),
  m_uiDisplayTime( 0 )
{
}

void CDboTSActTelecast::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "type" ) )
	{
		SetTelecastType( (eTSTELECAST_TYPE)clProperty.GetValueAsInt( "type" ) );
	}

	if ( clProperty.IsExist( "npcidx" ) )
	{
		SetNPCTableIndex( clProperty.GetValueAsInt( "npcidx" ) );
	}

	if ( clProperty.IsExist( "msgtype" ) )
	{
		SetMessageType((UCHAR)clProperty.GetValueAsInt( "msgtype" ) );
	}

	if ( clProperty.IsExist( "speechidx" ) )
	{
		SetSpeechTableIndex( clProperty.GetValueAsInt( "speechidx" ) );
	}

	if ( clProperty.IsExist( "dsptime" ) )
	{
		SetDisplayTime( clProperty.GetValueAsInt( "dsptime" ) );
	}
}

void CDboTSActTelecast::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetTelecastType() );
	clProperty.m_defProperty["type"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetNPCTableIndex() );
	clProperty.m_defProperty["npcidx"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetMessageType() );
	clProperty.m_defProperty["msgtype"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetSpeechTableIndex() );
	clProperty.m_defProperty["speechidx"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetDisplayTime() );
	clProperty.m_defProperty["dsptime"] = g_NtlTSString;
}

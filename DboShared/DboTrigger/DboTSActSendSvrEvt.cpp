#include "precomp_trigger.h"
#include "DboTSActSendSvrEvt.h"


NTL_TS_IMPLEMENT_RTTI( CDboTSActSendSvrEvt, CNtlTSAction )


CDboTSActSendSvrEvt::CDboTSActSendSvrEvt( void )
: m_uiType( 0 ),
  m_byTriggerType( TS_TYPE_INVALID ),
  m_uiEvtID( 0xffffffff ),
  m_uiTblIdx( 0xffffffff ),
  m_uiEvtSendType( 0 ),
  m_fEvtSendType_Radius( 0.f )
{
}

void CDboTSActSendSvrEvt::SetSvrEvtType( unsigned int uiType )
{
	m_uiType = uiType;
}

void CDboTSActSendSvrEvt::SetSvrEvtTriggerType( unsigned char byTriggerType )
{
	m_byTriggerType = byTriggerType;
}

void CDboTSActSendSvrEvt::SetSvrEvtID( unsigned int uiEvtID )
{
	m_uiEvtID = uiEvtID;
}

void CDboTSActSendSvrEvt::SetTblIdx( unsigned int uiTblIdx )
{
	m_uiTblIdx = uiTblIdx;
}

void CDboTSActSendSvrEvt::SetEvtSendType( unsigned int uiEvtSendType )
{
	m_uiEvtSendType = uiEvtSendType;
}

void CDboTSActSendSvrEvt::SetEvtSendType_Radius( float fRadius )
{
	m_fEvtSendType_Radius = fRadius;
}

void CDboTSActSendSvrEvt::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "type" ) )
	{
		SetSvrEvtType( clProperty.GetValueAsInt( "type" ) );
	}

	if ( clProperty.IsExist( "ttype" ) )
	{
		SetSvrEvtTriggerType( (UCHAR)clProperty.GetValueAsInt( "ttype" ) );
	}

	if ( clProperty.IsExist( "id" ) )
	{
		SetSvrEvtID( clProperty.GetValueAsInt( "id" ) );
	}

	if ( clProperty.IsExist( "tblidx" ) )
	{
		SetTblIdx( clProperty.GetValueAsInt( "tblidx" ) );
	}

	if ( clProperty.IsExist( "stype" ) )
	{
		SetEvtSendType( clProperty.GetValueAsInt( "stype" ) );
	}

	if ( clProperty.IsExist( "stradius" ) )
	{
		SetEvtSendType_Radius( (float)atof( clProperty.GetValue( "stradius" ).c_str() ) );
	}
}

void CDboTSActSendSvrEvt::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetSvrEvtType() );
	clProperty.m_defProperty["type"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetSvrEvtTriggerType() );
	clProperty.m_defProperty["ttype"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetSvrEvtID() );
	clProperty.m_defProperty["id"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetTblIdx() );
	clProperty.m_defProperty["tblidx"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetEvtSendType() );
	clProperty.m_defProperty["stype"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%f", GetEvtSendType_Radius() );
	clProperty.m_defProperty["stradius"] = g_NtlTSString;
}

#include "precomp_trigger.h"
#include "DboTSRcvSvrEvt.h"


NTL_TS_IMPLEMENT_RTTI( CDboTSRcvSvrEvt, CNtlTSEvent )


CDboTSRcvSvrEvt::CDboTSRcvSvrEvt( void )
: m_uiEvtID( 0xffffffff )
{
}

void CDboTSRcvSvrEvt::SetEvtID( unsigned int uiEvtID )
{
	m_uiEvtID = uiEvtID;
}

void CDboTSRcvSvrEvt::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "id" ) )
	{
		SetEvtID( clProperty.GetValueAsInt( "id" ) );
	}
}

void CDboTSRcvSvrEvt::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetEvtID() );
	clProperty.m_defProperty["id"] = g_NtlTSString;
}

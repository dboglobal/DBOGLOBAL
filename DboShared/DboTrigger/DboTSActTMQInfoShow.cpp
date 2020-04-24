#include "precomp_trigger.h"
#include "DboTSActTMQInfoShow.h"


NTL_TS_IMPLEMENT_RTTI( CDboTSActTMQInfoShow, CNtlTSAction )


CDboTSActTMQInfoShow::CDboTSActTMQInfoShow( void )
: m_eInfoType( INVALID_TIMEQUEST_INFORMATION_TYPE )
{
}

void CDboTSActTMQInfoShow::SetTMQInfoType( eTIMEQUEST_INFORMATION_TYPE eInfoType )
{
	m_eInfoType = eInfoType;
}

void CDboTSActTMQInfoShow::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "itype" ) )
	{
		SetTMQInfoType( (eTIMEQUEST_INFORMATION_TYPE)clProperty.GetValueAsInt( "itype" ) );
	}
}

void CDboTSActTMQInfoShow::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetTMQInfoType() );
	clProperty.m_defProperty["itype"] = g_NtlTSString;
}

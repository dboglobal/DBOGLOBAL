#include "precomp_trigger.h"
#include "DboTSActTMQStage.h"


NTL_TS_IMPLEMENT_RTTI( CDboTSActTMQStage, CNtlTSAction )


CDboTSActTMQStage::CDboTSActTMQStage( void )
: m_byStage( 0xff )
{
}

void CDboTSActTMQStage::SetTMQStage( unsigned char byStage )
{
	m_byStage = byStage;
}

void CDboTSActTMQStage::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "stage" ) )
	{
		SetTMQStage( (unsigned char)clProperty.GetValueAsInt( "stage" ) );
	}
}

void CDboTSActTMQStage::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetTMQStage() );
	clProperty.m_defProperty["stage"] = g_NtlTSString;
}

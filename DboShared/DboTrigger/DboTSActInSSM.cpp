#include "precomp_trigger.h"
#include "DboTSActInSSM.h"


/**
	Input SSM action
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSActInSSM, CNtlTSAction )


CDboTSActInSSM::CDboTSActInSSM( void )
: m_eSSMId( eSSM_ID_INVALID ),
  m_uiValue( 0xffffffff )
{
}

void CDboTSActInSSM::SetSSMId( eSSM_ID eSSMId )
{
	m_eSSMId = eSSMId;
}

void CDboTSActInSSM::SetValue( unsigned int uiValue )
{
	m_uiValue = uiValue;
}

void CDboTSActInSSM::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "ssmid" ) )
	{
		SetSSMId( (eSSM_ID)clProperty.GetValueAsInt( "ssmid" ) );
	}

	if ( clProperty.IsExist( "val" ) )
	{
		SetValue( clProperty.GetValueAsInt( "val" ) );
	}
}

void CDboTSActInSSM::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetSSMId() );
	clProperty.m_defProperty["ssmid"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetValue() );
	clProperty.m_defProperty["val"] = g_NtlTSString;
}

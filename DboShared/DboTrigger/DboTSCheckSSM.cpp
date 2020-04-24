#include "precomp_trigger.h"
#include "DboTSCheckSSM.h"


/**
	Check storage slot memory condition
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSCheckSSM, CNtlTSCond )


CDboTSCheckSSM::CDboTSCheckSSM( void )
: m_eOPType( eSSM_OP_TYPE_INVALID ),
  m_eSSMId( eSSM_ID_INVALID ),
  m_uiVariable( 0xffffffff )
{
}

void CDboTSCheckSSM::SetOPType( eSSM_OP_TYPE eOPType )
{
	m_eOPType = eOPType;
}

void CDboTSCheckSSM::SetSSMId( eSSM_ID eSSMId )
{
	m_eSSMId = eSSMId;
}

void CDboTSCheckSSM::SetVariable( unsigned int uiVariable )
{
	m_uiVariable = uiVariable;
}

void CDboTSCheckSSM::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "op" ) )
	{
		SetOPType( (eSSM_OP_TYPE)clProperty.GetValueAsInt( "op" ) );
	}

	if ( clProperty.IsExist( "ssmid" ) )
	{
		SetSSMId( (eSSM_ID)clProperty.GetValueAsInt( "ssmid" ) );
	}

	if ( clProperty.IsExist( "var" ) )
	{
		SetVariable( clProperty.GetValueAsInt( "var" ) );
	}
}

void CDboTSCheckSSM::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetOPType() );
	clProperty.m_defProperty["op"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetSSMId() );
	clProperty.m_defProperty["ssmid"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetVariable() );
	clProperty.m_defProperty["var"] = g_NtlTSString;
}

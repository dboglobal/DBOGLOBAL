#include "precomp_trigger.h"
#include "DboTSActSWProbSF.h"
#include "DboTSContSwitch.h"


/**
	Switch probability success fail
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSActSWProbSF, CNtlTSAction )


CDboTSActSWProbSF::CDboTSActSWProbSF( void )
: m_fProbility( 0.f ),
  m_uiSuccessBID( 0xffffffff ),
  m_uiFailBID( 0xffffffff )
{
}


void CDboTSActSWProbSF::SetProbility( float fProbility )
{
	m_fProbility = fProbility;
}


void CDboTSActSWProbSF::SetSuccessBID( unsigned int uiBID )
{
	m_uiSuccessBID = uiBID;
}


void CDboTSActSWProbSF::SetFailBID( unsigned int uiBID )
{
	m_uiFailBID = uiBID;
}


void CDboTSActSWProbSF::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "prob" ) )
	{
		SetProbility( (float)atof( clProperty.GetValue( "prob" ).c_str() ) );
	}

	if ( clProperty.IsExist( "sbi" ) )
	{
		SetSuccessBID( clProperty.GetValueAsInt( "sbi" ) );
	}

	if ( clProperty.IsExist( "fbi" ) )
	{
		SetFailBID( clProperty.GetValueAsInt( "fbi" ) );
	}
}

void CDboTSActSWProbSF::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	if ( GetParent()->GetEntityType() == DBO_CONT_TYPE_ID_CONT_SWITCH )
	{
		CDboTSContSwitch* pSwitch = (CDboTSContSwitch*) GetParent();

		if ( !pSwitch->IsNextLinkFromBId( GetSuccessBID() ) )
		{
			CNtlTSLog::Log( "The success branch id is mismatch. Info[%d]. [%s]", GetSuccessBID(), TS_CODE_TRACE() );
		}

		if ( !pSwitch->IsNextLinkFromBId( GetFailBID() ) )
		{
			CNtlTSLog::Log( "The fail branch id is mismatch. Info[%d]. [%s]", GetFailBID(), TS_CODE_TRACE() );
		}
	}
	else
	{
		CNtlTSLog::Log( "SWProbSF is not in a switch container. [%s]", TS_CODE_TRACE() );
	}

	sprintf_s( g_NtlTSString, "%f", GetProbility() );
	clProperty.m_defProperty["prob"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetSuccessBID() );
	clProperty.m_defProperty["sbi"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetFailBID() );
	clProperty.m_defProperty["fbi"] = g_NtlTSString;
}

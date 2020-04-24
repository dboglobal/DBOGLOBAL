#include "precomp_trigger.h"
#include "DboTSActDir.h"


/**
	Direction action
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSActDir, CNtlTSAction )


CDboTSActDir::CDboTSActDir( void )
: m_fRange( 10.f ),
  m_byDirectionType( 0xff ),
  m_uiDirectionTblIdx( 0xffffffff )
{
}

void CDboTSActDir::SetRange( float fRange )
{
	m_fRange = fRange;
}

void CDboTSActDir::SetDirectionType( unsigned char byDirectionType )
{
	m_byDirectionType = byDirectionType;
}

void CDboTSActDir::SetDirectionTblIdx( unsigned int TblIdx )
{
	m_uiDirectionTblIdx = TblIdx;
}

void CDboTSActDir::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "rng" ) )
	{
		SetRange( (float)atof( clProperty.GetValue( "rng" ).c_str() ) );
	}

	if ( clProperty.IsExist( "type" ) )
	{
		SetDirectionType( (unsigned char)clProperty.GetValueAsInt( "type" ) );
	}

	if ( clProperty.IsExist( "idx" ) )
	{
		SetDirectionTblIdx( (unsigned int)clProperty.GetValueAsInt( "idx" ) );
	}
}

void CDboTSActDir::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%f", GetRange() );
	clProperty.m_defProperty["rng"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetDirectionType() );
	clProperty.m_defProperty["type"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetDirectionTblIdx() );
	clProperty.m_defProperty["idx"] = g_NtlTSString;
}

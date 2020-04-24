#include "precomp_trigger.h"
#include "DboTSCheckOutRGN.h"


NTL_TS_IMPLEMENT_RTTI( CDboTSCheckOutRGN, CNtlTSCond )


CDboTSCheckOutRGN::CDboTSCheckOutRGN( void )
: m_uiWorldIdx( 0xffffffff ),
  m_fX( 0.f ),
  m_fZ( 0.f ),
  m_fRadius( 0.f )
{
}

void CDboTSCheckOutRGN::SetWorldTblIdx( unsigned int uiWorldTblIdx )
{
	m_uiWorldIdx = uiWorldTblIdx;
}

void CDboTSCheckOutRGN::SetPosX( float fX )
{
	m_fX = fX;
}

void CDboTSCheckOutRGN::SetPosZ( float fZ )
{
	m_fZ = fZ;
}

void CDboTSCheckOutRGN::SetRadius( float fRadius )
{
	m_fRadius = fRadius;
}

void CDboTSCheckOutRGN::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "widx" ) )
	{
		SetWorldTblIdx( clProperty.GetValueAsInt( "widx" ) );
	}
	if ( clProperty.IsExist( "fx" ) )
	{
		SetPosX( (float)atof( clProperty.GetValue( "fx" ).c_str() ) );
	}
	if ( clProperty.IsExist( "fz" ) )
	{
		SetPosZ( (float)atof( clProperty.GetValue( "fz" ).c_str() ) );
	}
	if ( clProperty.IsExist( "fr" ) )
	{
		SetRadius( (float)atof( clProperty.GetValue( "fr" ).c_str() ) );
	}
}

void CDboTSCheckOutRGN::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetWorldTblIdx() );
	clProperty.m_defProperty["widx"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%f", GetPosX() );
	clProperty.m_defProperty["fx"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%f", GetPosZ() );
	clProperty.m_defProperty["fz"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%f", GetRadius() );
	clProperty.m_defProperty["fr"] = g_NtlTSString;
}

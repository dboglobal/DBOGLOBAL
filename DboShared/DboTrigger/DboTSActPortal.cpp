#include "precomp_trigger.h"
#include "DboTSActPortal.h"
#include <float.h>

/**
	Direction action
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSActPortal, CNtlTSAction )


CDboTSActPortal::CDboTSActPortal( void )
{
}

void CDboTSActPortal::SetPotalType( ePORTAL_TYPE ePotalType )
{
	m_ePortalType = ePotalType;
}

void CDboTSActPortal::SetWorldIdx( unsigned int uiWorldIdx )
{
	m_uiWorldIdx = uiWorldIdx;
}

void CDboTSActPortal::SetPosition( float fPosX, float fPosY, float fPosZ )
{
	m_fPosX = fPosX;
	m_fPosY = fPosY;
	m_fPosZ = fPosZ;
}

void CDboTSActPortal::SetDirection( float fDirX, float fDirY, float fDirZ )
{
	m_fDirX = fDirX;
	m_fDirY = fDirY;
	m_fDirZ = fDirZ;
}

void CDboTSActPortal::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "type" ) )
	{
		SetPotalType( (ePORTAL_TYPE)clProperty.GetValueAsInt( "type" ) );
	}

	if ( clProperty.IsExist( "widx" ) )
	{
		SetWorldIdx( clProperty.GetValueAsInt( "widx" ) );
	}

	float fX = FLT_MAX;
	float fY = FLT_MAX;
	float fZ = FLT_MAX;

	if ( clProperty.IsExist( "px" ) )
	{
		fX = (float)atof( clProperty.GetValue( "px" ).c_str() );
	}
	if ( clProperty.IsExist( "py" ) )
	{
		fY = (float)atof( clProperty.GetValue( "py" ).c_str() );
	}
	if ( clProperty.IsExist( "pz" ) )
	{
		fZ = (float)atof( clProperty.GetValue( "pz" ).c_str() );
	}
	SetPosition( fX, fY, fZ );

	fX = FLT_MAX;
	fY = FLT_MAX;
	fZ = FLT_MAX;

	if ( clProperty.IsExist( "dx" ) )
	{
		fX = (float)atof( clProperty.GetValue( "dx" ).c_str() );
	}
	if ( clProperty.IsExist( "dy" ) )
	{
		fY = (float)atof( clProperty.GetValue( "dy" ).c_str() );
	}
	if ( clProperty.IsExist( "dz" ) )
	{
		fZ = (float)atof( clProperty.GetValue( "dz" ).c_str() );
	}
	SetDirection( fX, fY, fZ );

}

void CDboTSActPortal::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetPotalType() );
	clProperty.m_defProperty["type"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetWorldIdx() );
	clProperty.m_defProperty["widx"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%f", m_fPosX );
	clProperty.m_defProperty["px"] = g_NtlTSString;
	sprintf_s( g_NtlTSString, "%f", m_fPosY );
	clProperty.m_defProperty["py"] = g_NtlTSString;
	sprintf_s( g_NtlTSString, "%f", m_fPosZ );
	clProperty.m_defProperty["pz"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%f", m_fDirX );
	clProperty.m_defProperty["dx"] = g_NtlTSString;
	sprintf_s( g_NtlTSString, "%f", m_fDirY );
	clProperty.m_defProperty["dy"] = g_NtlTSString;
	sprintf_s( g_NtlTSString, "%f", m_fDirZ );
	clProperty.m_defProperty["dz"] = g_NtlTSString;
}

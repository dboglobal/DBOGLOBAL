#include "precomp_trigger.h"
#include "DboTSCheckPCCls.h"


/**
	Check pc class condition
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSCheckPCCls, CNtlTSCond )


CDboTSCheckPCCls::CDboTSCheckPCCls( void )
: m_uiClsFlags( 0 )
{
}

void CDboTSCheckPCCls::SetClsFlags( unsigned int uiClsFlags )
{
	m_uiClsFlags = uiClsFlags;
}

void CDboTSCheckPCCls::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "clsflg" ) )
	{
		SetClsFlags( clProperty.GetValueAsInt( "clsflg" ) );
	}
}

void CDboTSCheckPCCls::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetClsFlags() );
	clProperty.m_defProperty["clsflg"] = g_NtlTSString;
}

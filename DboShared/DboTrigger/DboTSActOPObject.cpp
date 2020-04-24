#include "precomp_trigger.h"
#include "DboTSActOPObject.h"


NTL_TS_IMPLEMENT_RTTI( CDboTSActOPObject, CNtlTSAction )


CDboTSActOPObject::CDboTSActOPObject( void )
: m_bApply ( true ),
  m_uiOperateTime( 0 ),
  m_uiDirectionTblIdx( 0xffffffff )
{
}

void CDboTSActOPObject::SetApply( bool bApply )
{
	m_bApply = bApply;
}

void CDboTSActOPObject::SetOperateTime( unsigned int uiOperateTime )
{
	m_uiOperateTime = uiOperateTime;
}

void CDboTSActOPObject::SetDirectionTableIndex( unsigned int uiDirTblIdx )
{
	m_uiDirectionTblIdx = uiDirTblIdx;
}

void CDboTSActOPObject::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "apply" ) )
	{
		SetApply( clProperty.GetValueAsInt( "apply" ) ? true : false );
	}

	if ( clProperty.IsExist( "time" ) )
	{
		SetOperateTime( clProperty.GetValueAsInt( "time" ) );
	}

	if ( clProperty.IsExist( "tblidx" ) )
	{
		SetDirectionTableIndex( clProperty.GetValueAsInt( "tblidx" ) );
	}
}

void CDboTSActOPObject::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", IsApply() );
	clProperty.m_defProperty["apply"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetOperateTime() );
	clProperty.m_defProperty["time"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetDirectionTableIndex() );
	clProperty.m_defProperty["tblidx"] = g_NtlTSString;
}

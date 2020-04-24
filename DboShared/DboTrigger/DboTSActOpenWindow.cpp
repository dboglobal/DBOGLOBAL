#include "precomp_trigger.h"
#include "DboTSActOpenWindow.h"


NTL_TS_IMPLEMENT_RTTI( CDboTSActOpenWindow, CNtlTSAction )


CDboTSActOpenWindow::CDboTSActOpenWindow( void )
: m_bShow( true ),
  m_eWindowType( eWINDOW_TYPE_HELP ),
  m_uiTblIdx( 0xffffffff )
{
}

void CDboTSActOpenWindow::SetShow( bool bShow )
{
	m_bShow = bShow;
}

void CDboTSActOpenWindow::SetWindowType( eWINDOW_TYPE eWindowType )
{
	m_eWindowType = eWindowType;
}

void CDboTSActOpenWindow::SetTableIdx( unsigned int uiTblIdx )
{
	m_uiTblIdx = uiTblIdx;
}

void CDboTSActOpenWindow::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "show" ) )
	{
		SetShow( clProperty.GetValueAsInt( "show" ) ? true : false );
	}

	if ( clProperty.IsExist( "wtype" ) )
	{
		SetWindowType( (eWINDOW_TYPE)clProperty.GetValueAsInt( "wtype" ) );
	}

	if ( clProperty.IsExist( "tblidx" ) )
	{
		SetTableIdx( clProperty.GetValueAsInt( "tblidx" ) );
	}
}

void CDboTSActOpenWindow::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", IsShow() ? 1 : 0 );
	clProperty.m_defProperty["show"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetWindowType() );
	clProperty.m_defProperty["wtype"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetTableIdx() );
	clProperty.m_defProperty["tblidx"] = g_NtlTSString;
}

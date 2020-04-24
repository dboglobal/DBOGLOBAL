#include "precomp_trigger.h"
#include "DboTSContNarration.h"


/**
	Narration container
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSContNarration, CNtlTSCont )


CDboTSContNarration::CDboTSContNarration( void )
{
	m_eProgState = eNARRATION_PROGRESS_STATE_START;
	m_eOwnerType = eNARRATION_OWNER_NPC;
	m_uiOwnerIdx = 0xffffffff;
	m_eOwnerState = eNARRATION_OWNER_STATE_1;
	m_eDialogDirType = eNARRATION_DIALOG_DIR_TYPE_NORMAL;
	m_uiDialog = 0xffffffff;
	m_eGUIType = eNARRATION_GUI_TYPE_NORMAL;
	m_uiMaxLifeTime = 0xffffffff;

	m_tcOkLink = NTL_TS_TC_ID_INVALID;
	m_tcCancelLink = NTL_TS_TC_ID_INVALID;
	m_tcLogInLink = NTL_TS_TC_ID_INVALID;
}

void CDboTSContNarration::SetProgressState( eNARRATION_PROGRESS_STATE eState )
{
	m_eProgState = eState;
}

void CDboTSContNarration::SetOwnerType( eNARRATION_OWNER_TYPE eType )
{
	m_eOwnerType = eType;
}

void CDboTSContNarration::SetOwnerIdx( unsigned int uiIdx )
{
	m_uiOwnerIdx = uiIdx;
}

void CDboTSContNarration::SetOwnerState( eNARRATION_OWNER_STATE eState )
{
	m_eOwnerState = eState;
}

void CDboTSContNarration::SetDialogDirType( eNARRATION_DIALOG_DIR_TYPE eType )
{
	m_eDialogDirType = eType;
}

void CDboTSContNarration::SetDialog( unsigned int uiDialog )
{
	m_uiDialog = uiDialog;
}

void CDboTSContNarration::SetGUIType( eNARRATION_GUI_TYPE eType )
{
	m_eGUIType = eType;
}

void CDboTSContNarration::SetMaxLifeTime( unsigned int uiMaxLifeTime )
{
	m_uiMaxLifeTime = uiMaxLifeTime;
}

void CDboTSContNarration::SetOkLink( NTL_TS_TC_ID tcId )
{
	m_tcOkLink = tcId;
}

void CDboTSContNarration::SetCancelLink( NTL_TS_TC_ID tcId )
{
	m_tcCancelLink = tcId;
}

void CDboTSContNarration::SetLogInLink( NTL_TS_TC_ID tcId )
{
	m_tcLogInLink = tcId;
}

void CDboTSContNarration::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSCont::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "pt" ) )
	{
		SetProgressState( (eNARRATION_PROGRESS_STATE)clProperty.GetValueAsInt( "pt" ) );
	}

	if ( clProperty.IsExist( "ot" ) )
	{
		SetOwnerType( (eNARRATION_OWNER_TYPE)clProperty.GetValueAsInt( "ot" ) );
	}

	if ( clProperty.IsExist( "oi" ) )
	{
		SetOwnerIdx( clProperty.GetValueAsInt( "oi" ) );
	}

	if ( clProperty.IsExist( "os" ) )
	{
		SetOwnerState( (eNARRATION_OWNER_STATE)clProperty.GetValueAsInt( "os" ) );
	}

	if ( clProperty.IsExist( "dt" ) )
	{
		SetDialogDirType( (eNARRATION_DIALOG_DIR_TYPE)clProperty.GetValueAsInt( "dt" ) );
	}

	if ( clProperty.IsExist( "dg" ) )
	{
		SetDialog( clProperty.GetValueAsInt( "dg" ) );
	}

	if ( clProperty.IsExist( "gt" ) )
	{
		SetGUIType( (eNARRATION_GUI_TYPE)clProperty.GetValueAsInt( "gt" ) );
	}

	if ( clProperty.IsExist( "mlt" ) )
	{
		SetMaxLifeTime( (unsigned int)clProperty.GetValueAsInt( "mlt" ) );
	}

	if ( clProperty.IsExist( "oklnk" ) )
	{
		SetOkLink( (NTL_TS_TC_ID)clProperty.GetValueAsInt( "oklnk" ) );
	}

	if ( clProperty.IsExist( "cancellnk" ) )
	{
		SetCancelLink( (NTL_TS_TC_ID)clProperty.GetValueAsInt( "cancellnk" ) );
	}

	if ( clProperty.IsExist( "lilnk" ) )
	{
		SetLogInLink( (NTL_TS_TC_ID)clProperty.GetValueAsInt( "lilnk" ) );
	}
}

void CDboTSContNarration::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSCont::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetProgressState() );
	clProperty.m_defProperty["pt"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetOwnerType() );
	clProperty.m_defProperty["ot"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetOwnerIdx() );
	clProperty.m_defProperty["oi"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetOwnerState() );
	clProperty.m_defProperty["os"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetDialogDirType() );
	clProperty.m_defProperty["dt"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetDialog() );
	clProperty.m_defProperty["dg"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetGUIType() );
	clProperty.m_defProperty["gt"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetMaxLifeTime() );
	clProperty.m_defProperty["mlt"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetOkLink() );
	clProperty.m_defProperty["oklnk"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetCancelLink() );
	clProperty.m_defProperty["cancellnk"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetLogInLink() );
	clProperty.m_defProperty["lilnk"] = g_NtlTSString;
}
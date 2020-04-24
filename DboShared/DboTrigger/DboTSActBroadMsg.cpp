#include "precomp_trigger.h"
#include "DboTSActBroadMsg.h"


/**
	Broad msg action
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSActBroadMsg, CNtlTSAction )


CDboTSActBroadMsg::CDboTSActBroadMsg( void )
: m_eExcuteType( eBROAD_MSG_EXCUTION_TYPE_TIME ),
  m_eUIDirType( eBROAD_MSG_UI_DIR_TYPE_NORMAL ),
  m_eUIShowHideDirType( eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_NORMAL ),
  m_eUIBalloonShapeType( eBROAD_MSG_UI_BALLOON_SHAPE_TYPE_BC_NORMAL ),
  m_eUISpeechDirType( eBROAD_MSG_UI_SPEECH_DIR_TYPE_NORMAL ),
  m_eOwnerCondition( eBROAD_MSG_OWNER_CONDITION_1 ),
  m_eOwnerType( eBROAD_MSG_OWNER_TYPE_NPC ),
  m_uiOwnerTblIdx( 0xffffffff ),
  m_uiQTextTblIdx( 0xffffffff ),
  m_uiDisplayTime( 0xffffffff )
{
}

void CDboTSActBroadMsg::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "et" ) )
	{
		m_eExcuteType = (eBROAD_MSG_EXCUTION_TYPE)clProperty.GetValueAsInt( "et" );
	}
	if ( clProperty.IsExist( "uidt" ) )
	{
		m_eUIDirType = (eBROAD_MSG_UI_DIR_TYPE)clProperty.GetValueAsInt( "uidt" );
	}
	if ( clProperty.IsExist( "uisht" ) )
	{
		m_eUIShowHideDirType = (eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE)clProperty.GetValueAsInt( "uisht" );
	}
	if ( clProperty.IsExist( "uibst" ) )
	{
		m_eUIBalloonShapeType = (eBROAD_MSG_UI_BALLOON_SHAPE_TYPE)clProperty.GetValueAsInt( "uibst" );
	}
	if ( clProperty.IsExist( "uisdt" ) )
	{
		m_eUISpeechDirType = (eBROAD_MSG_UI_SPEECH_DIR_TYPE)clProperty.GetValueAsInt( "uisdt" );
	}
	if ( clProperty.IsExist( "oc" ) )
	{
		m_eOwnerCondition = (eBROAD_MSG_OWNER_CONDITION)clProperty.GetValueAsInt( "oc" );
	}
	if ( clProperty.IsExist( "ot" ) )
	{
		m_eOwnerType = (eBROAD_MSG_OWNER_TYPE)clProperty.GetValueAsInt( "ot" );
	}
	if ( clProperty.IsExist( "oidx" ) )
	{
		m_uiOwnerTblIdx = clProperty.GetValueAsInt( "oidx" );
	}
	if ( clProperty.IsExist( "oqtidx" ) )
	{
		m_uiQTextTblIdx = clProperty.GetValueAsInt( "oqtidx" );
	}
	if ( clProperty.IsExist( "dt" ) )
	{
		m_uiDisplayTime = clProperty.GetValueAsInt( "dt" );
	}
}

void CDboTSActBroadMsg::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", m_eExcuteType );
	clProperty.m_defProperty["et"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", m_eUIDirType );
	clProperty.m_defProperty["uidt"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", m_eUIShowHideDirType );
	clProperty.m_defProperty["uisht"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", m_eUIBalloonShapeType );
	clProperty.m_defProperty["uibst"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", m_eUISpeechDirType );
	clProperty.m_defProperty["uisdt"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", m_eOwnerCondition );
	clProperty.m_defProperty["oc"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", m_eOwnerType );
	clProperty.m_defProperty["ot"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", m_uiOwnerTblIdx );
	clProperty.m_defProperty["oidx"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", m_uiQTextTblIdx );
	clProperty.m_defProperty["oqtidx"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", m_uiDisplayTime );
	clProperty.m_defProperty["dt"] = g_NtlTSString;
}

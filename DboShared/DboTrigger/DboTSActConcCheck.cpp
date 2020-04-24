#include "precomp_trigger.h"
#include "DboTSActConcCheck.h"


NTL_TS_IMPLEMENT_RTTI( CDboTSActConcCheck, CNtlTSAction )


CDboTSActConcCheck::CDboTSActConcCheck( void )
{
	m_nConcurrencyCnt = 2;
	m_uiResetTime = 0;

	m_uiExcuteObjTblIdx = 0xffffffff;
	m_tExcuteTID = NTL_TS_T_ID_INVALID;
}

void CDboTSActConcCheck::SetConcurrencyCnt( int nConcurrencyCnt )
{
	m_nConcurrencyCnt = nConcurrencyCnt;
}

void CDboTSActConcCheck::SetResetTime( unsigned int uiResetTime )
{
	m_uiResetTime = uiResetTime;
}

void CDboTSActConcCheck::SetExcuteObjTblIdx( unsigned int uiExcuteObjTblIdx )
{
	m_uiExcuteObjTblIdx = uiExcuteObjTblIdx;
}

void CDboTSActConcCheck::SetExcuteTID( NTL_TS_T_ID tExcuteTID )
{
	m_tExcuteTID = tExcuteTID;
}

void CDboTSActConcCheck::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "ccnt" ) )
	{
		SetConcurrencyCnt( clProperty.GetValueAsInt( "ccnt" ) );
	}

	if ( clProperty.IsExist( "rtime" ) )
	{
		SetResetTime( clProperty.GetValueAsInt( "rtime" ) );
	}

	if ( clProperty.IsExist( "eobjidx" ) )
	{
		SetExcuteObjTblIdx( clProperty.GetValueAsInt( "eobjidx" ) );
	}

	if ( clProperty.IsExist( "etsid" ) )
	{
		SetExcuteTID( (NTL_TS_T_ID)clProperty.GetValueAsInt( "etsid" ) );
	}
}

void CDboTSActConcCheck::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetConcurrencyCnt() );
	clProperty.m_defProperty["ccnt"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetResetTime() );
	clProperty.m_defProperty["rtime"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetExcuteObjTblIdx() );
	clProperty.m_defProperty["eobjidx"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetExcuteTID() );
	clProperty.m_defProperty["etsid"] = g_NtlTSString;
}

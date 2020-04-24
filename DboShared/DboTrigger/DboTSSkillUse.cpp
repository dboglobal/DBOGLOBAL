#include "precomp_trigger.h"
#include "DboTSSkillUse.h"


NTL_TS_IMPLEMENT_RTTI( CDboTSSkillUse, CNtlTSEvent )


CDboTSSkillUse::CDboTSSkillUse( void )
: m_eSkillType( eEVENT_SKILL_TYPE_SKILLIDX ),
  m_uiSkillIdx( 0xffffffff ),
  m_eSkillTargetType( eEVENT_SKILL_TARGET_TYPE_INVALID ),
  m_uiSkillTargetIdx( 0xffffffff ),
  m_uiSkillRPFlags( 0 )
{
}

void CDboTSSkillUse::SetSkillType( eEVENT_SKILL_TYPE eSkillType )
{
	m_eSkillType = eSkillType;
}

void CDboTSSkillUse::SetSkillIdx( unsigned int uiSkillIdx )
{
	m_uiSkillIdx = uiSkillIdx;
}

void CDboTSSkillUse::SetSkillTargetType( eEVENT_SKILL_TARGET_TYPE eSkillTargetType )
{
	m_eSkillTargetType = eSkillTargetType;
}

void CDboTSSkillUse::SetSkillTargetIdx( unsigned int uiSkillTargetIdx )
{
	m_uiSkillTargetIdx = uiSkillTargetIdx;
}

void CDboTSSkillUse::SetSkillRPFlags( unsigned int uiSkillRPFlags )
{
	m_uiSkillRPFlags = uiSkillRPFlags;
}

void CDboTSSkillUse::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "stype" ) )
	{
		SetSkillType( (eEVENT_SKILL_TYPE)clProperty.GetValueAsInt( "stype" ) );
	}

	if ( clProperty.IsExist( "idx" ) )
	{
		SetSkillIdx( clProperty.GetValueAsInt( "idx" ) );
	}

	if ( clProperty.IsExist( "ttype" ) )
	{
		SetSkillTargetType( (eEVENT_SKILL_TARGET_TYPE)clProperty.GetValueAsInt( "ttype" ) );
	}

	if ( clProperty.IsExist( "tidx" ) )
	{
		SetSkillTargetIdx( clProperty.GetValueAsInt( "tidx" ) );
	}

	if ( clProperty.IsExist( "rp" ) )
	{
		SetSkillRPFlags( clProperty.GetValueAsInt( "rp" ) );
	}
}

void CDboTSSkillUse::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetSkillType() );
	clProperty.m_defProperty["stype"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetSkillIdx() );
	clProperty.m_defProperty["idx"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetSkillTargetType() );
	clProperty.m_defProperty["ttype"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetSkillTargetIdx() );
	clProperty.m_defProperty["tidx"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetSkillRPFlags() );
	clProperty.m_defProperty["rp"] = g_NtlTSString;
}

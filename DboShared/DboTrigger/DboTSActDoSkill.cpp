#include "precomp_trigger.h"
#include "DboTSActDoSkill.h"


/**
	Do skill action
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSActDoSkill, CNtlTSAction )


CDboTSActDoSkill::CDboTSActDoSkill( void )
: m_uiSkillIdx( 0xffffffff )
{
}

void CDboTSActDoSkill::SetSkillIdx( unsigned int uiSkillIdx )
{
	m_uiSkillIdx = uiSkillIdx;
}

void CDboTSActDoSkill::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "idx" ) )
	{
		SetSkillIdx( (unsigned int)clProperty.GetValueAsInt( "idx" ) );
	}
}

void CDboTSActDoSkill::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetSkillIdx() );
	clProperty.m_defProperty["idx"] = g_NtlTSString;
}

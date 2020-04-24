#include "precomp_trigger.h"
#include "DboTSEMSkill.h"
#include "SkillTable.h"
#include "DboTSCoreDefine.h"
#include "DboTSSkillUse.h"


/** 
	Event mapper skill receiver
*/

struct sSKILL_PARAM
{
	sSKILL_PARAM( void ) : bFind( false ), uiSkillIdx( 0xffffffff ) { return; }

	bool						bFind;
	unsigned int				uiSkillIdx;
};

class CDboTSEMSkill_Recv : public CNtlTSRecv
{
public:
	NTL_TSRESULT				Search( CNtlTSEntity* pEntity, void* pParam );
	NTL_TSRESULT				Run( CNtlTSEntity*, void* ) { return 0; }
};

NTL_TSRESULT CDboTSEMSkill_Recv::Search( CNtlTSEntity* pEntity, void* pParam )
{
	if ( pEntity->GetEntityType() == DBO_EVENT_TYPE_ID_SKILL_USE )
	{
		((sSKILL_PARAM*)pParam)->bFind = true;

		CDboTSSkillUse* pSkillUse = (CDboTSSkillUse*) pEntity;
		((sSKILL_PARAM*)pParam)->uiSkillIdx = pSkillUse->GetSkillIdx();
	}

	return 0;
}


/** 
	Event mapper skill
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSEMSkill, CNtlTSEvtMapper )


CDboTSEMSkill::CDboTSEMSkill( void )
{
}

CDboTSEMSkill::~CDboTSEMSkill( void )
{
}

bool CDboTSEMSkill::AddBuildData( const std::string& strKey, const CNtlTSMain::mapdef_TLIST& TList, void* pTblList, void* pParameter )
{
	UNREFERENCED_PARAMETER( pTblList );
	UNREFERENCED_PARAMETER( pParameter );

	sSKILL_PARAM sParam;
	CDboTSEMSkill_Recv clRecv;

	// Trigger system script들을 순회하면서 해당 skill id로 시작할 수 있는
	// trigger를 찾는다

	CNtlTSTrigger* pTrig;
	CNtlTSMain::mapdef_TLIST::const_iterator citTSBegin = TList.begin();
	CNtlTSMain::mapdef_TLIST::const_iterator citTSEnd = TList.end();

	for ( ; citTSBegin != citTSEnd; ++citTSBegin )
	{
		pTrig = citTSBegin->second;

		sParam.bFind = false;

		pTrig->SearchTarget( NTL_TS_MAIN_GROUP_ID, START_CONTAINER_ID, &clRecv, &sParam );

		if ( sParam.bFind )
		{
			if  ( m_defMapper.find( sParam.uiSkillIdx ) == m_defMapper.end() )
			{
				m_defMapper[sParam.uiSkillIdx][strKey] = vecdef_TID_LIST();

				m_defMapper[sParam.uiSkillIdx][strKey].push_back( pTrig->GetID() );
			}
			else
			{
				if ( m_defMapper[sParam.uiSkillIdx].find( strKey ) == m_defMapper[sParam.uiSkillIdx].end() )
				{
					m_defMapper[sParam.uiSkillIdx][strKey] = vecdef_TID_LIST();

					m_defMapper[sParam.uiSkillIdx][strKey].push_back( pTrig->GetID() );
				}
				else
				{
					m_defMapper[sParam.uiSkillIdx][strKey].push_back( pTrig->GetID() );
				}
			}
		}
	}

	return true;
}

bool CDboTSEMSkill::DelBuildData( const std::string& strKey )
{
	mapdef_MAPPER::iterator itMapper = m_defMapper.begin();
	for ( ; itMapper != m_defMapper.end(); ++itMapper )
	{
		hashdef_TS_LIST& TSList = itMapper->second;
		hashdef_TS_LIST::iterator itFind = TSList.find( strKey );
		if ( itFind != TSList.end() )
		{
			TSList.erase( itFind );
		}
	}

	return true;
}


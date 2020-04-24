#include "StdAfx.h"
#include "ExportEvtTSMng.h"
#include "NtlSharedType.h"
#include "NPCTable.h"
#include "ObjectTable.h"
#include "MobTable.h"
#include "ItemTable.h"
#include "TableContainer.h"


class CExportEvtLog : public CNtlTSLog
{
protected:
	virtual void						OnLogEvent( const char* pLog )
	{
		if ( NULL == pLog )
		{
			return;
		}

		FILE* pFile;

		fopen_s( &pFile, "ExportEvt_Log.txt", "a+" );

		if ( pFile )
		{
			fprintf_s( pFile, "%s\n", pLog );
			fclose( pFile );
		}

		assert( 0 );
	}
};

CExportEvtTSMng::CExportEvtTSMng( void )
{
	m_pTblContainer = NULL;
}

CExportEvtTSMng::~CExportEvtTSMng( void )
{
}

bool CExportEvtTSMng::Create( const char* pTSPath, CTableContainer* pTblContainer )
{
	m_strQuestPath		= std::string( pTSPath ) + "quest.e";
	m_strTriggerPath	= std::string( pTSPath ) + "pctrigger.e";
	m_strObjectPath		= std::string( pTSPath ) + "objtrigger.e";

	m_pTblContainer = pTblContainer;

	return CDboTSMain::Create();
}

bool CExportEvtTSMng::LoadLog( void )
{
	UnloadLog();

	m_pLog = new CExportEvtLog;
	CNtlTSLog::Register( m_pLog );

	return true;
}

void CExportEvtTSMng::UnloadLog( void )
{
	if ( m_pLog )
	{
		CNtlTSLog::Unregister();
		delete m_pLog;
		m_pLog = 0;
	}
}

bool CExportEvtTSMng::LoadEventMappers( void )
{
	CNtlTSControlObject* pObj;

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMNPC" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMNPC" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMNPC*)pObj)->AddBuildData( "quest", m_defQuest, m_pTblContainer->GetNpcTable(), NULL ) )
		{
			CNtlTSLog::Log( "Building the npc in the npc event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMNPC*)pObj)->AddBuildData( "pctrigger", m_defTrigger, m_pTblContainer->GetNpcTable(), NULL ) )
		{
			CNtlTSLog::Log( "Building the npc in the npc event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMNPC. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	CTableContainer::OBJTABLEIT it = m_pTblContainer->BeginObjectTable();
	CTableContainer::OBJTABLEIT itEnd = m_pTblContainer->EndObjectTable();

	for ( ; it != itEnd; ++it )
	{
		pObj = m_pCtrlFactory->CreateObj( "CDboTSEMObject" );
		if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMObject" ) )
		{
			unsigned int uiWorldTblIdx = it->first;

			sprintf_s( g_NtlTSString, "CDboTSEMObject_%d", uiWorldTblIdx );

			m_defEvtMapper[g_NtlTSString] = (CNtlTSEvtMapper*)pObj;

			if ( !((CDboTSEMObject*)pObj)->AddBuildData( "quest", m_defQuest, it->second, &uiWorldTblIdx ) )
			{
				CNtlTSLog::Log( "Building the npc in the object event mapper is failed. [%s].", TS_CODE_TRACE() );
				return false;
			}

			if ( !((CDboTSEMObject*)pObj)->AddBuildData( "pctrigger", m_defTrigger, it->second, &uiWorldTblIdx ) )
			{
				CNtlTSLog::Log( "Building the npc in the object event mapper is failed. [%s].", TS_CODE_TRACE() );
				return false;
			}
		}
		else
		{
			CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMObject. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
			return false;
		}
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMItem" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMItem" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMItem*)pObj)->AddBuildData( "quest", m_defQuest, m_pTblContainer->GetItemTable(), NULL ) )
		{
			CNtlTSLog::Log( "Building the item in the item event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMItem*)pObj)->AddBuildData( "pctrigger", m_defTrigger, m_pTblContainer->GetItemTable(), NULL ) )
		{
			CNtlTSLog::Log( "Building the item in the item event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMItem. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMSvrEvt" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMSvrEvt" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMSvrEvt*)pObj)->AddBuildData( "quest", m_defQuest, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building server event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMSvrEvt*)pObj)->AddBuildData( "pctrigger", m_defTrigger, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building server event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMSvrEvt. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMSkill" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMSkill" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMSkill*)pObj)->AddBuildData( "quest", m_defQuest, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building skill event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMSkill*)pObj)->AddBuildData( "pctrigger", m_defTrigger, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building skill event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMSkill. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMColRgn" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMColRgn" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMColRgn*)pObj)->AddBuildData( "quest", m_defQuest, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building collision region event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMColRgn*)pObj)->AddBuildData( "pctrigger", m_defTrigger, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building collision region mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMColRgn. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMRB" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMRB" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMRB*)pObj)->AddBuildData( "quest", m_defQuest, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building rank-battle event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMRB*)pObj)->AddBuildData( "pctrigger", m_defTrigger, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building rank-battle mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMRB. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMMob" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMMob" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMNPC*)pObj)->AddBuildData( "quest", m_defQuest, m_pTblContainer->GetMobTable(), NULL ) )
		{
			CNtlTSLog::Log( "Building the mob in the mob event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMNPC*)pObj)->AddBuildData( "pctrigger", m_defTrigger, m_pTblContainer->GetMobTable(), NULL ) )
		{
			CNtlTSLog::Log( "Building the mob in the mob event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMMob. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMBindStone" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMBindStone" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMBindStone*)pObj)->AddBuildData( "quest", m_defQuest, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building bind stone event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMBindStone*)pObj)->AddBuildData( "pctrigger", m_defTrigger, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building bind stone event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMBindStone. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMSearchQuest" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMSearchQuest" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMSearchQuest*)pObj)->AddBuildData( "quest", m_defQuest, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building quest search event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMSearchQuest*)pObj)->AddBuildData( "pctrigger", m_defTrigger, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building quest search event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMSearchQuest. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMItemUpgrade" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMItemUpgrade" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMItemUpgrade*)pObj)->AddBuildData( "quest", m_defQuest, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building item upgrade event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMItemUpgrade*)pObj)->AddBuildData( "pctrigger", m_defTrigger, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building item upgrade event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMItemUpgrade. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMTeleport" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMTeleport" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMTeleport*)pObj)->AddBuildData( "quest", m_defQuest, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building teleport event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMTeleport*)pObj)->AddBuildData( "pctrigger", m_defTrigger, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building teleport event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMTeleport. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMBudokai" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMBudokai" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMBudokai*)pObj)->AddBuildData( "quest", m_defQuest, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building budokai mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMBudokai*)pObj)->AddBuildData( "pctrigger", m_defTrigger, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building budokai event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMBudokai. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMSlotMachine" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMSlotMachine" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMSlotMachine*)pObj)->AddBuildData( "quest", m_defQuest, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building SlotMachine event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMSlotMachine*)pObj)->AddBuildData( "pctrigger", m_defTrigger, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building SlotMachine event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMSlotMachine. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMHoipoiMix" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMHoipoiMix" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMHoipoiMix*)pObj)->AddBuildData( "quest", m_defQuest, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building HoipoiMix event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMHoipoiMix*)pObj)->AddBuildData( "pctrigger", m_defTrigger, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building HoipoiMix event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMHoipoiMix. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMPrivateShop" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMPrivateShop" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMPrivateShop*)pObj)->AddBuildData( "quest", m_defQuest, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building PrivateShop event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMPrivateShop*)pObj)->AddBuildData( "pctrigger", m_defTrigger, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building PrivateShop event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMPrivateShop. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMFreeBattle" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMFreeBattle" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMFreeBattle*)pObj)->AddBuildData( "quest", m_defQuest, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building FreeBattle event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMFreeBattle*)pObj)->AddBuildData( "pctrigger", m_defTrigger, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building FreeBattle event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMFreeBattle. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMItemIdentity" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMItemIdentity" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMItemIdentity*)pObj)->AddBuildData( "quest", m_defQuest, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building item identity event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMItemIdentity*)pObj)->AddBuildData( "pctrigger", m_defTrigger, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building item identity event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMItemIdentity. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMUseMail" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMUseMail" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMUseMail*)pObj)->AddBuildData( "quest", m_defQuest, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building use mail event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMUseMail*)pObj)->AddBuildData( "pctrigger", m_defTrigger, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building use mail event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMUseMail. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMParty" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMParty" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMParty*)pObj)->AddBuildData( "quest", m_defQuest, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building party event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMParty*)pObj)->AddBuildData( "pctrigger", m_defTrigger, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building party event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMParty. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	return true;
}
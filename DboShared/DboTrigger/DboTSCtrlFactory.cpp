#include "precomp_trigger.h"
#include "DboTSCtrlFactory.h"
#include "DboTSCtrlFactoryType.h"


/**
	Shared controller factory
*/


void CDboTSCtrlFactory::RegisterCtrlType( void )
{
	CNtlTSControlFactory::RegisterCtrlType();

	CNtlTSControlFactoryType_CNtlTSControlObject* pType;

	pType = new CDboTSControlFactoryType_CDboTSEMObject;
	m_defCtrlTypeList[pType->GetKeyword()] = pType;

	pType = new CDboTSControlFactoryType_CDboTSEMNPC;
	m_defCtrlTypeList[pType->GetKeyword()] = pType;

	pType = new CDboTSControlFactoryType_CDboTSEMItem;
	m_defCtrlTypeList[pType->GetKeyword()] = pType;

	pType = new CDboTSControlFactoryType_CDboTSEMSvrEvt;
	m_defCtrlTypeList[pType->GetKeyword()] = pType;

	pType = new CDboTSControlFactoryType_CDboTSEMSkill;
	m_defCtrlTypeList[pType->GetKeyword()] = pType;

	pType = new CDboTSControlFactoryType_CDboTSEMColRgn;
	m_defCtrlTypeList[pType->GetKeyword()] = pType;

	pType = new CDboTSControlFactoryType_CDboTSEMRB;
	m_defCtrlTypeList[pType->GetKeyword()] = pType;

	pType = new CDboTSControlFactoryType_CDboTSEMMob;
	m_defCtrlTypeList[pType->GetKeyword()] = pType;

	pType = new CDboTSControlFactoryType_CDboTSEMBindStone;
	m_defCtrlTypeList[pType->GetKeyword()] = pType;

	pType = new CDboTSControlFactoryType_CDboTSEMSearchQuest;
	m_defCtrlTypeList[pType->GetKeyword()] = pType;

	pType = new CDboTSControlFactoryType_CDboTSEMItemUpgrade;
	m_defCtrlTypeList[pType->GetKeyword()] = pType;

	pType = new CDboTSControlFactoryType_CDboTSEMTeleport;
	m_defCtrlTypeList[pType->GetKeyword()] = pType;

	pType = new CDboTSControlFactoryType_CDboTSEMBudokai;
	m_defCtrlTypeList[pType->GetKeyword()] = pType;

	pType = new CDboTSControlFactoryType_CDboTSEMSlotMachine;
	m_defCtrlTypeList[pType->GetKeyword()] = pType;

	pType = new CDboTSControlFactoryType_CDboTSEMHoipoiMix;
	m_defCtrlTypeList[pType->GetKeyword()] = pType;

	pType = new CDboTSControlFactoryType_CDboTSEMPrivateShop;
	m_defCtrlTypeList[pType->GetKeyword()] = pType;

	pType = new CDboTSControlFactoryType_CDboTSEMFreeBattle;
	m_defCtrlTypeList[pType->GetKeyword()] = pType;

	pType = new CDboTSControlFactoryType_CDboTSEMItemIdentity;
	m_defCtrlTypeList[pType->GetKeyword()] = pType;

	pType = new CDboTSControlFactoryType_CDboTSEMUseMail;
	m_defCtrlTypeList[pType->GetKeyword()] = pType;

	pType = new CDboTSControlFactoryType_CDboTSEMParty;
	m_defCtrlTypeList[pType->GetKeyword()] = pType;


	pType = new CDboTSControlFactoryType_CDboTSEMFLink;
	m_defCtrlTypeList[pType->GetKeyword()] = pType;

	pType = new CDboTSControlFactoryType_CDboTSEMSideIcon;
	m_defCtrlTypeList[pType->GetKeyword()] = pType;

	pType = new CDboTSControlFactoryType_CDboTSEMLevelCheck;
	m_defCtrlTypeList[pType->GetKeyword()] = pType;

	pType = new CDboTSControlFactoryType_CDboTSEMQuest;
	m_defCtrlTypeList[pType->GetKeyword()] = pType;

	pType = new CDboTSControlFactoryType_CDboTSEMDialogOpen;
	m_defCtrlTypeList[pType->GetKeyword()] = pType;


	pType = new CDboTSControlFactoryType_CDboTSTCtrl;
	m_defCtrlTypeList[pType->GetKeyword()] = pType;

	pType = new CDboTSControlFactoryType_CDboTSQCtrl;
	m_defCtrlTypeList[pType->GetKeyword()] = pType;
}

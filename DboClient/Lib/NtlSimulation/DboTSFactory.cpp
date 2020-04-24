#include "precomp_ntlsimulation.h"
#include "DboTSFactory.h"
#include "DboTSFactoryType.h"
#include "NtlDebug.h"


/**
	Control factory
*/


void CDboTSCCtrlFactory::RegisterCtrlType( void )
{
	CDboTSCtrlFactory::RegisterCtrlType();

	CNtlTSControlFactoryType_CNtlTSControlObject* pType;

	pType = NTL_NEW CDboTSControlFactoryType_CDboTSCQCtrl;
	m_defCtrlTypeList[pType->GetKeyword()] = pType;

	pType = NTL_NEW CDboTSControlFactoryType_CDboTSCTCtrl;
	m_defCtrlTypeList[pType->GetKeyword()] = pType;
}


/**
	UI factory
*/


void CDboTSCUIFactory::RegisterUIType( void )
{
	CDboTSUIFactory::RegisterUIType();

	CNtlTSUIFactoryType_CNtlTSUIObject* pType;

	pType = NTL_NEW CDboTSUIFactoryType_CDboTSCQRecv;
	m_defUITypeList[pType->GetKeyword()] = pType;

	pType = NTL_NEW CDboTSUIFactoryType_CDboTSCQAgency;
	m_defUITypeList[pType->GetKeyword()] = pType;

	pType = NTL_NEW CDboTSUIFactoryType_CDboTSCTRecv;
	m_defUITypeList[pType->GetKeyword()] = pType;

	pType = NTL_NEW CDboTSUIFactoryType_CDboTSCTAgency;
	m_defUITypeList[pType->GetKeyword()] = pType;

	pType = NTL_NEW CDboTSUIFactoryType_CDboTSCObjAgency;
	m_defUITypeList[pType->GetKeyword()] = pType;

	pType = NTL_NEW CDboTSUIFactoryType_CDboTSCMain;
	m_defUITypeList[pType->GetKeyword()] = pType;
}

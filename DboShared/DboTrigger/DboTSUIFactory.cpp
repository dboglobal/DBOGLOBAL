#include "precomp_trigger.h"
#include "DboTSUIFactory.h"
#include "DboTSUIFactoryType.h"


/**
	UI factory
*/


void CDboTSUIFactory::RegisterUIType( void )
{
	CNtlTSUIFactory::RegisterUIType();

	CNtlTSUIFactoryType_CNtlTSUIObject* pType;

	pType = new CDboTSUIFactoryType_CDboTSTRecv;
	m_defUITypeList[pType->GetKeyword()] = pType;

	pType = new CDboTSUIFactoryType_CDboTSQRecv;
	m_defUITypeList[pType->GetKeyword()] = pType;

	pType = new CDboTSUIFactoryType_CDboTSTAgency;
	m_defUITypeList[pType->GetKeyword()] = pType;

	pType = new CDboTSUIFactoryType_CDboTSQAgency;
	m_defUITypeList[pType->GetKeyword()] = pType;

	pType = new CDboTSUIFactoryType_CDboTSMain;
	m_defUITypeList[pType->GetKeyword()] = pType;
}

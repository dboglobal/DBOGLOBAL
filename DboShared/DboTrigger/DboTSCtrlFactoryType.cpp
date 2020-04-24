#include "precomp_trigger.h"
#include "DboTSCtrlFactoryType.h"
#include "DboTSEMObject.h"
#include "DboTSEMNPC.h"
#include "DboTSEMItem.h"
#include "DboTSEMSvrEvt.h"
#include "DboTSEMSkill.h"
#include "DboTSEMColRgn.h"
#include "DboTSEMRB.h"
#include "DboTSEMMob.h"
#include "DboTSEMBindStone.h"
#include "DboTSEMSearchQuest.h"
#include "DboTSEMItemUpgrade.h"
#include "DboTSEMTeleport.h"
#include "DboTSEMBudokai.h"
#include "DboTSEMSlotMachine.h"
#include "DboTSEMHoipoiMix.h"
#include "DboTSEMPrivateShop.h"
#include "DboTSEMFreeBattle.h"
#include "DboTSEMItemIdentity.h"
#include "DboTSEMUseMail.h"
#include "DboTSEMParty.h"

#include "DboTSEMFLink.h"
#include "DboTSEMSideIcon.h"
#include "DboTSEMLevelCheck.h"
#include "DboTSEMQuest.h"
#include "DboTSEMDialogOpen.h"

#include "DboTSTCtrl.h"
#include "DboTSQCtrl.h"


/** 
	Control 객체를 Runtime 시에 생성하기 위한 control type class 들
*/


//////////////////////////////////////////////////////////////////////////


CDboTSControlFactoryType_CDboTSEMObject::CDboTSControlFactoryType_CDboTSEMObject( void )
{
}

CNtlTSControlObject* CDboTSControlFactoryType_CDboTSEMObject::CreateObj( void )
{
	return new CDboTSEMObject;
}

void CDboTSControlFactoryType_CDboTSEMObject::DeleteObj( CNtlTSControlObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CDboTSControlFactoryType_CDboTSEMObject::GetKeyword( void ) const
{
	return CDboTSEMObject::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CDboTSControlFactoryType_CDboTSEMNPC::CDboTSControlFactoryType_CDboTSEMNPC( void )
{
}

CNtlTSControlObject* CDboTSControlFactoryType_CDboTSEMNPC::CreateObj( void )
{
	return new CDboTSEMNPC;
}

void CDboTSControlFactoryType_CDboTSEMNPC::DeleteObj( CNtlTSControlObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CDboTSControlFactoryType_CDboTSEMNPC::GetKeyword( void ) const
{
	return CDboTSEMNPC::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CDboTSControlFactoryType_CDboTSEMItem::CDboTSControlFactoryType_CDboTSEMItem( void )
{
}

CNtlTSControlObject* CDboTSControlFactoryType_CDboTSEMItem::CreateObj( void )
{
	return new CDboTSEMItem;
}

void CDboTSControlFactoryType_CDboTSEMItem::DeleteObj( CNtlTSControlObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CDboTSControlFactoryType_CDboTSEMItem::GetKeyword( void ) const
{
	return CDboTSEMItem::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CDboTSControlFactoryType_CDboTSEMSvrEvt::CDboTSControlFactoryType_CDboTSEMSvrEvt( void )
{
}

CNtlTSControlObject* CDboTSControlFactoryType_CDboTSEMSvrEvt::CreateObj( void )
{
	return new CDboTSEMSvrEvt;
}

void CDboTSControlFactoryType_CDboTSEMSvrEvt::DeleteObj( CNtlTSControlObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CDboTSControlFactoryType_CDboTSEMSvrEvt::GetKeyword( void ) const
{
	return CDboTSEMSvrEvt::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CDboTSControlFactoryType_CDboTSEMSkill::CDboTSControlFactoryType_CDboTSEMSkill( void )
{
}

CNtlTSControlObject* CDboTSControlFactoryType_CDboTSEMSkill::CreateObj( void )
{
	return new CDboTSEMSkill;
}

void CDboTSControlFactoryType_CDboTSEMSkill::DeleteObj( CNtlTSControlObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CDboTSControlFactoryType_CDboTSEMSkill::GetKeyword( void ) const
{
	return CDboTSEMSkill::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CDboTSControlFactoryType_CDboTSEMColRgn::CDboTSControlFactoryType_CDboTSEMColRgn( void )
{
}

CNtlTSControlObject* CDboTSControlFactoryType_CDboTSEMColRgn::CreateObj( void )
{
	return new CDboTSEMColRgn;
}

void CDboTSControlFactoryType_CDboTSEMColRgn::DeleteObj( CNtlTSControlObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CDboTSControlFactoryType_CDboTSEMColRgn::GetKeyword( void ) const
{
	return CDboTSEMColRgn::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CDboTSControlFactoryType_CDboTSEMRB::CDboTSControlFactoryType_CDboTSEMRB( void )
{
}

CNtlTSControlObject* CDboTSControlFactoryType_CDboTSEMRB::CreateObj( void )
{
	return new CDboTSEMRB;
}

void CDboTSControlFactoryType_CDboTSEMRB::DeleteObj( CNtlTSControlObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CDboTSControlFactoryType_CDboTSEMRB::GetKeyword( void ) const
{
	return CDboTSEMRB::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CDboTSControlFactoryType_CDboTSEMMob::CDboTSControlFactoryType_CDboTSEMMob( void )
{
}

CNtlTSControlObject* CDboTSControlFactoryType_CDboTSEMMob::CreateObj( void )
{
	return new CDboTSEMMob;
}

void CDboTSControlFactoryType_CDboTSEMMob::DeleteObj( CNtlTSControlObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CDboTSControlFactoryType_CDboTSEMMob::GetKeyword( void ) const
{
	return CDboTSEMMob::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CDboTSControlFactoryType_CDboTSEMBindStone::CDboTSControlFactoryType_CDboTSEMBindStone( void )
{
}

CNtlTSControlObject* CDboTSControlFactoryType_CDboTSEMBindStone::CreateObj( void )
{
	return new CDboTSEMBindStone;
}

void CDboTSControlFactoryType_CDboTSEMBindStone::DeleteObj( CNtlTSControlObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CDboTSControlFactoryType_CDboTSEMBindStone::GetKeyword( void ) const
{
	return CDboTSEMBindStone::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CDboTSControlFactoryType_CDboTSEMSearchQuest::CDboTSControlFactoryType_CDboTSEMSearchQuest( void )
{
}

CNtlTSControlObject* CDboTSControlFactoryType_CDboTSEMSearchQuest::CreateObj( void )
{
	return new CDboTSEMSearchQuest;
}

void CDboTSControlFactoryType_CDboTSEMSearchQuest::DeleteObj( CNtlTSControlObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CDboTSControlFactoryType_CDboTSEMSearchQuest::GetKeyword( void ) const
{
	return CDboTSEMSearchQuest::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CDboTSControlFactoryType_CDboTSEMItemUpgrade::CDboTSControlFactoryType_CDboTSEMItemUpgrade( void )
{
}

CNtlTSControlObject* CDboTSControlFactoryType_CDboTSEMItemUpgrade::CreateObj( void )
{
	return new CDboTSEMItemUpgrade;
}

void CDboTSControlFactoryType_CDboTSEMItemUpgrade::DeleteObj( CNtlTSControlObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CDboTSControlFactoryType_CDboTSEMItemUpgrade::GetKeyword( void ) const
{
	return CDboTSEMItemUpgrade::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CDboTSControlFactoryType_CDboTSEMTeleport::CDboTSControlFactoryType_CDboTSEMTeleport( void )
{
}

CNtlTSControlObject* CDboTSControlFactoryType_CDboTSEMTeleport::CreateObj( void )
{
	return new CDboTSEMTeleport;
}

void CDboTSControlFactoryType_CDboTSEMTeleport::DeleteObj( CNtlTSControlObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CDboTSControlFactoryType_CDboTSEMTeleport::GetKeyword( void ) const
{
	return CDboTSEMTeleport::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CDboTSControlFactoryType_CDboTSEMBudokai::CDboTSControlFactoryType_CDboTSEMBudokai( void )
{
}

CNtlTSControlObject* CDboTSControlFactoryType_CDboTSEMBudokai::CreateObj( void )
{
	return new CDboTSEMBudokai;
}

void CDboTSControlFactoryType_CDboTSEMBudokai::DeleteObj( CNtlTSControlObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CDboTSControlFactoryType_CDboTSEMBudokai::GetKeyword( void ) const
{
	return CDboTSEMBudokai::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CDboTSControlFactoryType_CDboTSEMSlotMachine::CDboTSControlFactoryType_CDboTSEMSlotMachine( void )
{
}

CNtlTSControlObject* CDboTSControlFactoryType_CDboTSEMSlotMachine::CreateObj( void )
{
	return new CDboTSEMSlotMachine;
}

void CDboTSControlFactoryType_CDboTSEMSlotMachine::DeleteObj( CNtlTSControlObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CDboTSControlFactoryType_CDboTSEMSlotMachine::GetKeyword( void ) const
{
	return CDboTSEMSlotMachine::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CDboTSControlFactoryType_CDboTSEMHoipoiMix::CDboTSControlFactoryType_CDboTSEMHoipoiMix( void )
{
}

CNtlTSControlObject* CDboTSControlFactoryType_CDboTSEMHoipoiMix::CreateObj( void )
{
	return new CDboTSEMHoipoiMix;
}

void CDboTSControlFactoryType_CDboTSEMHoipoiMix::DeleteObj( CNtlTSControlObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CDboTSControlFactoryType_CDboTSEMHoipoiMix::GetKeyword( void ) const
{
	return CDboTSEMHoipoiMix::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CDboTSControlFactoryType_CDboTSEMPrivateShop::CDboTSControlFactoryType_CDboTSEMPrivateShop( void )
{
}

CNtlTSControlObject* CDboTSControlFactoryType_CDboTSEMPrivateShop::CreateObj( void )
{
	return new CDboTSEMPrivateShop;
}

void CDboTSControlFactoryType_CDboTSEMPrivateShop::DeleteObj( CNtlTSControlObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CDboTSControlFactoryType_CDboTSEMPrivateShop::GetKeyword( void ) const
{
	return CDboTSEMPrivateShop::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CDboTSControlFactoryType_CDboTSEMFreeBattle::CDboTSControlFactoryType_CDboTSEMFreeBattle( void )
{
}

CNtlTSControlObject* CDboTSControlFactoryType_CDboTSEMFreeBattle::CreateObj( void )
{
	return new CDboTSEMFreeBattle;
}

void CDboTSControlFactoryType_CDboTSEMFreeBattle::DeleteObj( CNtlTSControlObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CDboTSControlFactoryType_CDboTSEMFreeBattle::GetKeyword( void ) const
{
	return CDboTSEMFreeBattle::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CDboTSControlFactoryType_CDboTSEMItemIdentity::CDboTSControlFactoryType_CDboTSEMItemIdentity( void )
{
}

CNtlTSControlObject* CDboTSControlFactoryType_CDboTSEMItemIdentity::CreateObj( void )
{
	return new CDboTSEMItemIdentity;
}

void CDboTSControlFactoryType_CDboTSEMItemIdentity::DeleteObj( CNtlTSControlObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CDboTSControlFactoryType_CDboTSEMItemIdentity::GetKeyword( void ) const
{
	return CDboTSEMItemIdentity::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CDboTSControlFactoryType_CDboTSEMUseMail::CDboTSControlFactoryType_CDboTSEMUseMail( void )
{
}

CNtlTSControlObject* CDboTSControlFactoryType_CDboTSEMUseMail::CreateObj( void )
{
	return new CDboTSEMUseMail;
}

void CDboTSControlFactoryType_CDboTSEMUseMail::DeleteObj( CNtlTSControlObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CDboTSControlFactoryType_CDboTSEMUseMail::GetKeyword( void ) const
{
	return CDboTSEMUseMail::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CDboTSControlFactoryType_CDboTSEMParty::CDboTSControlFactoryType_CDboTSEMParty( void )
{
}

CNtlTSControlObject* CDboTSControlFactoryType_CDboTSEMParty::CreateObj( void )
{
	return new CDboTSEMParty;
}

void CDboTSControlFactoryType_CDboTSEMParty::DeleteObj( CNtlTSControlObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CDboTSControlFactoryType_CDboTSEMParty::GetKeyword( void ) const
{
	return CDboTSEMParty::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CDboTSControlFactoryType_CDboTSEMFLink::CDboTSControlFactoryType_CDboTSEMFLink(void)
{
}

CNtlTSControlObject* CDboTSControlFactoryType_CDboTSEMFLink::CreateObj(void)
{
	return new CDboTSEMFLink;
}

void CDboTSControlFactoryType_CDboTSEMFLink::DeleteObj(CNtlTSControlObject*& pObj)
{
	if (pObj)
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CDboTSControlFactoryType_CDboTSEMFLink::GetKeyword(void) const
{
	return CDboTSEMFLink::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CDboTSControlFactoryType_CDboTSEMSideIcon::CDboTSControlFactoryType_CDboTSEMSideIcon(void)
{
}

CNtlTSControlObject* CDboTSControlFactoryType_CDboTSEMSideIcon::CreateObj(void)
{
	return new CDboTSEMSideIcon;
}

void CDboTSControlFactoryType_CDboTSEMSideIcon::DeleteObj(CNtlTSControlObject*& pObj)
{
	if (pObj)
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CDboTSControlFactoryType_CDboTSEMSideIcon::GetKeyword(void) const
{
	return CDboTSEMSideIcon::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CDboTSControlFactoryType_CDboTSEMLevelCheck::CDboTSControlFactoryType_CDboTSEMLevelCheck(void)
{
}

CNtlTSControlObject* CDboTSControlFactoryType_CDboTSEMLevelCheck::CreateObj(void)
{
	return new CDboTSEMLevelCheck;
}

void CDboTSControlFactoryType_CDboTSEMLevelCheck::DeleteObj(CNtlTSControlObject*& pObj)
{
	if (pObj)
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CDboTSControlFactoryType_CDboTSEMLevelCheck::GetKeyword(void) const
{
	return CDboTSEMLevelCheck::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CDboTSControlFactoryType_CDboTSEMQuest::CDboTSControlFactoryType_CDboTSEMQuest(void)
{
}

CNtlTSControlObject* CDboTSControlFactoryType_CDboTSEMQuest::CreateObj(void)
{
	return new CDboTSEMQuest;
}

void CDboTSControlFactoryType_CDboTSEMQuest::DeleteObj(CNtlTSControlObject*& pObj)
{
	if (pObj)
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CDboTSControlFactoryType_CDboTSEMQuest::GetKeyword(void) const
{
	return CDboTSEMQuest::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CDboTSControlFactoryType_CDboTSEMDialogOpen::CDboTSControlFactoryType_CDboTSEMDialogOpen(void)
{
}

CNtlTSControlObject* CDboTSControlFactoryType_CDboTSEMDialogOpen::CreateObj(void)
{
	return new CDboTSEMDialogOpen;
}

void CDboTSControlFactoryType_CDboTSEMDialogOpen::DeleteObj(CNtlTSControlObject*& pObj)
{
	if (pObj)
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CDboTSControlFactoryType_CDboTSEMDialogOpen::GetKeyword(void) const
{
	return CDboTSEMDialogOpen::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////







CDboTSControlFactoryType_CDboTSTCtrl::CDboTSControlFactoryType_CDboTSTCtrl( void )
{
}

CNtlTSControlObject* CDboTSControlFactoryType_CDboTSTCtrl::CreateObj( void )
{
	return new CDboTSTCtrl;
}

void CDboTSControlFactoryType_CDboTSTCtrl::DeleteObj( CNtlTSControlObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CDboTSControlFactoryType_CDboTSTCtrl::GetKeyword( void ) const
{
	return CDboTSTCtrl::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CDboTSControlFactoryType_CDboTSQCtrl::CDboTSControlFactoryType_CDboTSQCtrl( void )
{
}

CNtlTSControlObject* CDboTSControlFactoryType_CDboTSQCtrl::CreateObj( void )
{
	return new CDboTSQCtrl;
}

void CDboTSControlFactoryType_CDboTSQCtrl::DeleteObj( CNtlTSControlObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CDboTSControlFactoryType_CDboTSQCtrl::GetKeyword( void ) const
{
	return CDboTSQCtrl::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////

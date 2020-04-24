#include "precomp_trigger.h"
#include "DboTSEventFactoryType.h"


/** 
	Event 관련 객체를 Runtime 시에 생성하기 위한 event entity type class 들
*/


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSClickNPC::CDboTSEntityFactoryType_CDboTSClickNPC( void )
{
	m_defPropertyList["npcidx"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSClickNPC::CreateObj( void )
{
	return new CDboTSClickNPC;
}

void CDboTSEntityFactoryType_CDboTSClickNPC::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSClickObject::CDboTSEntityFactoryType_CDboTSClickObject( void )
{
	m_defPropertyList["widx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["objidx"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSClickObject::CreateObj( void )
{
	return new CDboTSClickObject;
}

void CDboTSEntityFactoryType_CDboTSClickObject::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSColObject::CDboTSEntityFactoryType_CDboTSColObject( void )
{
	m_defPropertyList["widx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["objidx"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSColObject::CreateObj( void )
{
	return new CDboTSColObject;
}

void CDboTSEntityFactoryType_CDboTSColObject::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSItemUse::CDboTSEntityFactoryType_CDboTSItemUse( void )
{
	m_defPropertyList["idx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["ttype"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["tidx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSItemUse::CreateObj( void )
{
	return new CDboTSItemUse;
}

void CDboTSEntityFactoryType_CDboTSItemUse::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSItemGet::CDboTSEntityFactoryType_CDboTSItemGet( void )
{
	m_defPropertyList["idx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSItemGet::CreateObj( void )
{
	return new CDboTSItemGet;
}

void CDboTSEntityFactoryType_CDboTSItemGet::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSItemEquip::CDboTSEntityFactoryType_CDboTSItemEquip( void )
{
	m_defPropertyList["idx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSItemEquip::CreateObj( void )
{
	return new CDboTSItemEquip;
}

void CDboTSEntityFactoryType_CDboTSItemEquip::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSScoutUse::CDboTSEntityFactoryType_CDboTSScoutUse( void )
{
	m_defPropertyList["idx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["tt"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["tidx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSScoutUse::CreateObj( void )
{
	return new CDboTSScoutUse;
}

void CDboTSEntityFactoryType_CDboTSScoutUse::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSRcvSvrEvt::CDboTSEntityFactoryType_CDboTSRcvSvrEvt( void )
{
	m_defPropertyList["id"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSRcvSvrEvt::CreateObj( void )
{
	return new CDboTSRcvSvrEvt;
}

void CDboTSEntityFactoryType_CDboTSRcvSvrEvt::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSSkillUse::CDboTSEntityFactoryType_CDboTSSkillUse( void )
{
	m_defPropertyList["stype"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["idx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["ttype"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["tidx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["rp"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSSkillUse::CreateObj( void )
{
	return new CDboTSSkillUse;
}

void CDboTSEntityFactoryType_CDboTSSkillUse::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSColRgn::CDboTSEntityFactoryType_CDboTSColRgn( void )
{
	m_defPropertyList["widx"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["cct"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["crt"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["x"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["z"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["r"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["x0"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["z0"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["x1"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["z1"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["x2"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["z2"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["x3"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["z3"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSColRgn::CreateObj( void )
{
	return new CDboTSColRgn;
}

void CDboTSEntityFactoryType_CDboTSColRgn::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSRB::CDboTSEntityFactoryType_CDboTSRB( void )
{
	m_defPropertyList["idx"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["cond"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSRB::CreateObj( void )
{
	return new CDboTSRB;
}

void CDboTSEntityFactoryType_CDboTSRB::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSClickMob::CDboTSEntityFactoryType_CDboTSClickMob( void )
{
	m_defPropertyList["mgidx"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSClickMob::CreateObj( void )
{
	return new CDboTSClickMob;
}

void CDboTSEntityFactoryType_CDboTSClickMob::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSSearchQuest::CDboTSEntityFactoryType_CDboTSSearchQuest( void )
{
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSSearchQuest::CreateObj( void )
{
	return new CDboTSSearchQuest;
}

void CDboTSEntityFactoryType_CDboTSSearchQuest::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSBindStone::CDboTSEntityFactoryType_CDboTSBindStone( void )
{
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSBindStone::CreateObj( void )
{
	return new CDboTSBindStone;
}

void CDboTSEntityFactoryType_CDboTSBindStone::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSItemUpgrade::CDboTSEntityFactoryType_CDboTSItemUpgrade( void )
{
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSItemUpgrade::CreateObj( void )
{
	return new CDboTSItemUpgrade;
}

void CDboTSEntityFactoryType_CDboTSItemUpgrade::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSTeleport::CDboTSEntityFactoryType_CDboTSTeleport( void )
{
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSTeleport::CreateObj( void )
{
	return new CDboTSTeleport;
}

void CDboTSEntityFactoryType_CDboTSTeleport::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSBudokai::CDboTSEntityFactoryType_CDboTSBudokai( void )
{
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSBudokai::CreateObj( void )
{
	return new CDboTSBudokai;
}

void CDboTSEntityFactoryType_CDboTSBudokai::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSSlotMachine::CDboTSEntityFactoryType_CDboTSSlotMachine( void )
{
	m_defPropertyList["st"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSSlotMachine::CreateObj( void )
{
	return new CDboTSSlotMachine;
}

void CDboTSEntityFactoryType_CDboTSSlotMachine::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSHoipoiMix::CDboTSEntityFactoryType_CDboTSHoipoiMix( void )
{
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSHoipoiMix::CreateObj( void )
{
	return new CDboTSHoipoiMix;
}

void CDboTSEntityFactoryType_CDboTSHoipoiMix::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSPrivateShop::CDboTSEntityFactoryType_CDboTSPrivateShop( void )
{
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSPrivateShop::CreateObj( void )
{
	return new CDboTSPrivateShop;
}

void CDboTSEntityFactoryType_CDboTSPrivateShop::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSFreeBattle::CDboTSEntityFactoryType_CDboTSFreeBattle( void )
{
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSFreeBattle::CreateObj( void )
{
	return new CDboTSFreeBattle;
}

void CDboTSEntityFactoryType_CDboTSFreeBattle::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSItemIdentity::CDboTSEntityFactoryType_CDboTSItemIdentity( void )
{
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSItemIdentity::CreateObj( void )
{
	return new CDboTSItemIdentity;
}

void CDboTSEntityFactoryType_CDboTSItemIdentity::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSUseMail::CDboTSEntityFactoryType_CDboTSUseMail( void )
{
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSUseMail::CreateObj( void )
{
	return new CDboTSUseMail;
}

void CDboTSEntityFactoryType_CDboTSUseMail::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSParty::CDboTSEntityFactoryType_CDboTSParty( void )
{
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSParty::CreateObj( void )
{
	return new CDboTSParty;
}

void CDboTSEntityFactoryType_CDboTSParty::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSClickSideIcon::CDboTSEntityFactoryType_CDboTSClickSideIcon( void )
{
	m_defPropertyList["sitype"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSClickSideIcon::CreateObj( void )
{
	return new CDboTSClickSideIcon;
}

void CDboTSEntityFactoryType_CDboTSClickSideIcon::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSLevelCheck::CDboTSEntityFactoryType_CDboTSLevelCheck(void)
{
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSLevelCheck::CreateObj(void)
{
	return new CDboTSLevelCheck;
}

void CDboTSEntityFactoryType_CDboTSLevelCheck::DeleteObj(CNtlTSScrObject*& pObj)
{
	if (pObj)
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSQuest::CDboTSEntityFactoryType_CDboTSQuest(void)
{
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSQuest::CreateObj(void)
{
	return new CDboTSQuest;
}

void CDboTSEntityFactoryType_CDboTSQuest::DeleteObj(CNtlTSScrObject*& pObj)
{
	if (pObj)
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////



CDboTSEntityFactoryType_CDboTSDialogOpen::CDboTSEntityFactoryType_CDboTSDialogOpen( void )
{
	m_defPropertyList["dotype"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSDialogOpen::CreateObj( void )
{
	return new CDboTSDialogOpen;
}

void CDboTSEntityFactoryType_CDboTSDialogOpen::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////




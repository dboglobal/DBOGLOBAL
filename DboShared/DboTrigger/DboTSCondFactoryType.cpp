#include "precomp_trigger.h"
#include "DboTSCondFactoryType.h"


/** 
	Condition 관련 객체를 Runtime 시에 생성하기 위한 condition entity type class 들
*/


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSCheckLvl::CDboTSEntityFactoryType_CDboTSCheckLvl( void )
{
	m_defPropertyList["minlvl"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["maxlvl"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSCheckLvl::CreateObj( void )
{
	return new CDboTSCheckLvl;
}

void CDboTSEntityFactoryType_CDboTSCheckLvl::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSCheckPCCls::CDboTSEntityFactoryType_CDboTSCheckPCCls( void )
{
	m_defPropertyList["clsflg"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSCheckPCCls::CreateObj( void )
{
	return new CDboTSCheckPCCls;
}

void CDboTSEntityFactoryType_CDboTSCheckPCCls::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSCheckPCRace::CDboTSEntityFactoryType_CDboTSCheckPCRace( void )
{
	m_defPropertyList["raceflg"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSCheckPCRace::CreateObj( void )
{
	return new CDboTSCheckPCRace;
}

void CDboTSEntityFactoryType_CDboTSCheckPCRace::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSCheckQItem::CDboTSEntityFactoryType_CDboTSCheckQItem( void )
{
	m_defPropertyList["ct"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["iidx"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["icnt"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSCheckQItem::CreateObj( void )
{
	return new CDboTSCheckQItem;
}

void CDboTSEntityFactoryType_CDboTSCheckQItem::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSCheckClrQst::CDboTSEntityFactoryType_CDboTSCheckClrQst( void )
{
	m_defPropertyList["and"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["or"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;

	m_defPropertyList["flink"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["not"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["flinknextqid"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSCheckClrQst::CreateObj( void )
{
	return new CDboTSCheckClrQst;
}

void CDboTSEntityFactoryType_CDboTSCheckClrQst::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSCheckItem::CDboTSEntityFactoryType_CDboTSCheckItem( void )
{
	m_defPropertyList["iidx"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["icnt"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSCheckItem::CreateObj( void )
{
	return new CDboTSCheckItem;
}

void CDboTSEntityFactoryType_CDboTSCheckItem::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSCheckSToCEvt::CDboTSEntityFactoryType_CDboTSCheckSToCEvt( void )
{
	m_defPropertyList["itype"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSCheckSToCEvt::CreateObj( void )
{
	return new CDboTSCheckSToCEvt;
}

void CDboTSEntityFactoryType_CDboTSCheckSToCEvt::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSCheckSSM::CDboTSEntityFactoryType_CDboTSCheckSSM( void )
{
	m_defPropertyList["op"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["ssmid"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["var"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSCheckSSM::CreateObj( void )
{
	return new CDboTSCheckSSM;
}

void CDboTSEntityFactoryType_CDboTSCheckSSM::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSCheckWItem::CDboTSEntityFactoryType_CDboTSCheckWItem( void )
{
	m_defPropertyList["iidx"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSCheckWItem::CreateObj( void )
{
	return new CDboTSCheckWItem;
}

void CDboTSEntityFactoryType_CDboTSCheckWItem::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSCheckObjItem::CDboTSEntityFactoryType_CDboTSCheckObjItem( void )
{
	m_defPropertyList["tid"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSCheckObjItem::CreateObj( void )
{
	return new CDboTSCheckObjItem;
}

void CDboTSEntityFactoryType_CDboTSCheckObjItem::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSCheckInNPC::CDboTSEntityFactoryType_CDboTSCheckInNPC( void )
{
	m_defPropertyList["idx"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["rad"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSCheckInNPC::CreateObj( void )
{
	return new CDboTSCheckInNPC;
}

void CDboTSEntityFactoryType_CDboTSCheckInNPC::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSCheckObjState::CDboTSEntityFactoryType_CDboTSCheckObjState( void )
{
	m_defPropertyList["widx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["oidx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["mstate"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["osh"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSCheckObjState::CreateObj( void )
{
	return new CDboTSCheckObjState;
}

void CDboTSEntityFactoryType_CDboTSCheckObjState::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSCheckInWorld::CDboTSEntityFactoryType_CDboTSCheckInWorld( void )
{
	m_defPropertyList["widx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSCheckInWorld::CreateObj( void )
{
	return new CDboTSCheckInWorld;
}

void CDboTSEntityFactoryType_CDboTSCheckInWorld::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSCheckOPObject::CDboTSEntityFactoryType_CDboTSCheckOPObject( void )
{
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSCheckOPObject::CreateObj( void )
{
	return new CDboTSCheckOPObject;
}

void CDboTSEntityFactoryType_CDboTSCheckOPObject::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSCheckHasCoupon::CDboTSEntityFactoryType_CDboTSCheckHasCoupon( void )
{
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSCheckHasCoupon::CreateObj( void )
{
	return new CDboTSCheckHasCoupon;
}

void CDboTSEntityFactoryType_CDboTSCheckHasCoupon::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSCheckNPCDead::CDboTSEntityFactoryType_CDboTSCheckNPCDead( void )
{
	m_defPropertyList["idxlist"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSCheckNPCDead::CreateObj( void )
{
	return new CDboTSCheckNPCDead;
}

void CDboTSEntityFactoryType_CDboTSCheckNPCDead::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSCheckDistWithNPC::CDboTSEntityFactoryType_CDboTSCheckDistWithNPC( void )
{
	m_defPropertyList["dist"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["idxlist"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSCheckDistWithNPC::CreateObj( void )
{
	return new CDboTSCheckDistWithNPC;
}

void CDboTSEntityFactoryType_CDboTSCheckDistWithNPC::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSCheckSToCDelivery::CDboTSEntityFactoryType_CDboTSCheckSToCDelivery( void )
{
	m_defPropertyList["iidx0"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["icnt0"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["iidx1"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["icnt1"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["iidx2"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["icnt2"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSCheckSToCDelivery::CreateObj( void )
{
	return new CDboTSCheckSToCDelivery;
}

void CDboTSEntityFactoryType_CDboTSCheckSToCDelivery::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSCheckAttachObj::CDboTSEntityFactoryType_CDboTSCheckAttachObj( void )
{
	m_defPropertyList["widx"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["objidx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSCheckAttachObj::CreateObj( void )
{
	return new CDboTSCheckAttachObj;
}

void CDboTSEntityFactoryType_CDboTSCheckAttachObj::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSCheckProgQuest::CDboTSEntityFactoryType_CDboTSCheckProgQuest( void )
{
	m_defPropertyList["not"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["qid"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSCheckProgQuest::CreateObj( void )
{
	return new CDboTSCheckProgQuest;
}

void CDboTSEntityFactoryType_CDboTSCheckProgQuest::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSCheckReputation::CDboTSEntityFactoryType_CDboTSCheckReputation( void )
{
	m_defPropertyList["minrt"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["maxrt"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSCheckReputation::CreateObj( void )
{
	return new CDboTSCheckReputation;
}

void CDboTSEntityFactoryType_CDboTSCheckReputation::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSCheckBudokaiState::CDboTSEntityFactoryType_CDboTSCheckBudokaiState( void )
{
	m_defPropertyList["md"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["st"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSCheckBudokaiState::CreateObj( void )
{
	return new CDboTSCheckBudokaiState;
}

void CDboTSEntityFactoryType_CDboTSCheckBudokaiState::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSCheckCustomEvent::CDboTSEntityFactoryType_CDboTSCheckCustomEvent( void )
{
	m_defPropertyList["qid"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSCheckCustomEvent::CreateObj( void )
{
	return new CDboTSCheckCustomEvent;
}

void CDboTSEntityFactoryType_CDboTSCheckCustomEvent::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSCheckAvatarDead::CDboTSEntityFactoryType_CDboTSCheckAvatarDead( void )
{
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSCheckAvatarDead::CreateObj( void )
{
	return new CDboTSCheckAvatarDead;
}

void CDboTSEntityFactoryType_CDboTSCheckAvatarDead::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSCheckOutRGN::CDboTSEntityFactoryType_CDboTSCheckOutRGN( void )
{
	m_defPropertyList["widx"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["fx"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["fz"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["fr"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSCheckOutRGN::CreateObj( void )
{
	return new CDboTSCheckOutRGN;
}

void CDboTSEntityFactoryType_CDboTSCheckOutRGN::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


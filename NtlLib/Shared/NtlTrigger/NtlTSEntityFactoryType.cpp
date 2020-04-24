#include "precomp_trigger.h"
#include "NtlTSEntityFactoryType.h"


/** 
	Entity 를 Runtime 시에 생성하기 위한 entity type class 들
*/


//////////////////////////////////////////////////////////////////////////


CNtlTSEntityFactoryType_CNtlTSEntity::CNtlTSEntityFactoryType_CNtlTSEntity( void )
{
}

CNtlTSScrObject* CNtlTSEntityFactoryType_CNtlTSEntity::CreateObj( void )
{
	return new CNtlTSEntity;
}

void CNtlTSEntityFactoryType_CNtlTSEntity::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CNtlTSEntityFactoryType_CNtlTSEvent::CNtlTSEntityFactoryType_CNtlTSEvent( void )
{
}

CNtlTSScrObject* CNtlTSEntityFactoryType_CNtlTSEvent::CreateObj( void )
{
	return new CNtlTSEvent;
}

void CNtlTSEntityFactoryType_CNtlTSEvent::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CNtlTSEntityFactoryType_CNtlTSCond::CNtlTSEntityFactoryType_CNtlTSCond( void )
{
}

CNtlTSScrObject* CNtlTSEntityFactoryType_CNtlTSCond::CreateObj( void )
{
	return new CNtlTSCond;
}

void CNtlTSEntityFactoryType_CNtlTSCond::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CNtlTSEntityFactoryType_CNtlTSAction::CNtlTSEntityFactoryType_CNtlTSAction( void )
{
	m_defPropertyList["taid"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CNtlTSEntityFactoryType_CNtlTSAction::CreateObj( void )
{
	return new CNtlTSAction;
}

void CNtlTSEntityFactoryType_CNtlTSAction::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CNtlTSEntityFactoryType_CNtlTSCont::CNtlTSEntityFactoryType_CNtlTSCont( void )
{
	m_defPropertyList["cid"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["prelnk"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
}

CNtlTSScrObject* CNtlTSEntityFactoryType_CNtlTSCont::CreateObj( void )
{
	return new CNtlTSCont;
}

void CNtlTSEntityFactoryType_CNtlTSCont::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CNtlTSEntityFactoryType_CNtlTSGroup::CNtlTSEntityFactoryType_CNtlTSGroup( void )
{
	m_defPropertyList["gid"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CNtlTSEntityFactoryType_CNtlTSGroup::CreateObj( void )
{
	return new CNtlTSGroup;
}

void CNtlTSEntityFactoryType_CNtlTSGroup::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CNtlTSEntityFactoryType_CNtlTSTrigger::CNtlTSEntityFactoryType_CNtlTSTrigger( void )
{
	m_defPropertyList["tid"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["rq"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["sq"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["sm"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["title"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["qc"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;//new
}

CNtlTSScrObject* CNtlTSEntityFactoryType_CNtlTSTrigger::CreateObj( void )
{
	return new CNtlTSTrigger;
}

void CNtlTSEntityFactoryType_CNtlTSTrigger::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////

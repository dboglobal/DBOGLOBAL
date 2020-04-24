#include "precomp_trigger.h"
#include "DboTSContFactoryType.h"


/** 
	Container 관련 객체를 Runtime 시에 생성하기 위한 container type class 들
*/


//////////////////////////////////////////////////////////////////////////


CDboTSContFactoryType_CDboTSContGCond::CDboTSContFactoryType_CDboTSContGCond( void )
{
	m_defPropertyList["yeslnk"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["nolnk"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["rm"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSContFactoryType_CDboTSContGCond::CreateObj( void )
{
	return new CDboTSContGCond;
}

void CDboTSContFactoryType_CDboTSContGCond::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSContFactoryType_CDboTSContStart::CDboTSContFactoryType_CDboTSContStart( void )
{
	m_defPropertyList["stdiag"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSContFactoryType_CDboTSContStart::CreateObj( void )
{
	return new CDboTSContStart;
}

void CDboTSContFactoryType_CDboTSContStart::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSContFactoryType_CDboTSContGAct::CDboTSContFactoryType_CDboTSContGAct( void )
{
	m_defPropertyList["nextlnk"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["elnk"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSContFactoryType_CDboTSContGAct::CreateObj( void )
{
	return new CDboTSContGAct;
}

void CDboTSContFactoryType_CDboTSContGAct::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSContFactoryType_CDboTSContReward::CDboTSContFactoryType_CDboTSContReward( void )
{
	m_defPropertyList["nextlnk"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["canclnk"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["elnk"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;

	m_defPropertyList["rwdtype"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["ltime"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["desc"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;

	m_defPropertyList["usetbl"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["rwdtbl"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	//new
	m_defPropertyList["rwdzeny"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["rwdexp"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;

	int i = 0;

	// 기본 보상
	for ( i = 0; i < 4; ++i )
	{
		sprintf_s( g_NtlTSString, "dtype%d", i );
		m_defPropertyList[g_NtlTSString].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;

		sprintf_s( g_NtlTSString, "didx%d", i );
		m_defPropertyList[g_NtlTSString].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;

		sprintf_s( g_NtlTSString, "dval%d", i );
		m_defPropertyList[g_NtlTSString].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	}

	// 선택 보상
	for ( i = 0; i < 4; ++i )
	{
		sprintf_s( g_NtlTSString, "stype%d", i );
		m_defPropertyList[g_NtlTSString].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;

		sprintf_s( g_NtlTSString, "sidx%d", i );
		m_defPropertyList[g_NtlTSString].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;

		sprintf_s( g_NtlTSString, "sval%d", i );
		m_defPropertyList[g_NtlTSString].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	}
}

CNtlTSScrObject* CDboTSContFactoryType_CDboTSContReward::CreateObj( void )
{
	return new CDboTSContReward;
}

void CDboTSContFactoryType_CDboTSContReward::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSContFactoryType_CDboTSContEnd::CDboTSContFactoryType_CDboTSContEnd( void )
{
	m_defPropertyList["type"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSContFactoryType_CDboTSContEnd::CreateObj( void )
{
	return new CDboTSContEnd;
}

void CDboTSContFactoryType_CDboTSContEnd::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSContFactoryType_CDboTSContUsrSel::CDboTSContFactoryType_CDboTSContUsrSel( void )
{
	m_defPropertyList["ust"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["uspt"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["type"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["idx"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["desc"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;

	const std::string strId = "nid";
	const std::string strDesc = "ndesc";

	std::string strConcId, strConcDesc;

	for ( int i = 0; i < CDboTSContUsrSel::MAX_NEXT_LINK_COUNT; ++i )
	{
		sprintf_s( g_NtlTSString, "%d", i );

		strConcId = strId + g_NtlTSString;
		strConcDesc = strDesc + g_NtlTSString;

		m_defPropertyList[strConcId].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
		m_defPropertyList[strConcDesc].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	}

	m_defPropertyList["cancellnk"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["lilnk"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSContFactoryType_CDboTSContUsrSel::CreateObj( void )
{
	return new CDboTSContUsrSel;
}

void CDboTSContFactoryType_CDboTSContUsrSel::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSContFactoryType_CDboTSContNarration::CDboTSContFactoryType_CDboTSContNarration( void )
{
	m_defPropertyList["pt"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["ot"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["oi"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["os"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["dt"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["dg"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["gt"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["mlt"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["oklnk"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["cancellnk"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["lilnk"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSContFactoryType_CDboTSContNarration::CreateObj( void )
{
	return new CDboTSContNarration;
}

void CDboTSContFactoryType_CDboTSContNarration::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSContFactoryType_CDboTSContProposal::CDboTSContFactoryType_CDboTSContProposal( void )
{
	m_defPropertyList["ds"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["title"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["area"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["goal"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["grade"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["gtype"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["sort"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["cont"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["rwd"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["oklnk"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["cancellnk"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSContFactoryType_CDboTSContProposal::CreateObj( void )
{
	return new CDboTSContProposal;
}

void CDboTSContFactoryType_CDboTSContProposal::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSContFactoryType_CDboTSContSwitch::CDboTSContFactoryType_CDboTSContSwitch( void )
{
	const std::string strContID = "cid";
	const std::string strBranchID = "bid";

	std::string strCId, strBId ;

	for ( int i = 0; i < CDboTSContSwitch::MAX_NEXT_LINK_COUNT; ++i )
	{
		sprintf_s( g_NtlTSString, "%d", i );

		strCId = strContID + g_NtlTSString;
		strBId = strBranchID + g_NtlTSString;

		m_defPropertyList[strCId].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
		m_defPropertyList[strBId].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	}
}

CNtlTSScrObject* CDboTSContFactoryType_CDboTSContSwitch::CreateObj( void )
{
	return new CDboTSContSwitch;
}

void CDboTSContFactoryType_CDboTSContSwitch::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSContFactoryType_CDboTSContUnifiedNarration::CDboTSContFactoryType_CDboTSContUnifiedNarration( void )
{
	m_defPropertyList["idx"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["oklnk"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["cancellnk"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["lilnk"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSContFactoryType_CDboTSContUnifiedNarration::CreateObj( void )
{
	return new CDboTSContUnifiedNarration;
}

void CDboTSContFactoryType_CDboTSContUnifiedNarration::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////

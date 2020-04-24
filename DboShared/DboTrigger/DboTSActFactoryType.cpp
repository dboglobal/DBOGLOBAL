#include "precomp_trigger.h"
#include "DboTSActFactoryType.h"


/** 
	Action 관련 객체를 Runtime 시에 생성하기 위한 action entity type class 들
*/


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActQItem::CDboTSEntityFactoryType_CDboTSActQItem( void )
{
	m_defPropertyList["type"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;

	m_defPropertyList["iidx0"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["icnt0"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["iprob0"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;

	m_defPropertyList["iidx1"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["icnt1"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["iprob1"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;

	m_defPropertyList["iidx2"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["icnt2"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["iprob2"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActQItem::CreateObj( void )
{
	return new CDboTSActQItem;
}

void CDboTSEntityFactoryType_CDboTSActQItem::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActItem::CDboTSEntityFactoryType_CDboTSActItem( void )
{
	m_defPropertyList["type"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;

	m_defPropertyList["stype0"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["iidx0"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;

	m_defPropertyList["stype1"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["iidx1"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;

	m_defPropertyList["stype2"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["iidx2"].eType = CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActItem::CreateObj( void )
{
	return new CDboTSActItem;
}

void CDboTSEntityFactoryType_CDboTSActItem::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActETimerS::CDboTSEntityFactoryType_CDboTSActETimerS( void )
{
	m_defPropertyList["sort"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["time"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["tgid"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActETimerS::CreateObj( void )
{
	return new CDboTSActETimerS;
}

void CDboTSEntityFactoryType_CDboTSActETimerS::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActETimerE::CDboTSEntityFactoryType_CDboTSActETimerE( void )
{
	m_defPropertyList["tceid"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["taeid"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActETimerE::CreateObj( void )
{
	return new CDboTSActETimerE;
}

void CDboTSEntityFactoryType_CDboTSActETimerE::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActSToCEvt::CDboTSEntityFactoryType_CDboTSActSToCEvt( void )
{
	m_defPropertyList["etype"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["apptype"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["eitype"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;

	m_defPropertyList["idx0"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["cnt0"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["idx1"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["cnt1"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["idx2"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["cnt2"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["nidx0"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["rate0"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["qtidx"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;

	m_defPropertyList["otype0"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["widx0"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["tidx0"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["it0"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["dt0"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["iidx0"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;

	m_defPropertyList["otype1"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["widx1"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["tidx1"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["it1"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["dt1"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["iidx1"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;

	m_defPropertyList["otype2"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["widx2"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["tidx2"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["it2"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["dt2"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["iidx2"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;

	m_defPropertyList["ectype"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;

	m_defPropertyList["cd0"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["cd1"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;

	m_defPropertyList["esctype"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;

	m_defPropertyList["scd0"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActSToCEvt::CreateObj( void )
{
	return new CDboTSActSToCEvt;
}

void CDboTSEntityFactoryType_CDboTSActSToCEvt::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActNPCConv::CDboTSEntityFactoryType_CDboTSActNPCConv( void )
{
	m_defPropertyList["ctype"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["idx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["conv"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActNPCConv::CreateObj( void )
{
	return new CDboTSActNPCConv;
}

void CDboTSEntityFactoryType_CDboTSActNPCConv::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActRegQInfo::CDboTSEntityFactoryType_CDboTSActRegQInfo( void )
{
	m_defPropertyList["title"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["area"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["goal"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["grade"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["gtype"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["stype"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["sort"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["cont"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["rwd"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;

	m_defPropertyList["m0widx"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["m0fx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["m0fy"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["m0fz"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["m0ttip"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;

	m_defPropertyList["m1widx"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["m1fx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["m1fy"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["m1fz"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["m1ttip"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;

	m_defPropertyList["m2widx"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["m2fx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["m2fy"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["m2fz"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["m2ttip"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;

	//new
	m_defPropertyList["m3widx"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["m3fx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["m3fy"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["m3fz"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["m3ttip"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["m4widx"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["m4fx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["m4fy"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["m4fz"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["m4ttip"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;

	m_defPropertyList["m0pat"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["m1pat"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["m2pat"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["m3pat"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["m4pat"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["scitem"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActRegQInfo::CreateObj( void )
{
	return new CDboTSActRegQInfo;
}

void CDboTSEntityFactoryType_CDboTSActRegQInfo::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActDir::CDboTSEntityFactoryType_CDboTSActDir( void )
{
	m_defPropertyList["rng"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["type"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["idx"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActDir::CreateObj( void )
{
	return new CDboTSActDir;
}

void CDboTSEntityFactoryType_CDboTSActDir::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActOPCam::CDboTSEntityFactoryType_CDboTSActOPCam( void )
{
	m_defPropertyList["fname"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActOPCam::CreateObj( void )
{
	return new CDboTSActOPCam;
}

void CDboTSEntityFactoryType_CDboTSActOPCam::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActTWaitTS::CDboTSEntityFactoryType_CDboTSActTWaitTS( void )
{
	m_defPropertyList["time"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActTWaitTS::CreateObj( void )
{
	return new CDboTSActTWaitTS;
}

void CDboTSEntityFactoryType_CDboTSActTWaitTS::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActInSSM::CDboTSEntityFactoryType_CDboTSActInSSM( void )
{
	m_defPropertyList["ssmid"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["val"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActInSSM::CreateObj( void )
{
	return new CDboTSActInSSM;
}

void CDboTSEntityFactoryType_CDboTSActInSSM::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActTSState::CDboTSEntityFactoryType_CDboTSActTSState( void )
{
	m_defPropertyList["type"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["state"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActTSState::CreateObj( void )
{
	return new CDboTSActTSState;
}

void CDboTSEntityFactoryType_CDboTSActTSState::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActPortal::CDboTSEntityFactoryType_CDboTSActPortal( void )
{
	m_defPropertyList["type"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["widx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["px"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["py"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["pz"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["dx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["dy"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["dz"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActPortal::CreateObj( void )
{
	return new CDboTSActPortal;
}

void CDboTSEntityFactoryType_CDboTSActPortal::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActObjState::CDboTSEntityFactoryType_CDboTSActObjState( void )
{
	m_defPropertyList["widx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["oidx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["mstate"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["osh"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["osh_sh"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["osh_uc"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActObjState::CreateObj( void )
{
	return new CDboTSActObjState;
}

void CDboTSEntityFactoryType_CDboTSActObjState::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActConcCheck::CDboTSEntityFactoryType_CDboTSActConcCheck( void )
{
	m_defPropertyList["ccnt"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["rtime"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["eobjidx"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["etsid"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActConcCheck::CreateObj( void )
{
	return new CDboTSActConcCheck;
}

void CDboTSEntityFactoryType_CDboTSActConcCheck::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActObjConv::CDboTSEntityFactoryType_CDboTSActObjConv( void )
{
	m_defPropertyList["widx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["ctype"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["idx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["conv"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActObjConv::CreateObj( void )
{
	return new CDboTSActObjConv;
}

void CDboTSEntityFactoryType_CDboTSActObjConv::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActHint::CDboTSEntityFactoryType_CDboTSActHint( void )
{
	m_defPropertyList["type"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["idx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActHint::CreateObj( void )
{
	return new CDboTSActHint;
}

void CDboTSEntityFactoryType_CDboTSActHint::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActSendSvrEvt::CDboTSEntityFactoryType_CDboTSActSendSvrEvt( void )
{
	m_defPropertyList["type"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["ttype"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["id"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["tblidx"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["stype"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["stradius"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActSendSvrEvt::CreateObj( void )
{
	return new CDboTSActSendSvrEvt;
}

void CDboTSEntityFactoryType_CDboTSActSendSvrEvt::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActTMQStage::CDboTSEntityFactoryType_CDboTSActTMQStage( void )
{
	m_defPropertyList["stage"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActTMQStage::CreateObj( void )
{
	return new CDboTSActTMQStage;
}

void CDboTSEntityFactoryType_CDboTSActTMQStage::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActOpenWindow::CDboTSEntityFactoryType_CDboTSActOpenWindow( void )
{
	m_defPropertyList["show"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["wtype"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["tblidx"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActOpenWindow::CreateObj( void )
{
	return new CDboTSActOpenWindow;
}

void CDboTSEntityFactoryType_CDboTSActOpenWindow::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActTMQTimeBonus::CDboTSEntityFactoryType_CDboTSActTMQTimeBonus( void )
{
	m_defPropertyList["bt"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActTMQTimeBonus::CreateObj( void )
{
	return new CDboTSActTMQTimeBonus;
}

void CDboTSEntityFactoryType_CDboTSActTMQTimeBonus::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActTelecast::CDboTSEntityFactoryType_CDboTSActTelecast( void )
{
	m_defPropertyList["type"].eType			= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["npcidx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["msgtype"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["speechidx"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["dsptime"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActTelecast::CreateObj( void )
{
	return new CDboTSActTelecast;
}

void CDboTSEntityFactoryType_CDboTSActTelecast::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActDirIndicator::CDboTSEntityFactoryType_CDboTSActDirIndicator( void )
{
	m_defPropertyList["self"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["stype"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["dtype"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["tblidx"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["x"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["y"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["z"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActDirIndicator::CreateObj( void )
{
	return new CDboTSActDirIndicator;
}

void CDboTSEntityFactoryType_CDboTSActDirIndicator::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActOPObject::CDboTSEntityFactoryType_CDboTSActOPObject( void )
{
	m_defPropertyList["apply"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["time"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["tblidx"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActOPObject::CreateObj( void )
{
	return new CDboTSActOPObject;
}

void CDboTSEntityFactoryType_CDboTSActOPObject::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActDrop::CDboTSEntityFactoryType_CDboTSActDrop( void )
{
	m_defPropertyList["ptype"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["tblidx"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActDrop::CreateObj( void )
{
	return new CDboTSActDrop;
}

void CDboTSEntityFactoryType_CDboTSActDrop::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActRmvCoupon::CDboTSEntityFactoryType_CDboTSActRmvCoupon( void )
{
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActRmvCoupon::CreateObj( void )
{
	return new CDboTSActRmvCoupon;
}

void CDboTSEntityFactoryType_CDboTSActRmvCoupon::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActEscort::CDboTSEntityFactoryType_CDboTSActEscort( void )
{
	m_defPropertyList["start"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["etype"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["tsshare"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["tblidx"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActEscort::CreateObj( void )
{
	return new CDboTSActEscort;
}

void CDboTSEntityFactoryType_CDboTSActEscort::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActTMQInfoShow::CDboTSEntityFactoryType_CDboTSActTMQInfoShow( void )
{
	m_defPropertyList["itype"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActTMQInfoShow::CreateObj( void )
{
	return new CDboTSActTMQInfoShow;
}

void CDboTSEntityFactoryType_CDboTSActTMQInfoShow::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActWorldPlayScript::CDboTSEntityFactoryType_CDboTSActWorldPlayScript( void )
{
	m_defPropertyList["start"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["uof"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["sid"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActWorldPlayScript::CreateObj( void )
{
	return new CDboTSActWorldPlayScript;
}

void CDboTSEntityFactoryType_CDboTSActWorldPlayScript::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActSWProbSF::CDboTSEntityFactoryType_CDboTSActSWProbSF( void )
{
	m_defPropertyList["prob"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING;
	m_defPropertyList["sbi"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["fbi"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActSWProbSF::CreateObj( void )
{
	return new CDboTSActSWProbSF;
}

void CDboTSEntityFactoryType_CDboTSActSWProbSF::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActPCConv::CDboTSEntityFactoryType_CDboTSActPCConv( void )
{
	m_defPropertyList["tblidx"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActPCConv::CreateObj( void )
{
	return new CDboTSActPCConv;
}

void CDboTSEntityFactoryType_CDboTSActPCConv::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActCustomEvt::CDboTSEntityFactoryType_CDboTSActCustomEvt( void )
{
	m_defPropertyList["ceid"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActCustomEvt::CreateObj( void )
{
	return new CDboTSActCustomEvt;
}

void CDboTSEntityFactoryType_CDboTSActCustomEvt::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActTelMudosa::CDboTSEntityFactoryType_CDboTSActTelMudosa( void )
{
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActTelMudosa::CreateObj( void )
{
	return new CDboTSActTelMudosa;
}

void CDboTSEntityFactoryType_CDboTSActTelMudosa::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActTelMinorMatch::CDboTSEntityFactoryType_CDboTSActTelMinorMatch( void )
{
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActTelMinorMatch::CreateObj( void )
{
	return new CDboTSActTelMinorMatch;
}

void CDboTSEntityFactoryType_CDboTSActTelMinorMatch::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActPIDgn::CDboTSEntityFactoryType_CDboTSActPIDgn( void )
{
	m_defPropertyList["type"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["nidx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["hidx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["ridx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActPIDgn::CreateObj( void )
{
	return new CDboTSActPIDgn;
}

void CDboTSEntityFactoryType_CDboTSActPIDgn::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActTLQ::CDboTSEntityFactoryType_CDboTSActTLQ( void )
{
	m_defPropertyList["type"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["idx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActTLQ::CreateObj( void )
{
	return new CDboTSActTLQ;
}

void CDboTSEntityFactoryType_CDboTSActTLQ::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActDoSkill::CDboTSEntityFactoryType_CDboTSActDoSkill( void )
{
	m_defPropertyList["idx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActDoSkill::CreateObj( void )
{
	return new CDboTSActDoSkill;
}

void CDboTSEntityFactoryType_CDboTSActDoSkill::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActTObjFriendly::CDboTSEntityFactoryType_CDboTSActTObjFriendly( void )
{
	m_defPropertyList["flgs"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActTObjFriendly::CreateObj( void )
{
	return new CDboTSActTObjFriendly;
}

void CDboTSEntityFactoryType_CDboTSActTObjFriendly::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActBroadMsg::CDboTSEntityFactoryType_CDboTSActBroadMsg( void )
{
	m_defPropertyList["et"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["uidt"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["uisht"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["uibst"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["uisdt"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["oc"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["ot"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["oidx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["oqtidx"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["dt"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActBroadMsg::CreateObj( void )
{
	return new CDboTSActBroadMsg;
}

void CDboTSEntityFactoryType_CDboTSActBroadMsg::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActMiniNarration::CDboTSEntityFactoryType_CDboTSActMiniNarration( void )
{
	m_defPropertyList["tidx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActMiniNarration::CreateObj( void )
{
	return new CDboTSActMiniNarration;
}

void CDboTSEntityFactoryType_CDboTSActMiniNarration::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActRegCTiming::CDboTSEntityFactoryType_CDboTSActRegCTiming( void )
{
	m_defPropertyList["rtype"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;

	m_defPropertyList["flags"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["0_midx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["1_nidx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["2_widx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["2_oidx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["3_widx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["3_minx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["3_minz"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["3_maxx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["3_maxz"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["4_qidx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;

	m_defPropertyList["etype"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["ntidx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActRegCTiming::CreateObj( void )
{
	return new CDboTSActRegCTiming;
}

void CDboTSEntityFactoryType_CDboTSActRegCTiming::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActExcCGroup::CDboTSEntityFactoryType_CDboTSActExcCGroup( void )
{
	m_defPropertyList["rtype"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["gid"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActExcCGroup::CreateObj( void )
{
	return new CDboTSActExcCGroup;
}

void CDboTSEntityFactoryType_CDboTSActExcCGroup::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActSkipCont::CDboTSEntityFactoryType_CDboTSActSkipCont( void )
{
	m_defPropertyList["stid"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["stcid"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActSkipCont::CreateObj( void )
{
	return new CDboTSActSkipCont;
}

void CDboTSEntityFactoryType_CDboTSActSkipCont::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActObjWPS::CDboTSEntityFactoryType_CDboTSActObjWPS( void )
{
	m_defPropertyList["sid"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActObjWPS::CreateObj( void )
{
	return new CDboTSActObjWPS;
}

void CDboTSEntityFactoryType_CDboTSActObjWPS::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActDojo::CDboTSEntityFactoryType_CDboTSActDojo( void )
{
	m_defPropertyList["type"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["idx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActDojo::CreateObj( void )
{
	return new CDboTSActDojo;
}

void CDboTSEntityFactoryType_CDboTSActDojo::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActOutMsg::CDboTSEntityFactoryType_CDboTSActOutMsg( void )
{
	m_defPropertyList["idx"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["taid"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActOutMsg::CreateObj( void )
{
	return new CDboTSActOutMsg;
}

void CDboTSEntityFactoryType_CDboTSActOutMsg::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActWPSFD::CDboTSEntityFactoryType_CDboTSActWPSFD( void )
{
	m_defPropertyList["sid"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["taid"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActWPSFD::CreateObj( void )
{
	return new CDboTSActWPSFD;
}

void CDboTSEntityFactoryType_CDboTSActWPSFD::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActMail::CDboTSEntityFactoryType_CDboTSActMail( void )
{
	m_defPropertyList["dtidx"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["itidx"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["stidx"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["icnt"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["taid"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["zenny"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActMail::CreateObj( void )
{
	return new CDboTSActMail;
}

void CDboTSEntityFactoryType_CDboTSActMail::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActAvatarDead::CDboTSEntityFactoryType_CDboTSActAvatarDead( void )
{
	m_defPropertyList["start"].eType	= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["taid"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActAvatarDead::CreateObj( void )
{
	return new CDboTSActAvatarDead;
}

void CDboTSEntityFactoryType_CDboTSActAvatarDead::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

//////////////////////////////////////////////////////////////////////////


CDboTSEntityFactoryType_CDboTSActSkyDgn::CDboTSEntityFactoryType_CDboTSActSkyDgn( void )
{
	m_defPropertyList["type"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
	m_defPropertyList["idx"].eType		= CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER;
}

CNtlTSScrObject* CDboTSEntityFactoryType_CDboTSActSkyDgn::CreateObj( void )
{
	return new CDboTSActSkyDgn;
}

void CDboTSEntityFactoryType_CDboTSActSkyDgn::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}


//////////////////////////////////////////////////////////////////////////


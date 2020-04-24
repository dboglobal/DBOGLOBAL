#ifndef _DBO_TSCONDFACTORYTYPE_H_
#define _DBO_TSCONDFACTORYTYPE_H_


#include "DboTSCheckLvl.h"
#include "DboTSCheckPCCls.h"
#include "DboTSCheckPCRace.h"
#include "DboTSCheckQItem.h"
#include "DboTSCheckClrQst.h"
#include "DboTSCheckItem.h"
#include "DboTSCheckSToCEvt.h"
#include "DboTSCheckSSM.h"
#include "DboTSCheckWItem.h"
#include "DboTSCheckObjItem.h"
#include "DboTSCheckInNPC.h"
#include "DboTSCheckObjState.h"
#include "DboTSCheckInWorld.h"
#include "DboTSCheckOPObject.h"
#include "DboTSCheckHasCoupon.h"
#include "DboTSCheckNPCDead.h"
#include "DboTSCheckDistWithNPC.h"
#include "DboTSCheckSToCDelivery.h"
#include "DboTSCheckAttachObj.h"
#include "DboTSCheckProgQuest.h"
#include "DboTSCheckReputation.h"
#include "DboTSCheckBudokaiState.h"
#include "DboTSCheckCustomEvent.h"
#include "DboTSCheckAvatarDead.h"
#include "DboTSCheckOutRgn.h"


/** 
	Condition 관련 객체를 Runtime 시에 생성하기 위한 condition entity type class 들
*/


class CDboTSEntityFactoryType_CDboTSCheckLvl : public CNtlTSEntityFactoryType_CNtlTSCond
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSCheckLvl )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSCheckLvl( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSCheckPCCls : public CNtlTSEntityFactoryType_CNtlTSCond
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSCheckPCCls )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSCheckPCCls( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSCheckPCRace : public CNtlTSEntityFactoryType_CNtlTSCond
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSCheckPCRace )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSCheckPCRace( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSCheckQItem : public CNtlTSEntityFactoryType_CNtlTSCond
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSCheckQItem )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSCheckQItem( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSCheckClrQst : public CNtlTSEntityFactoryType_CNtlTSCond
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSCheckClrQst )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSCheckClrQst( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSCheckItem : public CNtlTSEntityFactoryType_CNtlTSCond
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSCheckItem )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSCheckItem( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSCheckSToCEvt : public CNtlTSEntityFactoryType_CNtlTSCond
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSCheckSToCEvt )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSCheckSToCEvt( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSCheckSSM : public CNtlTSEntityFactoryType_CNtlTSCond
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSCheckSSM )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSCheckSSM( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSCheckWItem : public CNtlTSEntityFactoryType_CNtlTSCond
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSCheckWItem )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSCheckWItem( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSCheckObjItem : public CNtlTSEntityFactoryType_CNtlTSCond
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSCheckObjItem )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSCheckObjItem( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSCheckInNPC : public CNtlTSEntityFactoryType_CNtlTSCond
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSCheckInNPC )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSCheckInNPC( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSCheckObjState : public CNtlTSEntityFactoryType_CNtlTSCond
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSCheckObjState )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSCheckObjState( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSCheckInWorld : public CNtlTSEntityFactoryType_CNtlTSCond
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSCheckInWorld )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSCheckInWorld( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSCheckOPObject : public CNtlTSEntityFactoryType_CNtlTSCond
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSCheckOPObject )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSCheckOPObject( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSCheckHasCoupon : public CNtlTSEntityFactoryType_CNtlTSCond
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSCheckHasCoupon )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSCheckHasCoupon( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSCheckNPCDead : public CNtlTSEntityFactoryType_CNtlTSCond
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSCheckNPCDead )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSCheckNPCDead( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSCheckDistWithNPC : public CNtlTSEntityFactoryType_CNtlTSCond
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSCheckDistWithNPC )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSCheckDistWithNPC( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSCheckSToCDelivery : public CNtlTSEntityFactoryType_CNtlTSCond
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSCheckSToCDelivery )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSCheckSToCDelivery( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSCheckAttachObj : public CNtlTSEntityFactoryType_CNtlTSCond
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSCheckAttachObj )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSCheckAttachObj( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSCheckProgQuest : public CNtlTSEntityFactoryType_CNtlTSCond
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSCheckProgQuest )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSCheckProgQuest( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSCheckReputation : public CNtlTSEntityFactoryType_CNtlTSCond
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSCheckReputation )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSCheckReputation( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSCheckBudokaiState : public CNtlTSEntityFactoryType_CNtlTSCond
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSCheckBudokaiState )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSCheckBudokaiState( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSCheckCustomEvent : public CNtlTSEntityFactoryType_CNtlTSCond
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSCheckCustomEvent )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSCheckCustomEvent( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSCheckAvatarDead : public CNtlTSEntityFactoryType_CNtlTSCond
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSCheckAvatarDead )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSCheckAvatarDead( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSCheckOutRGN : public CNtlTSEntityFactoryType_CNtlTSCond
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSCheckOutRGN )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSCheckOutRGN( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};



#endif
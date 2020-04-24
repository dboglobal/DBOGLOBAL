#ifndef _DBO_TSEVENTFACTORYTYPE_H_
#define _DBO_TSEVENTFACTORYTYPE_H_


#include "DboTSClickNPC.h"
#include "DboTSClickObject.h"
#include "DboTSColObject.h"
#include "DboTSItemUse.h"
#include "DboTSItemGet.h"
#include "DboTSItemEquip.h"
#include "DboTSScoutUse.h"
#include "DboTSRcvSvrEvt.h"
#include "DboTSSkillUse.h"
#include "DboTSColRgn.h"
#include "DboTSRB.h"
#include "DboTSClickMob.h"
#include "DboTSSearchQuest.h"
#include "DboTSBindStone.h"
#include "DboTSItemUpgrade.h"
#include "DboTSTeleport.h"
#include "DboTSBudokai.h"
#include "DboTSSlotMachine.h"
#include "DboTSHoipoiMix.h"
#include "DboTSPrivateShop.h"
#include "DboTSFreeBattle.h"
#include "DboTSItemIdentity.h"
#include "DboTSUseMail.h"
#include "DboTSParty.h"
#include "DboTSClickSideIcon.h"
#include "DboTSDialogOpen.h"
#include "DboTSQuest.h"
#include "DboTSLevelCheck.h"

/** 
	Event 관련 객체를 Runtime 시에 생성하기 위한 event entity type class 들
*/


class CDboTSEntityFactoryType_CDboTSClickNPC : public CNtlTSEntityFactoryType_CNtlTSEvent
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSClickNPC )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSClickNPC( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSClickObject : public CNtlTSEntityFactoryType_CNtlTSEvent
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSClickObject )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSClickObject( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSColObject : public CNtlTSEntityFactoryType_CNtlTSEvent
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSColObject )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSColObject( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSItemUse : public CNtlTSEntityFactoryType_CNtlTSEvent
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSItemUse )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSItemUse( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSItemGet : public CNtlTSEntityFactoryType_CNtlTSEvent
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSItemGet )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSItemGet( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSItemEquip : public CNtlTSEntityFactoryType_CNtlTSEvent
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSItemEquip )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSItemEquip( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSScoutUse : public CNtlTSEntityFactoryType_CNtlTSEvent
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSScoutUse )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSScoutUse( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSRcvSvrEvt : public CNtlTSEntityFactoryType_CNtlTSEvent
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSRcvSvrEvt )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSRcvSvrEvt( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSEntityFactoryType_CDboTSSkillUse : public CNtlTSEntityFactoryType_CNtlTSEvent
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSSkillUse )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSSkillUse( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSEntityFactoryType_CDboTSColRgn : public CNtlTSEntityFactoryType_CNtlTSEvent
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSColRgn )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSColRgn( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSEntityFactoryType_CDboTSRB : public CNtlTSEntityFactoryType_CNtlTSEvent
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSRB )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSRB( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSEntityFactoryType_CDboTSClickMob : public CNtlTSEntityFactoryType_CNtlTSEvent
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSClickMob )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSClickMob( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSEntityFactoryType_CDboTSSearchQuest : public CNtlTSEntityFactoryType_CNtlTSEvent
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSSearchQuest )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSSearchQuest( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSEntityFactoryType_CDboTSBindStone : public CNtlTSEntityFactoryType_CNtlTSEvent
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSBindStone )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSBindStone( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSEntityFactoryType_CDboTSItemUpgrade : public CNtlTSEntityFactoryType_CNtlTSEvent
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSItemUpgrade )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSItemUpgrade( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSEntityFactoryType_CDboTSTeleport : public CNtlTSEntityFactoryType_CNtlTSEvent
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSTeleport )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSTeleport( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSEntityFactoryType_CDboTSBudokai: public CNtlTSEntityFactoryType_CNtlTSEvent
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSBudokai )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSBudokai( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSEntityFactoryType_CDboTSSlotMachine : public CNtlTSEntityFactoryType_CNtlTSEvent
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSSlotMachine )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSSlotMachine( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSEntityFactoryType_CDboTSHoipoiMix : public CNtlTSEntityFactoryType_CNtlTSEvent
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSHoipoiMix )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSHoipoiMix( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSEntityFactoryType_CDboTSPrivateShop : public CNtlTSEntityFactoryType_CNtlTSEvent
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSPrivateShop )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSPrivateShop( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSEntityFactoryType_CDboTSFreeBattle : public CNtlTSEntityFactoryType_CNtlTSEvent
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSFreeBattle )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSFreeBattle( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSEntityFactoryType_CDboTSItemIdentity : public CNtlTSEntityFactoryType_CNtlTSEvent
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSItemIdentity )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSItemIdentity( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSEntityFactoryType_CDboTSUseMail : public CNtlTSEntityFactoryType_CNtlTSEvent
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSUseMail )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSUseMail( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSEntityFactoryType_CDboTSParty : public CNtlTSEntityFactoryType_CNtlTSEvent
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSParty )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSParty( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSEntityFactoryType_CDboTSClickSideIcon : public CNtlTSEntityFactoryType_CNtlTSEvent
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSClickSideIcon )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSClickSideIcon( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSEntityFactoryType_CDboTSDialogOpen : public CNtlTSEntityFactoryType_CNtlTSEvent
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSDialogOpen )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSDialogOpen( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSEntityFactoryType_CDboTSQuest : public CNtlTSEntityFactoryType_CNtlTSEvent
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSQuest )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSQuest( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSEntityFactoryType_CDboTSLevelCheck : public CNtlTSEntityFactoryType_CNtlTSEvent
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSLevelCheck )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSLevelCheck( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};



#endif
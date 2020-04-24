#ifndef _DBO_TSCONTFACTORYTYPE_H_
#define _DBO_TSCONTFACTORYTYPE_H_


#include "DboTSContGCond.h"
#include "DboTSContStart.h"
#include "DboTSContGAct.h"
#include "DboTSContSyncAct.h"
#include "DboTSContReward.h"
#include "DboTSContEnd.h"
#include "DboTSContUsrSel.h"
#include "DboTSContNarration.h"
#include "DboTSContProposal.h"
#include "DboTSContSwitch.h"
#include "DboTSContUnifiedNarration.h"


/** 
	Container 관련 객체를 Runtime 시에 생성하기 위한 container type class 들
*/


class CDboTSContFactoryType_CDboTSContGCond : public CNtlTSEntityFactoryType_CNtlTSCont
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSContGCond )

// Constructions and Destructions
public:
	CDboTSContFactoryType_CDboTSContGCond( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSContFactoryType_CDboTSContStart : public CDboTSContFactoryType_CDboTSContGCond
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSContStart )

// Constructions and Destructions
public:
	CDboTSContFactoryType_CDboTSContStart( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSContFactoryType_CDboTSContGAct : public CNtlTSEntityFactoryType_CNtlTSCont
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSContGAct )

// Constructions and Destructions
public:
	CDboTSContFactoryType_CDboTSContGAct( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSContFactoryType_CDboTSContReward : public CNtlTSEntityFactoryType_CNtlTSCont
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSContReward )

// Constructions and Destructions
public:
	CDboTSContFactoryType_CDboTSContReward( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSContFactoryType_CDboTSContEnd : public CDboTSContFactoryType_CDboTSContGAct
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSContEnd )

// Constructions and Destructions
public:
	CDboTSContFactoryType_CDboTSContEnd( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSContFactoryType_CDboTSContUsrSel : public CNtlTSEntityFactoryType_CNtlTSCont
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSContUsrSel )

// Constructions and Destructions
public:
	CDboTSContFactoryType_CDboTSContUsrSel( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSContFactoryType_CDboTSContNarration : public CNtlTSEntityFactoryType_CNtlTSCont
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSContNarration )

// Constructions and Destructions
public:
	CDboTSContFactoryType_CDboTSContNarration( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSContFactoryType_CDboTSContProposal : public CNtlTSEntityFactoryType_CNtlTSCont
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSContProposal )

// Constructions and Destructions
public:
	CDboTSContFactoryType_CDboTSContProposal( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSContFactoryType_CDboTSContSwitch : public CNtlTSEntityFactoryType_CNtlTSCont
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSContSwitch )

// Constructions and Destructions
public:
	CDboTSContFactoryType_CDboTSContSwitch( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSContFactoryType_CDboTSContUnifiedNarration : public CNtlTSEntityFactoryType_CNtlTSCont
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSContUnifiedNarration )

// Constructions and Destructions
public:
	CDboTSContFactoryType_CDboTSContUnifiedNarration( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


#endif
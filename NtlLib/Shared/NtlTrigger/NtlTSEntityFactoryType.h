#ifndef _NTL_TSENTITYFACTORYTYPE_H_
#define _NTL_TSENTITYFACTORYTYPE_H_


#include "NtlTSScrFactoryType.h"
#include "NtlTSEntity.h"
#include "NtlTSEvent.h"
#include "NtlTSCond.h"
#include "NtlTSAction.h"
#include "NtlTSCont.h"
#include "NtlTSGroup.h"
#include "NtlTSTrigger.h"


class CNtlTSScrObject;


/** 
	Entity 를 Runtime 시에 생성하기 위한 entity type class 들
*/


class CNtlTSEntityFactoryType : public CNtlTSScrFactoryType
{
// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void ) = 0;
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj ) = 0;
};


class CNtlTSEntityFactoryType_CNtlTSEntity : public CNtlTSEntityFactoryType
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CNtlTSEntity )

// Constructions and Destructions
public:
	CNtlTSEntityFactoryType_CNtlTSEntity( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CNtlTSEntityFactoryType_CNtlTSEvent : public CNtlTSEntityFactoryType_CNtlTSEntity
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CNtlTSEvent )

// Constructions and Destructions
public:
	CNtlTSEntityFactoryType_CNtlTSEvent( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CNtlTSEntityFactoryType_CNtlTSCond : public CNtlTSEntityFactoryType_CNtlTSEntity
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CNtlTSCond )

// Constructions and Destructions
public:
	CNtlTSEntityFactoryType_CNtlTSCond( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CNtlTSEntityFactoryType_CNtlTSAction : public CNtlTSEntityFactoryType_CNtlTSEntity
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CNtlTSAction )

// Constructions and Destructions
public:
	CNtlTSEntityFactoryType_CNtlTSAction( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CNtlTSEntityFactoryType_CNtlTSCont : public CNtlTSEntityFactoryType_CNtlTSEntity
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CNtlTSCont )

// Constructions and Destructions
public:
	CNtlTSEntityFactoryType_CNtlTSCont( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CNtlTSEntityFactoryType_CNtlTSGroup : public CNtlTSEntityFactoryType_CNtlTSEntity
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CNtlTSGroup )

// Constructions and Destructions
public:
	CNtlTSEntityFactoryType_CNtlTSGroup( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CNtlTSEntityFactoryType_CNtlTSTrigger : public CNtlTSEntityFactoryType_CNtlTSEntity
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CNtlTSTrigger )

// Constructions and Destructions
public:
	CNtlTSEntityFactoryType_CNtlTSTrigger( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


#endif
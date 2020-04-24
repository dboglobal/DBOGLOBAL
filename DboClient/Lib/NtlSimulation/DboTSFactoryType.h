#ifndef _DBO_TSFFACTORYTYPE_H_
#define _DBO_TSFFACTORYTYPE_H_


#include "DboTSCore.h"


/** 
	Control factory type
*/


class CDboTSControlFactoryType_CDboTSCQCtrl : public CDboTSControlFactoryType_CDboTSQCtrl
{
// Constructions and Destructions
public:
	CDboTSControlFactoryType_CDboTSCQCtrl( void );

// Methods
public:
	virtual	CNtlTSControlObject*		CreateObj( void );
	virtual void						DeleteObj( CNtlTSControlObject*& pObj );
	virtual const char*					GetKeyword( void ) const;
};

class CDboTSControlFactoryType_CDboTSCTCtrl : public CDboTSControlFactoryType_CDboTSTCtrl
{
	// Constructions and Destructions
public:
	CDboTSControlFactoryType_CDboTSCTCtrl( void );

	// Methods
public:
	virtual	CNtlTSControlObject*		CreateObj( void );
	virtual void						DeleteObj( CNtlTSControlObject*& pObj );
	virtual const char*					GetKeyword( void ) const;
};


/** 
	UI factory type
*/


class CDboTSUIFactoryType_CDboTSCQRecv : public CDboTSUIFactoryType_CDboTSQRecv
{
// Constructions and Destructions
public:
	CDboTSUIFactoryType_CDboTSCQRecv( void );

// Methods
public:
	virtual	CNtlTSUIObject*				CreateObj( void );
	virtual void						DeleteObj( CNtlTSUIObject*& pObj );
	virtual const char*					GetKeyword( void ) const;
};


class CDboTSUIFactoryType_CDboTSCQAgency : public CDboTSUIFactoryType_CDboTSQAgency
{
	// Constructions and Destructions
public:
	CDboTSUIFactoryType_CDboTSCQAgency( void );

	// Methods
public:
	virtual	CNtlTSUIObject*				CreateObj( void );
	virtual void						DeleteObj( CNtlTSUIObject*& pObj );
	virtual const char*					GetKeyword( void ) const;
};


class CDboTSUIFactoryType_CDboTSCTRecv : public CDboTSUIFactoryType_CDboTSTRecv
{
	// Constructions and Destructions
public:
	CDboTSUIFactoryType_CDboTSCTRecv( void );

	// Methods
public:
	virtual	CNtlTSUIObject*				CreateObj( void );
	virtual void						DeleteObj( CNtlTSUIObject*& pObj );
	virtual const char*					GetKeyword( void ) const;
};


class CDboTSUIFactoryType_CDboTSCTAgency : public CDboTSUIFactoryType_CDboTSTAgency
{
	// Constructions and Destructions
public:
	CDboTSUIFactoryType_CDboTSCTAgency( void );

	// Methods
public:
	virtual	CNtlTSUIObject*				CreateObj( void );
	virtual void						DeleteObj( CNtlTSUIObject*& pObj );
	virtual const char*					GetKeyword( void ) const;
};


class CDboTSUIFactoryType_CDboTSCObjAgency : public CDboTSUIFactoryType_CDboTSTAgency
{
// Constructions and Destructions
public:
	CDboTSUIFactoryType_CDboTSCObjAgency( void );

// Methods
public:
	virtual	CNtlTSUIObject*				CreateObj( void );
	virtual void						DeleteObj( CNtlTSUIObject*& pObj );
	virtual const char*					GetKeyword( void ) const;
};


class CDboTSUIFactoryType_CDboTSCMain : public CDboTSUIFactoryType_CDboTSMain
{
	// Constructions and Destructions
public:
	CDboTSUIFactoryType_CDboTSCMain( void );

	// Methods
public:
	virtual	CNtlTSUIObject*				CreateObj( void );
	virtual void						DeleteObj( CNtlTSUIObject*& pObj );
	virtual const char*					GetKeyword( void ) const;
};


#endif
#ifndef _NTL_TSUIFACTORYTYPE_H_
#define _NTL_TSUIFACTORYTYPE_H_


class CNtlTSUIObject;


/** 
	UI 객체를 Runtime 시에 생성하기 위한 UI type class 들
*/


class CNtlTSUIFactoryType_CNtlTSUIObject
{
// Methods
public:
	virtual	CNtlTSUIObject*				CreateObj( void ) = 0;
	virtual void						DeleteObj( CNtlTSUIObject*& pObj ) = 0;
	virtual const char*					GetKeyword( void ) const = 0;
};


class CNtlTSUIFactoryType_CNtlTSRecv : public CNtlTSUIFactoryType_CNtlTSUIObject
{
// Constructions and Destructions
public:
	CNtlTSUIFactoryType_CNtlTSRecv( void );

// Methods
public:
	virtual	CNtlTSUIObject*				CreateObj( void );
	virtual void						DeleteObj( CNtlTSUIObject*& pObj );
	virtual const char*					GetKeyword( void ) const;
};


class CNtlTSUIFactoryType_CNtlTSAgency : public CNtlTSUIFactoryType_CNtlTSUIObject
{
// Constructions and Destructions
public:
	CNtlTSUIFactoryType_CNtlTSAgency( void );

// Methods
public:
	virtual	CNtlTSUIObject*				CreateObj( void );
	virtual void						DeleteObj( CNtlTSUIObject*& pObj );
	virtual const char*					GetKeyword( void ) const;
};


class CNtlTSUIFactoryType_CNtlTSMain : public CNtlTSUIFactoryType_CNtlTSUIObject
{
// Constructions and Destructions
public:
	CNtlTSUIFactoryType_CNtlTSMain( void );

// Methods
public:
	virtual	CNtlTSUIObject*				CreateObj( void );
	virtual void						DeleteObj( CNtlTSUIObject*& pObj );
	virtual const char*					GetKeyword( void ) const;
};


#endif
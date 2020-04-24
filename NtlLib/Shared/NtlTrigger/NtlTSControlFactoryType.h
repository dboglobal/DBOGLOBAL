#ifndef _NTL_TSCONTROLFACTORYTYPE_H_
#define _NTL_TSCONTROLFACTORYTYPE_H_


class CNtlTSControlObject;


/** 
	Control 객체를 Runtime 시에 생성하기 위한 control type class 들
*/


class CNtlTSControlFactoryType_CNtlTSControlObject
{
// Methods
public:
	virtual	CNtlTSControlObject*		CreateObj( void ) = 0;
	virtual void						DeleteObj( CNtlTSControlObject*& pObj ) = 0;
	virtual const char*					GetKeyword( void ) const = 0;
};


class CNtlTSControlFactoryType_CNtlTSController : public CNtlTSControlFactoryType_CNtlTSControlObject
{
// Constructions and Destructions
public:
	CNtlTSControlFactoryType_CNtlTSController( void );

// Methods
public:
	virtual	CNtlTSControlObject*		CreateObj( void );
	virtual void						DeleteObj( CNtlTSControlObject*& pObj );
	virtual const char*					GetKeyword( void ) const;
};


class CNtlTSControlFactoryType_CNtlTSEvtMapper : public CNtlTSControlFactoryType_CNtlTSControlObject
{
// Constructions and Destructions
public:
	CNtlTSControlFactoryType_CNtlTSEvtMapper( void );

// Methods
public:
	virtual	CNtlTSControlObject*		CreateObj( void );
	virtual void						DeleteObj( CNtlTSControlObject*& pObj );
	virtual const char*					GetKeyword( void ) const;
};


#endif
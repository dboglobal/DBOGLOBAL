#ifndef _NTL_TSSCRPROPERTY_H_
#define _NTL_TSSCRPROPERTY_H_


#include "NtlTSCoreDefine.h"
#include "NtlTSLog.h"


/** 
	Script property
*/


class CNtlTSScrProperty
{
// Declarations
public:
	typedef std::map<std::string,std::string> hashdef_PROPERTY;
	typedef hashdef_PROPERTY::iterator PROP_IT;
	typedef hashdef_PROPERTY::const_iterator PROP_CIT;

// Member variables
public:
	hashdef_PROPERTY					m_defProperty;

// Constructions and Destructions
public:
	CNtlTSScrProperty( void ) { return; }
	~CNtlTSScrProperty( void ) { return; }

// Methods
public:
	PROP_IT								Begin( void );
	PROP_IT								End( void );

	PROP_CIT							Begin( void ) const;
	PROP_CIT							End( void ) const;

	// 해당 Property 가 존재하는 지를 검사한다.
	bool								IsExist( const std::string& strPropertyName ) const;

	// 저장된 Property 의 개수를 리턴한다
	int									GetCount( const std::string& strPropertyName ) const;

	// 해당 Property Name 을 통해서 Property Value를 얻는다
	const std::string&					GetValue( const std::string& strPropertyName, int nOffset = 0 ) const;

	// int 로 해당 Property 를 얻는다
	int									GetValueAsInt( const std::string& strPropertyName, int nOffset = 0 ) const;

	// bool로 해당 Property 를 얻는다
	bool								GetValueAsBool( const std::string& strPropertyName, int nOffset = 0 ) const;

	// Property를 추가 한다
	void								AddProperty( const std::string& strPropertyName, const std::string& strPropertyValue );

	// 해당 Property를 제거 한다
	void								RemoveProperty( const std::string& strPropertyName );

	// 모든 Property를 삭제한다
	void								Clear( void );

// Operator
public:
	const std::string& operator[] ( const std::string& strPropertyName ) const;
};


#include "NtlTSScrProperty.inl"


#endif


#ifndef _NTL_TSRTTI_H_
#define _NTL_TSRTTI_H_


/** 
	Runtime type infomation
*/


//////////////////////////////////////////////////////////////////////////


#define NTL_TS_DECLARE_RTTI \
	public: \
		static	const CNtlTSRTTI	RTTI; \
		virtual	const CNtlTSRTTI&	GetRTTI( void )	const { return RTTI; } \
				const char*			GetClassName( void ) const { return GetRTTI().GetClassName(); } \
				bool				IsSameClass( const CNtlTSRTTI& clVal ) const { return GetRTTI().IsSameClass( clVal ); } \
				bool				IsDerivedClass( const CNtlTSRTTI& clVal ) const { return GetRTTI().IsDerivedClass( clVal ); } \
				bool				IsDerivedClass( const std::string& strClassName ) const { return GetRTTI().IsDerivedClass( strClassName ); }


#define NTL_TS_IMPLEMENT_RTTI_ROOT( classname ) \
	const CNtlTSRTTI	classname::RTTI( #classname, 0 );


#define NTL_TS_IMPLEMENT_RTTI( classname, baseclassname ) \
	const CNtlTSRTTI	classname::RTTI( #classname, &baseclassname::RTTI );


//////////////////////////////////////////////////////////////////////////


class CNtlTSRTTI
{
// Member variables
private:

	// 클래스의 이름에 대한 멤버 변수를 const char* 로 사용해서
	// 생성자를 통해 넘겨받은 문자열의 포인터를 그대로 저장해도 상관없는
	// 이유는 외부에서 넘겨 받는 클래스 이름이 전역 변수로 저장되므로
	// 단지 해당 문자열의 포인터를 멤버변수로 저장하는 것이 메모리
	// 절약 차원에서 이점이 있다.

	const char*							m_pClassName;

	const CNtlTSRTTI*					m_pBaseRTTI;


// Constructions and Destructions
public:
	CNtlTSRTTI( const char* pClassName, const CNtlTSRTTI* pBaseRTTI );


// 멤버 함수.
public:
	const char*							GetClassName( void ) const;

	bool								IsSameClass( const CNtlTSRTTI& clVal ) const;

	bool								IsDerivedClass( const CNtlTSRTTI& clVal ) const;

	bool								IsDerivedClass( const std::string& strClassName ) const;
};


#include "NtlTSRTTI.inl"


//////////////////////////////////////////////////////////////////////////


#endif
//////////////////////////////////////////////////////////////////////////
/**
*	Vector3 클래스
*
*	\file		NtlNaviVector3.h
*	\author		Jeong Ho, Rho
*/
//////////////////////////////////////////////////////////////////////////


#ifndef _TX_NAVI_VECTOR3_H_
#define _TX_NAVI_VECTOR3_H_


class CNtlNaviVector3
{
protected:

	float									m_fElem[3];


public:

	inline									CNtlNaviVector3( bool bZero = false );

	inline									CNtlNaviVector3( const CNtlNaviVector3& v3Val );

	inline									CNtlNaviVector3( float fX, float fY, float fZ );

	inline									CNtlNaviVector3( const float* pfVal );

	inline									~CNtlNaviVector3( void )	{ return; }


public:

	inline			float&					GetX( void );

	inline			const float&			GetX( void ) const;


	inline			float&					GetY( void );

	inline			const float&			GetY( void ) const;


	inline			float&					GetZ( void );

	inline			const float&			GetZ( void ) const;


	inline			void					GetXYZ( float& fX, float& fY, float& fZ ) const;


	inline			float&					GetElem( int nIdx );

	inline			const float&			GetElem( int nIdx ) const;


	inline			void					SetElem( float fVal );

	inline			void					SetElem( int nIdx, float fVal );

	inline			void					SetElem( const CNtlNaviVector3& v3Val );

	inline			void					SetElem( float fX, float fY, float fZ );

	inline			void					SetElem( float* pfVal );


// 멤버 함수.
public:

	// 벡터의 모든 요소들을 무효화한다.

	inline			void					MakeInvalid( void );


	// 벡터를 단위 벡터로 만든다.

	inline			void					MakeUnitX( void );

	inline			void					MakeUnitY( void );

	inline			void					MakeUnitZ( void );


	// 영 벡터로 만든다.

	inline			void					MakeZero( void );


	// 정규화.

	inline			CNtlNaviVector3			GetNormalize( void ) const;

	inline			void					MakeNormalize( void );


	// 두 벡터를 이용하여 자신을 선형 보간 < T = V1 + t * ( V2 - V1 ) > 한다.

	inline			void					MakeLerp( const CNtlNaviVector3& v3Val1, const CNtlNaviVector3& v3Val2, float t );


	// 길이.
	inline			float					GetLength( void ) const;


	// 길이의 제곱.

	inline			float					GetLengthSquared( void ) const;


	// 내적.

	inline			float					GetDot( const CNtlNaviVector3& v3Val ) const;


	// 외적 < R = T ^ V >.

	inline			CNtlNaviVector3			GetCross( const CNtlNaviVector3& v3Val ) const;

	inline			void					MakeCross( const CNtlNaviVector3& v3Val );


	// 부피.

	inline			float					GetVolume( void ) const;


	// 자신과 주어진 값(파라미터) 간의 거리.

	inline			float					GetDistance( const CNtlNaviVector3& v3Val ) const;


	// 자신과 주어진 값(파라미터) 간의 거리의 제곱.

	inline			float					GetDistanceSquared( const CNtlNaviVector3& v3Val ) const;


// 연산자.
public:

	// 할당 연산자.

	inline			CNtlNaviVector3&		operator = ( const CNtlNaviVector3& v3Val );


	// 멤버 접근 연산자.

	inline			float&					operator [] ( int nIdx );

	inline			const float&			operator [] ( int nIdx ) const;


	inline			float&					operator () ( int nIdx );

	inline			const float&			operator () ( int nIdx ) const;


	// 비교 연산자.

	inline			bool					operator == ( const CNtlNaviVector3& v3Val ) const;

	inline			bool					operator != ( const CNtlNaviVector3& v3Val ) const;


	// 산술 연산자.

	inline			CNtlNaviVector3			operator - ( void ) const;


	inline			CNtlNaviVector3			operator * ( float fVal ) const;

	inline			CNtlNaviVector3			operator * ( const CNtlNaviVector3& v3Val ) const;

	inline			CNtlNaviVector3			operator / ( float fVal ) const;

	inline			CNtlNaviVector3			operator / ( const CNtlNaviVector3& v3Val ) const;

	inline			CNtlNaviVector3			operator + ( const CNtlNaviVector3& v3Val ) const;

	inline			CNtlNaviVector3			operator - ( const CNtlNaviVector3& v3Val ) const;


	// 산술 갱신 연산자.

	inline			CNtlNaviVector3&		operator *= ( float fVal );

	inline			CNtlNaviVector3&		operator *= ( const CNtlNaviVector3& v3Val );

	inline			CNtlNaviVector3&		operator /= ( float fVal );

	inline			CNtlNaviVector3&		operator /= ( const CNtlNaviVector3& v3Val );


	inline			CNtlNaviVector3&		operator += ( const CNtlNaviVector3& v3Val );

	inline			CNtlNaviVector3&		operator -= ( const CNtlNaviVector3& v3Val );


	// 형변환 연산자.

	inline									operator float* ( void );

	inline									operator const float* ( void ) const;


	// 내적 연산자.

	inline			float					operator | ( const CNtlNaviVector3& v3Val ) const;


	// 외적 연산자.

	inline			CNtlNaviVector3			operator ^ ( const CNtlNaviVector3& v3Val ) const;


	// 내적 갱신 연산자 < T = T * (T | V) >.

	inline			CNtlNaviVector3&		operator |= ( const CNtlNaviVector3& v3Val );


	// 외적 갱신 연산자 < T = T ^ V >.

	inline			CNtlNaviVector3&		operator ^= ( const CNtlNaviVector3& v3Val );


	// Friend 처리 연산자.

	inline friend	CNtlNaviVector3			operator * ( float fVal1, const CNtlNaviVector3& v3Val2 );

	inline friend	CNtlNaviVector3			operator / ( float fVal1, const CNtlNaviVector3& v3Val2 );


// 내부 구현.
protected:

	inline			int						CompareArrays( const CNtlNaviVector3& v3Val ) const;
};


#include "NtlNaviVector3.inl"


#endif

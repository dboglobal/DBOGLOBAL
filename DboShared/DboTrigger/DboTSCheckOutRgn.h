#ifndef _DBO_TSCHECKOUTRGN_H_
#define _DBO_TSCHECKOUTRGN_H_


#include "DboTSCoreDefine.h"


class CDboTSCheckOutRGN : public CNtlTSCond
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	unsigned int						m_uiWorldIdx;
	float								m_fX;
	float								m_fZ;
	float								m_fRadius;

// Constructions
public:
	CDboTSCheckOutRGN( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_COND_TYPE_ID_CHECK_OUTRGN; }

	unsigned int						GetWorldTblIdx( void ) const;
	void								SetWorldTblIdx( unsigned int uiWorldTblIdx );

	float								GetPosX( void ) const;
	void								SetPosX( float fX );

	float								GetPosZ( void ) const;
	void								SetPosZ( float fZ );

	float								GetRadius( void ) const;
	void								SetRadius( float fRadius );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline unsigned int CDboTSCheckOutRGN::GetWorldTblIdx( void ) const
{
	return m_uiWorldIdx;
}

inline float CDboTSCheckOutRGN::GetPosX( void ) const
{
	return m_fX;
}

inline float CDboTSCheckOutRGN::GetPosZ( void ) const
{
	return m_fZ;
}

inline float CDboTSCheckOutRGN::GetRadius( void ) const
{
	return m_fRadius;
}


#endif
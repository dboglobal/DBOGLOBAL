#ifndef _DBO_TSACTPORTAL_H_
#define _DBO_TSACTPORTAL_H_


#include "DboTSCoreDefine.h"


/**
	Portal action
*/


class CDboTSActPortal : public CNtlTSAction
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	ePORTAL_TYPE						m_ePortalType;
	unsigned int						m_uiWorldIdx;
	float								m_fPosX, m_fPosY, m_fPosZ;
	float								m_fDirX, m_fDirY, m_fDirZ;

// Constructions
public:
	CDboTSActPortal( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_ACT_TYPE_ID_ACT_PORTAL; }

	ePORTAL_TYPE						GetPotalType( void ) const;
	void								SetPotalType( ePORTAL_TYPE ePotalType );

	unsigned int						GetWorldIdx( void ) const;
	void								SetWorldIdx( unsigned int uiWorldIdx );

	void								GetPosition( float& fPosX, float& fPosY, float& fPosZ ) const;
	void								SetPosition( float fPosX, float fPosY, float fPosZ );

	void								GetDirection( float& fDirX, float& fDirY, float& fDirZ ) const;
	void								SetDirection( float fDirX, float fDirY, float fDirZ );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline ePORTAL_TYPE CDboTSActPortal::GetPotalType( void ) const
{
	return m_ePortalType;
}

inline unsigned int CDboTSActPortal::GetWorldIdx( void ) const
{
	return m_uiWorldIdx;
}

inline void CDboTSActPortal::GetPosition( float& fPosX, float& fPosY, float& fPosZ ) const
{
	fPosX = m_fPosX;
	fPosY = m_fPosY;
	fPosZ = m_fPosZ;
}

inline void CDboTSActPortal::GetDirection( float& fDirX, float& fDirY, float& fDirZ ) const
{
	fDirX = m_fDirX;
	fDirY = m_fDirY;
	fDirZ = m_fDirZ;
}


#endif
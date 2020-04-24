#ifndef _DBO_TSSCOUTUSE_H_
#define _DBO_TSSCOUTUSE_H_


#include "DboTSCoreDefine.h"


class CDboTSScoutUse : public CNtlTSEvent
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	unsigned int						m_uiItemIdx;
	eEVENT_SCOUTER_TARGET_TYPE			m_eTargetType;
	unsigned int						m_uiTargetIdx;

// Constructions
public:
	CDboTSScoutUse( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_EVENT_TYPE_ID_SCOUT_USE; }

	unsigned int						GetItemIdx( void ) const;
	void								SetItemIdx( unsigned int uiItemIdx );

	eEVENT_SCOUTER_TARGET_TYPE			GetTargetType( void ) const;
	void								SetTargetType( eEVENT_SCOUTER_TARGET_TYPE eTargetType );

	unsigned int						GetTargetIdx( void ) const;
	void								SetTargetIdx( unsigned int uiTargetIdx );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline unsigned int CDboTSScoutUse::GetItemIdx( void ) const
{
	return m_uiItemIdx;
}


inline eEVENT_SCOUTER_TARGET_TYPE CDboTSScoutUse::GetTargetType( void ) const
{
	return m_eTargetType;
}

inline unsigned int CDboTSScoutUse::GetTargetIdx( void ) const
{
	return m_uiTargetIdx;
}


#endif
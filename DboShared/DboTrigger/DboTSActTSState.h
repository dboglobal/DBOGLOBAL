#ifndef _DBO_TSACTTSSTATE_H_
#define _DBO_TSACTTSSTATE_H_


#include "DboTSCoreDefine.h"


/**
	TS state update action
	TS 의 상태를 변경한다
*/


class CDboTSActTSState : public CNtlTSAction
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	// TS 적용 타입
	eTSSTATE_TYPE						m_eStateType;

	// TS 상태를 업데이트 시킨다
	unsigned short						m_wTSState;

// Constructions
public:
	CDboTSActTSState( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_ACT_TYPE_ID_ACT_TSSTATE; }

	eTSSTATE_TYPE						GetType( void ) const;
	void								SetType( eTSSTATE_TYPE eType );

	unsigned short						GetTSState( void ) const;
	void								SetTSState( unsigned short wTSState );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline eTSSTATE_TYPE CDboTSActTSState::GetType( void ) const
{
	return m_eStateType;
}

inline unsigned short CDboTSActTSState::GetTSState( void ) const
{
	return m_wTSState;
}


#endif
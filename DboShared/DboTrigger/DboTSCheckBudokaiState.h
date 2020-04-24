#ifndef _DBO_TSCHECKBUDOKAISTATE_H_
#define _DBO_TSCHECKBUDOKAISTATE_H_


#include "DboTSCoreDefine.h"


/**
	Check Budokai state condition
*/


class CDboTSCheckBudokaiState : public CNtlTSCond
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	unsigned int						m_uiState;

	eTS_BUDOKAI_MODE					m_eMode;
	eTS_BUDOKAI_STATE					m_eState;

// Constructions
public:
	CDboTSCheckBudokaiState( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_COND_TYPE_ID_CHECK_BUDOKAI_STATE; }

	eTS_BUDOKAI_MODE					GetMode( void ) const;
	void								SetMode( eTS_BUDOKAI_MODE eMode );

	eTS_BUDOKAI_STATE					GetState( void ) const;
	void								SetState( eTS_BUDOKAI_STATE eState );

	bool								IsConditionYes( unsigned char byState /*eBUDOKAI_STATE*/ );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline eTS_BUDOKAI_MODE CDboTSCheckBudokaiState::GetMode( void ) const
{
	return m_eMode;
}


inline eTS_BUDOKAI_STATE CDboTSCheckBudokaiState::GetState( void ) const
{
	return m_eState;
}


#endif
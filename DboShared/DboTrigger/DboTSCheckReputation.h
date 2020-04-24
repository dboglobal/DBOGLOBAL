#ifndef _DBO_TSCHECKREPUTATION_H_
#define _DBO_TSCHECKREPUTATION_H_


#include "DboTSCoreDefine.h"


/**
	Check reputation condition
*/


class CDboTSCheckReputation : public CNtlTSCond
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	unsigned int						m_uiMinRT;
	unsigned int						m_uiMaxRT;

// Constructions
public:
	CDboTSCheckReputation( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_COND_TYPE_ID_CHECK_REPUTATION; }

	unsigned int						GetMinReputation( void ) const;
	void								SetMinReputation( unsigned int uiRT );

	unsigned int						GetMaxReputation( void ) const;
	void								SetMaxReputation( unsigned int uiRT );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline unsigned int CDboTSCheckReputation::GetMinReputation( void ) const
{
	return m_uiMinRT;
}

inline unsigned int CDboTSCheckReputation::GetMaxReputation( void ) const
{
	return m_uiMaxRT;
}


#endif
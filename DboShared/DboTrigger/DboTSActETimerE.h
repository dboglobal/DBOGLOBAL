#ifndef _DBO_TSACTETIMERE_H_
#define _DBO_TSACTETIMERE_H_


#include "DboTSCoreDefine.h"


/**
	Except timer action end
	Description: The server is present for periodic checks on the exception
	Exceptions timer
*/


class CDboTSActETimerE : public CNtlTSAction
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	NTL_TS_TC_ID						m_tcId;
	NTL_TS_TA_ID						m_taId;

// Constructions
public:
	CDboTSActETimerE( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_ACT_TYPE_ID_ACT_EXCEPT_TIMER_E; }

	NTL_TS_TC_ID						GetTCId( void ) const;
	void								SetTCId( NTL_TS_TC_ID tcId );

	NTL_TS_TA_ID						GetTAId( void ) const;
	void								SetTAId( NTL_TS_TA_ID taId );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline NTL_TS_TC_ID CDboTSActETimerE::GetTCId( void ) const
{
	return m_tcId;
}

inline NTL_TS_TA_ID CDboTSActETimerE::GetTAId( void ) const
{
	return m_taId;
}


#endif
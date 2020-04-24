#ifndef _DBO_TSACTETIMERS_H_
#define _DBO_TSACTETIMERS_H_


#include "DboTSCoreDefine.h"


/**
	Except timer action start
	Description: The server is present for periodic checks on the exception
	Exceptions timer
*/


class CDboTSActETimerS : public CNtlTSAction
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	eEXCEPT_TIMER_SORT					m_eExceptTimerSort;

	// 대기 시간
	unsigned int						m_uiTime;

	// 실행할 Except group id
	NTL_TS_TG_ID						m_tgId;

// Constructions
public:
	CDboTSActETimerS( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_ACT_TYPE_ID_ACT_EXCEPT_TIMER_S; }

	eEXCEPT_TIMER_SORT					GetTimerSort( void ) const;
	void								SetTimerSort( eEXCEPT_TIMER_SORT eExceptTimerSort );

	char								GetTimerId( void ) const;
	void								SetTimerId( unsigned char byTimerId );

	unsigned int						GetTime( void ) const;
	void								SetTime( unsigned int uiTime );

	NTL_TS_TG_ID						GetTGId( void ) const;
	void								SetTGId( NTL_TS_TG_ID tgId );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline eEXCEPT_TIMER_SORT CDboTSActETimerS::GetTimerSort( void ) const
{
	return m_eExceptTimerSort;
}

inline unsigned int CDboTSActETimerS::GetTime( void ) const
{
	return m_uiTime;
}

inline NTL_TS_TG_ID CDboTSActETimerS::GetTGId( void ) const
{
	return m_tgId;
}


#endif
#ifndef _DBO_TMQTIMEBONUS_H_
#define _DBO_TMQTIMEBONUS_H_


#include "DboTSCoreDefine.h"


class CDboTSActTMQTimeBonus : public CNtlTSAction
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	int									m_nBonusTime;

// Constructions
public:
	CDboTSActTMQTimeBonus( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_ACT_TYPE_ID_ACT_TMQ_TIMEBONUS; }

	int									GetBonusTime( void ) const;
	void								SetBonusTime( int nBonusTime );


// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline int CDboTSActTMQTimeBonus::GetBonusTime( void ) const
{
	return m_nBonusTime;
}


inline void CDboTSActTMQTimeBonus::SetBonusTime( int nBonusTime )
{
	m_nBonusTime = nBonusTime;
}


#endif
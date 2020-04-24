#ifndef _DBO_TSCHECKCUSTOMEVENT_H
#define _DBO_TSCHECKCUSTOMEVENT_H


#include "DboTSCoreDefine.h"


/**
	Check custom event condition
*/


class CDboTSCheckCustomEvent : public CNtlTSCond
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	NTL_TS_T_ID							m_tQuestID;

// Constructions
public:
	CDboTSCheckCustomEvent( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_COND_TYPE_ID_CHECK_CUSTOMEVENT; }

	NTL_TS_T_ID							GetQuestID( void ) const;
	void								SetQuestID( NTL_TS_T_ID tQuestID );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline NTL_TS_T_ID CDboTSCheckCustomEvent::GetQuestID( void ) const
{
	return m_tQuestID;
}


#endif
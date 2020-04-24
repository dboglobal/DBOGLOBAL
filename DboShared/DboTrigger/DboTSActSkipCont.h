#ifndef _DBO_SKIP_CONT_H_
#define _DBO_SKIP_CONT_H_


#include "DboTSCoreDefine.h"


class CDboTSActSkipCont : public CNtlTSAction
{
	NTL_TS_DECLARE_RTTI


// Member variables
protected:
	NTL_TS_T_ID							m_tSkipQuestID;
	NTL_TS_TC_ID						m_tcSkipContID;


// Constructions
public:
	CDboTSActSkipCont( void );


// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_ACT_TYPE_ID_ACT_SKIP_CONT; }

	NTL_TS_T_ID							GetSkipQuestID( void ) const;
	void								SetSkipQuestID( NTL_TS_T_ID tQuestID );

	NTL_TS_TC_ID						GetSkipContID( void ) const;
	void								SetSkipContID( NTL_TS_TC_ID tcContID );


// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline NTL_TS_T_ID CDboTSActSkipCont::GetSkipQuestID( void ) const
{
	return m_tSkipQuestID;
}


inline NTL_TS_TC_ID CDboTSActSkipCont::GetSkipContID( void ) const
{
	return m_tcSkipContID;
}


#endif
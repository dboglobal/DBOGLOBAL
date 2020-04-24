#ifndef _DBO_TSRB_H_
#define _DBO_TSRB_H_


#include "DboTSCoreDefine.h"


class CDboTSRB : public CNtlTSEvent
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	unsigned int						m_uiRBTblIdx;
	unsigned int						m_uiRBCondition;	/* eRB_COND_FLAG */

// Constructions
public:
	CDboTSRB( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_EVENT_TYPE_ID_RB; }

	unsigned int						GetRBTblIdx( void ) const;
	void								SetRBTblIdx( unsigned int uiRBTblIdx );

	unsigned int						GetRBCondition( void ) const;
	void								SetRBCondition( unsigned int uiRBCondition );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline unsigned int CDboTSRB::GetRBTblIdx( void ) const
{
	return m_uiRBTblIdx;
}


inline unsigned int CDboTSRB::GetRBCondition( void ) const
{
	return m_uiRBCondition;
}


#endif
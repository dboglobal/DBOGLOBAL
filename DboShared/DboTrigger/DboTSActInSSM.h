#ifndef _DBO_TSACTINSSM_H_
#define _DBO_TSACTINSSM_H_


#include "DboTSCoreDefine.h"


/**
	Input SSM action
	SSM에 주어진 값을 입력한다
	용도 : 모두
	설명 : Slot storage memory에 해당 값을 입력할 때 사용
*/


class CDboTSActInSSM : public CNtlTSAction
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	eSSM_ID								m_eSSMId;
	unsigned int						m_uiValue;	// SSM 에 입력할 값

// Constructions
public:
	CDboTSActInSSM( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_ACT_TYPE_ID_ACT_INSSM; }

	eSSM_ID								GetSSMId( void ) const;
	void								SetSSMId( eSSM_ID eSSMId );

	unsigned int						GetValue( void ) const;
	void								SetValue( unsigned int uiValue );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline eSSM_ID CDboTSActInSSM::GetSSMId( void ) const
{
	return m_eSSMId;
}

inline unsigned int CDboTSActInSSM::GetValue( void ) const
{
	return m_uiValue;
}


#endif
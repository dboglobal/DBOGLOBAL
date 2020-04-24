#ifndef _DBO_TSACTTMQSTAGE_H_
#define _DBO_TSACTTMQSTAGE_H_


#include "DboTSCoreDefine.h"


class CDboTSActTMQStage : public CNtlTSAction
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	unsigned char						m_byStage;

// Constructions
public:
	CDboTSActTMQStage( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_ACT_TYPE_ID_ACT_TMQ_STAGE; }

	unsigned char						GetTMQStage( void ) const;
	void								SetTMQStage( unsigned char byStage );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline unsigned char CDboTSActTMQStage::GetTMQStage( void ) const
{
	return m_byStage;
}


#endif
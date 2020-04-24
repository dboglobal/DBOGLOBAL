#ifndef _DBO_TSACTPCCONV_H_
#define _DBO_TSACTPCCONV_H_


#include "DboTSCoreDefine.h"


/**
	PC conversation action
*/


class CDboTSActPCConv : public CNtlTSAction
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	unsigned int						m_uiConvTblIdx;

// Constructions
public:
	CDboTSActPCConv( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_ACT_TYPE_ID_ACT_PCCONV; }

	unsigned int						GetConvTblIdx( void ) const;
	void								SetConvTblIdx( unsigned int uiConvTblIdx );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline unsigned int CDboTSActPCConv::GetConvTblIdx( void ) const
{
	return m_uiConvTblIdx;
}


#endif
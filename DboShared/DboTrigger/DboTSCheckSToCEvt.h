#ifndef _DBO_TSCHECKSTOCEVT_H
#define _DBO_TSCHECKSTOCEVT_H


#include "DboTSCoreDefine.h"


/**
	Check server to client event condition
	용도 : Client quest, client trigger, server quest, server trigger pc
*/


class CDboTSCheckSToCEvt : public CNtlTSCond
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	eSTOC_EVT_DATA_TYPE					m_eSToCEvtDataType;

// Constructions
public:
	CDboTSCheckSToCEvt( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_COND_TYPE_ID_CHECK_STOCEVT; }

	eSTOC_EVT_DATA_TYPE					GetSToCEvtDataType( void ) const;
	void								SetSToCEvtDataType( eSTOC_EVT_DATA_TYPE eSToCEvtDataType );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline eSTOC_EVT_DATA_TYPE CDboTSCheckSToCEvt::GetSToCEvtDataType( void ) const
{
	return m_eSToCEvtDataType;
}


#endif
#ifndef _DBO_TSITEMIDENTITY_H_
#define _DBO_TSITEMIDENTITY_H_


#include "DboTSCoreDefine.h"


/**
	Item identity event
*/


class CDboTSItemIdentity : public CNtlTSEvent
{
	NTL_TS_DECLARE_RTTI

// Constructions
public:
	CDboTSItemIdentity( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_EVENT_TYPE_ID_ITEMIDENTITY; }

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


#endif
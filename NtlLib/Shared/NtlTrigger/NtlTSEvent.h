#ifndef _NTL_TSEVENT_H_
#define _NTL_TSEVENT_H_


#include "NtlTSEntity.h"


/**
	Event entity
*/


class CNtlTSEvent : public CNtlTSEntity
{
	NTL_TS_DECLARE_RTTI

// Constructions and Destructions
public:
	CNtlTSEvent( void );
	virtual ~CNtlTSEvent( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return NTL_TSENTITY_TYPE_EVENT_BEGIN; }

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& ) { return; }
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& ) { return; }
};


#endif
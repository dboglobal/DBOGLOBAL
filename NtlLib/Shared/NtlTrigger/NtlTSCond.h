#ifndef _NTL_TSCOND_H_
#define _NTL_TSCOND_H_


#include "NtlTSEntity.h"


/**
	Condition entity
*/

class CNtlTSCond : public CNtlTSEntity
{
	NTL_TS_DECLARE_RTTI

// Constructions and Destructions
public:
	CNtlTSCond( void  );
	virtual ~CNtlTSCond( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return NTL_TSENTITY_TYPE_COND_BEGIN; }

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& ) { return; }
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& ) { return; }
};


#endif
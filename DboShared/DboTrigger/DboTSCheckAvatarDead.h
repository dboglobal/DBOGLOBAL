#ifndef _DBO_TSCHECKAVATARDEAD_H_
#define _DBO_TSCHECKAVATARDEAD_H_


#include "DboTSCoreDefine.h"


class CDboTSCheckAvatarDead : public CNtlTSCond
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:

// Constructions
public:
	CDboTSCheckAvatarDead( void );


// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_COND_TYPE_ID_CHECK_AVATARDEAD; }


// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


#endif
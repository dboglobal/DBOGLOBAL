#ifndef _NTL_TSACTION_H_
#define _NTL_TSACTION_H_


#include "NtlTSEntity.h"


/**
	Action entity
*/


class CNtlTSAction : public CNtlTSEntity
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	NTL_TS_TA_ID						m_taId;

// Constructions and Destructions
public:
	CNtlTSAction( void ) { return; }
	virtual ~CNtlTSAction( void ) { return; }

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return NTL_TSENTITY_TYPE_ACT_BEGIN; }

	NTL_TS_TA_ID						GetActionId( void ) const;
	void								SetActionId( NTL_TS_TA_ID taId );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline NTL_TS_TA_ID CNtlTSAction::GetActionId( void ) const
{
	return m_taId;
}


#endif
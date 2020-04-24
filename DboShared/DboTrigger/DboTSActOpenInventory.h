#ifndef _DBO_TSACTOPENINVENTORY_H_
#define _DBO_TSACTOPENINVENTORY_H_


#include "DboTSCoreDefine.h"


class CDboTSActOpenInventory : public CNtlTSAction
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	bool								m_bShow;

// Constructions
public:
	CDboTSActOpenInventory( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_ACT_TYPE_ID_ACT_OPEN_INVENTORY; }

	bool								GetShow( void ) const;
	void								SetShow( bool bShow );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline bool CDboTSActOpenInventory::GetShow( void ) const
{
	return m_bShow;
}


#endif
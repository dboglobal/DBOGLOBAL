#ifndef _DBO_DROP_H_
#define _DBO_DROP_H_


#include "DboTSCoreDefine.h"


class CDboTSActDrop : public CNtlTSAction
{
	NTL_TS_DECLARE_RTTI


// Member variables
protected:
	eQUEST_DROP_PERMISSION_TYPE			m_ePermissionType;		// Permission type
	unsigned int						m_uiProbDropTblIdx;		// Probability Drop table index

// Constructions
public:
	CDboTSActDrop( void );


// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_ACT_TYPE_ID_ACT_DROP; }

	eQUEST_DROP_PERMISSION_TYPE			GetPermissionType( void ) const;
	void								SetPermissionType( eQUEST_DROP_PERMISSION_TYPE eType );

	unsigned int						GetTableIndex( void ) const;
	void								SetTableIndex( unsigned int uiProbDropTblIdx );


// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline unsigned int CDboTSActDrop::GetTableIndex( void ) const
{
	return m_uiProbDropTblIdx;
}

inline eQUEST_DROP_PERMISSION_TYPE CDboTSActDrop::GetPermissionType( void ) const
{
	return m_ePermissionType;
}


#endif
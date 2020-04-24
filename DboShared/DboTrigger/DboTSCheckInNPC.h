#ifndef _DBO_TSCHECKINNPC_H_
#define _DBO_TSCHECKINNPC_H_


#include "DboTSCoreDefine.h"


/**
	Check in NPC
*/


class CDboTSCheckInNPC : public CNtlTSCond
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	unsigned int						m_uiNPCIdx;
	int									m_nRadius;

// Constructions
public:
	CDboTSCheckInNPC( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_COND_TYPE_ID_CHECK_IN_NPC; }

	unsigned int						GetNPCIndex( void ) const;
	void								SetNPCIndex( unsigned int uiNPCIdx );

	int									GetRadius( void ) const;
	void								SetRadius( int nRadius );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline unsigned int CDboTSCheckInNPC::GetNPCIndex( void ) const
{
	return m_uiNPCIdx;
}

inline int CDboTSCheckInNPC::GetRadius( void ) const
{
	return m_nRadius;
}


#endif
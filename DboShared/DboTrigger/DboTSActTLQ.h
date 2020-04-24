#ifndef _DBO_TLQ_H_
#define _DBO_TLQ_H_


#include "DboTSCoreDefine.h"


class CDboTSActTLQ : public CNtlTSAction
{
	NTL_TS_DECLARE_RTTI


// Member variables
protected:
	eTLQ_DUNGEON_TYPE					m_eDungeonType;
	unsigned int						m_uiDungeonTblIdx;


// Constructions
public:
	CDboTSActTLQ( void );


// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_ACT_TYPE_ID_ACT_TLQ; }

	eTLQ_DUNGEON_TYPE					GetDungeonType( void ) const;
	void								SetDungeonType( eTLQ_DUNGEON_TYPE eDungeonType );

	unsigned int						GetDungeonTblIdx( void ) const;
	void								SetDungeonTblIdx( unsigned int uiTblIdx );


// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline eTLQ_DUNGEON_TYPE CDboTSActTLQ::GetDungeonType( void ) const
{
	return m_eDungeonType;
}

inline unsigned int CDboTSActTLQ::GetDungeonTblIdx( void ) const
{
	return m_uiDungeonTblIdx;
}


#endif
#ifndef _DBO_PARTY_INSTANCE_DUNGEON_H_
#define _DBO_PARTY_INSTANCE_DUNGEON_H_


#include "DboTSCoreDefine.h"


class CDboTSActPIDgn : public CNtlTSAction
{
	NTL_TS_DECLARE_RTTI


// Member variables
protected:
	ePARTY_DUNGEON_TYPE					m_eDungeonType;
	unsigned int						m_uiNormalDungeonTblIdx;
	unsigned int						m_uiHardDungeonTblIdx;

	unsigned int						m_uiRDungeonTblIdx;

// Constructions
public:
	CDboTSActPIDgn( void );


// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_ACT_TYPE_ID_ACT_PIDGN; }

	ePARTY_DUNGEON_TYPE					GetDungeonType( void ) const;
	void								SetDungeonType( ePARTY_DUNGEON_TYPE eDungeonType );

	unsigned int						GetNormalDungeonTblIdx( void ) const;
	void								SetNormalDungeonTblIdx( unsigned int uiTblIdx );

	unsigned int						GetHardDungeonTblIdx( void ) const;
	void								SetHardDungeonTblIdx( unsigned int uiTblIdx );

	unsigned int						GetRDungeonTblIdx( void ) const;
	void								SetRDungeonTblIdx( unsigned int uiTblIdx );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline ePARTY_DUNGEON_TYPE CDboTSActPIDgn::GetDungeonType( void ) const
{
	return m_eDungeonType;
}

inline unsigned int CDboTSActPIDgn::GetNormalDungeonTblIdx( void ) const
{
	return m_uiNormalDungeonTblIdx;
}


inline unsigned int CDboTSActPIDgn::GetHardDungeonTblIdx( void ) const
{
	return m_uiHardDungeonTblIdx;
}

inline unsigned int CDboTSActPIDgn::GetRDungeonTblIdx( void ) const
{
	return m_uiRDungeonTblIdx;
}

#endif
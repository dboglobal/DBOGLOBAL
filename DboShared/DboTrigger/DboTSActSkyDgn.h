#ifndef _DBO_SKY_DUNGEON_H_
#define _DBO_SKY_DUNGEON_H_


#include "DboTSCoreDefine.h"


class CDboTSActSkyDgn : public CNtlTSAction
{
	NTL_TS_DECLARE_RTTI


// Member variables
protected:
	ePARTY_DUNGEON_TYPE					m_eSkyDungeonType;
	unsigned int						m_uiSkyDungeonTblIdx;

// Constructions
public:
	CDboTSActSkyDgn( void );


// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_ACT_TYPE_ID_ACT_SKYDGN; }

	//TO-DO FIND OUT IF ITS "ePARTY_DUNGEON_TYPE"
	ePARTY_DUNGEON_TYPE					GetSkyDungeonType( void ) const;
	void								SetSkyDungeonType( ePARTY_DUNGEON_TYPE eDungeonType );

	unsigned int						GetSkyDungeonTblIdx( void ) const;
	void								SetSkyDungeonTblIdx( unsigned int uiTblIdx );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline ePARTY_DUNGEON_TYPE CDboTSActSkyDgn::GetSkyDungeonType( void ) const
{
	return m_eSkyDungeonType;
}

inline unsigned int CDboTSActSkyDgn::GetSkyDungeonTblIdx( void ) const
{
	return m_uiSkyDungeonTblIdx;
}

#endif
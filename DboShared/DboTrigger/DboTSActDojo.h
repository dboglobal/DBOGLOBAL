#ifndef _DBO_DOJO_H_
#define _DBO_DOJO_H_


#include "DboTSCoreDefine.h"


class CDboTSActDojo : public CNtlTSAction
{
	NTL_TS_DECLARE_RTTI


// Member variables
protected:
	eDOJO_DUNGEON_TYPE					m_eDungeonType;
	unsigned int						m_uiDojoTblIdx;


// Constructions
public:
	CDboTSActDojo( void );


// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_ACT_TYPE_ID_ACT_DOJO; }

	eDOJO_DUNGEON_TYPE					GetDungeonType( void ) const;
	void								SetDungeonType( eDOJO_DUNGEON_TYPE eDungeonType );

	unsigned int						GetDojoTblIdx( void ) const;
	void								SetDojoTblIdx( unsigned int uiTblIdx );


// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline eDOJO_DUNGEON_TYPE CDboTSActDojo::GetDungeonType( void ) const
{
	return m_eDungeonType;
}

inline unsigned int CDboTSActDojo::GetDojoTblIdx( void ) const
{
	return m_uiDojoTblIdx;
}


#endif
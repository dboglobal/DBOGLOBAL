#ifndef _DBO_TSCHECKPCRACE_H
#define _DBO_TSCHECKPCRACE_H


#include "DboTSCoreDefine.h"


/**
	Check pc race condition
	용도 : Client quest, client trigger, server quest, server trigger pc
*/


class CDboTSCheckPCRace : public CNtlTSCond
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	unsigned int						m_uiRaceFlags;

// Constructions
public:
	CDboTSCheckPCRace( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_COND_TYPE_ID_CHECK_PCRACE; }

	unsigned int						GetRaceFlags( void ) const;
	void								SetRaceFlags( unsigned int uiRaceFlags );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline unsigned int CDboTSCheckPCRace::GetRaceFlags( void ) const
{
	return m_uiRaceFlags;
}


#endif
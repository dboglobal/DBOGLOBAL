#ifndef _DBO_TSSKILLUSE_H_
#define _DBO_TSSKILLUSE_H_


#include "DboTSCoreDefine.h"


class CDboTSSkillUse : public CNtlTSEvent
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	eEVENT_SKILL_TYPE					m_eSkillType;
	unsigned int						m_uiSkillIdx;
	eEVENT_SKILL_TARGET_TYPE			m_eSkillTargetType;
	unsigned int						m_uiSkillTargetIdx;
	unsigned int						m_uiSkillRPFlags;

// Constructions
public:
	CDboTSSkillUse( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_EVENT_TYPE_ID_SKILL_USE; }

	eEVENT_SKILL_TYPE					GetSkillType( void ) const;
	void								SetSkillType( eEVENT_SKILL_TYPE eSkillType );

	unsigned int						GetSkillIdx( void ) const;
	void								SetSkillIdx( unsigned int uiSkillIdx );

	eEVENT_SKILL_TARGET_TYPE			GetSkillTargetType( void ) const;
	void								SetSkillTargetType( eEVENT_SKILL_TARGET_TYPE eSkillTargetType );

	unsigned int						GetSkillTargetIdx( void ) const;
	void								SetSkillTargetIdx( unsigned int uiSkillTargetIdx );

	unsigned int						GetSkillRPFlags( void ) const;
	void								SetSkillRPFlags( unsigned int uiSkillRPFlags );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline eEVENT_SKILL_TYPE CDboTSSkillUse::GetSkillType( void ) const
{
	return m_eSkillType;
}

inline unsigned int CDboTSSkillUse::GetSkillIdx( void ) const
{
	return m_uiSkillIdx;
}

inline eEVENT_SKILL_TARGET_TYPE CDboTSSkillUse::GetSkillTargetType( void ) const
{
	return m_eSkillTargetType;
}

inline unsigned int CDboTSSkillUse::GetSkillTargetIdx( void ) const
{
	return m_uiSkillTargetIdx;
}

inline unsigned int CDboTSSkillUse::GetSkillRPFlags( void ) const
{
	return m_uiSkillRPFlags;
}


#endif
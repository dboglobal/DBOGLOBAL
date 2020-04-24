#ifndef _DBO_TSACTBROADMSG_H_
#define _DBO_TSACTBROADMSG_H_


#include "DboTSCoreDefine.h"


/**
	Broad msg action
*/


class CDboTSActBroadMsg : public CNtlTSAction
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	eBROAD_MSG_EXCUTION_TYPE			m_eExcuteType;
	eBROAD_MSG_UI_DIR_TYPE				m_eUIDirType;
	eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE		m_eUIShowHideDirType;
	eBROAD_MSG_UI_BALLOON_SHAPE_TYPE	m_eUIBalloonShapeType;
	eBROAD_MSG_UI_SPEECH_DIR_TYPE		m_eUISpeechDirType;
	eBROAD_MSG_OWNER_CONDITION			m_eOwnerCondition;
	eBROAD_MSG_OWNER_TYPE				m_eOwnerType;
	unsigned int						m_uiOwnerTblIdx;
	unsigned int						m_uiQTextTblIdx;
	unsigned int						m_uiDisplayTime;		// 초

// Constructions
public:
	CDboTSActBroadMsg( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_ACT_TYPE_ID_ACT_BROAD_MSG; }

	eBROAD_MSG_EXCUTION_TYPE			GetExcutionType( void ) const;
	eBROAD_MSG_UI_DIR_TYPE				GetUIDirType( void ) const;
	eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE		GetUIShowHideDirType( void ) const;
	eBROAD_MSG_UI_BALLOON_SHAPE_TYPE	GetUIBalloonShapeType( void ) const;
	eBROAD_MSG_UI_SPEECH_DIR_TYPE		GetUISpeechDirType( void ) const;
	eBROAD_MSG_OWNER_CONDITION			GetOwnerCondition( void ) const;
	eBROAD_MSG_OWNER_TYPE				GetOwnerType( void ) const;
	unsigned int						GetOwnerTableIndex( void ) const;
	unsigned int						GetQuestTextTableIndex( void ) const;
	unsigned int						GetDisplayTime( void ) const;

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline eBROAD_MSG_EXCUTION_TYPE CDboTSActBroadMsg::GetExcutionType( void ) const
{
	return m_eExcuteType;
}

inline eBROAD_MSG_UI_DIR_TYPE CDboTSActBroadMsg::GetUIDirType( void ) const
{
	return m_eUIDirType;
}

inline eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE CDboTSActBroadMsg::GetUIShowHideDirType( void ) const
{
	return m_eUIShowHideDirType;
}

inline eBROAD_MSG_UI_BALLOON_SHAPE_TYPE CDboTSActBroadMsg::GetUIBalloonShapeType( void ) const
{
	return m_eUIBalloonShapeType;
}

inline eBROAD_MSG_UI_SPEECH_DIR_TYPE CDboTSActBroadMsg::GetUISpeechDirType( void ) const
{
	return m_eUISpeechDirType;
}

inline eBROAD_MSG_OWNER_CONDITION CDboTSActBroadMsg::GetOwnerCondition( void ) const
{
	return m_eOwnerCondition;
}

inline eBROAD_MSG_OWNER_TYPE CDboTSActBroadMsg::GetOwnerType( void ) const
{
	return m_eOwnerType;
}

inline unsigned int CDboTSActBroadMsg::GetOwnerTableIndex( void ) const
{
	return m_uiOwnerTblIdx;
}

inline unsigned int CDboTSActBroadMsg::GetQuestTextTableIndex( void ) const
{
	return m_uiQTextTblIdx;
}

inline unsigned int CDboTSActBroadMsg::GetDisplayTime( void ) const
{
	return m_uiDisplayTime;
}


#endif
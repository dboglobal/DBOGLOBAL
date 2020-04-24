#ifndef _DBO_TSACTOBJSTATE_H_
#define _DBO_TSACTOBJSTATE_H_


#include "DboTSCoreDefine.h"


/**
	Object state
*/


class CDboTSActObjState : public CNtlTSAction
{
	NTL_TS_DECLARE_RTTI

// Declarations
public:
	enum eMAIN_STATE
	{
		eMAIN_STATE_0					= 0,
		eMAIN_STATE_1					= 1,
		eMAIN_STATE_2					= 2,
		eMAIN_STATE_3					= 3,
		eMAIN_STATE_4					= 4,
		eMAIN_STATE_5					= 5,
		eMAIN_STATE_6					= 6,
		eMAIN_STATE_7					= 7,
		eMAIN_STATE_8					= 8,
		eMAIN_STATE_9					= 9,
		eMAIN_STATE_IGNORE				= 255
	};

	enum eSUB_STATE_OBJECT_SH
	{
		SUB_STATE_OBJECT_SH_HIDE		= 0,
		SUB_STATE_OBJECT_SH_SHOW		= 1,
		SUB_STATE_OBJECT_SH_IGNORE		= 255,
	};

	enum eSUB_STATE_OBJECT_UC
	{
		eSUB_STATE_OBJECT_UC_CHECK		= 0,
		eSUB_STATE_OBJECT_UC_UNCHECK	= 1,
		eSUB_STATE_OBJECT_UC_IGNORE		= 255,
	};


// Member variables
protected:
	unsigned int						m_uiWorldIdx;
	unsigned int						m_uiObjIdx;
	eMAIN_STATE							m_eObjMainState;
	eSUB_STATE_OBJECT_SH				m_eObjectShowHide;
	eSUB_STATE_OBJECT_UC				m_eObjectCheckUncheck;

// Constructions
public:
	CDboTSActObjState( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_ACT_TYPE_ID_ACT_OBJSTATE; }

	unsigned int						GetWorldIdx( void ) const;
	void								SetWorldIdx( unsigned int uiWorldIdx );

	unsigned int						GetObjectIdx( void ) const;
	void								SetObjectIdx( unsigned int uiObjectIdx );

	eMAIN_STATE							GetObjectMainState( void ) const;
	void								SetObjectMainState( eMAIN_STATE eObjectMainState );

	eSUB_STATE_OBJECT_SH				GetObjectShowHide( void ) const;
	void								SetObjectShowHide( eSUB_STATE_OBJECT_SH eObjectShowHide );

	eSUB_STATE_OBJECT_UC				GetObjectCheckUncheck( void ) const;
	void								SetObjectCheckUncheck( eSUB_STATE_OBJECT_UC eObjectCheckUncheck );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline unsigned int CDboTSActObjState::GetWorldIdx( void ) const
{
	return m_uiWorldIdx;
}


inline unsigned int CDboTSActObjState::GetObjectIdx( void ) const
{
	return m_uiObjIdx;
}


inline CDboTSActObjState::eMAIN_STATE CDboTSActObjState::GetObjectMainState( void ) const
{
	return m_eObjMainState;
}


inline CDboTSActObjState::eSUB_STATE_OBJECT_SH CDboTSActObjState::GetObjectShowHide( void ) const
{
	return m_eObjectShowHide;
}


inline CDboTSActObjState::eSUB_STATE_OBJECT_UC CDboTSActObjState::GetObjectCheckUncheck( void ) const
{
	return m_eObjectCheckUncheck;
}


#endif
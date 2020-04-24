#ifndef _DBO_TSACTCONTNARRATION_H_
#define _DBO_TSACTCONTNARRATION_H_


#include "DboTSContGAct.h"


/**
	Narration container
*/


class CDboTSContNarration : public CNtlTSCont
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	eNARRATION_PROGRESS_STATE			m_eProgState;		// Narration 진행 상태

	eNARRATION_OWNER_TYPE				m_eOwnerType;		// Owner의 종류
	unsigned int						m_uiOwnerIdx;		// Owner의 테이블 인덱스

	eNARRATION_OWNER_STATE				m_eOwnerState;		// Owner의 상태

	eNARRATION_DIALOG_DIR_TYPE			m_eDialogDirType;	// 대화 연출 종류
	unsigned int						m_uiDialog;			// 대화 내용

	eNARRATION_GUI_TYPE					m_eGUIType;			// GUI 종류

	unsigned int						m_uiMaxLifeTime;	// 이 시간 이상 나레이션 창이 유지 되는 경우 자동으로 Next로 넘어가야 한다.

	NTL_TS_TC_ID						m_tcOkLink;
	NTL_TS_TC_ID						m_tcCancelLink;
	NTL_TS_TC_ID						m_tcLogInLink;

// Constructions and Destructions
public:
	CDboTSContNarration( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_CONT_TYPE_ID_CONT_NARRATION; }

	eNARRATION_PROGRESS_STATE			GetProgressState( void ) const;
	void								SetProgressState( eNARRATION_PROGRESS_STATE eState );

	eNARRATION_OWNER_TYPE				GetOwnerType( void ) const;
	void								SetOwnerType( eNARRATION_OWNER_TYPE eType );

	unsigned int						GetOwnerIdx( void ) const;
	void								SetOwnerIdx( unsigned int uiIdx );

	eNARRATION_OWNER_STATE				GetOwnerState( void ) const;
	void								SetOwnerState( eNARRATION_OWNER_STATE eState );

	eNARRATION_DIALOG_DIR_TYPE			GetDialogDirType( void ) const;
	void								SetDialogDirType( eNARRATION_DIALOG_DIR_TYPE eType );

	unsigned int						GetDialog( void ) const;
	void								SetDialog( unsigned int uiDialog );

	eNARRATION_GUI_TYPE					GetGUIType( void ) const;
	void								SetGUIType( eNARRATION_GUI_TYPE eType );

	unsigned int						GetMaxLifeTime( void ) const;
	void								SetMaxLifeTime( unsigned int uiMaxLifeTime );

	NTL_TS_TC_ID						GetOkLink( void ) const;
	void								SetOkLink( NTL_TS_TC_ID tcId );

	NTL_TS_TC_ID						GetCancelLink( void ) const;
	void								SetCancelLink( NTL_TS_TC_ID tcId );

	NTL_TS_TC_ID						GetLogInLink( void ) const;
	void								SetLogInLink( NTL_TS_TC_ID tcId );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};

inline eNARRATION_PROGRESS_STATE CDboTSContNarration::GetProgressState( void ) const
{
	return m_eProgState;
}

inline eNARRATION_OWNER_TYPE CDboTSContNarration::GetOwnerType( void ) const
{
	return m_eOwnerType;
}

inline unsigned int CDboTSContNarration::GetOwnerIdx( void ) const
{
	return m_uiOwnerIdx;
}

inline eNARRATION_OWNER_STATE CDboTSContNarration::GetOwnerState( void ) const
{
	return m_eOwnerState;
}

inline eNARRATION_DIALOG_DIR_TYPE CDboTSContNarration::GetDialogDirType( void ) const
{
	return m_eDialogDirType;
}

inline unsigned int CDboTSContNarration::GetDialog( void ) const
{
	return m_uiDialog;
}

inline eNARRATION_GUI_TYPE CDboTSContNarration::GetGUIType( void ) const
{
	return m_eGUIType;
}

inline unsigned int CDboTSContNarration::GetMaxLifeTime( void ) const
{
	return m_uiMaxLifeTime;
}

inline NTL_TS_TC_ID CDboTSContNarration::GetOkLink( void ) const
{
	return m_tcOkLink;
}

inline NTL_TS_TC_ID CDboTSContNarration::GetCancelLink( void ) const
{
	return m_tcCancelLink;
}

inline NTL_TS_TC_ID CDboTSContNarration::GetLogInLink( void ) const
{
	return m_tcLogInLink;
}


#endif
#ifndef _DBO_TSACTOPENWINDOW_H_
#define _DBO_TSACTOPENWINDOW_H_


#include "DboTSCoreDefine.h"


class CDboTSActOpenWindow : public CNtlTSAction
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	bool								m_bShow;
	eWINDOW_TYPE						m_eWindowType;
	unsigned int						m_uiTblIdx;

// Constructions
public:
	CDboTSActOpenWindow( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_ACT_TYPE_ID_ACT_OPEN_WINDOW; }

	bool								IsShow( void ) const;
	void								SetShow( bool bShow );

	eWINDOW_TYPE						GetWindowType( void ) const;
	void								SetWindowType( eWINDOW_TYPE eWindowType );

	unsigned int						GetTableIdx( void ) const;
	void								SetTableIdx( unsigned int uiTblIdx );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline bool CDboTSActOpenWindow::IsShow( void ) const
{
	return m_bShow;
}


inline eWINDOW_TYPE CDboTSActOpenWindow::GetWindowType( void ) const
{
	return m_eWindowType;
}


inline unsigned int CDboTSActOpenWindow::GetTableIdx( void ) const
{
	return m_uiTblIdx;
}


#endif
#ifndef _DBO_TSACTCONTUNIFIEDNARRATION_H_
#define _DBO_TSACTCONTUNIFIEDNARRATION_H_


#include "DboTSCoreDefine.h"


/**
	Unified narration container
*/


class CDboTSContUnifiedNarration : public CNtlTSCont
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	unsigned int						m_uiNarrationTblIdx;

	NTL_TS_TC_ID						m_tcOkLink;
	NTL_TS_TC_ID						m_tcCancelLink;
	NTL_TS_TC_ID						m_tcLogInLink;

// Constructions and Destructions
public:
	CDboTSContUnifiedNarration( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_CONT_TYPE_ID_CONT_UNIFIED_NARRATION; }

	unsigned int						GetNarrationTblIdx( void ) const;
	void								SetNarrationTblIdx( unsigned int uiNarrationTblIdx );

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


inline unsigned int CDboTSContUnifiedNarration::GetNarrationTblIdx( void ) const
{
	return m_uiNarrationTblIdx;
}

inline NTL_TS_TC_ID CDboTSContUnifiedNarration::GetOkLink( void ) const
{
	return m_tcOkLink;
}

inline NTL_TS_TC_ID CDboTSContUnifiedNarration::GetCancelLink( void ) const
{
	return m_tcCancelLink;
}

inline NTL_TS_TC_ID CDboTSContUnifiedNarration::GetLogInLink( void ) const
{
	return m_tcLogInLink;
}


#endif
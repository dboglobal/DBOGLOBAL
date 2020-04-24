#ifndef _DBO_TSACTCONTGACT_H_
#define _DBO_TSACTCONTGACT_H_


#include "DboTSCoreDefine.h"


/**
	General action container
*/


class CDboTSContGAct : public CNtlTSCont
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	NTL_TS_TC_ID						m_tcNextLinkId;
	NTL_TS_TC_ID						m_tcErrorLinkId;

// Constructions
public:
	CDboTSContGAct( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_CONT_TYPE_ID_CONT_GACT; }

	// Next link container
	NTL_TS_TC_ID						GetNextLinkID( void ) const;
	bool								AttachNextLink( NTL_TS_TC_ID tcId );
	void								DetachNextLink( void );

	// Error link container
	NTL_TS_TC_ID						GetErrorLinkID( void ) const;
	bool								AttachErrorLink( NTL_TS_TC_ID tcId );
	void								DetachErrorLink( void );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline NTL_TS_TC_ID CDboTSContGAct::GetNextLinkID( void ) const
{
	return m_tcNextLinkId;
}

inline NTL_TS_TC_ID CDboTSContGAct::GetErrorLinkID( void ) const
{
	return m_tcErrorLinkId;
}


#endif
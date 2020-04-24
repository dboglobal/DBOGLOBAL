#ifndef _DBO_TSACTSTOCEVT_H_
#define _DBO_TSACTSTOCEVT_H_


#include "DboTSCoreDefine.h"


/**
	Server to client event action
	Purpose: Client quest, server quest
	Description: the server periodically and they bring a message for the eSTOC_EVT_TYPE
	Client is prepared to output the information to the display
*/


class CDboTSActSToCEvt : public CNtlTSAction
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	eSTOC_EVT_TYPE						m_eEvtType;
	bool								m_bApplyTypeWorld;
	eSTOC_EVT_DATA_TYPE					m_eEvtDataType;
	uSTOC_EVT_DATA						m_uEvtData;
	eSTOC_EVT_COND_DATA_TYPE			m_eEvtCondDataType;
	uSTOC_EVT_COND_DATA					m_uEvtCondData;
	unsigned int						m_uiEvtSubCondDataType;
	sSTOC_EVT_SUB_COND_DATA				m_sEvtSubCondData;

// Constructions
public:
	CDboTSActSToCEvt( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_ACT_TYPE_ID_ACT_STOCEVT; }

	eSTOC_EVT_TYPE						GetEvtType( void ) const;
	void								SetEvtType( eSTOC_EVT_TYPE eEvtType );

	bool								IsApplyTypeWorld( void ) const;
	void								SetApplyTypeWorld( bool bApplyTypeWorld );

	eSTOC_EVT_DATA_TYPE					GetEvtDataType( void ) const;
	void								SetEvtDataType( eSTOC_EVT_DATA_TYPE eEvtDataType );

	const uSTOC_EVT_DATA&				GetEvtData( void ) const;
	void								SetEvtData( const uSTOC_EVT_DATA& uEvtData );

	eSTOC_EVT_COND_DATA_TYPE			GetEvtCondDataType( void ) const;
	void								SetEvtCondDataType( eSTOC_EVT_COND_DATA_TYPE eEvtCondDataType );

	const uSTOC_EVT_COND_DATA&			GetEvtCondData( void ) const;
	void								SetEvtCondData( const uSTOC_EVT_COND_DATA& uEvtCondData );

	unsigned int						GetEvtSubCondDataType( void ) const;
	void								SetEvtSubCondDataType( unsigned int uiEvtSubCondDataType );

	const sSTOC_EVT_SUB_COND_DATA&		GetEvtSubCondData( void ) const;
	void								SetEvtSubCondData( const sSTOC_EVT_SUB_COND_DATA& sEvtSubCondData );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline eSTOC_EVT_TYPE CDboTSActSToCEvt::GetEvtType( void ) const
{
	return m_eEvtType;
}

inline bool CDboTSActSToCEvt::IsApplyTypeWorld( void ) const
{
	return m_bApplyTypeWorld;
}

inline eSTOC_EVT_DATA_TYPE CDboTSActSToCEvt::GetEvtDataType( void ) const
{
	return m_eEvtDataType;
}

inline const uSTOC_EVT_DATA& CDboTSActSToCEvt::GetEvtData( void ) const
{
	return m_uEvtData;
}

inline eSTOC_EVT_COND_DATA_TYPE CDboTSActSToCEvt::GetEvtCondDataType( void ) const
{
	return m_eEvtCondDataType;
}

inline const uSTOC_EVT_COND_DATA& CDboTSActSToCEvt::GetEvtCondData( void ) const
{
	return m_uEvtCondData;
}

inline unsigned int CDboTSActSToCEvt::GetEvtSubCondDataType( void ) const
{
	return m_uiEvtSubCondDataType;
}

inline const sSTOC_EVT_SUB_COND_DATA& CDboTSActSToCEvt::GetEvtSubCondData( void ) const
{
	return m_sEvtSubCondData;
}


#endif
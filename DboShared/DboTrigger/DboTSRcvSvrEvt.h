#ifndef _DBO_TSRCVSVREVT_H_
#define _DBO_TSRCVSVREVT_H_


#include "DboTSCoreDefine.h"


class CDboTSRcvSvrEvt : public CNtlTSEvent
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	unsigned int						m_uiEvtID;

// Constructions
public:
	CDboTSRcvSvrEvt( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_EVENT_TYPE_ID_RCV_SVR_EVT; }

	unsigned int						GetEvtID( void ) const;
	void								SetEvtID( unsigned int uiEvtID );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline unsigned int CDboTSRcvSvrEvt::GetEvtID( void ) const
{
	return m_uiEvtID;
}


#endif
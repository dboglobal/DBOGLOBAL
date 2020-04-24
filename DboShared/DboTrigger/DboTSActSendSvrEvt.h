#ifndef _DBO_TSACTSENDSVREVT_H_
#define _DBO_TSACTSENDSVREVT_H_


#include "DboTSCoreDefine.h"


class CDboTSActSendSvrEvt : public CNtlTSAction
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	unsigned int						m_uiType;			// eTRIGGER_SERVER_EVENT_TYPE
	unsigned char						m_byTriggerType;	// TS_TYPE_QUEST_CS, TS_TYPE_PC_TRIGGER_CS, TS_TYPE_OBJECT_TRIGGER_S, TS_TYPE_INVALID
	unsigned int						m_uiEvtID;
	unsigned int						m_uiTblIdx;			// eTRIGGER_SERVER_EVENT_TYPE의 종류에 따른 인덱스
	unsigned int						m_uiEvtSendType;	// eTS_EVENT_SEND_TYPE
	float								m_fEvtSendType_Radius;

// Constructions
public:
	CDboTSActSendSvrEvt( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_ACT_TYPE_ID_ACT_SEND_SVR_EVT; }

	unsigned int						GetSvrEvtType( void ) const;
	void								SetSvrEvtType( unsigned int uiType );

	unsigned char						GetSvrEvtTriggerType( void ) const;
	void								SetSvrEvtTriggerType( unsigned char byTriggerType );

	unsigned int						GetSvrEvtID( void ) const;
	void								SetSvrEvtID( unsigned int uiEvtID );

	unsigned int						GetTblIdx( void ) const;
	void								SetTblIdx( unsigned int uiTblIdx );

	unsigned int						GetEvtSendType( void ) const;
	void								SetEvtSendType( unsigned int uiEvtSendType );

	float								GetEvtSendType_Radius( void ) const;
	void								SetEvtSendType_Radius( float fRadius );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline unsigned int CDboTSActSendSvrEvt::GetSvrEvtType( void ) const
{
	return m_uiType;
}

inline unsigned char CDboTSActSendSvrEvt::GetSvrEvtTriggerType( void ) const
{
	return m_byTriggerType;
}


inline unsigned int CDboTSActSendSvrEvt::GetSvrEvtID( void ) const
{
	return m_uiEvtID;
}


inline unsigned int CDboTSActSendSvrEvt::GetTblIdx( void ) const
{
	return m_uiTblIdx;
}

inline unsigned int CDboTSActSendSvrEvt::GetEvtSendType( void ) const
{
	return m_uiEvtSendType;
}

inline float CDboTSActSendSvrEvt::GetEvtSendType_Radius( void ) const
{
	return m_fEvtSendType_Radius;
}


#endif
#ifndef _DBO_TELECAST_H_
#define _DBO_TELECAST_H_


#include "DboTSCoreDefine.h"


class CDboTSActTelecast : public CNtlTSAction
{
	NTL_TS_DECLARE_RTTI


// Member variables
protected:
	eTSTELECAST_TYPE					m_eType;
	unsigned int						m_uiNPCTblIdx;
	unsigned char						m_byMessageType;
	unsigned int						m_uiSpeechTblIdx;
	unsigned int						m_uiDisplayTime;	// 밀리 세컨드


// Constructions
public:
	CDboTSActTelecast( void );


// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_ACT_TYPE_ID_ACT_TELECAST; }

	eTSTELECAST_TYPE					GetTelecastType( void ) const;
	void								SetTelecastType( eTSTELECAST_TYPE eType );

	unsigned int						GetNPCTableIndex( void ) const;
	void								SetNPCTableIndex( unsigned int uiNPCTblIdx );

	unsigned char						GetMessageType( void ) const;
	void								SetMessageType( unsigned char byMessageType );

	unsigned int						GetSpeechTableIndex( void ) const;
	void								SetSpeechTableIndex( unsigned int uiSpeechTblIdx );

	unsigned int						GetDisplayTime( void ) const;
	void								SetDisplayTime( unsigned int uiDisplayTime );


// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline eTSTELECAST_TYPE CDboTSActTelecast::GetTelecastType( void ) const
{
	return m_eType;
}


inline void CDboTSActTelecast::SetTelecastType( eTSTELECAST_TYPE eType )
{
	m_eType = eType;
}


inline unsigned int CDboTSActTelecast::GetNPCTableIndex( void ) const
{
	return m_uiNPCTblIdx;
}


inline void CDboTSActTelecast::SetNPCTableIndex( unsigned int uiNPCTblIdx )
{
	m_uiNPCTblIdx = uiNPCTblIdx;
}


inline unsigned char CDboTSActTelecast::GetMessageType( void ) const
{
	return m_byMessageType;
}


inline void CDboTSActTelecast::SetMessageType( unsigned char byMessageType )
{
	m_byMessageType = byMessageType;
}


inline unsigned int CDboTSActTelecast::GetSpeechTableIndex( void ) const
{
	return m_uiSpeechTblIdx;
}


inline void CDboTSActTelecast::SetSpeechTableIndex( unsigned int uiSpeechTblIdx )
{
	m_uiSpeechTblIdx = uiSpeechTblIdx;
}


inline unsigned int CDboTSActTelecast::GetDisplayTime( void ) const
{
	return m_uiDisplayTime;
}


inline void CDboTSActTelecast::SetDisplayTime( unsigned int uiDisplayTime )
{
	m_uiDisplayTime = uiDisplayTime;
}


#endif
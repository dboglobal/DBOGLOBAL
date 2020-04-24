#ifndef _DBO_BROADCAST_H_
#define _DBO_BROADCAST_H_


#include "DboTSCoreDefine.h"


class CDboTSActBroadCast : public CNtlTSAction
{
	NTL_TS_DECLARE_RTTI


// Member variables
protected:
	unsigned int						m_uiNPCTblIdx;
	unsigned char						m_byMessageType;
	unsigned int						m_uiSpeechTblIdx;


// Constructions
public:
	CDboTSActBroadCast( void );


// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_ACT_TYPE_ID_ACT_BROADCAST; }

	unsigned int						GetNPCTableIndex( void ) const;
	void								SetNPCTableIndex( unsigned int uiNPCTblIdx );

	unsigned char						GetMessageType( void ) const;
	void								SetMessageType( unsigned char byMessageType );

	unsigned int						GetSpeechTableIndex( void ) const;
	void								SetSpeechTableIndex( unsigned int uiSpeechTblIdx );


// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline unsigned int CDboTSActBroadCast::GetNPCTableIndex( void ) const
{
	return m_uiNPCTblIdx;
}


inline void CDboTSActBroadCast::SetNPCTableIndex( unsigned int uiNPCTblIdx )
{
	m_uiNPCTblIdx = uiNPCTblIdx;
}


inline unsigned char CDboTSActBroadCast::GetMessageType( void ) const
{
	return m_byMessageType;
}


inline void CDboTSActBroadCast::SetMessageType( unsigned char byMessageType )
{
	m_byMessageType = byMessageType;
}


inline unsigned int CDboTSActBroadCast::GetSpeechTableIndex( void ) const
{
	return m_uiSpeechTblIdx;
}


inline void CDboTSActBroadCast::SetSpeechTableIndex( unsigned int uiSpeechTblIdx )
{
	m_uiSpeechTblIdx = uiSpeechTblIdx;
}


#endif
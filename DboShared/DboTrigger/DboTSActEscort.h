#ifndef _DBO_TSACTESCORT_H_
#define _DBO_TSACTESCORT_H_


#include "DboTSCoreDefine.h"


class CDboTSActEscort : public CNtlTSAction
{
	NTL_TS_DECLARE_RTTI


// Member variables
protected:
	bool								m_bStart;
	eESCORT_TYPE						m_eEscortType;
	bool								m_bTSShare;
	unsigned int						m_uiNPCTblIdx;


// Constructions
public:
	CDboTSActEscort( void );


// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_ACT_TYPE_ID_ACT_ESCORT; }

	bool								IsEscortStart( void ) const;
	void								SetEscortStart( bool bStart );

	eESCORT_TYPE						GetEscortType( void ) const;
	void								SetEscortType( eESCORT_TYPE eEscortType );

	bool								IsTSShare( void ) const;
	void								SetTSShare( bool bTSShare );

	unsigned int						GetNPCTableIndex( void ) const;
	void								SetNPCTableIndex( unsigned int uiNPCTblIdx );


// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline bool CDboTSActEscort::IsEscortStart( void ) const
{
	return m_bStart;
}


inline void CDboTSActEscort::SetEscortStart( bool bStart )
{
	m_bStart = bStart;
}


inline eESCORT_TYPE CDboTSActEscort::GetEscortType( void ) const
{
	return m_eEscortType;
}


inline void CDboTSActEscort::SetEscortType( eESCORT_TYPE eEscortType )
{
	m_eEscortType = eEscortType;
}


inline bool CDboTSActEscort::IsTSShare( void ) const
{
	return m_bTSShare;
}


inline void CDboTSActEscort::SetTSShare( bool bTSShare )
{
	m_bTSShare = bTSShare;
}


inline unsigned int CDboTSActEscort::GetNPCTableIndex( void ) const
{
	return m_uiNPCTblIdx;
}


inline void CDboTSActEscort::SetNPCTableIndex( unsigned int uiNPCTblIdx )
{
	m_uiNPCTblIdx = uiNPCTblIdx;
}


#endif
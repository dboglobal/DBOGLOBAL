#ifndef _DBO_TSACTREGCTIMING_H_
#define _DBO_TSACTREGCTIMING_H_


#include "DboTSCoreDefine.h"


/**
	Register client timing
*/


class CDboTSActRegCTiming : public CNtlTSAction
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	bool								m_bReg;

	unsigned int						m_uiTimingFlags;	// eREG_C_TIMING
	sREG_C_TIMING_DATA_MOB_CLICK		m_sMobClick;
	sREG_C_TIMING_DATA_NPC_CLICK		m_sNPCClick;
	sREG_C_TIMING_DATA_OBJ_CLICK		m_sObjClick;
	sREG_C_TIMING_DATA_ENTER_RGN		m_sEnterRgn;
	sREG_C_TIMING_DATA_GET_QITEM		m_sGetQItem;

	eREG_C_TIMING_EXCUTE_TYPE			m_eExcuteType;
	uREG_C_TIMING_EXCUTE_DATA			m_uExcuteData;

// Constructions
public:
	CDboTSActRegCTiming( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_ACT_TYPE_ID_ACT_REG_C_TIMING; }

	bool								IsRegisteMode( void ) const;

	unsigned int						GetFlags( void ) const;
	const sREG_C_TIMING_DATA_MOB_CLICK&	GetMobClickData( void ) const;
	const sREG_C_TIMING_DATA_NPC_CLICK&	GetNPCClickData( void ) const;
	const sREG_C_TIMING_DATA_OBJ_CLICK&	GetObjClickData( void ) const;
	const sREG_C_TIMING_DATA_ENTER_RGN&	GetEnterRgnData( void ) const;
	const sREG_C_TIMING_DATA_GET_QITEM&	GetGetQItemData( void ) const;

	eREG_C_TIMING_EXCUTE_TYPE			GetExcuteType( void ) const;
	const uREG_C_TIMING_EXCUTE_DATA&	GetExcuteData( void ) const;

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};

inline bool CDboTSActRegCTiming::IsRegisteMode( void ) const
{
	return m_bReg;
}

inline unsigned int CDboTSActRegCTiming::GetFlags( void ) const
{
	return m_uiTimingFlags;
}

inline const sREG_C_TIMING_DATA_MOB_CLICK& CDboTSActRegCTiming::GetMobClickData( void ) const
{
	return m_sMobClick;
}

inline const sREG_C_TIMING_DATA_NPC_CLICK& CDboTSActRegCTiming::GetNPCClickData( void ) const
{
	return m_sNPCClick;
}

inline const sREG_C_TIMING_DATA_OBJ_CLICK& CDboTSActRegCTiming::GetObjClickData( void ) const
{
	return m_sObjClick;
}

inline const sREG_C_TIMING_DATA_ENTER_RGN& CDboTSActRegCTiming::GetEnterRgnData( void ) const
{
	return m_sEnterRgn;
}

inline const sREG_C_TIMING_DATA_GET_QITEM& CDboTSActRegCTiming::GetGetQItemData( void ) const
{
	return m_sGetQItem;
}

inline eREG_C_TIMING_EXCUTE_TYPE CDboTSActRegCTiming::GetExcuteType( void ) const
{
	return m_eExcuteType;
}

inline const uREG_C_TIMING_EXCUTE_DATA& CDboTSActRegCTiming::GetExcuteData( void ) const
{
	return m_uExcuteData;
}


#endif
#ifndef _DBO_TSACTCONCCHECK_H_
#define _DBO_TSACTCONCCHECK_H_


#include "DboTSCoreDefine.h"


class CDboTSActConcCheck : public CNtlTSAction
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:

	int									m_nConcurrencyCnt;
	unsigned int						m_uiResetTime;

	unsigned int						m_uiExcuteObjTblIdx;	// 실행시킬 오브젝트 테이블 인덱스
	NTL_TS_T_ID							m_tExcuteTID;			// 실행시킬 오브젝트 트리거 아이디

// Constructions
public:
	CDboTSActConcCheck( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_ACT_TYPE_ID_ACT_CONC_CHECK; }

	int									GetConcurrencyCnt( void ) const;
	void								SetConcurrencyCnt( int nConcurrencyCnt );

	unsigned int						GetResetTime( void ) const;
	void								SetResetTime( unsigned int uiResetTime );

	unsigned int						GetExcuteObjTblIdx( void ) const;
	void								SetExcuteObjTblIdx( unsigned int uiExcuteObjTblIdx );

	NTL_TS_T_ID							GetExcuteTID( void ) const;
	void								SetExcuteTID( NTL_TS_T_ID tExcuteTID );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline int CDboTSActConcCheck::GetConcurrencyCnt( void ) const
{
	return m_nConcurrencyCnt;
}


inline unsigned int CDboTSActConcCheck::GetResetTime( void ) const
{
	return m_uiResetTime;
}


inline unsigned int CDboTSActConcCheck::GetExcuteObjTblIdx( void ) const
{
	return m_uiExcuteObjTblIdx;
}


inline NTL_TS_T_ID CDboTSActConcCheck::GetExcuteTID( void ) const
{
	return m_tExcuteTID;
}


#endif
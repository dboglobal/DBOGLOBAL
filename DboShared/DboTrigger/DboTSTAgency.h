#ifndef _DBO_TSTAGENCY_H_
#define _DBO_TSTAGENCY_H_


#include "DboTSCoreDefine.h"


class CDboTSCompleteQInfo;
class CDboTSTRecv;
class CDboTSTCtrl;
class CDboTSMain;


/** 
	Trigger agency
*/


class CDboTSTAgency : public CNtlTSAgency
{
	NTL_TS_DECLARE_RTTI

// Declarations
public:
	typedef std::map<NTL_TS_T_ID, CDboTSTCtrl*> mapdef_TRIGGER_LIST;

// Member variables
protected:
	CDboTSMain*							m_pParent;
	CDboTSTRecv*						m_pRecv;

	mapdef_TRIGGER_LIST					m_defCurTList;		// 현재 진행 중인 트리거 정보

// Constructions and Destructions
public:
	CDboTSTAgency( void );
	virtual ~CDboTSTAgency( void );

// Methods
public:
	// 트리거 에이젼시 갱신
	virtual void						Update( void );

	// 주 트리거 시스템
	CDboTSMain*							GetParent( void );
	void								SetParent( CDboTSMain* pParent );

	// 트리거 리시버
	CDboTSTRecv*						GetRecv( void );
	void								SetRecv( CDboTSTRecv* pRecv );

	// 현재 진행 중인 트리거 리스트
	mapdef_TRIGGER_LIST*				GetCurTList( void ) { return &m_defCurTList; }

	// 진행중 트리거
	CDboTSTCtrl*						FindProgressTrigger( NTL_TS_T_ID tId );
	void								AttachProgressTrigger( NTL_TS_T_ID tId, CDboTSTCtrl* pCtrl );
	void								DetachProgressTrigger( NTL_TS_T_ID tId );

// Implementations
protected:
	virtual CDboTSTCtrl*				MakeTriggerController( CNtlTSTrigger* pTrig );
};


inline CDboTSMain* CDboTSTAgency::GetParent( void )
{
	return m_pParent;
}

inline CDboTSTRecv* CDboTSTAgency::GetRecv( void )
{
	return m_pRecv;
}


#endif
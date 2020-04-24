#ifndef _NTL_TSCONTROLLER_H_
#define _NTL_TSCONTROLLER_H_


#include "NtlTSControlObject.h"


class CNtlTSRecv;
class CNtlTSTrigger;


/**
	Controller
*/


class CNtlTSController : public CNtlTSControlObject
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	CNtlTSTrigger*							m_pTrigger;

// Constructions and Destructions
public:
	CNtlTSController( void );
	virtual ~CNtlTSController( void );

// Methods
public:
	CNtlTSTrigger*						GetTrigger( void );
	CNtlTSTrigger*						SetTrigger( CNtlTSTrigger* pTrigger );

	NTL_TSRESULT						SearchAll( CNtlTSRecv* pTSRecv, void* pParam );
	NTL_TSRESULT						SearchTarget( NTL_TS_TG_ID tgID, NTL_TS_TC_ID tcID, CNtlTSRecv* pTSRecv, void* pParam );

	NTL_TSRESULT						RunTarget( NTL_TS_TG_ID tgID, NTL_TS_TC_ID tcID, CNtlTSRecv* pTSRecv, void* pParam );
};


inline CNtlTSTrigger* CNtlTSController::GetTrigger( void )
{
	return m_pTrigger;
}


#endif
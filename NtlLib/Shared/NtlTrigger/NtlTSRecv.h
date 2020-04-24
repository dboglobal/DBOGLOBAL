#ifndef _NTL_TSRECEIVER_H_
#define _NTL_TSRECEIVER_H_


#include "NtlTSUIObject.h"


class CNtlTSEntity;


/** 
	Receiver
*/


class CNtlTSRecv : public CNtlTSUIObject
{
	NTL_TS_DECLARE_RTTI

// Constructions and Destructions
public:
	CNtlTSRecv( void );
	virtual ~CNtlTSRecv( void );

// Methods
public:
	virtual	NTL_TSRESULT				Search( CNtlTSEntity* pEntity, void* pParam );
	virtual	NTL_TSRESULT				Run( CNtlTSEntity* pEntity, void* pParam );
};


#endif
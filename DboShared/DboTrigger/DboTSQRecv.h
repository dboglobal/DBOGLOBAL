#ifndef _DBO_TSQRECV_H_
#define _DBO_TSQRECV_H_


#include "DboTSTRecv.h"


/** 
	Quest receiver
*/


struct sQRUN_PARAM : public sTRUN_PARAM
{
public:
	sQRUN_PARAM( void ) { return; }

public:
	virtual void						Init( void ) { sTRUN_PARAM::Init(); }
};


class CDboTSQRecv : public CDboTSTRecv
{
	NTL_TS_DECLARE_RTTI

// Methods
public:
	virtual	NTL_TSRESULT				Search( CNtlTSEntity* pEntity, void* pParam );
	virtual	NTL_TSRESULT				Run( CNtlTSEntity* pEntity, void* pParam );
};


#endif
/*****************************************************************************
 *
 * File			: NtlSobFactory.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 8. 23	
 * Abstract		: Simulation entity factory class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_SOB_FACTORY_H__
#define __NTL_SOB_FACTORY_H__

#include "NtlSLDef.h"

#define MARK_CLIENT_SERIAL		0x80000000

class CNtlSob;

class CNtlSobFactory
{
private:

	static SERIAL_HANDLE m_uiSerialId;

public:

	static CNtlSob* CreateSobFactroy(RwUInt32 uiClassId);
	static void DeleteSobFactory(CNtlSob *pSobObj);
	
	static SERIAL_HANDLE	AcquireSerailId(void);
	static SERIAL_HANDLE	AcquireTriggerSerialId(void);
	static RwBool			IsClientCreateSerialId(SERIAL_HANDLE hSerialId);
};

#endif

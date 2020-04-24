#ifndef _NTL_TSCONTROLOBJECT_H_
#define _NTL_TSCONTROLOBJECT_H_


#include "NtlTSRTTI.h"
#include "NtlTSCoreDefine.h"


/**
	Control object
*/


class CNtlTSControlObject
{
	NTL_TS_DECLARE_RTTI

// Constructions and Destructions
public:
	virtual ~CNtlTSControlObject( void ) { return; }
};


#endif
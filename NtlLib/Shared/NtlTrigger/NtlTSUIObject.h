#ifndef _NTL_TSUIOBJECT_H_
#define _NTL_TSUIOBJECT_H_


#include "NtlTSRTTI.h"
#include "NtlTSCoreDefine.h"


/**
	UI object
*/


class CNtlTSUIObject
{
	NTL_TS_DECLARE_RTTI

// Constructions and Destructions
public:
	virtual ~CNtlTSUIObject( void ) { return; }
};


#endif
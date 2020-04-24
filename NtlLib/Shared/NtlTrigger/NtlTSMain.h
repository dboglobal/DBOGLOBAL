#ifndef _NTL_TSMAIN_H_
#define _NTL_TSMAIN_H_


#include "NtlTSUIObject.h"


class CNtlTSTrigger;


/** 
	Trigger system main
*/


class CNtlTSMain : public CNtlTSUIObject
{
	NTL_TS_DECLARE_RTTI

// Declarations
public:
	typedef std::map<NTL_TS_T_ID, CNtlTSTrigger*> mapdef_TLIST;
	typedef mapdef_TLIST::iterator mapdef_TLIST_IT;
	typedef mapdef_TLIST::const_iterator mapdef_TLISTC_CIT;

// Constructions and Destructions
public:
	CNtlTSMain( void );
	virtual ~CNtlTSMain( void );
};


#endif
/*****************************************************************************
 *
 * File			: NtlSobBuffIcon.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2006. 3. 14	
 * Abstract		: Simulation buff icon object
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_SOB_BUFF_ICON_H__
#define __NTL_SOB_BUFF_ICON_H__

#include "NtlSobIcon.h"

class CNtlSobBuff;

class CNtlSobBuffIcon : public CNtlSobIcon
{
	DECLEAR_MEMORY_POOL(CNtlSobBuffIcon, NTL_DEFAULT_MEMORY_POOL)
	
public:

	CNtlSobBuffIcon();
	virtual ~CNtlSobBuffIcon();

	virtual void HandleEvents(RWS::CMsg &pMsg);

	virtual RwBool IsDragAndDropPossible(void);
	virtual RwBool IsUsePossible(void);	
};

#endif

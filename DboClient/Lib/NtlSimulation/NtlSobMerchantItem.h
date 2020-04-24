/*****************************************************************************
*
* File			: NtlSobMerchantItem.h
* Author		: Hong SungBock
* Copyright		: (аж)NTL
* Date			: 2006. 8. 1
* Abstract		: Simulation Merchant item object
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_SOB_MERCHANT_ITEM_H__
#define __NTL_SOB_MERCHANT_ITEM_H__

#include "NtlSob.h"
#include "NtlDebug.h"

class CNtlSobMerchantItem : public CNtlSob
{
	//DECLEAR_MEMORY_POOL(CNtlSobMerchantItem, NTL_DEFAULT_MEMORY_POOL)

protected:


public:
	CNtlSobMerchantItem(){}
	virtual ~CNtlSobMerchantItem(){}

	virtual void HandleEvents(RWS::CMsg &pMsg){}
};

#endif

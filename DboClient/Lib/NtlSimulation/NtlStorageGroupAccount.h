/*****************************************************************************
* File			: NtlStorageGroupAccount.h
* Author		: HaeSung, Cho
* Copyright		: (¡÷)NTL
* Date			: 2009. 2. 5	
* Abstract		: Account¿« Storage group
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_STORAGE_GROUP_ACCOUNT_H__
#define __NTL_STORAGE_GROUP_ACCOUNT_H__

#pragma once
#include "NtlStorageGroup.h"

class CNtlStorageGroupAccount : public CNtlStorageGroup
{
public:
	CNtlStorageGroupAccount(void);
	virtual ~CNtlStorageGroupAccount(void);

	virtual bool Create();
};

#endif//__NTL_STORAGE_GROUP_ACCOUNT_H__
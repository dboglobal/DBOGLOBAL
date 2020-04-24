/*****************************************************************************
 *
 * File			: NtlSobProjectileAttr.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 11. 30	
 * Abstract		: Simulation object projectile attribute base class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_SOBPROJECTILE_ATTR_H__
#define __NTL_SOBPROJECTILE_ATTR_H__

#include "NtlSobAttr.h"

class CNtlSobProjectileAttr : public CNtlSobAttr
{
	DECLEAR_MEMORY_POOL(CNtlSobProjectileAttr, NTL_DEFAULT_MEMORY_POOL)
				
public:

	CNtlSobProjectileAttr();
	virtual ~CNtlSobProjectileAttr() {}

	virtual RwBool Create(void) { SetClassID(SLCLASS_PROJECTILE); return TRUE; }

	virtual void Destroy(void) {}

public:
	
};

#endif

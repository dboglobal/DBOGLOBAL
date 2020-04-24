/*****************************************************************************
 *
 * File			: NtlSobPet.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2006. 11. 23	
 * Abstract		: Simulation pet base class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_SOB_PET_H__
#define __NTL_SOB_PET_H__

#include "NtlSLServerDeclear.h"
#include "NtlSobFeelingActor.h"
#include "NtlSkillContainer.h"

class CNtlBeCharData;

class CNtlSobPet : public CNtlSobFeelingActor
{
	DECLEAR_MEMORY_POOL(CNtlSobPet, NTL_DEFAULT_MEMORY_POOL)
		
protected:

	CNtlBeCharData			*m_pBeData;				/** character behavior data */

	CNtlPetSkillContainer	m_SkillContainer;

public:

	CNtlSobPet();
	virtual ~CNtlSobPet();

	virtual RwBool			Create(void);
	virtual void			Destroy(void);
	virtual void			Update(RwReal fElapsed);
	virtual CNtlFSMLayer*	CreateFSM(void);
	virtual void			HandleEvents(RWS::CMsg &pMsg);

public:

	// control data station
	virtual void*			GetBehaviorData(void);

	CNtlPetSkillContainer*	GetSkillContainer(void);
};

inline void* CNtlSobPet::GetBehaviorData(void)
{
	return reinterpret_cast<void*>(m_pBeData); 
}

inline CNtlPetSkillContainer* CNtlSobPet::GetSkillContainer(void)
{
	return &m_SkillContainer;
}


#endif
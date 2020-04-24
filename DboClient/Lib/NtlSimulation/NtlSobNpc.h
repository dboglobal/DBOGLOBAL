/*****************************************************************************
 *
 * File			: NtlSobNpc.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 11. 16	
 * Abstract		: Simulation npc object
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_SOB_NPC_H__
#define __NTL_SOB_NPC_H__

#include "NtlSLServerDeclear.h"
#include "NtlSobFeelingActor.h"

class CNtlBeCharData;

class CNtlSobNpc : public CNtlSobFeelingActor
{
	DECLEAR_MEMORY_POOL(CNtlSobNpc, NTL_DEFAULT_MEMORY_POOL)
		
protected:

	CNtlBeCharData	*m_pBeData;				/** character behavior data */

	RwReal			m_fTime;
	RwUInt8			m_byQuestMark;
	RwBool			m_bActiveQuestMark;
	RwUInt16		m_tID;

protected:

	void	QeustMarkCheck(RwReal fElapsed);

public:

	CNtlSobNpc();
	virtual ~CNtlSobNpc();

	virtual RwBool			Create(void);

	virtual void			Destroy(void);

	virtual void			Update(RwReal fElapsed);

	virtual CNtlFSMLayer*	CreateFSM(void);

	virtual void			HandleEvents(RWS::CMsg &pMsg);

public:

	// control data station
	virtual void*			GetBehaviorData(void);
	
public:

	void					ActiveQuestMark(RwBool bQuestMark);

	RwUInt8					GetQuestMark(void);
};

inline void* CNtlSobNpc::GetBehaviorData(void)
{
	return reinterpret_cast<void*>(m_pBeData); 
}

inline RwUInt8 CNtlSobNpc::GetQuestMark(void)
{
	return m_byQuestMark;
}

#endif

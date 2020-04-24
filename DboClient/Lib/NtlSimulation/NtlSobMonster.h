/*****************************************************************************
 *
 * File			: NtlSobMonster.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 10. 31	
 * Abstract		: Simulation monster object
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_SOB_MONSTER_H__
#define __NTL_SOB_MONSTER_H__

#include "NtlSobFeelingActor.h"
#include "NtlBuffContainer.h"

class CNtlBeCharData;

class CNtlSobMonster : public CNtlSobFeelingActor
{
	DECLEAR_MEMORY_POOL(CNtlSobMonster, NTL_DEFAULT_MEMORY_POOL)
	
public:
	CNtlSobMonster();
	virtual ~CNtlSobMonster();

	virtual RwBool			Create(void);
	virtual void			Destroy(void);
	virtual void			Update(RwReal fElapsed);
	virtual CNtlFSMLayer*	CreateFSM(void);
	virtual void			HandleEvents(RWS::CMsg &pMsg);

	// control data station
	virtual void*			GetBehaviorData(void);

protected:
    // 이벤트 처리
    void    OnEventBotCautionNfy_Handler(RWS::CMsg& pMsg);          ///< 선공몹이 캐릭터를 발견했을때
    void    OnEventBotHelpMeNfy_Handler(RWS::CMsg& pMsg);           ///< 몹이 주위몹에게 도움을 요청할때

protected:
    CNtlBeCharData*         m_pBeData;				/** character behavior data */
};


inline void* CNtlSobMonster::GetBehaviorData(void)
{
	return reinterpret_cast<void*>(m_pBeData); 
}


#endif

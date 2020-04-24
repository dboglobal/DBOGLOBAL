#pragma once

#include "ceventhandler.h"
#include "NtlSLDef.h"
#include "NtlWorldConceptDefine.h"
/**
 * \ingroup NtlSimulation
 * \brief 각 World Concept들의 상위 클래스
 * \date 2007-06-14
 * \author agebreak
 */

class CNtlSobActor;

class CNtlWorldConceptController
{
public:

	CNtlWorldConceptController(void);
	virtual ~CNtlWorldConceptController(void);

	virtual void		Update(RwReal fElapsedTime);				///< 각 컨셉에 맞는 내용들의 Update처리 함수
	virtual RwBool		IsEnableAction(EAvatarAction eAction);		///< Avatar의 Action이 현재 처리 가능한지를 각 컨셉에 따라 판별한다.
	virtual void		ChangeState(RwInt32 iState);				///< State 상태를 변경한다.	
	virtual RwInt32		GetState() {return m_iWorldState;}			///< 현재 State 상태를 반환한다.
	
	virtual void		BeforeLoadingDirect(void)	{}
	virtual void		AfterLoadingDirect(void)	{}

	virtual RwBool		IsBeforeLoadingDirect(void)	{ return TRUE; }
	virtual RwBool		IsAfterLoadingDirect(void)	{ return TRUE; }

	virtual RwBool		IsMoveableArea(CNtlSobActor *pActor, const RwV3d *pPos, const RwV3d *pDestPos)	{ return TRUE; }
	virtual RwBool		IsVisiblePCInCinematic(VOID) { return FALSE; }

	virtual RwBool		CanUseSkill(RwUInt32 hSerialID, RwUInt32& uiResultCode) { return TRUE; }			

	void				SetConceptType(EWorldPlayConcept eType);
	EWorldPlayConcept	GetConceptType(void);

	void				Pause(RwBool bPause);
	RwBool				IsPause(void)				{ return m_bPause; }

	virtual void		HandleEvents(RWS::CMsg &pMsg) { return; }


protected:

	RwReal				m_fStateTime;									///< 각 State에서의 경과된 시간
	RwInt32				m_iWorldState;									///< 현재의 State 상태		
	EWorldPlayConcept	m_eType;
	RwBool				m_bPause;
};

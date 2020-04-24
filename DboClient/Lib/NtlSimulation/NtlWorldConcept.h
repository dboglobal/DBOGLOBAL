/*****************************************************************************
 *
 * File			: NtlWorldConcept.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 12. 04	
 * Abstract		: Simulation world concept class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_WORLDCONCEPT_H__
#define __NTL_WORLDCONCEPT_H__

#include "ceventhandler.h"
#include "NtlWorldConceptDefine.h"
#include "NtlWorldConceptController.h"

class CNtlSob;
class CNtlSobActor;

/**
 * \ingroup NtlSimulation
 * \brief 게임내의 여러 시스템에 관련된 World Concept를 정의하는 클래스 
 * (한번에 활성화되는 World Concept은 2개이상이 될 수도 있다)
 * \date 2007-06-14
 * \author agebreak
 */
class CNtlWorldConcept : public RWS::CEventHandler
{
public:

	CNtlWorldConcept();
	~CNtlWorldConcept();

	static CNtlWorldConcept* GetInstance(void);

	RwBool						Create(void);
	void						Destroy(void);

	void						Reset(void);

	void						Update(RwReal fElapsed);

	void						AddWorldPlayConcept(EWorldPlayConcept ePlayConcept);		///< 활성화된 WorldConcept 리스트에 새로운 World Concept를 추가한다.
	void						RemoveWorldPlayConcept(EWorldPlayConcept ePlayConcept);		///< 활성화된 WorldConcept 리스트에서 World Concept을 제거한다.

	void						DeleteGradeWorldPlayConcept(RwUInt32 uiGrade);
		
	RwBool						IsActivePlayConcept(EWorldPlayConcept ePlayConcept);		///< 현재 WorldConcept이 활성화 되어 있는지를 반환한다.	
	RwBool						IsActivePlayGrade(RwUInt32 uiGrade);						///< 현재 Grade가 활성화 되어있는가.
	CNtlWorldConceptController* GetWorldConceptController(EWorldPlayConcept eWorldConcept);	///< World Concept에 해당하는 컨트롤러 객체를 반환한다.
	CNtlWorldConceptController* FindGradeExtController(RwUInt32 uiGrade);		
	
	void						BeforeLoadingDirect(void);
	void						AfterLoadingDirect(void);

	RwBool						IsBeforeLoadingDirect(void);
	RwBool						IsAfterLoadingDirect(void);
	RwBool						IsGrade(RwUInt32 uiGrade, EWorldPlayConcept ePlayType);

	RwBool						HasTutorialWorldConcept( void );
	void						SetHaveTutorialWorldConcept( RwBool bHave );

	//---------------- Methods for status checking ---------------//
	void						EnableAction(RwBool	bEnable);
	RwBool						IsEnableAction(EAvatarAction eAction, BOOL bUnknown = FALSE);						///< Check whether the current action can be performed.

	//----------------- event handler ------------------------//
	void						HandleEvents(RWS::CMsg &pMsg);


	//----------------- world conpect rule ------------------------//

	RwBool						IsMoveableArea(CNtlSobActor *pActor, const RwV3d *pPos, const RwV3d *pDestPos);
	RwBool						IsVisiblePCInCinematic(VOID);

	RwBool						CanUseSkill(RwUInt32 hSerialID, RwUInt32& uiResultCode);

	RwBool						IsEnemyTargetFromAvatarActor(CNtlSobActor *pActor, SERIAL_HANDLE hTargetSerial);	///< 
	RwBool						IsEnemyTargetFromAvatarActor(CNtlSobActor* pActor, CNtlSob* pSobObj);
	CNtlSobActor*				FindNearEnemyFromAvatarActor(CNtlSobActor *pActor);									///< 

protected:

	typedef std::list<CNtlWorldConceptController*>	ListController;

	static CNtlWorldConcept		*m_pInstance;
	ListController				m_listController;

	RwBool						m_bActionEnable;
    RwBool                      m_bIsNightOn;                           ///< 현재 Night가 On인지 Off인지 판별하는 플래그

	RwBool						m_bHasTutorialWorldConcept;
};


/// World Concept 클래스의 싱글톤 객체를 반환한다.
static CNtlWorldConcept* GetNtlWorldConcept(void)
{
	return CNtlWorldConcept::GetInstance();
}

#endif
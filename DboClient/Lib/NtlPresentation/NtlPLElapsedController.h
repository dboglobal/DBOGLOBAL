/*****************************************************************************
*
* File			: NtlPLElapsedController.h
* Author		: Peessi
* Copyright		: (주)NTL
* Date			: 2006. 8. 17	
* Abstract		: Presentation entity elapsed controller
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_PLELAPSED_CONTROLLER_H__
#define __NTL_PLELAPSED_CONTROLLER_H__

class CNtlPLEntity;

class CNtlElapsedController
{
public:

	CNtlPLEntity *m_pPLEntity;	/** control entity */
	RwReal m_fCurrTime;				/** 현재 흐른 시간 */ 
	RwReal m_fLifeTime;				/** control 시간 */
	RwReal m_fWeightValue;			/** elapsed weight value */
	
	RwBool m_bFinish;

public:

	CNtlElapsedController();
	~CNtlElapsedController();

	void Enter(CNtlPLEntity *pPLEntity, RwReal fLifeTime, RwReal fWeightValue);
	void Update(RwReal fElapsed);
	void Exit(void);

	RwBool IsFinish(void);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlElapsedControllerGroup
{
#define MAX_ELAP_CONTROLLER		3

private:

	CNtlPLEntity *m_pPLEntity;
	CNtlElapsedController *m_pController[MAX_ELAP_CONTROLLER];
	RwInt8 m_chRef;
	RwBool m_bFinish;

private:

	RwInt32 DeleteLongTimeController(void);
		
public:

	CNtlElapsedControllerGroup();
	~CNtlElapsedControllerGroup();

	void CreateController(RwReal fLifeTime, RwReal fWeightValue);

	void Update(RwReal fElapsed);

	void SetControlEntity(CNtlPLEntity *pPLEntity);

	RwBool IsFinish(void);
};

#endif

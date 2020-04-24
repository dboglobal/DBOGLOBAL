/*****************************************************************************
 *
 * File			: NtlSobElapsedController.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2006. 8. 23	
 * Abstract		: simulation object elapsed controller
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_SOBELAPSED_CONTROLLER_H__
#define __NTL_SOBELAPSED_CONTROLLER_H__

#include "NtlSLDef.h"

class CNtlSobElapsedController
{
	DECLEAR_MEMORY_POOL(CNtlSobElapsedController, 10)

public:

	SERIAL_HANDLE m_hSerialId;		/** control simulation object serial */
	RwReal m_fCurrTime;				/** 현재 흐른 시간 */ 
	RwReal m_fLifeTime;				/** control 시간 */
	RwReal m_fWeightValue;			/** elapsed weight value */
	
	RwBool m_bFinish;

public:

	CNtlSobElapsedController();
	~CNtlSobElapsedController();

	void Enter(SERIAL_HANDLE hSerialId, RwReal fLifeTime, RwReal fWeightValue);
	void Update(RwReal fElapsed);
	void Exit(void);

	RwBool IsFinish(void);

	SERIAL_HANDLE GetSeiralID(void) const;
};

inline SERIAL_HANDLE CNtlSobElapsedController::GetSeiralID(void) const
{
	return m_hSerialId;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlSob;

class CNtlSobElapsedControlManager
{
private:

	struct SSobElapsedDelay
	{
		CNtlSob						*pSobObj;
		RwReal						fLifeTime;
		RwReal						fWeightValue;
		RwReal						fDelayTime;
		RwReal						fCurrTime;
		CNtlSobElapsedController	*pController;
	};

	typedef std::multimap<SERIAL_HANDLE, CNtlSobElapsedController*> MultiMapController;
	typedef std::list<SSobElapsedDelay> ListReserved;

	MultiMapController	m_mmapController;
	ListReserved		m_listReserved;

private:

	void AddController(CNtlSobElapsedController *pController, CNtlSob *pSobObj, RwReal fLifeTime, RwReal fWeightValue);

	void UpdateReserved(RwReal fElapsed);
	void UpdateController(RwReal fElapsed);

public:

	CNtlSobElapsedControlManager();
	~CNtlSobElapsedControlManager();

	static CNtlSobElapsedControlManager* GetInstance(void);

	void Reset(void);

	CNtlSobElapsedController* CreateController(CNtlSob *pSobObj, RwReal fLifeTime, RwReal fWeightValue, RwReal fDelayTime = 0.0f);
	void CreateController(CNtlSob *pSobObj, CNtlSob *pReferSobObj);
	void DeleteController(CNtlSobElapsedController *pController);

	void Update(RwReal fElapsed);

	RwBool GetControlFactor(CNtlSob *pSobObj, RwReal& fLifeTime, RwReal& fWeightValue);
};

static CNtlSobElapsedControlManager* GetNtlSobElapsedControlManager(void)
{
	return CNtlSobElapsedControlManager::GetInstance();
}

#endif
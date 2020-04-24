#include "precomp_ntlsimulation.h"
#include "NtlSobElapsedController.h"

// core
#include "NtlDebug.h"

// simulation
#include "NtlSob.h"
#include "NtlSobManager.h"
#include "NtlSobProxy.h"

DEFINITION_MEMORY_POOL(CNtlSobElapsedController)

CNtlSobElapsedController::CNtlSobElapsedController()
{
	m_hSerialId = INVALID_SERIAL_ID;
	m_fCurrTime = 0.0f;	
	m_fLifeTime = 0.0f;	
	m_fWeightValue = 1.0f;		
	
	m_bFinish = FALSE;

}

CNtlSobElapsedController::~CNtlSobElapsedController()
{
}

void CNtlSobElapsedController::Enter(SERIAL_HANDLE hSerialId, RwReal fLifeTime, RwReal fWeightValue)
{
	m_hSerialId		= hSerialId;
	m_fLifeTime		= fLifeTime;
	m_fWeightValue	= fWeightValue;

	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(hSerialId);
	if(pSobObj)
	{
		RwReal fSobWeightValue = pSobObj->GetWeightElapsedTime();
		pSobObj->SetWeightElapsedTime(fSobWeightValue*fWeightValue);
	}
}

void CNtlSobElapsedController::Update(RwReal fElapsed)
{
	m_fCurrTime += fElapsed;

	if(m_fCurrTime >= m_fLifeTime)
		m_bFinish = TRUE;
}

void CNtlSobElapsedController::Exit(void)
{
	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(m_hSerialId);
	RwReal fSobWeightValue = pSobObj->GetWeightElapsedTime();
	pSobObj->SetWeightElapsedTime(fSobWeightValue/m_fWeightValue);
}

RwBool CNtlSobElapsedController::IsFinish(void)
{
	return m_bFinish;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

CNtlSobElapsedControlManager::CNtlSobElapsedControlManager()
{
}

CNtlSobElapsedControlManager::~CNtlSobElapsedControlManager()
{
	NTL_ASSERT(m_mmapController.size() <= 0, "CNtlSobElapsedControlManager::~CNtlSobElapsedControlManager");
}

CNtlSobElapsedControlManager* CNtlSobElapsedControlManager::GetInstance(void)
{
	static CNtlSobElapsedControlManager Instance;
	return &Instance;
}


void CNtlSobElapsedControlManager::Reset(void)
{
	ListReserved::iterator reserit;
	for(reserit = m_listReserved.begin(); reserit != m_listReserved.end(); reserit++)
	{
		// 아래의 delete pController를 NTL_DELETE로 바꾸지 말아 주세요. Memory Pool을 사용합니다.
		// by Jang HyungSuk

		delete (*reserit).pController;
	}

	m_listReserved.clear();


	MultiMapController::iterator it;
    while(!m_mmapController.empty())
    {
        it = m_mmapController.begin();
        DeleteController((*it).second);		
    }

	m_mmapController.clear();
}

void CNtlSobElapsedControlManager::AddController(CNtlSobElapsedController *pController, CNtlSob *pSobObj, RwReal fLifeTime, RwReal fWeightValue)
{
	SERIAL_HANDLE hSerialId = pSobObj->GetSerialID();

	MultiMapController::iterator it = m_mmapController.find(hSerialId);
	if(it == m_mmapController.end())
		pSobObj->EnableWeightElapsedTime(TRUE);

	pController->Enter(hSerialId, fLifeTime, fWeightValue);

	m_mmapController.insert(MultiMapController::value_type(hSerialId, pController));

	pSobObj->GetSobProxy()->CreateElapsedController(fLifeTime, fWeightValue);
}

CNtlSobElapsedController* CNtlSobElapsedControlManager::CreateController(CNtlSob *pSobObj, RwReal fLifeTime, RwReal fWeightValue, RwReal fDelayTime)
{
	// 아래의 new CNtlSobElapsedController 에서 new를 NTL_NEW로 바꾸지 말아 주세요. memory pool 사용합니다.
	// by Jang HyungSuk

	CNtlSobElapsedController *pController = new CNtlSobElapsedController;

	if(fDelayTime >= 0.000001f)
	{
		SSobElapsedDelay sElapsedDelay;
		sElapsedDelay.pSobObj		= pSobObj;
		sElapsedDelay.fLifeTime		= fLifeTime;
		sElapsedDelay.fWeightValue	= fWeightValue;
		sElapsedDelay.fDelayTime	= fDelayTime;
		sElapsedDelay.fCurrTime	= 0.0f;
		sElapsedDelay.pController	= pController;

		m_listReserved.push_back(sElapsedDelay);

		return pController;
	}

	AddController(pController, pSobObj, fLifeTime, fWeightValue);
	
	return pController;
}

void CNtlSobElapsedControlManager::CreateController(CNtlSob *pSobObj, CNtlSob *pReferSobObj)
{
	MultiMapController::iterator referit = m_mmapController.find(pReferSobObj->GetSerialID());
	if(referit == m_mmapController.end())
		return;

	SERIAL_HANDLE hSerialId = pSobObj->GetSerialID();

	MultiMapController::iterator it = m_mmapController.find(hSerialId);
	if(it == m_mmapController.end())
		pSobObj->EnableWeightElapsedTime(TRUE);

	RwReal fLifeTime, fWeightValue;
	RwReal fSumLifeTime = 0.0f, fSumWeightValue = 0.0f;
	RwInt32 iCount = (RwInt32)m_mmapController.count(pReferSobObj->GetSerialID()); 
	for(RwInt32 i = 0; i < iCount; ++i, ++referit)
	{
		CNtlSobElapsedController *pController = (*referit).second;
		
		// 아래의 new CNtlSobElapsedController 에서 new를 NTL_NEW로 바꾸지 말아 주세요. memory pool 사용합니다.
		// by Jang HyungSuk

		CNtlSobElapsedController *pCopyController = new CNtlSobElapsedController;
		fLifeTime = pController->m_fLifeTime - pController->m_fCurrTime;
		if(fLifeTime <= 0.0f)
			fLifeTime = 0.0f;

		pCopyController->Enter(hSerialId, fLifeTime, pController->m_fWeightValue);

		m_mmapController.insert(MultiMapController::value_type(hSerialId, pCopyController));

		fSumLifeTime += fLifeTime;
		fSumWeightValue += pController->m_fWeightValue;
	}

	fLifeTime = fSumLifeTime/(RwReal)iCount;
	fWeightValue = fSumWeightValue/(RwReal)iCount;

	pSobObj->GetSobProxy()->CreateElapsedController(fLifeTime, fWeightValue);
}


void CNtlSobElapsedControlManager::DeleteController(CNtlSobElapsedController *pController)
{
	//-----------------------------------------------------------------------
	// reserved find
	ListReserved::iterator reserit;
	for(reserit = m_listReserved.begin(); reserit != m_listReserved.end(); reserit++)
	{
		if((*reserit).pController == pController)
		{
			// 아래의 delete pController를 NTL_DELETE로 바꾸지 말아 주세요. Memory Pool을 사용합니다.
			// by Jang HyungSuk
			delete pController;
			m_listReserved.erase(reserit);
			return;
		}
	}

	//-----------------------------------------------------------------------
	// controller
	SERIAL_HANDLE hSerialId = pController->GetSeiralID();

	MultiMapController::iterator it = m_mmapController.find(hSerialId);
	if(it == m_mmapController.end())
		return;

	pController->Exit();

	// 아래의 delete pController를 NTL_DELETE로 바꾸지 말아 주세요. Memory Pool을 사용합니다.
	// by Jang HyungSuk

	delete pController;

	RwInt32 iCount = (RwInt32)m_mmapController.count(hSerialId); 
	for(RwInt32 i = 0; i < iCount; i++)
	{
		if((*it).second == pController)
		{
			it = m_mmapController.erase(it); 
			break;
		}
		else
			++it;
	}

	it = m_mmapController.find(hSerialId);
	if(it == m_mmapController.end())
	{
		CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(hSerialId);
		pSobObj->EnableWeightElapsedTime(FALSE);
		pSobObj->SetWeightElapsedTime(1.0f);
		pSobObj->GetSobProxy()->DeleteElapsedController();
	}
}

void CNtlSobElapsedControlManager::UpdateReserved(RwReal fElapsed)
{
	ListReserved::iterator it;
	for(it = m_listReserved.begin(); it != m_listReserved.end(); )
	{
		(*it).fCurrTime += fElapsed;
		if((*it).fCurrTime >= (*it).fDelayTime)
		{
			AddController( (*it).pController, (*it).pSobObj, (*it).fLifeTime, (*it).fWeightValue );
			it = m_listReserved.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void CNtlSobElapsedControlManager::UpdateController(RwReal fElapsed)
{
	CNtlSobElapsedController *pController;
	CNtlSob *pSobObj;

	MultiMapController::iterator it, currit;
	for(it = m_mmapController.begin(); it != m_mmapController.end(); )
	{
		pController = (*it).second;
		pSobObj = GetNtlSobManager()->GetSobObject(pController->GetSeiralID());
		if(pSobObj == NULL)
		{
			// 아래의 delete pController를 NTL_DELETE로 바꾸지 말아 주세요. Memory Pool을 사용합니다.
			// by Jang HyungSuk

			delete pController;

			it = m_mmapController.erase(it);
		}
		else
		{
			pController->Update(fElapsed*pSobObj->GetWeightElapsedTime());
			if(pController->IsFinish())
			{
				pController->Exit();

				// 아래의 delete pController를 NTL_DELETE로 바꾸지 말아 주세요. Memory Pool을 사용합니다.
				// by Jang HyungSuk

				delete pController;

				it = m_mmapController.erase(it);

				currit = m_mmapController.find(pSobObj->GetSerialID());
				if(currit == m_mmapController.end())
				{
					pSobObj->EnableWeightElapsedTime(FALSE);
					pSobObj->SetWeightElapsedTime(1.0f);
					pSobObj->GetSobProxy()->DeleteElapsedController();
				}
			}
			else
				++it;
		}
	}
}

void CNtlSobElapsedControlManager::Update(RwReal fElapsed)
{
	UpdateReserved(fElapsed);
	UpdateController(fElapsed);
}


RwBool CNtlSobElapsedControlManager::GetControlFactor(CNtlSob *pSobObj, RwReal& fLifeTime, RwReal& fWeightValue)
{
	MultiMapController::iterator referit = m_mmapController.find(pSobObj->GetSerialID());
	if(referit == m_mmapController.end())
		return FALSE;

	RwReal fSumLifeTime = 0.0f, fSumWeightValue = 0.0f;

	RwInt32 iCount = (RwInt32)m_mmapController.count(pSobObj->GetSerialID()); 

	for(RwInt32 i = 0; i < iCount; ++i, ++referit)
	{
		CNtlSobElapsedController *pController = (*referit).second;
		
		fLifeTime = pController->m_fLifeTime - pController->m_fCurrTime;
		if(fLifeTime <= 0.0f)
			fLifeTime = 0.0f;

		fSumLifeTime += fLifeTime;
		fSumWeightValue += pController->m_fWeightValue;
	}

	fLifeTime = fSumLifeTime/(RwReal)iCount;
	fWeightValue = fSumWeightValue/(RwReal)iCount;

	return TRUE;
}
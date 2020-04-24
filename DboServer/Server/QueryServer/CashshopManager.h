#ifndef __CASHSHOP_MANAGER_H__
#define __CASHSHOP_MANAGER_H__

#include "NtlSingleton.h"
#include "HLSItemTable.h"

class CCashshopManager : public CNtlSingleton<CCashshopManager>
{

public:

	CCashshopManager();
	virtual ~CCashshopManager();


private:

	void						Init();

public:

	QWORD						AcquireProductId();

private:

	QWORD						m_qwLastProductId;

};

#define GetCashshopManager()	CCashshopManager::GetInstance()
#define g_pCashshopManager		GetCashshopManager()

#endif
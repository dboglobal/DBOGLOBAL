#ifndef __SERVERSCRIPT_DYNAMIC_FIELD_EVENT_SYSTEM__
#define __SERVERSCRIPT_DYNAMIC_FIELD_EVENT_SYSTEM__

#include "NtlSingleton.h"

class CDynamicFieldSystem : public CNtlSingleton<CDynamicFieldSystem>
{

public:

	CDynamicFieldSystem();
	virtual ~CDynamicFieldSystem();

private:

	void				Init();

	void				Destroy();


public:

	void				SetCurCount(DWORD dwCount) { m_dwCurCount = dwCount; }
	DWORD				GetCurCount() { return m_dwCurCount; }
	void				SetMaxCount(DWORD dwCount) { m_dwMaxCount = dwCount; }
	DWORD				GetMaxCount() { return m_dwMaxCount; }


private:

	DWORD				m_dwCurCount;
	DWORD				m_dwMaxCount;

};

#define GetDynamicFieldSystem()			CDynamicFieldSystem::GetInstance()
#define g_pDynamicFieldSystem			GetDynamicFieldSystem()

#endif
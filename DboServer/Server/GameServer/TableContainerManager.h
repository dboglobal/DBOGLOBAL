#ifndef __SERVER_tablecontainer_MANAGER__
#define __SERVER_tablecontainer_MANAGER__

#include "NtlSingleton.h"
#include "TableContainer.h"


class CTableContainerManager : public CNtlSingleton<CTableContainerManager>, public CTableContainer
{

public:

	CTableContainerManager();
	virtual ~CTableContainerManager();

public:

	bool			CreateTableContainer(BYTE LoadTableFormat, char* pszPath);

};

#define GetCTableContainerManager()		CTableContainerManager::GetInstance()
#define g_pTableContainer				GetCTableContainerManager()

#endif
#ifndef __SERVER_tablecontainer_MANAGER__
#define __SERVER_tablecontainer_MANAGER__


#include "NtlSingleton.h"
#include "TableContainer.h"


class CTableContainerManager : public CTableContainer, public CNtlSingleton<CTableContainerManager>
{

public:

	CTableContainerManager();
	virtual ~CTableContainerManager();

public:

	bool			CreateTableContainer(BYTE LoadTableFormat, char* pszPath);
	bool			LoadLocalizedTable();



};

#define GetTableContainerManager()		CTableContainerManager::GetInstance()
#define g_pTableContainer				GetTableContainerManager()

#endif
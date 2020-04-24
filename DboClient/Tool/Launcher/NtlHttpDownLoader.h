/*****************************************************************************
 *
 * File			: NtlHttpDownLoader.h
 * Author		: HongHoDong
 * Copyright	: (주)NTL
 * Date			: 2007. 2. 5.
 * Abstract		: 
 *****************************************************************************
 * Desc         : Patch Server의 File을 Http Protocol으로 다운 받는다.
 *
 *****************************************************************************/
#pragma once
#include "NtlDownLoader.h"
#include "NtlEventDownLoader.h"

class CNtlHttpDownLoader : public CNtlDownLoader
{
protected:
	CNtlHttpDownLoader();
	CNtlHttpDownLoader(const CNtlHttpDownLoader& rhs) {}

public:
	virtual ~CNtlHttpDownLoader();
	
	BOOL RequestFile( const char *pszServerName,
                      const char *pszServerPath,
					  const char *pszServerFileName,
					  const char *pszClientPath,
					  const char *pszClientFileName);

	void DestroyInstance() 
	{ 
		CNtlDownLoader::DestroyInstance();
	}

	static CNtlDownLoader *GetInstance()
	{ 
		if(s_pInstance == NULL)
			s_pInstance = new CNtlHttpDownLoader;
		ASSERT(s_pInstance != NULL);
		return s_pInstance;
	}
};

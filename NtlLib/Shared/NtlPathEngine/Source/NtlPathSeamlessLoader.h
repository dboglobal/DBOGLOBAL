/*****************************************************************************
* File			: NtlPathSeamlessLoader.h
* Author		: Haesung, Cho
* Copyright		: (аж)NTL
* Date			: 2009. 03. 19
* Abstract		: Pathengine seamless loader
*****************************************************************************
* Desc:
*
*****************************************************************************/

#ifndef _NTL_PATH_SEAMLESS_LOADER_H_
#define _NTL_PATH_SEAMLESS_LOADER_H_


class CNtlPathSeamlessLoadingManager;

class CNtlPathSeamlessLoader
{
public:
	CNtlPathSeamlessLoader();
	~CNtlPathSeamlessLoader();

	bool	Create( void );
	
	static unsigned int __stdcall	ThreadFunc( void* pParam );
	
	void	Destroy();
protected:
	CNtlPathSeamlessLoadingManager* m_pManager;

};

#endif
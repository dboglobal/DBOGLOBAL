////////////////////////////////////////////////////////////////////////////////
// File: NtlFlasherManager.h
// Desc: GFx Scaleform Integration Manager.
//
// 2006.07.13 Peessi@hitel.net   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __NTL_FLASHER_MANAGER_H__
#define __NTL_FLASHER_MANAGER_H__

// GFx Includes
#include "GFile.h"
#include "GFxPlayer.h"
#include "GFxLoader.h"
#include "GRendererD3D9.h"
#include "GFxFontLib.h"

#include "NtlDeviceRestoreHandler.h"


typedef void(*CallFlashResourcePack)(const char* pFileName, void** pData, int* iSize);
extern CallFlashResourcePack g_fnCallFlashPack;

void LinkFlashResourceLoad(CallFlashResourcePack fn);
void UnLinkFlashResourceLoad(void);


class FlasherFileOpener : public GFxFileOpener
{
public:
	virtual GFile* OpenFile(const char* pFilename, int flags = GFileConstants::Open_Read | GFileConstants::Open_Buffered,
		int mode = GFileConstants::Mode_ReadWrite);
};

class MySysAlloc : public GSysAllocPaged
{
public:
	virtual void GetInfo(Info* i) const
	{
		i->MinAlign = 1;
		i->MaxAlign = 1;
		i->Granularity = 128 * 1024;
		i->HasRealloc = false;
	}
	virtual void* Alloc(UPInt size, UPInt align)
	{
		// Ignore 'align' since reported MaxAlign is 1.
		return malloc(size);
	}
	virtual bool Free(void* ptr, UPInt size, UPInt align)
	{
		// free() doesn’t need size or alignment of the memory block, but
		// you can use it in your implementation if it makes things easier.
		free(ptr);
		return true;
	}
};

class FlasherCommandHandler : public GFxFSCommandHandler
{
public:
	FlasherCommandHandler() : GFxFSCommandHandler() {}

	virtual void Callback(GFxMovieView* pMovie, const char* command, const char* args);

	template<class Callbackclass>
	void LinkFSCallBack(int nId, Callbackclass* cbclass, INT(Callbackclass::* callback)(void* command, void* args))
	{
		if (m_callbackMap.find(nId) != m_callbackMap.end())
		{
			DBO_WARNING_MESSAGE("ID: " << nId << " already registered.");
			return;
		}

		CNtlCallbackVPParam2* pFSCallBack = new CNtlCallbackVPWidget2<Callbackclass>(cbclass, callback);
		m_callbackMap.insert(std::make_pair(nId, pFSCallBack));
	}

	void UnLinkFSCallBack(int nId)
	{
		std::map<int, CNtlCallbackVPParam2*>::iterator it = m_callbackMap.find(nId);
		if (it != m_callbackMap.end())
		{
			CNtlCallbackVPParam2* pCallBack = it->second;

			delete pCallBack;

			m_callbackMap.erase(it);
		}
	}

private:
	std::map<int, CNtlCallbackVPParam2*>	m_callbackMap;
};

class CNtlFlasherManager
{
public:
	//! Enumuration

	//! Destructor
	~CNtlFlasherManager(VOID);

	//! Operation
	BOOL CreateInstance( HWND hWnd );
	VOID DeleteInstance(VOID);
	VOID ResetRenderer(VOID);

	VOID SetVideoMode(VOID);

	BOOL IsEnableRender(VOID) { return m_bRendererEnable; }

	VOID SetFontLibData( std::string& strLibFileName );
	VOID SetFontMapData( std::string& strMapName, std::string& strFontname, std::string& strStyle );

	GPtr<GFxFontLib> GetFontLib(VOID) { return m_pFontLib; }
	GPtr<GFxFontMap> GetFontMap(VOID) { return m_pFontMap; }

	VOID SetScreenSize( INT nWidth, INT nHeight ) { m_nScreenWidth = nWidth; m_nScreenHeight = nHeight; }
	INT	GetScreenWidth(VOID) { return m_nScreenWidth; }
	INT	GetScreenHeight(VOID) { return m_nScreenHeight; }

	//! Access
	static CNtlFlasherManager*			GetInstance(VOID);

	inline GRendererD3D9*				GetFlashRenderer(VOID);
	inline LPDIRECT3DSTATEBLOCK9		GetStateBlock(VOID);

	inline MySysAlloc*					GetMemoryAlloc(VOID);

	inline GFxLoader*					GetLoader(VOID);

	FlasherCommandHandler*				GetCommandHandler(VOID) { return m_pCommandHandler.GetPtr(); }

	//! Callback
	INT DeviceRestoreCallback(VOID);
	INT DeviceReleaseCallback(VOID);

	int									GenerateUniqueID();

private:
	//! Implementation

	//! Variable
	HWND					m_hWnd;

	GFxSystem*				m_gfxSystem;
	GFxLoader*				m_gfxLoader;
	MySysAlloc				m_sysAlloc;

	GPtr<FlasherFileOpener>	m_pFileOpener;

	GPtr<GRendererD3D9>		m_pRenderer;

	GPtr<FlasherCommandHandler>	m_pCommandHandler;

	D3DPRESENT_PARAMETERS	m_d3dpp;
	LPDIRECT3DSTATEBLOCK9	m_pStateBlock;

	BOOL					m_bRendererEnable;

	DEVICE_RESTORE_HANDLE	m_hDeviceRestore;
	DEVICE_RESTORE_HANDLE	m_hDeviceRelease;

	GPtr<GFxFontLib>		m_pFontLib;
	GPtr<GFxFontMap>		m_pFontMap;
	GPtr<GFxMovieDef>		m_pLibMovie;

	INT						m_nScreenWidth;
	INT						m_nScreenHeight;

	int						m_nUniqueID;
	
	//! Prevented Constructor
	CNtlFlasherManager(VOID);
};

static CNtlFlasherManager* GetFlasherManager(VOID)
{
	return CNtlFlasherManager::GetInstance();
}

LPDIRECT3DSTATEBLOCK9 CNtlFlasherManager::GetStateBlock(VOID)
{
	return m_pStateBlock;
}

GRendererD3D9* CNtlFlasherManager::GetFlashRenderer(VOID)
{
	return m_pRenderer;
}

MySysAlloc* CNtlFlasherManager::GetMemoryAlloc(VOID)
{
	return &m_sysAlloc;
}

GFxLoader* CNtlFlasherManager::GetLoader(VOID)
{
	return m_gfxLoader;
}

#endif//__NTL_FLASHER_MANAGER_H__
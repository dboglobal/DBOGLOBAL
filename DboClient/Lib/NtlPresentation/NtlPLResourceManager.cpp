#include "precomp_ntlpresentation.h"
#include "NtlPLResourceManager.h"
#include "NtlPLHelpers.h"
#include "NtlDebug.h"
#include "NtlPLEntity.h"
#include "rtanim.h" 
#include "ntlworldcommon.h"
#include "NtlPLResourcePack.h"
#include "NtlPLResourceScheduling.h"
#include "NtlProfiler.h"

#include "NtlStringUtil.h"

/*_rwImageSwapPath(&m_pcPathTemp, &m_iPathSizeTemp);
RwImageSetPath(strImagePath.c_str());*/

// RwBool TextureLoadLevelPath(const RwChar* pInPath, RwChar** ppOutPath, RwInt32* pOutPathSize)
// {
// 	return GetNtlResourceManager()->TextureLoadLevelPath(pInPath, ppOutPath, pOutPathSize);
// }
//  
// RwBool CNtlPLResourceManager::TextureLoadLevelPath(const RwChar* pInPath, RwChar** ppOutPath, RwInt32* pOutPathSize)
// {
// 	RwBool bResult	= FALSE;
// 	m_strImagePath	= pInPath;
// 	m_strImagePath	= StringReplace(m_strImagePath, "\\", "/");
// 	
// 	std::transform(m_strImagePath.begin(), m_strImagePath.end(), m_strImagePath.begin(), tolower);
// 	for (TextureLoadDirectoryMap::iterator it = m_mapTextureLoadDirectroy.begin(); it != m_mapTextureLoadDirectroy.end(); ++it)
// 	{
// 		std::string strLevel	= it->second.strDirectory;
// 		switch (it->second.uiLevelCur)
// 		{
// 		case 1:
// 			strLevel += "_lv1/";
// 			break;
// 		case 2:
// 			strLevel += "_lv2/";
// 			break;
// 		case 3:
// 			strLevel += "_lv3/";
// 			break;
// 		default:
// 			continue;
// 		}
// 
// 		m_strImagePath	= StringReplace(m_strImagePath, it->second.strDirectory + "/", strLevel);
// 		bResult			= TRUE;
// 	}
// 
// 	if (bResult)
// 	{
// 		if (m_strImagePath.substr(m_strImagePath.size()) != ";")
// 		{
// 			m_strImagePath += ";";
// 		}		
// 		m_strImagePath += pInPath;
// 
// 		*ppOutPath		= const_cast<RwChar*>(m_strImagePath.c_str()); // 위험하지만 이렇게 하자...
// 		*pOutPathSize	= m_strImagePath.size();
// 	}
// 	return bResult;
// }
// 
// void CNtlPLResourceManager::SetTextureLoadLevel(RwUInt32 eResDictType, RwUInt32 uiLevel)
// {
// 
// 	TextureLoadDirectoryMap::iterator	it		= m_mapTextureLoadDirectroy.find(eResDictType);
// 	RwUInt32							uiMax	= 0;
// 
// 	if (it == m_mapTextureLoadDirectroy.end())
// 	{
// 		TextureLoadDirectoryMap::_Pairib	pairib;
// 		STextureLoadLevelDirectory			sTextureLoadLevelDirectory;
// 
// 		switch (eResDictType)
// 		{
// 		case RES_TEX_TERRAIN_DICT:
// 			sTextureLoadLevelDirectory.strDirectory = "ntlwe";
// 			sTextureLoadLevelDirectory.uiLevelCur	= 0;
// 			sTextureLoadLevelDirectory.uiLevelMax	= 3;
// 
// 			pairib = m_mapTextureLoadDirectroy.insert(TextureLoadDirectoryMap::value_type(RES_TEX_TERRAIN_DICT, sTextureLoadLevelDirectory));
// 			break;
// 		case RES_TEX_OBJECT_DICT:
// 			sTextureLoadLevelDirectory.strDirectory = "object";
// 			sTextureLoadLevelDirectory.uiLevelCur	= 0;
// 			sTextureLoadLevelDirectory.uiLevelMax	= 3;
// 
// 			pairib = m_mapTextureLoadDirectroy.insert(TextureLoadDirectoryMap::value_type(RES_TEX_OBJECT_DICT, sTextureLoadLevelDirectory));
// 			break;
// 		case RES_TEX_CHARACTER_DICT:
// 			sTextureLoadLevelDirectory.strDirectory = "character";
// 			sTextureLoadLevelDirectory.uiLevelCur	= 0;
// 			sTextureLoadLevelDirectory.uiLevelMax	= 3;
// 
// 			pairib = m_mapTextureLoadDirectroy.insert(TextureLoadDirectoryMap::value_type(RES_TEX_CHARACTER_DICT, sTextureLoadLevelDirectory));
// 			break;
// 		default:
// 			return;
// 		}
// 
// 		if (!pairib.second)
// 		{
// 			return;
// 		}
// 
// 		it = pairib.first;
// 	}
// 	
// 	it->second.uiLevelCur = uiLevel;
// 
// 	CLAMP(it->second.uiLevelCur, 0, it->second.uiLevelMax);	
// }

CNtlPLResourceManager::CNtlPLResourceManager()
{
// 	SetTextureLoadLevel(RES_TEX_TERRAIN_DICT, 0);
// 	SetTextureLoadLevel(RES_TEX_OBJECT_DICT, 0);
// 	SetTextureLoadLevel(RES_TEX_CHARACTER_DICT, 0);

	m_bLoadScheduling		= FALSE;
	m_bDeleteScheduling		= FALSE;

	m_pScheduleManager = NTL_NEW CNtlResourceScheduleManager;
}

/**
*  singleton CNtlPLResourceManager interface 함수.
*
*/
CNtlPLResourceManager* CNtlPLResourceManager::GetInstance(void)
{
	static CNtlPLResourceManager ResMgr;
	return &ResMgr;
}

/**
* Initial 함수.
*
*/
bool CNtlPLResourceManager::Crate(void)
{
	return true;
}


/**
*  Terminate 함수.
*
*/
void CNtlPLResourceManager::Destroy(void)
{
	if(m_pScheduleManager)
	{
		NTL_DELETE(m_pScheduleManager);
	}

#ifdef _DEBUG

	DebugResource();

#endif
}


/**
*  Update 함수.
*
*/

void CNtlPLResourceManager::Update(RwReal fElapsed)
{
	NTL_SPROFILE("CNtlPLResourceManager::Update")

	if(m_pScheduleManager)
	{
		m_pScheduleManager->Update(fElapsed);
	}

	NTL_EPROFILE()
}

/**
*  application이 종료 되었을 때 해지되지 않은 resource를 debugging을 위한 함수.
*  debugging 내용을 file or output window로 display 할 수 있도록 한다.
*
*/
void CNtlPLResourceManager::DebugResource(void)
{
	NTL_ASSERT(m_mapWorld.size() == 0, "CNtlPLResourceManager::DebugResource => world is not empty");
	NTL_ASSERT(m_mapClump.size() == 0, "CNtlPLResourceManager::DebugResource => clump is not empty");
	NTL_ASSERT(m_mapAtomic.size() == 0, "CNtlPLResourceManager::DebugResource => atomic is not empty");
	NTL_ASSERT(m_mmapCloneClump.size() == 0, "CNtlPLResourceManager::DebugResource => clone clump is not empty");
	NTL_ASSERT(m_mmapCloneAtomic.size() == 0, "CNtlPLResourceManager::DebugResource => clone atomic is not empty");
	NTL_ASSERT(m_mapAnim.size() == 0, "CNtlPLResourceManager::DebugResource => animation is not empty");
}


/**
*  CNtlPLResource 객체를 생성하는 함수.
*
*  \param pName resource name을 CNtlPLResource에 담는다.
*  \param iType renderware data의 type.
*  \param iType renderware data의 pointer.
*/
CNtlPLResource* CNtlPLResourceManager::CreateResource(const char *pName, unsigned int uiType, bool bClone, void *pData)
{
	NTL_FUNCTION("CNtlPLResourceManager::CreateResource");

	CNtlPLResource *pNewResource = NTL_NEW CNtlPLResource;
	pNewResource->SetName(pName);
	pNewResource->SetType(uiType); 
	pNewResource->SetClone(bClone); 
	pNewResource->SetData(pData);
	pNewResource->AddRef();

	NTL_RETURN(pNewResource);
}

/**
*  CNtlPLResource 객체를 container에 등록하는 함수.
*
*  \param pResource container 등록할 resource pointer
*/
void CNtlPLResourceManager::RegisterResource(const CNtlPLResource *pResource)
{
	NTL_FUNCTION("CNtlPLResourceManager::RegisterResource");

	std::string strName = pResource->GetName(); 

	switch(pResource->GetType())
	{
	case rwID_WORLD:  
		m_mapWorld[strName] = const_cast<CNtlPLResource*>(pResource);
		break;
    case rwID_CLUMP:   
		if(pResource->GetClone()) 
			m_mmapCloneClump.insert(ResourceMultiMap::value_type(strName, const_cast<CNtlPLResource*>(pResource)));
		else
			m_mapClump.insert(ResourceMultiMap::value_type(strName, const_cast<CNtlPLResource*>(pResource)));
		break;
	case rwID_ATOMIC:
		if(pResource->GetClone()) 
			m_mmapCloneAtomic.insert(ResourceMultiMap::value_type(strName, const_cast<CNtlPLResource*>(pResource)));
		else
			m_mapAtomic.insert(ResourceMultiMap::value_type(strName, const_cast<CNtlPLResource*>(pResource)));
		break;
	case rwID_ANIMANIMATION: 
		m_mapAnim[strName] = const_cast<CNtlPLResource*>(pResource);
		break;
	default:
		{
			NTL_ASSERTFAIL("CNtlPLResourceManager::RegisterResource doesn't support this RenderWare type ( " << pResource->GetType() << " )");
		}
	break;

	}

	NTL_RETURNVOID();
}


/**
*  CNtlPLResource 객체를 container에서 해제하는 함수.
*
*  \param pResource container 해제할 resource pointer
*/
void CNtlPLResourceManager::UnRegisterResource(const CNtlPLResource *pResource)
{
	NTL_FUNCTION("CNtlPLResourceManager::UnRegisterResource");

	std::string strName = pResource->GetName();

	switch(pResource->GetType())
	{
		case rwID_WORLD:  
			{
				ResourceMap::iterator it;
				it = m_mapWorld.find(strName);
				if(it != m_mapWorld.end())
				{
					m_mapWorld.erase(it); 
				}
			}
			break;
        case rwID_CLUMP:   
			{
				if(pResource->GetClone())
				{
					ResourceMultiMap::iterator it;
					it = m_mmapCloneClump.find(strName);
					if(it != m_mmapCloneClump.end())
					{
						RwInt32 iCount = (RwInt32)m_mmapCloneClump.count(strName); 
						for(RwInt32 i = 0; i < iCount; i++)
						{
							if((*it).second == pResource)
								it = m_mmapCloneClump.erase(it); 
							else
								++it;
						}
					}
				}
				else
				{
					ResourceMap::iterator it;
					it = m_mapClump.find(strName);
					if(it != m_mapClump.end())
					{
						m_mapClump.erase(it); 
					}
				}

				RwInt32 iCount = (RwInt32)m_mmapCloneClump.count(strName); 
				if(iCount <= 0)
				{
					ResourceMap::iterator it = m_mapClump.find(strName);
					if(it != m_mapClump.end())
					{
						CNtlPLResource *pResourceData = (*it).second;
						UnLoad(pResourceData);
					}
				}
			}
			break;
		case rwID_ATOMIC:
			{
				if(pResource->GetClone())
				{
					ResourceMultiMap::iterator it;
					it = m_mmapCloneAtomic.find(strName);
					if(it != m_mmapCloneAtomic.end())
					{
						RwInt32 iCount = (RwInt32)m_mmapCloneAtomic.count(strName); 
						for(RwInt32 i = 0; i < iCount; i++)
						{
							if((*it).second == pResource)
								it = m_mmapCloneAtomic.erase(it); 
							else
								++it;
						}
					}
				}
				else
				{
					ResourceMap::iterator it;
					it = m_mapAtomic.find(strName);
					if(it != m_mapAtomic.end())
					{
						m_mapAtomic.erase(it); 
					}
				}

				RwInt32 iCount = (RwInt32)m_mmapCloneAtomic.count(strName); 
				if(iCount <= 0)
				{
					ResourceMap::iterator it = m_mapAtomic.find(strName);
					if(it != m_mapAtomic.end())
					{
						CNtlPLResource *pResourceData = (*it).second;
						UnLoad(pResourceData);
					}
				}
			}
			break;
		case rwID_ANIMANIMATION: 
			{
				ResourceMap::iterator it;
				it = m_mapAnim.find(strName);
				if(it != m_mapAnim.end())
				{
					m_mapAnim.erase(it);
				}
			}
			break;
		default:
			{
				NTL_ASSERTFAIL("CNtlPLResourceManager::UnLoad doesn't support this RenderWare type ( " << pResource->GetType() << " )");
			}
		break;

	}

	NTL_RETURNVOID();
}

/**
*  RenderWare data를 읽는 함수.
*  clump와 atomic는 처음 생성하더라도 clone clump 및 atomic으로 다시 생성하여 리턴한다.
*
*  \param pStrName file name
*  \param pResourcePath resource path
*  \param uiType RednerWare resource type
*/


/*
bool g_ResFlag = false;
RwMemory Mem;

void Doremi(void *file, RwUInt32 size, RwUInt32 status, RtFSFileOperation fileOp, void *data)
{
	g_ResFlag = true;
	Mem.
}
*/

CNtlPLResource* CNtlPLResourceManager::LoadDefault(const char *pStrName, const char * pResourcePath, unsigned int uiType)
{
	NTL_FUNCTION("CNtlPLResourceManager::Load");

    NTL_PRE(pStrName);

	/*
	g_ResFlag = false;
	RtFileSystem *pDFS = NULL;
	RtFileSystemFileFunctionTable defaultFFT;
	pDFS = RtFSManagerGetDefaultFileSystem();
	defaultFFT = pDFS->fsFileFunc;

	FileAccessFlag |= RTFILE_ACCESS_OPEN_READ;
	FileAccessFlag |= RTFILE_ACCESS_OPEN_ASYNC;

	Mem.start = (unsigned char *)RwMalloc(512 * 131000 + 1, rwMEMHINTDUR_GLOBAL);
	ReturnFile = RtFSManagerFOpen(pStrName, FileAccessFlag, Doremi, Mem.start);

	ReturnFile->status = RTFILE_STATUS_READY;
	pDFS->fsFileFunc.read(ReturnFile, pBuffer, 512 * 131000);//33554432);

	*/

	RwChar *pFileName = Helper_AbstractFileName(const_cast<RwChar*>(pStrName));
    
	void *pData = NULL;
	CNtlPLResource *pNewResource = NULL;
	CNtlPLResource *pResource = Find(pFileName, uiType);

	// What if a resource of the same name does not exist?
	if(pResource == NULL)
	{
		RwStream *pStream = NULL;
		void *fp = NULL;

		SPackResFileData sPackFileData;
		bool bPack = GetNtlResourcePackManager()->LoadObject(pStrName, sPackFileData);

		if(bPack)
		{
			fp = RwFopen(sPackFileData.strPackFileName.c_str(), "rb");
			if(fp == NULL)
				return NULL;

			RwFseek(fp, sPackFileData.uiOffset, RTFILE_POS_BEGIN);
			pStream = RwStreamOpen(rwSTREAMFILE, rwSTREAMREAD, fp);
		}
		else
		{
			pStream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, pStrName);
		}

		if(pStream)
		{
			if (pResourcePath)
			{
				RwImageSetPath(pResourcePath);
			}

			RwChunkHeaderInfo HeaderInfo;
			RwStreamReadChunkHeaderInfo(pStream, &HeaderInfo);

			pData = CreateRwData(pStream, HeaderInfo.type);
			
			RwStreamClose( pStream, NULL );

			pNewResource = CreateResource(pFileName, uiType, false, pData); 
			RegisterResource(pNewResource);
		}
		else    // If the corresponding clump is not loaded, NULL is returned. (by agebreak 2006-11-29)
		{
			if(fp)
				RwFclose(fp);

			return NULL;
		}

		if(fp)
			RwFclose(fp);
	}
	else
	{
		// When a resource of the same name exists.
		// clone data, not animation, world.
		if(uiType != rwID_CLUMP && uiType != rwID_ATOMIC)
		{
			pNewResource = pResource;
			pNewResource->AddRef(); 
		}
	}

	// clone 생성을 위해서..
	pResource = Find(pFileName, uiType);

	// clone 생성.
	if(uiType == rwID_CLUMP || uiType == rwID_ATOMIC)
	{
		pData = CreateCloneRwData( pResource->GetType(), (void*)pResource->GetData() );  
		NTL_ASSERT(pData, "resource clone fail !!!" << NTL_VALIDSTRING(pResource->GetName()));

		if(pData)
		{
			pNewResource = CreateResource(pFileName, pResource->GetType(), true, pData); 
		}

		RegisterResource(pNewResource);
	}

	NTL_RETURN(pNewResource);
}

/**
*  RenderWare world data를 읽는 함수.
*
*  \param pStrName file name
*  \param pResourcePath resource path
*/

CNtlPLResource* CNtlPLResourceManager::LoadWorld(const char *pStrName, const char * pResourcePath)
{
	NTL_FUNCTION("CNtlPLResourceManager::LoadWorld");

	CNtlPLResource *pResource = LoadDefault(pStrName, pResourcePath, rwID_WORLD);

	NTL_RETURN(pResource); 
}

/**
*  RenderWare data를 읽는 함수.
*
*  \param pStrName file name
*  \param pResourcePath resource path
*  \param bClone clump를 clone으로 생성할 것인지를 결정하는 변수.
*/

CNtlPLResource* CNtlPLResourceManager::LoadClump(const char *pStrName, const char * pResourcePath)
{
	NTL_FUNCTION("CNtlPLResourceManager::LoadClump");	

	CNtlPLResource *pResource = LoadDefault(pStrName, pResourcePath, rwID_CLUMP);
	if(pResource == NULL)
	{		
		DBO_TRACE(FALSE, "CNtlPLResourceManager::LoadClump the clump find not(" << pStrName << ")");
		NTL_RETURN(NULL); 
	}
	
	NTL_RETURN(pResource); 
}

void CNtlPLResourceManager::LoadSchedulingClump(const char *pStrName, const char * pResourcePath, CNtlPLEntity *pEntity)
{
	if(!m_bLoadScheduling)
	{
		if(pEntity)
			pEntity->CallPreSchedulingResource();

		CNtlPLResource *pResource = LoadClump(pStrName, pResourcePath);

		if(pEntity)
			pEntity->CallSchedulingResource(pResource);

		return;
	}

	m_pScheduleManager->AddSchedulingClump(pStrName, pResourcePath, pEntity);
}

/**
*  RenderWare data를 읽는 함수.
*
*  \param pStrName file name
*  \param pResourcePath resource path
*  \param bClone atomic를 clone으로 생성할 것인지를 결정하는 변수.
*/

CNtlPLResource* CNtlPLResourceManager::LoadAtomic(const char *pStrName, const char * pResourcePath)
{
	NTL_FUNCTION("CNtlPLResourceManager::LoadAtomic");

	CNtlPLResource *pResource = LoadDefault(pStrName, pResourcePath, rwID_ATOMIC);

	NTL_RETURN(pResource); 
}

CNtlPLResource* CNtlPLResourceManager::LoadAnimation(const char *pStrName)
{
	NTL_FUNCTION("CNtlPLResourceManager::LoadAnim");

	CNtlPLResource *pResource = LoadDefault(pStrName, NULL, rwID_ANIMANIMATION);
	if(pResource == NULL)
	{
		DBO_ASSERT(FALSE, "CNtlPLResourceManager::LoadAnimation the animation find not(" << pStrName << ")");
		NTL_RETURN(NULL); 
	}

	NTL_RETURN(pResource); 
}

/**
*  RenderWare data를 읽는 함수.
*
*  \param pStrName file name
*  \param pResourcePath resource path
*  \param eFilterMode texture의 filtering mode. renderware RwTextureFilterMode를 참조.
*  \param EResDictType texture를 group으로 나누어 관리할 경우에 group의 id가 된다.
*         group으로 관리하는 이유는 검색 속도를 빠르게 할 수 있다.     
*         일반적으로 object, character, terrain, texture, ui등의 group을 생각해 볼 수 있다.
*/

RwTexture* CNtlPLResourceManager::LoadTexture(const char *pStrName, const char * pResourcePath, EResDictType eType /*= RES_TEX_DEFAULT_DICT*/ , const char *pStrMaskName /*= NULL*/)
{
	NTL_FUNCTION("CNtlPLResourceManager::LoadTexture");

	NTL_PRE(pStrName);
    NTL_PRE(pResourcePath);
	
	RwImageSetPath(pResourcePath);
	

	RwTexture *pTexture = RwTextureRead(pStrName, pStrMaskName);
	if (pTexture == NULL)
	{
		static RwChar acCurrentDirectory[1024];
		GetCurrentDirectory(1024, acCurrentDirectory);
		DBO_WARNING_MESSAGE("the texture find not(" << pStrName << ") " << " - " << acCurrentDirectory << " - " << pResourcePath);
	}
	
	// RwTextureSetFilterMode (pTexture, eFilterMode);
	// by HongHoDong (June 15, 2006) If you have the MIP option
	// Save and read If Mip is checked, it searches for Mipmap.
	// There is no problem for dds, but for png file there is a problem.
//	DBO_WARNING_MESSAGE("Load Texture: " << pStrName);
//	RwTextureSetFilterMode(pTexture, rwFILTERLINEARMIPLINEAR); // enabled by daneos
	
	NTL_RETURN(pTexture); 
}

RwTexture* CNtlPLResourceManager::CreateTexture(const char *pStrName, 
												RwInt32 iWidth, 
												RwInt32 iHeight, 
												RwInt32 iDepth,
												RwRasterFormat eFormat,
												EResDictType eType /*= RES_TEX_DEFAULT_DICT*/)
{
//	nWidth = GetSortSize(nWidth);
//	nHeight = GetSortSize(nHeight);

	RwRaster *pRaster = RwRasterCreate(iWidth, iHeight, iDepth, rwRASTERTYPETEXTURE | eFormat);
	if(pRaster == NULL)
		return NULL;
    
	RwTexture *pRwTexture = RwTextureCreate(pRaster); 
	if(pRwTexture == NULL)
	{
		RwRasterDestroy(pRaster);
		return NULL;
	}

	RwTextureSetName(pRwTexture, pStrName); 

	RwTextureSetFilterMode(pRwTexture, rwFILTERLINEARMIPLINEAR);

	return pRwTexture;
}


/**
*  resource를 unload 한다.
*
*  \param pResource unload할 resource pointer
*/
void CNtlPLResourceManager::UnLoad(CNtlPLResource *pResource)
{
	NTL_FUNCTION("CNtlPLResourceManager::UnLoad");
	
	pResource->ReleaseRef(); 
	if(pResource->GetRefCount() == 0)
	{
		UnRegisterResource(pResource);

		DBO_ASSERT(pResource->GetData(), "Resource Data is Null(" << pResource->GetName() << ")");

		if (pResource->GetData())
		{
			DestroyRwData(pResource->GetType(), (void*)pResource->GetData());  
		}

		NTL_DELETE(pResource);
	}
	
	NTL_RETURNVOID();
}

/**
*  scheduling상에 등록되어 있는 entity에 해당하는 scheduling node를 삭제한다.
*
*/
void CNtlPLResourceManager::UnLoadScheduling(CNtlPLEntity *pPLEntity, CNtlPLResource *pResource)
{
	if(pResource)
	{
		if(!m_bDeleteScheduling)
		{
			UnLoad(pResource);
			return;
		}
	}

	m_pScheduleManager->UnLoadScheduling(pPLEntity, pResource);
}

/**
*  container에서 name으로 CNtlPLResource를 찾는다.
*
*  \param pTexture texture pointer
*/
void CNtlPLResourceManager::UnLoadTexture(RwTexture *pTexture)
{
	RwTextureDestroy(pTexture);
}

/**
*  container에서 name으로 CNtlPLResource를 찾는다.
*
*  \param pName name string
*  \param uiType the resource type
*  \param uiType the resource dictory type
*/

CNtlPLResource* CNtlPLResourceManager::Find(const char *pName, unsigned int uiType, EResDictType eDictType /*= RES_TEX_DEFAULT_DICT*/)
{
	NTL_FUNCTION("CNtlPLResourceManager::Find");
	
	std::string strName = pName;

	switch(uiType)
	{
		case rwID_WORLD:  
			{
				ResourceMap::iterator it;
				it = m_mapWorld.find(strName);
				if(it != m_mapWorld.end())
				{
					NTL_RETURN( (*it).second ); 
				}
				else 
				{
					NTL_RETURN(NULL);
				}
			}
			break;
        case rwID_CLUMP:   
			{
				ResourceMap::iterator it;
				it = m_mapClump.find(strName);
				if(it != m_mapClump.end())
				{
					NTL_RETURN( (*it).second ); 
				}
				else 
				{
					NTL_RETURN(NULL);
				}
			}
			break;
		case rwID_ATOMIC:
			{
				ResourceMap::iterator it;
				it = m_mapAtomic.find(strName);
				if(it != m_mapAtomic.end())
				{
					NTL_RETURN( (*it).second ); 
				}
				else 
				{
					NTL_RETURN(NULL);
				}
			}
			break;
		case rwID_ANIMANIMATION: 
			{
				ResourceMap::iterator it;
				it = m_mapAnim.find(strName);
				if(it != m_mapAnim.end())
				{
					NTL_RETURN( (*it).second ); 
				}
				else 
				{
					NTL_RETURN(NULL);
				}
			}
			break;
		default:
			{
				NTL_ASSERTFAIL("CNtlPLResourceManager::DestroyRwData doesn't support this RenderWare type ( " << uiType << " )");
			}
		break;

	}

	NTL_RETURN(NULL);
}

/**
*  renderware world를 생성하는 함수.
*
*  \param pStream world stream data
*/
RpWorld* CNtlPLResourceManager::WorldStreamRead (RwStream *pStream)
{
	NTL_FUNCTION("CNtlPLResourceManager::WorldStreamRead");
    NTL_PRE(pStream);
         
    RpWorld *pWorld;

	pWorld = RpWorldStreamRead(pStream);

    NTL_POST(pWorld);

    NTL_RETURN( pWorld );
}


/**
*  renderware atomic를 생성하는 함수.
*
*  \param pStream atomic stream data
*/
RpAtomic* CNtlPLResourceManager::AtomicStreamRead (RwStream *pStream)
{
	NTL_FUNCTION("RWS::CRenderwareResource::AtomicStreamRead");
    NTL_PRE(pStream);

    RpAtomic *pAtomic;

    pAtomic = RpAtomicStreamRead(pStream);

    NTL_POST(pAtomic);

	NTL_RETURN ( pAtomic );
}



/**
*  renderware clump를 생성하는 함수.
*
*  \param pStream clump stream data
*/

RpClump* CNtlPLResourceManager::ClumpStreamRead (RwStream *pStream)
{
    NTL_FUNCTION("CNtlPLResourceManager::ClumpStreamRead");
    NTL_PRE(pStream);

    RpClump *pClump;

	//RwStreamFindChunk(pStream, rwID_CLUMP, NULL, NULL);

    pClump = RpClumpStreamRead(pStream);

    NTL_POST(pClump);

	NTL_RETURN(pClump);
}



/**
*  renderware data type에 해당하는 data를 생성하는 함수.
*
*  \param pSteam stream 객체 pointer.
*  \param iType renderware data type.
*/

void* CNtlPLResourceManager::CreateRwData(RwStream *pStream, unsigned int iType)
{
	NTL_FUNCTION("CNtlPLResourceManager::CreateRwData");

	void *pData = NULL;

	switch (iType)
	{
		default:
		case rwID_NAOBJECT:
			{
				RWS_ASSERTFAIL("CNtlPLResourceManager::Load doesn't support this RenderWare type ( " << iType << " )");
			}
		break;

		case rwID_WORLD:          
			pData = WorldStreamRead(pStream);         
			break;
		case rwID_CLUMP:       
			pData = ClumpStreamRead(pStream);       
			break;
		case rwID_ATOMIC:     
			pData = AtomicStreamRead(pStream);     
			break;
		case rwID_ANIMANIMATION:
			pData = RtAnimAnimationStreamRead (pStream); 
			break;
//		case rwID_HANIMANIMATION:
//			RtHAnimAnimationStreamRead (pStream); 
//			break;
//				case rwID_TEXDICTIONARY:   pData = TexDictionaryStreamRead(pStream);    break;
//				case rwID_PITEXDICTIONARY: pData = PiTexDictionaryStreamRead(pStream);  break;
//				case rwID_HANIMANIMATION:  pData = RpHAnimAnimationStreamRead(pStream); break;
//				case rwID_2DMAESTRO:       pData = MaestroStreamRead(pStream);          break;
//				case rwID_2DFONT:          pData = FontStreamRead(pStream);             break;
	}

	NTL_RETURN(pData);
}

/**
*  renderware data에 해당하는 clone data를 생성한다.
*  type을 비교하여 적절한 renderware clone 함수를 적용한다.
*
*  \param iType renderware data type.
*  \param pData clone할 renderware data.
*/
void* CNtlPLResourceManager::CreateCloneRwData(unsigned int iType, void *pData)
{
	NTL_FUNCTION("CNtlPLResourceManager::GetCloneData");

	void *pNewData = NULL;

	switch(iType)
	{
		case rwID_WORLD:          
			break;
        case rwID_CLUMP:   
			pNewData = RpClumpClone(reinterpret_cast<RpClump*>(pData));  
			break;
		case rwID_ATOMIC:
			pNewData = RpAtomicClone(reinterpret_cast<RpAtomic*>(pData));  
			break;
	}

	NTL_RETURN(pNewData);
}

/**
*  renderware data에 해당하는 type에 적절하게 삭제한다.
*
*  \param iType renderware data type.
*  \param pData 삭제할 renderware data.
*/
void CNtlPLResourceManager::DestroyRwData(unsigned int iType, void *pData)
{
	NTL_FUNCTION("CNtlPLResourceManager::DestroyRwData");

	switch(iType)
	{
		case rwID_WORLD:          
			RpWorldDestroy(reinterpret_cast<RpWorld*>(pData));         
			break;
        case rwID_CLUMP:   
			RpClumpDestroy(reinterpret_cast<RpClump*>(pData));  
			break;
		case rwID_ATOMIC:
			RpAtomicDestroy(reinterpret_cast<RpAtomic*>(pData));  
			break;
		case rwID_TEXTURE:
			RwTextureDestroy(reinterpret_cast<RwTexture*>(pData));
			break;
		case rwID_ANIMANIMATION: 
			RtAnimAnimationDestroy(reinterpret_cast<RtAnimAnimation*>(pData));
			break;
		default:
			{
				NTL_ASSERTFAIL("CNtlPLResourceManager::DestroyRwData doesn't support this RenderWare type ( " << iType << " )");
			}
		break;

	}

	NTL_RETURNVOID();
}

RwBool CNtlPLResourceManager::LoadAlphaBits(RwTexture *pTexture, RwUInt8 *pPreparedBits)
{	
	NTL_FUNCTION("CNtlPLResourceManager::LoadAlphaBits");

	if(!pTexture)
		NTL_RETURN(FALSE);

	RwInt32 RwTexW	= RwRasterGetWidth(pTexture->raster);
	RwInt32 RwTexH	= RwRasterGetHeight(pTexture->raster);
	RwInt32 CurIdx;
	DWORD *pDest;

	DWORD *pBits = (DWORD*)RwRasterLock(pTexture->raster, 0, rwRASTERLOCKWRITE);
	{
		for(RwInt32 PosY = 0; PosY < RwTexH; ++PosY)
		{
			for(RwInt32 PosX = 0; PosX < RwTexW; ++PosX)
			{
				CurIdx = RwTexW * PosY + PosX;
				pDest = pBits + CurIdx;
				(*pDest) = (((DWORD)pPreparedBits[CurIdx]) << 24);
			}
		}
	}
	RwRasterUnlock(pTexture->raster);

	NTL_RETURN(TRUE);
}

RwBool CNtlPLResourceManager::LoadAlphaBitsShadowMap(RwTexture *pTexture, FILE* pFile, RwRGBA* pShdowClr)
{
	NTL_FUNCTION("CNtlPLResourceManager::LoadAlphaBits");

	if(!pTexture)
		NTL_RETURN(FALSE);

	RwInt32 RwTexW	= RwRasterGetWidth(pTexture->raster);
	RwInt32 RwTexH	= RwRasterGetHeight(pTexture->raster);
	RwInt32 CurIdx;
	DWORD *pDest;

	RwUInt8 uiShadowGray = Helper_RGB2Gray(pShdowClr->red, pShdowClr->green, pShdowClr->blue);
	DWORD *pBits = (DWORD*)RwRasterLock(pTexture->raster, 0, rwRASTERLOCKWRITE);
	{
		for(RwInt32 PosY = 0; PosY < RwTexH; ++PosY)
		{
			for(RwInt32 PosX = 0; PosX < RwTexW; ++PosX)
			{
				CurIdx = RwTexW * PosY + PosX;
				pDest = pBits + CurIdx;			

				RwUInt8 uiGray = 255;
				::fread(&uiGray, sizeof(RwUInt8), 1, pFile);
				
				if (uiGray == 255)
				{
					(*pDest) = 0xffffffff;
				}
				else
				{
				RwInt32 iAdd = (RwInt32)uiGray - (RwInt32)uiShadowGray;
				
				RwInt32 R	= ((RwInt32)pShdowClr->red + iAdd);
				RwInt32 G	= ((RwInt32)pShdowClr->green + iAdd);
				RwInt32 B	= ((RwInt32)pShdowClr->blue + iAdd);

				CLAMP(R, 0, 255);
				CLAMP(G, 0, 255);
				CLAMP(B, 0, 255);

				(*pDest) = (DWORD)(0xff000000 | (BYTE)(R) << 16 | (BYTE)(G) << 8 | (BYTE)(B) << 0);
				}
			}
		}
	}
	RwRasterUnlock(pTexture->raster);

	NTL_RETURN(TRUE);
}

RwBool CNtlPLResourceManager::SaveAlphaBitsShadowMap(RwTexture *pTexture, FILE* pFile)
{
	NTL_FUNCTION("CNtlPLResourceManager::LoadAlphaBits");

	if(!pTexture)
		NTL_RETURN(FALSE);

	RwInt32 RwTexW	= RwRasterGetWidth(pTexture->raster);
	RwInt32 RwTexH	= RwRasterGetHeight(pTexture->raster);
	RwInt32 CurIdx;
	DWORD *pDest;

	DWORD *pBits = (DWORD*)RwRasterLock(pTexture->raster, 0, rwRASTERLOCKREAD);
	{
		for(RwInt32 PosY = 0; PosY < RwTexH; ++PosY)
		{
			for(RwInt32 PosX = 0; PosX < RwTexW; ++PosX)
			{				
				CurIdx = RwTexW * PosY + PosX;
				pDest = pBits + CurIdx;			
				
				RwUInt8 A	= (RwUInt8)(((*pDest) & 0xff000000) >> 24);
				RwUInt8 R	= (RwUInt8)(((*pDest) & 0x00ff0000) >> 16);
				RwUInt8 G	= (RwUInt8)(((*pDest) & 0x0000ff00) >> 8);
				RwUInt8 B	= (RwUInt8)(((*pDest) & 0x000000ff) >> 0);

				RwUInt8 uiGray = Helper_RGB2Gray(R, G, B);
				fwrite(&uiGray, sizeof(RwUInt8), 1, pFile);
			}
		}
	}
	RwRasterUnlock(pTexture->raster);

	NTL_RETURN(TRUE);
}

RwBool CNtlPLResourceManager::SetAlphaBits(RwTexture *pTexture, RwUInt8 Value, RwV2d Pos)
{
	NTL_FUNCTION("CNtlPLResourceManager::SetAlphaBits");
	NTL_PRE(pTexture);

	RwInt32 RwTexW = RwRasterGetWidth(pTexture->raster);
	RwInt32 RwTexH = RwRasterGetHeight(pTexture->raster);

	RwInt32 PosX = static_cast<RwInt32>(Pos.x);
	RwInt32 PosY = static_cast<RwInt32>(Pos.y);
	if(PosX < 0 || PosX >= RwTexW || PosY < 0 || PosY >= RwTexH)
		NTL_RETURN(FALSE);

	DWORD *pDest;
	DWORD *pBits = (DWORD*)RwRasterLock(pTexture->raster, 0, rwRASTERLOCKWRITE); 
	{
		pDest = pBits + RwTexW * PosY + PosX;
		(*pDest) = ((DWORD)Value) << 24;
	}
	RwRasterUnlock(pTexture->raster);
	NTL_RETURN(TRUE);
}

RwBool CNtlPLResourceManager::LoadTexture(RwTexture *pTexture, BYTE* pBuff)
{
	NTL_FUNCTION("CNtlPLResourceManager::LoadTexture");

	if(!pTexture)
		NTL_RETURN(FALSE);

	RwInt32 RwTexW = RwRasterGetWidth(pTexture->raster);
	RwInt32 RwTexH = RwRasterGetHeight(pTexture->raster);

	DWORD *pBitsSrc = (DWORD*)RwRasterLock(pTexture->raster, 0, rwRASTERLOCKWRITE);
	{
		CopyMemory(pBitsSrc, pBuff, sizeof(DWORD) * RwTexW * RwTexH);
	}
	RwRasterUnlock(pTexture->raster);

	NTL_RETURN(TRUE);
}

RwBool CNtlPLResourceManager::LoadTexture(RwTexture *pTexture, FILE *pFile)
{
	NTL_FUNCTION("CNtlPLResourceManager::LoadTexture");

	if(!pTexture)
		NTL_RETURN(FALSE);

	RwInt32 RwTexW = RwRasterGetWidth(pTexture->raster);
	RwInt32 RwTexH = RwRasterGetHeight(pTexture->raster);

	DWORD *pBitsSrc = (DWORD*)RwRasterLock(pTexture->raster, 0, rwRASTERLOCKWRITE);
	{
		::fread(pBitsSrc, sizeof(DWORD) * RwTexW * RwTexH, 1, pFile);
	}
	RwRasterUnlock(pTexture->raster);

	NTL_RETURN(TRUE);
}

RwBool CNtlPLResourceManager::SaveTextureShadowMap(RwTexture *pTexture, FILE *pFile)
{
	NTL_FUNCTION("CNtlPLResourceManager::SaveTextureShadowMap");

	if(!pTexture)
		NTL_RETURN(FALSE);

	RwInt32 RwTexW = RwRasterGetWidth(pTexture->raster);
	RwInt32 RwTexH = RwRasterGetHeight(pTexture->raster);

	DWORD *pBits = (DWORD*)RwRasterLock(pTexture->raster, 0, rwRASTERLOCKREAD);
	{
		for (RwInt32 i = 0; i< RwTexW * RwTexH; ++i)
		{
			DWORD clr = (*(pBits + i));

			RwUInt8 A = (RwUInt8)((clr & 0xff000000) >> 24);
			RwUInt8 R = (RwUInt8)((clr & 0x00ff0000) >> 16);
			RwUInt8 G = (RwUInt8)((clr & 0x0000ff00) >> 8);
			RwUInt8 B = (RwUInt8)((clr & 0x000000ff) >> 0);

			::fwrite(&R, sizeof(RwUInt8), 1, pFile);
			::fwrite(&G, sizeof(RwUInt8), 1, pFile);
			::fwrite(&B, sizeof(RwUInt8), 1, pFile);
		}		
	}
	RwRasterUnlock(pTexture->raster);

	NTL_RETURN(TRUE);
}

RwBool CNtlPLResourceManager::LoadTextureShadowMap(RwTexture *pTexture, FILE *pFile)
{
	NTL_FUNCTION("CNtlPLResourceManager::LoadTextureShadowMap");

	if(!pTexture)
		NTL_RETURN(FALSE);

	RwInt32 RwTexW = RwRasterGetWidth(pTexture->raster);
	RwInt32 RwTexH = RwRasterGetHeight(pTexture->raster);

	DWORD *pBits = (DWORD*)RwRasterLock(pTexture->raster, 0, rwRASTERLOCKWRITE);
	{
		for (RwInt32 i = 0; i< RwTexW * RwTexH; ++i)
		{
			RwUInt8 R = 255;
			RwUInt8 G = 255;
			RwUInt8 B = 255;

			::fread(&R, sizeof(RwUInt8), 1, pFile);
			::fread(&G, sizeof(RwUInt8), 1, pFile);
			::fread(&B, sizeof(RwUInt8), 1, pFile);

			DWORD clr = (DWORD)(0xff000000 | (BYTE)(R) << 16 | (BYTE)(G) << 8 | (BYTE)(B) << 0);
			(*(pBits + i)) = clr;			
		}		
	}
	RwRasterUnlock(pTexture->raster);

	NTL_RETURN(TRUE);
}

RwBool CNtlPLResourceManager::SaveTexture(RwTexture *pTexture, FILE *pFile)
{
	NTL_FUNCTION("CNtlPLResourceManager::SaveTexture");

	if(!pTexture)
		NTL_RETURN(FALSE);

	RwInt32 RwTexW = RwRasterGetWidth(pTexture->raster);
	RwInt32 RwTexH = RwRasterGetHeight(pTexture->raster);

	DWORD *pBitsSrc = (DWORD*)RwRasterLock(pTexture->raster, 0, rwRASTERLOCKREAD);
	{
		::fwrite(pBitsSrc, sizeof(DWORD) * RwTexW * RwTexH, 1, pFile);
	}
	RwRasterUnlock(pTexture->raster);

	NTL_RETURN(TRUE);
}

RwBool CNtlPLResourceManager::CopyRaster(RwTexture *pSrcTex, RwTexture *pDstTex)
{
	NTL_FUNCTION("CNtlPLResourceManager::CopyRaster");

	if(!pSrcTex || !pDstTex)
		NTL_RETURN(FALSE);

	RwInt32 RwTexW = RwRasterGetWidth(pSrcTex->raster);
	RwInt32 RwTexH = RwRasterGetHeight(pSrcTex->raster);

	DWORD *pBitsSrc = (DWORD*)RwRasterLock(pSrcTex->raster, 0, rwRASTERLOCKREAD);
	DWORD *pBitsDst = (DWORD*)RwRasterLock(pDstTex->raster, 0, rwRASTERLOCKWRITE);
	{
		// RW uses vertex alpha & texture alpha at the same time.
		// alpha bit should be considered when using vertex alpha.
		for(int i = 0; i < RwTexW * RwTexH; ++i)
		{
			*(pBitsDst + i) = 0xff000000 | *(pBitsSrc + i);
		}

		// this's wrong.
		//::CopyMemory(pBitsDst, pBitsSrc, sizeof(DWORD) * RwTexW * RwTexH);
	}
	RwRasterUnlock(pSrcTex->raster);
	RwRasterUnlock(pDstTex->raster);

	NTL_RETURN(TRUE);
}

RwBool CNtlPLResourceManager::SetColor(RwTexture* _pTexture, DWORD _FillColor)
{
	if(!_pTexture)
		return FALSE;

	RwInt32 RwTexW = RwRasterGetWidth(_pTexture->raster);
	RwInt32 RwTexH = RwRasterGetHeight(_pTexture->raster);
	DWORD* pDest;
	DWORD* pBits;

	for(RwInt32 i = 0; i < RwTexW * RwTexH; ++i)
	{
		pBits = (DWORD*)RwRasterLock(_pTexture->raster, 0, rwRASTERLOCKWRITE); 
		{
			pDest = pBits + i;
			(*pDest) = _FillColor;
		}

		RwRasterUnlock(_pTexture->raster);
	}

	return TRUE;
}

RwBool CNtlPLResourceManager::SetColor(RwTexture *pTexture, RwReal OpaLvl, POINT& TexelPos)
{
	if(!pTexture)
		return FALSE;

	RwInt32 RwTexW = RwRasterGetWidth(pTexture->raster);
	RwInt32 RwTexH = RwRasterGetHeight(pTexture->raster);

	RwInt32 PosX = static_cast<RwInt32>(TexelPos.x);
	RwInt32 PosY = static_cast<RwInt32>(TexelPos.y);

	if (PosX < 0 || PosX >= RwTexW || PosY < 0 || PosY >= RwTexH)
		return FALSE;

// 	if(PosX < 0 || PosY >= RwTexW * RwTexH)
// 		return FALSE;

	DWORD *pDest;
	DWORD CurTexel;
	DWORD OpaVal = (DWORD)(OpaLvl * 255.0f);
	DWORD *pBits = (DWORD*)RwRasterLock(pTexture->raster, 0, rwRASTERLOCKWRITE | rwRASTERLOCKREAD);
	{
		pDest		= pBits + RwTexW * PosY + PosX;
		CurTexel	= (*pDest);

		DWORD A	= (DWORD)((CurTexel & 0xff000000) >> 24);
		DWORD R	= (DWORD)((CurTexel & 0x00ff0000) >> 16);
		DWORD G	= (DWORD)((CurTexel & 0x0000ff00) >> 8);
		DWORD B	= (DWORD)((CurTexel & 0x000000ff) >> 0);

		A += OpaVal;
		R += OpaVal;
		G += OpaVal;
		B += OpaVal;
		CLAMP(A, 0, 255);
		CLAMP(R, 0, 255);
		CLAMP(G, 0, 255);
		CLAMP(B, 0, 255);
	
		CurTexel = (DWORD)(0xff000000 | (BYTE)(R) << 16 | (BYTE)(G) << 8 | (BYTE)(B) << 0);
		(*pDest) = CurTexel;
	}
	RwRasterUnlock(pTexture->raster);

	return TRUE;
}

RwBool CNtlPLResourceManager::SaveAlphaBits(RwTexture *pTexture, FILE* pFile)
{
	NTL_FUNCTION("CNtlPLResourceManager::SaveAlphaBits");

	if(!pTexture || !pFile)
		NTL_RETURN(FALSE);

	RwInt32 RwTexW = RwRasterGetWidth(pTexture->raster);
	RwInt32 RwTexH = RwRasterGetHeight(pTexture->raster);

	DWORD *pDest;
	DWORD Temp;
	RwUInt8 CurAlphaValue;
	DWORD *pBits = (DWORD*)RwRasterLock(pTexture->raster, 0, rwRASTERLOCKREAD);
	{
		for(RwInt32 PosY = 0; PosY < RwTexH; ++PosY)
			for(RwInt32 PosX = 0; PosX < RwTexW; ++PosX)
			{
				pDest = pBits + RwTexW * PosY + PosX;
				Temp = (*pDest);
				CurAlphaValue = static_cast<BYTE>(Temp >> 24);
				fwrite(&CurAlphaValue, sizeof(BYTE), 1, pFile);
			}
	}
	RwRasterUnlock(pTexture->raster);

	NTL_RETURN(TRUE);
}

RwBool CNtlPLResourceManager::SetColor(RwTexture *pTexture, RwInt32 Value, RwV2d Pos)
{
	NTL_FUNCTION("CNtlPLResourceManager::SetColor");

	if(!pTexture)
		NTL_RETURN(FALSE);

	RwInt32 RwTexW = RwRasterGetWidth(pTexture->raster);
	RwInt32 RwTexH = RwRasterGetHeight(pTexture->raster);

	RwInt32 PosX = static_cast<RwInt32>(Pos.x);
	RwInt32 PosY = static_cast<RwInt32>(Pos.y);

	if (PosX < 0 || PosX >= RwTexW || PosY < 0 || PosY >= RwTexH)
		return FALSE;

// 	if(PosX < 0 || PosY >= RwTexW * RwTexH)
// 		NTL_RETURN(FALSE);

	DWORD *pDest;
	DWORD *pBits = (DWORD*)RwRasterLock(pTexture->raster, 0, rwRASTERLOCKWRITE); 
	{
		pDest = pBits + RwTexW * PosY + PosX;
		(*pDest) = (DWORD)Value;
	}
	RwRasterUnlock(pTexture->raster);

	NTL_RETURN(TRUE);
}

RwBool CNtlPLResourceManager::SetAlphaValueDec(RwTexture *pTexture, RwInt32 Value, RwV2d Pos)
{
	NTL_FUNCTION("CNtlPLResourceManager::SetAlphaValue");

	if(!pTexture)
		NTL_RETURN(FALSE);

	RwInt32 RwTexW = RwRasterGetWidth(pTexture->raster);
	RwInt32 RwTexH = RwRasterGetHeight(pTexture->raster);

	RwInt32 PosX = static_cast<RwInt32>(Pos.x);
	RwInt32 PosY = static_cast<RwInt32>(Pos.y);

	if (PosX < 0 || PosX >= RwTexW || PosY < 0 || PosY >= RwTexH)
		return FALSE;
	
// 	if(PosX < 0 || PosY >= RwTexW * RwTexH)
// 		NTL_RETURN(FALSE);

	DWORD *pDest;
	DWORD *pBits = (DWORD*)RwRasterLock(pTexture->raster, 0, rwRASTERLOCKWRITE); 
	{
		pDest = pBits + RwTexW * PosY + PosX;
		(*pDest) &= 0xff000000;

		DWORD Temp = (*pDest);
		int Old = (Temp >> 24);
		int New = Value;
		int Res = Old - New;
		if(Res < 0)
			(*pDest) = 0x00000000;
		else
			(*pDest) -= ((DWORD)Value << 24);
	}
	RwRasterUnlock(pTexture->raster);

	NTL_RETURN(TRUE);
}

RwBool CNtlPLResourceManager::SetAlphaValueInc(RwTexture *pTexture, RwInt32 Value, RwV2d Pos)
{
	NTL_FUNCTION("CNtlPLResourceManager::SetAlphaValue");

	if(!pTexture)
		NTL_RETURN(FALSE);

	RwInt32 RwTexW = RwRasterGetWidth(pTexture->raster);
	RwInt32 RwTexH = RwRasterGetHeight(pTexture->raster);

	RwInt32 PosX = static_cast<RwInt32>(Pos.x);
	RwInt32 PosY = static_cast<RwInt32>(Pos.y);

	if (PosX < 0 || PosX >= RwTexW || PosY < 0 || PosY >= RwTexH)
		return FALSE;

// 	if(PosX < 0 || PosY >= RwTexW * RwTexH)
// 		NTL_RETURN(FALSE);

	DWORD *pDest;
	DWORD *pBits = (DWORD*)RwRasterLock(pTexture->raster, 0, rwRASTERLOCKWRITE); 
	{
		pDest = pBits + RwTexW * PosY + PosX;
		(*pDest) &= 0xff000000;

		DWORD Temp = (*pDest);
		if(static_cast<RwInt32>(Temp >> 24) + Value > 255 || static_cast<RwInt32>(Temp >> 24) + Value < 0)
			(*pDest) = 0xff000000;
		else
			(*pDest) += ((DWORD)Value << 24);
	}
	RwRasterUnlock(pTexture->raster);

	NTL_RETURN(TRUE);
}


RwBool CNtlPLResourceManager::IsEmptyLoadScheduling(void)
{
	return m_pScheduleManager->IsEmptyLoadScheduling((RwUInt8)PLENTITY_OBJECT);
}

RwReal CNtlPLResourceManager::GetAlphaAverageDensity(BYTE* _pAlphaBits, RwInt32 _Cnt)
{
	RwInt32 Cnt				= _Cnt;
	RwReal	AverageDensity	= 0.0f;
	RwReal	Ret;

	while(Cnt--)
	{
		AverageDensity += _pAlphaBits[Cnt];
	}

	Ret = AverageDensity / (static_cast<RwReal>(_Cnt) * 255.0f) * 100.0f;

	return Ret;
}

RwReal CNtlPLResourceManager::GetAlphaAverageDensity(RwTexture* _pTex)
{
	RwInt32 RwTexW	= RwRasterGetWidth(_pTex->raster);
	RwInt32 RwTexH	= RwRasterGetHeight(_pTex->raster);
	RwReal  Average = 0;
	DWORD*	pDest;
	DWORD	Temp;
	DWORD*	pBits;
	RwReal	Ret;
	
	pBits = (DWORD*)RwRasterLock(_pTex->raster, 0, rwRASTERLOCKREAD);
	{
		for(RwInt32 PosY = 0; PosY < RwTexH; ++PosY)
		{
			for(RwInt32 PosX = 0; PosX < RwTexW; ++PosX)
			{
				pDest	= pBits + RwTexW * PosY + PosX;
				Temp	= (*pDest);
				Average	+= static_cast<RwReal>(Temp >> 24);
			}
		}
	}
	RwRasterUnlock(_pTex->raster);

	Ret = Average / static_cast<RwReal>(RwTexH * RwTexW);

	return Ret;	
}
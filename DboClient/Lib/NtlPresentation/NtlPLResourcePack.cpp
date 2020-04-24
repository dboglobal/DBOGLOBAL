#include "precomp_ntlpresentation.h"

// shared 

#include "NtlPLResourcePack.h"

// core
#include "NtlDebug.h"
#include "NtlCoreUtil.h"
#include "NtlPackImage.h"



//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

CNtlPLResourcePack::CNtlPLResourcePack(int iUnknownFirstByte)
{
	m_piUnknownFirstByte = new int;
	*m_piUnknownFirstByte = iUnknownFirstByte;
}

CNtlPLResourcePack::~CNtlPLResourcePack()
{
	ClearHeader();
}

void CNtlPLResourcePack::ClearHeader(void)
{
	HMapPackHeader::iterator it;
	for(it = m_hmapPackHeader.begin(); it != m_hmapPackHeader.end(); it++)
	{
		NTL_DELETE( (*it).second );
	}

	m_hmapPackHeader.clear();
}

void CNtlPLResourcePack::RegistHeaderName(const RwChar *pFileName)
{
	m_strHeaderName = pFileName;
}

void CNtlPLResourcePack::RegistDataName(const RwChar *pFileName)
{
	m_strDataName = pFileName;
}


SFilePackHeaderElement*	CNtlPLResourcePack::FindHeader(std::string str)
{
	HMapPackHeader::iterator it = m_hmapPackHeader.find(str);
	if(it == m_hmapPackHeader.end())
		return NULL;

	return (*it).second;
}

SFilePackHeaderElement* CNtlPLResourcePack::FindHeader(const RwChar *pFileName)
{
	HMapPackHeader::iterator it = m_hmapPackHeader.find(pFileName);
	if(it == m_hmapPackHeader.end())
		return NULL;

	return (*it).second;
}

void CNtlPLResourcePack::AddHeader(RwUInt8 byUnit, const RwChar *pFileName, RwUInt32 uiSize, RwUInt32 uiOffset)
{
	if(!FindHeader(pFileName))
	{
		SFilePackHeaderElement *pHeader = NTL_NEW SFilePackHeaderElement;
		memset(pHeader, 0, sizeof(SFilePackHeaderElement));

		pHeader->byUnitKey	= byUnit;
		strcpy_s(pHeader->chName, NTL_FILEPACK_NAME_SIZE, pFileName);
		pHeader->uiSize		= uiSize;
		pHeader->uiOffset	= uiOffset;

		for(RwInt32 i = 0; i < NTL_FILEPACK_NAME_SIZE; i++)
			pHeader->chName[i] = tolower(pHeader->chName[i]);

		m_hmapPackHeader[pHeader->chName] = pHeader;	
	}
}

CNtlPLResourcePack::HMapPackHeader&	CNtlPLResourcePack::GetHeaders(void)
{
	return m_hmapPackHeader;
}

RwInt32	CNtlPLResourcePack::GetElementCount(void)
{
	return (RwInt32)m_hmapPackHeader.size();
}


void CNtlPLResourcePack::GetPackDataFileName(RwUInt8 byUnit, std::string& strPackDataFileName)
{
	static RwChar chBuffer[10];

	strPackDataFileName = m_strDataName;
	sprintf_s(chBuffer, 10, "%d", byUnit);
	strPackDataFileName += chBuffer;
	strPackDataFileName += ".pak";
}

RwInt32 CNtlPLResourcePack::LoadPack(const RwChar *pFileName)
{
	ClearHeader();

	CNtlFileSerializer s;
	
	bool bSuccess = s.LoadFile((char*)pFileName, true, NTL_PACK_CRYPT_KEY);
	if(!bSuccess)
		return NTL_FILEPACK_ERROR;

	SFilePackHeaderElement *pHeader;

	/*if (s.CheckOutBuffer(sizeof(int)))
	{
		s.Out(m_piUnknownFirstByte, sizeof(int));
	}*/

	while(!s.IsEmpty())
	{
		pHeader = NTL_NEW SFilePackHeaderElement;
        
        if(s.CheckOutBuffer(sizeof(SFilePackHeaderElement)))
        {
		    s.Out(pHeader, sizeof(SFilePackHeaderElement));
		    m_hmapPackHeader[pHeader->chName] = pHeader;
        }
        else
        {
			NTL_DELETE( pHeader );
        }
	}

	return NTL_FILEPACK_SUCCESS;
}

RwInt32 CNtlPLResourcePack::SavePack(const RwChar *pFileName)
{
	if(m_strHeaderName.size() == 0)
	{
		return NTL_FILEPACK_ERROR;
	}
	
	static char szPath[4096];
	static char szName[1024];
	static char szExt[128];
	static std::string strExt;

	_splitpath_s( pFileName, 0, 0, szPath, 4096, szName, 1024, szExt, 128 );
	strExt = szExt;

	if(strExt == "zip")
	{
		return NTL_FILEPACK_NOT_USEFILE;
	}

	DWORD dwFileAttribute = ::GetFileAttributes(pFileName);
	if(INVALID_FILE_ATTRIBUTES != dwFileAttribute)
	{
		if (dwFileAttribute & FILE_ATTRIBUTE_READONLY)
			return NTL_FILEPACK_FILEREADONLY;
	}

	CNtlFileSerializer s(1024*1024*5, 1024*1024*5);

	/*if (s.CheckOutBuffer(sizeof(int)))
	{
		s.In(m_piUnknownFirstByte, sizeof(int));
	}*/

	HMapPackHeader::iterator it;
	for(it = m_hmapPackHeader.begin(); it != m_hmapPackHeader.end(); it++)
	{
		s.In((*it).second, sizeof(SFilePackHeaderElement));
	}

	s.SaveFile((char*)pFileName, true, NTL_PACK_CRYPT_KEY);

	return NTL_FILEPACK_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

CNtlPLResoucePackManager::CNtlPLResoucePackManager()
{
	CreateAllPack();

	m_uiFlags		= 0;
	m_iMemSize		= 0;
	m_pMemBuffer	= NULL;

	m_pCallMakePackNotify = NULL;
}

CNtlPLResoucePackManager::~CNtlPLResoucePackManager()
{
	DeleteAllPack();

	NTL_ARRAY_DELETE( m_pMemBuffer );

	NTL_DELETE(m_pCallMakePackNotify);
}

CNtlPLResoucePackManager* CNtlPLResoucePackManager::GetInstance(void)
{
	static CNtlPLResoucePackManager ResPackMgr;
	return &ResPackMgr;
}

void CNtlPLResoucePackManager::SetActiveTypeFlags(RwUInt32 uiFlags)
{
	m_uiFlags = uiFlags;
}

void CNtlPLResoucePackManager::CreateAllPack(void)
{
	int nUnknown;

	for (RwInt32 i = 0; i < MAX_NTL_PACK_TYPE; i++)
	{
		switch (i)
		{
			case NTL_PACK_TYPE_TEXTURE: nUnknown = 3; break;
			case NTL_PACK_TYPE_OBJECT: nUnknown = 3; break;
			case NTL_PACK_TYPE_GUI: nUnknown = 3; break;
			case NTL_PACK_TYPE_FLASH: nUnknown = 5; break;
			case NTL_PACK_TYPE_SOUND: nUnknown = 5; break;
			case NTL_PACK_TYPE_LANGUAGE: nUnknown = 4; break;
			case NTL_PACK_TYPE_SCRIPT: nUnknown = 3; break;
			case NTL_PACK_TYPE_TABLE: nUnknown = 3; break;
			case NTL_PACK_TYPE_PROPERTY: nUnknown = 4; break;
			case NTL_PACK_TYPE_TERRAIN: nUnknown = 7; break;
		}

		m_pResPack[i] = NTL_NEW CNtlPLResourcePack(nUnknown);
	}

	CNtlPLResourcePack *pResPack; 

	// texture 
	pResPack = m_pResPack[NTL_PACK_TYPE_TEXTURE];
	pResPack->RegistHeaderName(NTL_PACKNAME_TEXTURE_HEADER);
	pResPack->RegistDataName(NTL_PACKNAME_TEXTURE_DATA);

	// obejct
	pResPack = m_pResPack[NTL_PACK_TYPE_OBJECT];
	pResPack->RegistHeaderName(NTL_PACKNAME_OBJECT_HEADER);
	pResPack->RegistDataName(NTL_PACKNAME_OBJECT_DATA);

	// terrain
	pResPack = m_pResPack[NTL_PACK_TYPE_TERRAIN];
	pResPack->RegistHeaderName(NTL_PACKNAME_TERRAIN_HEADER);
	pResPack->RegistDataName(NTL_PACKNAME_TERRAIN_DATA);

	// terrain
	pResPack = m_pResPack[NTL_PACK_TYPE_GUI];
	pResPack->RegistHeaderName(NTL_PACKNAME_GUI_HEADER);
	pResPack->RegistDataName(NTL_PACKNAME_GUI_DATA);

	// flash
	pResPack = m_pResPack[NTL_PACK_TYPE_FLASH];
	pResPack->RegistHeaderName( NTL_PACKNAME_FLASH_HEADER );
	pResPack->RegistDataName(NTL_PACKNAME_FLASH_DATA);

	// sound
	pResPack = m_pResPack[NTL_PACK_TYPE_SOUND];
	pResPack->RegistHeaderName( NTL_PACKNAME_SOUND_HEADER);
	pResPack->RegistDataName(NTL_PACKNAME_SOUND_DATA);

    // language
    pResPack = m_pResPack[NTL_PACK_TYPE_LANGUAGE];
    pResPack->RegistHeaderName(NTL_PACKNAME_LANGUAGE_HEADER);
    pResPack->RegistDataName(NTL_PACKNAME_LANGUAGE_DATA);

    // script 
    pResPack = m_pResPack[NTL_PACK_TYPE_SCRIPT];
    pResPack->RegistHeaderName(NTL_PACKNAME_SCRIPT_HEADER);
    pResPack->RegistDataName(NTL_PACKNAME_SCRIPT_DATA);

    // table
    pResPack = m_pResPack[NTL_PACK_TYPE_TABLE];
    pResPack->RegistHeaderName(NTL_PACKNAME_TABLE_HEADER);
    pResPack->RegistDataName(NTL_PACKNAME_TABLE_DATA);

    // Property
    pResPack = m_pResPack[NTL_PACK_TYPE_PROPERTY];
    pResPack->RegistHeaderName(NTL_PACKNAME_PROPERTY_HEADER);
    pResPack->RegistDataName(NTL_PACKNAME_PROPERTY_DATA);
}


void CNtlPLResoucePackManager::DeleteAllPack(void)
{
	for(RwInt32 i = 0; i < MAX_NTL_PACK_TYPE; i++)
	{
		NTL_DELETE( m_pResPack[i] );
	}
}

void CNtlPLResoucePackManager::GetAllFiles(std::string& strOffsetFolder, std::list<std::string>& listFiles)
{
	RwChar strCurPath[NTL_MAX_DIR_PATH];
	GetCurrentDirectory(NTL_MAX_DIR_PATH, strCurPath);

	std::string strFullFileName, strFileName, strCurrOffsetFolder;

	strCurrOffsetFolder = strOffsetFolder;

	WIN32_FIND_DATA FindFileData;
	HANDLE hFile = FindFirstFile("*.*", &FindFileData);
	if(hFile == INVALID_HANDLE_VALUE) 
		return;

	do 
	{
		strFullFileName		= strCurPath;
		strFullFileName		+= "\\";
		strFullFileName		+= FindFileData.cFileName;

		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//  디렉토리이면
			if(FindFileData.cFileName[0] == '.')
			{
				//  Dot 디렉토리는 스킵한다.
			} 
			else
			{
				SetCurrentDirectory(strFullFileName.c_str());

				strOffsetFolder = strCurrOffsetFolder;

				strOffsetFolder += "\\";
				strOffsetFolder += FindFileData.cFileName;

				GetAllFiles(strOffsetFolder, listFiles);
			}
		} 
		else
		{
			strFileName = strCurrOffsetFolder;
			strFileName += "\\";
			strFileName += FindFileData.cFileName;

			listFiles.push_back(strFileName.c_str());
		}
	} while(FindNextFile(hFile, &FindFileData));

	FindClose(hFile);

	strOffsetFolder = strCurrOffsetFolder;
}

RwInt32 CNtlPLResoucePackManager::SavePackFile(CNtlPLResourcePack *pResPack, std::list<std::string>& listFiles, const RwChar *pPackDataFileName)
{
	if(listFiles.size() == 0)
		return NTL_FILEPACK_ERROR;

	RwInt32 iPackDataSerial = 0;
	RwUInt32 uiDataOffset = 0;
	RwUInt64 uiFileSize = 0;
	RwChar *pMem = 0;
	RwUInt32 uiTotalFileNum = (RwUInt32)listFiles.size();
	RwUInt32 uiProcFileNum = 0;


	FILE *fp = NULL;
	std::string strPackFileName;
	RwChar chSerialBuffer[10];

	// file open
	strPackFileName = NTL_PACK_FILE_PATH;
	strPackFileName += pPackDataFileName;
	sprintf_s(chSerialBuffer, 10,"%d", iPackDataSerial);
	strPackFileName += chSerialBuffer;
	strPackFileName += ".pak"; 

	WIN32_FIND_DATA FindFileData;
	HANDLE hFile = ::FindFirstFile(strPackFileName.c_str(), &FindFileData);
	if(hFile != INVALID_HANDLE_VALUE) 
	{
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
			return NTL_FILEPACK_FILEREADONLY;
	}

	if(fopen_s(&fp, strPackFileName.c_str(), "wb") != 0)
		return NTL_FILEPACK_CREATEFAIL;

	std::list<std::string>::iterator it;
	for(it = listFiles.begin(); it != listFiles.end(); it++)
	{
		pMem = DumpFile((*it).c_str(), uiFileSize);

		//// pack limit size CHECK
		if(uiDataOffset > 0 && uiDataOffset + uiFileSize > NTL_MAX_FILEPACK_SIZE)
		{
			if(fp)
			{
				fclose(fp);
				fp = NULL;
			}

			uiDataOffset	= 0;
			iPackDataSerial++;

			strPackFileName = NTL_PACK_FILE_PATH;
			strPackFileName += pPackDataFileName;
			sprintf_s(chSerialBuffer, 10,"%d", iPackDataSerial);
			strPackFileName += chSerialBuffer;
			strPackFileName += ".pak"; 

			WIN32_FIND_DATA FindFileData2;
			HANDLE hFile2 = ::FindFirstFile(strPackFileName.c_str(), &FindFileData2);
			if(hFile2 != INVALID_HANDLE_VALUE) 
			{
				if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
				{
					return NTL_FILEPACK_FILEREADONLY;
				}
			}

			if(fopen_s(&fp, strPackFileName.c_str(), "wb") != 0)
			{
				return NTL_FILEPACK_CREATEFAIL;
			}
		}

		if(uiFileSize > 0)
		{
			fwrite(pMem, uiFileSize, 1, fp);

			pResPack->AddHeader(iPackDataSerial, (*it).c_str(), uiFileSize, uiDataOffset);

			uiDataOffset += uiFileSize;

			if(m_pCallMakePackNotify)
			{
				uiProcFileNum++;
				RwUInt32 uiPercent = (RwUInt32)(((RwReal)uiProcFileNum/(RwReal)uiTotalFileNum) * 100.0f);
				m_pCallMakePackNotify->Call((unsigned int)(*it).c_str(), uiPercent);
			}
		}
	}

	if(fp)
	{
		fclose(fp);
		fp = NULL;
	}

	return NTL_FILEPACK_SUCCESS;
}

RwChar* CNtlPLResoucePackManager::DumpFile(const char *pFileName, RwUInt64& uiSize)
{
	FILE *fp = NULL;

	if(fopen_s(&fp, pFileName, "rb") != 0)
	{
		uiSize = 0;
		return NULL;
	}

	fseek(fp, 0, SEEK_END);

	uiSize = (RwUInt64)ftell(fp);

	fseek(fp, 0, SEEK_SET);

	CheckMemoryBuffer(uiSize);

	fread(m_pMemBuffer, uiSize, 1, fp);

	fclose(fp);

	return m_pMemBuffer;
}


RwInt32 CNtlPLResoucePackManager::SavePack(RwUInt8 byPackType, std::list<std::string>& listFolder, const RwChar *pPackHeaderFileName, const RwChar *pPackDataFileName)
{
	CNtlPLResourcePack *pResPack = m_pResPack[byPackType];

	pResPack->ClearHeader();

	//----------------------------------------------------
	// 현재의 directory를 설정한다.
	RwChar strCurPath[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, strCurPath);

	RwBool bRet;
	std::string strFullFileName;
	std::list<std::string> listFiles;

	//----------------------------------------------------
	// texture

	std::string strOffsetFolder;

	std::list<std::string>::iterator it;
	for(it = listFolder.begin(); it != listFolder.end(); it++)
	{
		strOffsetFolder = (*it);

		strFullFileName = strCurPath;
		strFullFileName += strOffsetFolder;
		bRet = ::SetCurrentDirectory(strFullFileName.c_str());
		if(!bRet)
		{
			SetCurrentDirectory(strCurPath);
			return NTL_FILEPACK_FILENOTFOUND;
		}

		GetAllFiles(strOffsetFolder, listFiles);
	}
	
	SetCurrentDirectory(strCurPath);

	//----------------------------------------------------
	// save pack data

	RwInt32 iSaveRes = SavePackFile(pResPack, listFiles, pPackDataFileName);

	if(iSaveRes != NTL_FILEPACK_SUCCESS)
		return iSaveRes;

	//----------------------------------------------------
	// save pack header
	std::string str;
	str = NTL_PACK_FILE_PATH;
	str += pPackHeaderFileName;

	return pResPack->SavePack(str.c_str());
}

RwInt32 CNtlPLResoucePackManager::SaveTexturePack(void)
{
	std::list<std::string> listFolder;
	listFolder.push_back(NTL_FOLDER_TEXTURE);

	return SavePack(NTL_PACK_TYPE_TEXTURE, listFolder, NTL_PACKNAME_TEXTURE_HEADER, NTL_PACKNAME_TEXTURE_DATA);
}


RwInt32 CNtlPLResoucePackManager::SaveObjectPack(void)
{
	std::list<std::string> listFolder;
	listFolder.push_back(NTL_FOLDER_OBJECT01);
	listFolder.push_back(NTL_FOLDER_OBJECT02);
	listFolder.push_back(NTL_FOLDER_OBJECT03);
	listFolder.push_back(NTL_FOLDER_OBJECT04);
	listFolder.push_back(NTL_FOLDER_OBJECT05);

	return SavePack(NTL_PACK_TYPE_OBJECT, listFolder, NTL_PACKNAME_OBJECT_HEADER, NTL_PACKNAME_OBJECT_DATA);
}

RwInt32 CNtlPLResoucePackManager::SaveTerrainPack(void)
{
	std::list<std::string> listFolder;
	listFolder.push_back(NTL_FOLDER_TERRAIN);

	return SavePack(NTL_PACK_TYPE_TERRAIN, listFolder, NTL_PACKNAME_TERRAIN_HEADER, NTL_PACKNAME_TERRAIN_DATA);
}

RwInt32 CNtlPLResoucePackManager::SaveGuiPack(void)
{
	std::list<std::string> listFolder;
	listFolder.push_back(NTL_FOLDER_GUI);

	return SavePack(NTL_PACK_TYPE_GUI, listFolder, NTL_PACKNAME_GUI_HEADER, NTL_PACKNAME_GUI_DATA);
}

RwInt32 CNtlPLResoucePackManager::SaveFlashPack(void)
{
	std::list<std::string> listFolder;
	listFolder.push_back(NTL_FOLDER_FLASH);

	return SavePack(NTL_PACK_TYPE_FLASH, listFolder, NTL_PACKNAME_FLASH_HEADER, NTL_PACKNAME_FLASH_DATA);
}

RwInt32 CNtlPLResoucePackManager::SaveSoundPack(void)
{
	std::list<std::string> listFolder;
	listFolder.push_back(NTL_FOLDER_SOUND);

	return SavePack(NTL_PACK_TYPE_SOUND, listFolder, NTL_PACKNAME_SOUND_HEADER, NTL_PACKNAME_SOUND_DATA);
}

RwInt32 CNtlPLResoucePackManager::SaveLanguagePack( void ) 
{
    std::list<std::string> listFoler;
    listFoler.push_back(NTL_FOLDER_LANGUAGE);

    return SavePack(NTL_PACK_TYPE_LANGUAGE, listFoler, NTL_PACKNAME_LANGUAGE_HEADER, NTL_PACKNAME_LANGUAGE_DATA);
}


RwInt32 CNtlPLResoucePackManager::SaveScriptPack( void ) 
{
    std::list<std::string> listFolder;
    listFolder.push_back(NTL_FOLDER_SCRIPT);

    return SavePack(NTL_PACK_TYPE_SCRIPT, listFolder, NTL_PACKNAME_SCRIPT_HEADER, NTL_PACKNAME_SCRIPT_DATA);
}

RwInt32 CNtlPLResoucePackManager::SaveTablePack( void ) 
{
    std::list<std::string> listFoler;
    listFoler.push_back(NTL_FOLDER_TABLE);

    return SavePack(NTL_PACK_TYPE_TABLE, listFoler, NTL_PACKNAME_TABLE_HEADER, NTL_PACKNAME_TABLE_DATA);
}

RwInt32 CNtlPLResoucePackManager::SavePropertyPack( void ) 
{
    std::list<std::string> listFolder;
    listFolder.push_back(NTL_FOLDER_PROPERTY);

    return SavePack(NTL_PACK_TYPE_PROPERTY, listFolder, NTL_PACKNAME_PROPERTY_HEADER, NTL_PACKNAME_PROPERTY_DATA);
}

RwChar*	CNtlPLResoucePackManager::UnPackLeadBuffer(RwChar* pBuffer, const RwChar *pPackFileName, SFilePackHeaderElement *pElement)
{
	FILE *fp = NULL;

	if(fopen_s(&fp, pPackFileName, "rb") != 0)
		return NULL;

	fseek(fp, pElement->uiOffset, SEEK_SET);

	fread(pBuffer, pElement->uiSize, 1, fp);

	fclose(fp);

	return pBuffer;
}

void CNtlPLResoucePackManager::UnPackSaveBuffer(const RwChar *pFileName, const RwChar *pBuffer, SFilePackHeaderElement *pElement)
{
    // 읽기 전용도 덮어쓸수 있도록 속성을 변환한다.
    ::SetFileAttributes(pFileName, FILE_ATTRIBUTE_NORMAL);

	FILE *fp = NULL;

	if(fopen_s(&fp, pFileName, "wb") != 0)
		return;

	fwrite(pBuffer, pElement->uiSize, 1, fp);

	fclose(fp);
}

void CNtlPLResoucePackManager::UnPack(const RwChar *pFileName, const RwChar *pPackFileName, SFilePackHeaderElement *pElement, RwUInt32 uiPercent)
{
	HANDLE hFile;
	WIN32_FIND_DATA finddata;

	RwChar chBuffer[1024];
	memset(chBuffer, 0, 1024); 

	int nLen = strlen(pFileName);
	OutputDebugString(pFileName);
	for(RwInt32 i = 0; i < nLen; i++)
	{	
		if(pFileName[i] == '\\' && i > 0 && pFileName[i-1] != '.')
		{
			memcpy(chBuffer, pFileName, i);
			chBuffer[i+1] = 0;

			hFile = ::FindFirstFile(chBuffer, &finddata);
			if(hFile == INVALID_HANDLE_VALUE || !(finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				::CreateDirectory(chBuffer, NULL);		
		}
	}
	
	std::string strLeadPack = NTL_PACK_FILE_PATH;
	strLeadPack += pPackFileName;

    RwChar* pBuffer = NTL_NEW RwChar[pElement->uiSize];
    ZeroMemory(pBuffer, sizeof(RwChar) * pElement->uiSize);
	
	if(UnPackLeadBuffer(pBuffer, strLeadPack.c_str(), pElement))
	{
		UnPackSaveBuffer(pFileName, pBuffer, pElement);

		if(m_pCallMakePackNotify)
		{
			m_pCallMakePackNotify->Call((unsigned int)pFileName, uiPercent);
		}
	}

    NTL_ARRAY_DELETE(pBuffer);
}

RwInt32 CNtlPLResoucePackManager::LoadPackHeader(void)
{
    if(m_pCallMakePackNotify)
    {
        m_pCallMakePackNotify->Call((unsigned int)"Load Pack Header...", 0);
    }

	DeleteAllPack();
	CreateAllPack();

	LoadPack(NTL_PACK_TYPE_TEXTURE,	NTL_PACKNAME_TEXTURE_HEADER);
	LoadPack(NTL_PACK_TYPE_OBJECT,	NTL_PACKNAME_OBJECT_HEADER);
	LoadPack(NTL_PACK_TYPE_TERRAIN,	NTL_PACKNAME_TERRAIN_HEADER);
	LoadPack(NTL_PACK_TYPE_GUI,		NTL_PACKNAME_GUI_HEADER);
	LoadPack(NTL_PACK_TYPE_FLASH,	NTL_PACKNAME_FLASH_HEADER);
	LoadPack(NTL_PACK_TYPE_SOUND,	NTL_PACKNAME_SOUND_HEADER);
	LoadPack(NTL_PACK_TYPE_LANGUAGE, NTL_PACKNAME_LANGUAGE_HEADER);    
    LoadPack(NTL_PACK_TYPE_SCRIPT, NTL_PACKNAME_SCRIPT_HEADER);
    LoadPack(NTL_PACK_TYPE_TABLE, NTL_PACKNAME_TABLE_HEADER);
    LoadPack(NTL_PACK_TYPE_PROPERTY, NTL_PACKNAME_PROPERTY_HEADER);

	return NTL_FILEPACK_SUCCESS;
}

RwInt32 CNtlPLResoucePackManager::LoadPack(RwUInt8 byPackType, const RwChar *pPackHeaderFileName)
{
	CNtlPLResourcePack *pResPack = m_pResPack[byPackType];

	if(pResPack == NULL)
		return NTL_FILEPACK_ERROR; 

	std::string str;
	str = NTL_PACK_FILE_PATH;
	str += pPackHeaderFileName;

	return pResPack->LoadPack(str.c_str());
}


RwInt32 CNtlPLResoucePackManager::SavePack(RwUInt8 byPackType)
{
	if(m_pCallMakePackNotify)
	{
		m_pCallMakePackNotify->Call((unsigned int)"Pack file ready !!!", 0);
	}

	switch(byPackType)
	{
	case NTL_PACK_TYPE_TEXTURE:
		return SaveTexturePack();
	case NTL_PACK_TYPE_OBJECT:
		return SaveObjectPack();
	case NTL_PACK_TYPE_TERRAIN:
		return SaveTerrainPack();
	case NTL_PACK_TYPE_GUI:
		return SaveGuiPack();
	case NTL_PACK_TYPE_FLASH:
		return SaveFlashPack();
	case NTL_PACK_TYPE_SOUND:
		return SaveSoundPack();
    case NTL_PACK_TYPE_LANGUAGE:
        return SaveLanguagePack();
    case NTL_PACK_TYPE_SCRIPT:
        return SaveScriptPack();
    case NTL_PACK_TYPE_TABLE:
        return SaveTablePack();
    case NTL_PACK_TYPE_PROPERTY:
        return SavePropertyPack();
	}

	return NTL_FILEPACK_ERROR;
}


RwInt32	CNtlPLResoucePackManager::UnPack(void)
{
	if(m_pCallMakePackNotify)
	{
		m_pCallMakePackNotify->Call((unsigned int)"Pack file loading !!!", 0);
	}

	LoadPackHeader();

	RwInt32 iCurrNum = 0;
	RwInt32 iTotNum = 0;	

	std::string str;
	std::string strPackFile;

	SFilePackHeaderElement *pElement;

	for(RwInt32 i = 0; i < MAX_NTL_PACK_TYPE; i++)
	{
		if(m_pResPack[i])
			iTotNum += m_pResPack[i]->GetElementCount();
	}

	for(RwInt32 i = 0; i < MAX_NTL_PACK_TYPE; i++)
	{
		if(m_pResPack[i])
		{
			CNtlPLResourcePack::HMapPackHeader::iterator it;
			CNtlPLResourcePack::HMapPackHeader& mapPackHeader = m_pResPack[i]->GetHeaders();

			for(it = mapPackHeader.begin(); it != mapPackHeader.end(); it++)
			{
				str = (*it).first;
				pElement = (*it).second;

				m_pResPack[i]->GetPackDataFileName(pElement->byUnitKey, strPackFile);

				RwUInt32 uiPercent = (RwUInt32)(((RwReal)iCurrNum/(RwReal)iTotNum) * 100.0f);
				UnPack(str.c_str(), strPackFile.c_str(), pElement, uiPercent);

				iCurrNum++;
			}
		}
	}

	return NTL_FILEPACK_SUCCESS;
}

RwInt32 CNtlPLResoucePackManager::UnPack( EFilePackType ePackType ) 
{
    if(m_pCallMakePackNotify)
    {
        m_pCallMakePackNotify->Call((unsigned int)"Pack file loading !!!", 0);
    }

    RwInt32 iCurrNum = 0;
    RwInt32 iTotNum = 0;	

    std::string str, strPackFile;    

    SFilePackHeaderElement *pElement;

    if(m_pResPack[ePackType])
        iTotNum += m_pResPack[ePackType]->GetElementCount();
    
    if(m_pResPack[ePackType])
    {
        CNtlPLResourcePack::HMapPackHeader::iterator it;
        CNtlPLResourcePack::HMapPackHeader& mapPackHeader = m_pResPack[ePackType]->GetHeaders();

        for(it = mapPackHeader.begin(); it != mapPackHeader.end(); it++)
        {
            str = (*it).first;
            pElement = (*it).second;
			
            m_pResPack[ePackType]->GetPackDataFileName(pElement->byUnitKey, strPackFile);

            RwUInt32 uiPercent = (RwUInt32)(((RwReal)iCurrNum/(RwReal)iTotNum) * 100.0f);
            UnPack(str.c_str(), strPackFile.c_str(), pElement, uiPercent);

            iCurrNum++;
        }
    }

    return NTL_FILEPACK_SUCCESS;
}

CNtlPLResourcePack* CNtlPLResoucePackManager::FindResourcePack(RwUInt8 byPackType)
{
	return m_pResPack[byPackType];
}

SFilePackHeaderElement* CNtlPLResoucePackManager::FindHeaderElement(RwUInt8 byPackType, const RwChar *pFileName)
{
	if(m_pResPack[byPackType])
		return m_pResPack[byPackType]->FindHeader(pFileName);

	return NULL;
}

void CNtlPLResoucePackManager::FilterPathBuffer(const RwChar *pPath, const RwChar *pFileName)
{
	RwInt32 iOffset = 0;
	RwInt32 iLen = 0;

	// 확장자 filter 만들기.
	iLen = (RwInt32)strlen(pFileName);
	for(RwInt32 i = 0; i < iLen; i++)
	{
		m_chFileBuffer[iOffset] = tolower(pFileName[i]);

		if(m_chFileBuffer[iOffset] == '/')
			m_chFileBuffer[iOffset] = '\\';

		if(m_chFileBuffer[iOffset] == '\\' && iOffset > 0)
		{
			if(m_chFileBuffer[iOffset-1] != '\\')
				iOffset++;
		}
		else
			iOffset++;
	}

	m_chFileBuffer[iOffset] = 0;

	if(pPath == NULL)
		return;

	// path를 소문자로 만들기.
	m_iCurrPathNum = 0;
	iOffset = 0;
	iLen = (RwInt32)strlen(pPath);
	for(RwInt32 i = 0; i < iLen; i++)
	{
		if(pPath[i] == ';')
		{
			m_chPathBuffer[m_iCurrPathNum][iOffset] = 0;
			iOffset = 0;
			m_iCurrPathNum++;
		}

		m_chPathBuffer[m_iCurrPathNum][iOffset] = tolower(pPath[i]);
		if(m_chPathBuffer[m_iCurrPathNum][iOffset] == '/')
			m_chPathBuffer[m_iCurrPathNum][iOffset] = '\\';

		if(m_chPathBuffer[m_iCurrPathNum][iOffset] == '\\')
		{
			if(iOffset > 0)
				if(m_chPathBuffer[m_iCurrPathNum][iOffset-1] != '\\')
					iOffset++;
		}
		else
			iOffset++;
	}

	m_chPathBuffer[m_iCurrPathNum][iOffset] = 0;
}


void CNtlPLResoucePackManager::CheckMemoryBuffer(RwInt32 iSize)
{
	if(iSize > m_iMemSize)
	{
		m_iMemSize = iSize;

		NTL_ARRAY_DELETE( m_pMemBuffer );

		m_pMemBuffer = NTL_NEW RwChar [m_iMemSize];
	}
}


RwTexture* CNtlPLResoucePackManager::LoadTexture(const RwChar *pPath, const RwChar *pFileName, const RwChar *pMaskName)
{
	if(m_uiFlags == 0)
		return NULL;

	FilterPathBuffer(pPath, pFileName);

	// find header.

#define PACK_TEXUTRE_EXT_PNG	0
#define PACK_TEXUTRE_EXT_BMP	1
#define PACK_TEXUTRE_EXT_DDS	2

	static RwChar chExt[3][5] = {".png", ".bmp", ".dds" };

	SFilePackHeaderElement *pElement = NULL;
	CNtlPLResourcePack *pResPack = m_pResPack[NTL_PACK_TYPE_TEXTURE];
	RwUInt8 byExtType = 0;
	
	for(RwInt32 j = 0; j <= m_iCurrPathNum; j++)
	{
		RwChar *pPathRoot = rwstrstr(m_chPathBuffer[j], "texture");
		if (pPathRoot == NULL)
		{
			DBO_WARNING_MESSAGE("1 Load texture: " << pFileName << " path " << pPath);
			return NULL;
		}

		m_iBufferSize	= 0;
		byExtType		= 0;

		m_chFullBuffer[m_iBufferSize++] = '.';
		m_chFullBuffer[m_iBufferSize++] = '\\';

		strcpy_s(&m_chFullBuffer[m_iBufferSize], NTL_FILEPACK_BUFFER_SIZE - m_iBufferSize, pPathRoot);
		m_iBufferSize += strlen(pPathRoot);

		strcpy_s(&m_chFullBuffer[m_iBufferSize], NTL_FILEPACK_BUFFER_SIZE - m_iBufferSize, m_chFileBuffer);
		m_iBufferSize += strlen(m_chFileBuffer);
			
		RwChar *extender  = rwstrrchr(m_chFileBuffer, '.');
		if( extender != NULL )
		{
			m_chFullBuffer[m_iBufferSize] = 0;

			for(RwInt32 i = 0; i < 3; i++)
			{
				if(_stricmp(extender, chExt[i]) == 0)
				{
					byExtType = i;
					break;
				}
			}

			pElement = pResPack->FindHeader(m_chFullBuffer);
			if(pElement)
				goto ResourcPackTextureLoad_Exit;
		}
		else
		{
			RwInt32 iTempOffset = m_iBufferSize;

			for(RwInt32 i = 0; i < 3; i++)
			{
				m_iBufferSize = iTempOffset;
				strcpy_s(&m_chFullBuffer[m_iBufferSize], NTL_FILEPACK_BUFFER_SIZE - m_iBufferSize, chExt[i]);
				m_iBufferSize += strlen(chExt[i]);
				m_chFullBuffer[m_iBufferSize] = 0;

				pElement = pResPack->FindHeader(m_chFullBuffer);
				if(pElement)
				{
					byExtType = i;
					goto ResourcPackTextureLoad_Exit;
				}
			}
		}
	}

ResourcPackTextureLoad_Exit:

	if (pElement == NULL)
	{
		DBO_WARNING_MESSAGE("2 Load texture: " << pFileName << " path " << pPath);
		return NULL;
	}

	std::string strPackFileName;
	m_pResPack[NTL_PACK_TYPE_TEXTURE]->GetPackDataFileName(pElement->byUnitKey, strPackFileName);

	m_iBufferSize = 0;
	strcpy_s(&m_chFullBuffer[m_iBufferSize], NTL_FILEPACK_BUFFER_SIZE, NTL_PACK_FILE_PATH);
	m_iBufferSize += strlen(NTL_PACK_FILE_PATH);
	
	strcpy_s(&m_chFullBuffer[m_iBufferSize], NTL_FILEPACK_BUFFER_SIZE-m_iBufferSize, strPackFileName.c_str());
	m_iBufferSize += strPackFileName.size();
	
	m_chFullBuffer[m_iBufferSize] = 0;

	RwTexture *pTexture = NULL;

	SNtlPackImage sPackImage;
	sPackImage.pName				= pFileName;
	sPackImage.pMaskName			= pMaskName;
	sPackImage.strPackFileName		= m_chFullBuffer;
	sPackImage.uiPackOffset			= pElement->uiOffset;
	sPackImage.uiPackSize			= pElement->uiSize;


	if(byExtType == PACK_TEXUTRE_EXT_DDS)
		pTexture = Ntl_D3D9DDSTextureRead(&sPackImage);
	else if(byExtType == PACK_TEXUTRE_EXT_PNG)
		pTexture = Ntl_PNGTextureRead(&sPackImage);
	else if(byExtType == PACK_TEXUTRE_EXT_BMP)
		pTexture = Ntl_BMPTextureRead(&sPackImage);

	if (!pTexture)
	{
		DBO_WARNING_MESSAGE("fail Load texture: " << pFileName << " path " << pPath << " byExtType: " << (int)byExtType);
	}

	return pTexture;
}

RwBool CNtlPLResoucePackManager::LoadObject(const RwChar *pFileName, SPackResFileData& sPackFileData)
{
	if(m_uiFlags == 0)
		return FALSE;

	FilterPathBuffer(NULL, pFileName);

	SFilePackHeaderElement *pElement = NULL;
	RwChar *pPathRoot = NULL;

	static char szPath[4096];
	static char szName[1024];
	static std::string strPath;

	_splitpath_s( m_chFileBuffer, 0, 0, szPath, 4096, szName, 1024, 0, 0 );
	strPath = szPath;

	if ( strPath.find( "character" ) != std::string::npos )
	{
		pPathRoot = rwstrstr(m_chFileBuffer, "character");
	}
	if ( strPath.find( "item" ) != std::string::npos )
	{
		pPathRoot = rwstrstr(m_chFileBuffer, "item");
	}
	if ( strPath.find( "object" ) != std::string::npos )
	{
		pPathRoot = rwstrstr(m_chFileBuffer, "object");
	}
	if ( strPath.find( "effect" ) != std::string::npos )
	{
		pPathRoot = rwstrstr(m_chFileBuffer, "effect");
	}
	if ( strPath.find( "world" ) != std::string::npos )
	{
		pPathRoot = rwstrstr(m_chFileBuffer, "world");
	}

	m_iBufferSize	= 0;
	m_chFullBuffer[m_iBufferSize++] = '.';
	m_chFullBuffer[m_iBufferSize++] = '\\';

	strcpy_s(&m_chFullBuffer[m_iBufferSize], NTL_FILEPACK_BUFFER_SIZE - m_iBufferSize, pPathRoot);
	m_iBufferSize += strlen(pPathRoot);
	m_chFullBuffer[m_iBufferSize] = 0;

	RwChar *extender  = rwstrrchr(m_chFileBuffer, '.');
	if( extender == NULL )
		return FALSE;

	CNtlPLResourcePack *pResPack = m_pResPack[NTL_PACK_TYPE_OBJECT];
	pElement = pResPack->FindHeader(m_chFullBuffer);
	if(pElement == NULL)
		return FALSE;

	std::string strPackFileName;
	pResPack->GetPackDataFileName(pElement->byUnitKey, strPackFileName);

	sPackFileData.strPackFileName = NTL_PACK_FILE_PATH;
	sPackFileData.strPackFileName += strPackFileName.c_str();

	sPackFileData.uiOffset = pElement->uiOffset;
	sPackFileData.uiSize = pElement->uiSize;

	return TRUE;
}

RwBool CNtlPLResoucePackManager::LoadTerrain(const RwChar *pFileName, SPackResFileData& sPackFileData)
{
	if(m_uiFlags == 0)
		return FALSE;

	FilterPathBuffer(NULL, pFileName);

	SFilePackHeaderElement *pElement = NULL;
	RwChar *pPathRoot = NULL;
	
	m_iBufferSize	= 0;

	pPathRoot = rwstrstr(m_chFileBuffer, "world");
	
	m_chFullBuffer[m_iBufferSize++] = '.';
	m_chFullBuffer[m_iBufferSize++] = '\\';

	strcpy_s(&m_chFullBuffer[m_iBufferSize], NTL_FILEPACK_BUFFER_SIZE - m_iBufferSize, pPathRoot);
	m_iBufferSize += strlen(pPathRoot);
	m_chFullBuffer[m_iBufferSize] = 0;
	
	CNtlPLResourcePack *pResPack = m_pResPack[NTL_PACK_TYPE_TERRAIN];
	pElement = pResPack->FindHeader(m_chFullBuffer);
	if(pElement == NULL)
		return FALSE;

	std::string strPackFileName;
	pResPack->GetPackDataFileName(pElement->byUnitKey, strPackFileName);

	sPackFileData.strPackFileName = NTL_PACK_FILE_PATH;
	sPackFileData.strPackFileName += strPackFileName.c_str();

	sPackFileData.uiOffset = pElement->uiOffset;
	sPackFileData.uiSize = pElement->uiSize;

	return TRUE;
}

std::string CNtlPLResoucePackManager::LoadTerrain(const RwChar *pFileName, RwUInt32& uiOffset, RwUInt32& uiSize)
{
	SPackResFileData sFileData;
	RwBool bSuccess = LoadTerrain(pFileName, sFileData);

	uiOffset	= sFileData.uiOffset;
	uiSize		= sFileData.uiSize;

	return sFileData.strPackFileName;
}

void CNtlPLResoucePackManager::LoadGui(const RwChar *pFileName, void **pData, RwUInt32 *uiSize)
{
	*pData	= NULL;
	*uiSize = 0;

	if(m_uiFlags == 0)
		return;

	FilterPathBuffer(NULL, pFileName);

	SFilePackHeaderElement *pElement = NULL;
	RwChar *pPathRoot = NULL;
	
	m_iBufferSize	= 0;

	pPathRoot = rwstrstr(m_chFileBuffer, "gui");
	
	m_chFullBuffer[m_iBufferSize++] = '.';
	m_chFullBuffer[m_iBufferSize++] = '\\';

	strcpy_s(&m_chFullBuffer[m_iBufferSize], NTL_FILEPACK_BUFFER_SIZE - m_iBufferSize, pPathRoot);
	m_iBufferSize += strlen(pPathRoot);
	m_chFullBuffer[m_iBufferSize] = 0;
	
	CNtlPLResourcePack *pResPack = m_pResPack[NTL_PACK_TYPE_GUI];
	pElement = pResPack->FindHeader(m_chFullBuffer);
	if(pElement == NULL)
		return;

	std::string strPackFileName;
	pResPack->GetPackDataFileName(pElement->byUnitKey, strPackFileName);

	std::string strOpenPack;
	strOpenPack = NTL_PACK_FILE_PATH;
	strOpenPack += strPackFileName.c_str();

	FILE *fp = NULL;
	if(fopen_s(&fp, strOpenPack.c_str(), "rb") != 0)
		return;

	fseek(fp, pElement->uiOffset, SEEK_SET);
	*uiSize = pElement->uiSize;
	*pData = NTL_NEW char[pElement->uiSize];
	fread(*pData, pElement->uiSize, 1, fp);
	fclose(fp);

	return;
}

void CNtlPLResoucePackManager::LoadFlash(const RwChar *pFileName, void **pData, RwInt32 *iSize)
{
	*pData	= NULL;
	*iSize = 0;

	if(m_uiFlags == 0)
		return;

	FilterPathBuffer(NULL, pFileName);

	SFilePackHeaderElement *pElement = NULL;
	RwChar *pPathRoot = NULL;

	m_iBufferSize	= 0;

	pPathRoot = rwstrstr(m_chFileBuffer, "flash");

	m_chFullBuffer[m_iBufferSize++] = '.';
	m_chFullBuffer[m_iBufferSize++] = '\\';

	strcpy_s(&m_chFullBuffer[m_iBufferSize], NTL_FILEPACK_BUFFER_SIZE - m_iBufferSize, pPathRoot);
	m_iBufferSize += strlen(pPathRoot);
	m_chFullBuffer[m_iBufferSize] = 0;

	CNtlPLResourcePack *pResPack = m_pResPack[NTL_PACK_TYPE_FLASH];
	pElement = pResPack->FindHeader(m_chFullBuffer);
	if(pElement == NULL)
		return;

	// Flash Size는 int로 한정.
	if(pElement->uiSize >= 0x80000000 )
		return;

	std::string strPackFileName;
	pResPack->GetPackDataFileName(pElement->byUnitKey, strPackFileName);

	std::string strOpenPack;
	strOpenPack = NTL_PACK_FILE_PATH;
	strOpenPack += strPackFileName.c_str();

	FILE *fp = NULL;
	if(fopen_s(&fp, strOpenPack.c_str(), "rb") != 0)
		return;

	fseek(fp, pElement->uiOffset, SEEK_SET);
	*iSize = (RwInt32)pElement->uiSize;
	*pData = NTL_NEW BYTE[pElement->uiSize];
	fread(*pData, pElement->uiSize, 1, fp);
	fclose(fp);

	return;
}

void CNtlPLResoucePackManager::LoadSound(const RwChar *pFileName, void **pData, RwInt32 *iSize)
{
	*pData	= NULL;
	*iSize = 0;

	if(m_uiFlags == 0)
		return;

	FilterPathBuffer(NULL, pFileName);

	SFilePackHeaderElement *pElement = NULL;
	RwChar *pPathRoot = NULL;

	m_iBufferSize	= 0;

	pPathRoot = rwstrstr(m_chFileBuffer, "sound");

	m_chFullBuffer[m_iBufferSize++] = '.';
	m_chFullBuffer[m_iBufferSize++] = '\\';

	strcpy_s(&m_chFullBuffer[m_iBufferSize], NTL_FILEPACK_BUFFER_SIZE - m_iBufferSize, pPathRoot);
	m_iBufferSize += strlen(pPathRoot);
	m_chFullBuffer[m_iBufferSize] = 0;

	CNtlPLResourcePack *pResPack = m_pResPack[NTL_PACK_TYPE_SOUND];
	pElement = pResPack->FindHeader(m_chFullBuffer);
	if(pElement == NULL)
		return;

	std::string strPackFileName;
	pResPack->GetPackDataFileName(pElement->byUnitKey, strPackFileName);

	std::string strOpenPack;
	strOpenPack = NTL_PACK_FILE_PATH;
	strOpenPack += strPackFileName.c_str();

	FILE *fp = NULL;
	if(fopen_s(&fp, strOpenPack.c_str(), "rb") != 0)
		return;

	fseek(fp, pElement->uiOffset, SEEK_SET);
	*iSize = (RwInt32)pElement->uiSize;

    NTL_ARRAY_DELETE(*pData);
	*pData = NTL_NEW BYTE[pElement->uiSize];
	fread(*pData, pElement->uiSize, 1, fp);
	fclose(fp);

	return;
}


void CNtlPLResoucePackManager::LoadLanguage( const RwChar* pFileName, void** pData, RwInt32* iSize ) 
{
    *pData	= NULL;
    *iSize = 0;

    if(m_uiFlags == 0)
        return;

    FilterPathBuffer(NULL, pFileName);

    SFilePackHeaderElement *pElement = NULL;
    RwChar *pPathRoot = NULL;

    m_iBufferSize	= 0;

    pPathRoot = rwstrstr(m_chFileBuffer, "language");

    m_chFullBuffer[m_iBufferSize++] = '.';
    m_chFullBuffer[m_iBufferSize++] = '\\';

    strcpy_s(&m_chFullBuffer[m_iBufferSize], NTL_FILEPACK_BUFFER_SIZE - m_iBufferSize, pPathRoot);
    m_iBufferSize += strlen(pPathRoot);
    m_chFullBuffer[m_iBufferSize] = 0;

    CNtlPLResourcePack *pResPack = m_pResPack[NTL_PACK_TYPE_LANGUAGE];
    pElement = pResPack->FindHeader(m_chFullBuffer);
    if(pElement == NULL)
        return;

    std::string strPackFileName;
    pResPack->GetPackDataFileName(pElement->byUnitKey, strPackFileName);

    std::string strOpenPack;
    strOpenPack = NTL_PACK_FILE_PATH;
    strOpenPack += strPackFileName.c_str();

    FILE *fp = NULL;
    if(fopen_s(&fp, strOpenPack.c_str(), "rb") != 0)
        return;

    fseek(fp, pElement->uiOffset, SEEK_SET);
    *iSize = (RwInt32)pElement->uiSize;
    *pData = NTL_NEW BYTE[pElement->uiSize];
    fread(*pData, pElement->uiSize, 1, fp);
    fclose(fp);

    return;
}

void CNtlPLResoucePackManager::LoadScript( const RwChar* pFileName, void** pData, RwInt32* iSize ) 
{
    *pData	= NULL;
    *iSize = 0;

    if(m_uiFlags == 0)
        return;

    FilterPathBuffer(NULL, pFileName);

    SFilePackHeaderElement *pElement = NULL;
    RwChar *pPathRoot = NULL;

    m_iBufferSize	= 0;

    pPathRoot = rwstrstr(m_chFileBuffer, "script");

    m_chFullBuffer[m_iBufferSize++] = '.';
    m_chFullBuffer[m_iBufferSize++] = '\\';

    strcpy_s(&m_chFullBuffer[m_iBufferSize], NTL_FILEPACK_BUFFER_SIZE - m_iBufferSize, pPathRoot);
    m_iBufferSize += strlen(pPathRoot);
    m_chFullBuffer[m_iBufferSize] = 0;

    CNtlPLResourcePack *pResPack = m_pResPack[NTL_PACK_TYPE_SCRIPT];
    pElement = pResPack->FindHeader(m_chFullBuffer);
    if(pElement == NULL)
        return;

    std::string strPackFileName;
    pResPack->GetPackDataFileName(pElement->byUnitKey, strPackFileName);

    std::string strOpenPack;
    strOpenPack = NTL_PACK_FILE_PATH;
    strOpenPack += strPackFileName.c_str();

    FILE *fp = NULL;
    if(fopen_s(&fp, strOpenPack.c_str(), "rb") != 0)
        return;

    fseek(fp, pElement->uiOffset, SEEK_SET);
    *iSize = (RwInt32)pElement->uiSize;
    *pData = NTL_NEW BYTE[pElement->uiSize];
    fread(*pData, pElement->uiSize, 1, fp);
    fclose(fp);

    return;
}

void CNtlPLResoucePackManager::LoadProperty( const RwChar* pFileName, void** pData, RwInt32* iSize ) 
{
    *pData	= NULL;
    *iSize = 0;

    if(m_uiFlags == 0)
        return;

    FilterPathBuffer(NULL, pFileName);

    SFilePackHeaderElement *pElement = NULL;
    RwChar *pPathRoot = NULL;

    m_iBufferSize	= 0;

    pPathRoot = rwstrstr(m_chFileBuffer, "property");

    m_chFullBuffer[m_iBufferSize++] = '.';
    m_chFullBuffer[m_iBufferSize++] = '\\';

    strcpy_s(&m_chFullBuffer[m_iBufferSize], NTL_FILEPACK_BUFFER_SIZE - m_iBufferSize, pPathRoot);
    m_iBufferSize += strlen(pPathRoot);
    m_chFullBuffer[m_iBufferSize] = 0;

    CNtlPLResourcePack *pResPack = m_pResPack[NTL_PACK_TYPE_PROPERTY];
    pElement = pResPack->FindHeader(m_chFullBuffer);
    if(pElement == NULL)
        return;

    std::string strPackFileName;
    pResPack->GetPackDataFileName(pElement->byUnitKey, strPackFileName);

    std::string strOpenPack;
    strOpenPack = NTL_PACK_FILE_PATH;
    strOpenPack += strPackFileName.c_str();

    FILE *fp = NULL;
    if(fopen_s(&fp, strOpenPack.c_str(), "rb") != 0)
        return;

    fseek(fp, pElement->uiOffset, SEEK_SET);
    *iSize = (RwInt32)pElement->uiSize;
    *pData = NTL_NEW BYTE[pElement->uiSize];
    fread(*pData, pElement->uiSize, 1, fp);
    fclose(fp);
}

void CNtlPLResoucePackManager::LoadTable( const RwChar* pFileName, void** pData, RwInt32* iSize ) 
{
	*pData	= NULL;
	*iSize = 0;

	if(m_uiFlags == 0)
		return;
	
	FilterPathBuffer(NULL, pFileName);

	SFilePackHeaderElement *pElement = NULL;
	RwChar *pPathRoot = NULL;

	m_iBufferSize	= 0;

	pPathRoot = rwstrstr(m_chFileBuffer, "data");

	m_chFullBuffer[m_iBufferSize++] = '.';
	m_chFullBuffer[m_iBufferSize++] = '\\';

	strcpy_s(&m_chFullBuffer[m_iBufferSize], NTL_FILEPACK_BUFFER_SIZE - m_iBufferSize, pPathRoot);
	m_iBufferSize += strlen(pPathRoot);
	m_chFullBuffer[m_iBufferSize] = 0;

	CNtlPLResourcePack *pResPack = m_pResPack[NTL_PACK_TYPE_TABLE];
	pElement = pResPack->FindHeader(m_chFullBuffer);
	if (pElement == NULL)
	{
		return;
	}

	std::string strPackFileName;
	pResPack->GetPackDataFileName(pElement->byUnitKey, strPackFileName);

	std::string strOpenPack;
	strOpenPack = NTL_PACK_FILE_PATH;
	strOpenPack += strPackFileName.c_str();

	FILE *fp = NULL;
	if(fopen_s(&fp, strOpenPack.c_str(), "rb") != 0)
		return;

	fseek(fp, pElement->uiOffset, SEEK_SET);
	*iSize = (RwInt32)pElement->uiSize;
	*pData = NTL_NEW BYTE[pElement->uiSize];
	fread(*pData, pElement->uiSize, 1, fp);
	fclose(fp);
}

bool CNtlPLResoucePackManager::IsExistSoundFile(const RwChar *pFileName)
{
	if(m_uiFlags == 0)
		return false;

	FilterPathBuffer(NULL, pFileName);

	SFilePackHeaderElement *pElement = NULL;
	RwChar *pPathRoot = NULL;

	m_iBufferSize	= 0;

	pPathRoot = rwstrstr(m_chFileBuffer, "sound");

	m_chFullBuffer[m_iBufferSize++] = '.';
	m_chFullBuffer[m_iBufferSize++] = '\\';

	strcpy_s(&m_chFullBuffer[m_iBufferSize], NTL_FILEPACK_BUFFER_SIZE - m_iBufferSize, pPathRoot);
	m_iBufferSize += strlen(pPathRoot);
	m_chFullBuffer[m_iBufferSize] = 0;

	CNtlPLResourcePack *pResPack = m_pResPack[NTL_PACK_TYPE_SOUND];
	pElement = pResPack->FindHeader(m_chFullBuffer);
	if(pElement == NULL)
		return false;

	std::string strPackFileName;
	pResPack->GetPackDataFileName(pElement->byUnitKey, strPackFileName);

	std::string strOpenPack;
	strOpenPack = NTL_PACK_FILE_PATH;
	strOpenPack += strPackFileName.c_str();

	FILE *fp = NULL;
	if(fopen_s(&fp, strOpenPack.c_str(), "rb") != 0)
		return false;

	fclose(fp);
	return true;
}



RwTexture* ResourcPackTextureLoad(const RwChar *pPath, const RwChar *pFileName, const RwChar *pMaskName)
{
	return GetNtlResourcePackManager()->LoadTexture(pPath, pFileName, pMaskName);
}

void GuiPackLoad(const RwChar *pFileName, void **pData, unsigned int *uiSize)
{
	GetNtlResourcePackManager()->LoadGui(pFileName, pData, uiSize);
}

void FlashPackLoad(const RwChar *pFileName, void **pData, int *iSize)
{
	GetNtlResourcePackManager()->LoadFlash( pFileName, pData, iSize );
}

void SoundPackLoad(const RwChar *pFileName, void **pData, int *iSize)
{
	GetNtlResourcePackManager()->LoadSound( pFileName, pData, iSize );
}

void LanguagePackLoad(const RwChar *pFileName, void **pData, int *iSize)
{
	GetNtlResourcePackManager()->LoadLanguage( pFileName, pData, iSize );
}

void ScriptPackLoad( const RwChar* pFileName, void **pData, int *iSize )
{
	GetNtlResourcePackManager()->LoadScript( pFileName, pData, iSize );
}

bool IsExistSoundFile(const RwChar *pFileName)
{
	return GetNtlResourcePackManager()->IsExistSoundFile(pFileName);
}

//////////////////////////////////////////////////////////////////////////

CTablePackLoad::CTablePackLoad( void )
{
}

CTablePackLoad::~CTablePackLoad( void )
{
}

bool CTablePackLoad::Call( const char* pfilename, CNtlFileSerializer* pSeralize, const char* pszCryptPassword ) 
{
	if ( pszCryptPassword )
	{
		char* pBuffer = NULL;
		RwInt32 iBufferSize = 0;
		
		GetNtlResourcePackManager()->LoadTable( pfilename, (void**)&pBuffer, &iBufferSize );
		if ( NULL == pBuffer || 0 == iBufferSize )
		{
			NTL_ARRAY_DELETE( pBuffer );

			return false;
		}
		
		if ( !pSeralize->LoadFile( pBuffer, iBufferSize, TRUE, (char*)pszCryptPassword ) )
		{
			NTL_ARRAY_DELETE( pBuffer );

			return false;
		}

		NTL_ARRAY_DELETE( pBuffer );
	}
	else
	{
		void* pBuffer = NULL;
		RwInt32 iBufferSize = 0;


		GetNtlResourcePackManager()->LoadTable( pfilename, (void**)&pBuffer, &iBufferSize );
		if ( NULL == pBuffer || 0 == iBufferSize )
		{
			NTL_ARRAY_DELETE( pBuffer );

			return false;
		}

		pSeralize->In( pBuffer, iBufferSize );

		NTL_ARRAY_DELETE( pBuffer );
	}

	return true;
}
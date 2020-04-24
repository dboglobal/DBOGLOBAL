#include "stdafx.h"
#include "PatchConfigManager.h"
#include "AutoPatchLog.h"
#include <tlhelp32.h>

// shared
#include "NtlXMLDoc.h"

#define RENAME_NEW_SYSTEM_SLEEP			10000
#define RTPATCH_BUILD_PROCESS_SLEEP		100000
#define TSEVTMAKER_BUILD_PROCESS_SLEEP	10000
#define TERACOPY_COMMAND                "C:\\Program Files\\TeraCopy\\TeraCopy.exe Copy %s %s /OverwriteAll /Close"

CPatchFTPUploadScript::CPatchFTPUploadScript()
{
}

CPatchFTPUploadScript::~CPatchFTPUploadScript()
{
	
}

bool CPatchFTPUploadScript::Parse(const char *key, const char *data)
{
	int nSize = (int)m_strBuffer.size();
	int nOffset = (int)m_strBuffer.find(key);
	int nStart = nOffset + (int)strlen(key);

	char *pBefore = new char [nSize];
	char *pFileName = new char [nSize];
	char *pAfter = new char [nSize];

	memset(pBefore, 0, nSize);
	memset(pFileName, 0, nSize);
	memset(pAfter, 0, nSize);

	int nCopyIdx = 0;

	for(int i = nStart; i < nSize; i++)
	{
		if(m_strBuffer[i] != ' ')
		{
			nCopyIdx = i;
			break;
		}
	}

	memcpy(pBefore, m_strBuffer.c_str(), nCopyIdx);

	// put data filename 검출.
	nStart = nCopyIdx;

	nCopyIdx = nSize;

	for(int i = nStart; i < nSize; i++)
	{
		if(m_strBuffer[i] == ' ' || m_strBuffer[i] == '\n')
		{
			nCopyIdx = i;
			break;
		}
	}

	memcpy(pFileName, m_strBuffer.c_str()+nStart, nCopyIdx - nStart);
	memcpy(pAfter, m_strBuffer.c_str()+nCopyIdx, nSize - nCopyIdx);

	// file 이름 고치기.

	int nInsertLen = (int)strlen(data);
	int nFileLen = (int)strlen(pFileName);
	int nAfterLen = (int)strlen(pAfter);
	int nIdx;

	bool bStringMark = pFileName[nFileLen-1] == '"' ? true : false;

	for(int i = nFileLen-1; i >= 0; i--)
	{
		if(pFileName[i] == '\\' || pFileName[i] == '/')
		{
			memcpy(&pFileName[i+1], data, nInsertLen);
			nIdx = i+1+nInsertLen;

			if(bStringMark)
			{
				pFileName[nIdx] = '"';
				nIdx++;
			}

			pFileName[nIdx] = 0;

			goto success_exit;
		}
	}

	memcpy(pFileName, data, nInsertLen);
	nIdx = nInsertLen;
	if(bStringMark)
	{
		pFileName[nIdx] = '"';
		nIdx++;
	}

	pFileName[nIdx] = 0;

success_exit:

	m_strBuffer = pBefore;
	m_strBuffer += pFileName;
	m_strBuffer += pAfter;

	delete [] pBefore;
	delete [] pFileName;
	delete [] pAfter;
		
	return true;
}

bool CPatchFTPUploadScript::Load(const char *pFileName)
{
	FILE* fp = NULL;

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	if(fopen_s(&fp, pFileName, "rt") != 0)
		return false;
#else
	fp = fopen(pFileName, "rt");
	if(fp == NULL)
		return false;
#endif

	fseek(fp, -1, SEEK_END);
	long length = ftell(fp);

	fseek(fp, 0, SEEK_SET);

	char *pBuffer = new char [length+1];
	memset(pBuffer, 0, length+1);
	
	fread(pBuffer, 1, length, fp);

	fclose(fp);

	m_strFileName = pFileName;
	m_strBuffer = pBuffer;

	delete [] pBuffer;

	return true;
}

bool CPatchFTPUploadScript::Save(const char *pFileName)
{
	FILE* fp = NULL;

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	if(fopen_s(&fp, pFileName, "wt") != 0)
		return false;
#else
	fp = fopen(pFileName, "wt");
	if(fp == NULL)
		return false;
#endif

	fwrite(m_strBuffer.c_str(), 1, m_strBuffer.size(), fp);

	fclose(fp);

	return true;
}

bool CPatchFTPUploadScript::Run(const char *key, const char *data)
{
	Parse(key, data);

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


CPatchConfigManager::CPatchConfigManager()
{
	
}

CPatchConfigManager::~CPatchConfigManager()
{
	Clear();
}


CPatchConfigManager* CPatchConfigManager::GetInstance(void)
{
	static CPatchConfigManager pcm;
	return &pcm;
}

void CPatchConfigManager::Clear(void)
{
	m_sPatchConfig.sCommon.bDiffPatch = true;
	m_sPatchConfig.sCurrVer.nLVer = 0;
	m_sPatchConfig.sCurrVer.nRVer = 0;
	m_sPatchConfig.sCurrVer.nBVer = 0;

	m_sPatchConfig.sCopy.strOldSystem = "";
	m_sPatchConfig.sCopy.strNewSystem = "";

	for(int i = 0; i < ECDT_End; i++)
	{
		m_sPatchConfig.sCopy.strSrcFolder[i] = "";
	}

	std::list<SPCCopyData*>::iterator it;

	for(it = m_sPatchConfig.sCopy.listCopyData.begin(); it != m_sPatchConfig.sCopy.listCopyData.end(); it++)
	{
		delete (*it);
	}

	m_sPatchConfig.sCopy.listCopyData.clear();


	for(it = m_sServerPatchConfig.sCopy.listCopyData.begin(); it != m_sServerPatchConfig.sCopy.listCopyData.end(); it++)
	{
		delete (*it);
	}

	m_sServerPatchConfig.sCopy.listCopyData.clear();
}

void CPatchConfigManager::CreateSubDirectory(const char *pFullName, const char *pAttachName)
{
	std::string strFullName = pFullName;
	std::string strAttachName = pAttachName;

	int nAttackLen = (int)strAttachName.size();
	strFullName += "\\";

	bool bCreateDir = false;

	for(int i = 0; i < nAttackLen; i++)
	{
		bCreateDir = false;

		if(pAttachName[i] == '\\' || pAttachName[i] == '/')
		{
			CreateDirectory(strFullName.c_str(), NULL);
			bCreateDir = true;

			strFullName += '\\';
		}
		else
		{
			strFullName += pAttachName[i];
		}
	}

	if(!bCreateDir)
	{
		CreateDirectory(strFullName.c_str(), NULL);
	}
}

bool CPatchConfigManager::IsFolder(const char *pFileName)
{
	WIN32_FIND_DATA FileData;
	HANDLE hFile;

	hFile = FindFirstFile(pFileName, &FileData);
	if(hFile == INVALID_HANDLE_VALUE) 
		return false;

	if(FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		return true;
	else
		return false;
}

bool CPatchConfigManager::CopyFolder(const char *pSrcFolder, const char *pDestFolder)
{
    // 카피전에 폴더를 만들어 줘야 한다.
    CreateDirectory(pDestFolder, NULL);

    // 파일 복사 프로그램(TeraCopy)를 사용한다.   
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory( &si, sizeof(si) );
    ZeroMemory( &pi, sizeof(pi) );			
    si.cb = sizeof(si);

    // 폴더 카피는 하위 폴더로 생성된다. 그래서 destFolder에서 한단계 위 폴더로 정해야한다.
    std::string strDestFolderRoot = pDestFolder;
    size_t nPos = strDestFolderRoot.rfind("\\");
    strDestFolderRoot.replace(nPos, strDestFolderRoot.length() - (nPos), "\0");

    char szCommand[1024] = {0,};
    sprintf_s(szCommand, TERACOPY_COMMAND, pSrcFolder, strDestFolderRoot.c_str());
    OutLog(szCommand);   

    if(CreateProcess(NULL, szCommand, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        return true;
    }

    // Teracopy가 없으면 일반 카피 API 사용
    sprintf_s(szCommand, "Copy Folder - Teracopy is Not Install. Process Normal Copy API");
    OutLog(szCommand);

	WIN32_FIND_DATA FileData;
	HANDLE hFile;
	
	std::string strDirectory = pSrcFolder;
	strDirectory += "\\";
	strDirectory += "*.*";

	hFile = FindFirstFile(strDirectory.c_str(), &FileData);
	if(hFile == INVALID_HANDLE_VALUE) 
		return true;

	std::string strSrcFile, strDestFile;
	
	do
	{
		if(FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//  디렉토리이면
			if(FileData.cFileName[0] == '.')
			{
				//  Dot 디렉토리는 스킵한다.
			} 
			else
			{
				std::string strScrFolder, strDestFolder;
				strScrFolder = pSrcFolder;
				strScrFolder += "\\";
				strScrFolder += FileData.cFileName;

				strDestFolder = pDestFolder;
				strDestFolder += "\\";
				strDestFolder += FileData.cFileName;

				if(!CopyFolder(strScrFolder.c_str(), strDestFolder.c_str()))
				{
					FindClose(hFile);

                    char szTemp[256] = {0,};
                    sprintf_s(szTemp, "Copy Folder Fail %s -> %s", strSrcFile.c_str(), strDestFolder.c_str());
                    OutLog(szTemp);

					return false;
				}
			}
		} 
		else
		{
			strSrcFile = pSrcFolder;
			strSrcFile += "\\";
			strSrcFile += FileData.cFileName;

			strDestFile = pDestFolder;
			strDestFile += "\\";
			strDestFile += FileData.cFileName;

			if(::CopyFile(strSrcFile.c_str(), strDestFile.c_str(), FALSE) == FALSE)
			{
				FindClose(hFile);
				return false;
			}
		}
	} while(FindNextFile(hFile, &FileData));

	FindClose(hFile);

	return true;
}

bool CPatchConfigManager::CopyFile(const std::string& srcFile, const std::string& destFile)
{
    // 파일 복사 프로그램(TeraCopy)를 사용한다.   
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory( &si, sizeof(si) );
    ZeroMemory( &pi, sizeof(pi) );			
    si.cb = sizeof(si);

    // TeraCopy는 파일 카피때 대상 폴더명만 써야 된다.
    char szDrive[64] = {0,};
    char szDir[128] = {0,};      
    _splitpath(destFile.c_str(), szDrive, szDir, NULL, NULL);

    std::string destFolder = szDrive;
    destFolder += szDir;
    char szCommand[1024] = {0,};
    sprintf_s(szCommand, TERACOPY_COMMAND, srcFile.c_str(), destFolder.c_str());
    OutLog(szCommand);

    if(CreateProcess(NULL, szCommand, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        return true;
    }

    sprintf_s(szCommand, "Copy File - Teracopy is Not Install. Process Normal Copy API");
    OutLog(szCommand);

    // Teracopy가 없으면 일반 API 사용
    if(::CopyFile(srcFile.c_str(), destFile.c_str(), FALSE))
    {
        return true;
    }
    else
    {
        char szTemp[256] = {0,};
        sprintf_s(szTemp, "Copy File Fail %s -> %s", srcFile.c_str(), destFile.c_str());
        OutLog(szTemp);

        return false;
    }
}

bool CPatchConfigManager::DeleteFolder(const char *pFolder, bool bRootFolderDelete /*= true*/)
{
	WIN32_FIND_DATA FileData;
	HANDLE hFile;
	
	std::string strDirectory = pFolder;
	strDirectory += "\\";
	strDirectory += "*.*";

	hFile = FindFirstFile(strDirectory.c_str(), &FileData);
	if(hFile == INVALID_HANDLE_VALUE) 
	{
		RemoveDirectory(pFolder);
		return true;
	}

	std::list<std::string>::iterator it;
	std::list<std::string> listDel;

	do
	{
		if(FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//  디렉토리이면
			if(FileData.cFileName[0] == '.')
			{
				//  Dot 디렉토리는 스킵한다.
			} 
			else
			{
				std::string strDelFolder;
				strDelFolder = pFolder;
				strDelFolder += "\\";
				strDelFolder += FileData.cFileName;

				DeleteFolder(strDelFolder.c_str());
			}
		} 
		else
		{
			std::string strDelFile = pFolder;
			strDelFile = pFolder;
			strDelFile += "\\";
			strDelFile += FileData.cFileName;

			listDel.push_back(strDelFile);
		}
	} while(FindNextFile(hFile, &FileData));

	FindClose(hFile);

	for(it = listDel.begin(); it != listDel.end(); it++)
	{
		DWORD dwAttrs = ::GetFileAttributes((*it).c_str());
		if(dwAttrs == INVALID_FILE_ATTRIBUTES)
			continue; 

		if( dwAttrs & FILE_ATTRIBUTE_READONLY) 
		{ 
			dwAttrs &= ~FILE_ATTRIBUTE_READONLY;
			::SetFileAttributes((*it).c_str(), dwAttrs); 
		} 

		DeleteFile((*it).c_str());
	}

	if(bRootFolderDelete)
	{
		RemoveDirectory(pFolder);
	}

	return true;
}

bool CPatchConfigManager::DeleteChildFolder(const char *pFolder)
{
	return DeleteFolder(pFolder, false);
}

void CPatchConfigManager::FolderReadOnlyRelease(const char *pFolder)
{
	WIN32_FIND_DATA FileData;
	HANDLE hFile;
	
	std::string strDirectory = pFolder;
	strDirectory += "\\";
	strDirectory += "*.*";

	hFile = FindFirstFile(strDirectory.c_str(), &FileData);
	if(hFile == INVALID_HANDLE_VALUE) 
	{
		return;
	}

	std::list<std::string>::iterator it;
	std::list<std::string> listDel;

	do
	{
		if(FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//  디렉토리이면
			if(FileData.cFileName[0] == '.')
			{
				//  Dot 디렉토리는 스킵한다.
			} 
			else
			{
				std::string strNextFolder;
				strNextFolder = pFolder;
				strNextFolder += "\\";
				strNextFolder += FileData.cFileName;

				FolderReadOnlyRelease(strNextFolder.c_str());
			}
		} 
		else
		{
			std::string strNextFile = pFolder;
			strNextFile = pFolder;
			strNextFile += "\\";
			strNextFile += FileData.cFileName;

			FileReadOnlyRelease(strNextFile.c_str());
		}
	} while(FindNextFile(hFile, &FileData));

	FindClose(hFile);

	DWORD dwAttrs = ::GetFileAttributes(pFolder);
	if(dwAttrs == INVALID_FILE_ATTRIBUTES)
		return; 

	if( dwAttrs & FILE_ATTRIBUTE_READONLY) 
	{ 
		dwAttrs &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(pFolder, dwAttrs); 
	} 
}

void CPatchConfigManager::FileReadOnlyRelease(const char *pFileName)
{
	DWORD dwAttrs = ::GetFileAttributes(pFileName);
	if(dwAttrs == INVALID_FILE_ATTRIBUTES)
		return; 

	if( dwAttrs & FILE_ATTRIBUTE_READONLY) 
	{ 
		dwAttrs &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(pFileName, dwAttrs); 
	} 
}

std::string CPatchConfigManager::MakeVersion(int nLVer, int nRVer, int nBVer)
{
	char chBuffer[128];
	sprintf_s(chBuffer, 128, "(%d.%d.%d)", nLVer, nRVer, nBVer);

	std::string str = chBuffer;
	return str;
}

std::string CPatchConfigManager::GetDate(void)
{
	char chBuffer[128];
	SYSTEMTIME dt;
	GetLocalTime(&dt);

	sprintf_s(chBuffer, 128, "%02d%02d", dt.wMonth, dt.wDay);

	return chBuffer;
}

std::string	CPatchConfigManager::GetDataTime(void)
{
	char chBuffer[128];
	SYSTEMTIME dt;
	GetLocalTime(&dt);

	sprintf_s(chBuffer, 128, "%04d-%02d-%02d-%02d-%02d", dt.wYear, dt.wMonth, dt.wDay, dt.wHour, dt.wMinute);

	return chBuffer;
}

bool CPatchConfigManager::CopySourceAll(void)
{
	CreateDirectory(m_sPatchConfig.sCopy.strNewSystem.c_str(), NULL);

	DeleteChildFolder(m_sPatchConfig.sCopy.strNewSystem.c_str());

	std::list<SPCCopyData*>::iterator it;

	SPCCopyData *pCopyData;
	std::string strSrc, strDest;

	for(it = m_sPatchConfig.sCopy.listCopyData.begin(); it != m_sPatchConfig.sCopy.listCopyData.end(); it++)
	{
		pCopyData = (*it);

		if(pCopyData->eCopyDataType == ECDT_Code)
			continue;
	
		if(m_sPatchConfig.sCopy.strSrcFolder[pCopyData->eCopyDataType].size() <= 0)
			continue;

		strSrc = m_sPatchConfig.sCopy.strSrcFolder[pCopyData->eCopyDataType];
		strSrc += "\\";
		strSrc += pCopyData->strName;

		strDest = m_sPatchConfig.sCopy.strNewSystem;
		strDest += "\\";
		strDest += pCopyData->strName;

        if( IsFolder(strSrc.c_str()) )
        {
            if(!CopyFolder(strSrc.c_str(), strDest.c_str()))
            {
                char log[256] = {0,};
                sprintf_s(log, "Folder Copy Fail %s to %s", strSrc.c_str(), strDest.c_str());
                OutLog(log);

                return false;
            }
        }
        else
        {
            if(CopyFile(strSrc, strDest) == FALSE)
            {
                char log[256] = {0,};
                sprintf_s(log, "File Copy Fail %s to %s", strSrc.c_str(), strDest.c_str());
                OutLog(log);

                return false;
            }
        }
	}

	return true;
}



bool CPatchConfigManager::CopyDump(BOOL bFastMode /* = FALSE */)
{
	std::string strDumpFolder = m_sPatchConfig.sCopy.strSrcFolder[ECDT_Dump];
	strDumpFolder += "\\";
	strDumpFolder += MakeVersion(m_sPatchConfig.sCurrVer.nLVer, m_sPatchConfig.sCurrVer.nRVer, m_sPatchConfig.sCurrVer.nBVer + 1);

	CreateDirectory(strDumpFolder.c_str(), NULL);

    if(!bFastMode)
    {
        // code copy    

        std::list<SPCCopyData*>::iterator it;

        SPCCopyData *pCopyData;
        std::string strSrc, strDest;

        for(it = m_sPatchConfig.sCopy.listCopyData.begin(); it != m_sPatchConfig.sCopy.listCopyData.end(); it++)
        {
            pCopyData = (*it);

            if(pCopyData->eCopyDataType != ECDT_Code)
                continue;

            if(m_sPatchConfig.sCopy.strSrcFolder[pCopyData->eCopyDataType].size() <= 0)
                continue;

            CreateSubDirectory(strDumpFolder.c_str(), pCopyData->strName.c_str());

            strSrc = m_sPatchConfig.sCopy.strSrcFolder[pCopyData->eCopyDataType];
            strSrc += "\\";
            strSrc += pCopyData->strName;

            strDest = strDumpFolder;
            strDest += "\\";
            strDest += pCopyData->strName;

            if( IsFolder(strSrc.c_str()) )
            {
                FolderReadOnlyRelease(strDest.c_str());

                if(!CopyFolder(strSrc.c_str(), strDest.c_str()))
                    return false;
            }
            else
            {
                FileReadOnlyRelease(strDest.c_str());
                if(CopyFile(strSrc, strDest) == FALSE)            
                    return false;
            }
        }
    }

	//-----------------------------------------------------------------------------------------------------------------------

    // 디버깅에 필요한 파일들 카피

	std::string strSrcDump, strSrcExe, strDestDump, strDestExe, strSrcMAP, strDestMAP;

	strSrcDump = m_sPatchConfig.sCopy.strSrcFolder[ECDT_Exe];
	strSrcDump += "\\";

	strSrcExe = strSrcDump;
    strSrcMAP = strSrcDump;

	strSrcDump += "DBO.pdb";
	strSrcExe += "DBO.exe";
    strSrcMAP += "DBO.map";

	strDestDump = strDumpFolder;
	strDestDump += "\\";

	strDestExe = strDestDump;
    strDestMAP = strDestDump;

	strDestDump += "DBO.pdb";
	strDestExe += "DBO.exe";
    strDestMAP += "DBO.map";
	
	if(CopyFile(strSrcDump, strDestDump) == FALSE)
    {
        char log[256] = {0,};
        sprintf_s(log, "pdb File Copy Fail %s to %s", strSrcDump.c_str(), strDestDump.c_str());
        OutLog(log);
    }

	if(CopyFile(strSrcExe, strDestExe) == FALSE)
    {
        char log[256] = {0,};
        sprintf_s(log, "EXE File Copy Fail %s to %s", strSrcExe.c_str(), strDestExe.c_str());
        OutLog(log);
    }

    if(CopyFile(strSrcMAP, strDestMAP) == FALSE)
    {
        char log[256] = {0,};
        sprintf_s(log, "MAP File Copy Fail %s to %s", strSrcMAP.c_str(), strDestMAP.c_str());
        OutLog(log);
    }

	return true;
}

bool CPatchConfigManager::FindPatchData(void)
{
	char chBuffer[128];
	sprintf_s(chBuffer, 128, "%d.%d.%d.RTP", m_sPatchConfig.sCurrVer.nLVer, m_sPatchConfig.sCurrVer.nRVer, m_sPatchConfig.sCurrVer.nBVer+1);

	std::string strFileName = m_sPatchConfig.sRtPatch.strRtPatchExport;
	strFileName += "\\";
	strFileName += chBuffer;

	WIN32_FIND_DATA FileData;
	HANDLE hFile;

	DWORD dwTime = GetTickCount();

	while(1)
	{
		hFile = FindFirstFile(strFileName.c_str(), &FileData);
		if(hFile == INVALID_HANDLE_VALUE) 
		{
			FindClose(hFile);
			Sleep(1000);
		}
		else
		{
			return true;
		}

		if(GetTickCount() - dwTime >= 100000)
			return false;
	}

	return true;
}

bool CPatchConfigManager::RenameOldSystem(void)
{
	std::string str = m_sPatchConfig.sCopy.strOldSystem;
	str += MakeVersion(m_sPatchConfig.sCurrVer.nLVer, m_sPatchConfig.sCurrVer.nRVer, m_sPatchConfig.sCurrVer.nBVer);

	DeleteFolder(str.c_str());

	DWORD dwTime = GetTickCount();

	while(1)
	{
		int nRes = rename(m_sPatchConfig.sCopy.strOldSystem.c_str(), str.c_str());
		if(nRes != 0)
		{
            char log[256] = {0,};
            sprintf_s(log, "OldSystem Rename Fail - Err Code [%d] %s to %s", nRes, m_sPatchConfig.sCopy.strOldSystem.c_str(), str.c_str());
            OutLog(log);

			Sleep(1000);
		}
		else
		{
			break;
		}

		if(GetTickCount() - dwTime >= 60000)
			return false;
	}

	return true;
}

bool CPatchConfigManager::RenameNewSystem(void)
{
	Sleep(RENAME_NEW_SYSTEM_SLEEP);

	DeleteFolder(m_sPatchConfig.sCopy.strOldSystem.c_str());

	DWORD dwTime = GetTickCount();

	while(1)
	{
		int nRes = rename(m_sPatchConfig.sCopy.strNewSystem.c_str(), m_sPatchConfig.sCopy.strOldSystem.c_str());
		if(nRes != 0)
		{
			Sleep(1000);
		}
		else
		{
			break;
		}
		
		if(GetTickCount() - dwTime >= RENAME_NEW_SYSTEM_SLEEP)
			return false;
	}

	dwTime = GetTickCount();

	while(1)
	{
		if(!CreateDirectory(m_sPatchConfig.sCopy.strNewSystem.c_str(), NULL))
		{
			Sleep(1000);
		}
		else
		{
			break;
		}

		if(GetTickCount() - dwTime >= 10000)
			return false;
	}

	return true;
}

bool CPatchConfigManager::LoadVersion(const char *pFileName)
{
	CNtlXMLDoc doc;
	doc.Create();

	if(doc.Load( (char*)pFileName ) == false)
	{
		goto Error_Exit;
	}

	char chBuffer[1024];
	IXMLDOMNode* pNode = NULL;
	IXMLDOMNodeList* pAttrNodeList = NULL;

	//-----------------------------------------------------------------------------
	// client diff patch
	pNode = doc.SelectSingleNode((char*)"/PatchList/ClientDiffRtPatch");
	HRESULT hResult = pNode->get_childNodes(&pAttrNodeList);
	if(hResult != S_OK)
	{
		goto Error_Exit;
	}
	
	long length;

	if (FAILED(pAttrNodeList->get_length(&length)))
	{
		goto Error_Exit;
	}

	IXMLDOMNode* pCellNode = NULL;

	if(length <= 0)
	{
		goto Error_Exit;
	}

	if (FAILED(pAttrNodeList->get_item(length-1, &pCellNode)))
	{
		goto Error_Exit;
	}

	if(doc.GetTextWithAttributeName(pCellNode, "Ver", chBuffer, 1024))
	{
		int nLen = (int)strlen(chBuffer);
		std::string strTemp;

		int nIdx = 0;

		for(int i =0; i < nLen; i++)
		{
			if(chBuffer[i] == '.')
			{
				if(nIdx == 0)
				{
					m_sPatchConfig.sCurrVer.nLVer = (int)atoi( strTemp.c_str() );
				}
				else if(nIdx == 1)
				{
					m_sPatchConfig.sCurrVer.nRVer = (int)atoi( strTemp.c_str() );
				}

				strTemp = "";
				nIdx++;
			}
			else
			{
				strTemp += chBuffer[i];
			}
		}

		m_sPatchConfig.sCurrVer.nBVer = (int)atoi( strTemp.c_str() );
	}

	if(pNode)
	{
		pNode->Release();
	}

	if(pAttrNodeList)
	{
		pAttrNodeList->Release();
	}

	return true;

Error_Exit:

	if(pNode)
	{
		pNode->Release();
	}

	if(pAttrNodeList)
	{
		pAttrNodeList->Release();
	}

	OutLog("LoadVersion Fail");
	
	return false;
}

bool CPatchConfigManager::LoadConfig(const char *pFileName)
{
	CNtlXMLDoc doc;
	doc.Create();

	if(doc.Load( (char*)pFileName ) == false)
	{
		goto Error_Exit;
	}

	char chBuffer[1024];
	int nValue;
	long length = 0;
	HRESULT hResult;

	IXMLDOMNode* pNode = NULL;
	IXMLDOMNodeList* pAttrNodeList = NULL;
	IXMLDOMNode* pCellNode = NULL;
    
	//-----------------------------------------------------------------------------
	// common
	pNode = doc.SelectSingleNode((char*)"/PatchConfig/Common");
	
	if(!doc.GetTextWithAttributeName(pNode, "DiffPatch", chBuffer, 1024))
	{
		goto Error_Exit;
	}

	nValue = (int)atoi(chBuffer);
    m_sPatchConfig.sCommon.bDiffPatch = nValue ? true : false;

	pNode->Release(); 

	//-----------------------------------------------------------------------------
	// old system
	pNode = doc.SelectSingleNode((char*)"/PatchConfig/OldSystem");
	if(!doc.GetTextWithAttributeName(pNode, "Folder", chBuffer, 1024))
	{
		goto Error_Exit;
	}
	m_sPatchConfig.sCopy.strOldSystem = chBuffer;
	
	pNode->Release(); 

	//-----------------------------------------------------------------------------
	// new system
	pNode = doc.SelectSingleNode((char*)"/PatchConfig/NewSystem");
	if(!doc.GetTextWithAttributeName(pNode, "Folder", chBuffer, 1024))
	{
		goto Error_Exit;
	}
	m_sPatchConfig.sCopy.strNewSystem = chBuffer;
	
	pNode->Release(); 
		
	//-----------------------------------------------------------------------------
	// source folder

	pNode = doc.SelectSingleNode((char*)"/PatchConfig/SourceFolder");
	hResult = pNode->get_childNodes(&pAttrNodeList);
	if(hResult != S_OK)
	{
		goto Error_Exit;
	}
	
	if (FAILED(pAttrNodeList->get_length(&length)))
	{
		goto Error_Exit;
	}
	
	for (long i=0 ; i<length ; i++)
	{
		if (FAILED(pAttrNodeList->get_item(i, &pCellNode)))
		{
			goto Error_Exit;
		}
		if(doc.GetTextWithAttributeName(pCellNode, "Build", chBuffer, 1024))
		{
			m_sPatchConfig.sCopy.strSrcFolder[ECDT_Build] = chBuffer;
		}
		else if(doc.GetTextWithAttributeName(pCellNode, "Res", chBuffer, 1024))
		{
			m_sPatchConfig.sCopy.strSrcFolder[ECDT_Res] = chBuffer;
		}
		else if(doc.GetTextWithAttributeName(pCellNode, "Exe", chBuffer, 1024))
		{
			m_sPatchConfig.sCopy.strSrcFolder[ECDT_Exe] = chBuffer;
		}
		else if(doc.GetTextWithAttributeName(pCellNode, "Dump", chBuffer, 1024))
		{
			m_sPatchConfig.sCopy.strSrcFolder[ECDT_Dump] = chBuffer;
		}
		else if(doc.GetTextWithAttributeName(pCellNode, "Localize", chBuffer, 1024))
		{
			m_sPatchConfig.sCopy.strSrcFolder[ECDT_Local] = chBuffer;
		}
		else if(doc.GetTextWithAttributeName(pCellNode, "Code", chBuffer, 1024))
		{
			m_sPatchConfig.sCopy.strSrcFolder[ECDT_Code] = chBuffer;
		}

		if(pCellNode)
		{
			pCellNode->Release();
			pCellNode = NULL;
		}
	}

	pNode->Release(); 

	//-----------------------------------------------------------------------------
	// rtpatch 경로
	pNode = doc.SelectSingleNode((char*)"/PatchConfig/RtPatch");
	if(!doc.GetTextWithAttributeName(pNode, "Exe", chBuffer, 1024))
	{
		goto Error_Exit;
	}
	m_sPatchConfig.sRtPatch.strRtPatchExe = chBuffer;

	pNode->Release(); 

	//-----------------------------------------------------------------------------
	// rtpatch export

	pNode = doc.SelectSingleNode((char*)"/PatchConfig/RtPatch");
	if(!doc.GetTextWithAttributeName(pNode, "Export", chBuffer, 1024))
	{
		goto Error_Exit;
	}
	m_sPatchConfig.sRtPatch.strRtPatchExport = chBuffer;

	pNode->Release(); 

	pNode = NULL;

	//-----------------------------------------------------------------------------
	// copy data
	pNode = doc.SelectSingleNode((char*)"/PatchConfig/CopyData");
	hResult = pNode->get_childNodes(&pAttrNodeList);
	if(hResult != S_OK)
	{
		goto Error_Exit;
	}
	
	if (FAILED(pAttrNodeList->get_length(&length)))
	{
		goto Error_Exit;
	}
	
	for (long i=0 ; i<length ; i++)
	{
		if (FAILED(pAttrNodeList->get_item(i, &pCellNode)))
		{
			goto Error_Exit;
		}

		if(doc.GetTextWithAttributeName(pCellNode, "Res", chBuffer, 1024))
		{
			SPCCopyData *pCopyData = new SPCCopyData;
			pCopyData->eCopyDataType = ECDT_Res;
			pCopyData->strName = chBuffer;
			m_sPatchConfig.sCopy.listCopyData.push_back(pCopyData);
		}
		else if(doc.GetTextWithAttributeName(pCellNode, "Exe", chBuffer, 1024))
		{
			SPCCopyData *pCopyData = new SPCCopyData;
			pCopyData->eCopyDataType = ECDT_Exe;
			pCopyData->strName = chBuffer;
			m_sPatchConfig.sCopy.listCopyData.push_back(pCopyData);
		}
		else if(doc.GetTextWithAttributeName(pCellNode, "Localize", chBuffer, 1024))
		{
			SPCCopyData *pCopyData = new SPCCopyData;
			pCopyData->eCopyDataType = ECDT_Local;
			pCopyData->strName = chBuffer;
			m_sPatchConfig.sCopy.listCopyData.push_back(pCopyData);
		}
		else if(doc.GetTextWithAttributeName(pCellNode, "Code", chBuffer, 1024))
		{
			SPCCopyData *pCopyData = new SPCCopyData;
			pCopyData->eCopyDataType = ECDT_Code;
			pCopyData->strName = chBuffer;
			m_sPatchConfig.sCopy.listCopyData.push_back(pCopyData);
		}
		
		if(pCellNode)
		{
			pCellNode->Release();
			pCellNode = NULL;
		}
	}

	return true;

Error_Exit:

	if(pNode)
	{
		pNode->Release(); 
	}

	if(pAttrNodeList)
	{
		pAttrNodeList->Release();
	}

	if(pCellNode)
	{
		pCellNode->Release();
	}

	OutLog("LoadConfig Fail");

	return false;
}

bool CPatchConfigManager::CopySource(bool bCode)
{
	if(!CopySourceAll())
		return false;

	return true;
}

bool CPatchConfigManager::TSEvtMakerBuild(void)
{
	std::string strSrc, strDest;

	strSrc = m_sPatchConfig.sCopy.strSrcFolder[ECDT_Exe];
	strSrc += "\\";
	strSrc += "TSEvtMaker.exe";

	strDest = m_sPatchConfig.sCopy.strSrcFolder[ECDT_Build];
	strDest += "\\";
	strDest += "TSEvtMaker.exe";

	if(CopyFile(strSrc, strDest) == FALSE)
		return false;

	Sleep(1000);

	std::string strTable, strTS, strBuildTemp, strCopy, strFull;

	strTable = m_sPatchConfig.sCopy.strSrcFolder[ECDT_Res];
	strTable += "\\data\\";

	strTS = m_sPatchConfig.sCopy.strSrcFolder[ECDT_Res];
	strTS += "\\ts\\";

	strCopy = m_sPatchConfig.sCopy.strNewSystem;
	strCopy += "\\ts\\";

	char chBuffer[2048];
	::GetCurrentDirectory(2048, chBuffer);

	strBuildTemp = chBuffer;
	strBuildTemp += "\\TSEvtTemp\\";

	strFull = strTable;
	strFull += ";";
	strFull += strTS;
	strFull += ";";
	strFull += strBuildTemp;
	strFull += ";";
	strFull += strCopy;

	CreateDirectory(strBuildTemp.c_str(), NULL);
	Sleep(1000);

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );			

	std::string strCommandLine = "TSEvtMaker.exe ";
	strCommandLine += strFull;

	OutLog(strCommandLine.c_str());
	
	if( !CreateProcess( NULL, (LPSTR)strCommandLine.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi ) )
	{
		OutLog("TSEvtMakerBuild::CreateProcess Fail");
		return false;
	}

	while(1)
	{
		if(IsExistBuildProcess("TSEvtMaker.exe"))
		{
			Sleep(TSEVTMAKER_BUILD_PROCESS_SLEEP);
		}
		else
		{
			break;
		}
	}

	DWORD dwExitCode;
	::GetExitCodeProcess( pi.hProcess, &dwExitCode );

	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );

	if(dwExitCode != 0)
	{
		OutLog("TSEvtMaker Result Code Fail");
		return false;
	}

	return true;
}

bool CPatchConfigManager::SaveVersion(const char *pFileName)
{
	CNtlXMLDoc doc;
	doc.Create();

	char chBuffer[128];
	std::string strDate;

	HRESULT hr;
	CComVariant varValue;
	CComBSTR bstrName;
	CComBSTR bstrNamespaceURI;
	CComVariant bstrSaveFile = pFileName;

	if(doc.Load( (char*)pFileName ) == false)
	{
		goto Error_Exit;
	}

	IXMLDOMNode* pNode = NULL;
	IXMLDOMNode* pNewNode = NULL;
	IXMLDOMNode* pNewRefNode = NULL;
	IXMLDOMAttribute* pXMLAttribute = NULL;
	IXMLDOMNamedNodeMap* pNamedNodeMap = NULL;

	//-----------------------------------------------------------------------------
	// client diff patch

	IXMLDOMDocument *pXMLDoc = doc.GetDocument();
	pNode = doc.SelectSingleNode((char*)"/PatchList/ClientDiffRtPatch");

	// 새로운 node 생성.
	varValue = NODE_ELEMENT;
	bstrName = "Data";
	bstrNamespaceURI = "";
	hr = pXMLDoc->createNode(varValue, bstrName, bstrNamespaceURI, &pNewNode);
	if(FAILED(hr)) goto Error_Exit;

	hr = pNode->appendChild(pNewNode, &pNewRefNode);
	if(FAILED(hr)) goto Error_Exit;
	
	hr = pNewRefNode->get_attributes(&pNamedNodeMap);
	if(FAILED(hr)) goto Error_Exit;

	if(pNewRefNode)
	{
		pNewRefNode->Release();
	}

	// Ver attribute
	bstrName = "Ver";
	hr = pXMLDoc->createAttribute(bstrName, &pXMLAttribute);
	if(FAILED(hr)) goto Error_Exit;

	hr = pNamedNodeMap->setNamedItem(pXMLAttribute, &pNewRefNode);
	if(FAILED(hr)) goto Error_Exit;
	
	int nLVer = m_sPatchConfig.sCurrVer.nLVer;
	int nRVer = m_sPatchConfig.sCurrVer.nRVer;
	int nBVer = m_sPatchConfig.sCurrVer.nBVer + 1;

	sprintf_s(chBuffer, "%d.%d.%d", nLVer, nRVer, nBVer);
	varValue = chBuffer;
	hr = pNewRefNode->put_nodeValue(varValue);
	if(FAILED(hr)) goto Error_Exit;

	if(pNewRefNode)
	{
		pNewRefNode->Release();
	}

	// PatchFile attribute
	bstrName = "PatchFile";
	hr = pXMLDoc->createAttribute(bstrName, &pXMLAttribute);
	if(FAILED(hr)) goto Error_Exit;

	hr = pNamedNodeMap->setNamedItem(pXMLAttribute, &pNewRefNode);
	if(FAILED(hr)) goto Error_Exit;

	sprintf_s(chBuffer, "%d.%d.%d.RTP", nLVer, nRVer, nBVer);
	varValue = chBuffer;
	hr = pNewRefNode->put_nodeValue(varValue);
	if(FAILED(hr)) goto Error_Exit;

	if(pNewRefNode)
	{
		pNewRefNode->Release();
	}

	// Date attribute
	bstrName = "Date";
	hr = pXMLDoc->createAttribute(bstrName, &pXMLAttribute);
	if(FAILED(hr)) goto Error_Exit;

	hr = pNamedNodeMap->setNamedItem(pXMLAttribute, &pNewRefNode);
	if(FAILED(hr)) goto Error_Exit;

	strDate = GetDate();
//	sprintf_s(chBuffer, "%d.%d.%d.rtp", nLVer, nRVer, nBVer);
	varValue = strDate.c_str();
	hr = pNewRefNode->put_nodeValue(varValue);
	if(FAILED(hr)) goto Error_Exit;

	if(pNewRefNode)
	{
		pNewRefNode->Release();
	}

	// add text node
	varValue = NODE_TEXT;
	bstrName = "";
	bstrNamespaceURI = "";
	hr = pXMLDoc->createNode(varValue, bstrName, bstrNamespaceURI, &pNewNode);
	if(FAILED(hr)) goto Error_Exit;

	hr = pNode->appendChild(pNewNode, &pNewRefNode);
	if(FAILED(hr)) goto Error_Exit;

	varValue = "\n\t";
	pNewRefNode->put_nodeValue(varValue);

	// 저장..
	pXMLDoc->save(bstrSaveFile);

	if(pNode)
	{
		pNode->Release();
	}

	if(pNamedNodeMap)
	{
		pNamedNodeMap->Release();
	}

	if(pNewRefNode)
	{
		pNewRefNode->Release();
	}

	return true;

Error_Exit:

	if(pNode)
	{
		pNode->Release();
	}

	if(pNamedNodeMap)
	{
		pNamedNodeMap->Release();
	}

	if(pNewRefNode)
	{
		pNewRefNode->Release();
	}

	OutLog("SaveVersion Fail");

	return false;
}


bool CPatchConfigManager::SaveFTPUploadScript(const char *pFileName)
{
	if(!m_FTPUploadScript.Load(pFileName))
	{
		OutLog("SaveFTPUploadScript::m_FTPUploadScript.Load Fail");
		return false;
	}

	char chBuffer[128];
	sprintf_s(chBuffer, 128, "%d.%d.%d", m_sPatchConfig.sCurrVer.nLVer, m_sPatchConfig.sCurrVer.nRVer, m_sPatchConfig.sCurrVer.nBVer+1);

	std::string str = chBuffer;
	str += ".RTP";
	
	if( !m_FTPUploadScript.Run("put", str.c_str()) )
	{
		OutLog("SaveFTPUploadScript::m_FTPUploadScript.Run Fail");
		return false;
	}

	if(!m_FTPUploadScript.Save(pFileName))
	{
		OutLog("SaveFTPUploadScript::m_FTPUploadScript.Save Fail");
		return false;
	}

	return true;
}

bool CPatchConfigManager::SavePatchCommandScript(const char *pFileName)
{
	if(!m_PatchCmdScript.Load(pFileName))
	{
		OutLog("SavePatchCommandScript::m_PatchCmdScript.Load Fail");
		return false;
	}

	char chBuffer[128];
	sprintf_s(chBuffer, 128, "%d.%d.%d", m_sPatchConfig.sCurrVer.nLVer, m_sPatchConfig.sCurrVer.nRVer, m_sPatchConfig.sCurrVer.nBVer+1);

	std::string str = chBuffer;
	str += ".RTP";
	
	if( !m_PatchCmdScript.Run("OUTPUT", str.c_str()) )
	{
		OutLog("SavePatchCommandScript::m_PatchCmdScript.Run Fail");
		return false;
	}

	if(!m_PatchCmdScript.Save(pFileName))
	{
		OutLog("SavePatchCommandScript::m_PatchCmdScript.Save Fail");
		return false;
	}

	return true;
}


bool CPatchConfigManager::IsExistBuildProcess(const char *pName)
{
	PROCESSENTRY32 processEntry;
	processEntry.dwSize = sizeof(PROCESSENTRY32);

	// Takes a snapshot of all processes in the system.
	HANDLE hProcessSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hProcessSnapshot)
	{
		return false;
	}

	// Retrieves information about the first process.
	if (FALSE == ::Process32First(hProcessSnapshot, &processEntry))
	{
		DWORD dwErrorCode = ::GetLastError();
		if (ERROR_NO_MORE_FILES == dwErrorCode)
		{
			::CloseHandle(hProcessSnapshot);
			return false;
		}
		else
		{
			::CloseHandle(hProcessSnapshot);
			return false;
		}
	}

	// Trace all information about the processes.
	do
	{
		std::string str = processEntry.szExeFile;
		if(str == pName)
			return true;

	} while(FALSE != ::Process32Next(hProcessSnapshot, &processEntry));

	::CloseHandle(hProcessSnapshot);

	return false;
}



bool CPatchConfigManager::RtPatchBuild(void)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );			

	std::string strCommandLine = m_sPatchConfig.sRtPatch.strRtPatchExe;
	strCommandLine += " @DBOPatch.cmd";
	
	if( !CreateProcess( NULL, (LPSTR)strCommandLine.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi ) )
	{
		OutLog("RtPatchBuild::CreateProcess Fail");
		return false;
	}

	while(1)
	{
		if(IsExistBuildProcess("patchbld.exe"))
		{
			Sleep(RTPATCH_BUILD_PROCESS_SLEEP);
		}
		else
		{
			break;
		}
	}

	DWORD dwExitCode;
	::GetExitCodeProcess( pi.hProcess, &dwExitCode );

	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );

	if(dwExitCode != 0)
	{
		OutLog("RtPatchBuild Fail !!!");
		return false;
	}
	
	if(!FindPatchData())
	{
		OutLog("RtPatchBuild::FindPatchData Fail");
		return false;
	}

	return true;
}

bool CPatchConfigManager::EndPatchBuild(BOOL bFastMode /* = FALSE */)
{
	if(!CopyDump(bFastMode))
	{
		OutLog("EndPatchBuild::CopyDump Fail");
		return false;
	}

	if(!RenameOldSystem())
	{
		OutLog("EndPatchBuild::RenameOldSystem Fail");
		return false;
	}

	if(!RenameNewSystem())
	{
		OutLog("EndPatchBuild::RenameNewSystem Fail");
		return false;
	}

	return true;
}


bool CPatchConfigManager::LoadServerConfig(const char *pFileName)
{
	CNtlXMLDoc doc;
	doc.Create();

	if(doc.Load( (char*)pFileName ) == false)
	{
		goto Error_Exit;
	}

    char chBuffer[1024] = {0,};	
	long length = 0;
	HRESULT hResult;

	IXMLDOMNode* pNode = NULL;
	IXMLDOMNodeList* pAttrNodeList = NULL;
	IXMLDOMNode* pCellNode = NULL;
    
	//-----------------------------------------------------------------------------
	// new system
	pNode = doc.SelectSingleNode((char*)"/PatchConfig/NewSystem");
	if(!doc.GetTextWithAttributeName(pNode, "Folder", chBuffer, 1024))
	{
		goto Error_Exit;
	}
	m_sServerPatchConfig.sCopy.strNewSystem = chBuffer;
	
	pNode->Release(); 
		
	//-----------------------------------------------------------------------------
	// source folder

	pNode = doc.SelectSingleNode((char*)"/PatchConfig/SourceFolder");
	hResult = pNode->get_childNodes(&pAttrNodeList);
	if(hResult != S_OK)
	{
		goto Error_Exit;
	}
	
	if (FAILED(pAttrNodeList->get_length(&length)))
	{
		goto Error_Exit;
	}
	
	for (long i=0 ; i<length ; i++)
	{
		if (FAILED(pAttrNodeList->get_item(i, &pCellNode)))
		{
			goto Error_Exit;
		}

		if(doc.GetTextWithAttributeName(pCellNode, "Res", chBuffer, 1024))
		{
			m_sServerPatchConfig.sCopy.strSrcFolder[ECDT_Res] = chBuffer;
		}
		else if(doc.GetTextWithAttributeName(pCellNode, "Exe", chBuffer, 1024))
		{
			m_sServerPatchConfig.sCopy.strSrcFolder[ECDT_Exe] = chBuffer;
		}
		else if(doc.GetTextWithAttributeName(pCellNode, "Code", chBuffer, 1024))
		{
			m_sServerPatchConfig.sCopy.strSrcFolder[ECDT_Code] = chBuffer;
		}

		if(pCellNode)
		{
			pCellNode->Release();
			pCellNode = NULL;
		}
	}

	pNode->Release(); 

	
	//-----------------------------------------------------------------------------
	// copy data
	pNode = doc.SelectSingleNode((char*)"/PatchConfig/CopyData");
	hResult = pNode->get_childNodes(&pAttrNodeList);
	if(hResult != S_OK)
	{
		goto Error_Exit;
	}
	
	if (FAILED(pAttrNodeList->get_length(&length)))
	{
		goto Error_Exit;
	}
	
	for (long i=0 ; i<length ; i++)
	{
		if (FAILED(pAttrNodeList->get_item(i, &pCellNode)))
		{
			goto Error_Exit;
		}

		if(doc.GetTextWithAttributeName(pCellNode, "Res", chBuffer, 1024))
		{
			SPCCopyData *pCopyData = new SPCCopyData;
			pCopyData->eCopyDataType = ECDT_Res;
			pCopyData->strName = chBuffer;
			m_sServerPatchConfig.sCopy.listCopyData.push_back(pCopyData);
		}
		else if(doc.GetTextWithAttributeName(pCellNode, "Exe", chBuffer, 1024))
		{
			SPCCopyData *pCopyData = new SPCCopyData;
			pCopyData->eCopyDataType = ECDT_Exe;
			pCopyData->strName = chBuffer;
			m_sServerPatchConfig.sCopy.listCopyData.push_back(pCopyData);
		}
		else if(doc.GetTextWithAttributeName(pCellNode, "Code", chBuffer, 1024))
		{
			SPCCopyData *pCopyData = new SPCCopyData;
			pCopyData->eCopyDataType = ECDT_Code;
			pCopyData->strName = chBuffer;
			m_sServerPatchConfig.sCopy.listCopyData.push_back(pCopyData);
		}
		
		if(pCellNode)
		{
			pCellNode->Release();
			pCellNode = NULL;
		}
	}

	return true;

Error_Exit:

	if(pNode)
	{
		pNode->Release(); 
	}

	if(pAttrNodeList)
	{
		pAttrNodeList->Release();
	}

	if(pCellNode)
	{
		pCellNode->Release();
	}

	OutLog("LoadConfig Fail");

	return false;
}

bool CPatchConfigManager::MakeServerBuildFolder(void)
{
	m_sServerPatchConfig.strServerBuildFolder = m_sServerPatchConfig.sCopy.strNewSystem;

	m_sServerPatchConfig.strServerBuildFolder += "\\BuildData(";
	m_sServerPatchConfig.strServerBuildFolder += GetDataTime();
	m_sServerPatchConfig.strServerBuildFolder += ")";

	CreateDirectory(m_sServerPatchConfig.strServerBuildFolder.c_str(), NULL);

	return true;
}


bool CPatchConfigManager::CopyServerSource(bool bCode)
{
	std::string strStorageName = m_sServerPatchConfig.strServerBuildFolder;

	if(bCode)
	{
		strStorageName += "\\DragonBall_Beta";
	}
	else
	{
		strStorageName += "\\DBO_Resource";
	}

	CreateDirectory(strStorageName.c_str(), NULL);

	std::list<SPCCopyData*>::iterator it;

	SPCCopyData *pCopyData;
	std::string strSrc, strDest;

	for(it = m_sServerPatchConfig.sCopy.listCopyData.begin(); it != m_sServerPatchConfig.sCopy.listCopyData.end(); it++)
	{
		pCopyData = (*it);

		if(bCode)
		{
			if(pCopyData->eCopyDataType != ECDT_Code)
				continue;
		}
		else
		{
			if(pCopyData->eCopyDataType == ECDT_Code)
				continue;
		}

		if(m_sServerPatchConfig.sCopy.strSrcFolder[pCopyData->eCopyDataType].size() <= 0)
			continue;

		CreateSubDirectory(strStorageName.c_str(), pCopyData->strName.c_str());

		strSrc = m_sServerPatchConfig.sCopy.strSrcFolder[pCopyData->eCopyDataType];
		strSrc += "\\";
		strSrc += pCopyData->strName;

		strDest = strStorageName;
		strDest += "\\";
		strDest += pCopyData->strName;
	
        if( IsFolder(strSrc.c_str()) )
        {
            if(!CopyFolder(strSrc.c_str(), strDest.c_str()))
            {
                char log[256] = {0,};
                sprintf_s(log, "Folder Copy Fail %s to %s", strSrc.c_str(), strDest.c_str());
                OutLog(log);

                return false;
            }
        }
        else
        {
            if(CopyFile(strSrc, strDest) == FALSE)
            {
                char log[256] = {0,};
                sprintf_s(log, "File Copy Fail %s to %s", strSrc.c_str(), strDest.c_str());
                OutLog(log);

                return false;
            }
        }
	}

	return true;
}


SServerPatchConfig*	CPatchConfigManager::GetServerPatchConfig(void)
{
	return &m_sServerPatchConfig;
}


void CPatchConfigManager::OutLog(const char *pLog)
{
	CAutoPacthLog log;
	log.Out("AutoPatchLog.txt", pLog);
}

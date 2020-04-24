#include "StdAfx.h"
#include "ERSMWorker.h"
#include "ERSMConfig.h"
#include "ERSMDef.h"
#include "NtlUnzip.h"

CERSMWorker::CERSMWorker(void)
{
	m_vecErrorReportFiles.reserve(100);
}

CERSMWorker::~CERSMWorker(void)
{
	Clear();
}

void CERSMWorker::Clear() 
{
	for(UINT i = 0; i < m_vecErrorReportFiles.size(); ++i)
	{
		delete m_vecErrorReportFiles[i];
	}

	m_vecErrorReportFiles.clear();
}

bool CERSMWorker::ProcessErrReports() 
{
	Clear();

	if(!GetListZipFiles(NCGetSGT(ERSMConfig)->GetERSFolder()))
		return FALSE;

	cout<<"--- Get Error Report List(Zip Files) Success ---\n";

	// 처리할 파일이 없으면 True를 반환하고 끝낸다.
	if(m_vecErrorReportFiles.empty())
	{
		cout<<"--- Error Report Files Empty --- "<<GetTime()<<"\n";
		return TRUE;
	}

	if(!RenameZipFiles())
		return FALSE;

	cout<<"--- Rename Error Report Files Success ---"<<GetTime()<<"\n";

	return TRUE;
}

bool CERSMWorker::GetListZipFiles( const std::string& strFolderName ) 
{
	BOOL bResult = TRUE;
	WIN32_FIND_DATA wfd;
	std::string strFilter;
	std::string strFolder;
	HANDLE hSerach;	
	char buf[ERSM_BUF_SIZE] = {0,}; 
	SYSTEMTIME timeFile;
	std::vector<std::string> vecFolderName;
	
	// 1. 하위 폴더를 검색한다 (에러리포트 파일은 루트 폴더밑의 1단계 하위폴더에 존재한다)
	strFilter = strFolderName + "\\*.*";
	hSerach = FindFirstFile(strFilter.c_str(), &wfd);
	if(hSerach == INVALID_HANDLE_VALUE)
		return FALSE;

	while(bResult)
	{
		if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)	// 폴더인 경우
		{
			if(strcmp(wfd.cFileName, ".") != 0 &&
			   strcmp(wfd.cFileName, "..") != 0)
			{
				vecFolderName.push_back(wfd.cFileName);
			}
			
		}

		bResult = FindNextFile(hSerach, &wfd);
	}
	FindClose(hSerach);

	// 2. 하위 폴더들에서 ZipFile들을 검색한다.	
	for(UINT i = 0; i < vecFolderName.size(); ++i)
	{		
		strFolder = NCGetSGT(ERSMConfig)->GetERSFolder() + "\\";
		strFolder += vecFolderName[i];
		strFolder += "\\";		
		strFilter = strFolder + "*.zip";
		
		bResult = TRUE;

		hSerach = FindFirstFile(strFilter.c_str(), &wfd);
		if(hSerach == INVALID_HANDLE_VALUE)
			continue;

		while(bResult)
		{
			// 파일정보를 리스트에 담는다.
			SErrorReportFile* pERF = new SErrorReportFile();
			m_vecErrorReportFiles.push_back(pERF);

			// 폴더 이름
			pERF->m_strFolderName = strFolder;
			
			// 파일 이름			
			pERF->m_strFileName =  wfd.cFileName;
			
			// 파일 생성 날짜
			FileTimeToSystemTime(&wfd.ftLastWriteTime, &timeFile);
			sprintf_s(buf, "%d%02d%02d", timeFile.wYear, timeFile.wMonth, timeFile.wDay);
			pERF->m_strFileDate = buf;

			// 파일안에 포함된 유저 아이디
			std::string strFullPathName = strFolder + wfd.cFileName;
			pERF->m_strUserID = GetUserIDFromZipFile(strFullPathName);

			bResult = FindNextFile(hSerach	, &wfd);
			Sleep(0);
		}
		FindClose(hSerach);
	}

	return TRUE;
}

std::string CERSMWorker::GetUserIDFromZipFile( const std::string& strZipFileName ) 
{
	std::string strUserID;

	// 1. Zip File로부터 UserID정보를 가지고 있는 로그 파일을 꺼낸다.
	sUZ_FILEINFO uzFileInfo;
	ZeroMemory(&uzFileInfo, sizeof(uzFileInfo));

	char buf[ERSM_FILE_BUF_SIZE] = {0,};
	char szUserID[ERSM_BUF_SIZE] = {0,};

	CNtlUnzip unZip(strZipFileName.c_str());	
	bool bResult = unZip.GotoFirstFile();
	while(bResult)
	{
		ZeroMemory(buf, ERSM_FILE_BUF_SIZE);

		if(!unZip.GetFileInfo(uzFileInfo))
		{
			bResult = unZip.GotoNextFile();
			continue;
		}

		if(strcmp(uzFileInfo.szFileName, ERSM_ID_FILE_NAME) != 0)
		{
			bResult = unZip.GotoNextFile();
			continue;
		}

		if(!unZip.ReadFileData(buf, uzFileInfo.dwUncompressedSize))
		{
			bResult = unZip.GotoNextFile();
			continue;
		}

		// 2. 압축을 푼 파일(XML)로부터 유저ID를 읽는다.
		CNtlXMLDoc xmlDoc;
		if(xmlDoc.Create())
		{
			if(xmlDoc.LoadXML(buf))
			{
				ZeroMemory(szUserID, ERSM_BUF_SIZE);

				xmlDoc.GetDataWithXPath(ERSM_NODE_USER_ID, szUserID, ERSM_BUF_SIZE);

				strUserID = szUserID;
				break;
			}
		}

		bResult = unZip.GotoNextFile();
	}
	
	return strUserID;
}

bool CERSMWorker::RenameZipFiles() 
{
	for(UINT i = 0; i < m_vecErrorReportFiles.size(); ++i)
	{
		SErrorReportFile* pERF = m_vecErrorReportFiles[i];
		if(!pERF)
			continue;

		// 날짜별 폴더를 생성한다. (이미 폴더가 존재하고 있으면 False를 리턴하는데, 상관없다)
		std::string strNewFolder = pERF->m_strFolderName + pERF->m_strFileDate;
		CreateDirectory(strNewFolder.c_str(), NULL);

		// 생성된 폴더에 파일 이름을 변경하여 넣는다.
		std::string strOldFileName = pERF->m_strFolderName + pERF->m_strFileName;
		std::string strNewFileName = strNewFolder;
		strNewFileName += "\\";
		strNewFileName += pERF->m_strUserID + "_";
		strNewFileName += pERF->m_strFileName;
		
		rename(strOldFileName.c_str(), strNewFileName.c_str());

		cout<<"# Rename Process Success--- ["<<strOldFileName<<"] to ["<<strNewFileName<<"]"<<GetTime()<<"\n";

	}
	return TRUE;
}

std::string CERSMWorker::GetTime() 
{
	CTime ctCurrentTime = CTime::GetCurrentTime();
	
	char buf[64] = {0,};
	sprintf_s(buf, "[%d-%02d-%02d %2d:%2d]", ctCurrentTime.GetYear(), ctCurrentTime.GetMonth(), ctCurrentTime.GetDay(), ctCurrentTime.GetHour(), ctCurrentTime.GetMinute());

	return buf;
}
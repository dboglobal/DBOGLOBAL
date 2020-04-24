#include "precomp_ntlpresentation.h"
#include "FileController.h"
#include "NtlCoreUtil.h"


CFileController::CFileController(void)
{
}

CFileController::~CFileController(void)
{
}

VOID CFileController::ExtractFileName(string& FullPathFileName, string& FileName)
{
	char FileNameTemp[NTL_MAX_DIR_PATH] = {0,};
	int CharCount = static_cast<int>(FullPathFileName.find_last_of("\\") + 1);
	//FullPathFileName.copy(FileNameTemp, FullPathFileName.size() - CharCount, CharCount);
	FullPathFileName._Copy_s(FileNameTemp, FullPathFileName.size() - CharCount, CharCount);    
    
	FileName = FileNameTemp;

	int DotCount = static_cast<int>(FileName.find_last_of('.'));
	FileName.erase(DotCount, FullPathFileName.size());
}

VOID CFileController::ExtractFile2ExtName(string& FullPathFileName, string& File2ExtName)
{
	char FileNameTemp[NTL_MAX_DIR_PATH] = {0,};
	int CharCount = static_cast<int>(FullPathFileName.find_last_of("\\") + 1);
	//FullPathFileName.copy(FileNameTemp, FullPathFileName.size() - CharCount, CharCount);
    FullPathFileName._Copy_s(FileNameTemp, FullPathFileName.size() - CharCount, CharCount);
	File2ExtName = FileNameTemp;
}

VOID CFileController::ExtractFolderNameWithLastBSRemoved(string& FullPathFileName, string& FolderName)
{
	char FolderNameTemp[NTL_MAX_DIR_PATH] = {0,};
	int TotCharCount	= FullPathFileName.size();
	int CharCount		= static_cast<int>(FullPathFileName.find_last_of("\\") + 1);
	//FullPathFileName.copy(FolderNameTemp, TotCharCount - 1, 0);
    FullPathFileName._Copy_s(FolderNameTemp, TotCharCount - 1, 0);
	FolderName = FolderNameTemp;

	string Result;
	ExtractFile2ExtName(FolderName, Result);
	FolderName = Result;
}

VOID CFileController::ExtractFolderName(string& FullPathFileName, string& FolderName)
{
	char FolderNameTemp[NTL_MAX_DIR_PATH] = {0,};
	int CharCount = static_cast<int>(FullPathFileName.find_last_of("\\") + 1);
	//FullPathFileName.copy(FolderNameTemp, CharCount, 0);
    FullPathFileName._Copy_s(FolderNameTemp, CharCount, 0);

	FolderName = FolderNameTemp;
}

RwBool CFileController::FileExistenceInFolder(string& FullPathFolderName)
{
	HANDLE FileHandle = CreateFile(FullPathFolderName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if(FileHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(FileHandle);
		return true;
	}

	return false;
}

RwBool CFileController::DeleteFile(const char* pPathName)
{
	if((_access(pPathName, 0)) != -1)
	{
		// The file exists, force it to do write permission
		SetFileAttributes(pPathName, FILE_ATTRIBUTE_ARCHIVE);
		::DeleteFile(pPathName);

		return TRUE;
	}

	return FALSE;
}

RwBool CFileController::CopyFile(const char* pSrcPathName, const char* pDstPathName)
{
	DWORD dwAttrs = ::GetFileAttributes(pDstPathName);
	if(dwAttrs == INVALID_FILE_ATTRIBUTES)
	{
		return FALSE;
	}

	if(dwAttrs & FILE_ATTRIBUTE_READONLY) 
	{ 
		dwAttrs &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(pDstPathName, dwAttrs); 
	} 

	if(dwAttrs & FILE_ATTRIBUTE_HIDDEN) 
	{
		dwAttrs &= ~FILE_ATTRIBUTE_HIDDEN;
		::SetFileAttributes(pDstPathName, dwAttrs); 
	}

	if(!::CopyFile(pSrcPathName, pDstPathName, FALSE))
	{
		return FALSE;
	}

	return TRUE;
}

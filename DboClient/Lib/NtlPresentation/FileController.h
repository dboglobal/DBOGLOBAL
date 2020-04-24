#pragma once


#include <string>
using std::string;


class CFileController
{
public:
	CFileController(void);
	~CFileController(void);

public:
	RwBool	FileExistenceInFolder(string& FullPathFolderName);
	VOID	ExtractFileName(string& FullPathFileName, string& FileName);
	VOID	ExtractFile2ExtName(string& FullPathFileName, string& File2ExtName);
	VOID	ExtractFolderName(string& FullPathFileName, string& FolderName);
	VOID	ExtractFolderNameWithLastBSRemoved(string& FullPathFileName, string& FolderName);
	RwBool	DeleteFile(const char* pPathName);
	RwBool	CopyFile(const char* pSrcPathName, const char* pDstPathName);
};

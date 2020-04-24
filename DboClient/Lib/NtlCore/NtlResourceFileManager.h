//***********************************************************************************
//	File		:	NtlResourceFileManager.h
//	Desc		:	
//	Begin		:	2004.9.23
//	Copyright	:	ⓒ 2004 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************
#pragma once

#pragma warning(disable:4346) 
#pragma warning(disable:4183) 
#pragma warning(disable:4267) 

#include <Winsock2.h>
#include <windows.h>
#include <vector>
#include <string>
#include <map>

#include "rwcore.h"

#include "NtlDebug.h"


struct STRING_COMPARE_NOCASE : public std::binary_function<std::string, std::string, bool>
{
	static bool compare_nocase(wchar_t c1, wchar_t c2)
	{
		return toupper(c1) < toupper(c2);
	}
	bool operator() (const std::string& lhs, const std::string& rhs) const
	{
		return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), compare_nocase);
	}
};

class CNtlResourceFileManager
{
protected:
public:
	typedef std::map<std::string, std::string, STRING_COMPARE_NOCASE>	smdef_Entity;

	smdef_Entity		m_smEntity;
	std::string			m_strPath;

	typedef std::vector<std::string>	svdef_String;
	svdef_String		m_svExt;

public:
//				CNtlResourceFileManager()					{ }

	RwInt32					size()				{ return m_smEntity.size(); }
	RwBool					empty()				{ return m_smEntity.empty() ? TRUE: FALSE; }

protected:
public:
	virtual		~CNtlResourceFileManager()				{ }

	RwBool Destroy()
	{
		m_smEntity.clear();
		return TRUE;
	}

	void AddExt(const RwChar* strExt)
	{
		m_svExt.push_back(strExt);
	}
	void SetPath(const RwChar* strPath)
	{
		m_strPath = strPath;
	}

	RwBool IsExt(const RwChar* strFileName)
	{
		if (m_svExt.empty()) return TRUE;

		RwChar strExt[MAX_PATH], strName[NTL_MAX_DIR_PATH];

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
		strcpy_s(strName, NTL_MAX_DIR_PATH, strFileName);
		_strlwr_s(strName, NTL_MAX_DIR_PATH);
#else
		strcpy(strName, strFileName);
		strlwr(strName);
#endif

		svdef_String::iterator it;
		for (it = m_svExt.begin(); it != m_svExt.end(); ++ it)
		{
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
			strcpy_s(strExt, MAX_PATH, (*it).c_str());
			_strlwr_s(strExt, MAX_PATH);
#else
			strcpy(strExt, (*it).c_str());
			strlwr(strExt);
#endif
	
			if (strstr(strName, strExt) != NULL)
			{
				return TRUE;
			}
		}
		return FALSE;
	}


	const RwChar* GetPathName(const RwChar* strFileName)
	{
		smdef_Entity::iterator it = m_smEntity.find(strFileName);
		if (it != m_smEntity.end())
		{
			return (*it).second.c_str();
		}
		return NULL;
	}

	RwBool IsFile(const RwChar* strFileName)
	{
		return GetPathName(strFileName) ? TRUE : FALSE;
	}

	void InsertFile(const RwChar* strFileName, const RwChar* strPathName)
	{
		if (IsFile(strFileName) == FALSE)
		{
			m_smEntity.insert(smdef_Entity::value_type(strFileName, strPathName));
		}
	}

	//------------------------------------------------------------------
	//	FuncName	: LoadPath()
	//	Desc		: 
	//	Parameter	: 
	//	Return		: 
	//------------------------------------------------------------------
	RwBool LoadPath(const RwChar* path)
	{
		m_strPath = path;
		RwChar strCurDir[NTL_MAX_DIR_PATH];
		GetCurrentDirectory(NTL_MAX_DIR_PATH, strCurDir);

		if (!SetCurrentDirectory(path))
		{
			return FALSE;
		}

		SubLoadPath();

		SetCurrentDirectory(strCurDir);
		return TRUE;
	}

	void SubLoadPath()
	{
		RwChar strCurPath[NTL_MAX_DIR_PATH];
		GetCurrentDirectory(NTL_MAX_DIR_PATH, strCurPath);

		std::string strFullFileName;

		WIN32_FIND_DATA FindFileData;
		HANDLE hFile = FindFirstFile("*.*", &FindFileData);
		if(hFile == INVALID_HANDLE_VALUE) return;

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
					SubLoadPath();
				}
			} 
			else
			{
				if (IsExt(FindFileData.cFileName))
					InsertFile(FindFileData.cFileName, strFullFileName.c_str());
			}
		} while(FindNextFile(hFile, &FindFileData));

		FindClose(hFile);
	}
};

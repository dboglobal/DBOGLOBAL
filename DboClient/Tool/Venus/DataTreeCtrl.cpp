// DataTreeCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Venus.h"
#include "DataTreeCtrl.h"
#include ".\datatreectrl.h"

#include <map>

// CDataTreeCtrl

IMPLEMENT_DYNAMIC(CDataTreeCtrl, CTreeCtrl)
CDataTreeCtrl::CDataTreeCtrl()
{
}

CDataTreeCtrl::~CDataTreeCtrl()
{
	m_ImageList.Detach();
}


BEGIN_MESSAGE_MAP(CDataTreeCtrl, CTreeCtrl)
END_MESSAGE_MAP()



// CDataTreeCtrl 메시지 처리기입니다.

BOOL CDataTreeCtrl::InitTreeCtrl()
{
	SHFILEINFO shFinfo;
	HIMAGELIST hImgList = NULL;
	hImgList = (HIMAGELIST)SHGetFileInfo( "C:\\",
							  0,
							  &shFinfo,
							  sizeof( shFinfo ),
							  SHGFI_SYSICONINDEX | 
							  SHGFI_SMALLICON );

	m_ImageList.m_hImageList = hImgList;    
	SetImageList( &m_ImageList, TVSIL_NORMAL );

	return TRUE;
}

void CDataTreeCtrl::AddExt(const char* strExt)
{
	m_svExt.push_back(strExt);
}

//------------------------------------------------------------------
//	FuncName	: SerializeList
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CDataTreeCtrl::SerializeList(CString strPath)
{
	DeleteAllItems();

	char strCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, strCurrentDir);

	if (!SetCurrentDirectory(strPath))
	{
		return;
	}

	SerializeList(NULL);

	SetCurrentDirectory(strCurrentDir);
}

void CDataTreeCtrl::SerializeList(HTREEITEM hParentItem)
{
	HTREEITEM hItem;
	int iIcon, iIconSel;
	char strCurPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, strCurPath);

	std::string strFullFileName;

	WIN32_FIND_DATA FindFileData;
	HANDLE hFile = FindFirstFile("*.*", &FindFileData);
	if(hFile == INVALID_HANDLE_VALUE) return;

	typedef std::map<std::string, std::string>	smdef_File;
	smdef_File smList;

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
				BOOL ret = SetCurrentDirectory(strFullFileName.c_str());
	            hItem = InsertItem(FindFileData.cFileName, hParentItem);

				GetIconIndex(iIcon, iIconSel, strFullFileName.c_str());

				SetItemImage(hItem, iIcon, iIconSel);
				SerializeList(hItem);
			}
		} 
		else
		{
			if (IsExt(FindFileData.cFileName))
			{
				smList.insert(smdef_File::value_type(FindFileData.cFileName, strFullFileName.c_str()));
			}
		}
	} while(FindNextFile(hFile, &FindFileData));

	// 리스트에 담아 놓은 파일 리스트를 순서대로 출력한다.
	smdef_File::iterator it;
	for (it = smList.begin(); it != smList.end(); ++ it)
	{
		std::string strFile = (*it).first.c_str();
		hItem = InsertItem(strFile.c_str(), hParentItem);

		GetIconIndex(iIcon, iIconSel, (*it).second.c_str());
		SetItemImage(hItem, iIcon, iIconSel);
	}

	FindClose(hFile);
}

BOOL CDataTreeCtrl::GetIconIndex(int& iIcon, int& iIconSel, LPCTSTR strPath)
{
	// Adding the Item to the TreeCtrl with the current Icons
	SHFILEINFO shFinfo;
    CString    strTemp = strPath;
    
	if ( strTemp.Right(1) != '\\' )
		 strTemp += "\\";
	if ( !SHGetFileInfo( strTemp,
						0,
						&shFinfo,
						sizeof( shFinfo ),
						SHGFI_ICON | 
					    SHGFI_SMALLICON ) )
	{
		return FALSE;
	}

	iIcon = shFinfo.iIcon;
	DestroyIcon( shFinfo.hIcon );


	if ( !SHGetFileInfo( strTemp,
						0,
						&shFinfo,
						sizeof( shFinfo ),
						SHGFI_ICON | SHGFI_OPENICON |
					    SHGFI_SMALLICON ) )
	{
		return FALSE;
	}

	iIconSel = shFinfo.iIcon;
	DestroyIcon( shFinfo.hIcon );
	return TRUE;
}

BOOL CDataTreeCtrl::IsExt(const char* strFileName)
{
	if (m_svExt.empty()) return TRUE;

	char strExt[MAX_PATH], strName[MAX_PATH];

	strcpy(strName, strFileName);
	strlwr(strName);

	svdef_String::iterator it;
	for (it = m_svExt.begin(); it != m_svExt.end(); ++ it)
	{
		strcpy(strExt, (*it).c_str());
		strlwr(strExt);

		if (strstr(strName, strExt) != NULL)
		{
			return TRUE;
		}
	}
	return FALSE;
}

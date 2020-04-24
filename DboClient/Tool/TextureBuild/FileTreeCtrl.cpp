#include "Resource.h"
#include "FileTreeCtrl.h"

IMPLEMENT_DYNAMIC(CFileTreeCtrl, CTreeCtrl)
CFileTreeCtrl::CFileTreeCtrl()
{
}

CFileTreeCtrl::~CFileTreeCtrl()
{
	m_imgIcon.DeleteImageList();
}


BEGIN_MESSAGE_MAP(CFileTreeCtrl, CTreeCtrl)
END_MESSAGE_MAP()

void CFileTreeCtrl::Init()
{
	m_imgIcon.Create(IDB_ICON, 10, 1, RGB(255,255,255));
	SetImageList(&m_imgIcon, TVSIL_NORMAL);
}

HTREEITEM CFileTreeCtrl::InsertDirectory(const char* pcPath, HTREEITEM hParent)
{
	char			acTempPath[MAX_PATH];

	GetCurrentDirectory(MAX_PATH, acTempPath);
	if (!SetCurrentDirectory(pcPath))
	{
		return NULL;
	}

	WIN32_FIND_DATA findData;
	HANDLE			hfindFile;

	char			acPattern[MAX_PATH];
	sprintf_s(acPattern, MAX_PATH, "*.*", pcPath);
	
	HTREEITEM hDirectory = InsertItem(pcPath, 0, 0, hParent, TVI_FIRST);
	SetItemData(hDirectory, FTD_FLAG_DIRECTROY);
	//SetItemImage(hCurrent, 2, 2);
	
	hfindFile = ::FindFirstFile(acPattern, &findData);
	if(hfindFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (findData.cFileName[0] != '.')
			{
				if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					InsertDirectory(findData.cFileName, hDirectory);
				}
				else
				{	
					HTREEITEM hFile = InsertItem(findData.cFileName, hDirectory);
				}
			}
		} while(::FindNextFile(hfindFile, &findData));

		::FindClose(hfindFile);
	}

	SetCurrentDirectory(acTempPath);
	return hDirectory;	
}

CString CFileTreeCtrl::GetPathByItem(HTREEITEM hItem)
{
	CString strTemp;
	CString strPath;

	do 
	{
		strTemp = GetItemText(hItem);
		strPath = strTemp + strPath;
		
		hItem = GetParentItem(hItem);
		if (hItem)
		{
			strPath = "\\" + strPath;
		}
	} while (hItem);

	return strPath;
}

HTREEITEM CFileTreeCtrl::GetItemByPath(CString strPath)
{
	int iStart		= 0;
	int iEnd		= 0;
	HTREEITEM hItem = 0;
	HTREEITEM hTemp = 0;

	
	do 
	{
		 iEnd = strPath.Find('\\', iStart);
		
		 CString strTokken;
		 if (iEnd > 0)
		 {
			 strTokken	= strPath.Mid(iStart, iEnd - iStart);
			 iStart		= iEnd + 1;
		 }
		 else
		 {
			 strTokken = strPath.Mid(iStart);
		 }
		 
		 if (strTokken.GetLength())
		 {
			 hTemp = GetChildItem(hItem);
			 while(hTemp != NULL)
			 {
				 if (GetItemText(hTemp).MakeUpper() == strTokken.MakeUpper())
				 {
					 hItem = hTemp;
					 break;
				 }
				 hTemp = GetNextSiblingItem(hTemp);
			 }
			 if (hItem != hTemp || (!hItem && !hTemp))
			 {
				 return NULL;
			 }
		 }

	} while (iEnd > 0);

	return hItem;	
}

void CFileTreeCtrl::AllItems(HTREEITEM hItem, void (*_CALLBACK)(HTREEITEM,CTreeCtrl*,void*), void* pUserData)
{
	_CALLBACK(hItem, this, pUserData);
	hItem = GetChildItem(hItem);
	while(hItem != NULL)
	{
		_CALLBACK(hItem, this, pUserData);

		if (ItemHasChildren(hItem))
		{
			AllItems(hItem, _CALLBACK, pUserData);
		}
		
		hItem = GetNextSiblingItem(hItem);
	}
}

unsigned int CFileTreeCtrl::GetItemLevel(HTREEITEM hItem)
{
	return (unsigned int)(GetItemData(hItem));
}

void CFileTreeCtrl::SetItemLevel(HTREEITEM hItem, unsigned int uiLevel)
{
	if (GetChildItem(hItem))
	{
		hItem = GetChildItem(hItem);
		while(hItem != NULL)
		{
			SetItemLevel(hItem, uiLevel);
			hItem = GetNextSiblingItem(hItem);
		}
	}
	else if (GetItemData(hItem) != FTD_FLAG_DIRECTROY)
	{
		SetItemData(hItem, uiLevel);
		SetItemImage(hItem, uiLevel, uiLevel);
	}
}
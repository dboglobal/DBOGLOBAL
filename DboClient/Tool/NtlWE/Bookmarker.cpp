#include "StdAfx.h"

#include "NtlWorldCommon.h"

#include "TableContainer.h"
#include "TextAllTable.h"
#include "WorldTable.h"

#include "NtlSLApi.h"

#include "Bookmarker.h"

CBookmarker::CBookmarker(void)
{
}

CBookmarker::~CBookmarker(void)
{
}

VOID CBookmarker::Create()
{
	std::vector<std::string>	vecWorldName;
	CWorldTable*				pTable = static_cast<CWorldTable*>(API_GetTableContainer()->GetWorldTable());

	CTable::TABLEIT	it = pTable->Begin();
	while (it != pTable->End())
	{
		RwUInt32		uiIndex		= it->first;
		sWORLD_TBLDAT*	pTableData	= static_cast<sWORLD_TBLDAT*>(it->second);

		std::vector<std::string>::iterator itWorldName = std::find(vecWorldName.begin(), vecWorldName.end(), pTableData->szName);
		if (itWorldName == vecWorldName.end())
		{
			vecWorldName.push_back(pTableData->szName);
		}
		it++;
	}

	// end user name; computer name
	DWORD	Size				= 64;
	RwChar	ComputerName[64]	= {0,};

	GetComputerName(ComputerName, &Size);
	m_strEndUserName = ComputerName;

	// world project name only
	m_strWorldProjName = dGET_WORLD_PARAM()->WorldProjectFolderName;

	RwInt32 NameLength	= m_strWorldProjName.GetLength();
	m_strWorldProjName.Delete(NameLength - 1, 1);

	// bookmark file name
	CString Tmp = "_";
	CString TmpExt = ".bm";
	m_strBookmarkName = m_strWorldProjName;
	m_strBookmarkName.Delete(0, m_strWorldProjNameOnly.ReverseFind('\\') + 1);
	m_strBookmarkName = m_strBookmarkName + Tmp + m_strEndUserName + TmpExt;

	// bookmark file name only
	m_strWorldProjNameOnly = m_strWorldProjName;
	m_strWorldProjNameOnly.Delete(0, m_strWorldProjNameOnly.ReverseFind('\\') + 1);

	RwBool bIsIn = FALSE;
	for (int i = 0; i < (int)vecWorldName.size(); ++i)
	{
		CString strSrc = m_strWorldProjNameOnly;
		CString strCmp = vecWorldName.at(i).c_str();

		RwInt32 iLenSrc = strSrc.GetLength();
		RwInt32 iLenCmp = strCmp.GetLength();

		RwInt32 iPos = strSrc.Find(strCmp.GetBuffer());
		while (iPos != -1 && iPos <= iLenSrc - iLenCmp)
		{
			if (strSrc.Mid(iPos, iLenCmp) == strCmp)
			{
				bIsIn = TRUE;
				break;
			}
			iPos = strSrc.Find(strCmp.GetBuffer(), iPos);
		}
		if (bIsIn)
		{
			m_strWorldProjNameOnly = strCmp;
			break;
		}
	}	

	// load bookmark files
	m_vecBookmark.clear();

	// load files that have same world file name
	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	_chdir("tool");
	_chdir("saber");
	_chdir("bookmark");

	CFileFind	FF;
	CString		Ext	= "*.bm";

	RwBool bWorking = FF.FindFile(Ext.GetBuffer(0));
	while(bWorking)
	{
		bWorking = FF.FindNextFile();

		if(FF.IsDots() || FF.IsDirectory())
			continue;

		CString TempCompare = FF.GetFileTitle();
		TempCompare.Delete(TempCompare.ReverseFind('_'), TempCompare.GetLength() - TempCompare.ReverseFind('_') + 1);

		if(!TempCompare.Compare(m_strWorldProjNameOnly.GetBuffer(0)))
			LoadAdditionalBookmark(FF.GetFileTitle());
	}

	RegenerateIndices();

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
}

VOID CBookmarker::LoadAdditionalBookmark(CString& _FileName)
{
	CString strWithExt = ".bm";
	strWithExt = _FileName + strWithExt;

	FILE* pFile = NULL;
	if(fopen_s(&pFile, strWithExt.GetBuffer(0), "rb") != 0)
	{
		DBO_TRACE(FALSE, "file open failed. (" << strWithExt.GetBuffer(0) << ")");
		return;
	}

	RwInt32 Cnt = 0;
	fread(&Cnt, sizeof(RwInt32), 1, pFile);

	CBookmark	CurBM;
	RwInt32		StringLength;
	RwChar		CurString[512] = {0,};

	for(RwInt32 i = 0; i < Cnt; ++i)
	{
		fread(&CurBM.m_Pos.x, sizeof(RwReal), 1, pFile);
		fread(&CurBM.m_Pos.y, sizeof(RwReal), 1, pFile);
		fread(&CurBM.m_Pos.z, sizeof(RwReal), 1, pFile);

		fread(&CurBM.m_Dir.x, sizeof(RwReal), 1, pFile);
		fread(&CurBM.m_Dir.y, sizeof(RwReal), 1, pFile);
		fread(&CurBM.m_Dir.z, sizeof(RwReal), 1, pFile);

		fread(&CurBM.m_Right.x, sizeof(RwReal), 1, pFile);
		fread(&CurBM.m_Right.y, sizeof(RwReal), 1, pFile);
		fread(&CurBM.m_Right.z, sizeof(RwReal), 1, pFile);

		fread(&CurBM.m_Up.x, sizeof(RwReal), 1, pFile);
		fread(&CurBM.m_Up.y, sizeof(RwReal), 1, pFile);
		fread(&CurBM.m_Up.z, sizeof(RwReal), 1, pFile);

		ZeroMemory(CurString, 512);
		fread(&StringLength, sizeof(RwInt32), 1, pFile);
		fread(CurString, sizeof(RwChar) * StringLength, 1, pFile);
		CurBM.m_Date = CurString;

		ZeroMemory(CurString, 512);
		fread(&StringLength, sizeof(RwInt32), 1, pFile);
		fread(CurString, sizeof(RwChar) * StringLength, 1, pFile);
		CurBM.m_Memo = CurString;

		ZeroMemory(CurString, 512);
		fread(&StringLength, sizeof(RwInt32), 1, pFile);
		fread(CurString, sizeof(RwChar) * StringLength, 1, pFile);
		CurBM.m_User = CurString;

		ZeroMemory(CurString, 512);
		fread(&StringLength, sizeof(RwInt32), 1, pFile);
		fread(CurString, sizeof(RwChar) * StringLength, 1, pFile);
		CurBM.m_Preview = CurString;

		m_vecBookmark.push_back(CurBM);
	}

	fclose(pFile);
}

VOID CBookmarker::AddMark(CBookmark _Bookmark)
{
	m_vecBookmark.push_back(_Bookmark);
}

VOID CBookmarker::RemoveMark(RwInt32 _Index)
{
	CBookmark TmpBm;
	for(RwInt32 i = 0; i < (int)m_vecBookmark.size(); ++i)
	{
		TmpBm = m_vecBookmark[i];
		if(TmpBm.m_Idx == _Index)
		{
			m_vecBookmark.erase(m_vecBookmark.begin() + i);
			return;
		}
	}
}

VOID CBookmarker::RegenerateIndices()
{
	CBookmark CurBm;
	for(RwUInt32 i = 0; i < m_vecBookmark.size(); i++)
	{
		CurBm				= m_vecBookmark[i];
		CurBm.m_Idx			= i;
		m_vecBookmark[i]	= CurBm;
	}
}

RwBool CBookmarker::Save()
{
	if(!m_vecBookmark.size())
		return TRUE;

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	_chdir("tool");
	_chdir("saber");
	_chdir("bookmark");

	NtlFileAttrReadOnlyRelease(m_strBookmarkName.GetBuffer(0));

	CString Tmp = "_";
	CString TmpExt = ".bm";
	CString SaveFileName(m_strWorldProjNameOnly + Tmp + m_strEndUserName + TmpExt);

	FILE* pFile = NULL;
	if(fopen_s(&pFile, SaveFileName.GetBuffer(0), "wb") != 0)
	{
		DBO_TRACE(FALSE, "file open failed. (" << SaveFileName.GetBuffer(0) << ")");
		return FALSE;
	}

	RwInt32		Cnt = m_vecBookmark.size();
	CBookmark	CurBM;
	RwInt32		StringLength;

	fwrite(&Cnt, sizeof(RwInt32), 1, pFile);

	for(RwInt32 i = 0; i < Cnt; ++i)
	{
		CurBM = m_vecBookmark[i];
		if(CurBM.m_User != m_strEndUserName)
			continue;

		fwrite(&CurBM.m_Pos.x, sizeof(RwReal), 1, pFile);
		fwrite(&CurBM.m_Pos.y, sizeof(RwReal), 1, pFile);
		fwrite(&CurBM.m_Pos.z, sizeof(RwReal), 1, pFile);

		fwrite(&CurBM.m_Dir.x, sizeof(RwReal), 1, pFile);
		fwrite(&CurBM.m_Dir.y, sizeof(RwReal), 1, pFile);
		fwrite(&CurBM.m_Dir.z, sizeof(RwReal), 1, pFile);

		fwrite(&CurBM.m_Right.x, sizeof(RwReal), 1, pFile);
		fwrite(&CurBM.m_Right.y, sizeof(RwReal), 1, pFile);
		fwrite(&CurBM.m_Right.z, sizeof(RwReal), 1, pFile);

		fwrite(&CurBM.m_Up.x, sizeof(RwReal), 1, pFile);
		fwrite(&CurBM.m_Up.y, sizeof(RwReal), 1, pFile);
		fwrite(&CurBM.m_Up.z, sizeof(RwReal), 1, pFile);

		StringLength = CurBM.m_Date.GetLength();
		fwrite(&StringLength, sizeof(RwInt32), 1, pFile);
		fwrite(CurBM.m_Date.GetBuffer(0), sizeof(RwChar) * StringLength, 1, pFile);

		StringLength = CurBM.m_Memo.GetLength();
		fwrite(&StringLength, sizeof(RwInt32), 1, pFile);
		fwrite(CurBM.m_Memo.GetBuffer(0), sizeof(RwChar) * StringLength, 1, pFile);

		StringLength = CurBM.m_User.GetLength();
		fwrite(&StringLength, sizeof(RwInt32), 1, pFile);
		fwrite(CurBM.m_User.GetBuffer(0), sizeof(RwChar) * StringLength, 1, pFile);

		StringLength = CurBM.m_Preview.GetLength();
		fwrite(&StringLength, sizeof(RwInt32), 1, pFile);
		fwrite(CurBM.m_Preview.GetBuffer(0), sizeof(RwChar) * StringLength, 1, pFile);
	}

	fclose(pFile);

	DWORD dwAttrs = ::GetFileAttributes(m_strBookmarkName.GetBuffer(0));
	if(!(dwAttrs & FILE_ATTRIBUTE_READONLY)) 
	{ 
		dwAttrs |= FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(m_strBookmarkName.GetBuffer(0), dwAttrs);
	} 

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return TRUE;
}

CBookmark* CBookmarker::GetBM(RwInt32 _idx)
{
	CBookmark TmpBm;
	for(RwInt32 i = 0; i < (int)m_vecBookmark.size(); ++i)
	{
		TmpBm = m_vecBookmark[i];
		if(TmpBm.m_Idx == _idx)
		{
			return &m_vecBookmark[i];
		}
	}	

	return NULL;
}
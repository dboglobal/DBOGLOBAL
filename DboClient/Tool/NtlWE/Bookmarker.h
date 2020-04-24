#pragma once


#include <vector>
using std::vector;


class CBookmark
{
public:
	CBookmark(void) {;}
	~CBookmark(void) {;}

public:
	RwInt32 m_Idx;
	CString m_User;
	RwV3d	m_Pos;
	RwV3d	m_Dir;
	RwV3d	m_Right;
	RwV3d	m_Up;
	CString	m_Date;
	CString m_Memo;
	CString m_Preview;
};

class CBookmarker
{
public:

	CBookmarker(void);
	~CBookmarker(void);

	typedef std::vector<CBookmark>				tVECTOR_BM;
	typedef std::vector<CBookmark>::iterator	tVECTOR_BM_ITER;

protected:

	tVECTOR_BM	m_vecBookmark;
	CString		m_strEndUserName;
	CString		m_strWorldProjName;
	CString		m_strBookmarkName;
	CString		m_strWorldProjNameOnly;

protected:

	VOID	LoadAdditionalBookmark(CString& _FileName);

public:

	VOID			Clear() { m_vecBookmark.clear(); }
	VOID			AddMark(CBookmark _Bookmark);
	VOID			RemoveMark(RwInt32 _Index);
	VOID			RegenerateIndices();
	RwBool			Save();
	CString			GetEndUserName() { return m_strEndUserName; }
	CBookmark*		GetBM(RwInt32 _idx);
	VOID			Create();

	tVECTOR_BM_ITER	GetBmBIter()	{ return m_vecBookmark.begin(); }
	tVECTOR_BM_ITER	GetBmEIter()	{ return m_vecBookmark.end(); }
};


/*
if(dOKEY(DIK_NUMPADENTER))
{
	if(m_pCamera)
	{
		CreateDirectory("./ScreenShot",NULL);
		SYSTEMTIME systemTime;
		GetSystemTime(&systemTime);

		RwChar szBuffer[256];
		RwInt32 nYear   = (RwInt32)systemTime.wYear;
		RwInt32 nMonth  = (RwInt32)systemTime.wMonth;
		RwInt32 nDay	= (RwInt32)systemTime.wDay;
		RwInt32 nHour	= (RwInt32)systemTime.wHour;
		RwInt32 nSecond = (RwInt32)systemTime.wSecond;
		RwInt32 nMinute = (RwInt32)systemTime.wMinute;
		RwReal fMilliseconds = (RwReal)systemTime.wMilliseconds/1000.f;
		sprintf_s(szBuffer, 256, "./ScreenShot/%d-%d-%d-%d-%d-%d-%4.2f.bmp", nYear, nMonth, nDay, nHour, nSecond, nMinute, fMilliseconds);

		RwCamera *pCamera = m_pCamera->GetCamera();
		if(pCamera && strlen(szBuffer) > 0)
		{
			RwRaster *pRaster = RwCameraGetRaster(pCamera);		

			API_PL_RasterSaveImage(pRaster, szBuffer);
		}
	}
}*/

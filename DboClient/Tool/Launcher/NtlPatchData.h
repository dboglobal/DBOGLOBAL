/*****************************************************************************
 *
 * File			: NtlPatchData.h
 * Author		: HongHoDong
 * Copyright	: (주)NTL
 * Date			: 2007. 2. 5.
 * Abstract		: 
 *****************************************************************************
 * Desc         : Patch Data
 *
 *****************************************************************************/
#pragma once
#include <vector>

#define		PATCH_CUR_VER_CLIENT_XML_NODE		"/Version/Client"
#define		PATCH_CUR_VER_LAUNCHER_XML_NODE		"/Version/Launcher"
#define		PATCH_CUR_VER_ATTRIBUTE				"Ver"
#define     PATCH_CUR_VER_DATE                  "Date"

class CNtlPatchVersion
{
public:
	CNtlPatchVersion() : m_nMajor(0), m_nMinor(0), m_nBuild(0) {}

private:
	INT m_nMajor; 
	INT m_nMinor;
	INT m_nBuild;

	CString m_strVersion;

public:
	void Set(const char *pstrVersion)
	{
		CString strBuffer;
		CString retToken;
		INT	nCurPos = 0;
		INT nTokenCount = 0;

		strBuffer = pstrVersion;
		retToken = strBuffer.Tokenize(".", nCurPos);
		while( retToken != "")
		{
			if(nTokenCount == 0)
				m_nMajor = _tstoi(retToken);
			else if(nTokenCount == 1)
				m_nMinor = _tstoi(retToken);
			else if(nTokenCount == 2)
				m_nBuild = _tstoi(retToken);

			retToken = strBuffer.Tokenize(".", nCurPos);
			nTokenCount++;
		}

		m_strVersion = pstrVersion;
	}
	
	INT		GetMajor() { return m_nMajor; }
	INT		GetMinor() { return m_nMinor; }
	INT		GetBuild() { return m_nBuild; }

	const char *Get() { return m_strVersion; }
};

class CNtlPatchData
{
private:
	CNtlPatchVersion	m_PatchVersion;			  
	CString				m_strServerFullPathFileName;
	CString				m_strServerFileName;

	CString				m_strClientFullPathFileName;
	CString				m_strClientFileName;
    CString             m_strDate;

public:
	void SetVersion(char *pszVersion)
	{
		m_PatchVersion.Set(pszVersion);
	}

	void SetServerFullPathFileName(const char *pszServerFilePath,const char *pszServerFileName)
	{
		m_strServerFullPathFileName = pszServerFilePath;
		m_strServerFullPathFileName += "/";
		m_strServerFullPathFileName += pszServerFileName;

		m_strServerFileName = pszServerFileName;
	}
	
	void SetClientFullPathFileName(const char *pszClientFilePath,const char *pszClientFileName)
	{
		m_strClientFullPathFileName = pszClientFilePath;
		m_strClientFullPathFileName += "/";
		m_strClientFullPathFileName += pszClientFileName;

		m_strClientFileName = pszClientFileName;
	}

	CNtlPatchVersion *GetVersion() { return &m_PatchVersion; };
	
	const char *GetServerFileName() { return m_strServerFileName; };
	const char *GetServerFullPathFileName() { return m_strServerFullPathFileName; };

	const char *GetClientFileName() { return m_strClientFileName; };
	const char *GetClientFullPathFileName() { return m_strClientFullPathFileName; };

    void    SetDate(CString strDate) {m_strDate = strDate;}
    CString GetDate()                {return m_strDate;}
};

class CNtlPatchHistory
{
protected:
	static CNtlPatchHistory *s_pInstance;

	CNtlPatchHistory()
	{
		m_CurVerClient.Set("0.0.0");
		m_CurVerLauncher.Set("0.0.0");
		m_CurVerFullPathFileName = PATCH_CUR_VER_FILE_NAME;
	}
	CNtlPatchHistory(const CNtlPatchHistory& rhs) {}

public:
	~CNtlPatchHistory();

protected:
	CNtlPatchVersion				m_CurVerClient;					// 현재 Client Version
	CNtlPatchVersion				m_CurVerLauncher;				// 현재 Launcher Version
	CString							m_CurVerFullPathFileName;		// 현재 Version File이름
    CString                         m_strPatchDate;                 // 패치파일 날짜

	std::vector<CNtlPatchData *>	m_vecNewVerClientRtDiff;		// Client Diff RtPatch List
	CNtlPatchData					m_NewVerClientRtFull;			// Client Full RtPatch

	CNtlPatchData					m_NewVerLauncherRtFull;			// Launcher Full RtPatch

public:
	BOOL	LoadCurVersion(const char *pszFullPathFileName);
	BOOL	LoadPatchList(const char *pszFullPathFileName);
	
	BOOL	SaveCurVersion(const char *pszFullPathFileName);

	CNtlPatchVersion *GetCurVerClient() { return &m_CurVerClient; };
	void SetCurVerClient(CNtlPatchVersion *pVersion);

	CNtlPatchVersion *GetCurVerLauncher() { return &m_CurVerLauncher; };
	void SetCurVerLauncher(CNtlPatchVersion *pVersion);

	CNtlPatchData *GetNewVerClientRtDiff();
	CNtlPatchData *GetNewVerClientRtFull();
	CNtlPatchData *GetNewVerLauncherRtFull();

    CString        GetPatchFileDate()   {return m_strPatchDate;}
    void           SetPatchFileDate(CString strPatchFileDate) {m_strPatchDate = strPatchFileDate;}

	virtual BOOL Create() { return FALSE; };
	virtual void Destroy();

	void DestroyInstance()
	{
		if(s_pInstance != NULL)
		{
			delete s_pInstance;
			s_pInstance = NULL;
		}
	}

	static CNtlPatchHistory *GetInstance() 
	{ 
		if(s_pInstance == NULL)
			s_pInstance = new CNtlPatchHistory;

		ASSERT(s_pInstance != NULL);
		return s_pInstance;
	}
};

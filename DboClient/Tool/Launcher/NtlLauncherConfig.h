#pragma once

#define LAUNCHER_NAME_XML_NODE						"Config/LauncherName"
#define LAUNCHER_CAPTION_NAME_ATTRIBUTE				"CaptionName"

#define LAUNCHER_PATCH_SERVER_XML_NODE				"Config/PatchServer"
#define	LAUNCHER_SERVER_NAME_ATTRIBUTE				"ServerName"
#define	LAUNCHER_SERVER_FILE_PATH_ATTRIBUTE			"FilePath"
#define	LAUNCHER_HTML_INFO_XML_NODE					"Config/HtmlInfo"
#define	LAUNCHER_HTML_FULLPATH_FILE_NAME_ATTRIBUTE	"FullPathFileName"

class CNtlLauncherConfig
{
protected:
	CNtlLauncherConfig() {}
	CNtlLauncherConfig(const CNtlLauncherConfig& rhs) {}

public:
	~CNtlLauncherConfig();

	static CNtlLauncherConfig *s_pInstance;

	CString		m_strCaptionName;					//Launcher Name
	CString		m_strPatchServerName;				//Server 주소
	CString		m_strPatchServerFilePath;			//Server Directory
	CString		m_strHtmlInfoFullPathFileName;		//Html 정보 받을 주소

	

public:
	const char	*GetCaptionName();
	const char	*GetPatchServerName();
	const char	*GetPatchServerFilePath();
	const char	*GetHtmlInfoFullPathFileName();

	void DestroyInstance()							//Instance의 삭제
	{
		if(s_pInstance != NULL)
		{
			delete s_pInstance;
			s_pInstance = NULL;
		}
	}
	
	static CNtlLauncherConfig *GetInstance()
	{ 
		if(s_pInstance == NULL)
			s_pInstance = new CNtlLauncherConfig;

		ASSERT(s_pInstance != NULL);
		return s_pInstance;
	}

	BOOL	Create(const char *pszFileName);

};

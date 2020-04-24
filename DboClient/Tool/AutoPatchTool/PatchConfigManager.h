/*****************************************************************************
 *
 * File			: PatchConfigManager.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2008. 6. 23	
 * Abstract		: Patch config 
 *****************************************************************************
 * Desc         : patch에 대한 자료를 관리한다.
 *
 *****************************************************************************/

#ifndef __PATCH_CONFIG_MANAGER_H__
#define __PATCH_CONFIG_MANAGER_H__

#include <string>
#include <list>

typedef struct _SPCCommon
{
	bool	bDiffPatch;					// diff patch 이면 ? true : false
}SPCCommon;

typedef struct _SPCVersion
{
	int nLVer;					// 
	int nRVer;
	int nBVer; 
}SPCVersion;



enum ECopyDataType
{
	ECDT_Build,
	ECDT_Res,
	ECDT_Exe,
	ECDT_Dump,
	ECDT_Local,
	ECDT_Code,
	ECDT_End
};

typedef struct _SPCCopyData
{
	ECopyDataType	eCopyDataType;
	std::string		strName;
}SPCCopyData;

typedef struct _SPCCopy
{
	std::string				strOldSystem;
	std::string				strNewSystem;
	std::string				strSrcFolder[ECDT_End];
	std::list<SPCCopyData*>	listCopyData;
}SPCCopy;

typedef struct _SPCRtPatch
{
	std::string				strRtPatchExe;
	std::string				strRtPatchExport;
}SPCRtPacth;

typedef struct _SPatchConfig
{
	SPCCommon	sCommon;
	SPCVersion	sCurrVer;
	SPCCopy		sCopy;
	SPCRtPacth	sRtPatch;
}SPatchConfig;


typedef struct _SPCServerCopy
{
	std::string				strNewSystem;
	std::string				strSrcFolder[ECDT_End];
	std::list<SPCCopyData*>	listCopyData;
}SPCServerCopy;


typedef struct _SServerPatchConfig
{
	SPCServerCopy	sCopy;
	std::string		strServerBuildFolder;
}SServerPatchConfig;


/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


class CPatchFTPUploadScript
{
private:

	std::string	m_strFileName;
	std::string m_strBuffer;

private:

	bool Parse(const char *key, const char *pData);

public:

	CPatchFTPUploadScript();
	~CPatchFTPUploadScript();

	bool			Load(const char *pFileName);
	bool			Save(const char *pFileName);
	bool			Run(const char *key, const char *data);
};


/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


class CPatchConfigManager
{
private:

	// client
	SPatchConfig			m_sPatchConfig;
	
	CPatchFTPUploadScript	m_FTPUploadScript;
	CPatchFTPUploadScript	m_PatchCmdScript;

	// server
	SServerPatchConfig		m_sServerPatchConfig;


private:

	void				Clear(void);

	void				CreateSubDirectory(const char *pFullName, const char *pAttachName);
	bool				IsFolder(const char *pFileName);
	bool				CopyFolder(const char *pSrcFolder, const char *pDestFolder);
    bool                CopyFile(const std::string& srcFile, const std::string& destFile);
	bool				DeleteFolder(const char *pFolder, bool bRootFolderDelete = true);
	bool				DeleteChildFolder(const char *pFolder);
	void				FolderReadOnlyRelease(const char *pFolder);
	void				FileReadOnlyRelease(const char *pFileName);
	std::string			MakeVersion(int nLVer, int nRVer, int nBVer);
	std::string			GetDate(void);
	std::string			GetDataTime(void);

	bool				CopySourceAll(void);
	bool				CopyDump(BOOL bFastMode = FALSE);
	bool				FindPatchData(void);
	bool				RenameOldSystem(void);
	bool				RenameNewSystem(void);

	bool				IsExistBuildProcess(const char *pName);

public:

	CPatchConfigManager();
	~CPatchConfigManager();

	static CPatchConfigManager* GetInstance(void);

	// client
	bool				LoadVersion(const char *pFileName);
	bool				LoadConfig(const char *pFileName);
	bool				CopySource(bool bCode);
	bool				TSEvtMakerBuild(void);
	bool				SaveVersion(const char *pFileName);
	bool				SaveFTPUploadScript(const char *pFileName);
	bool				SavePatchCommandScript(const char *pFileName);
	bool				RtPatchBuild(void);
	bool				EndPatchBuild(BOOL bFastMode = FALSE);
	
	SPatchConfig*		GetPatchConfig(void);

	// server
	bool				LoadServerConfig(const char *pFileName);
	bool				MakeServerBuildFolder(void);
	bool				CopyServerSource(bool bCode);
	SServerPatchConfig*	GetServerPatchConfig(void);
	
	void				OutLog(const char *pLog);
};

static CPatchConfigManager* GetPatchConfigManager(void)
{
	return CPatchConfigManager::GetInstance();
}

inline SPatchConfig* CPatchConfigManager::GetPatchConfig(void)
{
	return &m_sPatchConfig;
}

#endif

/*****************************************************************************
*
* File			: DLEventDef.h
* Author		: 
* Copyright		: (주) NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          : 
*
*****************************************************************************/


#pragma once


//////////////////////////////////////////////////////////////////////////
//
// Event id definition
//
//////////////////////////////////////////////////////////////////////////


enum eDL_EVENT
{
	// Message
	eDL_EVENT_OUTPUT_MESSAGE_TO_USER,			// 사용자에게 알림 메시지
	eDL_EVENT_OUTPUT_MESSAGE_FOR_DEBUG,			// 디버깅을 위한 메시지

	// Skin
	eDL_EVENT_CREATE_SKIN,						// Skin을 생성한다
	eDL_EVENT_DELETE_SKIN,						// Skin을 제거한다
	eDL_EVENT_ACTIVE_START,						// 시작 버튼을 활성화 / 비활성화 한다
	eDL_EVENT_ACTIVE_INTEGRITY,					// 무결성 버튼을 활성화 / 비활성화 한다

	// Download
	eDL_EVENT_DO_DOWNLOAD,						// Download 시작 명령
	eDL_EVENT_DOWNLOADING_NFY,					// Download 진행 중 알림

	// Patch
	eDL_EVENT_DO_PATCH,							// Patch 시작 명령
	eDL_EVENT_PATCH_START_NFY,
	eDL_EVENT_PATCH_UPDATE_NFY,
	eDL_EVENT_PATCH_COMPLETE_NFY,
	eDL_EVENT_PATCH_ETC_UPDATE_NFY,

	// Version
	eDL_EVENT_UPDATE_VERSION_NFY,				// 현재 버전 업데이트

	// Integrity
	eDL_EVENT_DO_CHECK_INTEGRITY,				// 무결성 검사 시작
	eDL_EVENT_INTEGRITY_START_NFY,
	eDL_EVENT_INTEGRITY_UPDATE_NFY,
	eDL_EVENT_INTEGRITY_COMPLETE_NFY,

	// State
	eDL_EVENT_CHANGE_STATE,						// 상태 변경

	// Quit
	eDL_EVENT_USER_EXIT							// 사용자 종료
};


//////////////////////////////////////////////////////////////////////////
//
// Event data definition
//
//////////////////////////////////////////////////////////////////////////


struct sDL_EVENT_DATA_BASE
{
	virtual ~sDL_EVENT_DATA_BASE( void ) { return; }
	virtual sDL_EVENT_DATA_BASE* Clone( void ) = 0;
};


struct sDL_ED_MSGOUT_TO_USER : public sDL_EVENT_DATA_BASE
{
	unsigned int		uiMsgID;	// eDL_MSG

	virtual sDL_EVENT_DATA_BASE* Clone( void )
	{
		sDL_ED_MSGOUT_TO_USER* pData = new sDL_ED_MSGOUT_TO_USER;
		pData->uiMsgID = uiMsgID;
		return pData;
	}
};


struct sDL_ED_MSGOUT_FOR_DEBUG : public sDL_EVENT_DATA_BASE
{
	CString				strMsg;

	virtual sDL_EVENT_DATA_BASE* Clone( void )
	{
		sDL_ED_MSGOUT_FOR_DEBUG* pData = new sDL_ED_MSGOUT_FOR_DEBUG;
		pData->strMsg = strMsg;
		return pData;
	}
};


struct sDL_ED_CREATE_SKIN : public sDL_EVENT_DATA_BASE
{
	bool				bResult;
	unsigned int		uiPublisherID;	// ePUBLISHER_ID

	virtual sDL_EVENT_DATA_BASE* Clone( void )
	{
		sDL_ED_CREATE_SKIN* pData = new sDL_ED_CREATE_SKIN;
		pData->bResult = bResult;
		pData->uiPublisherID = uiPublisherID;
		return pData;
	}
};


struct sDL_ED_DELETE_SKIN : public sDL_EVENT_DATA_BASE
{
	virtual sDL_EVENT_DATA_BASE* Clone( void )
	{
		sDL_ED_DELETE_SKIN* pData = new sDL_ED_DELETE_SKIN;
		return pData;
	}
};


struct sDL_ED_ACTIVE_START : public sDL_EVENT_DATA_BASE
{
	bool				bOn;

	virtual sDL_EVENT_DATA_BASE* Clone( void )
	{
		sDL_ED_ACTIVE_START* pData = new sDL_ED_ACTIVE_START;
		pData->bOn = bOn;
		return pData;
	}
};


struct sDL_ED_ACTIVE_INTEGRITY : public sDL_EVENT_DATA_BASE
{
	bool				bOn;

	virtual sDL_EVENT_DATA_BASE* Clone( void )
	{
		sDL_ED_ACTIVE_INTEGRITY* pData = new sDL_ED_ACTIVE_INTEGRITY;
		pData->bOn = bOn;
		return pData;
	}
};


struct sDL_ED_DO_DOWNLOAD : public sDL_EVENT_DATA_BASE
{
	enum eRESULT
	{
		eRESULT_SUCCESS,
		eRESULT_ERROR,
		eRESULT_USER_EXIT
	};

	eRESULT				eResult;
	CString				strDownloadServerName;
	CString				strDownloadServerPath;
	CString				strDownloadFileName;
	CString				strClientPath;
	CString				strClientFileName;

	virtual sDL_EVENT_DATA_BASE* Clone( void )
	{
		sDL_ED_DO_DOWNLOAD* pData = new sDL_ED_DO_DOWNLOAD;
		pData->eResult					= eResult;
		pData->strDownloadServerName	= strDownloadServerName;
		pData->strDownloadServerPath	= strDownloadServerPath;
		pData->strDownloadFileName		= strDownloadFileName;
		pData->strClientPath			= strClientPath;
		pData->strClientFileName		= strClientFileName;
		return pData;
	}
};


struct sDL_ED_DOWNLOADING_NFY : public sDL_EVENT_DATA_BASE
{
	CString				strFileName;
	unsigned int		uiFileSize;
	unsigned int		uiCurUpdateSize;

	virtual sDL_EVENT_DATA_BASE* Clone( void )
	{
		sDL_ED_DOWNLOADING_NFY* pData = new sDL_ED_DOWNLOADING_NFY;
		pData->strFileName		= strFileName;
		pData->uiFileSize		= uiFileSize;
		pData->uiCurUpdateSize	= uiCurUpdateSize;
		return pData;
	}
};


struct sDL_ED_DO_PATCH : public sDL_EVENT_DATA_BASE
{
	enum eRESULT
	{
		eRESULT_SUCCESS,
		eRESULT_WARNING,
		eRESULT_ERROR,
		eRESULT_USER_EXIT
	};

	eRESULT				eResult;
	CString				strPatchSrcPath;
	CString				strPatchDestPath;

	virtual sDL_EVENT_DATA_BASE* Clone( void )
	{
		sDL_ED_DO_PATCH* pData = new sDL_ED_DO_PATCH;
		pData->eResult				= eResult;
		pData->strPatchSrcPath		= strPatchSrcPath;
		pData->strPatchDestPath		= strPatchDestPath;
		return pData;
	}
};


struct sDL_ED_PATCH_START_NFY : public sDL_EVENT_DATA_BASE
{
	unsigned int		uiPatchFileCnt;

	virtual sDL_EVENT_DATA_BASE* Clone( void )
	{
		sDL_ED_PATCH_START_NFY* pData = new sDL_ED_PATCH_START_NFY;
		pData->uiPatchFileCnt = uiPatchFileCnt;
		return pData;
	}
};


struct sDL_ED_PATCH_UPDATE_NFY : public sDL_EVENT_DATA_BASE
{
	CString				strPatchFileName;

	virtual sDL_EVENT_DATA_BASE* Clone( void )
	{
		sDL_ED_PATCH_UPDATE_NFY* pData = new sDL_ED_PATCH_UPDATE_NFY;
		pData->strPatchFileName = strPatchFileName;
		return pData;
	}
};


struct sDL_ED_PATCH_COMPLETE_NFY : public sDL_EVENT_DATA_BASE
{
	virtual sDL_EVENT_DATA_BASE* Clone( void )
	{
		sDL_ED_PATCH_COMPLETE_NFY* pData = new sDL_ED_PATCH_COMPLETE_NFY;
		return pData;
	}
};


struct sDL_ED_PATCH_ETC_UPDATE_NFY : public sDL_EVENT_DATA_BASE
{
	virtual sDL_EVENT_DATA_BASE* Clone( void )
	{
		sDL_ED_PATCH_ETC_UPDATE_NFY* pData = new sDL_ED_PATCH_ETC_UPDATE_NFY;
		return pData;
	}
};


struct sDL_ED_UPDATE_VERSION_NFY : public sDL_EVENT_DATA_BASE
{
	CString			strLauncherVer;
	CString			strClinetVer;
	CString			strDate;

	virtual sDL_ED_UPDATE_VERSION_NFY* Clone( void )
	{
		sDL_ED_UPDATE_VERSION_NFY* pData = new sDL_ED_UPDATE_VERSION_NFY;

		pData->strLauncherVer = strLauncherVer;
		pData->strClinetVer = strClinetVer;
		pData->strDate = strDate;

		return pData;
	}
};


struct sDL_ED_DO_CHECK_INTEGRITY : public sDL_EVENT_DATA_BASE
{
	int				nRet;
	CString			strIntegrityFileName;

	virtual sDL_EVENT_DATA_BASE* Clone( void )
	{
		sDL_ED_DO_CHECK_INTEGRITY* pData = new sDL_ED_DO_CHECK_INTEGRITY;
		pData->nRet = nRet;
		pData->strIntegrityFileName = strIntegrityFileName;
		return pData;
	}
};


struct sDL_ED_DO_INTEGRITY_START_NFY : public sDL_EVENT_DATA_BASE
{
	unsigned int		uiIntegrithFileCnt;

	virtual sDL_EVENT_DATA_BASE* Clone( void )
	{
		sDL_ED_DO_INTEGRITY_START_NFY* pData = new sDL_ED_DO_INTEGRITY_START_NFY;
		pData->uiIntegrithFileCnt = uiIntegrithFileCnt;
		return pData;
	}
};


struct sDL_ED_DO_INTEGRITY_UPDATE_NFY : public sDL_EVENT_DATA_BASE
{
	int				nRetCode;
	CString			strUpdateFileName;

	virtual sDL_EVENT_DATA_BASE* Clone( void )
	{
		sDL_ED_DO_INTEGRITY_UPDATE_NFY* pData = new sDL_ED_DO_INTEGRITY_UPDATE_NFY;
		pData->nRetCode = nRetCode;
		pData->strUpdateFileName = strUpdateFileName;
		return pData;
	}
};


struct sDL_ED_DO_INTEGRITY_COMPLETE_NFY : public sDL_EVENT_DATA_BASE
{
	virtual sDL_EVENT_DATA_BASE* Clone( void )
	{
		sDL_ED_DO_INTEGRITY_COMPLETE_NFY* pData = new sDL_ED_DO_INTEGRITY_COMPLETE_NFY;
		return pData;
	}
};


struct sDL_ED_CHANGE_STATE : public sDL_EVENT_DATA_BASE
{
	unsigned int uiState;
	void* pData;

	virtual sDL_EVENT_DATA_BASE* Clone( void )
	{
		sDL_ED_CHANGE_STATE* pData = new sDL_ED_CHANGE_STATE;
		pData->uiState = uiState;
		pData->pData = pData;
		return pData;
	}
};


struct sDL_ED_USER_EXIT : public sDL_EVENT_DATA_BASE
{
	virtual sDL_EVENT_DATA_BASE* Clone( void )
	{
		sDL_ED_USER_EXIT* pData = new sDL_ED_USER_EXIT;
		return pData;
	}
};

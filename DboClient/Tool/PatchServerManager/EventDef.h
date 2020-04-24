/*****************************************************************************
*
* File			: EventDef.h
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


enum eEVENT
{
	// Patch
	eEVENT_PATCH_START_NFY,

	// Integrity
	eEVENT_DO_BUILD_INTEGRITY,					// 무결성 빌드 시작
	eEVENT_INTEGRITY_START_NFY,
	eEVENT_INTEGRITY_UPDATE_NFY,
	eEVENT_INTEGRITY_COMPLETE_NFY,

	// Quit
	eEVENT_USER_EXIT							// 사용자 종료
};


//////////////////////////////////////////////////////////////////////////
//
// Event data definition
//
//////////////////////////////////////////////////////////////////////////


struct sEVENT_DATA_BASE
{
	virtual ~sEVENT_DATA_BASE( void ) { return; }
	virtual sEVENT_DATA_BASE* Clone( void ) = 0;
};


struct sPATCH_START_NFY : public sEVENT_DATA_BASE
{
	bool				bBreak;
	CString				strPatchFileName;

	virtual sEVENT_DATA_BASE* Clone( void )
	{
		sPATCH_START_NFY* pData = new sPATCH_START_NFY;
		pData->bBreak = bBreak;
		pData->strPatchFileName = strPatchFileName;
		return pData;
	}
};


struct sDO_BUILD_INTEGRITY : public sEVENT_DATA_BASE
{
	int				nRet;
	CString			strIntegrityFileName;
	CString			strIntegrityBuildFolderName;

	virtual sEVENT_DATA_BASE* Clone( void )
	{
		sDO_BUILD_INTEGRITY* pData = new sDO_BUILD_INTEGRITY;
		pData->nRet = nRet;
		pData->strIntegrityFileName = strIntegrityFileName;
		pData->strIntegrityBuildFolderName = strIntegrityBuildFolderName;
		return pData;
	}
};


struct sDO_INTEGRITY_START_NFY : public sEVENT_DATA_BASE
{
	unsigned int		uiIntegrithFileCnt;

	virtual sEVENT_DATA_BASE* Clone( void )
	{
		sDO_INTEGRITY_START_NFY* pData = new sDO_INTEGRITY_START_NFY;
		pData->uiIntegrithFileCnt = uiIntegrithFileCnt;
		return pData;
	}
};


struct sDO_INTEGRITY_UPDATE_NFY : public sEVENT_DATA_BASE
{
	int				nRetCode;
	CString			strUpdateFileName;

	virtual sEVENT_DATA_BASE* Clone( void )
	{
		sDO_INTEGRITY_UPDATE_NFY* pData = new sDO_INTEGRITY_UPDATE_NFY;
		pData->nRetCode = nRetCode;
		pData->strUpdateFileName = strUpdateFileName;
		return pData;
	}
};


struct sDO_INTEGRITY_COMPLETE_NFY : public sEVENT_DATA_BASE
{
	virtual sEVENT_DATA_BASE* Clone( void )
	{
		sDO_INTEGRITY_COMPLETE_NFY* pData = new sDO_INTEGRITY_COMPLETE_NFY;
		return pData;
	}
};


struct sUSER_EXIT : public sEVENT_DATA_BASE
{
	virtual sEVENT_DATA_BASE* Clone( void )
	{
		sUSER_EXIT* pData = new sUSER_EXIT;
		return pData;
	}
};

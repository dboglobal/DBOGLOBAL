/*****************************************************************************
*
* File			: DLMessageDef.h
* Author		: 
* Copyright		: (аж) NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          : 
*
*****************************************************************************/


#pragma once


//////////////////////////////////////////////////////////////////////////
//
// Message definition
//
//////////////////////////////////////////////////////////////////////////


enum eDL_MSG
{
	eDL_MSG_ERROR_NOT_PREPARED_LAUNCHER,

	eDL_MSG_ERROR_DOWNLOAD_FAILED,
	eDL_MSG_ERROR_PATCH_FAILED,
	eDL_MSG_ERROR_INTEGRITY_CHECK_FAILED,

	eDL_MSG_MUTEX_CHECK_CLIENT,
	eDL_MSG_MUTEX_CHECK_LAUNCHER,

	eDL_MSG_PREPARE_NEW_PATCH,
};

/*****************************************************************************
 *
 * File			: NtlEventDonwLoader.h
 * Author		: HongHoDong
 * Copyright	: (주)NTL
 * Date			: 2007. 2. 6.
 * Abstract		: 
 *****************************************************************************
 * Desc         : DownLoad 중에 발생하는 Event
 *
 *****************************************************************************/
#pragma once
#include "DataType.h"
#define EVENT_DL_ID_START					(LAUNCHER_EVENT_USER + 0)

#define EVENT_DL_COMPLETE_THIS_FILE			(1000 + EVENT_DL_ID_START)
#define EVENT_DL_PENDING_DESTORY			(1001 + EVENT_DL_ID_START)

#define EVENT_DL_ERROR_SERVER_CONNECT		(2001 + EVENT_DL_ID_START)
#define	EVENT_DL_ERROR_SERVER_FILE_DOWN		(2002 + EVENT_DL_ID_START)

#define EVENT_DL_DEAFULT_MSG_PUMP			(2003 + EVENT_DL_ID_START)

struct EventDLCompleteThisFileMsg
{
	unsigned int uiFileSize;
	unsigned int uiComplete;
	char	*pszFileName;
};

struct EventDLTextMsg
{
	char *pszMessage;
};






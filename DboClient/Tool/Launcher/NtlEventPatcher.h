/*****************************************************************************
 *
 * File			: NtlEventPatcher.h
 * Author		: HongHoDong
 * Copyright	: (주)NTL
 * Date			: 2007. 2. 6.
 * Abstract		: 
 *****************************************************************************
 * Desc         : Patch 중 발생 하는 Event
 *
 *****************************************************************************/
#pragma once
#include "DataType.h"

#define EVENT_PH_ID_START							(LAUNCHER_EVENT_USER + 50000)

#define EVENT_PH_RT_WARNING_MESSAGE_HEADER			(1001 + EVENT_PH_ID_START)
#define EVENT_PH_RT_WARNING_MESSAGE_TEXT			(1002 + EVENT_PH_ID_START)
#define EVENT_PH_RT_ERROR_MESSAGE_HEADER			(1003 + EVENT_PH_ID_START)
#define EVENT_PH_RT_ERROR_MESSAGE_TEXT				(1004 + EVENT_PH_ID_START)
#define EVENT_PH_RT_COMPLETE_THIS_FILE				(1005 + EVENT_PH_ID_START)	//% complete this file
#define EVENT_PH_RT_NUMBER_OF_FILES					(1006 + EVENT_PH_ID_START)

#define EVENT_PH_RT_FILE_PATCH_START				(1007 + EVENT_PH_ID_START)
#define EVENT_PH_RT_CURRENT_FILE_PATCH_COMPLETE		(1008 + EVENT_PH_ID_START)
#define	EVENT_PH_RT_PROGRESS_MESSAGE				(1009 + EVENT_PH_ID_START)				

#define EVENT_PH_PENDING_DESTROY					(5001 + EVENT_PH_ID_START)
#define EVENT_PH_DEFAULT_MSG_PUMP					(5002 + EVENT_PH_ID_START)


struct EventPHTextMsg
{
	char	*pszMessage;
};

struct EventPHCompleteThisFileMsg
{
	unsigned int uiComplete;
};

struct EventPHNumberOfFilesMsg
{
	int	nNumberOfFiles;
};

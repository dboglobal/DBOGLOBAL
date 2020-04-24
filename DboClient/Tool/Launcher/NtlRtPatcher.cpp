#include "StdAfx.h"
#include "NtlRtPatcher.h"

CNtlRtPatcher::CNtlRtPatcher()
{
}

CNtlRtPatcher::~CNtlRtPatcher()
{
	
}

LPVOID CALLBACK CallBack( UINT Id, LPVOID Parm );

BOOL CNtlRtPatcher::DoAction(const char *pszDestPatchPath, const char *pszCurFullPathFileName)
{
	char pszDestPatchFullPath[1024];

	GetCurrentDirectory(sizeof(pszDestPatchFullPath), pszDestPatchFullPath);
	strcat_s(pszDestPatchFullPath, pszDestPatchPath);

	char pszCmdLine[1024];	
    sprintf_s(pszCmdLine, "\"%s\" \"%s\" -i", pszCurFullPathFileName, pszDestPatchFullPath);
	
	SetActive(TRUE);
	INT nRetVal = RTPatchApply32( pszCmdLine, CallBack, TRUE );
	SetActive(FALSE);
	
	//Success
	if(nRetVal == 0)
	{
		return TRUE;
	}
	//User Abort
	else if(nRetVal == 0x8001)
	{
		SetPendingDestroy(TRUE);
		//CNtlRtPatcher::GetInstance()->SendEvent(EVENT_PH_PENDING_DESTROY, NULL);
		return TRUE;
	}
	//Error
	else
	{
		//ASSERT(0);
		OutputDebugString("RtPatch File Patch중 Rrror");
		return FALSE;
	}

	return TRUE;
}

LPVOID CALLBACK CallBack( UINT Id, LPVOID Parm )
{ 
	LPVOID RetVal;
	RetVal = "";
	
	switch( Id )
	{
		case 1: //Warning message header
		{
			EventPHTextMsg sTextMsg;
			sTextMsg.pszMessage = (char *) Parm;
			CNtlRtPatcher::GetInstance()->SendEvent(EVENT_PH_RT_WARNING_MESSAGE_HEADER, &sTextMsg);
			break;
		}

		case 2: //Warning message text
		{
			EventPHTextMsg sTextMsg;
			sTextMsg.pszMessage = (char *) Parm;
			CNtlRtPatcher::GetInstance()->SendEvent(EVENT_PH_RT_WARNING_MESSAGE_TEXT, &sTextMsg);
			break;
		}
		
		case 3: //Error message header
		{
			EventPHTextMsg sTextMsg;
			sTextMsg.pszMessage = (char *) Parm;
			CNtlRtPatcher::GetInstance()->SendEvent(EVENT_PH_RT_ERROR_MESSAGE_HEADER, &sTextMsg);
			break;
		}

		case 4: //Error message text
		{
			EventPHTextMsg sTextMsg;
			sTextMsg.pszMessage = (char *) Parm;
			CNtlRtPatcher::GetInstance()->SendEvent(EVENT_PH_RT_ERROR_MESSAGE_TEXT, &sTextMsg);
			break;
		}
		
		case 5: //% complete this file
		{
			EventPHCompleteThisFileMsg sCompleteThisFileMsg;
			sCompleteThisFileMsg.uiComplete = (unsigned int) (((long)(0xffff & *((UINT *) Parm))*100L)/0x8000L);
			CNtlRtPatcher::GetInstance()->SendEvent(EVENT_PH_RT_COMPLETE_THIS_FILE, &sCompleteThisFileMsg);
			break;
		}
		
		case 6: //Number of files to patch
		{
			EventPHNumberOfFilesMsg sNumberOfFilesMsg;
			sNumberOfFilesMsg.nNumberOfFiles = *(int *) Parm;
			CNtlRtPatcher::GetInstance()->SendEvent(EVENT_PH_RT_NUMBER_OF_FILES, &sNumberOfFilesMsg);
			break;
		}

		case 7: //File Patch Start
		{
			EventPHTextMsg sTextMsg;
			sTextMsg.pszMessage = (char *) Parm;            
			CNtlRtPatcher::GetInstance()->SendEvent(EVENT_PH_RT_FILE_PATCH_START, &sTextMsg);			
			break;
		}
		
		case 8: //Current file patch complete
		{
			CNtlRtPatcher::GetInstance()->SendEvent(EVENT_PH_RT_CURRENT_FILE_PATCH_COMPLETE, NULL);
			break;
		}
		
		
		case 9: //Progress message
		{
			//CNtlRtPatcher::GetInstance()->SendEvent(EVENT_PH_RT_CURRENT_FILE_PATCH_COMPLETE, NULL);
			int i = 0;
			break;
		}
		

		default:
		{
			break;
		}
	}
	
	CNtlRtPatcher::GetInstance()->SendEvent(EVENT_PH_DEFAULT_MSG_PUMP, NULL);

	//Abort일 경우 종료를 한다.
	if( CNtlRtPatcher::GetInstance()->GetAbort())
		return NULL;

	return (RetVal);
}

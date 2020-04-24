#include "precomp_dboclient.h"
#include "DboExceptionHandler.h"

// framework
#include "NtlStage.h"

// dbo
#include "callstack.h"
#include "NtlCoreUtil.h"
#include "DboGlobal.h"
#include "DboExtraErrorReport.h"
#include "DboDef.h"
#include "StageManager.h"
#include "DboLogic.h"

//woody
#include "NtlMaterialExt.h"

VOID DboExceptionHandler( PEXCEPTION_POINTERS pExceptionInfo )
{
	// 게임 정보를 저장한다.
	CNtlStage *pStage = CStageManager::GetInstance()->GetActiveStage();
	if(pStage)
	{
		std::string strStageName = pStage->GetName();
		if(strStageName == GAME_STAGE_NAME)
		{
			/*Logic_SaveCharacterGameInformation();*/
			Logic_SaveAccountOption();
			Logic_SaveGameOption();
			Logic_SaveSystemOption();
			Logic_SaveScouterOption();
			Logic_SaveQuestOption();
			Logic_SaveCharacterOption();
		}
	}

    // 추가 정보 로그 파일을 기록한다.
    CDboExtraErrorReport extraErrorReport;
    extraErrorReport.WriteInfo("USER_ID", CDboGlobal::GetInstance()->GetUserData()->wchUserID);

    char szVer[64] = {0,};
    sprintf_s(szVer, "%d.%d", CLIENT_LVER, CLIENT_RVER);
    extraErrorReport.WriteInfo("CLIENT_VER", szVer);

	sprintf_s(szVer, "%s", g_ToonErrStr[0]);
	extraErrorReport.WriteInfo("TOON_GEOMS", szVer);
						   
	sprintf_s(szVer, "%s", g_ToonErrStr[1]);
	extraErrorReport.WriteInfo("TOON_PAINT", szVer);


    extraErrorReport.Save(ERROR_REPORT_FILE_NAME);

	GetNtlCallStack()->LogFilePrint();
}
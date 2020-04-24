#include "precomp_dboclient.h"
#include "DBOCheatCommand.h"

// shared
#include "QuestNarrationTable.h"
#include "TableContainer.h"
#include "PCTable.h"
#include "TableFileNameList.h"
#include "NtlMovement.h"

// core
#include "NtlDebug.h"
#include "DumpCommand.h"
#include "NtlLuaState.h"

// sound
#include "NtlSoundEventGenerator.h"

// Presentation
#include "NtlPLGlobal.h"
#include "NtlPLCharacter.h"
#include "NtlPLOptionManager.h"
#include "NtlPLDojoContainer.h"
#include "NtlPLPalette.h"
#include "NtlPLCullingScheduling.h"

// Simulation
#include "NtlSLGlobal.h"
#include "NtlSLLuaGlue.h"
#include "NtlSLLogic.h"
#include "NtlSobAvatar.h"
#include "NtlSLLuaGlue.h"
#include "NtlWorldConceptTutorial.h"
#include "NtlSobProxy.h"
#include "NtlSLEventFunc.h"
#include "NtlSLApi.h"
#include "NtlSobAttr.h"
#include "NtlSobFactory.h"
#include "NtlSobAvatarAttr.h"
#include "NtlSobCharPerfController.h"
#include "NtlSobManager.h"

// dbo
#include "DboEventGenerator.h"
#include "AlarmManager.h"
#include "DboLogic.h"
#include "DialogManager.h"		/// woosungs_test
#include "SideDialogManager.h"	/// woosungs_test
#include "PacketTracer.h"

// NtlGui						/// woosungs_test
#include "gui_component.h"		
#include "gui_component_generic.h"		
#include "gui_guimanager_generic.h"		
#include "DisplayStringManager.h"		
#include "CommercialExtendGui.h"

CDBOCheatCommand::CDBOCheatCommand()
:m_hSound(INVALID_SOUND_HANDLE)
{
    Register();
	m_iCurrOpenedDialogIndex = 0;
}

CDBOCheatCommand::~CDBOCheatCommand()
{
    m_mapCheatHandler.clear();
}

void CDBOCheatCommand::Register() 
{
	m_mapCheatHandler["elapsedtime"]		= &CDBOCheatCommand::OnElapsedTime;

    m_mapCheatHandler["dt_console"]			= &CDBOCheatCommand::OnDumpToConsole;      
    m_mapCheatHandler["dt_gui"]				= &CDBOCheatCommand::OnDumpToGui;
    m_mapCheatHandler["dt_file"]			= &CDBOCheatCommand::OnDumpToFile;
    m_mapCheatHandler["dump"]				= &CDBOCheatCommand::OnDump;
	m_mapCheatHandler["dump_action"]		= &CDBOCheatCommand::OnDump_Action;
	m_mapCheatHandler["dump_sobcd"]			= &CDBOCheatCommand::OnDump_Sob;
	m_mapCheatHandler["dump_fsm"]			= &CDBOCheatCommand::OnDump_FSM;
	m_mapCheatHandler["dump_contents"]		= &CDBOCheatCommand::OnDump_Contents;

    m_mapCheatHandler["compilelua"]			= &CDBOCheatCommand::OnCompileLua;
	m_mapCheatHandler["runlua"]				= &CDBOCheatCommand::OnRunLua;
    m_mapCheatHandler["cangle"]				= &CDBOCheatCommand::OnCAngle;
    m_mapCheatHandler["sangle"]				= &CDBOCheatCommand::OnSAngle;
	m_mapCheatHandler["speech"]				= &CDBOCheatCommand::OnSpeech;
	m_mapCheatHandler["resetgui"]			= &CDBOCheatCommand::OnResetGui;
	m_mapCheatHandler["matrixdelaytime"]	= &CDBOCheatCommand::OnKnockdownMatrixDelayTime;
	m_mapCheatHandler["matrixtime"]			= &CDBOCheatCommand::OnKnockdownMatrixTime;
	m_mapCheatHandler["matrixvalue"]		= &CDBOCheatCommand::OnKnockdownMatrixValue;
	m_mapCheatHandler["matrixcamera"]		= &CDBOCheatCommand::OnKnockdownMatrixCameraLifeTime;
	m_mapCheatHandler["packlockmsg"]		= &CDBOCheatCommand::OnPacketLockMsg;
	m_mapCheatHandler["servermsg"]			= &CDBOCheatCommand::OnServerMsg;
	m_mapCheatHandler["shadow"]				= &CDBOCheatCommand::OnShadow;
	m_mapCheatHandler["charloadtime"]		= &CDBOCheatCommand::OnCharLoadTime;
    m_mapCheatHandler["test"]               = &CDBOCheatCommand::OnTest;
    m_mapCheatHandler["test2"]              = &CDBOCheatCommand::OnTest2;
    m_mapCheatHandler["test3"]              = &CDBOCheatCommand::OnTest3;
	m_mapCheatHandler["tutorialscript"]     = &CDBOCheatCommand::OnTutorialScript;
	m_mapCheatHandler["fn"]					= &CDBOCheatCommand::OnFlashNotify;
	m_mapCheatHandler["avatarshow"]			= &CDBOCheatCommand::OnAvatarShowOnOff;
	m_mapCheatHandler["serverchangeout"]	= &CDBOCheatCommand::OnServerChangeOut;
	m_mapCheatHandler["lowcharacter"]		= &CDBOCheatCommand::OnLowSpecCharacter;
	m_mapCheatHandler["skipedge"]			= &CDBOCheatCommand::OnSkipEdge;
	m_mapCheatHandler["skipterrain"]		= &CDBOCheatCommand::OnSkipTerrain;
	m_mapCheatHandler["objectdist"]			= &CDBOCheatCommand::OnObjectDistance;
	m_mapCheatHandler["terraindist"]		= &CDBOCheatCommand::OnTerrainDistance;
    m_mapCheatHandler["lowspeceffect"]      = &CDBOCheatCommand::OnLowSpecEffect;
    m_mapCheatHandler["lowspeceffectratio"] = &CDBOCheatCommand::OnLowSpecEffectRatio;
    m_mapCheatHandler["rendermesh"]         = &CDBOCheatCommand::OnRenderMesh;
    m_mapCheatHandler["renderdecal"]        = &CDBOCheatCommand::OnRenderDecal;
	m_mapCheatHandler["scrollfactor"]		= &CDBOCheatCommand::OnTestInfoScrollTime;	

	m_mapCheatHandler["renderspecular"]			    = &CDBOCheatCommand::OnRenderWaterSpecular;
	m_mapCheatHandler["renderterrainshadow"]		= &CDBOCheatCommand::OnRenderTerrainShadow;

	m_mapCheatHandler["weatherlevel"]		= &CDBOCheatCommand::OnWeatherLevel;

	m_mapCheatHandler["broadcastposx"]		= &CDBOCheatCommand::OnBroadCastPosX;
	m_mapCheatHandler["mininarration"]		= &CDBOCheatCommand::OnMiniNarration;

	m_mapCheatHandler["animsync"]			= &CDBOCheatCommand::OnAnimSyncActive;

	m_mapCheatHandler["reloadts"]			= &CDBOCheatCommand::OnTSReload;

	m_mapCheatHandler["dpreview"]			= &CDBOCheatCommand::OnDeveloperPreview;

	m_mapCheatHandler["dojo"]				= &CDBOCheatCommand::OnDojo;
    m_mapCheatHandler["runanimspeed"]       = &CDBOCheatCommand::OnRunAnimSpeedChange;

	m_mapCheatHandler["createpc"]			= &CDBOCheatCommand::OnCreatePC;

	m_mapCheatHandler["loadpalette"]		= &CDBOCheatCommand::LoadPalette;	

	m_mapCheatHandler["reloadtable"]		= &CDBOCheatCommand::ReloadTable;

//	m_mapCheatHandler["testcull"]               = &CDBOCheatCommand::OnTestCull;

	m_mapCheatHandler["crash"]				= &CDBOCheatCommand::OnCrash;

	m_mapCheatHandler["perf"]				= &CDBOCheatCommand::OnPerf;

	m_mapCheatHandler["dialog"]				= &CDBOCheatCommand::OnDialog;			/// woosungs_test	
	// _ENABLE_PACKET_TRACE_	/// Defalut로 Disable /// 프로토콜이 변경될수있다. 자동빌드가 실패할 수 있음
	m_mapCheatHandler["packettrace"]		= &CDBOCheatCommand::OnPacketTrace;		/// woosungs_test 20090804	

	// sound
	m_mapCheatHandler["playmanysound"]		= &CDBOCheatCommand::OnPlayManySound;
	m_mapCheatHandler["stopmanysound"]		= &CDBOCheatCommand::OnStopManySound;

	m_mapCheatHandler["minmaxrate"]			= &CDBOCheatCommand::OnMinMaxRate;

	m_mapCheatHandler["playsound"]			= &CDBOCheatCommand::OnPlaySound;
	m_mapCheatHandler["stopsound"]			= &CDBOCheatCommand::OnStopSound;

	m_mapCheatHandler["sharebgmrate"]		= &CDBOCheatCommand::OnShareBGMRate;

	m_mapCheatHandler["destmove"]			= &CDBOCheatCommand::OnDestMove;
}

RwBool CDBOCheatCommand::OnCmdParsing( const std::string& strCmd ) 
{
    CNtlTokenizer lexer(strCmd.c_str());
    if(!lexer.IsSuccess())
        return FALSE;

    // 첫 시작은 '/'로 시작해야 한다.
    RwInt32 iLine;
    std::string strToken = lexer.PeekNextToken(NULL, &iLine);
    if(strToken != "/")
        return FALSE;

    for(;;)
    {
        strToken = lexer.PeekNextToken(NULL, &iLine);

        if(strToken.empty())
            break;   

        MapCheatHandler::iterator it = m_mapCheatHandler.find(strToken);
        if(it != m_mapCheatHandler.end())
        {
            if((this->*(*it).second)(&lexer))
                return TRUE;
            
        }        
    }

    return FALSE;
}

RwBool CDBOCheatCommand::OnElapsedTime(CNtlTokenizer* pLexer)
{
	RwInt32 iLine;
    std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);
    g_fElapsedWeightValue = (RwReal)atof(strCommand.c_str());

	return TRUE;
}

RwBool CDBOCheatCommand::OnDump( CNtlTokenizer* pLexer ) 
{
    RwInt32 iLine;
    std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);
    if(strCommand == "avatar")
    {
        GetDumpCmdManager()->OutputSobObj(GetNtlSLGlobal()->GetSobAvatar()->GetSerialID());
    }
    else if(strCommand == "target")
    {   
        GetDumpCmdManager()->OutputSobObj(GetNtlSLGlobal()->GetSobAvatar()->GetTargetID());
    }   
	else if(strCommand == "regi")
	{
		GetDumpCmdManager()->m_hRegisterSerialId = GetNtlSLGlobal()->GetSobAvatar()->GetTargetID();
	}
	else if(strCommand == "unreg")
	{
		GetDumpCmdManager()->m_hRegisterSerialId = INVALID_SERIAL_ID;
	}
	else if(strCommand == "reg")
	{
		strCommand = pLexer->PeekNextToken(NULL, &iLine);
		GetDumpCmdManager()->OutputSobObj(atoi(strCommand.c_str()));
	}
    else
    {
        return FALSE;
    }

    return TRUE;
}

RwBool CDBOCheatCommand::OnDumpToConsole( CNtlTokenizer* pLexer ) 
{
    RwInt32 iLine;
    std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);
    if(strCommand == "on")
    {
        GetDumpCmdManager()->ActiveDumpTarget(CDumpCmdManager::DUMP_TARGET_CONSOLE, TRUE);
    }
    else if(strCommand == "off")
    {
        GetDumpCmdManager()->ActiveDumpTarget(CDumpCmdManager::DUMP_TARGET_CONSOLE, FALSE);
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

RwBool CDBOCheatCommand::OnDumpToGui( CNtlTokenizer* pLexer ) 
{
    RwInt32 iLine;
    std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);
    if(strCommand == "on")
    {
        GetDumpCmdManager()->ActiveDumpTarget(CDumpCmdManager::DUMP_TARGET_GUI, TRUE);
    }
    else if(strCommand == "off")
    {
        GetDumpCmdManager()->ActiveDumpTarget(CDumpCmdManager::DUMP_TARGET_GUI, FALSE);
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

RwBool CDBOCheatCommand::OnDumpToFile( CNtlTokenizer* pLexer ) 
{
    RwInt32 iLine;
    std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);
    if(strCommand == "on")
    {
		GetAlarmManager()->AlarmMessage(L"file dump target on");
        GetDumpCmdManager()->ActiveDumpTarget(CDumpCmdManager::DUMP_TARGET_FILE, TRUE);
    }
    else if(strCommand == "off")
    {
		GetAlarmManager()->AlarmMessage(L"file dump target off");
        GetDumpCmdManager()->ActiveDumpTarget(CDumpCmdManager::DUMP_TARGET_FILE, FALSE);
    }
    else 
    {
        return FALSE;
    }

    return TRUE;
}

RwBool CDBOCheatCommand::OnDump_Action(CNtlTokenizer* pLexer)
{
	RwInt32 iLine;
	std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);
	if(strCommand == "on")
	{
		GetAlarmManager()->AlarmMessage(L"action dump on");
		GetDumpCmdManager()->ActiveDumpType(DUMP_ACTION, TRUE);
	}
	else if(strCommand == "off")
	{
		GetAlarmManager()->AlarmMessage(L"action dump off");
		GetDumpCmdManager()->ActiveDumpType(DUMP_ACTION, FALSE);
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

RwBool CDBOCheatCommand::OnDump_Sob(CNtlTokenizer* pLexer)
{
	RwInt32 iLine;
	std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);
	if(strCommand == "on")
	{
		GetAlarmManager()->AlarmMessage(L"sob create/destroy dump on");
		GetDumpCmdManager()->ActiveDumpType(DUMP_SOB_CREATE_DESTROY, TRUE);
	}
	else if(strCommand == "off")
	{
		GetAlarmManager()->AlarmMessage(L"sob create/destroy dump off");
		GetDumpCmdManager()->ActiveDumpType(DUMP_SOB_CREATE_DESTROY, FALSE);
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

RwBool CDBOCheatCommand::OnDump_FSM(CNtlTokenizer* pLexer)
{
	RwInt32 iLine;
	std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);
	if(strCommand == "on")
	{
		GetAlarmManager()->AlarmMessage(L"FSM dump on");
		GetDumpCmdManager()->ActiveDumpType(DUMP_FSM, TRUE);
	}
	else if(strCommand == "off")
	{
		GetAlarmManager()->AlarmMessage(L"FSM dump off");
		GetDumpCmdManager()->ActiveDumpType(DUMP_FSM, FALSE);
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

RwBool CDBOCheatCommand::OnDump_Contents(CNtlTokenizer* pLexer)
{
	RwInt32 iLine;
	std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);
	if(strCommand == "on")
	{
		GetAlarmManager()->AlarmMessage(L"contents dump on");
		GetDumpCmdManager()->ActiveDumpType(DUMP_CONTENTS, TRUE);
	}
	else if(strCommand == "off")
	{
		GetAlarmManager()->AlarmMessage(L"contents dump off");
		GetDumpCmdManager()->ActiveDumpType(DUMP_CONTENTS, FALSE);
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

RwBool CDBOCheatCommand::OnCompileLua( CNtlTokenizer* pLexer ) 
{
    SLLua_Setup();
    return TRUE;
}

RwBool CDBOCheatCommand::OnRunLua(CNtlTokenizer* pLexer)
{
	RwInt32 iLine;
    std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);

	strCommand += "()";

	// lua run
	CNtlLuaState *pLuaState = GetLuaState();
	pLuaState->RunString(strCommand.c_str());

	return TRUE;
}

RwBool CDBOCheatCommand::OnCAngle( CNtlTokenizer* pLexer ) 
{
    RwInt32 iLine;
    std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);
    g_fCharColliAngle = (RwReal)atof(strCommand.c_str());
    
    return TRUE;    
}

RwBool CDBOCheatCommand::OnSAngle( CNtlTokenizer* pLexer ) 
{
    RwInt32 iLine;
    std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);
    g_fCharSlippingAngle = (RwReal)atof(strCommand.c_str());    

    return TRUE;
}


RwBool CDBOCheatCommand::OnSpeech( CNtlTokenizer* pLexer ) 
{
    RwInt32 iLine;
    std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);
    //g_fCharSlippingAngle = (RwReal)atof(strCommand.c_str());    
	g_uiFlashBalloonWidth = (RwUInt32)atoi(strCommand.c_str());
	strCommand = pLexer->PeekNextToken(NULL, &iLine);
	g_uiFlashBalloonHeight = (RwUInt32)atoi(strCommand.c_str());
    return TRUE;
}

RwBool CDBOCheatCommand::OnResetGui(CNtlTokenizer* pLexer)
{
	CDboEventGenerator::Debug_for_Developer(DEVELOPER_RESET_GUI);
	return TRUE;
}

RwBool CDBOCheatCommand::OnKnockdownMatrixDelayTime(CNtlTokenizer* pLexer)
{
	RwInt32 iLine;
    std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);
    g_fMatrixDelayTime = (RwReal)atof(strCommand.c_str());
	
	return TRUE;
}


RwBool CDBOCheatCommand::OnKnockdownMatrixTime(CNtlTokenizer* pLexer)
{
	RwInt32 iLine;
    std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);
    g_fMatrixTime = (RwReal)atof(strCommand.c_str());
	
	return TRUE;
}


RwBool CDBOCheatCommand::OnKnockdownMatrixValue(CNtlTokenizer* pLexer)
{
	RwInt32 iLine;
    std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);
    g_fMatrixValue = (RwReal)atof(strCommand.c_str());
	
	return TRUE;
}

RwBool CDBOCheatCommand::OnKnockdownMatrixCameraLifeTime(CNtlTokenizer* pLexer)
{
	RwInt32 iLine;
    std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);
    g_fMatrixCamera = (RwReal)atof(strCommand.c_str());
	
	return TRUE;
}


RwBool CDBOCheatCommand::OnPacketLockMsg(CNtlTokenizer* pLexer)
{
	RwInt32 iLine;
    std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);

	if(strCommand == "on")
	{
		g_bDispPacketLockMsg = TRUE;
	}
	else if(strCommand == "off")
	{
		g_bDispPacketLockMsg = FALSE;
	}
    	
	return TRUE;
}

RwBool CDBOCheatCommand::OnServerMsg(CNtlTokenizer* pLexer)
{
	RwInt32 iLine;
    std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);

    if(strCommand == "on")
	{
		g_bDispServerMsg = TRUE;
	}
	else if(strCommand == "off")
	{
		g_bDispServerMsg = FALSE;
	}
	
	return TRUE;
}


RwBool CDBOCheatCommand::OnShadow(CNtlTokenizer* pLexer)
{
	RwInt32 iLine;
    std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);
	g_byShadowRed = (RwUInt8)atoi(strCommand.c_str());

	strCommand = pLexer->PeekNextToken(NULL, &iLine);
	g_byShadowGreen = (RwUInt8)atoi(strCommand.c_str());

	strCommand = pLexer->PeekNextToken(NULL, &iLine);
	g_byShadowBlue = (RwUInt8)atoi(strCommand.c_str());

	return TRUE;
}

/**
* \brief PL단의 캐릭터 스케쥴링 로딩에 쓰일 변수
*/
RwBool CDBOCheatCommand::OnCharLoadTime( CNtlTokenizer* pLexer ) 
{
	RwInt32 iLine;
	std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);

	g_fCharScheduleTime = (RwReal)atof(strCommand.c_str());

	return TRUE;
}

RwBool CDBOCheatCommand::OnTest(CNtlTokenizer* pLexer)
{
    RwInt32 iLine;
    std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);
    
    g_fTestVal = (RwReal)atof(strCommand.c_str());

	/// commercialextend test
	std::string strCommand2 = pLexer->PeekNextToken(&iLine, &iLine);
	CNtlPLGui* pDialog = GetDialogManager()->GetpDialogTEST(strCommand.c_str());
	if(pDialog)
	{
		pDialog->Show(true);
		if(!strCommand2.empty())
		{
			CCommercialExtendGui* pExtendDialog = (CCommercialExtendGui*)pDialog;
			if(strCommand2 == "1")	
			{
				pExtendDialog->OpenDialog(CCommercialExtendGui::ZENNY_EXTEND, NULL, 80);
			}
			else if(strCommand2 == "2")	
			{
				pExtendDialog->OpenDialog(CCommercialExtendGui::NETPY_EXTEND, NULL, 80);
			}
			else if(strCommand2 == "3")	
			{
				pExtendDialog->OpenDialog(CCommercialExtendGui::CASH_EXTEND, NULL, 80);
			}
			else if(strCommand2 == "4")	
			{
				pExtendDialog->OpenDialog(CCommercialExtendGui::CASH_BUY, NULL, 80);
			}
		}
	}
	else
	{
		if(strCommand == "1")	
		{
			GetAlarmManager()->FormattedAlarmMessage("DST_COMMERCIAL_MB_DIRECT_BUY_MSG", FALSE, NULL, L"test_item", 1000);
		}
		else if(strCommand == "2")	
		{
			GetAlarmManager()->AlarmMessage("DST_COMMERCIAL_MB_ITEM_BUY_MSG");
		}
		else if(strCommand == "3")	
		{
			GetAlarmManager()->AlarmMessage("DST_COMMERCIAL_MB_CAPSULEKIT_ITEM_TERM_EXTENTION");
		}
		else if(strCommand == "4")	
		{
			GetAlarmManager()->AlarmMessage("DST_COMMERCIAL_MB_ITEM_BUY_CONFIRM_MSG");
		}
		else if(strCommand == "9")	
		{
			GetAlarmManager()->AlarmMessage("DST_COMMERCIAL_MB_ITEM_BUY_CONFIRM_MSG");
		}
	}

    return TRUE;
}

RwBool CDBOCheatCommand::OnTest2( CNtlTokenizer* pLexer ) 
{
    RwInt32 iLine;
    std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);

    g_fTestVal2 = (RwReal)atof(strCommand.c_str());

    return TRUE;
}

RwBool CDBOCheatCommand::OnTest3( CNtlTokenizer* pLexer ) 
{
    RwInt32 iLine;
    std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);

    g_fTestVal3 = (RwReal)atof(strCommand.c_str());

    return TRUE;
}

RwBool CDBOCheatCommand::OnTestInfoScrollTime( CNtlTokenizer* pLexer )
{
	RwInt32 iLine;
	std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);

	g_nTestVal = atoi(strCommand.c_str());

	return TRUE;
}

RwBool CDBOCheatCommand::OnTutorialScript(CNtlTokenizer* pLexer)
{
	CNtlWorldConceptTutorial* pTutorialConcept = reinterpret_cast<CNtlWorldConceptTutorial*>( GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_TUTORIAL) );
	if( pTutorialConcept == NULL )
		return TRUE;

	pTutorialConcept->LoadScript();

	return TRUE;
}

RwBool CDBOCheatCommand::OnFlashNotify(CNtlTokenizer* pLexer)
{
	RwInt32 iLine;
	std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);

	CDboEventGenerator::FlashNotifyString( strCommand.c_str(), NULL, FALSE );

	return TRUE;
}


RwBool CDBOCheatCommand::OnAvatarShowOnOff(CNtlTokenizer* pLexer)
{
	RwInt32 iLine;
    std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);
    
    RwBool bShow = (RwBool)atoi(strCommand.c_str());

	static CNtlSobProxySystemEffect *pSystemEffect = NULL;
	
	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlSobProxy *pSobProxy = pSobAvatar->GetSobProxy();

	if(bShow)
	{
		if(pSystemEffect)
		{
			pSobProxy->RemoveVisualSystemEffectAlpha(pSystemEffect);
			pSystemEffect = NULL;
		}
	}
	else
	{
		if(pSystemEffect)
		{
			pSobProxy->RemoveVisualSystemEffectAlpha(pSystemEffect);
		}

		pSystemEffect = pSobProxy->AddVisualSystemEffectAlphaBlend(0.0f, 10000.0f, TRUE);
	}

	return TRUE;
}

RwBool CDBOCheatCommand::OnServerChangeOut(CNtlTokenizer* pLexer)
{
	CNtlSLEventGenerator::GameServerChangeOut();
	GetNtlSLGlobal()->GetSobAvatar()->ServerChangeOut();

	return TRUE;
}

RwBool CDBOCheatCommand::OnLowSpecCharacter(CNtlTokenizer* pLexer)
{
	RwInt32 iLine;
    std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);
    
    RwBool bLowSpec = (RwBool)atoi(strCommand.c_str());

	if(bLowSpec)
		CNtlPLCharacter::SetSkipSimpleMaterial(FALSE);
	else
		CNtlPLCharacter::SetSkipSimpleMaterial(TRUE);
		

	return TRUE;
}

RwBool CDBOCheatCommand::OnLowSpecEffect( CNtlTokenizer* pLexer ) 
{
    RwInt32 iLine;
    std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);

    RwBool bLowSpec = (RwBool)atoi(strCommand.c_str());
    CNtlInstanceEffect::SetLowSpecEnable(bLowSpec);    

    return TRUE;

}

RwBool CDBOCheatCommand::OnLowSpecEffectRatio( CNtlTokenizer* pLexer ) 
{
    RwInt32 iLine;
    std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);

    RwReal fLowSpecRatio = (RwReal)atof(strCommand.c_str());
    CNtlInstanceEffect::SetLowSpecRatio(fLowSpecRatio);    

    return TRUE;

}


RwBool CDBOCheatCommand::OnSkipEdge(CNtlTokenizer* pLexer)
{
	RwInt32 iLine;
    std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);
    
    RwBool bSkipEdge = (RwBool)atoi(strCommand.c_str());

	if(bSkipEdge)
		CNtlPLCharacter::SetSkipEdge(TRUE);
	else
		CNtlPLCharacter::SetSkipEdge(FALSE);

	return TRUE;
}

RwBool CDBOCheatCommand::OnSkipTerrain(CNtlTokenizer* pLexer)
{
	RwInt32 iLine;
    std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);
    
    RwBool bSkipTerrain = (RwBool)atoi(strCommand.c_str());

	if(bSkipTerrain)
		CNtlPLGlobal::m_bWorldTerrainVisible = FALSE;
	else
		CNtlPLGlobal::m_bWorldTerrainVisible = TRUE;

	return TRUE;
}

RwBool CDBOCheatCommand::OnObjectDistance(CNtlTokenizer* pLexer)
{
	RwInt32 iLine;
    std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);
    
    RwReal fDist = (RwReal)atof(strCommand.c_str());

	GetNtlPLOptionManager()->SetObjectFar(fDist);

	return TRUE;
}

RwBool CDBOCheatCommand::OnTerrainDistance(CNtlTokenizer* pLexer)
{
	RwInt32 iLine;
    std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);
    
    RwReal fDist = (RwReal)atof(strCommand.c_str());

	GetNtlPLOptionManager()->SetTerrainFar(fDist);

	return TRUE;
}

RwBool CDBOCheatCommand::OnRenderMesh(CNtlTokenizer* pLexer)
{
    RwInt32 iLine;
    std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);
    RwBool bEnable = (RwBool)atoi(strCommand.c_str());
    CNtlInstanceEffect::SetRenderMeshSystem(bEnable);

    return TRUE;
}

RwBool CDBOCheatCommand::OnRenderDecal(CNtlTokenizer* pLexer)
{
    RwInt32 iLine;
    std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);
    RwBool bEnable = (RwBool)atoi(strCommand.c_str());
    CNtlInstanceEffect::SetRenderDecalSystem(bEnable);

    return TRUE;
}

RwBool CDBOCheatCommand::OnRenderWaterSpecular(CNtlTokenizer* pLexer)
{
	RwInt32 iLine;
	std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);
	RwBool bEnable = (RwBool)atoi(strCommand.c_str());

	GetNtlPLOptionManager()->SetWaterSpecular(bEnable);

	return TRUE;
}

RwBool CDBOCheatCommand::OnRenderTerrainShadow(CNtlTokenizer* pLexer)
{
	RwInt32 iLine;
	std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);
	RwBool bEnable = (RwBool)atoi(strCommand.c_str());
	
	GetNtlPLOptionManager()->SetTerrainShadow(bEnable);

	return TRUE;
}

RwBool CDBOCheatCommand::OnWeatherLevel(CNtlTokenizer* pLexer)
{
	RwInt32 iLine;
	std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);

	GetNtlPLOptionManager()->SetWeatherLevel(atoi(strCommand.c_str()));

	return TRUE;
}

RwBool CDBOCheatCommand::OnBroadCastPosX( CNtlTokenizer* pLexer )
{
	RwInt32 iLine;
	std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);

	g_nBroadCastGuiPosX = atoi(strCommand.c_str());

	return TRUE;
}

RwBool CDBOCheatCommand::OnMiniNarration(CNtlTokenizer* pLexer)
{
	RwInt32 iLine;
	std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);
	RwUInt32 uiMiniNarrationIndex = atoi(strCommand.c_str());
	CQuestNarrationTable* pQuestNarrationTable = API_GetTableContainer()->GetQuestNarrationTable();

	sQUEST_NARRATION_TBLDAT* pQUEST_NARRATION_TBLDAT = reinterpret_cast<sQUEST_NARRATION_TBLDAT*>( pQuestNarrationTable->FindData(uiMiniNarrationIndex) );
	if( !pQUEST_NARRATION_TBLDAT )
	{
		GetAlarmManager()->AlarmMessage(L"Not exist quest narration table");
		return FALSE;
	}

	if( !pQUEST_NARRATION_TBLDAT->bType )
	{
		GetAlarmManager()->AlarmMessage(L"Not exist quest narration table");
		return FALSE;
	}

	CNtlSLEventGenerator::BroadMsgNfy_MiniNarration(uiMiniNarrationIndex);

	return TRUE;
}

RwBool CDBOCheatCommand::OnAnimSyncActive(CNtlTokenizer* pLexer)
{
	RwInt32 iLine;
	std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);
	g_bActiveAnimSync = (RwBool)atoi(strCommand.c_str());

	return TRUE;
}

RwBool CDBOCheatCommand::OnTSReload(CNtlTokenizer* pLexer)
{
	CNtlSLEventGenerator::ReloadTS();

	return TRUE;
}

RwBool CDBOCheatCommand::OnDeveloperPreview(CNtlTokenizer* pLexer)
{
	RwInt32 iLine;
	std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);
	if(strCommand == "on")
	{
		CDboEventGenerator::Debug_for_Developer(DEVELOPER_PREVIEW_OPEN);
	}
	else if(strCommand == "off")
	{
		CDboEventGenerator::Debug_for_Developer(DEVELOPER_PREVIEW_CLOSE);
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

RwBool CDBOCheatCommand::OnDojo(CNtlTokenizer* pLexer)
{
	RwInt32 iLine;
	std::string strCommand1 = pLexer->PeekNextToken(NULL, &iLine);
	std::string strCommand2 = pLexer->PeekNextToken(&iLine, &iLine);
	std::string strCommand3 = pLexer->PeekNextToken(&iLine, &iLine);

	RwInt32 iDojoID = atoi(strCommand1.c_str());
	RwInt32 iDojoLv = atoi(strCommand2.c_str());
	RwInt32 iRefresh= atoi(strCommand3.c_str());

	if (iDojoID >= 0 && iDojoLv >= 0)
	{
		sEmblemFactor EmblemFactor;
		EmblemFactor.byTypeA = 2;
		EmblemFactor.byTypeB = 1;
		EmblemFactor.byTypeC = 2;
		EmblemFactor.byTypeAColor = 1;
		EmblemFactor.byTypeBColor = 2;
		EmblemFactor.byTypeCColor = 2;
		GetSceneManager()->GetDojoContainer()->SetDojo(iDojoID, iDojoLv, &EmblemFactor);
		if (iRefresh > 0)
		{
			GetSceneManager()->GetDojoContainer()->RefreshDojo();
		}
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

RwBool CDBOCheatCommand::OnRunAnimSpeedChange( CNtlTokenizer* pLexer ) 
{
    RwInt32 iLine;
    std::string strCommand1 = pLexer->PeekNextToken(NULL, &iLine);
    RwReal fRunAnimSpeed = (RwReal)atof(strCommand1.c_str());

    CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
    CNtlSobMoveableAttr *pMoveableAttr = reinterpret_cast<CNtlSobMoveableAttr*>(pAvatar->GetSobAttr());
    pMoveableAttr->SetRunAnimSpeed(fRunAnimSpeed);

    return TRUE;
}

RwBool CDBOCheatCommand::OnCreatePC(CNtlTokenizer* pLexer)
{
	SERIAL_HANDLE hSerialId = CNtlSobFactory::AcquireSerailId();

	CNtlSobAvatar* pAvatar			= (CNtlSobAvatar*) GetNtlSLGlobal()->GetSobAvatar();
	CNtlSobAvatarAttr* pAvatarAttr	= (CNtlSobAvatarAttr*) GetNtlSLGlobal()->GetSobAvatar()->GetSobAttr();

	RwV3d vPos = pAvatar->GetPosition();
	RwV3d vDir = pAvatar->GetDirection();

	//////////////////////////////////////////////////////////////////////////
	// Brief 정보 채워 넣기
	//////////////////////////////////////////////////////////////////////////

	sPC_BRIEF sPcBrief;

	sPcBrief.charId			= hSerialId;
	sPcBrief.tblidx			= pAvatarAttr->GetPcTbl()->tblidx;;
	sPcBrief.bIsAdult		= B2b(pAvatarAttr->IsAdult());

	wcscpy_s( sPcBrief.awchName, NTL_MAX_SIZE_CHAR_NAME, L"이름1" );
	wcscpy_s( sPcBrief.wszGuildName, NTL_MAX_SIZE_CHAR_NAME, L"길드1" );

	sPcBrief.sPcShape.byFace		= 1;
	sPcBrief.sPcShape.byHair		= 1;
	sPcBrief.sPcShape.byHairColor	= 1;
	sPcBrief.sPcShape.bySkinColor	= 1;

	sPcBrief.curLp					= 100;
	sPcBrief.maxLp					= 100;
	sPcBrief.wCurEP					= 100;
	sPcBrief.wMaxEP					= 100;
	sPcBrief.byLevel				= 100;

	sPcBrief.fLastRunSpeed			= 10.f;

	for ( int i = 0; i < EQUIP_SLOT_TYPE_COUNT; ++i )
	{
		sPcBrief.sItemBrief[i].tblidx = INVALID_TBLIDX;
	}

	sPcBrief.wAttackSpeedRate = 1;

	memset( &sPcBrief.sMark, 0xffffffff, sizeof( sPcBrief.sMark ) );

	//sPcBrief.sMarking.byCode = INVALID_MARKING_TYPE;

	memset( &sPcBrief.sDogi, 0xffffffff, sizeof( sPcBrief.sDogi ) );

	//////////////////////////////////////////////////////////////////////////
	// State 정보 채워 넣기
	//////////////////////////////////////////////////////////////////////////

	sCHARSTATE sCharState;

	sCharState.sCharStateBase.bFightMode		= FALSE;
	sCharState.sCharStateBase.byStateID			= CHARSTATE_SPAWNING;
	sCharState.sCharStateBase.dwConditionFlag	= 0;
	sCharState.sCharStateBase.aspectState.sAspectStateBase.byAspectStateId	= 0;
	sCharState.sCharStateBase.dwStateTime		= 0;
	sCharState.sCharStateBase.vCurLoc.x			= vPos.x;
	sCharState.sCharStateBase.vCurLoc.y			= vPos.y;
	sCharState.sCharStateBase.vCurLoc.z			= vPos.z;
	sCharState.sCharStateBase.vCurDir.x			= vDir.x;
	sCharState.sCharStateBase.vCurDir.y			= vDir.y;
	sCharState.sCharStateBase.vCurDir.z			= vDir.z;

	return CNtlSLEventGenerator::SobPlayerCreate( SLCLASS_PLAYER, hSerialId, vPos, vDir, &sPcBrief, &sCharState, TRUE ) ? TRUE : FALSE;
}

RwBool CDBOCheatCommand::LoadPalette( CNtlTokenizer* pLexer ) 
{
	if( !Logic_IsDevUser() )
	{
		return FALSE;
	}

	WCHAR awcBuffer[256];
	eLOAD_RESULT eResult = GetNtlPLPalette()->LoadPalette_forDeveloper();

	switch(eResult)
	{
	case LOAD_RESULT_OK:
		{
			GetAlarmManager()->AlarmMessage(L"Success load palette");
			break;
		}
	case LOAD_RESULT_FAIL_LOAD_FILE:
		{
			GetAlarmManager()->AlarmMessage(L"Error! Not exist Palette.xml file");
			break;
		}
	case LOAD_RESULT_NOT_EXIST_PALETTE:
		{			
			swprintf_s(awcBuffer, 256, L"Error! Not exist element %s", dPALETTE_EMBLEM_ELEMENT_NAME);
			GetAlarmManager()->AlarmMessage(awcBuffer);
			break;
		}
	case LOAD_RESULT_LESS_ELEMENT_COUNT:
		{
			swprintf_s(awcBuffer, 256, L"Error! Less element count than %d", dEMBLEM_PALETTE_COUNT);
			GetAlarmManager()->AlarmMessage(awcBuffer);
			break;
		}
	case LOAD_RESULT_MORE_ELEMENT_COUNT:
		{
			swprintf_s(awcBuffer, 256, L"Error! More element count than %d", dEMBLEM_PALETTE_COUNT);
			GetAlarmManager()->AlarmMessage(awcBuffer);
			break;
		}
	default:
		{
			GetAlarmManager()->AlarmMessage(L"Error! I have no idea");
			return FALSE;
		}
	}

	return TRUE;
}

RwBool CDBOCheatCommand::ReloadTable( CNtlTokenizer* pLexer ) 
{
	RwInt32 iLine;
	std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);

	if(strCommand == "worldmap")
	{
		//CTableFileNameList FileNameList;
		//
		//FileNameList.SetFileName(CTableContainer::TABLE_WORLD_MAP, "Table_Worldmap_Data");

		//API_GetTableContainer()->Reload(CTableContainer::TABLE_FLAG_WORLD_MAP, &FileNameList);
	}

	return TRUE;
}

// RwBool CDBOCheatCommand::OnTestCull(CNtlTokenizer* pLexer)
// {
// 	RwInt32 iLine;
// 	std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);
// 
// 	g_fTestVal = (RwReal)atof(strCommand.c_str());
// 
//  	CNtlPLCullingScheduling *pCullScheduling = GetNtlPLCullingScheduler();
//  	if(pCullScheduling)
//  	{
//  		pCullScheduling->SetMaxCullAtomicNum(PLENTITY_OBJECT, (RwInt32)g_fTestVal);
//  	}
// 
// 	return TRUE;
// }

RwBool CDBOCheatCommand::OnCrash( CNtlTokenizer* pLexer ) 
{
	int* p = NULL;
	*p = 0;

	return TRUE;
}

RwBool CDBOCheatCommand::OnPerf( CNtlTokenizer* pLexer ) 
{
	RwInt32 iLine;

	std::string strCommand1 = pLexer->PeekNextToken(NULL, &iLine);
	std::string strCommand2 = pLexer->PeekNextToken(&iLine, &iLine);
	std::string strCommand3 = pLexer->PeekNextToken(&iLine, &iLine);
	std::string strCommand4 = pLexer->PeekNextToken(&iLine, &iLine);
	std::string strCommand5 = pLexer->PeekNextToken(&iLine, &iLine);

	CNtlSobCharPerfController::s_fStandFrame			= !strCommand1.empty() ? (RwReal)atof( strCommand1.c_str() ) : 5.f;
	CNtlSobCharPerfController::s_fRemainIntervalFrame	= !strCommand2.empty() ? (RwReal)atof( strCommand2.c_str() ) : 0.5f;
	CNtlSobCharPerfController::s_fIncRenderCharPerSec	= !strCommand3.empty() ? (RwReal)atof( strCommand3.c_str() ) : 10.f;
	CNtlSobCharPerfController::s_fDecRenderCharPerSec	= !strCommand4.empty() ? (RwReal)atof( strCommand4.c_str() ) : 10.f;
	CNtlSobCharPerfController::s_fMinRenderChar			= !strCommand5.empty() ? (RwReal)atof( strCommand5.c_str() ) : 20.f;

	return TRUE;
}

RwBool CDBOCheatCommand::OnDialog( CNtlTokenizer* pLexer )			/// woosungs_test
{
	RwInt32 iLine;

	std::string strCommand1 = pLexer->PeekNextToken(NULL, &iLine);
	std::string strCommand2 = pLexer->PeekNextToken(&iLine, &iLine);
	std::string strCommand3 = pLexer->PeekNextToken(&iLine, &iLine);

	if(!strCommand1.empty())
	{
		if(strCommand1 == "mouseover" && !strCommand2.empty())		/// mouse overed dialog
		{
			if(strCommand2 == "dump")								/// dump  overed dialog info
			{
				gui::CComponent* pComp = gui::CSetupGUI::m_pGuiManager->GetpGUIManager_Generic()->GetMouseOver();
				if(pComp)
				{
					std::string strTemp = "Frm File Path: " +  pComp->GetComponent_Generic()->m_strPage + "\n"
						+ "Dialog Name: " + pComp->GetComponent_Generic()->m_strID + "\n";

					GetDumpCmdManager()->OutputDump(strTemp.c_str());
					return TRUE;
				}
			}
			else
				return FALSE;
		}

		int iDialogType = 0;			/// all dialog
		if(strCommand1 == "side")		/// side dialog manager
		{
			strCommand1 = strCommand2;
			strCommand2 = strCommand3;
			iDialogType = 1;			/// side dialog
		}

		if(!strCommand2.empty())	
		{
			bool bOpen = true;
			if(strCommand2 == "open")
				bOpen = true;
			else if(strCommand2 == "close")
				bOpen = false;
			else
				return FALSE;

			if(strCommand1 == "all")
			{
				if(1 == iDialogType)
					GetSideDialogManager()->ShowAllDialogTEST(bOpen);				/// side
				else
					GetDialogManager()->ShowAllDialogTEST(bOpen);					/// side 포함 all
			}
			else if(strCommand1 == "inc")
			{
				if(bOpen)
				{
					int iLimit = DIALOG_UNKNOWN;
					while(iLimit--)
					{
						++m_iCurrOpenedDialogIndex;
						if(GetDialogManager()->ShowDialogTEST((eDialogType) m_iCurrOpenedDialogIndex, bOpen))
							return TRUE;

						if(DIALOG_UNKNOWN - 1 < m_iCurrOpenedDialogIndex)
							m_iCurrOpenedDialogIndex = 0;
					}
				}
				else
					return FALSE;
			}
			else if(strCommand1 == "dec")
			{
				if(bOpen)
				{
					int iLimit = DIALOG_UNKNOWN;
					while(iLimit--)
					{
						--m_iCurrOpenedDialogIndex;
						if(GetDialogManager()->ShowDialogTEST((eDialogType) m_iCurrOpenedDialogIndex, bOpen))
							return TRUE;

						if(DIALOG_UNKNOWN - 1 < m_iCurrOpenedDialogIndex)
							m_iCurrOpenedDialogIndex = 0;
					}
				}
				else
					return FALSE;
			}
			else if(strCommand1.c_str()[0] <= '9' && strCommand1.c_str()[0] >= '0')	/// dialog번호로 해당 dialog open(close)
			{
				m_iCurrOpenedDialogIndex = atoi( strCommand1.c_str());
				if(DIALOG_UNKNOWN - 1 < m_iCurrOpenedDialogIndex)
					m_iCurrOpenedDialogIndex = 0;

				GetDialogManager()->ShowDialogTEST((eDialogType) m_iCurrOpenedDialogIndex, 
					bOpen);
			}
			else																	/// dialog frm file명으로 open(close)
			{
				CNtlPLGui* pDialog = GetDialogManager()->GetpDialogTEST(strCommand1.c_str());
				if(pDialog)
					pDialog->Show(bOpen);
			}


			return TRUE;
		}
		else
			return FALSE;
	}
	else
	{/*
		CDboEventGenerator::OpenTestGui();	/// open test dialog
		CNtlSLEventGenerator::PopoNotify(GetDisplayStringManager()->GetString("DST_SKILL_NOTYFY_UPGRADE_TITLE"),
			GetDisplayStringManager()->GetString("DST_SKILL_NOTYFY_UPGRADE_ALARM"), 
			TRUE, 30.0f);*/
		return FALSE;
	}

	return TRUE;
}

RwBool CDBOCheatCommand::OnPacketTrace(CNtlTokenizer* pLexer)	/// woosungs_test
{
	RwInt32 iLine;

	std::string strCommand1 = pLexer->PeekNextToken(NULL, &iLine);
	std::string strCommand2 = pLexer->PeekNextToken(&iLine, &iLine);
	std::string strCommand3 = pLexer->PeekNextToken(&iLine, &iLine);

	if(!strCommand1.empty())
	{
		if(strCommand1 == "dump")	
		{
			 if(!strCommand2.empty())
			 {
				 if(strCommand2 == "on")		
				 {	/// packet 기록 시작
					 if(CPacketTracer::GetInstance())
						CPacketTracer::GetInstance()->Activate();
				 }
				 else if(strCommand2 == "off")		
				 {  /// packet 기록 종료
					 if(CPacketTracer::GetInstance())
						 CPacketTracer::GetInstance()->Deactivate();			
				 }
				 else
					 return FALSE;
			 }
			 else
				 return FALSE;
		}
		else if(strCommand1 == "range")
		{
			if(!strCommand2.empty() && CPacketTracer::GetInstance())
			{	/// packet id  min
				/// atoi( strCommand2.c_str())
				if(!strCommand3.empty())
					CPacketTracer::GetInstance()->SetRangeOpCode(atoi( strCommand2.c_str()), atoi( strCommand3.c_str()));	
				else
					CPacketTracer::GetInstance()->SetRangeOpCode(atoi( strCommand2.c_str()));	
			}
			else
				return FALSE;
		}
		else if(strCommand1 == "mode")
		{
			if(!strCommand2.empty() && CPacketTracer::GetInstance())
			{
				if(strCommand2 == "A" || strCommand2 == "a")
					CPacketTracer::GetInstance()->SetTraceMode(CPacketTracer::CAPTURE_ALL);
				if(strCommand2 == "R" || strCommand2 == "r")
					CPacketTracer::GetInstance()->SetTraceMode(CPacketTracer::CAPTURE_RECEIVE);
				if(strCommand2 == "S" || strCommand2 == "s")
					CPacketTracer::GetInstance()->SetTraceMode(CPacketTracer::CAPTURE_SEND);
			}
			else
				return FALSE;
		}
		else
			return FALSE;
	}
	else
		return FALSE;

	return TRUE;
}

RwBool CDBOCheatCommand::OnPlayManySound(CNtlTokenizer* pLexer)
{
	if( false == m_listPlayList.empty() )
	{
		GetAlarmManager()->AlarmMessage(L"It's already working");
		return FALSE;
	}

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	RwV3d v3Pos = pAvatar->GetPosition();
	char acBuffer[][50] = { "Effect\\NFI_SKL_TVE_001.wav", 
							"Effect\\NFI_SKL_TVE_003.wav",
							"Effect\\NFI_SKL_TVE_004.wav",
							"Effect\\NMY_SKL_TVE_004_U.wav",
							"Effect\\NMY_SKL_TVE_021.wav",
							"Effect\\NMY_SKL_TVE_021_02.wav",
							"Effect\\NMY_SKL_TVE_021_03.wav",
							"Effect\\NMY_SKL_TVE_001.wav",
							"Effect\\NMY_SKL_TVE_018.wav",
							"Effect\\NMY_SKL_TVE_018_S.wav",
							"Effect\\NMY_SKL_TVE_020_02.wav",
							"Effect\\NMY_SKL_TVE_020_03.wav",
							"Effect\\NPC_PGM_TVE_002.wav",
							"Effect\\NPS_SKL_TVE_021.wav",
							"Effect\\NPS_SKL_TVE_021_02.wav",
							"Effect\\NPS_SKL_TVE_023.wav",
							"Effect\\NPS_SKL_TVE_023_S.wav",
							"Effect\\NPS_SKL_TVE_023_U.wav",
							"Effect\\NPS_SKL_TVE_024.wav",
							"Effect\\NSK_SKL_TVE_022.wav",
							"Effect\\NSK_SKL_TVE_026.wav",
							"Effect\\HMY_SKL_TVE_003.wav",
							"Effect\\HMY_SKL_TVE_004_02.wav",
							"Effect\\HMY_SKL_TVE_004_02_U.wav",
							"Effect\\HMY_SKL_TVE_004_03.wav",};

	RwInt32 iPlayCount = 0;

	for( RwUInt32 i = 0 ; i < 25 ; ++i )
	{
		sNtlSoundPlayParameta tSoundParam;
		tSoundParam.iChannelGroup	= CHANNEL_GROUP_EFFECT_SOUND;
		tSoundParam.pcFileName		= acBuffer[i];
		tSoundParam.bLoop			= true;
		tSoundParam.fXPos			= v3Pos.x;
		tSoundParam.fYPos			= v3Pos.y;
		tSoundParam.fZPos			= v3Pos.z;

		if( SOUNDRESULT_OK == GetSoundManager()->Play(&tSoundParam) )
			++iPlayCount;

		m_listPlayList.push_back(tSoundParam.hHandle);
	}

	WCHAR awcBuf[64] = L"";
	swprintf_s(awcBuf, 64, L"Play Count : %d", iPlayCount);
	GetAlarmManager()->AlarmMessage(awcBuf);
	swprintf_s(awcBuf, 64, L"Position  x : %f, y : %f, z : %f", v3Pos.x, v3Pos.y, v3Pos.z);
	GetAlarmManager()->AlarmMessage(awcBuf);

	return TRUE;
}

RwBool CDBOCheatCommand::OnStopManySound(CNtlTokenizer* pLexer)
{
	if( m_listPlayList.empty() )
		return FALSE;

	for each( SOUND_HANDLE hHandle in m_listPlayList )
		GetSoundManager()->Stop(hHandle);

	return TRUE;
}

RwBool CDBOCheatCommand::OnMinMaxRate(CNtlTokenizer* pLexer)
{
	RwInt32 iLine;
	std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);
	RwReal fMinMaxRate = (RwReal)atof( strCommand.c_str() );

	CNtlSoundEventGenerator::SendMinMaxRate(fMinMaxRate);

	return TRUE;
}

RwBool CDBOCheatCommand::OnPlaySound(CNtlTokenizer* pLexer)
{
	enum eParametaType
	{
		PT_PLAY_TYPE,			// 1 : 2D sound, 2 : 3D sound
		PT_FILE_NAME,			// DBO_Resource\sound 폴더에서만 파일을 찾는다
		PT_POS_X,
		PT_POS_Y,
		PT_POS_Z,
		PT_LOOP,
		PT_VOLUME,
		PT_MIN,
		PT_MAX,

		NUM_PT
	};

	RwInt32			iLine;
	RwInt32			iParametaType = PT_PLAY_TYPE;
	RwUInt8			byPlayType = 0xff;
	std::string		strFileName;

	// SoundManager에서의 디폴트 값과 일치시켜서 차후 인자로 받지 않은 것이
	// 있더라도 따로 받은 인자가 무엇무엇인지 검사하지 않고 전부 인자로 넘길 수
	// 있도록 한다
	RwV3d			v3Pos;
	bool			bLoop = false;
	RwReal			fVolume = 1.f;
	RwReal			fMin = 10.f;
	RwReal			fMax = 50.f;

	v3Pos.x = 0.f;
	v3Pos.y = 0.f;
	v3Pos.z = 0.f;


	std::string strCommand;
	for(;;)
	{
		strCommand = pLexer->PeekNextToken(NULL, &iLine);
		if( strCommand.empty() )
			break;


		if( iParametaType == PT_PLAY_TYPE )
		{
			if( strCommand.empty() )
				return FALSE;

			byPlayType = (RwUInt8)atoi( strCommand.c_str() );
			if( byPlayType == 0 )
				return FALSE;
		}
		else if( iParametaType == PT_FILE_NAME )
		{
			strFileName = strCommand;

			if( byPlayType == 1 )
			{
				// 2D Sound
				if( !strCommand.empty() )
				{
					sNtlSoundPlayParameta tSoundParam;
					tSoundParam.iChannelGroup	= CHANNEL_GROUP_BGM;
					tSoundParam.pcFileName		= (char*)strCommand.c_str();
					tSoundParam.bLoop			= true;
					tSoundParam.fXPos			= v3Pos.x;
					tSoundParam.fYPos			= v3Pos.y;
					tSoundParam.fZPos			= v3Pos.z;

					GetSoundManager()->Play(&tSoundParam);

					m_hSound = tSoundParam.hHandle;
					return TRUE;
				}

				return FALSE;
			}
		}
		else if( iParametaType == PT_POS_X )
		{
			if( strCommand == "-" )
			{
				strCommand = pLexer->PeekNextToken(NULL, &iLine);
				v3Pos.x = (float)atof( strCommand.c_str() ) * -1.f;
			}
			else
				v3Pos.x = (float)atof( strCommand.c_str() );

			if( v3Pos.x == 0 )
				return FALSE;
		}
		else if( iParametaType == PT_POS_Y )
		{
			if( strCommand == "-" )
			{
				strCommand = pLexer->PeekNextToken(NULL, &iLine);
				v3Pos.y = (float)atof( strCommand.c_str() ) * -1.f;
			}
			else
				v3Pos.y = (float)atof( strCommand.c_str() );

			if( v3Pos.y == 0 )
				return FALSE;
		}
		else if( iParametaType == PT_POS_Z )
		{
			if( strCommand == "-" )
			{
				strCommand = pLexer->PeekNextToken(NULL, &iLine);
				v3Pos.z = (float)atof( strCommand.c_str() ) * -1.f;
			}
			else
				v3Pos.z = (float)atof( strCommand.c_str() );

			if( v3Pos.z == 0 )
				return FALSE;
		}
		else if( iParametaType == PT_LOOP )
		{
			if( strCommand.compare("1") )
				bLoop = true;
		}
		else if( iParametaType == PT_VOLUME )
		{
			fVolume = (float)atof( strCommand.c_str() );

			if( fVolume == 0 )
				return FALSE;
		}
		else if( iParametaType == PT_MIN )
		{
			if( strCommand == "-" )
			{
				strCommand = pLexer->PeekNextToken(NULL, &iLine);
				fMin = (float)atof( strCommand.c_str() ) * -1.f;
			}
			else
				fMin = (float)atof( strCommand.c_str() );

			if( fMin == 0 )
				return FALSE;
		}
		else if( iParametaType == PT_MAX )
		{
			if( strCommand == "-" )
			{
				strCommand = pLexer->PeekNextToken(NULL, &iLine);
				fMax = (float)atof( strCommand.c_str() ) * -1.f;
			}
			else
				fMax = (float)atof( strCommand.c_str() );

			if( fMax == 0 )
				return FALSE;
		}

		++iParametaType;

		if( iParametaType == NUM_PT )
		{
			--iParametaType;
			break;
		}
	}

	if( byPlayType == 2 )
	{
		// 3D Sound
		sNtlSoundPlayParameta tSoundParam;
		tSoundParam.iChannelGroup	= CHANNEL_GROUP_EFFECT_SOUND;
		tSoundParam.pcFileName		= (char*)strFileName.c_str();
		tSoundParam.bLoop			= bLoop;
		tSoundParam.fVolume			= fVolume;
		tSoundParam.fXPos			= v3Pos.x;
		tSoundParam.fYPos			= v3Pos.y;
		tSoundParam.fZPos			= v3Pos.z;
		tSoundParam.fMinDistance	= fMin;
		tSoundParam.fMaxDistance	= fMax;

		GetSoundManager()->Play(&tSoundParam);

		m_hSound = tSoundParam.hHandle;

		return TRUE;
	}

	return FALSE;
}

RwBool CDBOCheatCommand::OnStopSound(CNtlTokenizer* pLexer)
{
	if( m_hSound != INVALID_SOUND_HANDLE )
		GetSoundManager()->Stop(m_hSound);

	return TRUE;
}

RwBool CDBOCheatCommand::OnShareBGMRate(CNtlTokenizer* pLexer)
{
	RwInt32 iLine;
	std::string strCommand = pLexer->PeekNextToken(NULL, &iLine);
	RwReal fPlayRate = (RwReal)atof( strCommand.c_str() );

	CNtlSoundEventGenerator::SendShareBGM_PlayRate(fPlayRate);

	return TRUE;
}

RwBool CDBOCheatCommand::OnDestMove(CNtlTokenizer* pLexer)
{
	RwV3d* pPathList = NULL;

	FILE* pFile = NULL;
	fopen_s(&pFile, "destmove.txt", "rt" );

	if ( pFile )
	{
		int nCnt;

		fscanf( pFile, "%d ", &nCnt );

		pPathList = new RwV3d[nCnt];

		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

		RwV3d vCurPos = pAvatar->GetPosition();

		for ( int i = 0; i < nCnt; ++i )
		{
			RwReal fX, fY;

			fscanf_s( pFile, "%f ", &pPathList[i].x );
			fscanf_s( pFile, "%f ", &pPathList[i].z );

			pPathList[i].y = vCurPos.y + 1.f;
		}

		fclose( pFile );

		if ( pPathList )
		{
			CNtlSLEventGenerator::SobDestMove( pAvatar->GetSerialID(), NTL_MOVE_FLAG_RUN, vCurPos, pPathList[0], 0, nCnt, pPathList );

			delete [] pPathList;
		}
	}

	return TRUE;
}

#include "precomp_dboclient.h"
#include "MrPoPoHandler.h"
#include "DboGlobal.h"
#include "TableContainer.h"
#include "ExpTable.h"
#include "NtlSLTBCrowdManager.h"
#include "DumpCommand.h"
#include "NtlPLCharacter.h"
#include "NtlCameraController.h"
#include "NtlInstanceEffect.h"
#include "SkillTable.h"
#include "PCTable.h"

// Presentation
#include "NtlPLGlobal.h"
#include "NtlPLCharacter.h"
#include "NtlPLOptionManager.h"


#include "NtlSobProxy.h"
#include "NtlSLEventFunc.h"
#include "NtlPLEventGenerator.h"
#include "NtlFSMDef.h"

CMrPoPoHandler::CMrPoPoHandler(void)
{
}

CMrPoPoHandler::~CMrPoPoHandler(void)
{

}

void CMrPoPoHandler::HandleEvent( RwInt32 nMsg, DWORD param ) 
{
    WCHAR wBuf[16] = {0,};
    RwBool bMsgSend = TRUE;

    std::wstring strMsg;
    switch(nMsg)
    {
    case MSG_MRPOPO_CREATE_ITEM:
        swprintf_s(wBuf, L"%d", param);
        strMsg = L"@createitem ";        
        strMsg += wBuf;        
        break;
    case MSG_MRPOPO_UPGRADE_ITEM:
        strMsg = L"@equipupgrade";
        break;
    case MSG_MRPOPO_SET_ZENNY:
        swprintf_s(wBuf, L"%d", param);
        strMsg = L"@setmoney ";
        strMsg += wBuf;
        break;
    case MSG_MRPOPO_ADD_EXP:
        swprintf_s(wBuf, L"%d", param);
        strMsg = L"@addexp ";
        strMsg += wBuf;
        break;    
    case MSG_MRPOPO_COLLECT_DRAGONBALL:
        OnCollectDragonBall();
        bMsgSend = FALSE;
        break;
    case MSG_MRPOPO_TEST_CROWD:
        OnTestCrowd((RwInt32)param);
        bMsgSend = FALSE;
        break;
    case MSG_MRPOPO_TELEPORT:
        strMsg = L"@teleport ";
        strMsg += OnTeleport((RwInt32)param);
        break;
    case MSG_MRPOPO_TELE_DIRECT_X:
        OnTeleport(MSG_MRPOPO_TELE_DIRECT_X, (RwInt32)param);
        bMsgSend = FALSE;
        break;
    case MSG_MRPOPO_TELE_DIRECT_Z:
        strMsg = L"@teleport ";
        strMsg += OnTeleport(MSG_MRPOPO_TELE_DIRECT_Z, (RwInt32)param);
        break;
    case MSG_MRPOPO_OBSERVER_STATIC:        
        CNtlSLEventGenerator::CameraObserver(E_OBSERVER_TYPE_STATIC, (RwInt32)param);
        bMsgSend = FALSE;
        break;
    case MSG_MRPOPO_OBSERVER_LUA:
        CNtlSLEventGenerator::CameraObserver(E_OBSERVER_TYPE_NODE, (RwInt32)param);
        bMsgSend = FALSE;
        break;
    case MSG_MRPOPO_TEST1:
        g_fTestVal = (RwReal)param;
        bMsgSend = FALSE;
        break;
    case MSG_MRPOPO_TEST2:
        g_fTestVal2 = (RwReal)param;
        bMsgSend = FALSE;
        break;
    case MSG_MRPOPO_TEST3:
        g_fTestVal3 = (RwReal)param;
        bMsgSend = FALSE;
        break;
    case MSG_MRPOPO_ELAPSEDTIME_WEIGHT:
        g_fElapsedWeightValue = (RwReal)param;
        bMsgSend = FALSE;
        break;
    case MSG_MRPOPO_DUMP_TO_CONSOLE:
        GetDumpCmdManager()->ActiveDumpTarget(CDumpCmdManager::DUMP_TARGET_CONSOLE, param);
        bMsgSend = FALSE;
        break;
        break;
    case MSG_MRPOPO_DUMP_TO_GUI:
        GetDumpCmdManager()->ActiveDumpTarget(CDumpCmdManager::DUMP_TARGET_GUI, param);
        bMsgSend = FALSE;
        break;
    case MSG_MRPOPO_DUMP_TO_FILE:
        GetDumpCmdManager()->ActiveDumpTarget(CDumpCmdManager::DUMP_TARGET_FILE, param);
        bMsgSend = FALSE;
        break;
    case MSG_MRPOPO_DUMP:
        if(param == MPP_DUMP_SELF)
        {
            GetDumpCmdManager()->OutputSobObj(GetNtlSLGlobal()->GetSobAvatar()->GetSerialID());
        }
        else if(param == MPP_DUMP_TARGET)
        {
            GetDumpCmdManager()->OutputSobObj(GetNtlSLGlobal()->GetSobAvatar()->GetTargetID());
        }
        bMsgSend = FALSE;
        break;
    case MSG_MRPOPO_DUMP_REG:        
        GetDumpCmdManager()->SetRegisterSerial(GetNtlSLGlobal()->GetSobAvatar()->GetTargetID());
        bMsgSend = FALSE;
        break;
    case MSG_MRPOPO_DUMP_UNREG:
        GetDumpCmdManager()->SetRegisterSerial(INVALID_SERIAL_ID);
        bMsgSend = FALSE;
        break;
    case MSG_MRPOPO_LOWSPEC_CHAR:
        CNtlPLCharacter::SetSkipSimpleMaterial(param);
        bMsgSend = FALSE;
        break;
    case MSG_MRPOPO_LOWSPEC_EFFECT:
        CNtlInstanceEffect::SetLowSpecEnable(param);
        bMsgSend = FALSE;
        break;
    case MSG_MRPOPO_FREECAMERA:
        if(param)
        {
            CNtlSLEventGenerator::CameraFree();
        }
        else
        {
            CNtlSLEventGenerator::CameraControlDelete(CAMERA_CONTROL_FREE);
        }
        bMsgSend = FALSE;
        break;
    case MSG_MRPOPO_SCOUTER_RENDER:
        CNtlPLGlobal::m_bRenderScouter = (RwBool)param;
        bMsgSend = FALSE;
        break;
    case MSG_MRPOPO_RENDER_MESHSYSTEM:
        CNtlInstanceEffect::SetRenderMeshSystem(param);
        bMsgSend = FALSE;
        break;
    case MSG_MRPOPO_RENDER_DECALSYSTEM:
        CNtlInstanceEffect::SetRenderDecalSystem(param);
        bMsgSend = FALSE;
        break;
    case MSG_MRPOPO_LEARN_ALL_SKILL:
        strMsg = L"@learnskillall";        
        break;
    case MSG_MRPOPO_UPGRADE_ALL_SKILL:
        OnUpgradeAllSkill();
        bMsgSend = FALSE;
        break;
    case MSG_MRPOPO_CREATE_LEVEL_ITEMS:
        strMsg = L"@createitemall";
        break;    
    case MSG_MRPOPO_SET_LEVEL:
        swprintf_s(wBuf, L"%d", (RwInt32)param);
        strMsg = L"@setlevel ";        
        strMsg += wBuf;
        break;
    case MSG_MRPOPO_SETSPEED:
        swprintf_s(wBuf, L"%d", (RwInt32)param);
        strMsg = L"@setspeed ";
        strMsg += wBuf;
        break;
    case MSG_MRPOPO_SETOFFENCE:
        swprintf_s(wBuf, L"%d", (RwInt32)param);
        strMsg = L"@setoffence ";
        strMsg += wBuf;
        break;
    case MSG_MRPOPO_SETDEFNECE:
        swprintf_s(wBuf, L"%d", (RwInt32)param);
        strMsg = L"@setdefence ";
        strMsg += wBuf;
        break;    
    case MSG_MRPOPO_SUPER_MODE:					OnSuperMode();					  bMsgSend = FALSE; break;        
    case MSG_MRPOPO_TENKAICHI_MARK:				OnTenkaichiMark((RwInt32)param);  bMsgSend = FALSE;  break;        
    case MSG_MRPOPO_TRANSLATE_STATE:			OnChangeState((RwUInt32)param);   bMsgSend = FALSE;   break;        
    case MSG_MRPOPO_TRANSFORM:					OnTransform((RwUInt32)param);     bMsgSend = FALSE;   break;
    case MSG_MRPOPO_STUN:						OnStun((RwUInt32)param);          bMsgSend = FALSE;   break;
    case MSG_MRPOPO_TARGET_MARKING:				OnTargetMarking((RwBool)param);   bMsgSend = FALSE;   break;
    case MSG_MRPOPO_SPEC_TERRAIN_DIST:			OnTerrainDist((RwUInt32)param);   bMsgSend = FALSE;   break;
    case MSG_MRPOPO_SPEC_TERRAIN_SHADOW:		OnTerrainShadow((RwBool)param);   bMsgSend = FALSE;  break;
    case MSG_MRPOPO_SPEC_WATER_SPECULAR:		OnWaterSpecular((RwBool)param);   bMsgSend = FALSE;  break;
	case MSG_MRPOPO_SPEC_CHAR_DIST:				OnCharDist((RwUInt32)param);	  bMsgSend = FALSE;   break;
    case MSG_MRPOPO_SPEC_CHAR_EDGE:				OnCharEdge((RwBool)param);        bMsgSend = FALSE;   break;
	case MSG_MRPOPO_SPEC_CHAR_GRAYCOLOR:		OnCharGrayColor((RwBool)param);   bMsgSend = FALSE;   break;
    case MSG_MRPOPO_SPEC_EFFECT:				OnLowEffect((RwBool)param);       bMsgSend = FALSE;   break;
	case MSG_MRPOPO_SPEC_EFFECT_PARTICLE_RATIO: OnParticleRatio((RwUInt32)param); bMsgSend = FALSE;   break;
	case MSG_MRPOPO_SPEC_EFFECT_MESH:			OnMeshEffect((RwBool)param);      bMsgSend = FALSE;   break;
	case MSG_MRPOPO_SPEC_EFFECT_DECAL:			OnDecalEffect((RwBool)param);     bMsgSend = FALSE;   break;
    case MSG_MRPOPO_PUSHING:					OnPushTest();                     bMsgSend = FALSE;   break;
    }

    if(bMsgSend)
    {
        GetDboGlobal()->GetGamePacketGenerator()->SendServerCommand(strMsg.c_str());
    }    
}

DWORD CMrPoPoHandler::OnSetLevel(RwInt32 nLevel)
{
    RwInt32 nCurrLevel = Logic_GetLevel(GetNtlSLGlobal()->GetSobAvatar());
    if(nLevel <= nCurrLevel)
        return 0;

    CExpTable* pTable = API_GetTableContainer()->GetExpTable();
    sEXP_TBLDAT* pCurrData = (sEXP_TBLDAT*)pTable->FindData(nCurrLevel);
    sEXP_TBLDAT* pSetData = (sEXP_TBLDAT*)pTable->FindData(nLevel);

    return pSetData->dwExp - pCurrData->dwExp;
}

void CMrPoPoHandler::OnCollectDragonBall() 
{
    std::wstring strMsg;
    int nIndex = 19981;
    WCHAR buf[64];
    for(int i = 0; i < 7; ++i)
    {
        swprintf_s(buf, L"%d", nIndex + i);
        strMsg = L"@createitem ";
        strMsg += buf;

        GetDboGlobal()->GetGamePacketGenerator()->SendServerCommand(strMsg.c_str());
    }
}

void CMrPoPoHandler::OnTestCrowd( RwInt32 nCrowdType ) 
{
    GetNtlSLCrowdManager()->ChangeStep((ETBCrowdStepType)nCrowdType);
}

std::wstring CMrPoPoHandler::OnTeleport( RwInt32 nIndex ) 
{
    std::wstring wstrRet;

    switch(nIndex)
    {
    case MPP_TELE_YAHOI:
        wstrRet = L"4493 4032";
        break;
    case MPP_TELE_YUREKA:
        wstrRet = L"6586 3254";
        break;
    case MPP_TELE_DALPANG:
        wstrRet = L"3246 -2749";
        break;
    case MPP_TELE_DRAGON:
        wstrRet = L"4620 -1729";
        break;
    case MPP_TELE_BAEE:
        wstrRet = L"4509 4031";
        break;
    case MPP_TELE_AJIRANG:
        wstrRet = L"4509 4031";
        break;
    case MPP_TELE_KARINGA_1:
        wstrRet = L"5928 658";
        break;
    case MPP_TELE_KARINGA_2:
        wstrRet = L"5884 827";
        break;
    case MPP_TELE_GREAT_TREE:
        wstrRet = L"5903 1711";
        break;
    case MPP_TELE_KARINGA_3:
        wstrRet = L"7527 -518";
        break;
    case MPP_TELE_MERMAID:
        wstrRet = L"4651 -206";
        break;
    case MPP_TELE_GANNET:
        wstrRet = L"3690 1396";
        break;
    case MPP_TELE_EMERALD:
        wstrRet = L"3390 -564";
        break;
    case MPP_TELE_TEMBARIN:
        wstrRet = L"1964 1072";
        break;    
    case MPP_TELE_CELL:
        wstrRet = L"-480 1646";
        break;
    case MPP_TELE_BUU:
        wstrRet = L"2481 3384";
        break;
    case MPP_TELE_CC:
        wstrRet = L"-1796 1106";
        break;
    case MPP_TELE_MUSHROOM:
        wstrRet = L"-1408 -1408";
        break;
    }

    return wstrRet;
}

std::wstring CMrPoPoHandler::OnTeleport( RwInt32 nMsg, RwInt32 nVal ) 
{
    std::wstring strRet;

    if(nMsg == MSG_MRPOPO_TELE_DIRECT_X)
    {
        m_nTemp = nVal;
    }
    else if(nMsg == MSG_MRPOPO_TELE_DIRECT_Z)
    {
        WCHAR buf[1024] = {0,};
        swprintf_s(buf, L"%d %d", m_nTemp, nVal);
        strRet = buf;
    }

    return strRet;
}
//
//void CMrPoPoHandler::OnLearnAllSkill() 
//{
//    RwUInt8 nLevel = Logic_GetLevel(GetNtlSLGlobal()->GetSobAvatar());
//    CNtlSkillContainer* pSkillContainer = GetNtlSLGlobal()->GetSobAvatar()->GetSkillContainer();
//    CPCTable* pPCTable = API_GetTableContainer()->GetPcTable();
//    CSkillTable* pSkillTable = API_GetTableContainer()->GetSkillTable();
//    CHTBSetTable* pHTBTable = API_GetTableContainer()->GetHTBSetTable();    
//    SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
//    sPC_TBLDAT* pPCData = reinterpret_cast<sPC_TBLDAT*>( pPCTable->FindData( pAvatarInfo->sCharPf.tblidx ) );
//    WCHAR buf[64] = {0,};
//
//    CTable::TABLEIT it = pSkillTable->Begin();
//    for(;it != pSkillTable->End(); ++it)
//    {
//        sSKILL_TBLDAT* pData = (sSKILL_TBLDAT*)(*it).second;
//        if(nLevel < pData->byRequire_Train_Level)
//            continue;
//
//        if(!(pData->dwPC_Class_Bit_Flag & pPCData->dwClass_Bit_Flag))
//            continue;
//
//        if(pSkillContainer->GetSkillFromTableId(pData->tblidx))
//            continue;
//
//        
//        swprintf_s(buf, L"@learnskill %d", pData->tblidx);        
//
//        GetDboGlobal()->GetGamePacketGenerator()->SendServerCommand(buf);
//    }
//}

void CMrPoPoHandler::OnUpgradeAllSkill() 
{

}

void CMrPoPoHandler::OnSuperMode() 
{
    WCHAR wBuf[128] = {0,};
    swprintf_s(wBuf, L"@setoffence 99999999");
    GetDboGlobal()->GetGamePacketGenerator()->SendServerCommand(wBuf);

    swprintf_s(wBuf, L"@setdefence 99999999");
    GetDboGlobal()->GetGamePacketGenerator()->SendServerCommand(wBuf);

    swprintf_s(wBuf, L"@setspeed 20");
    GetDboGlobal()->GetGamePacketGenerator()->SendServerCommand(wBuf);
}

void CMrPoPoHandler::OnNamekTest() 
{
    CNtlSobActor* pActor = (CNtlSobActor*)GetNtlSLGlobal()->GetSobAvatar();
    if(pActor->GetTargetID() != INVALID_SERIAL_ID)
    {
        CNtlSobActor* pTarget = (CNtlSobActor*)GetNtlSobManager()->GetSobObject(pActor->GetTargetID());
        RwV3d vDir = pTarget->GetPosition() - pActor->GetPosition();
        RwV3dNormalize(&vDir, &vDir);
        pActor->SetDirection(&vDir);
    }

    GetNtlSLGlobal()->GetSobAvatar()->GetSobProxy()->SetBaseAnimation(2001);
}

void CMrPoPoHandler::OnTenkaichiMark(RwInt32 nType)
{
    CNtlSLEventGenerator::TenkaichiMarking(GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(), (BYTE)nType);
}

void CMrPoPoHandler::OnChangeColor( RwInt32 nType ) 
{
    RwUInt32 hSerialID = GetNtlSLGlobal()->GetSobAvatar()->GetSerialID();
    SEventColorChange event;
    event.eType = (EColorChangeType)nType;
    event.colorEdge.red = event.colorEdge.green = event.colorEdge.blue = 255;
    event.colorBody.red = 208; event.colorBody.green = 99; event.colorBody.blue = 208;
    event.colorAdd.red = 30; event.colorAdd.green = 30; event.colorAdd.blue = 30;
    CNtlPLEventGenerator::AnimEventColorChange(hSerialID, &event);    
}

void CMrPoPoHandler::OnChangeState( RwUInt32 uiStateID ) 
{
    RwUInt32 hSerialID = GetNtlSLGlobal()->GetSobAvatar()->GetSerialID();
    CNtlSLEventGenerator::SobStateTransition(hSerialID, uiStateID);
}

void CMrPoPoHandler::OnTransform( RwUInt32 uiTransformID ) 
{
    RwUInt32 hSerialID = GetNtlSLGlobal()->GetSobAvatar()->GetSerialID();
    sASPECTSTATE state;
    state.sAspectStateBase.byAspectStateId = (BYTE)uiTransformID;    
    CNtlSLEventGenerator::SobTransform(hSerialID, state);
}

void CMrPoPoHandler::OnStun( RwUInt32 uiStunType ) 
{
    CNtlSobAvatar* pActor = GetNtlSLGlobal()->GetSobAvatar();
    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData()); 
    pBeData->GetCtrlStuff()->uExtra.sStun.byStunType = (BYTE)uiStunType;

    CNtlSLEventGenerator::SobStateTransition(pActor->GetSerialID(), NTL_FSMSID_STUN);
}

void CMrPoPoHandler::OnTargetMarking( RwBool bSelect ) 
{
    RwUInt32 hSerialID = GetNtlSLGlobal()->GetSobAvatar()->GetSerialID();

    if(bSelect)
    {
        CNtlSLEventGenerator::SobTargetMarkSelect(hSerialID);
    }
    else
    {
        CNtlSLEventGenerator::SobTargetMarkRelease(hSerialID);
    }
}

void CMrPoPoHandler::OnPushTest() 
{
    CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();    
    RwV3d vDestPos = pSobAvatar->GetPosition() - pSobAvatar->GetDirection() * 0.3f;

    CNtlSLEventGenerator::SobPushingNfy(pSobAvatar->GetSerialID(), BATTLE_ATTACK_RESULT_HIT, 99, vDestPos);
}

void CMrPoPoHandler::OnTerrainDist( RwUInt32 uiDist ) 
{
	GetNtlPLOptionManager()->SetTerrainFar((RwReal)uiDist);
}


void CMrPoPoHandler::OnTerrainShadow( RwBool bEnable ) 
{
	GetNtlPLOptionManager()->SetTerrainShadow(bEnable);
}

void CMrPoPoHandler::OnWaterSpecular( RwBool bEnable ) 
{
	GetNtlPLOptionManager()->SetWaterSpecular(bEnable);
}

void CMrPoPoHandler::OnCharDist( RwUInt32 uiDist ) 
{
	CNtlSobManager::SetOptionOutRange((RwReal)uiDist);
}


void CMrPoPoHandler::OnCharEdge( RwBool bEnable ) 
{
	if(bEnable)
		CNtlPLCharacter::SetSkipEdge(FALSE);
	else
		CNtlPLCharacter::SetSkipEdge(TRUE);
}

void CMrPoPoHandler::OnCharGrayColor(RwBool bEnable)
{
	/*
	if(bEnable)
		CNtlPLCharacter::SetSkipSimpleMaterial(TRUE);
	else
		CNtlPLCharacter::SetSkipSimpleMaterial(FALSE);
		*/
}


void CMrPoPoHandler::OnLowEffect( RwBool bEnable ) 
{
    CNtlInstanceEffect::SetLowSpecEnable(bEnable);    
}

void CMrPoPoHandler::OnParticleRatio(RwUInt32 uiRatio)
{
	RwReal fRatio = (RwReal)uiRatio/10.0f;

    CNtlInstanceEffect::SetLowSpecRatio(fRatio);    
}

void CMrPoPoHandler::OnMeshEffect(RwBool bEnable)
{
	CNtlInstanceEffect::SetRenderMeshSystem(bEnable);
}

void CMrPoPoHandler::OnDecalEffect(RwBool bEnable)
{
	CNtlInstanceEffect::SetRenderDecalSystem(bEnable);
}


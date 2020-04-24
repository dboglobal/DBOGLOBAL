/*****************************************************************************
 *
 * File			: DBOCheatCommand
 * Author		: agebreak
 * Copyright	: (주)NTL
 * Date			: 2007. 9. 13	
 * Abstract		: DBO Cheat Command
 *****************************************************************************
 * Desc         : DBO에서 사용하는 클라이언트 사이드의 치트 커맨드를 파싱하여 처리한다.
 *
 *****************************************************************************/

#pragma once

#include "NtlTokenizer.h"
#include "NtlSoundManager.h"


class CDBOCheatCommand
{
public:
    typedef RwBool (CDBOCheatCommand::*CheatHandler)(CNtlTokenizer* pLexer);
    typedef std::map<std::string, CheatHandler> MapCheatHandler;

public:
    CDBOCheatCommand();
    ~CDBOCheatCommand();

    RwBool  OnCmdParsing(const std::string& strCmd);            ///< 치트키를 파싱한다

protected:
    void		Register();                 ///< 치트 문자열과 함수들을 등록한다.

	RwBool		OnElapsedTime(CNtlTokenizer* pLexer);
    RwBool		OnDump(CNtlTokenizer* pLexer);
    RwBool		OnDumpToConsole(CNtlTokenizer* pLexer);
    RwBool		OnDumpToGui(CNtlTokenizer* pLexer);
    RwBool		OnDumpToFile(CNtlTokenizer* pLexer);
	RwBool      OnDump_Action(CNtlTokenizer* pLexer);
	RwBool      OnDump_Sob(CNtlTokenizer* pLexer);
	RwBool      OnDump_FSM(CNtlTokenizer* pLexer);
	RwBool      OnDump_Contents(CNtlTokenizer* pLexer);

    RwBool		OnCompileLua(CNtlTokenizer* pLexer);
	RwBool		OnRunLua(CNtlTokenizer* pLexer);
    RwBool		OnCAngle(CNtlTokenizer* pLexer);
    RwBool		OnSAngle(CNtlTokenizer* pLexer);
	RwBool		OnSpeech(CNtlTokenizer* pLexer);
	RwBool		OnResetGui(CNtlTokenizer* pLexer);
	RwBool		OnKnockdownMatrixDelayTime(CNtlTokenizer* pLexer);
	RwBool		OnKnockdownMatrixTime(CNtlTokenizer* pLexer);
	RwBool		OnKnockdownMatrixValue(CNtlTokenizer* pLexer);
	RwBool		OnKnockdownMatrixCameraLifeTime(CNtlTokenizer* pLexer);
	RwBool		OnPacketLockMsg(CNtlTokenizer* pLexer);
	RwBool		OnServerMsg(CNtlTokenizer* pLexer);
	RwBool		OnShadow(CNtlTokenizer* pLexer);
	RwBool		OnCharLoadTime(CNtlTokenizer* pLexer);			///< 캐릭터 로딩 테스트용 치트키
    RwBool		OnTest(CNtlTokenizer* pLexer);                    ///< 테스트용 치트키
    RwBool		OnTest2(CNtlTokenizer* pLexer);                    ///< 테스트용 치트키
    RwBool		OnTest3(CNtlTokenizer* pLexer);                    ///< 테스트용 치트키
	RwBool		OnTestInfoScrollTime(CNtlTokenizer* pLexer);		///< 인포박스 스크롤 속도 팩터
	RwBool		OnTutorialScript(CNtlTokenizer* pLexer);	
	RwBool		OnFlashNotify(CNtlTokenizer* pLexer);
	RwBool		OnAvatarShowOnOff(CNtlTokenizer* pLexer);
	RwBool		OnServerChangeOut(CNtlTokenizer* pLexer);
	RwBool		OnLowSpecCharacter(CNtlTokenizer* pLexer);
	RwBool		OnSkipEdge(CNtlTokenizer* pLexer);
	RwBool		OnSkipTerrain(CNtlTokenizer* pLexer);
	RwBool		OnObjectDistance(CNtlTokenizer* pLexer);
	RwBool		OnTerrainDistance(CNtlTokenizer* pLexer);
    RwBool      OnLowSpecEffect(CNtlTokenizer* pLexer);         ///< 이펙트 로우 스펙 적용
	RwBool      OnLowSpecEffectRatio(CNtlTokenizer* pLexer);    ///< 이펙트 로우 스펙 적용
    RwBool      OnRenderMesh(CNtlTokenizer* pLexer);            ///< Mesh 이펙트 렌더링 유무
    RwBool      OnRenderDecal(CNtlTokenizer* pLexer);           ///< 데칼 이펙트 렌더링 유무	

	RwBool      OnRenderWaterSpecular(CNtlTokenizer* pLexer);           ///< 워터 스페큘라 렌더링 유무
	RwBool		OnRenderTerrainShadow(CNtlTokenizer* pLexer);           ///< 지형 그림자 렌더링 유무

	RwBool		OnWeatherLevel(CNtlTokenizer* pLexer);           ///< 날씨 레벨 설정.

	RwBool		OnBroadCastPosX(CNtlTokenizer* pLexer);
	RwBool		OnMiniNarration(CNtlTokenizer* pLexer);

	RwBool		OnAnimSyncActive(CNtlTokenizer* pLexer);

	RwBool		OnTSReload(CNtlTokenizer* pLexer);

	RwBool		OnDeveloperPreview(CNtlTokenizer* pLexer);

	RwBool		OnDojo(CNtlTokenizer* pLexer);

    RwBool      OnRunAnimSpeedChange(CNtlTokenizer* pLexer);

	RwBool      OnCreatePC(CNtlTokenizer* pLexer);

	RwBool		LoadPalette(CNtlTokenizer* pLexer);

	RwBool		ReloadTable(CNtlTokenizer* pLexer);

//	RwBool      OnTestCull(CNtlTokenizer* pLexer);

	RwBool      OnCrash(CNtlTokenizer* pLexer);

	RwBool      OnPerf(CNtlTokenizer* pLexer);

	RwBool      OnDialog(CNtlTokenizer* pLexer);				///< dialog 관련 /dialog all open(close)
	RwBool      OnPacketTrace(CNtlTokenizer* pLexer);			/// woosungs_test 20090804

	// sound
	RwBool		OnPlayManySound(CNtlTokenizer* pLexer);
	RwBool		OnStopManySound(CNtlTokenizer* pLexer);

	RwBool		OnMinMaxRate(CNtlTokenizer* pLexer);
	RwBool		OnPlaySound(CNtlTokenizer* pLexer);
	RwBool		OnStopSound(CNtlTokenizer* pLexer);

	RwBool		OnShareBGMRate(CNtlTokenizer* pLexer);

	RwBool		OnDestMove(CNtlTokenizer* pLexer);

protected:
    MapCheatHandler m_mapCheatHandler;  ///< 치트 문자열과 처리 함수 맵    

	std::list<SOUND_HANDLE>		m_listPlayList;
	SOUND_HANDLE	m_hSound;

	RwInt32			m_iCurrOpenedDialogIndex;
};


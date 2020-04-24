#pragma once

#include "NtlXMLDoc.h"

// 말풍선 구조체
struct STeleCastBalloon
{
    RwV2d v2dOffset;
    RwV2d v2dSize;
    std::string strFileName;

    STeleCastBalloon()
    {
        ZeroMemory(&v2dOffset, sizeof(RwV2d));
        ZeroMemory(&v2dSize, sizeof(RwV2d));
    }
};

// 어느 타입의 방송창을 부를지 설정
enum TELECAST_TYPE
{
    TELECAST_TYPE_TMQ,                  
    TELECAST_TYPE_TUTORIAL,
    TELECAST_TYPE_SERVER_NOTIFY,
};

/// Telecast관련 UIConfig 구조체
struct SUIConfigTeleCast
{
    TELECAST_TYPE           m_eTelecastType; ///< 이전에 설정한 방송창 타입
    RwInt32     nYPos;                      ///< Y 위치
    RwV2d       v2dSize;                    ///< 크기
    RwReal      fFadeTime;                  ///< Fade 연출에 걸리는 Time
    std::string strFadeFlashFile;           ///< Fade 연출 플래시 파일명
    std::string strOpeningFlashFile;        ///< Open 연출 플래시 파일명
    std::string strClosingFlashFile;        ///< Close 연출 플래시 파일명
    std::string strBackNormalFlashFile;     ///< 일반상태 배경 플래시 파일명
    std::string strBackWarnFlashFile;       ///< 경고/위험 상태 배경 플래시 파일명
    RwReal      fShakeFreq;                 ///< Shake 연출시 초당 진동수
    RwInt32     nShakeAmp;                  ///< Shake 연출시 진폭 (픽셀 단위)    
    std::map<RwInt32, STeleCastBalloon>  mapBalloonRes; ///< 말풍선 리소스 파일명 맵
    RwReal      fSlideStartVel;             ///< 슬라이드 시작 속도
    RwReal      fSlideAccel;                ///< 슬라이드 가속도
    
    RwUInt32    uiNPCID;                    ///< NPC 테이블 ID (서버 알림창에서만 사용)
    RwReal      fShowTime;                  ///< 표시될 시간 (서버 알림창에서만 사용)
    
    SUIConfigTeleCast() 
    : fShakeFreq(0), nShakeAmp(0), nYPos(578), fFadeTime(5.0f), m_eTelecastType(TELECAST_TYPE_TMQ)
    {        
        v2dSize.x = 235.0f;
        v2dSize.y = 176.0f;
        fSlideStartVel = 50.0f;
        fSlideAccel = 20.0f;        
    }
};

#define dBROADCAST_SHAKE_TYPE_NUMS	3

/**
* \brief BroadCast UIConfig
*/
struct SUIConfigBroadCast
{
	RwReal		fPosXRate;									///< X Pos Rate
	RwReal		fPosYRate;									///< Y Pos Rate
	RwReal		fSlideStartVel;								///< Sliding : 시작 속도
	RwReal		fSlideAccelVel;								///< Sliding : 가속도
	RwReal		fBlendStartVel;								///< UI Blending : 시작 속도
	RwReal		fBlendAccelVel;								///< UI Blending : 가속도
	RwReal		fBlendTextStartVel;							///< Blend text 텍스트 : 시작 속도
	RwReal		fBlendTextAccelVel;							///< Blend text : 가속도
	RwReal		fIntervalTextTime;							///< Line : 업데이트 타임
	RwReal		fIntervalPresNum;							///< Line : 한번에 나타날 글자 수
	RwReal		fFadeSlidePos;								///< Fade-Slide : 시작 OffsetX
	RwReal		fFadeSlideStartVel;							///< Fade-Slide : 시작 속도
	RwReal		fFadeSlideAccelVel;							///< Fade-Slide : 가속도
	RwReal		fShakeXAmp[dBROADCAST_SHAKE_TYPE_NUMS];		///< ShakeX : 떨리는 폭
	RwReal		fShakeXFreq[dBROADCAST_SHAKE_TYPE_NUMS];	///< ShakeX : 떨리는 속도
	RwReal		fShakeYAmp[dBROADCAST_SHAKE_TYPE_NUMS];		///< ShakeY : 떨리는 폭
	RwReal		fShakeYFreq[dBROADCAST_SHAKE_TYPE_NUMS];	///< ShakeY : 떨리는 속도
};

// Notify 관련 구조체
struct SUIConfigNotifyPosition
{
	SUIConfigNotifyPosition(VOID)
	{
		fActionNotify_YPosRate = 0.215f;
		fPublicNotify_YPosRate = 0.123f;
		fTerritoryNotify_YPosRate = 0.345f;
		fCautionNotiry_YPosRate = 0.293f;
		fGroupNotiry_YPosRate = .7f;
		fNameNotiry_YPosRate = .6f;
		fBroadcastNotiry_YPosRate = 0.345f;
	}

	RwReal		fActionNotify_YPosRate;						///< YPos / 768; 
	RwReal		fPublicNotify_YPosRate;				
	RwReal		fTerritoryNotify_YPosRate;
	RwReal		fCautionNotiry_YPosRate;
	RwReal		fGroupNotiry_YPosRate;
	RwReal		fNameNotiry_YPosRate;
	RwReal		fBroadcastNotiry_YPosRate;
};

/// /script/UIConfig.XML 로부터 UI 설정들을 읽어와서 보관하고 있는 싱글톤 클래스
class CDBOUIConfig : CNtlXMLDoc
{
public:
    CDBOUIConfig(void);
    ~CDBOUIConfig(void);

    RwBool Load();                              ///< Read Config information from XML file.
    RwBool Reflash();                           ///< Reloads the information from the XML file.

    RwBool LoadTeleCast(TELECAST_TYPE eType);   ///< TeleCast용 정보들을 로딩한다.    
	RwBool LoadBroadCast();						///< BroadCast용 정보 로딩
	RwBool LoadNotify();					

    SUIConfigTeleCast*  GetTeleCastConfig()		{	return &m_TeleCast;		} ///< TeleCast용 Config 정보를 반환한다.
	SUIConfigBroadCast*	GetBroadCastConfig()	{	return &m_BroadCast;	}
	SUIConfigNotifyPosition* GetNotifyConfig()	{	return &m_Notify; }

protected:    
    RwBool LoadTest();                          ///< 테스트용 데이터들을 로딩한다.

protected:    
    char   szBuf[64];                           ///< 사용 버퍼

    SUIConfigTeleCast       m_TeleCast;         ///< Telecast용 설정 객체
	SUIConfigBroadCast		m_BroadCast;		///< BroadCast
	SUIConfigNotifyPosition	m_Notify;			///< Notify
    
    RwReal                  m_fCameraTargetRatio; ///< Follow Camera의 타겟 높이 비율 (캐릭터의 키에 대한 비율) - 테스트용
  
};

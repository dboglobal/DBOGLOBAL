#include "precomp_dboclient.h"
#include "DBOUIConfig.h"
#include "NtlPlHelpers.h"
#include "NtlSLEventFunc.h"
#include <atlbase.h>
#include "NtlCameraController.h"
#include "NtlPLResourcePack.h"



#define UI_CONFIG_XML ".\\script\\UIConfig.xml"

CDBOUIConfig::CDBOUIConfig( void ) 
{
    ZeroMemory(szBuf, sizeof(szBuf));

    Create();
}

CDBOUIConfig::~CDBOUIConfig( void ) 
{

}

RwBool CDBOUIConfig::Load() 
{
	if( GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_SCRIPT )
	{
		char* pData = NULL;
		int nSize = 0;

		GetNtlResourcePackManager()->LoadScript( UI_CONFIG_XML, (void**)&pData, &nSize );
		if( pData == NULL )
			return FALSE;
		
		// 버퍼 +1 생성
		char* pBuffer = NTL_NEW char[nSize+1];
		memcpy( pBuffer, pData, sizeof(char) * nSize );
		pBuffer[nSize] = '\0';
		
		if( !CNtlXMLDoc::LoadXML( pBuffer ) )
		{
			NTL_ARRAY_DELETE( pData );
			NTL_ARRAY_DELETE( pBuffer );

			return FALSE;
		}

		NTL_ARRAY_DELETE( pData );
		NTL_ARRAY_DELETE( pBuffer );
	}
	else
	{
		if(!CNtlXMLDoc::Load(UI_CONFIG_XML))
			return FALSE;
	}

    LoadTeleCast(m_TeleCast.m_eTelecastType);
    LoadTest();
	LoadBroadCast();
	LoadNotify();

    return TRUE;    
}

RwBool CDBOUIConfig::Reflash() 
{
    Destroy();
    Create();
    Load();

    CNtlSLEventGenerator::UIConfigReload();

    return TRUE;
}

RwBool CDBOUIConfig::LoadTeleCast(TELECAST_TYPE eType)
{
    // 타입 비교는 하지 않는다. (리로드)
    m_TeleCast.m_eTelecastType = eType;

    std::string strHeader;
    switch(eType)
    {
    case TELECAST_TYPE_TMQ:
        strHeader = "CONFIG/TMQ/TELECAST/";
        break;
    case TELECAST_TYPE_TUTORIAL:
        strHeader = "CONFIG/TUTORIAL/TELECAST/";
        break;
    case TELECAST_TYPE_SERVER_NOTIFY:
        strHeader = "CONFIG/SERVER_NOTIFY/TELECAST/";
        break;
    }    
    std::string strPos    = strHeader + "POS";
    std::string strFade   = strHeader + "FADE";    
    std::string strFlash  = strHeader + "FLASH";
    std::string strShake  = strHeader + "SHAKE";    
    std::string strBalloon = strHeader + "DIALOG_BALLOON/BALLOON_RES";    
    std::string strSlide    = strHeader + "SLIDE";    

    // POS
    IXMLDOMNode* pNodePos = SelectSingleNode((char*)strPos.c_str());
    if(GetTextWithAttributeName(pNodePos, "Y_POS", szBuf, sizeof(szBuf)))
    {
        m_TeleCast.nYPos = atoi(szBuf);
    }
    if(GetTextWithAttributeName(pNodePos, "SIZE", szBuf, sizeof(szBuf)))
    {
        m_TeleCast.v2dSize = Helper_String2V2D(szBuf);
    }
    pNodePos->Release();

    // FADE
    IXMLDOMNode* pNodeFade = SelectSingleNode((char*)strFade.c_str());
    if(GetTextWithAttributeName(pNodeFade, "FLASH", szBuf, sizeof(szBuf)))
    {
        m_TeleCast.strFadeFlashFile = szBuf;
    }
    if(GetTextWithAttributeName(pNodeFade, "TIME", szBuf, sizeof(szBuf)))
    {
        m_TeleCast.fFadeTime = (RwReal)atof(szBuf);
    }
    pNodeFade->Release();

    // Flash
    IXMLDOMNode* pNodeFlash = SelectSingleNode((char*)strFlash.c_str());            
    if(GetTextWithAttributeName(pNodeFlash, "OPENING", szBuf, sizeof(szBuf)))
    {
        m_TeleCast.strOpeningFlashFile = szBuf;
    }
    if(GetTextWithAttributeName(pNodeFlash, "CLOSING", szBuf, sizeof(szBuf)))
    {
        m_TeleCast.strClosingFlashFile = szBuf;
    }
    if(GetTextWithAttributeName(pNodeFlash, "BACK_NORMAL", szBuf, sizeof(szBuf)))
    {
        m_TeleCast.strBackNormalFlashFile = szBuf;
    }
    if(GetTextWithAttributeName(pNodeFlash, "BACK_WARN", szBuf, sizeof(szBuf)))
    {
        m_TeleCast.strBackWarnFlashFile = szBuf;
    }
    pNodeFlash->Release();

    // Shake
    IXMLDOMNode* pNodeShake = SelectSingleNode((char*)strShake.c_str());
    if(GetTextWithAttributeName(pNodeShake, "FREQ", szBuf, sizeof(szBuf)))
    {
        m_TeleCast.fShakeFreq = (RwReal)atof(szBuf);
    }
    if(GetTextWithAttributeName(pNodeFlash, "AMP", szBuf, sizeof(szBuf)))
    {
        m_TeleCast.nShakeAmp = atoi(szBuf);
    }
    pNodeShake->Release();

    // SLIDE
    IXMLDOMNode* pNodeSlide = SelectSingleNode((char*)strSlide.c_str());
    if(GetTextWithAttributeName(pNodeSlide, "START_VELOCITY", szBuf, sizeof(szBuf)))
    {
        m_TeleCast.fSlideStartVel = (RwReal)atof(szBuf);
    }
    if(GetTextWithAttributeName(pNodeSlide, "ACCEL", szBuf, sizeof(szBuf)))
    {
        m_TeleCast.fSlideAccel = (RwReal)atof(szBuf);
    }
    pNodeSlide->Release();

    // 말풍선     
    IXMLDOMNodeList* pBalloonRes = SelectNodeList((char*)strBalloon.c_str());
    long lCount = 0;
    pBalloonRes->get_length(&lCount);    
    for(long i = 0; i < lCount; ++i)
    {
        IXMLDOMNode* pNodeRes = NULL;
        RwInt32 nIndex = 0;
        pBalloonRes->get_item(i, &pNodeRes);

        STeleCastBalloon sTeleCastBalloon;        
        if(GetTextWithAttributeName(pNodeRes, "INDEX", szBuf, sizeof(szBuf)))
        {
            nIndex = atoi(szBuf);
        }
        if(GetTextWithAttributeName(pNodeRes, "OFFSET", szBuf, sizeof(szBuf)))
        {
            sTeleCastBalloon.v2dOffset = Helper_String2V2D(szBuf);
        }
        if(GetTextWithAttributeName(pNodeRes, "SIZE", szBuf, sizeof(szBuf)))
        {
            sTeleCastBalloon.v2dSize = Helper_String2V2D(szBuf);
        }
        if(GetTextWithAttributeName(pNodeRes, "FILE", szBuf, sizeof(szBuf)))
        {
            sTeleCastBalloon.strFileName = szBuf;
        }

        m_TeleCast.mapBalloonRes[nIndex] = sTeleCastBalloon;
    }

    // 서버 알림창용 설정
    if(eType == TELECAST_TYPE_SERVER_NOTIFY)
    {
        std::string strNPCID    = strHeader + "NPC_TBLID";
        std::string strShowTime = strHeader + "SHOWTIME";
        
        GetDataWithXPath((char*)strNPCID.c_str(), szBuf, sizeof(szBuf));        
        m_TeleCast.uiNPCID = (RwUInt32)atoi(szBuf);

        GetDataWithXPath((char*)strShowTime.c_str(), szBuf, sizeof(szBuf));
        m_TeleCast.fShowTime = (RwReal)atof(szBuf);
    }

        
    return TRUE;
}

RwBool CDBOUIConfig::LoadBroadCast()
{
	std::string strHeader = "CONFIG/BROADCAST/";
	
	std::string strPos			= strHeader + "POS";
	
	std::string strShowHide		= strHeader + "SHOWHIDE";
	std::string strShow_Slide	= strShowHide + "/SLIDE";
	std::string strShow_Fade	= strShowHide + "/FADE";
	std::string strShow_SlideFade = strShowHide + "/SLIDEFADE";

	std::string strSpeech		= strHeader + "SPEECH";
	std::string strSpeech_Blend = strSpeech + "/BLENDING";
	std::string strSpeech_Line	= strSpeech + "/LINE";

	std::string strShake		= strHeader + "SHAKE";
	std::string	strShake_Normal	= strShake + "/NORMAL";
	std::string strShake_Warning = strShake + "/WARNING";
	std::string strShake_Danger = strShake +"/DANGER";
	
	// POS

	IXMLDOMNode* pNodePos = SelectSingleNode((char*)strPos.c_str());
	if(GetTextWithAttributeName(pNodePos, "X_POS", szBuf, sizeof(szBuf)))
		m_BroadCast.fPosXRate = (RwReal)atof(szBuf);

	if(GetTextWithAttributeName(pNodePos, "Y_POS", szBuf, sizeof(szBuf)))
		m_BroadCast.fPosYRate = (RwReal)atof(szBuf);
	
	pNodePos->Release();

	// SHOWHIDE : SLIDE
	IXMLDOMNode* pNodeSlide = SelectSingleNode((char*)strShow_Slide.c_str());
	if(GetTextWithAttributeName(pNodeSlide, "STARTVEL", szBuf, sizeof(szBuf)))
		m_BroadCast.fSlideStartVel = (RwReal)atof(szBuf);

	if(GetTextWithAttributeName(pNodeSlide, "ACCELVEL", szBuf, sizeof(szBuf)))
		m_BroadCast.fSlideAccelVel = (RwReal)atof(szBuf);

	pNodeSlide->Release();

	// SHOWHIDE : FADE
	IXMLDOMNode* pNodeFade = SelectSingleNode((char*)strShow_Fade.c_str());
	if(GetTextWithAttributeName(pNodeFade, "STARTVEL", szBuf, sizeof(szBuf)))
		m_BroadCast.fBlendStartVel = (RwReal)atof(szBuf);

	if(GetTextWithAttributeName(pNodeFade, "ACCELVEL", szBuf, sizeof(szBuf)))
		m_BroadCast.fBlendAccelVel = (RwReal)atof(szBuf);

	pNodeFade->Release();

	// SHOWHIDE : SLIDE_FADE
	IXMLDOMNode* pNodeSlideFade = SelectSingleNode((char*)strShow_SlideFade.c_str());
	if(GetTextWithAttributeName(pNodeSlideFade, "OFFSETX", szBuf, sizeof(szBuf)))
		m_BroadCast.fFadeSlidePos = (RwReal)atof(szBuf);

	if(GetTextWithAttributeName(pNodeSlideFade, "STARTVEL", szBuf, sizeof(szBuf)))
		m_BroadCast.fFadeSlideStartVel = (RwReal)atof(szBuf);

	if(GetTextWithAttributeName(pNodeSlideFade, "ACCELVEL", szBuf, sizeof(szBuf)))
		m_BroadCast.fFadeSlideAccelVel = (RwReal)atof(szBuf);

	pNodeSlideFade->Release();

	// Speech Direction : Blending
	IXMLDOMNode* pNodeSpeechBlend = SelectSingleNode((char*)strSpeech_Blend.c_str());
	if(GetTextWithAttributeName(pNodeSpeechBlend, "STARTVEL", szBuf, sizeof(szBuf)))
		m_BroadCast.fBlendTextStartVel = (RwReal)atof(szBuf);

	if(GetTextWithAttributeName(pNodeSpeechBlend, "ACCELVEL", szBuf, sizeof(szBuf)))
		m_BroadCast.fBlendTextAccelVel = (RwReal)atof(szBuf);

	pNodeSpeechBlend->Release();

	// Speech Direction : Line
	IXMLDOMNode* pNodeSpeechLine = SelectSingleNode((char*)strSpeech_Line.c_str());
	if(GetTextWithAttributeName(pNodeSpeechLine, "INTERVALTIME", szBuf, sizeof(szBuf)))
		m_BroadCast.fIntervalTextTime = (RwReal)atof(szBuf);

	if(GetTextWithAttributeName(pNodeSpeechLine, "PRESENTNUM", szBuf, sizeof(szBuf)))
		m_BroadCast.fIntervalPresNum = (RwReal)atof(szBuf);

	pNodeSpeechLine->Release();

	// Shake Normal
	IXMLDOMNode* pNodeShakeNormal = SelectSingleNode((char*)strShake_Normal.c_str());
	if(GetTextWithAttributeName(pNodeShakeNormal, "AMPX", szBuf, sizeof(szBuf)))
		m_BroadCast.fShakeXAmp[0] = (RwReal)atof(szBuf);

	if(GetTextWithAttributeName(pNodeShakeNormal, "AMPY", szBuf, sizeof(szBuf)))
		m_BroadCast.fShakeYAmp[0] = (RwReal)atof(szBuf);

	if(GetTextWithAttributeName(pNodeShakeNormal, "FREQX", szBuf, sizeof(szBuf)))
		m_BroadCast.fShakeXFreq[0] = (RwReal)atof(szBuf);
	
	if(GetTextWithAttributeName(pNodeShakeNormal, "FREQY", szBuf, sizeof(szBuf)))
		m_BroadCast.fShakeYFreq[0] = (RwReal)atof(szBuf);

	pNodeShakeNormal->Release();

	// Shake Warning
	IXMLDOMNode* pNodeShakeWarning = SelectSingleNode((char*)strShake_Warning.c_str());
	if(GetTextWithAttributeName(pNodeShakeWarning, "AMPX", szBuf, sizeof(szBuf)))
		m_BroadCast.fShakeXAmp[1] = (RwReal)atof(szBuf);

	if(GetTextWithAttributeName(pNodeShakeWarning, "AMPY", szBuf, sizeof(szBuf)))
		m_BroadCast.fShakeYAmp[1] = (RwReal)atof(szBuf);

	if(GetTextWithAttributeName(pNodeShakeWarning, "FREQX", szBuf, sizeof(szBuf)))
		m_BroadCast.fShakeXFreq[1] = (RwReal)atof(szBuf);

	if(GetTextWithAttributeName(pNodeShakeWarning, "FREQY", szBuf, sizeof(szBuf)))
		m_BroadCast.fShakeYFreq[1] = (RwReal)atof(szBuf);

	pNodeShakeWarning->Release();

	// Shake Danger
	IXMLDOMNode* pNodeShakeDanger = SelectSingleNode((char*)strShake_Danger.c_str());
	if(GetTextWithAttributeName(pNodeShakeDanger, "AMPX", szBuf, sizeof(szBuf)))
		m_BroadCast.fShakeXAmp[2] = (RwReal)atof(szBuf);

	if(GetTextWithAttributeName(pNodeShakeDanger, "AMPY", szBuf, sizeof(szBuf)))
		m_BroadCast.fShakeYAmp[2] = (RwReal)atof(szBuf);

	if(GetTextWithAttributeName(pNodeShakeDanger, "FREQX", szBuf, sizeof(szBuf)))
		m_BroadCast.fShakeXFreq[2] = (RwReal)atof(szBuf);

	if(GetTextWithAttributeName(pNodeShakeDanger, "FREQY", szBuf, sizeof(szBuf)))
		m_BroadCast.fShakeYFreq[2] = (RwReal)atof(szBuf);

	pNodeShakeDanger->Release();

	return TRUE;
}

RwBool CDBOUIConfig::LoadNotify()
{
	std::string strHeader = "CONFIG/NOTIFY/";

	std::string strAction		= strHeader + "ACTION";
	std::string strPublic		= strHeader + "PUBLIC";
	std::string strTerritory	= strHeader + "TERRITORY";
	std::string strCaution		= strHeader + "CAUTION";	

	// ACTION
	IXMLDOMNode* pNodeAction = SelectSingleNode((char*)strAction.c_str());
	if(GetTextWithAttributeName(pNodeAction, "YPOSRATE", szBuf, sizeof(szBuf)))
		m_Notify.fActionNotify_YPosRate = (RwReal)atof(szBuf);
	pNodeAction->Release();

	// PUBLIC
	IXMLDOMNode* pNodePublic = SelectSingleNode((char*)strPublic.c_str());
	if(GetTextWithAttributeName(pNodePublic, "YPOSRATE", szBuf, sizeof(szBuf)))
		m_Notify.fPublicNotify_YPosRate = (RwReal)atof(szBuf);
	pNodePublic->Release();

	// TERRITORY
	IXMLDOMNode* pNodeTerritory = SelectSingleNode((char*)strTerritory.c_str());
	if(GetTextWithAttributeName(pNodeTerritory, "YPOSRATE", szBuf, sizeof(szBuf)))
		m_Notify.fTerritoryNotify_YPosRate = (RwReal)atof(szBuf);
	pNodeTerritory->Release();

	// CAUTION
	IXMLDOMNode* pNodeCaution = SelectSingleNode((char*)strCaution.c_str());
	if(GetTextWithAttributeName(pNodeCaution, "YPOSRATE", szBuf, sizeof(szBuf)))
		m_Notify.fCautionNotiry_YPosRate = (RwReal)atof(szBuf);
	pNodeCaution->Release();

	return TRUE;
}

RwBool CDBOUIConfig::LoadTest() 
{
    GetDataWithXPath("CONFIG/CAMERA_TARGET_RATIO", szBuf, sizeof(szBuf));
    
    CNtlCameraController::SetTargetHeightRatio((RwReal)atof(szBuf));

    return TRUE;
}

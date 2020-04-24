#include "precomp_dboclient.h"
#include "DboEventGenerator.h"
#include "NtlEventQueue.h"
#include "NtlSysEvent.h"
#include "DboEvent.h"
#include "ScriptLinkTable.h"
#include "NtlDebug.h"


void CDboEventGenerator::LogInServerConnect(RwUInt8 byConnectType)
{
	SDboEventServerConnect sConnect;
	sConnect.byConnectType = byConnectType;
	
	RWS::CMsg msg;
	msg.Id = g_EventLogInServerConnect;
	msg.pData = reinterpret_cast<void*>(&sConnect);
	PostMsg(msg, reinterpret_cast<void*>(&sConnect), sizeof(SDboEventServerConnect));
}

void CDboEventGenerator::LobbyServerConnect(RwUInt8 byConnectType)
{
	SDboEventServerConnect sConnect;
	sConnect.byConnectType = byConnectType;
	
	RWS::CMsg msg;
	msg.Id = g_EventLobbyServerConnect;
	msg.pData = reinterpret_cast<void*>(&sConnect);
	PostMsg(msg, reinterpret_cast<void*>(&sConnect), sizeof(SDboEventServerConnect));
}

void CDboEventGenerator::GameServerConnect(RwUInt8 byConnectType)
{
	SDboEventServerConnect sConnect;
	sConnect.byConnectType = byConnectType;

	RWS::CMsg msg;
	msg.Id = g_EventGameServerConnect;
	msg.pData = reinterpret_cast<void*>(&sConnect);
	
	PostMsg(msg, reinterpret_cast<void*>(&sConnect), sizeof(SDboEventServerConnect));
}

void CDboEventGenerator::StageCreate(const RwChar *pStageName)
{
	RWS::CMsg msg;
	msg.Id = g_EventCreateStage;

	SNtlEventStageData data;
	strcpy_s(data.chStageName, 128, pStageName); 
	
	PostMsg(msg, reinterpret_cast<void*>(&data), sizeof(SNtlEventStageData));
}

void CDboEventGenerator::StageDelete(const RwChar *pStageName)
{
	RWS::CMsg msg;
	msg.Id = g_EventDeleteStage;

	SNtlEventStageData data;
	strcpy_s(data.chStageName, 128, pStageName); 
	
	PostMsg(msg, reinterpret_cast<void*>(&data), sizeof(SNtlEventStageData));
}

void CDboEventGenerator::Debug_for_Developer(RwUInt32 uiType)
{
	sDboDeveloperData sDeveloperData;
	sDeveloperData.uiType = uiType;

	RWS::CMsg msg;
	msg.Id = g_EventDebug_for_Developer;
	msg.pData = reinterpret_cast<void*>(&sDeveloperData);

	PostMsg(msg, reinterpret_cast<void*>(&sDeveloperData), sizeof(sDboDeveloperData));
}

void CDboEventGenerator::LobbyPacketHelper(RwUInt32 uiCord)
{
	RWS::CMsg msg;
	msg.Id = g_EventLobbyPacketHelper;
	msg.pData = reinterpret_cast<void*>(&uiCord);
	_SendMsg(msg);
}

void CDboEventGenerator::LogInStageStateEnter(RwUInt8 byState)
{
	SDboEventLogInStageStateEnter sStageEnter;
	sStageEnter.byState = byState;
	
	RWS::CMsg msg;
	msg.Id = g_EventLogInStageStateEnter;
	msg.pData = reinterpret_cast<void*>(&sStageEnter);
	_SendMsg(msg);
}

void CDboEventGenerator::LogInStageStateExit(RwUInt8 byState)
{
	SDboEventLogInStageStateExit sStageExit;
	sStageExit.byState = byState;
	
	RWS::CMsg msg;
	msg.Id = g_EventLogInStageStateExit;
	msg.pData = reinterpret_cast<void*>(&sStageExit);
	_SendMsg(msg);
}

void CDboEventGenerator::LogInStageTimeOut(RwUInt8 byState)
{
	SDboEventLogInStageTimeOut sStageTimeOut;
	sStageTimeOut.byState = byState;
	
	RWS::CMsg msg;
	msg.Id = g_EventLogInStageTimeOut;
	msg.pData = reinterpret_cast<void*>(&sStageTimeOut);
	_SendMsg(msg);
}

void CDboEventGenerator::CharStageStateEnter(RwUInt8 byState)
{
	SDboEventCharStageStateEnter sStageEnter;
	sStageEnter.byState = byState;
	
	RWS::CMsg msg;
	msg.Id = g_EventCharStageStateEnter;
	msg.pData = reinterpret_cast<void*>(&sStageEnter);
	_SendMsg(msg);
}

void CDboEventGenerator::CharStageStateExit(RwUInt8 byState)
{
	SDboEventCharStageStateExit sStageExit;
	sStageExit.byState = byState;
	
	RWS::CMsg msg;
	msg.Id = g_EventCharStageStateExit;
	msg.pData = reinterpret_cast<void*>(&sStageExit);
	_SendMsg(msg);
}

void CDboEventGenerator::CharMaking(RwBool bSetDefaultCamera, RwUInt8	byRace, RwUInt8	byClass, RwUInt8 byHair, RwUInt8 byHairColor, RwUInt8 byFace, RwUInt8 bySkinColor,
									RwUInt8 byGender)
{
	SNtlEventCharMake sCharMake;
	sCharMake.bSetDefaultCamera = bSetDefaultCamera;
	sCharMake.byRace = byRace;
	sCharMake.byClass = byClass;	
	sCharMake.byHair = byHair;
	sCharMake.bySkinColor = bySkinColor;
	sCharMake.byFace = byFace;
	sCharMake.byHairColor = byHairColor;
	sCharMake.byGender = byGender;

	RWS::CMsg msg;
	msg.Id = g_EventCharMake;
	msg.pData = reinterpret_cast<void*>(&sCharMake);
	_SendMsg(msg);
}

void CDboEventGenerator::LoginEvent(RwUInt8 byMessage, RwReal fValue /* = 0.f */, RwUInt32 uiExData /* = 0 */)
{
	SDboEventLoginMessage sLoginMessage;

	sLoginMessage.byMessage		= byMessage;
	sLoginMessage.fValue		= fValue;
	sLoginMessage.uiExData		= uiExData;

	RWS::CMsg msg;
	msg.Id = g_EventLoginMessage;
	msg.pData = reinterpret_cast<void*>(&sLoginMessage);
	_SendMsg(msg);
}

void CDboEventGenerator::LoginPostEvent(RwUInt8 byMessage, RwReal fValue /* = 0.f */, RwUInt32 uiExData /* = 0 */)
{
	SDboEventLoginMessage sLoginMessage;

	sLoginMessage.byMessage		= byMessage;
	sLoginMessage.fValue		= fValue;
	sLoginMessage.uiExData		= uiExData;

	RWS::CMsg msg;
	msg.Id = g_EventLoginMessage;
	msg.pData = reinterpret_cast<void*>(&sLoginMessage);

	PostMsg(msg, reinterpret_cast<void*>(&sLoginMessage), sizeof(SDboEventLoginMessage));
}


void CDboEventGenerator::LobbyEvent(RwUInt8 byMessage, RwReal fValue /* = 0.f */, RwUInt32 uiExData /* = 0 */)
{
	SDboEventLobbyMessage sLobbyMessage;

	sLobbyMessage.byMessage		= byMessage;
	sLobbyMessage.fValue		= fValue;
	sLobbyMessage.uiExData		= uiExData;

	RWS::CMsg msg;
	msg.Id = g_EventLobbyMessage;
	msg.pData = reinterpret_cast<void*>(&sLobbyMessage);
	_SendMsg(msg);
}

void CDboEventGenerator::LoginGuiEnable(void)
{
	RWS::CMsg msg;
	msg.Id = g_EventLoginGuiEnable;
	
	_SendMsg(msg);
}

void CDboEventGenerator::CharSelectGuiEnable(void)
{
	RWS::CMsg msg;
	msg.Id = g_EventCharSelectGuiEnable;
		
	_SendMsg(msg);
}

void CDboEventGenerator::MsgBoxShow(const char* strKey, const WCHAR *pString, RwBool bPopup /* = FALSE */, RwBool bHasInput /*= FALSE*/, RwReal fShowTime /* = 0.0f */,
									 sMsgBoxData* pData /* = NULL */, std::list<sMsgBoxCustomBtn>* plistCustomBtn /* = NULL  */,  RwBool bAcceptDuplicate /* = TRUE */, RwBool bUpdate /* = FALSE */ )
{
	SDboEventMsgBoxShow data;

	if( pString )
		data.wstrString = pString;

	data.strKey		= strKey;
	data.bPopup		= bPopup;
	data.fShowTime	= fShowTime;
	data.bHasInput	= bHasInput;
	data.pData		= pData;
	data.plistCustomBtn = plistCustomBtn;
	data.bAcceptDuplicate = bAcceptDuplicate;
	data.bUpdate	= bUpdate;	

	RWS::CMsg msg;
	msg.Id = g_EventMsgBoxShow;
	msg.pData = reinterpret_cast<void*>(&data);

	_SendMsg(msg);
}

void CDboEventGenerator::MsgBoxResult( RwInt32 eResult, const char* StringID, sMsgBoxData* pData )
{
	SDboEventMsgBoxResult data;
	data.eResult = eResult;
	data.strID = StringID;
	data.pData = NULL;
	
	if (pData)
		data.pData = pData;

	SEND_MSG(g_EventMsgBoxResult, &data);
}

void CDboEventGenerator::ChatGuiMode(RwBool bMode)
{
	SDboEventChatGuiMode data;
	data.bMode = bMode;

	RWS::CMsg msg;
	msg.Id = g_EventChatGuiMode;
	msg.pData = reinterpret_cast<void*>(&data);

	_SendMsg(msg);
}

void CDboEventGenerator::SayMessage(RwUInt8 byMsgType, const WCHAR *pUserName, RwUInt16 wMsgLen, const WCHAR *pMessage, RwUInt32 hObject /* = INVALID_SERIAL_ID */, BYTE byChannel /*= 0xff*/)
{
	SDboEventSayMessage data;
	memset(&data, 0, sizeof(data));
	data.byMsgType = byMsgType;
	data.uiSerial = hObject;

	if (pUserName)
		NTL_SAFE_WCSCPY(data.wchUserName, pUserName);

	if( pMessage )
		NTL_SAFE_WCSCPY(data.wchMessage, pMessage);

	data.byChannel = byChannel;

	RWS::CMsg msg;
	msg.Id = g_EventSayMessage;
	msg.pData = reinterpret_cast<void*>(&data);

	_SendMsg(msg);
}

void CDboEventGenerator::SetTextToInputChat( WCHAR* pString )
{
	RWS::CMsg msg;
	msg.Id = g_EventSetTextToChatInput;
	msg.pData = reinterpret_cast<void*>(pString);

	_SendMsg(msg);
}

void CDboEventGenerator::TimeFixedNotifyMessage(RwBool bActive, std::string strStringID, const WCHAR* pString)
{
	SDboEventTimeFixedNotify data;
	memset(&data, 0, sizeof(data));
	data.bActive = bActive;
	data.strStringID = strStringID;
	data.pString = pString;
	
	RWS::CMsg msg;
	msg.Id = g_EventTimeFixedNotify;
	msg.pData = reinterpret_cast<void*>(&data);

	_SendMsg(msg);
}

void CDboEventGenerator::CautionSideNotify( RwBool bActive, std::string& strStringID, const WCHAR* pString, RwReal fLifeTime )
{
	SDboEventCautionSideNotify data;
	data.bActive = bActive;
	data.strStringID = strStringID;
	data.pString = pString;
	data.fLifeTime = fLifeTime;

	RWS::CMsg msg;
	msg.Id = g_EventCautionSideNotify;
	msg.pData = reinterpret_cast<void*>(&data);

	_SendMsg(msg);
}

void CDboEventGenerator::NotifyMessage( RwInt32 eType, const WCHAR* pMessage )
{
	SDboEventNotify data;
	memset( &data, 0, sizeof( data ) );
	data.eType = eType;
	NTL_SAFE_WCSCPY(data.wchMessage, pMessage);
	//memcpy( data.wchMessage, pMessage, sizeof( WCHAR ) * NTL_MAX_LENGTH_OF_CHAT_MESSAGE);
	
	SEND_MSG(g_EventNotify, &data);
}

void CDboEventGenerator::FlashNotify(RwUInt8 byProgressType /* = 0xff */, RwUInt8 byMessageValue /* = 0xff */,
									 RwUInt8 byMessageType /* = 0xff */)
{
	SDboEventFlashNotify data;
	data.byProgressType		= byProgressType;
	data.byMessageValue		= byMessageValue;
	data.byMessageType		= byMessageType;

	RWS::CMsg msg;
	msg.Id = g_EventFlashNotify;
	msg.pData = reinterpret_cast<void*>( &data );

	_SendMsg( msg );
}

void CDboEventGenerator::GroupNotify(RwUInt8 byType, WCHAR* pwcText)
{
	SDboEventGroupNotify data;
	data.byType				= byType;
	data.pwcText			= pwcText;

	RWS::CMsg msg;
	msg.Id = g_EventGroupNotify;
	msg.pData = reinterpret_cast<void*>( &data );

	_SendMsg( msg );
}

void CDboEventGenerator::NameNotify(WCHAR* pwcText)
{
	SDboEventNameNotify data;
	data.pwcText			= pwcText;

	RWS::CMsg msg;
	msg.Id = g_EventNameNotify;
	msg.pData = reinterpret_cast<void*>( &data );

	_SendMsg( msg );
}

void CDboEventGenerator::FlashNotifyString(const RwChar* pcFlashFile, const RwChar* pcSecondFlashFile /* = NULL */,
										   RwBool bFinshNotify /* = TRUE */)
{
	SDboEventFlashNotifyString data;
	data.pcFlashFile		= (RwChar*)pcFlashFile;
	data.pcSecondFlashFile	= (RwChar*)pcSecondFlashFile;
	data.bFinishNotify		= bFinshNotify;

	RWS::CMsg msg;
	msg.Id = g_EventFlashNotifyString;
	msg.pData = reinterpret_cast<void*>( &data );

	_SendMsg( msg );
}

void CDboEventGenerator::FlashFinishNotify(RwUInt8 byProgressType /* = 0xff */, RwUInt8 byMessageValue /* = 0xff */,
									 RwUInt8 byMessageType /* = 0xff */, char* pcText /* = NULL */, char* pcText2 /* = NULL */)
{
	SDboEventFlashFinishNotify data;
	data.byProgressType		= byProgressType;
	data.byMessageValue		= byMessageValue;
	data.byMessageType		= byMessageType;
	data.pcFlashFile		= pcText;
	data.pcSecondFlashFile  = pcText2;

	RWS::CMsg msg;
	msg.Id = g_EventFlashFinishNotify;
	msg.pData = reinterpret_cast<void*>( &data );

	_SendMsg( msg );
}

void CDboEventGenerator::IconMoveClick( RwBool bPickUp )
{
	RWS::CMsg msg;
	msg.Id = g_EventIconMoveClick;
	msg.pData = (void*)bPickUp;

	_SendMsg(msg);
}

void CDboEventGenerator::IconPopupShow(RwBool bShow, RwUInt32 uiSerial, RwInt32 nSrcPlace, RwInt32 nSrcSlotIdx /* = 0 */, RwInt32 nXPos /* = 0 */, RwInt32 nYPos /* = 0  */)
{
	SDboEventIconPopupShow data;
	memset(&data, 0, sizeof(data));
	data.bShow = bShow;
	data.uiSerial = uiSerial;
	data.nSrcSlotIdx = nSrcSlotIdx;
	data.nXPos = nXPos;
	data.nYPos = nYPos;
	data.nSrcPlace = nSrcPlace;

	RWS::CMsg msg;
	msg.Id = g_EventIconPopupShow;
	msg.pData = reinterpret_cast<void*>( &data );

	_SendMsg(msg);
}

void CDboEventGenerator::IconPopupResult(RwUInt32 uiSerial, RwInt32 nSrcPlace, RwInt32 nSrcSlotIdx, RwInt32 nWorkId)
{
	SDboEventIconPopupResult data;
	memset(&data, 0, sizeof(data));
	data.uiSerial = uiSerial;
	data.nSrcPlace = nSrcPlace;
	data.nSrcSlotIdx = nSrcSlotIdx;
	data.nWorkId = nWorkId;

	RWS::CMsg msg;
	msg.Id = g_EventIconPopupResult;
	msg.pData = reinterpret_cast<void*>( &data );

	_SendMsg(msg);
}

void CDboEventGenerator::CalcPopupShow(RwBool bShow, RwUInt32 uiSerial, RwInt32 nSrcPlace, RwInt32 nXPos, RwInt32 nYPos, RwUInt32 uiMaxValue, bool bAllowNull/* = false*/)
{
	SDboEventCalcPopupShow data;
	memset(&data, 0, sizeof(data));
	data.bShow = bShow;
	data.uiSerial = uiSerial;
	data.nXPos = nXPos;
	data.nYPos = nYPos;
	data.nSrcPlace = nSrcPlace;
	data.uiMaxValue = uiMaxValue;
	data.bAllowNull = bAllowNull;

	RWS::CMsg msg;
	msg.Id = g_EventCalcPopupShow;
	msg.pData = reinterpret_cast<void*>( &data );

	_SendMsg(msg);
}

void CDboEventGenerator::CalcPopupResult( RwUInt32 uiSerial, RwInt32 nSrcPlace, RwUInt32 uiValue )
{
	SDboEventCalcPopupResult data;
	memset(&data, 0, sizeof(data));
	data.uiSerial = uiSerial;
	data.nSrcPlace = nSrcPlace;
	data.uiValue = uiValue;

	RWS::CMsg msg;
	msg.Id = g_EventCalcPopupResult;
	msg.pData = reinterpret_cast<void*>( &data );

	_SendMsg(msg);
}

void CDboEventGenerator::PickedUpHide(RwInt32 nSrcPlace)
{
	RWS::CMsg msg;
	msg.Id = g_EventPickedUpHide;
	msg.pData = (void*)nSrcPlace;

	_SendMsg(msg);
}

void CDboEventGenerator::ShopEvent(RwInt32 iType, RwUInt32 uiSerial,
										  unsigned long ulPrice /* = 0 */,
										  wchar_t* pcName /* = NULL */, 
										  int iPlace /* = 0 */,
										  int iPosition /* = 0 */,
										  int iOverlapCount /* = 0 */, 
										RwUInt32 needItemCount /* = 0*/)
{
	RWS::CMsg msg;
	SDboEventShop TradeCart;

	TradeCart.iType = iType;

	TradeCart.uiSerial = uiSerial;;
	TradeCart.ulPrice = ulPrice;
	TradeCart.pcItemName = pcName;
	TradeCart.needItemCount = needItemCount;

	// for server
	TradeCart.iPlace = iPlace;
	TradeCart.iPosition = iPosition;
	TradeCart.iOverlapCount = iOverlapCount;

	msg.Id = g_EventShop;
	msg.pData = (void*)&TradeCart;

	_SendMsg(msg);
}

void CDboEventGenerator::DialogEvent(RwInt32 iType, RwInt32 iSrcPlace /* = 0 */, RwInt32 iDestPlace /* = 0 */,
									 RwInt32 iUserData /*  = 0 */, RwInt32 iUserData2 /*  = 0 */,
									 RwUInt32 uiUserData3 /*  = 0xffffffff */, RwUInt32 uiUserData4 /*  = 0xffffffff */)
{
	RWS::CMsg msg;
	SDboEventDialog packet;

	packet.iType = iType;

	packet.iSrcDialog = iSrcPlace;
	packet.iDestDialog = iDestPlace;
	packet.iUserData  = iUserData;
	packet.iUserData2 = iUserData2;
	packet.uiUserData3 = uiUserData3;
	packet.uiUserData4 = uiUserData4;

	msg.Id = g_EventDialog;
	msg.pData = (void*)&packet;

	_SendMsg(msg);
}

void CDboEventGenerator::DialogPostEvent(RwInt32 iType, RwInt32 iSrcPlace /* = 0 */, RwInt32 iDestPlace /* = 0 */,
										 RwInt32 iUserData /*  = 0 */, RwInt32 iUserData2 /*  = 0 */,
										 RwUInt32 uiUserData3 /*  = 0xffffffff */, RwUInt32 uiUserData4 /*  = 0xffffffff */)
{
	RWS::CMsg msg;
	SDboEventDialog packet;

	packet.iType = iType;

	packet.iSrcDialog = iSrcPlace;
	packet.iDestDialog = iDestPlace;
	packet.iUserData  = iUserData;
	packet.iUserData2 = iUserData2;
	packet.uiUserData3 = uiUserData3;
	packet.uiUserData4 = uiUserData4;

	msg.Id = g_EventPostDialog;
	msg.pData = (void*)&packet;

	PostMsg(msg, reinterpret_cast<void*>(&packet), sizeof(SDboEventDialog));
}

void CDboEventGenerator::UpdateRP( RwUInt32 hSerial, RwInt32 nRP, RwInt32 nMaxRP )
{
	SDboEventUpdateRP data;
	data.hSerial = hSerial;
	data.nRP = nRP;
	data.nMaxRP = nMaxRP;
	
    SEND_MSG(g_EventUpdateRP, &data);
}

void CDboEventGenerator::UpdateRPStock( RwUInt32 hSerial, RwInt32 nRpStock, RwInt32 nMaxRpStock, bool bDropByTime /* = false */)
{
    SDboEventUpdateRpStock data;
    data.hSerial = hSerial;
    data.nRpStock = nRpStock;
    data.nMaxRpStock = nMaxRpStock;
	data.bDropByTime = bDropByTime;

    SEND_MSG(g_EventUpdateRpStock, &data);
}

void CDboEventGenerator::UpdateExp(RwUInt32 uiIncreasedExp, RwUInt32 uiAcquisitionExp, RwUInt32 uiBonusExp )
{
	SDboEventUpdateExp data;
	data.uiIncreasedExp = uiIncreasedExp;
    data.uiAcquisitionExp = uiAcquisitionExp;
    data.uiBonusExp = uiBonusExp;

    SEND_MSG(g_EventUpdateExp, &data);
}

void CDboEventGenerator::CharObjDelete( RwUInt32 uiSerial )
{
	RWS::CMsg msg;

	msg.Id = g_EventCharObjDelete;
	msg.pData = &uiSerial;

	_SendMsg(msg);
}

void CDboEventGenerator::ItemUpgradeResult(WORD wResultcode, BYTE byGrade, WORD wResultMessageCode, BYTE byStoneCount, BYTE byGuardStoneCount)
{
	SDboItemUpgradeResult sResult;
	sResult.wResultcode = wResultcode;
	sResult.byGrade = byGrade;
	sResult.wResultMessageCode = wResultMessageCode;
	sResult.byStoneCount = byStoneCount;
	sResult.byGuardStoneCount = byGuardStoneCount;

	SEND_MSG(g_EventItemUpgradeResult, &sResult);
}

void CDboEventGenerator::RegistItemUpgrade( RwUInt32 hSerialID, RwUInt32 eSrcPlace, RwUInt32 uiSlotIdx )
{
	SDboRegisterItemUpgrade sData;
	sData.hSrcSerial = hSerialID;
	sData.eSrcPlace = eSrcPlace;
	sData.uiSrcSlotIdx = uiSlotIdx;

	SEND_MSG(g_EventRegisterItemUpgrade, &sData);
}

void CDboEventGenerator::ItemChangeOptionConfirmation()
{
	SEND_MSG(g_EventItemChangeOptionConfirmation, NULL);
}

void CDboEventGenerator::ItemChangeOptionResult(void* pItemOptionSet, BYTE byBoxCount)
{
	SDboItemChangeOptionResult sResult;
	sResult.pItemOptionSet = pItemOptionSet;
	sResult.byBoxCount = byBoxCount;

	SEND_MSG(g_EventItemChangeOptionResult, &sResult);
}

void CDboEventGenerator::ItemChangeBattleAttributeResult(WORD wResultCode, RwUInt8 byBattleAttribute)
{
	SDboItemChangeBattleAttributeResult sData;
	sData.wResultcode = wResultCode;
	sData.byBattleAttribute = byBattleAttribute;

	SEND_MSG(g_EventItemChangeBattleAttributeResult, &sData);
}

void CDboEventGenerator::RegisterItemChangeBattleAttribute(RwUInt32 hSerialID, RwUInt32 eSrcPlace, RwUInt32 uiSlotIdx)
{
	SDboRegisterItemUpgrade sData;
	sData.hSrcSerial = hSerialID;
	sData.eSrcPlace = eSrcPlace;
	sData.uiSrcSlotIdx = uiSlotIdx;

	SEND_MSG(g_EventRegisterItemChangeBattleAttribute, &sData);
}


void CDboEventGenerator::CharTitleSelectRes(TBLIDX uiTitle)
{
	SDboEventCharTitleSelectRes sData;
	sData.uiTitle = uiTitle;

	SEND_MSG(g_EventCharTitleSelectRes, &sData);
}

void CDboEventGenerator::CharTitleUpdate(TBLIDX uiTitle, bool bDelete)
{
	SDboEventCharTitleUpdate sData;
	sData.uiTitle = uiTitle;
	sData.bDelete = bDelete;

	SEND_MSG(g_EventCharTitleUpdate, &sData);
}

void CDboEventGenerator::QuickSlotInfo(void)
{
	RWS::CMsg msg;
	msg.Id = g_EventQuickSlotInfo;
	msg.pData = NULL;
	_SendMsg( msg );
}

void CDboEventGenerator::QuickSlotDelNfy( RwUInt8 bySlotIdx, RwUInt32 hSerialID /*= 0xffffffff*/, RwBool bSendPacket/* = FALSE*/)
{
	SDboEventQuickSlotDelNfy sData;
	sData.bySlotIdx = bySlotIdx;
	sData.bSendPacket = bSendPacket;

	SEND_MSG(g_EventQuickSlotDelNfy, &sData);
}

void CDboEventGenerator::NPCDialogOpen( RwUInt32 hSerialID, RwInt32 eDialog )
{
	SDboNpcDialogOpen sNPCDialog;
	sNPCDialog.hSerialId = hSerialID;
	sNPCDialog.eDialog = eDialog;

	SEND_MSG(g_EventNPCDialogOpen, &sNPCDialog);
}

void CDboEventGenerator::EnterWorld()
{
	RWS::CMsg msg;
	msg.Id = g_EventEndterWorld;
	msg.pData = NULL;
	_SendMsg( msg );
}

void CDboEventGenerator::MapEvent(RwInt32 iMessage, RwUInt32 uiValue /* = 0 */)
{
	SDboEventMap sEventMap;
	sEventMap.iMessage	= iMessage;
	sEventMap.uiValue	= uiValue;

	SEND_MSG(g_EventMap, &sEventMap);
}

void CDboEventGenerator::UserTrade(RwUInt8 byEventType, RwUInt32 uiTarget /* = 0xffffffff*/,
									RwUInt32 uiHandle /* = 0xffffffff*/, RwUInt8 byCount /* = 0*/, VOID* pData /* = NULL */)
{
	SDboEventUserTrade sEventUserTrade;

	sEventUserTrade.byEventType = byEventType;
	sEventUserTrade.uiTarget = uiTarget;
	sEventUserTrade.uiHandle = uiHandle;
	sEventUserTrade.byCount = byCount;
	sEventUserTrade.pData = pData;

	SEND_MSG(g_EventUserTrade, &sEventUserTrade);
}

void CDboEventGenerator::ServerCharDialog(RwUInt32 uiSerial, RwUInt8 byDialogType, RwUInt32 uiTextTblIdx, RwBool bIsRefSpeechTable, RwUInt16 wTextLen, WCHAR* pTextData)
{
	SDboEventServerCharDialog sCharDialog;

	sCharDialog.uiSerial = uiSerial;
	sCharDialog.byDialogType = byDialogType;
	sCharDialog.uiTextTblIdx = uiTextTblIdx;
	sCharDialog.bIsRefSpeechTable = bIsRefSpeechTable;
	sCharDialog.wTextLen = wTextLen;
	sCharDialog.pTextData = pTextData;

	SEND_MSG(g_EventServerCharDialog, &sCharDialog);
}

void CDboEventGenerator::OpenHelpWindow(RwUInt32 uiTableIndex)
{
	SDboEventOpenHelpWindow sOpenHelpWindow;
	sOpenHelpWindow.uiTableIndex = uiTableIndex;

	SEND_MSG(g_EventOpenHelpWindow, &sOpenHelpWindow);
}

void CDboEventGenerator::OpenHelpContent( RwUInt32 uiDialogType )
{
	SDboEventOpenHelpContent sData;
	sData.uiDialogType = uiDialogType;

	SEND_MSG( g_EventOpenHelpContent, &sData );
}

void CDboEventGenerator::EnableItemIcon( RwBool bEnable, RwInt32 ePlace, RwUInt32 uiSlotIdx, RwInt32 nPlaceIdx /* = -1  */ )
{
	SDboEventEnableItemIcon sEnableItemIcon;
	sEnableItemIcon.bEnable = bEnable;	
	sEnableItemIcon.ePlace = ePlace;
	sEnableItemIcon.uiSlotIdx = uiSlotIdx;	
	sEnableItemIcon.nPlaceIdx = nPlaceIdx;

	RWS::CMsg msg;
	msg.Id = g_EventEnableItemIcon;
	msg.pData = &sEnableItemIcon;
	_SendMsg(msg);
}

/**
* \brief 
*/
void CDboEventGenerator::CapsuleLockItem( RwBool bEnable, RwInt32 ePlace, RwUInt32 uiSlotIdx, RwInt32 nPlaceIdx /*= -1 */ )
{
	SDboEventCapsuleLockItem sData;

	sData.bEnable = bEnable;	
	sData.ePlace = ePlace;
	sData.uiSlotidx = uiSlotIdx;	
	sData.nPlaceIdx = nPlaceIdx;

	RWS::CMsg msg;
	msg.Id = g_EventCapsuleLockItem;
	msg.pData = &sData;
	_SendMsg(msg);
}

void CDboEventGenerator::CapsuleLockItemWithoutBag( RwBool bEnable, SERIAL_HANDLE hItem )
{
	SDboEventCapsuleLockItemWithoutBag sData;
	sData.bEnable = bEnable;
	sData.hItem = hItem;

	SEND_MSG( g_EventCapsuleLockItemWithoutBag, &sData );
}

void CDboEventGenerator::DirectMoveIcon(RwUInt32 uiSrcSerial, RwUInt32 eSrcPlace, RwUInt32 eDestPlace, RwUInt32 uiSrcSlotIdx, RwUInt32 uiSrcStackCount)
{
	SDboEventDirectMoveIcon sDirectMoveIcon;
	sDirectMoveIcon.uiSrcSerial = uiSrcSerial;
	sDirectMoveIcon.eSrcPlace = eSrcPlace;
	sDirectMoveIcon.eDestPlace = eDestPlace;
	sDirectMoveIcon.uiSrcSlotIdx = uiSrcSlotIdx;
	sDirectMoveIcon.uiSrcStackCount = uiSrcStackCount;

	RWS::CMsg msg;
	msg.Id = g_EventDirectMoveIcon;
	msg.pData = &sDirectMoveIcon;
	_SendMsg(msg);
}

void CDboEventGenerator::PrivateShopSelectNfy( RwUInt8 uiPrivateShopPos, RwUInt8 uiItemState ) 
{
	SDboEventPrivateShopSelectNfy sPrivateShopItemSelect;
	sPrivateShopItemSelect.uiPrivateShopPos = uiPrivateShopPos;
	sPrivateShopItemSelect.uiItemState = uiItemState;

	RWS::CMsg msg;
	msg.Id = g_EventPrivateShopSelectNfy;
	msg.pData = &sPrivateShopItemSelect;
	_SendMsg(msg);
}

void CDboEventGenerator::RegPrivateShopCartItem(RwUInt32 uiSerial, RwUInt32 uiPlace, RwUInt32 uiPos, RwUInt32 uiStackCount, RwUInt32 uiPrice, void* pData)
{
	SDboEventRegPrivateShopCartItem sRegPrivateShopCartItem;
	sRegPrivateShopCartItem.uiSerial	= uiSerial;
	sRegPrivateShopCartItem.uiPlace		= uiPlace;
	sRegPrivateShopCartItem.uiPos		= uiPos;
	sRegPrivateShopCartItem.uiStackCount= uiStackCount;
	sRegPrivateShopCartItem.uiPrice		= uiPrice;
	sRegPrivateShopCartItem.pData		= pData;

	RWS::CMsg msg;
	msg.Id = g_EventRegPrivateShopCartItem;
	msg.pData = &sRegPrivateShopCartItem;
	_SendMsg(msg);
}

void CDboEventGenerator::RegPrivateShopItem(RwUInt32 uiPlace, RwUInt32 uiIndex, RwUInt32 uiPos)
{
	SDboEventRegPrivateShopItem sRegPrivateShopItem;
	sRegPrivateShopItem.uiPlace	= uiPlace;
	sRegPrivateShopItem.uiIndex	= uiIndex;
	sRegPrivateShopItem.uiPos	= uiPos;

	RWS::CMsg msg;
	msg.Id = g_EventRegPrivateShopItem;
	msg.pData = &sRegPrivateShopItem;
	_SendMsg(msg);
}

void CDboEventGenerator::PrivateShopBusinessStart( RwUInt8 uiType, RwUInt32 uiShopOwner, WCHAR* pwcTargetName, sPRIVATESHOP_ITEM_DATA* pPrivateShopItemData, void* pGui ) 
{
	SDboEventPrivateShopBusinessStart sPrivateShopBusinessStart;
	sPrivateShopBusinessStart.uiType = uiType;
	sPrivateShopBusinessStart.uiShopOwner = uiShopOwner;
	swprintf_s(sPrivateShopBusinessStart.awcTargetName, NTL_MAX_SIZE_CHAR_NAME+1, L"%s", pwcTargetName);
	sPrivateShopBusinessStart.pPrivateShopItemData = pPrivateShopItemData;
	sPrivateShopBusinessStart.pGui = pGui;

	RWS::CMsg msg;
	msg.Id = g_EventPrivateShopBusinessStart;
	msg.pData = reinterpret_cast<void*>( &sPrivateShopBusinessStart );
	_SendMsg( msg );
}

void CDboEventGenerator::PrivateShopBusinessFluctuations( RwUInt8 uiType, sPRIVATESHOP_ITEM_DATA* pPrivateShopItemData, RwUInt32 uiStartPrice ) 
{
	SDboEventPrivateShopBusinessFluctuations sPrivateShopBusinessFluctuations;
	sPrivateShopBusinessFluctuations.uiType = uiType;
	sPrivateShopBusinessFluctuations.pPrivateShopItemData = pPrivateShopItemData;
	sPrivateShopBusinessFluctuations.uiStartPrice = uiStartPrice;

	RWS::CMsg msg;
	msg.Id = g_EventPrivateShopBusinessFluctuations;
	msg.pData = reinterpret_cast<void*>( &sPrivateShopBusinessFluctuations );
	_SendMsg( msg );
}

void CDboEventGenerator::PrivateShopBusinessCloseNfy( RwUInt32 uiShopOwner ) 
{
	SDboEventPrivateShopBusinessCloseWindowNfy sPrivateShopBusinessCloseWindow;
	sPrivateShopBusinessCloseWindow.uiShopOwner = uiShopOwner;

	RWS::CMsg msg;
	msg.Id = g_EventPrivateShopBusinessCloseWindow;
	msg.pData = reinterpret_cast<void*>( &sPrivateShopBusinessCloseWindow );
	_SendMsg( msg );
}


void CDboEventGenerator::BackboardEvent(RwUInt32 uiMessage, RwUInt8 byValue /* = 0 */,
										RwUInt8 byValue2 /* = 0 */, RwUInt8 byValue3 /* = 0 */)
{
	SDboEventBackboard sBackboard;
	sBackboard.uiMessage = uiMessage;
	sBackboard.byValue = byValue;
	sBackboard.byValue2 = byValue2;
	sBackboard.byValue3 = byValue3;

	RWS::CMsg msg;
	msg.Id = g_EventBackboard;
	msg.pData = &sBackboard;
	_SendMsg(msg);
}

void CDboEventGenerator::SummonPet( RwBool bCreate, RwUInt32 uiSerialID )
{
	SDboEventSummonPet sSummonPet;
	sSummonPet.bCreate = bCreate;
	sSummonPet.uiSerialID = uiSerialID;

	RWS::CMsg msg;
	msg.Id = g_EventSummonPet;
	msg.pData = &sSummonPet;
	_SendMsg(msg);
}

void CDboEventGenerator::OpenSendMail(const WCHAR* pwcTargetName)
{
	SDboEventOpenSendMail sOpenSendMail;
	wcscpy_s(sOpenSendMail.awcTargetName, NTL_MAX_SIZE_CHAR_NAME + 1, pwcTargetName);

	RWS::CMsg msg;
	msg.Id = g_EventOpenSendMail;
	msg.pData = &sOpenSendMail;
	_SendMsg(msg);
}

void CDboEventGenerator::ReadingMail(RwUInt32 uiMailID, RwBool bReading)
{
	SDboEventReadingMail sReadingMail;
	sReadingMail.uiMailID = uiMailID;
	sReadingMail.bReading = bReading;

	RWS::CMsg msg;
	msg.Id = g_EventReadingMail;
	msg.pData = &sReadingMail;
	_SendMsg(msg);
}

void CDboEventGenerator::MailSlotUpdate( RwUInt8 byType, RwUInt32 uiParam1, RwUInt32 uiParam2 )
{
	SDboEventMailSlotUpdate sData;
	sData.byType = byType;
	sData.uiParam1 = uiParam1;
	sData.uiParam2 = uiParam2;
	
	SEND_MSG( g_EventMailSlotUpdate, &sData )
}

void CDboEventGenerator::QuestMessage(RwUInt32 uiValue, RwUInt32 uiValue2)
{
	SDboEventQuestMessage sQuestMessage;
	sQuestMessage.uiValue			= uiValue;
	sQuestMessage.uiValue2			= uiValue2;

	RWS::CMsg msg;
	msg.Id = g_EventQuestMessage;
	msg.pData = reinterpret_cast<void*>(&sQuestMessage);
	_SendMsg(msg);
}

void CDboEventGenerator::QuestMessage(sQUEST_INFO* pQuestInfo, RwUInt32 uiEventGenType, RwUInt32 uiID)
{
	SDboEventQuestMessage2 sQuestMessage;
	sQuestMessage.pQuestInfo			= pQuestInfo;
	sQuestMessage.uiEventGenType		= uiEventGenType;
	sQuestMessage.uiID					= uiID;

	RWS::CMsg msg;
	msg.Id = g_EventQuestMessage2;
	msg.pData = reinterpret_cast<void*>(&sQuestMessage);
	_SendMsg(msg);
}

void CDboEventGenerator::RpBonusSetup( RwInt32 iXPos, RwInt32 iYPos, VOID* pData ) 
{
	SDboEventRpBonusSetup sRpBonus;
	sRpBonus.iXPos = iXPos;
	sRpBonus.iYPos = iYPos;
	sRpBonus.pData = pData;	

	RWS::CMsg msg;
	msg.Id = g_EventRpBonusSetup;
	msg.pData = reinterpret_cast<void*>( &sRpBonus );
	_SendMsg( msg );
}

/**
* \brief RpBonus 선택 UI를 오픈하라는 이벤트
* \param pData (CNtlSobSkillIcon*)
*/
void CDboEventGenerator::RpBonusSelect( VOID* pData ) 
{
	SEND_MSG( g_EventRpBonusSelect, pData);
}

void CDboEventGenerator::RpBonusAuto( RwUInt8 byRpBonusType ) 
{
	SDboEventRpBonusAuto sRpBonus;
	sRpBonus.byRpBonusType = byRpBonusType;

	RWS::CMsg msg;
	msg.Id = g_EventRpBonusAuto;
	msg.pData = reinterpret_cast<void*>( &sRpBonus );
	_SendMsg( msg );
}

void CDboEventGenerator::TargetStatusRefresh(VOID)
{
	RWS::CMsg msg;
	msg.Id = g_EventTargetStatusRefresh;
	msg.pData = NULL;
	_SendMsg( msg );
}

void CDboEventGenerator::RankBattleRankListRes( RwUInt16 wResultCode, RwUInt32 dwPage, RwUInt8 byCompareDay, RwUInt8 byRankInfoCount, RwUInt16 wRankInfo, void* sData ) 
{
	SDboEventRankBattleRankListRes sRankBattleRankListRes;
	sRankBattleRankListRes.wResultCode = wResultCode;
	sRankBattleRankListRes.dwPage = dwPage;
	sRankBattleRankListRes.byCompareDay = byCompareDay;
	sRankBattleRankListRes.byRankInfoCount = byRankInfoCount;
	sRankBattleRankListRes.wRankInfo = wRankInfo;
	sRankBattleRankListRes.sData = sData;

	RWS::CMsg msg;
	msg.Id = g_EventRankBattleRankListRes;
	msg.pData = reinterpret_cast<void*>( &sRankBattleRankListRes );
	_SendMsg( msg );
}

void CDboEventGenerator::RankBattleRankFindCharacterRes( RwUInt16 wResultCode, RwUInt32 dwPage, RwUInt8 byCompareDay, WCHAR* pwsCharName, RwUInt8 byRankInfoCount, RwUInt16 wRankInfo, void* sData ) 
{
	SDboEventRankBattleRankFindCharacterRes sRankBattleRankFindCharacterRes;
	sRankBattleRankFindCharacterRes.wResultCode = wResultCode;
	sRankBattleRankFindCharacterRes.dwPage = dwPage;
	sRankBattleRankFindCharacterRes.byCompareDay = byCompareDay;
	sRankBattleRankFindCharacterRes.pwsCharName = pwsCharName;
	sRankBattleRankFindCharacterRes.byRankInfoCount = byRankInfoCount;
	sRankBattleRankFindCharacterRes.wRankInfo = wRankInfo;
	sRankBattleRankFindCharacterRes.sData = sData;

	RWS::CMsg msg;
	msg.Id = g_EventRankBattleRankFindCharacterRes;
	msg.pData = reinterpret_cast<void*>( &sRankBattleRankFindCharacterRes );
	_SendMsg( msg );
}

void CDboEventGenerator::RankBattleRankCompareDayRes( RwUInt16 wResultCode, RwUInt32 dwPage, RwUInt8 byCompareDay, RwBool bIsFullList, RwUInt8 byInfoCount, RwUInt16 wRankInfo, RwUInt16 wCompareInfo, void* sData ) 
{
	SDboEventRankBattleRankCompareDayRes sRankBattleRankCompareDayRes;
	sRankBattleRankCompareDayRes.wResultCode = wResultCode;
	sRankBattleRankCompareDayRes.dwPage = dwPage;
	sRankBattleRankCompareDayRes.byCompareDay = byCompareDay;
	sRankBattleRankCompareDayRes.bIsFullList = bIsFullList;
	sRankBattleRankCompareDayRes.byInfoCount = byInfoCount;
	sRankBattleRankCompareDayRes.wRankInfo = wRankInfo;
	sRankBattleRankCompareDayRes.wCompareInfo = wCompareInfo;
	sRankBattleRankCompareDayRes.sData = sData;

	RWS::CMsg msg;
	msg.Id = g_EventRankBattleRankCompareDayRes;
	msg.pData = reinterpret_cast<void*>( &sRankBattleRankCompareDayRes );
	_SendMsg( msg );

}

void CDboEventGenerator::RankBoardDisable() 
{
	RWS::CMsg msg;
	msg.Id = g_EventRankBoardDisable;
	msg.pData = NULL;
	_SendMsg( msg );
}

void CDboEventGenerator::ZennyLootingEffect( RwUInt32 uiZenny )
{
	SDboEventZennyLootingEffect sData;
	sData.uiZenny = uiZenny;

	RWS::CMsg msg;
	msg.Id = g_EventZennyLootingEffect;
	msg.pData = &sData;
	_SendMsg( msg );
}

/**
* \brief 아이템이 생성되는 이펙트
* \param hSerial		(RwUInt32) 아이템의 핸들
* \param ucBagIdx		(RwUInt8) 가방의 번호
* \param ucSlotIdx		(RwUInt8) 가방 안의 위치
* \param bGambleUse		(RwBool) 겜블 효과
* \remarks RwBool 형인 bGambleUse를 추가하여 겜블 아이템 효과를 추가했습니다.
*/
void CDboEventGenerator::ItemCreatedEffect( RwUInt32 hSerial, RwUInt8 ucBagIdx, RwUInt8 ucSlotIdx )
{
	SDboEventItemCreatedEffect sData;
	sData.hSerial = hSerial;
	sData.ucBagIdx = ucBagIdx;
	sData.ucSlotIdx = ucSlotIdx;

	RWS::CMsg msg;
	msg.Id = g_EventItemCreatedEffect;
	msg.pData = &sData;
	_SendMsg( msg );
}

void CDboEventGenerator::SideDialogEvent(RwUInt8 byMessage, RwUInt32 uiSrcSDialog, RwUInt32 uiDestSDialog, RwUInt32 uiExData /* = 0 */)
{
	SDBoEventSideDialog sData;
	sData.byMessage		= byMessage;
	sData.uiSrcSDialog	= uiSrcSDialog;
	sData.uiDestSDialog	= uiDestSDialog;
	sData.uiExData		= uiExData;

	RWS::CMsg msg;
	msg.Id = g_EventSideDialog;
	msg.pData = &sData;
	_SendMsg( msg );
}

void CDboEventGenerator::TMQRecordListRes( RwUInt16 wResultCode, RwUInt32 tmqTblidx, RwUInt8 byDifficult, RwUInt8 byTeamCount, void* paData )
{
	SDboEventTMQRecordListRes sData;
	sData.wResultCode = wResultCode;
	sData.tmqTblidx = tmqTblidx;
	sData.byDifficult = byDifficult;
	sData.byTeamCount = byTeamCount;
	sData.paTeam = paData;

	SEND_MSG( g_EventTMQRecordListRes, &sData );
}

void CDboEventGenerator::TMQMemberListRes( RwUInt16 wResultCode, RwUInt32 tmqTblidx, RwUInt8 byDifficult, RwUInt8 byRank, void* pData )
{
	SDboEventTMQMemberListRes sData;
	sData.wResultCode = wResultCode;
	sData.tmqTblidx = tmqTblidx;
	sData.byDifficult = byDifficult;
	sData.byRank = byRank;
	sData.pTeam = pData;

	SEND_MSG( g_EventTMQMemberListRes, &sData );
}

void CDboEventGenerator::BudokaiNews( RwBool bShow, RwUInt8 byType /*= 0xFF*/ )
{
	SDboEventBudokaiNews sData;
	sData.bShow = bShow;
	sData.byType = byType;

	SEND_MSG( g_EventBudokaiNews, &sData );
}

void CDboEventGenerator::BudokaiPrizeWinnerName( BYTE byMatchType, WORD wWinner, WORD wSecondWinner, void* pData )
{
	SDboEventBudokaiPrizeWinnerName sData;
	
	sData.byMatchType = byMatchType;
	sData.wWinner = wWinner;
	sData.wSecondWinner = wSecondWinner;
	sData.pData = pData;

	SEND_MSG( g_EventBudokaiPrizeWinnerName, &sData );
}

void CDboEventGenerator::BudokaiTournamentIndividualList(RwUInt16 wEntryTeam_var, RwUInt8 byMatchDataCount, RwUInt16 wMatchData_var, sVARIABLE_DATA* pData)
{
	SDboEventBudokaiTournamentIndividualList sData;
	sData.wEntryTeam_var = wEntryTeam_var;
	sData.byMatchDataCount = byMatchDataCount;
	sData.wMatchData_var = wMatchData_var;
	sData.pData = pData;

	SEND_MSG( g_EventBudokaiTournamentIndividualList, &sData );
}

void CDboEventGenerator::BudokaiTournamentIndividualInfo(RwUInt16 wTeamType[2], sVARIABLE_DATA* pData)
{
	SDboEventBudokaiTournamentIndividualInfo sData;
	sData.wTeamType[MATCH_TEAM_TYPE_TEAM1] = wTeamType[MATCH_TEAM_TYPE_TEAM1];
	sData.wTeamType[MATCH_TEAM_TYPE_TEAM2] = wTeamType[MATCH_TEAM_TYPE_TEAM2];
	sData.pData = pData;

	SEND_MSG( g_EventBudokaiTournamentIndividualInfo, &sData );
}

void CDboEventGenerator::BudokaiTournamentTeamList(RwUInt16 wEntryTeam_var, RwUInt8 byMatchDataCount, RwUInt16 wMatchData_var, sVARIABLE_DATA* pData)
{
	SDboEventBudokaiTournamentTeamList sData;
	sData.wEntryTeam_var = wEntryTeam_var;
	sData.byMatchDataCount = byMatchDataCount;
	sData.wMatchData_var = wMatchData_var;
	sData.pData = pData;
	
	SEND_MSG( g_EventBudokaiTournamentTeamList, &sData );
}

void CDboEventGenerator::BudokaiTournamentTeamInfo(RwUInt16 wTeamType[2], sVARIABLE_DATA* pData)
{
	SDboEventBudokaiTournamentTeamInfo sData;
	sData.wTeamType[MATCH_TEAM_TYPE_TEAM1] = wTeamType[MATCH_TEAM_TYPE_TEAM1];
	sData.wTeamType[MATCH_TEAM_TYPE_TEAM2] = wTeamType[MATCH_TEAM_TYPE_TEAM2];
	sData.pData = pData;

	SEND_MSG( g_EventBudokaiTournamentTeamInfo, &sData );
}

void CDboEventGenerator::ScriptLinkNotify( RwUInt8 byType, RwUInt8 byAction, WCHAR* pwcValue )
{
	SDboEventScriptLinkNotify sData;
	sData.byType = byType;
	sData.byAction = byAction;
	::CopyMemory( sData.wszValue, pwcValue, sizeof(WCHAR) * DBO_MAX_LENGTH_FILE );

	SEND_MSG( g_EventScriptLinkNotify, &sData );
}

void CDboEventGenerator::DiceStart(RwUInt8 byWorkID, void* pExData /* = NULL */)
{
	SDboEventDiceStart sData;
	sData.byWorkID	= byWorkID;
	sData.pExData	= (void*)pExData;

	SEND_MSG( g_EventDiceStart, &sData );
}

void CDboEventGenerator::DiceResult(SERIAL_HANDLE hHandle, RwUInt8 byWorkID, RwUInt8 byResultNum)
{
	SDboEventDiceResult sData;
	sData.hHandle		= hHandle;
	sData.byWorkID		= byWorkID;
	sData.byResultNum	= byResultNum;

	SEND_MSG( g_EventDiceResult, &sData );
}

void CDboEventGenerator::DiceResultShow(SERIAL_HANDLE hHandle, RwUInt8 byWorkID, RwUInt8 byResultNum)
{
	SDboEventDiceResultShow sData;
	sData.hHandle		= hHandle;
	sData.byWorkID		= byWorkID;
	sData.byResultNum	= byResultNum;

	SEND_MSG( g_EventDiceResultShow, &sData );
}

void CDboEventGenerator::DiceRequestResulttoServer(RwUInt8 byWorkID)
{
	SDboEventDiceRequesttoServer sData;
	sData.byWorkID		= byWorkID;

	SEND_MSG( g_EventDiceRollReqtoServer, &sData );
}

void CDboEventGenerator::ShopGambleBuyRes( RwUInt16 wResultCode, RwUInt32 hHandle, RwUInt32 hItem)
{
	SDboEventShopGambleBuyRes sData;
	sData.wResultCode = wResultCode;
	sData.hHandle = hHandle;
	sData.hItem = hItem;

	SEND_MSG( g_EventShopGambleBuyRes, &sData );
}

void CDboEventGenerator::GambleUseEffect( RwUInt8 ucBagIdx, RwUInt8 ucSlotIdx, RwUInt32 hCreateSerial, RwChar* pcEffectFile )
{
	SDboEventGambleUseEffect sData;
	sData.ucBagIdx = ucBagIdx;
	sData.ucSlotIdx = ucSlotIdx;
	sData.hCreateSerial = hCreateSerial;
	sprintf_s( sData.szEffectFile, DBO_MAX_LENGTH_ITEM_ICON_NAME+1, "%s", pcEffectFile );
	
	SEND_MSG( g_EventGambleUseEffect, &sData );
}

void CDboEventGenerator::RegMailAttatchItem( SERIAL_HANDLE hHandle, RwUInt32 uiPlace, RwUInt32 uiIndex, RwUInt32 uiPos )
{
	SDboEventRegMailAttachItem sData;
	sData.hHandle = hHandle;
	sData.uiPlace = uiPlace;
	sData.uiIndex = uiIndex;
	sData.uiPos = uiPos;
	
	SEND_MSG( g_EventRegMailAttachItem, &sData );
}

void CDboEventGenerator::RegBindSealingItem(SERIAL_HANDLE hHandle, RwUInt32 uiPlace, RwUInt32 uiIndex, RwUInt32 uiPos)
{
	SDboEventRegMailAttachItem sData;
	sData.hHandle = hHandle;
	sData.uiPlace = uiPlace;
	sData.uiIndex = uiIndex;
	sData.uiPos = uiPos;

	SEND_MSG(g_EventRegBindSealingAttachItem, &sData);
}

void CDboEventGenerator::RegHoiPoiMixMaterialItem(SERIAL_HANDLE hHandle, RwUInt32 uiPlace, RwUInt32 uiIndex, RwUInt32 uiPos)
{
	SDboEventRegMailAttachItem sData;
	sData.hHandle = hHandle;
	sData.uiPlace = uiPlace;
	sData.uiIndex = uiIndex;
	sData.uiPos = uiPos;

	SEND_MSG(g_EventRegHoiPoiMixMaterialItem, &sData);
}

void CDboEventGenerator::RegCCBDCouponItem(SERIAL_HANDLE hHandle, RwUInt32 uiPlace, RwUInt32 uiIndex, RwUInt32 uiPos)
{
	SDboEventRegMailAttachItem sData;
	sData.hHandle = hHandle;
	sData.uiPlace = uiPlace;
	sData.uiIndex = uiIndex;
	sData.uiPos = uiPos;

	SEND_MSG(g_EventRegCCBDCouponItem, &sData);
}

void CDboEventGenerator::RegOptionReconstructItem(SERIAL_HANDLE hHandle, RwUInt32 uiPlace, RwUInt32 uiIndex, RwUInt32 uiPos)
{
	SDboEventRegMailAttachItem sData;
	sData.hHandle = hHandle;
	sData.uiPlace = uiPlace;
	sData.uiIndex = uiIndex;
	sData.uiPos = uiPos;

	SEND_MSG(g_EventRegOptionReconstructItem, &sData);
}

void CDboEventGenerator::EventOpenQuickTeleport(SERIAL_HANDLE hHandle, RwUInt32 uiPlace, RwUInt32 uiIndex, RwUInt32 uiPos)
{
	SDboEventRegMailAttachItem sData;
	sData.hHandle = hHandle;
	sData.uiPlace = uiPlace;
	sData.uiIndex = uiIndex;
	sData.uiPos = uiPos;

	SEND_MSG(g_EventOpenQuickTeleport, &sData);
}

void CDboEventGenerator::EventQuickTeleportLoad(BYTE byCount, sQUICK_TELEPORT_INFO* asData)
{
	SDboEventQuickTeleportLoad sData;
	sData.byCount = byCount;
	sData.asData = asData;

	SEND_MSG(g_EventQuickTeleportLoad, &sData);
}

void CDboEventGenerator::EventQuickTeleportUpdate(sQUICK_TELEPORT_INFO * asData)
{
	SDboEventQuickTeleportUpdate sData;
	sData.asData = asData;

	SEND_MSG(g_EventQuickTeleportUpdate, &sData);
}

void CDboEventGenerator::EventQuickTeleportDelete(BYTE bySlot)
{
	SDboEventQuickTeleportDeleteAndMove sData;
	sData.bySlot = bySlot;

	SEND_MSG(g_EventQuickTeleportDelete, &sData);
}

void CDboEventGenerator::EventQuickTeleportMove(BYTE bySlot)
{
	SDboEventQuickTeleportDeleteAndMove sData;
	sData.bySlot = bySlot;

	SEND_MSG(g_EventQuickTeleportMove, &sData);
}

void CDboEventGenerator::RegAuctionHouseItem(SERIAL_HANDLE hHandle, RwUInt32 uiPlace, RwUInt32 uiIndex, RwUInt32 uiPos)
{
	SDboEventRegMailAttachItem sData;
	sData.hHandle = hHandle;
	sData.uiPlace = uiPlace;
	sData.uiIndex = uiIndex;
	sData.uiPos = uiPos;

	SEND_MSG(g_EventRegAuctionHouseItem, &sData);
}

void CDboEventGenerator::PetitionEvent(RwUInt32 uiEventType)
{
	SDboEventPetition sData;
	sData.uiEventType = uiEventType;

	SEND_MSG( g_EventPetition, &sData );
}

void CDboEventGenerator::PetitionPostEvent(RwUInt32 uiEventType)
{
	SDboEventPetition sData;
	sData.uiEventType = uiEventType;

	RWS::CMsg msg;
	msg.Id = g_EventPetition;
	msg.pData = reinterpret_cast<void*>(&sData);	

	PostMsg(msg, reinterpret_cast<void*>(&sData), sizeof(SDboEventPetition));
}

void CDboEventGenerator::TeleportProposalNfy( SDboEventTeleportProposalNfy* pNotify )
{
	SEND_MSG( g_EventTeleportProposalNfy, pNotify );
}

void CDboEventGenerator::TeleportConfirmRes( SDboEventTeleportConfirmRes* pResult )
{
	SEND_MSG( g_EventTeleportConfirmRes, pResult);
}

void CDboEventGenerator::ObserverMenu(RwUInt8 byEventType, SERIAL_HANDLE hHandle /* = INVALID_SERIAL_ID */, RwUInt32 uiValue /* = 0xffffffff */, void* pExdata /* = NULL */)
{
	SDboEventObserverMenu sData;
	sData.byEventType	= byEventType;
	sData.hHandle		= hHandle;
	sData.uiValue		= uiValue;
	sData.pExdata		= pExdata;

	SEND_MSG( g_EventObserverMenu, &sData );
}

void CDboEventGenerator::ObserverMenuPost(RwUInt8 byEventType, SERIAL_HANDLE hHandle /* = INVALID_SERIAL_ID */, RwUInt32 uiValue /* = 0xffffffff */)
{
	SDboEventObserverMenu sData;
	sData.byEventType	= byEventType;
	sData.hHandle		= hHandle;
	sData.uiValue		= uiValue;

	RWS::CMsg msg;
	msg.Id = g_EventObserverMenu;
	msg.pData = reinterpret_cast<void*>(&sData);	

	PostMsg(msg, reinterpret_cast<void*>(&sData), sizeof(sData));
}

void CDboEventGenerator::RPCharge( RwBool bIncrease, RwBool bStop )
{
	SDboEventRPCharge sData;
	sData.bIncrease = bIncrease;
	sData.bStop = bStop;

	SEND_MSG( g_EventRPCharge, &sData );
}

void CDboEventGenerator::BattleCombatMode(bool bCombatMode)
{
	SDboEventBattleCombatMode sData;
	sData.bCombatMode = bCombatMode;

	SEND_MSG(g_EventBattleCombatMode, &sData);
}

void CDboEventGenerator::ItemIdentifyEffect( RwBool bHandle, SERIAL_HANDLE hHandle /*= INVALID_SERIAL_ID */
											, RwUInt8 byPlace /* = 0xFF */, RwUInt8 byPos /* = 0xFF */)
{
	SDboEventItemIdentifyEffect sData;
	sData.bHandle = bHandle;
	sData.hItemHandle = hHandle;
	sData.byPlace = byPlace;
	sData.byPos = byPos;

	SEND_MSG( g_EventItemIdentifyEffect, &sData );
}

void CDboEventGenerator::HoipoiMixItemMake( RwUInt16 wResultCode, TBLIDX itemTblidx, HOBJECT hObject, TBLIDX idxRecipe, SERIAL_HANDLE hItem )
{
	SDboEventHoipoiMixItemMakeRes sData;
	sData.wResultCode = wResultCode;
	sData.itemTblidx = itemTblidx;
	sData.hObject = hObject;
	sData.idxRecipeTbl = idxRecipe;
	sData.hItem = hItem;

	SEND_MSG( g_EventHoipoiMixItemMakeRes, &sData );
}

void CDboEventGenerator::HoipoiMixStartCreate( TBLIDX idxRecipeTbl )
{
	SDboEventHoipoiMixCreateStart sData;
	sData.idxRecipeTbl = idxRecipeTbl;

	SEND_MSG( g_EventHoipoiMixCreateStart, &sData );
}

void CDboEventGenerator::HintSideViewClosed()
{
	SEND_MSG( g_EventHintViewClosed, NULL );
}

void CDboEventGenerator::LoadingEvent( RwUInt8 byMessage )
{
	SDboEventLoading sData;
	sData.byMessage = byMessage;

	SEND_MSG( g_EventLoading, &sData );
}

void CDboEventGenerator::UpdateNetPy(NETP netPy, NETP dwAccumulationNetP, DWORD timeNextGainTime)
{
    SDboEventUpdateNetPy data;
    data.netPy = netPy;
    data.dwAccumlationNetPy = dwAccumulationNetP;
    data.timeNextGainTime = timeNextGainTime;

    SEND_MSG(g_EventUpdateNetPy, &data);
}

void CDboEventGenerator::NetMarbleMemberShipNfy() 
{
    SEND_MSG(g_EventNetMarbleMemberShipNfy, NULL);
}

void CDboEventGenerator::NetPyShopEvent( RwUInt8 byType, RwUInt32 uiSerial, 
										unsigned long ulPrice /*= 0*/, wchar_t* pcName /*= NULL*/, 
										int iPlace /*= 0*/, int iPosition/*= 0 */, 
										int iOverlapCount /*= 0*/ )
{
	SDboEventNetPyShopEvent sData;
	sData.byEventType = byType;
	sData.uiSerial = uiSerial;
	sData.ulPrice = ulPrice;
	sData.pcItemName = pcName;
	sData.nPlace = iPlace;
	sData.nPosition = iPosition;
	sData.nOverlapCount = iOverlapCount;

	SEND_MSG( g_EventNetPyShopEvent, &sData );
}

// CommercialExtend command Event
void CDboEventGenerator::StartProcessBuyDurItemNPC( RwUInt32 uiItemIdx, RwUInt32 uiNPCSerial, sSHOP_BUY_CART& ShopBuyCart )
{
	SDboEventCommercialExtendCommand sData;
	SDboEventCommercialExtendNPCData sExData;
	sData.eCommandType = eSTART_PROCESS_BUY_NPC_DURATION_ITEM;
	sData.pData = &sExData;
	sExData.uiItemIdx = uiItemIdx;
	sExData.uiNPCSerial = uiNPCSerial;
	sExData.ShopBuyCart = ShopBuyCart;

	SEND_MSG( g_EventCommercialExtendCommand, &sData );
}

void CDboEventGenerator::StartProcessBuyDurItemNetpy( RwUInt32 uiItemIdx, RwUInt32 uiMerchantTblidx, BYTE byMerchanSlotPos )
{
	SDboEventCommercialExtendCommand sData;
	SDboEventCommercialExtendNetpyData sExData;
	sData.eCommandType = eSTART_PROCESS_BUY_NETPY_DURATION_ITEM;
	sData.pData = &sExData;
	sExData.uiItemIdx = uiItemIdx;
	sExData.uiMerchantTblidx = uiMerchantTblidx;
	sExData.byMerchanSlotPos = byMerchanSlotPos;

	SEND_MSG( g_EventCommercialExtendCommand, &sData );
}

void CDboEventGenerator::StartProcessMoveDurItem( RwUInt32 uiItemIdx, RwUInt32 uiProductId )
{
	SDboEventCommercialExtendCommand sData;
	SDboEventCommercialExtendYardratData sExData;
	sData.eCommandType = eSTART_PROCESS_MOVE_DURATION_ITEM;
	sData.pData = &sExData;
	sExData.uiItemIdx = uiItemIdx;
	sExData.uiProductId = uiProductId;

	SEND_MSG( g_EventCommercialExtendCommand, &sData );
}

void CDboEventGenerator::SetDurItemOfCommercialExtend( RwUInt32 uiItemIdx )
{
	SDboEventCommercialExtendCommand sData;
	sData.eCommandType = eSET_DURATION_ITEM;
	sData.pData = &uiItemIdx;

	SEND_MSG( g_EventCommercialExtendCommand, &sData );
}

void CDboEventGenerator::SetDurExtendTargetItemOfCommercialExtend( void* pNtlSobItem )
{
	SDboEventCommercialExtendCommand sData;
	sData.eCommandType = eSET_DURATION_EXTEND_TARGET_ITEM;
	sData.pData = pNtlSobItem;

	SEND_MSG( g_EventCommercialExtendCommand, &sData );
}

void CDboEventGenerator::ChangeCommercialExtendmode( RwInt32 iMode )
{
	SDboEventCommercialExtendCommand sData;
	sData.eCommandType = eCHANGE_COMMERCIALEXTEND_MODE;
	sData.pData = &iMode;

	SEND_MSG( g_EventCommercialExtendCommand, &sData );
}

void CDboEventGenerator::OpenCommercialExtendGui(RwInt32 iMode, RwUInt32 uiItemIdx)
{
	SDboEventCommercialExtendCommand sData;
	SDboEventCommercialExtendOpenData sExData;
	sData.eCommandType = eOPEN_COMMERCIALEXTEND_DIALOG;
	sData.pData = &sExData;
	sExData.iMode = iMode;
	sExData.uiItemIdx = uiItemIdx;

	SEND_MSG( g_EventCommercialExtendCommand, &sData );
}


void CDboEventGenerator::HLShopEvent(BYTE byType, DWORD dwCash)
{
	SDboEventHLShopEvent sData;
	sData.byEventType = byType;
	sData.dwCash = dwCash;

	SEND_MSG(g_EventHLShopEvent, &sData);
}

void CDboEventGenerator::HLShopEventBuy(bool bConfirm, TBLIDX hlsItemIdx)
{
	SDboEventHLShopEventBuy sData;
	sData.bConfirm = bConfirm;
	sData.hlsItemIdx = hlsItemIdx;

	SEND_MSG(g_EventHLShopEventBuy, &sData);
}

void CDboEventGenerator::HLShopEventGift(bool bConfirm, TBLIDX hlsItemIdx, const WCHAR* wchPlayer)
{
	SDboEventHLShopEventGift sData;
	sData.bConfirm = bConfirm;
	sData.hlsItemIdx = hlsItemIdx;
	NTL_SAFE_WCSCPY(sData.wchPlayer, wchPlayer);

	SEND_MSG(g_EventHLShopEventGift, &sData);
}

void CDboEventGenerator::HLShopEventItemInfo(BYTE byCount, sCASHITEM_BRIEF * aInfo)
{
	SDboEventHLShopEventItemInfo sData;
	sData.byCount = byCount;
	sData.aInfo = aInfo;

	SEND_MSG(g_EventHLShopEventItemInfo, &sData);
}

void CDboEventGenerator::HLShopEventItemInfoRes()
{
	SEND_MSG(g_EventHLShopEventItemInfoRes, NULL);
}

void CDboEventGenerator::HLShopEventItemBuyRes(DWORD dwRemainingCash)
{
	SDboEventHLShopEventItemBuyRes sData;
	sData.dwCash = dwRemainingCash;

	SEND_MSG(g_EventHLShopEventItemBuyRes, &sData);
}

void CDboEventGenerator::HLShopEventItemGiftRes(DWORD dwRemainingCash)
{
	SDboEventHLShopEventItemGiftRes sData;
	sData.dwCash = dwRemainingCash;

	SEND_MSG(g_EventHLShopEventItemGiftRes, &sData);
}

void CDboEventGenerator::HLShopEventItemMoveRes(QWORD qwProductId)
{
	SDboEventHLShopEventItemMoveRes sData;
	sData.qwProductId = qwProductId;

	SEND_MSG(g_EventHLShopEventItemMoveRes, &sData);
}

void CDboEventGenerator::HLShopEventItemUseRes(QWORD qwProductId)
{
	SDboEventHLShopEventItemMoveRes sData;
	sData.qwProductId = qwProductId;

	SEND_MSG(g_EventHLShopEventItemUseRes, &sData);
}

void CDboEventGenerator::HLShopEventItemAddNfy(sCASHITEM_BRIEF * aInfo)
{
	SDboEventHLShopEventItemAddNfy sData;
	sData.aInfo = aInfo;

	SEND_MSG(g_EventHLShopEventItemAddNfy, &sData);
}

void CDboEventGenerator::HLShopEventItemDelNfy(QWORD qwProductId)
{
	SDboEventHLShopEventItemDelNfy sData;
	sData.qwProductId = qwProductId;

	SEND_MSG(g_EventHLShopEventItemDelNfy, &sData);
}

// open BagGui, ScouterBackgroundGui, ...
void CDboEventGenerator::OpenBagGui()
{
    SEND_MSG(g_EventOpenBagGui, NULL);
}

void CDboEventGenerator::OpenScouterBackgroundGui()
{
    SEND_MSG(g_EventOpenScouterBackgroundGui, NULL);
}

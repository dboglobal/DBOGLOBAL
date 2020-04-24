#include "precomp_dboclient.h"
#include "DboEventGenerator.h"
#include "NtlStage.h"
#include "StageManager.h"
#include "DisplayStringManager.h"
#include "MsgBoxGui.h"
#include "ChatGui.h"
#include "NtlResultCode.h"
#include "AlarmManager.h"
#include "DumpCommand.h"
#include "NtlTokenizer.h"
#include "NtlPLResourcePack.h"
#include "NtlSLLogic.h"


CAlarmManager::CAlarmManager(VOID)
{
	ResetRepeatCheck();
}
 
CAlarmManager* CAlarmManager::GetInstance(VOID)
{
	static CAlarmManager intance;
	return &intance;
}

VOID CAlarmManager::ResetRepeatCheck(VOID)
{
	m_RepeatMessageCheck.strLastChatMessageID = "";
	m_RepeatMessageCheck.nLastChatMessageType = -1;
	m_RepeatMessageCheck.nRepeatCount = 0;
}

VOID CAlarmManager::AlarmMessage(std::string string, RwBool bServerSync /* = FALSE */, sMsgBoxData* pExData /* = NULL */, RwReal fShowTime /* = 0.0f  */, RwInt32 nUknown/* = 0*/, RwInt32 nUknown2/* = 0*/)
{
	const WCHAR* szText = GetDisplayStringManager()->GetString(string, (CDisplayStringManager::EDispType)bServerSync );

	if( szText && wcslen( szText ) != 0 )
	{
		AlarmProcess( szText, string, bServerSync, pExData, fShowTime );	
	}
	else
	{
		DBO_WARNING_MESSAGE("Couldnt find text for key: " << string.c_str());
	}
	//else if (/*sub_44B700()*/) // always return 0 ...
	//{
	//	v9 = GetACP();
	//	MultiByteToWideChar(v9, 0, lpMultiByteStr, -1, &WideCharStr, 1024);
	//	sub_EFB2C0(a7, a4, (int)lpMultiByteStr, (int)&v12, 0x400u, (int)&off_119E2B8, a3, &WideCharStr);
	//	CDboEventGenerator::MsgBoxShow((int)lpMultiByteStr, (int)&v12, 0, a5, a4, a6, a7, 0);
	//}
}

VOID CAlarmManager::AlarmMessage( WCHAR* szText, RwUInt32 uType /* = ALARM_TYPE_CHAT_WARN */, char* strKey /*= NULL*/,
								  RwBool bPopup /* = TRUE */, RwBool bHasInput /*= FALSE*/, RwReal fShowTime /* = 0.0f */,
								  sMsgBoxData* pExData /* = NULL */, std::list<sMsgBoxCustomBtn>* plistCustomBtn /* = NULL  */)
{
	if( uType & ALARM_TYPE_CHAT_WARN )
	{
		RwInt32 nLength = wcslen( szText );
		CDboEventGenerator::SayMessage( CHAT_TYPE_SYSTEM, SENDERNAME_SYSTEM, (RwUInt16)nLength, szText );
	}

	if (uType & ALARM_TYPE_MSGBOX)
	{
		CDboEventGenerator::MsgBoxShow(strKey,  szText, bPopup, bHasInput, fShowTime, pExData, plistCustomBtn);
	}

	if (uType & ALARM_ACTION_NOTIFY)
	{
		CDboEventGenerator::NotifyMessage(SDboEventNotify::ACTION, szText);
	}

	if (uType & ALARM_TIMEFIXED_NOTIFY)
	{
		// TO DO ( WATCH void __stdcall sub_DB6CE0 )
	//	CDboEventGenerator::NotifyMessage(SDboEventNotify::ACTION, szText);
	}

	if (uType & ALARM_TYPE_LOG)
	{
		// TO DO (WATCH void __stdcall sub_DB6CE0 )
	//	CDboEventGenerator::NotifyMessage(SDboEventNotify::ACTION, szText);
	}
}

VOID CAlarmManager::FormattedAlarmMessage(std::string string, RwBool bServerSync, sMsgBoxData* pExData, ... )
{
	va_list args;
	va_start( args, pExData );
	vFormattedAlarmMessage(string, bServerSync, 0.0f, pExData, args );
	va_end( args );
}

VOID CAlarmManager::vFormattedAlarmMessage(std::string string, RwBool bServerSync, sMsgBoxData* pExData, va_list ArgList )
{
	vFormattedAlarmMessage(string, bServerSync, 0.0f, pExData, ArgList );
}

VOID CAlarmManager::FormattedAlarmMessage(std::string string, RwBool bServerSync, RwReal fShowTime, sMsgBoxData* pExData, ... )
{
	va_list args;
	va_start( args, pExData );
	vFormattedAlarmMessage(string, bServerSync, fShowTime, pExData, args );
	va_end( args );
}

VOID CAlarmManager::vFormattedAlarmMessage(std::string& string, RwBool bServerSync, RwReal fShowTime, sMsgBoxData* pExData, va_list ArgList )
{
	WCHAR formattedText[GUI_TEXT_BUFFER_SIZE];
	const WCHAR* szText = GetDisplayStringManager()->GetString(string, bServerSync );

	if( !szText || wcslen( szText ) == 0 )
	{
		if( Logic_IsUIDevInfoVisible() )
		{
			//swprintf_s( formattedText, GUI_TEXT_BUFFER_SIZE, L"Client %s(stringID) is not exist", string.c_str());

			//CDboEventGenerator::MsgBoxShow( formattedText, MBW_NONE, MBTF_OK );
		}

		return;
	}

	vswprintf_s( formattedText, GUI_TEXT_BUFFER_SIZE, szText, ArgList );
	
	AlarmProcess( formattedText, string, bServerSync, pExData, fShowTime );
}

VOID CAlarmManager::FormattedAlarmMessage( WCHAR* szText, ... )
{
	va_list args;
	va_start( args, szText );
	vFormattedAlarmMessage( szText, args );
	va_end( args );
}

VOID CAlarmManager::vFormattedAlarmMessage( WCHAR* szText, va_list ArgList )
{
	WCHAR formattedText[GUI_TEXT_BUFFER_SIZE];
	vswprintf_s( formattedText, GUI_TEXT_BUFFER_SIZE, szText, ArgList );

	RwInt32 nLength = wcslen( formattedText );
	CDboEventGenerator::SayMessage( CHAT_TYPE_SYSTEM, SENDERNAME_SYSTEM, (RwUInt16)nLength, formattedText );	
}

VOID CAlarmManager::UpdateAlarmMessage(std::string& string, RwBool bServerSync , sMsgBoxData* pExData )
{
	const WCHAR* szText = GetDisplayStringManager()->GetString(string, (CDisplayStringManager::EDispType)bServerSync );

	if( !szText || wcslen( szText ) == 0 )
	{
		

		return;		
	}		

	AlarmProcess( szText, string, bServerSync, pExData, 0.0f, TRUE );
}

VOID CAlarmManager::UpdateAlarmMessage(const WCHAR * text, std::string & strKey, RwBool bServerSync, sMsgBoxData * pExData)
{
	return VOID();
}

VOID CAlarmManager::UpdateFormattedAlarmMessage(std::string string, RwBool bServerSync, sMsgBoxData* pExData, ... )
{
	va_list args;
	va_start( args, pExData );
	UpdatevFormattedAlarmMessage(string, bServerSync, pExData, args );
	va_end( args );
}

VOID CAlarmManager::UpdatevFormattedAlarmMessage(std::string& string, RwBool bServerSync, sMsgBoxData* pExData, va_list ArgList )
{
	WCHAR formattedText[GUI_TEXT_BUFFER_SIZE];
	const WCHAR* szText = GetDisplayStringManager()->GetString(string, bServerSync );

	if( !szText || wcslen( szText ) == 0 )
	{
		return;
	}

	vswprintf_s( formattedText, GUI_TEXT_BUFFER_SIZE, szText, ArgList );

	AlarmProcess( formattedText, string, bServerSync, pExData, 0.0f, TRUE );
}

VOID CAlarmManager::AlarmProcess( const WCHAR* pAlarmMsg, std::string& strKey, RwBool bServerSync, sMsgBoxData* pExData, RwReal fShowTime, RwBool bMsgBoxUpdate /* = FALSE  */ )
{
	std::map<std::string,sALARMITEM>::iterator it;

	sALARMITEM sAlarmItem;

	if( bServerSync )
	{
		it = m_mapServerAlarm.find(strKey);
		if( it != m_mapServerAlarm.end() )
			sAlarmItem = (*it).second;
	}	
	else
	{
		it = m_mapClientAlarm.find(strKey);
		if( it != m_mapClientAlarm.end() )
			sAlarmItem = (*it).second;
	}

	if( bMsgBoxUpdate &&
		!( sAlarmItem.uType & ALARM_TYPE_MSGBOX || sAlarmItem.uType & ALARM_TYPE_LOBBY_MSGBOX ) )
		return;

	if( RepeatCheck( (CAlarmManager::eALARMTYPE)sAlarmItem.uType, strKey, bServerSync ) )
		return;

	if( fShowTime > 0.0f )
		sAlarmItem.fMsgBoxShowTime = fShowTime;

	if( sAlarmItem.uType & ALARM_TYPE_CHAT_WARN )
	{
		RwInt32 nLength = wcslen( pAlarmMsg );
		CDboEventGenerator::SayMessage( CHAT_TYPE_SYSTEM, SENDERNAME_SYSTEM, (RwUInt16)nLength, pAlarmMsg );
	}

	if (sAlarmItem.uType & ALARM_ACTION_NOTIFY)
	{
		CDboEventGenerator::NotifyMessage(SDboEventNotify::ACTION, pAlarmMsg);
	}

	if (sAlarmItem.uType & ALARM_TIMEFIXED_NOTIFY)
	{
		CDboEventGenerator::TimeFixedNotifyMessage(TRUE, strKey, pAlarmMsg);
	}

	if (sAlarmItem.uType & ALARM_TYPE_LOG)
	{

	}

	if( sAlarmItem.uType & ALARM_TYPE_CHAT_NOTICE )
	{
		RwInt32 nLength = wcslen( pAlarmMsg );
		CDboEventGenerator::SayMessage( CHAT_TYPE_SYSTEM, SENDERNAME_NOTICE, (RwUInt16)nLength, pAlarmMsg );
	}

	if( sAlarmItem.uType & ALARM_TYPE_MSGBOX )
	{
		CDboEventGenerator::MsgBoxShow(strKey.c_str(), pAlarmMsg, sAlarmItem.bMsgBoxPopup, sAlarmItem.bHasInput, sAlarmItem.fMsgBoxShowTime, pExData, &sAlarmItem.listCustomBtn, sAlarmItem.nUnknown4, bMsgBoxUpdate );
	}
}

RwBool CAlarmManager::RepeatCheck( eALARMTYPE eAlarmType, std::string& string, RwBool bServerSync )
{
	if( eAlarmType & ( ALARM_TYPE_LOBBY_MSGBOX | ALARM_TYPE_MSGBOX | ALARM_TIMEFIXED_NOTIFY | ALARM_ACTION_NOTIFY ) )
		return FALSE;

	if( m_RepeatMessageCheck.strLastChatMessageID == string && m_RepeatMessageCheck.nLastChatMessageType == bServerSync )
	{
		++m_RepeatMessageCheck.nRepeatCount;

		if( m_RepeatMessageCheck.nRepeatCount > 3 )
			return TRUE;		
		else
			return FALSE;
	}
	else
	{
		m_RepeatMessageCheck.nRepeatCount = 0;				

		return FALSE;
	}

	m_RepeatMessageCheck.nLastChatMessageType = bServerSync;
	m_RepeatMessageCheck.strLastChatMessageID = string;
}

//RwBool CAlarmManager::ParseData( const CDisplayParser& dispParser, std::string strFilename, RwBool bServer )
//{
	//CallTokenPack fnTokenPack = NULL;

	//if( GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_LANGUAGE )
	//	fnTokenPack = LanguagePackLoad;

	//CNtlTokenizer lexer( strFilename, fnTokenPack );

	//if( !lexer.IsSuccess() )
	//	return FALSE;

	//enum EParseState
	//{
	//	PARSE_STATE_ALARM_ID,
	//	PARSE_STATE_ALARM_TYPE,
	//	PARSE_STATE_MSGBOX_WORKID,
	//	PARSE_STATE_MSGBOX_STYLE,
	//	PARSE_STATE_MSGBOX_POPUP,
	//	PARSE_STATE_MSGBOX_TIME,		
	//	PARSE_STATE_MSGBOX_DUPLICATE,
	//	PARSE_STATE_MSGBOX_CUSTOM_BTN_STRING,
	//	PARSE_STATE_MSGBOX_CUSTOM_BTN_WORKID,
	//};

	//EParseState eState = PARSE_STATE_ALARM_ID;
	//const CDisplayParser::MapDefOption& mapDefOption = dispParser.GetDefOptions();
	//CDisplayParser::MapDefOption::const_iterator DefIt;
	//std::map<std::string,RwInt32>::const_iterator WorkIt;

	//std::string strStringID;
	//RwBool bReadData = TRUE;
	//RwBool bInsertData = FALSE;
	//sALARMITEM stAlarmItem;	
	//sMsgBoxCustomBtn stCustomBtn;

	//while( TRUE )
	//{
	//	const std::string& token = lexer.PeekNextToken();

	//	if( token.empty() )
	//		break;

	//	switch( eState )
	//	{
	//	case PARSE_STATE_ALARM_ID:
	//		{
	//			if( token == "=" )
	//			{
	//				if( strStringID.empty() )
	//				{
	//					ErrorMessage( strStringID.c_str(), "ALARM_ID is invalid" );						
	//					return FALSE;
	//				}

	//				eState = PARSE_STATE_ALARM_TYPE;
	//				bReadData = TRUE;
	//			}
	//			else if( token == "," || token == ";" )
	//			{
	//				ErrorMessage( strStringID.c_str(), "ALARM_ID has Invalid parameter" );
	//				return FALSE;
	//			}
	//			else 
	//			{
	//				strStringID = token;
	//			}
	//		}
	//		break;
	//	case PARSE_STATE_ALARM_TYPE:
	//		{
	//			if( token == "," )
	//			{
	//				eState = PARSE_STATE_MSGBOX_WORKID;
	//				bReadData = TRUE;
	//			}
	//			else if( token == ";" )
	//			{
	//				eState = PARSE_STATE_ALARM_ID;
	//				bReadData = TRUE;
	//				bInsertData = TRUE;
	//			}
	//			else if( token == "=" )
	//			{
	//				ErrorMessage( strStringID.c_str(), "ALARM_TYPE Invalid parameter" );
	//				return FALSE;
	//			}
	//			else
	//			{
	//				if( bReadData )
	//				{
	//					stAlarmItem.uType = (RwUInt32)( atoi( token.c_str() ) );
	//					bReadData = FALSE;

	//					if( stAlarmItem.uType & ALARM_TYPE_LOBBY_MSGBOX && stAlarmItem.uType & ALARM_TYPE_MSGBOX )
	//					{
	//						ErrorMessage( strStringID.c_str(), "Msgbox Type can't duplicated" );
	//						return FALSE;
	//					}
	//				}
	//				else
	//				{
	//					ErrorMessage( strStringID.c_str(), "ALARM_TYPE value input is twiced" );
	//					return FALSE;
	//				}
	//			}
	//		}
	//		break;
	//	case PARSE_STATE_MSGBOX_WORKID:
	//		{
	//			if( token == "," )
	//			{
	//				eState = PARSE_STATE_MSGBOX_STYLE;
	//				bReadData = TRUE;
	//			}
	//			else if( token == ";" )
	//			{
	//				eState = PARSE_STATE_ALARM_ID;
	//				bReadData = TRUE;
	//				bInsertData = TRUE;
	//			}
	//			else if( token == "=" )
	//			{
	//				ErrorMessage( strStringID.c_str(), "MSGBOX_WORKID Invalid parameter" );
	//				return FALSE;
	//			}
	//			else
	//			{
	//				if( bReadData )
	//				{
	//					std::map<std::string,RwInt32>::iterator iter;
	//					iter = m_mapWorkID.find( token );

	//					if( iter == m_mapWorkID.end() )
	//					{
	//						ErrorMessage( strStringID.c_str(), "MSGBOX_WORKID is Invalid" );
	//						return FALSE;
	//					}

	//					stAlarmItem.uWorkID = (RwUInt32)( (*iter).second );
	//					bReadData = FALSE;						
	//				}
	//				else
	//				{
	//					ErrorMessage( strStringID.c_str(), "MSGBOX_WORKID value input is twiced" );
	//					return FALSE;
	//				}
	//			}
	//		}
	//		break;
	//	case PARSE_STATE_MSGBOX_STYLE:
	//		{
	//			if( token == "," )
	//			{
	//				eState = PARSE_STATE_MSGBOX_POPUP;
	//				bReadData = TRUE;
	//			}
	//			else if( token == ";" )
	//			{
	//				eState = PARSE_STATE_ALARM_ID;
	//				bReadData = TRUE;
	//				bInsertData = TRUE;
	//			}
	//			else if( token == "=" )
	//			{
	//				ErrorMessage( strStringID.c_str(), "MSGBOX_STYLE Invalid parameter" );
	//				return FALSE;
	//			}
	//			else
	//			{
	//				if( bReadData )
	//				{
	//					RwUInt32 flagMsgBoxStyle = (RwUInt32)( atoi( token.c_str() ) );

	//					// 추후 NetMessageBox와 통합 예정.
	//					//if( stAlarmItem.uType & ALARM_TYPE_MSGBOX )
	//					//{
	//					//	if( uMsgBoxStyle == MBT_CANCEL )
	//					//	{
	//					//		ErrorMessage( wstrStringID.c_str(), "CAlarmManager::ParseData : LOBBY_MSGBOX doesn't support Cancel type" );
	//					//		return FALSE;
	//					//	}
	//					//}
	//					//else if( stAlarmItem.uType & ALARM_TYPE_LOBBY_MSGBOX )
	//					//{
	//					//	if( uMsgBoxStyle == MBT_INPUT )
	//					//	{
	//					//		ErrorMessage( wstrStringID.c_str(), "CAlarmManager::ParseData : LOBBY_MSGBOX doesn't support INPUT type" );
	//					//		return FALSE;
	//					//	}
	//					//}

	//					stAlarmItem.flagMsgBoxStyle = flagMsgBoxStyle;
	//					bReadData = FALSE;
	//				}
	//				else
	//				{
	//					ErrorMessage( strStringID.c_str(), "MSGBOX_STYLE value input is twiced" );
	//					return FALSE;
	//				}
	//			}				
	//		}
	//		break;
	//	case PARSE_STATE_MSGBOX_POPUP:
	//		{
	//			if( token == "," )
	//			{
	//				eState = PARSE_STATE_MSGBOX_TIME;
	//				bReadData = TRUE;
	//			}
	//			else if( token == ";" )
	//			{
	//				eState = PARSE_STATE_ALARM_ID;
	//				bReadData = TRUE;
	//				bInsertData = TRUE;
	//			}
	//			else if( token == "=" )
	//			{
	//				ErrorMessage( strStringID.c_str(), "MSGBOX_POPUP Invalid parameter" );
	//				return FALSE;
	//			}
	//			else
	//			{
	//				if( bReadData )
	//				{
	//					stAlarmItem.bMsgBoxPopup = (RwUInt32)( atoi( token.c_str() ) );
	//					bReadData = FALSE;
	//				}
	//				else
	//				{
	//					ErrorMessage( strStringID.c_str(), "MSGBOX_POPUP value input is twiced" );	
	//					return FALSE;
	//				}
	//			}
	//		}
	//		break;
	//	case PARSE_STATE_MSGBOX_TIME:
	//		{
	//			if( token == "," )
	//			{
	//				eState = PARSE_STATE_MSGBOX_DUPLICATE;
	//				bReadData = TRUE;					
	//			}
	//			else if( token == ";" )
	//			{
	//				eState = PARSE_STATE_ALARM_ID;
	//				bReadData = TRUE;
	//				bInsertData = TRUE;
	//			}
	//			else if( token == "=" )
	//			{
	//				ErrorMessage( strStringID.c_str(), "MSGBOX_TIME Invalid parameter" );
	//				return FALSE;
	//			}
	//			else
	//			{
	//				if( bReadData )
	//				{
	//					stAlarmItem.fMsgBoxShowTime = (RwReal)( atof( token.c_str() ) );
	//					bReadData = FALSE;
	//				}
	//				else
	//				{
	//					ErrorMessage( strStringID.c_str(), "MSGBOX_TIME value input is twiced" );							
	//					return FALSE;
	//				}
	//			}
	//		}
	//		break;		
	//	case PARSE_STATE_MSGBOX_DUPLICATE:
	//		{
	//			if( token == "," )
	//			{
	//				eState = PARSE_STATE_MSGBOX_CUSTOM_BTN_STRING;
	//				bReadData = TRUE;					
	//			}
	//			else if( token == ";" )
	//			{
	//				eState = PARSE_STATE_ALARM_ID;
	//				bReadData = TRUE;
	//				bInsertData = TRUE;
	//			}
	//			else if( token == "=" )
	//			{
	//				ErrorMessage( strStringID.c_str(), "MSGBOX_Duplicate Invalid parameter" );
	//				return FALSE;
	//			}
	//			else
	//			{
	//				if( bReadData )
	//				{
	//					stAlarmItem.bAcceptDuplicate = (RwUInt32)( atoi( token.c_str() ) );
	//					bReadData = FALSE;
	//				}
	//				else
	//				{
	//					ErrorMessage( strStringID.c_str(), "MSGBOX_Duplicate value input is twiced" );							
	//					return FALSE;
	//				}
	//			}
	//		}
	//		break;
	//	case PARSE_STATE_MSGBOX_CUSTOM_BTN_STRING:
	//		{
	//			if( token == "," )
	//			{
	//				eState = PARSE_STATE_MSGBOX_CUSTOM_BTN_WORKID;
	//				bReadData = TRUE;
	//			}
	//			else if( token == ";" )
	//			{
	//				stCustomBtn.uiWorkID = MBW_NONE;
	//				stAlarmItem.listCustomBtn.push_back( stCustomBtn );		
	//				stCustomBtn.Init();

	//				eState = PARSE_STATE_ALARM_ID;
	//				bReadData = TRUE;
	//				bInsertData = TRUE;				
	//			}
	//			else if( token == "=" )
	//			{
	//				ErrorMessage( strStringID.c_str(), "MSGBOX_CUSTOM_BTN_STRING Invalid parameter" );
	//				return FALSE;
	//			}
	//			else
	//			{
	//				if( bReadData )
	//				{
	//					DefIt = mapDefOption.find( token );
	//					if( DefIt != mapDefOption.end() )
	//					{
	//						stCustomBtn.uiStringID = (*DefIt).second;	
	//						bReadData = FALSE;
	//					}
	//					else
	//					{
	//						ErrorMessage( strStringID.c_str(), "MSGBOX_CUSTOM_BTN_STRING StringID is Invalid" );
	//						return FALSE;
	//					}						
	//				}
	//				else
	//				{
	//					ErrorMessage( strStringID.c_str(), "MSGBOX_CUSTOM_BTN_STRING value input is twiced" );							
	//					return FALSE;
	//				}
	//			}
	//		}
	//		break;		
	//	case PARSE_STATE_MSGBOX_CUSTOM_BTN_WORKID:
	//		{
	//			if( token == "," )
	//			{
	//				stAlarmItem.listCustomBtn.push_back( stCustomBtn );		
	//				stCustomBtn.Init();

	//				eState = PARSE_STATE_MSGBOX_CUSTOM_BTN_STRING;
	//				bReadData = TRUE;
	//			}
	//			else if( token == ";" )
	//			{
	//				stAlarmItem.listCustomBtn.push_back( stCustomBtn );		
	//				stCustomBtn.Init();

	//				eState = PARSE_STATE_ALARM_ID;
	//				bReadData = TRUE;
	//				bInsertData = TRUE;				
	//			}
	//			else if( token == "=" )
	//			{
	//				ErrorMessage( strStringID.c_str(), "MSGBOX_CUSTOM_BTN_WORKID Invalid parameter" );
	//				return FALSE;
	//			}
	//			else
	//			{
	//				if( bReadData )
	//				{
	//					WorkIt = m_mapWorkID.find( token );
	//					if( WorkIt != m_mapWorkID.end() )
	//					{
	//						stCustomBtn.uiWorkID = (*WorkIt).second;	
	//						bReadData = FALSE;
	//					}
	//					else
	//					{
	//						ErrorMessage( strStringID.c_str(), "MSGBOX_CUSTOM_BTN_WORKID StringID is Invalid" );
	//						return FALSE;
	//					}						
	//				}
	//				else
	//				{
	//					ErrorMessage( strStringID.c_str(), "MSGBOX_CUSTOM_BTN_WORKID value input is twiced" );							
	//					return FALSE;
	//				}
	//			}
	//		}
	//		break;
	//	}

	//	if( bInsertData )
	//	{
	//		DefIt = mapDefOption.find( strStringID );
	//		if( DefIt != mapDefOption.end() )
	//		{
	//			if( bServer )
	//				m_mapServerAlarm[(*DefIt).second] = stAlarmItem;
	//			else
	//				m_mapClientAlarm[(*DefIt).second] = stAlarmItem;
	//		}
	//		else
	//		{
	//			ErrorMessage( strStringID.c_str(), "Insert Phase Mismatched String ID" );							
	//			return FALSE;
	//		}

	//		bInsertData = FALSE;
	//		stAlarmItem.Init();
	//	}		
	//}

//	return TRUE;
//}

RwBool CAlarmManager::ParseData( const CDisplayParser& dispParser, std::string strFilename, RwBool bServer )
{
	CallTokenPack fnTokenPack = NULL;

	if( GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_LANGUAGE )
		fnTokenPack = LanguagePackLoad;

	CNtlTokenizer lexer( strFilename, fnTokenPack );

	if( !lexer.IsSuccess() )
		return FALSE;

	enum EParseState
	{
		PARSE_STATE_ALARM_ID,
		PARSE_STATE_ALARM_TYPE,

		/*PARSE_STATE_MSGBOX_WORKID,
		PARSE_STATE_MSGBOX_STYLE,
		PARSE_STATE_MSGBOX_POPUP,
		PARSE_STATE_MSGBOX_TIME,
		PARSE_STATE_MSGBOX_DUPLICATE,*/

		PARSE_STATE_POPUP,
		PARSE_STATE_HAS_INPUT,
		PARSE_STATE_UNKNOW3,
		PARSE_STATE_UNKNOW4,
		PARSE_STATE_UNKNOW5,


		PARSE_STATE_MSGBOX_BTN_HAS_BUTTON, // bool
		PARSE_STATE_MSGBOX_BTN_MSG_STRING, // string
		PARSE_STATE_MSGBOX_BTN_TOOLTIP_STRING, // string
	};

	/*
		1- PARSE_STATE_ALARM_ID (string)
		2- PARSE_STATE_ALARM_TYPE (byte)
		3,4,5,6,7- BYTE unknown

		8, 9, 10 = seems like 1 struct (bool {USE BUTTON?}, string {BUTTON MESSAGE}, string {BUTTON TOOL-TIP MESSAGE}) default: false, NOT_DEFINE, NO_TOOLTIP
		11, 12, 13 = same group
		14, 15, 16 = same group
		17, 18, 19 = same group
	*/

	EParseState eState = PARSE_STATE_ALARM_ID;
	const CDisplayParser::MapDataOption& mapDefOption = dispParser.GetDataOptions();
	CDisplayParser::MapDataOption::const_iterator DataIt;

	std::string strStringID;
	RwBool bReadData = TRUE;
	RwBool bInsertData = FALSE;
	BYTE byBtnCount = 0;

	sALARMITEM stAlarmItem;
	sMsgBoxCustomBtn stButtons;
	
	while( TRUE )
	{
		const std::string& token = lexer.PeekNextToken();

		if( token.empty() )
			break;

		switch( eState )
		{
			case PARSE_STATE_ALARM_ID:
			{
				if( token == "," )
				{
					if(strStringID.empty() )
					{
						ErrorMessage(strStringID.c_str(), "PARSE_STATE_ALARM_ID is invalid" );
						return FALSE;
					}

					eState = PARSE_STATE_ALARM_TYPE;
					bReadData = TRUE;
				}
				else 
				{
					if (bReadData)
					{
						strStringID = token;
						bReadData = FALSE;
					}
					else
					{
						ErrorMessage(strStringID.c_str(), "PARSE_STATE_ALARM_ID value input is twiced");
						return FALSE;
					}
				}
			}
			break;
			case PARSE_STATE_ALARM_TYPE:
			{
				if( token == "," )
				{
					eState = PARSE_STATE_POPUP;
					bReadData = TRUE;

					if (stAlarmItem.uType == 0)
					{
						ErrorMessage(strStringID.c_str(), "PARSE_STATE_ALARM_TYPE is invalid");
					}
				}
				else
				{
					if (bReadData)
					{
						stAlarmItem.uType = RwUInt32( atoi(token.c_str() ) );
						bReadData = FALSE;
					}
					else
					{
						ErrorMessage(strStringID.c_str(), "PARSE_STATE_ALARM_TYPE value input is twiced");
						return FALSE;
					}
				}
			}
			break;
			case PARSE_STATE_POPUP:
			{
				if( token == "," )
				{
					eState = PARSE_STATE_HAS_INPUT;
					bReadData = TRUE;
				}
				else
				{
					if (bReadData)
					{
						stAlarmItem.bMsgBoxPopup = atoi(token.c_str());
						bReadData = FALSE;
					}
					else
					{
						ErrorMessage(strStringID.c_str(), "PARSE_STATE_UNKNOW value input is twiced");
						return FALSE;
					}
				}
			}
			break;
			case PARSE_STATE_HAS_INPUT:
			{
				if (token == ",")
				{
					eState = PARSE_STATE_UNKNOW3;
					bReadData = TRUE;
				}
				else
				{
					if (bReadData)
					{
						stAlarmItem.bHasInput = (RwBool)(atoi(token.c_str()));
						bReadData = FALSE;
					}
					else
					{
						ErrorMessage(strStringID.c_str(), "PARSE_STATE_UNKNOW2 value input is twiced");
						return FALSE;
					}
				}
			}
			break;
			case PARSE_STATE_UNKNOW3:
			{
				if (token == ",")
				{
					eState = PARSE_STATE_UNKNOW4;
					bReadData = TRUE;
				}
				else
				{
					if (bReadData)
					{
						stAlarmItem.nUnknown3 = (RwUInt32)(atoi(token.c_str()));
						bReadData = FALSE;
					}
					else
					{
						ErrorMessage(strStringID.c_str(), "PARSE_STATE_UNKNOW3 value input is twiced");
						return FALSE;
					}
				}
			}
			break;
			case PARSE_STATE_UNKNOW4:
			{
				if (token == ",")
				{
					eState = PARSE_STATE_UNKNOW5;
					bReadData = TRUE;
				}
				else
				{
					if (bReadData)
					{
						stAlarmItem.nUnknown4 = (RwUInt32)(atoi(token.c_str()));
						bReadData = FALSE;
					}
					else
					{
						ErrorMessage(strStringID.c_str(), "PARSE_STATE_UNKNOW4 value input is twiced");
						return FALSE;
					}
				}
			}
			break;	
			case PARSE_STATE_UNKNOW5:
			{
				if (token == ",")
				{
					eState = PARSE_STATE_MSGBOX_BTN_HAS_BUTTON;
					bReadData = TRUE;
				}
				else
				{
					if (bReadData)
					{
						stAlarmItem.nUnknown5 = (RwUInt32)(atoi(token.c_str()));
						bReadData = FALSE;
					}
					else
					{
						ErrorMessage(strStringID.c_str(), "PARSE_STATE_UNKNOW5 value input is twiced");
						return FALSE;
					}
				}
			}
			break;
			case PARSE_STATE_MSGBOX_BTN_HAS_BUTTON:
			{
				if( token == "," )
				{
					eState = PARSE_STATE_MSGBOX_BTN_MSG_STRING;
					bReadData = TRUE;
				}
				else
				{
					if (bReadData)
					{
						stButtons.bHasButton = (RwReal)(atoi(token.c_str()));
						bReadData = FALSE;
					}
					else
					{
						ErrorMessage(strStringID.c_str(), "PARSE_STATE_MSGBOX_BTN_HAS_BUTTON value input is twiced");
						return FALSE;
					}
				}
			}
			break;		
			case PARSE_STATE_MSGBOX_BTN_MSG_STRING:
			{
				if( token == "," )
				{
					eState = PARSE_STATE_MSGBOX_BTN_TOOLTIP_STRING;
					bReadData = TRUE;
				}
				else
				{
					if( bReadData )
					{
						stButtons.strMessage = token;
						bReadData = FALSE;
					}
					else
					{
						ErrorMessage( strStringID.c_str(), "PARSE_STATE_MSGBOX_BTN_MSG_STRING value input is twiced" );							
						return FALSE;
					}
				}
			}
			break;

			case PARSE_STATE_MSGBOX_BTN_TOOLTIP_STRING:
			{
				if (token == ",")
				{
					eState = PARSE_STATE_MSGBOX_BTN_HAS_BUTTON;
					bReadData = TRUE;

					if (stButtons.bHasButton == TRUE)
						stAlarmItem.listCustomBtn.push_back(stButtons);

					stButtons.Init();
				}
				else if (token == ";")
				{
					if(stButtons.bHasButton == TRUE)
						stAlarmItem.listCustomBtn.push_back(stButtons);

					stButtons.Init();

					eState = PARSE_STATE_ALARM_ID;
					bReadData = TRUE;
					bInsertData = TRUE;
				}
				else
				{
					if (bReadData)
					{
						stButtons.strToolTip = token;
						bReadData = FALSE;
					}
					else
					{
						ErrorMessage(strStringID.c_str(), "PARSE_STATE_MSGBOX_BTN_TOOLTIP_STRING value input is twiced");
						return FALSE;
					}
				}
			}
			break;
		}

		if( bInsertData )
		{
			DataIt = mapDefOption.find(strStringID);
			if(DataIt != mapDefOption.end() )
			{
				//if(strStringID == "AUTH_USER_NOT_FOUND")
				//	DBO_WARNING_MESSAGE(strStringID.c_str() << " - HAS BUTTONS: " << (int)stAlarmItem.listCustomBtn.size());

				if( bServer )
					m_mapServerAlarm[(*DataIt).first] = stAlarmItem;
				else
					m_mapClientAlarm[(*DataIt).first] = stAlarmItem;
			}
			else
			{
			//	ErrorMessage(strStringID.c_str(), "Insert Phase Mismatched String ID" );
				//return FALSE;
			}

			bInsertData = FALSE;
			stAlarmItem.Init();
		}		
	}

	return TRUE;
}

VOID CAlarmManager::ErrorMessage( const RwChar* szID, const RwChar* szErrText )
{
	NTL_ASSERTFAIL( "[ " << szID << " ] " << szErrText );
}

VOID CAlarmManager::ErrorMessage( const WCHAR* wszID, const RwChar* szErrText )
{
	RwChar acBuffer[1024];
	::WideCharToMultiByte( GetACP(), 0, wszID, -1, acBuffer, 1024, NULL, NULL );
	NTL_ASSERTFAIL( "[" << acBuffer << " ] " << szErrText );
}
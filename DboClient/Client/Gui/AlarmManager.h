////////////////////////////////////////////////////////////////////////////////
// File: AlarmManager.h
// Desc:
//
// 2006.05.18 Peessi@hitel.net   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __ALARM_MANAGER_H__
#define __ALARM_MANAGER_H__

#include "DboDef.h"
#include "DisplayParser.h"

struct sALARMITEM
{
	sALARMITEM() 
	{
		Init();
	}

	sALARMITEM( RwInt32 flagAlarmType, RwReal fShowTime = 0.0f, RwBool bPopup = FALSE, RwBool hasinput = FALSE, int n3 = 0, int n4 = 0, int n5 = 0 )
	{
		uType = flagAlarmType;
		fMsgBoxShowTime = fShowTime;
		bMsgBoxPopup = bPopup;
		bHasInput = hasinput;
		nUnknown3 = n3;
		nUnknown4 = n4;
		nUnknown5 = n5;
	}

	VOID Init(VOID)
	{
		uType = 0x04;			// ALARM_TYPE_CHAT_WARN;
		/*uWorkID = MBW_NONE;
		flagMsgBoxStyle = MBTF_OK;
		bMsgBoxPopup = TRUE;
		bAcceptDuplicate = TRUE;*/
		fMsgBoxShowTime = 0.0f;
		bMsgBoxPopup = FALSE;
		bHasInput = FALSE;
		nUnknown3 = 0;
		nUnknown4 = 0;
		nUnknown5 = 0;
		listCustomBtn.clear();
	}

	RwUInt32	uType;
	/*
	RwUInt32	flagMsgBoxStyle;
	RwBool		bMsgBoxPopup;
	RwBool		bAcceptDuplicate;*/
	RwReal		fMsgBoxShowTime;
	RwBool		bMsgBoxPopup;
	RwBool		bHasInput;
	int			nUnknown3;
	int			nUnknown4;
	int			nUnknown5;
	std::list<sMsgBoxCustomBtn>	listCustomBtn;
};

class CAlarmManager
{
public:
//! Enumuration
	enum eALARMTYPE { ALARM_TYPE_MSGBOX			= 0x01, 
					  ALARM_TYPE_LOBBY_MSGBOX	= 0x02,
				      ALARM_TYPE_CHAT_WARN		= 0x04,
				      ALARM_TIMEFIXED_NOTIFY	= 0x08,
					  ALARM_ACTION_NOTIFY		= 0x10,
					  ALARM_TYPE_CHAT_NOTICE	= 0x20,
					  ALARM_TYPE_LOG			= 0x40,
					  ALARM_TYPE_CONSOLE		= 0x80,					  
					};

	typedef struct _stREPEATMESSAGECHECK
	{
		RwInt32 nLastChatMessageType;
		std::string	strLastChatMessageID;
		RwInt32 nRepeatCount;
	}stREPEATMESSAGECHECK;
	
//! Operation
	VOID	AlarmMessage( std::string string, RwBool bServerSync = FALSE, sMsgBoxData* pExData = NULL, RwReal fShowTime = 0.0f, RwInt32 nUknown = 0, RwInt32 nUknown2 = 0);
	VOID	AlarmMessage(WCHAR* szText, RwUInt32 uType = ALARM_TYPE_CHAT_WARN, char* strKey = NULL,
		RwBool bPopup = TRUE, RwBool bHasInput = FALSE, RwReal fShowTime = 0.0f,
		sMsgBoxData* pExData = NULL, std::list<sMsgBoxCustomBtn>* plistCustomBtn = NULL);
	VOID	FormattedAlarmMessage(std::string string, RwBool bServerSync, sMsgBoxData* pExData, ... );
	VOID	vFormattedAlarmMessage(std::string string, RwBool bServerSync, sMsgBoxData* pExData, va_list ArgList );
	VOID	FormattedAlarmMessage(std::string string, RwBool bServerSync, RwReal fShowTime, sMsgBoxData* pExData, ... );
	VOID	vFormattedAlarmMessage(std::string& string, RwBool bServerSync, RwReal fShowTime, sMsgBoxData* pExData, va_list ArgList );
	VOID	FormattedAlarmMessage( WCHAR* szText, ... ); 
	VOID	vFormattedAlarmMessage( WCHAR* szText, va_list ArgList );

	/// MsgBox인 경우 Msg 및 Data의 교체만 이루어짐. 시간속성 사용 불가. 
	VOID	UpdateAlarmMessage(std::string& string, RwBool bServerSync /* = FALSE */, sMsgBoxData* pExData /* = NULL */ );
	VOID	UpdateAlarmMessage(const WCHAR* text, std::string& strKey, RwBool bServerSync /* = FALSE */, sMsgBoxData* pExData /* = NULL */);
	VOID	UpdateFormattedAlarmMessage(std::string string, RwBool bServerSync, sMsgBoxData* pExData, ... );
	VOID	UpdatevFormattedAlarmMessage(std::string& string, RwBool bServerSync, sMsgBoxData* pExData, va_list ArgList );
	///

	VOID	ResetRepeatCheck( VOID );	

	RwBool	ParseData( const CDisplayParser& dispParser, std::string strFilename, RwBool bServer );
		
//! Access
	static CAlarmManager* GetInstance(VOID);

private:
//! Implementation
	VOID	AlarmProcess(const WCHAR* pAlarmMsg, std::string& strKey, RwBool bServerSync, sMsgBoxData* pExData, RwReal fShowTime, RwBool bMsgBoxUpdate = FALSE );
	VOID	SetServerHardCode(VOID);
	VOID	SetClientHardCode(VOID);
	RwBool	RepeatCheck( eALARMTYPE eAlarmType, std::string& string, RwBool bServerSync );
	VOID	ErrorMessage( const RwChar* szID, const RwChar* szErrText );
	VOID	ErrorMessage( const WCHAR* wszID, const RwChar* szErrText );

//! Variable
	std::map<std::string,sALARMITEM> m_mapServerAlarm;
	std::map<std::string,sALARMITEM> m_mapClientAlarm;	
	stREPEATMESSAGECHECK		 m_RepeatMessageCheck;

//! Prevented Constructor
	CAlarmManager(VOID);
};

static CAlarmManager* GetAlarmManager(VOID)
{
	return CAlarmManager::GetInstance();
};

#endif//__ALARM_MANAGER_H__
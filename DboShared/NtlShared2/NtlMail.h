//***********************************************************************************
//
//	File		:	NtlMail.h
//
//	Begin		:	2007-08-6
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	john@ntl-inc.com
//
//	Desc		:	
//
//***********************************************************************************

#pragma once
#include "NtlItem.h"
#include "NtlSharedType.h"

const BYTE		MAIL_SEND_REQUIRED_LEVEL = 29;		// Level required to send mail
const DWORD		NTL_MAX_BASIC_MAIL_SEND_ZENNY	= 10;		// Commission
const DWORD		NTL_MAX_ATTACH_MAIL_SEND_ZENNY	= 20;		// Commission
const DWORD		NTL_MAX_MAIL_SEND_ZENNY_AMOUNTS	= 100000000;	// 첨부 최대가능 금액
const DWORD		NTL_MAX_MAIL_SLOT_COUNT	= 30;				// 최대 메일 슬롯의 개수
const DWORD		NTL_MAIL_RELOAD_TIME    = 1 * 60 * 1000;    // DB 내용과 동기화 시간 조정 필요
const DWORD		NTL_MAIL_DISTANCE_CHECK_TIME = 3 * 1000;	// 메일박스와의 거리 체크 시간 
const BYTE		NTL_DEFAULT_MAIL_KEEPING_PERIOD = 10;		// 기본 메일 유지 기간. 일(day) 단위
const DWORD		NTL_MAX_COUNT_OF_MULTI_DEL		= 6;		// 멀티델가능 최대수
const BYTE		NTL_MAX_COUNT_MULTI_MAIL		= 50;		// GM-TOOL 멀티메일 최대치
const BYTE		NTL_MAX_LOAD_MAIL_PACKET		= 8;
//-----------------------------------------------------------------------------------
// 메일 타입 
//-----------------------------------------------------------------------------------
enum eMAIL_TYPE
{
	eMAIL_TYPE_NONE,			// When using error
	eMAIL_TYPE_BASIC,			// TEXT ONLY
	eMAIL_TYPE_ITEM,			// ITEM
	eMAIL_TYPE_ZENNY,			// ZENNY
	eMAIL_TYPE_ITEM_ZENNY,		// ITEM + ZENNY
	eMAIL_TYPE_ITEM_ZENNY_REQ,  // ITEM + ZENNY REQUEST
	eMAIL_TYPE_ZENNY_REQ,		// ZENNY REQUEST
	eMAIL_COUNT,				// GMTool에서 eMAIL_TYPE에 대한 스트링을 사용한다. 그래서 Count가 필요함[10/8/2007 SGpro]
};

//-----------------------------------------------------------------------------------
// sender 타입 
//-----------------------------------------------------------------------------------
enum eMAIL_SENDER_TYPE
{
	eMAIL_SENDER_TYPE_BASIC,		// 일반유저
	eMAIL_SENDER_TYPE_REPLY,		// 답장메일
	eMAIL_SENDER_TYPE_RETURN,		// 리턴메일
	eMAIL_SENDER_TYPE_SYSTEM,		// System
	eMAIL_SENDER_TYPE_GM,			// GM
	eMAIL_SENDER_TYPE_QUEST,

};

struct sMAIL_SLOT
{
	MAILID aMailID[NTL_MAX_MAIL_SLOT_COUNT];
};

struct sMAIL_BRIEF
{
	BYTE	byMailType;
	DWORD	dwZenny;
	bool	bIsRead;
	bool	bIsAccept;
	bool	bIsLock;
	BYTE	bySenderType;
	DBOTIME	endTime;
	DWORD	dwHandle;
};

struct sMAIL_DATA
{
	MAILID				mailID;			// 메일 아이디
	bool 				bIsRead;
	bool 				bIsAccept;
	bool 				bIsLock;
	BYTE 				byMailType;		// eMAIL_TYPE
	BYTE 				bySenderType;	// eMAIL_SENDER_TYPE
	DWORD				dwZenny;		// Req or Give Zenny
	BYTE				byExpired;		// 유효만료날짜 최대 10일
	sITEM_DATA			sItemData;		// 아이템내용	
	sDBO_TIME			tCreateTime;	// 보낸날짜
	WCHAR				wszFromName[NTL_MAX_SIZE_CHAR_NAME + 1];		// 보낸 캐릭 이름
	BYTE				byTextSize;
	WCHAR				wszText[NTL_MAX_LENGTH_OF_MAIL_MESSAGE + 1];	// 메일내용
	DBOTIME				endTime;
	DWORD				dwHandle;
};

struct sMAIL_GM_DATA
{
	MAILID				mailID;			// 메일 아이디
	bool 				bIsRead;
	bool 				bIsAccept;
	bool 				bIsLock;
	BYTE 				byMailType;		// eMAIL_TYPE
	BYTE 				bySenderType;	// eMAIL_SENDER_TYPE
	DWORD				dwZenny;		// Req or Give Zenny
	BYTE				byExpired;		// 유효만료날짜 최대 10일
	sITEM_DATA			sItemData;		// 아이템내용	
	sDBO_TIME			tCreateTime;	// 보낸날짜
	CHARACTERID			FromCharID;
	WCHAR				wszFromName[NTL_MAX_SIZE_CHAR_NAME + 1];		// 보낸 캐릭 이름
	BYTE				byTextSize;
	WCHAR				wszText[NTL_MAX_LENGTH_OF_MAIL_MESSAGE + 1];	// 메일내용
	bool				bIsTemp;	// tempMail ? True
};

struct sMAIL_PROFILE
{
	MAILID				mailID;			// 메일 아이디
	bool 				bIsRead;
	bool 				bIsAccept;
	bool 				bIsLock;
	BYTE 				byMailType;		// eMAIL_TYPE
	BYTE 				bySenderType;	// eMAIL_SENDER_TYPE
	DWORD				dwZenny;		// Req or Give Zenny
	BYTE				byExpired;		// 유효만료날짜 최대 10일
	sITEM_PROFILE		sItemProfile;	// 아이템내용	
	sDBO_TIME			tCreateTime;	// 보낸날짜
	WCHAR				wszFromName[NTL_MAX_SIZE_CHAR_NAME + 1];		// 보낸 캐릭 이름
	WCHAR				wszText[NTL_MAX_LENGTH_OF_MAIL_MESSAGE + 1];	// 메일내용
	DBOTIME				endTime;
};
struct sMAIL_NEW_PROFILE
{
	MAILID				mailID;			// 메일 아이디
	bool 				bIsRead;
	bool 				bIsAccept;
	bool 				bIsLock;
	BYTE 				byMailType;		// eMAIL_TYPE
	BYTE 				bySenderType;	// eMAIL_SENDER_TYPE
	DWORD				dwZenny;		// Req or Give Zenny
	BYTE				byExpired;		// 유효만료날짜 최대 10일
	sITEM_PROFILE		sItemProfile;	// 아이템내용	
	sDBO_TIME			tCreateTime;	// 보낸날짜
	WCHAR				wszFromName[NTL_MAX_SIZE_CHAR_NAME + 1];		// 보낸 캐릭 이름
	DBOTIME				endTime;
};

struct sMAIL_NEW_GM_PROFILE
{
	MAILID				mailID;			// 메일 아이디
	bool 				bIsRead;
	bool 				bIsAccept;
	bool 				bIsLock;
	BYTE 				byMailType;		// eMAIL_TYPE
	BYTE 				bySenderType;	// eMAIL_SENDER_TYPE
	DWORD				dwZenny;		// Req or Give Zenny
	BYTE				byExpired;		// 유효만료날짜 최대 10일
	sITEM_PROFILE		sItemProfile;	// 아이템내용	
	sDBO_TIME			tCreateTime;	// 보낸날짜
	CHARACTERID			FromCharID;
	WCHAR				wszFromName[NTL_MAX_SIZE_CHAR_NAME + 1];		// 보낸 캐릭 이름
	bool				bIsTemp;	// tempMail ? True
};

struct sMAIL_NEW_BREIF
{
	BYTE				byMailCount; // Total
	BYTE				byUnReadManager;
	BYTE				byUnReadNormal;
};
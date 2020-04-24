//***********************************************************************************
//
//	File		:	NtlHlsItem.h
//
//	Begin		:	2009-8-14
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	chung,DooSup ( john@ntl-inc.com )
//
//	Desc		: HLS는 호이포이 럭셔리 샵의 약자로 Cash를 갖고 구매하는 상점을 뜻한다.	
//
//***********************************************************************************
#pragma once
#include "NtlSharedType.h"


//-----------------------------------------------------------------------------------
// 캐쉬 아이템 타입
//-----------------------------------------------------------------------------------
enum eCASHITEM_TYPE
{
	CASHITEM_TYPE_ITEM_CREATE,			// Go to the capsule kit
	CASHITEM_TYPE_PACKAGE,				// Packages
	CASHITEM_TYPE_GMT,					// Reset time increase the number of gaming enthusiasts
	CASHITEM_TYPE_ACCOUNT_USE,			// Account Warehouse duration Items
	CASHITEM_TYPE_EXP_ZENNY,			// Updating the experience acquired zenny period

	CASHITEM_TYPE_UNKNOWN,				// 
	CASHITEM_TYPE_UNKNOWN2,				// 
	CASHITEM_TYPE_WAGUCOIN,				// 

	CASHITEM_TYPE_COUNT,

	CASHITEM_TYPE_INVALID = 0xff,
};

struct sCASHITEM_BRIEF
{
	QWORD			qwProductId;
	TBLIDX			HLSitemTblidx; // cash item table index
	BYTE			byStackCount;
	sDBO_TIME		tRegTime;
	WCHAR			wchSenderName[NTL_MAX_SIZE_CHAR_NAME + 1];
};

struct sCASHITEM : public sCASHITEM_BRIEF
{
	TBLIDX		itemTblidx; // item table index
};

//-----------------------------------------------------------------------------------
// Cash items Displacement Function Type Flag
//----------------------------------------------------------------------------------
enum eHLS_TAB_TYPE
{
	HLS_TAB_TYPE_NORMAL,
	HLS_TAB_TYPE_ALL,
	HLS_TAB_TYPE_NEW,
	HLS_TAB_TYPE_HOT,
	HLS_TAB_TYPE_LIMITED,

	HLS_TAB_TYPE_COUNT,

	HLS_TAB_TYPE_FIRST = HLS_TAB_TYPE_NORMAL,
	HLS_TAB_TYPE_LAST = HLS_TAB_TYPE_COUNT - 1,
};

enum eHLS_SELL_TYPE
{
	HLS_SELL_TYPE_NORMAL,
	HLS_SELL_TYPE_PERIOD,	// 기간
	HLS_SELL_TYPE_TOTAL_COUNT,	// 개수
	
	HLS_SELL_TYPE_COUNT,

	HLS_SELL_TYPE_FIRST = HLS_SELL_TYPE_NORMAL,
	HLS_SELL_TYPE_LAST = HLS_SELL_TYPE_COUNT - 1,
};

enum eHLS_DISPLAY_ITEM_FUNC
{
	HLS_DISPLAY_ITEM_FUNC_NORMAL,
	HLS_DISPLAY_ITEM_FUNC_NEW,
	HLS_DISPLAY_ITEM_FUNC_HOT,
	HLS_DISPLAY_ITEM_FUNC_LIMITED,
};

enum eHLS_DISPLAY_ITEM_FUNC_BIT_FLAG
{
	HLS_DISPLAY_ITEM_FUNC_BIT_FLAG_NORMAL	= 0x01 << HLS_DISPLAY_ITEM_FUNC_NORMAL,
	HLS_DISPLAY_ITEM_FUNC_BIT_FLAG_NEW		= 0x01 << HLS_DISPLAY_ITEM_FUNC_NEW,
	HLS_DISPLAY_ITEM_FUNC_BIT_FLAG_HOT		= 0x01 << HLS_DISPLAY_ITEM_FUNC_HOT,
	HLS_DISPLAY_ITEM_FUNC_BIT_FLAG_LIMITED  = 0x01 << HLS_DISPLAY_ITEM_FUNC_LIMITED,
};

enum eHLS_QUICK_LINK
{
	HLS_QUICK_LINK_NORMAL,
	HLS_QUICK_LINK_ACCOUNT_STORE,
	HLS_QUICK_LINK_HOIPOI_STONE_CORE,
	HLS_QUICK_LINK_COUNT,
	HLS_QUICK_LINK_FIRST = HLS_QUICK_LINK_NORMAL,
	HLS_QUICK_LINK_LAST = HLS_QUICK_LINK_COUNT - 1,
};

const DWORD		NTL_CASHITEM_RELOAD_TIME		= 60000;    // DB synchronization information and time adjustment needed
const DWORD		NTL_MAX_CASHITEM_COUNT			= 1000;		// Display or loading can not limit the maximum cache item // it Must once limited to loading
const DWORD		DBO_MAX_COUNT_HLSITEM_SUB_ITEM	= 5;

//new
const DWORD		DBO_MAX_CASHITEM_COUNT_IN_PACKET_SERVER = 50;
const DWORD		DBO_MAX_CASHITEM_COUNT_IN_PACKET_CLIENT = 30;




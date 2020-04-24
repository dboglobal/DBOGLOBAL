//***********************************************************************************
//
//	File		:	NtlMarket.h
//
//	Begin		:	23-sep-2018
//
//	Copyright	:	¨Ï DBO GLOBAL
//
//	Author		:	Daneos
//
//	Desc		:	
//
//***********************************************************************************

#pragma once


const int	DBO_MAX_MARKET_ITEM_COUNT_PER_PAGE = 10;

//-----------------------------------------------------------------------------------
//	AUCTION HOUSE PAGE
//-----------------------------------------------------------------------------------
enum eAUCTION_PAGE_TYPE
{
	eAUCTION_TAB_TYPE_BUY,
	eAUCTION_TAB_TYPE_SELL,

	eAUCTION_TAB_TYPE_COUNT
};

//-----------------------------------------------------------------------------------
//	AUCTION HOUSE TAB
//-----------------------------------------------------------------------------------
enum eAUCTION_TAB_TYPE
{
	eAUCTION_TAB_TYPE_NONE = 0x0,
	eAUCTION_TAB_TYPE_WEAPON,
	eAUCTION_TAB_TYPE_SUB_WEAPON,
	eAUCTION_TAB_TYPE_ARMOR,
	eAUCTION_TAB_TYPE_ACCESSORY,
	eAUCTION_TAB_TYPE_SCOUTER,
	eAUCTION_TAB_TYPE_RESTORE,
	eAUCTION_TAB_TYPE_RECIPE,
	eAUCTION_TAB_TYPE_OTHER,

	eAUCTION_TAB_TYPE_ALL = 0xFF,
};

//-----------------------------------------------------------------------------------
//	AUCTION HOUSE SORTING
//-----------------------------------------------------------------------------------
enum eAUCTION_SORT_TYPE
{
	eAUCTION_SORT_TYPE_BEGIN = 0x0,
	eAUCTION_SORT_TYPE_RANK_ASC,
	eAUCTION_SORT_TYPE_NAME_DESC,
	eAUCTION_SORT_TYPE_PRICE_DESC,
	eAUCTION_SORT_TYPE_LEVEL_DESC,
	eAUCTION_SORT_TYPE_RANK_DESC,
	eAUCTION_SORT_TYPE_NAME_ASC,
	eAUCTION_SORT_TYPE_PRICE_ASC,
	eAUCTION_SORT_TYPE_LEVEL_ASC,
	eAUCTION_SORT_TYPE_TIMELEFT_ASC,
	eAUCTION_SORT_TYPE_TIMELEFT_DESC,

	eAUCTION_SORT_TYPE_NONE = 0xff
};

//-----------------------------------------------------------------------------------
// DURATION IN HOURS INSIDE CASH SHOP
//-----------------------------------------------------------------------------------
enum eAUCTION_ITEM_SALE_DURATION
{
	eAUCTION_ITEM_SALE_DURATION_ONE_DAY = 24,
//	eAUCTION_ITEM_SALE_DURATION_TWO_DAYS = 48,
//	eAUCTION_ITEM_SALE_DURATION_ONE_WEEK = 168,
};
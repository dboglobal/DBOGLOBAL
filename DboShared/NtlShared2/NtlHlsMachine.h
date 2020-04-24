#pragma once

#include "NtlSharedDef.h"
#include "NtlSharedType.h"



#pragma pack(1)

const DWORD		DBO_MAX_HLS_SLOT_MACHINE_DATA_COUNT_IN_PACKET_SERVER = 81;
const DWORD		DBO_MAX_HLS_SLOT_MACHINES_PER_PAGE = 4;
const DWORD		DBO_MAX_HLS_SLOT_MACHINES_MAX_WINNERS = 3;
const DWORD		DBO_MAX_HLS_SLOT_MACHINES_MAX_ITEMS = 10;
const DWORD		DBO_MAX_WAGU_POINTS_PER_SPIN = 2;


enum eHLS_MACHINE_TYPE
{
	HLS_MACHINE_TYPE_WAGUWAGU = 0,
	HLS_MACHINE_TYPE_EVENT,

	HLS_MACHINE_TYPE_COUNT,
};


struct _sHLS_SLOT_MACHINE_ITEM
{
	TBLIDX		HlsItemTblidx;
	BYTE		bySetCount;
	WORD		wCurrentItemNum;
};

struct sHLS_SLOT_MACHINE_DATA
{
	WORD						wMachineIndex;
	BYTE						byType;
	BYTE						byCoin;
	BYTE						byOnOff;
	WORD						wForceWinCount;
	_sHLS_SLOT_MACHINE_ITEM		sItem[DBO_MAX_HLS_SLOT_MACHINES_MAX_ITEMS];
	WORD						wMaxQntt;
	WCHAR						wszFirstWinner[DBO_MAX_HLS_SLOT_MACHINES_MAX_WINNERS][NTL_MAX_SIZE_CHAR_NAME + 1];
	WORD						wFirstWinTryCount[DBO_MAX_HLS_SLOT_MACHINES_MAX_WINNERS];
	DBOTIME						nFirstWinnerExtractTime[DBO_MAX_HLS_SLOT_MACHINES_MAX_WINNERS];
	int							nFirstWinTime[DBO_MAX_HLS_SLOT_MACHINES_MAX_WINNERS];
	BYTE						byFirstWinnerNum;
	WORD						wResetTime;
	bool						bReseting;
};

struct sHLS_SLOT_MACHINE_EXTRACT_RESULT
{
	TBLIDX		itemTblidx;
	BYTE		byStackCount;
	BYTE		bySetCount;
	BYTE		byQnttIndex;
	bool		bFirstWin;
	WORD		wFirstWinTryCount;
};

struct sHLS_SLOT_MACHINE_EXTRACT_UPDATE_INFO
{
	bool		bFirstWin;
	WCHAR		wszFirstWinner[NTL_MAX_SIZE_CHAR_NAME + 1];
	WORD		wFirstWinTryCount;
	DBOTIME		nFirstWinnerExtractTime;
	int			nFirstWinTime;
	BYTE		byQnttIndex;
	WORD		wResultCode;
};

struct sHLS_SLOT_MACHINE_EXTRACT_DATA_FOR_CLIENT
{
	BYTE		byQnttIndex[DBO_MAX_HLS_SLOT_MACHINES_MAX_ITEMS];
	TBLIDX		ItemTblidx[DBO_MAX_HLS_SLOT_MACHINES_MAX_ITEMS];
	BYTE		byStackCount[DBO_MAX_HLS_SLOT_MACHINES_MAX_ITEMS];
	BYTE		bySetCount[DBO_MAX_HLS_SLOT_MACHINES_MAX_ITEMS];
	BYTE		byFinallyExtractCount;
};

#pragma pack()
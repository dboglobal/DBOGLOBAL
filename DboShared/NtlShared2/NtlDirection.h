//***********************************************************************************
//
//	File		:	NtlDirection.h
//
//	Begin		:	2007-06-21
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------------------
// 클라이언트 연출 타입
//-----------------------------------------------------------------------------------
enum eDIRECTION_TYPE
{
	DIRECTION_FLASH, // 2D production
	DIRECTION_CINEMATIC, // Cinema Directing tick
	DIRECTION_PLAY, // Director of animations specified by the server (the tick is unlike cinema and Sync) -> Delete scheduled

	MAX_DIECTION_TYPE,
	INVALID_DIRECTION_TYPE = 0xFF,
};

//-----------------------------------------------------------------------------------
// Directed character type (synchronization of state-level)
//-----------------------------------------------------------------------------------
enum eDIRECT_PLAY_TYPE
{
	DIRECT_PLAY_TQS_START,
	DIRECT_PLAY_TQS_ARRIVE,
	DIRECT_PLAY_TQS_LEAVE,

	DIRECT_PLAY_NORMAL,

	DIRECT_PLAY_VEHICLE,

	MAX_DIRECT_PLAY_TYPE,
	INVALID_DIRECT_PLAY_TYPE = 0xFF,
};

//-----------------------------------------------------------------------------------
//Broadcasting (radio trunks message type)
//-----------------------------------------------------------------------------------
enum eTELECAST_MESSAGE_TYPE
{
	TELECAST_MESSAGE_TYPE_NORMAL,
	TELECAST_MESSAGE_TYPE_WARNING,
	TELECAST_MESSAGE_TYPE_DANGER,
    TELECAST_MESSAGE_TYPE_NOTIFY,               ///< Carried notification message coming from the server (used only within the client)

	MAX_TELECAST_MESSAGE_TYPE,
	INVALID_TELECAST_MESSAGE_TYPE = 0xFF,
};

//-----------------------------------------------------------------------------------
// 진행 메시지 ( TMQ등에서 현재 진행상황을 안내하는 메시지 )
//-----------------------------------------------------------------------------------
enum ePROGRESS_MESSAGE_TYPE
{
	PROGRESS_MESSAGE_TYPE_TMQ,
	PGOGRESS_MESSAGE_TYPE_TMQ_STAGE,

	MAX_PROGRESS_MESSAGE_TYPE,
	INVALID_PROGRESS_MESSAGE_TYPE = 0xFF,
};

//-----------------------------------------------------------------------------------
// 방향 지시 메시지
//-----------------------------------------------------------------------------------
enum eDIRECTION_INDICATE_TYPE
{
	DIRECTION_INDICATE_TYPE_TOBJECT, // It indicates the object
	DIRECTION_INDICATE_TYPE_NPC, // It indicates the npc
	DIRECTION_INDICATE_TYPE_POINT, // Indicate a single spot

	MAX_DIRECTION_INDICATE_TYPE,
	INVALID_DIRECTION_INDICATE_TYPE = 0xFF,
};
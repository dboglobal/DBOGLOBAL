//***********************************************************************************
//
//	File		:	NtlPacketEncoder.cpp
//
//	Begin		:	2009-06-22
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	
//
//	Desc		:	
//
//***********************************************************************************

#include "stdafx.h"
#include "NtlPacketEncoder.h"
#include "NtlPacket.h"

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlPacketEncoder::RxDecrypt(CNtlPacket& rPacket)
{
	UNREFERENCED_PARAMETER( rPacket );
	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlPacketEncoder::TxEncrypt(CNtlPacket& rPacket)
{
	UNREFERENCED_PARAMETER( rPacket );
	return NTL_SUCCESS;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlPacketEncoder::RxDecrypt(CNtlPacket& rPacket, WORD sequence)
{
	UNREFERENCED_PARAMETER( rPacket );
	UNREFERENCED_PARAMETER( sequence );
	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlPacketEncoder::TxEncrypt(CNtlPacket& rPacket, WORD sequence)
{
	UNREFERENCED_PARAMETER( rPacket );
	UNREFERENCED_PARAMETER( sequence );
	return NTL_SUCCESS;
}
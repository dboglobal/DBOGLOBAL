//***********************************************************************************
//
//	File		:	NtlPacketEncoder.h
//
//	Begin		:	2007-01-27
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once


struct PACKETHEADER;
struct PACKETDATA;
class CNtlPacket;
class CNtlPacketEncoder
{
public:

	CNtlPacketEncoder(void) {}

	virtual ~CNtlPacketEncoder(void) {}


public:

	virtual int				RxDecrypt(void * pvPacketHeader) { UNREFERENCED_PARAMETER( pvPacketHeader ); return NTL_SUCCESS; }

	virtual int				TxEncrypt(void * pvPacketHeader) { UNREFERENCED_PARAMETER( pvPacketHeader ); return NTL_SUCCESS; }

	virtual int				RxDecrypt(CNtlPacket& rPacket);

	virtual int				TxEncrypt(CNtlPacket& rPacket);

	virtual int				RxDecrypt(CNtlPacket& rPacket, WORD sequence);

	virtual int				TxEncrypt(CNtlPacket& rPacket, WORD sequence);

};

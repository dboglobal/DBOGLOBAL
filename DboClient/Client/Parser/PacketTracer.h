/*****************************************************************************
* File			: PacketTracer.h
* Author		: woosung
* Copyright	: (주)NTL
* Date			: 2009. 08. 04
* Abstract		: 
*****************************************************************************
* Desc			:패킷추적을 위해 패킷을 패킷명과 시간을 dump
*****************************************************************************/

#pragma once

#include "NtlPacketCommon.h"
#include "ceventhandler.h"
#include <string>
#include <map>

using namespace	std;

#ifdef _DEBUG		/// 안전장치					
//#define	_ENABLE_PACKET_TRACE_		/// Defalut로 Disable /// 프로토콜이 변경될수있다. 자동빌드가 실패할 수 있음
#endif

class CPacketTracer : public RWS::CEventHandler
{
public:
	CPacketTracer();
	~CPacketTracer();

	enum TRACE_MODE
	{
		CAPTURE_ALL,
		CAPTURE_RECEIVE,
		CAPTURE_SEND,
	};

	void	Create();
	void	Destroy();
	void	SetRangeOpCode(int iMin, int iMax = -1);
	void	SetTraceMode(TRACE_MODE eTraceMode);
	const char*	GetPacketName(int iOpCode);

	void	Activate();
	void	Deactivate();
	void	Process(sNTLPACKETHEADER* pPacket, RwBool bReceive);
	//! Event
	virtual VOID HandleEvents( RWS::CMsg& msg );

	//! 싱글톤 생성/제거 
	static	void	CreateInstance();
	static	void	DeleteInstance();
	static CPacketTracer*	GetInstance();						///< 싱글톤 인스턴스 객체

private:
	RwBool m_bAvtivate;
	TRACE_MODE m_eTraceMode;

    typedef map< RwUInt32, string > PacketEnumNameMap;
	PacketEnumNameMap m_mapPacketName;

	int m_iMinOpCode;
	int m_iMaxOpCode;

	static CPacketTracer*	m_pInstance;

};
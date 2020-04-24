//***********************************************************************************
//
//	File		:	NtlNetworkProcessor.h
//
//	Begin		:	2007-01-16
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once


#include "NtlThread.h"


//---------------------------------------------------------------------------------------
// Sequential treatment for Accept / Connect / Receive / Close Event taking place on the network
//---------------------------------------------------------------------------------------
class CNtlNetwork;
class CNtlSession;

class CNtlNetworkProcessor : public CNtlRunObject
{
public:

	CNtlNetworkProcessor(CNtlNetwork *  pNetwork);

	~CNtlNetworkProcessor();


public:

	int							Create();

	void						Destroy();

public:

	int							PostNetEvent(WPARAM wParam, LPARAM lParam);

	virtual void				Run();

	virtual void				Close();

	int							SendNetEvent(WPARAM wParam, LPARAM lParam);

	void						ProcessNetEvent(ULONG_PTR netEvent, CNtlSession *pSession);

private:

	HANDLE						m_hEventIOCP;
};
//-----------------------------------------------------------------------------------

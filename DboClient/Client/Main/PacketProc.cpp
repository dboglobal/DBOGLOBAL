#include "precomp_dboclient.h"
#include "PacketProc.h"

// shared
#include "NtlClientNet.h"

// core
#include "NtlDebug.h"

// simulation
#include "NtlNetSender.h"


// dbo
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DumpCommand.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void NetworkLogCallback(const char *pNetLog)
{
//#ifndef NDEBUG

	std::string str = "network lib : ";
	str += pNetLog;

	NtlLogFilePrint((RwChar*)str.c_str());

//#endif

#ifdef  _ENABLE_PACKET_TRACE_
	GetDumpCmdManager()->RecvNet(pNetLog);
#endif
}

void NetworkEventHander(HSERVER hServer, int nEvent)
{
	SConnectData *pConnectData = GetDboGlobal()->GetConnectData();
	if(hServer == pConnectData->sAuthCon.hSocket)
	{
		switch(nEvent)
		{
		case NETCLIENT_EVENT_CONNECTED:
			{
				CNtlClientNet *pNet = GetDboGlobal()->GetNetwork(); 
				CLoginPacketGenerator *pLoginPackGenerator = GetDboGlobal()->GetLoginPacketGenerator(); 
				CNtlNetSender *pNetSender = pLoginPackGenerator->GetNetSender();
				pNetSender->SetData(pConnectData->sAuthCon.hSocket, pNet); 

				pConnectData->sAuthCon.byConnState = SConnectAtomic::SOCKET_STATE_CONNECT;

				CDboEventGenerator::LogInServerConnect(SERVER_CONNECT_TYPE_CONNENCT_SUCCESS);

				break;
			}
		case NETCLIENT_EVENT_CONNECT_FAIL:
			{
				pConnectData->sAuthCon.byConnState = SConnectAtomic::SOCKET_STATE_DISCONNECT;
				CDboEventGenerator::LogInServerConnect(SERVER_CONNECT_TYPE_CONNENCT_FAIL);
				break;
			}
		case NETCLIENT_EVENT_DISCONNECTED:
			{
				if(pConnectData->sAuthCon.byConnState == SConnectAtomic::SOCKET_STATE_CONNECT)
				{
					CDboEventGenerator::LogInServerConnect(SERVER_CONNECT_TYPE_CONNENCT_DISCONNECT);
				}

				pConnectData->sAuthCon.byConnState = SConnectAtomic::SOCKET_STATE_DISCONNECT;
				pConnectData->sAuthCon.hSocket = INVALID_HSERVER;

				break;
			}
		}
	}
	else if(hServer == pConnectData->sCharCon.hSocket)
	{
		switch(nEvent)
		{
		case NETCLIENT_EVENT_CONNECTED:
			{
				CNtlClientNet *pNet = GetDboGlobal()->GetNetwork(); 
				CLobbyPacketGenerator *pLobbyPackGenerator = GetDboGlobal()->GetLobbyPacketGenerator(); 
				CNtlNetSender *pNetSender = pLobbyPackGenerator->GetNetSender();
				pNetSender->SetData(pConnectData->sCharCon.hSocket, pNet); 

				pConnectData->sCharCon.byConnState = SConnectAtomic::SOCKET_STATE_CONNECT;

				CDboEventGenerator::LobbyServerConnect(SERVER_CONNECT_TYPE_CONNENCT_SUCCESS);

				break;
			}
		case NETCLIENT_EVENT_CONNECT_FAIL:
			{
				pConnectData->sCharCon.byConnState = SConnectAtomic::SOCKET_STATE_CONNECT;
				CDboEventGenerator::LobbyServerConnect(SERVER_CONNECT_TYPE_CONNENCT_FAIL);
				break;
			}
		case NETCLIENT_EVENT_DISCONNECTED:
			{
				if(pConnectData->sCharCon.byConnState == SConnectAtomic::SOCKET_STATE_CONNECT)
				{
					CDboEventGenerator::LobbyServerConnect(SERVER_CONNECT_TYPE_CONNENCT_DISCONNECT);
				}

				pConnectData->sCharCon.byConnState = SConnectAtomic::SOCKET_STATE_DISCONNECT;
				pConnectData->sCharCon.hSocket = INVALID_HSERVER;

				break;
			}
		}
	}
	else if(hServer == pConnectData->sGameCon.hSocket)
	{
		switch(nEvent)
		{
		case NETCLIENT_EVENT_CONNECTED:
			{
				CDboEventGenerator::GameServerConnect(SERVER_CONNECT_TYPE_CONNENCT_SUCCESS);
				break;
			}
		case NETCLIENT_EVENT_CONNECT_FAIL:
			{
				CDboEventGenerator::GameServerConnect(SERVER_CONNECT_TYPE_CONNENCT_FAIL);
				break;
			}
		case NETCLIENT_EVENT_DISCONNECTED:
			{
				if(pConnectData->sGameCon.byConnState == SConnectAtomic::SOCKET_STATE_CONNECT)
				{
					CDboEventGenerator::GameServerConnect(SERVER_CONNECT_TYPE_CONNENCT_DISCONNECT);
				}

				pConnectData->sGameCon.byConnState = SConnectAtomic::SOCKET_STATE_DISCONNECT;
				pConnectData->sGameCon.hSocket = INVALID_HSERVER;

				break;
			}
		}
	}
    else if(hServer == pConnectData->sChatCon.hSocket)
    {
        static bool bChatServerReconnect = false;
        switch(nEvent)
        {
        case NETCLIENT_EVENT_CONNECTED:
            {   
                if(bChatServerReconnect)
                {
                    GetDboGlobal()->GetGamePacketGenerator()->SendAuthKeyCommunityServerReq();	
                    pConnectData->sChatCon.byConnState = SConnectAtomic::SOCKET_STATE_CONNECT;
                    CNtlClientNet *pNet = GetDboGlobal()->GetNetwork(); 
                    // chatting packet generator 의 CNtlNetSender data setting.
                    CChatPacketGenerator *pChatPacketGenerator = GetDboGlobal()->GetChatPacketGenerator();
                    CNtlNetSender *pChatNetSender = pChatPacketGenerator->GetNetSender();
                    pChatNetSender->SetData(pConnectData->sChatCon.hSocket, pNet); 
                    bChatServerReconnect = false;

                    CNtlSLEventGenerator::PopoNotify(GetDisplayStringManager()->GetString("DST_CHAT_SERVER_TITLE"),
                                                     GetDisplayStringManager()->GetString("DST_CHAT_SERVER_RECONNECT"), 
                                                     TRUE, 5.0f);
                }               
            }            
            break;        
        case NETCLIENT_EVENT_DISCONNECTED:
            pConnectData->sChatCon.byConnState = SConnectAtomic::SOCKET_STATE_DISCONNECT;
            pConnectData->sChatCon.hSocket = INVALID_HSERVER;            
            if(pConnectData->sGameCon.hSocket != INVALID_HSERVER)
            {
                bChatServerReconnect = true;
                CNtlSLEventGenerator::PopoNotify(GetDisplayStringManager()->GetString("DST_CHAT_SERVER_TITLE"),
                                                 GetDisplayStringManager()->GetString("DST_CHAT_SERVER_DISCONNECT"), 
                                                 TRUE, 5.0f);
            }
            //break;  // NOTE: break가 없이 아래 재접속을 요청한다. (by agebreak)
        case NETCLIENT_EVENT_CONNECT_FAIL:            
            if(pConnectData->sGameCon.hSocket != INVALID_HSERVER)
            {
                // 재접속 요청 (비동기)
                CNtlClientNet *pNet = GetDboGlobal()->GetNetwork();             
                pNet->Connect((char*)pConnectData->sChatCon.chServerIP, pConnectData->sChatCon.wServerPort, &pConnectData->sChatCon.hSocket, true, NETCLIENT_ENCODE_NONE);
            }
            
            break;
        }
    }
	else
	{
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///
CClientRecvPacketProc::CClientRecvPacketProc( const HSERVER hServer )
	: CRecvPacketProcess( hServer )
{
}

bool CClientRecvPacketProc::RecvPacketProcess( void* const pData )
{
	GetDumpCmdManager()->RecvPacket(pData); 
	CNtlPacketHandler::DetectPacketHeader(pData);
	return true;
}

bool CClientRecvPacketProc::IsActivePop( void )
{
	return B2b(CPacketProc::GetInstance()->IsActivePop());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///

CPacketProc::CPacketProc()
{
	m_bPop = TRUE;
}

CPacketProc::~CPacketProc()
{
	ListVirtualPacket::iterator it;
	for(it = m_listPacket.begin(); it != m_listPacket.end(); ++it)
	{
		void *pData = (*it);
		NTL_ARRAY_DELETE(pData);
	}

	m_listPacket.clear();
}

CPacketProc* CPacketProc::GetInstance(void)
{
	static CPacketProc Instance;
	return &Instance;
}

void CPacketProc::PopPacket(HSERVER hSocketId)
{
	CNtlClientNet *pNet = CDboGlobal::GetInstance()->GetNetwork();

	pNet->RecvPacketProcess( CClientRecvPacketProc( hSocketId ) );

	//void *pData;
	//do
	//{
	//	if(!IsActivePop())
	//		break;
	//	
		//pData = pNet->PeekPacket(hSocketId);
	//	if(pData)
	//	{
	//		GetDumpCmdManager()->RecvPacket(pData); 
	//		CNtlPacketHandler::DetectPacketHeader(pData);
			//pNet->PopPacket( hSocketId );
	//	}
	//} while (pData);
}

void CPacketProc::PopVirutalPacket(void)
{
	if(m_listPacket.size() == 0)
		return;

	ListVirtualPacket::iterator it;
	for(it = m_listPacket.begin(); it != m_listPacket.end(); ++it)
	{
		void *pData = (*it);
		GetDumpCmdManager()->RecvPacket(pData); 
		CNtlPacketHandler::DetectPacketHeader(pData);
		NTL_ARRAY_DELETE(pData);
	}

	m_listPacket.clear();
}

void CPacketProc::RecvPacketProc(void)
{
	if(!IsActivePop())
		return;

	SConnectData *pConnectData = GetDboGlobal()->GetConnectData();
	if(pConnectData->sAuthCon.hSocket != INVALID_SOCKET)
		PopPacket(pConnectData->sAuthCon.hSocket);
	if(pConnectData->sCharCon.hSocket != INVALID_SOCKET)
		PopPacket(pConnectData->sCharCon.hSocket);
	if(pConnectData->sChatCon.hSocket != INVALID_SOCKET)
		PopPacket(pConnectData->sChatCon.hSocket);
	if(pConnectData->sGameCon.hSocket != INVALID_SOCKET)
		PopPacket(pConnectData->sGameCon.hSocket);
}

void CPacketProc::RecvVirualPackProc(void *pPacket)
{
	SDboEventVirtualServerPacket *pVirtualPacket = reinterpret_cast<SDboEventVirtualServerPacket*>(pPacket);

	if(!IsActivePop())
	{
		void *pPushData = NTL_NEW RwChar [pVirtualPacket->iDataLen];
		memcpy(pPushData, pVirtualPacket->chPacket, pVirtualPacket->iDataLen);
		m_listPacket.push_back(pPushData);
		return;
	}

	// 기존에 queue에 쌓여 있는 packet이 있으면 처리한다.
	PopVirutalPacket();

	CNtlPacketHandler::DetectPacketHeader(pVirtualPacket->chPacket);
}

void CPacketProc::ActivePop(RwBool bPop)
{
	m_bPop = bPop;

	if(IsActivePop())
	{
		// 기존에 queue에 쌓여 있는 packet이 있으면 처리한다.
		PopVirutalPacket();
	}
}


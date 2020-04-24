//***********************************************************************************
//
//	File		:	NtlClientNet.cpp
//
//	Begin		:	2006-01-17
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	NTL Client Library Implementation
//
//***********************************************************************************

#include "stdafx.h"

#include "NtlClientNet.h"

#include "NtlBase.h"
#include "NtlPacket.h"
#include "NtlSocket.h"
#include "NtlNetBuffer.h"
#include "NtlPacketSYS.h"

//#include "NtlPacketEncoder_XOR.h"
//#include "NtlPacketEncoder_RandKey.h"


#include "NtlQueue.h"
#include "NtlMutex.h"
#include "NtlError.h"
#include "NtlDebug.h"

#include <process.h>
#include <deque>
#include <map>



typedef CNtlQueue<CNtlPacket*> PACKETQUEUE;


//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
const int		BUFSIZE_LOG = 1024;
//---------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
#define NET_LOG( LOGMSG, ...)	Log( NULL, NULL, -1, false, LOGMSG, __VA_ARGS__)
#define NET_LOGD( LOGMSG, ...)	Log( NULL, NULL, -1, true,  LOGMSG, __VA_ARGS__)
#define NET_LOGDL( LOGMSG, ...) Log( __FILE__, __FUNCTION__, __LINE__, true, LOGMSG, __VA_ARGS__)
#define CONNECTION_LOG( LOGMSG, ...)	m_pParent->Log( NULL, NULL, -1, false, LOGMSG, __VA_ARGS__)
#define CONNECTION_LOGD( LOGMSG, ...)	m_pParent->Log( NULL, NULL, -1, true,  LOGMSG, __VA_ARGS__)
#define CONNECTION_LOGDL( LOGMSG, ...) m_pParent->Log( __FILE__, __FUNCTION__, __LINE__, true, LOGMSG, __VA_ARGS__)
//---------------------------------------------------------------------------------------


void NtlClientNetSetPrintFlag( unsigned int dwFlag )
{
	NtlSetPrintFlag( dwFlag );
}


//---------------------------------------------------------------------------------------
// CServerConnection class ( CNtlClientNet inner class )
//---------------------------------------------------------------------------------------
class CClientNetImp;
class CServerConnection
{

//#if __SGPRO_BOTSYSTEM_SENDANDRECVTESTCODE__
//
//public:
//	CBotSystemLog_Client m_cConsolLog;
//	void SetCharacterPCID( __int64 nCharacterPCID ) { m_nCharacterPCID = nCharacterPCID; }
//
//	void CreateLogFile( void )
//	{
//		char szCharacterPCID[1024] = { 0, };
//
//		sprintf_s( szCharacterPCID, sizeof( szCharacterPCID ), "BotClientSend_%d.log", m_nCharacterPCID );
//		m_cConsolLog.Create( szCharacterPCID );
//		return;
//
//	}
//
//private:
//	__int64 m_nCharacterPCID;
//
//#endif//__SGPRO_BOTSYSTEM_SENDANDRECVTESTCODE__

public:

	enum eSTATE
	{
		IDLE,
		CONNECTING,
		CONNECTED,
		DISCONNECTED,

		MAX_STATE
	};


public:

	CServerConnection();

	virtual ~CServerConnection();


public:

	bool							Create(CClientNetImp * pParent, CNtlSocket &rSocket, CNtlSockAddr & rSockAddr, eNETCLIENT_ENCODE_TYPE encode, int nPacketCount, int nPacketSize);

	void							Destroy();


public:


	void							Close();

	int								Recv();

	int								Send();

	int								RecvPendingData();

	int								SendPendingData();


public:


	SOCKET							GetRawSocket() {  return m_socket.GetRawSocket(); }

	HSERVER							GetHandle() { return GetRawSocket(); }

	PACKETQUEUE *					GetRecvQueue() { return &m_recvQueue; }

	PACKETQUEUE *					GetSendQueue() { return &m_sendQueue; }

	CNtlSockAddr &					GetRemoteAddr() { return m_remoteAddr; }

	CNtlSockAddr &					GetLocalAddr() { return m_remoteAddr; }

	eSTATE							GetState() { return m_state; }

	CNtlPacketEncoder *				GetPacketEncoder() { return m_pPacketEncoder; }

	void							SetState(eSTATE state) { m_state = state; }

	bool							IsState(eSTATE state) { return m_state == state; }

	DWORD							GetRecvPacketCount() { return m_dwRecvPacketCount; }

	DWORD							GetSendPacketCount() { return m_dwSendPacketCount; }

	int								GetHeaderSize() { return PACKET_HEADSIZE; }
	int								GetPacketLen(BYTE* pHeaderPointer);


protected:

	void							Init();

	CNtlPacketEncoder *				CreatePacketEncoder(eNETCLIENT_ENCODE_TYPE encode);


private:

	CNtlSocket						m_socket;

	CNtlSockAddr					m_remoteAddr;

	PACKETQUEUE						m_sendQueue;

	PACKETQUEUE						m_recvQueue;

	CNtlNetBuffer					m_recvBuffer;

	CNtlNetBuffer					m_sendBuffer;

	eSTATE							m_state;

	CNtlPacketEncoder *				m_pPacketEncoder;

	CClientNetImp *					m_pParent;

	DWORD							m_dwRecvPacketCount;

	DWORD							m_dwSendPacketCount;



public:

	void							Acquire() { InterlockedIncrement( (LONG*)&m_dwReferenceCount ); }

	void							Release() { if( 0 == InterlockedDecrement( (LONG*)&m_dwReferenceCount ) ) delete this; }

	int								MakeSureCompletedPacket(CNtlNetBuffer* pBuffer, WORD* pwPacketLength, CNtlPacketEncoder * pPacketEncoder);
private:

	DWORD							m_dwReferenceCount;

	//CNtlMutex						m_mutex; // [6/4/2008 SGpro]

#if __SGPRO_BOTSYSTEM_SENDANDRECVTESTCODE__
	WORD wPacketLength;
	CNtlPacket packet;
#endif


};

//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
#define RELEASE_CONNECTION(p)  if( p ) { p->Release(); p = NULL; }
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// CServerConnection Auto Pointer
//--------------------------------------------------------------------------------------
class CAutoPtr_ServerConnection
{
public:

	CAutoPtr_ServerConnection()
		:m_pServerConnection( NULL )
	{
	}

	CAutoPtr_ServerConnection(CServerConnection * pServerConnection)
		:m_pServerConnection( NULL )
	{
		m_pServerConnection = pServerConnection;
	}

	~CAutoPtr_ServerConnection()
	{
		RELEASE_CONNECTION( m_pServerConnection );
	}

public:

	CAutoPtr_ServerConnection & operator=(CServerConnection * pSeverConnection) { m_pServerConnection = pSeverConnection; }

	CServerConnection * operator->() { return m_pServerConnection; }

public:

	bool IsValidPtr() { return m_pServerConnection ? true : false; }

private:

	CServerConnection *				m_pServerConnection;
};


//---------------------------------------------------------------------------------------
// CLientNet Implementation class ( CNtlClientNet inner class )
//---------------------------------------------------------------------------------------
class CClientNetImp
{
public:

	CClientNetImp(CNtlClientNet * pClientNet);

	virtual ~CClientNetImp();


public:

	virtual void				OnThreadProc() {}


	virtual bool				Create(HWND hWnd, NtlClientNetEventCallback EventCallback, NtlClientNetLogCallback LogCallback, DWORD dwAliveTime, int nMaxConnection) = 0;

	virtual void				Destroy();

#if __SGPRO_BOTSYSTEM_SENDANDRECVTESTCODE__
	virtual bool				Connect(char* szServerAddress, WORD wPort, HSERVER * pServerHandle, bool bAsync, eNETCLIENT_ENCODE_TYPE encode, int nPacketCount, int nPacketSize, __int64 nBotID )
	{ (void)szServerAddress; (void)wPort; (void)pServerHandle; (void)bAsync; (void)encode; (void) nPacketSize; (void) nPacketCount; (void) nPacketSize; (void) nBotID; return false; }
#else
	virtual bool				Connect(char* szServerAddress, WORD wPort, HSERVER * pServerHandle, bool bAsync, eNETCLIENT_ENCODE_TYPE encode, int nPacketCount, int nPacketSize)
	{ (void)szServerAddress; (void)wPort; (void)pServerHandle; (void)bAsync; (void)encode; (void) nPacketSize; (void) nPacketCount; (void) nPacketSize; return false; }
#endif

	virtual bool				Connected(HSERVER hServer) { (void) hServer; return true; }

	virtual bool				Disconnect(HSERVER hServer) { (void)hServer; return false; }

	virtual bool				InternalDisconnect(HSERVER hServer) { (void)hServer; return false; }


	virtual bool				NetMsgProc(WPARAM wParam, LPARAM lParam) { (void)wParam; (void)lParam; return true; }

	virtual void * 				PeekPacket(HSERVER hServer) { (void)hServer; return NULL; }

#if __SGPRO_BOTSYSTEM_SENDANDRECVTESTCODE__
	virtual void * 				SGPeekPacket(HSERVER hServer) { (void)hServer; return NULL; }
#endif

	virtual void 				PopPacket(HSERVER hServer) { (void)hServer; }

	virtual bool				PushPacket(HSERVER hServer, void * pData, int nDataSize)
	{ (void)hServer; (void)pData; (void)nDataSize; return 0; }

	bool						IsProgress(HSERVER hServer);

	void						Log(const char * lpszFile, const char * lpszFunc, int nLine, bool bDate, const char * lpszText, ...);


public:

	LPCSTR						GetServerAddress(HSERVER);

	WORD						GetServerPort(HSERVER);

	LPCSTR						GetLocalAddress(HSERVER);

	DWORD						GetLocalAddr(HSERVER);

	WORD						GetLocalPort(HSERVER);


protected:

	void						DoAlive();

	bool						OpenThread();

	bool						Remove(HSERVER hServer);

public:

	CServerConnection *			Acquire(HSERVER hServer);

private:

	void						Init();


protected:

	typedef std::map<HSERVER, CServerConnection*>	SERVERLIST;
	typedef SERVERLIST::value_type					SERVERVAL;
	typedef SERVERLIST::iterator					SERVERIT;


	SERVERLIST					m_serverList;

	CNtlMutex					m_mutex;


	HANDLE						m_hThread;

	unsigned					m_threadID;

	bool						m_bProgress;


	NtlClientNetEventCallback	m_pfnEventCallback;

	NtlClientNetLogCallback 	m_pfnLogCallBack;

	DWORD						m_dwAliveTime;

	int							m_nCurConnection;

	CNtlClientNet *				m_pClientNetRef;

};


//-----------------------------------------------------------------------------------
//		Purpose	: ClientNetworkImp Thread Procedure
//		Return	:
//-----------------------------------------------------------------------------------
static unsigned __stdcall ThreadMain(void * arg)
{
	CClientNetImp * pImp = (CClientNetImp*) arg;
	pImp->OnThreadProc();

	return 0;
}



//---------------------------------------------------------------------------------------
// CMsgBaseNetworkImp Implementation class ( CNtlClientNet inner class )
//---------------------------------------------------------------------------------------
class CMsgBaseNetworkImp : public CClientNetImp
{
public:

	CMsgBaseNetworkImp(CNtlClientNet * pClientNet);

	virtual ~CMsgBaseNetworkImp();


public:

	void						OnThreadProc();


	bool						Create(HWND hWnd, NtlClientNetEventCallback EventCallback, NtlClientNetLogCallback LogCallback, DWORD dwAliveTime, int nMaxConnection);

	void						Destroy();


	bool						Connect(char* szServerAddress, WORD wPort, HSERVER * pServerHandle, bool bAsync, eNETCLIENT_ENCODE_TYPE encode, int nPacketCount, int nPacketSize);

	bool						Connected(HSERVER hServer);

	bool						Disconnect(HSERVER hServer);

	bool						InternalDisconnect(HSERVER hServer);


	bool						NetMsgProc(WPARAM wParam, LPARAM lParam);

	void * 						PeekPacket(HSERVER hServer);

	void 						PopPacket(HSERVER hServer);

	bool						PushPacket(HSERVER hServer, void * pData, int nDataSize);

private:

	HWND						m_hWnd;

};


//---------------------------------------------------------------------------------------
// CEventBaseNetworkImp Implementation class ( CNtlClientNet inner class )
//---------------------------------------------------------------------------------------
class CEventBaseNetworkImp : public CClientNetImp
{
public:

	typedef std::deque<WSAEVENT> LIST;
	typedef LIST::iterator LISTIT;

	struct EVENTINFO
	{
		EVENTINFO()
		{
			for(int i = 0; i < WSA_MAXIMUM_WAIT_EVENTS; i++ )
			{
				ahServer[ i ] = INVALID_HSERVER;
				ahEvent[ i ] = INVALID_HANDLE_VALUE;
			}
		}


		HSERVER		ahServer[WSA_MAXIMUM_WAIT_EVENTS];
		WSAEVENT	ahEvent[WSA_MAXIMUM_WAIT_EVENTS];
		LIST		eventFreeList;
	};


	CEventBaseNetworkImp(CNtlClientNet * pClientNet);

	virtual ~CEventBaseNetworkImp();


public:

	void						OnThreadProc();


	bool						Create(HWND hWnd, NtlClientNetEventCallback EventCallback, NtlClientNetLogCallback LogCallback, DWORD dwAliveTime, int nMaxConnection);

	void						Destroy();

#if __SGPRO_BOTSYSTEM_SENDANDRECVTESTCODE__
	bool						Connect(char* szServerAddress, WORD wPort, HSERVER * pServerHandle, bool bAsync, eNETCLIENT_ENCODE_TYPE encode, int nPacketCount, int nPacketSize, __int64 nBotID );

	void *						SGPeekPacket(HSERVER hServer);
#else
	bool						Connect(char* szServerAddress, WORD wPort, HSERVER * pServerHandle, bool bAsync, eNETCLIENT_ENCODE_TYPE encode, int nPacketCount, int nPacketSize );
#endif

	bool						Connected(HSERVER hServer);

	bool						Disconnect(HSERVER hServer);

	bool						InternalDisconnect(HSERVER hServer);


	void *						PeekPacket(HSERVER hServer);

	void 						PopPacket(HSERVER hServer);

	bool						PushPacket(HSERVER hServer, void * pData, int nDataSize);

public:

	bool						OpenEvent();


private:

	void						Init();


private:

	EVENTINFO					m_eventInfo;

};


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlClientNet::CNtlClientNet()
{	
	m_pImp = NULL;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlClientNet::~CNtlClientNet()
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlClientNet::Create(HWND hWnd /* = NULL */, NtlClientNetEventCallback EventCallback /* = NULL */, NtlClientNetLogCallback LogCallback /* = NULL */, DWORD dwAliveTime /* = ALIVE_PACKET_FREQUENCY */, int nMaxConnection /* = WSA_MAXIMUM_WAIT_EVENTS */)
{
	if( NULL != m_pImp )
	{
		return false;
	}


	WSADATA wsaData;
	if( 0 != WSAStartup(MAKEWORD(1, 1), &wsaData) )
	{
		int rc = WSAGetLastError();
		if( LogCallback )
		{
			(LogCallback)( NtlGetErrorMessage( rc ) );
		}

		return false;
	}


	if( NULL != hWnd )
	{
		m_pImp = new CMsgBaseNetworkImp( this );
	}
	else
	{
		m_pImp = new CEventBaseNetworkImp( this );
	}


	if( NULL == m_pImp )
	{
		if( LogCallback )
		{
			(LogCallback)( "NetworkImp Memory Allocate Error" );
		}

		return false;
	}


	if( false == m_pImp->Create( hWnd, EventCallback, LogCallback, dwAliveTime, nMaxConnection ) )
	{
		if( LogCallback )
		{
			(LogCallback)( "NetworkImp Create Error" );
		}

		SAFE_DELETE( m_pImp );
		return false;
	}


	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlClientNet::Destroy()
{
	_ASSERT( NULL != m_pImp );

	WSACleanup();

	SAFE_DELETE( m_pImp );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
#if __SGPRO_BOTSYSTEM_SENDANDRECVTESTCODE__
bool CNtlClientNet::Connect( __int64 nBotID, char* szServerAddress, WORD wPort, HSERVER * pServerHandle, bool bAsync /* = false */, eNETCLIENT_ENCODE_TYPE encode /* = NETCLIENT_ENCODE_NONE */, int nMaxPacketCount /* = 0 */, int nMaxPacketSize /* = 0 */ )
#else
bool CNtlClientNet::Connect(char* szServerAddress, WORD wPort, HSERVER * pServerHandle, bool bAsync /* = false */, eNETCLIENT_ENCODE_TYPE encode /* = NETCLIENT_ENCODE_NONE */, int nMaxPacketCount /* = 0 */, int nMaxPacketSize /* = 0 */)
#endif
{
	_ASSERT( NULL != m_pImp );

#if __SGPRO_BOTSYSTEM_SENDANDRECVTESTCODE__
	return m_pImp->Connect( szServerAddress, wPort, pServerHandle, bAsync, encode, nMaxPacketCount, nMaxPacketSize, nBotID );
#else
	return m_pImp->Connect( szServerAddress, wPort, pServerHandle, bAsync, encode, nMaxPacketCount, nMaxPacketSize );
#endif
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
#if __SGPRO_BOTSYSTEM_SENDANDRECVTESTCODE__
bool CNtlClientNet::Connect( __int64 nBotID, WCHAR* pwszServerAddress, WORD wPort, HSERVER * pServerHandle, bool bAsync /* = false */, eNETCLIENT_ENCODE_TYPE encode /* = NETCLIENT_ENCODE_NONE */, int nMaxPacketCount /* = 0 */, int nMaxPacketSize /* = 0 */)
#else
bool CNtlClientNet::Connect(WCHAR* pwszServerAddress, WORD wPort, HSERVER * pServerHandle, bool bAsync /* = false */, eNETCLIENT_ENCODE_TYPE encode /* = NETCLIENT_ENCODE_NONE */, int nMaxPacketCount /* = 0 */, int nMaxPacketSize /* = 0 */)
#endif
{
	char szServerAddress[64] = { 0x00, };
	if ( 0 == ::WideCharToMultiByte(GetACP(), 0, pwszServerAddress, -1, szServerAddress, _countof(szServerAddress), NULL, NULL) )
	{
		return false;
	}

#if __SGPRO_BOTSYSTEM_SENDANDRECVTESTCODE__
	return Connect( nBotID, szServerAddress, wPort, pServerHandle, bAsync, encode, nMaxPacketCount, nMaxPacketSize );
#else
	return Connect( szServerAddress, wPort, pServerHandle, bAsync, encode, nMaxPacketCount, nMaxPacketSize );
#endif
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlClientNet::Disconnect(HSERVER hServer)
{
	_ASSERT( NULL != m_pImp );

	return m_pImp->Disconnect( hServer );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlClientNet::NetMsgProc(WPARAM wParam, LPARAM lParam)
{
	_ASSERT( NULL != m_pImp );

	return m_pImp->NetMsgProc( wParam, lParam );
}

#if __SGPRO_BOTSYSTEM_SENDANDRECVTESTCODE__
//-- SGpro --------------------------------------------------------------------------
//  Purpose :
//  Return  :
//-----------------------------------------------------------------------------------
void * CNtlClientNet::SGPeekPacket(HSERVER hServer)
{
	_ASSERT( NULL != m_pImp );

	return m_pImp->SGPeekPacket( hServer );
}
#endif //end of #if __SGPRO_BOTSYSTEM_SENDANDRECVTESTCODE__

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void * CNtlClientNet::PeekPacket(HSERVER hServer)
{
	_ASSERT( NULL != m_pImp );

	return m_pImp->PeekPacket( hServer );
}

//-- SGpro --------------------------------------------------------------------------
//  Purpose : 수정
//  Return  :
//-----------------------------------------------------------------------------------
void CNtlClientNet::PopPacket(HSERVER hServer)
{
	_ASSERT( NULL != m_pImp );

	m_pImp->PopPacket( hServer );

	return;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlClientNet::PushPacket(HSERVER hServer, void * pData, int nDataSize)
{
	_ASSERT( NULL != m_pImp );

	return m_pImp->PushPacket( hServer, pData, nDataSize );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlClientNet::IsProgress(HSERVER hServer)
{
	return m_pImp->IsProgress( hServer );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
LPCSTR CNtlClientNet::GetServerAddress(HSERVER hServer)
{
	return m_pImp->GetServerAddress( hServer );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
WORD CNtlClientNet::GetServerPort(HSERVER hServer)
{
	return m_pImp->GetServerPort( hServer );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
LPCSTR CNtlClientNet::GetLocalAddress(HSERVER hServer)
{
	return m_pImp->GetLocalAddress( hServer );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
DWORD CNtlClientNet::GetLocalAddr(HSERVER hServer)
{
	return m_pImp->GetLocalAddr( hServer );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
WORD CNtlClientNet::GetLocalPort(HSERVER hServer)
{
	return m_pImp->GetLocalPort( hServer );
}

//-- SGpro --------------------------------------------------------------------------
//  Purpose :
//  Return  :
//-----------------------------------------------------------------------------------
void CNtlClientNet::RecvPacketProcess( CRecvPacketProcess& rProc )
{
	if( false == rProc.IsActivePop() )
	{
		return;
	}

	CAutoPtr_ServerConnection pConnection( m_pImp->Acquire( rProc.m_hServer ) );
	if( false == pConnection.IsValidPtr() )
	{
		return;
	}

	CNtlPacket* pPacket = pConnection->GetRecvQueue()->Peek();
	while ( NULL != pPacket )
	{
#if __SGPRO_BOTSYSTEM_SENDANDRECVTESTCODE__
		if( false == rProc.RecvPacketProcess( pPacket ) )
		{
			pPacket = pConnection->GetRecvQueue()->Pop();
			SAFE_DELETE( pPacket );
			return;
		}
#else
		if( false == rProc.RecvPacketProcess( pPacket->GetPacketData() ) )
		{					
			pPacket = pConnection->GetRecvQueue()->Pop();
			SAFE_DELETE( pPacket );
			return;
		}
#endif

		pPacket = pConnection->GetRecvQueue()->Pop();
		SAFE_DELETE( pPacket );

		if( false == rProc.IsActivePop() )
		{
			break;
		}

		pPacket = pConnection->GetRecvQueue()->Peek();

	}//end of while()

	return;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CServerConnection::CServerConnection()
{
	Init();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CServerConnection::~CServerConnection()
{
	Destroy();
}


int CServerConnection::GetPacketLen(BYTE * pHeaderPointer)
{
	STHeaderBase* pHeader = (STHeaderBase*)pHeaderPointer;
	return pHeader->wPacketLen;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CServerConnection::Init()
{
	m_pParent = NULL;

	SetState( IDLE );

	m_dwReferenceCount = 1;

	m_pPacketEncoder = NULL;

	m_dwRecvPacketCount = m_dwSendPacketCount = 0;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CServerConnection::Create(CClientNetImp * pParent, CNtlSocket &rSocket, CNtlSockAddr & rSockAddr, eNETCLIENT_ENCODE_TYPE encode, int nPacketCount, int nPacketSize)
{
	if( nPacketCount <= 0 )
	{
		nPacketCount = DEF_PACKET_MAX_COUNT;
	}

	if( nPacketSize <= 0 )
	{
		nPacketSize = PACKET_MAX_SIZE;
	}


#if __SGPRO_BOTSYSTEM_SENDANDRECVTESTCODE__
	encode;
#else
	m_pPacketEncoder = CreatePacketEncoder( encode );
#endif


	if( false == m_recvBuffer.Create( nPacketCount, nPacketSize ) )
	{
		return false;
	}

	if( false == m_sendBuffer.Create( nPacketCount, nPacketSize ) )
	{
		return false;
	}

	m_pParent		= pParent;
	m_socket		= rSocket;
	m_remoteAddr	= rSockAddr;


	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CServerConnection::Destroy()
{
	CNtlPacket * pPacket = NULL;

	do
	{
		pPacket = m_sendQueue.Pop();
		SAFE_DELETE( pPacket );        
	} while(!m_sendQueue.IsEmpty());

	do
	{
		pPacket = m_recvQueue.Pop();		
		SAFE_DELETE( pPacket );
	} while(!m_recvQueue.IsEmpty());


	m_socket.Close();
	m_recvBuffer.Clear();
	m_sendBuffer.Clear();


	SAFE_DELETE( m_pPacketEncoder );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CServerConnection::Close()
{
	m_socket.Close();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int	CServerConnection::Recv()
{
	int nSize = 0;

	if( false == m_recvBuffer.IsFull() )
	{
		nSize = m_socket.RecvStream( m_recvBuffer.GetQueuePushPtr(), m_recvBuffer.GetLinearFreeSize() );

		if( nSize > 0 )
		{
			m_recvBuffer.IncreasePushPos( nSize );
		}
	}

	return nSize;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CServerConnection::Send()
{

	int nSize = 0;

	if( false == m_sendBuffer.IsEmpty() )
	{

		// 아래 코드에서 마지막인자를 false에서 true로 바꿈[6/19/2008 SGpro]
		//nSize = m_socket.SendStream( m_sendBuffer.GetQueuePopPtr(), m_sendBuffer.GetCurSize(), false );
		nSize = m_socket.SendStream( m_sendBuffer.GetQueuePopPtr(), m_sendBuffer.GetCurSize(), true );

//#if __SGPRO_BOTSYSTEM_SENDANDRECVTESTCODE__
//		m_cConsolLog.WriteBotSystemLog( reinterpret_cast< void * >( m_sendBuffer.GetQueuePopPtr() )
//			, m_nCharacterPCID
//			, &m_sendBuffer
//			, nSize);
//#endif// __SGPRO_BOTSYSTEM_SENDANDRECVTESTCODE__

		if( nSize > 0 )
		{
			m_sendBuffer.IncreasePopPos( nSize );
		}
	}

	return nSize;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:	받은 패킷의 수
//-----------------------------------------------------------------------------------
int	CServerConnection::RecvPendingData()
{
	int nCompletePacket = 0;

#if 0
	bool bLoop = true;
	CNtlPacket packet;
	while( false != bLoop )
	{
		// 패킷 체크
		WORD wPacketLength = 0;
		if( false == MakeSureCompletedPacket( &m_recvBuffer, &wPacketLength, m_pPacketEncoder ) )
		{
			return false;
		}

		packet.Attach( m_recvBuffer.GetQueueWorkPtr(), wPacketLength );

		if( false == packet.IsValidPacket() )
		{
			return false;
		}

		m_recvBuffer.IncreaseWorkPos( packet.GetUsedSize() );


		// 완성된 패킷 큐에 저장
		CNtlPacket * pPacket = new CNtlPacket( packet );
		if( NULL == pPacket )
		{
			//NETL_LOGDL("Packet Memory Allocate Error");
			return false;
		}


		// 패킷 복호화
		if( m_pPacketEncoder )
		{
			if( NTL_SUCCESS != m_pPacketEncoder->RxDecrypt( *pPacket ) )
			{
				//NETL_LOGDL("Packet RxDecrypt Error");

				SAFE_DELETE( pPacket );
				return false;
			}
		}

		m_recvBuffer.IncreasePopPos( packet.GetPacketLen() );

		m_recvQueue.Push( pPacket );

		++nCompletePacket;

	}
#else
	int rc = NTL_SUCCESS;


#if __SGPRO_BOTSYSTEM_SENDANDRECVTESTCODE__
	wPacketLength = 0;
#else
	WORD wPacketLength = 0;
	CNtlPacket packet;
#endif 

	do 
	{
		rc = MakeSureCompletedPacket( &m_recvBuffer, &wPacketLength, m_pPacketEncoder );
		if( NTL_SUCCESS != rc )
		{
			if( NTL_ERR_NET_PACKET_PENDING_HEADER == rc || NTL_ERR_NET_PACKET_PENDING_DATA == rc )
			{
				break;
			}

			CONNECTION_LOGDL( "MakeSureCompletedPacket Fail : (%d)%s", rc, NtlGetErrorMessage(rc) );
			return -1;
		}


		packet.Attach( m_recvBuffer.GetQueueWorkPtr() );
		if( false == packet.IsValidPacket() )
		{
			CONNECTION_LOGDL( "Packet is Not Valid UsedSize[%u] DataSize[%u]", packet.GetUsedSize(), packet.GetPacketDataSize() );
			return -1;
		}


		m_recvBuffer.IncreaseWorkPos( packet.GetUsedSize() );


		CNtlPacket* pPacket = new CNtlPacket( packet );        
		if( NULL == pPacket )
		{
			CONNECTION_LOGDL( "Packet Memory Allocate Error" );
			return -1;
		}

#if __SGPRO_BOTSYSTEM_SENDANDRECVTESTCODE__
#else
		if( m_pPacketEncoder )
		{
			rc = m_pPacketEncoder->RxDecrypt( *pPacket );
			if( NTL_SUCCESS != rc )
			{
				CONNECTION_LOGDL( "RxDecrypt Fail : (%d)%s", rc, NtlGetErrorMessage(rc) );
				SAFE_DELETE( pPacket );
				return -1;
			}
		}
#endif


		if( pPacket->GetPacketHeader()->packetSequence != WORD( m_dwRecvPacketCount & PACKET_MAX_SEQUENCE ) )
		{
			CONNECTION_LOGDL( "Packet Sequence Error Packet Sequence[%u] != Cur Sequence[%u]", pPacket->GetPacketHeader()->packetSequence, m_dwRecvPacketCount & PACKET_MAX_SEQUENCE  );
			SAFE_DELETE( pPacket );
			return -1;
		}


		m_recvBuffer.IncreasePopPos( pPacket->GetPacketLen() );

		m_recvQueue.Push( pPacket );

		++nCompletePacket;

		++m_dwRecvPacketCount;

		packet.Destroy();

	} while( NTL_SUCCESS == rc );

#endif


	return nCompletePacket;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:	보내야 되는 Send 버퍼사이즈
//-----------------------------------------------------------------------------------
int CServerConnection::SendPendingData()
{
	CNtlPacket * pPacket = m_sendQueue.Peek();

	while( pPacket )	
	{
		if( m_sendBuffer.GetFreeSize() < pPacket->GetUsedSize() )
		{
			break;
		}

		pPacket->GetPacketHeader()->packetSequence = WORD( m_dwSendPacketCount & PACKET_MAX_SEQUENCE );

//#if __SGPRO_BOTSYSTEM_SENDANDRECVTESTCODE__
//#else
//		if( m_pPacketEncoder )
//		{
//			rc = m_pPacketEncoder->TxEncrypt( *pPacket );
//			if( NTL_SUCCESS != rc )
//			{
//				CONNECTION_LOGDL( "TxDecrypt Fail : (%d)%s", rc, NtlGetErrorMessage(rc) );
//				return -1;
//			}
//		}
//#endif

		memcpy( m_sendBuffer.GetQueuePushPtr(), pPacket->GetPacketBuffer(), pPacket->GetUsedSize() );
		m_sendBuffer.IncreasePushPos( pPacket->GetUsedSize() );

		m_sendQueue.Pop();

		SAFE_DELETE( pPacket );

		++m_dwSendPacketCount;

		pPacket = m_sendQueue.Peek();
	} 


	return m_sendBuffer.GetCurSize();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlPacketEncoder * CServerConnection::CreatePacketEncoder(eNETCLIENT_ENCODE_TYPE encode)
{
	/*switch( encode )
	{
	case NETCLIENT_ENCODE_XOR:
		return new CNtlPacketEncoder_XOR;

	case NETCLIENT_ENCODE_RANDKEY:
		return new CNtlPacketEncoder_RandKey( true );
	}*/

	return NULL;
}

//---------------------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------------------
int CServerConnection::MakeSureCompletedPacket(CNtlNetBuffer* pBuffer, WORD* pwPacketLength, CNtlPacketEncoder* pPacketEncoder)
{
	if (NULL == pwPacketLength)
	{
		_ASSERT(0);
		return NTL_ERR_SYS_INPUT_PARAMETER_WRONG;
	}


	// If the header has not been completed
	if (pBuffer->GetWorkRemainSize() < GetHeaderSize())
	{
		return NTL_ERR_NET_PACKET_PENDING_HEADER;
	}


	int nLinearSize = (int)(pBuffer->InGetQueueExtraPtr() - pBuffer->GetQueueWorkPtr());

	// If the header is cut off from the buffer
	if (nLinearSize < GetHeaderSize())
	{
		::CopyMemory(pBuffer->InGetQueueExtraPtr(), pBuffer->InGetQueueBufferPtr(), GetHeaderSize() - nLinearSize);
	}

	if (m_pPacketEncoder)
	{
		//	int rc = m_pPacketEncoder->RxDecrypt( pBuffer->GetQueueWorkPtr() );
		//	if ( NTL_SUCCESS != rc )
		//	{
		//		return rc;
		//	}
	}

	int wPacketLen = GetPacketLen(pBuffer->GetQueueWorkPtr());

	int nPacketSize = GetHeaderSize() + wPacketLen;
	if (nPacketSize >= pBuffer->GetMaxPacketSize())
	{
		CONNECTION_LOGDL("ALLOWED PACKET SIZE"/*, pBuffer->GetMaxPacketSize(), nPacketSize, GetHeaderSize(), wPacketLen, pBuffer->GetWorkRemainSize(), nLinearSize*/);
		return NTL_ERR_NET_PACKET_EXCEED_ALLOWED_SIZE;
	}


	// If the packet is not completed
	if (pBuffer->GetWorkRemainSize() < nPacketSize)
	{
		return NTL_ERR_NET_PACKET_PENDING_DATA;
	}


	//If the packet is truncated in the buffer
	if (nLinearSize < nPacketSize)
	{
		if (nLinearSize < GetHeaderSize())
		{
			::CopyMemory(pBuffer->GetQueueWorkPtr() + GetHeaderSize(),
				pBuffer->InGetQueueBufferPtr() + GetHeaderSize() - nLinearSize,
				wPacketLen);
		}
		else
		{
			::CopyMemory(pBuffer->InGetQueueExtraPtr(),
				pBuffer->InGetQueueBufferPtr(),
				nPacketSize - nLinearSize);
		}
	}

	*pwPacketLength = (WORD)nPacketSize;

	return NTL_SUCCESS;
}



//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CClientNetImp::CClientNetImp(CNtlClientNet * pClientNet)
:
m_pClientNetRef( pClientNet )
{
	Init();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CClientNetImp::~CClientNetImp()
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CClientNetImp::Init()
{
	m_bProgress = false;

	m_hThread = INVALID_HANDLE_VALUE;
	m_dwAliveTime = ALIVE_PACKET_FREQUENCY;

	m_pfnLogCallBack = NULL;
	m_pfnEventCallback = NULL;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CClientNetImp::Destroy()
{
	m_mutex.Lock();
	for(SERVERIT it = m_serverList.begin(); it != m_serverList.end(); it++ )
	{
		RELEASE_CONNECTION( it->second );
	}
	m_serverList.clear();
	m_mutex.Unlock();


	m_bProgress = false;


	if( INVALID_HANDLE_VALUE != m_hThread )
	{
		int rc = ::WaitForSingleObject( m_hThread, INFINITE );
		if( WAIT_FAILED == rc )
		{
			NET_LOGDL( "WaitForSingleObject Fail : (%d)%s", rc, NtlGetErrorMessage(rc) );
		}
		else
		{
			::CloseHandle( m_hThread );
		}

		m_hThread = INVALID_HANDLE_VALUE;
	}


	m_pfnLogCallBack = NULL;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CClientNetImp::OpenThread()
{
	m_hThread = (HANDLE) _beginthreadex(NULL, 0, ThreadMain, this, 0, &m_threadID);
	if( NULL == m_hThread )
	{
		m_hThread = INVALID_HANDLE_VALUE;
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CClientNetImp::Log(const char * lpszFile, const char * lpszFunc, int nLine, bool bDate, const char * lpszText, ...)
{
	if( m_pfnLogCallBack )
	{
		char szLogBuffer[ BUFSIZE_LOG + 1 ] = { 0x00, };
		int nBuffSize = sizeof( szLogBuffer );
		int nWriteSize = 0;


		if( bDate )
		{
			SYSTEMTIME	systemTime;
			GetLocalTime( &systemTime );
			nWriteSize += sprintf_s( szLogBuffer + nWriteSize, nBuffSize - nWriteSize, TEXT("[%d-%02d-%02d %d:%d:%d:%d]\t"), systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds );
		}


		va_list args;
		va_start( args, lpszText );
		nWriteSize += vsprintf_s( szLogBuffer + nWriteSize, nBuffSize - nWriteSize, lpszText, args );
		va_end( args );


		if( lpszFile )
		{
			nWriteSize += _stprintf_s( szLogBuffer + nWriteSize, nBuffSize - nWriteSize, TEXT(" func[%s]\tline[%d]\t"), lpszFile, nLine );
		}


		if( lpszFunc )
		{
			nWriteSize += _stprintf_s( szLogBuffer + nWriteSize, nBuffSize - nWriteSize, TEXT(" function[%s]\t"), lpszFunc );
		}


		m_pfnLogCallBack( szLogBuffer );
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
//
// 컨넥션 상태를 만들고, 이에 따라 전송하도록 할것 (비동기 connect 추가로 인하여 serverlist에 있어도 접속 안된 
// 컨넥션이 있을 수 있다
//
//-----------------------------------------------------------------------------------
void CClientNetImp::DoAlive()
{
	m_mutex.Lock();
	SERVERLIST serverList( m_serverList );
	m_mutex.Unlock();

	for( SERVERIT it = serverList.begin(); it != serverList.end(); it++ )
	{
		PACKETDATA aliveData;
		aliveData.wOpCode = SYS_ALIVE;

		PushPacket( it->first, &aliveData, sizeof(aliveData) );
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CClientNetImp::IsProgress(HSERVER hServer)
{
	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();

	SERVERIT it = m_serverList.find( hServer );
	if( it == m_serverList.end() )
	{
		return false;
	}

	mutex.Unlock();

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CServerConnection * CClientNetImp::Acquire(HSERVER hServer)
{
	CServerConnection * pConnection = NULL;

	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();

	SERVERIT it = m_serverList.find( hServer );
	if( it == m_serverList.end() )
	{
		return NULL;
	}

	pConnection = it->second;
	if( NULL == pConnection )
	{
		return NULL;
	}

	pConnection->Acquire();

	mutex.Unlock();

	return pConnection;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CClientNetImp::Remove(HSERVER hServer)
{
	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();

	SERVERIT it = m_serverList.find( hServer );
	if( it == m_serverList.end() )
	{
		return false;
	}

	RELEASE_CONNECTION( it->second );

	m_serverList.erase( it );

	mutex.Unlock();

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
LPCSTR CClientNetImp::GetServerAddress(HSERVER hServer)
{
	CAutoPtr_ServerConnection pConnection = Acquire( hServer );
	if( false == pConnection.IsValidPtr() )
	{
		return "";
	}

	return pConnection->GetRemoteAddr().GetDottedAddr();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
WORD CClientNetImp::GetServerPort(HSERVER hServer)
{
	CAutoPtr_ServerConnection pConnection = Acquire( hServer );
	if( false == pConnection.IsValidPtr() )
	{
		return 0xFFFF;
	}

	return pConnection->GetRemoteAddr().GetPort();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
LPCSTR CClientNetImp::GetLocalAddress(HSERVER hServer)
{
	CAutoPtr_ServerConnection pConnection = Acquire( hServer );
	if( false == pConnection.IsValidPtr() )
	{
		return "";
	}

	return pConnection->GetLocalAddr().GetDottedAddr();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
DWORD CClientNetImp::GetLocalAddr(HSERVER hServer)
{
	CAutoPtr_ServerConnection pConnection = Acquire( hServer );
	if( false == pConnection.IsValidPtr() )
	{
		return 0;
	}

	return pConnection->GetLocalAddr().GetAddr();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
WORD CClientNetImp::GetLocalPort(HSERVER hServer)
{
	CAutoPtr_ServerConnection pConnection = Acquire( hServer );
	if( false == pConnection.IsValidPtr() )
	{
		return 0xFFFF;
	}

	return pConnection->GetLocalAddr().GetPort();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CMsgBaseNetworkImp::CMsgBaseNetworkImp(CNtlClientNet * pClientNet)
:
CClientNetImp( pClientNet ),
m_hWnd( NULL )
{

}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CMsgBaseNetworkImp::~CMsgBaseNetworkImp()
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CMsgBaseNetworkImp::OnThreadProc()
{
	while( m_bProgress )
	{
		Sleep( m_dwAliveTime );

		DoAlive();
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CMsgBaseNetworkImp::Create(HWND hWnd, NtlClientNetEventCallback EventCallback, NtlClientNetLogCallback LogCallback, DWORD dwAliveTime, int nMaxConnection)
{
	UNREFERENCED_PARAMETER( nMaxConnection );

	if( NULL == hWnd )
	{
		_ASSERT( NULL != hWnd );
		return false;
	}

	m_bProgress = true;

	m_hWnd = hWnd;

	m_pfnEventCallback = EventCallback;

	m_pfnLogCallBack = LogCallback;

	m_dwAliveTime = dwAliveTime;


	if( false == OpenThread() )
	{
		NET_LOGDL( "OpenThread Error" );
		return false;
	}


	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CMsgBaseNetworkImp::Destroy()
{
	//  [10/13/2006 zeroera] : 수정필요 : 먼저 해제되어야만 하는 경우가 있다
	CClientNetImp::Destroy();

	m_hWnd =  NULL;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CMsgBaseNetworkImp::Connect(char* szServerAddress, WORD wPort, HSERVER * pServerHandle, bool bAsync, eNETCLIENT_ENCODE_TYPE encode, int nPacketCount, int nPacketSize)
{
	_ASSERT( NULL != pServerHandle );


	CNtlSocket socket;
	CNtlSockAddr remoteAddr( szServerAddress, htons(wPort) );

	int rc = socket.Create();
	if( NTL_SUCCESS != rc )
	{
		NET_LOGDL( "Socket Create Error : (%d)%s", rc, NtlGetErrorMessage( rc ) );
		return false;
	}

	if( bAsync )
	{
		rc = socket.SetNonBlocking( true );
		if( NTL_SUCCESS != rc )
		{
			NET_LOGDL( "Socket Set NonBlocking Error : (%d)%s", rc, NtlGetErrorMessage( rc ) );
			return false;
		}
	}


	CServerConnection * pConnection = new CServerConnection;
	if( NULL == pConnection )
	{
		NET_LOGDL( "Connection Memory Allocate Error" );
		return false;
	}


	if( false == pConnection->Create( this, socket, remoteAddr, encode, nPacketCount, nPacketSize ) )
	{
		NET_LOGDL( "Connection Create Error remoteAddr [%s]", remoteAddr.GetDottedAddr() );
		RELEASE_CONNECTION( pConnection );
		return false;
	}


	rc = socket.Connect( remoteAddr );
	if( NTL_SUCCESS == rc )
	{
		pConnection->SetState( CServerConnection::CONNECTED );
	}
	else
	{
		if( WSAEWOULDBLOCK == rc )
		{
			pConnection->SetState( CServerConnection::CONNECTING );
		}
		else
		{
			NET_LOGDL( "Socket Connect Error : (%d)%s", rc, NtlGetErrorMessage( rc ) );
			RELEASE_CONNECTION( pConnection );
			return false;
		}
	}



	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();
	if( false == m_serverList.insert( SERVERVAL(pConnection->GetRawSocket(), pConnection) ).second )
	{
		NET_LOGDL( "Connection List Insert Error" );
		RELEASE_CONNECTION( pConnection );
		return false;
	}
	mutex.Unlock();


	// 이미 소켓을 컨넥션에 할당 했으므로 Detach 시켜서 Local Variable 소멸시 SocketClose를 막음
	socket.Detach();

	*pServerHandle = pConnection->GetRawSocket();


	if( SOCKET_ERROR == WSAAsyncSelect( pConnection->GetRawSocket(), m_hWnd, WM_NETWORK_IO, FD_READ | FD_WRITE | FD_CLOSE | FD_CONNECT ) )
	{
		rc = WSAGetLastError();
		NET_LOGDL( "Socket AsyncSelect Error : (%d)%s", rc, NtlGetErrorMessage( rc ) );
		return false;
	}


	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CMsgBaseNetworkImp::Connected(HSERVER hServer)
{
	CAutoPtr_ServerConnection pConnection = Acquire( hServer );
	if( false == pConnection.IsValidPtr() )
	{
		return false;
	}


	pConnection->SetState( CServerConnection::CONNECTED );


	::PostMessage(m_hWnd, WM_CONNECTED, pConnection->GetRawSocket(), hServer);


	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CMsgBaseNetworkImp::Disconnect(HSERVER hServer)
{
	CAutoPtr_ServerConnection pConnection = Acquire( hServer );
	if( false == pConnection.IsValidPtr() )
	{
		return false;
	}

	//왜 있는지 모름.
	//_ASSERT( CServerConnection::CONNECTING == pConnection->GetState() );


	int rc = shutdown( pConnection->GetRawSocket(), SD_BOTH );
	if( ERROR_SUCCESS != rc )
	{
		NET_LOGDL( "Socket shutdown Error : (%d)%s", rc, NtlGetErrorMessage( rc ) );
		return false;
	}


	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CMsgBaseNetworkImp::InternalDisconnect(HSERVER hServer)
{
	CAutoPtr_ServerConnection pConnection = Acquire( hServer );
	if( false == pConnection.IsValidPtr() )
	{
		return false;
	}


	if( pConnection->IsState( CServerConnection::CONNECTING ) )
	{
		::PostMessage(m_hWnd, WM_CONNECT_FAIL, pConnection->GetRawSocket(), hServer);
	}
	else
	{
		::PostMessage(m_hWnd, WM_DISCONNECTED, pConnection->GetRawSocket(), hServer);
	}


	pConnection->SetState( CServerConnection::DISCONNECTED );


	closesocket( pConnection->GetRawSocket() );

	Remove( hServer );


	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CMsgBaseNetworkImp::NetMsgProc(WPARAM wParam, LPARAM lParam)
{
	CAutoPtr_ServerConnection pConnection = Acquire( wParam );
	if( false == pConnection.IsValidPtr() )
	{
		//정상적으로 찾지 못할 경우가 있다.
		//NET_LOGDL( "Socket NetMsgProc Error, Find Error Connection[%u]", wParam );
		return false;
	}


	switch( lParam )
	{
	case FD_CONNECT :
		{
			Connected( wParam );
		}
		break;

	case FD_READ :
		{
			DWORD dwRecvBytes = pConnection->Recv();

			if( SOCKET_ERROR == dwRecvBytes )
			{
				int rc = WSAGetLastError();
				NET_LOGDL( "Connection Recv Error (%d)%s", rc, NtlGetErrorMessage( rc ) );

				InternalDisconnect( pConnection->GetHandle() );
			}
			else if( dwRecvBytes > 0 )
			{
				int nResult = pConnection->RecvPendingData();
				if( nResult > 0 )
				{
					::PostMessage( m_hWnd, WM_INCOMING_PACKET, pConnection->GetRawSocket(), pConnection->GetRawSocket() );
				}
				else if( nResult < 0 )
				{
					InternalDisconnect( pConnection->GetHandle() );
				}
			}
			else
			{
			}
		}
		break;

	case FD_WRITE :
		{
			if( pConnection->SendPendingData() > 0 )
			{
				if( SOCKET_ERROR == pConnection->Send() )
				{
					int rc = WSAGetLastError();
					if( WSAEWOULDBLOCK != rc )
					{
						NET_LOGDL( "Connection Send Error (%d)%s", rc, NtlGetErrorMessage( rc ) );
						InternalDisconnect( pConnection->GetHandle() );
					}
				}
			}
		}
		break;

	case FD_CLOSE :
		{
			InternalDisconnect( wParam );
		}
		break;

	default :
		break;
	}


	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void * CMsgBaseNetworkImp::PeekPacket(HSERVER hServer)
{
	CAutoPtr_ServerConnection pConnection = Acquire( hServer );
	if( false == pConnection.IsValidPtr() )
	{
		return NULL;
	}


	CNtlPacket * pPacket = pConnection->GetRecvQueue()->Peek();
	if( NULL == pPacket )
	{
		return NULL;
	}


	return pPacket->GetPacketData();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CMsgBaseNetworkImp::PopPacket(HSERVER hServer)
{
	CAutoPtr_ServerConnection pConnection = Acquire( hServer );
	if( false == pConnection.IsValidPtr() )
	{
		return;
	}


	CNtlPacket * pPacket = pConnection->GetRecvQueue()->Pop();
	SAFE_DELETE( pPacket );	
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CMsgBaseNetworkImp::PushPacket(HSERVER hServer, void * pData, int nDataSize)
{
	CAutoPtr_ServerConnection pConnection = Acquire( hServer );
	if( false == pConnection.IsValidPtr() )
	{
		return NULL;
	}


	if( false == pConnection->IsState( CServerConnection::CONNECTED ) )
	{
		return false;
	}


	CNtlPacket * pPacket = new CNtlPacket( (BYTE*)pData, (WORD)nDataSize );
	if( false == pPacket->IsValidPacket() )
	{
		NET_LOGDL( "PushPacket Error, Packet wrong" );
		SAFE_DELETE( pPacket );
		return false;
	}


	pConnection->GetSendQueue()->Push( pPacket );

	::PostMessage( m_hWnd, WM_NETWORK_IO, pConnection->GetRawSocket(), FD_WRITE );

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CEventBaseNetworkImp ::CEventBaseNetworkImp(CNtlClientNet * pClientNet)
:
CClientNetImp( pClientNet )
{
	Init();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CEventBaseNetworkImp ::~CEventBaseNetworkImp ()
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CEventBaseNetworkImp ::Init()
{
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CEventBaseNetworkImp ::Create(HWND hWnd, NtlClientNetEventCallback EventCallback, NtlClientNetLogCallback LogCallback, DWORD dwAliveTime, int nMaxConnection)
{
	UNREFERENCED_PARAMETER( hWnd );
	UNREFERENCED_PARAMETER( nMaxConnection );


	m_bProgress = true;

	m_nCurConnection = 0;

	m_pfnEventCallback = EventCallback;

	m_pfnLogCallBack = LogCallback;

	m_dwAliveTime = dwAliveTime;



	if( false == OpenEvent() )
	{
		NET_LOGDL( "OpenEvent Error" );
		return false;
	}


	if( false == OpenThread() )
	{
		NET_LOGDL( "OpenThread Error" );
		return false;
	}


	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
//  [5/4/2006 zeroera] : 수정필요 : 디버그실행시에 종료하는 경우 교착상태 발생
void CEventBaseNetworkImp ::Destroy()
{
	//  [10/13/2006 zeroera] : 수정필요 : 먼저 해제되어야만 하는 경우가 있다
	CClientNetImp::Destroy();

	for( int i = 0; i < m_nCurConnection; i++ )
	{
		WSACloseEvent( m_eventInfo.ahEvent[i] );
	}
	m_nCurConnection = 0;

	for( LISTIT it = m_eventInfo.eventFreeList.begin(); it != m_eventInfo.eventFreeList.end(); it++ )
	{
		WSACloseEvent( *it );
	}
	m_eventInfo.eventFreeList.clear();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
#if __SGPRO_BOTSYSTEM_SENDANDRECVTESTCODE__
bool CEventBaseNetworkImp ::Connect(char* szServerAddress, WORD wPort, HSERVER * pServerHandle, bool bAsync, eNETCLIENT_ENCODE_TYPE encode, int nPacketCount, int nPacketSize, __int64 nBotID )
#else
bool CEventBaseNetworkImp ::Connect(char* szServerAddress, WORD wPort, HSERVER * pServerHandle, bool bAsync, eNETCLIENT_ENCODE_TYPE encode, int nPacketCount, int nPacketSize )
#endif
{
	_ASSERT( NULL != pServerHandle );

	if( false != m_eventInfo.eventFreeList.empty() )
	{
		NET_LOGDL( "There is no more Event Objects" );
		return false;
	}


	CNtlSocket socket;
	CNtlSockAddr remoteAddr( szServerAddress, htons(wPort) );

	int rc = socket.Create();
	if( NTL_SUCCESS != rc )
	{
		NET_LOGDL( "Socket Create Error : (%d)%s", rc, NtlGetErrorMessage( rc ) );
		return false;
	}

	if( bAsync )
	{
		rc = socket.SetNonBlocking( true );
		if( NTL_SUCCESS != rc )
		{
			NET_LOGDL( "Socket Set NonBlocking Error : (%d)%s", rc, NtlGetErrorMessage( rc ) );
			return false;
		}
	}


	CServerConnection * pConnection = new CServerConnection;
	if( NULL == pConnection )
	{
		NET_LOGDL( "Connection Memory Allocate Error" );
		return false;
	}


	if( false == pConnection->Create( this, socket, remoteAddr, encode, nPacketCount, nPacketSize ) )
	{
		NET_LOGDL( "Connection Create Error" );
		RELEASE_CONNECTION( pConnection );
		return false;
	}

//#if __SGPRO_BOTSYSTEM_SENDANDRECVTESTCODE__
//	pConnection->SetCharacterPCID( nBotID );
//	pConnection->CreateLogFile();
//#endif//__SGPRO_BOTSYSTEM_SENDANDRECVTESTCODE__


	rc = socket.Connect( remoteAddr );
	if( NTL_SUCCESS == rc )
	{
		pConnection->SetState( CServerConnection::CONNECTED );
	}
	else
	{
		if( WSAEWOULDBLOCK == rc )
		{
			pConnection->SetState( CServerConnection::CONNECTING );
		}
		else
		{
			NET_LOGDL( "Socket Connect Error : (%d)%s", rc, NtlGetErrorMessage( rc ) );
			RELEASE_CONNECTION( pConnection );
			return false;
		}
	}


	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();
	if( false == m_serverList.insert( SERVERVAL(pConnection->GetRawSocket(), pConnection) ).second )
	{
		NET_LOGDL( "Connection List Insert Error" );
		RELEASE_CONNECTION( pConnection );
		return false;
	}


	// 이미 소켓을 컨넥션에 할당 했으므로 Detach 시켜서 Local Variable 소멸시 SocketClose를 막음
	socket.Detach();
	*pServerHandle = pConnection->GetRawSocket();


	// 이벤트
	int nIndex = m_nCurConnection;

	WSAEVENT newEvent = m_eventInfo.eventFreeList.front();
	m_eventInfo.eventFreeList.pop_front();

	m_eventInfo.ahServer[ nIndex ] = pConnection->GetHandle();
	m_eventInfo.ahEvent[ nIndex ] = newEvent;

	++m_nCurConnection;

	if( SOCKET_ERROR == WSAEventSelect( pConnection->GetRawSocket(), newEvent ,FD_READ | FD_WRITE | FD_CLOSE | FD_CONNECT ) )
	{
		rc = WSAGetLastError();
		NET_LOGDL( "Socket AsyncSelect Error : (%d)%s", rc, NtlGetErrorMessage( rc ) );
		return false;
	}


	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CEventBaseNetworkImp::Connected(HSERVER hServer)
{
	CAutoPtr_ServerConnection pConnection = Acquire( hServer );
	if( false == pConnection.IsValidPtr() )
	{
		return false;
	}


	pConnection->SetState( CServerConnection::CONNECTED );


	if( m_pfnEventCallback )
	{
		m_pfnEventCallback( hServer, NETCLIENT_EVENT_CONNECTED);
	}
	else
	{
		m_pClientNetRef->OnNetEventProc( hServer, NETCLIENT_EVENT_CONNECTED );
	}


	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CEventBaseNetworkImp::Disconnect(HSERVER hServer)
{
	CAutoPtr_ServerConnection pConnection = Acquire( hServer );
	if( false == pConnection.IsValidPtr() )
	{
		return false;
	}


	int rc = shutdown( pConnection->GetRawSocket(), SD_BOTH );
	if( ERROR_SUCCESS != rc )
	{
		NET_LOGDL( "Socket shutdown Error : (%d)%s", rc, NtlGetErrorMessage( rc ) );
		return false;
	}


	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CEventBaseNetworkImp::InternalDisconnect(HSERVER hServer)
{
	CAutoPtr_ServerConnection pConnection = Acquire( hServer );
	if( false == pConnection.IsValidPtr() )
	{
		return false;
	}

	
	if( pConnection->IsState( CServerConnection::CONNECTING ) )
	{
		if( m_pfnEventCallback )
			m_pfnEventCallback( hServer, NETCLIENT_EVENT_CONNECT_FAIL );
		else
			m_pClientNetRef->OnNetEventProc( hServer, NETCLIENT_EVENT_CONNECT_FAIL );
	}
	else
	{
		if( m_pfnEventCallback )
			m_pfnEventCallback( hServer, NETCLIENT_EVENT_DISCONNECTED );
		else
			m_pClientNetRef->OnNetEventProc( hServer, NETCLIENT_EVENT_DISCONNECTED );
	}


	pConnection->SetState( CServerConnection::DISCONNECTED );


	// 해당 컨넥션의 이벤트 해제
	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();

	for( int i = 0; i < m_nCurConnection; i++ )
	{
		if( m_eventInfo.ahServer[ i ] == pConnection->GetHandle() )
		{
			// 이벤트 배열 정리
			m_eventInfo.eventFreeList.push_back( m_eventInfo.ahEvent[ i ] );

			m_eventInfo.ahEvent[ i ] = m_eventInfo.ahEvent[ m_nCurConnection - 1 ];
			m_eventInfo.ahServer[ i ] = m_eventInfo.ahServer[ m_nCurConnection - 1 ];

			m_eventInfo.ahEvent[ m_nCurConnection - 1 ] = INVALID_HANDLE_VALUE;
			m_eventInfo.ahServer[ m_nCurConnection - 1 ] = INVALID_HSERVER;

			--m_nCurConnection;

			break;
		}
	}
	mutex.Unlock();

	Remove( hServer );


	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void * CEventBaseNetworkImp::PeekPacket(HSERVER hServer)
{
	CAutoPtr_ServerConnection pConnection = Acquire( hServer );
	if( false == pConnection.IsValidPtr() )
	{
		return NULL;
	}


	CNtlPacket * pPacket = pConnection->GetRecvQueue()->Peek();
	if( NULL == pPacket )
	{
		return NULL;
	}

	return pPacket->GetPacketData();
}

#if __SGPRO_BOTSYSTEM_SENDANDRECVTESTCODE__
//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void * CEventBaseNetworkImp::SGPeekPacket(HSERVER hServer)
{
	CAutoPtr_ServerConnection pConnection = Acquire( hServer );
	if( false == pConnection.IsValidPtr() )
	{
		return NULL;
	}


	CNtlPacket * pPacket = pConnection->GetRecvQueue()->Peek();
	if( NULL == pPacket )
	{
		return NULL;
	}


	return pPacket; 
}
#endif //end of #if __SGPRO_BOTSYSTEM_SENDANDRECVTESTCODE__

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CEventBaseNetworkImp::PopPacket(HSERVER hServer)
{
	CAutoPtr_ServerConnection pConnection = Acquire( hServer );
	if( false == pConnection.IsValidPtr() )
	{
		return;
	}


	CNtlPacket * pPacket = pConnection->GetRecvQueue()->Pop();
	SAFE_DELETE( pPacket );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------------
bool CEventBaseNetworkImp::PushPacket(HSERVER hServer, void * pData, int nDataSize)
{
	// Application Thread와 Event Thread 양쪽에서 접근 가능하므로 동기화 
	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();

	CAutoPtr_ServerConnection pConnection = Acquire( hServer );
	if( false == pConnection.IsValidPtr() )
	{
		return false;
	}


	if( false == pConnection->IsState( CServerConnection::CONNECTED ) )
	{
		return false;
	}

	CNtlPacket * pPacket = new CNtlPacket( (BYTE*)pData, (WORD)nDataSize );
	if( false == pPacket->IsValidPacket() )
	{
		NET_LOGDL( "PushPacket Error, Packet wrong" );
		SAFE_DELETE( pPacket );
		return false;
	}

	pConnection->GetSendQueue()->Push( pPacket );

	if( pConnection->SendPendingData() > 0 )
	{
		if( SOCKET_ERROR == pConnection->Send() )
		{
			int rc = WSAGetLastError();
			if( WSAEWOULDBLOCK != rc )
			{
				NET_LOGDL( "Connection Send Error, (%d)%s", rc, NtlGetErrorMessage( rc ) );
				InternalDisconnect( pConnection->GetHandle() );
			}
		}
	}


	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	: 
//		Return	:
//-----------------------------------------------------------------------------------
void CEventBaseNetworkImp::OnThreadProc()
{
	DWORD dwIndex = 0;
	WSANETWORKEVENTS wsaNetworkEvents;


	DWORD dwTickOld = 0;
	DWORD dwTickCur = 0;
	dwTickOld = dwTickCur = GetTickCount();

	while( m_bProgress )
	{
		Sleep(1);

		DWORD dwEventTotal = (DWORD) m_nCurConnection;

		//  [5/23/2007 zeroera] : EventTotal 0 When I come in, I get an error in Windows 2000
		if( 0 == dwEventTotal )
		{
			Sleep( 1000 );
			continue;
		}


		dwIndex = WSAWaitForMultipleEvents(	dwEventTotal,
			m_eventInfo.ahEvent,
			FALSE,
			1000,
			FALSE );



		dwTickCur = GetTickCount();
		if( dwTickCur - dwTickOld >= m_dwAliveTime )
		{
			DoAlive();	
			dwTickOld = dwTickCur;
		}


		if( WSA_WAIT_FAILED == dwIndex )
		{
			int rc = WSAGetLastError();
			NET_LOGDL( "WSAWaitForMultipleEvents Fail (%d)%s", rc, NtlGetErrorMessage(rc) );
			continue;
		}


		if( WSA_WAIT_TIMEOUT == dwIndex )
		{
			continue;
		}


		//dwIndex = dwIndex - WSA_WAIT_EVENT_0;


		//// 선점순서에 의한 불공평항 event 처리 보정
		//for( DWORD i = dwIndex; i < dwEventTotal; i++ )
		//{
		//	dwIndex = WSAWaitForMultipleEvents( 1, &m_eventInfo.ahEvent[i], FALSE, 0, FALSE );
		//	if( dwIndex == WAIT_FAILED || dwIndex == WSA_WAIT_TIMEOUT )
		//	{
		//		continue;
		//	}

		DWORD i = dwIndex;

		CAutoPtr_ServerConnection pConnection = Acquire( m_eventInfo.ahServer[i] );
		if( false == pConnection.IsValidPtr() )
		{
			NET_LOGDL( "Connection Find Error" );
			continue;					
		}

		if( SOCKET_ERROR == WSAEnumNetworkEvents( m_eventInfo.ahServer[i], m_eventInfo.ahEvent[i], &wsaNetworkEvents ) )
		{
			int rc = WSAGetLastError();
			NET_LOGDL( "WSAEnumNetworkEvents Fail (%d)%s", rc, NtlGetErrorMessage(rc) );
			continue;
		}


		if( wsaNetworkEvents.lNetworkEvents & FD_CONNECT )
		{
			if( 0 != wsaNetworkEvents.iErrorCode[FD_CONNECT_BIT] )
			{					
				int rc = wsaNetworkEvents.iErrorCode[FD_CONNECT_BIT];
				NET_LOGDL( "WSAEnumNetworkEvents FD_CONNECT_BIT Fail (%d)%s", rc, NtlGetErrorMessage(rc) );

				InternalDisconnect( pConnection->GetHandle() );					
			}
			else
			{
				Connected( pConnection->GetHandle() );
			}
		}


		if( wsaNetworkEvents.lNetworkEvents & FD_READ )
		{
			if( 0 != wsaNetworkEvents.iErrorCode[FD_READ_BIT] )
			{
				int rc = wsaNetworkEvents.iErrorCode[FD_READ_BIT];
				NET_LOGDL( "WSAEnumNetworkEvents FD_READ Fail (%d)%s", rc, NtlGetErrorMessage(rc) );
				continue;						
			}

			DWORD dwRecvBytes = pConnection->Recv();

			if( SOCKET_ERROR == dwRecvBytes )
			{
				int rc = WSAGetLastError();
				NET_LOGDL( "Connection Recv Error, (%d)%s", rc, NtlGetErrorMessage( rc ) );

				InternalDisconnect( pConnection->GetHandle() );
			}
			else if( dwRecvBytes > 0 )
			{
				int nResult = pConnection->RecvPendingData();
				if( nResult < 0 )
				{
					NET_LOGDL( "Connection RecvPendingData Error, Result(%d)", nResult );
					InternalDisconnect( pConnection->GetHandle() );
				}
			}
			else
			{
			}
		}


		if( wsaNetworkEvents.lNetworkEvents & FD_WRITE )
		{
			if( 0 != wsaNetworkEvents.iErrorCode[FD_WRITE_BIT] )
			{
				int rc = wsaNetworkEvents.iErrorCode[FD_WRITE_BIT];
				NET_LOGDL( "WSAEnumNetworkEvents FD_WRITE Fail (%d)%s", rc, NtlGetErrorMessage(rc) );
				continue;						
			}

			/*				if( pConnection->SendPendingData() > 0 )
			{
			if( SOCKET_ERROR == pConnection->Send() )
			{
			int rc = WSAGetLastError();
			if( WSAEWOULDBLOCK != rc )
			{
			NETL_LOGDL("Connection Send Error, (%d)%s", rc, NtlGetErrorMessage( rc ) );
			InternalDisconnect( pConnection->GetHandle() );
			}
			}
			}
			*/
		}


		if( wsaNetworkEvents.lNetworkEvents & FD_CLOSE )
		{
			if( 0 != wsaNetworkEvents.iErrorCode[FD_CLOSE_BIT] )
			{
				int rc = wsaNetworkEvents.iErrorCode[FD_CLOSE_BIT];
				NET_LOGDL( "WSAEnumNetworkEvents FD_CLOSE_BIT Fail (%d)%s", rc, NtlGetErrorMessage(rc) );
			}

			InternalDisconnect( pConnection->GetHandle() );
		}


		//} // end of for

	} // end of while
}


//-----------------------------------------------------------------------------------
//		Purpose	: WSAEventSelect에 사용할 Event를 생성
//		Return	:
//-----------------------------------------------------------------------------------
bool CEventBaseNetworkImp::OpenEvent()
{
	for( int i = 0; i < WSA_MAXIMUM_WAIT_EVENTS; i++ )
	{
		WSAEVENT openEvent = WSACreateEvent();

		m_eventInfo.eventFreeList.push_back( openEvent );
	}

	return true;
}


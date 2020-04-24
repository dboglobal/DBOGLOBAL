//***********************************************************************************
//
//	File		:	NtlSocket.cpp
//
//	Begin		:	2005-12-13
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Socket Class
//
//***********************************************************************************

#include "stdafx.h"
#include "NtlSocket.h"

#include <mstcpip.h>


//-----------------------------------------------------------------------------------
// static variable
//-----------------------------------------------------------------------------------
LPFN_ACCEPTEX				CNtlSocket::m_lpfnAcceptEx				= NULL;
LPFN_CONNECTEX				CNtlSocket::m_lpfnConnectEx				= NULL;
LPFN_DISCONNECTEX			CNtlSocket::m_lpfnDisconnectEx			= NULL;
LPFN_GETACCEPTEXSOCKADDRS	CNtlSocket::m_lpfnGetAcceptExSockAddrs	= NULL;
LPFN_TRANSMITFILE			CNtlSocket::m_lpfnTransmitFile			= NULL;
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlSocket::CNtlSocket()
{
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlSocket::~CNtlSocket()
{
	Close();
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlSocket::StartUp()
{
	WSADATA wsaData;

	if ( 0 != WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) )
	{
		// Winsock DLL 못 찾음
		return WSAGetLastError();
	}

	// Winsock이 2.2를 지원하는지 확인
	if ( LOBYTE( wsaData.wVersion ) != 2 ||	HIBYTE( wsaData.wVersion ) != 2 )
	{
		WSACleanup();
		return WSAGetLastError(); 
	}


	// Load MS Winsock Extension API 
	int nResult = LoadExtensionAPI();
	if( NTL_SUCCESS != nResult )
	{
		WSACleanup();
		return nResult;
	}

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlSocket::CleanUp()
{
	if( 0 != WSACleanup() )
		return WSAGetLastError();

	return NTL_SUCCESS;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlSocket::LoadExtensionAPI()
{
	int nResult = 0;

	// AcceptEx Load
	GUID funcAcceptEx = WSAID_ACCEPTEX;
	nResult = LoadExtensionFunction( funcAcceptEx , (LPVOID*) &m_lpfnAcceptEx );
	if( NTL_SUCCESS != nResult )
		return nResult;

	// ConnectEx Load
	GUID funcConnectEx = WSAID_CONNECTEX;
	nResult = LoadExtensionFunction( funcConnectEx, (LPVOID*) &m_lpfnConnectEx );
	if( NTL_SUCCESS != nResult )
		return nResult;

	// DisconnectEx
	GUID funcDisconnectEx = WSAID_DISCONNECTEX;
	nResult = LoadExtensionFunction( funcDisconnectEx, (LPVOID*) &m_lpfnDisconnectEx );
	if( NTL_SUCCESS != nResult )
		return nResult;

	// GetAcceptExAddr
	GUID funcGetAcceptExAddr = WSAID_GETACCEPTEXSOCKADDRS;
	nResult = LoadExtensionFunction( funcGetAcceptExAddr, (LPVOID*) &m_lpfnGetAcceptExSockAddrs );
	if( NTL_SUCCESS != nResult )
		return nResult;

	// TransmitFile
	GUID funcTransmitFile = WSAID_TRANSMITFILE;
	nResult = LoadExtensionFunction( funcTransmitFile, (LPVOID*) &m_lpfnTransmitFile );
	if( NTL_SUCCESS != nResult )
		return nResult;

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlSocket::LoadExtensionFunction(GUID functionID, LPVOID *pFunc)
{
	CNtlSocket socket;
	if( 0 != socket.Create(CNtlSocket::eSOCKET_TCP) )
		return WSAGetLastError();

	DWORD dwBytes = 0;
	int nResult = WSAIoctl(	socket,
		SIO_GET_EXTENSION_FUNCTION_POINTER,
		&functionID,
		sizeof(GUID),
		pFunc,
		sizeof(LPVOID),
		&dwBytes,
		0,
		0);

	if( 0 != nResult )
		return WSAGetLastError();

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlSocket::Create(int nSocketType)
{
	UNREFERENCED_PARAMETER( nSocketType );

	m_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if( INVALID_SOCKET == m_socket )
		return WSAGetLastError();

	return NTL_SUCCESS;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlSocket::Bind(CNtlSockAddr& rSockAddr)
{
	if( 0 != bind(m_socket, (struct sockaddr *) rSockAddr, sizeof(struct sockaddr) ) )
	{
		return WSAGetLastError();
	}

	return NTL_SUCCESS;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlSocket::Listen(int nBackLog)
{
	if( 0 != listen(m_socket, nBackLog) )
	{
		return WSAGetLastError();
	}

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlSocket::Close()
{
	if( INVALID_SOCKET == m_socket )
	{
		return NTL_SUCCESS;
	}

	if( SOCKET_ERROR == closesocket( m_socket ) )
	{
		return WSAGetLastError();
	}

	m_socket = INVALID_SOCKET;

	return NTL_SUCCESS;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlSocket::Shutdown(int how)
{
	if( INVALID_SOCKET == m_socket )
	{
		return NTL_SUCCESS;
	}

	if( SOCKET_ERROR == shutdown( m_socket, how ) )
	{
		return WSAGetLastError();
	}

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlSocket::GetPeerName(CNtlString & rAddress, WORD & rPort)
{
	struct sockaddr_in sockAddr;
	int nSockAddrLen = sizeof(sockAddr);

	if( 0 != getpeername(m_socket, (struct sockaddr*) &sockAddr, &nSockAddrLen) )
	{
		return WSAGetLastError();
	}

	rAddress	= inet_ntoa( sockAddr.sin_addr );
	rPort		= ntohs( sockAddr.sin_port );

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlSocket::GetLocalName(CNtlString & rAddress, WORD & rPort)
{
	struct sockaddr_in sockAddr;
	int nSockAddrLen = sizeof(sockAddr);

	if( 0 != getsockname(m_socket, (struct sockaddr*) &sockAddr, &nSockAddrLen) )
	{
		return WSAGetLastError();
	}

	rAddress	= inet_ntoa( sockAddr.sin_addr );
	rPort		= ntohs( sockAddr.sin_port );

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlSocket::GetPeerAddr(CNtlSockAddr & rAddr)
{
	struct sockaddr_in sockAddr;
	int nSockAddrLen = sizeof(sockAddr);

	if( 0 != getpeername(m_socket, (struct sockaddr*) &sockAddr, &nSockAddrLen) )
	{
		return WSAGetLastError();
	}

	rAddr.SetSockAddr( inet_ntoa( sockAddr.sin_addr ), ntohs( sockAddr.sin_port ) ); 

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlSocket::GetLocalAddr(CNtlSockAddr & rAddr)
{
	struct sockaddr_in sockAddr;
	int nSockAddrLen = sizeof(sockAddr);

	if( 0 != getsockname(m_socket, (struct sockaddr*) &sockAddr, &nSockAddrLen) )
	{
		return WSAGetLastError();
	}

	rAddr.SetSockAddr( inet_ntoa( sockAddr.sin_addr ), ntohs( sockAddr.sin_port ) ); 

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlSocket::SetNonBlocking(BOOL bActive)
{
	unsigned long i = bActive;
	int result = ioctlsocket( m_socket, FIONBIO, &i );

	if( SOCKET_ERROR == result )
		return WSAGetLastError();

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlSocket::SetReuseAddr(BOOL bActive)
{
	int result = setsockopt( m_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&bActive, sizeof(bActive) );

	if( SOCKET_ERROR == result )
		return WSAGetLastError();	

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlSocket::SetLinger(BOOL bActive, WORD wTime)
{
	struct linger so_linger;

	so_linger.l_onoff	= (u_short)bActive;
	so_linger.l_linger	= wTime;

	int result = setsockopt( m_socket, SOL_SOCKET, SO_LINGER, (char *)&so_linger, sizeof(so_linger) );

	if( SOCKET_ERROR == result )
		return WSAGetLastError();	

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlSocket::SetTCPNoDelay(BOOL bActive)
{
	int result = setsockopt( m_socket, IPPROTO_TCP, TCP_NODELAY, (char*)&bActive, sizeof(bActive) );

	if( SOCKET_ERROR == result )
		return WSAGetLastError();

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlSocket::SetKeepAlive(BOOL bActive)
{
	int result = setsockopt( m_socket, SOL_SOCKET, SO_KEEPALIVE, (char*)&bActive, sizeof(bActive) );

	if( SOCKET_ERROR == result )
		return WSAGetLastError();

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlSocket::SetKeepAlive(DWORD dwKeepAliveTime, DWORD dwKeepAliveInterval)
{
	tcp_keepalive keepAlive = { TRUE, dwKeepAliveTime, dwKeepAliveInterval };

	DWORD dwBytesReturned;
	int result = WSAIoctl( m_socket, SIO_KEEPALIVE_VALS, &keepAlive, sizeof(keepAlive), 0, 0, &dwBytesReturned, NULL, NULL );

	if( SOCKET_ERROR == result )
		return WSAGetLastError();

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
//  [1/5/2007 zeroera] : 설명 : Accept가 실제로 일어나지 않으면 클라이언트에 ACK + SYN가 가지 않게되어
// BackLog로 인한 선행 Connect가 일어나지 않는다.
// 일정 시간이내에 Accept가 호출되지 않으면 클라이언트는 TIMEOUT 된다
// performance decrease 가 있었음 : 현재는 윈도우 패치로 수정 됌
//-----------------------------------------------------------------------------------
int CNtlSocket::SetConditionalAccept(BOOL bActive)
{
	int result = setsockopt( m_socket, SOL_SOCKET, SO_CONDITIONAL_ACCEPT, (char*)&bActive, sizeof(bActive) );

	if( SOCKET_ERROR == result )
		return WSAGetLastError();

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlSocket::GetCurReadSocketBuffer()
{
	unsigned long nRead = 0;

	ioctlsocket( m_socket, FIONREAD , &nRead );

	return nRead;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlSocket::Connect(struct sockaddr_in * sockaddr)
{
	int rc = connect(m_socket, (struct sockaddr *)sockaddr, sizeof(struct sockaddr_in));
	if( SOCKET_ERROR == rc )    
	{
		rc = WSAGetLastError();
		return rc;
	}

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		FuncName	:
//		Desc		: 
//		Parameter	:
//		Return		:
//-----------------------------------------------------------------------------------
int CNtlSocket::SendStream(unsigned char *pSendBuffer, int nSendSize, bool bSendOut)
{
	int nResult = 0;
	bool bProcess = true;
	BYTE * pBuffer = pSendBuffer;

	if( bSendOut )
	{
		while( bProcess )
		{
			nResult = send( m_socket, (const char *) pBuffer, nSendSize, 0 );

			if( SOCKET_ERROR == nResult )
			{
				bProcess = false;
				return SOCKET_ERROR;
			}
			else if( nResult < nSendSize )
			{
				pBuffer += nResult;
				nSendSize -= nResult;

				bProcess = true;       			    		
			}
			else
			{
				bProcess = false;
			}
		}
	}
	else
	{
		nResult = send( m_socket, (const char *) pBuffer, nSendSize, 0 );

		if( SOCKET_ERROR == nResult )
		{
			return SOCKET_ERROR;
		}
	}

	return nResult;	
}


//-----------------------------------------------------------------------------------
//		FuncName	:
//		Desc		: 
//		Parameter	:
//		Return		:
//-----------------------------------------------------------------------------------
int CNtlSocket::RecvStream(BYTE * pRecvBuffer, int nRecvSize)
{
	return recv( m_socket, (char *) pRecvBuffer, nRecvSize, 0 );
}
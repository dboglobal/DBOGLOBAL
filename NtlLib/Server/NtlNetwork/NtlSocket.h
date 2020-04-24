//***********************************************************************************
//
//	File		:	NtlSocket.h
//
//	Begin		:	2005-12-13
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Socket Class
//
//***********************************************************************************

#pragma once

#include <mswsock.h>
#include "NtlSockAddr.h"
#include "NtlString.h"

class CNtlSocket
{
public:
	enum
	{
		eSOCKET_TCP = 0,
		eSOCKET_UDP,
	};

public:

	CNtlSocket();

	virtual ~CNtlSocket();


public:

	static int							StartUp();

	static int							CleanUp();


protected:

	static int							LoadExtensionAPI();

	static int							LoadExtensionFunction(GUID functionID, LPVOID *pFunc);


public:

	int									Create(int nSocketType = 0);

	int									Bind(CNtlSockAddr& rSockAddr);

	int									Listen(int nBackLog = SOMAXCONN);

	int									Close();

	int									Shutdown(int how);

	int									Connect(struct sockaddr_in * sockaddr);

	int									SendStream(BYTE * pSendBuffer, int nSendSize, bool bSendOut);

	int									RecvStream(BYTE * pRecvBuffer, int nRecvSize);


	int									AcceptEx(CNtlSocket &rAcceptSocket, PVOID lpOutputBuffer, DWORD dwReceiveDataLength, DWORD dwLocalAddressLength, DWORD dwRemoteAddressLength, LPDWORD lpdwBytesReceived, LPOVERLAPPED lpOverlapped);

	int									ConnectEx(const struct sockaddr* name, int namelen, PVOID lpSendBuffer, DWORD dwSendDataLength, LPDWORD lpdwBytesSent, LPOVERLAPPED lpOverlapped);

	int									DisconnectEx(LPOVERLAPPED lpOverlapped, DWORD dwFlags, DWORD reserved);

	void								GetAcceptExSockaddrs(PVOID lpOutputBuffer, DWORD dwReceiveDataLength, DWORD dwLocalAddressLength, DWORD dwRemoteAddressLength, LPSOCKADDR* LocalSockaddr, LPINT LocalSockaddrLength, LPSOCKADDR* RemoteSockaddr, LPINT RemoteSockaddrLength);

	int									RecvEx(LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags, LPWSAOVERLAPPED lpOverlapped);

	int									SendEx(LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, LPWSAOVERLAPPED lpOverlapped);



	void								Attach(SOCKET socket) { m_socket = socket; }

	void								Detach() { m_socket = INVALID_SOCKET; }


	int									GetPeerName(CNtlString & rAddress, WORD & rPort);

	int									GetLocalName(CNtlString & rAddress, WORD & rPort);

	int									GetPeerAddr(CNtlSockAddr & rAddr);

	int									GetLocalAddr(CNtlSockAddr & rAddr);


	SOCKET								GetRawSocket() { return m_socket; }

	bool								IsCreated() { return INVALID_SOCKET != m_socket; }


	operator							SOCKET() { return *((SOCKET *)&m_socket); }

	CNtlSocket &						operator=(const CNtlSocket & rhs);


public:

	int									SetOption(DWORD dwSockOption);

	int									SetNonBlocking(BOOL bActive);

	int									SetReuseAddr(BOOL bActive);

	int									SetLinger(BOOL bActive, WORD wTime);

	int									SetTCPNoDelay(BOOL bActive);

	int									SetKeepAlive(BOOL bActive);

	int									SetKeepAlive(DWORD dwKeepAliveTime, DWORD dwKeepAliveInterval);

	int									SetConditionalAccept(BOOL bActive);

	int									GetCurReadSocketBuffer();




protected:


	SOCKET								m_socket;

	static LPFN_ACCEPTEX				m_lpfnAcceptEx;

	static LPFN_CONNECTEX				m_lpfnConnectEx;

	static LPFN_DISCONNECTEX			m_lpfnDisconnectEx;

	static LPFN_GETACCEPTEXSOCKADDRS	m_lpfnGetAcceptExSockAddrs;

	static LPFN_TRANSMITFILE			m_lpfnTransmitFile;

};


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline int CNtlSocket::AcceptEx(CNtlSocket &rAcceptSocket, PVOID lpOutputBuffer, DWORD dwReceiveDataLength, DWORD dwLocalAddressLength, DWORD dwRemoteAddressLength, LPDWORD lpdwBytesReceived, LPOVERLAPPED lpOverlapped)
{
	if( !m_lpfnAcceptEx( m_socket, rAcceptSocket.GetRawSocket(), lpOutputBuffer, dwReceiveDataLength, dwLocalAddressLength, dwRemoteAddressLength, lpdwBytesReceived, lpOverlapped ) )
	{
		int rc = WSAGetLastError();
		if( ERROR_IO_PENDING !=  rc )
		{
			return rc;
		}
	}

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline int CNtlSocket::ConnectEx(const struct sockaddr* name, int namelen, PVOID lpSendBuffer, DWORD dwSendDataLength, LPDWORD lpdwBytesSent, LPOVERLAPPED lpOverlapped)
{
	if( !m_lpfnConnectEx( m_socket, name, namelen, lpSendBuffer, dwSendDataLength, lpdwBytesSent, lpOverlapped ) )
	{
		int rc = WSAGetLastError();
		if( ERROR_IO_PENDING !=  rc )
		{
			return rc;
		}
	}

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline int CNtlSocket::DisconnectEx(LPOVERLAPPED lpOverlapped, DWORD dwFlags, DWORD reserved)
{
	if( !m_lpfnDisconnectEx( m_socket, lpOverlapped, dwFlags, reserved) )
	{
		int rc = WSAGetLastError();
		if( ERROR_IO_PENDING !=  rc )
		{
			return rc;
		}
	}

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline void CNtlSocket::GetAcceptExSockaddrs(PVOID lpOutputBuffer, DWORD dwReceiveDataLength, DWORD dwLocalAddressLength, DWORD dwRemoteAddressLength, LPSOCKADDR* LocalSockaddr, LPINT LocalSockaddrLength, LPSOCKADDR* RemoteSockaddr, LPINT RemoteSockaddrLength)
{
	m_lpfnGetAcceptExSockAddrs(lpOutputBuffer, dwReceiveDataLength, dwLocalAddressLength, dwRemoteAddressLength, LocalSockaddr, LocalSockaddrLength, RemoteSockaddr, RemoteSockaddrLength);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline int CNtlSocket::RecvEx(LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags, LPWSAOVERLAPPED lpOverlapped)
{
	if( 0 != ::WSARecv( m_socket, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpOverlapped, NULL) )
	{
		int rc = WSAGetLastError();
		if( ERROR_IO_PENDING !=  rc )
		{
			return rc;
		}
	}

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline int CNtlSocket::SendEx(LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, LPWSAOVERLAPPED lpOverlapped)
{
	if( 0 != ::WSASend( m_socket, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpOverlapped, NULL) )
	{
		int rc = WSAGetLastError();
		if( ERROR_IO_PENDING !=  rc )
		{
			return rc;
		}
	}

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline CNtlSocket & CNtlSocket::operator=(const CNtlSocket & rhs)
{
	m_socket = rhs.m_socket;
	return *this;
}


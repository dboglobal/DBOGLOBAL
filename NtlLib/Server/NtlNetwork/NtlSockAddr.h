//***********************************************************************************
//
//	File		:	NtlSockAddr.h
//
//	Begin		:	2005-12-13
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Socke Address Class
//
//***********************************************************************************

#pragma once


#include "NtlString.h"

class CNtlSockAddr
{
public: 

	CNtlSockAddr()
		:m_bInit(false) { memset( &m_sockAddr, 0x00, sizeof(m_sockAddr) ); }


	CNtlSockAddr(unsigned long addr, unsigned short port)
		:m_bInit(false)
	{
		m_sockAddr.sin_family = AF_INET;
		m_sockAddr.sin_port = port;
		m_sockAddr.sin_addr.s_addr = addr;
	}


	CNtlSockAddr(const char * addr, unsigned short port)
		:m_bInit(false)
	{
		m_sockAddr.sin_family = AF_INET;
		m_sockAddr.sin_port = port;

		struct hostent *remoteHost = gethostbyname(addr);
		if ( NULL == remoteHost )
		{
			m_sockAddr.sin_addr.s_addr = inet_addr(addr);
		}
		else
		{
			m_sockAddr.sin_addr.s_addr = *(u_long*)remoteHost->h_addr_list[0];
		}
	}


	CNtlSockAddr(const CNtlSockAddr & sockAddr)
		:m_sockAddr(sockAddr.m_sockAddr), m_bInit(false) {}


	CNtlSockAddr(const sockaddr & sockAddr)
		:m_sockAddr(*(sockaddr_in*)&sockAddr), m_bInit(false) {}


	CNtlSockAddr(const sockaddr_in & sockAddr)
		:m_sockAddr(sockAddr), m_bInit(false) {}


	~CNtlSockAddr() {}



public:


	// Operation

	//
	void SetSockAddr(const char * addr, unsigned short port)
	{
		m_sockAddr.sin_family		= AF_INET;
		m_sockAddr.sin_port			= port;
		m_sockAddr.sin_addr.s_addr	= inet_addr(addr);
	}


	//
	void Clear()
	{
		memset(&m_sockAddr, '\0', sizeof(sockaddr_in));
		m_bInit = false;
		m_strAddr.clear();
	}


	//
	unsigned short GetPort() const
	{
		return ntohs(m_sockAddr.sin_port);
	}


	//
	void SetPort(const unsigned short port)
	{
		m_sockAddr.sin_port = htons(port);
	}


	//
	unsigned int GetAddr() const
	{
		return m_sockAddr.sin_addr.s_addr;
	}


	//
	const char * GetDottedAddr()
	{
		if( !m_bInit )
		{
			m_strAddr = InetNtoA();
			m_bInit = true;
		}

		return m_strAddr.c_str();
	}


	//
	void SetAddr(const unsigned int addr)
	{
		m_sockAddr.sin_addr.s_addr = htonl(addr);
	}


	//
	const char * InetNtoA()
	{
		return inet_ntoa(m_sockAddr.sin_addr);
	}


	//
	CNtlSockAddr & operator=(const CNtlSockAddr & rhs)
	{
		if(this != &rhs)
		{
			m_sockAddr = rhs.m_sockAddr;
			m_bInit = rhs.m_bInit;
			m_strAddr = rhs.m_strAddr;
		}

		return *this;
	}


	//
	CNtlSockAddr & operator=(sockaddr & sockAddr)
	{
		m_sockAddr = *(sockaddr_in*)&sockAddr;
		m_bInit = false;
		m_strAddr.clear();

		return *this;
	}


	//
	CNtlSockAddr & operator=(sockaddr_in & sockAddr)
	{
		m_sockAddr = sockAddr;
		m_bInit = false;
		m_strAddr.clear();

		return *this;
	}


	//
	operator sockaddr()
	{
		return *((sockaddr *)&m_sockAddr); 
	}


	//
	operator sockaddr_in()
	{
		return *((sockaddr_in *)&m_sockAddr);
	}


	//
	operator sockaddr*()
	{
		return (sockaddr *)&m_sockAddr;
	}


	//
	operator sockaddr_in*()
	{
		return (sockaddr_in *)&m_sockAddr;
	}


	//operator SOCKADDR() { return *((sockaddr *)&m_sockAddr); }


	//operator SOCKADDR_IN() { return *((sockaddr_in *)&m_sockAddr); }


private:

	sockaddr_in m_sockAddr;	// real socket address

	std::string	m_strAddr;

	bool		m_bInit;

};


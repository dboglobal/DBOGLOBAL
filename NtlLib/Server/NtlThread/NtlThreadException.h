//***********************************************************************************
//
//	File		:	NtlThreadException.h
//
//	Begin		:	2005-11-30
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Thread 관련 예외처리 Class
//
//***********************************************************************************

#ifndef __NTLTHREADEXCEPTION_H__
#define __NTLTHREADEXCEPTION_H__


#include "NtlException.h"
#include "NtlError.h"
#include "NtlString.h"

enum eTHREAD_ERROR
{
	eTHREAD_ERR_UNEXPECTED = 1,
	eTHREAD_ERR_MUTEX_CREATE,
	eTHREAD_ERR_MUTEX_LOCK,
	eTHREAD_ERR_MUTEX_UNLOCK,
	eTHREAD_ERR_EVENT_CREATE,
	eTHREAD_ERR_EVENT_DESTROY,
	eTHREAD_ERR_EVENT_RESET,
	eTHREAD_ERR_EVENT_WAIT,
	eTHREAD_ERR_EVENT_NOTIFY,
	eTHREAD_ERR_MONITOR_STATE,
	eTHREAD_ERR_MONITOR_DESTROYED,
	eTHREAD_ERR_MONITOR_BUSY,
	eTHREAD_ERR_THREAD_CREATE,
	eTHREAD_ERR_THREAD_JOIN,
	eTHREAD_ERR_TLS_KEY_CREATE,
	eTHREAD_ERR_TLS_SET_VALUE,

	MAX_THREAD_ERROR
};

#define THROW_THREAD_EXCEPTION(c, rc) throw CNtlThreadException(__FILE__, __LINE__, __FUNCTION__, c, rc)

class CNtlThreadException : public CNtlException
{
public:

	CNtlThreadException(const char *filename, int line, const char * func, int category, int errorCode)
		:CNtlException(filename, line, func, ""), m_category(category), m_errorCode(errorCode)
	{
		::NtlGetErrorString( m_strWhat, m_errorCode );
	}

	virtual ~CNtlThreadException(void) {}


protected:

	int			m_category;

	int			m_errorCode;	

};

#endif // __NTLTHREADEXCEPTION_H__

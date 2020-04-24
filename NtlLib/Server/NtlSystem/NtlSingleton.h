//***********************************************************************************
//
//	File		:	NtlSingleton.h
//
//	Begin		:	2006-01-05 Update 1.9.2016
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#ifndef __NTLSINGLETON_H__
#define __NTLSINGLETON_H__

#include "NtlDebug.h"

template <typename TYPE> class CNtlSingleton
{
public:

	static TYPE * m_pInstance;

	CNtlSingleton()
	{
		NTL_ASSERT( NULL == m_pInstance );

		m_pInstance = static_cast<TYPE*>(this);
	}

	virtual ~CNtlSingleton()
	{
		NTL_ASSERT(NULL != m_pInstance);
		m_pInstance = NULL;
	}


	static TYPE & instance()
	{
		assert(m_pInstance);
		return (*m_pInstance);
	}

	static TYPE & Instance()
	{
		assert(m_pInstance);
		return (*m_pInstance);
	}

	static TYPE * instance_ptr()
	{
		return (m_pInstance);
	}

	static TYPE* GetInstance()
	{
		return (m_pInstance);
	}


};

template <typename TYPE> TYPE* CNtlSingleton<TYPE>::m_pInstance = NULL;



#endif // __NTLSINGLETON_H__

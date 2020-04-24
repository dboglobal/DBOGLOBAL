//***********************************************************************************
//
//	File		:	NtlProfiler.cpp
//
//	Begin		:	2006-07-10
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#include "stdafx.h"
#include "NtlError.h"

#include "NtlProfiler.h"
#include "NtlDebug.h"


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline void GetProfileTickCount(__int64 * pCounter)
{
	QueryPerformanceCounter( (LARGE_INTEGER *) pCounter );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline float GetProfileTickRate()
{
	static float fFrequency = -1.0f;

	if( -1.0f == fFrequency )
	{
		__int64 frequence = 0;
		QueryPerformanceFrequency ( (LARGE_INTEGER *) &frequence );
		fFrequency = (float) frequence;
	} 

	return fFrequency;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlProfileNode::CNtlProfileNode(const char * lpszNodeName, CNtlProfileNode * pParent)
:
m_lpszNodeName( lpszNodeName ),
m_pParent( pParent )
{
	Init();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlProfileNode::~CNtlProfileNode()
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlProfileNode::Init()
{
	m_dwTotalCalls = 0;

	m_fTotalTime = 0.0f;

	m_startTime = 0;

	m_dwRecursionCounter = 0;

	m_pChild = NULL;

	m_pSibling = NULL;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlProfileNode::Destroy()
{
	if( m_pSibling )
	{
		SAFE_DELETE( m_pSibling );
	}

	if( m_pChild )
	{
		SAFE_DELETE( m_pChild );
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlProfileNode::Reset()
{
	m_dwTotalCalls = 0;
	m_fTotalTime = 0.0f;

	if ( m_pChild )
	{
		m_pChild->Reset();
	}
	
	if ( m_pSibling )
	{
		m_pSibling->Reset();
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlProfileNode::Call()
{
	m_dwTotalCalls++;

	if( m_dwRecursionCounter++ == 0)
	{
		GetProfileTickCount( &m_startTime );
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	: Recursive가 아니면 true
//-----------------------------------------------------------------------------------
bool CNtlProfileNode::Return()
{
	if ( --m_dwRecursionCounter == 0 && m_dwTotalCalls != 0 )
	{ 
		__int64 time;

		GetProfileTickCount( &time );
		time -= m_startTime;

		m_fTotalTime += (float) time / GetProfileTickRate();
	}

	return ( m_dwRecursionCounter == 0 );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlProfileNode * CNtlProfileNode::GetSubNode(const char * lpszNodeName)
{
	// Find SubNode
	CNtlProfileNode * pChild = m_pChild;
	while ( pChild )
	{
		if( pChild->GetName() == lpszNodeName )
		{
			return pChild;
		}

		pChild = pChild->m_pSibling;
	}


	// Create New Node
	CNtlProfileNode * pNewNode = new CNtlProfileNode( lpszNodeName, this );
	if( NULL == pNewNode )
	{
		NTL_ASSERT( 0 );
		return NULL;
	}

	pNewNode->m_pSibling = pChild;
	pChild = pNewNode;

	return pNewNode;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlProfileIterator::CNtlProfileIterator( CNtlProfileNode * start )
{
	m_pCurParent = start;
	m_pCurChild = m_pCurParent->GetChild();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlProfileIterator::First(void)
{
	m_pCurChild = m_pCurParent->GetChild();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlProfileIterator::Next(void)
{
	m_pCurChild = m_pCurChild->GetSibling();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlProfileIterator::IsDone(void)
{
	return m_pCurChild == NULL;
}


//-----------------------------------------------------------------------------------
//		Purpose	: Make the given child the new parent
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlProfileIterator::EnterChild( int index )
{
	m_pCurChild = m_pCurParent->GetChild();

	while ( (m_pCurChild != NULL) && (index != 0) )
	{
		index--;
		m_pCurChild = m_pCurChild->GetSibling();
	}

	if ( m_pCurChild != NULL )
	{
		m_pCurParent = m_pCurChild;
		m_pCurChild = m_pCurParent->GetChild();
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	: Make the current parent's parent the new parent
//		Return	: 
//-----------------------------------------------------------------------------------
void CNtlProfileIterator::EnterParent()
{
	if( m_pCurParent->GetParent() != NULL )
	{
		m_pCurParent = m_pCurParent->GetParent();
	}

	m_pCurChild = m_pCurParent->GetChild();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlProfiler::CNtlProfiler()
:
m_root( "Root", NULL )
{
	Init();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlProfiler::~CNtlProfiler()
{
	CloseProfiler();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlProfiler * CNtlProfiler::GetInstance()
{
	static CNtlProfiler	s_profiler;

	return &s_profiler;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlProfiler::Init()
{
	m_file = NULL;

	m_pCurNode = &m_root;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int	CNtlProfiler::OpenProfiler(const char * lpszFileName)
{
	if( m_file )
	{
		return NTL_ERR_SYS_PROFILE_INITIALIZE_FAIL;
	}

	int rc = fopen_s( &m_file, lpszFileName, "w" );
	if( NTL_SUCCESS != rc )
	{
		return rc;
	}

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlProfiler::CloseProfiler()
{
	if( m_file )
	{
		fclose( m_file );
	}

	Init();
}


//-----------------------------------------------------------------------------------
//		Purpose	: This resets everything except for the tree structure.  All of the timing data is reset
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlProfiler::Reset()
{
	m_root.Reset(); 

	GetProfileTickCount( &m_resetTime );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
float CNtlProfiler::GetTimeSinceReset( void )
{
	__int64 time;

	GetProfileTickCount( &time );
	time -= m_resetTime;

	return (float) time / GetProfileTickRate();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlProfiler::StartProfile(const char * lpszNodeName)
{
	if( lpszNodeName != m_pCurNode->GetName() )
	{
		m_pCurNode = m_pCurNode->GetSubNode( lpszNodeName );
	} 

	m_pCurNode->Call();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlProfiler::StopProfile()
{
	if( m_pCurNode->Return() )
	{
		m_pCurNode = m_pCurNode->GetParent();
	}
}

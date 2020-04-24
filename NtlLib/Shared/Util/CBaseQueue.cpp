/////////////////////////////////////////////////////////////////////////////
//
// BaseQueue.cpp : Base Queue Class Code
//
//        Programmed by June C. Kim (cynis@hotmail.com)
//
//        Last Update 2003.6.19
//
///////////////////////////////////////////////////////////////////////////////

//////////////////
// Header Table //
//////////////////

#include "stdafx.h"

#include "CBaseQueue.h"


/////////////////////////////////////////
// Constant Variables Definition Table //
/////////////////////////////////////////

const DWORD		CBaseQueue::SIZE_MIN					= BQ_SIZE_MIN;
const DWORD		CBaseQueue::RESIZING_FACTOR				= BQ_RESIZING_FACTOR;

const WORD		CBaseQueue::MODE_NOP					= BQ_MODE_NOPUSH;
const WORD		CBaseQueue::MODE_POP_BEFORE_PUSH		= BQ_MODE_POP_BEFORE_PUSH;
const WORD		CBaseQueue::MODE_RESIZE					= BQ_MODE_RESIZE;

const WORD		CBaseQueue::STATE_READY					= BQ_STATE_READY;
const WORD		CBaseQueue::STATE_PUSH					= BQ_STATE_PUSH;
const WORD		CBaseQueue::STATE_POP					= BQ_STATE_POP;
const WORD		CBaseQueue::STATE_RESIZE				= BQ_STATE_RESIZE;
const WORD		CBaseQueue::STATE_DISABLED				= BQ_STATE_DISABLED;


////////////////////////////////
// CBaseQueue() : Constructor //
////////////////////////////////

CBaseQueue::CBaseQueue( void )
{
	Initialize();

	return;
}


////////////////////////////////
// ~CBaseQueue() : Destructor //
////////////////////////////////

CBaseQueue::~CBaseQueue( void )
{
	Destroy();

	return;
}


/////////////////////////////////////
// Init() : Clear Member Variables //
/////////////////////////////////////

void CBaseQueue::Initialize( void )
{
	m_wMode				= MODE_NOP;

	m_dwMaxSize			= 0;
	m_dwSize			= 0;

	m_dwHead			= 0;
	m_dwTail			= 0;

	m_lpInfoQueueBuffer	= NULL;
	m_lpDataQueueBuffer	= NULL;

	m_dwErrorCode		= ERROR_SUCCESS;
	m_wState			= STATE_DISABLED;

	return;
}


/////////////////////////////////////
// Create() : Create Queue Buffers //
/////////////////////////////////////

bool CBaseQueue::Create( DWORD dwMaxSize, WORD wMode )
{
	// Check Process State

	if( m_wState != STATE_DISABLED )
	{
		this->SetLastError( ERROR_NOT_READY );

		return false;
	}

	DestroyBuffer();


	// Check & Set Parameters

	if( SetMaxSize( dwMaxSize ) == false )
	{
		return false;
	}

	if( SetMode( wMode ) == false )
	{
		return false;
	}

	// Reset Queue State

	m_wState = STATE_READY;

	return true;
}


//////////////////////////////////////////////////////////////////
// Destroy() : Destroy Queue Buffers and Clear Member Variables //
//////////////////////////////////////////////////////////////////

void CBaseQueue::Destroy( void )
{
	// Destroy Queue Buffers

	DestroyBuffer();

	// Clear Member Variables

	Initialize();

	return;
}


/////////////////////////////////////////////
// DestroyBuffer() : Destroy Queue Buffers //
/////////////////////////////////////////////

void CBaseQueue::DestroyBuffer( void )
{
	DWORD			dwIndex;

	// Set Queue State

	m_wState |= STATE_RESIZE;


	// Destroy Information Queue Buffer

	if( m_lpInfoQueueBuffer != NULL )
	{
		for( dwIndex = 0; dwIndex < m_dwMaxSize; ++dwIndex )
		{
			if( m_lpInfoQueueBuffer[dwIndex] != NULL )
			{
				delete m_lpInfoQueueBuffer[dwIndex];
				m_lpInfoQueueBuffer[dwIndex] = NULL;
			}
		}

		delete[] m_lpInfoQueueBuffer;
		m_lpInfoQueueBuffer = NULL;
	}


	// Destroy Data Queue Buffer

	if( m_lpDataQueueBuffer != NULL )
	{
		for( dwIndex = 0; dwIndex < m_dwMaxSize; ++dwIndex )
		{
			if( m_lpDataQueueBuffer[dwIndex] != NULL )
			{
				delete[] m_lpDataQueueBuffer[dwIndex];
				m_lpDataQueueBuffer[dwIndex] = NULL;
			}
		}

		delete[] m_lpDataQueueBuffer;
		m_lpDataQueueBuffer = NULL;
	}


	// Reset Queue State

	m_wState &= ~STATE_RESIZE;

	return;
}


///////////////////////////////////////
// Refresh() : Clear all Buffer Data //
///////////////////////////////////////

bool CBaseQueue::Refresh( void )
{
	// if Current Queue is empty, Do Nothing

	if( m_dwSize == 0 )
		return true;


	// Check Process State

	if( m_wState != STATE_READY )
	{
		this->SetLastError( ERROR_NOT_READY );

		return false;
	}

	DWORD			dwIndex = 0;

	// Set Queue State

	m_wState |= STATE_POP;


	// Clear Information Queue Buffer Data

	if( m_lpInfoQueueBuffer != NULL )
	{
		for( dwIndex = 0; dwIndex < m_dwMaxSize; ++dwIndex )
		{
			if( m_lpInfoQueueBuffer[dwIndex] != NULL )
			{
				delete m_lpInfoQueueBuffer[dwIndex];
				m_lpInfoQueueBuffer[dwIndex] = NULL;
			}
		}
	}


	// Clear Data Queue Buffer Data

	if( m_lpDataQueueBuffer != NULL )
	{
		for( dwIndex = 0; dwIndex < m_dwMaxSize; ++dwIndex )
		{
			if( m_lpDataQueueBuffer[dwIndex] != NULL )
			{
				delete[] m_lpDataQueueBuffer[dwIndex];
				m_lpDataQueueBuffer[dwIndex] = NULL;
			}
		}
	}


	// Update Variables

	m_dwHead	= 0;
	m_dwTail	= 0;

	m_dwSize	= 0;


	// Reset Queue State

	m_wState &= ~STATE_POP;

	return true;
}


///////////////////////////////////////////
// Push() : Push Data into Queue Buffers //
///////////////////////////////////////////

bool CBaseQueue::Push( LPQUEUEINFO lpInfo, LPVOID lpvData )
{
	// Check Parameters

	if( lpInfo == NULL || lpvData == NULL )
	{
		this->SetLastError( ERROR_INVALID_PARAMETER );

		return false;
	}


	// Wait for other Processes

	if( m_wState != STATE_READY )
	{
		this->SetLastError( ERROR_NOT_READY );

		return false;
	}


	// Set Queue State

	m_wState |= STATE_PUSH;


	// Check Queue Capacity

	if( m_dwSize == m_dwMaxSize )
	{
		// If Current Mode is Resize Mode, Resize Queue Buffer

		if( m_wMode == MODE_RESIZE )
		{
			if( AutoResize() == false )
			{
				m_wState &= ~STATE_PUSH;

				return false;
			}
		}

		// If Current Mode is Pop Before Push Mode, Pop Data from Tail, and Push Data into Head

		else if( m_wMode == MODE_POP_BEFORE_PUSH )
		{
			delete m_lpInfoQueueBuffer[m_dwTail];
			m_lpInfoQueueBuffer[m_dwTail] = NULL;

			delete[] m_lpDataQueueBuffer[m_dwTail];
			m_lpDataQueueBuffer[m_dwTail] = NULL;

			++m_dwTail;
			--m_dwSize;

			if( m_dwTail >= m_dwMaxSize )
				m_dwTail = 0;
		}

		// If Current Mode is No Operation Mode, return false

		else
		{
			m_wState &= ~STATE_PUSH;

			this->SetLastError( ERROR_NOT_ENOUGH_MEMORY );

			return false;
		}
	}


	// Create Information Node

	m_lpInfoQueueBuffer[m_dwHead] = new QUEUEINFO;
	if( m_lpInfoQueueBuffer[m_dwHead] == NULL )
	{
		this->SetLastError( ERROR_OUTOFMEMORY );

		return false;
	}

	memcpy( m_lpInfoQueueBuffer[m_dwHead], lpInfo, sizeof(QUEUEINFO) );


	// Create Data Node

	m_lpDataQueueBuffer[m_dwHead] = new BYTE[lpInfo->wSize];
	if( m_lpDataQueueBuffer[m_dwHead] == NULL )
	{
		this->SetLastError( ERROR_OUTOFMEMORY );

		delete m_lpInfoQueueBuffer[m_dwHead];
		m_lpInfoQueueBuffer[m_dwHead] = NULL;

		return false;
	}

	memcpy( m_lpDataQueueBuffer[m_dwHead], (LPBYTE)lpvData, lpInfo->wSize );


	// Move Head Position and Validate Queue Size

	++m_dwHead;
	++m_dwSize;


	// If Head Position is in End of Buffer, Move to Begin of Buffer (Circular Queue)

	if( m_dwHead >= m_dwMaxSize )
	{
		m_dwHead = 0;
	}


	// Reset Queue State

	m_wState &= ~STATE_PUSH;

	return true;
}


/////////////////////////////////////////
// Pop() : Pop Data from Queue Buffers //
/////////////////////////////////////////

bool CBaseQueue::Pop( LPQUEUEINFO lpInfo, LPVOID lpvData )
{
	// Check Parameters

	if( ( lpInfo == NULL ) || ( lpvData == NULL ) )
	{
		this->SetLastError( ERROR_INVALID_PARAMETER );

		return false;
	}


	// if Current Queue is empty, return false

	if( m_dwSize == 0 )
	{
		this->SetLastError( ERROR_EMPTY );

		return false;
	}

	// Wait for other Processes

	if( m_wState != STATE_READY )
	{
		SetLastError( ERROR_NOT_READY );

		return false;
	}

	// Set Queue State

	m_wState |= STATE_POP;


	// Copy Information from Queue Buffer and Remove from Buffer

	if( lpInfo != NULL )
		memcpy( lpInfo, m_lpInfoQueueBuffer[m_dwTail], sizeof(QUEUEINFO) );

	delete m_lpInfoQueueBuffer[m_dwTail];
	m_lpInfoQueueBuffer[m_dwTail] = NULL;


	// Copy Data from Queue Buffer and Remove from Buffer

	if( lpvData != NULL )
		memcpy( lpvData, m_lpDataQueueBuffer[m_dwTail], lpInfo->wSize );

	delete[] m_lpDataQueueBuffer[m_dwTail];
	m_lpDataQueueBuffer[m_dwTail] = NULL;


	// Move Tail Position and Validate Queue Size

	++m_dwTail;
	--m_dwSize;

	// If Tail Position is in Begin of Buffer, Move to End of Buffer (Circular Queue)

	if( m_dwTail >= m_dwMaxSize )
	{
		m_dwTail = 0;
	}

	// If Queue Mode is Resize Mode, Check Queue Capacity and Downsize Buffer if Needed

	if( m_wMode == MODE_RESIZE )
	{
		AutoResize();
	}

	// Reset Queue State

	m_wState &= ~STATE_POP;

	return true;
}


///////////////////////////////////////////////////////////////
// PushFast() : Push Data into Queue Buffers by Shallow Copy //
///////////////////////////////////////////////////////////////

bool CBaseQueue::PushFast( LPQUEUEINFO lpInfo, LPVOID lpvData )
{
	// Check Parameters

	if( lpInfo == NULL || lpvData == NULL )
	{
		this->SetLastError( ERROR_INVALID_PARAMETER );

		return false;
	}


	// Wait for other Processes

	if( m_wState != STATE_READY )
	{
		SetLastError( ERROR_NOT_READY );

		return false;
	}


	// Set Queue State

	m_wState |= STATE_PUSH;


	// Check Queue Capacity

	if( m_dwSize == m_dwMaxSize )
	{
		// If Current Mode is Resize Mode, Resize Queue Buffer

		if( m_wMode == MODE_RESIZE )
		{
			if( AutoResize() == false )
			{
				m_wState &= ~STATE_PUSH;

				return false;
			}
		}

		// If Current Mode is Pop Before Push Mode, Pop Data from Tail, and Push Data into Head

		else if( m_wMode == MODE_POP_BEFORE_PUSH )
		{
			delete m_lpInfoQueueBuffer[m_dwTail];
			m_lpInfoQueueBuffer[m_dwTail] = NULL;

			delete[] m_lpDataQueueBuffer[m_dwTail];
			m_lpDataQueueBuffer[m_dwTail] = NULL;

			++m_dwTail;
			--m_dwSize;

			if( m_dwTail >= m_dwMaxSize )
				m_dwTail = 0;
		}

		// If Current Mode is No Operation Mode, return false

		else
		{
			m_wState &= ~STATE_PUSH;

			return false;
		}
	}


	// Copy Information and Data by Using Shallow Copy
	
	m_lpInfoQueueBuffer[m_dwHead] = lpInfo;
	m_lpDataQueueBuffer[m_dwHead] = (LPBYTE)lpvData;


	// Move Head Position and Validate Queue Size

	++m_dwHead;
	++m_dwSize;


	// If Head Position is in End of Buffer, Move to Begin of Buffer (Circular Queue)

	if( m_dwHead >= m_dwMaxSize )
	{
		m_dwHead = 0;
	}


	// Reset Queue State

	m_wState &= ~STATE_PUSH;

	return true;
}


/////////////////////////////////////////////////////////////
// PopFast() : Pop Data from Queue Buffers by Shallow Copy //
/////////////////////////////////////////////////////////////

bool CBaseQueue::PopFast( LPQUEUEINFO lpInfo, LPVOID lpvData )
{
	// if Current Queue is empty, return false

	if( m_dwSize == 0 )
	{
		this->SetLastError( ERROR_EMPTY );

		return false;
	}


	// Wait for other Processes

	if( m_wState != STATE_READY )
	{
		SetLastError( ERROR_NOT_READY );

		return false;
	}


	// Set Queue State

	m_wState |= STATE_POP;


	// Copy Information and Data by Using Shallow Copy

	lpInfo = m_lpInfoQueueBuffer[m_dwTail];
	lpvData = (LPVOID)m_lpDataQueueBuffer[m_dwTail];


	// Move Tail Position and Validate Queue Size

	++m_dwTail;
	--m_dwSize;


	// If Tail Position is in Begin of Buffer, Move to End of Buffer (Circular Queue)

	if( m_dwTail >= m_dwMaxSize )
	{
		m_dwTail = 0;
	}


	// If Queue Mode is Resize Mode, Check Queue Capacity and Downsize Buffer if Needed

	if( m_wMode == MODE_RESIZE )
	{
		AutoResize();
	}


	// Reset Queue State

	m_wState &= ~STATE_POP;

	return true;
}


//////////////////////////////////////////////////////////////////////////////
// SetMaxSize() : Modify Maximum Queue Buffer Size and Validate Buffer //
//////////////////////////////////////////////////////////////////////////////

bool CBaseQueue::SetMaxSize( DWORD dwNewMaxSize )
{
	// if New Maximum Queue Size is smaller than Current Valid Queue Size, return false ( for Protect Valid data )

	if( dwNewMaxSize < m_dwSize )
	{
		this->SetLastError( ERROR_INVALID_PARAMETER );

		return false;
	}

	// Set Queue State

	m_wState |= STATE_RESIZE;


	LPQUEUEINFO *			pNewInfoQueueBuffer;
	LPBYTE *				pNewDataQueueBuffer;
	DWORD					dwOldIndex = m_dwTail;
	DWORD					dwNewIndex = 0;

	// Create & Clear New Information Queue Buffer

	pNewInfoQueueBuffer = new LPQUEUEINFO[dwNewMaxSize];
	if( pNewInfoQueueBuffer == NULL )
	{
		this->SetLastError( ERROR_OUTOFMEMORY );

		return false;
	}

	ZeroMemory( pNewInfoQueueBuffer, dwNewMaxSize * sizeof(LPQUEUEINFO) );

	// Create & Clear New Data Queue Buffer

	pNewDataQueueBuffer = new LPBYTE[dwNewMaxSize];
	if( pNewDataQueueBuffer == NULL )
	{
		this->SetLastError( ERROR_OUTOFMEMORY );

		delete[] pNewInfoQueueBuffer;

		return false;
	}

	ZeroMemory( pNewDataQueueBuffer, dwNewMaxSize * sizeof(LPBYTE) );


	// In valid Scoope, Copy Inforamtion and Data to New Queue Buffer from Old Queue Buffer

	while( m_dwSize > 0 )
	{
		pNewInfoQueueBuffer[dwNewIndex] = m_lpInfoQueueBuffer[dwOldIndex];
		m_lpInfoQueueBuffer[dwOldIndex] = NULL;

		pNewDataQueueBuffer[dwNewIndex] = m_lpDataQueueBuffer[dwOldIndex];
		m_lpDataQueueBuffer[dwOldIndex] = NULL;

		++dwNewIndex;
		++dwOldIndex;

		if( dwOldIndex >= m_dwMaxSize )
			dwOldIndex = 0;

		--m_dwSize;
	}

	// Delete Old Queue Buffers

	delete[] m_lpInfoQueueBuffer;
	delete[] m_lpDataQueueBuffer;


	// Update Variables

	m_dwHead			= dwNewIndex;
	m_dwTail			= 0;

	m_dwMaxSize			= dwNewMaxSize;
	m_dwSize			= dwNewIndex;


	// Set New Queue Buffers

	m_lpInfoQueueBuffer	= pNewInfoQueueBuffer;
	m_lpDataQueueBuffer	= pNewDataQueueBuffer;

	// Reset State

	m_wState &= ~STATE_RESIZE;

	return true;
}


///////////////////////////////////
// SetMode() : Modify Queue Mode //
///////////////////////////////////

bool CBaseQueue::SetMode( WORD wNewMode )
{
	switch( wNewMode )
	{
		case MODE_NOP:
		{
			m_wMode = MODE_NOP;

			break;
		}

		case MODE_POP_BEFORE_PUSH:
		{
			m_wMode = MODE_POP_BEFORE_PUSH;

			break;
		}

		case MODE_RESIZE:
		{
			m_wMode = MODE_RESIZE;

			break;
		}

		default:
		{
			this->SetLastError( ERROR_INVALID_PARAMETER );

			return false;
		}
	}

	return true;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// AutoResize() : Check Queue Capacity, Calculate New Queue Buffer Size, and Resize Queue Buffers //
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CBaseQueue::AutoResize( void )
{
	// If Queue Buffers are in Full Capacity( X >= 100% ), Upsize( x(1 << RESIZING_FACTOR) ) Queue Buffers
	if( m_dwSize >= m_dwMaxSize )
	{
		return SetMaxSize( m_dwMaxSize << RESIZING_FACTOR );
	}
	// If Queue Buffers are in Low Capacity( X < ( 100% >> RESIZING_FACTOR) ), Downsize( x(1 >> RESIZING_FACTOR) ) Queue Buffers
	else if( ( m_dwSize > ( SIZE_MIN << RESIZING_FACTOR ) )
		&& ( m_dwSize < ( m_dwMaxSize >> RESIZING_FACTOR ) ) )
	{
		return SetMaxSize( m_dwSize >> RESIZING_FACTOR );
	}

	// If Queue Buffers are in Proper Capacity ( 100% >> RESIZING_FACTOR) < X < 100%, Do Nothing

	return true;
}


//////////////////////////////////////////////////////////////////////////////
//
// End of File : BaseQueue.cpp
//
//////////////////////////////////////////////////////////////////////////////
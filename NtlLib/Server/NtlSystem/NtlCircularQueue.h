//***********************************************************************************
//
//	File		:	NtlCircularQueue.h
//
//	Begin		:	2005-12-08
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	È¯ÇüÅ¥
//
//***********************************************************************************

#pragma once


template <class TYPE>
class CNtlCircularQueue
{
public:

	CNtlCircularQueue()
	{
		Init();
	}

	virtual ~CNtlCircularQueue() {	Destroy(); }


public:

	bool				Create(int nQueueSize, int nExtraSize);

	void				Destroy();


public:

	bool				Realloc(int nQueueSize, int nExtraSize) { return Create(nQueueSize, nExtraSize); }

	void				Clear();

	int					GetQueueSize() { return QUEUE_SIZE; }

	int					GetQueueExtraSize() { return EXTRA_SIZE; }

	int					GetQueueTotalSize() { return GetQueueSize() + GetQueueExtraSize(); }


public:

	int					GetFreeSize() { return GetQueueSize() - GetCurSize(); }

	int					GetLinearFreeSize() { return GetQueueTotalSize() - GetPushPos(); }

	int					GetCurSize();

	int					GetPushAvailableSize();

	int					GetWorkRemainSize();

	int					GetQueueLoopCount() { return m_nLoopCount; }


public:

	int					GetPushPos() { return m_nPushPos; }

	int					GetPopPos() { return m_nPopPos; }

	int					GetWorkPos() { return m_nWorkPos; }


	TYPE *				GetQueuePtr(int nPos);

	TYPE *				GetQueuePushPtr() { return GetQueuePtr( GetPushPos() ); }

	TYPE *				GetQueuePopPtr() { return GetQueuePtr( GetPopPos() ); }

	TYPE *				GetQueueWorkPtr() { return GetQueuePtr( GetWorkPos() ); }


public:

	bool				IsFull() { return GetCurSize() == GetQueueSize(); }

	bool				IsEmpty() { return 0 == GetCurSize(); }

	bool				IncreasePushPos(int nSize);

	bool				IncreasePopPos(int nSize);

	bool				IncreaseWorkPos(int nSize);


protected:

	void				Init();

	TYPE *				GetQueueBufferPtr() { return m_queueBuffer; }

	TYPE *				GetQueueExtraPtr() { return &m_queueBuffer[QUEUE_SIZE]; }


private:

	int					QUEUE_SIZE;

	int					EXTRA_SIZE;

	int					m_nPushPos;

	int					m_nPopPos;

	int					m_nWorkPos;

	TYPE * 				m_queueBuffer;

	int					m_nLoopCount;

};


//---------------------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------------------
template <class TYPE>
inline void CNtlCircularQueue<TYPE>::Init()
{
	m_queueBuffer = NULL;

	m_nLoopCount = 0;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
template <class TYPE>
inline bool CNtlCircularQueue<TYPE>::Create(int nQueueSize, int nExtraSize)
{
	if( NULL != m_queueBuffer )
	{
		return false;
	}

	QUEUE_SIZE = nQueueSize;
	EXTRA_SIZE = nExtraSize;

	m_queueBuffer = new TYPE[ GetQueueTotalSize() ];
	if( NULL == m_queueBuffer )
	{
		return false;
	}

	Clear();

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
template <class TYPE>
inline void CNtlCircularQueue<TYPE>::Destroy()
{
	SAFE_DELETE_ARRAY( m_queueBuffer );
}


//---------------------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------------------
template <class TYPE>
inline void CNtlCircularQueue<TYPE>::Clear()
{
	m_nPushPos = m_nPopPos = m_nWorkPos = 0;

	if( m_queueBuffer )
	{
		ZeroMemory( m_queueBuffer, sizeof(TYPE) * GetQueueTotalSize() );
	}
}


//---------------------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------------------
template <class TYPE>
TYPE * CNtlCircularQueue<TYPE>::GetQueuePtr(int nPos)
{
	if( nPos >= GetQueueTotalSize() )
		return (TYPE*)0;

	return &m_queueBuffer[nPos];
}


//---------------------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------------------
template <class TYPE>
bool CNtlCircularQueue<TYPE>::IncreasePushPos(int nSize)
{
	if( GetFreeSize() <= nSize )
		return false;

	m_nPushPos += nSize;

	if( m_nPushPos >= GetQueueSize() )
	{
		m_nPushPos -= GetQueueSize();
		memcpy( GetQueueBufferPtr(), GetQueueExtraPtr(), sizeof(TYPE) * m_nPushPos );

		m_nLoopCount++;
	}

	return true;
}


//---------------------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------------------
template <class TYPE>
bool CNtlCircularQueue<TYPE>::IncreasePopPos(int nSize)
{
	m_nPopPos += nSize;

	if( m_nPopPos >= GetQueueSize() )
		m_nPopPos -= GetQueueSize();

	return true;
}


//---------------------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------------------
template <class TYPE>
bool CNtlCircularQueue<TYPE>::IncreaseWorkPos(int nSize)
{
	m_nWorkPos += nSize;

	if( m_nWorkPos >= GetQueueSize() )
		m_nWorkPos -= GetQueueSize();

	return true;
}



//---------------------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------------------
template <class TYPE>
inline int CNtlCircularQueue<TYPE>::GetCurSize()
{
	int nRetValue = GetPushPos() - GetPopPos();

	if( nRetValue < 0 )
		return nRetValue + GetQueueSize();

	return nRetValue;
}


//---------------------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------------------
template <class TYPE>
inline int CNtlCircularQueue<TYPE>::GetWorkRemainSize()
{
	int nRetValue = GetPushPos() - GetWorkPos();

	if( nRetValue < 0 )
		return nRetValue + GetQueueSize();

	return nRetValue;
}


//---------------------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------------------
template <class TYPE>
int CNtlCircularQueue<TYPE>::GetPushAvailableSize()
{
	int nFreeSize = GetFreeSize();
	int nLinearFreeSize = GetLinearFreeSize();

	return ( nLinearFreeSize < nFreeSize ) ? nLinearFreeSize : ( nFreeSize - 1 );
}
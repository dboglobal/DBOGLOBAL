//***********************************************************************************
//
//	File		:	NtlMemoryPool.h
//
//	Begin		:	2005-12-13
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	NTL 객체 메모리 풀 ( static 및 dynamic )
//
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------------------
// class CNtlMemoryPool_Dynamic
//-----------------------------------------------------------------------------------
#define __DONOT_USE_MEMORYPOOL__

#include <deque>

#include "NtlMutex.h"
template<class TYPE>
class CNtlMemoryPool_Dynamic
{
public:

	CNtlMemoryPool_Dynamic(int nReserved = 0);

	virtual ~CNtlMemoryPool_Dynamic() { Destroy(); }

public:

	//
	void				Destroy();

	//
	TYPE*				Alloc();

	//
	void				Free(TYPE * t);


	// 전체 메모리 생성 개수
	int					GetTotalCount() { return m_nTotalCount; }

	// 할당 가능한 공간 반환(리스트에 있는 여분의 공간의 개수)
	int					GetAvailableCount()	{ return store.size(); }

	// 처음 생성시 예약한 개수 
	int					GetReservedCount() { return m_nReserved; }

	// 클라이언트에 메모리를 할당해준 개수
	int					GetAllocatedCount() { return GetTotalCount() - GetAvailableCount(); }


private:

	void				Init();

	bool				Reserve(int nCount);


private:

	typedef std::deque<TYPE*> LIST;
	typedef typename LIST::iterator	LISTIST;

	LIST				m_store;

	int					m_nTotalCount;	// 메모리 생성 개수
	int					m_nReserved;	// 예약한 개수	

};


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
template <class TYPE>
inline CNtlMemoryPool_Dynamic<TYPE>::CNtlMemoryPool_Dynamic(int nReserved)
:
m_nReserved( nReserved ),
m_nTotalCount( 0 ) 
{
	if( nReserved > 0 )
	{
		Reserve( m_nReserved );
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
template <class TYPE>
inline void CNtlMemoryPool_Dynamic<TYPE>::Destroy()
{
	LISTIST it = m_store.begin();
	while( it != m_store.end() )
	{
		free( *it );

		it = m_store.erase(it);
		m_nTotalCount--;
	}	
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
template <class TYPE>
inline bool CNtlMemoryPool_Dynamic<TYPE>::Reserve(int nCount)
{
	for( int i = 0; i < nCount; i++ )
	{
		TYPE * pObject = (TYPE*) malloc( sizeof(TYPE) );
		if ( NULL == pObject )
		{
			Destroy();
			return false;
		}

		m_store.push_back( pObject );
		m_nTotalCount++;
	}

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
template <class TYPE>
inline TYPE* CNtlMemoryPool_Dynamic<TYPE>::Alloc()
{
	TYPE * pObject = NULL;

	if( true == m_store.empty() )
	{
		pObject = (TYPE*) malloc( sizeof(TYPE) );
		if ( NULL == pObject )
		{
			return NULL;
		}

		m_nTotalCount++;
	}
	else
	{
		pObject = *m_store.begin();
		m_store.pop_front();
	}

	return pObject;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
template <class TYPE>
inline void CNtlMemoryPool_Dynamic<TYPE>::Free(TYPE * t)
{
	m_store.push_front(t);
}



//-----------------------------------------------------------------------------------
// DYNAMIC MEMORY POOL MACRO
//-----------------------------------------------------------------------------------
#define DECLARE_DYNAMIC_MEMORYPOOL(classname)								\
public:																		\
	void * operator new( size_t stAllocationBlock );						\
	void operator delete( void * pMem );									\
	private:																\
	static CNtlMemoryPool_Dynamic<classname> __m_memoryPool__;		


#define DEFINE_DYNAMIC_MEMORYPOOL(classname, reserve)						\
	CNtlMemoryPool_Dynamic<classname> classname::__m_memoryPool__(reserve);	\
	void * classname::operator new( size_t stAllocationBlock )				\
	{																		\
		(void) stAllocationBlock;											\
		classname * pMem = __m_memoryPool__.Alloc();						\
		return pMem;														\
	}																		\
																			\
	void classname::operator delete( void * pMem )							\
	{																		\
		__m_memoryPool__.Free((classname*)pMem);							\
	}


#ifdef __DONOT_USE_MEMORYPOOL__
	#undef DECLARE_DYNAMIC_MEMORYPOOL
	#define DECLARE_DYNAMIC_MEMORYPOOL(classname)

	#undef DEFINE_DYNAMIC_MEMORYPOOL
	#define DEFINE_DYNAMIC_MEMORYPOOL(classname, reserve)
#endif

//-----------------------------------------------------------------------------------
// DYNAMIC MEMORY POOL MACRO ( THREAD SAFE )
//-----------------------------------------------------------------------------------
#define DECLARE_DYNAMIC_MEMORYPOOL_THREADSAFE(classname)					\
public:																		\
	void * operator new( size_t stAllocationBlock );						\
	void operator delete( void * pMem );									\
private:																	\
	static CNtlMutex __m_poolMutex__;										\
	static CNtlMemoryPool_Dynamic<classname> __m_memoryPool__;


#define DEFINE_DYNAMIC_MEMORYPOOL_THREADSAFE(classname, reserve)			\
	CNtlMutex classname::__m_poolMutex__;									\
	CNtlMemoryPool_Dynamic<classname> classname::__m_memoryPool__(reserve); \
	void * classname::operator new( size_t stAllocationBlock )				\
	{																		\
		(void) stAllocationBlock;											\
		__m_poolMutex__.Lock();												\
		classname * pMem = __m_memoryPool__.Alloc();						\
		__m_poolMutex__.Unlock();											\
		return pMem;														\
	}																		\
																			\
	void classname::operator delete( void * pMem )							\
	{																		\
		__m_poolMutex__.Lock();												\
		__m_memoryPool__.Free((classname*)pMem);							\
		__m_poolMutex__.Unlock();											\
	}


#ifdef __DONOT_USE_MEMORYPOOL__
	#undef DECLARE_DYNAMIC_MEMORYPOOL_THREADSAFE
	#define DECLARE_DYNAMIC_MEMORYPOOL_THREADSAFE(classname)

	#undef DEFINE_DYNAMIC_MEMORYPOOL_THREADSAFE
	#define DEFINE_DYNAMIC_MEMORYPOOL_THREADSAFE(classname, reserve)
#endif


/////////////////////////////////////////////////////////////////////////////////////
//
// class CNtlMemoryPool_Static
//
/////////////////////////////////////////////////////////////////////////////////////
template <class TYPE>
class CNtlMemoryPool_Static  
{
public:
	CNtlMemoryPool_Static(int nSize = 0);

	virtual ~CNtlMemoryPool_Static()
	{
		Destroy(); 
	}

public:

	bool				Create(int nSize);

	bool				Destroy();

	TYPE * 				Alloc();

	void				Free(TYPE * t);


protected:

	int					m_nPoolSize;

	int					m_nAllocPos;

	TYPE *				m_pTypeStore;

	TYPE **				m_ppTypePtrStore;
};


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
template <class TYPE>
inline CNtlMemoryPool_Static<TYPE>::CNtlMemoryPool_Static(int nSize)
:
m_nPoolSize( nSize ),
m_nAllocPos( 0 ),
m_pTypeStore( 0 ),
m_ppTypePtrStore( 0 )
{
	if( nSize )
	{
		Create( nSize );
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
template <class TYPE>
inline bool CNtlMemoryPool_Static<TYPE>::Create(int nSize)
{
	if( nSize <= 0 )
	{
		return false;
	}

	m_nPoolSize = nSize;

	m_pTypeStore = (TYPE*) malloc( sizeof(TYPE) * m_nPoolSize );
	if( NULL == m_pTypeStore )
	{
		return false;
	}

	m_ppTypePtrStore = (TYPE**) malloc( sizeof(TYPE*) * m_nPoolSize );
	if( NULL == m_ppTypePtrStore )
	{
		return false;
	}

	for(int i = 0; i < nSize; i++)
	{
		m_ppTypePtrStore[i] = &m_pTypeStore[i];
	}

	m_nAllocPos = 0;


	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
template <class TYPE>
inline bool CNtlMemoryPool_Static<TYPE>::Destroy()
{
	if( !m_nPoolSize )
		return false;

	if( m_pTypeStore )
	{
		free( m_pTypeStore );
		m_pTypeStore = NULL;
	}

	if( m_ppTypePtrStore)
	{
		free( m_ppTypePtrStore );
		m_ppTypePtrStore = NULL;
	}

	m_nPoolSize = 0;

	m_nAllocPos = 0;


	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
template <class TYPE>
inline TYPE * CNtlMemoryPool_Static<TYPE>::Alloc()
{
	if( m_nAllocPos >= m_nPoolSize )
	{
		return (TYPE*) 0;
		printf("alloc fail \n");
	}
	printf("alloc success \n");
	return m_ppTypePtrStore[ m_nAllocPos++ ];
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
template <class TYPE>
inline void CNtlMemoryPool_Static<TYPE>::Free(TYPE * t)
{
	if( m_nAllocPos <= 0 )
		return;

	m_ppTypePtrStore[--m_nAllocPos] = t;
}



//-----------------------------------------------------------------------------------
// STATIC MEMORY POOL DECLARE MACRO
//-----------------------------------------------------------------------------------
#define DECLARE_STATIC_MEMORYPOOL(classname)								\
public:																		\
	void * operator new( size_t stAllocationBlock );						\
	void operator delete( void * pMem );									\
private:																	\
	static CNtlMemoryPool_Static<classname> __m_memoryPool__;				


#define DEFINE_STATIC_MEMORYPOOL(classname, reserve)						\
	CNtlMemoryPool_Static<classname> classname::__m_memoryPool__(reserve);	\
	void * classname::operator new( size_t stAllocationBlock )				\
	{																		\
		(void) stAllocationBlock;											\
		classname * pMem = __m_memoryPool__.Alloc();						\
		return pMem;														\
	}																		\
																			\
	void classname::operator delete( void * pMem )							\
	{																		\
		__m_memoryPool__.Free((classname*)pMem);							\
	}


#ifdef __DONOT_USE_MEMORYPOOL__
	#undef DECLARE_STATIC_MEMORYPOOL
	#define DECLARE_STATIC_MEMORYPOOL(classname)

	#undef DEFINE_STATIC_MEMORYPOOL
	#define DEFINE_STATIC_MEMORYPOOL(classname, reserve)
#endif

//-----------------------------------------------------------------------------------
// STATIC MEMORY POOL DECLARE MACRO ( THREAD SAFE )
//-----------------------------------------------------------------------------------
#define DECLARE_STATIC_MEMORYPOOL_THREADSAFE(classname)						\
public:																		\
	void * operator new( size_t stAllocationBlock );						\
	void operator delete( void * pMem );									\
private:																	\
	static CNtlMutex __m_poolMutex__;										\
	static CNtlMemoryPool_Static<classname> __m_memoryPool__;				\


#define DEFINE_STATIC_MEMORYPOOL_THREADSAFE(classname, reserve)				\
	CNtlMutex classname::__m_poolMutex__;									\
	CNtlMemoryPool_Static<classname> classname::__m_memoryPool__(reserve);	\
	void * classname::operator new( size_t stAllocationBlock )				\
	{																		\
		(void) stAllocationBlock;											\
		__m_poolMutex__.Lock();												\
		classname * pMem = __m_memoryPool__.Alloc();						\
		__m_poolMutex__.Unlock();											\
		return pMem;														\
	}																		\
																			\
	void classname::operator delete( void * pMem )							\
	{																		\
		__m_poolMutex__.Lock();												\
		__m_memoryPool__.Free((classname*)pMem);							\
		__m_poolMutex__.Unlock();											\
	}


#ifdef __DONOT_USE_MEMORYPOOL__
	#undef DECLARE_STATIC_MEMORYPOOL_THREADSAFE
	#define DECLARE_STATIC_MEMORYPOOL_THREADSAFE(classname)

	#undef DEFINE_STATIC_MEMORYPOOL_THREADSAFE
	#define DEFINE_STATIC_MEMORYPOOL_THREADSAFE(classname, reserve)
#endif

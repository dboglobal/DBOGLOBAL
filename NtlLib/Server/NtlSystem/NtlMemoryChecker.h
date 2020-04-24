//***********************************************************************************
//
//	File		:	MemoryLeakChecker.h
//
//	Begin		:	2007-01-12
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once


/*----------------------------------------------------------------------------------
  [1/12/2007 zeroera] 
-----------------------------------------------------------------------------------
If the memory range checking for leaks and check it kige significant overhead
  Do not use when it is not a memory leak check
-----------------------------------------------------------------------------------*/

#define __MEMORY_LEAK_CHECK__



#if defined(__MEMORY_LEAK_CHECK__) && defined(_MSC_VER) && defined (_DEBUG) // For Windows Debug

#define _CRTDBG_MAP_ALLOC // For memory leak check
#include <windows.h>
#include <crtdbg.h> // Include CRTDBG.H after all other headers

#if !defined (_CONSOLE)
	#include <cstdlib> // for Consol Application
#endif


#ifdef NEW_INLINE
	#undef NEW_INLINE
#endif
#define NEW_INLINE new( _NORMAL_BLOCK, __FILE__, __LINE__ )

#ifdef new
	#undef new
#endif
#define new NEW_INLINE



class CNtlMemoryChecker
{
public:

	CNtlMemoryChecker()
	{
		// CrtDbg Flag
		_CrtSetDbgFlag(	_CRTDBG_ALLOC_MEM_DF | // 디버그 힙 할당을 활성화 하고 블록 구분자를 사용한다 ( 디폴트로 활성화 되어 있다 )
						_CRTDBG_CHECK_ALWAYS_DF | // 할당과 할당 해제 요청이 들어올 때마다 모든 메모리를 검사하고 검증한다. ( 언더라이트와 오버라이트를 알려준다 )
						//_CRTDBG_CHECK_CRT_DF | // CRT 라이브러리의 내부의 메모리 할당까지 보고한다. ( 보통 키지 않는다 )
						_CRTDBG_DELAY_FREE_MEM_DF | // 진짜 메모리 해제 대신 할당된 블록과 내부 힙 리스트를 유지한다. ( 해제된 블럭 액세스 탐지 )
						_CRTDBG_LEAK_CHECK_DF // 프로그램 종료시 메모리 누수를 검사한다. ( MFC 에서와 같이 )
						);

/*
#if defined (_CONSOLE)
		// Send all reports to STDOUT
		_CrtSetReportMode( _CRT_WARN,   
			_CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG );
		_CrtSetReportFile( _CRT_WARN,   
			_CRTDBG_FILE_STDERR );

		_CrtSetReportMode( _CRT_ERROR,  
			_CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG  );
		_CrtSetReportFile( _CRT_ERROR,  
			_CRTDBG_FILE_STDERR );

		_CrtSetReportMode( _CRT_ASSERT, 
			_CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG  );
		_CrtSetReportFile( _CRT_ASSERT, 
			_CRTDBG_FILE_STDERR );
			
#else

		_CrtSetReportMode( _CRT_WARN,   
			_CRTDBG_MODE_DEBUG  );
		_CrtSetReportMode( _CRT_ERROR,  
			_CRTDBG_MODE_DEBUG  );
		_CrtSetReportMode( _CRT_ASSERT, 
			_CRTDBG_MODE_DEBUG  );
			
#endif
*/
	}
	

public:

	void				Test()
	{
		TCHAR * pNew1 = new TCHAR[20];
		TCHAR * pNew2 = new TCHAR[200];
		//void * pMalloc3 = malloc(10);

		memset( pNew1, 0xAA, sizeof(TCHAR) * 20 );
		memset( pNew2, 0xFF, sizeof(TCHAR) * 200 );

		//memcpy( pNew1, pNew2, sizeof(TCHAR) * 30 );
		//	_ASSERT( _CrtCheckMemory() );


		//delete[] pNew1;
		//delete[] pNew2;
	}
};

static CNtlMemoryChecker __sMemoryLeakChecker;


#endif // defined(_MSC_VER) && defined (_DEBUG)

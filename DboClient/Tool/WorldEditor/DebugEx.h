// DebugEx.h - Extended tracing/dumping/asserting utility set
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <windows.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <tchar.h>
#include <stdarg.h>
#include <malloc.h>
#include <crtdbg.h>
#include <ImageHlp.h>

#pragma comment( lib, "imagehlp.lib" )

#pragma warning (disable: 4005 4530 4267 4786)

#include <algorithm>
#include <string>
#include <vector>
#include <list>
#include <set>

#if !defined(MAX_SUPPORTED_TRACE_LEVEL)
	#define  MAX_SUPPORTED_TRACE_LEVEL		9
#endif
#if !defined(MAX_FORMAT_LEN)
	#define  MAX_FORMAT_LEN			1024
#endif
#if !defined(MAX_TRACE_MSG_LEN)
	#define  MAX_TRACE_MSG_LEN		1024
#endif
#if !defined(MAX_DUMPED_DATA_LEN)
	#define  MAX_DUMPED_DATA_LEN	2048
#endif

#if !defined(BLIND_ARG_LIST_DEPTH)
	#define BLIND_ARG_LIST_DEPTH	5
#endif

#define  MAX_ROLLBACKS			32

#if !defined(MAX_STR_ARG_LEN)
	#define  MAX_STR_ARG_LEN	32
#endif

#if !defined(MAX_THREADS)
	#define  MAX_THREADS	64
#endif

#define  ALL_TRACE_LEVELS		(-1)
#define  CRASH_REPORT_LEVEL		(-2)

enum  dbgTracer_FileOpts
{
		TraceWriteThrough = 0x01,
		TraceNewLog = 0x02
};

enum  dbgTracer_TraceOpts
{
		// Additional information flags
		TraceSystemTime = 0x01,
		TraceLocation = 0x02, 
		TracePID = 0x04,
		TraceTID = 0x08,
		TracePrTime = 0x10,
		TraceThrTime = 0x20,
		TraceRealPrThrTime = 0x40,
		TraceAddInfoMask = 0xFF,
		// Functions information flags
		TraceFnName = 0x0100,
		TraceFnArgTypes = 0x0200,
		TraceFnArgs = 0x0400,
		TraceFnCallDisp = 0x0800,
		TraceFnStripTemplArgs = 0x1000,
		TraceFnStripStringArgs = 0x2000,
		TraceFnNoRawWideStrings = 0x4000,
		TraceFnDontUndecorate = 0x8000,
		TraceFnInfoMask = TraceFnName | TraceFnArgTypes | TraceFnArgs | TraceFnCallDisp |
						  TraceFnStripStringArgs | TraceFnStripTemplArgs | TraceFnDontUndecorate,
		// Representation flags
		TraceMicroseconds = 0x00010000,
		TraceRtiHex = 0x00020000,
		TraceFnDecIntArgs = 0x00040000,
		TraceFnHexIntArgs = 0x00080000,
		TraceFnDispHex = 0x00100000,
		TraceFnReprMask = TraceFnDecIntArgs | TraceFnHexIntArgs | TraceFnDispHex,
		TraceFnMask = TraceFnInfoMask | TraceFnReprMask,
		// Destination flags
		TraceDbgOutput = 0x00400000,
		TraceMemBuffer = 0x00800000,
		// Predefined flag combinations
		TracePrThrID = TracePID | TraceTID,
		TracePrThrTime = TracePrTime | TraceThrTime,
		TraceRTI = TracePrThrID | TracePrThrTime,
		TraceFnArgsStripped = TraceFnArgs | TraceFnStripTemplArgs | TraceFnStripStringArgs,
		TraceFnArgTypesStripped = TraceFnArgTypes | TraceFnStripTemplArgs,
		TraceStdInfo = TraceDbgOutput | TraceSystemTime | TraceTID | TraceMicroseconds,
		TraceAll = TraceDbgOutput | TraceSystemTime | TraceLocation | TraceRTI | TraceFnArgs | TraceMicroseconds,
		TraceStdStackInfo = TraceLocation | TraceFnArgsStripped | TraceFnCallDisp
};

enum  dbgTracer_DumpTypes  {
		DumpHex = 0,
		DumpDec = 1,
		DumpRadixMask = 1,

		DumpBytes =  0,
		DumpWords =  2,
		DumpDwords = 4,
		DumpQwords = 6,
		Dump8 =  0,
		Dump16 =  2,
		Dump32 = 4,
		Dump64 = 6,
		DumpElemSizeMask = 0x0E,	// 1110

		DumpNoText = 0,
		DumpAscii = 16,
		DumpUnicode = 32,
		DumpTchars = 48,
		DumpTextReprMask = 0x30,		// 0011 0000

		DumpBoundNone = 0,
		DumpBoundBrackets = 64,
		DumpBoundBraces = 128,
		DumpBoundAngleBrackets = 192,
		DumpBoundMask = 0xC0,			// 1100 0000

		DumpDelimSpace = 0,
		DumpDelimComma = 256,
		DumpDelimMask = 0x100
};

#if defined(TRACE)
	#undef  TRACE
#endif
#if  defined(VERIFY)
	#undef  VERIFY
#endif
#if  defined(ASSERT)
	#undef  ASSERT
#endif

typedef  void  (*COMPUTER_CFG_INFO) ( LPTSTR pszBuf );	// The size of pszBuf is 4K.

typedef  void  (*CRASHREPORTCALLBACK) ( PEXCEPTION_POINTERS pExceptionInfo, HANDLE hRptFile,
									    LPCTSTR pcszRptFullPath, LPCTSTR pcszRptFileName,
										LPCTSTR pcszAppFullPath, LPCTSTR pcszAppFileName);

#if defined(_DEBUG)  ||  defined(RELEASE_TRACE)  ||  !defined(TRACE_NO_CRASH_REPORT)

namespace  __dbg_util  {

//////////////////////////////////////////////////////////////////////////
// class CBooster - Temporary priority boost
//

#if defined(_MT)
class  CBooster
{
	DWORD	m_dwPrevPriority;
	HANDLE	m_hThread;
public:
	CBooster ( DWORD dwNewPriority = THREAD_PRIORITY_TIME_CRITICAL )
		: m_hThread(NULL)
	{
		Boost(dwNewPriority);
	}
	CBooster ( bool bBoost, DWORD dwNewPriority = THREAD_PRIORITY_TIME_CRITICAL )
		: m_hThread(NULL)
	{
		if ( bBoost )
			Boost(dwNewPriority);
	}
	~CBooster ()  {  Restore();  }

	void  Boost ( DWORD dwNewPriority = THREAD_PRIORITY_TIME_CRITICAL )
	{
		m_dwPrevPriority = GetThreadPriority (m_hThread);
		if ( m_dwPrevPriority != dwNewPriority )  {
			m_hThread = GetCurrentThread();
			SetThreadPriority (m_hThread, dwNewPriority);
		}
	}

	void  Restore ()
	{
		if ( m_hThread )  {
			SetThreadPriority (m_hThread, m_dwPrevPriority);
			m_hThread = NULL;
		}
	}
};
#else
class  CBooster
{
	public:

		CBooster ( DWORD dwNewPriority = THREAD_PRIORITY_TIME_CRITICAL )
		{
		}

		CBooster ( bool bBoost, DWORD dwNewPriority = THREAD_PRIORITY_TIME_CRITICAL )
		{
		}

		~CBooster ()
		{
		}

		void  Boost ( DWORD dwNewPriority = THREAD_PRIORITY_TIME_CRITICAL )
		{
		}

		void  Restore ()
		{
		}
};
#endif //_MT

//////////////////////////////////////////////////////////////////////////
// PSAPI declarations
//
#ifndef _PSAPI_H_

typedef struct _MODULEINFO {
	LPVOID lpBaseOfDll;
	DWORD SizeOfImage;
	LPVOID EntryPoint;
} MODULEINFO, *LPMODULEINFO;

#endif  // !_PSAPI_H_

typedef  DWORD (WINAPI *GETMAPPEDFILENAMEA) ( HANDLE hProcess, LPVOID lpv, LPSTR lpFilename, DWORD nSize);
typedef  BOOL  (WINAPI *ENUMPROCESSMODULES) ( HANDLE hProcess, HMODULE *lphModule, DWORD cb, LPDWORD lpcbNeeded);
typedef  BOOL  (WINAPI *GETMODULEINFORMATION) ( HANDLE hProcess, HMODULE hModule, LPMODULEINFO lpmodinfo, DWORD cb);

//////////////////////////////////////////////////////////////////////////
// class CToolHelp
//
class  CToolHelp
{
public:
	#ifndef _INC_TOOLHELP32

	typedef struct tagMODULEENTRY32
	{
		DWORD   dwSize;
		DWORD   th32ModuleID;       // This module
		DWORD   th32ProcessID;      // owning process
		DWORD   GlblcntUsage;       // Global usage count on the module
		DWORD   ProccntUsage;       // Module usage count in th32ProcessID's context
		BYTE  * modBaseAddr;        // Base address of module in th32ProcessID's context
		DWORD   modBaseSize;        // Size in bytes of module starting at modBaseAddr
		HMODULE hModule;            // The hModule of this module in th32ProcessID's context
		char    szModule[256];
		char    szExePath[MAX_PATH];
	} MODULEENTRY32;
	typedef MODULEENTRY32 *  PMODULEENTRY32;
	typedef MODULEENTRY32 *  LPMODULEENTRY32;

	typedef struct tagTHREADENTRY32
	{
		DWORD   dwSize;
		DWORD   cntUsage;
		DWORD   th32ThreadID;       // this thread
		DWORD   th32OwnerProcessID; // Process this thread is associated with
		LONG    tpBasePri;
		LONG    tpDeltaPri;
		DWORD   dwFlags;
	} THREADENTRY32;
	typedef THREADENTRY32 *  PTHREADENTRY32;
	typedef THREADENTRY32 *  LPTHREADENTRY32;

	#define TH32CS_SNAPHEAPLIST 0x00000001
	#define TH32CS_SNAPPROCESS  0x00000002
	#define TH32CS_SNAPTHREAD   0x00000004
	#define TH32CS_SNAPMODULE   0x00000008
	#define TH32CS_SNAPMODULE32 0x00000010
	#define TH32CS_SNAPALL      (TH32CS_SNAPHEAPLIST | TH32CS_SNAPPROCESS | TH32CS_SNAPTHREAD | TH32CS_SNAPMODULE)
	#define TH32CS_INHERIT      0x80000000

	#endif  // !_INC_TOOLHELP32

	enum  Flags  {
			HeapList = TH32CS_SNAPHEAPLIST,
			Process = TH32CS_SNAPPROCESS,
			Thread = TH32CS_SNAPTHREAD,
			Module = TH32CS_SNAPMODULE,
			Module32 = TH32CS_SNAPMODULE32,
			All = TH32CS_SNAPALL,
			Inherit = TH32CS_INHERIT
	};

private:
	typedef  HANDLE  (WINAPI *CREATETOOLHELP32SNAPSHOT) ( DWORD dwFlags, DWORD th32ProcessID );
	typedef  BOOL  (WINAPI *MODULE32FIRST) ( HANDLE hSnapshot, LPMODULEENTRY32 lpme );
	typedef  BOOL  (WINAPI *MODULE32NEXT) ( HANDLE hSnapshot, LPMODULEENTRY32 lpme );
	typedef  BOOL  (WINAPI *THREAD32FIRST) ( HANDLE hSnapshot, LPTHREADENTRY32 lpte );
	typedef  BOOL  (WINAPI *THREAD32NEXT) ( HANDLE hSnapshot, LPTHREADENTRY32 lpte );
	typedef  HANDLE  (WINAPI *OPENTHREAD) ( DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwThreadId);

	CREATETOOLHELP32SNAPSHOT	m_fnCreateToolhelp32Snapshot;
	MODULE32FIRST				m_fnModule32First;
	MODULE32NEXT				m_fnModule32Next;
	THREAD32FIRST				m_fnThread32First;
	THREAD32NEXT				m_fnThread32Next;
	OPENTHREAD					m_fnOpenThread;

	HMODULE			m_hDll;
	HANDLE			m_hSnapshot;
	MODULEENTRY32	m_me32;
	THREADENTRY32	m_te32;

public:
	CToolHelp ()
		: m_hSnapshot(INVALID_HANDLE_VALUE)
	{
		m_hDll = LoadLibrary (_T("kernel32.dll"));
		if ( m_hDll )
		{
			m_fnCreateToolhelp32Snapshot = (CREATETOOLHELP32SNAPSHOT)GetProcAddress(m_hDll, "CreateToolhelp32Snapshot");
			m_fnModule32First = (MODULE32FIRST)GetProcAddress(m_hDll, "Module32First");
			m_fnModule32Next = (MODULE32NEXT)GetProcAddress(m_hDll, "Module32Next");
			m_fnThread32First = (THREAD32FIRST)GetProcAddress(m_hDll, "Thread32First");
			m_fnThread32Next = (THREAD32NEXT)GetProcAddress(m_hDll, "Thread32Next");
			m_fnOpenThread = (OPENTHREAD)GetProcAddress(m_hDll, "OpenThread");
			if ( !m_fnCreateToolhelp32Snapshot  ||  
				 !m_fnModule32First  ||  !m_fnModule32Next  ||
				 !m_fnThread32First  ||  !m_fnThread32Next )
			{
				FreeLibrary (m_hDll);
				m_hDll = NULL;
			}
		}
		ZeroMemory (&m_me32, sizeof(MODULEENTRY32));
		m_me32.dwSize = sizeof(MODULEENTRY32);
		ZeroMemory (&m_te32, sizeof(THREADENTRY32));
		m_te32.dwSize = sizeof(THREADENTRY32);
	}
	~CToolHelp ()
	{
		CloseSnapshot();
		if ( m_hDll )
			FreeLibrary (m_hDll);
	}

	bool  IsAvailable ()  const  {  return  m_hDll ? true : false;  }
	bool  CanGetThreadHandles ()  const  {  return  m_fnOpenThread ? true : false;  }

	bool  CreateSnapshot ( DWORD dwFlags, DWORD dwProcessID = 0 )
	{
		if ( m_hDll )  {
			m_hSnapshot = m_fnCreateToolhelp32Snapshot(dwFlags, dwProcessID);
			if ( m_hSnapshot != INVALID_HANDLE_VALUE )
				return  true;
		}
		return  false;
	}
	void  CloseSnapshot ()
	{
		if ( m_hSnapshot != INVALID_HANDLE_VALUE )  {
			CloseHandle (m_hSnapshot);
			m_hSnapshot = INVALID_HANDLE_VALUE;
		}
	}

	BOOL  Module32First ()  {  return	m_fnModule32First (m_hSnapshot, &m_me32);  }
	BOOL  Module32Next ()  {  return	m_fnModule32Next (m_hSnapshot, &m_me32);  }
	BYTE*  GetModuleBaseAddr ()  {  return  m_me32.modBaseAddr;  }
	DWORD  GetModuleSize ()  {  return  m_me32.modBaseSize;  }
	BOOL  Thread32First ()  {  return	m_fnThread32First (m_hSnapshot, &m_te32);  }
	BOOL  Thread32Next ()  {  return	m_fnThread32Next (m_hSnapshot, &m_te32);  }
	DWORD  GetThreadID ()  {  return  m_te32.th32ThreadID;  }
	DWORD  GetThreadOwnerProcessID ()  const  {  return  m_te32.th32OwnerProcessID;  }
	HANDLE  GetThreadHandle ( DWORD dwDesiredAccess = THREAD_ALL_ACCESS, BOOL bInheritHandle = FALSE )
	{
		return  m_fnOpenThread (dwDesiredAccess, bInheritHandle, m_te32.th32ThreadID);
	}
	
};

//////////////////////////////////////////////////////////////////////////
// Crash report callbacks

// Processor information 
#define  dw  dword ptr

struct  ProcessorSignature
{
	DWORD	m_Stepping		: 4,
			m_Model			: 4,
			m_InstFamily	: 4;
};

struct  ProcessorFeatureBits
{
	DWORD	m_FPU			: 1,	// 0: Floating Point Unit
			m_VME			: 1,	// 1: Virtual Mode Extension
			m_DE			: 1,	// 2: Debugging Extension
			m_PSE			: 1,	// 3: Page Size Extension
			m_RDTSC			: 1,	// 4: Time Stamp Counter
			m_MSR			: 1,	// 5: Model Specific Registers
			m_PAE			: 1,	// 6: Physical Address Extension
			m_MCE			: 1,	// 7: Machine Check Exception
			m_CMPXCHG8B		: 1,	// 8: CMPXCHG8B instruction
			m_APIC			: 1,	// 9: On-chip APIC unit
			m_Reserved		: 1,	// 
			m_SEP			: 1,	// 11: Fast System Call (SYSENTER/SYSEXIT)
			m_MTRR			: 1,	// 12: Memory Type Range Registers
			m_PGE			: 1,	// 13: Paging Global Extension
			m_MCA			: 1,	// 14: Machine-Check Architecture.
			m_CMOV			: 1,	// 15: Conditional Move Instructions (CMOVcc/FCMOVcc/FCOMI)
			m_PAT			: 1,	// 16: Page Attribute Table
			m_PSE36			: 1,	// 17: 36-bit Page Size Extension
			m_IntelAmdSpec	: 5,	// Intel/AMD specific features
			m_MMX			: 1,	// 23: MMX?Technology
			m_FXSR			: 1,	// 24: Fast Save and Restore (FXSAVE/FXSTOR)
			m_SSE			: 1;	// 25: Streaming SIMD Extensions
};

struct  IntelFeatureBits
{
	DWORD	m_CommonBits1	: 19,	// 
			m_CLFLUSH		: 1,	// 19: CLFLUSH Instruction
			m_Reserved1		: 1,	// 
			m_DS			: 1,	// 21: Debug Trace Store.
			m_ACPI			: 1,	// 22: ACPI Support.
			m_CommonBits2	: 3,	// 
			m_SSE2			: 1,	// 26: Streaming SIMD Extensions - 2
			m_SS			: 1,	// 27: Self-Snoop
			m_Reserved2		: 1,	// 
			m_TM			: 1;	// 29: Thermal Monitor
};

struct  AmdFeatureBits
{
	DWORD	m_CommonBits1	: 19,	// 
			m_MP			: 1,	// 19: Multiprocessing Capable
			m_Reserved1		: 2,	// 
			m_MME			: 1,	// 22: Multimedia Instruction Extensions
			m_CommonBits2	: 3,	// 
			m_Reserved2		: 4,	// 
			m_3DNowExt		: 1,	// 30: Extensions to the 3DNow! instructions set
			m_3DNow			: 1;	// 31: 3DNow! instructions set
};

inline
void  CharToTchar ( LPCSTR psz, LPTSTR ptsz, int nLen )
{
	#if defined(_UNICODE) || defined(UNICODE)
		MultiByteToWideChar (CP_ACP, 0, psz, nLen, ptsz, nLen);
	#else
		lstrcpyA (ptsz, psz);
	#endif  // !_UNICODE
}

inline
void  TraceProcessorFeatures ( LPTSTR pszBuf, BOOL bIntel, DWORD dwFeatures, DWORD dwExtFeatures )
{
	ProcessorFeatureBits	pfb = *(ProcessorFeatureBits*)&dwFeatures;
	int		nLen = 0;
	nLen += wsprintf (pszBuf + nLen, _T("        Processor features: "));
	// \r\n
	if ( pfb.m_FPU )
		nLen += wsprintf (pszBuf + nLen, _T("FPU, "));
	if ( pfb.m_MMX )
		nLen += wsprintf (pszBuf + nLen, _T("MMX, "));
	if ( pfb.m_SSE )
		nLen += wsprintf (pszBuf + nLen, _T("SSE, "));
	if ( bIntel )  {
		IntelFeatureBits	ifb = *(IntelFeatureBits*)&dwExtFeatures;
		if ( ifb.m_SSE2 )
			nLen += wsprintf (pszBuf + nLen, _T("SSE2, "));
		if ( ifb.m_CLFLUSH )
			nLen += wsprintf (pszBuf + nLen, _T("CLFLUSH, "));
	}
	else  {
		AmdFeatureBits	afb = *(AmdFeatureBits*)&dwExtFeatures;
		if ( afb.m_MME )
			nLen += wsprintf (pszBuf + nLen, _T("MMX extensions, "));
		if ( afb.m_3DNow )
			nLen += wsprintf (pszBuf + nLen, _T("3DNow, "));
		if ( afb.m_3DNowExt )
			nLen += wsprintf (pszBuf + nLen, _T("3DNow extensions, "));
	}
	if ( pfb.m_RDTSC )
		nLen += wsprintf (pszBuf + nLen, _T("RDTSC, "));
	if ( pfb.m_CMPXCHG8B )
		nLen += wsprintf (pszBuf + nLen, _T("CMPXCHG8B, "));
	if ( pfb.m_CMOV )
		nLen += wsprintf (pszBuf + nLen, _T("CMOV, "));

	if ( pszBuf[nLen - 2] == _T(',') )
		nLen -= 2;
	nLen += wsprintf (pszBuf + nLen, _T("\r\n        More processor features: "));

	if ( pfb.m_VME )
		nLen += wsprintf (pszBuf + nLen, _T("VME, "));
	if ( pfb.m_DE )
		nLen += wsprintf (pszBuf + nLen, _T("DE, "));
	if ( pfb.m_PSE )
		nLen += wsprintf (pszBuf + nLen, _T("PSE, "));
	if ( pfb.m_MSR )
		nLen += wsprintf (pszBuf + nLen, _T("MSR, "));
	if ( pfb.m_PAE )
		nLen += wsprintf (pszBuf + nLen, _T("PAE, "));
	if ( pfb.m_MCE )
		nLen += wsprintf (pszBuf + nLen, _T("MCE, "));
	if ( pfb.m_APIC )
		nLen += wsprintf (pszBuf + nLen, _T("APIC, "));
	if ( pfb.m_SEP )
		nLen += wsprintf (pszBuf + nLen, _T("SEP, "));
	if ( pfb.m_MTRR )
		nLen += wsprintf (pszBuf + nLen, _T("MTRR, "));
	if ( pfb.m_PGE )
		nLen += wsprintf (pszBuf + nLen, _T("PGE, "));
	if ( pfb.m_MCA )
		nLen += wsprintf (pszBuf + nLen, _T("MCA, "));
	if ( pfb.m_PAT )
		nLen += wsprintf (pszBuf + nLen, _T("PAT, "));
	if ( pfb.m_PSE36 )
		nLen += wsprintf (pszBuf + nLen, _T("PSE36, "));
	if ( pfb.m_FXSR )
		nLen += wsprintf (pszBuf + nLen, _T("FXSR, "));

	if ( bIntel )  {
		IntelFeatureBits	ifb = *(IntelFeatureBits*)&dwExtFeatures;
		if ( ifb.m_DS )
			nLen += wsprintf (pszBuf + nLen, _T("DS, "));
		if ( ifb.m_SS )
			nLen += wsprintf (pszBuf + nLen, _T("SS, "));
		if ( ifb.m_TM )
			nLen += wsprintf (pszBuf + nLen, _T("TM, "));
	}
	else  {
		AmdFeatureBits	afb = *(AmdFeatureBits*)&dwExtFeatures;
		if ( afb.m_MP )
			nLen += wsprintf (pszBuf + nLen, _T("MP, "));
	}
	if ( pszBuf[nLen - 2] == _T(',') )
		nLen -= 2;
	wsprintf (pszBuf + nLen, _T("\r\n"));
}

inline
void  GetProcessorInfo ( LPTSTR pszBuf )
{
	_ASSERTE (sizeof(AmdFeatureBits) == 4  &&  sizeof(IntelFeatureBits) == 4);

	int		nMaxStdFn, nMaxExtFn;
	char	pszVendor[13],
			pszProcessorName[49];
	TCHAR	ptszVendor[13],
			ptszProcessorName[49];
	pszVendor[12] = 0;
	pszProcessorName[48] = 0;
	__asm  {
		xor		eax, eax
		cpuid
		mov		nMaxStdFn, eax
		mov		dw pszVendor, ebx
		mov		dw pszVendor[4], edx
		mov		dw pszVendor[8], ecx
		mov		eax, 80000000h
		cpuid
		mov		nMaxExtFn, eax
	}
	CharToTchar (pszVendor, ptszVendor, 13);

	bool	bIntel = lstrcmpA (pszVendor, "AuthenticAMD") != 0;

	ProcessorSignature		ps;
	DWORD		dwFeatures,
				dwExtFeatures;
	if ( nMaxStdFn >= 1 )  __asm  {
		mov		eax, 1
		cpuid
		mov		dw ps, eax
		mov		dwFeatures, edx
	}
	if ( bIntel )  
		dwExtFeatures = dwFeatures;
	else if ( nMaxExtFn >= 0x80000001 )  __asm  {
		mov		eax, 80000001h
		cpuid
		mov		dwExtFeatures, edx
	}

	if ( nMaxExtFn >= 0x80000004 )  __asm  {
		mov		eax, 80000002h
		lea		edi, pszProcessorName
		cpuid
		call save_string
		mov eax, 80000003h
		cpuid
		call save_string
		mov eax, 80000004h
		cpuid
		call save_string
		jmp  end

		save_string:
		mov dword ptr [edi], eax
		mov dword ptr [edi+4], ebx
		mov dword ptr [edi+8], ecx
		mov dword ptr [edi+12], edx
		add edi, 16
		ret
		end:
	}
	CharToTchar (pszProcessorName, ptszProcessorName, 49);

	int		nLen = 0;
	nLen += wsprintf (pszBuf + nLen, _T("        Processor type: %s, stepping %u, model %u, instruction family %u\r\n"),
					 ptszVendor, ps.m_Stepping, ps.m_Model, ps.m_InstFamily);
	if ( nMaxExtFn >= 0x80000004 )
		nLen += wsprintf (pszBuf + nLen, _T("        Processor name: %s\r\n"), ptszProcessorName);

	TraceProcessorFeatures (pszBuf + nLen, bIntel, dwFeatures, dwExtFeatures);
}

// Basic computer configuration info callback
inline
void  ComputerCfgInfo ( LPTSTR pszBuf )
{
	// The size of pszBuf is 4K
	OSVERSIONINFO	vi = { sizeof(OSVERSIONINFO) };
	SYSTEM_INFO		si;
	GetVersionEx (&vi);
	GetSystemInfo(&si);
	int		nLen = 0;
	nLen += wsprintf (pszBuf + nLen, _T("        OS type: %s\r\n"), 
					 (vi.dwPlatformId == VER_PLATFORM_WIN32_NT ? _T("WIN32 NT") : _T("WINDOWS")));
	nLen += wsprintf (pszBuf + nLen, _T("        OS version: major %u, minor %u, build %u\r\n"), 
					  vi.dwMajorVersion, vi.dwMinorVersion, vi.dwBuildNumber);
	nLen += wsprintf (pszBuf + nLen, _T("        Number of processors: %u\r\n"), si.dwNumberOfProcessors);
	GetProcessorInfo (pszBuf + nLen);
}

// Custom message about failure
inline
void  CrashReportCallback ( PEXCEPTION_POINTERS pei, HANDLE hRptFile, 
						    LPCTSTR pcszRptPath, LPCTSTR pcszRptFileName, 
							LPCTSTR pcszAppFullPath, LPCTSTR pcszAppFileName )
{
	LPCTSTR		pszcErrorType = pei ? _T("Unexpected problem") : _T("Deadlock");
	TCHAR		pszMsg[256];
	wsprintf (pszMsg, _T("%s detected in the program \"%s\".\r\n\r\n")
					  _T("See the report file \"%s\" for more info."), 
			  pszcErrorType, pcszAppFileName, pcszRptPath);		// pcszRptFileName
	MessageBox (NULL, pszMsg, pcszAppFullPath, MB_OK | MB_ICONERROR);
	// Close file handle only if the process will be terminated by the callback.
	CloseHandle(hRptFile);
	//     This callback prevents standard Windows' application error message box from 
	// being subsequently displayed by simply terminating the current process:
	TerminateProcess (GetCurrentProcess(), 1);
	//     A good idea is to use this callback in the release builds to mail crash report
	// to the developer.
}


//////////////////////////////////////////////////////////////////////////
// dbgTracer helpers
//
inline
bool  AddNameToPath ( LPTSTR pszPath, long lPathBufSize, LPCTSTR pcszFileName )
{
	// iStartPos is used to skip the possible leading (back-)slash in the pszFileName
	int		iStartPos = (pcszFileName[0] == _T('\\') || pcszFileName[0] == _T('/')) ? 1 : 0;
	long	i;
	for ( i = lstrlen(pszPath) - 1; i >= 0; i-- )  {
		if ( pszPath[i] == _T('\\')  ||  pszPath[i] == _T('/') )
			break;
	}
	lstrcpyn (pszPath + i + 1, pcszFileName + iStartPos, max(lPathBufSize - (i + 2) , 0));
	pszPath[lPathBufSize - 1] = NULL;
	return  true;
}
inline
bool  AddNameToModulePath ( LPTSTR pszPath, long lPathBufSize, LPCTSTR pcszFileName )
{
	if ( !pcszFileName  ||  !GetModuleFileName (GetModuleHandle(NULL), pszPath, lPathBufSize) )
		return  false;
	return  AddNameToPath (pszPath, lPathBufSize, pcszFileName);
}

inline
LPCTSTR  GetFileNamePtr ( LPCTSTR pcszPath )
{
	if ( !pcszPath  ||  !pcszPath[0] )
		return  NULL;
	int		nLen = lstrlen(pcszPath);
	int		i;
	for ( i = nLen - 1; i >= 0; i-- )  {
		if ( pcszPath[i] == '\\'  ||  pcszPath[i] == '/' )
			break;
	}
	return  pcszPath + i + 1;
}

inline
LPTSTR  GetTstr ( const std::string& str )
{
	LPTSTR  psz = new TCHAR[str.length() + 1];
	#if defined(_UNICODE) || defined(UNICODE)
		MultiByteToWideChar (CP_ACP, 0, str.c_str(), str.length() + 1, psz, str.length() + 1);
	#else
		lstrcpyA (psz, str.c_str());
	#endif  // !_UNICODE
	return  psz;
}

inline
std::string  ExtractPath ( PSTR pszPath, long nLen )
{
	int i;
	for ( i = nLen - 1; i >= 0; i-- )  {
		if ( pszPath[i] == '\\'  ||  pszPath[i] == '/' )
			break;
	}
	pszPath[i + 1] = 0;
	return  pszPath;
}
inline
std::string  ExtractName ( PSTR pszPath, long nLen )
{
	int		i;
	for ( i = nLen - 1; i >= 0; i-- )  {
		if ( pszPath[i] == '\\'  ||  pszPath[i] == '/' )
			break;
	}
	return  pszPath + i + 1;
}


//////////////////////////////////////////////////////////////////////////
// class  dbgTracer
//
#define  CheckTraceLevelCorrectness(nLevel)		\
	_ASSERTE ((nLevel >= 0  &&  nLevel <= MAX_SUPPORTED_TRACE_LEVEL) || nLevel == CRASH_REPORT_LEVEL);	\
	if ( !(nLevel >= 0  &&  nLevel <= MAX_SUPPORTED_TRACE_LEVEL  ||  nLevel == CRASH_REPORT_LEVEL) )	\
		return;

#define  IsTraceLevelDisabled(nLevel)								\
	(m_pActiveLevelsSet->empty() ?									\
				((nLevel > *m_pnMaxLevel  ||  nLevel < 0)  &&		\
				nLevel != CRASH_REPORT_LEVEL) :						\
				!std::binary_search (m_pActiveLevelsSet->begin(), m_pActiveLevelsSet->end(), nLevel))

enum  TraceFlags  { Prefix = 1, TrailingLineFeed = 2, RollbackInfo = 4,
					Pf = Prefix, Lf = TrailingLineFeed, Rb = RollbackInfo,
					PfLf = Prefix | TrailingLineFeed,
					PfLfRb = Prefix | TrailingLineFeed | RollbackInfo,
					DontAssert = 8,
					ExtraNestingMask = 0xC000
				  };
#define  GET_EXTRA_NESTING(dwFlags)				((dwFlags & ExtraNestingMask) >> 14)
#define  SET_EXTRA_NESTING(dwFlags, dwNesting)	((dwFlags & ~ExtraNestingMask) | (dwNesting & 3) << 14)

#define  NO_SYM_HANDLER		(-1L)

#define  NPOS		std::string::npos

class  dbgTracer
{
	//     Because of the bug in STL (incorrect destruction of static containers based on
	// _Tree class), the m_pActiveLevelsSet member is compelled to be a pointer to the 'vector'
	// instead of more convenient 'set' container. This bug could have been worked around by
	// allocating a pointer to the set dynamically and never destroying it. Yet such a technique
	// causes MFC to produce memory leak report in debug builds (global allocator does not
	// help to get rid of the warning). Though this leak is harmless, the warnings are too
	// embarrassing to put up with them.

	typedef  std::list<UINT>		FILE_POS_LIST;
	typedef  std::set<std::string>	PATHS_SET;
	typedef  PATHS_SET::iterator	PATHS_SET_IT;
	typedef  std::vector<int>		LEVELS_VEC;
	typedef  LEVELS_VEC::iterator	LEVELS_VEC_IT;

	//     m_hMutex:
	// Used to reveal whether symbolic info has already been initialized for the current process
	HANDLE		m_hMutex;

	//     m_iLevel, m_pszFile, m_dwLine, m_dwTraceFlags:
	//     These members are set by the SetTraceInfo method (which is called by TRACE and
	// DUMP macros) to route this information to the Trace and Dump methods correspondingly.
	int			m_iLevel;
	LPTSTR		m_pszFile;
	DWORD		m_dwLine;
	DWORD		m_dwTraceFlags;

	int					*m_pnMaxLevel, *m_pnActiveLevel;
	LEVELS_VEC			*m_pActiveLevelsSet;

	//     m_pdwOpts, m_phRptFiles, m_pFilePosLists:
	//     These members point to the arrays of objects, which elements contain settings
	// associated with corresponding trace level. The settings for the CRASH_REPORT_LEVEL
	// are kept in the next group of members.
	DWORD			*m_pdwOpts;
	HANDLE			*m_phRptFiles;
	FILE_POS_LIST	*m_pFilePosLists;

	//     m_pdwCrashRptOpts, m_pdwCrashRptFileOpts, m_phCrashRptFile, m_pszCrashRptFile:
	//     Crash report related info (CRASH_REPORT_LEVEL). The file name is stored in order 
	// to open the report file only when a fault occur. This prevents the previous crash
	// report from being instantly erased by the next run of the program, because by default 
	// *m_pdwCrashRptOpts contains TraceNewLog flag.
	DWORD			*m_pdwCrashRptOpts,
					*m_pdwCrashRptFileOpts;
	LPTSTR			m_pszCrashRptFile;
	HANDLE			*m_phCrashRptFile;

	//     m_pfnComputerCfgInfo, m_pfnCrashRptCallback:
	//     Callback functions that can be registered by the application to extend crash 
	// reporting capabilities.
	COMPUTER_CFG_INFO		*m_pfnComputerCfgInfo;
	CRASHREPORTCALLBACK		*m_pfnCrashRptCallback;

	__int64			m_i64PerfFreq,
					m_i64PerfCntAmendment, m_i64PerfCntAmendmentUTC;
	SYSTEMTIME		m_stCurrent;

	DWORD		*m_pnObjects;

	LPTSTR		*m_ppszBuffer;
	long		*m_pnBufferSize,
				*m_pnBufferCurPos,
				*m_pnBufferDeadLine,
				*m_pnBufferEnd;

	long		*m_pnExtraNesting, 
				*m_pnPrepareFmtStringExtraNesting;

	DWORD		*m_pdwStackTraceOpts;
		
	DWORD		*m_pnTestNumber;
	LPTSTR		m_pszTestsSeparatorFmt;


	LPTSTR  GetExpectedBuf ()
	{
		static  TCHAR	s_pszExpected[MAX_TRACE_MSG_LEN] = { 0 };
		return  s_pszExpected;
	}
	LPTSTR  GetResultedBuf ()  {
		static  TCHAR	s_pszResulted[MAX_TRACE_MSG_LEN];
		return  s_pszResulted;
	}

	__int64  GetTimes ( FILETIME& ftCreationTime, FILETIME& ftKernelMode, FILETIME& ftUserMode,
						DWORD& dwSec, DWORD& dwMilliSec, DWORD& dwMicroSec, bool bReal )
	{
		__int64		t = 0, i64PerfCnt = 0;
		if ( bReal )  {
			t = ((__int64)(ftKernelMode.dwHighDateTime + ftUserMode.dwHighDateTime)*__int64(ULONG_MAX)
				+ (__int64)(ftKernelMode.dwLowDateTime + ftUserMode.dwLowDateTime)) / 10;
		}
		else  {
			FILETIME	ftCurTime;
			if ( m_i64PerfFreq )  {
				QueryPerformanceCounter((PLARGE_INTEGER)&i64PerfCnt);
				*(PINT64)&ftCurTime = ((i64PerfCnt * 10000000) / m_i64PerfFreq + m_i64PerfCntAmendmentUTC);
			}
			else
				GetSystemTimeAsFileTime (&ftCurTime);
			t = (*(PINT64)&ftCurTime - *(PINT64)&ftCreationTime) / 10;
		}
		dwMicroSec = t % 1000;
		dwMilliSec = (t / 1000) % 1000;
		dwSec = (t / 1000000) % 1000;
		return  t;
	}
	__int64  GetCurThreadTime ( DWORD& dwSec, DWORD& dwMilliSec, DWORD& dwMicroSec, bool bReal )
	{
		FILETIME	ftCreationTime, ftExit, ftKernelMode, ftUserMode;
		if ( GetThreadTimes (GetCurrentThread(), &ftCreationTime, &ftExit, &ftKernelMode, &ftUserMode) )
			return  GetTimes (ftCreationTime, ftKernelMode, ftUserMode, dwSec, dwMilliSec, dwMicroSec, bReal);
		else
			return  0;
	}
	__int64  GetCurProcessTime ( DWORD& dwSec, DWORD& dwMilliSec, DWORD& dwMicroSec, bool bReal )
	{
		FILETIME	ftCreationTime, ftExit, ftKernelMode, ftUserMode;
		if ( GetProcessTimes (GetCurrentProcess(), &ftCreationTime, &ftExit, &ftKernelMode, &ftUserMode) )
			return  GetTimes (ftCreationTime, ftKernelMode, ftUserMode, dwSec, dwMilliSec, dwMicroSec, bReal);
		else
			return  0;
	}

	DWORD  RemoveTrailingNewline ( LPTSTR pszMsg )
	{
		DWORD	dwLen = lstrlen(pszMsg);
		if ( pszMsg[dwLen - 1] == _T('\n') )  {
			if ( pszMsg[dwLen - 2] == _T('\r') )
				dwLen -= 2;
			else
				dwLen -= 1;
			pszMsg[dwLen] = 0;
		}
		return  dwLen;
	}
	void  AddTrailingNewline ( LPTSTR pszMsg )
	{
		DWORD	dwLen = lstrlen(pszMsg);
		if ( pszMsg[dwLen - 1] == _T('\n') )  {
			if ( pszMsg[dwLen - 2] == _T('\r') )
				return;
			dwLen--;
		}
		pszMsg[dwLen] = _T('\r');
		pszMsg[dwLen + 1] = _T('\n');
		pszMsg[dwLen + 2] = 0;
	}

	void  DumpData ( LPTSTR pszMsg, PVOID pvData, DWORD dwDataSize, DWORD dwParams, DWORD nAbsentElems )
	{
		bool	bHex = (dwParams & DumpRadixMask) == DumpHex;
		bool	bComma = (dwParams & DumpDelimMask) == DumpDelimComma;
		TCHAR	pszDumpFmt[16];
		DWORD	dwLen = lstrlen(pszMsg);

		switch ( dwParams & DumpBoundMask )  {
		case  DumpBoundBrackets:
			pszMsg[dwLen++] = _T('[');
			break;
		case  DumpBoundBraces:
			pszMsg[dwLen++] = _T('{');
			break;
		case  DumpBoundAngleBrackets:
			pszMsg[dwLen++] = _T('<');
			break;
		default:
			break;
		}
		if ( (dwParams & DumpBoundMask) != DumpBoundNone )
			pszMsg[dwLen++] = _T(' ');

		DWORD	dwElemCellLen = 0;
		switch ( dwParams & DumpElemSizeMask )  {
		case  DumpBytes:
		{
			DWORD	nElems = dwDataSize / sizeof(BYTE);
			PBYTE	&pbData = reinterpret_cast<PBYTE&>(pvData);
			wsprintf (pszDumpFmt, (bHex ? _T("%%02X%s") : _T("%%03u%s")), (bComma ? _T(", ") : _T(" ")));
			for ( int i = 0; i < nElems; i++ )
				dwLen += _stprintf (pszMsg + dwLen, pszDumpFmt, pbData[i]);
			dwElemCellLen = (bHex ? 2 : 3) + (bComma ? 2 : 1);
			break;
		}
		case  DumpWords:
		{
			DWORD	nElems = dwDataSize / sizeof(WORD);
			PWORD	&pwData = reinterpret_cast<PWORD&>(pvData);
			wsprintf (pszDumpFmt, (bHex ? _T("%%04X%s") : _T("%%05u%s")), (bComma ? _T(", ") : _T(" ")));
			for ( int i = 0; i < nElems; i++ )
				dwLen += _stprintf (pszMsg + dwLen, pszDumpFmt, pwData[i]);
			dwElemCellLen = (bHex ? 4 : 5) + (bComma ? 2 : 1);
			break;
		}
		case  DumpDwords:
		{
			DWORD	nElems = dwDataSize / sizeof(DWORD);
			PDWORD	&pdwData = reinterpret_cast<PDWORD&>(pvData);
			wsprintf (pszDumpFmt, (bHex ? _T("%%08X%s") : _T("%%010u%s")), (bComma ? _T(", ") : _T(" ")));
			for ( int i = 0; i < nElems; i++ )
				dwLen += _stprintf (pszMsg + dwLen, pszDumpFmt, pdwData[i]);
			dwElemCellLen = (bHex ? 8 : 10) + (bComma ? 2 : 1);
			break;
		}
		case  DumpQwords:
		{
			DWORD	nElems = dwDataSize / sizeof(UINT64);
			PUINT64	&pqwData = reinterpret_cast<PUINT64&>(pvData);
			wsprintf (pszDumpFmt, (bHex ? _T("%%016I64X%s") : _T("%%020I64u%s")), (bComma ? _T(", ") : _T(" ")));
			for ( int i = 0; i < nElems; i++ )
				dwLen += _stprintf (pszMsg + dwLen, pszDumpFmt, pqwData[i]);
			dwElemCellLen = (bHex ? 16 : 20) + (bComma ? 2 : 1);
			break;
		}
		default:
			break;
		}
		if ( bComma )
			pszMsg[dwLen - 2] = _T(' ');	// removing trailing comma
		if ( nAbsentElems )
		{
			for ( int i = 0; i < nAbsentElems * dwElemCellLen; i++, dwLen++ )
				pszMsg[dwLen] = _T(' ');
		}

		if ( dwParams & DumpTextReprMask )  {
			pszMsg[dwLen++] = _T(' ');
			switch ( dwParams & DumpTextReprMask )  {
			case  DumpAscii:
				{
					#if defined(UNICODE) || defined(_UNICODE)
						LPCTSTR	pcszCharFmt = _T("%C");
					#else
						LPCTSTR	pcszCharFmt = _T("%c");
					#endif
					DWORD	nElems = dwDataSize / sizeof(CHAR);
					PCHAR	&pcData = reinterpret_cast<PCHAR&>(pvData);
					for ( int i = 0; i < nElems; i++ )  {
						CHAR	c = isprint(pcData[i]) ? pcData[i] : '.';
						dwLen += wsprintf (pszMsg + dwLen, pcszCharFmt, c);
					}
					break;
				}
			case  DumpUnicode:
				{
					#if defined(UNICODE) || defined(_UNICODE)
						LPCTSTR	pcszCharFmt = _T("%c");
					#else
						LPCTSTR	pcszCharFmt = _T("%C");
					#endif
					DWORD	nElems = dwDataSize / sizeof(WCHAR);
					PWCHAR	&pwcData = reinterpret_cast<PWCHAR&>(pvData);
					for ( int i = 0; i < nElems; i++ )  {
						WCHAR	wc = iswprint(pwcData[i]) ? pwcData[i] : L'.';
						dwLen += wsprintf (pszMsg + dwLen, pcszCharFmt, wc);
					}
					break;
				}
			case  DumpTchars:
				{
					DWORD	nElems = dwDataSize / sizeof(TCHAR);
					TCHAR	*&ptcData = reinterpret_cast<TCHAR*&>(pvData);
					for ( int i = 0; i < nElems; i++ )  {
						TCHAR	tc = _istprint(ptcData[i]) ? ptcData[i] : _T('.');
						dwLen += wsprintf (pszMsg + dwLen, _T("%c"), tc);
					}
					break;
				}
			default:
				break;
			}
			pszMsg[dwLen++] = _T(' ');
		}

		switch ( dwParams & DumpBoundMask )  {
		case  DumpBoundBrackets:
			pszMsg[dwLen++] = _T(']');
			break;
		case  DumpBoundBraces:
			pszMsg[dwLen++] = _T('}');
			break;
		case  DumpBoundAngleBrackets:
			pszMsg[dwLen++] = _T('>');
			break;
		default:
			break;
		}
		pszMsg[dwLen] = _T('\r');
		pszMsg[dwLen + 1] = _T('\n');
		pszMsg[dwLen + 2] = 0;
	}

	DWORD  GetParams ( int nLevel, PHANDLE phFile = NULL, FILE_POS_LIST** ppfpl = NULL )
	{
		DWORD	dwOpts;
		if ( nLevel == CRASH_REPORT_LEVEL )  {
			dwOpts = *m_pdwCrashRptOpts;
			if ( phFile )  {
				if ( !*m_phCrashRptFile )
					OpenRptFile (m_phCrashRptFile, m_pszCrashRptFile, *m_pdwCrashRptFileOpts);
				*phFile = *m_phCrashRptFile;
			}
		}
		else  {
			dwOpts = m_pdwOpts[nLevel];
			if ( phFile )  {
				*phFile = m_phRptFiles[nLevel];
				*ppfpl = m_pFilePosLists + nLevel;
			}
		}
		return  dwOpts;
	}

	void  PrepareFmtString ( int nLevel, LPTSTR pszFmt, LPCTSTR pcszSrcFmt, DWORD dwForceFnInfo = 0, DWORD dwForceOpts = -1 )
	{
		DWORD	nLen = 0,
				dwOpts = (dwForceOpts == -1 ? GetParams(nLevel) : dwForceOpts);

		pszFmt[0] = 0;

		if ( dwOpts & TraceLocation )
			nLen += wsprintf (pszFmt + nLen, _T("%s(%d) : "), m_pszFile, m_dwLine);

		if ( dwOpts & TraceSystemTime )  {
			//EnterCriticalSection (&m_critSettings);
			DWORD	dwHr, dwMin, dwSec, dwMs, dwMcs = 0;
			if ( m_i64PerfFreq )  {
				__int64		i64PerfCnt;
				QueryPerformanceCounter((PLARGE_INTEGER)&i64PerfCnt);
				i64PerfCnt = (i64PerfCnt * 10000000) / m_i64PerfFreq + m_i64PerfCntAmendment;
				if ( dwOpts & TraceMicroseconds )
					dwMcs = (i64PerfCnt / 10) % 1000;
				dwMs = (i64PerfCnt / 10000) % 1000;
				dwSec = (i64PerfCnt / 10000000) % 60;
				dwMin = (i64PerfCnt / 600000000) % 60;
				dwHr = (i64PerfCnt / 36000000000) % 24;
			}
			else  {
				SYSTEMTIME	st;
				GetLocalTime (&st);
				dwMs = st.wMilliseconds;
				dwSec = st.wSecond;
				dwMin = st.wMinute;
				dwHr = st.wHour;
			}
			if ( dwOpts & TraceMicroseconds )
				nLen += wsprintf (pszFmt + nLen, _T("%02d:%02d:%02d:%03d.%03d "), dwHr, dwMin, dwSec, dwMs, dwMcs);
			else
				nLen += wsprintf (pszFmt + nLen, _T("%02d:%02d:%02d.%03d "), dwHr, dwMin, dwSec, dwMs);
			//LeaveCriticalSection (&m_critSettings);
		}
		if ( dwOpts & TracePrThrID )  {
			if ( (dwOpts & TracePrThrID) == TracePrThrID )  {
				nLen += wsprintf (pszFmt + nLen, (dwOpts & TraceRtiHex ? _T("[%04X,%04X] ") : _T("[%05u,%05u] ")), 
								  GetCurrentProcessId(), GetCurrentThreadId());
			}
			else  {
				nLen += wsprintf (pszFmt + nLen, (dwOpts & TraceRtiHex ? _T("[%04X] ") : _T("[%05u] ")), 
								  (dwOpts & TracePID ? GetCurrentProcessId() : GetCurrentThreadId()));
			}
		}
		if ( dwOpts & TracePrThrTime )
		{
			bool	bReal = (dwOpts & TraceRealPrThrTime ? true : false),
					bMicroseconds = dwOpts & TraceMicroseconds  &&  !bReal;

			if ( (dwOpts & TracePrThrTime) == TracePrThrTime )  {
				DWORD	dwSecThr, dwMilliSecThr, dwMicroSecThr,
						dwSecPr, dwMilliSecPr, dwMicroSecPr;
				GetCurThreadTime (dwSecThr, dwMilliSecThr, dwMicroSecThr, bReal);
				GetCurProcessTime (dwSecPr, dwMilliSecPr, dwMicroSecPr, bReal);
				if ( bMicroseconds )  {
					nLen += wsprintf (pszFmt + nLen, _T("{%04u:%03u.%03u,%04u:%03u.%03u} "), 
									  dwSecPr, dwMilliSecPr, dwMicroSecPr, 
									  dwSecPr, dwMilliSecPr, dwMicroSecPr);
				}
				else  {
					nLen += wsprintf (pszFmt + nLen, _T("{%04u.%03u,%04u.%03u} "), 
									  dwSecPr, dwMilliSecPr, dwSecPr, dwMilliSecPr);
				}
			}
			else  {
				DWORD	dwSec, dwMilliSec, dwMicroSec;
				if ( dwOpts & TracePrTime )
					GetCurProcessTime (dwSec, dwMilliSec, dwMicroSec, bReal);
				else
					GetCurThreadTime (dwSec, dwMilliSec, dwMicroSec, bReal);
				if ( bMicroseconds )
					nLen += wsprintf (pszFmt + nLen, _T("{%04u:%03u.%03u} "), dwSec, dwMilliSec, dwMicroSec);
				else
					nLen += wsprintf (pszFmt + nLen, _T("{%04u.%03u} "), dwSec, dwMilliSec);
			}
		}

		#if !defined(TRACE_NO_SYM_INFO)
		DWORD	dwExtraNesting = *m_pnPrepareFmtStringExtraNesting + GET_EXTRA_NESTING(dwForceFnInfo);
		dwForceFnInfo &= ~ExtraNestingMask;
		dwForceFnInfo = dwForceFnInfo ? dwForceFnInfo : dwOpts & TraceFnMask;
		if ( dwForceFnInfo & (TraceFnName | TraceFnArgTypes | TraceFnArgs) )
		{
			std::string		strFn = GetCallerFn(dwForceFnInfo, dwExtraNesting);
			if ( nLen + strFn.length() + 3 >= MAX_TRACE_MSG_LEN )  {
				_ASSERTE (!"The size of additional information prepended to the trace message exceeds MAX_TRACE_MSG_LEN limit");
				return;
			}
			LPTSTR	pszFn = GetTstr (strFn.c_str());
			nLen += wsprintf (pszFmt + nLen, _T("%s : "), pszFn);
			delete []  pszFn;
		}
		#endif // !TRACE_NO_SYM_INFO

		if ( nLen )  {
			_ASSERTE (nLen > 1);
			if ( pszFmt[nLen - 2] != _T(':') )  {
				_ASSERTE (pszFmt[nLen - 1] == _T(' '));
				lstrcpy (pszFmt + nLen, _T(": "));
				nLen += 2;
			}
		}
		wsprintf (pszFmt + nLen, _T("%s"), pcszSrcFmt);
	}

	void  OutputMsgToMemBuffer ( LPCTSTR pszMsg, long nMsgSize )
	{
		DWORD	dwCurPos, dwNewPos, dwPrevPos, dwBufferEnd;
		// Step 1: calculating nMsgSize - already done
		start:
		// Step 2: 
		dwCurPos = InterlockedExchangeAdd (m_pnBufferCurPos, nMsgSize);
		dwNewPos = *m_pnBufferCurPos;
		dwBufferEnd = *m_pnBufferEnd;	
		if ( dwCurPos > *m_pnBufferDeadLine )
		{
			//     We can get here only if the concurrent thread (thread 1) has been 
			// preempted by this thread (thread 2) while it was storing the message that
			// was to cross the buffer's deadline. The context switch must have been
			// occured after step 2's InterlockedExchangeAdd but before the step 5's
			// InterlockedCompareExchange.
			//     If the thread 2 reaches step 2 just before being preempted in its turn 
			// by the thread 1, the thread switch can occur between the steps 2 and 3.
			// In this case the thread 1 may update the *m_pnBufferCurPos itself in its
			// step 3 handler (because its step 5 won't do this update due to the the 
			// thread 2's step 2, that has already modified (incremented) *m_pnBufferCurPos).
			// If this occur the thread 2 does not update the m_dwBufferCurPos variable
			// and repeat its step 2 in order to get correct position for writing.
			//     Note also that in such a case the thread 1 will properly update the
			// *m_pnBufferEnd variable in its step 5, because it has not been updated by
			// the thread 2 yet.
			
			// Step 3:	
			dwPrevPos = InterlockedCompareExchange  (m_pnBufferCurPos, nMsgSize, dwNewPos);
			if ( dwPrevPos == dwNewPos )  {
				// No context switches occured
				InterlockedCompareExchange  (m_pnBufferEnd, dwCurPos, dwBufferEnd);
				dwCurPos = 0;
			}
			else
				// There were context switches since the step 2 started - repeat it
				goto start;
		}
		else  {
			// Step 4:
			if ( dwNewPos > *m_pnBufferDeadLine )
			{
				// Update *m_pnBufferCurPos and *m_pnBufferEnd only if they 
				// has not been updated by a concurrent thread
				
				// Step 5:
				dwPrevPos = InterlockedCompareExchange  (m_pnBufferCurPos, 0, dwNewPos);
				if ( dwPrevPos == dwNewPos )
					// No context switches occured
					InterlockedCompareExchange  (m_pnBufferEnd, dwNewPos, dwBufferEnd);
				else
					goto start;
			}
		}
		CopyMemory (*m_ppszBuffer + dwCurPos, pszMsg, nMsgSize);
	}

	void  OutputMsgToFile ( int nLevel, LPCTSTR pszMsg, long nMsgSize, bool bSetRollbackInfo )
	{
		HANDLE	hFile;
		FILE_POS_LIST		*pFilePosList;
		DWORD	dwOpts = GetParams(nLevel, &hFile, &pFilePosList);
		if ( !hFile )
			return;

		DWORD	dwWritten;
		bool	bCritSectEntered = false;
		#if !defined(TRACE_NO_PRIORITY_BOOST)
			CBooster		boost;
		#endif
		SetFilePointer (hFile, NULL, NULL, FILE_END);
		if ( bSetRollbackInfo )  {
			pFilePosList->push_back (GetFileSize(hFile, NULL));
			if ( pFilePosList->size() > MAX_ROLLBACKS )
				pFilePosList->pop_front();
		}
		WriteFile (hFile, pszMsg, nMsgSize, &dwWritten, NULL);
	}

	void  OutputMsg ( int nLevel, LPCTSTR pszMsg, bool bSetRollbackInfo = false )
	{
		DWORD	dwOpts = GetParams(nLevel);
		long	nMsgLen = lstrlen(pszMsg);

		if ( dwOpts & TraceMemBuffer  &&  *m_ppszBuffer )
			OutputMsgToMemBuffer (pszMsg, nMsgLen + 1);
		else  {
			if ( dwOpts & TraceDbgOutput )
				OutputDebugString (pszMsg);
			OutputMsgToFile (nLevel, pszMsg, nMsgLen * sizeof(TCHAR), bSetRollbackInfo);
		}
	}

	static  void  OpenRptFile ( HANDLE *phFile, LPCTSTR pcszFile, DWORD dwFileOpts )
	{
		if ( *phFile )
			CloseHandle (*phFile);
		*phFile = CreateFile (pcszFile, GENERIC_WRITE, 
					  FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE, NULL, OPEN_ALWAYS, 
					  (dwFileOpts & TraceWriteThrough ? FILE_FLAG_WRITE_THROUGH : FILE_ATTRIBUTE_NORMAL), NULL);
		if ( *phFile == INVALID_HANDLE_VALUE )
			*phFile = NULL;
		else if ( dwFileOpts & TraceNewLog )
			SetEndOfFile (*phFile);
		else
			SetFilePointer (*phFile, NULL, NULL, FILE_END);
	}

	void  InternalDump ( int nLevel, LPTSTR pszMsg, PVOID pvData, DWORD dwDataSize, DWORD dwParams, LPCTSTR pcszFmt, va_list argptr )
	{
		if ( pcszFmt )
		{
			TCHAR		*pszFmt, pszBuf[1024];
			if ( m_dwTraceFlags & Pf )  {
				pszFmt = pszBuf;
				PrepareFmtString (nLevel, pszFmt, pcszFmt);
			}
			else
				pszFmt = (LPTSTR)pcszFmt;
			_vstprintf (pszMsg, pszFmt, argptr);
			RemoveTrailingNewline (pszMsg);
			OutputMsg (nLevel, pszMsg);
		}
		pszMsg[0] = 0;
		DumpData (pszMsg, pvData, dwDataSize, dwParams, 0);
		OutputMsg (nLevel, pszMsg);
	}

	void  InternalDumpTable ( int nLevel, LPTSTR pszMsg, PVOID pvData, DWORD dwDataSize, DWORD nCols, 
					  DWORD dwParams, LPCTSTR pcszFmt, va_list argptr )
	{
		DWORD	nElemSize, nElems, nRows, nRowSize;
		if ( pcszFmt )
		{
			TCHAR		pszFmt[1024];
			PrepareFmtString (nLevel, pszFmt, pcszFmt);
			_vstprintf (pszMsg, pszFmt, argptr);
			AddTrailingNewline (pszMsg);
			OutputMsg (nLevel, pszMsg);
		}
		switch ( dwParams & DumpElemSizeMask )  {
		case  DumpBytes:
			nElemSize = sizeof(BYTE);
			break;
		case  DumpWords:
			nElemSize = sizeof(WORD);
			break;
		case  DumpDwords:
			nElemSize = sizeof(DWORD);
			break;
		case  DumpQwords:
			nElemSize = sizeof(UINT64);
			break;
		default:
			break;
		}
		nElems = dwDataSize / nElemSize;
		nRows = (nElems + nCols - 1) / nCols;
		nRowSize = nCols * nElemSize;
		for ( int i = 0; i < nRows; i++ )
		{
			DWORD	dwCurRowDataSize = min(nRowSize, dwDataSize);
			pszMsg[0] = 0;
			DumpData (pszMsg, pvData, dwCurRowDataSize, dwParams, (nRowSize - dwCurRowDataSize) / nElemSize);
			OutputMsg (nLevel, pszMsg, false);
			pvData = (PBYTE)pvData + nRowSize;
			dwDataSize -= nRowSize;
		}
	}

	void  InternalTrace ( int nLevel, LPTSTR pszMsg, DWORD dwFlags, LPCTSTR pcszFmt, va_list argptr )
	{
		if ( dwFlags & Prefix )
		{
			TCHAR	pszFmt[MAX_FORMAT_LEN];
			PrepareFmtString (nLevel, pszFmt, pcszFmt, dwFlags & (TraceFnMask | ExtraNestingMask));
			_vstprintf (pszMsg, pszFmt, argptr);
		}
		else
			_vstprintf (pszMsg, pcszFmt, argptr);
		if ( dwFlags & TrailingLineFeed )
			AddTrailingNewline (pszMsg);
		OutputMsg (nLevel, pszMsg, (dwFlags & RollbackInfo ? true : false));
	}

	void  InternalTrace ( int nLevel, DWORD dwFlags, LPCTSTR pcszFmt, va_list argptr )
	{
		TCHAR	pszMsg[MAX_TRACE_MSG_LEN];
		InternalTrace (nLevel, pszMsg, dwFlags, pcszFmt, argptr);
	}

	void  InternalTraceStr ( int nLevel, LPCTSTR pcszMsg, DWORD dwFlags )
	{
		if ( dwFlags & Prefix )
		{
			TCHAR	pszFmt[MAX_FORMAT_LEN];
			PrepareFmtString (nLevel, pszFmt, pcszMsg, dwFlags & TraceFnMask);
			OutputMsg (nLevel, pszFmt, false);
		}
		else
			OutputMsg (nLevel, pcszMsg, false);
	}

	void  AddPathFromEnvVar ( std::string& strPath, LPSTR pszEnvVarName )
	{
		DWORD	n;
		CHAR	pszPath[MAX_PATH];
		if ( (n = GetEnvironmentVariableA(pszEnvVarName, pszPath, MAX_PATH)) != 0 )
		{
			#if !defined(_DEBUG)
				std::string		strEnvVar = pszPath;
				if ( strEnvVar.find("symsrv*") != NPOS )
					return;
			#endif  // _DEBUG
			strPath += ";";
			strPath += pszPath;
		}
	}

	static  BOOL  CALLBACK  EnumLoadedModulesProc ( PSTR pszModuleName, ULONG dwModuleBase, 
													ULONG dwModuleSize, PVOID pUserContext )
	{
		DWORD	n;
		CHAR	pszPath[MAX_PATH];
		PATHS_SET	*pPathsSet = (PATHS_SET*)pUserContext;
		if ( !(n = GetModuleFileNameA((HMODULE)dwModuleBase, pszPath, MAX_PATH)) )
			return  TRUE;
		std::string strPath = ExtractPath (pszPath, n);
		if ( pPathsSet->find(strPath) == pPathsSet->end() )
			pPathsSet->insert(strPath);
		return  TRUE;
	}

	void  SetExtraNesting ()
	{
		CHAR			pszPath[MAX_PATH];
		std::string		strSearchPath;
		AddPathFromEnvVar (strSearchPath, "_NT_SYMBOL_PATH");

		long	nSymsrvStart = strSearchPath.find("symsrv*");
		if ( nSymsrvStart != NPOS )
		{
			nSymsrvStart += 7;
			long	nSymsrvEnd = strSearchPath.find("*", nSymsrvStart);
			if ( nSymsrvEnd != NPOS  &&  nSymsrvEnd != nSymsrvStart + 1 )
			{
				std::string& strSymSrv = strSearchPath.substr(nSymsrvStart, nSymsrvEnd - nSymsrvStart);
				if ( strSymSrv.find(':') != NPOS )
					// Absolute path
					lstrcpyA (pszPath, strSymSrv.c_str());
				else
					// Relative path
					lstrcatA (pszPath, strSymSrv.c_str() + 
							 (strSymSrv[0] == '\\' || strSymSrv[0] == '/' ? 1 : 0));
				if ( GetFileAttributesA(pszPath) != 0xFFFFFFFF )  {
					// Symbol server is available and will be used for this executable
					#if defined(_DEBUG)
						*m_pnExtraNesting = 3;
						*m_pnPrepareFmtStringExtraNesting = 3;
					#else
						*m_pnExtraNesting = 2;
						*m_pnPrepareFmtStringExtraNesting = 2;
					#endif
					return;
				}
			}
		}
		// No symbol server will be used for this executable

		#if defined(_DEBUG)
			*m_pnExtraNesting = 2;
			*m_pnPrepareFmtStringExtraNesting = 3;
		#else
			*m_pnExtraNesting = 1;
			*m_pnPrepareFmtStringExtraNesting = 2;
		#endif

	}

	void  InitializeModule ()
	{
		DWORD	n = 0;
		CHAR	pszPath[MAX_PATH];
		HMODULE		hDll = LoadLibrary (_T("psapi.dll"));
		if ( FALSE  &&  hDll )  {
			// The GetMappedFileName function does not work.
			ENUMPROCESSMODULES		fnEnumProcessModules = (ENUMPROCESSMODULES)GetProcAddress(hDll, "EnumProcessModules");
			GETMODULEINFORMATION	fnGetModuleInformation = (GETMODULEINFORMATION)GetProcAddress(hDll, "GetModuleInformation");
			if ( fnEnumProcessModules  &&  fnGetModuleInformation )
			{
				HANDLE		hProcess = GetCurrentProcess();
				HMODULE		ahm[256];
				DWORD		dwStored;
				MODULEINFO	mi;
				n = fnEnumProcessModules (hProcess, ahm, 256 * sizeof(HMODULE), &dwStored);
				dwStored /= sizeof(HMODULE);
				_ASSERTE (dwStored <= 256);
				for ( int i = 0; i < dwStored; i++ )  {
					if ( !fnGetModuleInformation(hProcess, ahm[i], &mi, sizeof(MODULEINFO)) )
						continue;
					if ( (PBYTE)mi.lpBaseOfDll > (PBYTE)this  ||  (PBYTE)mi.lpBaseOfDll + mi.SizeOfImage < (PBYTE)this )
						continue;
					n = GetModuleFileNameA((HMODULE)mi.lpBaseOfDll, pszPath, MAX_PATH);
					if ( n )  {
						std::string		strName = ExtractName(pszPath, n);
						SymLoadModule (GetCurrentProcess(), NULL, pszPath, (PSTR)strName.c_str(), 
										(DWORD)mi.lpBaseOfDll, mi.SizeOfImage);
					}
					break;
				}
			}
			FreeLibrary (hDll);
		}
		else  {
			CToolHelp	th;
			if ( th.IsAvailable()  &&  th.CreateSnapshot(CToolHelp::Module) )
			{
				BOOL	bNext = th.Module32First();
				while ( bNext )  { 
					if ( th.GetModuleBaseAddr() > (PBYTE)this  ||  
						 th.GetModuleBaseAddr() + th.GetModuleSize() < (PBYTE)this )
					{
						bNext = th.Module32Next();
						continue;
					}
					n = GetModuleFileNameA((HMODULE)th.GetModuleBaseAddr(), pszPath, MAX_PATH);
					if ( n )  {
						std::string		strName = ExtractName(pszPath, n);
						SymLoadModule (GetCurrentProcess(), NULL, pszPath, (PSTR)strName.c_str(), 
									   (DWORD)th.GetModuleBaseAddr(), th.GetModuleSize());
					}
					break;
				} 
			} 
		}
	}

	bool  GetSymSearchPath ( std::string& strSearchPath )
	{
		DWORD	n;
		CHAR	pszPath[MAX_PATH];
		TCHAR	pszMutexName[32];
		if ( !(n = GetModuleFileNameA(GetModuleHandle(NULL), pszPath, MAX_PATH)) )
		{
			strSearchPath = ".";
			_ASSERT (!"Unable to determine executable's path name");
		}
		else
			strSearchPath = ExtractPath (pszPath, n);

		wsprintf (pszMutexName, _T("z__dbg_util__mutex_%05u"), GetCurrentProcessId());
		m_hMutex = CreateMutex (NULL, FALSE, pszMutexName);
		if ( GetLastError() == ERROR_ALREADY_EXISTS )
		{
			// We are in the DLL module
			CloseHandle (m_hMutex);
			m_hMutex = NULL;
			InitializeModule();
			return  false;
		}

		AddPathFromEnvVar (strSearchPath, "_NT_SYMBOL_PATH");
		AddPathFromEnvVar (strSearchPath, "_NT_ALTERNATE_SYMBOL_PATH");
		AddPathFromEnvVar (strSearchPath, "SYSTEMROOT");
		// "symsrv*symsrv.dll*F:\\SymStore*http://msdl.microsoft.com/download/symbols"
		// "symsrv*F:\SymStore\symsrv.dll*F:\SymStore*http://msdl.microsoft.com/download/symbols"

		PATHS_SET	setPaths;
		EnumerateLoadedModules (GetCurrentProcess(), EnumLoadedModulesProc, &setPaths);
		PATHS_SET_IT	itPath = setPaths.begin();
		for ( ; itPath != setPaths.end(); itPath++ )  {
			strSearchPath += ";";
			strSearchPath += *itPath;
		}

		#if defined(SYM_SEARCH_PATH)
			LPCSTR		pszUserPath = SYM_SEARCH_PATH;
			if ( pszUserPath != NULL  &&  pszUserPath[0] != 0 )  {
				strSearchPath += ";";
				strSearchPath += pszUserPath,;
			}
		#endif

		return  true;
	}

	void  InitSymHandler ()
	{
		std::string		strSearchPath = ".";
		SymSetOptions (SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES);
		// If the GetSymSearchPath returns false, we are in the DLL and no more initialization required
		if ( !GetSymSearchPath (strSearchPath)  ||
			 SymInitialize(GetCurrentProcess(), (LPSTR)strSearchPath.c_str(), TRUE) )
		{
			SetExtraNesting ();
		}
		else  {
			*m_pnExtraNesting = NO_SYM_HANDLER;
			*m_pnPrepareFmtStringExtraNesting = NO_SYM_HANDLER;
		}
	}

	bool  GetModuleNameFromAddr ( DWORD dwAddr, std::string& strModuleName )
	{
		IMAGEHLP_MODULE   ihm = { sizeof(IMAGEHLP_MODULE) };
		if ( SymGetModuleInfo(GetCurrentProcess(), dwAddr, &ihm) )  {
			strModuleName = ihm.ModuleName;
			return  true;
		}
		strModuleName = "?";
		return  false;
	}

	void  Replace ( std::string& str, const std::string& strOld, const std::string& strNew )
	{
		long	_DLen = strNew.length() - strOld.length();
		long	_Pos = 0;
		while ( (_Pos = str.find(strOld, _Pos)) != NPOS )  {
			if ( _DLen < 0 )
				str.erase (_Pos, -_DLen);
			else if ( _DLen > 0 )
				str.insert (_Pos, _DLen, ' ');
			str.replace(_Pos, strNew.length(), strNew);
			_Pos += strNew.length();
		}
	}

	bool  GetTemplArgListPos ( std::string& strSymName, DWORD dwPrevPos, DWORD& dwCurPos, 
							   DWORD& dwTmplArgListStartPos, DWORD& dwTmplArgListEndPos )
	{
		if ( strSymName[dwCurPos] != '<' )  {
			dwTmplArgListStartPos = dwTmplArgListEndPos = 0;
			return  false;
		}
		dwTmplArgListStartPos = dwCurPos - dwPrevPos;
		int		nNesting = 1;
		while ( nNesting > 0  &&  
			(dwCurPos = strSymName.find_first_of("<>", ++dwCurPos)) != NPOS  )
		{
			if ( strSymName[dwCurPos] == '<' )
				nNesting++;
			else
				nNesting--;
		}
		_ASSERTE (nNesting == 0  &&  strSymName[dwCurPos] == '>');
		dwTmplArgListEndPos = dwCurPos - dwPrevPos;
		return  true;
	}

	//     GetPtrLevel:
	//     Returns the pointer (and references) nesting (indirection) level: 0 for non 
	// pointers, 1 for pointers, 2 for pointers to pointers, and so on.
	int  GetPtrLevel ( const std::string& strType, DWORD dwTmplArgListEndPos, bool* pbRef = NULL )
	{
		int		nLevel = 0;
		DWORD	dwCurPos = dwTmplArgListEndPos - 1;
		if ( pbRef )
			*pbRef = false;
		while ( (dwCurPos = strType.find_first_of ("&*", ++dwCurPos)) != NPOS )  {
			if ( nLevel == 0 )  {
				if ( pbRef )  {
					if ( strType[dwCurPos] == '&' )
						*pbRef = true;
					else
						*pbRef = false;
				}
			}
			nLevel++;
		}
		return  nLevel;
	}

	int  TraceStringPtr ( DWORD dwInfoFlags, char* pszBuf, DWORD* pdwArg, bool bWide )
	{
		if ( !*pdwArg )  {
			lstrcpyA (pszBuf, "NULL");
			return  4;
		}
		char	pszString[MAX_STR_ARG_LEN];
		if ( bWide )  {
			wchar_t		pwszString[MAX_STR_ARG_LEN];
			lstrcpynW (pwszString, (LPWSTR)*pdwArg, MAX_STR_ARG_LEN);
			WideCharToMultiByte (CP_ACP, 0, pwszString, -1, pszString, MAX_STR_ARG_LEN, NULL, NULL);
		}
		else
			lstrcpynA (pszString, (LPSTR)*pdwArg, MAX_STR_ARG_LEN);
		if ( lstrlenA (pszString) == MAX_STR_ARG_LEN - 1 )  {
			pszString[MAX_STR_ARG_LEN - 2] = '.';
			pszString[MAX_STR_ARG_LEN - 3] = '.';
			pszString[MAX_STR_ARG_LEN - 4] = '.';
		}
		int		nLen;
		if ( dwInfoFlags & TraceFnStripStringArgs )
			nLen = wsprintfA (pszBuf, "\"%s\"", pszString);
		else
			nLen = wsprintfA (pszBuf, "{0x%08X \"%s\"}", *pdwArg, pszString);
		return  nLen;
	}

	//     TraceStringObjArg:
	//     The dwObjSize is the size of the string object represented by the pdwArg argument
	// in DWORDs. The dwOffset is the offset in DWORDs from the beginning of the string object
	// to the character string pointer it contains.
	void  TraceStringObjArg ( const std::string& strArgType, DWORD dwInfoFlags, DWORD dwTmplArgListEndPos,
							  char* pszBuf, DWORD*& pdwArg, bool bWide, DWORD dwObjSize, DWORD dwOffset, int nPtrLevel = -1 )
	{
		nPtrLevel = nPtrLevel == -1 ? GetPtrLevel(strArgType, dwTmplArgListEndPos) : nPtrLevel;
		switch ( nPtrLevel )  {
		case 0:
			TraceStringPtr (dwInfoFlags, pszBuf, pdwArg + dwOffset, bWide);
			break;
		case 1:
			{
				//TraceFnStripStringArgs
				char	pszString[32 + MAX_STR_ARG_LEN];
				if ( *pdwArg )
					TraceStringPtr (dwInfoFlags, pszString, PDWORD(*pdwArg) + dwOffset, bWide);
				else
					lstrcpyA (pszString, "NULL");
				if ( dwInfoFlags & TraceFnStripStringArgs )
					wsprintfA (pszBuf, "%s", pszString);
				else
					wsprintfA (pszBuf, "0x%08X %s", *pdwArg, pszString);
				break;
			}
		default:
			if ( *pdwArg )
				wsprintfA (pszBuf, "0x%08X", *pdwArg);
			else
				lstrcpyA (pszBuf, "NULL");
		}	// <- switch ( nPtrLevel )
		if ( dwObjSize != NPOS )
			pdwArg += nPtrLevel ? 1 : dwObjSize;
	}

	bool  IsStringChar ( char c )
	{
		return  isprint(c)  ||  c == '\r'  ||  c == '\n'  ||  c == '\t' ;
	}
	bool  IsStringChar ( wchar_t c )
	{
		return  iswprint(c)  ||  c == L'\r'  ||  c == L'\n'  ||  c == L'\t' ;
	}

	template<class _E>
	bool  IsValidString ( const _E* pcsz )
	{
		bool	bString = true;
		if ( !pcsz )
			return  true;
		int		n = 0;
		try  {
			while ( *pcsz  &&  ++n < MAX_STR_ARG_LEN )  {
				if ( !IsStringChar(*pcsz++) )  {
					bString = false;
					break;
				}
			}
		}
		catch ( ... )  {
			bString = false;
		}
		return  bString;
	}

	#define  IsType(StrType, Type)		\
		(nTypeLen == sizeof(Type) - 1  &&  StrType.substr(0, nTypeLen) == Type)

	std::string  GetArgValue ( std::string& strArgType, DWORD dwInfoFlags, DWORD*& pdwArg, 
							   DWORD dwTmplArgListStartPos, DWORD& dwTmplArgListEndPos, bool& bUnknown )
	{
		char	pszBuf[64 + MAX_STR_ARG_LEN];
		int		nTypeLen = strArgType.find_first_of(" ,&*<)");
		if ( nTypeLen == NPOS )
			nTypeLen = strArgType.length();
		bUnknown = false;
		if ( dwTmplArgListStartPos )  {
			// Template class
			if ( IsType(strArgType, "basic_string")  ||
				 IsType(strArgType, "BasicString") )
			{
				// STL strings
				bool	bWide = strArgType.substr(dwTmplArgListStartPos + 1, 4) == "WORD";
				if ( bWide )  {
					Replace (strArgType, "WORD", "wchar_t");
					dwTmplArgListEndPos += 9;
				}
				else if ( strArgType.find("BYTE") != NPOS )  {
					Replace (strArgType, "BYTE", "u_char");
					dwTmplArgListEndPos += 6;
				}
				try  {
					TraceStringObjArg (strArgType, dwInfoFlags, dwTmplArgListEndPos, pszBuf, pdwArg, bWide, 4, 1);
				}
				catch ( ... ) {
					wsprintfA (pszBuf, "0x%08X {??}", *pdwArg);
				}
				return  pszBuf;
			}
		}
		else  {
			_ASSERTE (dwTmplArgListEndPos == 0);
			bool	bRef;
			int		nPtrLevel = GetPtrLevel(strArgType, 0, &bRef);
			if ( IsType(strArgType, "CString") )
			{
				#if defined(_UNICODE) || defined(UNICODE)
					bool	bWide = true;
				#else
					bool	bWide = false;
				#endif  // !_UNICODE
				try  {
					TraceStringObjArg (strArgType, dwInfoFlags, dwTmplArgListEndPos, pszBuf, 
									   pdwArg, bWide, 1, 0, nPtrLevel);
				}
				catch ( ... ) {
					wsprintfA (pszBuf, "0x%08X {??}", *pdwArg);
				}
				return  pszBuf;
			}
			else if ( IsType(strArgType, "_bstr_t") )
			{
				try  {
					TraceStringObjArg (strArgType, dwInfoFlags, dwTmplArgListEndPos, pszBuf, 
									   *(PDWORD*)pdwArg, true, NPOS, 0, nPtrLevel);
				}
				catch ( ... ) {
					wsprintfA (pszBuf, "0x%08X {??}", *pdwArg);
				}
				pdwArg++;
				return  pszBuf;
			}
			else if ( nPtrLevel == 1  &&  IsType(strArgType, "char")  &&  IsValidString(*(LPCSTR*)pdwArg) )
			{
				TraceStringObjArg (strArgType, dwInfoFlags, dwTmplArgListEndPos, pszBuf, 
								   pdwArg, false, 1, 0, 0);
				return  pszBuf;
			}
			else if ( nPtrLevel == 1  &&  !(dwInfoFlags & TraceFnNoRawWideStrings)  &&  
					  IsType(strArgType, "WORD")  &&  IsValidString(*(LPCWSTR*)pdwArg) )
			{
				Replace (strArgType, "WORD", "wchar_t");
				TraceStringObjArg (strArgType, dwInfoFlags, dwTmplArgListEndPos, pszBuf, 
								   pdwArg, true, 1, 0, 0);
				return  pszBuf;
			}
			else if ( nPtrLevel == 0  ||  bRef  &&  nPtrLevel == 1 )
			{
				// Not a pointer
				PCSTR	pcszFmt = NULL;
				bool	bForceDec = dwInfoFlags & TraceFnDecIntArgs  &&  !(dwInfoFlags & TraceFnHexIntArgs);
				bool	bForceHex = dwInfoFlags & TraceFnHexIntArgs  &&  !(dwInfoFlags & TraceFnDecIntArgs);
				if ( IsType(strArgType, "char") )
					pcszFmt = "'%c'";
				else if ( IsType(strArgType, "short") )
					pcszFmt = bForceHex ? "0x%X" : "%hd";
				else if ( IsType(strArgType, "int") )
					pcszFmt = bForceHex ? "0x%X" : "%d";
				else if ( IsType(strArgType, "long") )
					pcszFmt = bForceHex ? "0x%X" : "%ld";
				else if ( IsType(strArgType, "DWORD") )
					pcszFmt = bForceDec ? "%ld" : "0x%X";
				if ( pcszFmt )  {
					sprintf (pszBuf, pcszFmt, (bRef ? **(PDWORD*)pdwArg++ : *pdwArg++));
					return  pszBuf;
				}
				if ( IsType(strArgType, "__int64") )
					pcszFmt = bForceHex ? "0x%I64X" : "%I64d";
				else if ( IsType(strArgType, "DWORD64") )
					pcszFmt = bForceDec ? "%I64d" : "0x%I64X";
				if ( pcszFmt )  {
					if ( bRef )
						sprintf (pszBuf, pcszFmt, **(PDWORD64*)pdwArg++);
					else  {
						sprintf (pszBuf, pcszFmt, *(PDWORD64)pdwArg);
						pdwArg += 2;
					}
					return  pszBuf;
				}
				double	d;
				if ( IsType(strArgType, "float") )  {
					pcszFmt = "%g";
					d = bRef ? **(float**)pdwArg++ : *(float*)pdwArg++;
				}
				else if ( IsType(strArgType, "double") )  {
					pcszFmt = "%g";
					if ( bRef )
						d = **(double**)pdwArg++;
					else  {
						d = *(double*)pdwArg;
						pdwArg += 2;
					}
				}
				if ( pcszFmt )  {
					sprintf (pszBuf, pcszFmt, d);
					return  pszBuf;
				}
				if ( IsType(strArgType, "BYTE") )
					pcszFmt = bForceDec ? "%hd('%hc')" : "0x%X('%hc')";
				else if ( IsType(strArgType, "WORD") )
					pcszFmt = bForceDec ? "%hd('%lC')" : "0x%X('%lC')";
				if ( pcszFmt )  {
					WORD	wVal = (WORD)(bRef ? **(PDWORD*)pdwArg++ : *pdwArg++);
					sprintf (pszBuf, pcszFmt, wVal, wVal);
					return  pszBuf;
				}
				// Unknown class or structure
				if ( !bRef )  {
					lstrcpyA (pszBuf, GetRawDwordList(pdwArg, 3, "{", ",...}").c_str());
					bUnknown = true;
				}
				else  {
					pcszFmt = "0x%08X {0x%08X, ...}";
					sprintf (pszBuf, pcszFmt, *pdwArg, **(PDWORD*)pdwArg);
					pdwArg++;
				}
				return  pszBuf;
			}
		}
		wsprintfA (pszBuf, "0x%08X", *(pdwArg++));
		return  pszBuf;
	}

	#define  ProcessPrefix(str, prefix, flag)		\
				(str.find(prefix) == 0 ? str.erase(0, sizeof(prefix) - 1), flag = true : false )

	bool  GetSymFromAddr ( DWORD dwAddr, DWORD dwStackAddr, DWORD dwInfoFlags, std::string& strSymbol )
	{
		DWORD		dwSymInfoSize = sizeof(IMAGEHLP_SYMBOL) + MAX_SYM_NAME;
		PIMAGEHLP_SYMBOL  pihSym = (PIMAGEHLP_SYMBOL) _alloca (dwSymInfoSize);
		DWORD		dwDisplacement = 0;
		// Skip stack frame pointer and return address
		PDWORD		pdwCurParam = PDWORD(dwStackAddr + 2 * sizeof(DWORD));
		std::string	strSymName;
		DWORD		dwCurPos = 0, dwPrevPos = 0;
		CBooster	boost(false);

		::ZeroMemory (pihSym, dwSymInfoSize);
		pihSym->SizeOfStruct = dwSymInfoSize;
		pihSym->MaxNameLength = MAX_SYM_NAME;

		_ASSERTE (dwInfoFlags & (TraceFnName | TraceFnArgTypes | TraceFnArgs));

		DWORD	dwUndFlags = //UNDNAME_NO_MS_KEYWORDS |
							UNDNAME_NO_FUNCTION_RETURNS |
							UNDNAME_NO_ALLOCATION_MODEL | 
							//UNDNAME_NO_ALLOCATION_LANGUAGE |
							//UNDNAME_NO_MS_THISTYPE | UNDNAME_NO_CV_THISTYPE | 
							//UNDNAME_NO_ACCESS_SPECIFIERS |
							UNDNAME_NO_THROW_SIGNATURES | 
							UNDNAME_NO_RETURN_UDT_MODEL |
							UNDNAME_NO_SPECIAL_SYMS;
		BOOL	bSymInfoAvailable = SymGetSymFromAddr(GetCurrentProcess(), dwAddr, &dwDisplacement, pihSym);
		if ( !bSymInfoAvailable  ||  pihSym->Name[0] == 0 )
		{
			wsprintfA (pihSym->Name, "0x%08X", dwAddr);
			strSymbol = pihSym->Name;
			if ( dwInfoFlags & TraceFnArgs )
				strSymbol += GetRawDwordList(pdwCurParam, BLIND_ARG_LIST_DEPTH, "(", ",...)");
			else
				strSymbol += "()";
			goto  end;
		}
		if ( dwInfoFlags & TraceFnDontUndecorate )
		{
			strSymbol = pihSym->Name;
			goto  end;
		}
		DWORD	dwRes;
		char	pszBuf[MAX_SYM_NAME];
		pszBuf[0] = 0;
		if ( !(dwInfoFlags & (TraceFnArgTypes | TraceFnArgs)) )
		{
			DWORD		dwCurPos = 0, dwTmplArgListStartPos = 0, dwTmplArgListEndPos = 0;
			boost.Boost();
			dwRes = UnDecorateSymbolName (pihSym->Name, pszBuf, MAX_SYM_NAME, UNDNAME_NAME_ONLY);
			if ( !dwRes )  {
				Sleep(0);
				dwRes = UnDecorateSymbolName (pihSym->Name, pszBuf, MAX_SYM_NAME, UNDNAME_NAME_ONLY);
			}
			boost.Restore();
			_ASSERTE (dwRes && pszBuf[0]);
			strSymbol = pszBuf;
			if ( (dwInfoFlags & TraceFnStripTemplArgs  ||  
				  !(dwInfoFlags & (TraceFnArgTypes | TraceFnArgs)))  &&
				  (dwCurPos = strSymbol.find('<')) != NPOS  &&
				  GetTemplArgListPos (strSymbol, 0, dwCurPos, dwTmplArgListStartPos, dwTmplArgListEndPos) )
			{
				strSymbol.erase (dwTmplArgListStartPos + 1, dwTmplArgListEndPos - dwTmplArgListStartPos - 1);
			}
			goto  end;
		}
		boost.Boost();
		dwRes = UnDecorateSymbolName (pihSym->Name, pszBuf, MAX_SYM_NAME, dwUndFlags);
		if ( !dwRes )  {
			Sleep(0);
			dwRes = UnDecorateSymbolName (pihSym->Name, pszBuf, MAX_SYM_NAME, dwUndFlags);
		}
		boost.Restore();
		strSymName = pszBuf;
		dwCurPos = strSymName.find('(');
		if ( dwCurPos != NPOS )
		{
			// The symbol is a function with arguments type info
			bool	bMemberFn = false,
					bVirtual = false,
					bThisCall = false,
					bStdCall = false,
					bCDecl = false,
					bFastCall = false;
			int		nArgsToSkip = 0;	// used for __fastcall functions
			// Check whether it is a member function
			dwPrevPos = strSymName.find(": ");
			if ( dwPrevPos != NPOS )  {
				// This is a member function
				_ASSERTE (dwPrevPos < dwCurPos);
				if ( !ProcessPrefix(strSymName, "public: ", bMemberFn) )
					if ( !ProcessPrefix(strSymName, "protected: ", bMemberFn) )
						ProcessPrefix(strSymName, "private: ", bMemberFn);
				_ASSERTE (bMemberFn);
				ProcessPrefix(strSymName, "virtual ", bVirtual);
			}
			if ( !ProcessPrefix(strSymName, "__thiscall ", bThisCall) )
				if ( !ProcessPrefix(strSymName, "__stdcall ", bStdCall) )
					if ( !ProcessPrefix(strSymName, "__cdecl ", bCDecl) )
						ProcessPrefix(strSymName, "__fastcall ", bFastCall);
			// Get new open paranthesis location (previous operations has changed it)
			dwCurPos = strSymName.find('(');
			if ( bMemberFn  &&  (bStdCall  ||  bCDecl) )  {
				// This member function has its 'this' pointer passed through the stack
				strSymName.insert (dwCurPos + 1, "this*,");
			}
			if ( bFastCall )
				nArgsToSkip = bMemberFn ? 1 : 2;
			dwPrevPos = 0;
			DWORD		dwTmplArgListStartPos = 0, dwTmplArgListEndPos = 0;
			if ( dwInfoFlags & TraceFnStripTemplArgs  ||  
				 !(dwInfoFlags & (TraceFnArgTypes | TraceFnArgs)) )
			{
				// Strip template arguments from template
				dwPrevPos = strSymName.find('<');
				if ( dwPrevPos != NPOS  &&  dwPrevPos < dwCurPos )
				{
					_ASSERTE (bMemberFn);
					GetTemplArgListPos (strSymName, 0, dwPrevPos, dwTmplArgListStartPos, dwTmplArgListEndPos);
					strSymName.erase (dwTmplArgListStartPos + 1, dwTmplArgListEndPos - dwTmplArgListStartPos - 1);
				}
				dwPrevPos = 0;
			}
			Replace (strSymName, " &", "&");
			Replace (strSymName, " *", "*");
			Replace (strSymName, "class ", "");
			Replace (strSymName, "struct ", "");
			Replace (strSymName, "std::", "");
			Replace (strSymName, "unsigned char", "BYTE");
			Replace (strSymName, "unsigned short", "WORD");
			Replace (strSymName, "unsigned long", "DWORD");
			Replace (strSymName, "unsigned __int64", "DWORD64");
			dwCurPos = strSymName.find('(');
			if ( dwInfoFlags & (TraceFnArgTypes | TraceFnArgs)  &&
				 strSymName.find("(void)") == NPOS  &&  strSymName.find("()") == NPOS )
			{
				bool		bUnknown = false;
				std::string		strArgType, strArgValue;
				// Store function name and opening paranthesis
				strSymbol = strSymName.substr(dwPrevPos, dwCurPos - dwPrevPos + 1);
				dwPrevPos = ++dwCurPos;
				while ( (dwCurPos = strSymName.find_first_of("<,)", dwCurPos)) != NPOS )
				{
					if ( GetTemplArgListPos (strSymName, dwPrevPos, dwCurPos, dwTmplArgListStartPos, dwTmplArgListEndPos) )
					{
						// Template args specification may be followed by a combination
						// of the 'const', '*' or '&' modifiers. Skip them.
						dwCurPos = strSymName.find_first_of(",)", ++dwCurPos);
						_ASSERTE (dwCurPos != NPOS);
					}
					else if ( strSymName[dwCurPos] == ')'  &&  
							  strSymName.find("...", dwPrevPos) != NPOS )
					{
						break;
					}
					
					strArgType = strSymName.substr(dwPrevPos, dwCurPos - dwPrevPos);
					if ( dwInfoFlags & TraceFnArgs )  {
						if ( !bUnknown  &&  --nArgsToSkip < 0 )  {
							strArgValue = "=";
							strArgValue += GetArgValue (strArgType, dwInfoFlags, pdwCurParam, 
														dwTmplArgListStartPos, dwTmplArgListEndPos, bUnknown);
						}
						else
							strArgValue = "=??";
					}
					if ( dwInfoFlags & TraceFnStripTemplArgs  &&  dwTmplArgListStartPos )
					{
						strArgType.erase (dwTmplArgListStartPos + 1, 
										  dwTmplArgListEndPos - dwTmplArgListStartPos - 1);
					}
					strSymbol += strArgType;
					strSymbol += strArgValue;
					strSymbol += strSymName[dwCurPos];
					dwPrevPos = ++dwCurPos;
				}
				strSymbol += strSymName.substr(dwPrevPos);
			}
			else  {
				if ( dwInfoFlags & (TraceFnArgTypes | TraceFnArgs) )
					strSymbol = strSymName.substr(dwPrevPos);
				else
					// Store function name only
					strSymbol = strSymName.substr(dwPrevPos, dwCurPos - dwPrevPos);
			}
		}
		else  {
			int		nBytes;
			if ( strSymName[0] == '_' )  {
				if ( (dwCurPos = strSymName.find("@")) != NPOS  &&
					 sscanf (strSymName.c_str() + dwCurPos + 1, "%u", &nBytes) == 1 )
				{
					strSymName += GetRawDwordList(pdwCurParam, nBytes/sizeof(DWORD));
				}
			}
			else
				strSymName += GetRawDwordList(pdwCurParam, BLIND_ARG_LIST_DEPTH);
			strSymbol = strSymName;
		}
		if ( strSymbol[strSymbol.length() - 1] == ' ' )
			strSymbol.resize(strSymbol.length() - 1);

		end:
		bool	bRes = bSymInfoAvailable && pihSym->Name[0];
		if ( bSymInfoAvailable  &&  dwInfoFlags & TraceFnCallDisp )
		{
			CHAR	pszDisp[32];
			if ( dwAddr >= pihSym->Address )
			{
				wsprintfA (pszDisp, (dwInfoFlags & TraceFnDispHex ? " + 0x%X bytes" : " + %d bytes"), 
						   dwAddr - pihSym->Address);
			}
			else  {
				//     When Microsoft's implementation of the symbol server is used (symsrv.dll),
				// the SymGetSymFromAddr API used above reports symbol's base address incorrectly.
				// Here we are trying to amend it (usually successfully).
				IMAGEHLP_MODULE		ihm = { sizeof(IMAGEHLP_MODULE) };
				if ( SymGetModuleInfo(GetCurrentProcess(), dwAddr, &ihm)  &&  ihm.ModuleName[0] )
				{
					dwCurPos = strSymbol.find('(');
					if ( dwCurPos )
					{
						//     Prepending module name to the symbol name prevents symbols for
						// other modules from unnecessary loading (otherwise we would have
						// defeated deferred loading option).
						std::string		strFn = ihm.ModuleName;
						strFn += '!';
						strFn += strSymbol.substr(0, dwCurPos);
						IMAGEHLP_SYMBOL		ihs = { sizeof(IMAGEHLP_SYMBOL) };
						if ( SymGetSymFromName(GetCurrentProcess(), (PSTR)strFn.c_str(), &ihs)  &&
							 dwAddr >= ihs.Address )
						{
							wsprintfA (pszDisp, (dwInfoFlags & TraceFnDispHex ? 
												" + 0x%X bytes" : " + %d bytes"), dwAddr - ihs.Address);
						}
					}
				}
			}
			strSymbol += pszDisp;
		}
		return  bRes;
	}

	std::string  GetRawDwordList ( DWORD*& pdwCurPos, DWORD nDwords, PSTR pszPrefix = "(", PSTR pszPostfix = ")" )
	{
		CHAR	pszArgs[64 * 11 + 16];	// max - 64 arguments
		int		nLen = 0, nLenSrc;
		nLen += wsprintfA (pszArgs, "%s", pszPrefix);
		nLenSrc = nLen;
		ProtectedGetRawDwordList (pszArgs, nLen, pdwCurPos, nDwords, pszPrefix, pszPostfix);
		if ( !nDwords  ||  nLen == nLenSrc )
			nLen += 1;
		lstrcpyA (pszArgs + nLen - 1, pszPostfix);
		return  pszArgs;
	}

	void  ProtectedGetRawDwordList ( LPSTR pszArgs, int& nLen, DWORD*& pdwCurPos, DWORD nDwords, PSTR pszPrefix, PSTR pszPostfix )
	{
		__try  {
		//try  {
		for ( int i = 0; i < nDwords; i++ )
			nLen += wsprintfA (pszArgs + nLen, "0x%08X,", *pdwCurPos++);
		//}
		//catch (...) {}
		}
		__except ( EXCEPTION_EXECUTE_HANDLER )  {}
	}

	BOOL  GetLocationFromAddr ( DWORD dwAddr, std::string& strLocation )
	{
		IMAGEHLP_LINE  ihLine = { sizeof(IMAGEHLP_LINE) };
		DWORD          dwDisplacement = 0;
		char			pszBuf[32];
		
		if ( SymGetLineFromAddr(GetCurrentProcess(), dwAddr, &dwDisplacement, &ihLine) )
		{
			strLocation = ihLine.FileName;
			wsprintfA (pszBuf, "(%d)", ihLine.LineNumber);
			strLocation += pszBuf;
			return  true;
		}
		else  {
			GetModuleNameFromAddr (dwAddr, strLocation);
			strLocation += "(?)";
			return  false;
		}
	}

	void  TraceStackStr ( int nLevel, LPCTSTR pszText )
	{
		if ( nLevel == CRASH_REPORT_LEVEL )
		{
			DWORD	dwWritten = 0;
			SetFilePointer (*m_phCrashRptFile, NULL, NULL, FILE_END);
			WriteFile (*m_phCrashRptFile, pszText, lstrlen(pszText), &dwWritten, NULL);
		}
		else
			InternalTraceStr (nLevel, pszText, 0);
	}

	BOOL  IntelStackWalk ( LPSTACKFRAME psf )
	{
		PDWORD	pFrame = (PDWORD)psf->AddrFrame.Offset,
				pPrevFrame;

		psf->AddrPC.Offset = pFrame[1];
		pPrevFrame = pFrame;
		pFrame = (PDWORD)pFrame[0]; // Proceed to next frame on the stack
		if ( (DWORD)pFrame & 3 )    // Frame pointer must be aligned on a DWORD boundary
			return  FALSE;                  
		if ( pFrame <= pPrevFrame )	// Next frame must be higher on the stack
			return  FALSE;
		// Can two DWORDs be read from the supposed frame address?          
		if ( IsBadWritePtr(pFrame, sizeof(PVOID)*2) )
			return  FALSE;
		psf->AddrFrame.Offset = (DWORD)pFrame;
		return  TRUE;
	}

	void  InternalTraceStack ( int nLevel, PCONTEXT	pCtx, DWORD nExtraNesting, HANDLE hThread, 
					   bool bImagehlpStackWalk = true )
	{
		STACKFRAME	sf = { 0 };
		HANDLE		hProcess = GetCurrentProcess();
		
		sf.AddrPC.Offset    = pCtx->Eip;
		sf.AddrPC.Mode      = AddrModeFlat;
		sf.AddrStack.Offset = pCtx->Esp;
		sf.AddrStack.Mode   = AddrModeFlat;
		sf.AddrFrame.Offset = pCtx->Ebp;
		sf.AddrFrame.Mode   = AddrModeFlat;
	
		bool			bStarted = false;
		int				nCurLevel = 0, 
						nMaxLocationLen = 0;
		std::string		strSymbol, strLocation;
		typedef  std::list<std::pair<std::string, std::string> >	FN_INFO_LIST;
		typedef  FN_INFO_LIST::iterator								FN_INFO_LIST_IT;
		FN_INFO_LIST	listFnCalls;

		//TCHAR	pszBuf[128];
		//wsprintf (pszBuf, _T("Start: Frm 0x%08X, IP 0x%08X\r\n"), sf.AddrFrame.Offset, sf.AddrPC.Offset);
		//OutputDebugString(pszBuf);

		while ( true )
		{
			BOOL	bRes;
			if ( bImagehlpStackWalk )  {
				bRes = StackWalk (IMAGE_FILE_MACHINE_I386, hProcess, hThread, &sf, 
								  NULL, NULL, SymFunctionTableAccess, SymGetModuleBase, NULL);
			}
			else
				bRes = IntelStackWalk (&sf);
			if ( nCurLevel++ < nExtraNesting )
				continue;
			if ( !bRes  ||  !sf.AddrFrame.Offset )
				break;

			// Use TraceFnName to ensure tracing at least function names 
			GetSymFromAddr (sf.AddrPC.Offset, sf.AddrFrame.Offset, *m_pdwStackTraceOpts | TraceFnName, strSymbol);
			//     With some release configurations predicted extra nesting value may prove
			// to be underestimated. So check whether we have skipped all __dbg_util's methods.
			if ( !bStarted  &&  strSymbol.find("__dbg_util::") != NPOS )
				continue;
			bStarted = true;
			strSymbol += "\r\n";
			if ( *m_pdwStackTraceOpts & TraceLocation )
			{
				GetLocationFromAddr (sf.AddrPC.Offset, strLocation);
				strLocation.insert (0, "        ");
				strLocation += " : ";
				nMaxLocationLen = max(nMaxLocationLen, strLocation.length());
			}
			listFnCalls.push_back(FN_INFO_LIST::value_type(strLocation, strSymbol));
		}
		LPTSTR			pszSymbol = NULL,
						pszLocation = NULL;
		FN_INFO_LIST_IT	itFn = listFnCalls.begin();
		CBooster		boost;
		for ( ; itFn != listFnCalls.end(); itFn++ )
		{
			if ( *m_pdwStackTraceOpts & TraceLocation )
			{
				itFn->first.append (nMaxLocationLen - itFn->first.length(), ' ');
				pszLocation = GetTstr (itFn->first);
				TraceStackStr (nLevel, pszLocation);
				delete []  pszLocation;
			}
			// Use TraceFnName to grant tracing at least function names 
			pszSymbol = GetTstr (itFn->second);
			TraceStackStr (nLevel, pszSymbol);
			delete []  pszSymbol;
		}
	}

	void  InternalTraceStack ( int nLevel, DWORD dwFlags, LPCTSTR pcszFmt, va_list argptr )
	{
		CONTEXT		ctx = { 0 };
		ctx.ContextFlags = CONTEXT_FULL;
		if ( !GetThreadContext(GetCurrentThread(), &ctx) )
			return;
		if ( pcszFmt )
			InternalTrace (nLevel, SET_EXTRA_NESTING(dwFlags, 0), pcszFmt, argptr);
		else
			InternalTraceStr (nLevel, _T("Call stack info:\r\n"), Prefix);
		InternalTraceStack (nLevel, &ctx, *m_pnExtraNesting, GetCurrentThread());
	}

	std::string  GetCallerFn ( DWORD dwInfoFlags, long nExtraNesting = 0 )
	{
		STACKFRAME	sf = { 0 };
		CONTEXT		ctx = { 0 };
		HANDLE		hProcess = GetCurrentProcess();
		HANDLE		hThread = GetCurrentThread();
		std::string		strFn;
		
		ctx.ContextFlags = CONTEXT_FULL;
		if ( !GetThreadContext( hThread, &ctx ) )
			return  strFn;

		sf.AddrPC.Offset    = ctx.Eip;
		sf.AddrPC.Mode      = AddrModeFlat;
		sf.AddrStack.Offset = ctx.Esp;
		sf.AddrStack.Mode   = AddrModeFlat;
		sf.AddrFrame.Offset = ctx.Ebp;
		sf.AddrFrame.Mode   = AddrModeFlat;

		BOOL	bRes = FALSE;
		int		nCurLevel = 0;
		while ( true )
		{
			bRes = StackWalk (IMAGE_FILE_MACHINE_I386, hProcess, hThread, &sf, 
							  NULL, NULL, SymFunctionTableAccess, SymGetModuleBase, NULL);
			if ( nCurLevel++ < *m_pnExtraNesting + nExtraNesting )
				continue;
			if ( bRes  &&  sf.AddrFrame.Offset )  {
				GetSymFromAddr (sf.AddrPC.Offset, sf.AddrFrame.Offset, dwInfoFlags, strFn);
				if ( strFn.find("__dbg_util::") != NPOS )
					continue;
			}
			break;
		}
		return  strFn;
	}

	#if !defined(TRACE_NO_CRASH_REPORT)

	static  PTOP_LEVEL_EXCEPTION_FILTER&  GetPevExceptionFilter ()
	{
		static PTOP_LEVEL_EXCEPTION_FILTER	pPrevExceptionFilter = 
								SetUnhandledExceptionFilter(DbgUtilUnhandledExceptionFilter);
		return  pPrevExceptionFilter;
	}

	LPCTSTR  GetExceptionName ( DWORD dwCode )
	{
		#define CheckException( x )  case  EXCEPTION_##x:  return _T(#x);
		switch ( dwCode )
		{
			CheckException (ACCESS_VIOLATION)
			CheckException (DATATYPE_MISALIGNMENT)
			CheckException (BREAKPOINT)
			CheckException (SINGLE_STEP)
			CheckException (ARRAY_BOUNDS_EXCEEDED)
			CheckException (FLT_DENORMAL_OPERAND)
			CheckException (FLT_DIVIDE_BY_ZERO)
			CheckException (FLT_INEXACT_RESULT)
			CheckException (FLT_INVALID_OPERATION)
			CheckException (FLT_OVERFLOW)
			CheckException (FLT_STACK_CHECK)
			CheckException (FLT_UNDERFLOW)
			CheckException (INT_DIVIDE_BY_ZERO)
			CheckException (INT_OVERFLOW)
			CheckException (PRIV_INSTRUCTION)
			CheckException (IN_PAGE_ERROR)
			CheckException (ILLEGAL_INSTRUCTION)
			CheckException (NONCONTINUABLE_EXCEPTION)
			CheckException (STACK_OVERFLOW)
			CheckException (INVALID_DISPOSITION)
			CheckException (GUARD_PAGE)
			CheckException (INVALID_HANDLE)
			default:
				return  _T("UNKNOWN TYPE");
		}
	}

	void  WriteCrashReportTitle ( LPTSTR pszBuf )
	{
		LPTSTR	pszFmt;
		DWORD	dwWritten = 0;
		if ( *m_pdwCrashRptOpts & TraceAddInfoMask )
		{
			// There are some additional information options
			pszFmt = pszBuf + lstrlen(pszBuf) + 1;
			try  {  PrepareFmtString (CRASH_REPORT_LEVEL, pszFmt, pszBuf, 0);  }
			catch (...)  {  pszFmt = pszBuf;  }
		}
		else
			pszFmt = pszBuf;
		SetFilePointer (*m_phCrashRptFile, NULL, NULL, FILE_END);
		WriteFile (*m_phCrashRptFile, pszFmt, lstrlen(pszFmt), &dwWritten, NULL);
	}

	static LONG WINAPI DbgUtilUnhandledExceptionFilter ( PEXCEPTION_POINTERS pExceptionInfo );
	#endif // !defined(TRACE_NO_CRASH_REPORT)

public:
	dbgTracer ()
	{
		static  int				nMaxLevel = MAX_SUPPORTED_TRACE_LEVEL;
		static  LEVELS_VEC		vActiveLevels;
		static  DWORD			adwOpts[MAX_SUPPORTED_TRACE_LEVEL + 1];
		static  HANDLE			ahRptFile[MAX_SUPPORTED_TRACE_LEVEL + 1];
		static  FILE_POS_LIST	aFilePosList[MAX_SUPPORTED_TRACE_LEVEL + 1];

		static  DWORD			dwCrashRptOpts = 0;
		static  DWORD			dwCrashRptFileOpts = TraceWriteThrough | TraceNewLog;
		static  TCHAR			pszCrashRptFile[MAX_PATH];
		static  HANDLE			hCrashRptFile = NULL;
		static  COMPUTER_CFG_INFO		fnComputerCfgInfo = NULL;
		static  CRASHREPORTCALLBACK		fnCrashRptCallback = CrashReportCallback;

		static  LPTSTR			pszBuffer = NULL;
		static  long			nBufferSize = 0,
								nBufferCurPos = 0,
								nBufferDeadLine = 0,
								nBufferEnd = 0,
								nExtraNesting = NO_SYM_HANDLER,
								nPrepareFmtStringExtraNesting = NO_SYM_HANDLER;
		static  DWORD			dwStackTraceOpts = TraceStdStackInfo,
								nTestNumber = 0,
								nObjects = 0;
		static  bool			bStart = true;

		m_pnMaxLevel = &nMaxLevel;
		m_pActiveLevelsSet = &vActiveLevels;
		m_pdwOpts = adwOpts;
		m_phRptFiles = ahRptFile;
		m_pFilePosLists = aFilePosList;

		m_pdwCrashRptOpts = &dwCrashRptOpts;
		m_pdwCrashRptFileOpts = &dwCrashRptFileOpts;
		m_pszCrashRptFile = pszCrashRptFile;
		m_phCrashRptFile = &hCrashRptFile;
		m_pfnComputerCfgInfo = &fnComputerCfgInfo;
		m_pfnCrashRptCallback = &fnCrashRptCallback;

		m_ppszBuffer = &pszBuffer;
		m_pnBufferSize = &nBufferSize;
		m_pnBufferCurPos = &nBufferCurPos;
		m_pnBufferDeadLine = &nBufferDeadLine;
		m_pnBufferEnd = &nBufferEnd;
		m_pnExtraNesting = &nExtraNesting;
		m_pnPrepareFmtStringExtraNesting = &nPrepareFmtStringExtraNesting;
		m_pdwStackTraceOpts = &dwStackTraceOpts;
		m_pnTestNumber = &nTestNumber;
		m_pnObjects = &nObjects;
		m_hMutex = NULL;

		if ( !nObjects++ )
		{
			AddNameToModulePath (pszCrashRptFile, MAX_PATH, _T("CrashReport.txt"));
			#if !defined(TRACE_NO_SYM_INFO)
				InitSymHandler();
			#else
				SetExtraNesting();
			#endif
			#if !defined(TRACE_NO_CRASH_REPORT)
				// Force SetUnhandledExceptionFilter to be invoked:
				GetPevExceptionFilter();
			#endif
			for ( int i = 0; i <= MAX_SUPPORTED_TRACE_LEVEL; i++ )
			{
				if ( bStart )
					ahRptFile[i] = 0;
				adwOpts[i] = TraceStdInfo;
			}
			bStart = false;
		}

		m_pszTestsSeparatorFmt = NULL;
		SetTestsSeparator (_T("%02u --------------------------------------------------------------------------------------"));
		//InitializeCriticalSection (&m_critSettings);
		if ( QueryPerformanceFrequency((PLARGE_INTEGER)&m_i64PerfFreq) )
		{
			__int64		i64PerfCnt;
			FILETIME	ftPrev, ft, ftLocal;
			GetSystemTimeAsFileTime (&ft);
			do  {
				ftPrev.dwLowDateTime = ft.dwLowDateTime;
				GetSystemTimeAsFileTime (&ft);
			}  while ( ftPrev.dwLowDateTime == ft.dwLowDateTime);
			QueryPerformanceCounter((PLARGE_INTEGER)&i64PerfCnt);
			FileTimeToLocalFileTime(&ft, &ftLocal);
			m_i64PerfCntAmendment = *(PINT64)&ftLocal - (i64PerfCnt * 10000000) / m_i64PerfFreq;
			m_i64PerfCntAmendmentUTC = *(PINT64)&ft - (i64PerfCnt * 10000000) / m_i64PerfFreq;
		}
		else
			m_i64PerfFreq = m_i64PerfCntAmendment = 0;
	}

	~dbgTracer()
	{
		if ( m_pszTestsSeparatorFmt )
			delete []  m_pszTestsSeparatorFmt;
		//DeleteCriticalSection (&m_critSettings);
		if ( !--*m_pnObjects )
		{
			#if !defined(TRACE_NO_SYM_INFO)
				if ( m_hMutex )
					SymCleanup (GetCurrentProcess());
			#endif
			//for ( int i = 0; i <= MAX_SUPPORTED_TRACE_LEVEL; i++ )  {
				//if ( m_phRptFiles[i] )  {
				//	CloseHandle (m_phRptFiles[i]);
				//	m_phRptFiles[i] = NULL;
				//}
				//m_pFilePosLists[i].clear();
			//}
		}
		if ( m_hMutex )
			CloseHandle (m_hMutex);
	}

	LONG  TraceCrashReport ( PEXCEPTION_POINTERS pExceptionInfo, BOOL bKillProcess = FALSE )
	{
		bool	bAllThreads = pExceptionInfo || bKillProcess,
				bOK = true;
		LPTSTR	pszBuf = NULL,
				pszPath;
		DWORD	dwBufSize;
		CONTEXT	*pCtx;
		DWORD	dwWritten = 0;
		DWORD	nThreads = 1,
				dwCurProcessID = GetCurrentProcessId(),
				adwThreads[MAX_THREADS] = { GetCurrentThreadId() };
		HANDLE	ahThreads[MAX_THREADS] = { GetCurrentThread() };

		CBooster	boost;
		// Try to enumerate and suspend all threads except the current one.
		// This won't have any effect on NT4 or Win98.
		if ( bAllThreads )  try
		{
			CToolHelp	th;
			if ( th.IsAvailable()  &&  th.CanGetThreadHandles()  &&  th.CreateSnapshot(CToolHelp::Thread) )
			{
				BOOL	bNext = th.Thread32First();
				while ( bNext )  {
					if ( th.GetThreadOwnerProcessID() == dwCurProcessID  &&
						 th.GetThreadID() != adwThreads[0] )
					{
						adwThreads[nThreads] = th.GetThreadID();
						ahThreads[nThreads] = th.GetThreadHandle (THREAD_GET_CONTEXT | THREAD_SUSPEND_RESUME);
						if ( ahThreads[nThreads] )
							SuspendThread (ahThreads[nThreads]);
						nThreads++;
					}
					bNext = th.Thread32Next();
				}
			}
		}
		catch (...)  {}

		const int  nBaseBufLen = 4096;
		dwBufSize = (nBaseBufLen + MAX_PATH) * sizeof(TCHAR) + sizeof(CONTEXT);
		// Allocate report buffer, sequentially trying different memory banks (an exception 
		// might probably have corrupted some of them, but at least one should stay working).
		// This buffer won't be freed later since the program will be completed all the same.
		try  {
			pszBuf = (LPTSTR)_alloca(dwBufSize);
		}
		catch (...)  {
			try  {
				pszBuf = (LPTSTR)new BYTE[dwBufSize];
				if ( !pszBuf )
					throw  0;
			}
			catch (...)  {
				try  {
					pszBuf = (LPTSTR)GlobalAlloc(GMEM_FIXED, dwBufSize);
					if ( !pszBuf )
						throw  0;
				}
				catch (...)  {
					try  {
						pszBuf = (LPTSTR)VirtualAlloc(NULL, dwBufSize, MEM_COMMIT, PAGE_READWRITE);
					}
					catch (...)  {
						pszBuf = NULL;
					}
				}
			}
		}
		if ( !pszBuf )
			return  EXCEPTION_CONTINUE_SEARCH;

		pCtx = (PCONTEXT)(pszBuf + nBaseBufLen + MAX_PATH);
		pszPath = pszBuf + nBaseBufLen;

		try  {
			if ( !*m_phCrashRptFile )
				OpenRptFile (m_phCrashRptFile, m_pszCrashRptFile, *m_pdwCrashRptFileOpts);
		}
		catch (...)  {  bOK = false;  }
		if ( !bOK )
			return  EXCEPTION_CONTINUE_SEARCH;

		if ( pExceptionInfo )  try
		{  
			// Called from the unhandled exceptions handler
			PEXCEPTION_RECORD	pExceptionRecord = pExceptionInfo->ExceptionRecord;
			PCONTEXT			pCtx = pExceptionInfo->ContextRecord;
			DWORD	dwLastError = -1;
			try  {  dwLastError = GetLastError();  }
			catch (...) {}
			wsprintf (pszBuf, _T("Unhandled exception 0x%08X (%s) caught at address 0x%08X in the thread %d.\r\n")
					_T(" -> Registers:\r\n")
					_T("        EAX = 0x%08X, EBX = 0x%08X, ECX = 0x%08X, EDX = 0x%08X, \r\n")
					_T("        ESI = 0x%08X, EDI = 0x%08X, EBP = 0x%08X, ESP = 0x%08X, \r\n")
					_T("        EFlags = 0x%08X, CS = 0x%04X, SS = 0x%04X, DS = 0x%04X, ES = 0x%0X, FS = 0x%04X, GS = 0x%04X\r\n")
					_T(" -> Last error: 0x%08X\r\n")
					_T(" -> Call stack (thread %d):\r\n"), 
					pExceptionRecord->ExceptionCode, GetExceptionName(pExceptionRecord->ExceptionCode),
					pExceptionRecord->ExceptionAddress, adwThreads[0],
					pCtx->Eax, pCtx->Ebx, pCtx->Ecx, pCtx->Edx, pCtx->Esi, pCtx->Edi, pCtx->Ebp, pCtx->Esp,
					pCtx->EFlags, pCtx->SegCs, pCtx->SegSs, pCtx->SegDs, pCtx->SegEs, pCtx->SegFs, pCtx->SegGs, 
					dwLastError, adwThreads[0]
				);
			_ASSERTE (!(*m_pdwCrashRptOpts & (TraceDbgOutput | TraceFnMask | ExtraNestingMask)));
			WriteCrashReportTitle (pszBuf);
		}
		catch (...)  {}

		try  {  
			pCtx->ContextFlags = CONTEXT_FULL;
			SetFilePointer (*m_phCrashRptFile, NULL, NULL, FILE_END);
			if ( pExceptionInfo )  {
				CopyMemory (pCtx, pExceptionInfo->ContextRecord, sizeof(CONTEXT));
				InternalTraceStack (CRASH_REPORT_LEVEL, pCtx, 0, ahThreads[0]);
			}
			else if ( GetThreadContext(GetCurrentThread(), pCtx) )
			{
				wsprintf (pszBuf, _T("Deadlock detected in the thread %d. Call stack:\r\n"), adwThreads[0]);
				WriteCrashReportTitle (pszBuf);
				InternalTraceStack (CRASH_REPORT_LEVEL, pCtx, *m_pnExtraNesting, GetCurrentThread());
				//wsprintf (pszBuf, _T("    Call stack (thread %d) using simple StackWalk:\r\n"), adwThreads[0]);
				//WriteFile (*m_phCrashRptFile, pszBuf, lstrlen(pszBuf), &dwWritten, NULL);
				//TraceStack (&ctx, 0, ahThreads[0], true, false);
			}
			if ( bAllThreads )  {
				for ( int i = 1; i < nThreads; i++ )
				{
					if ( !ahThreads[i]  ||  !GetThreadContext(ahThreads[i], pCtx) )
						continue;
					wsprintf (pszBuf, _T(" -> Call stack (thread %d):\r\n"), adwThreads[i]);
					WriteFile (*m_phCrashRptFile, pszBuf, lstrlen(pszBuf), &dwWritten, NULL);
					InternalTraceStack (CRASH_REPORT_LEVEL, pCtx, 0, ahThreads[i]);
				}
			}
		}
		catch (...)  {}

		try  {
			#define	 COMPUTER_CFG		_T(" -> Computer configuration:\r\n")
			#define	 ADDITIONAL_INFO	_T(" -> Additional information:\r\n")
			WriteFile (*m_phCrashRptFile, COMPUTER_CFG, sizeof(COMPUTER_CFG) - sizeof(TCHAR), &dwWritten, NULL);
			ComputerCfgInfo(pszBuf);
			WriteFile (*m_phCrashRptFile, pszBuf, lstrlen(pszBuf), &dwWritten, NULL);
			if ( *m_pfnComputerCfgInfo )  {
				pszBuf[0] = 0;
				(*m_pfnComputerCfgInfo)(pszBuf);
				if ( pszBuf[0] )  {
					WriteFile (*m_phCrashRptFile, ADDITIONAL_INFO, sizeof(ADDITIONAL_INFO) - sizeof(TCHAR), &dwWritten, NULL);
					WriteFile (*m_phCrashRptFile, pszBuf, lstrlen(pszBuf), &dwWritten, NULL);
				}
			}
		}
		catch (...)  {}

		try  {
			#define	 BUFFERED_TRACES	_T(" -> Buffered traces:\r\n")
			bool	bMemBuffer = false;
			for ( int i =0; i <= MAX_SUPPORTED_TRACE_LEVEL; i++ )  {
				if ( m_pdwOpts[i] & TraceMemBuffer )  {
					bMemBuffer = true;
					break;
				}
			}
			if ( bMemBuffer )  {
				WriteFile (*m_phCrashRptFile, BUFFERED_TRACES, sizeof(BUFFERED_TRACES) - sizeof(TCHAR), &dwWritten, NULL);
				FlushTraceBuffer(CRASH_REPORT_LEVEL);
			}
		}
		catch (...)  {}
		if ( *m_pfnCrashRptCallback ) try
		{
			pszPath[0] = 0;
			try  {  GetModuleFileName (GetModuleHandle(NULL), pszPath, MAX_PATH);  }
			catch (...)  {}
			(*m_pfnCrashRptCallback)(pExceptionInfo, *m_phCrashRptFile, 
									 m_pszCrashRptFile, GetFileNamePtr(m_pszCrashRptFile),
									 pszPath, GetFileNamePtr(pszPath));
		}
		catch (...) {}

		if ( bKillProcess )
			TerminateProcess (GetCurrentProcess(), 1);

		// Never close *m_phCrashRptFile if there is a chance of process continuation.

		//     If this method is called from the unhandled exceptions handler, don't
		// resume other threads, because they may crash as well and produce unnecessary
		// crash reports.
		if ( bAllThreads  &&  !pExceptionInfo )  {
			for ( int i = 1; i < nThreads; i++ )  {
				if ( ahThreads[i] )  {
					ResumeThread(ahThreads[i]);
					CloseHandle(ahThreads[i]);
				}
			}
		}
		if ( pExceptionInfo  &&  GetPevExceptionFilter() )
			return  GetPevExceptionFilter()(pExceptionInfo);
		else
			return  EXCEPTION_CONTINUE_SEARCH;
	}

	dbgTracer*  SetTraceInfo ( LPCTSTR pcszFile, unsigned uLine, DWORD dwFlags )
	{
		m_pszFile = const_cast<LPTSTR>(pcszFile);
		m_dwLine = uLine;
		m_dwTraceFlags = dwFlags;
		return  this;
	}

	void  SetRptOpts ( int nLevel, DWORD dwOpts )
	{
		if ( nLevel == CRASH_REPORT_LEVEL )  {
			*m_pdwCrashRptOpts = dwOpts & ~(TraceDbgOutput | TraceFnMask | ExtraNestingMask);
			return;
		}
		if ( nLevel == ALL_TRACE_LEVELS )  {
			for ( int i = 0; i <= MAX_SUPPORTED_TRACE_LEVEL; i++ )
				m_pdwOpts[i] = dwOpts;
			return;
		}
		CheckTraceLevelCorrectness(nLevel);
		m_pdwOpts[nLevel] = dwOpts;
	}

	void  SetRptFile ( int nLevel, LPCTSTR pcszFile, DWORD dwFileOpts )
	{
		if ( nLevel == CRASH_REPORT_LEVEL )  {
			if ( !*m_phCrashRptFile )  {
				CloseHandle(*m_phCrashRptFile);
				*m_phCrashRptFile = NULL;
			}
			lstrcpy (m_pszCrashRptFile, pcszFile);
			*m_pdwCrashRptFileOpts = dwFileOpts;
			return;
		}
		if ( nLevel == ALL_TRACE_LEVELS )  {
			for ( int i = 0; i <= MAX_SUPPORTED_TRACE_LEVEL; i++ )
				SetRptFile ( i, pcszFile, dwFileOpts );
			return;
		}
		CheckTraceLevelCorrectness(nLevel);
		OpenRptFile (m_phRptFiles + nLevel, pcszFile, dwFileOpts);
	}

	void  SetStackTraceOpts ( DWORD dwOpts )
	{
		*m_pdwStackTraceOpts = dwOpts;
	}

	void  SetMaxTraceLevel ( int nLevel )
	{
		CheckTraceLevelCorrectness(nLevel);
		*m_pnMaxLevel = nLevel;
		m_pActiveLevelsSet->clear();
	}

	void  SetActiveTraceLevel ( int nLevel )
	{
		CheckTraceLevelCorrectness(nLevel);
		m_pActiveLevelsSet->clear();
		m_pActiveLevelsSet->push_back (nLevel);
	}

	void  SetActiveTraceLevels ( int nCount, int* pLevels )
	{
		if ( !pLevels )
			return;
		m_pActiveLevelsSet->clear();
		for ( int i = 0; i < nCount; i++ )  {
			CheckTraceLevelCorrectness(pLevels[i]);
			// Keep up the vector sorted
			LEVELS_VEC_IT	itNext = std::lower_bound (m_pActiveLevelsSet->begin(), 
													   m_pActiveLevelsSet->end(), pLevels[i]);
			m_pActiveLevelsSet->insert (itNext, pLevels[i]);
		}
	}

	void  SetComputerCfgInfo ( COMPUTER_CFG_INFO fnComputerCfgInfo )
	{
		*m_pfnComputerCfgInfo = fnComputerCfgInfo;
	}

	void  SetCrashRptCallback ( CRASHREPORTCALLBACK fnCrashRptCallback )
	{
		*m_pfnCrashRptCallback = fnCrashRptCallback;
	}

	void  SetTraceBuffer ( LPTSTR pszBuffer, long nBufferSize )
	{
		*m_ppszBuffer = pszBuffer;
		*m_pnBufferSize = nBufferSize;
		*m_pnBufferCurPos = 0;
		*m_pnBufferDeadLine = nBufferSize - MAX_TRACE_MSG_LEN;
		*m_pnBufferEnd = *m_pnBufferDeadLine;
	}

	void  FlushTraceBuffer ( int nLevel, LPTSTR pszBuffer, long nCurPos, long nBufferEnd, DWORD dwOpts )
	{
		while ( pszBuffer[nCurPos]  &&  nCurPos < nBufferEnd )
		{
			long	nMsgLen = lstrlen(pszBuffer + nCurPos);
			if ( dwOpts & TraceDbgOutput )
				OutputDebugString (pszBuffer + nCurPos);
			OutputMsgToFile (nLevel, pszBuffer + nCurPos, nMsgLen * sizeof(TCHAR), false);
			nCurPos += nMsgLen + 1;
		}
	}

	void  FlushTraceBuffer ( int nLevel )
	{
		CheckTraceLevelCorrectness(nLevel);
		if ( !*m_ppszBuffer )
			return;

		CBooster	boost;
		m_iLevel = nLevel;
		long	nCurPosSrc = *m_pnBufferCurPos,
				nBufferEnd = *m_pnBufferEnd;
		LPTSTR	pszBuffer = (LPTSTR)_alloca(nBufferEnd * sizeof(TCHAR));
		CopyMemory (pszBuffer, *m_ppszBuffer, nBufferEnd * sizeof(TCHAR));
		*m_pnBufferCurPos = 0;
		*m_pnBufferEnd = *m_pnBufferDeadLine;
		ZeroMemory(*m_ppszBuffer, *m_pnBufferSize);

		long	nCurPos = nCurPosSrc;
		DWORD	dwOpts = GetParams(nLevel);

		if ( nCurPos < nBufferEnd  &&  pszBuffer[nCurPos]  ||
			 nCurPos < nBufferEnd - 1  &&  pszBuffer[nCurPos + 1] )
		{
			while ( pszBuffer[nCurPos++] );	// skips partially overwritten message
			FlushTraceBuffer (nLevel, pszBuffer, nCurPos, nBufferEnd, dwOpts);
		}
		FlushTraceBuffer (nLevel, pszBuffer, 0, nCurPosSrc, dwOpts);
	}
	
	void  RemoveLastTrace ( int nLevel = 0 )
	{
		HANDLE	hFile;
		FILE_POS_LIST		*pFilePosList;
		DWORD	dwOpts = GetParams(nLevel, &hFile, &pFilePosList);

		CBooster	boost;
		if ( hFile  &&  pFilePosList->size() )  {
			SetFilePointer (hFile, pFilePosList->back(), NULL, FILE_BEGIN);
			pFilePosList->pop_back();
			SetEndOfFile (hFile);
		}
	}

	void  Dump ( PVOID pvData, DWORD dwDataSize, DWORD dwParams = 0, LPCTSTR pcszFmt = NULL, ... )
	{
		if ( IsTraceLevelDisabled(0) )
			return;
		TCHAR	pszMsg[max(MAX_TRACE_MSG_LEN, MAX_DUMPED_DATA_LEN)];
		va_list		argptr; 
		va_start (argptr, pcszFmt);
		InternalDump (0, pszMsg, pvData, dwDataSize, dwParams, pcszFmt, argptr);
		va_end (argptr);
	}
	void  Dump ( int nLevel, PVOID pvData, DWORD dwDataSize, DWORD dwParams = 0, LPCTSTR pcszFmt = NULL, ... )
	{
		CheckTraceLevelCorrectness(nLevel);
		if ( IsTraceLevelDisabled(nLevel) )
			return;
		TCHAR	pszMsg[max(MAX_TRACE_MSG_LEN, MAX_DUMPED_DATA_LEN)];
		va_list		argptr; 
		va_start (argptr, pcszFmt);
		InternalDump (nLevel, pszMsg, pvData, dwDataSize, dwParams, pcszFmt, argptr);
		va_end (argptr);
	}

	void  DumpTable ( PVOID pvData, DWORD dwDataSize, DWORD nCols, 
					  DWORD dwParams = 0, LPCTSTR pcszFmt = NULL, ... )
	{
		if ( IsTraceLevelDisabled(0) )
			return;
		TCHAR	pszMsg[max(MAX_TRACE_MSG_LEN, MAX_DUMPED_DATA_LEN)];
		va_list		argptr; 
		va_start (argptr, pcszFmt);
		InternalDumpTable (0, pszMsg, pvData, dwDataSize, nCols, dwParams, pcszFmt, argptr);
		va_end (argptr);
	}
	void  DumpTable ( int nLevel, PVOID pvData, DWORD dwDataSize, DWORD nCols, 
					  DWORD dwParams = 0, LPCTSTR pcszFmt = NULL, ... )
	{
		CheckTraceLevelCorrectness(nLevel);
		if ( IsTraceLevelDisabled(nLevel) )
			return;
		TCHAR	pszMsg[max(MAX_TRACE_MSG_LEN, MAX_DUMPED_DATA_LEN)];
		va_list		argptr; 
		va_start (argptr, pcszFmt);
		InternalDumpTable (nLevel, pszMsg, pvData, dwDataSize, nCols, dwParams, pcszFmt, argptr);
		va_end (argptr);
	}

	void  Trace ( LPCTSTR pcszFmt, ... )
	{
		if ( IsTraceLevelDisabled(0) )
			return;
		va_list		argptr; 
		va_start (argptr, pcszFmt);
		InternalTrace (0, m_dwTraceFlags, pcszFmt, argptr);
		va_end (argptr);
	}

	void  Trace ( int nLevel, LPCTSTR pcszFmt, ... )
	{
		if ( IsTraceLevelDisabled(nLevel) )
			return;
		va_list		argptr; 
		va_start (argptr, pcszFmt);
		InternalTrace (nLevel, m_dwTraceFlags, pcszFmt, argptr);
		va_end (argptr);
	}

	void  TraceStr ( LPCTSTR pcszMsg )
	{
		if ( IsTraceLevelDisabled(0) )
			return;
		InternalTraceStr (0, pcszMsg, 0);
	}

	void  TraceStr ( int nLevel, LPCTSTR pcszMsg )
	{
		if ( IsTraceLevelDisabled(nLevel) )
			return;
		InternalTraceStr (nLevel, pcszMsg, 0);
	}

	void  TraceStrPrefix ( LPCTSTR pcszMsg )
	{
		if ( IsTraceLevelDisabled(0) )
			return;
		InternalTraceStr (0, pcszMsg, Prefix);
	}

	void  TraceStrPrefix ( int nLevel, LPCTSTR pcszMsg )
	{
		if ( IsTraceLevelDisabled(nLevel) )
			return;
		InternalTraceStr (nLevel, pcszMsg, Prefix);
	}

	void  TraceStack ( LPCTSTR pcszFmt = NULL, ... )
	{
		if ( IsTraceLevelDisabled(0) )
			return;
		va_list		argptr; 
		va_start (argptr, pcszFmt);
		InternalTraceStack (0, m_dwTraceFlags, pcszFmt, argptr);
		va_end (argptr);
	}

	void  TraceStack ( int nLevel, LPCTSTR pcszFmt = NULL, ... )
	{
		if ( IsTraceLevelDisabled(nLevel) )
			return;
		va_list		argptr; 
		va_start (argptr, pcszFmt);
		InternalTraceStack (nLevel, m_dwTraceFlags, pcszFmt, argptr);
		va_end (argptr);
	}

	void  TrAssert ( BOOL bExpr, LPCTSTR pcszFmt, ... )
	{
		if ( bExpr )
			return;
		TCHAR	pszMsg[MAX_TRACE_MSG_LEN];
		va_list		argptr; 
		va_start (argptr, pcszFmt);
		InternalTrace (0, pszMsg, m_dwTraceFlags, pcszFmt, argptr);
		va_end (argptr);
		#if defined(_DEBUG)
		CHAR	pszMsgA[MAX_TRACE_MSG_LEN];
		CHAR	pszFileA[MAX_PATH * 2];
		if ( sizeof(TCHAR) == sizeof(WCHAR) )  {
			WideCharToMultiByte (CP_ACP, 0, (LPCWSTR)pszMsg, -1, pszMsgA, MAX_TRACE_MSG_LEN, NULL, NULL);
			WideCharToMultiByte (CP_ACP, 0, (LPCWSTR)m_pszFile, -1, pszFileA, MAX_PATH * 2, NULL, NULL);
		}
		else  {
			lstrcpyA (pszMsgA, (LPCSTR)pszMsg);
			lstrcpyA (pszFileA, (LPCSTR)m_pszFile);
		}
		if ( _CrtDbgReport(_CRT_ASSERT, pszFileA, m_dwLine, NULL, pszMsgA) == 1 )
			_CrtDbgBreak();
		#endif
	}

	void  SetTestsSeparator ( LPCTSTR pcszTestsSeparatorFmt )
	{
		if ( m_pszTestsSeparatorFmt )
			delete []  m_pszTestsSeparatorFmt;
		if ( pcszTestsSeparatorFmt )  {
			m_pszTestsSeparatorFmt = new TCHAR[lstrlen(pcszTestsSeparatorFmt) + 1];
			lstrcpy (m_pszTestsSeparatorFmt, pcszTestsSeparatorFmt);
		}
		else
			m_pszTestsSeparatorFmt = NULL;
	}

	void  ResetTestNumber ()  {  *m_pnTestNumber = 0;  }

	void  TraceExpected ( LPCTSTR pcszFmt, ... )
	{
		if ( m_pszTestsSeparatorFmt )  {
			m_dwTraceFlags = PfLf;
			Trace (m_pszTestsSeparatorFmt, ++(*m_pnTestNumber));
		}
		m_dwTraceFlags = Pf;
		Trace (_T("Expected: "));
		va_list		argptr; 
		va_start (argptr, pcszFmt);
		InternalTrace (0, GetExpectedBuf(), Lf, pcszFmt, argptr);
		va_end (argptr);
	}

	void  TraceResulted ( LPCTSTR pcszFmt, ... )
	{
		bool	bAssert = !(m_dwTraceFlags & DontAssert);
		m_dwTraceFlags = Pf;
		Trace (_T("Resulted: "));
		va_list		argptr; 
		va_start (argptr, pcszFmt);
		InternalTrace (0, GetResultedBuf(), Lf, pcszFmt, argptr);
		va_end (argptr);
		if ( bAssert  &&  lstrcmp(GetExpectedBuf(), GetResultedBuf()) != 0 )
		{
			GetExpectedBuf()[lstrlen(GetExpectedBuf()) - 2] = 0;	// suppress trailing \r\n
			m_dwTraceFlags = Lf;
			TrAssert (FALSE, _T("Expectation \"%s\" failed"), GetExpectedBuf());
		}
	}

};

}  // end namespace __dbg_util

static __dbg_util::dbgTracer	__dbg_util__dbgTracer;

inline
LONG  WINAPI  __dbg_util::dbgTracer::DbgUtilUnhandledExceptionFilter ( PEXCEPTION_POINTERS pExceptionInfo )
{
	return  __dbg_util__dbgTracer.TraceCrashReport(pExceptionInfo);
}

struct  dbgTracer__FileInitializer
{
	dbgTracer__FileInitializer ( int nLevel, LPCTSTR pcszFile, DWORD dwOpts )
	{
		__dbg_util__dbgTracer.SetRptFile (nLevel, pcszFile, dwOpts);
	}
};

struct  dbgTracer__OptInitializer
{
	dbgTracer__OptInitializer ( int nLevel, DWORD dwOpts )
	{
		__dbg_util__dbgTracer.SetRptOpts (nLevel, dwOpts);
	}
};

struct  dbgTracer__StackTraceOptInitializer
{
	dbgTracer__StackTraceOptInitializer ( DWORD dwOpts )
	{
		__dbg_util__dbgTracer.SetStackTraceOpts (dwOpts);
	}
};

struct  dbgTracer__CompCfgInfoInitializer
{
	dbgTracer__CompCfgInfoInitializer ( COMPUTER_CFG_INFO fnComputerCfgInfo )
	{
		__dbg_util__dbgTracer.SetComputerCfgInfo (fnComputerCfgInfo);
	}
};

struct  dbgTracer__CrashRptCallbackInitializer
{
	dbgTracer__CrashRptCallbackInitializer ( CRASHREPORTCALLBACK fnCrashRptCallback )
	{
		__dbg_util__dbgTracer.SetCrashRptCallback (fnCrashRptCallback);
	}
};

#define  SET_TRACE_FILE(nLevel, pcszFile, dwOpts)	static dbgTracer__FileInitializer  __dbg_util__fi__##nLevel(nLevel, pcszFile, dwOpts)
#define  SET_TRACE_OPTS(nLevel, dwOpts)				static dbgTracer__OptInitializer  __dbg_util__oi__##nLevel(nLevel, dwOpts)
#define  SET_STACK_TRACE_OPTS(dwOpts)		static dbgTracer__StackTraceOptInitializer  __dbg_util__sti__(dwOpts)
#define  SET_COMPUTER_CFG_INFO(fnComputerCfgInfo)	static  dbgTracer__CompCfgInfoInitializer __dbg_util__ccii__(fnComputerCfgInfo)
#define  SET_CRASH_RPT_CALLBACK(fnCrashRptCallback)	static  dbgTracer__CrashRptCallbackInitializer __dbg_util__crci__(fnCrashRptCallback)

#define  TRACE_DEADLOCK(bKillProcess)	\
					__dbg_util__dbgTracer.SetTraceInfo(FileLineInfo, 0)->TraceCrashReport(NULL, bKillProcess)

#else  // if  !defined(_DEBUG)  &&  !defined(RELEASE_TRACE)  &&  defined(TRACE_NO_CRASH_REPORT)

#define  SET_TRACE_FILE(nLevel, pcszFile, dwOpts)
#define  SET_TRACE_OPTS(nLevel, dwOpts)
#define  SET_STACK_TRACE_OPTS(dwOpts)
#define  SET_COMPUTER_CFG_INFO(fnComputerCfgInfo)
#define  SET_CRASH_RPT_CALLBACK(fnCrashRptCallback)
#define  TRACE_DEADLOCK(bKillProcess)

#endif  // !defined(_DEBUG)  &&  !defined(RELEASE_TRACE)  &&  defined(TRACE_NO_CRASH_REPORT)



#if defined(_DEBUG)  ||  defined(RELEASE_TRACE)


struct  dbgTracer__TraceLevelInitializer
{
	dbgTracer__TraceLevelInitializer ( int nLevel, bool bMax )
	{
		if ( bMax )
			__dbg_util__dbgTracer.SetMaxTraceLevel (nLevel);
		else
			__dbg_util__dbgTracer.SetActiveTraceLevel (nLevel);
	}
	dbgTracer__TraceLevelInitializer ( int nCount, int* pLevels )
	{
		__dbg_util__dbgTracer.SetActiveTraceLevels (nCount, pLevels);
	}
};

struct  dbgTracer__TraceBufferInitializer
{
	dbgTracer__TraceBufferInitializer ( LPTSTR pszBuffer, long nBufferSize )
	{
		__dbg_util__dbgTracer.SetTraceBuffer (pszBuffer, nBufferSize);
	}
};


#define  SET_TRACE_BUFFER(dwLen)		\
	static  TCHAR  __gs__dbg_util__trace_buffer[max(dwLen, MAX_TRACE_MSG_LEN) + MAX_TRACE_MSG_LEN] = { 0 };	\
	static  dbgTracer__TraceBufferInitializer __dbg_util__bi__ (__gs__dbg_util__trace_buffer, max(dwLen, MAX_TRACE_MSG_LEN) + MAX_TRACE_MSG_LEN)

#define  SET_MAX_TRACE_LEVEL(nLevel)				static dbgTracer__TraceLevelInitializer __dbg_util__li__(nLevel, true)
#define  SET_ACTIVE_TRACE_LEVEL(nLevel)				static dbgTracer__TraceLevelInitializer __dbg_util__li__(nLevel, false)
#define  SET_ACTIVE_TRACE_LEVELS(nCount, pLevels)	static dbgTracer__TraceLevelInitializer __dbg_util__li__(nCount, pLevels)

#define  FileLineInfo	_T(__FILE__), __LINE__

#define  TRACE		__dbg_util__dbgTracer.SetTraceInfo(FileLineInfo, __dbg_util::PfLf)->Trace
#define  ELSE_TRACE	else __dbg_util__dbgTracer.SetTraceInfo(FileLineInfo, __dbg_util::PfLf)->Trace
#define  TRACENP	__dbg_util__dbgTracer.SetTraceInfo(FileLineInfo, __dbg_util::Lf)->Trace
#define  TRACENL	__dbg_util__dbgTracer.SetTraceInfo(FileLineInfo, __dbg_util::Pf)->Trace
#define  TRACENLP	__dbg_util__dbgTracer.SetTraceInfo(FileLineInfo, 0)->Trace
#define  TRACENPL	__dbg_util__dbgTracer.SetTraceInfo(FileLineInfo, 0)->Trace
#define  TRACERB	__dbg_util__dbgTracer.SetTraceInfo(FileLineInfo, __dbg_util::PfLfRb)->Trace
#define  REMOVE_LAST_TRACE	__dbg_util__dbgTracer.RemoveLastTrace
#define  TRACESTR	__dbg_util__dbgTracer.TraceStr
#define  TRACESTRP	__dbg_util__dbgTracer.SetTraceInfo(FileLineInfo, __dbg_util::Pf)->TraceStrPrefix
#define  DUMP		__dbg_util__dbgTracer.SetTraceInfo(FileLineInfo, __dbg_util::Pf)->Dump
#define  DUMPNP		__dbg_util__dbgTracer.SetTraceInfo(FileLineInfo, 0)->Dump
#define  DUMPTBL	__dbg_util__dbgTracer.SetTraceInfo(FileLineInfo, __dbg_util::Pf)->DumpTable

#define  TRACE_ARGS			__dbg_util__dbgTracer.SetTraceInfo(FileLineInfo, __dbg_util::PfLf | TraceFnArgsStripped)->Trace
#define  TRACE_ARGS_FULL	__dbg_util__dbgTracer.SetTraceInfo(FileLineInfo, __dbg_util::PfLf | TraceFnArgs)->Trace

#define  TRACE_STACK	__dbg_util__dbgTracer.SetTraceInfo(FileLineInfo, __dbg_util::PfLf)->TraceStack
#define  TRACE_STACKNP	__dbg_util__dbgTracer.SetTraceInfo(FileLineInfo, __dbg_util::Lf)->TraceStack

#define  FLUSH_TRACE_BUFFER(nLevel)		__dbg_util__dbgTracer.FlushTraceBuffer(nLevel);

// Simple testing routines support
#define  SET_TESTS_SEPARATOR(strFmt)	__dbg_util__dbgTracer.SetTestsSeparator(strFmt)
#define  RESET_TEST_NUM()	__dbg_util__dbgTracer.ResetTestNumber()
#define  EXPECTED			__dbg_util__dbgTracer.SetTraceInfo(FileLineInfo, 0)->TraceExpected
#define  RESULTED			__dbg_util__dbgTracer.SetTraceInfo(FileLineInfo, 0)->TraceResulted
#define  RESULTEDNA			__dbg_util__dbgTracer.SetTraceInfo(FileLineInfo, __dbg_util::DontAssert)->TraceResulted


#else  //if !defined(_DEBUG)  &&  !defined(RELEASE_TRACE)

inline  void __dbg_util__release_Trace( LPCTSTR = 0, ... )  {}
inline  void __dbg_util__release_Trace( int, LPCTSTR = 0, ... )  {}
inline  void __dbg_util__release_RemoveLastTrace ( int = 0 )  {}
inline  void __dbg_util__release_Dump( PVOID, DWORD, DWORD = 0, LPCTSTR = NULL, ...  )  {}
inline  void __dbg_util__release_Dump( int, PVOID, DWORD, DWORD = 0, LPCTSTR = NULL, ...  )  {}
inline  void __dbg_util__release_DumpTable( PVOID, DWORD, DWORD, DWORD = 0, LPCTSTR = NULL, ...  )  {}
inline  void __dbg_util__release_DumpTable( int, PVOID, DWORD, DWORD, DWORD = 0, LPCTSTR = NULL, ...  )  {}

#define  SET_TRACE_BUFFER(dwSize)
#define  FLUSH_TRACE_BUFFER(nLevel)
#define  SET_MAX_TRACE_LEVEL(nLevel)
#define  SET_ACTIVE_TRACE_LEVEL(nLevel)
#define  SET_ACTIVE_TRACE_LEVELS(nCount, pLevels)
#define  TRACE			1 ? (void)0 : __dbg_util__release_Trace
#define  ELSE_TRACE		1 ? (void)0 : __dbg_util__release_Trace
#define  TRACENP		1 ? (void)0 : __dbg_util__release_Trace
#define  TRACENL		1 ? (void)0 : __dbg_util__release_Trace
#define  TRACERB		1 ? (void)0 : __dbg_util__release_Trace
#define  TRACENLP		1 ? (void)0 : __dbg_util__release_Trace
#define  TRACENPL		1 ? (void)0 : __dbg_util__release_Trace
#define  REMOVE_LAST_TRACE	1 ? (void)0 : __dbg_util__release_RemoveLastTrace
#define  TRACESTR		1 ? (void)0 : __dbg_util__release_Trace
#define  TRACESTRP		1 ? (void)0 : __dbg_util__release_Trace
#define  DUMP			1 ? (void)0 : __dbg_util__release_Dump
#define  DUMPNP			1 ? (void)0 : __dbg_util__release_Dump
#define  DUMPTBL		1 ? (void)0 : __dbg_util__release_DumpTable

#define  TRACE_ARGS				1 ? (void)0 : __dbg_util__release_Trace
#define  TRACE_ARGS_STRIPPED	1 ? (void)0 : __dbg_util__release_Trace

#define  TRACE_STACK	1 ? (void)0 : __dbg_util__release_Trace
#define  TRACE_STACKNP	1 ? (void)0 : __dbg_util__release_Trace

// Simple testing routines support
#define  SET_TESTS_SEPARATOR(strFmt)
#define  RESET_TEST_NUM()
#define  EXPECTED			1 ? (void)0 : __dbg_util__release_Trace
#define  RESULTED			1 ? (void)0 : __dbg_util__release_Trace
#define  RESULTEDNA			1 ? (void)0 : __dbg_util__release_Trace

#endif  // !defined(_DEBUG)  &&  !defined(RELEASE_TRACE)


#if defined(_DEBUG)
	#define  TRASSERT		__dbg_util__dbgTracer.SetTraceInfo(FileLineInfo, __dbg_util::Lf)->TrAssert
	#define	 VERIFY(expr)	_ASSERTE(expr)
#else
	inline  void __dbg_util__release_TrAssert( BOOL, LPCTSTR, ... )  {}
	#define  TRASSERT		1 ? (void)0 : __dbg_util__release_TrAssert
	#define	 VERIFY(expr)	((void)(expr))
#endif  // !_DEBUG

#define  ASSERT			_ASSERTE
#define  ASSERTEX		TRASSERT
#define  RESULTED_		RESULTEDNA

#pragma warning (default: 4005 4530)


#pragma once

#include "BugTrap.h"

#ifdef _UNICODE
#pragma comment(lib, "BugTrapU.lib")  // Link to Unicode DLL
#else
#pragma comment(lib, "BugTrap.lib")      // Link to ANSI DLL
#endif

/**
 * \ingroup NtlCore
 * \brief BugTrap을 이용하는 클래스 (http://www.codeproject.com/tools/BugTrap.asp)
 * 이 헤더만 Include하면 프로그램이 뻑날때 버그트랩이 실행된다.
 * 출력 메시지를 변경하기 위해서는 cpp 파일의 주석을 참조한다.
 * \date 2006-12-08
 * \author agebreak
 */
class CNtlBugTrap
{
public:
    CNtlBugTrap(void);    
};

static CNtlBugTrap g_dumpTrap; 
#include "precomp_ntlcore.h"
#include "NtlBugTrap.h"
#include "NtlDebug.h"
#include "new.h"

void InvalidParameterHandler(const wchar_t* expression,
                             const wchar_t* function,
                             const wchar_t* file,
                             unsigned int line,
                             uintptr_t pReserved)
{
    throw NTL_NEW std::exception("InvalidParameterHandler");
}

void PurecallHandler(void)
{
    throw NTL_NEW std::exception("PurecallHandler");
}

int NewHandler( size_t )
{
    NTL_ASSERTFAIL("new Handler Fail!");
    throw NTL_NEW std::exception("NewHandler");
}

CNtlBugTrap::CNtlBugTrap(void)
{
    // Setup bug trapper
    // 버그트랩을 사용할 각 프로그램에서 다시 설정하면 변경된다.
    BT_SetAppName(_T("BugTrap Report"));    
    //BT_SetSupportEMail(_T("agebreak@ntl-inc.com"));
    BT_SetFlags(BTF_DETAILEDMODE | BTF_EDITMAIL | BTF_ATTACHREPORT | BTF_SCREENCAPTURE | BTF_INTERCEPTSUEF);
	//BT_SetFlags(BTF_NONE);
    BT_SetSupportURL(_T("https://dboglobal.to"));

    // BugTrapServer ===========================================
    //BT_SetSupportServer(_T("localhost"), 9999);    

    // C 표준함수에 대한 잘못된 인자 사용
    _set_invalid_parameter_handler(InvalidParameterHandler);

    // 잘못된 순수 가상함수 호출
    _set_purecall_handler(PurecallHandler);

    // 너무 큰 메모리 할당 시 New 연산자의 실패
    _set_new_handler( NewHandler );
}

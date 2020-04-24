/* 
 * Querying processor features
 *
 * Copyright (c) 2001 Criterion Software Ltd.
 */

#include <string.h>

#include "batypes.h"
#include "barwtyp.h"
#include "badebug.h"

#include "cpuext.h"

#ifdef WIN32
#include <windows.h>
#endif

#define CPUID_MMX   (1 << 23)
#define CPUID_SSE   (1 << 25)
#define CPUID_SSE2  (1 << 26)

#define CPUID_EXT_3DNOW (1 << 31)

/**************************************************************************
 CPUHaveCPUID

 Does the processor support the CPUID function?

 On entry   : Nothing
 On exit    : TRUE if processor supports CPUID, FALSE otherwise
 */
static RwBool
CPUHaveCPUID(void)
{
    RwUInt32  cpuidOkay = 0;

    RWFUNCTION( RWSTRING( "CPUHaveCPUID" ) );

#if (!defined(__GNUC__))
    __asm
    {
        push    ebx

        pushf

        pop     eax
        mov     ebx, eax
        xor     eax, 0x00200000
        push    eax
        popf
        pushf
        pop     eax
        xor     eax, ebx
        je      cq_endex

        mov     cpuidOkay, 1

    cq_endex:
        push    ebx
        popf
        pop     ebx
    }
#endif /* (!defined(__GNUC__)) */

    RWRETURN( 0 != cpuidOkay );
}

/**
 * CPUGetFeatures Get CPU features flags
 *
 * Returns value of EDX
 */
static RwUInt32
CPUGetFeatures(void)
{
    RwUInt32    result = 0;

    RWFUNCTION(RWSTRING("CPUGetFeatures"));

#if (!defined(__GNUC__))
    __asm
    {
        mov     eax, 1
        xor     ebx, ebx
        xor     ecx, ecx
        xor     edx, edx
        _emit   0x0f;
        _emit   0xa2;
        mov     result, edx
    }
#endif /* (!defined(__GNUC__)) */

    RWRETURN(result);
}

/**
 * CPUHaveExtFeatures Does the CPU support extended features
 *
 * Returns value of EDX
 */
static RwBool
CPUHaveExtFeatures(void)
{
    RwBool  extFeaturesOkay = FALSE;

    RWFUNCTION(RWSTRING("CPUHaveExtFeatures"));

#if (!defined(__GNUC__))
    __asm
    {
        mov     eax, 0x80000000
        xor     ebx, ebx
        xor     ecx, ecx
        xor     edx, edx
        _emit   0x0f;
        _emit   0xa2;
        cmp     eax, 0x80000000
        jbe     NO_EXT_FEATURES

        mov     extFeaturesOkay, 1

    NO_EXT_FEATURES:
    }
#endif /* (!defined(__GNUC__)) */

    RWRETURN(extFeaturesOkay);
}

/**
 * CPUGetExtFeatures Get CPU extended features flags
 *
 * Returns value of EDX
 */
static RwUInt32
CPUGetExtFeatures(void)
{
    RwUInt32    result = 0;

    RWFUNCTION(RWSTRING("CPUGetExtFeatures"));

#if (!defined(__GNUC__))
    __asm
    {
        mov     eax, 0x80000001
        xor     ebx, ebx
        xor     ecx, ecx
        xor     edx, edx
        _emit   0x0f;
        _emit   0xa2;
        mov     result, edx
    }
#endif /* (!defined(__GNUC__)) */

    RWRETURN(result);
}

#if (!defined(__GNUC__))
/**
 * CPUGetProcessorFamilyModel Get CPU processor flags
 *
 * Returns value of EDX
 */
static void
CPUGetProcessorFamilyModel(RwUInt32 *family, RwUInt32 *model)
{
    RWFUNCTION(RWSTRING("CPUGetProcessorFamilyModel"));

    *family = 0;
    *model = 0;

    if (CPUHaveCPUID())
    {
        #if (!defined(__GNUC__))
        __asm
        {
            mov     eax, 0x80000000
            xor     ebx, ebx
            xor     ecx, ecx
            xor     edx, edx
            _emit   0x0f;
            _emit   0xa2;
            cmp     eax, 0x80000001
            jb      NO_EXT_FEATURES

            mov     eax, 0x80000001
            xor     ebx, ebx
            xor     ecx, ecx
            xor     edx, edx
            _emit   0x0f;
            _emit   0xa2;
            test    eax, eax
            jnz short GETFAMILYMODEL

NO_EXT_FEATURES:
            mov     eax, 1
            xor     ebx, ebx
            xor     ecx, ecx
            xor     edx, edx
            _emit   0x0f;
            _emit   0xa2;

GETFAMILYMODEL:
            mov     ebx, eax
            mov     esi, family
            mov     edi, model
            shr     eax, 8
            shr     ebx, 4
            and     eax, 0xf
            and     ebx, 0xf
            mov     [esi], eax
            mov     [edi], ebx
        }
        #endif /* (!defined(__GNUC__)) */
    }

    RWRETURNVOID();
}
#endif /* (!defined(__GNUC__)) */

/*
 * _rwCPUGetCPUName copies the cpu name to the buffer
 */
void
_rwCPUGetCPUName(RwChar *cpuName)
{
    RWFUNCTION(RWSTRING("_rwCPUGetCPUName"));

    cpuName[0] = 0;

    if (CPUHaveCPUID())
    {
#if (!defined(__GNUC__))
        RwBool  fakeCPUName = FALSE;

        __asm
        {
            mov     eax, 0x080000000    ; Check for support of extended functions.
            xor     ebx, ebx
            xor     ecx, ecx
            xor     edx, edx
            _emit   0x0f;
            _emit   0xa2;
            ; Check which extended functions can be called
            cmp     eax, 0x080000004    ; CPU Name string
            jb short no_ext_feature_name

            ; Query and save the CPU name string
            mov     edi, cpuName
            mov     eax, 0x080000002
            _emit   0x0f;
            _emit   0xa2;
            mov     [edi+0],eax
            mov     [edi+4],ebx
            mov     [edi+8],ecx
            mov     [edi+12],edx
            mov     eax, 0x080000003
            _emit   0x0f;
            _emit   0xa2;
            mov     [edi+16],eax
            mov     [edi+20],ebx
            mov     [edi+24],ecx
            mov     [edi+28],edx
            mov     eax, 0x080000004
            _emit   0x0f;
            _emit   0xa2;
            mov     [edi+32],eax
            mov     [edi+36],ebx
            mov     [edi+40],ecx
            mov     [edi+44],edx
            mov     [edi+48],0      ; just in case

            ; remove leading spaces
            xor     edx, edx
searchspaces:
            mov     al, [edi + edx]
            cmp     al, 0x20
            jnz short removespaces
            inc     edx
            jmp short searchspaces

removespaces:
            test    edx, edx
            jz short end_getname
            lea     esi, [edi + edx]
            mov     ecx, 49
            sub     ecx, edx
            rep     movsb
            jmp short end_getname

no_ext_feature_name:
            mov     eax, 0
            xor     ebx, ebx
            xor     ecx, ecx
            xor     edx, edx
            _emit   0x0f;
            _emit   0xa2;
            mov     eax, cpuName
            mov     [eax], ebx
            mov     [eax+ 4], edx
            mov     [eax + 8], ecx
            mov     [eax + 12], 0

            mov     fakeCPUName, 1

end_getname:
        }

        if (fakeCPUName)
        {
            RwUInt32 family;
            RwUInt32 model;

            CPUGetProcessorFamilyModel(&family, &model);

            if (memcmp(cpuName, "GenuineIntel", 12) == 0)
            {
                switch (family)
                {
                    case 4:
                        strcpy(cpuName, "Intel 486");
                        break;

                    case 5:
                        switch (model)
                        {
                            case 4:
                            case 8:
                                strcpy(cpuName, "Intel Pentium MMX");
                                break;

                            default:
                                strcpy(cpuName, "Intel Pentium");
                                break;
                        }
                        break;

                    case 6:
                        switch (model)
                        {
                            case 0:
                            case 1:
                                strcpy(cpuName, "Intel Pentium Pro");
                                break;

                            case 3:
                            case 5:
                                strcpy(cpuName, "Intel Pentium II");
                                break;

                            case 6:
                                strcpy(cpuName, "Intel Celeron");
                                break;

                            case 7:
                            case 8:
                                strcpy(cpuName, "Intel Pentium III");
                                break;

                            case 0xa:
                                strcpy(cpuName, "Intel Pentium III Xeon");
                                break;

                            default:
                                break;
                        }
                        break;

                    case 0xf:
                        strcpy(cpuName, "Intel Pentium 4");
                        break;

                    default:
                        break;
                }
            }
            else if (memcmp(cpuName, "AuthenticAMD", 12) == 0 ||
                    memcmp(cpuName, "AMD ISBETTER", 12) == 0 )
            {
                switch (family)
                {
                    case 4:
                        strcpy(cpuName, "AMD 486");
                        break;

                    case 5:
                        switch (model)
                        {
                            case 0:
                            case 1:
                            case 2:
                            case 3:
                                strcpy(cpuName, "AMD K5");
                                break;

                            case 4:
                            case 5:
                            case 6:
                            case 7:
                                strcpy(cpuName, "AMD K6");
                                break;

                            case 8:
                                strcpy(cpuName, "AMD K6 2");
                                break;

                            case 9:
                            case 0xd:
                                strcpy(cpuName, "AMD K6 III");
                                break;

                            default:
                                break;
                        }
                        break;

                    case 6: /* K7 Athlon */
                        switch (model)
                        {
                            case 1:
                            case 2:
                                strcpy(cpuName, "AMD Athlon");
                                break;

                            case 3:
                                strcpy(cpuName, "AMD Duron");
                                break;

                            case 4:
                                strcpy(cpuName, "AMD Athlon");
                                break;

                            default:
                                strcpy(cpuName, "AMD Athlon");
                                break;
                        }
                        break;

                    default:
                        break;
                }
            }
        }
        #endif /* (!defined(__GNUC__)) */
    }

    RWRETURNVOID();
}

/*
 * _rwCPUHaveMMX checks whether CPU supports MMX functions
 */
RwBool
_rwCPUHaveMMX(void)
{
    RwBool  result = FALSE;

    RWFUNCTION(RWSTRING("_rwCPUHaveMMX"));

    if (CPUHaveCPUID())
    {
        result = CPUGetFeatures();

        result = ((result & CPUID_MMX) != 0);
    }

    RWRETURN(result);
}

/*
 * _rwCPUHaveSSE checks whether CPU supports SSE functions
 */
RwBool
_rwCPUHaveSSE(void)
{
    RwBool  result = FALSE;

    RWFUNCTION(RWSTRING("_rwCPUHaveSSE"));

    if (CPUHaveCPUID())
    {
        result = CPUGetFeatures();

        result = ((result & (CPUID_SSE | CPUID_SSE2)) != 0);
    }

    RWRETURN(result);
}

/*
 * _rwCPUHaveSSE2 checks whether CPU supports SSE functions
 */
RwBool
_rwCPUHaveSSE2(void)
{
    RwBool  result = FALSE;

    RWFUNCTION(RWSTRING("_rwCPUHaveSSE2"));

    if (CPUHaveCPUID())
    {
        result = CPUGetFeatures();

        result = ((result & CPUID_SSE2) != 0);
    }

    RWRETURN(result);
}

/*
 * _rwCPUHave3DNow checks whether CPU supports 3DNow functions
 */
RwBool
_rwCPUHave3DNow(void)
{
    RwBool  result = FALSE;

    RWFUNCTION(RWSTRING("_rwCPUHave3DNow"));

    if (CPUHaveCPUID())
    {
        if (CPUHaveExtFeatures())
        {
            result = CPUGetExtFeatures();

            result = ((result & CPUID_EXT_3DNOW) != 0);
        }
    }

    RWRETURN(result);
}

/*
 * _rwGetWindowsVersion copies the operating system version name to the buffer
 */
#if defined(WIN32)
RwBool
_rwCPUGetOSName(RwChar *osName)
{
    OSVERSIONINFOEX osvi;
    RwBool osVersionInfoEx;

    RWFUNCTION(RWSTRING("_rwCPUGetWindowsVersion"));

    osName[0] = 0;

    /* Try calling GetVersionEx using the OSVERSIONINFOEX structure.
       If that fails, try using the OSVERSIONINFO structure. */

    memset(&osvi, 0, sizeof(OSVERSIONINFOEX));

    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    osVersionInfoEx = GetVersionEx((OSVERSIONINFO *)&osvi);
    if (osVersionInfoEx == FALSE)
    {
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        if (GetVersionEx((OSVERSIONINFO *)&osvi) == FALSE)
        {
            RWRETURN(FALSE);
        }
    }

    switch (osvi.dwPlatformId)
    {
        /* Test for the Windows NT product family. */
        case VER_PLATFORM_WIN32_NT:
            /* Test for the specific product family. */
            if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
            {
                osName += rwsprintf(osName, "Microsoft Windows .NET Server 2003 family, ");
            }
            else if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
            {
                osName += rwsprintf(osName, "Microsoft Windows XP ");
            }
            else if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
            {
                osName += rwsprintf(osName, "Microsoft Windows 2000 ");
            }
            else if ( osvi.dwMajorVersion <= 4 )
            {
                osName += rwsprintf(osName, "Microsoft Windows NT ");
            }

            /* Test for specific product on Windows NT 4.0 SP6 and later. */
            #if defined(VER_NT_WORKSTATION) && defined(VER_NT_SERVER)
            if (osVersionInfoEx)
            {
                /* Test for the workstation type. */
                if ( osvi.wProductType == VER_NT_WORKSTATION )
                {
                    if ( osvi.dwMajorVersion == 4 )
                    {
                        osName += rwsprintf(osName, "Workstation 4.0 " );
                    }
                    else if ( osvi.wSuiteMask & VER_SUITE_PERSONAL )
                    {
                        osName += rwsprintf(osName, "Home Edition " );
                    }
                    else
                    {
                        osName += rwsprintf(osName, "Professional " );
                    }
                }
                /* Test for the server type. */
                else if ( osvi.wProductType == VER_NT_SERVER )
                {
                    if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
                    {
                        if ( osvi.wSuiteMask & VER_SUITE_DATACENTER )
                        {
                            osName += rwsprintf(osName, "Datacenter Edition " );
                        }
                        else if ( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                        {
                            osName += rwsprintf(osName, "Enterprise Edition " );
                        }
#if (!defined(__GNUC__))       
                        else if ( osvi.wSuiteMask == VER_SUITE_BLADE )
                        {
                            osName += rwsprintf(osName, "Web Edition " );
                        }
#endif /* (!defined(__GNUC__)) */
                        else
                        {
                            osName += rwsprintf(osName, "Standard Edition " );
                        }
                    }
                    else if( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
                    {
                        if ( osvi.wSuiteMask & VER_SUITE_DATACENTER )
                        {
                            osName += rwsprintf(osName, "Datacenter Server " );
                        }
                        else if ( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                        {
                            osName += rwsprintf(osName, "Advanced Server " );
                        }
                        else
                        {
                            osName += rwsprintf(osName, "Server " );
                        }
                    }
                    else  /* Windows NT 4.0 */
                    {
                        if ( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                        {
                            osName += rwsprintf(osName, "Server 4.0, Enterprise Edition " );
                        }
                        else
                        {
                            osName += rwsprintf(osName, "Server 4.0 " );
                        }
                    }
                }
            }
            else
            #endif /* defined(VER_NT_WORKSTATION) && defined(VER_NT_SERVER) */
            {
                if ( osvi.dwMajorVersion <= 4 )
                {
                    osName += rwsprintf(osName, "%d.%d ", osvi.dwMajorVersion, osvi.dwMinorVersion);
                }
            }

            /* Test Service Pack info. */
            if (osVersionInfoEx)
            {
                if (osvi.wServicePackMajor)
                {
                    if (osvi.wServicePackMinor)
                    {
                        osName += rwsprintf(osName, "Service Pack %d.%d", osvi.wServicePackMajor, osvi.wServicePackMinor);
                    }
                    else
                    {
                        osName += rwsprintf(osName, "Service Pack %d", osvi.wServicePackMajor);
                    }
                }
            }
            break;

        /* Test for the Windows 9x product family. */
        case VER_PLATFORM_WIN32_WINDOWS:
            if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
            {
                osName += rwsprintf(osName, "Microsoft Windows 95 ");
                if ( osvi.szCSDVersion[1] == 'C' || osvi.szCSDVersion[1] == 'B' )
                {
                    osName += rwsprintf(osName, "OSR2 " );
                }
            }
            else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
            {
                osName += rwsprintf(osName, "Microsoft Windows 98 ");
                if ( osvi.szCSDVersion[1] == 'A' )
                {
                    osName += rwsprintf(osName, "SE " );
                }
            }
            else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
            {
                osName += rwsprintf(osName, "Microsoft Windows Millennium Edition");
            } 
            break;
    }

    RWRETURN(TRUE);
}
#endif /* defined(WIN32) */

/*
 * String handling
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 */

/****************************************************************************
 Includes
 */

#ifndef RWUNICODE
#include <stdio.h>
#endif /* RWUNICODE */
#include <string.h>

#include "batypes.h"
#include "balibtyp.h"
#include "babinary.h"

#include "rwstring.h"


/****************************************************************************
 Local Types
 */

typedef union RwPtrChar RwPtrChar;
union RwPtrChar
{
    RwChar       *ptrChar;
    const RwChar *constptrChar;
};

/****************************************************************************
 Local (static) Prototypes
 */

/****************************************************************************
 Local Defines
 */

/****************************************************************************
 Globals (across program)
 */

 /****************************************************************************
 Local (static) Globals
 */
static const RwChar nullString[] = RWSTRING("");

/* Implement some string functions for platforms which don't have them,
 * or they just plain don't work.
 */
#ifndef RWUNICODE

static int
StrICmp(const RwChar * s1, const RwChar * s2)
{
    RWFUNCTION(RWSTRING("StrICmp"));

    if (s1 && s2)
    {
        RwChar          c1, c2;

        do
        {
            c1 = *s1;
            c2 = *s2;

            /* Convert to lower case for comparison */
            if ((c1 >= 'A') && (c1 <= 'Z'))
            {
                c1 += 'a' - 'A';
            }

            if ((c2 >= 'A') && (c2 <= 'Z'))
            {
                c2 += 'a' - 'A';
            }

            /* Compare - anything beats NUL!!! */
            if (c1 != c2)
            {
                RWRETURN(c1 - c2);
            }

            /* Next char */
            s1++;
            s2++;
        }
        while (c1 && c2);

        if (c1 != c2)
        {
            RWRETURN(c1 - c2);
        }
    }

    /* Same or not got two strings */
    RWRETURN(0);
}

static RwChar  *
StrUpr(RwChar * s)
{
    RWFUNCTION(RWSTRING("StrUpr"));

    if (s)
    {
        RwChar         *p = s;

        while (*p)
        {
            RwChar          c = *p;

            if ((c >= 'a') && (c <= 'z'))
            {
                c += 'A' - 'a';
                *p = c;
            }
            p++;
        }
    }

    RWRETURN(s);
}

static RwChar  *
StrLwr(RwChar * s)
{
    RWFUNCTION(RWSTRING("StrLwr"));

    if (s)
    {
        RwChar         *p = s;

        while (*p)
        {
            RwChar          c = *p;

            if ((c >= 'A') && (c <= 'Z'))
            {
                c -= 'A' - 'a';
                *p = c;
            }
            p++;
        }
    }

    RWRETURN(s);
}

static RwChar *
StrChr(const RwChar *s, int c)
{
    RwPtrChar    result;
    const RwChar match = (RwChar) c;
    RwChar       ch;

    RWFUNCTION(RWSTRING("StrChr"));
    RWASSERT(NULL != s);

    result.constptrChar = (const char *)NULL;

    /*
     * For a parallized/optimized version of strchr(), see
     *    glibc-2.1.2/sysdeps/generic/strchr.c
     * in
     *    ftp://prep.ai.mit.edu/pub/gnu/glibc/glibc-2_1_2.tar.gz
     */

    do
    {
        ch = *s;
        if (ch == match)
        {
            result.constptrChar = s;
            break;
        }
        s++;
    }   while (ch);

    RWRETURN(result.ptrChar);
}

static RwChar  *
StrRChr(const RwChar * s, int c)
{
    RwPtrChar    result;
    const RwChar match = (RwChar) c;
    RwChar       ch;

    RWFUNCTION(RWSTRING("StrRChr"));
    RWASSERT(NULL != s);

    result.constptrChar = (const char *)NULL;

    /*
     * For a parallized/optimized version of strrchr(), see
     *    glibc-2.1.2/sysdeps/generic/strrchr.c
     * in
     *    ftp://prep.ai.mit.edu/pub/gnu/glibc/glibc-2_1_2.tar.gz
     */

    do
    {
        ch = *s;
        if (ch == match)
        {
            result.constptrChar = s;
        }
        s++;
    }   while (ch);


    RWRETURN(result.ptrChar);
}

#endif /* !RWUNICODE */

/****************************************************************************
 _rwStringOpen

 On entry   : None
 On exit    : TRUE on success
 */

RwBool
_rwStringOpen(void)
{
    RWFUNCTION(RWSTRING("_rwStringOpen"));

#ifdef RWUNICODE
    /* Install UNICODE string functions */
    RWSRCGLOBAL(stringFuncs).vecSprintf = swprintf;
    RWSRCGLOBAL(stringFuncs).vecVsprintf = vswprintf;
    RWSRCGLOBAL(stringFuncs).vecStrcpy = wcscpy;
    RWSRCGLOBAL(stringFuncs).vecStrncpy = wcsncpy;
    RWSRCGLOBAL(stringFuncs).vecStrcat = wcscat;
    RWSRCGLOBAL(stringFuncs).vecStrncat = wcsncat;

    /* Next two lines generate warnings
     * because the docs don't match the header files */
    /* The docs say the character param is an int,
     * the header files have a wchar_t */
    RWSRCGLOBAL(stringFuncs).vecStrrchr = wcsrchr;
    RWSRCGLOBAL(stringFuncs).vecStrchr = wcschr;

    RWSRCGLOBAL(stringFuncs).vecStrstr = wcsstr;
    RWSRCGLOBAL(stringFuncs).vecStrcmp = wcscmp;
    RWSRCGLOBAL(stringFuncs).vecStrncmp = wcsncmp;
    RWSRCGLOBAL(stringFuncs).vecStricmp = wcsicmp;
    RWSRCGLOBAL(stringFuncs).vecStrlen = wcslen;
    RWSRCGLOBAL(stringFuncs).vecStrupr = wcsupr;
    RWSRCGLOBAL(stringFuncs).vecStrlwr = wcslwr;
    RWSRCGLOBAL(stringFuncs).vecStrtok = wcstok;
    RWSRCGLOBAL(stringFuncs).vecSscanf = swscanf;
#else /* UNICODE */
    /* Install ANSI char string system */
    RWSRCGLOBAL(stringFuncs).vecSprintf = sprintf;
    RWSRCGLOBAL(stringFuncs).vecVsprintf = vsprintf;
    RWSRCGLOBAL(stringFuncs).vecStrcpy = strcpy;
    RWSRCGLOBAL(stringFuncs).vecStrncpy = strncpy;
    RWSRCGLOBAL(stringFuncs).vecStrcat = strcat;
    RWSRCGLOBAL(stringFuncs).vecStrncat = strncat;
    RWSRCGLOBAL(stringFuncs).vecStrrchr = StrRChr;
    RWSRCGLOBAL(stringFuncs).vecStrchr = StrChr;
    RWSRCGLOBAL(stringFuncs).vecStrstr = strstr;
    RWSRCGLOBAL(stringFuncs).vecStrcmp = strcmp;
    RWSRCGLOBAL(stringFuncs).vecStrncmp = strncmp;
    RWSRCGLOBAL(stringFuncs).vecStricmp = StrICmp;
    RWSRCGLOBAL(stringFuncs).vecStrlen = strlen;
    RWSRCGLOBAL(stringFuncs).vecStrupr = StrUpr;
    RWSRCGLOBAL(stringFuncs).vecStrlwr = StrLwr;
    RWSRCGLOBAL(stringFuncs).vecStrtok = strtok;
//#ifdef PSP_DRVMODEL_H
//    RWSRCGLOBAL(stringFuncs).vecSscanf = NULL;
//#else /* PSP_DRVMODEL_H */
    RWSRCGLOBAL(stringFuncs).vecSscanf = sscanf;
//#endif /* PSP_DRVMODEL_H */
#endif /* UNICODE */

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwFileSystemClose

 On entry   : void
 On exit    : void
 */

void
_rwStringClose(void)
{
    RWFUNCTION(RWSTRING("_rwStringClose"));

    RWRETURNVOID();
}

/****************************************************************************
 _rwStringDestroy

 On entry   : String to destroy
 On exit    : TRUE on success
 */

RwBool
_rwStringDestroy(RwChar * string)
{
    RWFUNCTION(RWSTRING("_rwStringDestroy"));
    RWASSERT(string);
    RwFree(string);
    RWRETURN(TRUE);
}

/****************************************************************************
 _rwStringStreamGetSize

 On entry   :
 On exit    : Size of Binary String
 */

RwUInt32
_rwStringStreamGetSize(const RwChar * string)
{
    RWFUNCTION(RWSTRING("_rwStringStreamGetSize"));

    /* Provide an empty string if we are writing a NULL string
     * - aren't we kind!!! */
    if (!string)
    {
        string = nullString;
    }

    /* +1 for the terminator, ((+3) &~3) to
     * round up to nearest 4 byte boundary */
    RWRETURN((((rwstrlen(string) + 1) * sizeof(RwChar)) + 3) & ~3);
}

/****************************************************************************
 _rwStringStreamWrite

 On entry   : Stream to write to
 On exit    :
 */
const RwChar       *
_rwStringStreamWrite(const RwChar * string, RwStream * stream)
{
    RwUInt32            stringSize;

    RWFUNCTION(RWSTRING("_rwStringStreamWrite"));
    RWASSERT(stream);

    /* Provide an empty string if we are writing a NULL string - aren't we kind!!! */
    if (!string)
    {
        string = nullString;
    }

    /* Get the binary size */
    stringSize = _rwStringStreamGetSize(string);

    /* Write out the correct chunk header */
#ifdef RWUNICODE
    if (!RwStreamWriteChunkHeader(stream, rwID_UNICODESTRING, stringSize))
    {
        RWRETURN(NULL);
    }
#else /* RWUNICODE */
    if (!RwStreamWriteChunkHeader(stream, rwID_STRING, stringSize))
    {
        RWRETURN((const char *)NULL);
    }
#endif /* RWUNICODE */

    if (!RwStreamWrite(stream, string, stringSize))
    {
        RWRETURN((const char *)NULL);
    }

    RWRETURN(string);
}

/****************************************************************************
 StringStreamRead

 This reads a non unicode string, converting to unicode if necessary

 On entry   : String to fill, or NULL if malloc required
            : Stream to read from
            : Length of the string to read
 On exit    : String created
 */

static RwChar      *
StringStreamRead(RwChar *nativeString, RwStream * stream, RwUInt32 length)
{
    /* NOTE: This is one of the rare places we really do need to use char.
     *     : The binary standard defines the things in the file as being chars.
     */
    RWALIGN(char multiByteString[64], 64);
    RwChar *baseString;
    RwBool  mallocced;

    RWFUNCTION(RWSTRING("StringStreamRead"));
    RWASSERT(stream);

    /* The length is the length in bytes in the stream.  However, since
     * we are reading a non unicode string, it is also the num of chars.
     * NOTE The malloc has been put in to support the RpAnim plugin which
     * sneakily calls this function via StringStreamFindAndRead(), and
     * which for the time being does not support fixed length strings.
     */

    if (nativeString == NULL)
    {
        nativeString = (RwChar *) RwMalloc(length * sizeof(RwChar),
                                 rwID_STRING | rwMEMHINTDUR_EVENT);
        if (!nativeString)
        {
            RWERROR((E_RW_NOMEM, length * sizeof(RwChar)));
            RWRETURN((char *)NULL);
        }
        mallocced = TRUE;
    }

    /* We make the assumption that few strings will be greater then 64 bytes,
     * so these are treated as a special case
     */
    baseString = nativeString;
    while(length>0)
    {
        RwUInt32  bytesToRead = (length>64) ? 64 : length;
        RwUInt32  i;

        if (RwStreamRead(stream, multiByteString, bytesToRead) != bytesToRead)
        {
            RWRETURN((char *)NULL);
        }

        /* Reduce by the amount we read */
        length -= bytesToRead;

        /* Now we need to convert into native string format
         * - this might be chars, it might be wchar_t */
        for (i=0; i<bytesToRead; i++)
        {
            /* We just cast it to an RwChar */
            baseString[i] = (RwChar) multiByteString[i];
        }
        baseString += bytesToRead;
    }

    RWRETURN(nativeString);
}

/****************************************************************************
 UnicodeStringStreamRead

 This reads a non unicode string, converting to unicode if necessary

 On entry   : String to fill, or NULL if malloc required
            : Stream to read from
            : Length of the string to read
 On exit    : String created
 */

static RwChar      *
UnicodeStringStreamRead(RwChar *nativeString, RwStream * stream, RwUInt32 length)
{
    /* NOTE: This is one of the rare places we really do need to use unsigned shorts.
     *     : The binary standard defines the things in the file as being unsigned shorts.
     */
    RWALIGN(unsigned short uniCodeString[64], 64);
    RwChar            *baseString;
    RwBool             mallocced = FALSE;

    RWFUNCTION(RWSTRING("UnicodeStringStreamRead"));
    RWASSERT(stream);

    /* The length is the length in bytes in the stream.  However, since
     * we are reading a unicode string, it is also the num of chars*2.
     * NOTE The malloc has been put in to support the RpAnim plugin which
     * sneakily calls this function via StringStreamFindAndRead(), and
     * which for the time being does not support fixed length strings.
     */
    if (nativeString == NULL)
    {
        nativeString = (RwChar *) RwMalloc(length,
            rwID_STRING | rwMEMHINTDUR_EVENT);
        if (!nativeString)
        {
            RWERROR((E_RW_NOMEM, length));
            RWRETURN((char *)NULL);
        }
        mallocced = TRUE;
    }

    /* We make the assumption that few strings will be greater then 64 bytes,
     * so these are treated as a special case
     */
    baseString = nativeString;
    while(length>0)
    {
        RwUInt32  bytesToRead = (length>64*2) ? 64*2 : length;
        RwUInt32  i,CharCount;

        if (RwStreamRead(stream, uniCodeString, bytesToRead) != bytesToRead)
        {
            if (mallocced)
            {
                RwFree(nativeString);
            }
            RWRETURN((char *)NULL);
        }

        /* Reduce by the amount we read */
        length -= bytesToRead;

        /* Now we need to convert into native string format
         * - this might be chars, it might be wchar_t */
        CharCount = bytesToRead >> 1;
        for (i=0; i<CharCount; i++)
        {
            /* We just cast it to an RwChar */
            baseString[i] = (RwChar) uniCodeString[i];
        }
        baseString += CharCount;
    }

    RWRETURN(nativeString);
}

/****************************************************************************
 _rwStringStreamFindAndRead

 On entry   : String to destroy
 On exit    : TRUE on success
 */

RwChar             *
_rwStringStreamFindAndRead(RwChar *string, RwStream * stream)
{
    RwUInt32            type, length, version;
    RWFUNCTION(RWSTRING("_rwStringStreamFindAndRead"));
    RWASSERT(stream);

    while (_rwStreamReadChunkHeader(stream, &type, &length, &version, (RwUInt32 *)NULL))
	{
        const RwBool valid = ( (version >= rwLIBRARYBASEVERSION) &&
                               (version <= rwLIBRARYCURRENTVERSION) );

        /* Reject invalid versions */
        if (!valid)
        {
            RWERROR((E_RW_BADVERSION));
            RWRETURN((char *)NULL);
        }


        if (type == rwID_STRING)
        {
            RWRETURN(StringStreamRead(string, stream, length));
        }
        else if (type == rwID_UNICODESTRING)
        {
            RWRETURN(UnicodeStringStreamRead(string, stream, length));
        }
		else
		{
			RWRETURN((char *)NULL);
		}

        if (!RwStreamSkip(stream, length))
        {
            RWRETURN((char *)NULL);
        }
    };

    RWRETURN((char *)NULL);
}


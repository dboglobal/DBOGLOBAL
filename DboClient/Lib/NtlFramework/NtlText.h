/*****************************************************************************
 *
 * File			: NtlText.h
 * Author		: HongHoDong
 * Copyright	: (주)NTL
 * Date			: 2005. 7. 14	
 * Abstract		: NtlText
 *****************************************************************************
 * Desc         : Renderware 화면에 출력하기 위한 
 *
 *****************************************************************************/
#ifndef __NTL_TEXT__
#define __NTL_TEXT__

#if (!defined(RsSprintf))
#define RsSprintf rwsprintf
#endif /* (!defined(RsSprintf)) */


enum RsPrintPos
{
    rsPRINTPOSMIDDLE    = 0,

    rsPRINTPOSLEFT   = 1,
    rsPRINTPOSRIGHT  = 2,
    rsPRINTPOSTOP    = 4,
    rsPRINTPOSBOTTOM = 8,

    rsPRINTPOSTOPLEFT     = rsPRINTPOSTOP    | rsPRINTPOSLEFT,
    rsPRINTPOSTOPRIGHT    = rsPRINTPOSTOP    | rsPRINTPOSRIGHT,
    rsPRINTPOSBOTTOMLEFT  = rsPRINTPOSBOTTOM | rsPRINTPOSLEFT,
    rsPRINTPOSBOTTOMRIGHT = rsPRINTPOSBOTTOM | rsPRINTPOSRIGHT,

    rsPRINTPOSFORCENUMSIZEINT = 0x7FFFFFFF
};

/* Screen margins in charset width/height units, used w/ RsPrintPos */
enum RsPrintMargin
{
    rsPRINTMARGINMIDDLE = 0,

#if ((defined(SKY2_DRVMODEL_H)) || (defined(GCN_DRVMODEL_H)) || (defined(XBOX_DRVMODEL_H)))
    rsPRINTMARGINTOP    = 2,
    rsPRINTMARGINBOTTOM = 4,
    rsPRINTMARGINLEFT   = 5,
    rsPRINTMARGINRIGHT  = 5,
#else /* ((defined(SKY2_DRVMODEL_H)) || (defined(GCN_DRVMODEL_H)) || (defined(XBOX_DRVMODEL_H)) */
    rsPRINTMARGINTOP    = 1,
    rsPRINTMARGINBOTTOM = 1,
    rsPRINTMARGINLEFT   = 1,
    rsPRINTMARGINRIGHT  = 1,
#endif /* ((defined(SKY2_DRVMODEL_H)) || (defined(GCN_DRVMODEL_H)) || (defined(XBOX_DRVMODEL_H)) */
    rsPRINTMARGINFORCENUMSIZEINT = 0x7FFFFFFF
};

#if (defined(RWDEBUG))
#if (!defined(RsAssert))
#include <assert.h>
#define RsAssert(_condition) assert(_condition)
#endif /* (!defined(RsAssert)) */

#define RSASSERT(_condition)                      \
do                                                \
{                                                 \
    if (!(_condition))                            \
    {                                             \
        _rwDebugSendMessage(rwDEBUGMESSAGE,       \
                            RWSTRING(__FILE__),   \
                            __LINE__,             \
                            "Assertion failed:",  \
                            #_condition);         \
    }                                             \
    RsAssert((_condition));                       \
}                                                 \
while (0)

#endif /* (defined(RWDEBUG)) */

#if (!defined(RSASSERT))
#define RSASSERT(_condition) /* No-op */
#endif /* (!defined(RSASSERT)) */

RtCharset*	RsCharsetPrint(RtCharset *charSet, const RwChar *string,RwInt32 x, RwInt32 y, RsPrintPos pos);
RtCharset*	RsCharsetPrint(RtCharset *charSet, const RwChar *string,RwInt32 x, RwInt32 y);

RwChar*		RsPathnameCreate(const RwChar *srcBuffer);
void		RsPathnameDestroy(RwChar *pBuffer);

#endif
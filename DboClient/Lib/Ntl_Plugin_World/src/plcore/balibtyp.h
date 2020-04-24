/***************************************************************************
 *                                                                         *
 * Module  : balibtyp.h                                                    *
 *                                                                         *
 * Purpose : Macros/types only accessable from the library                 *
 *           This should be included directly after batypes.h in all       *
 *           modules which are part of the library (as opposed to the      *
 *           device.                                                       *
 *                                                                         *
 **************************************************************************/

#ifndef RWLIBTYP_H
#define RWLIBTYP_H

/****************************************************************************
 Includes
 */

#include "badebug.h"
#include "batkreg.h"
#include "badevice.h"
#include "bamemory.h"
#include "rwstring.h"
#include "drvmodel.h"

/****************************************************************************
 Macros
 */

/* RWPUBLIC */
/* Finding MSBs */

#define RWBYTEFINDMSB(a) \
   (_rwMsbBit[(a)]-1)

#define RWWORDFINDMSB(a) \
   (((a)&0xff00)?RWBYTEFINDMSB((a)>>8)+8: RWBYTEFINDMSB(a))

#define RWLONGFINDMSB(a) \
   (((a)&0xffff0000UL)?RWWORDFINDMSB((a)>>16)+16: RWWORDFINDMSB(a))

/****************************************************************************
 Defines
 */

/* macros used to access plugin data in objects */
#define RWPLUGINOFFSET(_type, _base, _offset)                   \
   ((_type *)((RwUInt8 *)(_base) + (_offset)))

#define RWPLUGINOFFSETCONST(_type, _base, _offset)              \
   ((const _type *)((const RwUInt8 *)(_base) + (_offset)))

/* macro used to access global data structure (the root type is RwGlobals) */
#define RWSRCGLOBAL(variable) \
   (((RwGlobals *)RwEngineInstance)->variable)

#define RWASSERTISTYPE(_f, _t) \
   RWASSERT((((const RwObject *)(_f))->type)==(_t))

/****************************************************************************
 Global Types
 */

enum RwEngineStatus
{
    rwENGINESTATUSIDLE = 0,                     /* This needs to be zero */
    rwENGINESTATUSINITED = 1,
    rwENGINESTATUSOPENED = 2,
    rwENGINESTATUSSTARTED = 3,
    rwENGINESTATUSFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RwEngineStatus RwEngineStatus;

typedef struct RwGlobals RwGlobals;

#if (!defined(DOXYGEN))
struct RwGlobals
{
#ifdef RWDEBUG
        RwDebugHandler      debugFunction;          /* debug string handler */
        RwInt32             debugStackDepth;        /* current depth of function stack */
        RwBool              debugTrace;             /* is function tracing enabled */
#endif

        /* Current entities */
        void                *curCamera;             /* Current camera */
        void                *curWorld;              /* Current World */

        /* Checking counters */
        RwUInt16            renderFrame;            /* Camera display count */
        RwUInt16            lightFrame;             /* Used to ensure each light is applied only once. */
        RwUInt16            pad[2];                 /* Longword align it again */

        /* For the currently accessed device */
        RwDevice            dOpenDevice;

        /* Standard renderers and functions */
        RwStandardFunc      stdFunc[rwSTANDARDNUMOFSTANDARD];

        /* All of the frames which have been updated */
        RwLinkList          dirtyFrameList;

        /* The string functions */
        RwStringFunctions   stringFuncs;

        /* The memory allocation functions */
        RwMemoryFunctions   memoryFuncs;
#ifdef RWDEBUG
        RwBool              freeListExtraDebug;
#endif /* RWDEBUG */

        /* virtual memory alloc/free functions */
        RwMemoryAllocFn         memoryAlloc;
        RwMemoryFreeFn          memoryFree;

        RwMetrics           *metrics;

        /* Current engine status */
        RwEngineStatus      engineStatus;

        /* Resource arena init size. */
        RwUInt32            resArenaInitSize;
};

typedef struct RwModuleInfo RwModuleInfo;
struct RwModuleInfo
{
        RwInt32     globalsOffset;
        RwInt32     numInstances;
};
#endif /* (!defined(DOXYGEN)) */



/****************************************************************************
 Program wide globals
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

#ifdef RWGLOBALSIZE
extern RwUInt32     ourGlobals[RWGLOBALSIZE / sizeof(RwUInt32)];
#define RwEngineInstance ourGlobals
#else /* RWGLOBALSIZE */

#ifdef _RWDLL
__declspec(dllimport) extern void         *RwEngineInstance;
#else
extern void         *RwEngineInstance;
#endif

#endif /* RWGLOBALSIZE */

#if defined(_RWDLL)
__declspec(dllimport) extern RwInt8 _rwMsbBit[];
#else /* defined(_RWDLL) */
extern RwInt8 _rwMsbBit[];
#endif /* defined(_RWDLL) */

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

/* RWPUBLICEND */

/****************************************************************************
 Function prototypes
 */

#endif /* RWLIBTYP_H */

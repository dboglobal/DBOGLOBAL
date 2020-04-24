/***************************************************************************
 *                                                                         *
 * Module  : barwtyp.h                                                     *
 *                                                                         *
 * Purpose : Macros/types only accessable from the RwenderWare device      *
 *           This should be included directly after batypes.h in all       *
 *           modules which are part of the library (as opposed to the      *
 *           device.                                                       *
 *                                                                         *
 **************************************************************************/

#ifndef RWRWTYP_H
#define RWRWTYP_H

/****************************************************************************
 Includes
 */

#include "bacamera.h"
#include "badevice.h"
#include "bamemory.h"

/****************************************************************************
 Macros
 */

/****************************************************************************
 Defines
 */
#ifndef __ELATE__
#ifndef ZMARGIN                 /* Buffer around dynamic depth range */
#define ZMARGIN 0x00000100UL    /* Floats only have 24 bit mantissa */
#endif /* ZMARGIN */

#ifndef ZINF                    /* Smallest number in the range buffer */
#define ZINF   ((ZMARGIN) << ((sizeof(long)-sizeof(RwRangel))<<3))
#endif /* ZINF */

#ifndef ZSUP                    /* Largest number in the range buffer */
#define ZSUP ((long)(0x80000000UL - (ZMARGIN)))
#endif /*  ZSUP  */

#define rwGRADIENTSIZE 32
#endif /* ELATE */

/* Memory functions */
#define RwDriverMalloc   dgGGlobals.memFuncs->rwmalloc
#define RwDriverFree     dgGGlobals.memFuncs->rwfree
#define RwDriverCalloc   dgGGlobals.memFuncs->rwcalloc
#define RwDriverRealloc   dgGGlobals.memFuncs->rwrealloc

/****************************************************************************
 Global Types across device
 */

/* Globals used across this device */
typedef struct RwRwDeviceGlobals RwRwDeviceGlobals;
struct RwRwDeviceGlobals
{
    /* Current camera */
    RwCamera           *curCamera;

    /* Memory allocators */
    RwMemoryFunctions  *memFuncs;
};



typedef struct RwStandard RwStandard;
struct RwStandard
{
    RwInt32             nStandard;
    RwStandardFunc      fpStandard;
};


/****************************************************************************
 Globals across the device
 */

#ifdef    __cplusplus
extern          "C"
{
#endif         /* __cplusplus */

extern RwRwDeviceGlobals dgGGlobals;

#ifdef    __cplusplus
}
#endif         /* __cplusplus */

/****************************************************************************
 Function prototypes
 */

#include "barwasmm.h"

#endif /* RWRWTYP_H */

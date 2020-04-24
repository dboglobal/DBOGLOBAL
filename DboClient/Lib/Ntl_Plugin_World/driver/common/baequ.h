
/***************************************************************************
 *                                                                         *
 * Module  : baequ.h                                                       *
 *                                                                         *
 * Purpose : Device specific equates                                       *
 *                                                                         *
 **************************************************************************/

#ifndef BAEQU_H
#define BAEQU_H

/****************************************************************************
 Includes
 */

#include <stddef.h>

#include "batypes.h"
#include "bamatrix.h"
#include "bacolor.h"
#include "batextur.h"
#include "bacamera.h"
#include "bapipe.h"
#include "baresour.h"
#include "ostypes.h"
#include "drvmodel.h"

/* Get us the sky stuff. 
 * Because of the slightly hacky way of getting some world stuff,
 * the core stuff is not included and this causes rpworld problems 
 * because of the dependency.
 * At present it is RxPS2DMASessionRecord, 
 * in rpworld and needs RxSkyTransTypeFlags
 * which is in rwcore.h
 */
#if (defined(SKY))
#include "skyinst.h"
#endif /* (defined(SKY)) */


/****************************************************************************
 Defines
 */

/* ANSI C defines the offsetof(type,member) macro; should be in <stddef.h> */

/* If not, fall back to this: */
#if (!defined(offsetof))
#define offsetof(type, member) \
    ((size_t)((char *)&((type *) 0)->member - (char *)((type *) 0)))
#endif /* (!defined(offsetof)) */

#ifdef    __cplusplus
extern              "C"
{
#endif                          /* __cplusplus */

/* RwMatrix */
    extern int          mat_mxx;
    extern int          mat_myx;
    extern int          mat_mzx;
    extern int          mat_mxy;
    extern int          mat_myy;
    extern int          mat_mzy;
    extern int          mat_mxz;
    extern int          mat_myz;
    extern int          mat_mzz;
    extern int          mat_mwz;
    extern int          mat_mwy;
    extern int          mat_mwx;

/* RwV2d */
    extern int          _rwV2d_nX;
    extern int          _rwV2d_nY;
    extern int          sizeof_rwV2d;

/* RwV3d */
    extern int          _rwV3d_nX;
    extern int          _rwV3d_nY;
    extern int          _rwV3d_nZ;
    extern int          sizeof_rwV3d;

/* RwRaster */
    extern int          _rwRaster_cpPixels;
    extern int          _rwRaster_nWidth;
    extern int          _rwRaster_nHeight;
    extern int          _rwRaster_nDepth;
    extern int          _rwRaster_nStride;
    extern int          _rwRaster_cType;
    extern int          _rwRaster_cFlags;
    extern int          sizeof_rwRaster;

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#endif                          /* BAEQU_H */

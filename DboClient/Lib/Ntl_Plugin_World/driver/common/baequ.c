
/***************************************************************************
 *                                                                         *
 * Module  : baequ.c                                                       *
 *                                                                         *
 * Purpose : Device specific equates                                       *
 *                                                                         *
 **************************************************************************/

#include "baequ.h"

/****************************************************************************
 Defines
 */

/****************************************************************************
 Local (Static) Prototypes
 */

/****************************************************************************
 Globals (across program)
 */

/****************************************************************************
 Local (static) Globals
 */

/****************************************************************************
 Functions
 */

/* RwMatrix */
int                 mat_mxx = offsetof(RwMatrix, right.x);
int                 mat_myx = offsetof(RwMatrix, up.x);
int                 mat_mzx = offsetof(RwMatrix, at.x);
int                 mat_mxy = offsetof(RwMatrix, right.y);
int                 mat_myy = offsetof(RwMatrix, up.y);
int                 mat_mzy = offsetof(RwMatrix, at.y);
int                 mat_mxz = offsetof(RwMatrix, right.z);
int                 mat_myz = offsetof(RwMatrix, up.z);
int                 mat_mzz = offsetof(RwMatrix, at.z);
int                 mat_mwz = offsetof(RwMatrix, pos.z);
int                 mat_mwy = offsetof(RwMatrix, pos.y);
int                 mat_mwx = offsetof(RwMatrix, pos.x);

/* RwV2d */
int                 _rwV2d_nX = offsetof(RwV2d, x);
int                 _rwV2d_nY = offsetof(RwV2d, y);
int                 sizeof_rwV2d = sizeof(RwV2d);

/* RwV3d */
int                 _rwV3d_nX = offsetof(RwV3d, x);
int                 _rwV3d_nY = offsetof(RwV3d, y);
int                 _rwV3d_nZ = offsetof(RwV3d, z);
int                 sizeof_rwV3d = sizeof(RwV3d);

/* RwRaster */
int                 _rwRaster_cpPixels = offsetof(RwRaster, cpPixels);
int                 _rwRaster_nWidth = offsetof(RwRaster, width);
int                 _rwRaster_nHeight = offsetof(RwRaster, height);
int                 _rwRaster_nDepth = offsetof(RwRaster, depth);
int                 _rwRaster_nStride = offsetof(RwRaster, stride);
int                 _rwRaster_cType = offsetof(RwRaster, cType);
int                 _rwRaster_cFlags = offsetof(RwRaster, cFlags);
int                 sizeof_rwRaster = sizeof(RwRaster);

/* Get the pipe specific ones */
#include "pipequat.c"

/* Get the device specific ones */
#include "drvequat.c"

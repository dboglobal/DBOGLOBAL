/***************************************************************************
 *                                                                         *
 * Module  : d3d9metric.c                                                    *
 *                                                                         *
 * Purpose :                                                               *
 *                                                                         *
 **************************************************************************/

/****************************************************************************
 Includes
 */

#include <string.h>

#include "batypes.h"
#include "batype.h"
#include "balibtyp.h"
#include "barwtyp.h"
#include <d3d9.h>

/* This header file */
#include "d3d9metric.h"

/****************************************************************************
 Defines
 */

/****************************************************************************
 Globals (across program)
 */

/****************************************************************************
 Local (static) Globals
 */

static RwD3D9Metrics _RwD3D9Metrics;

/****************************************************************************
 _rwD3D9MetricsReset

 On entry   :
 On exit    :
 */
void
_rwD3D9MetricsReset(void)
{
    RWFUNCTION( RWSTRING( "_rwD3D9MetricsReset" ) );

    memset( &_RwD3D9Metrics, 0, sizeof(RwD3D9Metrics) );

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D9MetricsGet

 On entry   :
 On exit    :
 */
RwD3D9Metrics *
_rwD3D9MetricsGet(void)
{
    RWFUNCTION( RWSTRING( "_rwD3D9MetricsGet" ) );

    RWRETURN(&_RwD3D9Metrics);
}


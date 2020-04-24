/***************************************************************************
 *                                                                         *
 * Module  : d3d8metric.c                                                    *
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
#include <d3d8.h>

/* This header file */
#include "d3d8metric.h"

/****************************************************************************
 Defines
 */

/****************************************************************************
 Globals (across program)
 */

/****************************************************************************
 Local (static) Globals
 */

static RwD3D8Metrics _RwD3D8Metrics;

/****************************************************************************
 _rwD3D8MetricsReset

 On entry   :
 On exit    :
 */
void
_rwD3D8MetricsReset(void)
{
    RWFUNCTION( RWSTRING( "_rwD3D8MetricsReset" ) );

    memset( &_RwD3D8Metrics, 0, sizeof(RwD3D8Metrics) );

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D8MetricsGet

 On entry   :
 On exit    :
 */
RwD3D8Metrics *
_rwD3D8MetricsGet(void)
{
    RWFUNCTION( RWSTRING( "_rwD3D8MetricsGet" ) );

    RWRETURN(&_RwD3D8Metrics);
}


/***************************************************************************
 *                                                                         *
 * Module  : d3d8metric.h                                                    *
 *                                                                         *
 * Purpose : D3D8 metrics                                                  *
 *                                                                         *
 **************************************************************************/

#ifndef D3D8METRIC_H
#define D3D8METRIC_H

/****************************************************************************
 Includes
 */

#include "batypes.h"

/****************************************************************************
 Defines
 */

/****************************************************************************
 Global Types
 */

/****************************************************************************
 Global variables (across program)
 */

/****************************************************************************
 Function prototypes
 */
#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

extern void _rwD3D8MetricsReset(void);
extern RwD3D8Metrics *_rwD3D8MetricsGet(void);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#endif /* D3D8METRIC_H */

/***************************************************************************
 *                                                                         *
 * Module  : d3d9metric.h                                                    *
 *                                                                         *
 * Purpose : D3D9 metrics                                                  *
 *                                                                         *
 **************************************************************************/

#ifndef D3D9METRIC_H
#define D3D9METRIC_H

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

extern void _rwD3D9MetricsReset(void);
extern RwD3D9Metrics *_rwD3D9MetricsGet(void);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#endif /* D3D9METRIC_H */

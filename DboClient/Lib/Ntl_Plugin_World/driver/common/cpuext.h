/***************************************************************************
 *                                                                         *
 * Purpose : CPU extensions                                                *
 *                                                                         *
 **************************************************************************/

#ifndef CPUEXT_H
#define CPUEXT_H

/****************************************************************************
 Includes
 */

/****************************************************************************
 Defines
 */

/****************************************************************************
 Global Types
 */


/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

/*
 * The CPU name length could be up to 48 bytes (49 with the '\0').
 */
extern void _rwCPUGetCPUName(RwChar *cpuName);

extern RwBool _rwCPUHaveMMX(void);

extern RwBool _rwCPUHaveSSE(void);

extern RwBool _rwCPUHaveSSE2(void);

extern RwBool _rwCPUHave3DNow(void);

/*
 * The OS version name length could be up to 127 bytes (128 with the '\0').
 */
extern RwBool
_rwCPUGetOSName(RwChar *osName);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#endif /* CPUEXT_H */

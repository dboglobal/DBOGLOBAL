/**********************************************************************
 *
 * This file is a product of Criterion Software Ltd.
 *
 * This file is provided as is with no warranties of any kind and is
 * provided without any obligation on Criterion Software Ltd.
 * to assist in its use or modification.
 *
 * Criterion Software Ltd. will not, under any
 * circumstances, be liable for any lost revenue or other damages arising
 * from the use of this file.
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 * All Rights Reserved.
 *
 * RenderWare is a trademark of Criterion Software Ltd.
 *
 ************************************************************************/


/* RWPUBLIC */
#define RWECODE(a,b) a,

/* Construct an enum type with all the plugin error codes (for the app to use) */
enum RwErrorCodePlugin_errcore 
{
#include "errcore.def"
    rwLASTERROR_errcore = RWFORCEENUMSIZEINT
};
typedef enum RwErrorCodePlugin_errcore RwErrorCodePlugin_errcore;


#undef RWECODE

/* RWPUBLICEND */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */
RwChar *_rwdb_errcore(RwInt32 code, ...);
#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

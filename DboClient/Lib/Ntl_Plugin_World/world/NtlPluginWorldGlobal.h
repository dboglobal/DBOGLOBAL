#ifndef NTL_PLUGIN_WORLD_GLOBAL_H
#define NTL_PLUGIN_WORLD_GLOBAL_H

#include "rwplcore.h"

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

	extern RwUInt32 NTL_DDS_MIPMAP_SKIP_CNT;

	extern void		SetNtlDDSMipmapSkipCnt(RwUInt32 uiSkipCnt);
	extern RwUInt32 GetNtlDDSMipmapSkipCnt();

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */


#endif /*NTL_PLUGIN_WORLD_GLOBAL_H*/
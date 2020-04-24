#include "NtlPluginWorldGlobal.h"

RwUInt32 NTL_DDS_MIPMAP_SKIP_CNT = 0;

void SetNtlDDSMipmapSkipCnt(RwUInt32 uiSkipCnt)
{
	NTL_DDS_MIPMAP_SKIP_CNT = uiSkipCnt;
}

RwUInt32 GetNtlDDSMipmapSkipCnt()
{
	return NTL_DDS_MIPMAP_SKIP_CNT;
}


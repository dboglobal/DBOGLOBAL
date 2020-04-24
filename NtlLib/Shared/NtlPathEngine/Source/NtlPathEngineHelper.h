/*****************************************************************************
* File			: NtlPathEngineHelper.h
* Author		: Haesung, Cho
* Copyright		: (аж)NTL
* Date			: 2009. 03. 14
* Abstract		: Pathengine common helper function
*****************************************************************************
* Desc:
*
*****************************************************************************/

// Collision test for AABB
extern bool NTLPE_RangesOverlap(const cHorizontalRange& r1, const cHorizontalRange& r2);

// Inclusion Test r1 in r2
extern bool NTLPE_RangesInclusion(const cHorizontalRange& srcRange, const cHorizontalRange& destRange);

// Length r1 to r2
extern int NTLPE_RangesLengthTo(const cHorizontalRange& r1, const cHorizontalRange& r2);

// Alocated Buffer
extern char* NTLPE_LoadBinary_AllocatedBuffer(const char* filename, unsigned long& size );

// Free Buffer
extern void NTLPE_FreeBuffer(char* buffer);
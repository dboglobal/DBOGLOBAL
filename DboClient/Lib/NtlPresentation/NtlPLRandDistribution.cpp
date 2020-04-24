#include "precomp_ntlpresentation.h"
#include "NtlPLRandDistribution.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


CNtlPLRandDistribution::CNtlPLRandDistribution(VOID)
{
}

CNtlPLRandDistribution::~CNtlPLRandDistribution(VOID)
{
}

VOID CNtlPLRandDistribution::GetRandInCircle(RwReal& XPos, RwReal& ZPos, RwInt32 Radius)
{
	RwReal Rand4One0		= static_cast<RwReal>(rand()) / static_cast<RwReal>(RAND_MAX);
	RwReal Rand4One1		= static_cast<RwReal>(rand()) / static_cast<RwReal>(RAND_MAX);
	RwReal Rand4One2		= static_cast<RwReal>(rand()) / static_cast<RwReal>(RAND_MAX);
	RwReal RandCircleRadian	= 2.0f * dPI * Rand4One0;
	XPos = Rand4One1 * static_cast<RwReal>(Radius) * cos(RandCircleRadian);
	ZPos = Rand4One2 * static_cast<RwReal>(Radius) * sin(RandCircleRadian);
}
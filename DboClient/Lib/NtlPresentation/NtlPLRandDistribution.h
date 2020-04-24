#pragma once


#define dPI (3.1415926535f)


class CNtlPLRandDistribution
{
public:
	CNtlPLRandDistribution(VOID);
	virtual ~CNtlPLRandDistribution(VOID);

public:
	static VOID GetRandInCircle(RwReal& XPos, RwReal& ZPos, RwInt32 Radius);
};

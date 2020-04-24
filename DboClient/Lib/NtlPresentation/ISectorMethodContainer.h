#pragma once


#define dSSM_FLAG_SPLAT_TEX_MIN_BLEND	(0x00000001)
#define dSSM_FLAG_SPLAT_TEX_MAX_BLEND	(0x00000002)
#define dSSM_FLAG_GET_AABB				(0x00000004)
#define dSSM_FLAG_GET_DOODAD_NUM		(0x00000008)
#define dSSM_FLAG_GET_RULE_OUT_DOODAD	(0x00000010)
#define dSSM_FLAG_GET_RULE_OUT_WATER	(0x00000020)
#define dSSM_FLAG_GET_TILE_TRANSPARENCY	(0x00000040)

#include "ISectorMethod.h"

using std::vector;
#include <vector>


class ISectorMethod;


class ISectorMethodContainer
{
public:
	ISectorMethodContainer(DWORD _SearchFlag);
	virtual ~ISectorMethodContainer(void);

protected:
	vector<ISectorMethod*> m_vecSM;

public:
	virtual RwBool Analyze() = 0;
	virtual RwBool Execute() = 0;
	virtual RwBool Report() = 0;
};

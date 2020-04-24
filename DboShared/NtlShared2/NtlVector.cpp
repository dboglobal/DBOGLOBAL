#include "stdafx.h"
#include "NtlVector.h"


#define DegreeToRadian( degree ) ((degree) * (NTL_PI / 180.0f))
#define RadianToDegree( radian ) ((radian) * (180.0f / NTL_PI))

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
CNtlVector CNtlVector::ZERO(0.0f, 0.0f, 0.0f);
CNtlVector CNtlVector::UNIT_X(1.0f, 0.0f, 0.0f);
CNtlVector CNtlVector::UNIT_Y(0.0f, 1.0f, 0.0f);
CNtlVector CNtlVector::UNIT_MINUS_Y(0.0f, -1.0f, 0.0f);
CNtlVector CNtlVector::UNIT_Z(0.0f, 0.0f, 1.0f);
CNtlVector CNtlVector::INVALID(INVALID_FLOAT, INVALID_FLOAT, INVALID_FLOAT);
CNtlVector CNtlVector::INVALID_XZ(INVALID_FLOAT, 0.0f, INVALID_FLOAT);
//-----------------------------------------------------------------------------------



void NormalizeV3D(sVECTOR3 * pV1, sVECTOR3 * pV2)
{
	float l = sqrtf(pV1->x * pV1->x + pV1->y * pV1->y + pV1->z * pV1->z);

	pV2->x = pV1->x / l;
	pV2->y = pV1->y / l;
	pV2->z = pV1->z / l;
}


void GetDirToDestLoc(sVECTOR3 * outDir, sVECTOR3 targetLoc, sVECTOR3 selfLoc)
{
	CNtlVector vDir(targetLoc.x - selfLoc.x, 0, targetLoc.z - selfLoc.z);
	vDir.Normalize( vDir.Length() );

	outDir->x = vDir.x; 
	outDir->y = 0.0f; 
	outDir->z = vDir.z;
}

void GetMoveTargetDestLoc(sVECTOR3 * outLoc, sVECTOR3 targetLoc, sVECTOR3 selfLoc)
{
	CNtlVector vNewPos(targetLoc.x - selfLoc.x, targetLoc.y - selfLoc.y, targetLoc.z - selfLoc.z);
	vNewPos.Normalize( vNewPos.Length() );

	outLoc->x = targetLoc.x - vNewPos.x;	
	outLoc->y = targetLoc.y - vNewPos.y;	
	outLoc->z = targetLoc.z - vNewPos.z;
}
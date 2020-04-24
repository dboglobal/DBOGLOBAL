#include "StdAfx.h"
#include <Math.h>
#include "NtlMovement.h"
#include "NtlVector.h"



void NtlLocationCompress(_compressedLocation *pResultLoc, const float fX, const float fY, const float fZ)
{
	int nX = int(fX * 100.0f);
	int nY = int(fY * 100.0f);
	int nZ = int(fZ * 100.0f);

	bool bNegativeX = false;
	bool bNegativeY = false;
	bool bNegativeZ = false;

	if (nX < 0)
	{
		bNegativeX = true;
		nX = -nX;
	}
	if (nY < 0)
	{
		bNegativeY = true;
		nY = -nY;
	}
	if (nZ < 0)
	{
		bNegativeZ = true;
		nZ = -nZ;
	}

	pResultLoc->x.second = (WORD)nX;
	pResultLoc->x.first = (BYTE)(nX >> 16);
	if (bNegativeX)
		pResultLoc->x.first |= 0x80;

	pResultLoc->y.second = (WORD)nY;
	pResultLoc->y.first = (BYTE)(nY >> 16);
	if (bNegativeY)
		pResultLoc->y.first |= 0x80;

	pResultLoc->z.second = (WORD)nZ;
	pResultLoc->z.first = (BYTE)(nZ >> 16);
	if (bNegativeZ)
		pResultLoc->z.first |= 0x80;
}

void NtlLocationDecompress(_compressedLocation *pLoc, float *pfResultX, float *pfResultY, float *pfResultZ)
{
	char cTempFirst = pLoc->x.first;
	if (cTempFirst >= 0)
	{
		*pfResultX = (float)(pLoc->x.second + (cTempFirst << 16));
	}
	else
	{
		*pfResultX = (float)(pLoc->x.second + ((cTempFirst & 0x7F) << 16)) * -1.f;
	}

	cTempFirst = pLoc->y.first;
	if (cTempFirst >= 0)
	{
		*pfResultY = (float)(pLoc->y.second + (cTempFirst << 16));
	}
	else
	{
		*pfResultY = (float)(pLoc->y.second + ((cTempFirst & 0x7F) << 16)) * -1.f;
	}

	cTempFirst = pLoc->z.first;
	if (cTempFirst >= 0)
	{
		*pfResultZ = (float)(pLoc->z.second + (cTempFirst << 16));
	}
	else
	{
		*pfResultZ = (float)(pLoc->z.second + ((cTempFirst & 0x7F) << 16)) * -1.f;
	}

	*pfResultX /= 100.0f;
	*pfResultY /= 100.0f;
	*pfResultZ /= 100.0f;
}

void NtlDirectionCompress(_compressedDirection *pResultDir, const float fX, const float fY, const float fZ)
{
	pResultDir->x = (SHORT)floor(fX * 10000.0f);
	pResultDir->y = (SHORT)floor(fY * 10000.0f);
	pResultDir->z = (SHORT)floor(fZ * 10000.0f);
}

void NtlDirectionDecompress(_compressedDirection *pDir, float *pfResultX, float *pfResultY, float *pfResultZ)
{
	*pfResultX = (float)pDir->x / 10000.0f;
	*pfResultY = (float)pDir->y / 10000.0f;
	*pfResultZ = (float)pDir->z / 10000.0f;
}

float CalcDboAngle(CNtlVector *vFirst, CNtlVector *vSecond)
{
	float v4 = -vSecond->x * -vFirst->x + vSecond->z * vFirst->z;
	float v6 = -vSecond->x * vFirst->z - vSecond->z * -vFirst->x;
	return (float)atan2(v6, v4);
}

float CalcNormalAngle(float x1, float y1, float x2, float y2)
{
	float v7 = x2 * y1 - y2 * x1;
	return (float)atan2(v7, x2 * x1 + y2 * y1);
}


bool NtlSin(float fX, float fZ, float* pfSin)
{
	if (0.0f == fX && 0.0f == fZ)
		return false;

	*pfSin = (float)(fZ / sqrt(fX * fX + fZ * fZ));
	return true;
}

bool NtlCos(float fX, float fZ, float* pfCos)
{
	if (0.0f == fX && 0.0f == fZ)
		return false;

	*pfCos = (float)(fX / sqrt(fX * fX + fZ * fZ));
	return true;
}

bool RotateVector45DegreeToLeft(float fX, float fZ, float* pfResultX, float* pfResultZ)
{
	*pfResultX = NTL_SQRT_OF_2 / 2 * fX + NTL_SQRT_OF_2 / 2 * fZ;
	*pfResultZ = -(NTL_SQRT_OF_2 / 2) * fX + NTL_SQRT_OF_2 / 2 * fZ;

	return true;
}

bool RotateVector45DegreeToRight(float fX, float fZ, float* pfResultX, float* pfResultZ)
{
	*pfResultX = NTL_SQRT_OF_2 / 2 * fX - NTL_SQRT_OF_2 / 2 * fZ;
	*pfResultZ = NTL_SQRT_OF_2 / 2 * fX + NTL_SQRT_OF_2 / 2 * fZ;

	return true;
}

bool RotateVector90DegreeToLeft(float fX, float fZ, float* pfResultX, float* pfResultZ)
{
	*pfResultX = fZ;
	*pfResultZ = -fX;

	return true;
}

bool RotateVector90DegreeToRight(float fX, float fZ, float* pfResultX, float* pfResultZ)
{
	*pfResultX = -fZ;
	*pfResultZ = fX;

	return true;
}

bool RotateVector135DegreeToLeft(float fX, float fZ, float* pfResultX, float* pfResultZ)
{
	*pfResultX = -(NTL_SQRT_OF_2 / 2) * fX + NTL_SQRT_OF_2 / 2 * fZ;
	*pfResultZ = -(NTL_SQRT_OF_2 / 2) * fX - NTL_SQRT_OF_2 / 2 * fZ;

	return true;
}

bool RotateVector135DegreeToRight(float fX, float fZ, float* pfResultX, float* pfResultZ)
{
	*pfResultX = -(NTL_SQRT_OF_2 / 2) * fX - NTL_SQRT_OF_2 / 2 * fZ;
	*pfResultZ = NTL_SQRT_OF_2 / 2 * fX - NTL_SQRT_OF_2 / 2 * fZ;

	return true;
}

bool RotateVector180Degree(float fX, float fZ, float* pfResultX, float* pfResultZ)
{
	*pfResultX = -fX;
	*pfResultZ = -fZ;

	return true;
}

bool RotateVector(float fX, float fZ, float fAngleInRadian, float* pfResultX, float* pfResultZ)
{
	*pfResultX = (float)(cos(fAngleInRadian) * fX + sin(fAngleInRadian) * fZ);
	*pfResultZ = (float)(-sin(fAngleInRadian) * fX + cos(fAngleInRadian) * fZ);

	return true;
}

bool GetVectorWithDegree(int nDegree, float* pfVectorX, float* pfVectorZ)
{
	float fRadian = ConvertDegreeToRadian(nDegree);

	*pfVectorX = (float)sin(fRadian);
	*pfVectorZ = (float)cos(fRadian);

	return true;
}

float ConvertDegreeToRadian(int nDegree)
{
	int nTemp;

	if (0 <= nDegree)
	{
		nTemp = nDegree % 360;
	}
	else
	{
		nTemp = -(-nDegree % 360) + 360;
	}

	return NTL_PI * nTemp / 180;
}

float GetDegreeToRadian(int nDegree)
{
	return (float)(NTL_PI * (float)nDegree) / 180.f;
}

CNtlVector& RotateVector45DegreeToLeft(CNtlVector* pVector)
{
	static CNtlVector vResult;

	vResult.x = NTL_SQRT_OF_2 / 2 * pVector->x + NTL_SQRT_OF_2 / 2 * pVector->z;
	vResult.y = 0.0f;
	vResult.z = -(NTL_SQRT_OF_2 / 2) * pVector->x + NTL_SQRT_OF_2 / 2 * pVector->z;

	return vResult;
}

CNtlVector& RotateVector45DegreeToRight(CNtlVector* pVector)
{
	static CNtlVector vResult;

	vResult.x = NTL_SQRT_OF_2 / 2 * pVector->x - NTL_SQRT_OF_2 / 2 * pVector->z;
	vResult.y = 0.0f;
	vResult.z = NTL_SQRT_OF_2 / 2 * pVector->x + NTL_SQRT_OF_2 / 2 * pVector->z;

	return vResult;
}

CNtlVector& RotateVector90DegreeToLeft(CNtlVector* pVector)
{
	static CNtlVector vResult;

	vResult.x = pVector->z;
	vResult.y = 0.0f;
	vResult.z = -pVector->x;

	return vResult;
}

CNtlVector& RotateVector90DegreeToRight(CNtlVector* pVector)
{
	static CNtlVector vResult;

	vResult.x = -pVector->z;
	vResult.y = 0.0f;
	vResult.z = pVector->x;

	return vResult;
}

CNtlVector& RotateVector135DegreeToLeft(CNtlVector* pVector)
{
	static CNtlVector vResult;

	vResult.x = -(NTL_SQRT_OF_2 / 2) * pVector->x + NTL_SQRT_OF_2 / 2 * pVector->z;
	vResult.y = 0.0f;
	vResult.z = -(NTL_SQRT_OF_2 / 2) * pVector->x - NTL_SQRT_OF_2 / 2 * pVector->z;

	return vResult;
}

CNtlVector& RotateVector135DegreeToRight(CNtlVector* pVector)
{
	static CNtlVector vResult;

	vResult.x = -(NTL_SQRT_OF_2 / 2) * pVector->x - NTL_SQRT_OF_2 / 2 * pVector->z;
	vResult.y = 0.0f;
	vResult.z = NTL_SQRT_OF_2 / 2 * pVector->x - NTL_SQRT_OF_2 / 2 * pVector->z;

	return vResult;
}

CNtlVector& RotateVector180Degree(CNtlVector* pVector)
{
	static CNtlVector vResult;

	vResult.x = -pVector->x;
	vResult.y = -pVector->y;
	vResult.z = -pVector->z;

	return vResult;
}

CNtlVector& RotateVector(CNtlVector* pVector, float fAngleInRadian)
{
	static CNtlVector vResult;

	vResult.x = (float)(cos(fAngleInRadian) * pVector->x + sin(fAngleInRadian) * pVector->z);
	vResult.y = pVector->y;
	vResult.z = (float)(-sin(fAngleInRadian) * pVector->x + cos(fAngleInRadian) * pVector->z);

	return vResult;
}

void RotateVector(CNtlVector &rResult, CNtlVector* pVector, float fAngleInRadian)
{
	rResult.x = (float)(cos(fAngleInRadian) * pVector->x + sin(fAngleInRadian) * pVector->z);
	rResult.y = pVector->y;
	rResult.z = (float)(-sin(fAngleInRadian) * pVector->x + cos(fAngleInRadian) * pVector->z);
}

#include <stdio.h>


void NtlGetDestination_Keyboard(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
								float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
								BYTE byMoveDirection, DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
								CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	typedef void(*MOVEMENT_KEYBOARD_FUNC)(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
											float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
											DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
											CNtlVector* pNewHeadingVector, CNtlVector* pDestination);

	static bool bIsInitialized = false;
	static MOVEMENT_KEYBOARD_FUNC g_pMovementKeyboardFunc[NTL_MOVE_COUNT];

	if (false == bIsInitialized)
	{
		g_pMovementKeyboardFunc[NTL_MOVE_NONE] = NtlGetDestination_Keyboard_NONE;

		g_pMovementKeyboardFunc[NTL_MOVE_F] = NtlGetDestination_Keyboard_F;
		g_pMovementKeyboardFunc[NTL_MOVE_B] = NtlGetDestination_Keyboard_B;
		g_pMovementKeyboardFunc[NTL_MOVE_L] = NtlGetDestination_Keyboard_L;
		g_pMovementKeyboardFunc[NTL_MOVE_R] = NtlGetDestination_Keyboard_R;

		g_pMovementKeyboardFunc[NTL_MOVE_HEIGHT_UP] = NtlGetDestination_Keyboard_HEIGHT_UP;
		g_pMovementKeyboardFunc[NTL_MOVE_HEIGHT_UP_F] = NtlGetDestination_Keyboard_HEIGHT_UP_F;
		g_pMovementKeyboardFunc[NTL_MOVE_HEIGHT_UP_B] = NtlGetDestination_Keyboard_HEIGHT_UP_B;
		g_pMovementKeyboardFunc[NTL_MOVE_HEIGHT_UP_L] = NtlGetDestination_Keyboard_HEIGHT_UP_L;
		g_pMovementKeyboardFunc[NTL_MOVE_HEIGHT_UP_R] = NtlGetDestination_Keyboard_HEIGHT_UP_R;
		g_pMovementKeyboardFunc[NTL_MOVE_HEIGHT_DOWN] = NtlGetDestination_Keyboard_HEIGHT_DOWN;

		g_pMovementKeyboardFunc[NTL_MOVE_TURN_L] = NtlGetDestination_Keyboard_TURN_L;
		g_pMovementKeyboardFunc[NTL_MOVE_TURN_R] = NtlGetDestination_Keyboard_TURN_R;

		g_pMovementKeyboardFunc[NTL_MOVE_F_TURN_L] = NtlGetDestination_Keyboard_F_TURN_L;
		g_pMovementKeyboardFunc[NTL_MOVE_F_TURN_R] = NtlGetDestination_Keyboard_F_TURN_R;
		g_pMovementKeyboardFunc[NTL_MOVE_B_TURN_L] = NtlGetDestination_Keyboard_B_TURN_L;
		g_pMovementKeyboardFunc[NTL_MOVE_B_TURN_R] = NtlGetDestination_Keyboard_B_TURN_R;
		g_pMovementKeyboardFunc[NTL_MOVE_L_TURN_L] = NtlGetDestination_Keyboard_L_TURN_L;
		g_pMovementKeyboardFunc[NTL_MOVE_L_TURN_R] = NtlGetDestination_Keyboard_L_TURN_R;
		g_pMovementKeyboardFunc[NTL_MOVE_R_TURN_L] = NtlGetDestination_Keyboard_R_TURN_L;
		g_pMovementKeyboardFunc[NTL_MOVE_R_TURN_R] = NtlGetDestination_Keyboard_R_TURN_R;

		g_pMovementKeyboardFunc[NTL_MOVE_L_TURN_L_JUMP] = NtlGetDestination_Keyboard_L;
		g_pMovementKeyboardFunc[NTL_MOVE_L_TURN_R_JUMP] = NtlGetDestination_Keyboard_L;
		g_pMovementKeyboardFunc[NTL_MOVE_R_TURN_L_JUMP] = NtlGetDestination_Keyboard_R;
		g_pMovementKeyboardFunc[NTL_MOVE_R_TURN_R_JUMP] = NtlGetDestination_Keyboard_R;

		g_pMovementKeyboardFunc[NTL_MOVE_F_TURN_L_JUMP] = NtlGetDestination_Keyboard_F;
		g_pMovementKeyboardFunc[NTL_MOVE_F_TURN_R_JUMP] = NtlGetDestination_Keyboard_F;
		g_pMovementKeyboardFunc[NTL_MOVE_B_TURN_L_JUMP] = NtlGetDestination_Keyboard_B;
		g_pMovementKeyboardFunc[NTL_MOVE_B_TURN_R_JUMP] = NtlGetDestination_Keyboard_B;

		bIsInitialized = true;
	}

	if(g_pMovementKeyboardFunc[byMoveDirection] == NULL)
	{
		_ASSERTE(0);
		return;
	}

	g_pMovementKeyboardFunc[byMoveDirection](fCurrentHeadingVectorX, fCurrentHeadingVectorY, fCurrentHeadingVectorZ, fSpeedInSecs,
												fCurrentPositionX, fCurrentPositionY, fCurrentPositionZ,
												dwDeltaTimeInMillisecs, fTurningSpeedRatio,
												pNewHeadingVector, pDestination);
}

void NtlGetDestination_Keyboard_NONE(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
										float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
										DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
										CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( dwDeltaTimeInMillisecs );
	UNREFERENCED_PARAMETER( fSpeedInSecs );
	UNREFERENCED_PARAMETER( fTurningSpeedRatio );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = fCurrentHeadingVectorY;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;
}

void NtlGetDestination_Keyboard_F(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( fTurningSpeedRatio );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = fCurrentHeadingVectorY;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += *pNewHeadingVector * fDistanceToGo;
}

void NtlGetDestination_Keyboard_B(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( fTurningSpeedRatio );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = fCurrentHeadingVectorY;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	float fDistanceToGo = fSpeedInSecs * NTL_BACKWARD_MOVEMENT_SPEED_RATE * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += RotateVector180Degree(pNewHeadingVector) * fDistanceToGo;
}

void NtlGetDestination_Keyboard_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( fTurningSpeedRatio );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = fCurrentHeadingVectorY;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += RotateVector90DegreeToLeft(pNewHeadingVector) * fDistanceToGo;
}

void NtlGetDestination_Keyboard_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( fTurningSpeedRatio );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = fCurrentHeadingVectorY;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += RotateVector90DegreeToRight(pNewHeadingVector) * fDistanceToGo;
}

void NtlGetDestination_Keyboard_HEIGHT_UP(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
	float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
	DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
	CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER(fTurningSpeedRatio);
	UNREFERENCED_PARAMETER(fCurrentHeadingVectorY);

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += CNtlVector::UNIT_Y * fDistanceToGo;
}

void NtlGetDestination_Keyboard_HEIGHT_UP_F(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
	float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
	DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
	CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER(fTurningSpeedRatio);
	UNREFERENCED_PARAMETER(fCurrentHeadingVectorY);

	CNtlVector vec(fCurrentHeadingVectorX, 1.0f, fCurrentHeadingVectorZ);
	vec.Normalize();

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += vec * fDistanceToGo;
}

void NtlGetDestination_Keyboard_HEIGHT_UP_B(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
	float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
	DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
	CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER(fTurningSpeedRatio);
	UNREFERENCED_PARAMETER(fCurrentHeadingVectorY);

	CNtlVector vec;
	RotateVector180Degree(fCurrentHeadingVectorX, fCurrentHeadingVectorZ, &vec.x, &vec.z);
	vec.y = 1.0f;

	vec.Normalize();

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += vec * fDistanceToGo;
}

void NtlGetDestination_Keyboard_HEIGHT_UP_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
	float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
	DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
	CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER(fTurningSpeedRatio);
	UNREFERENCED_PARAMETER(fCurrentHeadingVectorY);

	CNtlVector vec;
	RotateVector90DegreeToLeft(fCurrentHeadingVectorX, fCurrentHeadingVectorZ, &vec.x, &vec.z);
	vec.y = 1.0f;

	vec.Normalize();

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += vec * fDistanceToGo;
}

void NtlGetDestination_Keyboard_HEIGHT_UP_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
	float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
	DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
	CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER(fTurningSpeedRatio);
	UNREFERENCED_PARAMETER(fCurrentHeadingVectorY);

	CNtlVector vec;
	RotateVector90DegreeToRight(fCurrentHeadingVectorX, fCurrentHeadingVectorZ, &vec.x, &vec.z);
	vec.y = 1.0f;

	vec.Normalize();

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += vec * fDistanceToGo;
}

void NtlGetDestination_Keyboard_HEIGHT_DOWN(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
	float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
	DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
	CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER(fTurningSpeedRatio);
	UNREFERENCED_PARAMETER(fCurrentHeadingVectorY);

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += CNtlVector::UNIT_MINUS_Y * fDistanceToGo;
}

void NtlGetDestination_Keyboard_F_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( fTurningSpeedRatio );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = fCurrentHeadingVectorY;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += RotateVector45DegreeToLeft(pNewHeadingVector) * fDistanceToGo;
}

void NtlGetDestination_Keyboard_F_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( fTurningSpeedRatio );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = fCurrentHeadingVectorY;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += RotateVector45DegreeToRight(pNewHeadingVector) * fDistanceToGo;
}

void NtlGetDestination_Keyboard_B_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( fTurningSpeedRatio );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = fCurrentHeadingVectorY;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	float fDistanceToGo = fSpeedInSecs * NTL_BACKWARD_MOVEMENT_SPEED_RATE * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += RotateVector135DegreeToLeft(pNewHeadingVector) * fDistanceToGo;
}

void NtlGetDestination_Keyboard_B_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( fTurningSpeedRatio );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = fCurrentHeadingVectorY;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	float fDistanceToGo = fSpeedInSecs * NTL_BACKWARD_MOVEMENT_SPEED_RATE * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += RotateVector135DegreeToRight(pNewHeadingVector) * fDistanceToGo;
}

void NtlGetDestination_Keyboard_TURN_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
										float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
										DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
										CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( fSpeedInSecs );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = fCurrentHeadingVectorY;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	float fRadian = 2 * NTL_PI * ((float)dwDeltaTimeInMillisecs / (float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS);
	fRadian *= fTurningSpeedRatio;

	*pNewHeadingVector = RotateVector(pNewHeadingVector, fRadian);
}

void NtlGetDestination_Keyboard_TURN_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
										float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
										DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
										CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( fSpeedInSecs );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = fCurrentHeadingVectorY;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	float fRadian = 2 * NTL_PI * ((float)dwDeltaTimeInMillisecs / (float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS);
	fRadian *= fTurningSpeedRatio;

	*pNewHeadingVector = RotateVector(pNewHeadingVector, -fRadian);
}

void NtlGetDestination_Keyboard_F_TURN_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
											float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
											DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
											CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = fCurrentHeadingVectorY;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	float fRadian = 2 * NTL_PI * ((float)dwDeltaTimeInMillisecs / (float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS);
	fRadian *= fTurningSpeedRatio;

	*pNewHeadingVector = RotateVector(pNewHeadingVector, fRadian);

	float fRadius = 0.0f;
	fRadius = fSpeedInSecs * ((float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS / (float)1000) / (2 * NTL_PI);

	float fTempX = (float)(fCurrentHeadingVectorX * fRadius / sqrt(fCurrentHeadingVectorX * fCurrentHeadingVectorX + fCurrentHeadingVectorZ * fCurrentHeadingVectorZ));
	float fTempZ = (float)(fCurrentHeadingVectorZ * fRadius / sqrt(fCurrentHeadingVectorX * fCurrentHeadingVectorX + fCurrentHeadingVectorZ * fCurrentHeadingVectorZ));

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	pDestination->x = (float)(fCurrentPositionX + fTempX * sin(fRadian) + fTempZ * (1 - cos(fRadian)));
	pDestination->y += pNewHeadingVector->y * fDistanceToGo;
	pDestination->z = (float)(fCurrentPositionZ + fTempZ * sin(fRadian) - fTempX * (1 - cos(fRadian)));
}

void NtlGetDestination_Keyboard_F_TURN_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
											float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
											DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
											CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = fCurrentHeadingVectorY;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	float fRadian = 2 * NTL_PI * ((float)dwDeltaTimeInMillisecs / (float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS);
	fRadian *= fTurningSpeedRatio;

	*pNewHeadingVector = RotateVector(pNewHeadingVector, -fRadian);

	float fRadius = 0.0f;
	fRadius = fSpeedInSecs * ((float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS / (float)1000) / (2 * NTL_PI);

	float fTempX = (float)(fCurrentHeadingVectorX * fRadius / sqrt(fCurrentHeadingVectorX * fCurrentHeadingVectorX + fCurrentHeadingVectorZ * fCurrentHeadingVectorZ));
	float fTempZ = (float)(fCurrentHeadingVectorZ * fRadius / sqrt(fCurrentHeadingVectorX * fCurrentHeadingVectorX + fCurrentHeadingVectorZ * fCurrentHeadingVectorZ));

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	pDestination->x = (float)(fCurrentPositionX + fTempX * sin(fRadian) - fTempZ * (1 - cos(fRadian)));
	pDestination->y += pNewHeadingVector->y * fDistanceToGo;
	pDestination->z = (float)(fCurrentPositionZ + fTempZ * sin(fRadian) - fTempX * (1 - cos(fRadian)));
}

void NtlGetDestination_Keyboard_B_TURN_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
											float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
											DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
											CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = fCurrentHeadingVectorY;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	float fRadian = 2 * NTL_PI * ((float)dwDeltaTimeInMillisecs / (float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS);
	fRadian *= fTurningSpeedRatio;

	*pNewHeadingVector = RotateVector(pNewHeadingVector, fRadian);

	float fRadius = 0.0f;
	fRadius = fSpeedInSecs * NTL_BACKWARD_MOVEMENT_SPEED_RATE * ((float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS / (float)1000) / (2 * NTL_PI);

	float fTempX = (float)(fCurrentHeadingVectorX * fRadius / sqrt(fCurrentHeadingVectorX * fCurrentHeadingVectorX + fCurrentHeadingVectorZ * fCurrentHeadingVectorZ));
	float fTempZ = (float)(fCurrentHeadingVectorZ * fRadius / sqrt(fCurrentHeadingVectorX * fCurrentHeadingVectorX + fCurrentHeadingVectorZ * fCurrentHeadingVectorZ));

	float fDistanceToGo = fSpeedInSecs * NTL_BACKWARD_MOVEMENT_SPEED_RATE * (float)dwDeltaTimeInMillisecs / 1000.0f;

	pDestination->x = (float)(fCurrentPositionX - fTempX * sin(fRadian) - fTempZ * (1 - cos(fRadian)));
	pDestination->y += pNewHeadingVector->y * fDistanceToGo;
	pDestination->z = (float)(fCurrentPositionZ - fTempZ * sin(fRadian) - fTempX * (1 - cos(fRadian)));
}

void NtlGetDestination_Keyboard_B_TURN_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
											float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
											DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
											CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = fCurrentHeadingVectorY;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	float fRadian = 2 * NTL_PI * ((float)dwDeltaTimeInMillisecs / (float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS);
	fRadian *= fTurningSpeedRatio;

	*pNewHeadingVector = RotateVector(pNewHeadingVector, -fRadian);

	float fRadius = 0.0f;
	fRadius = fSpeedInSecs * NTL_BACKWARD_MOVEMENT_SPEED_RATE * ((float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS / (float)1000) / (2 * NTL_PI);

	float fTempX = (float)(fCurrentHeadingVectorX * fRadius / sqrt(fCurrentHeadingVectorX * fCurrentHeadingVectorX + fCurrentHeadingVectorZ * fCurrentHeadingVectorZ));
	float fTempZ = (float)(fCurrentHeadingVectorZ * fRadius / sqrt(fCurrentHeadingVectorX * fCurrentHeadingVectorX + fCurrentHeadingVectorZ * fCurrentHeadingVectorZ));

	float fDistanceToGo = fSpeedInSecs * NTL_BACKWARD_MOVEMENT_SPEED_RATE * (float)dwDeltaTimeInMillisecs / 1000.0f;

	pDestination->x = (float)(fCurrentPositionX - fTempX * sin(fRadian) + fTempZ * (1 - cos(fRadian)));
	pDestination->y += pNewHeadingVector->y * fDistanceToGo;
	pDestination->z = (float)(fCurrentPositionZ - fTempZ * sin(fRadian) - fTempX * (1 - cos(fRadian)));
}

void NtlGetDestination_Keyboard_L_TURN_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
	float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
	DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
	CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = fCurrentHeadingVectorY;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	CNtlVector rot = RotateVector90DegreeToLeft(pNewHeadingVector);

	float fRadius = fSpeedInSecs * ((float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS / 1000.0f) / (2 * NTL_PI);

	float fRadian = (2 * NTL_PI) * ((float)dwDeltaTimeInMillisecs / (float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS);
	fRadian *= fTurningSpeedRatio;

	float fTempX = (float)(rot.x * fRadius / sqrt((rot.x * rot.x) + (rot.z * rot.z)));
	float fTempZ = (float)(rot.z * fRadius / sqrt((rot.x * rot.x) + (rot.z * rot.z)));

	*pNewHeadingVector = RotateVector(pNewHeadingVector, fRadian);

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	pDestination->x = (float)(fCurrentPositionX + (fTempX * sin(fRadian)) + fTempZ * (1.0f - cos(fRadian)));
	pDestination->y += pNewHeadingVector->y * fDistanceToGo;
	pDestination->z = (float)(fCurrentPositionZ + (fTempZ * sin(fRadian)) - fTempX * (1.0f - cos(fRadian)));
}

void NtlGetDestination_Keyboard_L_TURN_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
	float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
	DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
	CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = fCurrentHeadingVectorY;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	CNtlVector rot = RotateVector90DegreeToLeft(pNewHeadingVector);

	float fRadian = 2 * NTL_PI * ((float)dwDeltaTimeInMillisecs / (float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS);
	fRadian *= fTurningSpeedRatio;

	float fRadius = fSpeedInSecs * ((float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS / 1000.0f) / (2 * NTL_PI);

	float fTempX = (float)(rot.x * fRadius / sqrt((rot.x * rot.x) + (rot.z * rot.z)));
	float fTempZ = (float)(rot.z * fRadius / sqrt((rot.x * rot.x) + (rot.z * rot.z)));

	*pNewHeadingVector = RotateVector(pNewHeadingVector, -fRadian);

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	pDestination->x = (float)(fCurrentPositionX + (fTempX * sin(fRadian)) + fTempZ * (1 - cos(fRadian)));
	pDestination->y += pNewHeadingVector->y * fDistanceToGo;
	pDestination->z = (float)(fCurrentPositionZ + (fTempZ * sin(fRadian)) - fTempX * (1 - cos(fRadian)));
}

void NtlGetDestination_Keyboard_R_TURN_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
	float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
	DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
	CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = fCurrentHeadingVectorY;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	CNtlVector rot = RotateVector90DegreeToRight(pNewHeadingVector);

	float fRadian = 2 * NTL_PI * ((float)dwDeltaTimeInMillisecs / (float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS) * fTurningSpeedRatio;

	float fRadius = fSpeedInSecs * ((float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS / 1000.0f) / (2 * NTL_PI);

	float fTempX = (float)(rot.x * fRadius / sqrt((rot.x * rot.x) + (rot.z * rot.z)));
	float fTempZ = (float)(rot.z * fRadius / sqrt((rot.x * rot.x) + (rot.z * rot.z)));

	*pNewHeadingVector = RotateVector(pNewHeadingVector, fRadian);

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	pDestination->x = (float)(fCurrentPositionX + (fTempX * sin(fRadian)) + fTempZ * (1 - cos(fRadian)));
	pDestination->y += pNewHeadingVector->y * fDistanceToGo;
	pDestination->z = (float)(fCurrentPositionZ + (fTempZ * sin(fRadian)) - fTempX * (1 - cos(fRadian)));
}

void NtlGetDestination_Keyboard_R_TURN_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
	float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
	DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
	CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = fCurrentHeadingVectorY;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	CNtlVector rot = RotateVector90DegreeToRight(pNewHeadingVector);

	float fRadian = 2 * NTL_PI * ((float)dwDeltaTimeInMillisecs / (float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS) * fTurningSpeedRatio;

	float fRadius = fSpeedInSecs * ((float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS / 1000.0f) / (2 * NTL_PI);

	float fTempX = (float)(rot.x * fRadius / sqrt((rot.x * rot.x) + (rot.z * rot.z)));
	float fTempZ = (float)(rot.z * fRadius / sqrt((rot.x * rot.x) + (rot.z * rot.z)));

	*pNewHeadingVector = RotateVector(pNewHeadingVector, -fRadian);

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	pDestination->x = (float)(fCurrentPositionX + (fTempX * sin(fRadian)) + fTempZ * (1 - cos(fRadian)));
	pDestination->y += pNewHeadingVector->y * fDistanceToGo;
	pDestination->z = (float)(fCurrentPositionZ + (fTempZ * sin(fRadian)) - fTempX * (1 - cos(fRadian)));
}

void NtlGetDestination_Jump(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
								float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
								float fJumpDirectionX, float fJumpDirectionZ,
								BYTE byMoveDirection, DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
								CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	typedef void(*MOVEMENT_JUMP_FUNC)(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
										float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
										float fJumpDirectionX, float fJumpDirectionZ,
										DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
										CNtlVector* pNewHeadingVector, CNtlVector* pDestination);

	static bool bIsInitialized = false;
	static MOVEMENT_JUMP_FUNC g_pMovementJumpFunc[NTL_MOVE_COUNT];

	if (false == bIsInitialized)
	{
		g_pMovementJumpFunc[NTL_MOVE_NONE] = NtlGetDestination_Jump_NONE;

		g_pMovementJumpFunc[NTL_MOVE_F] = NtlGetDestination_Jump_F;
		g_pMovementJumpFunc[NTL_MOVE_B] = NtlGetDestination_Jump_B;
		g_pMovementJumpFunc[NTL_MOVE_L] = NtlGetDestination_Jump_L;
		g_pMovementJumpFunc[NTL_MOVE_R] = NtlGetDestination_Jump_R;

		g_pMovementJumpFunc[NTL_MOVE_TURN_L] = NtlGetDestination_Jump_TURN_L;
		g_pMovementJumpFunc[NTL_MOVE_TURN_R] = NtlGetDestination_Jump_TURN_R;

		g_pMovementJumpFunc[NTL_MOVE_L_TURN_L_JUMP] = NtlGetDestination_Jump_L_TURN_L;
		g_pMovementJumpFunc[NTL_MOVE_L_TURN_R_JUMP] = NtlGetDestination_Jump_L_TURN_R;
		g_pMovementJumpFunc[NTL_MOVE_R_TURN_L_JUMP] = NtlGetDestination_Jump_R_TURN_L;
		g_pMovementJumpFunc[NTL_MOVE_R_TURN_R_JUMP] = NtlGetDestination_Jump_R_TURN_R;

		g_pMovementJumpFunc[NTL_MOVE_F_TURN_L_JUMP] = NtlGetDestination_Jump_F_TURN_L;
		g_pMovementJumpFunc[NTL_MOVE_F_TURN_R_JUMP] = NtlGetDestination_Jump_F_TURN_R;
		g_pMovementJumpFunc[NTL_MOVE_B_TURN_L_JUMP] = NtlGetDestination_Jump_B_TURN_L;
		g_pMovementJumpFunc[NTL_MOVE_B_TURN_R_JUMP] = NtlGetDestination_Jump_B_TURN_R;

		bIsInitialized = true;
	}

	if(g_pMovementJumpFunc[byMoveDirection] == NULL)
	{
		//- yoshiki : Find out why this can happen while a character is jumping!
		//_ASSERTE(0);
		return;
	}

	g_pMovementJumpFunc[byMoveDirection](fCurrentHeadingVectorX, fCurrentHeadingVectorY, fCurrentHeadingVectorZ, fSpeedInSecs,
											fCurrentPositionX, fCurrentPositionY, fCurrentPositionZ,
											fJumpDirectionX, fJumpDirectionZ,
											dwDeltaTimeInMillisecs, fTurningSpeedRatio,
											pNewHeadingVector, pDestination);
}

void NtlGetDestination_Jump_NONE(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									float fInitialHeadingVectorX, float fInitialHeadingVectorZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( dwDeltaTimeInMillisecs );
	UNREFERENCED_PARAMETER( fSpeedInSecs );
	UNREFERENCED_PARAMETER( fInitialHeadingVectorX );
	UNREFERENCED_PARAMETER( fInitialHeadingVectorZ );
	UNREFERENCED_PARAMETER( fTurningSpeedRatio );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = fCurrentHeadingVectorY;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;
}

void NtlGetDestination_Jump_F(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
								float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
								float fInitialHeadingVectorX, float fInitialHeadingVectorZ,
								DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
								CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( fTurningSpeedRatio );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = fCurrentHeadingVectorY;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	CNtlVector vInitialHeading(fInitialHeadingVectorX, 0, fInitialHeadingVectorZ);

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += vInitialHeading * fDistanceToGo;
}

void NtlGetDestination_Jump_B(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
								float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
								float fInitialHeadingVectorX, float fInitialHeadingVectorZ,
								DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
								CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( fTurningSpeedRatio );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = fCurrentHeadingVectorY;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	CNtlVector vInitialHeading(fInitialHeadingVectorX, 0, fInitialHeadingVectorZ);

	float fDistanceToGo = fSpeedInSecs * NTL_BACKWARD_MOVEMENT_SPEED_RATE * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += RotateVector180Degree(&vInitialHeading) * fDistanceToGo;
}

void NtlGetDestination_Jump_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
								float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
								float fInitialHeadingVectorX, float fInitialHeadingVectorZ,
								DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
								CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( fTurningSpeedRatio );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = fCurrentHeadingVectorY;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	CNtlVector vInitialHeading(fInitialHeadingVectorX, 0, fInitialHeadingVectorZ);

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += RotateVector90DegreeToLeft(&vInitialHeading) * fDistanceToGo;
}

void NtlGetDestination_Jump_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
								float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
								float fInitialHeadingVectorX, float fInitialHeadingVectorZ,
								DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
								CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( fTurningSpeedRatio );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = fCurrentHeadingVectorY;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	CNtlVector vInitialHeading(fInitialHeadingVectorX, 0, fInitialHeadingVectorZ);

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += RotateVector90DegreeToRight(&vInitialHeading) * fDistanceToGo;
}

void NtlGetDestination_Jump_TURN_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									float fInitialHeadingVectorX, float fInitialHeadingVectorZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( fSpeedInSecs );
	UNREFERENCED_PARAMETER( fInitialHeadingVectorX );
	UNREFERENCED_PARAMETER( fInitialHeadingVectorZ );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = fCurrentHeadingVectorY;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	float fRadian = 2 * NTL_PI * ((float)dwDeltaTimeInMillisecs / (float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS);
	fRadian *= fTurningSpeedRatio;

	*pNewHeadingVector = RotateVector(pNewHeadingVector, fRadian);
}

void NtlGetDestination_Jump_TURN_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									float fInitialHeadingVectorX, float fInitialHeadingVectorZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( fSpeedInSecs );
	UNREFERENCED_PARAMETER( fInitialHeadingVectorX );
	UNREFERENCED_PARAMETER( fInitialHeadingVectorZ );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = fCurrentHeadingVectorY;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	float fRadian = 2 * NTL_PI * ((float)dwDeltaTimeInMillisecs / (float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS);
	fRadian *= fTurningSpeedRatio;

	*pNewHeadingVector = RotateVector(pNewHeadingVector, -fRadian);
}

void NtlGetDestination_Jump_F_TURN_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
										float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
										float fInitialHeadingVectorX, float fInitialHeadingVectorZ,
										DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
										CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = fCurrentHeadingVectorY;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	float fRadian = 2 * NTL_PI * ((float)dwDeltaTimeInMillisecs / (float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS);
	fRadian *= fTurningSpeedRatio;

	*pNewHeadingVector = RotateVector(pNewHeadingVector, fRadian);

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	CNtlVector vInitialHeading(fInitialHeadingVectorX, 0, fInitialHeadingVectorZ);

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += vInitialHeading * fDistanceToGo;
}

void NtlGetDestination_Jump_F_TURN_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
										float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
										float fInitialHeadingVectorX, float fInitialHeadingVectorZ,
										DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
										CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = fCurrentHeadingVectorY;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	float fRadian = 2 * NTL_PI * ((float)dwDeltaTimeInMillisecs / (float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS);
	fRadian *= fTurningSpeedRatio;

	*pNewHeadingVector = RotateVector(pNewHeadingVector, -fRadian);

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	CNtlVector vInitialHeading(fInitialHeadingVectorX, 0, fInitialHeadingVectorZ);

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += vInitialHeading * fDistanceToGo;
}

void NtlGetDestination_Jump_B_TURN_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
										float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
										float fInitialHeadingVectorX, float fInitialHeadingVectorZ,
										DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
										CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = fCurrentHeadingVectorY;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	float fRadian = 2 * NTL_PI * ((float)dwDeltaTimeInMillisecs / (float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS);
	fRadian *= fTurningSpeedRatio;

	*pNewHeadingVector = RotateVector(pNewHeadingVector, fRadian);

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	CNtlVector vInitialHeading(fInitialHeadingVectorX, 0, fInitialHeadingVectorZ);

	float fDistanceToGo = fSpeedInSecs * NTL_BACKWARD_MOVEMENT_SPEED_RATE * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += RotateVector180Degree(&vInitialHeading) * fDistanceToGo;
}

void NtlGetDestination_Jump_B_TURN_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
										float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
										float fInitialHeadingVectorX, float fInitialHeadingVectorZ,
										DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
										CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = fCurrentHeadingVectorY;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	float fRadian = 2 * NTL_PI * ((float)dwDeltaTimeInMillisecs / (float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS);
	fRadian *= fTurningSpeedRatio;

	*pNewHeadingVector = RotateVector(pNewHeadingVector, -fRadian);

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	CNtlVector vInitialHeading(fInitialHeadingVectorX, 0.0f, fInitialHeadingVectorZ);

	float fDistanceToGo = fSpeedInSecs * NTL_BACKWARD_MOVEMENT_SPEED_RATE * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += RotateVector180Degree(&vInitialHeading) * fDistanceToGo;
}

void NtlGetDestination_Jump_L_TURN_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
	float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
	float fInitialHeadingVectorX, float fInitialHeadingVectorZ,
	DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
	CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = fCurrentHeadingVectorY;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	float fRadian = 2 * NTL_PI * ((float)dwDeltaTimeInMillisecs / (float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS);
	fRadian *= fTurningSpeedRatio;

	*pNewHeadingVector = RotateVector(pNewHeadingVector, fRadian);

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	CNtlVector vInitialHeading(fInitialHeadingVectorX, 0.0f, fInitialHeadingVectorZ);

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += RotateVector90DegreeToLeft(&vInitialHeading) * fDistanceToGo;
}

void NtlGetDestination_Jump_L_TURN_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
	float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
	float fInitialHeadingVectorX, float fInitialHeadingVectorZ,
	DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
	CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = fCurrentHeadingVectorY;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	float fRadian = 2 * NTL_PI * ((float)dwDeltaTimeInMillisecs / (float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS);
	fRadian *= fTurningSpeedRatio;

	*pNewHeadingVector = RotateVector(pNewHeadingVector, -fRadian);

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	CNtlVector vInitialHeading(fInitialHeadingVectorX, 0.0f, fInitialHeadingVectorZ);

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += RotateVector90DegreeToLeft(&vInitialHeading) * fDistanceToGo;
}

void NtlGetDestination_Jump_R_TURN_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
	float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
	float fInitialHeadingVectorX, float fInitialHeadingVectorZ,
	DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
	CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = fCurrentHeadingVectorY;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	float fRadian = 2 * NTL_PI * ((float)dwDeltaTimeInMillisecs / (float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS);
	fRadian *= fTurningSpeedRatio;

	*pNewHeadingVector = RotateVector(pNewHeadingVector, fRadian);

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	CNtlVector vInitialHeading(fInitialHeadingVectorX, 0.0f, fInitialHeadingVectorZ);

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += RotateVector90DegreeToRight(&vInitialHeading) * fDistanceToGo;
}

void NtlGetDestination_Jump_R_TURN_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
	float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
	float fInitialHeadingVectorX, float fInitialHeadingVectorZ,
	DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
	CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = fCurrentHeadingVectorY;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	float fRadian = 2 * NTL_PI * ((float)dwDeltaTimeInMillisecs / (float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS);
	fRadian *= fTurningSpeedRatio;

	*pNewHeadingVector = RotateVector(pNewHeadingVector, -fRadian);

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	CNtlVector vInitialHeading(fInitialHeadingVectorX, 0.0f, fInitialHeadingVectorZ);

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += RotateVector90DegreeToRight(&vInitialHeading) * fDistanceToGo;
}


void NtlGetDestination_Mouse(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
								float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
								float fDestinationX, float fDestinationY, float fDestinationZ,
								DWORD dwDeltaTimeInMillisecs,
								CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	float fDeltaX = fDestinationX - fCurrentPositionX;
	float fDeltaZ = fDestinationZ - fCurrentPositionZ;

	//  [6/21/2006 zeroera] : 수정 : float 비교 오차 범위
	if (fabs(fDeltaX) < 0.001f && fabs(fDeltaZ) < 0.001f)
	{
		pNewHeadingVector->x = fCurrentHeadingVectorX;
		pNewHeadingVector->y = 0.0f;
		pNewHeadingVector->z = fCurrentHeadingVectorZ;

		pDestination->x = fDestinationX;
		pDestination->y = fDestinationY;
		pDestination->z = fDestinationZ;

		return;
	}

	CNtlVector vDelta(fDeltaX, 0.0f, fDeltaZ);
	float fDeltaLength = vDelta.Length();

	vDelta.Normalize(fDeltaLength);

	pNewHeadingVector->x = vDelta.x;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = vDelta.z;

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	if (fDeltaLength <= fDistanceToGo)
	{
		pDestination->x = fDestinationX;
		pDestination->y = fDestinationY;
		pDestination->z = fDestinationZ;
	}
	else
	{
		vDelta *= fDistanceToGo;

		pDestination->x = fCurrentPositionX + vDelta.x;
		pDestination->y = fCurrentPositionY + (fDestinationY - fCurrentPositionY) * fDistanceToGo / fDeltaLength;
		pDestination->z = fCurrentPositionZ + vDelta.z;
	}
}

void NtlGetDestination_Follow(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
								float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
								float fDestinationX, float fDestinationY, float fDestinationZ,
								DWORD dwDeltaTimeInMillisecs,
								float fTargetDistance,
								CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	float fDeltaX = fDestinationX - fCurrentPositionX;
	float fDeltaZ = fDestinationZ - fCurrentPositionZ;

	//  [6/21/2006 zeroera] : 수정 : float 비교 오차 범위
	if (fabs(fDeltaX) < 0.001f && fabs(fDeltaZ) < 0.001f)
	{
		pNewHeadingVector->x = fCurrentHeadingVectorX;
		pNewHeadingVector->y = 0.0f;
		pNewHeadingVector->z = fCurrentHeadingVectorZ;

		pDestination->x = fCurrentPositionX;
		pDestination->y = fCurrentPositionY;
		pDestination->z = fCurrentPositionZ;

		return;
	}

	CNtlVector vDelta(fDeltaX, 0, fDeltaZ);
	float fDeltaLength = vDelta.Length();

	vDelta.Normalize(fDeltaLength);

	pNewHeadingVector->x = vDelta.x;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = vDelta.z;

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	if (fDeltaLength <= fTargetDistance)
	{
		pDestination->x = fCurrentPositionX;
		pDestination->y = fCurrentPositionY;
		pDestination->z = fCurrentPositionZ;
	}
	else if (fDeltaLength > fTargetDistance && fDeltaLength < fTargetDistance + fDistanceToGo)
	{
		vDelta *= fDeltaLength - fTargetDistance * 0.99f;

		pDestination->x = fCurrentPositionX + vDelta.x;
		pDestination->y = fCurrentPositionY + (fDestinationY - fCurrentPositionY) * (fDeltaLength - fTargetDistance * 0.99f) / fDeltaLength;
		pDestination->z = fCurrentPositionZ + vDelta.z;
	}
	else
	{
		vDelta *= fDistanceToGo;

		pDestination->x = fCurrentPositionX + vDelta.x;
		pDestination->y = fCurrentPositionY + (fDestinationY - fCurrentPositionY) * fDistanceToGo / fDeltaLength;
		pDestination->z = fCurrentPositionZ + vDelta.z;
	}
}

void NtlGetDestination_Dash(float fCurrentMoveVectorX, float fCurrentMoveVectorZ, float fSpeedInSecs,
							float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
							float fDestinationX, float fDestinationY, float fDestinationZ,
							DWORD dwDeltaTimeInMillisecs,
							CNtlVector* pDestination)
{
	float fDeltaX = fDestinationX - fCurrentPositionX;
	float fDeltaZ = fDestinationZ - fCurrentPositionZ;

	//  [6/21/2006 zeroera] : Modify: float comparison tolerance
	if (fabs(fDeltaX) < 0.001f && fabs(fDeltaZ) < 0.001f)
	{
		pDestination->x = fDestinationX;
		pDestination->y = fDestinationY;
		pDestination->z = fDestinationZ;

		return;
	}

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	CNtlVector vDelta(fDeltaX, 0, fDeltaZ);
	float fDeltaLength = vDelta.Length();

	if (fDeltaLength <= fDistanceToGo)
	{
		pDestination->x = fDestinationX;
		pDestination->y = fDestinationY;
		pDestination->z = fDestinationZ;
	}
	else
	{
		pDestination->x = fCurrentPositionX + fCurrentMoveVectorX * fDistanceToGo;
		pDestination->y = fCurrentPositionY +(fDestinationY - fCurrentPositionY) * fDistanceToGo / fDeltaLength;
		pDestination->z = fCurrentPositionZ + fCurrentMoveVectorZ * fDistanceToGo;
	}
}

void NtlGetDestination_AirFollow(CNtlVector& svCurrentHeadingVector, float fSpeedInSecs,
	float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
	float fDestinationX, float fDestinationY, float fDestinationZ,
	DWORD dwDeltaTimeInMillisecs,
	float fTargetDistance,
	CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	float fDeltaX = fDestinationX - fCurrentPositionX;
	float fDeltaY = fDestinationY - fCurrentPositionY;
	float fDeltaZ = fDestinationZ - fCurrentPositionZ;

	//  [6/21/2006 zeroera] : 수정 : float 비교 오차 범위
	if (fabs(fDeltaX) < 0.001f && fabs(fDeltaZ) < 0.001f)
	{
		*pNewHeadingVector = svCurrentHeadingVector;

		pDestination->x = fCurrentPositionX;
		pDestination->y = fCurrentPositionY;
		pDestination->z = fCurrentPositionZ;

		return;
	}

	CNtlVector vDelta(fDeltaX, fDeltaY, fDeltaZ);
	float fDeltaLength = vDelta.Length();

	vDelta.Normalize(fDeltaLength);

	pNewHeadingVector->x = vDelta.x;
	pNewHeadingVector->y = vDelta.y;
	pNewHeadingVector->z = vDelta.z;

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	if (fDeltaLength <= fTargetDistance)
	{
		pDestination->x = fCurrentPositionX;
		pDestination->y = fCurrentPositionY;
		pDestination->z = fCurrentPositionZ;
	}
	else if (fDeltaLength > fTargetDistance && fDeltaLength < fTargetDistance + fDistanceToGo)
	{
		vDelta *= fDeltaLength - fTargetDistance * 0.99f;

		pDestination->x = fCurrentPositionX + vDelta.x;
		pDestination->y = fCurrentPositionY + vDelta.y;
		pDestination->z = fCurrentPositionZ + vDelta.z;
	}
	else
	{
		vDelta *= fDistanceToGo;

		pDestination->x = fCurrentPositionX + vDelta.x;
		pDestination->y = fCurrentPositionY + vDelta.y;
		pDestination->z = fCurrentPositionZ + vDelta.z;
	}
}

float NtlGetDistance(const float fPositionX1, const float fPositionY1, const float fPositionZ1, const float fPositionX2, const float fPositionY2, const float fPositionZ2)
{
	UNREFERENCED_PARAMETER(fPositionY1);
	UNREFERENCED_PARAMETER(fPositionY2);
	//return sqrtf((fPositionX1 - fPositionX2) * (fPositionX1 - fPositionX2) + (fPositionY1 - fPositionY2) * (fPositionY1 - fPositionY2) + (fPositionZ1 - fPositionZ2) * (fPositionZ1 - fPositionZ2));
	return sqrtf( (fPositionX1 - fPositionX2) * (fPositionX1 - fPositionX2) + (fPositionZ1 - fPositionZ2) * (fPositionZ1 - fPositionZ2) );
}
float NtlGetDistance(const sVECTOR3& vec1, const sVECTOR3& vec2)
{
	//return sqrtf((vec1.x - vec2.x) * (vec1.x - vec2.x) + (vec1.y - vec2.y) * (vec1.y - vec2.y) + (vec1.z - vec2.z) * (vec1.z - vec2.z));
	return sqrtf((vec1.x - vec2.x) * (vec1.x - vec2.x) + (vec1.z - vec2.z) * (vec1.z - vec2.z));
}

float NtlGetDistance(CNtlVector& vec1, CNtlVector& vec2)
{
	return sqrtf((vec1.x - vec2.x) * (vec1.x - vec2.x) + (vec1.z - vec2.z) * (vec1.z - vec2.z));
}

float NtlGetDistance3(CNtlVector& vec1, CNtlVector& vec2)
{
	return sqrtf((vec1.x - vec2.x) * (vec1.x - vec2.x) + (vec1.y - vec2.y) * (vec1.y - vec2.y) + (vec1.z - vec2.z) * (vec1.z - vec2.z));
}

bool NtlIsInsideOrNot(float fPositionX,float fPositionZ, float fDestinationX, float fDestinationZ, float fInputX, float fInputZ)
{
	if( (fInputX - fPositionX) * (fInputX - fDestinationX) <= 0 && 
		(fInputZ - fPositionZ) * (fInputZ - fDestinationZ) <= 0 )
		return true;
	
	return false;
}

bool IsDashPossible(BYTE byMoveDirection)
{
	if (byMoveDirection == NTL_MOVE_F || byMoveDirection == NTL_MOVE_B || byMoveDirection == NTL_MOVE_L || byMoveDirection == NTL_MOVE_R)
		return true;

	return false;
}

float Dbo_GetMaxDashDistanceForward(float fBaseRunSpeed, float fLastRunSpeed)
{
	return NTL_DASH_DISTANCE_F * (fLastRunSpeed / fBaseRunSpeed);
}

float Dbo_GetMaxDashDistanceBackwardLeftRight(float fBaseRunSpeed, float fLastRunSpeed)
{
	return NTL_DASH_DISTANCE_B * (fLastRunSpeed / fBaseRunSpeed);
}

float Dbo_GetDashSpeed(float fBaseRunSpeed, float fLastRunSpeed)
{
	return NTL_DASH_SPEED * (fLastRunSpeed / fBaseRunSpeed);
}

BYTE Dbo_MoveDirectionToJumpDirection(BYTE byMoveDirection, BYTE byDefaultDirection)
{
	BYTE byRetKey = byDefaultDirection;

	if (byMoveDirection == NTL_MOVE_F_TURN_L)
		byRetKey = NTL_MOVE_F_TURN_L_JUMP;
	else if (byMoveDirection == NTL_MOVE_F_TURN_R)
		byRetKey = NTL_MOVE_F_TURN_R_JUMP;
	else if (byMoveDirection == NTL_MOVE_B_TURN_L)
		byRetKey = NTL_MOVE_B_TURN_L_JUMP;
	else if (byMoveDirection == NTL_MOVE_B_TURN_R)
		byRetKey = NTL_MOVE_B_TURN_R_JUMP;
	else if (byMoveDirection == NTL_MOVE_L_TURN_L)
		byRetKey = NTL_MOVE_L_TURN_L_JUMP;
	else if (byMoveDirection == NTL_MOVE_L_TURN_R)
		byRetKey = NTL_MOVE_L_TURN_R_JUMP;
	else if (byMoveDirection == NTL_MOVE_R_TURN_L)
		byRetKey = NTL_MOVE_R_TURN_L_JUMP;
	else if (byMoveDirection == NTL_MOVE_R_TURN_R)
		byRetKey = NTL_MOVE_R_TURN_R_JUMP;

	return byRetKey;
}



#undef NTL_SQRT_OF_2
#undef NTL_PI
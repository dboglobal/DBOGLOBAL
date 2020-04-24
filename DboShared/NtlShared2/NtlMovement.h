#pragma once

#include "NtlVector.h"

#define NTL_DEFAULT_MOVEMENT_SPEED		(7.0f)
#define NTL_DEFAULT_MOVEMENT_SPEED_ADULT		(8.0f)

#define NTL_MAX_RADIUS_OF_VISIBLE_AREA		(100.0f)
#define NTL_MAX_NUMBER_OF_PLAYERS_IN_VISIBLE_AREA		(100)
#define NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS		(3000)
#define NTL_BACKWARD_MOVEMENT_SPEED_RATE		(0.5f)		// 50% of forward movement speed
#define NTL_DEFAULT_MOVEMENT_SPEED_B		(NTL_DEFAULT_MOVEMENT_SPEED * NTL_BACKWARD_MOVEMENT_SPEED_RATE)

#define NTL_DASH_DISTANCE_F		(15.0f)
#define NTL_DASH_DISTANCE_B		(10.0f)
#define NTL_DASH_SPEED			(25.0f)
#define NTL_MAX_DASH_DISTANCE_FOR_SKILL		(25.0f)
#define NTL_ACTIVE_DASH_SPEED				(40.0f)
#define DBO_DASH_INVERVAL_WITHOUT_EP_LOSS_IN_MILLISECS						(4000)

#define DBO_VEHICLE_TURNING_RATIO		(1.0f)		// no turning speed change when pc is on the vehicle
#define DBO_SWIMMING_SPEED_RATIO		(0.5f)		// 50% of movement speed on land.

#define DBO_FALLING_SPEED			(8.0f)		//

#define DBO_DISTANCE_CHECK_TOLERANCE		(10.0f)

const BYTE DBO_MAX_NEXT_DEST_LOC_COUNT = 20;
const BYTE DBO_AIR_MOVE_SYNC_SEND_STEP = 200;

enum ENtlMovementDirection
{
	NTL_MOVE_NONE = 0,

	NTL_MOVE_F,		// Forward
	NTL_MOVE_B,		// Backward
	NTL_MOVE_L,		// Left
	NTL_MOVE_R,		// Right

	NTL_MOVE_HEIGHT_UP,
	NTL_MOVE_HEIGHT_UP_F,
	NTL_MOVE_HEIGHT_UP_B,
	NTL_MOVE_HEIGHT_UP_L,
	NTL_MOVE_HEIGHT_UP_R,
	NTL_MOVE_HEIGHT_DOWN,

	NTL_MOVE_TURN_L,		// Turning Left
	NTL_MOVE_TURN_R,		// Turning Right

	NTL_MOVE_F_TURN_L,		// Forward + Turning Left
	NTL_MOVE_F_TURN_R,		// Forward + Turning Right
	NTL_MOVE_B_TURN_L,		// Backward + Turning Left
	NTL_MOVE_B_TURN_R,		// Backward + Turning Right
	NTL_MOVE_L_TURN_L,
	NTL_MOVE_L_TURN_R,
	NTL_MOVE_R_TURN_L,
	NTL_MOVE_R_TURN_R,

	NTL_MOVE_L_TURN_L_JUMP,
	NTL_MOVE_L_TURN_R_JUMP,
	NTL_MOVE_R_TURN_L_JUMP,
	NTL_MOVE_R_TURN_R_JUMP,
	NTL_MOVE_F_TURN_L_JUMP,		// Forward + Turning Left + Jump
	NTL_MOVE_F_TURN_R_JUMP,		// Forward + Turning Right + Jump
	NTL_MOVE_B_TURN_L_JUMP,		// Backward + Turning Left + Jump
	NTL_MOVE_B_TURN_R_JUMP,		// Backward + Turning Right + Jump

	NTL_MOVE_MOUSE_MOVEMENT,
	NTL_MOVE_FOLLOW_MOVEMENT,

	NTL_MOVE_COUNT,

	NTL_MOVE_UNKNOWN = 0xFF,

	NTL_MOVE_KEYBOARD_FIRST = NTL_MOVE_F,
	NTL_MOVE_KEYBOARD_LAST = NTL_MOVE_B_TURN_R_JUMP,

	NTL_MOVE_FIRST = NTL_MOVE_NONE,
	NTL_MOVE_LAST = NTL_MOVE_COUNT - 1,
};

enum ENtlMovementFlag
{
	NTL_MOVE_FLAG_WALK = 0,
	NTL_MOVE_FLAG_RUN,
	NTL_MOVE_FLAG_SWIM,

	NTL_MOVE_FLAG_FLY,
	NTL_MOVE_FLAG_FLY_DASH,
	NTL_MOVE_FLAG_FLY_ACCEL,
	NTL_MOVE_FLAG_FLY_QUICK_DOWN,	// fly fast down (like falling)

	NTL_MOVE_FLAG_JUMP,
	NTL_MOVE_FLAG_DASH,
	NTL_MOVE_FLAG_FALLING,

	NTL_MOVE_FLAG_INVALID = 0xFF,
};

enum eBOT_RETURNMODE
{
	BOT_RETURNMODE_WALK = 0x0,
	BOT_RETURNMODE_RUN = 0x1,
	BOT_RETURNMODE_FASTRUN = 0x2,
	INVALID_BOT_RETURNMODE = 0xFF,
};

enum eDBO_MOVE_FOLLOW_REASON
{
	DBO_MOVE_FOLLOW_FRIENDLY,
	DBO_MOVE_FOLLOW_AUTO_ATTACK,
	DBO_MOVE_FOLLOW_SKILL_ATTACK,
	DBO_MOVE_FOLLOW_HTB_ATTACK,

	DBO_MOVE_FOLLOW_COUNT,

	DBO_MOVE_FOLLOW_INVALID = 0xFF,
};

struct _3bytevalue
{
	BYTE first;
	WORD second;
};

struct _compressedLocation
{
	_3bytevalue x;
	_3bytevalue y;
	_3bytevalue z;
};
typedef _compressedLocation compLoc;

struct _compressedDirection
{
	SHORT x;
	SHORT y;
	SHORT z;
};
typedef _compressedDirection compDir;

void NtlLocationCompress(_compressedLocation *pResultLoc, const float fX, const float fY, const float fZ);
void NtlLocationDecompress(_compressedLocation *pLoc, float *pfResultX, float *pfResultY, float *pfResultZ);
void NtlDirectionCompress(_compressedDirection *pResultDir, const float fX, const float fY, const float fZ);
void NtlDirectionDecompress(_compressedDirection *pDir, float *pfResultX, float *pfResultY, float *pfResultZ);

float CalcDboAngle(CNtlVector *vFirst, CNtlVector *vSecond);
float CalcNormalAngle(float x1, float y1, float x2, float y2);


bool NtlSin(float fX, float fZ, float* pfSin);
bool NtlCos(float fX, float fZ, float* pfCos);

bool RotateVector45DegreeToLeft(float fX, float fZ, float* pfResultX, float* pfResultZ);
bool RotateVector45DegreeToRight(float fX, float fZ, float* pfResultX, float* pfResultZ);
bool RotateVector90DegreeToLeft(float fX, float fZ, float* pfResultX, float* pfResultZ);
bool RotateVector90DegreeToRight(float fX, float fZ, float* pfResultX, float* pfResultZ);
bool RotateVector135DegreeToLeft(float fX, float fZ, float* pfResultX, float* pfResultZ);
bool RotateVector135DegreeToRight(float fX, float fZ, float* pfResultX, float* pfResultZ);
bool RotateVector180Degree(float fX, float fZ, float* pfResultX, float* pfResultZ);
bool RotateVector(float fX, float fZ, float fAngleInRadian, float* pfResultX, float* pfResultZ);
bool GetVectorWithDegree(int nDegree, float* pfVectorX, float* pfVectorZ);
float ConvertDegreeToRadian(int nDegree);

float GetDegreeToRadian(int nDegree);

CNtlVector& RotateVector45DegreeToLeft(CNtlVector* pVector);
CNtlVector& RotateVector45DegreeToRight(CNtlVector* pVector);
CNtlVector& RotateVector90DegreeToLeft(CNtlVector* pVector);
CNtlVector& RotateVector90DegreeToRight(CNtlVector* pVector);
CNtlVector& RotateVector135DegreeToLeft(CNtlVector* pVector);
CNtlVector& RotateVector135DegreeToRight(CNtlVector* pVector);
CNtlVector& RotateVector180Degree(CNtlVector* pVector);
CNtlVector& RotateVector(CNtlVector* pVector, float fAngleInRadian);
void RotateVector(CNtlVector &rResult, CNtlVector* pVector, float fAngleInRadian);

bool NtlGetDestination(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
						float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
						float fDestinationX, float fDestinationY, float fDestinationZ,
						BYTE byMoveDirection, DWORD dwDeltaTimeInMillisecs,
						float fAttackDistance,
						float* pfNewHeadingVectorX, float* pfNewHeadingVectorZ,
						float* pfDestinationX, float* pfDestinationY, float* pfDestinationZ,
						float fTurningSpeedRatio);

// It's assumed that byMoveDirecton is valid. Please check if it has valid value before calling this function.
// by YOSHIKI(2006-07-06)
void NtlGetDestination_Keyboard(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
								float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
								BYTE byMoveDirection, DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
								CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_NONE(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
										float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
										DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
										CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_F(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_B(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination);

void NtlGetDestination_Keyboard_HEIGHT_UP(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_HEIGHT_UP_F(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_HEIGHT_UP_B(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_HEIGHT_UP_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_HEIGHT_UP_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_HEIGHT_DOWN(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination);

void NtlGetDestination_Keyboard_F_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_F_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_B_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_B_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_TURN_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
										float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
										DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
										CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_TURN_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
										float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
										DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
										CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_F_TURN_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
											float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
											DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
											CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_F_TURN_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
											float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
											DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
											CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_B_TURN_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
											float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
											DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
											CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_B_TURN_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
											float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
											DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
											CNtlVector* pNewHeadingVector, CNtlVector* pDestination);

void NtlGetDestination_Keyboard_L_TURN_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
											float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
											DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
											CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_L_TURN_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
											float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
											DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
											CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_R_TURN_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
											float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
											DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
											CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_R_TURN_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
											float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
											DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
											CNtlVector* pNewHeadingVector, CNtlVector* pDestination);


void NtlGetDestination_Jump(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
							float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
							float fJumpDirectionX, float fJumpDirectionZ,
							BYTE byMoveDirection, DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
							CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Jump_NONE(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									float fJumpDirectionX, float fJumpDirectionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Jump_F(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
								float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
								float fJumpDirectionX, float fJumpDirectionZ,
								DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
								CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Jump_B(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
								float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
								float fJumpDirectionX, float fJumpDirectionZ,
								DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
								CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Jump_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
								float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
								float fJumpDirectionX, float fJumpDirectionZ,
								DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
								CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Jump_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
								float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
								float fJumpDirectionX, float fJumpDirectionZ,
								DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
								CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Jump_TURN_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									float fJumpDirectionX, float fJumpDirectionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Jump_TURN_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									float fJumpDirectionX, float fJumpDirectionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Jump_F_TURN_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
										float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
										float fJumpDirectionX, float fJumpDirectionZ,
										DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
										CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Jump_F_TURN_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
										float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
										float fJumpDirectionX, float fJumpDirectionZ,
										DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
										CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Jump_B_TURN_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
										float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
										float fJumpDirectionX, float fJumpDirectionZ,
										DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
										CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Jump_B_TURN_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
										float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
										float fJumpDirectionX, float fJumpDirectionZ,
										DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
										CNtlVector* pNewHeadingVector, CNtlVector* pDestination);

void NtlGetDestination_Jump_L_TURN_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
										float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
										float fJumpDirectionX, float fJumpDirectionZ,
										DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
										CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Jump_L_TURN_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
										float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
										float fJumpDirectionX, float fJumpDirectionZ,
										DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
										CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Jump_R_TURN_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
										float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
										float fJumpDirectionX, float fJumpDirectionZ,
										DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
										CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Jump_R_TURN_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorY, float fCurrentHeadingVectorZ, float fSpeedInSecs,
										float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
										float fJumpDirectionX, float fJumpDirectionZ,
										DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
										CNtlVector* pNewHeadingVector, CNtlVector* pDestination);

void NtlGetDestination_Mouse(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
								float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
								float fDestinationX, float fDestinationY, float fDestinationZ,
								DWORD dwDeltaTimeInMillisecs,
								CNtlVector* pNewHeadingVector, CNtlVector* pDestination);

void NtlGetDestination_Follow(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
								float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
								float fDestinationX, float fDestinationY, float fDestinationZ,
								DWORD dwDeltaTimeInMillisecs,
								float fTargetDistance,
								CNtlVector* pNewHeadingVector, CNtlVector* pDestination);

void NtlGetDestination_Dash(float fCurrentMoveVectorX, float fCurrentMoveVectorZ, float fSpeedInSecs,
							float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
							float fDestinationX, float fDestinationY, float fDestinationZ,
							DWORD dwDeltaTimeInMillisecs,
							CNtlVector* pDestination);

void NtlGetDestination_AirFollow(CNtlVector& svCurrentHeadingVector, float fSpeedInSecs,
							float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
							float fDestinationX, float fDestinationY, float fDestinationZ,
							DWORD dwDeltaTimeInMillisecs,
							float fTargetDistance,
							CNtlVector* pNewHeadingVector, CNtlVector* pDestination);

float NtlGetDistance(const float fPositionX1, const float fPositionY1, const float fPositionZ1, const float fPositionX2, const float fPositionY2, const float fPositionZ2);
float NtlGetDistance(const sVECTOR3& vec1, const sVECTOR3& vec2);
float NtlGetDistance(CNtlVector& vec1, CNtlVector& vec2);
float NtlGetDistance3(CNtlVector& vec1, CNtlVector& vec2);
bool NtlIsInsideOrNot(float fPositionX,float fPositionZ, float fDestX, float fDestZ, float fInputX, float fInputZ);

bool IsDashPossible(BYTE byMoveDirection);
float Dbo_GetMaxDashDistanceForward(float fBaseRunSpeed, float fLastRunSpeed);
float Dbo_GetMaxDashDistanceBackwardLeftRight(float fBaseRunSpeed, float fLastRunSpeed);
float Dbo_GetDashSpeed(float fBaseRunSpeed, float fLastRunSpeed);

BYTE	Dbo_MoveDirectionToJumpDirection(BYTE byMoveDirection, BYTE byDefaultDirection);
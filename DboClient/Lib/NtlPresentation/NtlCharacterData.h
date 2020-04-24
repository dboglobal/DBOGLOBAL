/*****************************************************************************
 *
 * File			: NtlCharacterData.h
 * Author		: HongHoDong
 * Copyright	: (주)NTL
 * Date			: 2006. 4. 11	
 * Abstract		: NTL NtlCharacterData
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_CHARACTER_DATA_H_
#define __NTL_CHARACTER_DATA_H_

#include "GraphicDataTable.h"
#include "NtlSharedType.h"
#include "NtlSerializer.h"

//#define ANIMATION_NONE	INVALID_ANIMATIONID
const RwUInt32 INVALID_GRAPHIC_ANIMATION_ID = 0xFFFFFFFF;
#define MAX_NUM_SCALE_BONE	90

struct STypeScaleBone
{
	RwReal	fLength;									///< Bone의 길이
	RwReal	fWidth;										///< Bone의 두께
	RwV3d	vAngle;										///< Bone의 각도(현재는 미사용)
	
	STypeScaleBone() : fLength(1.f), fWidth(1.f)
	{
		memset(&vAngle, 0, sizeof(vAngle));
	}
};

struct STypeBoneData
{
	RwBool			bBoneScale;							///< BoneScale을 적용을 할것인지
	RwV3d			vPosOffset;							///< 최상위 Bone 위치 Offset
	RwInt32			nBoneCount;                         ///< Bone의 갯수
	STypeScaleBone	BoneScale[MAX_NUM_SCALE_BONE];		///< Bone Scale Data


	RwBool			bBaseScale;							///< Base Scale를 적용을 할것인지
	RwV3d			vBaseScale;							///< Base Scale

	STypeBoneData() : bBoneScale(FALSE),
					  bBaseScale(FALSE),
					  nBoneCount(0)
	{
		vPosOffset.x = 0.f;
		vPosOffset.y = 0.f;
		vPosOffset.z = 0.f;

		vBaseScale.x = 1.f;
		vBaseScale.y = 1.f;
		vBaseScale.z = 1.f;
	}

	// 최적화를 위해서 Bone Scale이나 Base가 적용되어 있을때만 저장을 한다.
	void SaveSerialize(CNtlSerializer& sOut)
	{
		// Bone Scale
		sOut<<bBoneScale;
		sOut<<bBaseScale;
		sOut<<nBoneCount;
		sOut.In(&vPosOffset, sizeof(RwV3d));		

		if(bBoneScale)
		{
			sOut.In(BoneScale, sizeof(STypeScaleBone) * nBoneCount);
		}		

		if(bBaseScale)
		{
			sOut.In(&vBaseScale, sizeof(RwV3d));
		}
	}

	void LoadSerialize(CNtlSerializer& sIn)
	{
		sIn>>bBoneScale;
		sIn>>bBaseScale;
		sIn>>nBoneCount;		
		sIn.Out(&vPosOffset, sizeof(RwV3d));

		if(bBoneScale)
		{
			sIn.Out(BoneScale, sizeof(STypeScaleBone) * nBoneCount);
		}

		if(bBaseScale)
		{
			sIn.Out(&vBaseScale, sizeof(RwV3d));
		}
	}
};

#define MAX_CHAR_EFFECT_BONE_NAME	32
struct STypeCharEffectData
{
	char	chBoneName[MAX_CHAR_EFFECT_BONE_NAME];
	
};


enum ECharWorkState
{
	CHAR_WORK_STATE_NONE,								
	CHAR_WORK_STATE_RES_LOAD_END,					// Res Load 완료
};

struct SCharScheduleResInfo
{
	RwBool		bVisible;								// 화면에 보여주는지 아닌지
	RwBool		bLoadComplete;
	
	RwUInt32	uiAnimKey;
	RwReal		fAnimElapsedTime;
	RwBool		bAnimLoop;

	RwUInt32	uiSerialId;
	RwBool		bPCFlag;
	RwUInt32	uiClass;
	RwUInt32	uiRace;
	RwUInt32	uiGender;
	RwUInt32	uiHeadType;
	RwUInt32	uiFaceType;
	RwUInt32	uiHeadColorType;
	RwUInt32	uiSkinColorType;
	RwBool		bNotShadingFlag;    
    RwBool      bIsAdult;
	
	SCharScheduleResInfo() : bVisible(TRUE),
							 bLoadComplete(FALSE),
							 uiAnimKey(NML_IDLE_LOOP),
							 fAnimElapsedTime(0.f),
							 bAnimLoop(FALSE),                             
							 bNotShadingFlag(FALSE),
                             bPCFlag(FALSE),
                             bIsAdult(FALSE) {}
};

#endif
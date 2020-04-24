

#ifndef _NTLEFFECT_DEFINE_
#define _NTLEFFECT_DEFINE_

#include "NtlSoundDefines.h"
#include "NtlMath.h"

#include <string>

class CNtlInstanceEffect;


#define EFFECT_FILE_EXE			"eff"
#define EFFECT_FILE_HEADER		"effect"
#define EFFECT_FILE_VERSION		104

const RwV3d VEC_X = { 1.f, 0.f, 0.f };
const RwV3d VEC_Y = { 0.f, 1.f, 0.f };
const RwV3d VEC_Z = { 0.f, 0.f, 1.f };

const RwRGBA RGBA_BASE = { 255, 255, 255, 255 };

#define NTLPRTSTD_SRAND_MAX                    0xFFFFFFFF
#define NTLPRTSTD_SRAND_IMAX                   ((RwReal) 1.0 / (RwReal) NTLPRTSTD_SRAND_MAX)

#define NTLPRTSTD_SRAND(_seed)                 ((_seed) = ((_seed) * 196314165 + 907633515))
#define NTLPRTSTD_RSRAND(_seed)                (((RwReal)((RwUInt32) (_seed) * NTLPRTSTD_SRAND_IMAX) * (RwReal) 2.0) - (RwReal) 1.0)

#define EFFECT_NAME_TRACE       "TraceEffect"
#define EFFECT_NAME_GRADE       "GradeEffect"

// Blur Effect 설정
#define EFFECT_BLUR_TEXTURE_WIDTH   512
#define EFFECT_BLUR_TEXTURE_HEIGHT  384

#define EFFECT_BLUR_ALPHA   100         ///< Blur Alpha 값 (0 ~ 255)


// Predefined flags in renderware.
// Custom flags should use the following flags except for the following flags.
/*
#define rpPRTSTDEMITTERDATAFLAGSTANDARD                     0x00000001
#define rpPRTSTDEMITTERDATAFLAGPRTCOLOR                     0x00000002
#define rpPRTSTDEMITTERDATAFLAGPRTTEXCOORDS                 0x00000004
#define rpPRTSTDEMITTERDATAFLAGPRT2DROTATE                  0x00000008
#define rpPRTSTDEMITTERDATAFLAGPRTSIZE                      0x00000010
#define rpPRTSTDEMITTERDATAFLAGPTANK                        0x00000020
#define rpPRTSTDEMITTERDATAFLAGPRTMATRIX                    0x00000040

#define rpPRTADVEMITTERDATAFLAGPRTEMITTER                   0x00000100
#define rpPRTADVEMITTERDATAFLAGMULTICOLOR                   0x00000200
#define rpPRTADVEMITTERDATAFLAGMULTITEXCOORDS               0x00000400
#define rpPRTADVEMITTERDATAFLAGMULTITEXCOORDSSTEP           0x00000800
#define rpPRTADVEMITTERDATAFLAGMULTISIZE                    0x00001000

#define rpPRTADVEMITTERDATAFLAGPOINTLIST                    0x00010000
#define rpPRTADVEMITTERDATAFLAGCIRCLE                       0x00020000
#define rpPRTADVEMITTERDATAFLAGSPHERE                       0x00030000

*/

// 사용자 정의 Flag
#define rpPRTSTDEMITTERDATAFLAGPRTORBITPOINT				0x00000080

#define rpPRTADVEMITTERDATAFLAGMULTIROTATE					0x00002000
#define rpPRTSTDEMITTERDATAFLAGTRAIL                        0x00004000                  ///< Trail Action 플래그 
#define rpPRTSTDEMITTERDATAFLAGUVANIM						0x00008000                  ///< UV Anim 플래그 (Decal에서 사용)

#define rpPRTADVEMITTERDATAFLAGMULTITEXTURES				0x01000000
#define rpPRTSTDEMITTERDATAFLAGPRTHURRICANE					0x02000000
#define rpPRTSTDEMITTERDATAFLAGPRTFADEOUT					0x04000000

#define rpPRTADVEMITTERDATAFLAGSPLINEPATH					0x10000000
#define rpPRTSTDEMITTERDATAFLAGPRTFINISH                    0x40000000

// 파티클 전용 정의 Flag
#define NTLrpPRTADVEMITTERDATAFLAGFOLLOW                    0x80000000                  ///< 생성 위치를 따라다니는 파티클 플래그
#define NTLrpPRTSTDEMITTERDATAFLAGPRTYBILLBOARD             0x00100000                  ///< 파티클의 Y빌보드 플래그
#define NTLrpPRTSTDEMITTERDATAFLAGPRTXBILLBOARD             0x00200000                  ///< 파티클의 X빌보드 플래그
#define NTLrpPRTSTDEMITTERDATAFLAGPRTZBILLBOARD             0x00400000                  ///< 파티클의 Z빌보드 플래그
#define NTLrpPRTSTDEMITTERDATAFLAGPRTZWRITE					0x00800000					///< 파티클에서 Z Write 유무 플래그

// 포스트 이펙트 전용 Flag
#define NTLrpPRTSTDEMITTERDATAFLAGPRTCENTERFIX			    0x08000000					///< 파티클에서 Z Write 유무 플래그

// Fade In/Out시 사용되는 플래그
enum EFadeStatus
{
	FADE_STATUS_IN,
	FADE_STATUS_ING,
	FADE_STATUS_OUT,
	FADE_STATUS_OUT_ING,
	FADE_STATUS_IN_ING,	
};

struct SNtlPrtStdEmitterPrtOrbitPoint
{
	RwV3d point;
};

struct SNtlPrtEmitterSplinePath
{
	RwChar			strName[MAX_PATH];

	RwBool			loop;
	RwBool			useRotate;

	RwV3d			vPosition;
	RwV3d			vRotate;

	RwReal			rwUnknown;
};

struct SNtlPrtStdEmitterPrtSize
{
    RwV3d                       prtStartSize,
                                prtStartSizeBias;
    RwV3d                       prtEndSize,
                                prtEndSizeBias;

	RwReal						rwUnknown;
};

struct SNtlPrtStdEmitterPrtRotate
{
    RwV3d                       prtStartRotate,
                                prtStartRotateBias;
    RwV3d                       prtEndRotate,
                                prtEndRotateBias;

    SNtlPrtStdEmitterPrtRotate()
    {
        ZeroMemory(&prtStartRotate, sizeof(RwV3d));
        ZeroMemory(&prtStartRotateBias, sizeof(RwV3d));
        ZeroMemory(&prtEndRotate, sizeof(RwV3d));
        ZeroMemory(&prtEndRotateBias, sizeof(RwV3d));
    }
};

struct SNtlAdvEmtPrtSizeItem
{
	RwReal                              time;           /**<Time.*/
	RwReal                              timeBias;       /**<Time bias.*/
	RwV3d                               midSize;        /**<Size.*/
	RwV3d                               midSizeBias;    /**<Size bias.*/
};
struct SNtlAdvMultiSizeEmitter
{
	RwUInt32                            numSize;
	SNtlAdvEmtPrtSizeItem               *list;    
};

struct SNtlAdvEmtPrtRotateItem
{
    RwReal                              time;           /**<Time.*/
    RwReal                              timeBias;       /**<Time bias.*/
    RwV3d                               midRotate;      /**<Size.*/
    RwV3d                               midRotateBias;	/**<Size bias.*/
};
struct SNtlAdvMultiRotateEmitter
{
    RwUInt32							numRotate;
    SNtlAdvEmtPrtRotateItem*			list;

    SNtlAdvMultiRotateEmitter() : list(NULL),
                                  numRotate(0)
    {

    }
};


struct SNtlPrtStdEmitterPrtHurricane
{
	RwReal			fSpeed;
    RwReal			radius;
	RwReal			revolutionCount;
	RwReal			revolutionAngle;
};

struct SNtlPrtStdEmitterPrtFadeOut
{
	RwReal			time;
	RwBool			follow;
	RwChar			strFollowSystem[MAX_PATH];
};

/// UV Animation 관련 프로퍼티 구조체 (Decal에서 사용한다)
struct SNtlPrtStdEmitterPrtUVAnim
{
	RwV3d			vDir;			///< UV Anim방향 (X, Z 값을 사용)
	RwReal			fVelocity;		///< UV Anim 속도
	RwReal			fMaxOffset;		///< Max Offset 값 (이 값을 설정하면 Offset 까지 왕복하는 UV Anim이 된다)
	
	SNtlPrtStdEmitterPrtUVAnim()
	{
		vDir.x = 1.0f;
		vDir.y = vDir.z = 0.0f;
		fVelocity = 0.0f;
		fMaxOffset = 0.0f;
	}
};

struct RpPrtAdvEmtPrtTextureItem
{
    RwReal                              time;
    RwReal                              timeBias;

	std::string							strTexture;
};

struct RpPrtAdvEmtPrtMultiTextures
{
    RwReal                              time;
	RwBool								bLoop;
    RwUInt32							numTextures;
    RpPrtAdvEmtPrtTextureItem			*list;
};

struct SNtlPostEffectEmitterStandard
{
	RwBool			bDrawFullScreen;
	RwInt32			nLoopCount;
    RwInt32         nVertexCount;               ///< Post Effect의 Vertex 개수
    RwReal          fRadius;                    ///< 반지름 (길이)

    SNtlPostEffectEmitterStandard()
    : nVertexCount(16), nLoopCount(1), bDrawFullScreen(FALSE), fRadius(512.0f)
    {
    }
};

struct SNtlMeshEmitterStandard
{
	RwV3d			prtSize;
	RwRGBA			prtColor;
	RwV3d			prtRotate;
	RwBool			bWriteEnable;

	SNtlMeshEmitterStandard()
	{
		prtSize = ZeroAxis;
		ZeroMemory(&prtColor, sizeof(RwRGBA));
		prtRotate = ZeroAxis;
		bWriteEnable = FALSE;
	}
};

struct SNtlBeamEmitterStandard
{
	RwReal			fSpeed;
	RwV3d			prtSize;
	RwRGBA			prtColor;

	RwChar			strHeadSystem[MAX_PATH];
};

struct SNtlLightningEmitterStandard
{
	RwRGBA			prtColor;
	RwV3d			prtSize;		// X = start width, Y = end width, Z = center width

	RwReal			fAmp;
	RwReal			fRough;

	RwReal			fUpdateTime;
};

/// Decal System의 정보를 가지고 있는 구조체
struct SNtlDecalEmitterStandard_Old
{
	RwTextureAddressMode 			eWrapType;			///< Texture의 Wrap Type
	RwReal							fVisibleDistance;	///< Decal Effect가 보이는 시야 거리
	RwV3d							vSize;				///< Decal의 Size
	RwReal							fScale;				///< Decal Effect의 Scale값    
	
	SNtlDecalEmitterStandard_Old()
	: eWrapType(rwTEXTUREADDRESSCLAMP), fVisibleDistance(40.0f),
	  fScale(1.0f)
	{	
		vSize.x = vSize.y = vSize.z = 1.0f;
	}
};

/// Y Offset 값을 추가한 데이터 (스크립트 버전 8에 추가)
struct SNtlDecalEmitterStandard
{
    RwTextureAddressMode 			eWrapType;			///< Texture의 Wrap Type
    RwReal							fVisibleDistance;	///< Decal Effect가 보이는 시야 거리
    RwV3d							vSize;				///< Decal의 Size
    RwReal							fScale;				///< Decal Effect의 Scale값   
    RwReal                          fYOffset;           ///< Y Offset
    RwRGBA                          color;              ///< 기본 컬러값

    SNtlDecalEmitterStandard()
    : eWrapType(rwTEXTUREADDRESSCLAMP), fVisibleDistance(40.0f),
      fScale(1.0f)
    {
        vSize.x = vSize.y = vSize.z = 1.0f;
        fYOffset = 0.02f;
        color.red = color.green = color.blue = color.alpha = 255;
    }
};

struct SNtlHurricaneEmitterStandard
{
	RwReal			fSpeed;
	RwV3d			prtSize;
	RwRGBA			prtColor;

	RwInt32			maxPolygonCount;

	RwBool			bEternity;
	RwReal			lifeTime;

    RwReal			radius;
    RwReal			radiusGap;

	RwInt32			revolutionCount;

	RwReal			revolutionAngle;
};

/// Line System의 기본 속성값들
struct SNtlLineEmitterStandard
{
    // 프로퍼티 속성들
    RwReal  m_fLineLifeTime;            ///< 라인의 LifeTime
    RwInt32 m_nMaxCount;                ///< 라인의 최대 생성 개수
    RwInt32 m_nEmitterCount;            ///< 한 Frame에 생성되는 라인의 개수
    RwReal  m_fEmitterGap;              ///< Emitter에서 평면이 생성되는 Gap
    RwReal  m_fEmitterRaduis;           ///< Emitter의 반지름
    RwReal  m_fEmitterBias;             ///< Emitter의 Bias값
    RwReal  m_fTargetRadius;            ///< 라인이 이동할 Target의 반지름
    RwV2d   m_sizeLine;                 ///< 생성되는 Line의 크기 (x: 넓이, y:길이)
    RwReal  m_fVelocity;                ///< 라인이 이동하는 속도    
    RwBool  m_bShake;                   ///< 라인의 흔들림 여부
    RwReal  m_fShakeBias;               ///< 흔들림 값
    RwBool  m_bMoveLine;                ///< 라인이 Dir방향으로 이동할지, 단지 늘어나기만 할지 여부
    RwBool  m_bZBufferEnable;           ///< Z-Buffer 사용 유무
    RwBool  m_bZBiilBoard;              ///< Z-BillBoard 적용 유무

    SNtlLineEmitterStandard() : m_fLineLifeTime(1.0f),
                                m_nMaxCount(100),
                                m_nEmitterCount(1),
                                m_fEmitterGap(0.03f),
                                m_fEmitterRaduis(0.0f),
                                m_fEmitterBias(0.0f),
                                m_fTargetRadius(1.0f),
                                m_fVelocity(50.0f),
                                m_bShake(FALSE),
                                m_fShakeBias(0.1f),
                                m_bMoveLine(TRUE),
                                m_bZBufferEnable(TRUE),
                                m_bZBiilBoard(TRUE)
    {
        m_sizeLine.x = 0.2f;
        m_sizeLine.y = 10.0f;
    }
};

// 구형 Emitter 프로퍼티 (타원형도 설정가능, Line System의 Emitter에 사용된다)
struct SNtlPrtStdEmitterPrtSphere
{
    RwV3d   m_vEmitterSize;    

    SNtlPrtStdEmitterPrtSphere() 
    {
        m_vEmitterSize.x = 1.0f;
        m_vEmitterSize.y = 1.0f;
        m_vEmitterSize.z = 1.0f;
    }
};

// Effect의 Finish 타입 플래그
enum EPrtFinishType
{
    FINISH_TYPE_ALPHA   = 1,      ///< 알파가 없어지면서 사라진다.
    FINISH_TYPE_SIZE    = 2,      ///< 사이즈가 작아지면서 사라진다.
    FINISH_TYPE_FOLLOW  = 4,      ///< 끝점을 따라서 첫점이 이동하면서 길이가 작아지면서 사라진다.
};

// Finish Action에 사용될 프로퍼티
struct SNtlPrtStdEmitterPrtFinish
{
    RwInt32        m_nFinishTypeFlag;
    RwReal         m_fFinishTime;       ///< Finish 작업에 걸리는 시간

    SNtlPrtStdEmitterPrtFinish()
    {
        m_nFinishTypeFlag = FINISH_TYPE_ALPHA;
        m_fFinishTime = 0.5f;
    }
};


/// Trace Action에 사용될 프로퍼티 구조체
struct SNtlPrtStdEmitterTrail
{
    RwReal  fEdgeLifeTime;            ///< Edge의 Life Time       
    RwReal  fMaxLength;               ///< 궤적의 최대 길이
    RwReal  fWidth;                   ///< 궤적의 넓이  
    RwChar  strTexture[MAX_PATH];     ///< Texture File Name
    RwReal  fEdgeGap;                 ///< The value of whether Edge should be generated every few seconds
    RwInt32 nSplinePointCount;        ///< Spline으로 생성하는 버텍스의 갯수 (두개의 Edge사이에 생성되는 버텍스의 개수이다)
    RwInt32 nMaxEdgeCount;            ///< 최대 Edge Count    
    RwBlendFunction eSrcBlend;        ///< Src Blend State
    RwBlendFunction eDestBlend;       ///< Dest Blend State
    RwRGBA  colStartColor;            ///< 시작 Color값
    RwRGBA  colEndColor;              ///< 끝 Color값   
    RwV3d   vOffset;                  ///< Offset from center point where trajectory will occur

    SNtlPrtStdEmitterTrail() : fEdgeLifeTime(0.3f),
                    fEdgeGap(0.003f),	// by daneos from 0.0 to 0.003
                    nSplinePointCount(10),
                    nMaxEdgeCount(500), // by daneos from 100 to 500
                    fMaxLength(1.0f),
                    fWidth(0.5f)
    {
        eSrcBlend = rwBLENDSRCALPHA;
        eDestBlend = rwBLENDONE;                       
        ZeroMemory(&vOffset, sizeof(RwV3d));
        colStartColor.red = colStartColor.green = colStartColor.blue = 255;
        colEndColor.red = colEndColor.green = colEndColor.blue = 255;
        colStartColor.alpha = 255;
        colEndColor.alpha = 0;
        ZeroMemory(strTexture, MAX_PATH);
    }
};

/// 이펙트 사운드 구조체
struct SEffectSound
{
    RwChar	chSoundName[128]; ///< Sound 파일명 (Sound 폴더및 경로명 포함)
    RwChar	chSoundName2[128]; ///< Sound 파일명 (Sound 폴더및 경로명 포함)
    RwChar	chSoundName3[128]; ///< Sound 파일명 (Sound 폴더및 경로명 포함)
    RwChar	chSoundName4[128]; ///< Sound 파일명 (Sound 폴더및 경로명 포함)    
    RwBool  bLoop;                              ///< Sound Loop 유무
    RwReal  fSoundVolume;                       ///< Sound Volume
    RwReal  fSoundDist;                         ///< 유효 거리
    RwReal  fSoundDecayDist;                    ///< 사운드 감쇠 거리
    RwReal  fSoundPitchMin;                     ///< 사운드 피치 최소값    

    SEffectSound()
    {
        ZeroMemory(chSoundName, sizeof(chSoundName));
        ZeroMemory(chSoundName2, sizeof(chSoundName2));
        ZeroMemory(chSoundName3, sizeof(chSoundName3));
        ZeroMemory(chSoundName4, sizeof(chSoundName4));        
        bLoop = FALSE;
        fSoundVolume = 100.0f;
        fSoundDist = 4.0f;
        fSoundDecayDist = 18.0f;
        fSoundPitchMin = dNTLSOUND_PITCH_DEFAULT;        
    }
};

struct SEffectSoundEx : public SEffectSound
{
    RwReal fSoundPitchMax;

    SEffectSoundEx()
    {
        fSoundPitchMax = dNTLSOUND_PITCH_DEFAULT;
    }
};

/// 이펙트 바운딩 스피어
struct SEffectBoundingSphere
{
    RwBool      bDisableAuto;           ///< 바운딩 스피어를 수동으로 조절할때 True, False면 자동생성된 바운딩 스피어 사용
    RwReal      fRadius;                ///< 바운딩 스피어 반지름. 수동 설정일때만 유효

    SEffectBoundingSphere()
    {
        bDisableAuto = FALSE;
        fRadius = 5.0f;
    }
};

/**
* \ingroup NtlPresentation
* \brief LoopEffect List에 사용되는 구조체, Effect-Bone 쌍으로 구현된다.
* \date 2006-08-21
* \author agebreak
*/
struct SLoopEffect
{
	CNtlInstanceEffect* pLoopEffectIntance;     ///< LoopEffect의 포인터
	char*               szBoneName;             ///< LoopEffect가 붙는 Bone의 이름
};

typedef std::list<SLoopEffect*> ListLoopEffect;
typedef std::list<SOUND_HANDLE> ListSoundHandle;

/// 바닥에서 표시되는 이펙트 시스템을 설정하기 위한 구조체
struct SGroundFlag
{
    RwBool bGround;                             ///< 지면의 높이에서 표시될지의 유무 플래그
    RwReal fOffsetY;                            ///< Y Offset 설정

    SGroundFlag()
    {
        bGround = FALSE;
        fOffsetY = 0.0f;
    }
};

#endif
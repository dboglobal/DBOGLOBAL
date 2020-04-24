#pragma once

#define dPL_WEATHER_MAX_COUNT			5
#define dPL_WEATHER_CHAR_BUFFER_SMALL	64
#define dPL_WEATHER_CHAR_BUFFER_MIDDLE	128
#define dPL_WEATHER_CHAR_BUFFER_LARGE	256

#define dPL_WEATHER_COLLISION_EFFECT_WATER "_WATER"

#define dPL_WEATHER_RANDOM(R)			NtlRandomNumber(-(R), (R))

enum EPLWEATHER_FADE_STATE
{
	EPLWEATHER_FADE_NONE = 0,
	EPLWEATHER_FADE_OUT,
	EPLWEATHER_FADE_IN,
	EPLWEATHER_FADE_OUT_DONE,
	EPLWEATHER_FADE_IN_DONE,
};

enum EPLWEATHER_UPDATE_FLAG
{
	EPLWEATHER_UPDATE_MATRIX	= 0x00000001,
	EPLWEATHER_UPDATE_COLOR		= 0x00000002,
};

enum EPLWEATHER_EMITTER_PLACE_TYPE
{
	EPLWEATHER_EMITTER_PLACE_CAMERA_POS,
	EPLWEATHER_EMITTER_PLACE_CAMERA_AT,
};

enum EPLWEATHER_PARTICLE_MOTION_TYPE
{
	EPLWEATHER_PARTICLE_MOTION_STATIC,
	EPLWEATHER_PARTICLE_MOTION_TSNAP,
	EPLWEATHER_PARTICLE_MOTION_LINE,
	EPLWEATHER_PARTICLE_MOTION_SPLINE,
};

enum EPLWEATHER_PARTICLE_BILLBOARD_TYPE
{
	EPLWEATHER_PARTICLE_BILLBOARD_N,
	EPLWEATHER_PARTICLE_BILLBOARD_Y,
};

enum EPLWEATHER_PARTICLE_BLENDING_TYPE
{
	EPLWEATHER_PARTICLE_BLENDING_N,
	EPLWEATHER_PARTICLE_BLENDING_Y,
};

enum EPLWEATHER_PARTICLE_COLLISION_FLAG
{
	EPLWEATHER_PARTICLE_COLLISION_NONE		= 0x00000000,
	EPLWEATHER_PARTICLE_COLLISION_WATER		= 0x00000001,
	EPLWEATHER_PARTICLE_COLLISION_TERRAIN	= 0x00000002,
	EPLWEATHER_PARTICLE_COLLISION_OBJECT	= 0x00000004,
	EPLWEATHER_PARTICLE_COLLISION_DESTORY	= 0x10000000,
	EPLWEATHER_PARTICLE_COLLISION_COMPLETE	= 0x20000000,
};

enum EPLWEATHER_PARTICLE_USE_FLAG
{
	EPLWEATHER_PARTICLE_USE_NONE					= 0x00000000,

	EPLWEATHER_PARTICLE_USE_EFFECT_CREATE			= 0x00000001,
	EPLWEATHER_PARTICLE_USE_EFFECT_DESTROY			= 0x00000002,
	EPLWEATHER_PARTICLE_USE_EFFECT_COLLISION		= 0x00000004,
	EPLWEATHER_PARTICLE_USE_EFFECT_COLLISION_WATER	= 0x00000008,
	
// 	EPLWEATHER_PARTICLE_USE_SOUND_CREATE	= 0x00000010,
// 	EPLWEATHER_PARTICLE_USE_SOUND_DESTROY	= 0x00000020,
// 	EPLWEATHER_PARTICLE_USE_SOUND_COLLISION	= 0x00000040,	

	EPLWEATHER_PARTICLE_USE_SOUND_PARTICLE	= 0x00000100,	
	EPLWEATHER_PARTICLE_USE_SOUND_GLOBAL	= 0x00000200,	
};

enum EPLWEATHER_PARTICLE_RENDER_STATE_FLAG
{
	EPLWEATHER_PARTICLE_RENDER_STATE_NONE	= 0x00000000,
	EPLWEATHER_PARTICLE_RENDER_STATE_FOG	= 0x00000001,
	EPLWEATHER_PARTICLE_RENDER_STATE_LIGHT	= 0x00000002,
	EPLWEATHER_PARTICLE_RENDER_STATE_ZTEST	= 0x00000004,
};

enum EPLWATHER_PARTICLE_EVENT_TYPE
{
	EPLWATHER_PARTICLE_EVENT_NONE,
	EPLWATHER_PARTICLE_EVENT_LIGHTNING,
	EPLWATHER_PARTICLE_EVENT_LIGHTNING2,
};

struct SPLWeatherEmitter
{
	std::string							strName;				// World Editor에서 표시되는 이름.

	RwUInt32							eEmitterPlaceType;	// EPLWEATHER_EMITTER_PLACE_TYPE		
	RwV3d								vEmitterPos;
	RwV3d								vEmitterSize;
	RwReal								fEmitterRadius;

	std::string							strParticleTextureName;
	RwV2d								vParticleSize;
	RwV2d								vParticleHalfSize;
	RwReal								fParticleRadius;

	RwUInt32							eParticleMotionType; // EPLWEATHER_PARTICLE_MOTION_TYPE
	RwV3d								vParticleMotionDir;
	RwReal								fParticleMotionVel;

	RwRGBA								clrParticleStart;
	RwRGBA								clrParticleCenter;
	RwRGBA								clrParticleEnd;
	RwReal								fParticleSColorTime;
	RwReal								fParticleCColorTime;
	RwReal								fParticleEColorTime;

	RwReal								fParticleCreateTime;
	RwReal								fParticleCreateTimeGap;
	RwInt32								iParticleCreateNum;
	RwInt32								iParticleCreateNumGap;
	RwInt32								iParticleCreateMax;
	RwInt32								iParticleRenderMax;

	RwReal								fParticleCreateTerrainDist;

	RwUInt32							eParticleUseFlag;	// EPLWEATHER_PARTICLE_USE_FLAG

	std::vector<std::string>			vecParticleCreateEffect;
	//std::string						strParticleCreateEffect;
	//char								acParticleCreateSound[dPL_WEATHER_CHAR_BUFFER_SMALL];

	std::string							strParticleDestroyEffect;
	//char								acParticleDestroySound[dPL_WEATHER_CHAR_BUFFER_SMALL];

	std::string							strParticleCollisionEffect;
	std::string							strParticleCollisionEffectWater;
	//char								acParticleCollisionSound[dPL_WEATHER_CHAR_BUFFER_SMALL];

	RwReal								fParticleLifeTime;

	RwUInt32							eParticleCollisionFlag; // EPLWEATHER_PARTICLE_COLLISION_TYPE
	RwUInt32							eParticleCreateEventType;

	RwReal								fParticleFrustumLengthSquared;
	RwReal								fParticleFrustumFast;		// 보이지 않는 파티클에 대한 업데이트 가중치


	RwUInt32							eParticleBillboardType; // EPLWEATHER_PARTICLE_BILLBOARD_TYPE
	RwV3d								vParticleBillboard;

	RwUInt32							eParticleBlendingType; // EPLWEATHER_PARTICLE_BLENDING_TYPE

	RwReal								fParticleSplinePower;
	RwReal								fParticleSplineTime;

	RwUInt32							eParticleRenderStateFlag; // EPLWEATHER_PARTICLE_RENDER_STATE_FLAG

	RwReal								fEffectFrustumLengthSquared;


	typedef std::pair<RwInt32, std::string>	PAIR_SOUND;
	typedef std::vector<PAIR_SOUND>			VEC_SOUND;
	typedef VEC_SOUND::iterator				VEC_SOUND_IT;

	
	std::string							strGlobalSound;
	VEC_SOUND							vecParticleSound;
	RwReal								fParticleSoundTime;

	RwReal								fCollisionLineScale;
};

struct SPLWeatherParticle
{
	// Time Data
	RwReal		fTimeCur;
	RwReal		fTimeEnd;
	RwReal		fTimeDelta;

	RwReal		fTimeMoveCur; // TSnap 이나 Spline에서 사용할 임시 시간값.
	RwReal		fTimeMoveEnd;

	// Collision Data
	RwInt32		eCollisionFlag;
	RwReal		fCollisionDelta;
	RwV3d		vCollisionPoint;

	// Pos Data
	RwReal		fSplinePower;
	RwV3d		vPosCtrl;
	RwV3d		vPosPrev;
	RwV3d		vPosNext;
	RwV3d		vPos;
	RwV3d		vVel;

	// Mat Data
	RwMatrix	mat;

	// Color Data
	RwRGBA		clr[4];

	// Frustom Data
	RwV2d		vDist2d;
	RwReal		fDist2dSquared;

	void* operator new(size_t size);
	void operator delete(void *p);
};

typedef std::list<SPLWeatherParticle*>	PARTICLE_LIST;
typedef PARTICLE_LIST::iterator			PARTICLE_LIST_IT;

class CNtlPLWeatherParticleFreeList
{
	static RwFreeList* m_pFreeList;

public:
	static RwBool	AllocFreeList();
	static void		DeAllocFreeList();
	static void*	Alloc();
	static void		Free(void *p);
};
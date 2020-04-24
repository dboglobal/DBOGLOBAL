#pragma once

#include "NtlSchedule.h"
#include "NtlWorldCommon.h"
#include "NtlPLEvent.h"

/*
WorldFileFormat - Schedule
WorldData가 변경되면 Schedule Loading 부분 또한 규칙에 맞게 변경해주어야 한다.
*/

class CNtlSCDEField;
class CNtlSCDESector;

enum ESCHEDULE_FREELIST_TYPE
{
	ESCHEDULE_FREELIST_FIELD_DELETE = 0,
	ESCHEDULE_FREELIST_FIELD_CREATE,
	ESCHEDULE_FREELIST_FIELD_CREATE_INDOOR,
	ESCHEDULE_FREELIST_FIELD,
	ESCHEDULE_FREELIST_MHD,
	ESCHEDULE_FREELIST_MHD_INDOOR,
	ESCHEDULE_FREELIST_DOODADS,
	ESCHEDULE_FREELIST_DOODADS_INDOOR,
	ESCHEDULE_FREELIST_WATER,
	ESCHEDULE_FREELIST_SHADOW,
	ESCHEDULE_FREELIST_EFFECT,
	ESCHEDULE_FREELIST_SE,
	// CZ-SS
	//ESCHEDULE_FREELIST_SS,
	ESCHEDULE_FREELIST_SECTORCULL,
	ESCHEDULE_FREELIST_TITETRANSPARENCY,
	ESCHEDULE_FREELIST_SHORELINE,
	ESCHEDULE_FREELIST_DECAL,
	ESCHEDULE_FREELIST_PLANT,
	ESCHEDULE_FREELIST_WORLDLIGHT,
	ESCHEDULE_FREELIST_OCCLUDER,
	ESCHEDULE_FREELIST_HEATHAZEOBJECT,
	ESCHEDULE_FREELIST_LIGHT_OBJECT,
	ESCHEDULE_FREELIST_DECAL_VOLUME,
	ESCHEDULE_FREELIST_GAME_PROPERTY,
	ESCHEDULE_FREELIST_DOJO,

	ESCHEDULE_FREELIST_NUM,
};

class CNtlWorldSchedulerFreeList
{
	static RwFreeList*	m_pFreeList[ESCHEDULE_FREELIST_NUM];
	static RwInt32		m_iRefCount; // AllocFreeList나 DeAllocFreeList가 2번 호출 되는걸 막는다.

public:
	static RwBool	AllocFreeList();
	static void		DeAllocFreeList();
	static void*	Alloc(const ESCHEDULE_FREELIST_TYPE eType);
	static void		Free(const ESCHEDULE_FREELIST_TYPE eType, void *p);
};

class CNtlWorldFieldScheduler : public CScheduler
{
};

class CNtlWorldCreateScheduler : public CScheduler
{
	virtual RwBool SchedulingProcess();
};

class CNtlWorldDeleteScheduler : public CScheduler
{
};

class CNtlWorldScheduler
{
protected:
	CNtlWorldCreateScheduler	m_WCS;
	CNtlWorldDeleteScheduler	m_WDS;
	RwV3d						m_vLastPos;
	RwReal						m_fElapsedTime;


public:
	CNtlWorldScheduler();
	virtual ~CNtlWorldScheduler();


	virtual void				RemoveSchedule(RwInt32 iPrimaryKey);
	virtual void				ClearSchedule();

	void						CreateField(RwInt32 iFieldIdx);
	void						DeleteField(RwInt32 iFieldIdx);

	RwBool						Scheduling(RwReal fElapsedTime, RwV3d& vAvatarPos);

	RwV3d						GetLastPos() { return m_vLastPos; }
};

// class CNtlWorldScheduler
// {
// protected:
// 	CNtlWorldFieldScheduler		m_NtlWorldFieldScheduler;
// 
// 	RwV3d						m_vLastPos;
// 	RwReal						m_fElapsedTime;
// 
// 
// public:
// 	CNtlWorldScheduler();
// 	virtual ~CNtlWorldScheduler();
// 
// 
// 	//virtual void				RemoveSchedule(RwInt32 iPrimaryKey);
// 	//virtual void				ClearSchedule();
// 
// 	void						CreateField(RwInt32 iFieldIdx);
// 	void						DeleteField(RwInt32 iFieldIdx);
// 
// 	RwBool						Scheduling(RwReal fElapsedTime, RwV3d& vAvatarPos);
// 
// 	RwV3d						GetLastPos() { return m_vLastPos; }
// };



/////////////////////////////////////////////////////////////////////////

class CNtlWSEFieldDelete : public CScheduleElement
{
protected:
	RwInt32					m_iFieldIdx;

public:
	CNtlWSEFieldDelete(RwInt32 iPrimaryKey, RwReal fUsedTime, RwInt32 iFieldIdx);
	virtual ~CNtlWSEFieldDelete();

	void* operator new(size_t size)
	{
		return CNtlWorldSchedulerFreeList::Alloc(ESCHEDULE_FREELIST_FIELD_DELETE);
	}

	void operator delete(void *p)
	{
		CNtlWorldSchedulerFreeList::Free(ESCHEDULE_FREELIST_FIELD_DELETE, p);
	}

	virtual RwBool		Begin();
	virtual RwBool		Scheduling();
	virtual RwBool		End();
};

/////////////////////////////////////////////////////////////////////////

class CNtlWSEFieldCreate : public CScheduleElement
{
protected:
	RwInt32					m_iFieldIdx;
	FILE*					m_pFile;	

	CScheduler				m_Scheduler;

public:
	CNtlWSEFieldCreate(RwInt32 iPrimaryKey, RwReal fUsedTime, RwInt32 iFieldIdx);
	virtual ~CNtlWSEFieldCreate();

	void* operator new(size_t size)
	{
		return CNtlWorldSchedulerFreeList::Alloc(ESCHEDULE_FREELIST_FIELD_CREATE);
	}

	void operator delete(void *p)
	{
		CNtlWorldSchedulerFreeList::Free(ESCHEDULE_FREELIST_FIELD_CREATE, p);
	}

	virtual RwReal		GetUsedTime()
	{ 
		if (m_eScheduleStep == ESCHEDULE_STEP_SCHEDULING)
		{
			if (m_Scheduler.GetFirstScedule())
				return m_Scheduler.GetFirstScedule()->GetUsedTime();
			return 0.0f;
		}
		return m_fUsedTime;
	}

	virtual RwBool		Begin();
	virtual RwBool		Scheduling();
	virtual RwBool		End();
};

//////////////////////////////////////////////////////////////////////////

class CNtlWSEFieldCreateIndoor : public CScheduleElement
{
protected:
	RwInt32					m_iFieldIdx;
	FILE*					m_pFile;	

	CScheduler				m_Scheduler;

public:
	CNtlWSEFieldCreateIndoor(RwInt32 iPrimaryKey, RwReal fUsedTime, RwInt32 iFieldIdx);
	virtual ~CNtlWSEFieldCreateIndoor();

	void* operator new(size_t size)
	{
		return CNtlWorldSchedulerFreeList::Alloc(ESCHEDULE_FREELIST_FIELD_CREATE_INDOOR);
	}

	void operator delete(void *p)
	{
		CNtlWorldSchedulerFreeList::Free(ESCHEDULE_FREELIST_FIELD_CREATE_INDOOR, p);
	}

	virtual RwReal		GetUsedTime()
	{ 
		if (m_eScheduleStep == ESCHEDULE_STEP_SCHEDULING)
		{
			if (m_Scheduler.GetFirstScedule())
				return m_Scheduler.GetFirstScedule()->GetUsedTime();
			return 0.0f;
		}
		return m_fUsedTime;
	}

	virtual RwBool		Begin();
	virtual RwBool		Scheduling();
	virtual RwBool		End();
};

/////////////////////////////////////////////////////////////////////////

class CNtlWSEField : public CScheduleElement
{
protected:
	RwInt32 m_iFieldIdx;
	
public:
	CNtlWSEField(RwInt32 iPrimaryKey, RwReal fUsedTime, RwInt32 iFieldIdx);
	virtual ~CNtlWSEField();

	void* operator new(size_t size)
	{
		return CNtlWorldSchedulerFreeList::Alloc(ESCHEDULE_FREELIST_FIELD);
	}

	void operator delete(void *p)
	{
		CNtlWorldSchedulerFreeList::Free(ESCHEDULE_FREELIST_FIELD, p);
	}

	virtual RwBool		Begin();
	virtual RwBool		Scheduling();
	virtual RwBool		End();
};

/////////////////////////////////////////////////////////////////////////

class CNtlWSEMHD : public CScheduleElement
{
protected:
	CNtlWorldSector*		m_pNtlWorldSector;
	FILE*					m_pFile;
	RwUInt32					m_iFilePos;

	sCUR_FIELD_TEX_INFO*	m_pFieldTexInfo;

public:
	CNtlWSEMHD(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos, sCUR_FIELD_TEX_INFO* pFieldTexInfo);
	virtual ~CNtlWSEMHD();

	void* operator new(size_t size)
	{
		return CNtlWorldSchedulerFreeList::Alloc(ESCHEDULE_FREELIST_MHD);
	}

	void operator delete(void *p)
	{
		CNtlWorldSchedulerFreeList::Free(ESCHEDULE_FREELIST_MHD, p);
	}

	virtual RwBool		Begin();
	virtual RwBool		Scheduling();
	virtual RwBool		End();
};

//////////////////////////////////////////////////////////////////////////

class CNtlWSEMHDIndoor : public CScheduleElement
{
protected:
	CNtlWorldSector*		m_pNtlWorldSector;
	FILE*					m_pFile;
	RwInt32					m_iFilePos;

public:
	CNtlWSEMHDIndoor(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos);
	virtual ~CNtlWSEMHDIndoor();

	void* operator new(size_t size)
	{
		return CNtlWorldSchedulerFreeList::Alloc(ESCHEDULE_FREELIST_MHD_INDOOR);
	}

	void operator delete(void *p)
	{
		CNtlWorldSchedulerFreeList::Free(ESCHEDULE_FREELIST_MHD_INDOOR, p);
	}

	virtual RwBool		Begin();
	virtual RwBool		Scheduling();
	virtual RwBool		End();
};

//////////////////////////////////////////////////////////////////////////

class CNtlWSEDoodads : public CScheduleElement
{
protected:
	CNtlWorldSector*		m_pNtlWorldSector;
	FILE*					m_pFile;
	RwInt32					m_iFilePos;

public:
	CNtlWSEDoodads(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos);
	virtual ~CNtlWSEDoodads();

	void* operator new(size_t size)
	{
		return CNtlWorldSchedulerFreeList::Alloc(ESCHEDULE_FREELIST_DOODADS);
	}

	void operator delete(void *p)
	{
		CNtlWorldSchedulerFreeList::Free(ESCHEDULE_FREELIST_DOODADS, p);
	}

	virtual RwBool		Begin();
	virtual RwBool		Scheduling();
	virtual RwBool		End();
};

//////////////////////////////////////////////////////////////////////////

class CNtlWSEDoodadsIndoor : public CScheduleElement
{
protected:
	CNtlWorldSector*		m_pNtlWorldSector;
	FILE*					m_pFile;
	RwInt32					m_iFilePos;

public:
	CNtlWSEDoodadsIndoor(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos);
	virtual ~CNtlWSEDoodadsIndoor();

	void* operator new(size_t size)
	{
		return CNtlWorldSchedulerFreeList::Alloc(ESCHEDULE_FREELIST_DOODADS_INDOOR);
	}

	void operator delete(void *p)
	{
		CNtlWorldSchedulerFreeList::Free(ESCHEDULE_FREELIST_DOODADS_INDOOR, p);
	}

	virtual RwBool		Begin();
	virtual RwBool		Scheduling();
	virtual RwBool		End();
};

//////////////////////////////////////////////////////////////////////////

class CNtlWSEWater : public CScheduleElement
{
protected:
	CNtlWorldSector*		m_pNtlWorldSector;
	FILE*					m_pFile;
	RwInt32					m_iFilePos;

public:
	CNtlWSEWater(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos);
	virtual ~CNtlWSEWater();

	void* operator new(size_t size)
	{
		return CNtlWorldSchedulerFreeList::Alloc(ESCHEDULE_FREELIST_WATER);
	}

	void operator delete(void *p)
	{
		CNtlWorldSchedulerFreeList::Free(ESCHEDULE_FREELIST_WATER, p);
	}

	virtual RwBool		Begin();
	virtual RwBool		Scheduling();
	virtual RwBool		End();
};

//////////////////////////////////////////////////////////////////////////

class CNtlWSEShadow : public CScheduleElement
{
protected:
	CNtlWorldSector*		m_pNtlWorldSector;
	FILE*					m_pFile;
	RwInt32					m_iFilePos;

public:
	CNtlWSEShadow(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos);
	virtual ~CNtlWSEShadow();

	void* operator new(size_t size)
	{
		return CNtlWorldSchedulerFreeList::Alloc(ESCHEDULE_FREELIST_SHADOW);
	}

	void operator delete(void *p)
	{
		CNtlWorldSchedulerFreeList::Free(ESCHEDULE_FREELIST_SHADOW, p);
	}

	virtual RwBool		Begin();
	virtual RwBool		Scheduling();
	virtual RwBool		End();
};

//////////////////////////////////////////////////////////////////////////

class CNtlWSEEffect : public CScheduleElement
{
protected:
	CNtlWorldSector*		m_pNtlWorldSector;
	FILE*					m_pFile;
	RwInt32					m_iFilePos;

public:
	CNtlWSEEffect(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos);
	virtual ~CNtlWSEEffect();

	void* operator new(size_t size)
	{
		return CNtlWorldSchedulerFreeList::Alloc(ESCHEDULE_FREELIST_EFFECT);
	}

	void operator delete(void *p)
	{
		CNtlWorldSchedulerFreeList::Free(ESCHEDULE_FREELIST_EFFECT, p);
	}
	
	virtual RwBool		Begin();
	virtual RwBool		Scheduling();
	virtual RwBool		End();
};

//////////////////////////////////////////////////////////////////////////

class CNtlWSESE : public CScheduleElement
{
protected:
	CNtlWorldSector*		m_pNtlWorldSector;
	FILE*					m_pFile;
	RwInt32					m_iFilePos;

public:
	CNtlWSESE(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos);
	virtual ~CNtlWSESE();

	void* operator new(size_t size)
	{
		return CNtlWorldSchedulerFreeList::Alloc(ESCHEDULE_FREELIST_SE);
	}

	void operator delete(void *p)
	{
		CNtlWorldSchedulerFreeList::Free(ESCHEDULE_FREELIST_SE, p);
	}

	virtual RwBool		Begin();
	virtual RwBool		Scheduling();
	virtual RwBool		End();
};

//////////////////////////////////////////////////////////////////////////
// CZ-SS
// class CNtlWSESS : public CScheduleElement
// {
// protected:
// 	CNtlWorldSector*		m_pNtlWorldSector;
// 	FILE*					m_pFile;
// 	RwInt32					m_iFilePos;
// 
// public:
// 	CNtlWSESS(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos);
// 	virtual ~CNtlWSESS();
// 
// 	void* operator new(size_t size)
// 	{
// 		return CNtlWorldSchedulerFreeList::Alloc(ESCHEDULE_FREELIST_SS);
// 	}
// 
// 	void operator delete(void *p)
// 	{
// 		CNtlWorldSchedulerFreeList::Free(ESCHEDULE_FREELIST_SS, p);
// 	}
// 
// 	virtual RwBool		Begin();
// 	virtual RwBool		Scheduling();
// 	virtual RwBool		End();
// };

//////////////////////////////////////////////////////////////////////////

class CNtlWSESectorCull : public CScheduleElement
{
protected:
	CNtlWorldSector*		m_pNtlWorldSector;
	FILE*					m_pFile;
	RwInt32					m_iFilePos;

public:
	CNtlWSESectorCull(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos);
	virtual ~CNtlWSESectorCull();

	void* operator new(size_t size)
	{
		return CNtlWorldSchedulerFreeList::Alloc(ESCHEDULE_FREELIST_SECTORCULL);
	}

	void operator delete(void *p)
	{
		CNtlWorldSchedulerFreeList::Free(ESCHEDULE_FREELIST_SECTORCULL, p);
	}

	virtual RwBool		Begin();
	virtual RwBool		Scheduling();
	virtual RwBool		End();
};

//////////////////////////////////////////////////////////////////////////

class CNtlWSETileTransparency : public CScheduleElement
{
protected:
	CNtlWorldSector*		m_pNtlWorldSector;
	FILE*					m_pFile;
	RwInt32					m_iFilePos;

public:
	CNtlWSETileTransparency(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos);
	virtual ~CNtlWSETileTransparency();

	void* operator new(size_t size)
	{
		return CNtlWorldSchedulerFreeList::Alloc(ESCHEDULE_FREELIST_TITETRANSPARENCY);
	}

	void operator delete(void *p)
	{
		CNtlWorldSchedulerFreeList::Free(ESCHEDULE_FREELIST_TITETRANSPARENCY, p);
	}

	virtual RwBool		Begin();
	virtual RwBool		Scheduling();
	virtual RwBool		End();
};

//////////////////////////////////////////////////////////////////////////

class CNtlWSEShoreLine : public CScheduleElement
{
protected:
	CNtlWorldSector*		m_pNtlWorldSector;
	FILE*					m_pFile;
	RwInt32					m_iFilePos;

public:
	CNtlWSEShoreLine(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos);
	virtual ~CNtlWSEShoreLine();

	void* operator new(size_t size)
	{
		return CNtlWorldSchedulerFreeList::Alloc(ESCHEDULE_FREELIST_SHORELINE);
	}

	void operator delete(void *p)
	{
		CNtlWorldSchedulerFreeList::Free(ESCHEDULE_FREELIST_SHORELINE, p);
	}

	virtual RwBool		Begin();
	virtual RwBool		Scheduling();
	virtual RwBool		End();
};

//////////////////////////////////////////////////////////////////////////

class CNtlWSEDecal : public CScheduleElement
{
protected:
	CNtlWorldSector*		m_pNtlWorldSector;
	FILE*					m_pFile;
	RwInt32					m_iFilePos;

public:
	CNtlWSEDecal(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos);
	virtual ~CNtlWSEDecal();

	void* operator new(size_t size)
	{
		return CNtlWorldSchedulerFreeList::Alloc(ESCHEDULE_FREELIST_DECAL);
	}

	void operator delete(void *p)
	{
		CNtlWorldSchedulerFreeList::Free(ESCHEDULE_FREELIST_DECAL, p);
	}

	virtual RwBool		Begin();
	virtual RwBool		Scheduling();
	virtual RwBool		End();
};

//////////////////////////////////////////////////////////////////////////

class CNtlWSEPlant : public CScheduleElement
{
protected:
	CNtlWorldSector*		m_pNtlWorldSector;
	FILE*					m_pFile;
	RwInt32					m_iFilePos;

public:
	CNtlWSEPlant(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos);
	virtual ~CNtlWSEPlant();

	void* operator new(size_t size)
	{
		return CNtlWorldSchedulerFreeList::Alloc(ESCHEDULE_FREELIST_PLANT);
	}

	void operator delete(void *p)
	{
		CNtlWorldSchedulerFreeList::Free(ESCHEDULE_FREELIST_PLANT, p);
	}

	virtual RwBool		Begin();
	virtual RwBool		Scheduling();
	virtual RwBool		End();
};

//////////////////////////////////////////////////////////////////////////

class CNtlWSEWorldLight : public CScheduleElement
{
protected:
	CNtlWorldSector*		m_pNtlWorldSector;
	FILE*					m_pFile;
	RwInt32					m_iFilePos;

public:
	CNtlWSEWorldLight(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos);
	virtual ~CNtlWSEWorldLight();

	void* operator new(size_t size)
	{
		return CNtlWorldSchedulerFreeList::Alloc(ESCHEDULE_FREELIST_WORLDLIGHT);
	}

	void operator delete(void *p)
	{
		CNtlWorldSchedulerFreeList::Free(ESCHEDULE_FREELIST_WORLDLIGHT, p);
	}

	virtual RwBool		Begin();
	virtual RwBool		Scheduling();
	virtual RwBool		End();
};

//////////////////////////////////////////////////////////////////////////

class CNtlWSEOccluder : public CScheduleElement
{
protected:
	CNtlWorldSector*		m_pNtlWorldSector;
	FILE*					m_pFile;
	RwInt32					m_iFilePos;

public:
	CNtlWSEOccluder(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos);
	virtual ~CNtlWSEOccluder();

	void* operator new(size_t size)
	{
		return CNtlWorldSchedulerFreeList::Alloc(ESCHEDULE_FREELIST_OCCLUDER);
	}

	void operator delete(void *p)
	{
		CNtlWorldSchedulerFreeList::Free(ESCHEDULE_FREELIST_OCCLUDER, p);
	}

	virtual RwBool		Begin();
	virtual RwBool		Scheduling();
	virtual RwBool		End();
};

//////////////////////////////////////////////////////////////////////////
class CNtlWSEHeatHazeObject : public CScheduleElement
{
protected:
	CNtlWorldSector*		m_pNtlWorldSector;
	FILE*					m_pFile;
	RwInt32					m_iFilePos;

public:
	CNtlWSEHeatHazeObject(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos);
	virtual ~CNtlWSEHeatHazeObject();

	void* operator new(size_t size)
	{
		return CNtlWorldSchedulerFreeList::Alloc(ESCHEDULE_FREELIST_HEATHAZEOBJECT);
	}

	void operator delete(void *p)
	{
		CNtlWorldSchedulerFreeList::Free(ESCHEDULE_FREELIST_HEATHAZEOBJECT, p);
	}

	virtual RwBool		Begin();
	virtual RwBool		Scheduling();
	virtual RwBool		End();
};

//////////////////////////////////////////////////////////////////////////
class CNtlWSELightObject : public CScheduleElement
{
protected:
	CNtlWorldSector*		m_pNtlWorldSector;
	FILE*					m_pFile;
	RwInt32					m_iFilePos;

public:
	CNtlWSELightObject(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos);
	virtual ~CNtlWSELightObject();

	void* operator new(size_t size)
	{
		return CNtlWorldSchedulerFreeList::Alloc(ESCHEDULE_FREELIST_LIGHT_OBJECT);
	}

	void operator delete(void *p)
	{
		CNtlWorldSchedulerFreeList::Free(ESCHEDULE_FREELIST_LIGHT_OBJECT, p);
	}

	virtual RwBool		Begin();
	virtual RwBool		Scheduling();
	virtual RwBool		End();
};

//////////////////////////////////////////////////////////////////////////
class CNtlWSEDecalVolume : public CScheduleElement
{
protected:
	CNtlWorldSector*		m_pNtlWorldSector;
	FILE*					m_pFile;
	RwInt32					m_iFilePos;

public:
	CNtlWSEDecalVolume(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos);
	virtual ~CNtlWSEDecalVolume();

	void* operator new(size_t size)
	{
		return CNtlWorldSchedulerFreeList::Alloc(ESCHEDULE_FREELIST_DECAL_VOLUME);
	}

	void operator delete(void *p)
	{
		CNtlWorldSchedulerFreeList::Free(ESCHEDULE_FREELIST_DECAL_VOLUME, p);
	}

	virtual RwBool		Begin();
	virtual RwBool		Scheduling();
	virtual RwBool		End();
};

//////////////////////////////////////////////////////////////////////////
class CNtlWSEGameProperty : public CScheduleElement
{
protected:
	CNtlWorldSector*		m_pNtlWorldSector;
	FILE*					m_pFile;
	RwInt32					m_iFilePos;

public:
	CNtlWSEGameProperty(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos);
	virtual ~CNtlWSEGameProperty();

	void* operator new(size_t size)
	{
		return CNtlWorldSchedulerFreeList::Alloc(ESCHEDULE_FREELIST_GAME_PROPERTY);
	}

	void operator delete(void *p)
	{
		CNtlWorldSchedulerFreeList::Free(ESCHEDULE_FREELIST_GAME_PROPERTY, p);
	}

	virtual RwBool		Begin();
	virtual RwBool		Scheduling();
	virtual RwBool		End();
};

//////////////////////////////////////////////////////////////////////////
class CNtlWSEDojo : public CScheduleElement
{
protected:
	CNtlWorldSector*		m_pNtlWorldSector;
	FILE*					m_pFile;
	RwInt32					m_iFilePos;

public:
	CNtlWSEDojo(RwInt32 iPrimaryKey, RwReal fUsedTime, CNtlWorldSector* pNtlWorldSector, FILE* pFile, RwInt32 iFilePos);
	virtual ~CNtlWSEDojo();

	void* operator new(size_t size)
	{
		return CNtlWorldSchedulerFreeList::Alloc(ESCHEDULE_FREELIST_DOJO);
	}

	void operator delete(void *p)
	{
		CNtlWorldSchedulerFreeList::Free(ESCHEDULE_FREELIST_DOJO, p);
	}

	virtual RwBool		Begin();
	virtual RwBool		Scheduling();
	virtual RwBool		End();
};
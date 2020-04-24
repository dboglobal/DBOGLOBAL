#ifndef __NTL_PL_WATER_H__
#define __NTL_PL_WATER_H__


#include <RwCore.h>
#include "NtlDebug.h"
#include "NtlPLAttach.h"
#include "NtlPLApi.h"
#include "ceventhandler.h"
#include "NtlWorldSector.h"


using std::map;
#include <map>


class CNtlPLWaterProp;
class CNtlWorldSector;


struct sWATER
{
	sWATER::sWATER()
	{
		_ppTexPack		= NULL;
		_ppTexSpecular	= NULL;
	}
	sWATER::~sWATER()
	{
		for(RwInt32 i = 0; i < _NumFrame; ++i)
		{
			if (_ppTexPack[i])
			{
				RwTextureDestroy(_ppTexPack[i]);
			}
			if (_ppTexSpecular[i])
			{
				RwTextureDestroy(_ppTexSpecular[i]);
			}
		}
		NTL_ARRAY_DELETE(_ppTexPack);
		NTL_ARRAY_DELETE(_ppTexSpecular);
	}

	RwTexture**		_ppTexPack;
	RwTexture**		_ppTexSpecular;
	RwInt32			_IdxElement;
	RwInt32			_NumFrame;
	RwInt32			_IdxFrame;
	RwReal			_FPS;
	RwReal			_AccumulatedTime;

public:
	void Update(RwReal fElapsed);
};


#define dWATER_DEFAULT_HEIGHT_FROM_TERRAIN (10.0f)


class CNtlPLWater : public CNtlPLEntity, public RWS::CEventHandler
{
public:	
	CNtlPLWater(void);
	virtual	~CNtlPLWater(void);

public:
	typedef std::map<DWORD, sWATER*>	WATER_MAP;
	typedef WATER_MAP::iterator			WATER_ITER;

public:
	CNtlPLWaterProp*	m_pWaterProp;
	WATER_MAP			m_mapWater;

public:
	virtual RwBool	Create(const SPLEntityCreateParam* pParam = NULL);
	virtual void	Destroy();
	virtual void	HandleEvents(RWS::CMsg &pMsg);

	virtual RwBool	Update(RwReal fElapsed);
	virtual RwBool	Render(void);
	void			Draw(RxD3D9InstanceData *pInstancedData, RxD3D9ResEntryHeader *pResEntryHeader, RwBool DepthMapEnable);

	virtual RwBool	SetProperty(const CNtlPLProperty *pProperty);
	string&			GetWaterPackName(RwInt32 Idx);

	RwBool			OnMove(CNtlWorldSector* pNtlWorldSector, RwReal Append, RwBool ReplaceFlag = FALSE);
	RwBool			OnCreate(CNtlWorldSector* pNtlWorldSector, sSECTOR_WATER_ATTR& SectorWaterAttr);
	RwBool			OnDelete(CNtlWorldSector* pNtlWorldSector);
	RwBool			OnRender(CNtlWorldSector* pNtlWorldSector, RxD3D9InstanceData* pInstancedData, RxD3D9ResEntryHeader *pResEntryHeader);

	RwBool			BlendingWater(sSECTOR_WATER_ATTR* pSectorWaterAttr, RwReal fNear, RwReal fFar);

protected:
	static RwFrame* s_pWaterFrame;
	static RwInt32  m_iWaterFrameRef;
};


#endif //__NTL_PLSKY_H__
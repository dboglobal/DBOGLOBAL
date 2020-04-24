#pragma  once

#include <RwCore.h>
#include "NtlDebug.h"
#include "NtlPLEntity.h"
#include "NtlWorldCommon.h"

#include "ceventhandler.h"

//Light Mode 에서 사용할 더미 정보.
struct SLightModeData
{
	RwV3d		vPosision;
	RwV3d		vRotation;
	RwRGBAReal	rgbafColor;
};

class CNtlWorldRpLightProxy
{
protected:
	RwReal	m_fLightValue;

public:
	CNtlWorldRpLightProxy()												{ m_fLightValue = RwRealMINVAL; }
	virtual ~CNtlWorldRpLightProxy()									{}

	virtual void		OnAddRpLight(RpLight* pRpLight)					{}
	virtual void		OnRemoveRpLight(RpLight* pRpLight)				{}
	
	virtual void		OnRequestAddRpLight(RpLight* pRpLight)			{}
	virtual void		OnRequestRemoveRpLight(RpLight* pRpLight)		{}

	virtual void		OnUpdate(RpLight* pRpLight, RwReal fElapsed)	{}
	virtual void		OnRender(RpLight* pRpLight)						{}

	virtual RwReal		GetLightValue()									{ return m_fLightValue; }
	virtual void		SetLightValue(RwReal fLightValue)				{ m_fLightValue = fLightValue; }
};

class CNtlWorldRpLightManager : public RWS::CEventHandler
{
protected:
	static CNtlWorldRpLightManager*		g_pNtlWorldRpLightManager;

public:
	struct SRpLightData
	{
		RpLight*				pRpLight;
		RwReal					fDistance;

		CNtlWorldRpLightProxy*	pProxy;

		SRpLightData::SRpLightData(RpLight* _pRpLight, CNtlWorldRpLightProxy* _pProxy)
		{
			pRpLight	= _pRpLight;
			pProxy		= _pProxy;
			fDistance	= 0.0f;
		}

		void SRpLightData::CalcDistance(RwV3d* pvPos)
		{
			if (RpLightGetType(pRpLight) == rpLIGHTAMBIENT ||
				RpLightGetType(pRpLight) == rpLIGHTDIRECTIONAL)
			{
				fDistance = RwRealMINVAL;
			}
			else
			{
				RwV3d vTemp;
				RwV3dSubMacro(&vTemp, pvPos, RwMatrixGetPos(RwFrameGetMatrix(RpLightGetFrame(pRpLight))));
				fDistance = CNtlMath::MathRwV3dSquaredLength(&vTemp);
			}
		}
	};

	typedef std::list<SRpLightData>			RPLIGHT_LIST;
	typedef RPLIGHT_LIST::iterator			RPLIGHT_LIST_IT;

protected:
	RPLIGHT_LIST							m_listRpLight;
	RPLIGHT_LIST							m_listRpLightStandBy;

	RPLIGHT_LIST							m_listRpLightRealLoaded;	
	RPLIGHT_LIST							m_listRpLightDummyLoaded;

	RPLIGHT_LIST							m_listRpLightRealLoadedStatic;	
	RPLIGHT_LIST							m_listRpLightDummyLoadedStatic;
		
public:
	CNtlWorldRpLightManager();
	virtual ~CNtlWorldRpLightManager();

	static CNtlWorldRpLightManager*		GetInstance() { return g_pNtlWorldRpLightManager; }
	static void								CreateInstance();
	static void								DestroyInstance();

public:
	void									Create();
	void									Destroy();

	void									SaveSwap();
	void									LoadSwap();

	void									Update(RwReal fElapsed);
	void									Render();

	void									HandleEvents(RWS::CMsg &pMsg);

	void									RequestAddRpLight(RpLight* pRpLight, CNtlWorldRpLightProxy* pProxy);	
	void									RequestRemoveRpLight(RpLight* pRpLight);
	void									RequestCreateRpLight(RpLight* pRpLight, CNtlWorldRpLightProxy* pProxy);	
	void									RequestDestroyRpLight(RpLight* pRpLight);

protected:
	RwBool									AddRpLightList(RPLIGHT_LIST& listRpLight, RpLight* pRpLight, CNtlWorldRpLightProxy* pProxy);
	RwBool									RemoveRpLightList(RPLIGHT_LIST& listRpLight, RpLight* pRpLight);
	RwBool									FindRpLightList(RPLIGHT_LIST& listRpLight, RpLight* pRpLight, RPLIGHT_LIST_IT& it);
	void									SortRpLightList(RPLIGHT_LIST& listRpLight);
	void									DistRpLightList(RPLIGHT_LIST& listRpLight);
	void									UpdateRpLightList(RPLIGHT_LIST& listRpLight, RwReal fElapsed);
	void									RenderRpLightList(RPLIGHT_LIST& listRpLight);

	RwBool									AddRpLight(SRpLightData& sRpLightData);
	RwBool									RemoveRpLight(SRpLightData& sRpLightData);

	// DUMMY DIRECTION SETTING
protected:
	CNtlPLEntity*							m_pNtlPLLightObjectStaticDummy[3];
	RpAtomic*								m_pAtomicCtlPt;

public:
	CNtlPLEntity*							GetStaticDummy(RwInt32 iIdx) {return m_pNtlPLLightObjectStaticDummy[iIdx]; }

	RwBool									PickLightMode(SLightModeData& sLightModeData, RwReal& fDist);
	void									RenderLightMode(SLightModeData& sLightModeData, RwBool bPick);

	RwInt32									GetLightNumRealLoaded()			{ return m_listRpLightRealLoaded.size(); }
	RwInt32									GetLightNumDummyLoaded()		{ return m_listRpLightDummyLoaded.size(); }

	RwInt32									GetLightNumStaticRealLoaded()	{ return m_listRpLightRealLoadedStatic.size(); }
	RwInt32									GetLightNumStaticDummyLoaded()	{ return m_listRpLightDummyLoadedStatic.size(); }
};
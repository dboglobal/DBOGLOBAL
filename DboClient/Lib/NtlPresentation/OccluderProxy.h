#pragma once

class CNtlPLOccluder_Base;
class CNtlOccluderManager;

/*
	OccluderProxy 는 Mananger와 Object 간의 연결 고리 역활만 해 준다.
*/

class COccluderProxy
{
	friend CNtlOccluderManager;

public:
	enum EOccluderProxyState
	{
		EOCCLUDER_CHECK		= 0,	// INIT
		EOCCLUDER_NONE		= 1,
		EOCCLUDER_CULLED	= 2,
	};

	enum EOccluderQueryType
	{
		EOCCLUDER_SPHERE	= 0,
		EOCCLUDER_BOX		= 1,
	};

public:
	COccluderProxy();
	virtual ~COccluderProxy();

	
	virtual RwBool			CreateOccluderProxy(RwReal fOccluderQueryUpdateTime, RwUInt32 uiTestNum);
	virtual	void			DestroyOccluderProxy();

	virtual RwBool			OccluderQuery(RwUInt32/*EOccluderQueryType*/ eType, RwUInt32 eFlag, void* pData, RwUInt32 uiTestCnt);

protected:
	struct SOCCLDER_PROXY_DATA
	{
		CNtlPLOccluder_Base*	pPLOccluder;
		EOccluderProxyState		eOccluderProxyState;	
		RwReal					fOccluderQueryUpdateTime;			
	};

	RwUInt32				m_uiOccluderTestNum;
	RwReal					m_fOccluderQueryTime;
	SOCCLDER_PROXY_DATA*	m_pOccluderProxyData;
};
#pragma once

class COccluderProxy;
class CNtlPLOccluder_Base;

class CNtlOccluderManager
{
public:
	typedef std::vector<CNtlPLOccluder_Base*>					VEC_OCC;
	typedef VEC_OCC::iterator									VEC_OCC_IT;

	typedef	std::pair<COccluderProxy*, RwUInt32>				PAIR_PROXY;
	typedef std::vector<PAIR_PROXY>								VEC_PROXY;
	typedef VEC_PROXY::iterator									VEC_PROXY_IT;

	typedef std::map<CNtlPLOccluder_Base*, VEC_PROXY>			MAP_OCC;
	typedef MAP_OCC::iterator									MAP_OCC_IT;

protected:
	static CNtlOccluderManager*					g_pOccluderManager;

	MAP_OCC										m_mapOccluder;
	VEC_OCC										m_vecOccluderVisibility;

	RwV3d										m_fErrorPos;
	RwReal										m_fErrorTime;
	RwReal										m_fErrorPoint;

	RwBool										m_bActivation;

public:
	CNtlOccluderManager();
	virtual ~CNtlOccluderManager();

	static CNtlOccluderManager*		GetInstance() { return g_pOccluderManager; }
	static void						CreateInstance();
	static void						DestroyInstance();

	RwBool							Create();
	void							Destroy();	

	RwBool							Update(RwReal fElapsed);

	RwBool							RegisterOccluder(CNtlPLOccluder_Base* pPLOccluder);
	void							UnRegisterOccluder(CNtlPLOccluder_Base* pPLOccluder);
	void							ClearOccluderProxyList(CNtlPLOccluder_Base* pPLOccluder);
	void							ClearAllOccluderProxyList();

	void							RegisterProxyInOccluder(CNtlPLOccluder_Base* pPLOccluder, COccluderProxy* pOccluderProxy);
	void							UnRegisterProxyInOccluder(COccluderProxy* pOccluderProxy);

	RwBool							OccluderQuery(COccluderProxy* pOccluderProxy, RwUInt32 eType/*COccluderProxy::EOccluderQueryType*/, RwUInt32 eFlag, void* pData, RwUInt32 uiTestCnt);
	RwBool							OccluderQuery(RwUInt32 eType, RwUInt32 eFlag, void* pData, RwInt8 iNum);

	void							AddOccluderVisiblity(CNtlPLOccluder_Base* pPLOccluder);

	void							SetActivation(RwBool bActivation);
	RwBool							GetActivation()							{ return m_bActivation; }
	RwReal							GetErrorPoint()							{ return m_fErrorPoint; }

	RwInt32							GetOccluderCulledNum(CNtlPLOccluder_Base* pPLOccluder);
};
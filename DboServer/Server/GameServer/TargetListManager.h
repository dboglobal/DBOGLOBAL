#ifndef __DBOG_TARGETLISTMANAGER_H__
#define __DBOG_TARGETLISTMANAGER_H__


#include "NtlSharedType.h"
#include <map>

class CCharacter;
struct sBOT_AGGROPOINT;
enum eOBJTYPE;
struct sCHAR_AGGRO_INFO;
class CNtlPacket;
class CSpellAreaChecker;

class CTargetListManager
{

public:

	CTargetListManager();
	virtual	~CTargetListManager();

	typedef std::map<HOBJECT, sBOT_AGGROPOINT> AGGROPOINT_MAP;
	typedef AGGROPOINT_MAP::value_type AGGROPOINT_VAL;
	typedef std::map<HOBJECT, eOBJTYPE> TARGETME_MAP;

public:

	bool Create(CCharacter* pBot);
	void Destroy();
	void TickProcess(DWORD dwTickDiff, float fMultiple);
	
	void AddAggro(HOBJECT hProvoker, DWORD dwAggroPoint, bool bCalledByHelp);

	std::map<HOBJECT, sBOT_AGGROPOINT>::iterator RemoveAggro(AGGROPOINT_MAP::iterator itRemove);

	void RemoveAggro(HOBJECT hProvoker);
	void ClearAggro();

	bool ChangeAggroDirectly(HOBJECT hProvoker, BYTE byAggroChangeType, DWORD dwValue);

	size_t GetAggroCount();
	
	HOBJECT GetHighestAggroTarget();
	HOBJECT GetMeanTarget();
	HOBJECT GetBraveTarget();

	void GetTargetApplyRange(CSpellAreaChecker & rSpellAreaChecker, sSKILL_TARGET_LIST & rTargetList, BYTE byMaxTargetCount);

	void GetAggroList(DWORD *rTotalAggroPoint, BYTE *rCount, sCHAR_AGGRO_INFO *aAggroInfo);

	void SetAggroLastUpdateTime();
	inline DWORD GetAggroLastUpdateTime() { return m_dwAggroLastUpdateTime; }

	inline void SetFixedTarget(HOBJECT hTarget) { this->m_hFixedTarget = hTarget; }
	inline HOBJECT GetFixedTarget() { return m_hFixedTarget; }

	inline void SetTauntTarget(HOBJECT hTarget) { this->m_hTauntTarget = hTarget; }
	inline HOBJECT GetTauntTarget() { return m_hTauntTarget; }
	
	inline bool IsCalledByHelp() { return m_bCalledByHelp; }

	sBOT_AGGROPOINT* GetAggroPoint(HOBJECT hObject);

	void		ReleaseAllTargets(bool bLoseTargetNfy = true);

private:

	void OnAggroTargetAdded(HOBJECT hTarget);
	void OnAggroTargetRemoved(HOBJECT hTarget);

public:

	void OnTargeted(HOBJECT hTarget, eOBJTYPE eObjType);
	void OnTargetReleased(HOBJECT hTarget);

	void BroadcastTargetPacket(CNtlPacket* pPacket);

private:

	CCharacter* m_pBotRef;
	DWORD m_dwAggroLastUpdateTime;
	bool m_bNeedSync;
	DWORD m_dwSyncTickTime;
	AGGROPOINT_MAP m_mapAggroPoint;
	TARGETME_MAP m_mapTargetMe;
	DWORD m_dwTickTime;
	bool m_bCalledByHelp;
	HOBJECT m_hFixedTarget;
	HOBJECT m_hTauntTarget;

public:

	AGGROPOINT_MAP::iterator	AggroBegin() { return m_mapAggroPoint.begin(); }
	AGGROPOINT_MAP::iterator	AggroEnd() { return m_mapAggroPoint.end(); }


};

#endif
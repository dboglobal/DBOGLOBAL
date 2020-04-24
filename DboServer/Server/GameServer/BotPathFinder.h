#ifndef __DBOG_BOTPATHFINDER_H__
#define __DBOG_BOTPATHFINDER_H__


#include "WayPointList.h"
#include "NtlNaviDefine.h"

class CCharacter;

class CBotPathFinder
{

public:

	enum ePATH_FINDING_RESULT
	{
		PATH_FINDING_RESULT_FAILED,
		PATH_FINDING_RESULT_CACHE_HIT,
		PATH_FINDING_RESULT_NEW_RESULT,
		PATH_FINDING_RESULT_SRC_FAILED,
	};

	struct sCACHED_MOVE_NEAREST_DEST
	{
		bool bCached;
		TBLIDX worldTblidx;
		sVECTOR3 sSrcPos;
		HOBJECT hTarget;
		sVECTOR3 sDestPos;
		sVECTOR3 sNearestDest;
	};

	CBotPathFinder();
	virtual	~CBotPathFinder();


public:

	bool					Create(CCharacter* pBot, bool bUsePathFinding);

	void					Destroy();

	void					Reset();

	void					SetDestLoc(CNtlVector& rLoc);
	
	void					SetTarget(CCharacter* pBot);

	void					SetWayPointData(sPATH_LIST* pPathData);

	bool					IsVisible() { return true; }

	bool					IsReachable() { return true; }

	bool					IsArrived();

	size_t					GetNaviPositionCount() { return m_vecPosList.size(); }

	//GetCurDestLoc;

	bool					GetCurWayPoint(CNtlVector& rLoc);
	bool					GetNextWayPoint(CNtlVector& rLoc);

	bool					GetCurNavLoc(CNtlVector& rLoc);
	bool					GetNextNavLoc(CNtlVector& rLoc);

	BYTE					GetAllNextNavLoc(sVECTOR3* pavLoc);

	ePATH_FINDING_RESULT	PathFind();

	bool					PathDirectFind();

	ePATH_FINDING_RESULT	WanderPathFind();

	bool					IsValidPos(CNtlVector& rLoc);

	bool					GetNearestPos(CNtlVector& rLoc);

	bool					IsCachedMoveNearestDest(CCharacter* pBot);


protected:

	void					Init();

private:

	CCharacter*				m_pOwnerRef;
	bool					m_bUsePathFinding;

	CNtlVector				m_vDestLoc;
	HOBJECT					m_hTarget;
	HOBJECT					m_hChachedTarget;

	CNtlVector				m_vCachedLoc;
	bool					m_bCachedResult;

	CWayPointList			m_wayPointList;

	std::vector<sNAVI_POS>	m_vecPosList;

	DWORD					m_dwCurNavStep;

	sCACHED_MOVE_NEAREST_DEST m_sCachedMoveNearestDest;

};

#endif
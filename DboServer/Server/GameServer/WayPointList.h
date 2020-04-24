#ifndef __DBOG_WAYPOINTLIST_H__
#define __DBOG_WAYPOINTLIST_H__


#include "WorldPathTable.h"


class CWayPointList
{

public:

	CWayPointList();
	virtual	~CWayPointList();


public:

	bool					Create(sPATH_LIST* pPathData);
	bool					Create(TBLIDX worldTblidx, TBLIDX pathTblidx);

	void					Destroy();

	bool					GetCurWayPoint(CNtlVector& rLoc);

	bool					GetNextWayPoint(CNtlVector& rLoc);

	bool					GetSecondNextWayPoint(CNtlVector& rLoc);

	float					GetWayPointRadius();

	bool					IsEmpty();

	void					InitStep();

	BYTE					GetCurStep() { return m_byCurStep; }

	BYTE					GetMaxStep() { return m_pPathData->_NumPathPoint; }


protected:

	void					Init();

private:

	sPATH_LIST*				m_pPathData;

	BYTE					m_byCurStep;

};

#endif
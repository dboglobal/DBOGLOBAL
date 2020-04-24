#ifndef __DBOG_SPELL_AREA_CHECKER__
#define __DBOG_SPELL_AREA_CHECKER__


#include "NtlVector.h"

class CSpawnObject;

class CSpellAreaChecker 
{

public:

	CSpellAreaChecker();
	virtual ~CSpellAreaChecker();

public:

	virtual bool			Create();

	void					Destroy();

protected:

	void					Init();

public:

	bool					PrepareForSelection(CSpawnObject *pCaster, CSpawnObject *pAppointTarget, BYTE byApplyRangeType, float fApplyAreaSize1, float fApplyAreaSize2);

	bool					PrepareForChecking(CSpawnObject *pCaster, CSpawnObject *pAppointTarget, BYTE byApplyRangeType, float fApplyAreaSize1, float fApplyAreaSize2);

	bool					IsObjectInApplyRange(CSpawnObject *pObject, float *pfSquaredLength);

protected:

	bool					Prepare(CSpawnObject *pCaster, CSpawnObject *pAppointTarget, BYTE byApplyRangeType, float fApplyAreaSize1, float fApplyAreaSize2);

private:

	float					APPLY_RANGE_BUFFER_IN_METERS = 8.0f;

private:

	BYTE					m_byApplyRangeType;
	HOBJECT					m_hAppointTarget;
	CNtlVector				m_vAppointTargetLoc;
	float					m_fRadius;
	CNtlVector				m_vFrontDirFromPivot;
	CNtlVector				m_vLeftDirFromPivot;
	CNtlVector				m_vRightDirFromPivot;
	CNtlVector				m_vLeftWidthPoint;
	CNtlVector				m_vRightWidthPoint;
	CNtlVector				m_vLeftForwardPoint;
	CNtlVector				m_vAppointTargetLoc_Donut;

	float					m_fRadius1;
	float					m_fRadius2;

	CNtlVector				m_vPivot;
	CNtlVector				m_vHeading;
	float					m_fRadius_Fan;
	float					m_fAngleInDegree;

};


#endif
#include "stdafx.h"
#include "SpellAreaChecker.h"
#include "SpawnObject.h"



CSpellAreaChecker::CSpellAreaChecker()
{
	Init();
}

CSpellAreaChecker::~CSpellAreaChecker()
{
	Destroy();
}



bool CSpellAreaChecker::Create()
{
	return true;
}


void CSpellAreaChecker::Destroy()
{
	m_byApplyRangeType = INVALID_BYTE;
}


void CSpellAreaChecker::Init()
{
	m_byApplyRangeType = INVALID_BYTE;
}


bool CSpellAreaChecker::PrepareForSelection(CSpawnObject *pCaster, CSpawnObject *pAppointTarget, BYTE byApplyRangeType, float fApplyAreaSize1, float fApplyAreaSize2)
{
	if (pCaster)
	{
		if (pAppointTarget)
		{
			return Prepare(pCaster, pAppointTarget, byApplyRangeType, fApplyAreaSize1, fApplyAreaSize2);
		}
		else
		{
			ERR_LOG(LOG_GENERAL, "\"pAppointTarget\" is not CSpawnObject type., pAppointTarget = %016x", pAppointTarget);
		}
	}
	else
	{
		ERR_LOG(LOG_GENERAL, "\"pCaster\" is not CSpawnObject type., pCaster = %016x", pCaster);
	}

	return false;
}


bool CSpellAreaChecker::PrepareForChecking(CSpawnObject *pCaster, CSpawnObject *pAppointTarget, BYTE byApplyRangeType, float fApplyAreaSize1, float fApplyAreaSize2)
{
	if (pCaster)
	{
		if (pAppointTarget)
		{
			return Prepare(pCaster, pAppointTarget, byApplyRangeType, fApplyAreaSize1 + APPLY_RANGE_BUFFER_IN_METERS, fApplyAreaSize2 + APPLY_RANGE_BUFFER_IN_METERS);
		}
		else
		{
			ERR_LOG(LOG_GENERAL, "\"pAppointTarget\" is not CSpawnObject type., pAppointTarget = %016x", pAppointTarget);
		}
	}
	else
	{
		ERR_LOG(LOG_GENERAL, "\"pCaster\" is not CSpawnObject type., pCaster = %016x", pCaster);
	}

	return false;
}


bool CSpellAreaChecker::IsObjectInApplyRange(CSpawnObject *pObject, float *pfSquaredLength)
{
	if (pObject)
	{
		switch (m_byApplyRangeType)
		{
			case DBO_SKILL_APPLY_RANGE_SINGLE:
			{
				if (m_hAppointTarget == pObject->GetID())
				{
					if (pfSquaredLength)
						*pfSquaredLength = 0.0f;
					return true;
				}
			}
			break;

			case DBO_SKILL_APPLY_RANGE_CIRCLE:
			{
				CNtlVector vTemp(m_vAppointTargetLoc - pObject->GetCurLoc());
				vTemp.y = 0.0f;
				float fSquaredLength = vTemp.SquaredLength();

				if (fSquaredLength <= (m_fRadius * m_fRadius))
				{
					if (pfSquaredLength)
						*pfSquaredLength = fSquaredLength;
					return true;
				}
			}
			break;

			case DBO_SKILL_APPLY_RANGE_RECTANGLE:
			case DBO_SKILL_APPLY_RANGE_FORWARD:
			case DBO_SKILL_APPLY_RANGE_FLEXIBLE_FORWARD:
			{
				CNtlVector vTemp(pObject->GetCurLoc().operator-(m_vLeftWidthPoint));
				if (vTemp.Dot(m_vRightDirFromPivot) >= 0.0f)
				{
					vTemp.operator=(pObject->GetCurLoc().operator-(m_vRightWidthPoint));
					if (vTemp.Dot(m_vLeftDirFromPivot) >= 0.0f)
					{
						vTemp.operator=(pObject->GetCurLoc().operator-(m_vLeftWidthPoint));
						if (vTemp.Dot(m_vFrontDirFromPivot) >= 0.0f)
						{
							vTemp.operator=(pObject->GetCurLoc().operator-(m_vLeftForwardPoint));
							if (vTemp.Dot(-m_vFrontDirFromPivot) >= 0.0f)
							{
								if (pfSquaredLength)
								{
									vTemp.operator=(m_vAppointTargetLoc.operator-(pObject->GetCurLoc()));
									vTemp.y = 0.0f;
									*pfSquaredLength = vTemp.SquaredLength();
								}
								return true;
							}
						}
					}
				}
			}
			break;

			case DBO_SKILL_APPLY_RANGE_DONUT:
			{
				CNtlVector vTemp(m_vAppointTargetLoc_Donut.operator-(pObject->GetCurLoc()));
				vTemp.y = 0.0f;
				float fSquaredLength = vTemp.SquaredLength();
				if (fSquaredLength <= (m_fRadius * m_fRadius))
				{
					if ((m_fRadius2 * m_fRadius2) <= fSquaredLength)
					{
						if (pfSquaredLength)
							*pfSquaredLength = fSquaredLength;
						return true;
					}
				}
			}
			break;

			case DBO_SKILL_APPLY_RANGE_FAN:
			{
				CNtlVector vTemp(m_vPivot.operator-(pObject->GetCurLoc()));
				vTemp.y = 0.0f;
				float fSquaredLength = vTemp.SquaredLength();
				if (fSquaredLength <= (m_fRadius_Fan * m_fRadius_Fan))
				{
					float fRadian = ConvertDegreeToRadian((int)(m_fAngleInDegree * 0.5f));

					CNtlVector vAngleLimit(RotateVector(&m_vHeading, fRadian));
					if (!vAngleLimit.SafeNormalize())
						vTemp.operator=(m_vHeading);

					if (m_vHeading.Dot(vAngleLimit) <= m_vHeading.Dot(vTemp))
					{
						if (pfSquaredLength)
							*pfSquaredLength = fSquaredLength;
						return true;
					}
				}
			}
			break;

			default: ERR_LOG(LOG_GENERAL, "The value of \"m_byApplyRangeType\" is not allowed here., m_byApplyRangeType = %u", m_byApplyRangeType); break;
		}
	}
	else
	{
		ERR_LOG(LOG_GENERAL, "\"pObject\" is not CSpawnObject type., pObject = %016x", pObject);
	}

	return false;
}


bool CSpellAreaChecker::Prepare(CSpawnObject *pCaster, CSpawnObject *pAppointTarget, BYTE byApplyRangeType, float fApplyAreaSize1, float fApplyAreaSize2)
{
	switch (byApplyRangeType)
	{
		case DBO_SKILL_APPLY_RANGE_SINGLE:
		{
			m_hAppointTarget = pAppointTarget->GetID();

			m_byApplyRangeType = byApplyRangeType;
			return true;
		}
		break;

		case DBO_SKILL_APPLY_RANGE_CIRCLE:
		{
			m_vAppointTargetLoc.operator=(pAppointTarget->GetCurLoc());
			m_vAppointTargetLoc.y = 0.0f;
			m_fRadius = fApplyAreaSize1;

			m_byApplyRangeType = byApplyRangeType;
			return true;
		}
		break;

		case DBO_SKILL_APPLY_RANGE_RECTANGLE:
		case DBO_SKILL_APPLY_RANGE_FORWARD:
		case DBO_SKILL_APPLY_RANGE_FLEXIBLE_FORWARD:
		{
			CNtlVector vCasterLoc(pCaster->GetCurLoc());
			vCasterLoc.y = 0.0f;
			CNtlVector vAppointTargetLoc(pAppointTarget->GetCurLoc());
			vAppointTargetLoc.y = 0.0f;
			CNtlVector vApplyTargetLoc;
			float fDistance = 0.0f;

			m_vFrontDirFromPivot.operator=(vAppointTargetLoc.operator-(vCasterLoc));

			if (!m_vFrontDirFromPivot.SafeNormalize(fDistance))
			{
				m_vFrontDirFromPivot.operator=(pCaster->GetCurDir());
			}

			if (byApplyRangeType == DBO_SKILL_APPLY_RANGE_RECTANGLE)
			{
				vApplyTargetLoc.operator=(m_vFrontDirFromPivot.operator*(fApplyAreaSize2).operator/(2.0f));
				vApplyTargetLoc.operator=(vAppointTargetLoc.operator-(vApplyTargetLoc));
			}
			else
			{
				vApplyTargetLoc.operator=(vCasterLoc);
			}

			m_vLeftDirFromPivot.operator=(RotateVector90DegreeToLeft(&m_vFrontDirFromPivot));

			m_vLeftWidthPoint.operator=(m_vLeftDirFromPivot.operator*(fApplyAreaSize1).operator/(2.0f));
			m_vLeftWidthPoint.operator+=(vApplyTargetLoc);

			m_vRightDirFromPivot.operator=(RotateVector90DegreeToRight(&m_vFrontDirFromPivot));

			m_vRightWidthPoint.operator=(m_vRightDirFromPivot.operator*(fApplyAreaSize1).operator/(2.0f));
			m_vRightWidthPoint.operator+=(vApplyTargetLoc);

			float fLastApplyAreaSize2 = fApplyAreaSize2;

			if (byApplyRangeType == DBO_SKILL_APPLY_RANGE_FLEXIBLE_FORWARD)
				fLastApplyAreaSize2 = fDistance + 5.0f;

			m_vLeftForwardPoint.operator=(m_vFrontDirFromPivot.operator*(fLastApplyAreaSize2));
			m_vLeftForwardPoint.operator+=(m_vLeftWidthPoint);

			m_byApplyRangeType = byApplyRangeType;
			return true;
		}
		break;

		case DBO_SKILL_APPLY_RANGE_DONUT:
		{
			m_vAppointTargetLoc_Donut.operator=(pAppointTarget->GetCurLoc());
			m_vAppointTargetLoc_Donut.y = 0.0f;

			m_fRadius1 = fApplyAreaSize1;
			m_fRadius2 = fApplyAreaSize2;

			m_byApplyRangeType = byApplyRangeType;
			return true;
		}
		break;

		case DBO_SKILL_APPLY_RANGE_FAN:
		{
			m_vPivot.operator=(pCaster->GetCurLoc());
			m_vPivot.y = 0.0f;

			m_vHeading.operator=(pCaster->GetCurDir());
			m_vHeading.y = 0.0f;

			if (!m_vHeading.SafeNormalize())
			{
				m_vHeading.operator=(pCaster->GetCurDir());
			}

			m_fRadius_Fan = fApplyAreaSize1;
			m_fAngleInDegree = fApplyAreaSize2;

			m_byApplyRangeType = byApplyRangeType;
			return true;
		}
		break;

		default: ERR_LOG(LOG_GENERAL, "The value of \"byApplyRangeType\" is not allowed here., byApplyRangeType = %u", byApplyRangeType); break;
	}

	return false;
}

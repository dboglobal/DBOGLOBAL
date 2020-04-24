#include "precomp_ntlsimulation.h"
#include "NtlDTProp.h"

// core
#include "NtlDebug.h"
#include "NtlMath.h"
#include "NtlSerializer.h"

// presentation
#include "NtlPLGlobal.h"
#include "NtlPLResourcePack.h"

RwUInt32 g_uiDTPropKey = 0;

CNtlDTProp::CNtlDTProp()
{
	std::string str = "DTProp";

	RwChar chBuffer[128];
	sprintf_s(chBuffer, 128, "%d", g_uiDTPropKey);
	str += chBuffer;
	SetName(str.c_str());

	g_uiDTPropKey++;

	// transform
	ResetTransform();
}

RwBool CNtlDTProp::Load(CNtlSerializer& s)
{
	static RwChar chBuffer[128];
	RwUInt8 byType;
	RwInt32 iSize;

	while(1)
	{
		if(s.IsEmpty())
			return TRUE;

		s >> byType;

		switch(byType)
		{
		case DTPROP_SERIALIZE_NAME:
			{
				s >> iSize;
				if(iSize > 0)
				{
					s.Out(chBuffer, iSize);
					chBuffer[iSize] = 0;
					SetName(chBuffer);
				}
			}
			break;
		case DTPROP_SERIALIZE_TRANSFORM:
			{
				s >> m_fPitch;
				s >> m_fYaw;
				s >> m_fRoll;
				s >> m_vScale.x;
				s >> m_vScale.y;
				s >> m_vScale.z;
				s >> m_vPos.x;
				s >> m_vPos.y;
				s >> m_vPos.z;

				CalcTransform();
			}
			break;
		case DTPROP_SERIALIZE_END:
			return TRUE;
			break;
		}
	}

}

RwBool CNtlDTProp::Save(CNtlSerializer& s)
{
	s << (RwUInt8)DTPROP_SERIALIZE_NAME;
	s << (RwInt32)m_strName.size();
	s.In(m_strName.c_str(), m_strName.size());

	s << (RwUInt8)DTPROP_SERIALIZE_TRANSFORM;
	s << m_fPitch;
	s << m_fYaw;
	s << m_fRoll;
	s << m_vScale.x;
	s << m_vScale.y;
	s << m_vScale.z;
	s << m_vPos.x;
	s << m_vPos.y;
	s << m_vPos.z;

	s << (RwUInt8)DTPROP_SERIALIZE_END;

	return TRUE;
}


void CNtlDTProp::SetName(const RwChar *pName)
{
	m_strName =	pName;
}

const RwChar* CNtlDTProp::GetName(void)
{
	return m_strName.c_str();
}

void CNtlDTProp::CalcTransform(void)
{
	RwMatrixSetIdentity(&m_matTrans);

	RwMatrixRotate (&m_matTrans, &CNtlPLGlobal::m_vXAxisV3, m_fPitch, rwCOMBINEPOSTCONCAT);		// Pitch
	RwMatrixRotate (&m_matTrans, &CNtlPLGlobal::m_vYAxisV3, m_fYaw, rwCOMBINEPOSTCONCAT);		// Yaw
	RwMatrixRotate (&m_matTrans, &CNtlPLGlobal::m_vZAxisV3, m_fRoll, rwCOMBINEPOSTCONCAT);		// Roll

	RwMatrixScale(&m_matTrans, &m_vScale, rwCOMBINEPOSTCONCAT);

	RwMatrixTranslate(&m_matTrans, &m_vPos, rwCOMBINEPOSTCONCAT);
}

void CNtlDTProp::AddAngle(RwReal fPitch, RwReal fYaw, RwReal fRoll)
{
	m_fPitch	+= fPitch;
	m_fYaw		+= fYaw;
	m_fRoll		+= fRoll;

	CalcTransform();
}

void CNtlDTProp::AddScale(RwReal fScale)
{
	m_vScale.x	+= fScale;
	m_vScale.y	+= fScale;
	m_vScale.z	+= fScale;

	CalcTransform();
}

void CNtlDTProp::AddPosition(RwV3d vAdd)
{
	RwV3dAdd(&m_vPos, &m_vPos, &vAdd);

	CalcTransform();
}

void CNtlDTProp::ResetTransform(void)
{
	m_fPitch	= 0.0f;
	m_fYaw		= 0.0f;
	m_fRoll		= 0.0f;

	m_vScale.x	= 1.0f;
	m_vScale.y	= 1.0f;
	m_vScale.z	= 1.0f;
	
	m_vPos.x = 0.0;
	m_vPos.y = 0.0;
	m_vPos.z = 0.0;

	RwMatrixSetIdentity(&m_matTrans);
}

const RwMatrix*	CNtlDTProp::GetTransMatrix(void)
{
	return &m_matTrans;
}

RwV3d CNtlDTProp::GetSplineControlPointWorldPosition(RwInt32 iIdx)
{
	return CNtlPLGlobal::m_vZeroV3;
}


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

CNtlDTSpline::CNtlDTSpline()
{
	m_bUseWorldPos			= FALSE;
	m_bUniformLerp			= FALSE;
	m_fUniformSpeed			= 10.0f;

	m_pCtrlPt				= NULL;
	m_pCurvePt				= NULL;
	m_iTotCurveCnt			= NULL;
	m_ppCustomData			= NULL;
}

CNtlDTSpline::~CNtlDTSpline()
{
	DeAllocate();
}

RwBool CNtlDTSpline::Load(CNtlSerializer& s)
{
	CNtlDTProp::Load(s);

	DeAllocate();

	RwUInt8 byType;

	while(1)
	{
		if(s.IsEmpty())
			break;

		s >> byType;

		switch(byType)
		{
		case DTPROP_SPLINE_SERIALIZE_SHAPE:
			{
				RwUInt8 byShape;
				s >> byShape;
				m_sProp.eShape = (EDTSplineShape)byShape;
			}
			break;
		case DTPROP_SPLINE_SERIALIZE_TIME:
			s >> m_sProp.fTime;
			break;
		case DTPROP_SPLINE_SERIALIZE_CTRLNUM:
			s >> m_sProp.iCtrlNum;
			break;
		case DTPROP_SPLINE_SERIALIZE_CURVESUB:
			s >> m_sProp.iCurveSubDivision;
			break;
		case DTPROP_SPLINE_SERIALIZE_RADIUS:
			s >> m_sProp.fRadius;
			break;
		case DTPROP_SPLINE_SERIALIZE_ROTNUM:
			s >> m_sProp.iRotNum;
			break;
		case DTPROP_SPLINE_SERIALIZE_DIST:
			s >> m_sProp.fDist;
			break;
		case DTPROP_SPLINE_SERIALIZE_TOTCURVECNT:
			s >> m_iTotCurveCnt;
			break;
		case DTPROP_SPLINE_SERIALIZE_CTRLDATA:
			m_pCtrlPt = NTL_NEW SSplineCtrlPt [m_sProp.iCtrlNum];
			s.Out(m_pCtrlPt, sizeof(SSplineCtrlPt)*m_sProp.iCtrlNum);
			break;
		case DTPROP_SPLINE_SERIALIZE_CURVEDATA:
			m_pCurvePt		= NTL_NEW SSplineCurvePt[m_iTotCurveCnt];
			s.Out(m_pCurvePt, sizeof(SSplineCurvePt)*m_iTotCurveCnt);
			break;
		case DTPROP_SPLINE_SERIALIZE_STANDARDPOS_FLAG:
			s >> m_bUseWorldPos;
			break;
		case DTPROP_SPLINE_SERIALIZE_STANDARDPOS:
			{
				RwV3d vStandardPos;
				s >> vStandardPos.x;
				s >> vStandardPos.y;
				s >> vStandardPos.z;
			}
			break;
		case DTPROP_SPLINE_SERIALIZE_UNIFORMLERP:
			s >> m_bUniformLerp;
			break;
		case DTPROP_SPLINE_SERIALIZE_UNIFORMSPEED:
			s >> m_fUniformSpeed;
			break;
		case DTPROP_SPLINE_SERIALIZE_CUSTOMDATA:
			{
				RwInt32 iDataSize, iValidCount;
				s >> iDataSize;
				s >> iValidCount;

				m_ppCustomData	= NTL_NEW SSplineCustomData* [m_iTotCurveCnt];
				for(RwInt32 i = 0; i < m_iTotCurveCnt; i++)
					m_ppCustomData[i] = NULL;

				RwInt32 iIdx;

				for(RwInt32 i = 0; i < iValidCount; i++)
				{
					s >> iIdx;
					m_ppCustomData[iIdx] = NTL_NEW SSplineCustomData;
					s.Out(m_ppCustomData[iIdx], iDataSize);
				}
				break;
			}

		case DTPROP_SPLINE_SERIALIZE_END:
			return TRUE;
			break;
		}
	}

	return TRUE;
}

RwBool CNtlDTSpline::Save(CNtlSerializer& s)
{
	CNtlDTProp::Save(s);

	s << (RwUInt8)DTPROP_SPLINE_SERIALIZE_SHAPE;
	s << (RwUInt8)m_sProp.eShape;

	s << (RwUInt8)DTPROP_SPLINE_SERIALIZE_TIME;
	s << m_sProp.fTime;

	s << (RwUInt8)DTPROP_SPLINE_SERIALIZE_CTRLNUM;
	s << m_sProp.iCtrlNum;

	s << (RwUInt8)DTPROP_SPLINE_SERIALIZE_CURVESUB;
	s << m_sProp.iCurveSubDivision;

	s << (RwUInt8)DTPROP_SPLINE_SERIALIZE_RADIUS;
	s << m_sProp.fRadius;

	s << (RwUInt8)DTPROP_SPLINE_SERIALIZE_ROTNUM;
	s << m_sProp.iRotNum;

	s << (RwUInt8)DTPROP_SPLINE_SERIALIZE_DIST;
	s << m_sProp.fDist;
	
	s << (RwUInt8)DTPROP_SPLINE_SERIALIZE_TOTCURVECNT;
	s << m_iTotCurveCnt;

	s << (RwUInt8)DTPROP_SPLINE_SERIALIZE_CTRLDATA;
	s.In(m_pCtrlPt, sizeof(SSplineCtrlPt)*m_sProp.iCtrlNum);

	s << (RwUInt8)DTPROP_SPLINE_SERIALIZE_CURVEDATA;
	s.In(m_pCurvePt, sizeof(SSplineCurvePt)*m_iTotCurveCnt);

	s << (RwUInt8)DTPROP_SPLINE_SERIALIZE_STANDARDPOS_FLAG;
	s << m_bUseWorldPos;

	s << (RwUInt8)DTPROP_SPLINE_SERIALIZE_UNIFORMLERP;
	s << m_bUniformLerp;

	s << (RwUInt8)DTPROP_SPLINE_SERIALIZE_UNIFORMSPEED;
	s << m_fUniformSpeed;

	s << (RwUInt8)DTPROP_SPLINE_SERIALIZE_CUSTOMDATA;

	RwInt32 iValidCount = GetValidSplineCustomDataCount();
	RwInt32 iDataSize = sizeof(SSplineCustomData);
	s << iDataSize;
	s << iValidCount;

	if(m_ppCustomData)
	{
		for(RwInt32 i = 0; i < m_iTotCurveCnt; i++)
		{
			if(m_ppCustomData[i])
			{
				s << i;
				s.In(m_ppCustomData[i], iDataSize);
			}
		}
	}

	s << (RwUInt8)DTPROP_SPLINE_SERIALIZE_END;

	return TRUE;
}

RwBool CNtlDTSpline::Update(RwReal fElapsed)
{
	return TRUE;
}

void CNtlDTSpline::Allocate(SSplineProp *pProp)
{
	DeAllocate();

	memcpy(&m_sProp, pProp, sizeof(SSplineProp));

	m_pCtrlPt		= NTL_NEW SSplineCtrlPt [m_sProp.iCtrlNum];
	m_iTotCurveCnt	= (m_sProp.iCtrlNum-1) * m_sProp.iCurveSubDivision+1;
	m_pCurvePt		= NTL_NEW SSplineCurvePt[m_iTotCurveCnt];

	m_ppCustomData	= NTL_NEW SSplineCustomData* [m_iTotCurveCnt];

	for(RwInt32 i = 0; i < m_iTotCurveCnt; i++)
		m_ppCustomData[i] = NULL;
}

void CNtlDTSpline::DeAllocate(void)
{
	if(m_pCtrlPt)
	{
		NTL_ARRAY_DELETE(m_pCtrlPt);
	}

	if(m_pCurvePt)
	{
		NTL_ARRAY_DELETE(m_pCurvePt);
	}

	if(m_ppCustomData)
	{
		for(RwInt32 i = 0; i < m_iTotCurveCnt; i++)
		{
			if(m_ppCustomData[i])
			{
				NTL_DELETE(m_ppCustomData[i])
				m_ppCustomData[i] = NULL;
			}
		}
		
		NTL_ARRAY_DELETE(m_ppCustomData);
		m_ppCustomData = NULL;
	}

	m_iTotCurveCnt = 0;
}

void CNtlDTSpline::SetData(void *pData)
{
	SSplineProp *pProp = reinterpret_cast<SSplineProp*>( pData );
	Allocate(pProp);
}

void CNtlDTSpline::ReCalcTotTime(void)
{
	RwReal fTotTime = 0.0f;

	for(RwInt32 i = 0; i < m_sProp.iCtrlNum; ++i)
	{
		fTotTime += m_pCtrlPt[i].fTime;
	}
	
	m_sProp.fTime = fTotTime;
}

void CNtlDTSpline::BuildLinePoint(void)
{
	RwMatrix matRot;
	RwReal fAngle = 360.0f;
	RwMatrixSetIdentity(&matRot);

	for(RwInt32 i = 0; i < m_sProp.iCtrlNum; ++i)
	{
		RwV3d vPoint = { 0, 0, m_sProp.fRadius };
		RwV3dTransformPoint(&vPoint, &vPoint, &matRot);

		m_pCtrlPt[i].vPos.x = 0.0f;
		m_pCtrlPt[i].vPos.y = m_sProp.fDist;
		m_pCtrlPt[i].vPos.z = m_sProp.fRadius*i/(m_sProp.iCtrlNum - 1);

		m_pCtrlPt[i].vRot.x = 0.0f;
		m_pCtrlPt[i].vRot.y = 0.0f;
		m_pCtrlPt[i].vRot.z = 0.0f;

		m_pCtrlPt[i].fTime = m_sProp.fTime / m_sProp.iCtrlNum;
	}
}

void CNtlDTSpline::BuildCirclePoint(void)
{
	RwMatrix matRot;
	RwReal fAngle = 360.0f;
	RwMatrixSetIdentity(&matRot);

	for(RwInt32 i = 0; i < m_sProp.iCtrlNum; ++i)
	{
		RwMatrixRotate(&matRot, &CNtlPLGlobal::m_vYAxisV3, fAngle * ((RwReal)i / (m_sProp.iCtrlNum - 1)), rwCOMBINEREPLACE);

		RwV3d vPoint = { 0, 0, m_sProp.fRadius };
		RwV3dTransformPoint(&vPoint, &vPoint, &matRot);

		m_pCtrlPt[i].vPos.x = vPoint.x;
		m_pCtrlPt[i].vPos.y = vPoint.y + m_sProp.fDist;
		m_pCtrlPt[i].vPos.z = vPoint.z;

		m_pCtrlPt[i].vRot.x = 0.0f;
		m_pCtrlPt[i].vRot.y = fAngle * ((RwReal)i / (m_sProp.iCtrlNum - 1));
		m_pCtrlPt[i].vRot.z = 0.0f;

		m_pCtrlPt[i].fTime = m_sProp.fTime / m_sProp.iCtrlNum;
	}
}



void CNtlDTSpline::BuildHurricanePoint(void)
{
	RwMatrix matRot;
	RwReal fAngle = 360.0f * m_sProp.iRotNum;
	RwReal fIncDist = m_sProp.fDist/m_sProp.iCtrlNum;
		
	for(RwInt32 i = 0; i < m_sProp.iCtrlNum; ++ i)
	{
		RwMatrixRotate(&matRot, &CNtlPLGlobal::m_vYAxisV3, fAngle * ((RwReal)(i) / (m_sProp.iCtrlNum - 1)), rwCOMBINEREPLACE);

		RwV3d vPoint = { 0, 0, m_sProp.fRadius };
		RwV3dTransformPoint(&vPoint, &vPoint, &matRot);
		m_pCtrlPt[i].vPos.x = vPoint.x;
		m_pCtrlPt[i].vPos.y = vPoint.y + fIncDist * i;
		m_pCtrlPt[i].vPos.z = vPoint.z;

		m_pCtrlPt[i].vRot.x = 0.0f;
		m_pCtrlPt[i].vRot.y = fAngle * ((RwReal)(i) / (m_sProp.iCtrlNum - 1));
		m_pCtrlPt[i].vRot.z = 0.0f;

		m_pCtrlPt[i].fTime = m_sProp.fTime / m_sProp.iCtrlNum;
	}

	RwReal fTime = m_sProp.fTime / (m_sProp.iCtrlNum-1);

	for(RwInt32 i = 0; i < m_sProp.iCtrlNum-1; ++ i)
	{
		m_pCtrlPt[i].fTime = fTime;
	}

	m_pCtrlPt[m_sProp.iCtrlNum-1].fTime = 0.0f;
}

void CNtlDTSpline::BuildCurvePoint(void)
{
	RwInt32	nIdx = 0;
	RwInt32 i0, i1, i2, i3;

	for (RwInt32 i = -1; i < m_sProp.iCtrlNum; ++i) 
	{
		// For each subdivision
		for(RwInt32 j = 0; j < m_sProp.iCurveSubDivision; ++j) 
		{
			if (nIdx >= m_iTotCurveCnt)
			{
				continue;
			}

			RwReal fWeightValue = 0.5f;
			if(m_sProp.iCurveSubDivision > 0)
				fWeightValue = (RwReal)j / m_sProp.iCurveSubDivision;

			i0 = i;
			i1 = (i+1 >= m_sProp.iCtrlNum) ? m_sProp.iCtrlNum-1 : i+1;
			i2 = (i+2 >= m_sProp.iCtrlNum) ? m_sProp.iCtrlNum-1 : i+2;
			i3 = (i+3 >= m_sProp.iCtrlNum) ? m_sProp.iCtrlNum-1 : i+3;

			if(i0 < 0) i0 = 0;
			if(i1 < 0) i1 = 0;
			if(i2 < 0) i2 = 0;
			if(i3 < 0) i3 = 0;

			D3DXVec3CatmullRom( (D3DXVECTOR3*)&m_pCurvePt[nIdx].vPos,  
								(D3DXVECTOR3*)&m_pCtrlPt[i0].vPos,
								(D3DXVECTOR3*)&m_pCtrlPt[i1].vPos,
								(D3DXVECTOR3*)&m_pCtrlPt[i2].vPos,
								(D3DXVECTOR3*)&m_pCtrlPt[i3].vPos,
								fWeightValue );		

			++ nIdx;
		}
	}
}

void CNtlDTSpline::BuildCurveTime(void)
{
	RwInt32	nIdx = 0;

	for (RwInt32 i = -1; i < m_sProp.iCtrlNum; ++i) 
	{
		// For each subdivision
		for(RwInt32 j = 0; j < m_sProp.iCurveSubDivision; ++j) 
		{
			if (nIdx >= m_iTotCurveCnt)
			{
				continue;
			}

			m_pCurvePt[nIdx].fTime = m_sProp.fTime / m_iTotCurveCnt;

			++ nIdx;
		}
	}
}

void CNtlDTSpline::BuildCurveLineRotate(void)
{
	RwInt32	nIdx = 0;
	
	for (RwInt32 i = -1; i < m_sProp.iCtrlNum; ++i) 
	{
		// For each subdivision
		for(RwInt32 j = 0; j < m_sProp.iCurveSubDivision; ++j) 
		{
			if (nIdx >= m_iTotCurveCnt)
				continue;

			RwMatrix matRot;
			RwMatrixRotate(&matRot, &CNtlPLGlobal::m_vYAxisV3, 0.0f, rwCOMBINEREPLACE);
			RtQuatConvertFromMatrix(&m_pCurvePt[nIdx].qRot, &matRot);
			
			++ nIdx;
		}
	}
}

void CNtlDTSpline::BuildCurveRotate(void)
{
	RwInt32	nIdx = 0;
	RwInt32 i0, i1, i2, i3;

	for (RwInt32 i = -1; i < m_sProp.iCtrlNum; ++i) 
	{
		// For each subdivision
		for(RwInt32 j = 0; j < m_sProp.iCurveSubDivision; ++j) 
		{
			if (nIdx >= m_iTotCurveCnt)
			{
				continue;
			}

			RwReal fWeightValue = 0.5f;
			if(m_sProp.iCurveSubDivision > 0)
				fWeightValue = (RwReal)j / m_sProp.iCurveSubDivision;

			i0 = i;
			i1 = (i+1 >= m_sProp.iCtrlNum) ? m_sProp.iCtrlNum-1 : i+1;
			i2 = (i+2 >= m_sProp.iCtrlNum) ? m_sProp.iCtrlNum-1 : i+2;
			i3 = (i+3 >= m_sProp.iCtrlNum) ? m_sProp.iCtrlNum-1 : i+3;

			if(i0 < 0) i0 = 0;
			if(i1 < 0) i1 = 0;
			if(i2 < 0) i2 = 0;
			if(i3 < 0) i3 = 0;


			RwMatrix matRotate1, matRotate2;
			RwMatrixRotate(&matRotate1, &CNtlPLGlobal::m_vXAxisV3, m_pCtrlPt[i1].vRot.x, rwCOMBINEREPLACE);
			RwMatrixRotate(&matRotate1, &CNtlPLGlobal::m_vYAxisV3, m_pCtrlPt[i1].vRot.y, rwCOMBINEPOSTCONCAT);
			RwMatrixRotate(&matRotate1, &CNtlPLGlobal::m_vZAxisV3, m_pCtrlPt[i1].vRot.z, rwCOMBINEPOSTCONCAT);

			RwMatrixRotate(&matRotate2, &CNtlPLGlobal::m_vXAxisV3, m_pCtrlPt[i2].vRot.x, rwCOMBINEREPLACE);
			RwMatrixRotate(&matRotate2, &CNtlPLGlobal::m_vYAxisV3, m_pCtrlPt[i2].vRot.y, rwCOMBINEPOSTCONCAT);
			RwMatrixRotate(&matRotate2, &CNtlPLGlobal::m_vZAxisV3, m_pCtrlPt[i2].vRot.z, rwCOMBINEPOSTCONCAT);

			RwReal fAngleX, fAngleY;
			RwV3d vRot;

			RwV3dScale(&vRot, &matRotate1.at, -1.0f);
			CNtlMath::LineToAngleXY(&vRot, fAngleX, fAngleY);

			RwMatrixSetIdentity(&matRotate1);
			RwMatrixRotate(&matRotate1, &CNtlPLGlobal::m_vXAxisV3, fAngleX, rwCOMBINEREPLACE);
			RwMatrixRotate(&matRotate1, &CNtlPLGlobal::m_vYAxisV3, fAngleY, rwCOMBINEPOSTCONCAT);

			RwV3dScale(&vRot, &matRotate2.at, -1.0f);
			CNtlMath::LineToAngleXY(&vRot, fAngleX, fAngleY);

			RwMatrixSetIdentity(&matRotate2);
			RwMatrixRotate(&matRotate2, &CNtlPLGlobal::m_vXAxisV3, fAngleX, rwCOMBINEREPLACE);
			RwMatrixRotate(&matRotate2, &CNtlPLGlobal::m_vYAxisV3, fAngleY, rwCOMBINEPOSTCONCAT);

			RtQuat quat1, quat2;
			RtQuatConvertFromMatrix(&quat1, &matRotate1);
			RtQuatConvertFromMatrix(&quat2, &matRotate2);
			
			D3DXQuaternionSlerp((D3DXQUATERNION*)&m_pCurvePt[nIdx].qRot, 
								(D3DXQUATERNION*)&quat1, 
								(D3DXQUATERNION*)&quat2, 
								fWeightValue);

			++ nIdx;
		}
	}
}

void CNtlDTSpline::Build(void)
{
	RwInt32 i = 0;
	for(i = 0; i < m_sProp.iCtrlNum; ++i)
	{
		m_pCtrlPt[i].fTime = m_sProp.fTime * i;
		memset(&m_pCtrlPt[i].vRot, 0, sizeof(RwV3d));
	}

	if(m_sProp.eShape == EDT_SPLINE_SHAPE_LINE)
	{
		BuildLinePoint();
		BuildCurvePoint();
		BuildCurveTime();
		BuildCurveLineRotate();
	}
	else if(m_sProp.eShape == EDT_SPLINE_SHAPE_CIRCLE)
	{
		BuildCirclePoint();
		BuildCurvePoint();
		BuildCurveTime();
		BuildCurveRotate();
	}
	else
	{
		BuildHurricanePoint();
		BuildCurvePoint();
		BuildCurveTime();
		BuildCurveRotate();
	}
}

void CNtlDTSpline::SetCurveRotate(RwInt32 iSx, RwInt32 iEx, RwV3d vAt)
{
	if(iSx >= m_sProp.iCtrlNum)
		return;

	if(iEx >= m_sProp.iCtrlNum)
		iEx = m_sProp.iCtrlNum;

	RwInt32 iCurveSx = iSx*m_sProp.iCurveSubDivision;
	RwInt32 iCurveEx = iEx*m_sProp.iCurveSubDivision;

	if(iEx == m_sProp.iCtrlNum)
		iCurveEx = (iEx-1)*m_sProp.iCurveSubDivision + 1;


	RwV3dNormalize(&vAt, &vAt);
	
	RwReal fAngleX, fAngleY;
	RwMatrix matRot;
	
	for(RwInt32 i = iCurveSx; i < iCurveEx; ++i)
	{
		if (i >= m_iTotCurveCnt)
			continue;

		CNtlMath::LineToAngleXY(&vAt, fAngleX, fAngleY);

		RwMatrixRotate(&matRot, &CNtlPLGlobal::m_vXAxisV3, fAngleX, rwCOMBINEREPLACE);
		RwMatrixRotate(&matRot, &CNtlPLGlobal::m_vYAxisV3, fAngleY, rwCOMBINEPOSTCONCAT);

		RtQuatConvertFromMatrix(&m_pCurvePt[i].qRot, &matRot);
	}
}

void CNtlDTSpline::SetCurveRotate(RwInt32 iSx, std::list<RwV3d>& listAt)
{
	if(iSx >= m_sProp.iCtrlNum)
		return;

	std::list<RwV3d>::iterator it = listAt.begin();

	RwReal fAngleX, fAngleY;
	RwV3d vAt;
	RwMatrix matRot;

	RwInt32 iCurveSx = iSx*m_sProp.iCurveSubDivision;

	for(RwInt32 i = 0; i < m_sProp.iCurveSubDivision; i++)
	{
		if (iCurveSx + i >= m_iTotCurveCnt)
			continue;

		if(it == listAt.end())
			return;

		vAt = (*it); 

		RwV3dNormalize(&vAt, &vAt);
		CNtlMath::LineToAngleXY(&vAt, fAngleX, fAngleY);

		RwMatrixRotate(&matRot, &CNtlPLGlobal::m_vXAxisV3, fAngleX, rwCOMBINEREPLACE);
		RwMatrixRotate(&matRot, &CNtlPLGlobal::m_vYAxisV3, fAngleY, rwCOMBINEPOSTCONCAT);

		RtQuatConvertFromMatrix(&m_pCurvePt[iCurveSx + i].qRot, &matRot);

		it++;
	}
}

void CNtlDTSpline::SetCurvePointDirectRotate(RwInt32 iSx, RwInt32 iEx)
{
	if(iSx >= m_sProp.iCtrlNum)
		return;

	if(iEx >= m_sProp.iCtrlNum)
		iEx = m_sProp.iCtrlNum;

	RwInt32 iCurveSx = iSx*m_sProp.iCurveSubDivision;
	RwInt32 iCurveEx = iEx*m_sProp.iCurveSubDivision;

	if(iEx == m_sProp.iCtrlNum)
		iCurveEx = (iEx-1)*m_sProp.iCurveSubDivision + 1;


	RwV3d vDir;
	
	RwReal fAngleX, fAngleY;
	RwMatrix matRot;
	
	for(RwInt32 i = iCurveSx; i < iCurveEx; ++i)
	{
		if (i >= m_iTotCurveCnt)
			continue;

		if(i+1 >= m_iTotCurveCnt)
		{
			RwV3dSubMacro(&vDir, &m_pCurvePt[i].vPos, &m_pCurvePt[i-1].vPos);
			RwV3dNormalize(&vDir, &vDir);
		}
		else
		{
			RwV3dSubMacro(&vDir, &m_pCurvePt[i+1].vPos, &m_pCurvePt[i].vPos);
			RwV3dNormalize(&vDir, &vDir);
		}

		CNtlMath::LineToAngleXY(&vDir, fAngleX, fAngleY);

		RwMatrixRotate(&matRot, &CNtlPLGlobal::m_vXAxisV3, fAngleX, rwCOMBINEREPLACE);
		RwMatrixRotate(&matRot, &CNtlPLGlobal::m_vYAxisV3, fAngleY, rwCOMBINEPOSTCONCAT);

		RtQuatConvertFromMatrix(&m_pCurvePt[i].qRot, &matRot);
	}
}


void CNtlDTSpline::SetCurveRotateInterpolation(RwInt32 iSx, RwInt32 iEx)
{
	if(iSx >= m_sProp.iCtrlNum)
		return;

	if(iEx >= m_sProp.iCtrlNum)
		iEx = m_sProp.iCtrlNum;

	RwInt32 iCurveSx = iSx*m_sProp.iCurveSubDivision;
	RwInt32 iCurveEx = iEx*m_sProp.iCurveSubDivision;

	if(iEx == m_sProp.iCtrlNum)
		iCurveEx = (iEx-1)*m_sProp.iCurveSubDivision + 1;


	RwV3d vDir;
	RwReal fAngleX, fAngleY;
	RwMatrix matRot;
	RwInt32 iInterNum = 0;
	RwV3d vRot[3];
	
	for(RwInt32 i = iCurveSx; i < iCurveEx; ++i)
	{
		if (i >= m_iTotCurveCnt)
			continue;

		if(i-1 >= 0)
		{
			RtQuatConvertToMatrix(&m_pCurvePt[i-1].qRot, &matRot);
			RwV3dAssignMacro(&vRot[iInterNum], &matRot.at);
			iInterNum++;
		}

		RtQuatConvertToMatrix(&m_pCurvePt[i].qRot, &matRot);
		RwV3dAssignMacro(&vRot[iInterNum], &matRot.at);
		iInterNum++;

		if(i+1 < m_iTotCurveCnt)
		{
			RtQuatConvertToMatrix(&m_pCurvePt[i+1].qRot, &matRot);
			RwV3dAssignMacro(&vRot[iInterNum], &matRot.at);
			iInterNum++;
		}

		if(iInterNum == 0)
			continue;

		vDir.x = 0.0f;
		vDir.y = 0.0f;
		vDir.z = 0.0f;

		for(int j = 0; j < iInterNum; j++)
		{
			vDir.x += vRot[j].x;
			vDir.y += vRot[j].y;
			vDir.z += vRot[j].z;
		}

		vDir.x = vDir.x / (RwReal)iInterNum;
		vDir.y = vDir.y / (RwReal)iInterNum;
		vDir.z = vDir.z / (RwReal)iInterNum;

		RwV3dNormalize(&vDir, &vDir);

		iInterNum = 0;

		CNtlMath::LineToAngleXY(&vDir, fAngleX, fAngleY);

		RwMatrixRotate(&matRot, &CNtlPLGlobal::m_vXAxisV3, fAngleX, rwCOMBINEREPLACE);
		RwMatrixRotate(&matRot, &CNtlPLGlobal::m_vYAxisV3, fAngleY, rwCOMBINEPOSTCONCAT);

		RtQuatConvertFromMatrix(&m_pCurvePt[i].qRot, &matRot);
	}
}


void CNtlDTSpline::InsertCustomData(RwInt32 iNewTotNum, RwInt32 iOldTotNum, RwInt32 iBeofreIdx, RwInt32 iAfterIdx)
{
    if(iBeofreIdx > iNewTotNum)
        return;

	if(m_ppCustomData == NULL)
		return;

	SSplineCustomData **ppCustomData = NTL_NEW SSplineCustomData* [iNewTotNum];

	for(RwInt32 i = 0; i < iNewTotNum; i++)
	{
		ppCustomData[i] = NULL;
	}

	for(RwInt32 i = 0; i < iBeofreIdx; i++)
	{
		if(i >= iOldTotNum)
			continue;

		ppCustomData[i] = m_ppCustomData[i];
	}

	for(RwInt32 i = iAfterIdx; i < iNewTotNum; i++)
	{
		if(i >= iOldTotNum)
			continue;

		ppCustomData[i] = m_ppCustomData[iBeofreIdx+i-iAfterIdx];
	}

	NTL_ARRAY_DELETE(m_ppCustomData);

	m_ppCustomData = ppCustomData;
}

void CNtlDTSpline::RemoveCustomData(RwInt32 iNewTotNum, RwInt32 iOldTotNum)
{
	if(m_ppCustomData == NULL)
		return;

	SSplineCustomData **ppCustomData = NTL_NEW SSplineCustomData* [iNewTotNum];

	for(RwInt32 i = 0; i < iNewTotNum; i++)
	{
		ppCustomData[i] = NULL;
	}

	for(RwInt32 i = 0; i < iNewTotNum; i++)
	{
		ppCustomData[i] = m_ppCustomData[i];
	}

	NTL_ARRAY_DELETE(m_ppCustomData);

	m_ppCustomData = ppCustomData;
}

void CNtlDTSpline::RemoveCustomData(RwInt32 iNewTotNum, RwInt32 iOldTotNum, RwInt32 iBeofreIdx, RwInt32 iAfterIdx)
{
	if(m_ppCustomData == NULL)
		return;

	SSplineCustomData **ppCustomData = NTL_NEW SSplineCustomData* [iNewTotNum];

	for(RwInt32 i = 0; i < iNewTotNum; i++)
	{
		ppCustomData[i] = NULL;
	}

	for(RwInt32 i = 0; i < iBeofreIdx; i++)
	{
		if(i >= iNewTotNum)
			continue;

		ppCustomData[i] = m_ppCustomData[i];
	}

	RwInt32 iCopyIdx;
	for(RwInt32 i = iBeofreIdx; i < iNewTotNum; i++)
	{
		if(i >= iNewTotNum)
			continue;

		iCopyIdx = iAfterIdx+i-iBeofreIdx;
		if(iCopyIdx >= iOldTotNum)
			continue;

		ppCustomData[i] = m_ppCustomData[iCopyIdx];
	}

	NTL_ARRAY_DELETE(m_ppCustomData);

	m_ppCustomData = ppCustomData;
}


void CNtlDTSpline::InsertControlPoint(RwInt32 iIdx, const RwV3d *pPos, const RwV3d *pDir)
{
	if( iIdx < 0)
		iIdx = 0;

	if(iIdx >= m_sProp.iCtrlNum)
		iIdx = m_sProp.iCtrlNum - 1;

	m_sProp.iCtrlNum++;
	
	RwInt32 iNewTotCurveCnt		= (m_sProp.iCtrlNum-1) * m_sProp.iCurveSubDivision + 1;
	SSplineCtrlPt *pNewCtrlPt	= NTL_NEW SSplineCtrlPt [m_sProp.iCtrlNum];
	SSplineCurvePt *pNewCurvePt	= NTL_NEW SSplineCurvePt[iNewTotCurveCnt];

	InsertCustomData( iNewTotCurveCnt, m_iTotCurveCnt, (iIdx+1)*m_sProp.iCurveSubDivision, (iIdx+2)*m_sProp.iCurveSubDivision );

	RwInt32 iCtrlNum	= iIdx+1;
	RwInt32 iCurveNum	= (iIdx+1) * m_sProp.iCurveSubDivision;

	if(iCurveNum >= m_iTotCurveCnt)
		iCurveNum = m_iTotCurveCnt;

	memcpy(pNewCtrlPt, m_pCtrlPt, sizeof(SSplineCtrlPt)*iCtrlNum);
	memcpy(pNewCurvePt, m_pCurvePt, sizeof(SSplineCurvePt)*iCurveNum);

	RwInt32 iCtrlIdx	= iIdx+2;
	RwInt32 iCurveIdx	= (iIdx+2) * m_sProp.iCurveSubDivision;

	// control point position
	RwInt32 iCopyCtrlIdx, iCopyCurveIdx;

	iCtrlNum  = m_sProp.iCtrlNum - iCtrlNum - 1;
	iCurveNum = iNewTotCurveCnt - iCurveNum - m_sProp.iCurveSubDivision;

	iCopyCtrlIdx	= iIdx+1;
	iCopyCurveIdx	= (iIdx+1) * m_sProp.iCurveSubDivision;

	if(iCtrlNum > 0 && iCurveNum > 0)
	{
		memcpy(&pNewCtrlPt[iCtrlIdx], &m_pCtrlPt[iCopyCtrlIdx], sizeof(SSplineCtrlPt)*iCtrlNum);
		memcpy(&pNewCurvePt[iCurveIdx], &m_pCurvePt[iCopyCurveIdx], sizeof(SSplineCurvePt)*iCurveNum);
	}

	NTL_ARRAY_DELETE(m_pCtrlPt);
	NTL_ARRAY_DELETE(m_pCurvePt);

	m_iTotCurveCnt	= iNewTotCurveCnt;
	m_pCtrlPt		= pNewCtrlPt;
	m_pCurvePt		= pNewCurvePt;

	
	iCopyCtrlIdx	= iIdx;
	iCtrlIdx		= iIdx+1;

	RwV3dAssignMacro(&m_pCtrlPt[iCtrlIdx].vPos, pPos);
	SetCurveRotate(iCtrlIdx-1, iCtrlIdx, *(RwV3d*)pDir);
	m_pCtrlPt[iCtrlIdx].fTime	= m_pCtrlPt[iCopyCtrlIdx].fTime;

	BuildCurvePoint();

	iCurveIdx		= (iIdx+1) * m_sProp.iCurveSubDivision;
	iCopyCurveIdx	= iIdx * m_sProp.iCurveSubDivision;

	if(iCurveIdx > m_iTotCurveCnt - m_sProp.iCurveSubDivision)
		iCurveIdx = iIdx * m_sProp.iCurveSubDivision;

	for(RwInt32 i = 0; i < m_sProp.iCurveSubDivision; i++)
	{
		m_pCurvePt[iCurveIdx+i].qRot = m_pCurvePt[iCopyCurveIdx+i].qRot;
		m_pCurvePt[iCurveIdx+i].fTime = m_pCurvePt[iCopyCurveIdx+i].fTime;
	}

	ReCalcTotTime();
}

void CNtlDTSpline::RemoveControlPoint(void)
{
	if(m_sProp.iCtrlNum <= 2)
		return;

	m_sProp.iCtrlNum--;
	
	RwInt32 iNewTotCurveCnt		= (m_sProp.iCtrlNum-1) * m_sProp.iCurveSubDivision+1;
	SSplineCtrlPt *pNewCtrlPt	= NTL_NEW SSplineCtrlPt [m_sProp.iCtrlNum];
	SSplineCurvePt *pNewCurvePt	= NTL_NEW SSplineCurvePt[iNewTotCurveCnt];

	RemoveCustomData(iNewTotCurveCnt, m_iTotCurveCnt);

	memcpy(pNewCtrlPt, m_pCtrlPt, sizeof(SSplineCtrlPt)*m_sProp.iCtrlNum);
	memcpy(pNewCurvePt, m_pCurvePt, sizeof(SSplineCurvePt)*iNewTotCurveCnt);

	NTL_ARRAY_DELETE(m_pCtrlPt);
	NTL_ARRAY_DELETE(m_pCurvePt);

	m_iTotCurveCnt	= iNewTotCurveCnt;
	m_pCtrlPt		= pNewCtrlPt;
	m_pCurvePt		= pNewCurvePt;

	ReCalcTotTime();
}

void CNtlDTSpline::RemoveControlPoint(RwInt32 iIdx)
{
	if(m_sProp.iCtrlNum <= 2)
		return;

	if( iIdx < 0 || iIdx >= m_sProp.iCtrlNum)
		iIdx = m_sProp.iCtrlNum - 1;

	m_sProp.iCtrlNum--;

	
	RwInt32 iNewTotCurveCnt		= (m_sProp.iCtrlNum-1) * m_sProp.iCurveSubDivision + 1;
	SSplineCtrlPt *pNewCtrlPt	= NTL_NEW SSplineCtrlPt [m_sProp.iCtrlNum];
	SSplineCurvePt *pNewCurvePt	= NTL_NEW SSplineCurvePt[iNewTotCurveCnt];

	RemoveCustomData(iNewTotCurveCnt, m_iTotCurveCnt, (iIdx) * m_sProp.iCurveSubDivision, (iIdx+1) * m_sProp.iCurveSubDivision);

	RwInt32 iCtrlNum	= iIdx;
	RwInt32 iCurveNum	= (iIdx) * m_sProp.iCurveSubDivision;
	if(iCurveNum > iNewTotCurveCnt)
		iCurveNum = iNewTotCurveCnt;

	if(iCtrlNum > 0)
	{
		memcpy(pNewCtrlPt, m_pCtrlPt, sizeof(SSplineCtrlPt)*iCtrlNum);
		memcpy(pNewCurvePt, m_pCurvePt, sizeof(SSplineCurvePt)*iCurveNum);
	}

	RwInt32 iCtrlIdx	= iIdx;
	RwInt32 iCurveIdx	= iIdx * m_sProp.iCurveSubDivision;

	// control point position
	RwInt32 iCopyCtrlIdx, iCopyCurveIdx;

	iCtrlNum  = m_sProp.iCtrlNum - iCtrlNum;
	iCurveNum = iNewTotCurveCnt - iCurveNum;

	if(iCurveNum < 0)
		iCurveNum = 1;

	iCopyCtrlIdx	= iIdx+1;
	iCopyCurveIdx	= (iIdx+1) * m_sProp.iCurveSubDivision;

	if(iCopyCurveIdx > m_iTotCurveCnt)
		iCopyCurveIdx = m_iTotCurveCnt - 1;

	if(iCtrlNum > 0 && iCurveNum > 0)
	{
		memcpy(&pNewCtrlPt[iCtrlIdx], &m_pCtrlPt[iCopyCtrlIdx], sizeof(SSplineCtrlPt)*iCtrlNum);
		memcpy(&pNewCurvePt[iCurveIdx], &m_pCurvePt[iCopyCurveIdx], sizeof(SSplineCurvePt)*iCurveNum);
	}

	NTL_ARRAY_DELETE(m_pCtrlPt);
	NTL_ARRAY_DELETE(m_pCurvePt);

	m_iTotCurveCnt	= iNewTotCurveCnt;
	m_pCtrlPt		= pNewCtrlPt;
	m_pCurvePt		= pNewCurvePt;

	ReCalcTotTime();
}

void CNtlDTSpline::ChangeControlPointTime(RwReal fSumTime)
{
	RwReal fCtrlTime = fSumTime / (RwReal)m_sProp.iCtrlNum;
	
	for(RwInt32 i = 0; i < m_sProp.iCtrlNum; ++i)
	{
		m_pCtrlPt[i].fTime = fCtrlTime;
	}

	RwReal fCurveTime = fSumTime / (RwReal)(m_iTotCurveCnt-1);

	for(RwInt32 i = 0; i < m_iTotCurveCnt; ++i)
	{
		m_pCurvePt[i].fTime = fCurveTime;
	}

	ReCalcTotTime();
}

void CNtlDTSpline::ChangeControlPointTime(RwInt32 iSx, RwInt32 iEx, RwReal fTime)
{
	if(iSx >= m_sProp.iCtrlNum)
		return;

	if(iEx >= m_sProp.iCtrlNum)
		iEx = m_sProp.iCtrlNum;
	
	for(RwInt32 i = iSx; i < iEx; ++i)
	{
		m_pCtrlPt[i].fTime = fTime;
	}

	RwInt32 iCurveSx = iSx*m_sProp.iCurveSubDivision;
	RwInt32 iCurveEx = iEx*m_sProp.iCurveSubDivision+1;
	RwReal fCurveTime = fTime/m_sProp.iCurveSubDivision;

	for(RwInt32 i = iCurveSx; i < iCurveEx; ++i)
	{
		if(i < 0 || i >= m_iTotCurveCnt)
			continue;

		m_pCurvePt[i].fTime = fCurveTime;
	}

	ReCalcTotTime();
}

void CNtlDTSpline::AddControlPointPosition(RwInt32 iSx, RwInt32 iEx, RwV3d vDelta)
{
	if(iSx >= m_sProp.iCtrlNum || iEx > m_sProp.iCtrlNum)
		return;

	for(RwInt32 i = iSx; i < iEx; ++i)
	{
		RwV3dAdd(&m_pCtrlPt[i].vPos, &m_pCtrlPt[i].vPos, &vDelta);
	}

	BuildCurvePoint();

	ReCalcTotTime();
}

void CNtlDTSpline::SetControlPointPosition(RwInt32 iSx, RwInt32 iEx, RwV3d vPos)
{
	if(iSx >= m_sProp.iCtrlNum || iEx > m_sProp.iCtrlNum)
		return;

	for(RwInt32 i = iSx; i < iEx; ++i)
	{
		RwV3dAssignMacro(&m_pCtrlPt[i].vPos, &vPos);
	}

	BuildCurvePoint();
}

void CNtlDTSpline::SetControlPointInterpolation(RwInt32 iSx, RwInt32 iEx)
{
	if(iSx < 0)
		iSx = 0;

	if(iSx >= m_sProp.iCtrlNum)
		iSx = m_sProp.iCtrlNum;

	if(iEx < 0)
		iEx = 0;

	if(iEx >= m_sProp.iCtrlNum)
		iEx = m_sProp.iCtrlNum;

	RwInt32 iInterNum = 0;
	RwV3d vNewPos;
	RwV3d vInterPos[3];

	for(RwInt32 i = iSx; i < iEx; ++i)
	{
		if(i - 1 >= 0)
		{
			RwV3dAssignMacro(&vInterPos[iInterNum], &m_pCtrlPt[i-1].vPos);
			iInterNum++;
		}

		RwV3dAssignMacro(&vInterPos[iInterNum], &m_pCtrlPt[i].vPos);
		iInterNum++;

		if(i + 1 < m_sProp.iCtrlNum)
		{
			RwV3dAssignMacro(&vInterPos[iInterNum], &m_pCtrlPt[i+1].vPos);
			iInterNum++;
		}

		if(iInterNum == 0)
			continue;

		vNewPos.x = 0;
		vNewPos.y = 0;
		vNewPos.z = 0;

		for(RwInt32 j = 0; j < iInterNum; j++)
		{
			vNewPos.x += vInterPos[j].x;
			vNewPos.y += vInterPos[j].y;
			vNewPos.z += vInterPos[j].z;
		}


		vNewPos.x = vNewPos.x/(RwReal)iInterNum;
		vNewPos.y = vNewPos.y/(RwReal)iInterNum;
		vNewPos.z = vNewPos.z/(RwReal)iInterNum;

		iInterNum = 0;

		RwV3dAssignMacro(&m_pCtrlPt[i].vPos, &vNewPos);
	}

	BuildCurvePoint();
}


void CNtlDTSpline::AddCurveRotate(RwInt32 iSx, RwInt32 iEx, RwReal fFront, RwReal fYaw)
{
	if(iSx >= m_sProp.iCtrlNum-1)
		return;

	if(iEx >= m_sProp.iCtrlNum)
		iEx = m_sProp.iCtrlNum-1;

	RwInt32 iCurveSx = iSx*m_sProp.iCurveSubDivision;
	RwInt32 iCurveEx = iEx*m_sProp.iCurveSubDivision;

	RwMatrix mat;
	
	for(RwInt32 i = iCurveSx; i < iCurveEx; ++i)
	{
		if (i >= m_iTotCurveCnt)
			continue;

		RtQuatConvertToMatrix(&m_pCurvePt[i].qRot, &mat);
		RtQuatRotate(&m_pCurvePt[i].qRot, &mat.right, fFront, rwCOMBINEPOSTCONCAT);
		RtQuatRotate(&m_pCurvePt[i].qRot, &CNtlPLGlobal::m_vYAxisV3, fYaw, rwCOMBINEPOSTCONCAT);
	}
}

RwReal CNtlDTSpline::GetSumPointTime(RwInt32 iSx, RwInt32 iEx)
{
	RwReal fSumTime = 0.0f;
	for(RwInt32 i = iSx; i < iEx; ++i)
	{
		fSumTime += m_pCtrlPt[i].fTime;
	}

	return fSumTime;
}

RwV3d CNtlDTSpline::GetSplineControlPointWorldPosition(RwInt32 iIdx)
{
	RwV3d vPos = { 0.0f, 0.0f, 0.0f };

	if( iIdx < 0 || iIdx >= m_sProp.iCtrlNum )
		return vPos;

	m_pCtrlPt[iIdx].vPos;

	RwV3dTransformPoints(&vPos, &m_pCtrlPt[iIdx].vPos, 1, &m_matTrans);

	return vPos;
}

RwV3d CNtlDTSpline::GetSplineControlPointRotate(RwInt32 iIdx)
{
	if(iIdx > m_sProp.iCtrlNum-1)
		return CNtlPLGlobal::m_vXAxisV3;

	RwInt32 iCurve = iIdx*m_sProp.iCurveSubDivision;

	RwMatrix mat;
	RtQuatConvertToMatrix(&m_pCurvePt[iCurve].qRot, &mat);

	return mat.at;
}

void CNtlDTSpline::GetSplineControlPointRotate(RwInt32 iIdx, std::list<RwV3d>& listRot)
{
	if(iIdx >= m_sProp.iCtrlNum)
		return;

	RwInt32 iCurveSubDivision = m_sProp.iCurveSubDivision;
	if(iIdx == m_sProp.iCtrlNum)
		iCurveSubDivision = 1;

	RwInt32 iCurve = iIdx*m_sProp.iCurveSubDivision;

	RwMatrix mat;

	for(RwInt32 i = 0; i < m_sProp.iCurveSubDivision; i++)
	{
		RtQuatConvertToMatrix(&m_pCurvePt[iCurve+i].qRot, &mat);
		listRot.push_back(mat.at);
	}
}

void CNtlDTSpline::SetSplineCustomSoundData(RwInt32 iCurvePointIdx, const RwChar *pFileName, RwBool bLoop) 
{
	if(m_ppCustomData[iCurvePointIdx] == NULL)
	{
		m_ppCustomData[iCurvePointIdx] = NTL_NEW SSplineCustomData;
		m_ppCustomData[iCurvePointIdx]->bSoundLoop = FALSE;
		m_ppCustomData[iCurvePointIdx]->chSoundFile[0] = 0;
	}

	m_ppCustomData[iCurvePointIdx]->bSoundLoop = bLoop;
	strcpy_s(m_ppCustomData[iCurvePointIdx]->chSoundFile, SPLINE_CUSTOM_SOUND_FILNE_LEN, pFileName);
}

void CNtlDTSpline::ClearSplineCustomData(RwInt32 iCurvePointIdx)
{
	if(m_ppCustomData[iCurvePointIdx])
	{
		NTL_DELETE(m_ppCustomData[iCurvePointIdx]);
		m_ppCustomData[iCurvePointIdx] = NULL;
	}
}

RwInt32	CNtlDTSpline::GetValidSplineCustomDataCount(void)
{
	if(m_ppCustomData == NULL)
		return 0;

	RwInt32 iNum = 0;
	for(RwInt32 i = 0; i < m_iTotCurveCnt; i++)
	{
		if(m_ppCustomData[i])
			iNum++;
	}

	return iNum;
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

CNtlDTContainer::CNtlDTContainer()
{
		
}

CNtlDTContainer::~CNtlDTContainer()
{
	
}

RwBool CNtlDTContainer::Create(const RwChar *pFileName)
{
	return TRUE;
}

void CNtlDTContainer::Destroy(void)
{
	CNtlDTProp *pDTProp;
	HMapDTProp::iterator it;
	for(it = m_hmapDTProp.begin(); it != m_hmapDTProp.end(); ++it)
	{
		pDTProp = (*it).second;
		NTL_DELETE(pDTProp);
	}

	m_hmapDTProp.clear();
}

RwBool CNtlDTContainer::Load(const RwChar *pFileName)
{
	// file open

//	NtlFileAttrReadOnlyRelease( (RwChar*)pFileName);

	CNtlSerializer s;

	if ( GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_PROPERTY )    
	{
		void* pBuffer;
		int iSize;

		GetNtlResourcePackManager()->LoadProperty( pFileName, &pBuffer, &iSize );

		DBO_ASSERTE( pBuffer );

		if ( NULL == pBuffer )
		{
			return FALSE;
		}

		s.In( pBuffer, iSize );

		NTL_ARRAY_DELETE( pBuffer );
	}
	else
	{
		FILE* fp = NULL;

		if ( fopen_s(&fp, pFileName, "rb") != 0 )
		{
			DBO_FAIL( "Open the file is failed" << pFileName );
			return FALSE;
		}

		fseek( fp, 0, SEEK_END );
		RwInt32 iSize = ftell( fp );
		fseek( fp, 0, SEEK_SET );

		DBO_ASSERTE( iSize );

		if ( iSize == 0 )
		{
			fclose( fp );
			return FALSE;
		}

		char* pBuffer = NTL_NEW char[iSize+1];
		DBO_ASSERTE( pBuffer );

		fread( pBuffer, iSize, 1, fp );
		pBuffer[iSize] = '\0';

		fclose(fp);

		s.In( pBuffer, iSize );

		NTL_ARRAY_DELETE( pBuffer );
	}

	// date load

	RwUInt8 byType;
	RwReal fVer;

	while(1)
	{
		if(s.IsEmpty())
			return TRUE;

		s >> byType;

		switch(byType)
		{
		case DTCONTAINER_SERIALIZE_VER:
			s >> fVer;
			break;
		case DTCONTAINER_SERIALIZE_PROPKEY:
			s >> g_uiDTPropKey;
			break;
		case DTCONTAINER_SERIALIZE_PROPDATA:
			{
				CNtlDTProp *pProp = NTL_NEW CNtlDTSpline;
				pProp->Load(s);
				m_hmapDTProp[pProp->GetName()] = pProp;
			}
			break;
		case DTCONTAINER_SERIALIZE_END:
			return TRUE;
			break;
		}
	}
	
}

RwBool CNtlDTContainer::Save(const RwChar *pFileName)
{
	// buffer save

	RwInt32 iBufferSize = 1024*1024*10;
	CNtlSerializer s(iBufferSize, iBufferSize/10);

	s << (RwUInt8)DTCONTAINER_SERIALIZE_VER;
	s << (RwReal)DT_VER;

	s << (RwUInt8)DTCONTAINER_SERIALIZE_PROPKEY;
	s << g_uiDTPropKey;
	
	CNtlDTProp *pDTProp;
	
	HMapDTProp::iterator it;
	for(it = m_hmapDTProp.begin(); it != m_hmapDTProp.end(); it++)
	{
		pDTProp = (*it).second;
		s << (RwUInt8)DTCONTAINER_SERIALIZE_PROPDATA;
		pDTProp->Save(s);
	}

	s << (RwUInt8)DTCONTAINER_SERIALIZE_END;

	// file save
	NtlFileAttrReadOnlyRelease((RwChar*)pFileName);

	FILE *fp = NULL;

	if(fopen_s(&fp, pFileName, "wb") != 0)
	{
		return FALSE;
	}

	fwrite(s.GetData(), s.GetDataSize(), 1, fp);

	fclose(fp);

	return TRUE;
}

CNtlDTProp* CNtlDTContainer::CreateSplineProp(SSplineProp& sProp)
{
	CNtlDTProp *pDTProp = NTL_NEW CNtlDTSpline;

	pDTProp->SetData(&sProp);
	pDTProp->Build();

	m_hmapDTProp[pDTProp->GetName()] = pDTProp;

	return pDTProp;
}

void CNtlDTContainer::DeleteProp(CNtlDTProp *pProp)
{
	HMapDTProp::iterator it;
	it = m_hmapDTProp.find(pProp->GetName());
	if(it == m_hmapDTProp.end())
		return;

	NTL_DELETE(pProp);

	m_hmapDTProp.erase(it);
}

void CNtlDTContainer::RenameProp(CNtlDTProp *pProp, const RwChar *pNewName)
{
	HMapDTProp::iterator it;
	it = m_hmapDTProp.find(pProp->GetName());
	if(it != m_hmapDTProp.end())
		m_hmapDTProp.erase(it);

	pProp->SetName(pNewName);
	m_hmapDTProp[pProp->GetName()] = pProp;
}

void CNtlDTContainer::Append(CNtlDTContainer& container)
{
	HMapDTProp::iterator it;
	HMapDTProp& mapDTProp = container.Gets();

	CNtlDTProp *pProp;

	for(it = mapDTProp.begin(); it != mapDTProp.end(); it++)
	{
		pProp = (*it).second;
		m_hmapDTProp[pProp->GetName()] = pProp;
	}

	mapDTProp.clear();
}

CNtlDTProp* CNtlDTContainer::Find(const RwChar *pName)
{
	HMapDTProp::iterator it = m_hmapDTProp.find(pName);
	if(it == m_hmapDTProp.end())
		return NULL;

	return (*it).second;
}

HMapDTProp& CNtlDTContainer::Gets(void)
{
	return m_hmapDTProp;
}


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

RwUInt8 CNtlDTRender::m_byDTRenderMark = DT_RENDER_MARK_ALL;

void SelectPointBox(const RwV3d *pPos, const RwRGBA *pColor, RwMatrix *pWorldMatrix, RwReal fOffset)
{
	static RwIm3DVertex LineVertex[24];

	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void *)NULL);

	// 박스 라인 1.
	RwIm3DVertexSetPos(&LineVertex[0], pPos->x-fOffset, pPos->y+fOffset, pPos->z+fOffset);
	RwIm3DVertexSetRGBA(&LineVertex[0], pColor->red, pColor->green, pColor->blue, pColor->alpha);

	RwIm3DVertexSetPos(&LineVertex[1], pPos->x+fOffset, pPos->y+fOffset, pPos->z+fOffset);
	RwIm3DVertexSetRGBA(&LineVertex[1], pColor->red, pColor->green, pColor->blue, pColor->alpha);

	RwIm3DVertexSetPos(&LineVertex[2], pPos->x-fOffset, pPos->y+fOffset, pPos->z-fOffset);
	RwIm3DVertexSetRGBA(&LineVertex[2], pColor->red, pColor->green, pColor->blue, pColor->alpha);

	RwIm3DVertexSetPos(&LineVertex[3], pPos->x+fOffset, pPos->y+fOffset, pPos->z-fOffset);
	RwIm3DVertexSetRGBA(&LineVertex[3], pColor->red, pColor->green, pColor->blue, pColor->alpha);

	RwIm3DVertexSetPos(&LineVertex[4], pPos->x-fOffset, pPos->y+fOffset, pPos->z+fOffset);
	RwIm3DVertexSetRGBA(&LineVertex[4], pColor->red, pColor->green, pColor->blue, pColor->alpha);

	RwIm3DVertexSetPos(&LineVertex[5], pPos->x-fOffset, pPos->y+fOffset, pPos->z-fOffset);
	RwIm3DVertexSetRGBA(&LineVertex[5], pColor->red, pColor->green, pColor->blue, pColor->alpha);

	RwIm3DVertexSetPos(&LineVertex[6], pPos->x+fOffset, pPos->y+fOffset, pPos->z+fOffset);
	RwIm3DVertexSetRGBA(&LineVertex[6], pColor->red, pColor->green, pColor->blue, pColor->alpha);

	RwIm3DVertexSetPos(&LineVertex[7], pPos->x+fOffset, pPos->y+fOffset, pPos->z-fOffset);
	RwIm3DVertexSetRGBA(&LineVertex[7], pColor->red, pColor->green, pColor->blue, pColor->alpha);




	RwIm3DVertexSetPos(&LineVertex[8], pPos->x-fOffset, pPos->y-fOffset, pPos->z+fOffset);
	RwIm3DVertexSetRGBA(&LineVertex[8], pColor->red, pColor->green, pColor->blue, pColor->alpha);

	RwIm3DVertexSetPos(&LineVertex[9], pPos->x+fOffset, pPos->y-fOffset, pPos->z+fOffset);
	RwIm3DVertexSetRGBA(&LineVertex[9], pColor->red, pColor->green, pColor->blue, pColor->alpha);

	RwIm3DVertexSetPos(&LineVertex[10], pPos->x-fOffset, pPos->y-fOffset, pPos->z-fOffset);
	RwIm3DVertexSetRGBA(&LineVertex[10], pColor->red, pColor->green, pColor->blue, pColor->alpha);

	RwIm3DVertexSetPos(&LineVertex[11], pPos->x+fOffset, pPos->y-fOffset, pPos->z-fOffset);
	RwIm3DVertexSetRGBA(&LineVertex[11], pColor->red, pColor->green, pColor->blue, pColor->alpha);

	RwIm3DVertexSetPos(&LineVertex[12], pPos->x-fOffset, pPos->y-fOffset, pPos->z+fOffset);
	RwIm3DVertexSetRGBA(&LineVertex[12], pColor->red, pColor->green, pColor->blue, pColor->alpha);

	RwIm3DVertexSetPos(&LineVertex[13], pPos->x-fOffset, pPos->y-fOffset, pPos->z-fOffset);
	RwIm3DVertexSetRGBA(&LineVertex[13], pColor->red, pColor->green, pColor->blue, pColor->alpha);

	RwIm3DVertexSetPos(&LineVertex[14], pPos->x+fOffset, pPos->y-fOffset, pPos->z+fOffset);
	RwIm3DVertexSetRGBA(&LineVertex[14], pColor->red, pColor->green, pColor->blue, pColor->alpha);

	RwIm3DVertexSetPos(&LineVertex[15], pPos->x+fOffset, pPos->y-fOffset, pPos->z-fOffset);
	RwIm3DVertexSetRGBA(&LineVertex[15], pColor->red, pColor->green, pColor->blue, pColor->alpha);



	RwIm3DVertexSetPos(&LineVertex[16], pPos->x-fOffset, pPos->y+fOffset, pPos->z+fOffset);
	RwIm3DVertexSetRGBA(&LineVertex[16], pColor->red, pColor->green, pColor->blue, pColor->alpha);

	RwIm3DVertexSetPos(&LineVertex[17], pPos->x-fOffset, pPos->y-fOffset, pPos->z+fOffset);
	RwIm3DVertexSetRGBA(&LineVertex[17], pColor->red, pColor->green, pColor->blue, pColor->alpha);

	RwIm3DVertexSetPos(&LineVertex[18], pPos->x+fOffset, pPos->y+fOffset, pPos->z+fOffset);
	RwIm3DVertexSetRGBA(&LineVertex[18], pColor->red, pColor->green, pColor->blue, pColor->alpha);

	RwIm3DVertexSetPos(&LineVertex[19], pPos->x+fOffset, pPos->y-fOffset, pPos->z+fOffset);
	RwIm3DVertexSetRGBA(&LineVertex[19], pColor->red, pColor->green, pColor->blue, pColor->alpha);

	RwIm3DVertexSetPos(&LineVertex[20], pPos->x-fOffset, pPos->y+fOffset, pPos->z-fOffset);
	RwIm3DVertexSetRGBA(&LineVertex[20], pColor->red, pColor->green, pColor->blue, pColor->alpha);

	RwIm3DVertexSetPos(&LineVertex[21], pPos->x-fOffset, pPos->y-fOffset, pPos->z-fOffset);
	RwIm3DVertexSetRGBA(&LineVertex[21], pColor->red, pColor->green, pColor->blue, pColor->alpha);

	RwIm3DVertexSetPos(&LineVertex[22], pPos->x+fOffset, pPos->y+fOffset, pPos->z-fOffset);
	RwIm3DVertexSetRGBA(&LineVertex[22], pColor->red, pColor->green, pColor->blue, pColor->alpha);

	RwIm3DVertexSetPos(&LineVertex[23], pPos->x+fOffset, pPos->y-fOffset, pPos->z-fOffset);
	RwIm3DVertexSetRGBA(&LineVertex[23], pColor->red, pColor->green, pColor->blue, pColor->alpha);

	if(RwIm3DTransform(LineVertex, 24, pWorldMatrix, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA))
	{
		RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);        
		RwIm3DEnd();
	}
}

void RenderSplineLine(const RwV3d *pSrc, const RwV3d *pDest, RwRGBA *pColor, RwMatrix *pWorldMatrix)
{
	RwIm3DVertex LineVertex[2];

	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void *)NULL);

	RwIm3DVertexSetPos(&LineVertex[0], pSrc->x, pSrc->y, pSrc->z);
	RwIm3DVertexSetRGBA(&LineVertex[0], pColor->red, pColor->green, pColor->blue, pColor->alpha);

	RwIm3DVertexSetPos(&LineVertex[1], pDest->x, pDest->y, pDest->z);
	RwIm3DVertexSetRGBA(&LineVertex[1], pColor->red, pColor->green, pColor->blue, pColor->alpha);

	if(RwIm3DTransform(LineVertex, 2, pWorldMatrix, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA))
	{
		RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);        
		RwIm3DEnd();
	}
}

void CNtlDTRender::IncludeRenderMarkFlag(RwUInt8 byMarkFlag)
{
	m_byDTRenderMark |= byMarkFlag;
}

void CNtlDTRender::ExcludeRenderMarkFlag(RwUInt8 byMarkFlag)
{
	m_byDTRenderMark &= ~byMarkFlag; 
}

RwUInt8	CNtlDTRender::GetRenderMarkFlags(void)
{
	return m_byDTRenderMark;
}


void CNtlDTRender::SplineRender(CNtlDTSpline *pDTSpline, RwMatrix* pWorldMatrix, int nSx, int nEx, RwReal fBoxSize)
{
	static RwRGBA LineColor		= { 255,	0,		255,	255 };
	static RwRGBA NormalColor	= { 200,	200,	0,		255 };
	static RwRGBA SelectColor	= { 0,		255,	0,		255 };
	static RwRGBA CtrlLineColor = { 0,		200,	255,	255 };

	// =======================================================================================
	// Draw Curve Point Line
	// =======================================================================================
	
	RwInt32 iTotCurveCnt			= pDTSpline->GetTotCurveCount();
	const SSplineProp *pProp		= pDTSpline->GetSplineProp();	 
	const SSplineCtrlPt *pCtrlPt	= pDTSpline->GetSplineControlPoint();
	const SSplineCurvePt *pCurvePt	= pDTSpline->GetSplineCurvePoint();

	RwMatrix matWorld;
	RwV3d vDest, vAt;

	for(RwInt32 i = 0; i < iTotCurveCnt; i++ )
	{
		if(m_byDTRenderMark & DT_RENDER_MARK_CURVE_POINT)
		{
			if (i < iTotCurveCnt-1)
			{
				RenderSplineLine(&pCurvePt[i].vPos, &pCurvePt[i+1].vPos, &LineColor, pWorldMatrix);
			}
		}

		if(m_byDTRenderMark & DT_RENDER_MARK_CURVE_ROTATE)
		{
			RtQuatConvertToMatrix(&pCurvePt[i].qRot, &matWorld);
			RwV3dScale(&vAt, &matWorld.at, 0.3f);
			RwV3dAdd(&vDest, &pCurvePt[i].vPos, &vAt);

			RenderSplineLine(&pCurvePt[i].vPos, &vDest, &NormalColor, pWorldMatrix);
		}
	}

	// =======================================================================================
	// Draw Control Point Line
	// =======================================================================================

	RwRGBA color;
	for(RwInt32 i = 0; i < pProp->iCtrlNum - 1; i++ )
	{
		if(i >= nSx && i < nEx)
			color = SelectColor;
		else
			color = CtrlLineColor;
			
		if(m_byDTRenderMark & DT_RENDER_MARK_CONTROL_POINT)
			RenderSplineLine(&pCtrlPt[i].vPos, &pCtrlPt[i+1].vPos, &color, pWorldMatrix);
	}

	// =======================================================================================
	// Draw Select Control Point
	// =======================================================================================

	if(nSx >= 0 && nEx >= 0)
	{
		for(RwInt32 i = nSx; i < nEx; i++)
		{
			SelectPointBox(&pCtrlPt[i].vPos, &SelectColor, pWorldMatrix, fBoxSize);
		}
	}
}


void CNtlDTRender::DTPropRender(CNtlDTProp *pDTProp, RwMatrix* pWorldMatrix, RwReal fBoxSize, int nSx /*= -1*/, int nEx /*= -1*/)
{
	static RwMatrix mat;
	RwMatrixSetIdentity(&mat);

	const RwMatrix *pMat = pDTProp->GetTransMatrix();
	RwMatrixMultiply(&mat, pMat, pWorldMatrix);

	SplineRender(reinterpret_cast<CNtlDTSpline*>( pDTProp ), &mat, nSx, nEx, fBoxSize);
}


void CNtlDTRender::DTPropFocusPointRender(CNtlDTProp *pDTProp, RwMatrix* pWorldMatrix, int nIdx, RwReal fBoxSize)
{ 
	CNtlDTSpline *pDTSpline = reinterpret_cast<CNtlDTSpline*>( pDTProp );

	const SSplineProp *pProp = pDTSpline->GetSplineProp();	 
	if(nIdx < 0 || nIdx >= pProp->iCtrlNum)
		return;

	const SSplineCtrlPt *pCtrlPt	= pDTSpline->GetSplineControlPoint();

	static RwMatrix mat;
	RwMatrixSetIdentity(&mat);

	const RwMatrix *pMat = pDTProp->GetTransMatrix();
	RwMatrixMultiply(&mat, pMat, pWorldMatrix);

	static RwRGBA FocusColor = { 255,		255,	0,	255 };
	SelectPointBox(&pCtrlPt[nIdx].vPos, &FocusColor, &mat, fBoxSize);
}


void CNtlDTRender::DTPropValidCustomDataRender(CNtlDTProp *pDTProp, RwMatrix* pWorldMatrix, RwReal fBoxSize)
{
	CNtlDTSpline *pDTSpline = reinterpret_cast<CNtlDTSpline*>( pDTProp );
	RwInt32	iTotCount = pDTSpline->GetTotCurveCount();
	const SSplineCurvePt *pCurvePt = pDTSpline->GetSplineCurvePoint();
	
	static RwMatrix mat;
	RwMatrixSetIdentity(&mat);

	const RwMatrix *pMat = pDTProp->GetTransMatrix();
	RwMatrixMultiply(&mat, pMat, pWorldMatrix);

	static RwRGBA CustomColor = { 255, 50, 30, 255 };

	for(RwInt32 i = 0; i < iTotCount; i++)
	{
		SSplineCustomData *pCustomData = pDTSpline->GetSplineCustomData(i);
		if(pCustomData == NULL)
			continue;

		SelectPointBox(&pCurvePt[i].vPos, &CustomColor, &mat, fBoxSize);
	}
}
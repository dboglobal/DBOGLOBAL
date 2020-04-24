//***********************************************************************************
//	File		:	NtlEffectSplinePath.cpp
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************
#include "precomp_ntlpresentation.h"
#include ".\ntleffectsplinepath.h"

#include "NtlDebug.h"
#include "NtlMath.h"

CNtlEffectSplinePath::CNtlEffectSplinePath(const RwChar* strName)
{
	SetName(strName);

	memset(&m_Info, 0, sizeof(INFO));
	
	m_pControlPoint		= NULL;
	m_pCurveData		= NULL;
	m_nTotalCurveCount	= 0;
}

CNtlEffectSplinePath::~CNtlEffectSplinePath(void)
{
	Destroy();
}


//------------------------------------------------------------------
//	FuncName	: Destroy
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlEffectSplinePath::Destroy()
{
	NTL_ARRAY_DELETE(m_pControlPoint);
	NTL_ARRAY_DELETE(m_pCurveData);
}

//------------------------------------------------------------------
//	FuncName	: SetInfo
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlEffectSplinePath::SetInfo(INFO& Info)
{
	Destroy();

	memcpy(&m_Info, &Info, sizeof(INFO));

	m_pControlPoint = NTL_NEW SNTL_SPLINE_CONTROL_POINT[m_Info.nControlCount];
	if(!m_pControlPoint)
	{
		return FALSE;
	}
	memset(m_pControlPoint, 0, sizeof(SNTL_SPLINE_CONTROL_POINT) * m_Info.nControlCount);

	m_nTotalCurveCount = (m_Info.nControlCount - 4) * m_Info.nCurveSubDivision + 1;
	m_pCurveData = NTL_NEW SNTL_SPLINE_CURVE_POINT[m_nTotalCurveCount];
	if(!m_pCurveData)
	{
		return FALSE;
	}
	memset(m_pCurveData, 0, sizeof(SNTL_SPLINE_CURVE_POINT) * m_nTotalCurveCount);

	return TRUE;
}

void CNtlEffectSplinePath::FrameAnimation(RwReal fAniFrame, RwMatrix& matWorld, RwBool bLoop, RwBool bUseRotate, RwReal fScale /* = 1.0f */)
{
	if (bLoop)
	{
		fAniFrame = fmod(fAniFrame, m_pCurveData[m_nTotalCurveCount - 1].fTime);
	}

	RwMatrixSetIdentity(&matWorld);

	if (bUseRotate)
	{
		RtQuat quat;

		// 최대 키값을 넘으면 마지막 값을 넣어준다.
		if(fAniFrame > m_pCurveData[m_nTotalCurveCount - 1].fTime)
		{
			quat = m_pCurveData[m_nTotalCurveCount - 1].quat;
		}
		else
		{
			RwInt32 i1 = 0, i2 = 0;

			for (RwInt32 iKey = 0; iKey < m_nTotalCurveCount; ++iKey)
			{
				if (m_pCurveData[iKey].fTime > fAniFrame)
				{
					i1 = (iKey > 0) ? iKey - 1 : 0;
					i2 = iKey;
					break;
				}
			}

			if (m_pCurveData[i1].fTime == m_pCurveData[i2].fTime)
			{
				quat = m_pCurveData[i1].quat;
			}
			else
			{
				RwReal fLerpValue = (RwReal)(fAniFrame - m_pCurveData[i1].fTime)  / (RwReal)(m_pCurveData[i2].fTime - m_pCurveData[i1].fTime);
				D3DXQuaternionSlerp((D3DXQUATERNION*)&quat, 
									(D3DXQUATERNION*)&m_pCurveData[i1].quat, 
									(D3DXQUATERNION*)&m_pCurveData[i2].quat, 
									fLerpValue);
			}
			RtQuatConvertToMatrix(&quat, &matWorld);
		}
	}

	RwV3d v3Position;
	if(fAniFrame >= m_pCurveData[m_nTotalCurveCount - 1].fTime)
	{
		v3Position = m_pCurveData[m_nTotalCurveCount - 1].pos;
	}
	else
	{
		RwInt32 i1 = 0, i2 = 0;

		for (RwInt32 iKey = 0; iKey < m_nTotalCurveCount; ++iKey)
		{
			if (m_pCurveData[iKey].fTime > fAniFrame)
			{
				i1 = (iKey > 0) ? iKey - 1 : 0;
				i2 = iKey;
				break;
			}
		}

		if (m_pCurveData[i1].fTime == m_pCurveData[i2].fTime)
		{
			v3Position = m_pCurveData[i1].pos;
		}
		else
		{
			RwReal fLerpValue = (RwReal)(fAniFrame - m_pCurveData[i1].fTime)  / (RwReal)(m_pCurveData[i2].fTime - m_pCurveData[i1].fTime);

			v3Position.x = m_pCurveData[i1].pos.x + fLerpValue * (m_pCurveData[i2].pos.x - m_pCurveData[i1].pos.x);
			v3Position.y = m_pCurveData[i1].pos.y + fLerpValue * (m_pCurveData[i2].pos.y - m_pCurveData[i1].pos.y);
			v3Position.z = m_pCurveData[i1].pos.z + fLerpValue * (m_pCurveData[i2].pos.z - m_pCurveData[i1].pos.z);
		}
	}

    v3Position *= fScale;

	RwMatrixTranslate(&matWorld, &v3Position, rwCOMBINEPOSTCONCAT);
}


//------------------------------------------------------------------
//	FuncName	: Load
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlEffectSplinePath::Load(FILE* pFile)
{
	fread(&m_Info,					sizeof(CNtlEffectSplinePath::INFO),	1,						pFile);

	SetInfo(m_Info);

	fread(m_pControlPoint,			sizeof(SNTL_SPLINE_CONTROL_POINT),	m_Info.nControlCount,	pFile);

	fread(m_pCurveData,				sizeof(SNTL_SPLINE_CURVE_POINT),	m_nTotalCurveCount,		pFile);

	return TRUE;
}
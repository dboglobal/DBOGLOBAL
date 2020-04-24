//***********************************************************************************
//	File		:	CVenusPropertyContainer.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#include "StdAfx.h"
#include ".\venuspropertycontainer.h"

#include ".\venuscamera.h"

#include "VenusConfig.h"

#include "VenusDefine.h"
#include "VenusAPI.h"
#include "NtlPlApi.h"

#include "EditDialog.h"

#include <d3dx9.h>

//------------------------------------------------------------------
//	FuncName	: BuildSplinePath
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
CNtlEffectSplinePath* CVenusPropertyContainer::BuildSplinePath(CNtlEffectSplinePath* pEffectSpline, CNtlEffectSplinePath::INFO& Info, CNtlEffectSplinePath* pSrcSpline)
{
	pEffectSpline->SetInfo(Info);

	if (pSrcSpline != NULL)
	{
		memcpy(pEffectSpline->m_pControlPoint, pSrcSpline->m_pControlPoint, sizeof(SNTL_SPLINE_CONTROL_POINT) * pEffectSpline->m_Info.nControlCount);
		memcpy(pEffectSpline->m_pCurveData, pSrcSpline->m_pCurveData, sizeof(SNTL_SPLINE_CURVE_POINT) * pEffectSpline->m_nTotalCurveCount);

		return pEffectSpline;
	}

	// 시간
	// 형석 2005 버전 수정하면서 for문의 i 변수를 밖으로 빼냈다.
	RwInt32 i = 0;
	for(i = 0; i < pEffectSpline->m_Info.nControlCount; ++ i)
	{
		pEffectSpline->m_pControlPoint[i].fTime = Info.fTime * i;
		memset(&pEffectSpline->m_pControlPoint[i].rotate, 0, sizeof(RwV3d));
	}

	switch(Info.nSplineType)
	{
	case 0:// line
		{
			for(RwInt32 i = 0; i < pEffectSpline->m_Info.nControlCount; ++ i)
			{
				pEffectSpline->m_pControlPoint[i].pos.x = Info.vForwardFactor.x * (i - 1);
				pEffectSpline->m_pControlPoint[i].pos.y = Info.vForwardFactor.y * (i - 1);
				pEffectSpline->m_pControlPoint[i].pos.z = Info.vForwardFactor.z * (i - 1);
			}
		}
		break;
	case 1:// Circle
		{
			RwMatrix mRot;
			RwReal fAngle = Info.bRotateDirection ? 360.f : -360.f;

			RwReal fStartAngle = (360.f / (Info.nControlCount - 4));
			if (Info.bRotateDirection)
			{
				fStartAngle *= -(1.f);
			}
			for(RwInt32 i = 0; i < Info.nControlCount; ++ i)
			{
				switch(Info.nAxisType)
				{
				case CNtlEffectSplinePath::AXIS_X:
					{
						RwMatrixRotate(&mRot, &VEC_X, fAngle * ((RwReal)(i - 1) / (Info.nControlCount - 4)), rwCOMBINEREPLACE);

						RwV3d vPoint = { 0, 0, Info.fRadius };
						RwV3dTransformPoint(&vPoint, &vPoint, &mRot);
						pEffectSpline->m_pControlPoint[i].pos.x = vPoint.x;
						pEffectSpline->m_pControlPoint[i].pos.y = vPoint.y;
						pEffectSpline->m_pControlPoint[i].pos.z = vPoint.z;

						if (Info.bRotateDirection)
						{
							pEffectSpline->m_pControlPoint[i].rotate.x = 270 + fStartAngle + (360.f * ((RwReal)i / (RwReal)(Info.nControlCount - 4)));
						}
						else
						{
							pEffectSpline->m_pControlPoint[i].rotate.x = 90 + fStartAngle - (360.f * ((RwReal)i / (RwReal)(Info.nControlCount - 4)));
						}
					}
					break;
				case CNtlEffectSplinePath::AXIS_Y:
					{
						RwMatrixRotate(&mRot, &VEC_Y, fAngle * ((RwReal)(i - 1) / (Info.nControlCount - 4)), rwCOMBINEREPLACE);

						RwV3d vPoint = { 0, 0, Info.fRadius };
						RwV3dTransformPoint(&vPoint, &vPoint, &mRot);
						pEffectSpline->m_pControlPoint[i].pos.x = vPoint.x;
						pEffectSpline->m_pControlPoint[i].pos.y = vPoint.y;
						pEffectSpline->m_pControlPoint[i].pos.z = vPoint.z;

						if (Info.bRotateDirection)
						{
							pEffectSpline->m_pControlPoint[i].rotate.x = 270.f;
							pEffectSpline->m_pControlPoint[i].rotate.y = fStartAngle + (360.f * ((RwReal)i / (RwReal)(Info.nControlCount - 4)));
						}
						else
						{
							pEffectSpline->m_pControlPoint[i].rotate.x = 90.f;
							pEffectSpline->m_pControlPoint[i].rotate.y = fStartAngle - (360.f * ((RwReal)i / (RwReal)(Info.nControlCount - 4)));
						}
					}
					break;
				case CNtlEffectSplinePath::AXIS_Z:
					{
						RwMatrixRotate(&mRot, &VEC_Z, fAngle * ((RwReal)(i - 1) / (Info.nControlCount - 4)), rwCOMBINEREPLACE);

						RwV3d vPoint = { Info.fRadius, 0, 0 };
						RwV3dTransformPoint(&vPoint, &vPoint, &mRot);
						pEffectSpline->m_pControlPoint[i].pos.x = vPoint.x;
						pEffectSpline->m_pControlPoint[i].pos.y = vPoint.y;
						pEffectSpline->m_pControlPoint[i].pos.z = vPoint.z;

						if (Info.bRotateDirection)
						{
							pEffectSpline->m_pControlPoint[i].rotate.z = 90 + fStartAngle + (360.f * ((RwReal)i / (RwReal)(Info.nControlCount - 4)));
						}
						else
						{
							pEffectSpline->m_pControlPoint[i].rotate.z = 270 + fStartAngle - (360.f * ((RwReal)i / (RwReal)(Info.nControlCount - 4)));
						}
					}
					break;
				}
			}
		}
		break;
	case 2:// Herix
		{
			RwMatrix mRot;
			RwReal fAngle = Info.bRotateDirection ? 360.f * Info.nRotateCount : -360.f * Info.nRotateCount;

			RwReal fStartAngle = (360.f / ((Info.nControlCount - 4) / (RwReal)Info.nRotateCount));
			if (Info.bRotateDirection)
			{
				fStartAngle *= -(1.f);
			}

			for(RwInt32 i = 0; i < Info.nControlCount; ++ i)
			{
				switch(Info.nAxisType)
				{
				case CNtlEffectSplinePath::AXIS_X:
					{
						RwMatrixRotate(&mRot, &VEC_X, fAngle * ((RwReal)(i - 1) / (Info.nControlCount - 4)), rwCOMBINEREPLACE);

						RwV3d vPoint = { 0, 0, Info.fRadius };
						RwV3dTransformPoint(&vPoint, &vPoint, &mRot);
						pEffectSpline->m_pControlPoint[i].pos.x = vPoint.x + Info.fPointSpeed * i;
						pEffectSpline->m_pControlPoint[i].pos.y = vPoint.y;
						pEffectSpline->m_pControlPoint[i].pos.z = vPoint.z;

						if (Info.bRotateDirection)
						{
							pEffectSpline->m_pControlPoint[i].rotate.y = 270.f + fStartAngle + (360.f * Info.nRotateCount * ((RwReal)i / (RwReal)(Info.nControlCount - 4)));
						}
						else
						{
							pEffectSpline->m_pControlPoint[i].rotate.y = 90.f + fStartAngle - (360.f * Info.nRotateCount * ((RwReal)i / (RwReal)(Info.nControlCount - 4)));
						}
					}
					break;
				case CNtlEffectSplinePath::AXIS_Y:
					{
						RwMatrixRotate(&mRot, &VEC_Y, fAngle * ((RwReal)(i - 1) / (Info.nControlCount - 4)), rwCOMBINEREPLACE);

						RwV3d vPoint = { 0, 0, Info.fRadius };
						RwV3dTransformPoint(&vPoint, &vPoint, &mRot);
						pEffectSpline->m_pControlPoint[i].pos.x = vPoint.x;
						pEffectSpline->m_pControlPoint[i].pos.y = vPoint.y + Info.fPointSpeed * i;
						pEffectSpline->m_pControlPoint[i].pos.z = vPoint.z;

						if (Info.bRotateDirection)
						{
							pEffectSpline->m_pControlPoint[i].rotate.x = 270.f;
							pEffectSpline->m_pControlPoint[i].rotate.y = fStartAngle + (360.f * Info.nRotateCount * ((RwReal)i / (RwReal)(Info.nControlCount - 4)));
						}
						else
						{
							pEffectSpline->m_pControlPoint[i].rotate.x = 90.f;
							pEffectSpline->m_pControlPoint[i].rotate.y = fStartAngle - (360.f * Info.nRotateCount * ((RwReal)i / (RwReal)(Info.nControlCount - 4)));
						}
					}
					break;
				case CNtlEffectSplinePath::AXIS_Z:
					{
						RwMatrixRotate(&mRot, &VEC_Z, fAngle * ((RwReal)(i - 1) / (Info.nControlCount - 4)), rwCOMBINEREPLACE);

						RwV3d vPoint = { Info.fRadius, 0, 0 };
						RwV3dTransformPoint(&vPoint, &vPoint, &mRot);
						pEffectSpline->m_pControlPoint[i].pos.x = vPoint.x;
						pEffectSpline->m_pControlPoint[i].pos.y = vPoint.y;
						pEffectSpline->m_pControlPoint[i].pos.z = vPoint.z + Info.fPointSpeed * i;

						if (Info.bRotateDirection)
						{
							pEffectSpline->m_pControlPoint[i].rotate.y = 90.f + fStartAngle + (360.f * Info.nRotateCount * ((RwReal)i / (RwReal)(Info.nControlCount - 4)));
						}
						else
						{
							pEffectSpline->m_pControlPoint[i].rotate.y = 270.f + fStartAngle - (360.f * Info.nRotateCount * ((RwReal)i / (RwReal)(Info.nControlCount - 4)));
						}
					}
					break;
				}
			}
		}
		break;
	case 3:// Sine curve
		{
			pEffectSpline->m_pControlPoint[0].pos.x = 0;
			pEffectSpline->m_pControlPoint[0].pos.y = 0;
			pEffectSpline->m_pControlPoint[0].pos.z = 0;
			pEffectSpline->m_pControlPoint[i].rotate.z = 90;

			RwBool signflag = Info.bRotateDirection;

			for(RwInt32 i = 1; i < Info.nControlCount; ++ i)
			{
				switch(Info.nAxisType)
				{
				case CNtlEffectSplinePath::AXIS_X:
					{
						pEffectSpline->m_pControlPoint[i].pos.x = pEffectSpline->m_pControlPoint[0].pos.x + (Info.fPointSpeed * i);;
						pEffectSpline->m_pControlPoint[i].pos.y = pEffectSpline->m_pControlPoint[0].pos.y;
						pEffectSpline->m_pControlPoint[i].pos.z = pEffectSpline->m_pControlPoint[0].pos.z;

						pEffectSpline->m_pControlPoint[i].rotate.x = 90;

						if( signflag )
						{
							pEffectSpline->m_pControlPoint[i].pos.z  += Info.fRadius;
							signflag = FALSE;
						}
						else
						{
							pEffectSpline->m_pControlPoint[i].pos.z  -= Info.fRadius;
							signflag = TRUE;
						}
					}
					break;
				case CNtlEffectSplinePath::AXIS_Y:
					{
						pEffectSpline->m_pControlPoint[i].pos.x = pEffectSpline->m_pControlPoint[0].pos.x;
						pEffectSpline->m_pControlPoint[i].pos.y = pEffectSpline->m_pControlPoint[0].pos.y + (Info.fPointSpeed * i);
						pEffectSpline->m_pControlPoint[i].pos.z = pEffectSpline->m_pControlPoint[0].pos.z;

						pEffectSpline->m_pControlPoint[i].rotate.y = 90;

						if( signflag )
						{
							pEffectSpline->m_pControlPoint[i].pos.x  += Info.fRadius;
							signflag = FALSE;
						}
						else
						{
							pEffectSpline->m_pControlPoint[i].pos.x  -= Info.fRadius;
							signflag = TRUE;
						}
					}
					break;
				case CNtlEffectSplinePath::AXIS_Z:
					{
						pEffectSpline->m_pControlPoint[i].pos.x = pEffectSpline->m_pControlPoint[0].pos.x;
						pEffectSpline->m_pControlPoint[i].pos.y = pEffectSpline->m_pControlPoint[0].pos.y;
						pEffectSpline->m_pControlPoint[i].pos.z = pEffectSpline->m_pControlPoint[0].pos.z + (Info.fPointSpeed * i);

						pEffectSpline->m_pControlPoint[i].rotate.z = 90;

						if( signflag )
						{
							pEffectSpline->m_pControlPoint[i].pos.x  += Info.fRadius;
							signflag = FALSE;
						}
						else
						{
							pEffectSpline->m_pControlPoint[i].pos.x  -= Info.fRadius;
							signflag = TRUE;
						}
					}
					break;
				}
			}
		}
		break;
	}

	BuildSplineCurvePoint(pEffectSpline);

	return pEffectSpline;
}

//------------------------------------------------------------------
//	FuncName	: BuildSplineCurvePoint
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::BuildSplineCurvePoint(CNtlEffectSplinePath* pEffectSpline)
{
	NTL_ASSERTE(pEffectSpline);

	RwReal u, u_2, u_3;
	RwInt32	index = 0;

	for (RwInt32 i = 0; i < pEffectSpline->m_Info.nControlCount - 3; ++i) 
	{
		// For each subdivision
		for(RwInt32 j = 0; j < pEffectSpline->m_Info.nCurveSubDivision; ++j) 
		{
			if (index >= pEffectSpline->m_nTotalCurveCount)
			{
				continue;
			}

			u = (RwReal)j / pEffectSpline->m_Info.nCurveSubDivision;
			u_2 = u * u;
			u_3 = u_2 * u;

			// 원래 Curve 포인터는 Control 포인터의 +1 자리부터 생기므로 여기서 미리 한칸 위로 결정한다.
			RwMatrix matRotate1, matRotate2;
			RwMatrixRotate(&matRotate1, &VEC_X, pEffectSpline->m_pControlPoint[i+1].rotate.x, rwCOMBINEREPLACE);
			RwMatrixRotate(&matRotate1, &VEC_Y, pEffectSpline->m_pControlPoint[i+1].rotate.y, rwCOMBINEPOSTCONCAT);
			RwMatrixRotate(&matRotate1, &VEC_Z, pEffectSpline->m_pControlPoint[i+1].rotate.z, rwCOMBINEPOSTCONCAT);

			RwMatrixRotate(&matRotate2, &VEC_X, pEffectSpline->m_pControlPoint[i+2].rotate.x, rwCOMBINEREPLACE);
			RwMatrixRotate(&matRotate2, &VEC_Y, pEffectSpline->m_pControlPoint[i+2].rotate.y, rwCOMBINEPOSTCONCAT);
			RwMatrixRotate(&matRotate2, &VEC_Z, pEffectSpline->m_pControlPoint[i+2].rotate.z, rwCOMBINEPOSTCONCAT);

			RtQuat quat1, quat2;
			RtQuatConvertFromMatrix(&quat1, &matRotate1);
			RtQuatConvertFromMatrix(&quat2, &matRotate2);

			D3DXQuaternionSlerp((D3DXQUATERNION*)&pEffectSpline->m_pCurveData[index].quat, 
								(D3DXQUATERNION*)&quat1, 
								(D3DXQUATERNION*)&quat2, 
								u);

			pEffectSpline->m_pCurveData[index].fTime = pEffectSpline->m_pControlPoint[i].fTime + (pEffectSpline->m_pControlPoint[i+1].fTime - pEffectSpline->m_pControlPoint[i].fTime) * ((RwReal)j / (RwReal)pEffectSpline->m_Info.nCurveSubDivision);

			switch(pEffectSpline->m_Info.nCurveType)
			{
			case CNtlEffectSplinePath::SPLINE_TYPE_B:
				{
					// Position
					pEffectSpline->m_pCurveData[index].pos.x = _XSL_B_SPLINE(u, u_2, u_3,	pEffectSpline->m_pControlPoint[i  ].pos.x,pEffectSpline->m_pControlPoint[i+1].pos.x,
																							pEffectSpline->m_pControlPoint[i+2].pos.x,pEffectSpline->m_pControlPoint[i+3].pos.x);
					pEffectSpline->m_pCurveData[index].pos.y = _XSL_B_SPLINE(u, u_2, u_3,	pEffectSpline->m_pControlPoint[i  ].pos.y,pEffectSpline->m_pControlPoint[i+1].pos.y,
																							pEffectSpline->m_pControlPoint[i+2].pos.y,pEffectSpline->m_pControlPoint[i+3].pos.y);
					pEffectSpline->m_pCurveData[index].pos.z = _XSL_B_SPLINE(u, u_2, u_3,	pEffectSpline->m_pControlPoint[i  ].pos.z,pEffectSpline->m_pControlPoint[i+1].pos.z,
																							pEffectSpline->m_pControlPoint[i+2].pos.z,pEffectSpline->m_pControlPoint[i+3].pos.z);
				}
				break;
			case CNtlEffectSplinePath::SPLINE_TYPE_CATMULL:
				{
					pEffectSpline->m_pCurveData[index].pos.x = _XSL_CATMULL_ROM_SPLINE(u, u_2, u_3,	pEffectSpline->m_pControlPoint[i  ].pos.x,pEffectSpline->m_pControlPoint[i+1].pos.x,
																									pEffectSpline->m_pControlPoint[i+2].pos.x,pEffectSpline->m_pControlPoint[i+3].pos.x);
					pEffectSpline->m_pCurveData[index].pos.y = _XSL_CATMULL_ROM_SPLINE(u, u_2, u_3,	pEffectSpline->m_pControlPoint[i  ].pos.y,pEffectSpline->m_pControlPoint[i+1].pos.y,
																									pEffectSpline->m_pControlPoint[i+2].pos.y,pEffectSpline->m_pControlPoint[i+3].pos.y);
					pEffectSpline->m_pCurveData[index].pos.z = _XSL_CATMULL_ROM_SPLINE(u, u_2, u_3,	pEffectSpline->m_pControlPoint[i  ].pos.z,pEffectSpline->m_pControlPoint[i+1].pos.z,
																									pEffectSpline->m_pControlPoint[i+2].pos.z,pEffectSpline->m_pControlPoint[i+3].pos.z);
				}
				break;
			case CNtlEffectSplinePath::SPLINE_TYPE_HERMITE:
				{
					// Position
					pEffectSpline->m_pCurveData[index].pos.x = HERMITE_SPLINE(u, u_2, u_3,	pEffectSpline->m_pControlPoint[i  ].pos.x,pEffectSpline->m_pControlPoint[i+1].pos.x,
																							pEffectSpline->m_pControlPoint[i+2].pos.x,pEffectSpline->m_pControlPoint[i+3].pos.x);
					pEffectSpline->m_pCurveData[index].pos.y = HERMITE_SPLINE(u, u_2, u_3,	pEffectSpline->m_pControlPoint[i  ].pos.y,pEffectSpline->m_pControlPoint[i+1].pos.y,
																							pEffectSpline->m_pControlPoint[i+2].pos.y,pEffectSpline->m_pControlPoint[i+3].pos.y);
					pEffectSpline->m_pCurveData[index].pos.z = HERMITE_SPLINE(u, u_2, u_3,	pEffectSpline->m_pControlPoint[i  ].pos.z,pEffectSpline->m_pControlPoint[i+1].pos.z,
																							pEffectSpline->m_pControlPoint[i+2].pos.z,pEffectSpline->m_pControlPoint[i+3].pos.z);
				}
				break;
			case CNtlEffectSplinePath::SPLINE_TYPE_BEZIER:
				{
					pEffectSpline->m_pCurveData[index].pos.x = BEZIER_SPLINE(u, u_2, u_3,	pEffectSpline->m_pControlPoint[i  ].pos.x,pEffectSpline->m_pControlPoint[i+1].pos.x,
																							pEffectSpline->m_pControlPoint[i+2].pos.x,pEffectSpline->m_pControlPoint[i+3].pos.x);
					pEffectSpline->m_pCurveData[index].pos.y = BEZIER_SPLINE(u, u_2, u_3,	pEffectSpline->m_pControlPoint[i  ].pos.y,pEffectSpline->m_pControlPoint[i+1].pos.y,
																							pEffectSpline->m_pControlPoint[i+2].pos.y,pEffectSpline->m_pControlPoint[i+3].pos.y);
					pEffectSpline->m_pCurveData[index].pos.z = BEZIER_SPLINE(u, u_2, u_3,	pEffectSpline->m_pControlPoint[i  ].pos.z,pEffectSpline->m_pControlPoint[i+1].pos.z,
																							pEffectSpline->m_pControlPoint[i+2].pos.z,pEffectSpline->m_pControlPoint[i+3].pos.z);
				}
				break;
			case CNtlEffectSplinePath::SPLINE_TYPE_LINE:
				{
					RwV3d vDir;
					RwV3dSubMacro(&vDir, &pEffectSpline->m_pControlPoint[i+2].pos, &pEffectSpline->m_pControlPoint[i+1].pos);
					RwV3dScale(&vDir, &vDir, (RwReal)j / (RwReal)(pEffectSpline->m_Info.nCurveSubDivision));
					RwV3dAdd (&pEffectSpline->m_pCurveData[index].pos, &pEffectSpline->m_pControlPoint[i+1].pos, &vDir);
				}
				break;
			}
			++ index;
		}
	}
}

//------------------------------------------------------------------
//	FuncName	: BuildSplineCurveTime
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::BuildSplineCurveTime(CNtlEffectSplinePath* pEffectSpline)
{
	RwInt32	index = 0;
	for (RwInt32 i = 0; i < pEffectSpline->m_Info.nControlCount - 3; ++i) 
	{
		// For each subdivision
		for(RwInt32 j = 0; j < pEffectSpline->m_Info.nCurveSubDivision; ++j) 
		{
			if (index >= pEffectSpline->m_nTotalCurveCount)
			{
				continue;
			}
			pEffectSpline->m_pCurveData[index].fTime = pEffectSpline->m_pControlPoint[i].fTime + (pEffectSpline->m_pControlPoint[i+1].fTime - pEffectSpline->m_pControlPoint[i].fTime) * ((RwReal)j / (RwReal)pEffectSpline->m_Info.nCurveSubDivision);
			++ index;
		}
	}
}

//------------------------------------------------------------------
//	FuncName	: SetSplineVertexColor
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::SetSplineVertexColor(RwRGBA& color)
{
	for(RwInt32 i = 0; i < 8; ++i)
	{
		RwIm3DVertexSetRGBA(&m_SplinePointVertex[i], color.red,  color.green, color.blue, color.alpha);
	}
}

//---------------------------------------------------------------------------
//Name		: GetDrawScaleFactor
//Desc		: 
//Return	: 
//Parameter	: 
//---------------------------------------------------------------------------
RwReal CVenusPropertyContainer::GetDrawScaleFactor(const RwV3d* pViewPos, RwReal x, RwReal y, RwReal z, RwReal factor)
{	
	FLOAT distance = sqrtf((pViewPos->x-x)*(pViewPos->x-x) + (pViewPos->y-y)*(pViewPos->y-y) + (pViewPos->z-z)*(pViewPos->z-z));
	return distance / factor;
}

//---------------------------------------------------------------------------
//Name		: ModulateCameraScaleMatrix
//Desc		: 
//Return	: 
//Parameter	: 
//---------------------------------------------------------------------------
RwReal CVenusPropertyContainer::ModulateCameraScaleMatrix(RwMatrix& matScale, const RwV3d* pViewPos, const RwV3d* pObjectPos, RwReal factor)
{
	FLOAT distancefactor = GetDrawScaleFactor(pViewPos, pObjectPos->x, pObjectPos->y, pObjectPos->z, factor);

	RwV3d vScale = {distancefactor, distancefactor, distancefactor};
	RwMatrixScale(&matScale, &vScale, rwCOMBINEREPLACE);

	return distancefactor;
}

//------------------------------------------------------------------
//	FuncName	: DrawSpline
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::DrawSpline(RwV3d* pSrc, RwV3d* pDest, RwRGBA* pColor, RwMatrix* pWorldMatrix)
{
	static RwIm3DVertex LineVertex[2];

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


//------------------------------------------------------------------
//	FuncName	: DrawSpline
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::DrawSpline(CNtlEffectSplinePath* pEffectSpline, RwMatrix* pWorldMatrix)
{
	static RwRGBA LineColor = { 255, 0, 255, 255 };
	static RwRGBA NormalColor = { 255, 0, 0, 255 };

	// =======================================================================================
	// Draw Curve Point Line
	// =======================================================================================
	RwMatrix matWorld;
	RwV3d vDest, vHeight;
	for(RwInt32 i = 0; i < pEffectSpline->m_nTotalCurveCount; i++ )
	{
		if (i < pEffectSpline->m_nTotalCurveCount-1)
		{
			DrawSpline(&pEffectSpline->m_pCurveData[i].pos, &pEffectSpline->m_pCurveData[i+1].pos, &LineColor, pWorldMatrix);
		}

		RtQuatConvertToMatrix(&pEffectSpline->m_pCurveData[i].quat, &matWorld);

		RwV3dScale(&vHeight, &matWorld.up, 0.2f);
		RwV3dAdd(&vDest, &pEffectSpline->m_pCurveData[i].pos, &vHeight);

		DrawSpline(&pEffectSpline->m_pCurveData[i].pos, &vDest, &NormalColor, pWorldMatrix);
	}

	// =======================================================================================
	// Draw Control Point Line
	// =======================================================================================
	for(RwInt32 i = 0; i < pEffectSpline->m_Info.nControlCount - 1; i++ )
	{
		static RwRGBA Color = { 0, 255, 255, 255 };
		DrawSpline(&pEffectSpline->m_pControlPoint[i].pos, &pEffectSpline->m_pControlPoint[i+1].pos, &Color, pWorldMatrix);
	}

	// =======================================================================================
	// Draw Control Point Line
	// =======================================================================================
	RwV3d vViewPosition = *CVenusCamera::GetInstance().GetViewPosition();
	RwMatrix matScale, matRender, matTemp;
	RwRenderStateSet(rwRENDERSTATECULLMODE,					(void *)rwCULLMODECULLNONE);

	ModulateCameraScaleMatrix(matScale, &vViewPosition, RwMatrixGetPos(pWorldMatrix), 300);

	for(RwInt32 i = 0; i < pEffectSpline->m_Info.nControlCount; i++ )
	{
		RwRGBA color = { 200, 200, 200, 255 };
		if (CVenusConfig::GetInstance().m_nCurrentLayer == EDIT_LAYER_PATH)
		{
			svdef_Int32::iterator it = std::find(m_svSelectSplineControlPoint.begin(), m_svSelectSplineControlPoint.end(), i);
			if (it != m_svSelectSplineControlPoint.end())
			{
				color.alpha = 255;
				color.red = 255;
				color.green = 255;
				color.blue = 0;
			}
		}
		SetSplineVertexColor(color);

		RwMatrixTranslate(&matRender, &pEffectSpline->m_pControlPoint[i].pos, rwCOMBINEREPLACE);

        matTemp = matRender;
		RwMatrixMultiply(&matRender, &matScale, &matTemp);
        matTemp = matRender;
		RwMatrixMultiply(&matRender, &matTemp, pWorldMatrix);

		m_fControlPointRadius = RwV3dLength(RwMatrixGetUp(&matRender));
		if(RwIm3DTransform(m_SplinePointVertex, 8, &matRender, rwIM3D_ALLOPAQUE))
		{
			RwIm3DRenderIndexedPrimitive(rwPRIMTYPETRILIST, m_SplinePointIndices, 36);
			RwIm3DEnd();
		}
	}
	RwRenderStateSet(rwRENDERSTATECULLMODE,					(void *)rwCULLMODECULLBACK);
}

//------------------------------------------------------------------
//	FuncName	: IntersectionSplineControlPoint
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::IntersectionSplineControlPoint(POINT point, RwBool bShiftButton, RwBool bCtrlButton)
{
	if (m_pEffectSplinePath == NULL) return;

	// 계속해서 더해지는 단계가 아니면 리스트를 새로 지우고 한다.
	if (!bShiftButton && !bCtrlButton)
	{
		m_svSelectSplineControlPoint.clear();
	}

	RwV3d vRayPos, vRayDir;
	CVenusCamera::GetInstance().GetMouseRay(point, vRayPos, vRayDir);

	RwInt32 nSelectIndex = -1;
	RwReal fDist = RwRealMAXVAL, fPickDist;

	for(RwInt32 i = 0; i < m_pEffectSplinePath->m_Info.nControlCount; ++ i)
	{
		RwV3d vPosition = m_pEffectSplinePath->m_pControlPoint[i].pos;

		if (API_IntersectionSphereFromRay(&vRayPos, &vRayDir, &vPosition, m_fControlPointRadius, &fPickDist))
		{
			if (fDist > fPickDist)
			{
				nSelectIndex = i;
				fDist = fPickDist;
			}
		}
	}

	if (nSelectIndex != -1)
	{
		IntersectionSplineControlPoint(nSelectIndex, bShiftButton, bCtrlButton);
	}
	CEditDialog::GetInstance().Reset();
}

//------------------------------------------------------------------
//	FuncName	: IntersectionEffect
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::IntersectionSplineControlPoint(POINT OldPoint, POINT CurPoint, RwBool bShiftButton, RwBool bCtrlButton)
{
	if (m_pEffectSplinePath == NULL) return;

	// 계속해서 더해지는 단계가 아니면 리스트를 새로 지우고 한다.
	if (!bShiftButton && !bCtrlButton)
	{
		m_svSelectSplineControlPoint.clear();
	}

	POINT MinPoint, MaxPoint;
	if (OldPoint.x < CurPoint.x) MinPoint.x = OldPoint.x;	else MinPoint.x = CurPoint.x;
	if (OldPoint.y < CurPoint.y) MinPoint.y = OldPoint.y;	else MinPoint.y = CurPoint.y;
	
	if (OldPoint.x > CurPoint.x) MaxPoint.x = OldPoint.x;	else MaxPoint.x = CurPoint.x;
	if (OldPoint.y > CurPoint.y) MaxPoint.y = OldPoint.y;	else MaxPoint.y = CurPoint.y;

    RwInt32 nWidth = RwRasterGetWidth(RwCameraGetRaster(CVenusCamera::GetInstance().GetCamera()));
    RwInt32 nHeight = RwRasterGetHeight(RwCameraGetRaster(CVenusCamera::GetInstance().GetCamera()));

	for(RwInt32 i = 0; i < m_pEffectSplinePath->m_Info.nControlCount; ++ i)
	{
		RwV3d vPosition = m_pEffectSplinePath->m_pControlPoint[i].pos;
        RwV2d v2DPos = API_PL_Calc3DPosTo2D(&vPosition, nWidth, nHeight);
		
		if (v2DPos.x >= MinPoint.x && v2DPos.y >= MinPoint.y && v2DPos.x < MaxPoint.x && v2DPos.y < MaxPoint.y)
		{
			IntersectionSplineControlPoint(i, bShiftButton, bCtrlButton);
		}
	}

	CEditDialog::GetInstance().Reset();
}

//------------------------------------------------------------------
//	FuncName	: IntersectionSplineControlPoint
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::IntersectionSplineControlPoint(RwInt32 index, RwBool bShiftButton, RwBool bCtrlButton)
{
	svdef_Int32::iterator it = std::find(m_svSelectSplineControlPoint.begin(), m_svSelectSplineControlPoint.end(), index);
	if (bCtrlButton)
	{
		if (it != m_svSelectSplineControlPoint.end())
		{
			m_svSelectSplineControlPoint.erase(it);
		}
	}
	else if (bShiftButton)
	{
		if (it == m_svSelectSplineControlPoint.end())
		{
			m_svSelectSplineControlPoint.push_back(index);
		}
	}
	else
	{
		m_svSelectSplineControlPoint.push_back(index);
	}
}

//------------------------------------------------------------------
//	FuncName	: EditSelectControlPoint
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::EditSelectControlPoint(RwMatrix* pEditMatrix)
{
	if (m_pEffectSplinePath == NULL) return;

	svdef_Int32::iterator it;
	for (it = m_svSelectSplineControlPoint.begin(); it != m_svSelectSplineControlPoint.end(); ++ it)
	{
		RwInt32 index = (*it);

		RwV3dTransformPoint(&m_pEffectSplinePath->m_pControlPoint[index].pos, &m_pEffectSplinePath->m_pControlPoint[index].pos, pEditMatrix);
	}

	BuildSplineCurvePoint(m_pEffectSplinePath);

	CEditDialog::GetInstance().Reset();

	m_bSave = TRUE;
}


//------------------------------------------------------------------
//	FuncName	: AddSelectControlPoint
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::AddSelectControlPoint(RwV3d* pEditVector)
{
	if (m_pEffectSplinePath == NULL) return;

	svdef_Int32::iterator it;
	for (it = m_svSelectSplineControlPoint.begin(); it != m_svSelectSplineControlPoint.end(); ++ it)
	{
		RwInt32 index = (*it);

		switch(CVenusConfig::GetInstance().m_nEditMode)
		{
		case CVenusConfig::EDIT_MODE_MOVE:
			RwV3dAdd(&m_pEffectSplinePath->m_pControlPoint[index].pos, &m_pEffectSplinePath->m_pControlPoint[index].pos, pEditVector);
			break;
		case CVenusConfig::EDIT_MODE_ROTATE:
			RwV3dAdd(&m_pEffectSplinePath->m_pControlPoint[index].rotate, &m_pEffectSplinePath->m_pControlPoint[index].rotate, pEditVector);
			break;
		case CVenusConfig::EDIT_MODE_SCALE:
			break;
		}
	}

	BuildSplineCurvePoint(m_pEffectSplinePath);

	CEditDialog::GetInstance().Reset();

	m_bSave = TRUE;
}

//------------------------------------------------------------------
//	FuncName	: SetSelectControlPoint
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::SetSelectControlPoint(RwV3d* pEditVector)
{
	if (m_pEffectSplinePath == NULL) return;

	svdef_Int32::iterator it;
	for (it = m_svSelectSplineControlPoint.begin(); it != m_svSelectSplineControlPoint.end(); ++ it)
	{
		RwInt32 index = (*it);

		switch(CVenusConfig::GetInstance().m_nEditMode)
		{
		case CVenusConfig::EDIT_MODE_MOVE:
			m_pEffectSplinePath->m_pControlPoint[index].pos = *pEditVector;
			break;
		case CVenusConfig::EDIT_MODE_ROTATE:
			m_pEffectSplinePath->m_pControlPoint[index].rotate = *pEditVector;
			break;
		case CVenusConfig::EDIT_MODE_SCALE:
			break;
		}
	}

	BuildSplineCurvePoint(m_pEffectSplinePath);

	CEditDialog::GetInstance().Reset();

	m_bSave = TRUE;
}

//------------------------------------------------------------------
//	FuncName	: SetSelectControlPoint_X
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::SetSelectControlPoint_X(float x)
{
	if (m_pEffectSplinePath == NULL) return;

	svdef_Int32::iterator it;
	for (it = m_svSelectSplineControlPoint.begin(); it != m_svSelectSplineControlPoint.end(); ++ it)
	{
		RwInt32 index = (*it);

		switch(CVenusConfig::GetInstance().m_nEditMode)
		{
		case CVenusConfig::EDIT_MODE_MOVE:
			m_pEffectSplinePath->m_pControlPoint[index].pos.x = x;
			break;
		case CVenusConfig::EDIT_MODE_ROTATE:
			m_pEffectSplinePath->m_pControlPoint[index].rotate.x = x;
			break;
		case CVenusConfig::EDIT_MODE_SCALE:
			break;
		}
	}

	BuildSplineCurvePoint(m_pEffectSplinePath);

	CEditDialog::GetInstance().Reset();

	m_bSave = TRUE;
}

//------------------------------------------------------------------
//	FuncName	: SetSelectControlPoint_X
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::SetSelectControlPoint_Y(float y)
{
	if (m_pEffectSplinePath == NULL) return;

	svdef_Int32::iterator it;
	for (it = m_svSelectSplineControlPoint.begin(); it != m_svSelectSplineControlPoint.end(); ++ it)
	{
		RwInt32 index = (*it);

		switch(CVenusConfig::GetInstance().m_nEditMode)
		{
		case CVenusConfig::EDIT_MODE_MOVE:
			m_pEffectSplinePath->m_pControlPoint[index].pos.y = y;
			break;
		case CVenusConfig::EDIT_MODE_ROTATE:
			m_pEffectSplinePath->m_pControlPoint[index].rotate.y = y;
			break;
		case CVenusConfig::EDIT_MODE_SCALE:
			break;
		}
	}

	BuildSplineCurvePoint(m_pEffectSplinePath);

	CEditDialog::GetInstance().Reset();

	m_bSave = TRUE;
}

//------------------------------------------------------------------
//	FuncName	: SetSelectControlPoint_X
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::SetSelectControlPoint_Z(float z)
{
	if (m_pEffectSplinePath == NULL) return;

	svdef_Int32::iterator it;
	for (it = m_svSelectSplineControlPoint.begin(); it != m_svSelectSplineControlPoint.end(); ++ it)
	{
		RwInt32 index = (*it);

		switch(CVenusConfig::GetInstance().m_nEditMode)
		{
		case CVenusConfig::EDIT_MODE_MOVE:
			m_pEffectSplinePath->m_pControlPoint[index].pos.z = z;
			break;
		case CVenusConfig::EDIT_MODE_ROTATE:
			m_pEffectSplinePath->m_pControlPoint[index].rotate.z = z;
			break;
		case CVenusConfig::EDIT_MODE_SCALE:
			break;
		}
	}

	BuildSplineCurvePoint(m_pEffectSplinePath);

	CEditDialog::GetInstance().Reset();

	m_bSave = TRUE;
}

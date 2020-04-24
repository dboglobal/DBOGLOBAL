#include "stdafx.h"

#include "NtlWE.h"
#include "NtlWEFrm.h"
#include "NtlWEDoc.h"

#include "NtlText.h"

#include "NtlWorldBrush.h"
#include "NtlWorldBrush2DCircle.h"
#include "NtlWorldBrushCircle.h"
#include "NtlWorldBrushRectangle.h"
#include "NtlWorldBrushTile.h"
#include "NtlWorldBrushTileEx.h"
#include "NtlWorldBrushFrustum.h"
#include "NtlWorldBrushRoad.h"
#include "NtlWorldBrushController.h"

#include "NtlWorldPaletteDefine.h"

#include "NtlWorldFieldManager.h"

#include "NtlPLWorldEntity.h"

#include "DojoPropPopup.h"
#include "PaletteDojoDlg.h"

#include "UpdateEventDojoPalette.h"

RpAtomic* WorldIntersection4DojoCB_CTLine2Sphere(RpIntersection* pIntersection, RpWorldSector* pWorldSector, RpAtomic* pAtomic, RwReal fDistance, void* pData)
{
	if((static_cast<CNtlPLEntity*>(RpNtlAtomicGetData(pAtomic))->GetClassType() != PLENTITY_OBJECT))
	{
		return pAtomic;
	}

	if (!static_cast<CNtlPLObject*>(RpNtlAtomicGetData(pAtomic))->GetDojo())
	{
		return pAtomic;
	}

	sNPE_COLLISION_PARAM*	pNPECollisionParam	= static_cast<sNPE_COLLISION_PARAM*>(pData);
	CNtlPLEntity*			pNtlPLEntityCur		= static_cast<CNtlPLEntity*>(static_cast<CNtlPLObject*>(RpNtlAtomicGetData(pAtomic))->GetDojo());
	RwReal					DistSphere2LineSPos;

	RwSphere*	pSphere = RpAtomicGetBoundingSphere(pAtomic);
	RwV3d		SphereC;
	RwV3d		Src2Dst;

	// dummy scaling
	RwV3dTransformPoints(&SphereC, &pSphere->center, 1, RwFrameGetLTM(RpAtomicGetFrame(pAtomic)));

	for(unsigned int i = 0; i < pNPECollisionParam->_vecNPECollisionParam.size(); ++i)
	{
		if(pAtomic == pNPECollisionParam->_vecNPECollisionParam[i]._pAtomic)
		{
			RwV3dSubMacro(&Src2Dst, &SphereC, &pIntersection->t.sphere.center);
			DistSphere2LineSPos = ::sqrtf(CNtlMath::MathRwV3dSquaredLength(&Src2Dst));

			if(static_cast<RwReal>(::fabs(pNPECollisionParam->_vecNPECollisionParam[i]._Dist)) > DistSphere2LineSPos)
			{
				pNPECollisionParam->_vecNPECollisionParam[i]._Dist = fDistance * DistSphere2LineSPos;
			}

			return pAtomic;
		}
	}

	sCOLLISION_PARAM sCollisionParam;
	RwV3dSubMacro(&Src2Dst, &SphereC, &pIntersection->t.sphere.center);
	DistSphere2LineSPos = ::sqrtf(CNtlMath::MathRwV3dSquaredLength(&Src2Dst));
	sCollisionParam._Dist			= fDistance * DistSphere2LineSPos;
	sCollisionParam._pAtomic		= pAtomic;
	sCollisionParam._pNtlPLEntity	= static_cast<CNtlPLEntity*>(pNtlPLEntityCur);
	pNPECollisionParam->_vecNPECollisionParam.push_back(sCollisionParam);

	return pAtomic;
}

CUpdateEventDojoPalette::CUpdateEventDojoPalette()
{
	m_pBrushSelection = NULL;
}

CUpdateEventDojoPalette::~CUpdateEventDojoPalette()
{
	NTL_DELETE(m_pBrushSelection);
}

void CUpdateEventDojoPalette::UpdateDojoPalette()
{
	if (dGET_CUR_PALETTE() != ePM_DOJO)
	{
		return;
	}

	CNtlWorldFieldManager *pMgr = dGETMGR();

	// placing mode
	if(dGET_BRUSH_ENTITY())
	{
		dGETFRM()->m_pDojoPalette->UpdatePreview();

		m_vecSelected.clear();
		if(dOKEY(DIK_SPACE))
		{
			if(dGET_BRUSH_ENTITY())
			{
				GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
				dGET_BRUSH_ENTITY() = NULL;
				dGETFRM()->m_pDojoPalette->UpdatePropertyData(NULL);
				return;
			}
		}

		// object moving with picking variation
		sNPE_COLLISION_PARAM sNPECollisionParam;
		sNPECollisionParam._pNtlPLEntity = dGET_BRUSH_ENTITY();

		RwV3d vDist;
		vDist.x = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.x;
		vDist.y = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.y;
		vDist.z = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.z;
		dGET_BRUSH_ENTITY()->SetPosition(&vDist);

		// place effects
		if(dGET_MOUSE()->GetLBOnce())
		{
			if(dGET_BRUSH_ENTITY())
			{
				RwInt32 CurSectorIdx = pMgr->GetSectorIdx(vDist);
				if(CurSectorIdx != -1)
				{
					dGETFRM()->m_pDojoPalette->UpdatePropertyData(dGET_BRUSH_ENTITY());		
					if (dGETFRM()->m_pDojoPalette->GetCreateParam(dGET_BRUSH_ENTITY()))
					{
						CNtlPLEntity *pNtlPLEntity	= GetSceneManager()->CreateEntity(PLENTITY_DOJO, NULL, dGETFRM()->m_pDojoPalette->GetCreateParam(dGET_BRUSH_ENTITY()));
						DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

						pMgr->m_pSectors[CurSectorIdx].InsertIn2World(pNtlPLEntity, 11);						 
						return;
					}					
				}
			}
		}
	}
	// capture mode; picking
	else if(!dGET_BRUSH_ENTITY())
	{
		if(dGET_MOUSE()->GetLBOnce() && !dKEY(DIK_LSHIFT) && !dKEY(DIK_RSHIFT) && !dKEY(DIK_SPACE))
		{
			RwV2d ScreenPos;
			ScreenPos.x = (RwReal)dGET_MOUSE()->GetPosClicked().x;
			ScreenPos.y = (RwReal)dGET_MOUSE()->GetPosClicked().y;

			RwLine cameraRay;
			RwCameraCalcPixelRay(CNtlPLGlobal::m_RwCamera, &cameraRay, &ScreenPos);

			RwV3d RwRayDir;
			RwV3dSubMacro(&RwRayDir, &cameraRay.end, &cameraRay.start);
			RwV3dNormalize(&RwRayDir, &RwRayDir);

			// pick map objects
			RpIntersection Intersection;
			Intersection.t.line	= cameraRay;
			Intersection.type	= rpINTERSECTLINE;

			sNtlMapObjCollisionParam CollisionParam;
			::memset(&CollisionParam, 0, sizeof(sNtlMapObjCollisionParam));

			RpWorldForAllAtomicIntersections(CNtlPLGlobal::m_pRpWorld, &Intersection, WorldIntersectionAtomicCB, &CollisionParam);
			
			m_vecSelected.clear();
			dGETFRM()->m_pDojoPalette->UpdatePropertyData(NULL);

			if(CollisionParam.IsCollided)
			{
				// skip specfic interface stuff
				if(!strcmp(static_cast<CNtlPLEntity*>(RpNtlAtomicGetData(CollisionParam.pCurAtomic))->GetName(), "FLY_DUMMY"))
				{
					return;
				}
				if (!static_cast<CNtlPLObject*>(RpNtlAtomicGetData(CollisionParam.pCurAtomic))->GetDojo())
				{
					return;
				}
				else
				{
					CNtlPLObject* pCurEntity = static_cast<CNtlPLObject*>(RpNtlAtomicGetData(CollisionParam.pCurAtomic));
					
					RwBool bWasPlaced = FALSE;
					for(int i = 0; i < (int)m_vecSelected.size(); ++i)
					{	
						if(m_vecSelected[i] == pCurEntity->GetDojo())
						{
							bWasPlaced = TRUE;
							break;
						}
					}

					if (!bWasPlaced)
					{
						m_vecSelected.push_back(pCurEntity->GetDojo());
						dGETFRM()->m_pDojoPalette->UpdatePropertyData(pCurEntity->GetDojo());
						
						if(!m_pBrushSelection)
						{
							m_pBrushSelection						= NTL_NEW CNtlWorldBrush;
							CNtlWorldBrushController *pController	= new CNtlWB_Circle<CNtlWorldFieldManager>(eNBT_CIRCLE, pMgr, &CNtlWorldFieldManager::GetWorldSectorHeight);
							m_pBrushSelection->SetController(pController);
						}
					}
				}
			}
			return;
		}

		if(m_vecSelected.size() == 1)
		{
			if(dGET_MOUSE()->GetLBDC())
			{
				CDojoPropPopup Dlg;
				Dlg.DoModal();
			}
		}

		// editing mode
		if(m_vecSelected.size())
		{
			CNtlPLDojo*	pPLDojo = (CNtlPLDojo*)m_vecSelected[0];

			if(dOKEY(DIK_DELETE))
			{
				RwBool BreakFlag = FALSE;

				for(int j = 0; j < (int)g_vecVisibleSectors.size(); ++j)
				{
					RwInt32 NumObj = static_cast<RwInt32>(pMgr->m_pSectors[g_vecVisibleSectors[j]].m_vecNtlPLEntityDojo.size());
					for(int k = 0; k < NumObj; ++k)
					{
						if(pMgr->m_pSectors[g_vecVisibleSectors[j]].m_vecNtlPLEntityDojo[k] == pPLDojo)
						{
							pMgr->m_pSectors[g_vecVisibleSectors[j]].DeleteEntity(pPLDojo, 11);
							GetSceneManager()->DeleteEntity(pPLDojo);
							m_vecSelected.clear();
							dGETFRM()->m_pDojoPalette->UpdatePropertyData(NULL);
							return;
						}
					}	
				}
			}

			if(dKEY(DIK_LCONTROL) && dGET_MOUSE()->GetLBOnce() && m_vecSelected.size() == 1)
			{
				if(dGET_BRUSH_ENTITY())
				{
					GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
					dGET_BRUSH_ENTITY() = NULL;
				}

				if (dGETFRM()->m_pDojoPalette->GetCreateParam(m_vecSelected.at(0)))
				{
					dGET_BRUSH_ENTITY() = GetSceneManager()->CreateEntity(PLENTITY_DOJO, NULL, dGETFRM()->m_pDojoPalette->GetCreateParam(m_vecSelected.at(0)));
					DBO_ASSERT(dGET_BRUSH_ENTITY(), "Entity create failed.");

					dGETFRM()->m_pDojoPalette->UpdatePropertyData(dGET_BRUSH_ENTITY());
					return;
				}					
			}

			if(dGET_MOUSE()->GetBehavior(MB_LBDN) && dGET_MOUSE()->GetBehavior(MB_MOVE))
			{
				if(dKEY(DIK_LSHIFT))
				{
					float DistX = dGET_MOUSE()->GetXMovementWithRate(10.0f);
					float DistY = dGET_MOUSE()->GetYMovementWithRate(10.0f);

					if(dKEY(DIK_Z))
					{
						RwV3d vPos = pPLDojo->GetPosition();
						vPos.y -= DistY;
						pPLDojo->SetPosition(&vPos);
						dGETFRM()->m_pDojoPalette->UpdatePropertyData(pPLDojo);
					}
					else
					{					
						RwFrame *pCameraFrame	= RwCameraGetFrame(CNtlPLGlobal::m_RwCamera);
						RwMatrix *pM			= RwFrameGetMatrix(pCameraFrame);
						RwV3d *pRight			= RwMatrixGetRight(pM);
						RwV3d *pUp				= RwMatrixGetUp(pM);
						RwV3d *pAt				= RwMatrixGetAt(pM);
						RwV3d vFront, vRight;
						RwReal TerrainHeight, TotalHeight, ObjHeight;

						CNtlMath::MathRwV3dAssign(&vFront, pAt->x, 0.0f, pAt->z);
						CNtlMath::MathRwV3dAssign(&vRight, pRight->x, 0.0f, pRight->z);
						RwV3dNormalize(&vFront, &vFront);
						RwV3dNormalize(&vRight, &vRight);

						RwV3d vPos = pPLDojo->GetPosition();
						// delete from the previous field
						RwInt32 CurSectorIdx = pMgr->GetSectorIdx(vPos);
						if(CurSectorIdx != -1)
						{
							pMgr->m_pSectors[CurSectorIdx].DeleteEntity(static_cast<CNtlPLEntity*>(pPLDojo), 11);
						}

						TotalHeight		= dGET_POSITIVE_VALUE(vPos.y);
						TerrainHeight	= dGET_POSITIVE_VALUE(pMgr->GetWorldSectorHeight(vPos));
						ObjHeight		= TotalHeight - TerrainHeight;

						// transform
						RwV3dIncrementScaled(&vPos, &vFront, -DistY);
						RwV3dIncrementScaled(&vPos, &vRight, -DistX);

						vPos.y = pMgr->GetWorldSectorHeight(vPos) + ObjHeight;
						pPLDojo->SetPosition(&vPos);
						dGETFRM()->m_pDojoPalette->UpdatePropertyData(pPLDojo);

						// create into the new field
						CurSectorIdx = pMgr->GetSectorIdx(vPos);
						if(CurSectorIdx != -1)
						{
							pMgr->m_pSectors[CurSectorIdx].InsertIn2World(static_cast<CNtlPLEntity*>(pPLDojo), 11);
						}
					}
				}
				// Rot
				else if(dKEY(DIK_SPACE))
				{
					float DistX = dGET_MOUSE()->GetXMovementWithRate(1.0f);
					float DistY = dGET_MOUSE()->GetYMovementWithRate(1.0f);

					if(dKEY(DIK_V))
					{
						RwV3d vRot = *pPLDojo->GetRotate();
						vRot.x -= DistX;
						pPLDojo->SetRotate(vRot.x, vRot.y, vRot.z);
						dGETFRM()->m_pDojoPalette->UpdatePropertyData(pPLDojo);
					}
					else if(dKEY(DIK_N))
					{
						RwV3d vRot = *pPLDojo->GetRotate();
						vRot.z -= DistX;
						pPLDojo->SetRotate(vRot.x, vRot.y, vRot.z);
						dGETFRM()->m_pDojoPalette->UpdatePropertyData(pPLDojo);
					}
					else
					{
						RwV3d vRot = *pPLDojo->GetRotate();
						vRot.y -= DistX;
						pPLDojo->SetRotate(vRot.x, vRot.y, vRot.z);
						dGETFRM()->m_pDojoPalette->UpdatePropertyData(pPLDojo);
					}
				}
				// Scale
				else if(dKEY(DIK_RSHIFT))
				{
					float DistX = dGET_MOUSE()->GetXMovementWithRate(10.0f);
					float DistY = dGET_MOUSE()->GetYMovementWithRate(10.0f);

					RwReal fScale = pPLDojo->GetScale();
					fScale -= DistY;
					CLAMP(fScale, 1.0f, dGET_WORLD_PARAM()->WorldSectorSize);
					pPLDojo->SetScale(fScale);
					dGETFRM()->m_pDojoPalette->UpdatePropertyData(pPLDojo);
				}
			}

			// Rotation using Keyboard
			if(dKEY(DIK_SPACE))
			{
				if(dKEY(DIK_LSHIFT))
				{
					if(dOKEY(DIK_LEFTARROW))
					{
						RwV3d vRot = *pPLDojo->GetRotate();
						vRot.y -= dGETHDL()->m_RotationTabValue;
						pPLDojo->SetRotate(vRot.x, vRot.y, vRot.z);
						dGETFRM()->m_pDojoPalette->UpdatePropertyData(pPLDojo);
					}
					else if(dOKEY(DIK_RIGHTARROW))
					{
						RwV3d vRot = *pPLDojo->GetRotate();
						vRot.y += dGETHDL()->m_RotationTabValue;
						pPLDojo->SetRotate(vRot.x, vRot.y, vRot.z);
						dGETFRM()->m_pDojoPalette->UpdatePropertyData(pPLDojo);
					}
				}
				else
				{
					if(dKEY(DIK_LEFTARROW))
					{
						RwV3d vRot = *pPLDojo->GetRotate();
						vRot.y -= dGETHDL()->m_RotationTabValue;
						pPLDojo->SetRotate(vRot.x, vRot.y, vRot.z);
						dGETFRM()->m_pDojoPalette->UpdatePropertyData(pPLDojo);
					}
					else if(dKEY(DIK_RIGHTARROW))
					{
						RwV3d vRot = *pPLDojo->GetRotate();
						vRot.y += dGETHDL()->m_RotationTabValue;
						pPLDojo->SetRotate(vRot.x, vRot.y, vRot.z);
						dGETFRM()->m_pDojoPalette->UpdatePropertyData(pPLDojo);
					}
				}
			}

			// translation keyboard
			if(dKEY(DIK_SPACE))
			{
				if(dKEY(DIK_LSHIFT))
				{
					if(dOKEY(DIK_UPARROW))
					{
						RwV3d vPos = pPLDojo->GetPosition();
						vPos.y += dGETHDL()->m_fObjMovementSensitivity;
						pPLDojo->SetPosition(&vPos);
						dGETFRM()->m_pDojoPalette->UpdatePropertyData(pPLDojo);
					}
					else if(dOKEY(DIK_DOWNARROW))
					{
						RwV3d vPos = pPLDojo->GetPosition();
						vPos.y -= dGETHDL()->m_fObjMovementSensitivity;
						pPLDojo->SetPosition(&vPos);
						dGETFRM()->m_pDojoPalette->UpdatePropertyData(pPLDojo);
					}
				}
				else
				{
					if(dKEY(DIK_UPARROW))
					{
						RwV3d vPos = pPLDojo->GetPosition();
						vPos.y += dGETHDL()->m_fObjMovementSensitivity;
						pPLDojo->SetPosition(&vPos);
						dGETFRM()->m_pDojoPalette->UpdatePropertyData(pPLDojo);
					}
					else if(dKEY(DIK_DOWNARROW))
					{
						RwV3d vPos = pPLDojo->GetPosition();
						vPos.y -= dGETHDL()->m_fObjMovementSensitivity;
						pPLDojo->SetPosition(&vPos);
						dGETFRM()->m_pDojoPalette->UpdatePropertyData(pPLDojo);
					}
				}
			}

			if(!dKEY(DIK_SPACE))
			{
				if(dKEY(DIK_LEFTARROW) || dKEY(DIK_RIGHTARROW) || dKEY(DIK_UPARROW) || dKEY(DIK_DOWNARROW))
				{
					RwFrame *pCameraFrame	= RwCameraGetFrame(CNtlPLGlobal::m_RwCamera);
					RwMatrix *pM			= RwFrameGetMatrix(pCameraFrame);
					RwV3d *pRight			= RwMatrixGetRight(pM);
					RwV3d *pUp				= RwMatrixGetUp(pM);
					RwV3d *pAt				= RwMatrixGetAt(pM);
					RwV3d vFront, vRight;

					CNtlMath::MathRwV3dAssign(&vFront, pAt->x, 0.0f, pAt->z);
					CNtlMath::MathRwV3dAssign(&vRight, pRight->x, 0.0f, pRight->z);
					RwV3dNormalize(&vFront, &vFront);
					RwV3dNormalize(&vRight, &vRight);

					RwV3d vPos = pPLDojo->GetPosition();

					// delete from the previous field
					RwInt32 CurSectorIdx = pMgr->GetSectorIdx(vPos);
					if(CurSectorIdx != -1)
					{
						pMgr->m_pSectors[CurSectorIdx].DeleteEntity(static_cast<CNtlPLEntity*>(pPLDojo), 11);
					}

					if(!dKEY(DIK_LSHIFT))
					{
						if(dKEY(DIK_LEFTARROW))
						{
							RwV3dIncrementScaled(&vPos, &vRight, dGETHDL()->m_fObjMovementSensitivity);
						}
						else if(dKEY(DIK_RIGHTARROW))
						{
							RwV3dIncrementScaled(&vPos, &vRight, -dGETHDL()->m_fObjMovementSensitivity);
						}
						else if(dKEY(DIK_UPARROW))
						{
							RwV3dIncrementScaled(&vPos, &vFront, dGETHDL()->m_fObjMovementSensitivity);
						}
						else if(dKEY(DIK_DOWNARROW))
						{
							RwV3dIncrementScaled(&vPos, &vFront, -dGETHDL()->m_fObjMovementSensitivity);
						}
					}
					else
					{
						if(dOKEY(DIK_LEFTARROW))
						{
							RwV3dIncrementScaled(&vPos, &vRight, dGETHDL()->m_fObjMovementSensitivity);
						}
						else if(dOKEY(DIK_RIGHTARROW))
						{
							RwV3dIncrementScaled(&vPos, &vRight, -dGETHDL()->m_fObjMovementSensitivity);
						}
						else if(dOKEY(DIK_UPARROW))
						{
							RwV3dIncrementScaled(&vPos, &vFront, dGETHDL()->m_fObjMovementSensitivity);
						}
						else if(dOKEY(DIK_DOWNARROW))
						{
							RwV3dIncrementScaled(&vPos, &vFront, -dGETHDL()->m_fObjMovementSensitivity);
						}
					}

					pPLDojo->SetPosition(&vPos);
					dGETFRM()->m_pDojoPalette->UpdatePropertyData(pPLDojo);

					// create into the new field
					CurSectorIdx = pMgr->GetSectorIdx(vPos);
					if(CurSectorIdx != -1)
					{
						pMgr->m_pSectors[CurSectorIdx].InsertIn2World(static_cast<CNtlPLEntity*>(pPLDojo), 11);
					}
				}
			}
		}
	}
}

void CUpdateEventDojoPalette::RenderDojoPalette()
{
	if(dGET_CUR_PALETTE() == ePM_DOJO)
	{
		dGETFRM()->m_pDojoPalette->RenderPreview();

		if (m_vecSelected.size())
		{
			CNtlPLDojo* pPLDojo = (CNtlPLDojo*)m_vecSelected[0];
			if (pPLDojo->GetDojoObject())
			{
				CNtlWB_Circle<CNtlWorldFieldManager>* pCurBrush	= reinterpret_cast<CNtlWB_Circle<CNtlWorldFieldManager>*>(m_pBrushSelection->GetController());
				RwV3d Scale = *pPLDojo->GetScaleVector();

				pCurBrush->Render(pPLDojo->GetPosition(), pPLDojo->GetDojoObject()->GetBoundingSphere()->radius, Scale);
			}
		}
	}
}

void CUpdateEventDojoPalette::RenderDisplayInfoDojoPalette(RtCharset* pRtCharSet, RwInt32 iWindowWidth, RwInt32 iWindowHeight)
{
	if(dGET_CUR_PALETTE() == ePM_DOJO)
	{
		RwChar	acCaption[256];
		RwV3d	v2dPoint;
		RwRGBA	clrFont; clrFont.alpha = 255;


		if (m_vecSelected.size())
		{
			CNtlPLDojo*	pPLDojo	= (CNtlPLDojo*)m_vecSelected.at(0);
			RwV3d		vPos	= pPLDojo->GetPosition();	
			RwV3d		vScale	= *pPLDojo->GetScaleVector();
			RwV3d		vRot	= *pPLDojo->GetRotate();

			RwMatrix *pMatView = RwCameraGetViewMatrix(CNtlPLGlobal::m_RwCamera);
			RwV3dTransformPoints(&v2dPoint, &vPos, 1, pMatView);
			v2dPoint.x = (v2dPoint.x / v2dPoint.z) * iWindowWidth + 10.0f;
			v2dPoint.y = (v2dPoint.y / v2dPoint.z) * iWindowHeight + 10.0f;

			clrFont.red		= 255;
			clrFont.green	= 255;
			clrFont.blue	= 255;
			RtCharsetSetColors(pRtCharSet, &clrFont, &dGETAPP().m_BackgroundColor);
			RsSprintf(acCaption, RWSTRING("ID : %d"), pPLDojo->GetDojoID());
			RsCharsetPrint(pRtCharSet, acCaption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y);

			clrFont.red		= 255;
			clrFont.green	= 0;
			clrFont.blue	= 0;
			RtCharsetSetColors(pRtCharSet, &clrFont, &dGETAPP().m_BackgroundColor);
			RsSprintf(acCaption, RWSTRING("S : %.2f, %.2f, %.2f"), vScale.x, vScale.y, vScale.z);
			RsCharsetPrint(pRtCharSet, acCaption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y + 15);

			clrFont.red		= 0;
			clrFont.green	= 255;
			clrFont.blue	= 0;
			RtCharsetSetColors(pRtCharSet, &clrFont, &dGETAPP().m_BackgroundColor);
			RsSprintf(acCaption, RWSTRING("R : %.2f, %.2f, %.2f"), vRot.x, vRot.y, vRot.z);
			RsCharsetPrint(pRtCharSet, acCaption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y + 30);

			clrFont.red		= 0;
			clrFont.green	= 0;
			clrFont.blue	= 255;
			RtCharsetSetColors(pRtCharSet, &clrFont, &dGETAPP().m_BackgroundColor);
			RsSprintf(acCaption, RWSTRING("T : %.2f, %.2f, %.2f"), vPos.x, vPos.y, vPos.z);
			RsCharsetPrint(pRtCharSet, acCaption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y + 45);
		}
	}
}

CNtlPLDojo* CUpdateEventDojoPalette::GetCurDojo()
{
	if (m_vecSelected.size())
	{
		return (CNtlPLDojo*)m_vecSelected.at(0);
	}
	return NULL;
}

void CUpdateEventDojoPalette::DeleteDojoPaletteDummies()
{
	m_vecSelected.clear();
}
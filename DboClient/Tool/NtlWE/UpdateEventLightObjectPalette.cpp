#include "stdafx.h"

#include "NtlWE.h"
#include "NtlWEFrm.h"
#include "NtlWEDoc.h"

#include "NtlWorldFieldManager.h"

#include "NtlPLRenderGeometry.h"
#include "NtlPLWorldEntity.h"

#include "NtlWorldPaletteDefine.h"

#include "PaletteLightAndShadow.h"
#include "PaletteLightObject.h"

#include "UpdateEventLightObjectPalette.h"

RwFrame* GetBoneList(RwFrame *pFrame, void *pData)
{
	std::vector<std::string>* vecBoneNameList = static_cast<std::vector<std::string>*>(pData);

	RpUserDataArray *pUserData = RwFrameGetUserDataArray(pFrame, 0);
	if(pUserData != NULL)
	{
		char *chBuffer = RpUserDataArrayGetString(pUserData, 0);
		if(chBuffer != NULL)
		{
			vecBoneNameList->push_back(chBuffer);
		}
		RwFrameForAllChildren(pFrame, GetBoneList, pData);
	}

	return pFrame;
}

RpAtomic* GetBoneList(RpIntersection* pIntersection, RpWorldSector* pWorldSector, RpAtomic* pAtomic, RwReal fDistance, void* pData)
{
	if((static_cast<CNtlPLEntity*>(RpNtlAtomicGetData(pAtomic))->GetClassType() != PLENTITY_OBJECT))
	{
		return pAtomic;
	}

	CNtlPLObject* pNtlPLObject = static_cast<CNtlPLObject*>(RpNtlAtomicGetData(pAtomic));

#ifdef dNTL_WORLD_TOOL_MODE
	if (((pNtlPLObject->GetFlags() & NTL_PLEFLAG_COLLISION) && (RpNtlAtomicGetFlag(pAtomic) & NTL_NOT_VISIBLE)) && !CNtlPLGlobal::m_bCollObjVisible)
	{
		return pAtomic;
	}
#endif 
	std::vector<SAttachBone>* pAttachBoneList = static_cast<std::vector<SAttachBone>*>(pData);
	for (std::vector<SAttachBone>::iterator it = pAttachBoneList->begin(); it != pAttachBoneList->end(); ++it)
	{
		SAttachBone& sAttachBone = *it;
		if (sAttachBone.pNtlPLObject == pNtlPLObject)
		{
			return pAtomic;
		}
	}

	std::vector<std::string> vecBoneNameList;
	pNtlPLObject->GetBoneName(vecBoneNameList);
	//RwFrameForAllChildren( RpClumpGetFrame(pNtlPLObject->GetClump()), GetBoneList, &vecBoneNameList);

	for (int i = 0; i < (int)vecBoneNameList.size(); ++i)
	{
		RwMatrix* pMat = pNtlPLObject->GetBoneMatrix(vecBoneNameList.at(i).c_str());
		if (pMat)
		{
			SAttachBone sAttachBone;
			sAttachBone.pNtlPLObject	= pNtlPLObject;
			sAttachBone.strName			= vecBoneNameList.at(i);
			sAttachBone.fDistanceDelta	= RwV3dLength(&(pIntersection->t.sphere.center - *RwMatrixGetPos(pMat)));
			pAttachBoneList->push_back(sAttachBone);			
		}
	}
	
	return pAtomic;
}

CUpdateEventLightObjectPalette::CUpdateEventLightObjectPalette()
:m_iTabAttachBone(-1)
{
}

CUpdateEventLightObjectPalette::~CUpdateEventLightObjectPalette()
{
}
bool CompareAttachBone(SAttachBone& left, SAttachBone& right)
{
	return left.fDistanceDelta < right.fDistanceDelta;
}

void CUpdateEventLightObjectPalette::UpdateLightObjectPalette()
{
	CNtlWorldFieldManager *pMgr = dGETMGR();

	if(dGET_BRUSH_ENTITY())
	{
		if(dOKEY(DIK_SPACE))
		{
			if(dGET_BRUSH_ENTITY())
			{
				dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(NULL);
				GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
				dGET_BRUSH_ENTITY() = NULL;
				return;
			}
		}

		// move entity
		sNPE_COLLISION_PARAM sNPECollisionParam;
		sNPECollisionParam._pNtlPLEntity = dGET_BRUSH_ENTITY();

		RwV3d vDist;
		vDist.x = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.x - dGET_COLLISION_INFO()->RayDir.x * 10.0f;
		vDist.y = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.y - dGET_COLLISION_INFO()->RayDir.y * 10.0f;
		vDist.z = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.z - dGET_COLLISION_INFO()->RayDir.z * 10.0f;

		dGET_BRUSH_ENTITY()->SetPosition(&vDist);
		if (dGET_COLLISION_INFO()->IsCollidedAtSectors)
		{
			// place effects
			if(dGET_MOUSE()->GetLBOnce())
			{
				if(dGET_BRUSH_ENTITY())
				{
					RwInt32 CurSectorIdx = pMgr->GetSectorIdx(vDist);
					if(CurSectorIdx != -1)
					{
						if (dGETFRM()->m_pLightObjectPalette->GetCreateParam(dGET_BRUSH_ENTITY()))
						{
							CNtlPLEntity *pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_LIGHT_OBJECT, NULL, dGETFRM()->m_pLightObjectPalette->GetCreateParam(dGET_BRUSH_ENTITY()));
							DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

							pMgr->m_pSectors[CurSectorIdx].InsertIn2World(pNtlPLEntity, 8);
							return;
						}					
					}
				}
			}
		}
	}
	else
	{
		// picking
		if(dGET_MOUSE()->GetLBOnce() && !dKEY(DIK_LSHIFT) && !dKEY(DIK_RSHIFT) && !dKEY(DIK_SPACE))
		{
			RwReal					fCurDist;
			CNtlPLLightObject*		pNtlPLEntity	= NULL;
			RwReal					fResultDist		= RwRealMAXVAL;	

			RwV3d vCur;
			if (GetSceneManager()->GetWorld()->GetPlayerPosition(vCur))
			{
				CNtlWorldFieldManager*	pWFMgr		= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
				RwInt32					iSectorIdx	= pWFMgr->GetSectorIdx(vCur);

				if (iSectorIdx)
				{
					vector<RwInt32> vecNeighborSectorIndices;
					pWFMgr->GetNeighborSectors(iSectorIdx, vecNeighborSectorIndices);
					vecNeighborSectorIndices.push_back(iSectorIdx);

					for (int i = 0; i < (int)vecNeighborSectorIndices.size(); ++i)
					{
						RwInt32 iIdx = vecNeighborSectorIndices.at(i);
						if (iIdx == -1)
						{
							continue;
						}

						CNtlWorldSector* pNtlWorldSector = &pWFMgr->m_pSectors[vecNeighborSectorIndices.at(i)];
						for (int j = 0; j < (int)pNtlWorldSector->m_vecNtlPLEntityLightObject.size(); ++j)
						{
							CNtlPLLightObject* pNtlPLEntityTemp = ((CNtlPLLightObject*)pNtlWorldSector->m_vecNtlPLEntityLightObject.at(j));
							fCurDist = RwRealMAXVAL;
							if(pNtlPLEntityTemp->Pick(fCurDist))
							{
								if(fCurDist < fResultDist)
								{
									fResultDist		= fCurDist;
									pNtlPLEntity	= pNtlPLEntityTemp;
								}
							}
						}

						for (int j = 0; j < (int)pNtlWorldSector->m_vecNtlPLEntityLightObjectDummy.size(); ++j)
						{
							CNtlPLLightObject* pNtlPLEntityTemp = ((CNtlPLLightObject*)pNtlWorldSector->m_vecNtlPLEntityLightObjectDummy.at(j));
							fCurDist = RwRealMAXVAL;
							if(pNtlPLEntityTemp->Pick(fCurDist))
							{
								if(fCurDist < fResultDist)
								{
									fResultDist		= fCurDist;
									pNtlPLEntity	= pNtlPLEntityTemp;
								}
							}
						}

						for (int j = 0; j < (int)pNtlWorldSector->m_vecNtlPLEntityOrnament.size(); ++j)
						{
							CNtlPLObject* pPLObject = ((CNtlPLObject*)pNtlWorldSector->m_vecNtlPLEntityOrnament.at(j));

							for (ListAttachAttr::iterator it = pPLObject->GetAttachAttrList()->begin(); it != pPLObject->GetAttachAttrList()->end(); ++it)
							{
								SPLAttachAttr* pPLAttachAttr = *it;
								if (pPLAttachAttr->pPLEntity->GetClassType() == PLENTITY_LIGHT_OBJECT)
								{
									CNtlPLLightObject* pNtlPLEntityTemp = (CNtlPLLightObject*)pPLAttachAttr->pPLEntity;									
									fCurDist = RwRealMAXVAL;
									if(pNtlPLEntityTemp->Pick(fCurDist))
									{
										if(fCurDist < fResultDist)
										{
											fResultDist		= fCurDist;
											pNtlPLEntity	= pNtlPLEntityTemp;
										}
									}
								}
							}					
						}
					}
				}
			}

			m_vecSelected.clear();
			if (pNtlPLEntity)
			{
				m_vecSelected.push_back(pNtlPLEntity);
			}
			dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pNtlPLEntity);
		}

		m_vecAttachBoneList.clear();
		if (m_vecSelected.size())
		{
			CNtlWorldFieldManager*	pMgr				= dGETMGR();
			CNtlPLLightObject*		pPLEntitySelected	= (CNtlPLLightObject*)m_vecSelected.front();
						
			if (dGETFRM()->m_pLightObjectPalette->GetAttachMode())
			{
				if (pPLEntitySelected->GetAttachParent())
				{
					//intersection.t.sphere.center	= *RwMatrixGetPos(&pPLEntitySelected->GetMatrix());
				}
				else
				{
					RpIntersection intersection;
					intersection.type				= rpINTERSECTSPHERE;
					intersection.t.sphere.center	= pPLEntitySelected->GetPosition();
					intersection.t.sphere.radius	= 5.0f;
					RpWorldForAllAtomicIntersections(CNtlPLGlobal::m_pRpWorld, &intersection, GetBoneList, &m_vecAttachBoneList);
					std::sort(m_vecAttachBoneList.begin(), m_vecAttachBoneList.end(), CompareAttachBone);
				}			

				if (dOKEY(DIK_TAB))
				{
					if (++m_iTabAttachBone >= (RwInt32)m_vecAttachBoneList.size())
					{
						m_iTabAttachBone = -1;
					}
				}

				if (m_vecAttachBoneList.empty())
				{
					m_iTabAttachBone = -1;
				}
				else
				{
					CLAMP(m_iTabAttachBone, 0, (RwInt32)m_vecAttachBoneList.size() - 1);
				}

				if (dOKEY(DIK_T))
				{
					CNtlPLAttach* pParent = pPLEntitySelected->GetAttachParent();
					if (pParent)
					{
						pParent->Detach(pPLEntitySelected);

						RwInt32 CurSectorIdx = pMgr->GetSectorIdx(pParent->GetPosition());
						if (CurSectorIdx != -1)
						{
							pPLEntitySelected->SetPosition(&pParent->GetPosition());
							pMgr->m_pSectors[CurSectorIdx].InsertIn2World(static_cast<CNtlPLEntity*>(pPLEntitySelected), 8);
						}
						else
						{
							GetSceneManager()->DeleteEntity(pPLEntitySelected);
							m_vecSelected.clear();
							dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(NULL);
							return;
						}
					}
					else
					{
						if (m_iTabAttachBone != -1)
						{
							RwInt32 CurSectorIdx = pMgr->GetSectorIdx(pPLEntitySelected->GetPosition());
							if (CurSectorIdx != -1)
							{
								pMgr->m_pSectors[CurSectorIdx].DeleteEntity(static_cast<CNtlPLEntity*>(pPLEntitySelected), 8);
							}
							SAttachBone& sAttachBone = m_vecAttachBoneList.at(m_iTabAttachBone);
							sAttachBone.pNtlPLObject->AttachBone(pPLEntitySelected, sAttachBone.strName.c_str());

							RwV3d vPos = { 0.0f, 0.0f, 0.0f };
							pPLEntitySelected->SetPosition(&vPos);
						}
					}
					dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pPLEntitySelected);
				}
			}

			if (dOKEY(DIK_RETURN) && GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
			{
				dGETFRM()->m_pLightAndShadowPalette->CreateWorldLightByLightObject(pPLEntitySelected);
				return;
			}

			if(dOKEY(DIK_DELETE))
			{
				RwInt32 iSectorIdx = pMgr->GetSectorIdx(pPLEntitySelected->GetPosition());
				if (iSectorIdx != -1)
				{
					pMgr->m_pSectors[iSectorIdx].DeleteEntity(pPLEntitySelected, 8);
					m_vecSelected.clear();
					dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(NULL);
				}
				GetSceneManager()->DeleteEntity(pPLEntitySelected);
				return;
			}

			if(dKEY(DIK_LCONTROL) && dGET_MOUSE()->GetLBOnce() && m_vecSelected.size() == 1)
			{
				if(dGET_BRUSH_ENTITY())
				{
					GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
					dGET_BRUSH_ENTITY() = NULL;
				}

				if (dGETFRM()->m_pLightObjectPalette->GetCreateParam(m_vecSelected.at(0)))
				{
					dGET_BRUSH_ENTITY() = GetSceneManager()->CreateEntity(PLENTITY_LIGHT_OBJECT, NULL, dGETFRM()->m_pLightObjectPalette->GetCreateParam(m_vecSelected.at(0)));
					DBO_ASSERT(dGET_BRUSH_ENTITY(), "Entity create failed.");

					dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(dGET_BRUSH_ENTITY());

					m_vecSelected.clear();
					return;
				}					
			}

			if(dGET_MOUSE()->GetWheelMovement())
			{
				int iValue = static_cast<int>(dGET_MOUSE()->GetWheelMovement() / ::fabsf(dGET_MOUSE()->GetWheelMovement()));

				if(dKEY(DIK_LSHIFT))
				{
					RwReal fRadius = pPLEntitySelected->GetRadius();
					fRadius += static_cast<RwReal>(iValue);
					CLAMP(fRadius, 0.5f, static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize));
					pPLEntitySelected->SetRadius(fRadius);
					dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pPLEntitySelected);

				}
				else if (pPLEntitySelected->GetLightType() != rpLIGHTPOINT)
				{
					RwReal fAngle = pPLEntitySelected->GetAngle();
					fAngle += static_cast<RwReal>(iValue);
					CLAMP(fAngle, 0.0f, 90.0f);
					pPLEntitySelected->SetAngle(fAngle);
					dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pPLEntitySelected);
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
						RwV3d vPos = pPLEntitySelected->GetPosition();
						vPos.y -= DistY;
						pPLEntitySelected->SetPosition(&vPos);
						dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pPLEntitySelected);
					}
					else
					{
						RwFrame *pCameraFrame	= RwCameraGetFrame(CNtlPLGlobal::m_RwCamera);
						RwMatrix *pM			= RwFrameGetMatrix(pCameraFrame);
						RwV3d *pRight			= RwMatrixGetRight(pM);
						RwV3d *pUp				= RwMatrixGetUp(pM);
						RwV3d *pAt				= RwMatrixGetAt(pM);
						RwV3d vFront, vRight;
						RwReal /*TerrainHeight, TotalHeight, */ObjHeight;

						CNtlMath::MathRwV3dAssign(&vFront, pAt->x, 0.0f, pAt->z);
						CNtlMath::MathRwV3dAssign(&vRight, pRight->x, 0.0f, pRight->z);
						RwV3dNormalize(&vFront, &vFront);
						RwV3dNormalize(&vRight, &vRight);

						RwV3d vPos = pPLEntitySelected->GetPosition();
						// delete from the previous field
						RwInt32 CurSectorIdx = pMgr->GetSectorIdx(vPos);
						if(CurSectorIdx != -1 && !pPLEntitySelected->GetAttachParent())
						{
							pMgr->m_pSectors[CurSectorIdx].DeleteEntity(static_cast<CNtlPLEntity*>(pPLEntitySelected), 8);
						}

						ObjHeight		= vPos.y;

						// transform
						RwV3dIncrementScaled(&vPos, &vFront, -DistY);
						RwV3dIncrementScaled(&vPos, &vRight, -DistX);

						vPos.y = ObjHeight;

						pPLEntitySelected->SetPosition(&vPos);
						dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pPLEntitySelected);

						// create into the new field
						CurSectorIdx = pMgr->GetSectorIdx(vPos);
						if(CurSectorIdx != -1 && !pPLEntitySelected->GetAttachParent())
						{
							pMgr->m_pSectors[CurSectorIdx].InsertIn2World(static_cast<CNtlPLEntity*>(pPLEntitySelected), 8);
						}
					}
				}
				// Rot
				else if(dKEY(DIK_SPACE) && pPLEntitySelected->GetLightType() != rpLIGHTPOINT)
				{
					float DistX = dGET_MOUSE()->GetXMovementWithRate(1.0f);
					float DistY = dGET_MOUSE()->GetYMovementWithRate(1.0f);

					if(dKEY(DIK_V))
					{
						RwV3d vRot = *pPLEntitySelected->GetRotate();
						vRot.x -= DistX;
						pPLEntitySelected->SetRotate(vRot.x, vRot.y, vRot.z);
						dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pPLEntitySelected);
					}
					else if(dKEY(DIK_N))
					{
						RwV3d vRot = *pPLEntitySelected->GetRotate();
						vRot.z -= DistX;
						pPLEntitySelected->SetRotate(vRot.x, vRot.y, vRot.z);
						dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pPLEntitySelected);
					}
					else
					{
						RwV3d vRot = *pPLEntitySelected->GetRotate();
						vRot.y -= DistX;
						pPLEntitySelected->SetRotate(vRot.x, vRot.y, vRot.z);
						dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pPLEntitySelected);
					}
				}
				// Scale
				else if(dKEY(DIK_RSHIFT))
				{
					float DistX = dGET_MOUSE()->GetXMovementWithRate(10.0f);
					float DistY = dGET_MOUSE()->GetYMovementWithRate(10.0f);

					RwReal fRadius = pPLEntitySelected->GetRadius();
					fRadius -= DistY;
					CLAMP(fRadius, 0.5f, dGET_WORLD_PARAM()->WorldSectorSize);
					pPLEntitySelected->SetRadius(fRadius);
					dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pPLEntitySelected);
				}
			}

			// Rotation using Keyboard
			if(dKEY(DIK_SPACE))
			{
				if(dKEY(DIK_LSHIFT))
				{
					if(dOKEY(DIK_LEFTARROW))
					{
						RwV3d vRot = *pPLEntitySelected->GetRotate();
						vRot.y -= dGETHDL()->m_RotationTabValue;
						pPLEntitySelected->SetRotate(vRot.x, vRot.y, vRot.z);
						dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pPLEntitySelected);
					}
					else if(dOKEY(DIK_RIGHTARROW))
					{
						RwV3d vRot = *pPLEntitySelected->GetRotate();
						vRot.y += dGETHDL()->m_RotationTabValue;
						pPLEntitySelected->SetRotate(vRot.x, vRot.y, vRot.z);
						dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pPLEntitySelected);
					}
				}
				else
				{
					if(dKEY(DIK_LEFTARROW))
					{
						RwV3d vRot = *pPLEntitySelected->GetRotate();
						vRot.y -= dGETHDL()->m_RotationTabValue;
						pPLEntitySelected->SetRotate(vRot.x, vRot.y, vRot.z);
						dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pPLEntitySelected);
					}
					else if(dKEY(DIK_RIGHTARROW))
					{
						RwV3d vRot = *pPLEntitySelected->GetRotate();
						vRot.y += dGETHDL()->m_RotationTabValue;
						pPLEntitySelected->SetRotate(vRot.x, vRot.y, vRot.z);
						dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pPLEntitySelected);
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
						RwV3d vPos = pPLEntitySelected->GetPosition();
						vPos.y += dGETHDL()->m_fObjMovementSensitivity;
						pPLEntitySelected->SetPosition(&vPos);
						dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pPLEntitySelected);
					}
					else if(dOKEY(DIK_DOWNARROW))
					{
						RwV3d vPos = pPLEntitySelected->GetPosition();
						vPos.y -= dGETHDL()->m_fObjMovementSensitivity;
						pPLEntitySelected->SetPosition(&vPos);
						dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pPLEntitySelected);
					}
				}
				else
				{
					if(dKEY(DIK_UPARROW))
					{
						RwV3d vPos = pPLEntitySelected->GetPosition();
						vPos.y += dGETHDL()->m_fObjMovementSensitivity;
						pPLEntitySelected->SetPosition(&vPos);
						dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pPLEntitySelected);
					}
					else if(dKEY(DIK_DOWNARROW))
					{
						RwV3d vPos = pPLEntitySelected->GetPosition();
						vPos.y -= dGETHDL()->m_fObjMovementSensitivity;
						pPLEntitySelected->SetPosition(&vPos);
						dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pPLEntitySelected);
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

					RwV3d vPos = pPLEntitySelected->GetPosition();

					// delete from the previous field
					RwInt32 CurSectorIdx = pMgr->GetSectorIdx(vPos);
					if(CurSectorIdx != -1 && !pPLEntitySelected->GetAttachParent())
					{
						pMgr->m_pSectors[CurSectorIdx].DeleteEntity(static_cast<CNtlPLEntity*>(pPLEntitySelected), 8);
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

					pPLEntitySelected->SetPosition(&vPos);
					dGETFRM()->m_pLightObjectPalette->UpdatePropertyData(pPLEntitySelected);

					// create into the new field
					CurSectorIdx = pMgr->GetSectorIdx(vPos);
					if(CurSectorIdx != -1 && !pPLEntitySelected->GetAttachParent())
					{
						pMgr->m_pSectors[CurSectorIdx].InsertIn2World(static_cast<CNtlPLEntity*>(pPLEntitySelected), 8);
					}
				}
			}
		}
	}
}

void CUpdateEventLightObjectPalette::RenderLightObjectPalette()
{
	if (dGET_CUR_PALETTE() != ePM_LIGHTOBJECT)
	{
		return;
	}

	CNtlWorldRpLightManager::GetInstance()->Render();
	
	if (dGET_BRUSH_ENTITY() && dGET_BRUSH_ENTITY()->GetClassType() == PLENTITY_LIGHT_OBJECT)
	{
		((CNtlPLLightObject*)dGET_BRUSH_ENTITY())->OnRender(NULL);
	}


	if (dGETFRM()->m_pLightObjectPalette->GetAttachMode())
	{
		RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *) FALSE);
		RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *) FALSE);

		for (int i = 0; i < (int)m_vecAttachBoneList.size(); ++i)
		{
			SAttachBone&	sAttachBone	= m_vecAttachBoneList.at(i);
			RwMatrix*		pMatrix		= sAttachBone.pNtlPLObject->GetBoneMatrix(sAttachBone.strName.c_str());

			RwRGBA rgba = { 255, 255 ,255 ,255 };

			if (i == m_iTabAttachBone)
			{
				rgba.red = 255; rgba.green = 0; rgba.blue = 0;
			}

			RenderGeometryBillboardSphere(pMatrix, &rgba, 1.0f, CNtlPLGlobal::m_RwCamera, TRUE);

			dGETAPP().RenderText(*RwMatrixGetPos(pMatrix), sAttachBone.strName.c_str(), rgba.red, rgba.green, rgba.blue);
		}

		for (int i = 0; i < (int)g_vecVisibleSectors.size(); ++i)
		{
			RwInt32 iIdx = g_vecVisibleSectors.at(i);
			if (iIdx == -1)
			{
				continue;
			}

			CNtlWorldSector* pNtlWorldSector = &dGETMGR()->m_pSectors[iIdx];
			for (int j = 0; j < (int)pNtlWorldSector->m_vecNtlPLEntityOrnament.size(); ++j)
			{
				CNtlPLObject* pPLObject = ((CNtlPLObject*)pNtlWorldSector->m_vecNtlPLEntityOrnament.at(j));

				for (ListAttachAttr::iterator it = pPLObject->GetAttachAttrList()->begin(); it != pPLObject->GetAttachAttrList()->end(); ++it)
				{
					SPLAttachAttr* pPLAttachAttr = *it;
					if (pPLAttachAttr->pPLEntity->GetClassType() == PLENTITY_LIGHT_OBJECT)
					{
						RwMatrix*		pMatrix		= pPLObject->GetBoneMatrix(pPLAttachAttr->chBoneName);
						RwRGBA			rgbaColor	= { 0, 0 , 255, 255 };

						RwIm3DVertex	aImLine[2];
						RwV3d*			pvPosLine = NULL;

						RenderGeometryBillboardSphere(pMatrix, &rgbaColor, 1.0f, CNtlPLGlobal::m_RwCamera, TRUE);


						pvPosLine = RwMatrixGetPos(pMatrix);
						RwIm3DVertexSetRGBA(&(aImLine[0]), rgbaColor.red, rgbaColor.green, rgbaColor.blue, rgbaColor.alpha);
						RwIm3DVertexSetPos(&(aImLine[0]), pvPosLine->x, pvPosLine->y, pvPosLine->z);

						pvPosLine = RwMatrixGetPos(&pPLAttachAttr->pPLEntity->GetMatrix());
						RwIm3DVertexSetRGBA(&(aImLine[1]), rgbaColor.red, rgbaColor.green, rgbaColor.blue, rgbaColor.alpha);
						RwIm3DVertexSetPos(&(aImLine[1]), pvPosLine->x, pvPosLine->y, pvPosLine->z);

						RwD3D9SetTexture(NULL, 0);
						if(RwIm3DTransform(aImLine, 2, NULL, rwIM3D_ALLOPAQUE))
						{
							RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);
							RwIm3DEnd();
						}

						dGETAPP().RenderText(*RwMatrixGetPos(pMatrix), pPLAttachAttr->chBoneName, rgbaColor.red, rgbaColor.green, rgbaColor.blue);
					}
				}					
			}
		}
		RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *) TRUE);
		RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *) FALSE);
	}
}

void CUpdateEventLightObjectPalette::DeleteLightObjectPaletteDummies()
{
	m_vecSelected.clear();
}
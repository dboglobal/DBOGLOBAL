#include "StdAfx.h"
#include "DecalIOHandler.h"
#include "NtlWorldDI.h"
#include "NtlPLSceneManager.h"
#include "NtlWe.h"
#include "PaletteDecalDlg.h"
#include "NtlWorldFieldManager.h"
#include "NtlPLWorldEntity.h"
#include "NtlWorldBrush.h"
#include "NtlWorldBrushRectangleInDetail.h"
#include "NtlEffectDefine.h"
#include "NtlPLDecalVolume.h"
#include "NtlPLEntity.h"
#include "NtlText.h"

using std::string;
#include <string>


CDecalIOHandler::CDecalIOHandler(void)
{
	m_Mode							= eDEM_IDLE_MODE;
	m_pNtlPLEntity					= NULL;
	m_pRectInDetailBrush			= NULL;
	m_pRectInDetailBrushSelecter	= NULL;
}

CDecalIOHandler::~CDecalIOHandler(void)
{
	if(m_pNtlPLEntity)
	{
		GetSceneManager()->DeleteEntity(m_pNtlPLEntity);
	}

	if(m_pRectInDetailBrush)
	{
		NTL_DELETE(m_pRectInDetailBrush);
	}

	if(m_pRectInDetailBrushSelecter)
	{
		NTL_DELETE(m_pRectInDetailBrushSelecter);
	}
}

VOID CDecalIOHandler::On_DEM_IDLE()
{
	CNtlWorldFieldManager*	pMgr	= dGETMGR();
	CNtlWorldField*			pFields	= const_cast<CNtlWorldField*>(pMgr->GetFields());

	if(!dKEY(DIK_LCONTROL))
	{
		// create dragging
		if(dGET_MOUSE()->GetLBUp())
		{
			if(m_pRectInDetailBrush)
			{
				CNtlWB_RectangleInDetail<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_RectangleInDetail<CNtlWorldFieldManager>*>(m_pRectInDetailBrush->GetController());

				RwV3d OldPosS	= pCurBrush->GetPosS();
				RwV3d PosS		= pCurBrush->GetPosS();
				RwV3d PosE		= pCurBrush->GetPosE();
				RwV3d FPosS;
				RwV3d FPosE;

				if(PosS.x <= PosE.x)
				{
					FPosS.x = PosS.x;
					FPosE.x = PosE.x;
				}
				else
				{
					FPosS.x = PosE.x;
					FPosE.x = PosS.x;
				}

				if(PosS.z <= PosE.z)
				{
					FPosS.z = PosS.z;
					FPosE.z = PosE.z;
				}
				else
				{
					FPosS.z = PosE.z;
					FPosE.z = PosS.z;
				}

				PosS = FPosS;
				PosE = FPosE;

				if(dKEY(DIK_SPACE) && m_pNtlPLEntity)
				{
					// get a decal user pointer
					m_pRectInDetailBrush->SetUP(m_pNtlPLEntity);	

					// create a final decal entity
					RwV3d CurPos = m_pNtlPLEntity->GetPosition();
					pMgr->m_pSectors[pMgr->GetSectorIdx(CurPos)].InsertIn2World(m_pNtlPLEntity, 4);

					// feedback to the UI dialog
					dGETFRM()->m_pDecalPalette->Refresh(static_cast<CNtlPLWorldDecal*>(m_pNtlPLEntity));

					m_pNtlPLEntity = NULL;

					// create a selection brush from the dummy brush using pointer swapping
					m_pRectInDetailBrushSelecter	= m_pRectInDetailBrush;
					m_pRectInDetailBrush			= NULL;
				}

				// cancel the current dummy entity
				if(m_pNtlPLEntity)
				{
					GetSceneManager()->DeleteEntity(m_pNtlPLEntity);
					m_pNtlPLEntity = NULL;
				}

				// delete brush
				NTL_DELETE(m_pRectInDetailBrush);

				if(dKEY(DIK_SPACE))
				{
					m_Mode = eDEM_EDIT_MODE;

					return;
				}
			}
		}

		if(dGET_MOUSE()->GetLBOnce())
		{
			if(!m_pRectInDetailBrush)
			{
				m_pRectInDetailBrush					= NTL_NEW CNtlWorldBrush;
				CNtlWorldBrushController *pController	= NTL_NEW CNtlWB_RectangleInDetail<CNtlWorldFieldManager>(eNBT_RECTANGLE_IN_DETAIL, pMgr, &CNtlWorldFieldManager::GetWorldSectorHeight);
				m_pRectInDetailBrush->SetController(pController);

				CNtlWB_RectangleInDetail<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_RectangleInDetail<CNtlWorldFieldManager>*>(m_pRectInDetailBrush->GetController());
				pCurBrush->SetPosS(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain);
			}

			if(m_pRectInDetailBrushSelecter)
			{
				NTL_DELETE(m_pRectInDetailBrushSelecter);
			}
		}

		if(dGET_MOUSE()->GetBehavior(MB_LBDN) && dGET_MOUSE()->GetBehavior(MB_MOVE))
		{
			if(m_pRectInDetailBrush)
			{
				CNtlWB_RectangleInDetail<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_RectangleInDetail<CNtlWorldFieldManager>*>(m_pRectInDetailBrush->GetController());
				pCurBrush->SetPosE(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain);

				if(m_pNtlPLEntity)
				{
					GetSceneManager()->DeleteEntity(m_pNtlPLEntity);
					m_pNtlPLEntity = NULL;
				}

				// get a current decal prop. param.
				sDECAL_PROP_PARAM sDecalPropParam = dGETFRM()->m_pDecalPalette->GetDecalParam();

				// run-time create decals
				RwV3d PosS	= pCurBrush->GetPosS();
				RwV3d PosE	= pCurBrush->GetPosE();

				SPLDecalCreateParam Param;
				Param.pTexName				= sDecalPropParam._TexName;
				Param.pTexPath				= ".\\Texture\\NtlWe\\Decal\\";

				// should be different size
				RwReal Width	= fabs((PosS.x - PosE.x) / 2.0f);
				RwReal Height	= fabs((PosS.z - PosE.z) / 2.0f);
				if(Width > Height)
				{
					Param.fIntersectionRadius = Width;
				}
				else
				{
					Param.fIntersectionRadius = Height;
				}

				Param.vSize.x				= fabs(PosS.x - PosE.x);
				Param.vSize.z				= fabs(PosS.z - PosE.z);
				Param.fScale				= sDecalPropParam._TexScale;
				Param.fVisibleSquaredDist	= sDecalPropParam._VisibilityDist * sDecalPropParam._VisibilityDist;		
				Param.eDecalType			= DECAL_TERRAIN;

				string CurTexName = Param.pTexName;
				string W_Prefix("w_");
				string C_Prefix("c_");

				if(-1 == CurTexName.find_first_of(W_Prefix) && -1 == CurTexName.find_first_of(C_Prefix))
				{
					DBO_TRACE(FALSE, "Decal textures should've got prefix; w_, c_"); 
				}
				else
				{
					RwInt32 IdxWPrefix = CurTexName.find_first_of(W_Prefix);
					RwInt32 IdxCPrefix = CurTexName.find_first_of(C_Prefix);

					if(!IdxWPrefix)
					{
						Param.rwWrapType = rwTEXTUREADDRESSWRAP;
					}
					else if(!IdxCPrefix)
					{
						Param.rwWrapType = rwTEXTUREADDRESSCLAMP;
					}
					else
					{
						DBO_TRACE(FALSE, "Decal textures should've got prefix; w_, c_"); 
					}
				}
				
				if(sDecalPropParam._DecalMode == 0)
				{
					Param.nFlagEffect = E_FLAG_DECAL_RENDER_WORLD;
				}
				else if(sDecalPropParam._DecalMode == 1)
				{
					Param.nFlagEffect = E_FLAG_DECAL_RENDER_OBJECT;
				}
				else if(sDecalPropParam._DecalMode == 2)
				{
					Param.nFlagEffect = E_FLAG_DECAL_RENDER_WORLD | E_FLAG_DECAL_RENDER_OBJECT;
				}
				else if(sDecalPropParam._DecalMode == 3)
				{
					Param.nFlagEffect = E_FLAG_DECAL_RENDER_OBJECT | E_FLAG_DECAL_WATER;
				}

				m_pNtlPLEntity = static_cast<CNtlPLWorldDecal*>(GetSceneManager()->CreateEntity(PLENTITY_DECAL, "NULL", &Param));
				DBO_ASSERT(m_pNtlPLEntity, "Entity create failed.");

				CNtlPLWorldDecal* pNtlPLWorldDecal = static_cast<CNtlPLWorldDecal*>(m_pNtlPLEntity);
				pNtlPLWorldDecal->SetRotate(sDecalPropParam._TexAngle);
				pNtlPLWorldDecal->SetColor(sDecalPropParam._RGBA.red, sDecalPropParam._RGBA.green, sDecalPropParam._RGBA.blue);
				pNtlPLWorldDecal->SetAlpha(sDecalPropParam._RGBA.alpha);
				pNtlPLWorldDecal->SetAlphaBlend(TRUE, rwBLENDSRCALPHA, rwBLENDINVSRCALPHA);
				pNtlPLWorldDecal->SetName(sDecalPropParam._TexName);

				if(sDecalPropParam._UVAniCheck)
				{
					if(sDecalPropParam._UVAniPatternIdx == 0)
					{
						SNtlPrtStdEmitterPrtUVAnim EmitterUVAnim;
						EmitterUVAnim.vDir.x		= 0.0f;
						EmitterUVAnim.vDir.y		= 0.0f;
						EmitterUVAnim.vDir.z		= 1.0f;
						EmitterUVAnim.fVelocity		= sDecalPropParam._UVAniSpeed;
						EmitterUVAnim.fMaxOffset	= 0.0f;
						pNtlPLWorldDecal->SetUVAnimAction(EmitterUVAnim);
					}
					else if(sDecalPropParam._UVAniPatternIdx == 1)
					{
						SNtlPrtStdEmitterPrtUVAnim EmitterUVAnim;
						EmitterUVAnim.vDir.x		= 0.0f;
						EmitterUVAnim.vDir.y		= 0.0f;
						EmitterUVAnim.vDir.z		= 1.0f;
						EmitterUVAnim.fVelocity		= sDecalPropParam._UVAniSpeed;
						EmitterUVAnim.fMaxOffset	= sDecalPropParam._UVAniOffset;
						pNtlPLWorldDecal->SetUVAnimAction(EmitterUVAnim);
					}
					else if(sDecalPropParam._UVAniPatternIdx == 2)
					{
						pNtlPLWorldDecal->Set2DRoateAction(sDecalPropParam._UVAniOffset, sDecalPropParam._UVAniSpeed);
					}
					else if(sDecalPropParam._UVAniPatternIdx == 3)
					{
						pNtlPLWorldDecal->SetSizeAction(sDecalPropParam._TexScale, sDecalPropParam._UVAniOffset, sDecalPropParam._UVAniSpeed);
					}
				}

				RwV3d MidPos;
				MidPos.x = (PosS.x - PosE.x) / 2.0f + PosE.x;
				MidPos.z = (PosS.z - PosE.z) / 2.0f + PosE.z;
				MidPos.y = pMgr->GetWorldSectorHeight(MidPos);

				pNtlPLWorldDecal->SetPosition(&MidPos);
			}
		}
	}
	else
	{
		// pick decals
		if(dGET_MOUSE()->GetLBOnce())
		{	
			for(RwInt32 i = 0; i < 36; ++i)
			{
				RwInt32 CurFieldIdx = pMgr->m_Fields6x6[1][i];
				
				if(CurFieldIdx == -1)
				{
					continue;
				}

				RwV3d				SPos = pFields[CurFieldIdx].GetSPos();
				RwV3d				SectorSPos;
				RwInt32				SectorIdx;
				int					l, m;
				RwUInt32			k;
				CNtlWorldSector*	pCurNtlWorldSector;

				for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
				{
					for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
					{
						SectorSPos.x		= (RwReal)m;
						SectorSPos.z		= (RwReal)l;
						SectorIdx			= pMgr->GetSectorIdx(SectorSPos);
						pCurNtlWorldSector	= &pMgr->m_pSectors[SectorIdx];
						
						// collision test between a camera ray and decals
						for(k = 0; k < pCurNtlWorldSector->m_vecNtlPLEntityDecal.size(); ++k)
						{
							CNtlPLWorldDecal*	pNtlPLWorldDecal	= static_cast<CNtlPLWorldDecal*>(pCurNtlWorldSector->m_vecNtlPLEntityDecal[k]);
							RwV3d				CurSize				= pNtlPLWorldDecal->GetSize();
							RwV3d				CurPos				= pNtlPLWorldDecal->GetPosition();
							RwV3d				SPos, EPos, SBottom, ETop;
							RwV3d				Poly0[3], Poly1[3];

							RwV3d CurHalfSize;
							CurHalfSize.x	= CurSize.x / 2.0f;
							CurHalfSize.z	= CurSize.z / 2.0f;
							SPos.x			= CurPos.x + CurHalfSize.x;
							SPos.z			= CurPos.z + CurHalfSize.z;
							SPos.y			= pMgr->GetWorldSectorHeight(SPos);
							EPos.x			= CurPos.x - CurHalfSize.x;
							EPos.z			= CurPos.z - CurHalfSize.z;
							EPos.y			= pMgr->GetWorldSectorHeight(EPos);
							SBottom.x		= SPos.x;
							SBottom.z		= EPos.z;
							SBottom.y		= pMgr->GetWorldSectorHeight(SBottom);
							ETop.x			= EPos.x;
							ETop.z			= SPos.z;
							ETop.y			= pMgr->GetWorldSectorHeight(ETop);

							//
							// [S]
							//  0----2 
							//  |  / |
							//  | /  |
							//  1----3
							//      [E]
							//
							// (0, 1, 2), (2, 1, 3)
							//
							Poly0[0] = SPos;
							Poly0[1] = SBottom;
							Poly0[2] = ETop;
							Poly1[0] = ETop;
							Poly1[1] = SBottom;
							Poly1[2] = EPos;

							// Find unit ray vector
							RwV2d ScreenPos;
							ScreenPos.x = (RwReal)dGET_MOUSE()->GetPosClicked().x;
							ScreenPos.y = (RwReal)dGET_MOUSE()->GetPosClicked().y;

							RwLine cameraRay;
							RwCameraCalcPixelRay(CNtlPLGlobal::m_RwCamera, &cameraRay, &ScreenPos);

							RwV3d RwRayDir, RayDeltha;
							RwV3dSubMacro(&RwRayDir, &cameraRay.end, &cameraRay.start);

							RwV3d IncIntersection = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;
							RwV3dIncrementScaled(&IncIntersection, &RwRayDir, 1000.0f);
							RwV3dSubMacro(&RayDeltha, &IncIntersection, &cameraRay.start);

							RwReal ResultDist;
							RwBool Picked	= FALSE;
							RwReal _CurDist	= RwRealMAXVAL;

							if(	RtIntersectionLineTriangle(&cameraRay.start, &RayDeltha, &Poly0[0], &Poly0[1], &Poly0[2], &ResultDist) || 
								RtIntersectionLineTriangle(&cameraRay.start, &RayDeltha, &Poly1[0], &Poly1[1], &Poly1[2], &ResultDist))
							{
								// picked results
								if(!m_pRectInDetailBrushSelecter)
								{
									m_pRectInDetailBrushSelecter			= NTL_NEW CNtlWorldBrush;
									CNtlWorldBrushController *pController	= NTL_NEW CNtlWB_RectangleInDetail<CNtlWorldFieldManager>(eNBT_RECTANGLE_IN_DETAIL, pMgr, &CNtlWorldFieldManager::GetWorldSectorHeight);
									m_pRectInDetailBrushSelecter->SetController(pController);
								}

								m_pRectInDetailBrushSelecter->SetUP(pNtlPLWorldDecal);

								CNtlWB_RectangleInDetail<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_RectangleInDetail<CNtlWorldFieldManager>*>(m_pRectInDetailBrushSelecter->GetController());
								pCurBrush->SetPosS(SPos);
								pCurBrush->SetPosE(EPos);

								// feedback to the UI dialog
								dGETFRM()->m_pDecalPalette->Refresh(pNtlPLWorldDecal);

								m_Mode = eDEM_EDIT_MODE;

								return;
							}
						}
					}
				}
			}
		}
	}
}

VOID CDecalIOHandler::On_DEM_EDIT()
{
	CNtlWorldFieldManager* pMgr = dGETMGR();

	// idle mode
	if(dGET_MOUSE()->GetLBOnce() && !dKEY(DIK_LSHIFT) && !dKEY(DIK_RSHIFT) && !dKEY(DIK_V))
	{
		On_DEM_IDLE();

		m_Mode = eDEM_IDLE_MODE;

		return;
	}

	// cancel selecting
	if(dKEY(DIK_ESCAPE))
	{
		if(m_pRectInDetailBrushSelecter)
		{
			NTL_DELETE(m_pRectInDetailBrushSelecter);

			m_Mode = eDEM_IDLE_MODE;

			return;
		}
	}

	// delete
	if(dOKEY(DIK_DELETE))
	{
		if(m_pRectInDetailBrushSelecter)
		{
			CNtlPLEntity*	pCurEntity	= static_cast<CNtlPLEntity*>(m_pRectInDetailBrushSelecter->GetUP());
			RwV3d			Pos			= pCurEntity->GetPosition();
			RwInt32			SectorIdx	= pMgr->GetSectorIdx(Pos);

			// delete entities
			pMgr->m_pSectors[SectorIdx].DeleteEntity(pCurEntity, 4);
			GetSceneManager()->DeleteEntity(pCurEntity);
			pCurEntity = NULL;

			// delete brushes
			NTL_DELETE(m_pRectInDetailBrushSelecter);

			// back to the idle handler
			m_Mode = eDEM_IDLE_MODE;
			return;
		}
	}

	// translate
	if(dKEY(DIK_LSHIFT) && dGET_MOUSE()->GetBehavior(MB_LBDN) && dGET_MOUSE()->GetBehavior(MB_MOVE))
	{
		if(m_pRectInDetailBrushSelecter)
		{
			float DistX = dGET_MOUSE()->GetXMovementWithRate(10.0f);
			float DistY = dGET_MOUSE()->GetYMovementWithRate(10.0f);

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

			CNtlPLWorldDecal*	pNtlPLDecal	= static_cast<CNtlPLWorldDecal*>(m_pRectInDetailBrushSelecter->GetUP());
			RwV3d				PosCur		= pNtlPLDecal->GetPosition();

			// delete from the previous field
			RwInt32 CurSectorIdx = pMgr->GetSectorIdx(PosCur);
			if(CurSectorIdx != -1)
			{
				pMgr->m_pSectors[CurSectorIdx].DeleteEntity(static_cast<CNtlPLEntity*>(pNtlPLDecal), 4);
			}

			// transform
			RwV3dIncrementScaled(&PosCur, &vFront, -DistY);
			RwV3dIncrementScaled(&PosCur, &vRight, -DistX);
			pNtlPLDecal->SetPosition(&PosCur);

			// create into the new field
			CurSectorIdx = pMgr->GetSectorIdx(PosCur);
			pMgr->m_pSectors[CurSectorIdx].InsertIn2World(static_cast<CNtlPLEntity*>(pNtlPLDecal), 4);

			// move brush
			RwV3d	SPos, EPos;
			RwV3d	CurHalfSize;
			RwV3d	CurSize = pNtlPLDecal->GetSize();
			CurHalfSize.x	= CurSize.x / 2.0f;
			CurHalfSize.z	= CurSize.z / 2.0f;
			SPos.x			= PosCur.x + CurHalfSize.x;
			SPos.z			= PosCur.z + CurHalfSize.z;
			SPos.y			= pMgr->GetWorldSectorHeight(SPos);
			EPos.x			= PosCur.x - CurHalfSize.x;
			EPos.z			= PosCur.z - CurHalfSize.z;
			EPos.y			= pMgr->GetWorldSectorHeight(EPos);

			CNtlWB_RectangleInDetail<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_RectangleInDetail<CNtlWorldFieldManager>*>(m_pRectInDetailBrushSelecter->GetController());
			pCurBrush->SetPosS(SPos);
			pCurBrush->SetPosE(EPos);
		}
	}

	// rotate
	if(dKEY(DIK_V) && dGET_MOUSE()->GetBehavior(MB_LBDN) && dGET_MOUSE()->GetBehavior(MB_MOVE))
	{
		if(m_pRectInDetailBrushSelecter)
		{
			float DistX = dGET_MOUSE()->GetXMovementWithRate(10.0f);
			float DistY = dGET_MOUSE()->GetYMovementWithRate(10.0f);

			CNtlPLWorldDecal*	pNtlPLCurDecal	= static_cast<CNtlPLWorldDecal*>(m_pRectInDetailBrushSelecter->GetUP());
			RwReal				Rotate			= pNtlPLCurDecal->GetRotate();

			Rotate -= DistX;
			pNtlPLCurDecal->SetRotate(Rotate);
		}
	}

	// scale
	if(dKEY(DIK_RSHIFT) && dGET_MOUSE()->GetBehavior(MB_LBDN) && dGET_MOUSE()->GetBehavior(MB_MOVE))
	{
		if(m_pRectInDetailBrushSelecter)
		{
			float DistX = dGET_MOUSE()->GetXMovementWithRate(10.0f);
			float DistY = dGET_MOUSE()->GetYMovementWithRate(10.0f);

			CNtlPLWorldDecal*	pNtlPLCurDecal	= static_cast<CNtlPLWorldDecal*>(m_pRectInDetailBrushSelecter->GetUP());
			RwReal				Scale			= pNtlPLCurDecal->GetBaseScale();

			Scale -= DistY;
			CLAMP(Scale, 0.0f, 20.0f);
			pNtlPLCurDecal->SetScale(Scale);
		}
	}
}

VOID CDecalIOHandler::ApplyDecalPropParam(sDECAL_PROP_PARAM& _DecalPropParam)
{
	if(m_pRectInDetailBrushSelecter)
	{
		CNtlWorldFieldManager*	pMgr			= dGETMGR();
		CNtlPLWorldDecal*		pNtlPLCurDecal	= static_cast<CNtlPLWorldDecal*>(m_pRectInDetailBrushSelecter->GetUP());
		RwV3d					PosCur			= pNtlPLCurDecal->GetPosition();
		SPLDecalCreateParam		Param;

		// delete previous decals
		pMgr->m_pSectors[pMgr->GetSectorIdx(PosCur)].DeleteEntity(static_cast<CNtlPLEntity*>(pNtlPLCurDecal), 4);
		GetSceneManager()->DeleteEntity(static_cast<CNtlPLEntity*>(pNtlPLCurDecal));
		pNtlPLCurDecal = NULL;

		// create new decals
		Param.pTexName	= _DecalPropParam._TexName;
		Param.pTexPath	= ".\\Texture\\NtlWe\\Decal\\";

		// should be different size
		RwReal Width	= _DecalPropParam._SizeX;
		RwReal Height	= _DecalPropParam._SizeZ;
		if(Width > Height)
		{
			Param.fIntersectionRadius = Width;
		}
		else
		{
			Param.fIntersectionRadius = Height;
		}

		Param.vSize.x				= Width;
		Param.vSize.z				= Height;
		Param.fScale				= _DecalPropParam._TexScale;
		Param.fVisibleSquaredDist	= _DecalPropParam._VisibilityDist * _DecalPropParam._VisibilityDist;		
		Param.eDecalType			= DECAL_TERRAIN;

		string CurTexName = _DecalPropParam._TexName;
		string W_Prefix("w_");
		string C_Prefix("c_");

		if(-1 == CurTexName.find_first_of(W_Prefix) && -1 == CurTexName.find_first_of(C_Prefix))
		{
			DBO_TRACE(FALSE, "Decal textures should've got prefix; w_, c_"); 
		}
		else
		{
			RwInt32 IdxWPrefix = CurTexName.find_first_of(W_Prefix);
			RwInt32 IdxCPrefix = CurTexName.find_first_of(C_Prefix);

			if(!IdxWPrefix)
			{
				Param.rwWrapType = rwTEXTUREADDRESSWRAP;
			}
			else if(!IdxCPrefix)
			{
				Param.rwWrapType = rwTEXTUREADDRESSCLAMP;
			}
			else
			{
				DBO_TRACE(FALSE, "Decal textures should've got prefix; w_, c_"); 
			}
		}

		if(_DecalPropParam._DecalMode == 0)
		{
			Param.nFlagEffect = E_FLAG_DECAL_RENDER_WORLD;
		}
		else if(_DecalPropParam._DecalMode == 1)
		{
			Param.nFlagEffect = E_FLAG_DECAL_RENDER_OBJECT;
		}
		else if(_DecalPropParam._DecalMode == 2)
		{
			Param.nFlagEffect = E_FLAG_DECAL_RENDER_WORLD | E_FLAG_DECAL_RENDER_OBJECT;
		}
		else if(_DecalPropParam._DecalMode == 3)
		{
			Param.nFlagEffect = E_FLAG_DECAL_RENDER_OBJECT | E_FLAG_DECAL_WATER;
		}

		CNtlPLEntity* pNewEntity = GetSceneManager()->CreateEntity(PLENTITY_DECAL, "NULL", &Param);
		DBO_ASSERT(pNewEntity, "Entity create failed.");

		m_pRectInDetailBrushSelecter->SetUP(static_cast<VOID*>(pNewEntity));
		pMgr->m_pSectors[pMgr->GetSectorIdx(PosCur)].InsertIn2World(pNewEntity, 4);

		CNtlPLWorldDecal* pNtlPLWorldDecal = static_cast<CNtlPLWorldDecal*>(pNewEntity);
		pNtlPLWorldDecal->SetRotate(_DecalPropParam._TexAngle);
		pNtlPLWorldDecal->SetColor(_DecalPropParam._RGBA.red, _DecalPropParam._RGBA.green, _DecalPropParam._RGBA.blue);
		pNtlPLWorldDecal->SetAlpha(_DecalPropParam._RGBA.alpha);
		pNtlPLWorldDecal->SetAlphaBlend(TRUE, rwBLENDSRCALPHA, rwBLENDINVSRCALPHA);
		pNtlPLWorldDecal->SetPosition(&PosCur);
		pNtlPLWorldDecal->SetName(Param.pTexName);

		if(_DecalPropParam._UVAniCheck)
		{
			if(_DecalPropParam._UVAniPatternIdx == 0)
			{
				SNtlPrtStdEmitterPrtUVAnim EmitterUVAnim;
				EmitterUVAnim.vDir.x		= 0.0f;
				EmitterUVAnim.vDir.y		= 0.0f;
				EmitterUVAnim.vDir.z		= 1.0f;
				EmitterUVAnim.fVelocity		= _DecalPropParam._UVAniSpeed;
				EmitterUVAnim.fMaxOffset	= 0.0f;
				pNtlPLWorldDecal->SetUVAnimAction(EmitterUVAnim);
			}
			else if(_DecalPropParam._UVAniPatternIdx == 1)
			{
				SNtlPrtStdEmitterPrtUVAnim EmitterUVAnim;
				EmitterUVAnim.vDir.x		= 0.0f;
				EmitterUVAnim.vDir.y		= 0.0f;
				EmitterUVAnim.vDir.z		= 1.0f;
				EmitterUVAnim.fVelocity		= _DecalPropParam._UVAniSpeed;
				EmitterUVAnim.fMaxOffset	= _DecalPropParam._UVAniOffset;
				pNtlPLWorldDecal->SetUVAnimAction(EmitterUVAnim);
			}
			else if(_DecalPropParam._UVAniPatternIdx == 2)
			{
				pNtlPLWorldDecal->Set2DRoateAction(_DecalPropParam._UVAniOffset, _DecalPropParam._UVAniSpeed);
			}
			else if(_DecalPropParam._UVAniPatternIdx == 3)
			{
				pNtlPLWorldDecal->SetSizeAction(_DecalPropParam._TexScale, _DecalPropParam._UVAniOffset, _DecalPropParam._UVAniSpeed);
			}
		}

		// retain the selected rectangles
		CNtlWB_RectangleInDetail<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_RectangleInDetail<CNtlWorldFieldManager>*>(m_pRectInDetailBrushSelecter->GetController());

		RwV3d SPos, EPos;
		SPos.x = PosCur.x + _DecalPropParam._SizeX / 2.0f;
		SPos.z = PosCur.z + _DecalPropParam._SizeZ / 2.0f;
		EPos.x = PosCur.x - _DecalPropParam._SizeX / 2.0f;
		EPos.z = PosCur.z - _DecalPropParam._SizeZ / 2.0f;
		pCurBrush->SetPosS(SPos);
		pCurBrush->SetPosE(EPos);

		// retain the current picking mode
		m_Mode = eDEM_EDIT_MODE;

		return;
	}
}

VOID CDecalIOHandler::Delete()
{
	if(m_pNtlPLEntity)
	{
		GetSceneManager()->DeleteEntity(m_pNtlPLEntity);
		m_pNtlPLEntity = NULL;
	}
	
	NTL_DELETE(m_pRectInDetailBrush);
	NTL_DELETE(m_pRectInDetailBrushSelecter);

	m_Mode = eDEM_IDLE_MODE;
}

VOID CDecalIOHandler::Update()
{
	switch(m_Mode)
	{
	case eDEM_IDLE_MODE:	On_DEM_IDLE();	break;
	case eDEM_EDIT_MODE:	On_DEM_EDIT();	break;
	}
}

VOID CDecalIOHandler::RenderText(RtCharset* _pCharset, RwInt32 LenWindowWidth, RwInt32 LenWindowHeight)
{
	if(m_pRectInDetailBrushSelecter)
	{
		CNtlPLEntity*	pCurEntity	= static_cast<CNtlPLEntity*>(m_pRectInDetailBrushSelecter->GetUP());
		RwV3d			Pos			= pCurEntity->GetPosition();
		RwChar			caption[128];

		RwV3d v2dPoint;
		RwMatrix *pMatView = RwCameraGetViewMatrix(CNtlPLGlobal::m_RwCamera);
		RwV3dTransformPoints(&v2dPoint, &Pos, 1, pMatView);
		v2dPoint.x = (v2dPoint.x / v2dPoint.z) * LenWindowWidth + 10.0f;
		v2dPoint.y = (v2dPoint.y / v2dPoint.z) * LenWindowHeight + 10.0f;

		RwRGBA ClrFont;
		ClrFont.red		= 255;
		ClrFont.green	= 255;
		ClrFont.blue	= 255;
		RtCharsetSetColors(_pCharset, &ClrFont, &dGETAPP().m_BackgroundColor);
		RsSprintf(caption, RWSTRING("[ %s ]"), pCurEntity->GetName());
		RsCharsetPrint(_pCharset, caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y);
	}
}

VOID CDecalIOHandler::Render()
{
	if(m_pRectInDetailBrush)
	{
		m_pRectInDetailBrush->Render();
	}

	if(m_pRectInDetailBrushSelecter)
	{
		m_pRectInDetailBrushSelecter->Render();
	}
}
#include "stdafx.h"

#include "NtlWE.h"
#include "NtlWEDoc.h"

#include "NtlMath.h"
#include "NtlCamera.h"

#include "NtlWorldPlugin.h"
#include "NtlWorldCommon.h"
#include "NtlWorldInterface.h"
#include "NtlWorldMaterialPlugin.h"

#include "NtlWorldFieldManager.h"
#include "NtlWorldFieldManager4RwWorld.h"
#include "NtlWorldBlockManager.h"

#include "NtlPLRenderState.h"

#include "NtlPLRenderGeometry.h"

#include "NtlPLWorldEntity.h"
#include "NtlPLHeatHaze.h"

#include "NtlPostEffectCamera.h"

#include "NtlWeControlUi.h"

#include "ProgressWnd.h"

#include "GamePropVisibleDlg.h"

#include "WorldViewDlg.h"

IMPLEMENT_DYNAMIC(CWorldViewDlg, CDialog)

CWorldViewDlg::CWorldViewDlg(CWnd* pParent /*=NULL*/)
: CDialog(CWorldViewDlg::IDD, pParent)
,m_pRpWorld(NULL)
,m_pRwCamera(NULL)
,m_pRpLight(NULL)
,m_pRwTexCamera(NULL)
,m_pRwTexWorldMap(NULL)
,m_pRwTexWorldTile(NULL)
,m_pRwTexWorldTileSelect(NULL)
,m_pRwTexWorldLoading(NULL)
,m_fCameraScale(1.0f)
,m_pGamePropVisibleDlg(NULL)
{
	m_vCameraPos.x = m_vCameraPos.y = m_vCameraPos.z = 0.0f;
}

CWorldViewDlg::~CWorldViewDlg()
{
	DestoryWorldViewResource();
}

void CWorldViewDlg::InitializeWorldData()
{
	CreateWorldViewResource();
}

BOOL CWorldViewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_CBBoxSelect.SetCurSel(2);

	UpdateData(FALSE);

	return TRUE;
}

void CWorldViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_FIELD, m_BtnField);
	DDX_Control(pDX, IDC_CHECK_SECTOR, m_BtnSector);
	DDX_Control(pDX, IDC_CHECK_BLOCK, m_BtnBlock);
	DDX_Control(pDX, IDC_COMBO_SELECT, m_CBBoxSelect);
	DDX_Control(pDX, IDC_CHECK_ACTIVE, m_BtnActive);
	DDX_Control(pDX, IDC_CHECK_PROP, m_BtnPropVisible);
}

BOOL CWorldViewDlg::PreTranslateMessage(MSG* pMsg)
{
	// prevent keys; escape, return
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_SPACE:
			{
				ResetSelectBlock();
				return true;
			}
		case VK_ESCAPE:
			{
				return true;
			}
		case VK_RETURN:
			{
				return true;
			}
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CWorldViewDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		{
			ResetSelectBlock();

			m_BtnActive.SetCheck(FALSE);

			UpdateData(FALSE);
			
			break;
		}

	case WM_SHOWWINDOW:
		{
			break;
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

BEGIN_MESSAGE_MAP(CWorldViewDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_LOAD_WORLD_MAP, &CWorldViewDlg::OnBnClickedBtnLoadWorldMap)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_KILLFOCUS()
	ON_WM_ACTIVATE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDC_BTN_SABER_CAMERA, &CWorldViewDlg::OnBnClickedBtnSaberCamera)
	ON_CBN_SELCHANGE(IDC_COMBO_SELECT, &CWorldViewDlg::OnCbnSelchangeComboSelect)
	ON_BN_CLICKED(IDC_CHECK_PROP, &CWorldViewDlg::OnBnClickedCheckProp)
END_MESSAGE_MAP()

void CWorldViewDlg::CreateWorldViewResource()
{
	if (dGET_WORLD_PARAM()->WorldSizeHalf == 0 && m_pRpWorld)
	{
		return;
	}

	CRect rtView;
	GetDlgItem(IDC_VIEW)->GetClientRect(&rtView);

	RwBBox BBox;
	BBox.inf.x = BBox.inf.y = BBox.inf.z = -(static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSize) * 0.5f);
	BBox.sup.x = BBox.sup.y = BBox.sup.z =  (static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSize) * 0.5f);

	m_pRpWorld = RpWorldCreate(&BBox);
	DBO_ASSERT(m_pRpWorld, "World create failed.");
	if (m_pRpWorld)
	{
		m_pRwCamera	= RwCameraCreate();
		if (m_pRwCamera)
		{
			RwCameraSetFrame(m_pRwCamera, RwFrameCreate());
			RwCameraSetRaster(m_pRwCamera, RwRasterCreate(rtView.Width(), rtView.Height(), 0, rwRASTERTYPECAMERA));
			RwCameraSetZRaster(m_pRwCamera, RwRasterCreate(rtView.Width(), rtView.Height(), 0, rwRASTERTYPEZBUFFER));

			RwCameraSetProjection(m_pRwCamera, rwPERSPECTIVE);

			RwCameraSetFarClipPlane(m_pRwCamera, static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSize));
			RwCameraSetNearClipPlane(m_pRwCamera, 0.001f);

			RwV2d vw;
			vw.x = 0.5f;//static_cast<RwReal>(rtView.Width()) * 0.5f / (static_cast<RwReal>(rtView.Width()) + static_cast<RwReal>(rtView.Height()));
			vw.y = 0.5f;//static_cast<RwReal>(rtView.Height()) * 0.5f / (static_cast<RwReal>(rtView.Width()) + static_cast<RwReal>(rtView.Height())); 
			RwCameraSetViewWindow(m_pRwCamera, &vw);

			RwD3D9CameraAttachWindow(m_pRwCamera, GetDlgItem(IDC_VIEW)->GetSafeHwnd());

			RpWorldAddCamera(m_pRpWorld, m_pRwCamera);
		}
		m_pRpLight = RpLightCreate(rpLIGHTAMBIENT);
		RpWorldAddLight(m_pRpWorld, m_pRpLight);

		// 3+--------------+1
		//  |			   |
		//  |			   |
		//  |			   |
		//  |			   |
		//  |              |
		// 2+--------------+0

		m_bboxWorld = BBox;


		m_pRwTexCamera = CNtlPLResourceManager::GetInstance()->LoadTexture("CameraField", "\\Tool\\SABER\\Texture\\UI\\");
		DBO_ASSERT(m_pRwTexCamera, "Texture load failed.");

		RwTextureSetFilterMode(m_pRwTexCamera, rwFILTERLINEARMIPLINEAR);

		m_pRwTexWorldTile = CNtlPLResourceManager::GetInstance()->LoadTexture("CameraField", "\\Tool\\SABER\\Texture\\UI\\");
		DBO_ASSERT(m_pRwTexWorldTile, "Texture load failed.");

		RwTextureSetFilterMode(m_pRwTexWorldTile, rwFILTERLINEARMIPLINEAR);

		m_pRwTexWorldTileSelect = CNtlPLResourceManager::GetInstance()->LoadTexture("CameraField", "\\Tool\\SABER\\Texture\\UI\\");
		DBO_ASSERT(m_pRwTexWorldTileSelect, "Texture load failed.");
		
		RwTextureSetFilterMode(m_pRwTexWorldTileSelect, rwFILTERLINEARMIPLINEAR);
		
		m_pRwTexWorldLoading = CNtlPLResourceManager::GetInstance()->LoadTexture("CameraField", "\\Tool\\SABER\\Texture\\UI\\");
		DBO_ASSERT(m_pRwTexWorldLoading, "Texture load failed.");
		
		RwTextureSetFilterMode(m_pRwTexWorldLoading, rwFILTERLINEARMIPLINEAR);

		RwMatrixSetIdentity(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)));
		RwMatrixGetRight(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->x = 1.0f;
		RwMatrixGetRight(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->y = 0.0f;
		RwMatrixGetRight(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->z = 0.0f;
		RwMatrixGetUp(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->x = 0.0f;
		RwMatrixGetUp(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->y = 0.0f;
		RwMatrixGetUp(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->z = 1.0f;
		RwMatrixGetAt(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->x = 0.0f;
		RwMatrixGetAt(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->y = -1.0f;
		RwMatrixGetAt(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->z = 0.0f;
		RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->x = m_vCameraPos.x;
		RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->y = m_vCameraPos.y = (static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSize) - m_fCameraScale);
		RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->z = m_vCameraPos.z;
		RwMatrixUpdate(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)));
		RwFrameUpdateObjects(RwCameraGetFrame(m_pRwCamera));
	}

	if (!m_pGamePropVisibleDlg)
	{
		m_pGamePropVisibleDlg = new CGamePropVisibleDlg;
		m_pGamePropVisibleDlg->Create(CGamePropVisibleDlg::IDD, GetDesktopWindow());
	}

	OnSize(0, 512, 512);
}

void CWorldViewDlg::DestoryWorldViewResource()
{
	if (m_pGamePropVisibleDlg)
	{
		m_pGamePropVisibleDlg->DestroyWindow();
		delete m_pGamePropVisibleDlg;
		m_pGamePropVisibleDlg = NULL;
	}
	if (m_pRwTexCamera)
	{
		CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_pRwTexCamera);
		m_pRwTexCamera = NULL;
	}
	if (m_pRwTexWorldMap)
	{
		CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_pRwTexWorldMap);
		m_pRwTexWorldMap = NULL;
	}
	if (m_pRwTexWorldTile)
	{
		CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_pRwTexWorldTile);
		m_pRwTexWorldTile = NULL;
	}
	if (m_pRwTexWorldTileSelect)
	{
		CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_pRwTexWorldTileSelect);
		m_pRwTexWorldTileSelect = NULL;
	}
	if (m_pRwTexWorldLoading)
	{
		CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_pRwTexWorldLoading);
		m_pRwTexWorldLoading = NULL;
	}


	if (m_pRpWorld)
	{
		if (m_pRpLight)
		{
			RpWorldRemoveLight(m_pRpWorld, m_pRpLight);
			RpLightDestroy(m_pRpLight);
			m_pRpLight = NULL;
		}

		if (m_pRwCamera)
		{
			RpWorldRemoveCamera(m_pRpWorld, m_pRwCamera);

			RwRaster*	pRwRasterZ	= RwCameraGetZRaster(m_pRwCamera); RwCameraSetZRaster(m_pRwCamera, NULL);
			RwRaster*	pRwRaster	= RwCameraGetRaster(m_pRwCamera); RwCameraSetRaster(m_pRwCamera, NULL);
			RwFrame*	pRwFrame	= RwCameraGetFrame(m_pRwCamera); RwCameraSetFrame(m_pRwCamera, NULL);

			if (pRwRasterZ)
			{
				RwRasterDestroy(pRwRasterZ);
			}

			if (pRwRaster)
			{
				RwRasterDestroy(pRwRaster);
			}

			if (pRwFrame)
			{
				RwFrameDestroy(pRwFrame);
			}

			RwCameraDestroy(m_pRwCamera);
			m_pRwCamera = NULL;
		}

		RpWorldDestroy(m_pRpWorld);
		m_pRpWorld = NULL;
	}
}

void CWorldViewDlg::Render()
{
	if (IsWindowVisible() && m_pRpWorld && m_pRwCamera)
	{
		RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->x = m_vCameraPos.x;
		RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->y = m_vCameraPos.y = (static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSize) - m_fCameraScale);
		RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->z = m_vCameraPos.z;
		RwMatrixUpdate(RwFrameGetLTM(RwCameraGetFrame(m_pRwCamera)));
		RwFrameUpdateObjects(RwCameraGetFrame(m_pRwCamera));

		static RwRGBA rgbaBackGround = { 150, 150, 150, 255 };
		if (RwCameraClear(m_pRwCamera, &rgbaBackGround, rwCAMERACLEARZ | rwCAMERACLEARIMAGE))
		{
			if (RwCameraBeginUpdate(m_pRwCamera))
			{
				RwReal fWrapU, fWrapV;

				BeginFogState(FALSE);

				RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)FALSE);
				RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);

				RenderRwBBox(m_pRwTexWorldMap, &m_bboxWorld, 1.0f, 1.0f, 255, 255, 255, 255);

				RwInt32 iSup = RwInt32MINVAL;
				RwInt32 iInf = RwInt32MAXVAL;
				for (int i = 0; i < 36; ++i)
				{
					if (GetSceneManager()->GetWorld()->GetWorldFieldMgr()->m_Fields6x6[1][i] == -1)
					{
						continue;
					}
					if (GetSceneManager()->GetWorld()->GetWorldFieldMgr()->m_Fields6x6[1][i] > iSup)
					{
						iSup = GetSceneManager()->GetWorld()->GetWorldFieldMgr()->m_Fields6x6[0][i];
					}
					if (GetSceneManager()->GetWorld()->GetWorldFieldMgr()->m_Fields6x6[1][i] < iInf)
					{
						iInf = GetSceneManager()->GetWorld()->GetWorldFieldMgr()->m_Fields6x6[0][i];
					}
				}

				RwBBox bboxLoadedFields;
				bboxLoadedFields.inf.x = static_cast<RwReal>((iInf % dGET_WORLD_PARAM()->WorldFieldNum) * dGET_WORLD_PARAM()->WorldFieldSize) - dGET_WORLD_PARAM()->WorldSizeHalf;
				bboxLoadedFields.inf.z = static_cast<RwReal>((iInf / dGET_WORLD_PARAM()->WorldFieldNum) * dGET_WORLD_PARAM()->WorldFieldSize) - dGET_WORLD_PARAM()->WorldSizeHalf;
				bboxLoadedFields.sup.x = static_cast<RwReal>((iSup % dGET_WORLD_PARAM()->WorldFieldNum + 1) * dGET_WORLD_PARAM()->WorldFieldSize) - dGET_WORLD_PARAM()->WorldSizeHalf;
				bboxLoadedFields.sup.z = static_cast<RwReal>((iSup / dGET_WORLD_PARAM()->WorldFieldNum + 1) * dGET_WORLD_PARAM()->WorldFieldSize) - dGET_WORLD_PARAM()->WorldSizeHalf;

				CLAMP(bboxLoadedFields.inf.x, m_bboxWorld.inf.x, m_bboxWorld.sup.x);
				CLAMP(bboxLoadedFields.inf.z, m_bboxWorld.inf.z, m_bboxWorld.sup.z);
				CLAMP(bboxLoadedFields.sup.x, m_bboxWorld.inf.x, m_bboxWorld.sup.x);
				CLAMP(bboxLoadedFields.sup.z, m_bboxWorld.inf.z, m_bboxWorld.sup.z);

				fWrapU = (bboxLoadedFields.sup.x - bboxLoadedFields.inf.x) / static_cast<RwReal>(dGET_WORLD_PARAM()->WorldFieldSize);
				fWrapV = (bboxLoadedFields.sup.z - bboxLoadedFields.inf.z) / static_cast<RwReal>(dGET_WORLD_PARAM()->WorldFieldSize);
				RenderRwBBox(m_pRwTexWorldLoading, &bboxLoadedFields, 1.0f/*fWrapU*/, 1.0f/*fWrapV*/, 255, 0, 0, 255);

				RwInt32 iBlockSize = GetBlockSize();			
				if (iBlockSize && !IsSelectEmpty())
				{
					fWrapU = (m_bboxSelectBlock.sup.x - m_bboxSelectBlock.inf.x) / static_cast<RwReal>(iBlockSize);
					fWrapV = (m_bboxSelectBlock.sup.z - m_bboxSelectBlock.inf.z) / static_cast<RwReal>(iBlockSize);
					RenderRwBBox(m_pRwTexWorldTileSelect, &m_bboxSelectBlock, fWrapU, fWrapV, 255, 255, 255, 255);
				}

				if (m_BtnBlock.GetCheck())
				{
					fWrapV = fWrapU = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSize) / static_cast<RwReal>(dGET_WORLD_PARAM()->WorldBlockSize);
					RenderRwBBox(m_pRwTexWorldTile, &m_bboxWorld, fWrapU, fWrapV, 255, 104, 104, 104);
				}

				if (m_BtnSector.GetCheck())
				{
					fWrapV = fWrapU = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSize) / static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize);
					RenderRwBBox(m_pRwTexWorldTile, &m_bboxWorld, fWrapU, fWrapV, 255, 0, 255, 0);
				}

				if (m_BtnField.GetCheck())
				{
					fWrapV = fWrapU = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSize) / static_cast<RwReal>(dGET_WORLD_PARAM()->WorldFieldSize);
					RenderRwBBox(m_pRwTexWorldTile, &m_bboxWorld, fWrapV, fWrapU, 255, 255, 0, 0);
				}

				if (m_BtnPropVisible.GetCheck())
				{
					RenderVisibleProp(m_pGamePropVisibleDlg->GetFlags(), &bboxLoadedFields, static_cast<RwReal>(dGET_WORLD_PARAM()->WorldBlockSize));
				}

				RwBBox bboxCamera;
				bboxCamera.sup = bboxCamera.inf = *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
				bboxCamera.inf.x -= 32.0f; bboxCamera.inf.y = 0.0f; bboxCamera.inf.z -= 32.0f;
				bboxCamera.sup.x += 32.0f; bboxCamera.sup.y = 0.0f; bboxCamera.sup.z += 32.0f;
				RenderRwBBox(m_pRwTexCamera, &bboxCamera, 1.0f, 1.0f, 255, 255, 255, 255);

				RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
				RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);

				EndFogState();

				RwCameraEndUpdate(m_pRwCamera);
			}
			RwCameraShowRaster(m_pRwCamera, GetDlgItem(IDC_VIEW)->GetSafeHwnd(), NULL);
		}
	}
}

void CWorldViewDlg::UpdateMainView()
{	
	if (!m_BtnActive.GetCheck() || GetBlockMode() == EBLOCK_MODE_NONE)
	{
		return;
	}

	if (dGET_MOUSE()->GetLBOnce())
	{
		RwReal	fDist	= RwRealMAXVAL;
		RwBBox	bboxSelect;
		RwBool	bPicked	= FALSE;

		static RwImVertexIndex indices[36] =
		{
			0, 2, 1, 1, 2, 3, 4, 6, 0, 0, 6, 2, 5, 7, 4, 4, 7, 6,
			1, 3, 5, 5, 3, 7, 4, 0, 5, 5, 0, 1, 2, 6, 3, 3, 6, 7
		};

		RwV2d ptPickScreen;
		ptPickScreen.x = (RwReal)dGET_MOUSE()->GetPosClicked().x;
		ptPickScreen.y = (RwReal)dGET_MOUSE()->GetPosClicked().y;

		RwLine rayCamera;
		RwCameraCalcPixelRay(CNtlPLGlobal::m_RwCamera, &rayCamera, &ptPickScreen);

		RwV3d vDirRay, vRayDeltha;
		RwV3dSubMacro(&vRayDeltha, &rayCamera.end, &rayCamera.start);
		RwV3dNormalize(&vDirRay, &vRayDeltha);

		RwReal							fCheckSize	= static_cast<RwReal>(GetBlockSize());
		CNtlWorldFieldManager4RWWorld*	pFieldMgr	= (CNtlWorldFieldManager4RWWorld*)dGETMGR();
		RwBBox							bbox		= RpNtlCameraFrustumGetStandardBBox(CNtlPLGlobal::m_RwCamera, static_cast<RwInt32>(fCheckSize));

		for (RwReal sz = bbox.inf.z; sz < bbox.sup.z; sz += fCheckSize)
		{
			for (RwReal sx = bbox.inf.x; sx < bbox.sup.x; sx += fCheckSize)
			{
				RwBBox	bboxCheck;
				RwInt32 iTemp = 0;
				RwV3d	avTemp[32];

				for (RwReal z = sz; z < sz + fCheckSize; z += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldBlockSize))
				{
					for (RwReal x = sx; x < sx + fCheckSize; x += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldBlockSize))
					{
						RwV3d vPos;
						vPos.x = x;
						vPos.z = z;

						sNTL_BLOCK_PROP sNtlBlockProp;
						if (pFieldMgr->GetWorldBlockManager()->GetABlockProp(vPos, sNtlBlockProp))
						{
							if (sNtlBlockProp._BBox.sup.y == RwRealMINVAL ||
								sNtlBlockProp._BBox.inf.y == RwRealMAXVAL)
							{
								continue;
							}

							avTemp[iTemp++] = sNtlBlockProp._BBox.inf;
							avTemp[iTemp++] = sNtlBlockProp._BBox.sup;
						}
					}
				}

				if (iTemp != 0)
				{
					RwBBoxCalculate(&bboxCheck, avTemp, iTemp);

					bboxCheck.inf.x = sx;
					bboxCheck.inf.z = sz;
					bboxCheck.sup.x = sx + fCheckSize;
					bboxCheck.sup.z = sz + fCheckSize;

					RwV3d avVertices[8];
					for (RwInt32 i = 0; i < 8; ++i)
					{
						avVertices[i].x = (i & 1 ? bboxCheck.sup.x : bboxCheck.inf.x);
						avVertices[i].y = (i & 2 ? bboxCheck.sup.y : bboxCheck.inf.y);
						avVertices[i].z = (i & 4 ? bboxCheck.sup.z : bboxCheck.inf.z);
					}	

					RwReal fTemp	= 0.0f;
					for (int i = 0; i < 36; i += 3)
					{
						if (RtIntersectionLineTriangle(&rayCamera.start, &vRayDeltha, &avVertices[indices[i]], &avVertices[indices[i + 1]], &avVertices[indices[i + 2]], &fTemp))
						{
							if (fTemp < fDist)
							{
								fDist		= fTemp;
								bboxSelect	= bboxCheck;
								bPicked		= TRUE;
							}
						}
					}
				}
			}
		}

		if (bPicked)
		{
			m_bboxSelectBlock.inf	= bboxSelect.inf;
			m_bboxSelectBlock.inf.y = 0.0f;
			m_bboxSelectBlock.sup	= bboxSelect.sup;
			m_bboxSelectBlock.sup.y = 0.0f;
		}
	}

	if(dGET_MOUSE()->GetBehavior(MB_LBDN) && dGET_MOUSE()->GetBehavior(MB_MOVE))
	{
		RwReal	fDist	= RwRealMAXVAL;
		RwBBox	bboxSelect;
		RwBool	bPicked	= FALSE;

		static RwImVertexIndex indices[36] =
		{
			0, 2, 1, 1, 2, 3, 4, 6, 0, 0, 6, 2, 5, 7, 4, 4, 7, 6,
			1, 3, 5, 5, 3, 7, 4, 0, 5, 5, 0, 1, 2, 6, 3, 3, 6, 7
		};

		RwV2d ptPickScreen;
		ptPickScreen.x = (RwReal)dGET_MOUSE()->GetCusPos().x;
		ptPickScreen.y = (RwReal)dGET_MOUSE()->GetCusPos().y;

		RwLine rayCamera;
		RwCameraCalcPixelRay(CNtlPLGlobal::m_RwCamera, &rayCamera, &ptPickScreen);

		RwV3d vDirRay, vRayDeltha;
		RwV3dSubMacro(&vDirRay, &rayCamera.end, &rayCamera.start);
		RwV3dSubMacro(&vRayDeltha, &dGET_COLLISION_INFO()->RayIntersectionPt4Terrain, &rayCamera.start);

		RwV3dNormalize(&vDirRay, &vDirRay);

		RwReal							fCheckSize	= static_cast<RwReal>(GetBlockSize());
		CNtlWorldFieldManager4RWWorld*	pFieldMgr	= (CNtlWorldFieldManager4RWWorld*)dGETMGR();
		RwBBox							bbox		= RpNtlCameraFrustumGetStandardBBox(CNtlPLGlobal::m_RwCamera, static_cast<RwInt32>(fCheckSize));

		for (RwReal sz = bbox.inf.z; sz < bbox.sup.z; sz += fCheckSize)
		{
			for (RwReal sx = bbox.inf.x; sx < bbox.sup.x; sx += fCheckSize)
			{
				RwBBox	bboxCheck;
				RwInt32 iTemp = 0;
				RwV3d	avTemp[32];

				for (RwReal z = sz; z < sz + fCheckSize; z += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldBlockSize))
				{
					for (RwReal x = sx; x < sx + fCheckSize; x += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldBlockSize))
					{
						RwV3d vPos;
						vPos.x = x;
						vPos.z = z;

						sNTL_BLOCK_PROP sNtlBlockProp;
						if (pFieldMgr->GetWorldBlockManager()->GetABlockProp(vPos, sNtlBlockProp))
						{
							if (sNtlBlockProp._BBox.sup.y == RwRealMINVAL ||
								sNtlBlockProp._BBox.inf.y == RwRealMAXVAL)
							{
								continue;
							}

							avTemp[iTemp++] = sNtlBlockProp._BBox.inf;
							avTemp[iTemp++] = sNtlBlockProp._BBox.sup;
						}
					}
				}

				if (iTemp != 0)
				{
					RwBBoxCalculate(&bboxCheck, avTemp, iTemp);

					bboxCheck.inf.x = sx;
					bboxCheck.inf.z = sz;
					bboxCheck.sup.x = sx + fCheckSize;
					bboxCheck.sup.z = sz + fCheckSize;

					RwV3d avVertices[8];
					for (RwInt32 i = 0; i < 8; ++i)
					{
						avVertices[i].x = (i & 1 ? bboxCheck.sup.x : bboxCheck.inf.x);
						avVertices[i].y = (i & 2 ? bboxCheck.sup.y : bboxCheck.inf.y);
						avVertices[i].z = (i & 4 ? bboxCheck.sup.z : bboxCheck.inf.z);
					}	

					RwReal fTemp	= 0.0f;
					for (int i = 0; i < 36; i += 3)
					{
						if (RtIntersectionLineTriangle(&rayCamera.start, &vRayDeltha, &avVertices[indices[i]], &avVertices[indices[i + 1]], &avVertices[indices[i + 2]], &fTemp))
						{
							if (fTemp < fDist)
							{
								fDist		= fTemp;
								bboxSelect	= bboxCheck;
								bPicked		= TRUE;
							}
						}
					}
				}
			}
		}

		if (bPicked)
		{
			RwBBoxAddPoint(&m_bboxSelectBlock, &bboxSelect.inf);
			RwBBoxAddPoint(&m_bboxSelectBlock, &bboxSelect.sup);
			m_bboxSelectBlock.inf.y = 0.0f;
			m_bboxSelectBlock.sup.y = 0.0f;
		}
	}

	if (dOKEY(DIK_SPACE))
	{
		ResetSelectBlock();
	}

	if (m_bboxSelectBlock.inf.x != m_bboxSelectBlock.sup.x &&
		m_bboxSelectBlock.inf.z != m_bboxSelectBlock.sup.z)
	{		
		if (dOKEY(DIK_LEFT))
		{
			if (dKEY(DIK_LALT))
			{
				m_bboxSelectBlock.inf.x += GetBlockSize();
			}
			else if (dKEY(DIK_LCONTROL))
			{
				m_bboxSelectBlock.sup.x += GetBlockSize();
			}
			else
			{
				m_bboxSelectBlock.inf.x += GetBlockSize();
				m_bboxSelectBlock.sup.x += GetBlockSize();
			}
		}
		if (dOKEY(DIK_RIGHT))
		{
			if (dKEY(DIK_LALT))
			{
				m_bboxSelectBlock.sup.x -= GetBlockSize();
			}
			else if (dKEY(DIK_LCONTROL))
			{
				m_bboxSelectBlock.inf.x -= GetBlockSize();
			}		
			else
			{
				m_bboxSelectBlock.inf.x -= GetBlockSize();
				m_bboxSelectBlock.sup.x -= GetBlockSize();
			}
		}
		if (dOKEY(DIK_UP))
		{
			if (dKEY(DIK_LALT))
			{
				m_bboxSelectBlock.inf.z += GetBlockSize();
			}
			else if (dKEY(DIK_LCONTROL))
			{
				m_bboxSelectBlock.sup.z += GetBlockSize();
			}
			else
			{
				m_bboxSelectBlock.inf.z += GetBlockSize();
				m_bboxSelectBlock.sup.z += GetBlockSize();
			}
		}
		if (dOKEY(DIK_DOWN))
		{
			if (dKEY(DIK_LALT))
			{
				m_bboxSelectBlock.sup.z -= GetBlockSize();
			}
			else if (dKEY(DIK_LCONTROL))
			{
				m_bboxSelectBlock.inf.z -= GetBlockSize();
			}
			else
			{
				m_bboxSelectBlock.inf.z -= GetBlockSize();
				m_bboxSelectBlock.sup.z -= GetBlockSize();
			}
		}
	}	
}

void CWorldViewDlg::RenderMainView()
{
	RwCamera* pCamera = RwCameraGetCurrentCamera();
	if (pCamera)
	{
		CNtlWorldFieldManager4RWWorld* pFieldMgr = (CNtlWorldFieldManager4RWWorld*)dGETMGR();

		BeginFogState(FALSE);

		RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
		RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);

		RenderMainViewBBox(static_cast<RwReal>(dGET_WORLD_PARAM()->WorldBlockSize), pCamera, m_BtnBlock.GetCheck(), GetBlockMode() == EBLOCK_MODE_BLOCK, &CWorldViewDlg::RenderBlockBBox);
		RenderMainViewBBox(static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize), pCamera, m_BtnSector.GetCheck(), GetBlockMode() == EBLOCK_MODE_SECTOR, &CWorldViewDlg::RenderSectorBBox);
		RenderMainViewBBox(static_cast<RwReal>(dGET_WORLD_PARAM()->WorldFieldSize), pCamera, m_BtnField.GetCheck(), GetBlockMode() == EBLOCK_MODE_FIELD, &CWorldViewDlg::RenderFieldBBox);

		RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
		RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);

		EndFogState();
	}
}

void CWorldViewDlg::RenderBlockBBox(RwBool bSelected, RwBBox bboxRender, RwUInt32 uiFlags)
{
	static RwImVertexIndex indicesLine[24] =
	{
		0, 1, 1, 3, 3, 2, 2, 0, 4, 5, 5, 7,
		7, 6, 6, 4, 0, 4, 1, 5, 2, 6, 3, 7
	};

	static RwImVertexIndex indicesTriangle[36] =
	{
		2, 6, 3,
		3, 6, 7, // 拉搁
		4, 0, 5, 
		5, 0, 1, // 关搁
 		5, 7, 4,
 		4, 7, 6, 
		0, 2, 1,
		1, 2, 3, 
		4, 6, 0,
		0, 6, 2,
		1, 3, 5,
		5, 3, 7,
	};

	static RwIm3DVertex	im3DVertices[8];
	static RwRGBA		rgbaColor;

	RwIm3DVertex* pVertex = im3DVertices;

	rgbaColor.alpha = 25;
	rgbaColor.red	= 104;
	rgbaColor.green	= 104;
	rgbaColor.blue	= 104;

	RwUInt32	uiResultFlags = uiFlags & m_pGamePropVisibleDlg->GetFlags();
	RwRGBA		rgbaTemp;
	if (m_pGamePropVisibleDlg->GetGamePropColor(uiResultFlags, &rgbaTemp))
	{
		rgbaColor.red	= rgbaTemp.red;
		rgbaColor.green	= rgbaTemp.green;
		rgbaColor.blue	= rgbaTemp.blue;
	}

	for(int i = 0; i < 8; ++i)
	{
		RwIm3DVertexSetPos(pVertex, 
			i & 1 ? bboxRender.sup.x : bboxRender.inf.x,
			i & 2 ? bboxRender.sup.y : bboxRender.inf.y,
			i & 4 ? bboxRender.sup.z : bboxRender.inf.z);

		RwIm3DVertexSetRGBA(pVertex, rgbaColor.red, rgbaColor.green, rgbaColor.blue, rgbaColor.alpha);
		pVertex++;
	}

	RwD3D9SetTexture(NULL, 0);

	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *) TRUE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *) FALSE);
	if(RwIm3DTransform(im3DVertices, 8, NULL, rwIM3D_ALLOPAQUE))
	{
		RwIm3DRenderIndexedPrimitive(rwPRIMTYPELINELIST, indicesLine, 24);

		RwIm3DEnd();
	}
	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *) TRUE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *) TRUE);

	RwInt32 IdxRender = 6;
	if (bSelected)
	{
		IdxRender	= 0;
		pVertex		= im3DVertices;
		rgbaColor.alpha = 85;
		for(int i = 0; i < 8; ++i)
		{
			RwIm3DVertexSetRGBA(pVertex, rgbaColor.red, rgbaColor.green, rgbaColor.blue, rgbaColor.alpha);
			pVertex++;
		}
	}
	RwRenderStateSet(rwRENDERSTATECULLMODE,(void *)rwCULLMODECULLNONE);
	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *) TRUE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *) FALSE);
	if(RwIm3DTransform(im3DVertices, 8, NULL, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA))
	{
		RwIm3DRenderIndexedPrimitive(rwPRIMTYPETRILIST, &indicesTriangle[IdxRender], 36 - IdxRender);
		RwIm3DEnd();
	}
	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *) TRUE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *) TRUE);
	RwRenderStateSet(rwRENDERSTATECULLMODE,(void *)rwCULLMODECULLBACK);
}

void CWorldViewDlg::RenderSectorBBox(RwBool bSelected, RwBBox bboxRender, RwUInt32 uiFlags)
{
	static RwImVertexIndex indicesLine[24] =
	{
		0, 1, 1, 3, 3, 2, 2, 0, 4, 5, 5, 7,
		7, 6, 6, 4, 0, 4, 1, 5, 2, 6, 3, 7
	};

	static RwImVertexIndex indicesTriangle[36] =
	{
		4, 0, 5, 
		5, 0, 1, // 关搁
		2, 6, 3,
		3, 6, 7, // 拉搁
		5, 7, 4,
		4, 7, 6, 
		0, 2, 1,
		1, 2, 3, 
		4, 6, 0,
		0, 6, 2,
		1, 3, 5,
		5, 3, 7,
	};

	static RwIm3DVertex	im3DVertices[8];
	static RwRGBA		rgbaColor;

	RwIm3DVertex* pVertex = im3DVertices;

	rgbaColor.alpha = 25;
	rgbaColor.red	= 0;
	rgbaColor.green	= 255;
	rgbaColor.blue	= 0;

	const RwBBox* pBBoxWorld = RpWorldGetBBox(CNtlPLGlobal::m_pRpWorld);
	for(int i = 0; i < 8; ++i)
	{
		RwIm3DVertexSetPos(pVertex, 
			i & 1 ? bboxRender.sup.x : bboxRender.inf.x,
			i & 2 ? pBBoxWorld->sup.y/*bboxRender.sup.y*/ : pBBoxWorld->inf.y/*bboxRender.inf.y*/,
			i & 4 ? bboxRender.sup.z : bboxRender.inf.z);

		RwIm3DVertexSetRGBA(pVertex, rgbaColor.red, rgbaColor.green, rgbaColor.blue, rgbaColor.alpha);
		pVertex++;
	}

	RwD3D9SetTexture(NULL, 0);

	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *) TRUE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *) FALSE);
	if(RwIm3DTransform(im3DVertices, 8, NULL, rwIM3D_ALLOPAQUE))
	{
		RwIm3DRenderIndexedPrimitive(rwPRIMTYPELINELIST, indicesLine, 24);

		RwIm3DEnd();
	}
	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *) TRUE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *) TRUE);

	RwInt32 IdxRender = 12;
	if (bSelected)
	{
		IdxRender	= 6;
		pVertex		= im3DVertices;
		rgbaColor.alpha = 85;
		for(int i = 0; i < 8; ++i)
		{
			RwIm3DVertexSetRGBA(pVertex, rgbaColor.red, rgbaColor.green, rgbaColor.blue, rgbaColor.alpha);
			pVertex++;
		}
	}
	RwRenderStateSet(rwRENDERSTATECULLMODE,(void *)rwCULLMODECULLNONE);
	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *) TRUE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *) FALSE);
	if(RwIm3DTransform(im3DVertices, 8, NULL, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA))
	{
		RwIm3DRenderIndexedPrimitive(rwPRIMTYPETRILIST, &indicesTriangle[IdxRender], 36 - IdxRender);
		RwIm3DEnd();
	}
	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *) TRUE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *) TRUE);
	RwRenderStateSet(rwRENDERSTATECULLMODE,(void *)rwCULLMODECULLBACK);
}

void CWorldViewDlg::RenderFieldBBox(RwBool bSelected, RwBBox bboxRender, RwUInt32 uiFlags)
{
	static RwImVertexIndex indicesLine[24] =
	{
		0, 1, 1, 3, 3, 2, 2, 0, 4, 5, 5, 7,
		7, 6, 6, 4, 0, 4, 1, 5, 2, 6, 3, 7
	};

	static RwImVertexIndex indicesTriangle[36] =
	{
		4, 0, 5, 
		5, 0, 1, // 关搁
		2, 6, 3,
		3, 6, 7, // 拉搁
		5, 7, 4,
		4, 7, 6, 
		0, 2, 1,
		1, 2, 3, 
		4, 6, 0,
		0, 6, 2,
		1, 3, 5,
		5, 3, 7,
	};

	static RwIm3DVertex	im3DVertices[8];
	static RwRGBA		rgbaColor;

	RwIm3DVertex* pVertex = im3DVertices;

	rgbaColor.alpha = 25;
	rgbaColor.red	= 255;
	rgbaColor.green	= 0;
	rgbaColor.blue	= 0;

	const RwBBox* pBBoxWorld = RpWorldGetBBox(CNtlPLGlobal::m_pRpWorld);
	for(int i = 0; i < 8; ++i)
	{
		RwIm3DVertexSetPos(pVertex, 
			i & 1 ? bboxRender.sup.x : bboxRender.inf.x,
			i & 2 ? pBBoxWorld->sup.y/*bboxRender.sup.y*/ : pBBoxWorld->inf.y/*bboxRender.inf.y*/,
			i & 4 ? bboxRender.sup.z : bboxRender.inf.z);

		RwIm3DVertexSetRGBA(pVertex, rgbaColor.red, rgbaColor.green, rgbaColor.blue, rgbaColor.alpha);
		pVertex++;
	}

	RwD3D9SetTexture(NULL, 0);

	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *) TRUE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *) FALSE);
	if(RwIm3DTransform(im3DVertices, 8, NULL, rwIM3D_ALLOPAQUE))
	{
		RwIm3DRenderIndexedPrimitive(rwPRIMTYPELINELIST, indicesLine, 24);

		RwIm3DEnd();
	}
	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *) TRUE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *) TRUE);

	RwInt32 IdxRender = 12;
	if (bSelected)
	{
		IdxRender	= 6;
		pVertex		= im3DVertices;
		rgbaColor.alpha = 85;
		for(int i = 0; i < 8; ++i)
		{
			RwIm3DVertexSetRGBA(pVertex, rgbaColor.red, rgbaColor.green, rgbaColor.blue, rgbaColor.alpha);
			pVertex++;
		}
	}
	RwRenderStateSet(rwRENDERSTATECULLMODE,(void *)rwCULLMODECULLNONE);
	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *) TRUE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *) FALSE);
	if(RwIm3DTransform(im3DVertices, 8, NULL, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA))
	{
		RwIm3DRenderIndexedPrimitive(rwPRIMTYPETRILIST, &indicesTriangle[IdxRender], 36 - IdxRender);
		RwIm3DEnd();
	}
	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *) TRUE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *) TRUE);
	RwRenderStateSet(rwRENDERSTATECULLMODE,(void *)rwCULLMODECULLBACK);
}

void CWorldViewDlg::RenderMainViewBBox(RwReal fRenderSize, RwCamera* pCamera, RwBool bRender, RwBool bRenderSelected, void (CWorldViewDlg::*CB_RENDER)(RwBool, RwBBox, RwUInt32))
{
	CNtlWorldFieldManager4RWWorld*	pFieldMgr	= (CNtlWorldFieldManager4RWWorld*)dGETMGR();
	RwBBox							bbox		= RpNtlCameraFrustumGetStandardBBox(pCamera, static_cast<RwInt32>(fRenderSize));

	for (RwReal sz = bbox.inf.z; sz < bbox.sup.z; sz += fRenderSize)
	{
		for (RwReal sx = bbox.inf.x; sx < bbox.sup.x; sx += fRenderSize)
		{
			RwBBox	bboxRender;
			RwInt32 iTemp = 0;
			RwV3d	avTemp[32];

			RwUInt32 uiFlags = 0x00000000;
			for (RwReal z = sz; z < sz + fRenderSize; z += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldBlockSize))
			{
				for (RwReal x = sx; x < sx + fRenderSize; x += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldBlockSize))
				{
					RwV3d vPos;
					vPos.x = x;
					vPos.z = z;
					
					sNTL_BLOCK_PROP sNtlBlockProp;
					if (pFieldMgr->GetWorldBlockManager()->GetABlockProp(vPos, sNtlBlockProp))
					{
						if (sNtlBlockProp._BBox.sup.y == RwRealMINVAL ||
							sNtlBlockProp._BBox.inf.y == RwRealMAXVAL)
						{
							continue;
						}

						uiFlags |= sNtlBlockProp._GameProp;

						avTemp[iTemp++] = sNtlBlockProp._BBox.inf;
						avTemp[iTemp++] = sNtlBlockProp._BBox.sup;
					}
				}
			}

			if (iTemp != 0)
			{
				RwBBoxCalculate(&bboxRender, avTemp, iTemp);

				bboxRender.inf.x = sx;
				bboxRender.inf.z = sz;
				bboxRender.sup.x = sx + fRenderSize;
				bboxRender.sup.z = sz + fRenderSize;

				RwSphere sphere;
				sphere.center	= (bboxRender.sup + bboxRender.inf) * 0.5f;
				sphere.radius	= fRenderSize * 0.5f;

				if (RwCameraFrustumTestSphere(pCamera, &sphere))
				{
					if (bRenderSelected &&						
						m_bboxSelectBlock.inf.x != m_bboxSelectBlock.sup.x && m_bboxSelectBlock.inf.z != m_bboxSelectBlock.sup.z &&
						(m_bboxSelectBlock.inf.x <= bboxRender.sup.x - 0.01f) && (m_bboxSelectBlock.inf.z <= bboxRender.sup.z - 0.01f) && 
						(m_bboxSelectBlock.sup.x >= bboxRender.inf.x + 0.01f) && (m_bboxSelectBlock.sup.z >= bboxRender.inf.z + 0.01f))
					{
						(this->*CB_RENDER)(TRUE, bboxRender, uiFlags);
					}
					else if (bRender)
					{
						(this->*CB_RENDER)(FALSE, bboxRender, uiFlags);
					}
				}
			}
		}
	}
}

void CWorldViewDlg::RenderVisibleProp(RwUInt32 uiVisibleProp, RwBBox* pBBox, RwReal fSize)
{
	CNtlWorldFieldManager4RWWorld* pMgr = ((CNtlWorldFieldManager4RWWorld*)dGETMGR());
	for (RwReal z = pBBox->inf.z; z < pBBox->sup.z; z += fSize)
	{
		for (RwReal x = pBBox->inf.x; x < pBBox->sup.x; x += fSize)
		{
			RwV3d vPos;
			vPos.x = x;
			vPos.z = z;

			sNTL_BLOCK_PROP sNtlBlockProp;
			if (pMgr->GetABlockProp(vPos, sNtlBlockProp))
			{
				RwUInt32	uiFlags = sNtlBlockProp._GameProp & uiVisibleProp;
				RwRGBA		rgba;
				if (m_pGamePropVisibleDlg->GetGamePropColor(uiFlags, &rgba))
				{
					RwBBox bboxRender;
					bboxRender.inf.x = x;
					bboxRender.inf.z = z;
					bboxRender.sup.x = x + fSize;
					bboxRender.sup.z = z + fSize;;
					RenderRwBBox(m_pRwTexWorldTileSelect, &bboxRender, 1.0f, 1.0f, rgba.alpha, rgba.red, rgba.green, rgba.blue);
				}					
			}				
		}						
	}
}

void CWorldViewDlg::RenderRwBBox(RwTexture* pRwTexture, RwBBox* pRwBBoxBlock, RwReal fWrapU, RwReal fWrapV, RwUInt8 a, RwUInt8 r, RwUInt8 g, RwUInt8 b, RwReal fHeight)
{
	if (!pRwTexture || !pRwBBoxBlock)
	{
		return;
	}

	if (pRwBBoxBlock->sup.x == pRwBBoxBlock->inf.x &&
		pRwBBoxBlock->sup.z == pRwBBoxBlock->inf.z)
	{
		return;
	}

	SetRwIm3DVertex(&m_aRwIm3DVertex[0], pRwBBoxBlock->inf.x,  fHeight, pRwBBoxBlock->inf.z, 1.0f, 1.0f, a, r, g, b);
	SetRwIm3DVertex(&m_aRwIm3DVertex[1], pRwBBoxBlock->inf.x,  fHeight, pRwBBoxBlock->sup.z, 1.0f, 0.0f, a, r, g, b);
	SetRwIm3DVertex(&m_aRwIm3DVertex[2], pRwBBoxBlock->sup.x,  fHeight, pRwBBoxBlock->inf.z, 0.0f, 1.0f, a, r, g, b);
	SetRwIm3DVertex(&m_aRwIm3DVertex[3], pRwBBoxBlock->sup.x,  fHeight, pRwBBoxBlock->sup.z, 0.0f, 0.0f, a, r, g, b);

	D3DXMATRIXA16 matTemp;
	D3DXMatrixIdentity(&matTemp); 
	matTemp._11 = fWrapU;
	matTemp._22 = fWrapV;

	RwD3D9SetTexture(pRwTexture, 0);
	RwD3D9SetTransform(D3DTS_TEXTURE0, &matTemp);
	RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

	if (RwIm3DTransform(m_aRwIm3DVertex, 4, NULL, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXUV | rwIM3D_VERTEXRGBA))
	{
		RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);        
		RwIm3DEnd(); 
	}

	RwD3D9SetTexture(NULL, 0);
	D3DXMatrixIdentity(&matTemp);
	RwD3D9SetTransform(D3DTS_TEXTURE0, &matTemp);
	RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
}

void CWorldViewDlg::OnBnClickedBtnLoadWorldMap()
{
	CFileDialog	FileDlg(TRUE,
		"register detail texture layer",
		"*.dds; *.bmp; *.tga; *.jpg",
		OFN_HIDEREADONLY | OFN_NOCHANGEDIR,
		"Data Files (*.dds;*.bmp;*.tga;*.jpg)|*.dds;*.bmp;*.tga;*.jpg||");

	FileDlg.m_ofn.lpstrInitialDir = "tool\\saber\\texture\\Grid Map";

	if(FileDlg.DoModal() == IDCANCEL)
	{
		return;		
	}

	CString		strImagePath= FileDlg.GetPathName().Mid(0, FileDlg.GetPathName().GetLength() - FileDlg.GetFileName().GetLength());
	RwTexture*	pRwTexture	= CNtlPLResourceManager::GetInstance()->LoadTexture(FileDlg.GetFileName().GetBuffer(), strImagePath.GetBuffer());
	DBO_ASSERT(pRwTexture, "Texture load failed.");

	if (pRwTexture)
	{
		if (m_pRwTexWorldMap)
		{
			CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_pRwTexWorldMap);
		}
		m_pRwTexWorldMap = pRwTexture;
		RwTextureSetFilterMode(m_pRwTexWorldMap, rwFILTERLINEARMIPLINEAR);

	}
	else
	{
		AfxMessageBox("Load Failed...");
	}
}
void CWorldViewDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CRect rtView;
	GetClientRect(&rtView);

	if (::IsWindow(GetDlgItem(IDC_VIEW)->GetSafeHwnd()) && m_pRpWorld && m_pRwCamera)
	{
		GetDlgItem(IDC_VIEW)->SetWindowPos(NULL, 0, dWORLD_VIEW_TOP_MARGIN, rtView.Width(), rtView.Height() - dWORLD_VIEW_TOP_MARGIN, NULL);	
		GetDlgItem(IDC_VIEW)->GetClientRect(&rtView);

		RwRaster *pRwRaster		= RwCameraGetRaster(m_pRwCamera);
		RwRaster *pRwRasterZ	= RwCameraGetZRaster(m_pRwCamera);
		pRwRasterZ->width		= pRwRaster->width = rtView.Width();
		pRwRasterZ->height		= pRwRaster->height= rtView.Height();

		RwV2d vw;
		vw.x = 0.5f; //0.5f;//static_cast<RwReal>(rtView.Width()) * 0.5f / (static_cast<RwReal>(rtView.Width()) + static_cast<RwReal>(rtView.Height()));
		vw.y = 0.5f; //0.5f;//static_cast<RwReal>(rtView.Height()) * 0.5f / (static_cast<RwReal>(rtView.Width()) + static_cast<RwReal>(rtView.Height())); 
		RwCameraSetViewWindow(m_pRwCamera, &vw);
	}
}

static CWnd* g_pWndMouseCapture = NULL;
void CWorldViewDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	if (!IsMouseOut())
	{
		GetDlgItem(IDC_VIEW)->SetFocus();
		if (CursorIntersectionPlane(FALSE, &m_vMouseMoveStart))
		{
			if (nFlags == MK_LBUTTON && CursorIntersectionPlane(TRUE, &m_vMouseSelectStart))
			{
				UpdateSelectBlock(&m_vMouseSelectStart, &m_vMouseSelectStart);
			}
			g_pWndMouseCapture = this;
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CWorldViewDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (g_pWndMouseCapture && CursorIntersectionPlane(FALSE, &m_vMouseMoveEnd))
	{
		if (nFlags == MK_LBUTTON && CursorIntersectionPlane(TRUE, &m_vMouseSelectEnd))
		{
			UpdateSelectBlock(&m_vMouseSelectStart, &m_vMouseSelectEnd);
		}

		/*if (nFlags & MK_CONTROL)
		{
			m_vCameraPos.x -= (m_vMouseMoveEnd.x - m_vMouseMoveStart.x) * 0.5f;
			m_vCameraPos.z -= (m_vMouseMoveEnd.z - m_vMouseMoveStart.z) * 0.5f;
		}
		else if (IsMouseOut())
		{
			m_vCameraPos.x += (m_vMouseMoveEnd.x - m_vMouseMoveStart.x) * 0.5f;
			m_vCameraPos.z += (m_vMouseMoveEnd.z - m_vMouseMoveStart.z) * 0.5f;
		}*/
		m_vMouseMoveStart = m_vMouseMoveEnd;
		g_pWndMouseCapture= NULL;
	}

	ReleaseCapture();
	CDialog::OnLButtonUp(nFlags, point);
}

void CWorldViewDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags & MK_LBUTTON)
	{
		if (g_pWndMouseCapture && CursorIntersectionPlane(FALSE, &m_vMouseMoveEnd))
		{
			if (nFlags == MK_LBUTTON && CursorIntersectionPlane(TRUE, &m_vMouseSelectEnd))
			{
				UpdateSelectBlock(&m_vMouseSelectStart, &m_vMouseSelectEnd);
			}

			if (nFlags & MK_CONTROL)
			{
				m_vCameraPos.x -= (m_vMouseMoveEnd.x - m_vMouseMoveStart.x) * (m_fCameraScale / static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSize));
				m_vCameraPos.z -= (m_vMouseMoveEnd.z - m_vMouseMoveStart.z) * (m_fCameraScale / static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSize));
			}
			else if (IsMouseOut())
			{
				m_vCameraPos.x += (m_vMouseMoveEnd.x - m_vMouseMoveStart.x) * (m_fCameraScale / static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSize));
				m_vCameraPos.z += (m_vMouseMoveEnd.z - m_vMouseMoveStart.z) * (m_fCameraScale / static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSize));
			}
			m_vMouseMoveStart = m_vMouseMoveEnd;
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}

RwBool CWorldViewDlg::IsMouseOut()
{
	CPoint ptCursor;
	GetCursorPos(&ptCursor);
	GetDlgItem(IDC_VIEW)->ScreenToClient(&ptCursor);

	CRect rtView;
	GetDlgItem(IDC_VIEW)->GetClientRect(&rtView);

	if (ptCursor.x < rtView.left || ptCursor.x > rtView.right ||
		ptCursor.y < rtView.top || ptCursor.y > rtView.bottom)
	{
		return TRUE;
	}
	return FALSE;
}

RwBool CWorldViewDlg::CursorIntersectionWorld(RwBool bClipView, RwV3d* pvCollPos, RwReal* pfDist)
{
	CPoint ptCursor;
	GetCursorPos(&ptCursor);
	GetDlgItem(IDC_VIEW)->ScreenToClient(&ptCursor);

	if (bClipView)
	{
		CRect rtView;
		GetDlgItem(IDC_VIEW)->GetClientRect(&rtView);

		CLAMP(ptCursor.x, rtView.left, rtView.right);
		CLAMP(ptCursor.y, rtView.top, rtView.bottom);
	}

	RwV2d vPixelPos;
	vPixelPos.x = static_cast<RwReal>(ptCursor.x);
	vPixelPos.y = static_cast<RwReal>(ptCursor.y);
	return PixelRayIntersectionWorld(&vPixelPos, pvCollPos, pfDist);
}

RwBool CWorldViewDlg::CursorIntersectionPlane(RwBool bClipView, RwV3d* pvCollPos, RwReal* pfDist)
{
	CPoint ptCursor;
	GetCursorPos(&ptCursor);
	GetDlgItem(IDC_VIEW)->ScreenToClient(&ptCursor);

	if (bClipView)
	{
		CRect rtView;
		GetDlgItem(IDC_VIEW)->GetClientRect(&rtView);

		CLAMP(ptCursor.x, rtView.left, rtView.right);
		CLAMP(ptCursor.y, rtView.top, rtView.bottom);
	}

	RwV2d vPixelPos;
	vPixelPos.x = static_cast<RwReal>(ptCursor.x);
	vPixelPos.y = static_cast<RwReal>(ptCursor.y);
	return PixelRayIntersectionPlane(&vPixelPos, pvCollPos, pfDist);
}

RwBool CWorldViewDlg::PixelRayIntersectionWorld(RwV2d* pvPixelPos, RwV3d* pvCollPos, RwReal* pfDist)
{
	RwLine rayCamera;
	RwCameraCalcPixelRay(m_pRwCamera, &rayCamera, pvPixelPos);

	RwV3d vDir;
	vDir = rayCamera.end - rayCamera.start;
	RwV3dNormalize(&vDir, &vDir);
	rayCamera.end = vDir * ((static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSize) - m_fCameraScale) * 2.0f);

	RwReal fDist;
	if(RtIntersectionLineTriangle(&rayCamera.start, &rayCamera.end,
		&m_aRwIm3DVertex[0].objVertex,
		&m_aRwIm3DVertex[1].objVertex,
		&m_aRwIm3DVertex[2].objVertex,
		&fDist) || 
		RtIntersectionLineTriangle(&rayCamera.start, &rayCamera.end,
		&m_aRwIm3DVertex[2].objVertex,
		&m_aRwIm3DVertex[1].objVertex,
		&m_aRwIm3DVertex[3].objVertex,
		&fDist))
	{
		if (pfDist)
		{
			*pfDist = fDist;
		}
		if (pvCollPos)
		{
			*pvCollPos = rayCamera.start + (rayCamera.end * fDist);
		}
		return TRUE;
	}
	return FALSE;
}

RwBool CWorldViewDlg::PixelRayIntersectionPlane(RwV2d* pvPixelPos, RwV3d* pvCollPos, RwReal* pfDist)
{
	RwLine rayCamera;
	RwCameraCalcPixelRay(m_pRwCamera, &rayCamera, pvPixelPos);

	RwV3d vDir;
	vDir = rayCamera.end - rayCamera.start;
	RwV3dNormalize(&vDir, &vDir);
	rayCamera.end = vDir * ((static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSize) - m_fCameraScale) * 2.0f);

	RwV4dTag vPlane;
	vPlane.x = 0.0f; vPlane.y = 1.0f; vPlane.z = 0.0f; vPlane.w = 0.0f;

	RwReal fDist;
	RwReal fDot = (vPlane.x * vDir.x) + (vPlane.y * vDir.y) + (vPlane.z * vDir.z);
	if (fDot != 0.0f)
	{
		fDist = -((vPlane.x * rayCamera.start.x) + (vPlane.y * rayCamera.start.y) + (vPlane.z * rayCamera.start.z) + fDot) / fDot;
		if (fDist <= RwV3dLength(&rayCamera.end))
		{
			if (pfDist)
			{
				*pfDist = fDist;
			}
			if (pvCollPos)
			{
				*pvCollPos = rayCamera.start + (vDir * fDist);
			}
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CWorldViewDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	m_fCameraScale += zDelta;

	CLAMP(m_fCameraScale, 1.0f, static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSize) - 1.0f);

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CWorldViewDlg::OnKillFocus(CWnd* pNewWnd)
{
	CDialog::OnKillFocus(pNewWnd);
}

void CWorldViewDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
}

void CWorldViewDlg::UpdateSelectBlock(RwV3d* pvP1, RwV3d* pvP2)
{
	RwV3d avPos[2];
	avPos[0] = *pvP1;
	avPos[1] = *pvP2;

	CLAMP(avPos[0].x, m_bboxWorld.inf.x, m_bboxWorld.sup.x);
	CLAMP(avPos[0].z, m_bboxWorld.inf.z, m_bboxWorld.sup.z);
	CLAMP(avPos[1].x, m_bboxWorld.inf.x, m_bboxWorld.sup.x);
	CLAMP(avPos[1].z, m_bboxWorld.inf.z, m_bboxWorld.sup.z);

	RwBBoxCalculate(&m_bboxSelectBlock, avPos, 2);

	// 3+--------------+1
	//  |			   |
	//  |			   |
	//  |			   |
	//  |			   |
	//  |              |
	// 2+--------------+0

	CalcSelectBlock(&m_bboxSelectBlockBlock, &m_bboxSelectBlock, dGET_WORLD_PARAM()->WorldBlockSize);
	CalcSelectBlock(&m_bboxSelectBlockSector, &m_bboxSelectBlock, dGET_WORLD_PARAM()->WorldSectorSize);
	CalcSelectBlock(&m_bboxSelectBlockField, &m_bboxSelectBlock, dGET_WORLD_PARAM()->WorldFieldSize);

	switch (GetBlockMode())
	{
	case EBLOCK_MODE_NONE:
		{
			m_bboxSelectBlock.inf.x = m_bboxSelectBlock.inf.y = m_bboxSelectBlock.inf.z = 0.0f;
			m_bboxSelectBlock.sup.x = m_bboxSelectBlock.sup.y = m_bboxSelectBlock.sup.z = 0.0f;
			break;
		}
	case EBLOCK_MODE_BLOCK:
		{
			m_bboxSelectBlock = m_bboxSelectBlockBlock;
			break;
		}
	case EBLOCK_MODE_SECTOR:
		{
			m_bboxSelectBlock = m_bboxSelectBlockSector;
			break;
		}
	case EBLOCK_MODE_FIELD:
		{
			m_bboxSelectBlock = m_bboxSelectBlockField;
			break;
		}
	}
}

void CWorldViewDlg::CalcSelectBlock(RwBBox* pRwBoxOut, RwBBox* pRwBoxIn, RwUInt32 iBlockSize)
{
	RwV3d avPos[2];
	avPos[0].x = static_cast<RwReal>(static_cast<RwInt32>((static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSize) * 0.5f + pRwBoxIn->inf.x) / iBlockSize) * iBlockSize - (static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSize) * 0.5f));
	avPos[0].z = static_cast<RwReal>(static_cast<RwInt32>((static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSize) * 0.5f + pRwBoxIn->inf.z) / iBlockSize) * iBlockSize - (static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSize) * 0.5f));
	avPos[1].x = static_cast<RwReal>(static_cast<RwInt32>((static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSize) * 0.5f + pRwBoxIn->sup.x) / iBlockSize) * iBlockSize - (static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSize) * 0.5f) + iBlockSize);
	avPos[1].z = static_cast<RwReal>(static_cast<RwInt32>((static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSize) * 0.5f + pRwBoxIn->sup.z) / iBlockSize) * iBlockSize - (static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSize) * 0.5f) + iBlockSize);

	CLAMP(avPos[0].x, m_bboxWorld.inf.x, m_bboxWorld.sup.x);
	CLAMP(avPos[0].z, m_bboxWorld.inf.z, m_bboxWorld.sup.z);
	CLAMP(avPos[1].x, m_bboxWorld.inf.x, m_bboxWorld.sup.x);
	CLAMP(avPos[1].z, m_bboxWorld.inf.z, m_bboxWorld.sup.z);

	RwBBoxCalculate(pRwBoxOut, avPos, 2);
}

void CWorldViewDlg::SetRwIm3DVertex(RwIm3DVertex* pRwIm3DVertex, RwReal fX, RwReal fY, RwReal fZ, RwReal fU, RwReal fV, RwUInt8 a, RwUInt8 r, RwUInt8 g, RwUInt8 b)
{
	RwIm3DVertexSetPos(pRwIm3DVertex, fX,  fY, fZ);
	RwIm3DVertexSetRGBA(pRwIm3DVertex, r, g, b, a);
	RwIm3DVertexSetU(pRwIm3DVertex, fU);
	RwIm3DVertexSetV(pRwIm3DVertex, fV);  
}
void CWorldViewDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	CDialog::OnRButtonDown(nFlags, point);
}

void CWorldViewDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (!IsMouseOut())
	{
		CMenu	menu, *pContextMenu = NULL;
		CPoint	ptCursor;

		GetCursorPos(&ptCursor);

		menu.LoadMenu(IDR_POPUP_WORLD_VIEW);

		switch (GetBlockMode())
		{
		case EBLOCK_MODE_NONE:
			break;
		case EBLOCK_MODE_BLOCK:
			pContextMenu = menu.GetSubMenu(0);
			break;
		case EBLOCK_MODE_SECTOR:
			pContextMenu = menu.GetSubMenu(1);
			break;
		case EBLOCK_MODE_FIELD:
			pContextMenu = menu.GetSubMenu(2);
			break;
		}

		if (pContextMenu)
		{
			dGETDOC()->SetFieldPropMode(EFIELDPROP_MODE_INDOOR);
			pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptCursor.x ,ptCursor.y, (CWnd*)dGETAFX()->m_pView);
		}
	}

	CDialog::OnRButtonUp(nFlags, point);
}

RwV3d CWorldViewDlg::GetSelectBlockMidPos()
{
	RwV3d vResult;
	RwV3dAdd(&vResult, &m_bboxSelectBlock.sup, &m_bboxSelectBlock.inf);
	RwV3dScale(&vResult, &vResult, 0.5f);
	vResult.y = 0.0f;

	return vResult;
}
void CWorldViewDlg::OnBnClickedBtnSaberCamera()
{
	m_vCameraPos.x = RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)))->x;
	m_vCameraPos.z = RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)))->z;
}

void CWorldViewDlg::ResetSelectBlock()
{
	m_bboxSelectBlock.inf = m_bboxSelectBlock.sup;
}

RwInt32 CWorldViewDlg::GetBlockMode()
{
	if (m_CBBoxSelect.GetCurSel() == 0)
	{
		return EBLOCK_MODE_FIELD;
	}
	else if (m_CBBoxSelect.GetCurSel() == 1)
	{
		return EBLOCK_MODE_SECTOR;
	}
	else if (m_CBBoxSelect.GetCurSel() == 2)
	{
		return EBLOCK_MODE_BLOCK;
	}
	return EBLOCK_MODE_NONE;
}

RwInt32 CWorldViewDlg::GetBlockSize()
{
	switch (GetBlockMode())
	{
	case EBLOCK_MODE_BLOCK:
		{
			return dGET_WORLD_PARAM()->WorldBlockSize;
		}
	case EBLOCK_MODE_SECTOR:
		{
			return dGET_WORLD_PARAM()->WorldSectorSize;
		}
	case EBLOCK_MODE_FIELD:
		{
			return dGET_WORLD_PARAM()->WorldFieldSize;
		}
	}
	return 0;
}

RwBool CWorldViewDlg::IsSelectEmpty()
{
	if (m_bboxSelectBlock.inf == m_bboxSelectBlock.sup)
	{
		return TRUE;
	}
	return FALSE;
}

RwBool CWorldViewDlg::GetSelectBlockNum(RwInt32* piNumX, RwInt32* piNumY)
{
	if (IsSelectEmpty())
	{
		return FALSE;
	}

	RwInt32 iSize = GetBlockSize();
	if (iSize == 0)
	{
		return FALSE;
	}

	RwV3d vSize = m_bboxSelectBlock.sup - m_bboxSelectBlock.inf;

	*piNumX = static_cast<RwInt32>(vSize.x) / iSize;
	*piNumY = static_cast<RwInt32>(vSize.z) / iSize;
	
	return TRUE;
}

RwBool CWorldViewDlg::GetSelectBlockPos(RwV3d* pvSup, RwV3d* pvInf)
{
	if (IsSelectEmpty())
	{
		return FALSE;
	}

	*pvSup = m_bboxSelectBlock.sup;
	*pvInf = m_bboxSelectBlock.inf;

	return TRUE;
}
void CWorldViewDlg::OnCbnSelchangeComboSelect()
{
	ResetSelectBlock();
}

void CWorldViewDlg::ApplyFieldProp(sNTL_FIELD_PROP* pNtlFieldProp, eNTL_FIELD_PROPID eNtlFieldPropID)
{
	if(IsSelectEmpty())
	{
		AfxMessageBox("can't get a prop. select blocks.");
		return;
	}

	CNtlWorldFieldManager*	pFieldMgr = dGETMGR();

	RwV3d vSup, vInf;
	GetSelectBlockPos(&vSup, &vInf);
	vInf.x += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldBlockSize / 2);
	vInf.z += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldBlockSize / 2);
	vSup.x -= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldBlockSize / 2);
	vSup.z -= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldBlockSize / 2);

	for (RwReal z = vInf.z; z <= vSup.z; z += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldBlockSize))
	{
		for (RwReal x = vInf.x; x <= vSup.x; x += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldBlockSize))
		{
			RwV3d vPos;
			vPos.x = x;
			vPos.z = z;

			pFieldMgr->SetAFieldProp(vPos, *pNtlFieldProp, eNtlFieldPropID);
		}
	}
	
	AfxMessageBox("Set.");
}

void CWorldViewDlg::ApplyBlockProp(sNTL_BLOCK_PROP* pNtlBlockProp, eNTL_BLOCK_PROPID eNtlBlockPropID)
{
	if(IsSelectEmpty())
	{
		AfxMessageBox("can't get a prop. select blocks.");
		return;
	}

	if (GetBlockMode() != EBLOCK_MODE_BLOCK)
	{
		AfxMessageBox("select block mode.");
		return;
	}

	CNtlWorldFieldManager4RWWorld* pFieldMgr = (CNtlWorldFieldManager4RWWorld*)dGETMGR();

	RwV3d vSup, vInf;
	GetSelectBlockPos(&vSup, &vInf);
	vInf.x += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldBlockSize / 2);
	vInf.z += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldBlockSize / 2);
	vSup.x -= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldBlockSize / 2);
	vSup.z -= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldBlockSize / 2);

	for (RwReal z = vInf.z; z <= vSup.z; z += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldBlockSize))
	{
		for (RwReal x = vInf.x; x <= vSup.x; x += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldBlockSize))
		{
			RwV3d vPos;
			vPos.x = x;
			vPos.z = z;

			pFieldMgr->SetABlockProp(vPos, *pNtlBlockProp, eNtlBlockPropID);
		}
	}

	AfxMessageBox("Set.");
}

void CWorldViewDlg::RenderBlockTightBBox(RwReal fRenderSize, RwCamera* pCamera, RwBool bRender,
										 RwUInt8 a, RwUInt8 r, RwUInt8 g, RwUInt8 b, RwBool bWire,
										 RwBool bSelectedRender,
										 RwUInt8 aS, RwUInt8 rS, RwUInt8 gS, RwUInt8 bS, RwBool bWireS)
{
	CNtlWorldFieldManager4RWWorld*	pFieldMgr	= (CNtlWorldFieldManager4RWWorld*)dGETMGR();
	RwBBox							bbox		= RpNtlCameraFrustumGetStandardBBox(pCamera, static_cast<RwInt32>(fRenderSize));

	for (RwReal sz = bbox.inf.z; sz < bbox.sup.z; sz += fRenderSize)
	{
		for (RwReal sx = bbox.inf.x; sx < bbox.sup.x; sx += fRenderSize)
		{
			RwInt32 iBufferSize = (static_cast<RwInt32>(fRenderSize) / dGET_WORLD_PARAM()->WorldBlockSize) *
				(static_cast<RwInt32>(fRenderSize) / dGET_WORLD_PARAM()->WorldBlockSize) * 2;

			if (iBufferSize == 0)
			{
				continue;
			}

			RwBBox	bboxRender;
			RwInt32 iTemp = 0;
			RwV3d	avTemp[32];

			for (RwReal z = sz; z < sz + fRenderSize; z += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldBlockSize))
			{
				for (RwReal x = sx; x < sx + fRenderSize; x += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldBlockSize))
				{
					RwV3d vPos;
					vPos.x = x;
					vPos.z = z;

					sNTL_BLOCK_PROP sNtlBlockProp;
					if (pFieldMgr->GetWorldBlockManager()->GetABlockProp(vPos, sNtlBlockProp))
					{
						if (sNtlBlockProp._BBox.sup.y == RwRealMINVAL ||
							sNtlBlockProp._BBox.inf.y == RwRealMAXVAL)
						{
							continue;
						}

						avTemp[iTemp++] = sNtlBlockProp._BBox.inf;
						avTemp[iTemp++] = sNtlBlockProp._BBox.sup;
					}
				}
			}

			if (iTemp != 0)
			{
				RwRGBA rgba;
				RwBBoxCalculate(&bboxRender, avTemp, iTemp);

				bboxRender.inf.x = sx;
				bboxRender.inf.z = sz;
				bboxRender.sup.x = sx + fRenderSize;
				bboxRender.sup.z = sz + fRenderSize;

				if (bSelectedRender &&						
					m_bboxSelectBlock.inf.x != m_bboxSelectBlock.sup.x && m_bboxSelectBlock.inf.z != m_bboxSelectBlock.sup.z &&
					(m_bboxSelectBlock.inf.x <= bboxRender.sup.x - 0.01f) && (m_bboxSelectBlock.inf.z <= bboxRender.sup.z - 0.01f) && 
					(m_bboxSelectBlock.sup.x >= bboxRender.inf.x + 0.01f) && (m_bboxSelectBlock.sup.z >= bboxRender.inf.z + 0.01f))
				{
					rgba.alpha	= aS;
					rgba.red	= rS;
					rgba.green	= gS;
					rgba.blue	= bS;
					RenderGeometryBox(NULL, &bboxRender, &rgba, &rgba, 0.0f, bWireS);
				}
				else if (bRender)
				{
					rgba.alpha	= a;
					rgba.red	= r;
					rgba.green	= g;
					rgba.blue	= b;
					RenderGeometryBox(NULL, &bboxRender, &rgba, &rgba, 0.0f, bWire);
				}
			}
		}
	}
}
void CWorldViewDlg::OnBnClickedCheckProp()
{
	UpdateData(TRUE);

	if (m_BtnPropVisible.GetCheck())
	{
		m_pGamePropVisibleDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pGamePropVisibleDlg->ShowWindow(SW_HIDE);
	}
}

void CWorldViewDlg::SetGamePropVisible(RwBool bVisible)
{
	m_BtnPropVisible.SetCheck(bVisible);

	UpdateData(FALSE);
}
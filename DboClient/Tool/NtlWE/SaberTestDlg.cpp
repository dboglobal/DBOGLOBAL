#include "stdafx.h"

#include "NtlWE.h"

#include "NtlWorldPlugin.h"
#include "NtlWorldCommon.h"
#include "NtlWorldInterface.h"
#include "NtlWorldMaterialPlugin.h"

#include "NtlWorldFieldManager.h"

#include "NtlPLRenderState.h"

#include "NtlPLWorldEntity.h"
#include "NtlPLHeatHaze.h"

#include "NtlPostEffectCamera.h"

#include "NtlWeControlUi.h"

#include "ProgressWnd.h"

#include "SaberTestDlg.h"

static void
CombineRenderCallback(RwResEntry *repEntry, void *object, RwUInt8 type, RwUInt32 flags)
{
	RxD3D9ResEntryHeader *pResEntryHeader;
	RxD3D9InstanceData *pInstancedData;
	RwInt32 numMeshes;

	// get d3d devices
	LPDIRECT3DDEVICE9 pD3DDev = static_cast<LPDIRECT3DDEVICE9>(RwD3D9GetCurrentD3DDevice());

	// Set the Default Pixel shader
	RwD3D9SetPixelShader(NULL);

	// Set clipping
	_rwD3D9EnableClippingIfNeeded(object, type);

	// Get header
	pResEntryHeader = (RxD3D9ResEntryHeader *)(repEntry + 1);

	// Vertex Declaration
	RwD3D9SetVertexDeclaration(pResEntryHeader->vertexDeclaration);

	/* Get the instanced data */
	pInstancedData = (RxD3D9InstanceData*)(pResEntryHeader + 1);

	/* Get the number of meshes */
	numMeshes = pResEntryHeader->numMeshes;
	if (numMeshes)
	{
		do
		{
			RpMaterial *pMaterial;
			RwRGBA *pMatcolor;

			pMaterial = pInstancedData->material;
			pMatcolor = &(pMaterial->color);

			// Vertex shader
			RwD3D9SetVertexShader(pInstancedData->vertexShader);

			sRpNtlWorldSectorSplat	*pNtlWorldSectorSplat	= RpNtlWorldGetSectorSplat(pInstancedData->material);
			CNtlWorldSector			*pNtlWorldSector		= pNtlWorldSectorSplat->pNtlWorldSector;

			BeginLightState(FALSE);

			if (pNtlWorldSector->m_IsTileTransparency)
			{
				RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)TRUE);
			}

			RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
			RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

			RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
			RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); 
			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

			if (pNtlWorldSector->m_IsTileTransparency)
			{
				RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
				RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
				RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			}
			else
			{
				RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
				RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
			}


			RwD3D9SetTexture(pNtlWorldSector->m_pTextureBase, 0);

			RwD3D9SetTransform(D3DTS_TEXTURE0, &pNtlWorldSector->m_TextureMatBase[1]);

			_RpNtlWorldSectorDrawPrimitive(pInstancedData, pResEntryHeader);

			RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)TRUE); 
			RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
			RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);

			RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_CURRENT);
			RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
			RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); 
			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

			RwD3D9SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			RwD3D9SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
			RwD3D9SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

			if (pNtlWorldSector->m_IsTileTransparency)
			{
				RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
				RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
				RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			}
			else
			{
				RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
				RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
				RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
			}

			for(RwInt32 i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
			{
				if(pNtlWorldSector->m_pTextureSplat[i] && pNtlWorldSector->m_pTextureAlpha[i])
				{
					RwD3D9SetTexture(pNtlWorldSector->m_pTextureAlpha[i], 0);
					RwD3D9SetTexture(pNtlWorldSector->m_pTextureSplat[i], 1);
					RwD3D9SetTransform(D3DTS_TEXTURE0, &pNtlWorldSector->m_TextureMat[i][0]);
					RwD3D9SetTransform(D3DTS_TEXTURE1, &pNtlWorldSector->m_TextureMat[i][1]);

					_RpNtlWorldSectorDrawPrimitive(pInstancedData, pResEntryHeader);
				}
			}

			

			EndLightState();

			RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);

			RwD3D9SetTexture(NULL, 0);
			RwD3D9SetTexture(NULL, 1);

			D3DXMATRIXA16 mIdentity;
			D3DXMatrixIdentity(&mIdentity);
			RwD3D9SetTransform(D3DTS_TEXTURE0, &mIdentity);
			RwD3D9SetTransform(D3DTS_TEXTURE1, &mIdentity);

			RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
			RwD3D9SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
			RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
			RwD3D9SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);

			RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)FALSE); 
			RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)rwBLENDSRCALPHA);
			RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)rwBLENDINVSRCALPHA);

			RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
			RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT); 
			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

			RwD3D9SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
			RwD3D9SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			RwD3D9SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
			RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
			RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
		} while(++pInstancedData, --numMeshes); // Move onto the next pInstancedData
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CSaberTestDlg, CDialog)

CSaberTestDlg::CSaberTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSaberTestDlg::IDD, pParent)
	, m_strTest1(_T(""))
	, m_strTest2(_T(""))
{

}

CSaberTestDlg::~CSaberTestDlg()
{
}

void CSaberTestDlg::InitializeWorldData()
{
}

BOOL CSaberTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSaberTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT7, m_strTest1);
	DDX_Text(pDX, IDC_EDIT8, m_strTest2);
}

BOOL CSaberTestDlg::PreTranslateMessage(MSG* pMsg)
{
	// prevent keys; escape, return
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
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


BEGIN_MESSAGE_MAP(CSaberTestDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON6, &CSaberTestDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CSaberTestDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CSaberTestDlg::OnBnClickedButton8)
END_MESSAGE_MAP()


// CSaberTestDlg 메시지 처리기입니다.

// void CSaberTestDlg::OnBnClickedTest1()
// {
// 	CProgressWnd	WndProgress((CNtlWEFrm*)AfxGetMainWnd(), "Progress");
// 	RwChar			StrProgress[128];
// 	RwInt32			NumAll = dGET_WORLD_PARAM()->WorldSectorNum * dGET_WORLD_PARAM()->WorldSectorNum;
// 
// 	WndProgress.GoModal();
// 	WndProgress.SetRange(0, NumAll);
// 
// 	for(RwInt32 i = 0; i < NumAll; ++i)
// 	{
// 		::sprintf_s(StrProgress, 128, "%d / %d", i, NumAll);
// 		WndProgress.SetText(StrProgress);
// 		WndProgress.StepIt();
// 		WndProgress.PeekAndPump();
// 
// 		TerrainTextrueCombine(i);
// 	}
// 
// 	dGETMGR()->RefreshFieldAll();
// 
// 	WndProgress.SetText("Done.");
// 	OnOK();
// }
// 
// void CSaberTestDlg::TerrainTextrueCombine(RwUInt32 iSectorIdx)
// {
// 	CNtlWorldSector*	pSector		= &dGETMGR()->m_pSectors[iSectorIdx];
// 	RwV3d				vSectorMid;	vSectorMid.x = pSector->DatumPoint.x; vSectorMid.y = pSector->DatumPoint.y; vSectorMid.z = pSector->DatumPoint.z;
// 
// 	RwCamera*			pCamera		= RwCameraCreate();
// 	RwFrame*			pFrame		= RwFrameCreate();
// 
// 	RwRaster*			pRaster		= RwRasterCreate(512, 512, 0, rwRASTERFORMAT8888 | rwRASTERTYPECAMERATEXTURE);
// 	RwRaster*			pRasterZ	= RwRasterCreate(512, 512, 0, rwRASTERTYPEZBUFFER);
// 
// 	RwCameraSetFrame(pCamera, pFrame);
// 	RwCameraSetRaster(pCamera, pRaster);
// 	RwCameraSetZRaster(pCamera, pRasterZ);
// 
// 	RwCameraSetProjection(pCamera, rwPARALLEL);
// 	RwCameraSetNearClipPlane(pCamera, 0.001f);
// 	RwCameraSetFarClipPlane(pCamera, 2048.0f);
// 	
// 	RwV2d vw; vw.x = vw.y = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize / 2);
// 	RwCameraSetViewWindow(pCamera, &vw);
// 
// 	dGETMGR()->SetAnotherField(TRUE);
// 	((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlPLWorldEntity()->SetPortalPosition(vSectorMid);
// 	CNtlWeControlUi::GetInstance().Update();
// 	((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.OnIdle();
// 
// 	RwChar acFilename[512];
// 	sprintf_s(acFilename, 512, "%scmap\\%.8d.png", dGET_WORLD_PARAM()->WorldProjectFolderName, iSectorIdx);
// 
// 	RwMatrix*	pMatrix	= RwFrameGetLTM(pFrame);
// 
// 	CNtlMath::MathRwV3dAssign(RwMatrixGetAt(pMatrix), 0.0f, -1.0f, 0.0f);
// 	CNtlMath::MathRwV3dAssign(RwMatrixGetRight(pMatrix), 1.0f, 0.0f, 0.0f);
// 	CNtlMath::MathRwV3dAssign(RwMatrixGetUp(pMatrix), 0.0f, 0.0f, 1.0f);
// 
// 	*RwMatrixGetPos(pMatrix)	= vSectorMid;	
// 	RwV3dIncrementScaled(RwMatrixGetPos(pMatrix), RwMatrixGetAt(pMatrix), -0.5f * RwCameraGetFarClipPlane(pCamera));
// 	
// 	RwMatrixUpdate(pMatrix);
// 	//RwFrameUpdateObjects(pFrame);
// 
// 	RwCameraClear(pCamera, &((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.m_BackgroundColor, rwCAMERACLEARZ | rwCAMERACLEARIMAGE);
// 	if(RwCameraBeginUpdate(pCamera))
// 	{
// 		//RxD3D9AllInOneSetLightingCallBack(g_pNodeWorld, NULL);
// 		RxD3D9AllInOneSetRenderCallBack(g_pNodeWorld, CombineRenderCallback);
// 		RpAtomicRender(pSector->m_pAtomic);
// 		
// 		RwCameraEndUpdate(RwCameraGetCurrentCamera());
// 	}
// 	API_PL_RasterSaveImage(pRaster, acFilename);	
// 	
// 	RwCameraSetFrame(pCamera, NULL);
// 	RwCameraSetRaster(pCamera, NULL);
// 	RwCameraSetZRaster(pCamera, NULL);
// 
// 	RwRasterDestroy(pRasterZ);
// 	RwRasterDestroy(pRaster);
// 	RwFrameDestroy(pFrame);
// 	RwCameraDestroy(pCamera);
// }

#include "NtlPLDojoContainer.h"

void CSaberTestDlg::OnBnClickedButton6()
{
	UpdateData(TRUE);

	RwInt32 iDojoID		= ::atoi(m_strTest1.GetBuffer());
	RwInt32 iDojoLevel	= ::atoi(m_strTest2.GetBuffer());

	if (iDojoID < 0)
	{
		AfxMessageBox("error");
		return;
	}

	static RwUInt8 A = 2;

	sEmblemFactor EmblemFactor;
	EmblemFactor.byTypeA = A;
	EmblemFactor.byTypeB = 1;
	EmblemFactor.byTypeC = 2;
	EmblemFactor.byTypeAColor = 1;
	EmblemFactor.byTypeBColor = 2;
	EmblemFactor.byTypeCColor = 2;

	GetSceneManager()->GetDojoContainer()->SetDojo(iDojoID, iDojoLevel, &EmblemFactor);
}

void CSaberTestDlg::OnBnClickedButton7()
{
	GetSceneManager()->GetDojoContainer()->RefreshDojo();
}

void CSaberTestDlg::OnBnClickedButton8()
{
	UpdateData(TRUE);

	RwInt32 iDojoLevel	= ::atoi(m_strTest2.GetBuffer());

	static RwUInt8 A = 2;

	sEmblemFactor EmblemFactor;
	EmblemFactor.byTypeA = A;
	EmblemFactor.byTypeB = 1;
	EmblemFactor.byTypeC = 2;
	EmblemFactor.byTypeAColor = 1;
	EmblemFactor.byTypeBColor = 2;
	EmblemFactor.byTypeCColor = 2;

	GetSceneManager()->GetDojoContainer()->SetDojoWorld(iDojoLevel, &EmblemFactor);


}

#include "StdAfx.h"
#include "NtlWorldEntityPreview.h"
#include "NtlMath.h"
#include "ntlworlddi.h"
#include "NtlPLCameraRenderTexture.h"
#include "NtlCoreUtil.h"
#include "NtlPLResourceManager.h"
#include "NtlPLRenderState.h"
#include "NtlPLObject.h"


CNtlWorldEntityPreview::CNtlWorldEntityPreview(void)
{
	m_pR2T = NTL_NEW CNtlPLCameraRenderTexture;
	m_pR2T->Create(512, 512, 512, 512, 45.0f, TRUE);
	m_pR2T->SetFar(1000.0f);

	RwV3d PosCam	= {0.0f, 10.0f, -10.0f};
	RwV3d PosLookAt = {0.0f, 0.0f, 0.0f};

	m_pR2T->SetCameraPosition(&PosCam, &PosLookAt);

	// preview texture & frame
	m_pPreviewTexFrame = CNtlPLResourceManager::GetInstance()->LoadTexture("PreviewFrame", "Tool\\SABER\\Texture\\UI\\");
	DBO_ASSERT(m_pPreviewTexFrame, "Texture load failed.");

	RwReal recipCameraZ = 1.0f / RwCameraGetNearClipPlane(CNtlPLGlobal::m_RwCamera);
	RwInt32 nStartX = 0;
	RwInt32 nStartY = 512;
	RwInt32 width	= 512;
	RwInt32 height	= 512;

	RwIm2DVertexSetScreenX(&m_PreviewTexPanel[0], nStartX - 0.5f);
	RwIm2DVertexSetScreenY(&m_PreviewTexPanel[0], nStartY - 0.5f);
	RwIm2DVertexSetScreenZ(&m_PreviewTexPanel[0], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_PreviewTexPanel[0], recipCameraZ);
	RwIm2DVertexSetU(&m_PreviewTexPanel[0], 0, recipCameraZ);
	RwIm2DVertexSetV(&m_PreviewTexPanel[0], 0, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_PreviewTexPanel[0], 255, 255, 255, 255); 

	RwIm2DVertexSetScreenX(&m_PreviewTexPanel[1], nStartX - 0.5f);
	RwIm2DVertexSetScreenY(&m_PreviewTexPanel[1], nStartY + height - 0.5f);
	RwIm2DVertexSetScreenZ(&m_PreviewTexPanel[1], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_PreviewTexPanel[1], recipCameraZ);
	RwIm2DVertexSetU(&m_PreviewTexPanel[1], 0, recipCameraZ);
	RwIm2DVertexSetV(&m_PreviewTexPanel[1], 1, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_PreviewTexPanel[1], 255, 255, 255, 255);

	RwIm2DVertexSetScreenX(&m_PreviewTexPanel[2], nStartX + width - 0.5f);
	RwIm2DVertexSetScreenY(&m_PreviewTexPanel[2], nStartY - 0.5f);
	RwIm2DVertexSetScreenZ(&m_PreviewTexPanel[2], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_PreviewTexPanel[2], recipCameraZ);
	RwIm2DVertexSetU(&m_PreviewTexPanel[2], 1, recipCameraZ);
	RwIm2DVertexSetV(&m_PreviewTexPanel[2], 0, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_PreviewTexPanel[2], 255, 255, 255, 255); 

	RwIm2DVertexSetScreenX(&m_PreviewTexPanel[3], nStartX + width - 0.5f);
	RwIm2DVertexSetScreenY(&m_PreviewTexPanel[3], nStartY + height - 0.5f);
	RwIm2DVertexSetScreenZ(&m_PreviewTexPanel[3], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_PreviewTexPanel[3], recipCameraZ);
	RwIm2DVertexSetU(&m_PreviewTexPanel[3], 1, recipCameraZ);
	RwIm2DVertexSetV(&m_PreviewTexPanel[3], 1, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_PreviewTexPanel[3], 255, 255, 255, 255);
}

CNtlWorldEntityPreview::~CNtlWorldEntityPreview(void)
{
	RwTextureDestroy(m_pPreviewTexFrame);
	m_pR2T->Destroy();
	NTL_DELETE(m_pR2T);
}

RwCamera* CNtlWorldEntityPreview::GetCamera()
{
	return m_pR2T->GetCamera();
}

VOID CNtlWorldEntityPreview::SetRotation(RwV3d& m_Rot)
{

}

VOID CNtlWorldEntityPreview::SetCameraAtObj(CNtlPLEntity* _pNtlPLEntity)
{
	CNtlMath::MathRwV3dAssign(&m_Rot, 0.0f, 0.0f, 0.0f);
	m_Scale = 0.0f;

	RwV3d Rot = {10.0f, 180.0f, 0.0f};

	// light direction
	RwMatrix mRay;
	RwMatrixRotate(&mRay, &XAxis, Rot.x, rwCOMBINEREPLACE);
	RwMatrixRotate(&mRay, &YAxis, Rot.y, rwCOMBINEPOSTCONCAT);
	RwMatrixUpdate(&mRay);

	// camera direction
	RwFrame *pCamFrame = RwCameraGetFrame(m_pR2T->GetCamera());
	RwMatrixCopy(RwFrameGetMatrix(pCamFrame), &mRay);
	RwFrameUpdateObjects(pCamFrame);

	// Set the frustum (a cube) to enclose an object of the given radius. 
	// The frustum may be centered on the object using 
	//if (_pNtlPLEntity->GetClassType() == PLENTITY_OBJECT)
	//{
	//	static_cast<CNtlPLObject*>(_pNtlPLEntity)->CalcBoundingSphere();
	//}

	RwV2d			vw;
	const RwSphere*	pSphere = _pNtlPLEntity->GetBoundingSphere();
	RwReal			Radius	= pSphere->radius;

	m_pR2T->SetFar(2.0f * Radius);
	m_pR2T->SetNear(0.001f * Radius);
	vw.x = 1.0f;
	vw.y = 1.0f;
	RwCameraSetViewWindow(m_pR2T->GetCamera(), &vw);

	// Set the center of the doodads camera frustum volume.
	// -0.5f could be changed 'cos the frustum is orthogonal.
	// It depends on deciding how big the frustum could include objects but if the cam. was far from the target,
	// shadow texture detail will be more lower.
	RwMatrix*	pCamMatrix	= RwFrameGetMatrix(pCamFrame);
	RwV3d       Translation;

	RwV3dNegate(&Translation, RwMatrixGetPos(pCamMatrix)); Translation.y += Radius * 0.25f;
	//RwV3dAdd(&Translation, &Translation, &pSphere->center);
	RwV3dIncrementScaled(&Translation, RwMatrixGetAt(pCamMatrix), -0.8f * RwCameraGetFarClipPlane(m_pR2T->GetCamera()));

	RwFrameTranslate(pCamFrame, &Translation, rwCOMBINEPOSTCONCAT);

	m_pR2T->SetFar(1000.0f);
}

VOID CNtlWorldEntityPreview::Update(CNtlPLEntity* pNtlPLEntity)
{
	m_Rot.y += g_GetElapsedTime() * 10.0f;

	RwV3d RotOld;
	CNtlMath::MathRwV3dAssign(&RotOld, pNtlPLEntity->GetRotate()->x, pNtlPLEntity->GetRotate()->y, pNtlPLEntity->GetRotate()->z);

	pNtlPLEntity->SetRotate(m_Rot.x, m_Rot.y, m_Rot.z);

	m_pR2T->RenderTexture(pNtlPLEntity);

	pNtlPLEntity->SetRotate(RotOld.x, RotOld.y, RotOld.z);
}

VOID CNtlWorldEntityPreview::Render()
{
	RwV2d PosScreen = {0, 512};
	RwV2d LenScreen = {512, 512};

	// preview frame
	UpdateAlphaState(TRUE);

	RwD3D9SetTexture(m_pPreviewTexFrame, 0);
	RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_PreviewTexPanel, 4);

	UpdateAlphaState(FALSE);

	// preview render2surface
	m_pR2T->Render(CNtlPLGlobal::m_RwCamera, PosScreen, LenScreen);
}

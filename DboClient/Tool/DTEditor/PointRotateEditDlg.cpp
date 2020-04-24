// PointRotateEditDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DTEditor.h"
#include "PointRotateEditDlg.h"
#include "DTEditorGlobal.h"

// renderware
#include <rwcore.h>
#include <rpworld.h>
#include <rtintsec.h>
#include <rpcollis.h>
#include <rtquat.h> 
#include <rtanim.h>
#include <rphanim.h>
#include <rptoon.h>
#include <rpskin.h>
#include <rpusrdat.h>
#include <rpid.h>
#include <rpmatfx.h>

#include <rpuvanim.h>
#include <rpprtstd.h>
#include <rpprtadv.h>

// framework
#include "NtlCamera.h"

// presentation
#include "NtlPLGlobal.h"
#include "NtlPLApi.h"

// simulation
#include "NtlDTProp.h"

#define	EDIT_BOX_PLANE_NONE		-1
#define EDIT_BOX_PLANE_FRONT	0
#define EDIT_BOX_PLANE_BACK		1
#define EDIT_BOX_PLANE_LEFT		2
#define EDIT_BOX_PLANE_RIGHT	3
#define EDIT_BOX_PLANE_UP		4
#define EDIT_BOX_PLANE_DOWN		5


struct SPickEditData
{
	RwBool	bSuccess;
	RwReal	fDist;
	RwV3d	vPickPos;
};

SPickEditData g_PickEditData;

RpCollisionTriangle* PickEdit_IntersectionLineTriangleCB( RpIntersection *pIntersection, RpCollisionTriangle *pRpCollisionTriangle, RwReal fRatio, void* pData )
{
	RwLine *pLine = &pIntersection->t.line;
	RwV3d vCollPos = pLine->start + fRatio * (pLine->end - pLine->start);

	RwV3d vSub;
	RwV3dSubMacro( &vSub, &vCollPos, &pLine->start);

	RwReal fDist =  RwV3dLength( &vSub );
	if( fDist < g_PickEditData.fDist )
	{
		g_PickEditData.fDist = fDist;
		RwV3dAssign(&g_PickEditData.vPickPos, &vCollPos);
	}

	g_PickEditData.bSuccess = TRUE;

	return pRpCollisionTriangle;
}


RpAtomic* PickEdit_IntersectionLineAtomicCB( RpIntersection* pIntersection, RpWorldSector* pWorldSector, RpAtomic* pAtomic, RwReal fDistance, void* pData )
{
	RpAtomicForAllIntersections(pAtomic, pIntersection, PickEdit_IntersectionLineTriangleCB, NULL);

	return pAtomic;
}


// CPointRotateEditDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPointRotateEditDlg, CDialog)

CPointRotateEditDlg::CPointRotateEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPointRotateEditDlg::IDD, pParent)
	, m_nGridNum(0)
{
	m_pWorld	= NULL;
	m_pCamera	= NULL;

	m_BackgroundColor.red   = 200;
	m_BackgroundColor.green = 200;
	m_BackgroundColor.blue  = 200;
	m_BackgroundColor.alpha = 0;

	m_fGridScale			= 1.0f;
	m_fGridIntervalScale	= 1.0f;
	m_nGridNum				= 10;
	m_fAxisOffset			= 1.5f;

	m_bLBtnClick			= FALSE;

	ResetCamera();
}

CPointRotateEditDlg::~CPointRotateEditDlg()
{
}

void CPointRotateEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_POINTROTATEEDIT_GRID_NUM, m_nGridNum);
	DDX_Control(pDX, IDC_POINTROTATEEDIT_SNAP, m_btnSnap);
}


BEGIN_MESSAGE_MAP(CPointRotateEditDlg, CDialog)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_POINTROTATEEDIT_RESET_CAMERA, &CPointRotateEditDlg::OnBnClickedPointrotateeditResetCamera)
	ON_EN_CHANGE(IDC_POINTROTATEEDIT_GRID_NUM, &CPointRotateEditDlg::OnEnChangePointrotateeditGridNum)
	ON_EN_KILLFOCUS(IDC_POINTROTATEEDIT_GRID_NUM, &CPointRotateEditDlg::OnEnKillfocusPointrotateeditGridNum)
	ON_BN_CLICKED(IDC_POINTROTATEEDIT_BACK, &CPointRotateEditDlg::OnBnClickedPointrotateeditBack)
	ON_BN_CLICKED(IDC_POINTROTATEEDIT_POINT_DIRECTION, &CPointRotateEditDlg::OnBnClickedPointrotateeditPointDirection)
	ON_BN_CLICKED(IDC_POINTROTATEEDIT_INTERPOLATION, &CPointRotateEditDlg::OnBnClickedPointrotateeditInterpolation)
END_MESSAGE_MAP()


// CPointRotateEditDlg 메시지 처리기입니다.


void CPointRotateEditDlg::CreateResouce(void)
{
	if(m_pWorld)
		return;

	CRect rtClinet;
	GetDlgItem(IDC_ROTATE_VIEW)->GetClientRect(rtClinet);

	CreateWorld();
	CreateCamera(rtClinet.Width(), rtClinet.Height());
	CreateBoxPlane();
}

void CPointRotateEditDlg::CreateWorld(void)
{
	RwBBox bb;
	
	bb.inf.x = -1000.0f; bb.inf.y = -1000.0f; bb.inf.z = -1000.0f;
	bb.sup.x = bb.sup.y = bb.sup.z = 1000.0f;

	m_pWorld = RpWorldCreate(&bb);
}

void CPointRotateEditDlg::CreateCamera(INT nWidth, INT nHeight)
{
	m_pCamera = NTL_NEW CNtlCamera;
	if(!m_pCamera->Create(nWidth, nHeight, TRUE, 0.1f , 500.0f))
	{
		return;
	}

	RwD3D9CameraAttachWindow(m_pCamera->GetCamera(), GetDlgItem(IDC_ROTATE_VIEW)->GetSafeHwnd());

	m_pAmbient = RpLightCreate(rpLIGHTAMBIENT);

    RpWorldAddLight(m_pWorld, m_pAmbient);
	RpWorldAddCamera(m_pWorld, m_pCamera->GetCamera());

	// dummy box create

	std::string strBox = ".\\Tool\\DTEditor\\cube.dff";
	RwReal fAxisBoxScale = 0.3f;
	RwReal fBoxScale = 2.0f;
	
	m_pMClump = CreateClump(strBox.c_str());
    if(m_pMClump)
    {
		RwV3d vScale = { fBoxScale, fBoxScale, fBoxScale };
		RwFrame *pFrame = RpClumpGetFrame( m_pMClump ); 
		RwMatrix *pMatrix = RwFrameGetMatrix( pFrame );
		RwMatrixScale( pMatrix, &vScale, rwCOMBINEREPLACE );

		RwFrameUpdateObjects( pFrame );
	    RpWorldAddClump(m_pWorld, m_pMClump);
	}

	m_pAxisXClump = CreateClump(".\\Tool\\DTEditor\\X.dff");
    if(m_pAxisXClump)
    {
		RwReal fAxisScale = m_fGridScale * m_fAxisOffset;

		RwV3d vScale = { fAxisBoxScale, fAxisBoxScale, fAxisBoxScale };
		RwV3d vPos = { fAxisScale, 0.0f, 0.0f };
		
		RwFrame *pFrame = RpClumpGetFrame( m_pAxisXClump ); 
		RwMatrix *pMatrix = RwFrameGetMatrix( pFrame );
		RwMatrixScale( pMatrix, &vScale, rwCOMBINEREPLACE );
		RwMatrixTranslate( pMatrix, &vPos, rwCOMBINEPOSTCONCAT );

		RwFrameUpdateObjects( pFrame );
	    RpWorldAddClump(m_pWorld, m_pAxisXClump);
	}

	m_pAxisYClump = CreateClump(".\\Tool\\DTEditor\\Y.dff");
    if(m_pAxisYClump)
    {
		RwReal fAxisScale = m_fGridScale * m_fAxisOffset;

		RwV3d vScale = { fAxisBoxScale, fAxisBoxScale, fAxisBoxScale };
		RwV3d vPos = { 0.0f, fAxisScale, 0.0f };

		RwFrame *pFrame = RpClumpGetFrame( m_pAxisYClump ); 
		RwMatrix *pMatrix = RwFrameGetMatrix( pFrame );
		RwMatrixScale( pMatrix, &vScale, rwCOMBINEREPLACE );
		RwMatrixTranslate( pMatrix, &vPos, rwCOMBINEPOSTCONCAT );

		RwFrameUpdateObjects( pFrame );
	    RpWorldAddClump(m_pWorld, m_pAxisYClump);
	}

	m_pAxisZClump = CreateClump(".\\Tool\\DTEditor\\Z.dff");
    if(m_pAxisZClump)
    {
		RwReal fAxisScale = m_fGridScale * m_fAxisOffset;

		RwV3d vScale = { fAxisBoxScale, fAxisBoxScale, fAxisBoxScale };
		RwV3d vPos = { 0.0f, 0.0f, fAxisScale };

		RwFrame *pFrame = RpClumpGetFrame( m_pAxisZClump ); 
		RwMatrix *pMatrix = RwFrameGetMatrix( pFrame );
		RwMatrixScale( pMatrix, &vScale, rwCOMBINEREPLACE );
		RwMatrixTranslate( pMatrix, &vPos, rwCOMBINEPOSTCONCAT );

		RwFrameUpdateObjects( pFrame );
	    RpWorldAddClump(m_pWorld, m_pAxisZClump);
	}

	/*

	 if (Ambient)
    {
        RpWorldRemoveLight(World, Ambient);
        RpLightDestroy(Ambient);
        Ambient = NULL;
    }
	*/
}

RpClump* CPointRotateEditDlg::CreateClump(const RwChar *filename)
{
	RwStream    *stream = NULL;
    RpClump     *clump = NULL;
    /* Open stream */
    
    stream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, filename);
    if (stream)
    {
        /* Find clump chunk */
        if (RwStreamFindChunk(stream, rwID_CLUMP, NULL, NULL))
        {
            /* Load clump */
            clump = RpClumpStreamRead(stream);
        }

        /* close the stream */
        RwStreamClose( stream, NULL );
    }

    return (clump);
}

void CPointRotateEditDlg::CreateBoxPlane(void)
{
	// 앞뒤
	RwV3d vPoint, vNormal;

	CNtlMath::MathRwV3dAssign(&vPoint, 0.0f, 0.0f, -1.0f);
	CNtlMath::MathRwV3dAssign(&vNormal, 0.0f, 0.0f, 1.0f);
	m_planeBox[EDIT_BOX_PLANE_FRONT].PlaneFromPointNormal(&vPoint, &vNormal);

	CNtlMath::MathRwV3dAssign(&vPoint, 0.0f, 0.0f, 1.0f);
	CNtlMath::MathRwV3dAssign(&vNormal, 0.0f, 0.0f, -1.0f);
	m_planeBox[EDIT_BOX_PLANE_BACK].PlaneFromPointNormal(&vPoint, &vNormal);

	CNtlMath::MathRwV3dAssign(&vPoint, 1.0f, 0.0f, 0.0f);
	CNtlMath::MathRwV3dAssign(&vNormal, -1.0f, 0.0f, 0.0f);
	m_planeBox[EDIT_BOX_PLANE_LEFT].PlaneFromPointNormal(&vPoint, &vNormal);

	CNtlMath::MathRwV3dAssign(&vPoint, -1.0f, 0.0f, 0.0f);
	CNtlMath::MathRwV3dAssign(&vNormal, 1.0f, 0.0f, 0.0f);
	m_planeBox[EDIT_BOX_PLANE_RIGHT].PlaneFromPointNormal(&vPoint, &vNormal);

	CNtlMath::MathRwV3dAssign(&vPoint, 0.0f, 1.0f, 0.0f);
	CNtlMath::MathRwV3dAssign(&vNormal, 0.0f, -1.0f, 0.0f);
	m_planeBox[EDIT_BOX_PLANE_UP].PlaneFromPointNormal(&vPoint, &vNormal);

	CNtlMath::MathRwV3dAssign(&vPoint, 0.0f, -1.0f, 0.0f);
	CNtlMath::MathRwV3dAssign(&vNormal, 0.0f, 1.0f, 0.0f);
	m_planeBox[EDIT_BOX_PLANE_DOWN].PlaneFromPointNormal(&vPoint, &vNormal);
}


void CPointRotateEditDlg::DestroyWorld(void)
{
}

void CPointRotateEditDlg::DestroyCamera(void)
{
}

void CPointRotateEditDlg::ResetCamera(void)
{
	m_fMinDist	= 0.5f;
	m_fMaxDist	= 8.0f;
	m_fDist		= 8.0f;

	m_fPitch	= 20.0f;	
	m_fMinPitch = -89.0f;
	m_fMaxPitch = 75.0f;
	m_fYaw		= 0.0f;
	m_fRoll		= 0.0f;

	m_vUp.x		= 0.0f;
	m_vUp.y		= -1.0f;
	m_vUp.z		= 0.0f;
}


void CPointRotateEditDlg::UpdateCamera(void)
{
	RwMatrixSetIdentity(&m_matTrans);

	RwV3d vPos, vCamPos, vLookAt;
	
	CNtlMath::MathRwV3dAssign(&vPos, 0.0f, 0.0f, m_fDist); 
	CNtlMath::MathRwV3dAssign(&vLookAt, 0.0f, 0.0f, 0.0f); 

	RwMatrixTranslate (&m_matTrans, &vPos, rwCOMBINEREPLACE);									// Position
	RwMatrixRotate (&m_matTrans, &CNtlPLGlobal::m_vXAxisV3, m_fPitch, rwCOMBINEPOSTCONCAT);		// Pitch
	RwMatrixRotate (&m_matTrans, &CNtlPLGlobal::m_vYAxisV3, m_fYaw, rwCOMBINEPOSTCONCAT);			// Yaw
	RwMatrixUpdate(&m_matTrans);

	CNtlMath::MathRwV3dAssign(&vCamPos, vLookAt.x - m_matTrans.pos.x, vLookAt.y - m_matTrans.pos.y, vLookAt.z - m_matTrans.pos.z); 

	API_PL_CameraLookAt(m_pCamera->GetCamera(), &vCamPos, &vLookAt, &m_vUp, m_fRoll);
}

void CPointRotateEditDlg::AddPitch(RwReal fDeltaPitch)
{
	m_fPitch += fDeltaPitch;

	if(m_fPitch < m_fMinPitch) 
		m_fPitch = m_fMinPitch;

	if(m_fPitch > m_fMaxPitch) 
		m_fPitch = m_fMaxPitch;
}

void CPointRotateEditDlg::AddYaw(RwReal fDeltaYaw)
{
	m_fYaw += fDeltaYaw;
		
	// 카메라 각도는 0 ~ 360 을 사용한다. 
	if(m_fYaw >= 360.0f)
		m_fYaw -= 360.0f;

	if(m_fYaw < 0.0f) 
		m_fYaw += 360.0f;
}

void CPointRotateEditDlg::ZoomInOut(RwReal fDelta)
{
	m_fDist += fDelta;

	if (m_fDist < m_fMinDist) 
		m_fDist = m_fMinDist;
	if (m_fDist > m_fMaxDist) 
		m_fDist = m_fMaxDist;
}

void CPointRotateEditDlg::Picking(RwReal fScreenPosX, RwReal fScreenPosY)
{
#define ROTATE_EDIT_RAY_DIST	100.0f

	g_PickEditData.bSuccess = FALSE;
	g_PickEditData.fDist	= FLT_MAX;

	RwV2d vScreenPos;
	vScreenPos.x = fScreenPosX;
	vScreenPos.y = fScreenPosY;

	RwLine CameraRay;
	RwCameraCalcPixelRay( m_pCamera->GetCamera(), &CameraRay, &vScreenPos );

	RpIntersection Intersection;

	Intersection.t.line	= CameraRay;
	Intersection.type = rpINTERSECTLINE;    

	RwV3d vRayDir;
	RwV3dSubMacro(&vRayDir, &Intersection.t.line.end, &Intersection.t.line.start);
	RwV3dNormalize(&vRayDir, &vRayDir);

	Intersection.t.line.end.x = Intersection.t.line.start.x + vRayDir.x*ROTATE_EDIT_RAY_DIST;
	Intersection.t.line.end.y = Intersection.t.line.start.y + vRayDir.y*ROTATE_EDIT_RAY_DIST;
	Intersection.t.line.end.z = Intersection.t.line.start.z + vRayDir.z*ROTATE_EDIT_RAY_DIST;

	RpWorldForAllAtomicIntersections( m_pWorld, &Intersection, PickEdit_IntersectionLineAtomicCB, NULL);

	if(g_PickEditData.bSuccess && m_btnSnap.GetCheck())
	{
		RwV3d vAt, vSplit;
		RwV3dNormalize(&vAt, &g_PickEditData.vPickPos);
		
		RwInt32 iPlaneId = FindRotatePlaneSplitPoint(vAt, vSplit);
		RwReal fInterval = 1.0f / (RwReal)m_nGridNum;
		RwReal fHalfInterval = fInterval / 2.0f;

		int x, y, z;
		RwReal fAx, fAy, fAz;

		if(iPlaneId == EDIT_BOX_PLANE_FRONT || iPlaneId == EDIT_BOX_PLANE_BACK)
		{
			x = (int)(g_PickEditData.vPickPos.x / fInterval);
			y = (int)(g_PickEditData.vPickPos.y / fInterval);
			
			fAx = x * fInterval;
			fAy = y * fInterval;

			if(fabs(g_PickEditData.vPickPos.x - fAx) > fHalfInterval)
			{
				if( g_PickEditData.vPickPos.x > 0 )
					x += 1;
				else
					x -= 1;
			}
			if(fabs(g_PickEditData.vPickPos.y - fAy) > fHalfInterval)
			{
				if( g_PickEditData.vPickPos.y > 0 )
					y += 1;
				else
					y -= 1;
			}

			g_PickEditData.vPickPos.x = x * fInterval;
			g_PickEditData.vPickPos.y = y * fInterval;
		}
		else if(iPlaneId == EDIT_BOX_PLANE_LEFT || iPlaneId == EDIT_BOX_PLANE_RIGHT)
		{
			y = (int)(g_PickEditData.vPickPos.y / fInterval);
			z = (int)(g_PickEditData.vPickPos.z / fInterval);
			
			fAy = y * fInterval;
			fAz = z * fInterval;

			if(fabs(g_PickEditData.vPickPos.y - fAy) > fHalfInterval)
			{
				if( g_PickEditData.vPickPos.y > 0 )
					y += 1;
				else
					y -= 1;
			}
			if(fabs(g_PickEditData.vPickPos.z - fAz) > fHalfInterval)
			{
				if( g_PickEditData.vPickPos.z > 0 )
					z += 1;
				else
					z -= 1;
			}

			g_PickEditData.vPickPos.y = y * fInterval;
			g_PickEditData.vPickPos.z = z * fInterval;
		}
		else
		{
			x = (int)(g_PickEditData.vPickPos.x / fInterval);
			z = (int)(g_PickEditData.vPickPos.z / fInterval);
			
			fAx = x * fInterval;
			fAz = z * fInterval;

			if(fabs(g_PickEditData.vPickPos.x - fAx) > fHalfInterval)
			{
				if( g_PickEditData.vPickPos.x > 0 )
					x += 1;
				else
					x -= 1;
			}
			if(fabs(g_PickEditData.vPickPos.z - fAz) > fHalfInterval)
			{
				if( g_PickEditData.vPickPos.z > 0 )
					z += 1;
				else
					z -= 1;
			}

			g_PickEditData.vPickPos.x = x * fInterval;
			g_PickEditData.vPickPos.z = z * fInterval;
		}
	}
}

RwInt32 CPointRotateEditDlg::FindRotatePlaneSplitPoint(RwV3d vAt, RwV3d& vSplit)
{
	RwInt32 iPlaneId = EDIT_BOX_PLANE_NONE;

	RwReal fMin = FLT_MAX;
	RwReal fDist;
	RwReal fDotValue;

	RwV3d vDir = vAt;
	RwV3d vSplitPos;
	RwV3d vFrom = { 0.0f, 0.0f, 0.0f };
	CNtlMath::MathRwV3dAssign(&vAt, vAt.x * 100.0f, vAt.y * 100.0f, vAt.z * 100.f);

	for(RwInt32 i = 0; i < 6; i++)
	{
		vSplitPos = m_planeBox[i].SplitPoint(&vFrom, &vAt);
		fDotValue = RwV3dDotProduct(&vDir, &m_planeBox[i].n);
		fDist = RwV3dLength(&vSplitPos);
		
		if(fDotValue <= 0.0f && fDist < fMin)
		{
			fMin		= fDist;
			vSplit		= vSplitPos;
			iPlaneId	= i;
		}
	}

	return iPlaneId;
}

void CPointRotateEditDlg::AxisRender(void)
{
	RwReal fAxisScale = m_fGridScale * m_fAxisOffset;

	RwIm3DVertex AxisXVertex[2];
	RwIm3DVertex AxisYVertex[2];
	RwIm3DVertex AxisZVertex[2];

	RwInt32 iIdx;

	// x-axis
	iIdx = 0;

	RwIm3DVertexSetPos(&AxisXVertex[iIdx], -fAxisScale, 0.0f, 0.0f);
	RwIm3DVertexSetNormal(&AxisXVertex[iIdx], 0.0f, 1.0f, 0.0f);
	RwIm3DVertexSetRGBA(&AxisXVertex[iIdx], 255, 0, 0, 100);
	AxisXVertex[iIdx].u = 0.0f;
	AxisXVertex[iIdx].v = 0.0f;

	iIdx = 1;

	RwIm3DVertexSetPos(&AxisXVertex[iIdx], fAxisScale, 0.0f, 0.0f);
	RwIm3DVertexSetNormal(&AxisXVertex[iIdx], 0.0f, 1.0f, 0.0f);
	RwIm3DVertexSetRGBA(&AxisXVertex[iIdx], 255, 0, 0, 100);
	AxisXVertex[iIdx].u = 0.0f;
	AxisXVertex[iIdx].v = 0.0f;

	// x-axis
	iIdx = 0;

	RwIm3DVertexSetPos(&AxisYVertex[iIdx], 0.0f, -fAxisScale, 0.0f);
	RwIm3DVertexSetNormal(&AxisYVertex[iIdx], 0.0f, 1.0f, 0.0f);
	RwIm3DVertexSetRGBA(&AxisYVertex[iIdx], 0, 255, 0, 100);
	AxisYVertex[iIdx].u = 0.0f;
	AxisYVertex[iIdx].v = 0.0f;

	iIdx = 1;

	RwIm3DVertexSetPos(&AxisYVertex[iIdx], 0.0f, fAxisScale, 0.0f);
	RwIm3DVertexSetNormal(&AxisYVertex[iIdx], 0.0f, 1.0f, 0.0f);
	RwIm3DVertexSetRGBA(&AxisYVertex[iIdx], 0, 255, 0, 100);
	AxisYVertex[iIdx].u = 0.0f;
	AxisYVertex[iIdx].v = 0.0f;

	// z-axis
	iIdx = 0;

	RwIm3DVertexSetPos(&AxisZVertex[iIdx], 0.0f, 0.0f, -fAxisScale);
	RwIm3DVertexSetNormal(&AxisZVertex[iIdx], 0.0f, 1.0f, 0.0f);
	RwIm3DVertexSetRGBA(&AxisZVertex[iIdx], 0, 0, 255, 100);
	AxisZVertex[iIdx].u = 0.0f;
	AxisZVertex[iIdx].v = 0.0f;

	iIdx = 1;

	RwIm3DVertexSetPos(&AxisZVertex[iIdx], 0.0f, 0.0f, fAxisScale);
	RwIm3DVertexSetNormal(&AxisZVertex[iIdx], 0.0f, 1.0f, 0.0f);
	RwIm3DVertexSetRGBA(&AxisZVertex[iIdx], 0, 0, 255, 100);
	AxisZVertex[iIdx].u = 0.0f;
	AxisZVertex[iIdx].v = 0.0f;


	RwV3d vPos;
	CNtlMath::MathRwV3dAssign(&vPos, 0.0f, 0.0f, 0.0f);

	RwMatrix mat;
	RwMatrixSetIdentity(&mat);
	RwMatrixTranslate( &mat, &vPos, rwCOMBINEPOSTCONCAT );
	
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void *)NULL);

	if(RwIm3DTransform(AxisXVertex, 2, &mat, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA))
	{
		RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);        
		RwIm3DEnd();
	}

	if(RwIm3DTransform(AxisYVertex, 2, &mat, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA))
	{
		RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);        
		RwIm3DEnd();
	}

	if(RwIm3DTransform(AxisZVertex, 2, &mat, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA))
	{
		RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);        
		RwIm3DEnd();
	}
}

void CPointRotateEditDlg::GridRender(void)
{
	RwInt32 iIdx = 0;
	RwIm3DVertex Vertex[2];

	// x-axis
	iIdx = 0;

	RwIm3DVertexSetPos(&Vertex[iIdx], -1.0f, 0.0f, 0.0f);
	RwIm3DVertexSetNormal(&Vertex[iIdx], 0.0f, 1.0f, 0.0f);
	RwIm3DVertexSetRGBA(&Vertex[iIdx], 200, 200, 200, 255);
	Vertex[iIdx].u = 0.0f;
	Vertex[iIdx].v = 0.0f;

	iIdx = 1;

	RwIm3DVertexSetPos(&Vertex[iIdx], 1.0f, 0.0f, 0.0f);
	RwIm3DVertexSetNormal(&Vertex[iIdx], 0.0f, 1.0f, 0.0f);
	RwIm3DVertexSetRGBA(&Vertex[iIdx], 200, 200, 200, 255);
	Vertex[iIdx].u = 0.0f;
	Vertex[iIdx].v = 0.0f;

	RwReal fAxisScale = m_fGridScale + 0.01f;

	RwV3d vPos;
	CNtlMath::MathRwV3dAssign(&vPos, 0.0f, 0.0f, 0.0f);

	RwMatrix mat;
	RwMatrixSetIdentity(&mat);
	RwMatrixTranslate( &mat, &vPos, rwCOMBINEPOSTCONCAT );
	
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void *)NULL);

	for(RwInt32 i = -m_nGridNum; i <= m_nGridNum; i++)
	{
		if( i == 0 )
		{
			RwIm3DVertexSetRGBA(&Vertex[0], 200, 200, 10, 255);
			RwIm3DVertexSetRGBA(&Vertex[1], 200, 200, 10, 255);
		}
		else
		{
			RwIm3DVertexSetRGBA(&Vertex[0], 150, 150, 150, 255);
			RwIm3DVertexSetRGBA(&Vertex[1], 150, 150, 150, 255);
		}

		// 전후면 (가로)
		RwIm3DVertexSetPos(&Vertex[0], -fAxisScale, fAxisScale*i/(RwReal)m_nGridNum, fAxisScale);
		RwIm3DVertexSetPos(&Vertex[1], fAxisScale,  fAxisScale*i/(RwReal)m_nGridNum, fAxisScale);

		if(RwIm3DTransform(Vertex, 2, &mat, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA))
		{
			RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);        
			RwIm3DEnd();
		}

		RwIm3DVertexSetPos(&Vertex[0], -fAxisScale, fAxisScale*i/(RwReal)m_nGridNum, -fAxisScale);
		RwIm3DVertexSetPos(&Vertex[1], fAxisScale,  fAxisScale*i/(RwReal)m_nGridNum, -fAxisScale);

		if(RwIm3DTransform(Vertex, 2, &mat, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA))
		{
			RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);        
			RwIm3DEnd();
		}

		// 좌우면 (가로)

		RwIm3DVertexSetPos(&Vertex[0], -fAxisScale, fAxisScale*i/(RwReal)m_nGridNum, -fAxisScale);
		RwIm3DVertexSetPos(&Vertex[1], -fAxisScale,  fAxisScale*i/(RwReal)m_nGridNum, fAxisScale);

		if(RwIm3DTransform(Vertex, 2, &mat, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA))
		{
			RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);        
			RwIm3DEnd();
		}

		RwIm3DVertexSetPos(&Vertex[0], fAxisScale, fAxisScale*i/(RwReal)m_nGridNum, -fAxisScale);
		RwIm3DVertexSetPos(&Vertex[1], fAxisScale,  fAxisScale*i/(RwReal)m_nGridNum, fAxisScale);

		if(RwIm3DTransform(Vertex, 2, &mat, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA))
		{
			RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);        
			RwIm3DEnd();
		}

		// 상하면 (가로)

		RwIm3DVertexSetPos(&Vertex[0], -fAxisScale, fAxisScale, fAxisScale*i/(RwReal)m_nGridNum);
		RwIm3DVertexSetPos(&Vertex[1], fAxisScale,  fAxisScale, fAxisScale*i/(RwReal)m_nGridNum);

		if(RwIm3DTransform(Vertex, 2, &mat, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA))
		{
			RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);        
			RwIm3DEnd();
		}

		RwIm3DVertexSetPos(&Vertex[0], -fAxisScale, -fAxisScale, fAxisScale*i/(RwReal)m_nGridNum);
		RwIm3DVertexSetPos(&Vertex[1], fAxisScale,  -fAxisScale, fAxisScale*i/(RwReal)m_nGridNum);

		if(RwIm3DTransform(Vertex, 2, &mat, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA))
		{
			RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);        
			RwIm3DEnd();
		}

		// 전후면 (세로)
		RwIm3DVertexSetPos(&Vertex[0], fAxisScale*i/(RwReal)m_nGridNum, -fAxisScale, fAxisScale);
		RwIm3DVertexSetPos(&Vertex[1], fAxisScale*i/(RwReal)m_nGridNum, fAxisScale, fAxisScale);

		if(RwIm3DTransform(Vertex, 2, &mat, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA))
		{
			RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);        
			RwIm3DEnd();
		}

		RwIm3DVertexSetPos(&Vertex[0], fAxisScale*i/(RwReal)m_nGridNum, -fAxisScale, -fAxisScale);
		RwIm3DVertexSetPos(&Vertex[1], fAxisScale*i/(RwReal)m_nGridNum, fAxisScale, -fAxisScale);

		if(RwIm3DTransform(Vertex, 2, &mat, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA))
		{
			RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);        
			RwIm3DEnd();
		}

		// 좌우면 (세로)
		RwIm3DVertexSetPos(&Vertex[0], fAxisScale, -fAxisScale, fAxisScale*i/(RwReal)m_nGridNum);
		RwIm3DVertexSetPos(&Vertex[1], fAxisScale, fAxisScale, fAxisScale*i/(RwReal)m_nGridNum);

		if(RwIm3DTransform(Vertex, 2, &mat, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA))
		{
			RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);        
			RwIm3DEnd();
		}

		RwIm3DVertexSetPos(&Vertex[0], -fAxisScale, -fAxisScale, fAxisScale*i/(RwReal)m_nGridNum);
		RwIm3DVertexSetPos(&Vertex[1], -fAxisScale, fAxisScale, fAxisScale*i/(RwReal)m_nGridNum);

		if(RwIm3DTransform(Vertex, 2, &mat, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA))
		{
			RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);        
			RwIm3DEnd();
		}

		// 상하면 (세로)
		RwIm3DVertexSetPos(&Vertex[0], fAxisScale*i/(RwReal)m_nGridNum, fAxisScale, -fAxisScale);
		RwIm3DVertexSetPos(&Vertex[1], fAxisScale*i/(RwReal)m_nGridNum, fAxisScale, fAxisScale);

		if(RwIm3DTransform(Vertex, 2, &mat, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA))
		{
			RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);        
			RwIm3DEnd();
		}

		RwIm3DVertexSetPos(&Vertex[0], fAxisScale*i/(RwReal)m_nGridNum, -fAxisScale, -fAxisScale);
		RwIm3DVertexSetPos(&Vertex[1], fAxisScale*i/(RwReal)m_nGridNum, -fAxisScale, fAxisScale);

		if(RwIm3DTransform(Vertex, 2, &mat, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA))
		{
			RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);        
			RwIm3DEnd();
		}
	}
}

void CPointRotateEditDlg::PlaneRender(void)
{
	return;

	RwD3D9SetTexture(NULL, 0);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)rwBLENDSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)rwBLENDINVSRCALPHA);

	RwIm3DVertex Vertices[4];

	RwIm3DVertexSetRGBA(&Vertices[0], 200, 200, 200, 255);
	RwIm3DVertexSetRGBA(&Vertices[1], 200, 200, 200, 255);
	RwIm3DVertexSetRGBA(&Vertices[2], 200, 200, 200, 255);
	RwIm3DVertexSetRGBA(&Vertices[3], 200, 200, 200, 255);
	
	RwIm3DVertexSetU (&Vertices[0], 0.0f);
	RwIm3DVertexSetV (&Vertices[0], 0.0f);

	RwIm3DVertexSetU (&Vertices[1], 0.0f);
	RwIm3DVertexSetV (&Vertices[1], 1.0f);

	RwIm3DVertexSetU (&Vertices[2], 1.0f);
	RwIm3DVertexSetV (&Vertices[2], 0.0f);

	RwIm3DVertexSetU (&Vertices[3], 1.0f);
	RwIm3DVertexSetV (&Vertices[3], 1.0f);

	// plane 1.
	RwIm3DVertexSetPos(&Vertices[0], m_fGridScale,  -m_fGridScale, 0.0f);
	RwIm3DVertexSetPos(&Vertices[1], m_fGridScale,  m_fGridScale,  0.0f);
	RwIm3DVertexSetPos(&Vertices[2], -m_fGridScale, -m_fGridScale, 0.0f);
	RwIm3DVertexSetPos(&Vertices[3], -m_fGridScale, m_fGridScale,  0.0f);
		
	RwMatrix mat;
	RwMatrixSetIdentity(&mat);

	if( RwIm3DTransform(Vertices, 4, &mat, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXUV | rwIM3D_VERTEXRGBA) )
    {                         
        RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);  
        
        RwIm3DEnd();
    }

	RwD3D9SetTexture(NULL, 0);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)FALSE);
	RwRenderStateSet(rwRENDERSTATESRCBLEND,(void *)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND,(void*)rwBLENDINVSRCALPHA);
}

void CPointRotateEditDlg::SelectPointRender(void)
{
	CNtlDTProp *pDTProp = GetActiveDTProp();

	if(pDTProp == NULL)
		return;

	int nSelSx, nSelEx;

	GetActiveSelectControlPoint(nSelSx, nSelEx);

	if(nSelSx < 0)
		return;

	CNtlDTSpline *pDTSpline = (CNtlDTSpline*)pDTProp;

	RwV3d vAt = pDTSpline->GetSplineControlPointRotate(nSelSx);

	// 6개 plane

	RwV3d vSplit;
	FindRotatePlaneSplitPoint(vAt, vSplit);

	// select point rendering
	RwRGBA rgb;
	rgb.red		= 50;
	rgb.green	= 255;
	rgb.blue	= 50;
	rgb.alpha	= 255;

	RwMatrix mat;
	RwMatrixSetIdentity(&mat);

	PointBoxRender(&vSplit, &rgb, &mat, 0.04f);
}


void CPointRotateEditDlg::PointBoxRender(const RwV3d *pPos, const RwRGBA *pColor, RwMatrix *pWorldMatrix, RwReal fOffset)
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


void CPointRotateEditDlg::Render(float fElapsed)
{
	if(m_pCamera == NULL)
		return;

	// camear update

	UpdateCamera();

	// camera rendering

	if (m_pCamera->CameraClear(&m_BackgroundColor, rwCAMERACLEARZ|rwCAMERACLEARIMAGE))
	{
		if( m_pCamera->MainCameraBeginUpdate(fElapsed) )
		{
			RpWorldRender(m_pWorld);

			AxisRender();
			PlaneRender();
			GridRender();
			SelectPointRender();

			m_pCamera->MainCameraEndUpdate();
		}

		m_pCamera->CameraShowRaster(GetDlgItem(IDC_ROTATE_VIEW)->GetSafeHwnd(), 0);
	}
}

int CPointRotateEditDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	

	return 0;
}

void CPointRotateEditDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	if (m_pCamera)
	{
		RwRect rt;
		rt.x = 0; rt.y = 0; rt.w = cx; rt.h = cy;
		m_pCamera->SetCameraSize(&rt); 
	}
}

void CPointRotateEditDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(nFlags & MK_CONTROL)
	{
		CNtlDTProp *pDTProp = GetActiveDTProp();
		if(pDTProp == NULL)
			return CDialog::OnLButtonDown(nFlags, point);
	
		int nSelSx, nSelEx;
		GetActiveSelectControlPoint(nSelSx, nSelEx);

		if(nSelSx < 0)
			return CDialog::OnLButtonDown(nFlags, point);
		
		CRect rtClient;
		GetDlgItem(IDC_ROTATE_VIEW)->GetClientRect(rtClient);

		// 좌표 구하기.
		CPoint ptDlg, ptEdit;
		ClientToScreen(&ptDlg);
		GetDlgItem(IDC_ROTATE_VIEW)->ClientToScreen(&ptEdit);
		ptEdit = ptEdit - ptDlg; 

		Picking( (RwReal)point.x - ptEdit.x, (RwReal)point.y - ptEdit.y);

		if(g_PickEditData.bSuccess)
		{
			RwReal fLen = RwV3dLength(&g_PickEditData.vPickPos);

			static RwV3d vLimit = { 1.0f, 1.0f, 1.0f };
			static RwReal fLimitLen = RwV3dLength(&vLimit);

			if(fLen <= fLimitLen)
			{
				CNtlDTSpline *pDTSpline = (CNtlDTSpline*)pDTProp;

				pDTSpline->SetCurveRotate(nSelSx, nSelEx, g_PickEditData.vPickPos);
			}
		}
	}
	else
	{
		m_bLBtnClick		= TRUE;
		m_ptOldLBtnMouse	= point;

		SetCapture();
	}

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnLButtonDown(nFlags, point);
}

void CPointRotateEditDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_bLBtnClick)
		ReleaseCapture();

	m_bLBtnClick = FALSE;

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnLButtonUp(nFlags, point);
}

void CPointRotateEditDlg::OnMouseMove(UINT nFlags, CPoint point)
{
#define NTL_MOUSE_SENSITIVITY_SCALE	0.5f

	if(m_bLBtnClick)
	{
		RwReal fDeltaYaw = ((RwReal)(m_ptOldLBtnMouse.x - point.x) * NTL_MOUSE_SENSITIVITY_SCALE);
		RwReal fDeltaPitch = ((RwReal)(m_ptOldLBtnMouse.y - point.y) * NTL_MOUSE_SENSITIVITY_SCALE);
		
		AddYaw(fDeltaYaw);
		AddPitch(-fDeltaPitch);

		m_ptOldLBtnMouse = point;
	}

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnMouseMove(nFlags, point);
}

BOOL CPointRotateEditDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
#define NTL_ZOOM_INOUT_SENSITIVITY	0.005f
	
	RwReal fDelta = -(RwReal)zDelta * NTL_ZOOM_INOUT_SENSITIVITY;

	ZoomInOut(fDelta);

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CPointRotateEditDlg::OnBnClickedPointrotateeditResetCamera()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	ResetCamera();
}

void CPointRotateEditDlg::OnEnChangePointrotateeditGridNum()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정하고  마스크에 OR 연산하여 설정된
	// ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출해야만
	// 해당 알림 메시지를 보냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);
}

void CPointRotateEditDlg::OnEnKillfocusPointrotateeditGridNum()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	if(m_nGridNum < 0)
		m_nGridNum = 1;
}

void CPointRotateEditDlg::OnBnClickedPointrotateeditBack()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CNtlDTProp *pDTProp = GetActiveDTProp();
	if(pDTProp == NULL)
		return;

	int nSelSx, nSelEx;

	GetActiveSelectControlPoint(nSelSx, nSelEx);

	if(nSelSx < 0)
		return;

	RwV3d vAt;

	std::list<RwV3d>::iterator it;
	std::list<RwV3d> listRot, listNewRot;

	CNtlDTSpline *pDTSpline = (CNtlDTSpline*)pDTProp;

	for(int i = nSelSx; i < nSelEx; i++)
	{
		pDTSpline->GetSplineControlPointRotate(i, listRot);
		for(it = listRot.begin(); it != listRot.end(); it++)
		{
			vAt = (*it);
			RwV3dScale(&vAt, &vAt, -1.0f);
			listNewRot.push_back(vAt);
			pDTSpline->SetCurveRotate(i, listNewRot);
		}

		listRot.clear();
		listNewRot.clear();
	}
}

void CPointRotateEditDlg::OnBnClickedPointrotateeditPointDirection()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CNtlDTProp *pDTProp = GetActiveDTProp();
	if(pDTProp == NULL)
		return;

	int nSelSx, nSelEx;

	GetActiveSelectControlPoint(nSelSx, nSelEx);

	if(nSelSx < 0)
		return;

	CNtlDTSpline *pDTSpline = (CNtlDTSpline*)pDTProp;
	pDTSpline->SetCurvePointDirectRotate(nSelSx, nSelEx);
}

void CPointRotateEditDlg::OnBnClickedPointrotateeditInterpolation()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CNtlDTProp *pDTProp = GetActiveDTProp();
	if(pDTProp == NULL)
		return;

	int nSelSx, nSelEx;

	GetActiveSelectControlPoint(nSelSx, nSelEx);

	if(nSelSx < 0)
		return;

	CNtlDTSpline *pDTSpline = (CNtlDTSpline*)pDTProp;
	pDTSpline->SetCurveRotateInterpolation(nSelSx, nSelEx);
}

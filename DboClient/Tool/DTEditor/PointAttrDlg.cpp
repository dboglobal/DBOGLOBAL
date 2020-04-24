// PointAttrDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DTEditor.h"
#include "PointAttrDlg.h"
#include "MainFrm.h"
#include "DTEditorGlobal.h"
#include "PropertyPane.h"
#include "PointEditDlg.h"

// simulation
#include "precomp_ntlsimulation.h"
#include "NtlDTProp.h"
#include "NtlCameraManager.h"



// CPointAttrDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPointAttrDlg, CDialog)

CPointAttrDlg::CPointAttrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPointAttrDlg::IDD, pParent)
	, m_fOffsetY(0)
{
	m_nEditType = POINT_ATTR_CAMERA;
}

CPointAttrDlg::~CPointAttrDlg()
{
}

void CPointAttrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_POINTATTR_POINT_OFFSETY, m_editOffsetY);
	DDX_Text(pDX, IDC_POINTATTR_POINT_OFFSETY, m_fOffsetY);
	DDX_Control(pDX, IIDC_POINTATTR_COMBO_TYPE, m_comboType);
	DDX_Control(pDX, IDC_POINTATTR_CHECK_POS, m_btnCheckPos);
	DDX_Control(pDX, IDC_POINTATTR_CHECK_DIR, m_btnCheckDir);
	DDX_Control(pDX, IDC_POINTATTR_CHECK_CAMERA_MOVE, m_btnCameraMovePointClick);
	DDX_Control(pDX, IDC_POINTATTR_CHECK_CAMERA_COLLISION, m_btnCameraColli);
	DDX_Control(pDX, IDC_POINTATTR_CHECK_AVATAR_COLLISION, m_btnAvatarColli);
}


BEGIN_MESSAGE_MAP(CPointAttrDlg, CDialog)
	ON_BN_CLICKED(IDC_POINTATTR_POINT_POS, &CPointAttrDlg::OnBnClickedPointattrPointPos)
	ON_BN_CLICKED(IDC_POINTATTR_ADD_POINT_POS, &CPointAttrDlg::OnBnClickedPointattrAddPointPos)
	ON_BN_CLICKED(IDC_POINTATTR_POINT_SELECTED_INTERPOLATION, &CPointAttrDlg::OnBnClickedPointattrPointSelectedInterpolation)
	ON_CBN_SELCHANGE(IIDC_POINTATTR_COMBO_TYPE, &CPointAttrDlg::OnCbnSelchangePointattrComboType)
	ON_EN_CHANGE(IDC_POINTATTR_CAMERA_SPEED, &CPointAttrDlg::OnEnChangePointattrCameraSpeed)
	ON_BN_CLICKED(IDC_POINTATTR_CHECK_POS, &CPointAttrDlg::OnBnClickedPointattrCheckPos)
	ON_BN_CLICKED(IDC_POINTATTR_CHECK_DIR, &CPointAttrDlg::OnBnClickedPointattrCheckDir)
	ON_EN_CHANGE(IDC_POINTATTR_POINT_BOX, &CPointAttrDlg::OnEnChangePointattrPointBox)
	ON_BN_CLICKED(IDC_POINTATTR_CHECK_CAMERA_MOVE, &CPointAttrDlg::OnBnClickedPointattrCheckCameraMove)
	ON_BN_CLICKED(IDC_POINTATTR_CHECK_CAMERA_COLLISION, &CPointAttrDlg::OnBnClickedPointattrCheckCameraCollision)
	ON_BN_CLICKED(IDC_POINTATTR_CHECK_AVATAR_COLLISION, &CPointAttrDlg::OnBnClickedPointattrCheckAvatarCollision)
	ON_BN_CLICKED(IDC_POINTATTR_RESET_AVATAR_DIR, &CPointAttrDlg::OnBnClickedPointattrResetAvatarDir)
	ON_BN_CLICKED(IDC_POINT_CUSTOM_DIALOG, &CPointAttrDlg::OnBnClickedPointCustomDialog)
END_MESSAGE_MAP()


void CPointAttrDlg::OnPropUpdate(void)
{
	m_CustomDlg.OnPropUpdate();
}

void CPointAttrDlg::OnPropSelectUpdate(int nSx, int nEx)
{
	m_CustomDlg.OnPropSelectUpdate(nSx, nEx);
}


// CPointAttrDlg 메시지 처리기입니다.

void CPointAttrDlg::OnBnClickedPointattrPointPos()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int nSelSx, nSelEx;
	GetActiveSelectControlPoint(nSelSx, nSelEx);
	if(nSelSx < 0)
		return;

	CNtlDTProp *pDTProp = GetActiveDTProp();
	if(pDTProp == NULL)
		return;

	CNtlDTSpline *pDTSpline = (CNtlDTSpline*)pDTProp;

	RwV3d vPointPos;
	RwBool bUseWorldPos = pDTSpline->IsUseWorldPosition();

	if(bUseWorldPos)
	{
		RwV3dAssign(&vPointPos, GetNtlGameCameraManager()->GetCameraPos());
	}
	else
	{
		RwV3d vPos = GetSplineStandardPosition();
		RwV3dSubMacro(&vPointPos, GetNtlGameCameraManager()->GetCameraPos(), &vPos);
	}

	RwV3d vAt;
	RwV3dAssign(&vAt, GetNtlGameCameraManager()->GetCameraDir());

	UpdateData(TRUE);

	if(IsChangePointPosition())
		pDTSpline->SetControlPointPosition(nSelSx, nSelEx, vPointPos);

	if(IsChangePointDirection())
		pDTSpline->SetCurveRotate(nSelSx, nSelEx, vAt);

	m_pEditDlg->OnPropSelectUpdate(nSelSx, nSelEx);
}

void CPointAttrDlg::OnBnClickedPointattrAddPointPos()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CNtlDTProp *pDTProp = GetActiveDTProp();
	if(pDTProp == NULL)
		return;

	CNtlDTSpline *pDTSpline = (CNtlDTSpline*)pDTProp;

	const SSplineProp *pSplineProp = pDTSpline->GetSplineProp();
	int nSelSx = pSplineProp->iCtrlNum;

	RwV3d vPointPos;
	RwBool bUseWorldPos = pDTSpline->IsUseWorldPosition();

	if(bUseWorldPos)
	{
		RwV3dAssign(&vPointPos, GetNtlGameCameraManager()->GetCameraPos());
	}
	else
	{
		RwV3d vPos = GetSplineStandardPosition();
		RwV3dSubMacro(&vPointPos, GetNtlGameCameraManager()->GetCameraPos(), &vPos);
	}
	
	pDTSpline->InsertControlPoint(nSelSx, &vPointPos, GetNtlGameCameraManager()->GetCameraDir());

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

	CControlPane *pControlPane = pMainFrame->GetControlPane();
	pControlPane->RebuildProp();

	CPropertyPane *pPropertyPane = pMainFrame->GetPropertyPane();
	pPropertyPane->RefreshSplineProp();

	UpdateData(TRUE);
}

void CPointAttrDlg::OnBnClickedPointattrPointSelectedInterpolation()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nSelSx, nSelEx;
	GetActiveSelectControlPoint(nSelSx, nSelEx);
	if(nSelSx < 0)
		return;

	CNtlDTProp *pDTProp = GetActiveDTProp();
	if(pDTProp == NULL)
		return;

	CNtlDTSpline *pDTSpline = (CNtlDTSpline*)pDTProp;
	pDTSpline->SetControlPointInterpolation(nSelSx, nSelEx);

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

	CControlPane *pControlPane = pMainFrame->GetControlPane();
	pControlPane->SplinePropUpdate();
}

BOOL CPointAttrDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_fOffsetY = 1.0f;

	m_comboType.AddString("Camera");
	m_comboType.AddString("Avatar");
	m_comboType.SetCurSel(0);


	CString strValue;

	// free camera speed
	RwReal fCameraSpeed = GetFreeCameraSpeed();
	strValue.Format("%f", fCameraSpeed);
	SetDlgItemText(IDC_POINTATTR_CAMERA_SPEED, strValue);

	// spline box size
	RwReal fBoxSize = GetSplinePointBoxSize();
	strValue.Format("%f", fBoxSize);
	SetDlgItemText(IDC_POINTATTR_POINT_BOX, strValue);

	// 
	m_btnCheckPos.SetCheck(TRUE);
	m_btnCheckDir.SetCheck(TRUE);
	m_btnCameraColli.SetCheck(TRUE);
	m_btnAvatarColli.SetCheck(TRUE);
	

	// point custom dialog
	m_CustomDlg.Create(IDD_POINTCUSTOMEDIT, NULL);
	m_CustomDlg.ShowWindow(SW_HIDE);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CPointAttrDlg::OnCbnSelchangePointattrComboType()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_nEditType = m_comboType.GetCurSel();
}

void CPointAttrDlg::OnEnChangePointattrCameraSpeed()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정하고  마스크에 OR 연산하여 설정된
	// ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출해야만
	// 해당 알림 메시지를 보냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString strValue;
	GetDlgItemText(IDC_POINTATTR_CAMERA_SPEED, strValue);
	float fCameraSpeed = atof(strValue.GetBuffer(strValue.GetLength()));

	SetFreeCameraSpeed(fCameraSpeed);
}

void CPointAttrDlg::OnBnClickedPointattrCheckPos()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	EnableChangePointPosition(m_btnCheckPos.GetCheck());
}

void CPointAttrDlg::OnBnClickedPointattrCheckDir()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	EnableChangePointDirection(m_btnCheckDir.GetCheck());
}

void CPointAttrDlg::OnEnChangePointattrPointBox()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정하고  마스크에 OR 연산하여 설정된
	// ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출해야만
	// 해당 알림 메시지를 보냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString strValue;
	GetDlgItemText(IDC_POINTATTR_POINT_BOX, strValue);
	float fBoxSize = atof(strValue.GetBuffer(strValue.GetLength()));

	SetSplinePointBoxSize(fBoxSize);
}

void CPointAttrDlg::OnBnClickedPointattrCheckCameraMove()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EnableCameraMovePointClick(m_btnCameraMovePointClick.GetCheck());
}

void CPointAttrDlg::OnBnClickedPointattrCheckCameraCollision()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_btnCameraColli.GetCheck())
	{
		GetNtlGameCameraManager()->SetCollisionCheck(TRUE);
	}
	else
	{
		GetNtlGameCameraManager()->SetCollisionCheck(FALSE);
	}
}


void CPointAttrDlg::OnBnClickedPointattrCheckAvatarCollision()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	RwUInt32 uiFlags = GetNtlSLGlobal()->GetSobAvatar()->GetFlags();

	if(m_btnAvatarColli.GetCheck())
	{
		GetNtlSLGlobal()->GetSobAvatar()->SetFlags(uiFlags | SLFLAG_OBJECT_COLLISION) ;
	}
	else
	{
		GetNtlSLGlobal()->GetSobAvatar()->SetFlags(uiFlags & ~SLFLAG_OBJECT_COLLISION);
	}
}

void CPointAttrDlg::OnBnClickedPointattrResetAvatarDir()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	RwV3d vDir;
	CNtlMath::MathRwV3dAssign(&vDir, 0.0f, 0.0f, 1.0f);

	GetNtlSLGlobal()->GetSobAvatar()->SetDirection(&vDir);
}

void CPointAttrDlg::OnBnClickedPointCustomDialog()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_CustomDlg.ShowWindow(SW_SHOW);
}

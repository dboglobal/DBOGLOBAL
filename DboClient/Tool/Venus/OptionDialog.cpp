// OptionDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Venus.h"
#include "OptionDialog.h"
#include ".\optiondialog.h"

#include "VenusFramework.h"

#include ".\venusconfig.h"
#include ".\venusgrid.h"
#include ".\venuscamera.h"

// COptionDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(COptionDialog, CDialog)
COptionDialog::COptionDialog(CWnd* pParent /*=NULL*/)
	: CDialog(COptionDialog::IDD, pParent)
	, m_fObjectMoveSpeed(0)
	, m_fObjectRotateSpeed(0)
	, m_fObjectScaleSpeed(0)
	, m_fCameraMoveSpeed(0)
	, m_fCameraRotateSpeed(0)
	, m_fCameraWheelSpeed(0)
	, m_strGridWidth(_T(""))
	, m_strGridCount(_T(""))
{
}

COptionDialog::~COptionDialog()
{
}

void COptionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_WIDTH, m_GridWidthSlider);
	DDX_Control(pDX, IDC_GRID_COUNT, m_GridCountSlider);
	DDX_Text(pDX, IDC_EDIT_OBJECT_MOVE_SPEED, m_fObjectMoveSpeed);
	DDX_Text(pDX, IDC_EDIT_OBJECT_ROTATE_SPEED, m_fObjectRotateSpeed);
	DDX_Text(pDX, IDC_EDIT_OBJECT_SCALE_SPEED, m_fObjectScaleSpeed);
	DDX_Control(pDX, IDC_OBJECT_MOVE_SPEED, m_ObjectMoveSpeedSlider);
	DDX_Control(pDX, IDC_OBJECT_ROTATE_SPEED, m_ObjectRotateSpeedSlider);
	DDX_Control(pDX, IDC_OBJECT_SCALE_SPEED, m_ObjectScaleSpeedSlider);
	DDX_Control(pDX, IDC_CAMERA_MOVE_SPEED, m_CameraMoveSpeedSlider);
	DDX_Control(pDX, IDC_CAMERA_ROTATE_SPEED, m_CameraRotateSpeedSlider);
	DDX_Control(pDX, IDC_CAMERA_WHEEL_SPEED, m_CameraWheelSpeedSlider);
	DDX_Text(pDX, IDC_EDIT_CAMERA_MOVE_SPEED, m_fCameraMoveSpeed);
	DDX_Text(pDX, IDC_EDIT_CAMERA_ROTATE_SPEED, m_fCameraRotateSpeed);
	DDX_Text(pDX, IDC_EDIT_CAMERA_WHEEL_SPEED, m_fCameraWheelSpeed);
	DDX_Control(pDX, IDC_BACKGROUND, m_BackGroundButton);
	DDX_Text(pDX, IDC_EDIT_GRID_WIDTH, m_strGridWidth);
	DDX_Text(pDX, IDC_EDIT_GRID_COUNT, m_strGridCount);
}


BEGIN_MESSAGE_MAP(COptionDialog, CDialog)
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BACKGROUND, OnBnClickedBackground)
END_MESSAGE_MAP()


// COptionDialog 메시지 처리기입니다.
COptionDialog& COptionDialog::GetInstance()
{
	static COptionDialog dlg;
	return dlg;
}
BOOL COptionDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CRect rect;
	GetWindowRect(&rect);
	CPoint posStart;
	posStart.x = GetSystemMetrics(SM_CXSCREEN) / 2 - rect.Width() / 2;
	posStart.y = GetSystemMetrics(SM_CYSCREEN) / 2 - rect.Height() / 2;

	rect.left += posStart.x;
	rect.right += posStart.x;
	rect.top += posStart.y;
	rect.bottom += posStart.y;
	MoveWindow(rect);

	// =====================================================================
	// Grid
	// =====================================================================
	m_GridWidthSlider.SetRange(1, 50);	
	m_GridWidthSlider.SetTic(10);
	m_GridWidthSlider.SetTicFreq(5);
	m_GridWidthSlider.SetChannelColor(RGB(0, 255, 0));
	BuildGridWidth();

	m_GridCountSlider.SetRange(10, 100);	
	m_GridCountSlider.SetTicFreq(5);
	m_GridCountSlider.SetTic(10);
	m_GridCountSlider.SetChannelColor(RGB(0, 0, 255));
	BuildGridCount();

	// =====================================================================
	// Object Edit
	// =====================================================================
	m_fObjectMoveSpeed = CVenusConfig::GetInstance().m_fEditMoveSpeed;
	m_fObjectRotateSpeed = CVenusConfig::GetInstance().m_fEditRotateSpeed;
	m_fObjectScaleSpeed = CVenusConfig::GetInstance().m_fEditScaleSpeed;

	m_ObjectMoveSpeedSlider.SetRange(1, 100);	
	m_ObjectMoveSpeedSlider.SetTic(10);
	m_ObjectMoveSpeedSlider.SetTicFreq(5);
	m_ObjectMoveSpeedSlider.SetChannelColor(RGB(255, 0, 0));
	m_ObjectMoveSpeedSlider.SetPos((int)(m_fObjectMoveSpeed * 1000.f));

	m_ObjectRotateSpeedSlider.SetRange(1, 100);
	m_ObjectRotateSpeedSlider.SetTicFreq(5);
	m_ObjectRotateSpeedSlider.SetTic(10);
	m_ObjectRotateSpeedSlider.SetChannelColor(RGB(0, 255, 0));
	m_ObjectRotateSpeedSlider.SetPos((int)(m_fObjectRotateSpeed * 10.f));

	m_ObjectScaleSpeedSlider.SetRange(1, 100);
	m_ObjectScaleSpeedSlider.SetTicFreq(5);
	m_ObjectScaleSpeedSlider.SetTic(10);
	m_ObjectScaleSpeedSlider.SetChannelColor(RGB(0, 0, 255));
	m_ObjectScaleSpeedSlider.SetPos((int)(m_fObjectScaleSpeed * 1000.f));


	// =====================================================================
	// Camera
	// =====================================================================
	m_fCameraMoveSpeed = CVenusCamera::GetInstance().GetMoveSpeed();
	m_fCameraRotateSpeed = CVenusCamera::GetInstance().GetRotationSpeed();
	m_fCameraWheelSpeed = CVenusCamera::GetInstance().GetWheelSpeed();

	m_CameraMoveSpeedSlider.SetRange(1, 100);	
	m_CameraMoveSpeedSlider.SetTic(10);
	m_CameraMoveSpeedSlider.SetTicFreq(5);
	m_CameraMoveSpeedSlider.SetChannelColor(RGB(255, 0, 0));
	m_CameraMoveSpeedSlider.SetPos((int)(m_fCameraMoveSpeed * 100.f));

	m_CameraRotateSpeedSlider.SetRange(1, 100);
	m_CameraRotateSpeedSlider.SetTicFreq(5);
	m_CameraRotateSpeedSlider.SetTic(10);
	m_CameraRotateSpeedSlider.SetChannelColor(RGB(0, 255, 0));
	m_CameraRotateSpeedSlider.SetPos((int)(m_fCameraMoveSpeed * 100.f));

	m_CameraWheelSpeedSlider.SetRange(1, 100);
	m_CameraWheelSpeedSlider.SetTicFreq(5);
	m_CameraWheelSpeedSlider.SetTic(10);
	m_CameraWheelSpeedSlider.SetChannelColor(RGB(0, 0, 255));
	m_CameraWheelSpeedSlider.SetPos((int)(m_fCameraMoveSpeed * 100.f));


	m_BackGroundButton.SetColor(CVenusFramework::GetInstance().GetBackGroundColor().red, CVenusFramework::GetInstance().GetBackGroundColor().green, CVenusFramework::GetInstance().GetBackGroundColor().blue);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void COptionDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void COptionDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(pScrollBar->GetDlgCtrlID())
	{
		case IDC_GRID_WIDTH:
			CVenusGrid::GetInstance().SetGridWidth((float)m_GridWidthSlider.GetPos() / 10.f);
			BuildGridWidth();
			break;
		case IDC_GRID_COUNT:
			CVenusGrid::GetInstance().CreateGrid(m_GridCountSlider.GetPos());
			BuildGridCount();
			break;

		case IDC_OBJECT_MOVE_SPEED:
			m_fObjectMoveSpeed = (float)m_ObjectMoveSpeedSlider.GetPos() / 1000.f;
			CVenusConfig::GetInstance().m_fEditMoveSpeed = m_fObjectMoveSpeed;
			UpdateData(FALSE);
			break;
		case IDC_OBJECT_ROTATE_SPEED:
			m_fObjectRotateSpeed = (float)m_ObjectRotateSpeedSlider.GetPos() / 10.f;
			CVenusConfig::GetInstance().m_fEditRotateSpeed = m_fObjectRotateSpeed;
			UpdateData(FALSE);
			break;
		case IDC_OBJECT_SCALE_SPEED:
			m_fObjectScaleSpeed = (float)m_ObjectScaleSpeedSlider.GetPos() / 1000.f;
			CVenusConfig::GetInstance().m_fEditScaleSpeed = m_fObjectScaleSpeed;
			UpdateData(FALSE);
			break;

		case IDC_CAMERA_MOVE_SPEED:
			m_fCameraMoveSpeed = (float)m_CameraMoveSpeedSlider.GetPos() / 100.f;
			CVenusCamera::GetInstance().SetMoveSpeed( m_fCameraMoveSpeed);;
			UpdateData(FALSE);
			break;
		case IDC_CAMERA_ROTATE_SPEED:
			m_fCameraRotateSpeed = (float)m_CameraRotateSpeedSlider.GetPos() / 100.f;
			CVenusCamera::GetInstance().SetRotationSpeed( m_fCameraRotateSpeed);
			UpdateData(FALSE);
			break;
		case IDC_CAMERA_WHEEL_SPEED:
			m_fCameraWheelSpeed = (float)m_CameraWheelSpeedSlider.GetPos() / 100.f;
			CVenusCamera::GetInstance().SetWheelSpeed( m_fCameraWheelSpeed);
			UpdateData(FALSE);
			break;
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

//---------------------------------------------------------------------------
//Name		: BuildGridWidth
//Desc		: 
//Return	: 
//Parameter	: 
//---------------------------------------------------------------------------
void COptionDialog::BuildGridWidth()
{
	m_strGridWidth.Format("%3.2f", CVenusGrid::GetInstance().GetGridWidth());
	m_GridWidthSlider.SetPos((int)(CVenusGrid::GetInstance().GetGridWidth() * 10.f));

	UpdateData(FALSE);
}

//---------------------------------------------------------------------------
//Name		: BuildGridWidth
//Desc		: 
//Return	: 
//Parameter	: 
//---------------------------------------------------------------------------
void COptionDialog::BuildGridCount()
{
	m_strGridCount.Format("%d", CVenusGrid::GetInstance().GetTileCount());
	m_GridCountSlider.SetPos(CVenusGrid::GetInstance().GetTileCount());

	UpdateData(FALSE);
}


void COptionDialog::OnBnClickedBackground()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CColorDialog dlg;
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	COLORREF colorVertex = dlg.GetColor();
    RwRGBA rgba = {GetRValue(colorVertex), GetGValue(colorVertex), GetBValue(colorVertex), 255};
	CVenusFramework::GetInstance().SetBackGroundColor(rgba);
	m_BackGroundButton.SetColor(CVenusFramework::GetInstance().GetBackGroundColor().red, CVenusFramework::GetInstance().GetBackGroundColor().green, CVenusFramework::GetInstance().GetBackGroundColor().blue);
	Invalidate(FALSE);
}

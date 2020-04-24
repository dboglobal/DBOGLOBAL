// EditDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Venus.h"
#include "EditDialog.h"
#include ".\editdialog.h"

#include "VenusConfig.h"

#include "VenusPropertyContainer.h"
#include "VenusVisualManager.h"

// CEditDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEditDialog, CDialog)
CEditDialog::CEditDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CEditDialog::IDD, pParent)
	, m_strAbsoluteX(_T(""))
	, m_strAbsoluteY(_T(""))
	, m_strAbsoluteZ(_T(""))
	, m_strOffsetX(_T(""))
	, m_strOffsetY(_T(""))
	, m_strOffsetZ(_T(""))
{
}

CEditDialog::~CEditDialog()
{
}

void CEditDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ABSOLUTE_X, m_strAbsoluteX);
	DDX_Text(pDX, IDC_ABSOLUTE_Y, m_strAbsoluteY);
	DDX_Text(pDX, IDC_ABSOLUTE_Z, m_strAbsoluteZ);
	DDX_Text(pDX, IDC_OFFSET_X, m_strOffsetX);
	DDX_Text(pDX, IDC_OFFSET_Y, m_strOffsetY);
	DDX_Text(pDX, IDC_OFFSET_Z, m_strOffsetZ);
}


BEGIN_MESSAGE_MAP(CEditDialog, CDialog)
	ON_WM_SIZE()
	ON_EN_CHANGE(IDC_ABSOLUTE_X, OnEnChangeAbsoluteX)
	ON_EN_CHANGE(IDC_ABSOLUTE_Y, OnEnChangeAbsoluteY)
	ON_EN_CHANGE(IDC_ABSOLUTE_Z, OnEnChangeAbsoluteZ)
	ON_EN_CHANGE(IDC_OFFSET_X, OnEnChangeOffsetX)
	ON_EN_CHANGE(IDC_OFFSET_Y, OnEnChangeOffsetY)
	ON_EN_CHANGE(IDC_OFFSET_Z, OnEnChangeOffsetZ)
	ON_EN_KILLFOCUS(IDC_ABSOLUTE_X, OnEnKillfocusAbsoluteX)
	ON_EN_KILLFOCUS(IDC_ABSOLUTE_Y, OnEnKillfocusAbsoluteY)
	ON_EN_KILLFOCUS(IDC_ABSOLUTE_Z, OnEnKillfocusAbsoluteZ)
	ON_EN_KILLFOCUS(IDC_OFFSET_X, OnEnKillfocusOffsetX)
	ON_EN_KILLFOCUS(IDC_OFFSET_Y, OnEnKillfocusOffsetY)
	ON_EN_KILLFOCUS(IDC_OFFSET_Z, OnEnKillfocusOffsetZ)
END_MESSAGE_MAP()


// CEditDialog 메시지 처리기입니다.

CEditDialog& CEditDialog::GetInstance()
{
	static CEditDialog dlg;
	return dlg;
}

BOOL CEditDialog::OnInitDialog()
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

	Clear();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CEditDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


BOOL CEditDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN) 
	{
		if(pMsg->wParam == VK_RETURN)
		{
			if (this->GetFocus()->GetSafeHwnd() == GetDlgItem(IDC_ABSOLUTE_X)->GetSafeHwnd() ||
				this->GetFocus()->GetSafeHwnd() == GetDlgItem(IDC_ABSOLUTE_Y)->GetSafeHwnd() ||
				this->GetFocus()->GetSafeHwnd() == GetDlgItem(IDC_ABSOLUTE_Z)->GetSafeHwnd())
			{
				if (this->GetFocus()->GetSafeHwnd() == GetDlgItem(IDC_ABSOLUTE_X)->GetSafeHwnd())
				{
					SetEditPoint_X((float)atof(m_strAbsoluteX));
				}
				else if (this->GetFocus()->GetSafeHwnd() == GetDlgItem(IDC_ABSOLUTE_Y)->GetSafeHwnd())
				{
					SetEditPoint_Y((float)atof(m_strAbsoluteY));
				}
				else if (this->GetFocus()->GetSafeHwnd() == GetDlgItem(IDC_ABSOLUTE_Z)->GetSafeHwnd())
				{
					SetEditPoint_Z((float)atof(m_strAbsoluteZ));
				}
			}

			if (this->GetFocus()->GetSafeHwnd() == GetDlgItem(IDC_OFFSET_X)->GetSafeHwnd() ||
				this->GetFocus()->GetSafeHwnd() == GetDlgItem(IDC_OFFSET_Y)->GetSafeHwnd() ||
				this->GetFocus()->GetSafeHwnd() == GetDlgItem(IDC_OFFSET_Z)->GetSafeHwnd())
			{
				RwV3d vOffset = { 0, 0, 0 };
				if (this->GetFocus()->GetSafeHwnd() == GetDlgItem(IDC_OFFSET_X)->GetSafeHwnd())
				{
					vOffset.x = (float)atof(m_strOffsetX);
				}
				else if (this->GetFocus()->GetSafeHwnd() == GetDlgItem(IDC_OFFSET_Y)->GetSafeHwnd())
				{
					vOffset.y = (float)atof(m_strOffsetY);
				}
				else if (this->GetFocus()->GetSafeHwnd() == GetDlgItem(IDC_OFFSET_Z)->GetSafeHwnd())
				{
					vOffset.z = (float)atof(m_strOffsetZ);
				}
				if (RwV3dLength(&vOffset) > 0.f)
				{
					AddEditPoint(vOffset);
				}
			}
			return FALSE;
		}
		if(pMsg->wParam == VK_ESCAPE) return FALSE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CEditDialog::Clear()
{
	m_strAbsoluteX.Format("0.0");
	m_strAbsoluteY.Format("0.0");
	m_strAbsoluteZ.Format("0.0");

	m_strOldAbsoluteX = m_strAbsoluteX;
	m_strOldAbsoluteY = m_strAbsoluteY;
	m_strOldAbsoluteZ = m_strAbsoluteZ;


	m_strOffsetX = "";
	m_strOffsetY = "";
	m_strOffsetZ = "";

	GetDlgItem(IDC_ABSOLUTE_X)->EnableWindow(FALSE);
	GetDlgItem(IDC_ABSOLUTE_Y)->EnableWindow(FALSE);
	GetDlgItem(IDC_ABSOLUTE_Z)->EnableWindow(FALSE);
	GetDlgItem(IDC_OFFSET_X)->EnableWindow(FALSE);
	GetDlgItem(IDC_OFFSET_Y)->EnableWindow(FALSE);
	GetDlgItem(IDC_OFFSET_Z)->EnableWindow(FALSE);

	UpdateData(FALSE);
}

void CEditDialog::SetAbsolute()
{
	m_strOldAbsoluteX = m_strAbsoluteX = "";
	m_strOldAbsoluteY = m_strAbsoluteY = "";
	m_strOldAbsoluteZ = m_strAbsoluteZ = "";

	m_strOffsetX.Format("0.0");
	m_strOffsetY.Format("0.0");
	m_strOffsetZ.Format("0.0");

	GetDlgItem(IDC_ABSOLUTE_X)->EnableWindow(TRUE);
	GetDlgItem(IDC_ABSOLUTE_Y)->EnableWindow(TRUE);
	GetDlgItem(IDC_ABSOLUTE_Z)->EnableWindow(TRUE);
	GetDlgItem(IDC_OFFSET_X)->EnableWindow(TRUE);
	GetDlgItem(IDC_OFFSET_Y)->EnableWindow(TRUE);
	GetDlgItem(IDC_OFFSET_Z)->EnableWindow(TRUE);

	UpdateData(FALSE);
}

void CEditDialog::SetAbsolute(float x, float y, float z)
{
	m_strAbsoluteX.Format("%f", x);
	m_strAbsoluteY.Format("%f", y);
	m_strAbsoluteZ.Format("%f", z);

	m_strOldAbsoluteX = m_strAbsoluteX;
	m_strOldAbsoluteY = m_strAbsoluteY;
	m_strOldAbsoluteZ = m_strAbsoluteZ;

	m_strOffsetX.Format("0.0");
	m_strOffsetY.Format("0.0");
	m_strOffsetZ.Format("0.0");

	GetDlgItem(IDC_ABSOLUTE_X)->EnableWindow(TRUE);
	GetDlgItem(IDC_ABSOLUTE_Y)->EnableWindow(TRUE);
	GetDlgItem(IDC_ABSOLUTE_Z)->EnableWindow(TRUE);
	GetDlgItem(IDC_OFFSET_X)->EnableWindow(TRUE);
	GetDlgItem(IDC_OFFSET_Y)->EnableWindow(TRUE);
	GetDlgItem(IDC_OFFSET_Z)->EnableWindow(TRUE);

	UpdateData(FALSE);
}

void CEditDialog::AddEditPoint(RwV3d& vOffset)
{
	if (CVenusConfig::GetInstance().m_nCurrentLayer == EDIT_LAYER_PATH)
	{
		CVenusPropertyContainer::GetInstance().AddSelectControlPoint(&vOffset);
	}
	else
	{
		CVenusVisualManager::GetInstance().AddSelectInstanceEffectPoint(&vOffset);
	}
}

void CEditDialog::SetEditPoint(RwV3d& vAbsolute)
{
	if (CVenusConfig::GetInstance().m_nCurrentLayer == EDIT_LAYER_PATH)
	{
		CVenusPropertyContainer::GetInstance().SetSelectControlPoint(&vAbsolute);
	}
	else
	{
		CVenusVisualManager::GetInstance().SetSelectInstanceEffectPoint(&vAbsolute);
	}
}

void CEditDialog::SetEditPoint_X(float x)
{
	if (CVenusConfig::GetInstance().m_nCurrentLayer == EDIT_LAYER_PATH)
	{
		CVenusPropertyContainer::GetInstance().SetSelectControlPoint_X(x);
	}
	else
	{
		CVenusVisualManager::GetInstance().SetSelectInstanceEffectPoint_X(x);
	}
}
void CEditDialog::SetEditPoint_Y(float y)
{
	if (CVenusConfig::GetInstance().m_nCurrentLayer == EDIT_LAYER_PATH)
	{
		CVenusPropertyContainer::GetInstance().SetSelectControlPoint_Y(y);
	}
	else
	{
		CVenusVisualManager::GetInstance().SetSelectInstanceEffectPoint_Y(y);
	}
}
void CEditDialog::SetEditPoint_Z(float z)
{
	if (CVenusConfig::GetInstance().m_nCurrentLayer == EDIT_LAYER_PATH)
	{
		CVenusPropertyContainer::GetInstance().SetSelectControlPoint_Z(z);
	}
	else
	{
		CVenusVisualManager::GetInstance().SetSelectInstanceEffectPoint_Z(z);
	}
}

void CEditDialog::OnEnChangeAbsoluteX()
{
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}

void CEditDialog::OnEnChangeAbsoluteY()
{
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}

void CEditDialog::OnEnChangeAbsoluteZ()
{
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}

void CEditDialog::OnEnChangeOffsetX()
{
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}

void CEditDialog::OnEnChangeOffsetY()
{
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}

void CEditDialog::OnEnChangeOffsetZ()
{
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}

void CEditDialog::OnEnKillfocusAbsoluteX()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	float x = (float)atof(m_strAbsoluteX);
	if (m_strOldAbsoluteX != m_strAbsoluteX)
	{
		SetEditPoint_X(x);
	}
}

void CEditDialog::OnEnKillfocusAbsoluteY()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	float y = (float)atof(m_strAbsoluteY);
	if (m_strOldAbsoluteY != m_strAbsoluteY)
	{
		SetEditPoint_Y(y);
	}
}

void CEditDialog::OnEnKillfocusAbsoluteZ()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	float z = (float)atof(m_strAbsoluteZ);
	if (m_strOldAbsoluteZ != m_strAbsoluteZ)
	{
		SetEditPoint_Z(z);
	}
}

void CEditDialog::OnEnKillfocusOffsetX()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	RwV3d vOffset = { 0, 0, 0 };
	vOffset.x = (float)atof(m_strOffsetX);
	if (RwV3dLength(&vOffset) > 0.f)
	{
		AddEditPoint(vOffset);
	}
}

void CEditDialog::OnEnKillfocusOffsetY()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	RwV3d vOffset = { 0, 0, 0 };
	vOffset.y = (float)atof(m_strOffsetY);
	if (RwV3dLength(&vOffset) > 0.f)
	{
		AddEditPoint(vOffset);
	}
}

void CEditDialog::OnEnKillfocusOffsetZ()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	RwV3d vOffset = { 0, 0, 0 };
	vOffset.z = (float)atof(m_strOffsetZ);
	if (RwV3dLength(&vOffset) > 0.f)
	{
		AddEditPoint(vOffset);
	}
}

void CEditDialog::Reset()
{
	if (CVenusConfig::GetInstance().m_nCurrentLayer == EDIT_LAYER_PATH)
	{
		ResetSplineControlPoint();
	}
	else
	{
		ResetInstanceEffect();
	}
}

//------------------------------------------------------------------
//	FuncName	: ResetEditDialog
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CEditDialog::ResetInstanceEffect()
{
	if (CVenusVisualManager::GetInstance().m_svSelectInstanceEffect.empty())
	{
		Clear();
	}
	else if (CVenusVisualManager::GetInstance().m_svSelectInstanceEffect.size() > 1)
	{
		SetAbsolute();
	}
	else
	{
		CNtlInstanceEffect* pInstanceEffect = *CVenusVisualManager::GetInstance().m_svSelectInstanceEffect.begin();
		switch(CVenusConfig::GetInstance().m_nEditMode)
		{
		case CVenusConfig::EDIT_MODE_MOVE:
			SetAbsolute(pInstanceEffect->GetPosition().x,
						pInstanceEffect->GetPosition().y,
						pInstanceEffect->GetPosition().z);
			break;
		case CVenusConfig::EDIT_MODE_ROTATE:
		case CVenusConfig::EDIT_MODE_SCALE:
			Clear();
			break;
		}
	}
}
//------------------------------------------------------------------
//	FuncName	: ResetSplineControlPoint
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CEditDialog::ResetSplineControlPoint()
{
	if (CVenusPropertyContainer::GetInstance().m_svSelectSplineControlPoint.empty())
	{
		Clear();
	}
	else if (CVenusPropertyContainer::GetInstance().m_svSelectSplineControlPoint.size() > 1)
	{
		SetAbsolute();
	}
	else
	{
		RwInt32 index = *CVenusPropertyContainer::GetInstance().m_svSelectSplineControlPoint.begin();

		switch(CVenusConfig::GetInstance().m_nEditMode)
		{
		case CVenusConfig::EDIT_MODE_MOVE:
			SetAbsolute(CVenusPropertyContainer::GetInstance().m_pEffectSplinePath->m_pControlPoint[index].pos.x,
						CVenusPropertyContainer::GetInstance().m_pEffectSplinePath->m_pControlPoint[index].pos.y,
						CVenusPropertyContainer::GetInstance().m_pEffectSplinePath->m_pControlPoint[index].pos.z);
			break;
		case CVenusConfig::EDIT_MODE_ROTATE:
			SetAbsolute(CVenusPropertyContainer::GetInstance().m_pEffectSplinePath->m_pControlPoint[index].rotate.x,
						CVenusPropertyContainer::GetInstance().m_pEffectSplinePath->m_pControlPoint[index].rotate.y,
						CVenusPropertyContainer::GetInstance().m_pEffectSplinePath->m_pControlPoint[index].rotate.z);
			break;
		case CVenusConfig::EDIT_MODE_SCALE:
			Clear();
			break;
		}
	}
}


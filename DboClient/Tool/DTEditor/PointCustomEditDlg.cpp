// PointCustomEditDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DTEditor.h"
#include "PointCustomEditDlg.h"
#include "CustomItems.h"
#include "DTEditorGlobal.h"

// simulation
#include "precomp_ntlsimulation.h"
#include "NtlDTProp.h"



#define IDC_PROPERTY_CUSTOM				201


#define CUSTOM_ITEM_SOUND_LOOP			1000
#define FILEBOX_FILTER_SOUND			"Sound File(*.wav;*.ogg;*.mp3)|*.wav;*.ogg;*.mp3||"

#define CUSTOM_CURVE_RECT_SIZE			20
#define CUSTOM_CURVE_RECT_INTERVAL		20
#define CUSTOM_CURVE_SEL_RECT_SIZE		24


// CPointCustomEditDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPointCustomEditDlg, CDialog)

CPointCustomEditDlg::CPointCustomEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPointCustomEditDlg::IDD, pParent)
{
	m_nCurveNum = 3;
	m_nSelIdx	= 0;
}

CPointCustomEditDlg::~CPointCustomEditDlg()
{
}

void CPointCustomEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSVE_SOUND_LOOP, m_btnLoop);
	DDX_Control(pDX, IDC_CUSVE_SOUND_FILENAME, m_editFileName);
}


BEGIN_MESSAGE_MAP(CPointCustomEditDlg, CDialog)
	ON_WM_SIZE()
	ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, OnAttributeNotify)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_CUSVE_SOUND_LOOP, &CPointCustomEditDlg::OnBnClickedCusveSoundLoop)
	ON_EN_CHANGE(IDC_CUSVE_SOUND_FILENAME, &CPointCustomEditDlg::OnEnChangeCusveSoundFilename)
	ON_BN_CLICKED(IDC_CUSVE_SOUND_CLEAR, &CPointCustomEditDlg::OnBnClickedCusveSoundClear)
END_MESSAGE_MAP()

void CPointCustomEditDlg::OnPropUpdate(void)
{
	m_nCurveNum = 0;
	m_nSelIdx	= 0;

	m_btnLoop.SetCheck(FALSE);
	SetDlgItemText(IDC_CUSVE_SOUND_FILENAME, "");

	Invalidate(TRUE);
}

void CPointCustomEditDlg::OnPropSelectUpdate(int nSx, int nEx)
{
	CNtlDTSpline *pDTSplineProp = (CNtlDTSpline*)GetActiveDTProp();
	if(pDTSplineProp == NULL)
		return;

	const SSplineProp *pSplineProp = pDTSplineProp->GetSplineProp();
	SSplineCustomData *pCustomData = pDTSplineProp->GetSplineCustomData(nSx*pSplineProp->iCurveSubDivision);

	m_nCurveNum = pSplineProp->iCurveSubDivision;
	m_nSelIdx	= 0;

	m_btnLoop.SetCheck(FALSE);
	SetDlgItemText(IDC_CUSVE_SOUND_FILENAME, "");

	if(pCustomData)
	{
		if(pCustomData->bSoundLoop)
			m_btnLoop.SetCheck(TRUE);

		if(strlen(pCustomData->chSoundFile) > 0)
			SetDlgItemText(IDC_CUSVE_SOUND_FILENAME, pCustomData->chSoundFile);
	}
	
	Invalidate(TRUE);
}

void CPointCustomEditDlg::InData(void)
{
	CNtlDTSpline *pDTSplineProp = (CNtlDTSpline*)GetActiveDTProp();
	if(pDTSplineProp == NULL)
		return;

	int nSelSx, nSelEx;
	GetActiveSelectControlPoint(nSelSx, nSelEx);

	const SSplineProp *pSplineProp = pDTSplineProp->GetSplineProp();
	SSplineCustomData *pCustomData = pDTSplineProp->GetSplineCustomData(nSelSx*pSplineProp->iCurveSubDivision + m_nSelIdx);

	m_btnLoop.SetCheck(FALSE);
	SetDlgItemText(IDC_CUSVE_SOUND_FILENAME, "");

	if(pCustomData)
	{
		if(pCustomData->bSoundLoop)
			m_btnLoop.SetCheck(TRUE);

		if(strlen(pCustomData->chSoundFile) > 0)
			SetDlgItemText(IDC_CUSVE_SOUND_FILENAME, pCustomData->chSoundFile);
	}
}

void CPointCustomEditDlg::OutData(void)
{
	CNtlDTSpline *pDTSplineProp = (CNtlDTSpline*)GetActiveDTProp();
	if(pDTSplineProp == NULL)
		return;

	int nSelSx, nSelEx;
	GetActiveSelectControlPoint(nSelSx, nSelEx);

	RwBool bLoop;
	if(m_btnLoop.GetCheck())
		bLoop = TRUE;
	else
		bLoop = FALSE;

	CString str;
	GetDlgItemText(IDC_CUSVE_SOUND_FILENAME, str);

	if(str.GetLength() > 0 && str.GetLength() < SPLINE_CUSTOM_SOUND_FILNE_LEN)
	{
		const SSplineProp *pSplineProp = pDTSplineProp->GetSplineProp();
		pDTSplineProp->SetSplineCustomSoundData(nSelSx*pSplineProp->iCurveSubDivision + m_nSelIdx, 
												(LPCTSTR)str, bLoop);
	}
}

CRect CPointCustomEditDlg::GetCurveRect(INT nIdx)
{
	CRect rt;
	rt.SetRect(m_rtCurveArea.left + (CUSTOM_CURVE_RECT_SIZE + CUSTOM_CURVE_RECT_INTERVAL)*nIdx, 
				m_rtCurveArea.top,
				m_rtCurveArea.left + (CUSTOM_CURVE_RECT_SIZE + CUSTOM_CURVE_RECT_INTERVAL)*nIdx+CUSTOM_CURVE_RECT_SIZE, 
				m_rtCurveArea.top + CUSTOM_CURVE_RECT_SIZE);

	return rt;
}

// CPointCustomEditDlg 메시지 처리기입니다.

BOOL CPointCustomEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	CRect rtCustom, rtDlg, rtCurve;

	GetClientRect(rtDlg);
	ClientToScreen(rtDlg);

	CWnd *pCurveWnd = GetDlgItem(IDC_CUSTOM_CURVE_POINT);
	pCurveWnd->GetClientRect(rtCurve);
	pCurveWnd->ClientToScreen(rtCurve);
		
	m_rtCurveArea.SetRect(rtCurve.left - rtDlg.left,
						  rtCurve.top - rtDlg.top, 
						  rtCurve.left - rtDlg.left + rtCurve.Width(),
						  rtCurve.top - rtDlg.top + rtCurve.Height());

	m_rtCurveArea.left += 10;
	m_rtCurveArea.top = m_rtCurveArea.top + (m_rtCurveArea.Height() - CUSTOM_CURVE_RECT_SIZE)/2;
	m_rtCurveArea.bottom = m_rtCurveArea.top + CUSTOM_CURVE_RECT_SIZE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CPointCustomEditDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

LRESULT CPointCustomEditDlg::OnAttributeNotify(WPARAM wParam, LPARAM lParam)
{
    if(wParam == XTP_PGN_ITEMVALUE_CHANGED)
    {
//		OnItemValueChangedGridNotify(wParam, lParam);
	}
	else if(wParam == XTP_PGN_EDIT_CHANGED)
	{
//		OnEditChangedGridNotify(wParam, lParam);
	}

	return 0;
}

void CPointCustomEditDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.

	CRect rt;

	if(m_nCurveNum > 0)
	{
		for(INT i = 0; i < m_nCurveNum; i++)
		{
			rt = GetCurveRect(i);

			if(i == m_nSelIdx)
			{
				dc.FillSolidRect(rt, RGB(170, 13, 60));
				dc.Draw3dRect(rt, RGB(232, 17, 82), RGB(87, 6, 31));
			}
			else
			{
				dc.FillSolidRect(rt, RGB(170, 170, 60));
				dc.Draw3dRect(rt, RGB(232, 232, 82), RGB(87, 87, 31));
			}
		}
	}
}

void CPointCustomEditDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CPointCustomEditDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_nCurveNum > 0)
	{
		for(INT i = 0; i < m_nCurveNum; i++)
		{
			CRect rt = GetCurveRect(i);
			if(rt.PtInRect(point))
			{
				m_nSelIdx = i;
				InData();
				Invalidate(TRUE);
				break;
			}
		}
	}
	
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnLButtonDown(nFlags, point);
}

void CPointCustomEditDlg::OnBnClickedCusveSoundLoop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	OutData();
}

void CPointCustomEditDlg::OnEnChangeCusveSoundFilename()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정하고  마스크에 OR 연산하여 설정된
	// ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출해야만
	// 해당 알림 메시지를 보냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	OutData();
}

void CPointCustomEditDlg::OnBnClickedCusveSoundClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_btnLoop.SetCheck(FALSE);
	SetDlgItemText(IDC_CUSVE_SOUND_FILENAME, "");

	CNtlDTSpline *pDTSplineProp = (CNtlDTSpline*)GetActiveDTProp();
	if(pDTSplineProp == NULL)
		return;

	int nSelSx, nSelEx;
	GetActiveSelectControlPoint(nSelSx, nSelEx);
	const SSplineProp *pSplineProp = pDTSplineProp->GetSplineProp();
	pDTSplineProp->ClearSplineCustomData(nSelSx*pSplineProp->iCurveSubDivision + m_nSelIdx);
}

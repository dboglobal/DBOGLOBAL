// LightDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Venus.h"
#include "LightDialog.h"
#include ".\lightdialog.h"

#include "VenusConfig.h"

#include "VenusFramework.h"

// CLightDialog 대화 상자입니다.

const RwChar* g_LightString[] = {	"rpLIGHTDIRECTIONAL",	\
									"rpLIGHTPOINT",			\
									"rpLIGHTSPOT",			\
									"rpLIGHTSPOTSOFT" };
const RwInt32 g_nLightCount = 4;

static RwInt32 LightFunctions[g_nLightCount] =
{
    rpLIGHTDIRECTIONAL,
    rpLIGHTPOINT,
    rpLIGHTSPOT,
    rpLIGHTSPOTSOFT
};



IMPLEMENT_DYNAMIC(CLightDialog, CDialog)
CLightDialog::CLightDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CLightDialog::IDD, pParent)
	, m_strAmbientColorR(_T(""))
	, m_strAmbientColorG(_T(""))
	, m_strAmbientColorB(_T(""))
	, m_strLightColorR(_T(""))
	, m_strLightColorG(_T(""))
	, m_strLightColorB(_T(""))
	, m_strLightRadius(_T(""))
	, m_strLightAngle(_T(""))
	, m_strLightColorAll(_T(""))
	, m_strAmbientColorAll(_T(""))
{
	m_nAmbientColorAll	= 0;
	m_nLightColorAll	= 0;
}

CLightDialog::~CLightDialog()
{
}

void CLightDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ABMIENT_COLOR, m_AmbientColor);
	DDX_Control(pDX, IDC_ABMIENT_COLOR_R, m_AmbientColorR);
	DDX_Control(pDX, IDC_ABMIENT_COLOR_G, m_AmbientColorG);
	DDX_Control(pDX, IDC_ABMIENT_COLOR_B, m_AmbientColorB);
	DDX_Text(pDX, IDC_ABMIENT_EDIT_COLOR_R, m_strAmbientColorR);
	DDX_Text(pDX, IDC_ABMIENT_EDIT_COLOR_G, m_strAmbientColorG);
	DDX_Text(pDX, IDC_ABMIENT_EDIT_COLOR_B, m_strAmbientColorB);
	DDX_Control(pDX, IDC_LIGHT_TYPE, m_LightType);
	DDX_Control(pDX, IDC_LIGHT_COLOR, m_LightColor);
	DDX_Control(pDX, IDC_LIGHT_COLOR_R, m_LightColorR);
	DDX_Control(pDX, IDC_LIGHT_COLOR_G, m_LightColorG);
	DDX_Control(pDX, IDC_LIGHT_COLOR_B, m_LightColorB);
	DDX_Text(pDX, IDC_LIGHT_EDIT_COLOR_R, m_strLightColorR);
	DDX_Text(pDX, IDC_LIGHT_EDIT_COLOR_G, m_strLightColorG);
	DDX_Text(pDX, IDC_LIGHT_EDIT_COLOR_B, m_strLightColorB);
	DDX_Control(pDX, IDC_LIGHT_RADIUS, m_LightRadius);
	DDX_Text(pDX, IDC_LIGHT_EDIT_RADIUS, m_strLightRadius);
	DDX_Control(pDX, IDC_LIGHT_ANGLE, m_LightAngle);
	DDX_Text(pDX, IDC_LIGHT_EDIT_ANGLE, m_strLightAngle);
	DDX_Text(pDX, IDC_LIGHT_EDIT_COLOR_ALL, m_strLightColorAll);
	DDX_Control(pDX, IDC_LIGHT_COLOR_ALL, m_LightColorAll);
	DDX_Control(pDX, IDC_ABMIENT_COLOR_ALL, m_AmbientColorAll);
	DDX_Text(pDX, IDC_ABMIENT_EDIT_COLOR_ALL, m_strAmbientColorAll);
}


BEGIN_MESSAGE_MAP(CLightDialog, CDialog)
	ON_WM_HSCROLL()
	ON_CBN_SELCHANGE(IDC_LIGHT_TYPE, OnCbnSelchangeLightType)
	ON_BN_CLICKED(IDC_LIGHT_COLOR, OnBnClickedLightColor)
	ON_BN_CLICKED(IDC_ABMIENT_COLOR, OnBnClickedAbmientColor)
END_MESSAGE_MAP()


// CLightDialog 메시지 처리기입니다.
CLightDialog& CLightDialog::GetInstance()
{
	static CLightDialog dlg;
	return dlg;
}

BOOL CLightDialog::OnInitDialog()
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


	InitSliderButton(&m_AmbientColorR, 0, 255, 20, RGB(255, 0, 0));
	InitSliderButton(&m_AmbientColorG, 0, 255, 20, RGB(0, 255, 0));
	InitSliderButton(&m_AmbientColorB, 0, 255, 20, RGB(0, 0, 255));
	InitSliderButton(&m_AmbientColorAll, 0, 255, 20, RGB(255, 255, 255));

	InitSliderButton(&m_LightColorR, 0, 255, 20, RGB(255, 0, 0));
	InitSliderButton(&m_LightColorG, 0, 255, 20, RGB(0, 255, 0));
	InitSliderButton(&m_LightColorB, 0, 255, 20, RGB(0, 0, 255));
	InitSliderButton(&m_LightColorAll, 0, 255, 20, RGB(255, 255, 255));

	InitSliderButton(&m_LightRadius, 1, 200, 10, RGB(255, 255, 255));
	InitSliderButton(&m_LightAngle,  1,  90, 9, RGB(255, 255, 255));

	RwInt32 index;
	for (RwInt32 i = 0; i < g_nLightCount; ++ i)
	{
		index = m_LightType.AddString(g_LightString[i]);
		m_LightType.SetItemData(index, LightFunctions[i]);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CLightDialog::InitSliderButton(CMacSliderCtrl* pSlider, int nStart, int nEnd, int nTic, COLORREF color)
{
	pSlider->SetRange(nStart, nEnd);
	pSlider->SetTicFreq(nTic);
	pSlider->SetChannelColor(color);
}

void CLightDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(pScrollBar->GetDlgCtrlID())
	{
		case IDC_ABMIENT_COLOR_R:
			CVenusConfig::GetInstance().m_AmbientColor.red = (float)m_AmbientColorR.GetPos() / 255.f;
			BuildAmbientColor();
			UpdateData(FALSE);
			break;
		case IDC_ABMIENT_COLOR_G:
			CVenusConfig::GetInstance().m_AmbientColor.green = (float)m_AmbientColorG.GetPos() / 255.f;
			BuildAmbientColor();
			UpdateData(FALSE);
			break;
		case IDC_ABMIENT_COLOR_B:
			CVenusConfig::GetInstance().m_AmbientColor.blue = (float)m_AmbientColorB.GetPos() / 255.f;
			BuildAmbientColor();
			UpdateData(FALSE);
			break;
		case IDC_ABMIENT_COLOR_ALL:
			m_nAmbientColorAll = m_AmbientColorAll.GetPos();
			CVenusConfig::GetInstance().m_AmbientColor.red = (float)m_nAmbientColorAll / 255.f;
			CVenusConfig::GetInstance().m_AmbientColor.green = (float)m_nAmbientColorAll / 255.f;
			CVenusConfig::GetInstance().m_AmbientColor.blue = (float)m_nAmbientColorAll / 255.f;
			BuildAmbientColor();
			UpdateData(FALSE);
			break;

		case IDC_LIGHT_COLOR_R:
			CVenusConfig::GetInstance().m_LightColor.red = (float)m_LightColorR.GetPos() / 255.f;
			BuildLightColor();
			UpdateData(FALSE);
			break;
		case IDC_LIGHT_COLOR_G:
			CVenusConfig::GetInstance().m_LightColor.green = (float)m_LightColorG.GetPos() / 255.f;
			BuildLightColor();
			UpdateData(FALSE);
			break;
		case IDC_LIGHT_COLOR_B:
			CVenusConfig::GetInstance().m_LightColor.blue = (float)m_LightColorB.GetPos() / 255.f;
			BuildLightColor();
			UpdateData(FALSE);
			break;
		case IDC_LIGHT_COLOR_ALL:
			m_nLightColorAll = m_LightColorAll.GetPos();
			CVenusConfig::GetInstance().m_LightColor.red = (float)m_nLightColorAll / 255.f;
			CVenusConfig::GetInstance().m_LightColor.green = (float)m_nLightColorAll / 255.f;
			CVenusConfig::GetInstance().m_LightColor.blue = (float)m_nLightColorAll / 255.f;
			BuildLightColor();
			UpdateData(FALSE);
			break;

		case IDC_LIGHT_RADIUS:
			CVenusConfig::GetInstance().m_fLightRadius = (float)m_LightRadius.GetPos();
			BuildLightRadius();
			UpdateData(FALSE);
			break;

		case IDC_LIGHT_ANGLE:
			CVenusConfig::GetInstance().m_fLightConeAngle = (float)m_LightAngle.GetPos();
			BuildLightAngle();
			UpdateData(FALSE);
			break;
	}


	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CLightDialog::BuildData()
{
	BuildAmbientColor();

	BuildLightType();

	BuildLightColor();

	BuildLightRadius();

	BuildLightAngle();
}

//---------------------------------------------------------------------------
//Name		: BuildAmbientColor
//Desc		: 
//Return	: 
//Parameter	: 
//---------------------------------------------------------------------------
void CLightDialog::BuildAmbientColor()
{
	int R = (int)((float)CVenusConfig::GetInstance().m_AmbientColor.red * 255.f);
	int G = (int)((float)CVenusConfig::GetInstance().m_AmbientColor.green * 255.f);
	int B = (int)((float)CVenusConfig::GetInstance().m_AmbientColor.blue * 255.f);

	m_strAmbientColorR.Format("%d", R);
	m_strAmbientColorG.Format("%d", G);
	m_strAmbientColorB.Format("%d", B);
	m_strAmbientColorAll.Format("%d", m_nAmbientColorAll);

	m_AmbientColorR.SetPos(R);
	m_AmbientColorG.SetPos(G);
	m_AmbientColorB.SetPos(B);
	m_AmbientColorAll.SetPos(m_nAmbientColorAll);
	UpdateData(FALSE);

	m_AmbientColor.SetColor(R, G, B);

	Invalidate(FALSE);

	CVenusFramework::GetInstance().UpdateAmbient();
}

//---------------------------------------------------------------------------
//Name		: BuildLightType
//Desc		: 
//Return	: 
//Parameter	: 
//---------------------------------------------------------------------------
void CLightDialog::BuildLightType()
{
	for (RwInt32 i = 0; i < g_nLightCount; ++ i)
	{
		if (LightFunctions[i] == CVenusConfig::GetInstance().m_nLightType)
		{
			m_LightType.SetCurSel(i);
			break;
		}
	}
}

//---------------------------------------------------------------------------
//Name		: BuildLightColor
//Desc		: 
//Return	: 
//Parameter	: 
//---------------------------------------------------------------------------
void CLightDialog::BuildLightColor()
{
	int R = (int)((float)CVenusConfig::GetInstance().m_LightColor.red * 255.f);
	int G = (int)((float)CVenusConfig::GetInstance().m_LightColor.green * 255.f);
	int B = (int)((float)CVenusConfig::GetInstance().m_LightColor.blue * 255.f);

	m_strLightColorR.Format("%d", R);
	m_strLightColorG.Format("%d", G);
	m_strLightColorB.Format("%d", B);
	m_strLightColorAll.Format("%d", m_nLightColorAll);

	m_LightColorR.SetPos(R);
	m_LightColorG.SetPos(G);
	m_LightColorB.SetPos(B);
	m_LightColorAll.SetPos(m_nLightColorAll);
	UpdateData(FALSE);

	m_LightColor.SetColor(R, G, B);
	Invalidate(FALSE);

	CVenusFramework::GetInstance().UpdateMainLight();
}

//---------------------------------------------------------------------------
//Name		: BuildLightRadius
//Desc		: 
//Return	: 
//Parameter	: 
//---------------------------------------------------------------------------
void CLightDialog::BuildLightRadius()
{
	m_strLightRadius.Format("%d", (int)CVenusConfig::GetInstance().m_fLightRadius);
	
	m_LightRadius.SetPos((int)CVenusConfig::GetInstance().m_fLightRadius);

	UpdateData(FALSE);
	CVenusFramework::GetInstance().UpdateMainLight();
}

//---------------------------------------------------------------------------
//Name		: BuildLightAngle
//Desc		: 
//Return	: 
//Parameter	: 
//---------------------------------------------------------------------------
void CLightDialog::BuildLightAngle()
{
	m_strLightAngle.Format("%d", (int)CVenusConfig::GetInstance().m_fLightConeAngle);
	
	m_LightAngle.SetPos((int)CVenusConfig::GetInstance().m_fLightConeAngle);

	UpdateData(FALSE);
	CVenusFramework::GetInstance().UpdateMainLight();
}
void CLightDialog::OnCbnSelchangeLightType()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CVenusConfig::GetInstance().m_nLightType = (int)m_LightType.GetItemData(m_LightType.GetCurSel());
	CVenusFramework::GetInstance().CreateMainLight();
}

void CLightDialog::OnBnClickedLightColor()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CColorDialog dlg;
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	COLORREF colorVertex = dlg.GetColor();
	CVenusConfig::GetInstance().m_LightColor.red	= GetRValue(colorVertex) / 255.f;
	CVenusConfig::GetInstance().m_LightColor.green	= GetGValue(colorVertex) / 255.f;
	CVenusConfig::GetInstance().m_LightColor.blue	= GetBValue(colorVertex) / 255.f;

	BuildLightColor();
}

void CLightDialog::OnBnClickedAbmientColor()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CColorDialog dlg;
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	COLORREF colorVertex = dlg.GetColor();
	CVenusConfig::GetInstance().m_AmbientColor.red		= GetRValue(colorVertex) / 255.f;
	CVenusConfig::GetInstance().m_AmbientColor.green	= GetGValue(colorVertex) / 255.f;
	CVenusConfig::GetInstance().m_AmbientColor.blue		= GetBValue(colorVertex) / 255.f;

	BuildAmbientColor();
}

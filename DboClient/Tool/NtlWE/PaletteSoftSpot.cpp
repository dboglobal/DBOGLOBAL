// PaletteSoftSpot.cpp : 구현 파일입니다.
//

#include "stdafx.h"

// CZ-SS

// #include "NtlWE.h"
// #include "PaletteSoftSpot.h"
// #include "NtlWorldPaletteDefine.h"
// #include "NtlPLSoftSpot.h"
// #include "NtlWorldFieldManager.h"
// #include "NtlPLWorldEntity.h"
// #include "UpdateEvent.h"
// 
// 
// // CPaletteSoftSpot 대화 상자입니다.
// 
// IMPLEMENT_DYNAMIC(CPaletteSoftSpot, CDialog)
// 
// CPaletteSoftSpot::CPaletteSoftSpot(CWnd* pParent /*=NULL*/)
// 	: CDialog(CPaletteSoftSpot::IDD, pParent)
// 	, m_SoftR(_T("255"))
// 	, m_SoftG(_T("0"))
// 	, m_SoftB(_T("0"))
// 	, m_SoftAngle(_T(""))
// 	, m_SoftRadius(_T(""))
// 	, m_SoftPosX(_T("0.00"))
// 	, m_SoftPosY(_T("0.00"))
// 	, m_SoftPosZ(_T("0.00"))
// 	, m_SoftRotX(_T("0.00"))
// 	, m_SoftRotY(_T("0.00"))
// 	, m_SoftRotZ(_T("0.00"))
// {
// 
// }
// 
// CPaletteSoftSpot::~CPaletteSoftSpot()
// {
// }
// 
// void CPaletteSoftSpot::DoDataExchange(CDataExchange* pDX)
// {
// 	CDialog::DoDataExchange(pDX);
// 	DDX_Text(pDX, IDC_SOFT_R, m_SoftR);
// 	DDX_Text(pDX, IDC_SOFT_G, m_SoftG);
// 	DDX_Text(pDX, IDC_SOFT_B, m_SoftB);
// 	DDX_Text(pDX, IDC_ANGLE, m_SoftAngle);
// 	DDX_Text(pDX, IDC_RADIUS, m_SoftRadius);
// 	DDX_Control(pDX, IDC_SOFT_PICKER, m_SoftClrPicker);
// 	DDX_ColourPicker(pDX, IDC_SOFT_PICKER, m_clrDiffuser);
// 	DDX_Text(pDX, IDC_SOFT_POS_X, m_SoftPosX);
// 	DDX_Text(pDX, IDC_SOFT_POS_Y, m_SoftPosY);
// 	DDX_Text(pDX, IDC_SOFT_POS_Z, m_SoftPosZ);
// 	DDX_Text(pDX, IDC_SOFT_ROT_X, m_SoftRotX);
// 	DDX_Text(pDX, IDC_SOFT_ROT_Y, m_SoftRotY);
// 	DDX_Text(pDX, IDC_SOFT_ROT_Z, m_SoftRotZ);
// }
// 
// 
// BEGIN_MESSAGE_MAP(CPaletteSoftSpot, CDialog)
// 	ON_BN_CLICKED(IDC_SOFT_CREATE_BTN, &CPaletteSoftSpot::OnBnClickedSoftCreateBtn)
// END_MESSAGE_MAP()
// 
// 
// // CPaletteSoftSpot 메시지 처리기입니다.
// VOID CPaletteSoftSpot::InitializeWorldData()
// {
// 	UpdateData(TRUE);
// 
// 	m_clrDiffuser = RGB(255, 0, 0);
// 	SetColor(255, 0, 0);
// 
// 	m_SoftAngle.Format("%f", dCONE_ANGLE);
// 	m_SoftRadius.Format("%f", dCONE_RADIUS);
// 
// 	UpdateData(FALSE);
// }
// 
// void CPaletteSoftSpot::SetColor(int r, int g, int b)
// {
// 	UpdateData();
// 
// 	m_SoftR.Format("%d", r);
// 	m_SoftG.Format("%d", g);
// 	m_SoftB.Format("%d", b);
// 
// 	m_clrDiffuser = RGB(r, g, b);
// 
// 	UpdateData(FALSE);
// 
// 	SetCurSoftSpotProp();
// }
// 
// VOID CPaletteSoftSpot::RefreshProp(CNtlPLSpot* _pNtlPLSpot)
// {
// 	if(!_pNtlPLSpot)
// 	{
// 		return;
// 	}
// 
// 	RwInt32 RGB[3];
// 	RGB[0] = static_cast<RwInt32>(_pNtlPLSpot->GetColor().red * 255.0f);
// 	RGB[1] = static_cast<RwInt32>(_pNtlPLSpot->GetColor().green * 255.0f);
// 	RGB[2] = static_cast<RwInt32>(_pNtlPLSpot->GetColor().blue * 255.0f);
// 
// 	m_SoftR.Format("%d", RGB[0]);
// 	m_SoftG.Format("%d", RGB[1]);
// 	m_SoftB.Format("%d", RGB[2]);
// 	m_clrDiffuser = RGB(RGB[0], RGB[1], RGB[2]);
// 
// 	RwV3d  Pos	= _pNtlPLSpot->GetPosition();
// 	RwV3d* pRot = _pNtlPLSpot->GetRotate();
// 
// 	m_SoftPosX.Format("%.2f", Pos.x);
// 	m_SoftPosY.Format("%.2f", Pos.y);
// 	m_SoftPosZ.Format("%.2f", Pos.z);
// 	m_SoftRotX.Format("%.2f", pRot->x);
// 	m_SoftRotY.Format("%.2f", pRot->y);
// 	m_SoftRotZ.Format("%.2f", pRot->z);
// 
// 	m_SoftAngle.Format("%.2f", _pNtlPLSpot->GetAngle());
// 	m_SoftRadius.Format("%.2f", _pNtlPLSpot->GetRadius());
// 
// 	UpdateData(FALSE);
// }
// 
// VOID CPaletteSoftSpot::SetCurSoftSpotProp()
// {
// 	CNtlPLSpot* pSpotEntity;
// 
// 	int r = ::atoi(m_SoftR.GetBuffer(0));
// 	int g = ::atoi(m_SoftG.GetBuffer(0));
// 	int b = ::atoi(m_SoftB.GetBuffer(0));
// 
// 	RwRGBAReal RGBA;
// 	RGBA.red	= static_cast<RwReal>(r) / 255.0f;
// 	RGBA.green	= static_cast<RwReal>(g) / 255.0f;
// 	RGBA.blue	= static_cast<RwReal>(b) / 255.0f;
// 	RGBA.alpha	= 1.0f;
// 
// 	if(!dGET_BRUSH_ENTITY())
// 	{
// 		if(dGETHDL()->m_SelectedSS)
// 		{
// 			pSpotEntity = dGETHDL()->m_SelectedSS;
// 		}
// 		else
// 		{
// 			return;
// 		}
// 	}
// 	else
// 	{
// 		pSpotEntity = static_cast<CNtlPLSpot*>(dGET_BRUSH_ENTITY());
// 	}
// 
// 	pSpotEntity->SetRadius(static_cast<RwReal>(::atof(m_SoftRadius.GetBuffer(0))));
// 	pSpotEntity->SetAngle(static_cast<RwReal>(::atof(m_SoftAngle.GetBuffer(0))));
// 	pSpotEntity->SetColor(RGBA);
// 
// 	return;
// }
// 
// BOOL CPaletteSoftSpot::PreTranslateMessage(MSG* pMsg)
// {
// 	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
// 	{		
// 		if(	pMsg->hwnd == GetDlgItem(IDC_SOFT_R)->m_hWnd ||
// 			pMsg->hwnd == GetDlgItem(IDC_SOFT_G)->m_hWnd ||
// 			pMsg->hwnd == GetDlgItem(IDC_SOFT_B)->m_hWnd)
// 
// 		{
// 			CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
// 
// 			UpdateData(TRUE);
// 
// 			int r = ::atoi(m_SoftR.GetBuffer(0));
// 			int g = ::atoi(m_SoftG.GetBuffer(0));
// 			int b = ::atoi(m_SoftB.GetBuffer(0));
// 
// 			m_clrDiffuser = RGB(r, g, b);
// 
// 			UpdateData(FALSE);
// 
// 			SetCurSoftSpotProp();
// 		}
// 
// 		if(	pMsg->hwnd == GetDlgItem(IDC_ANGLE)->m_hWnd)
// 		{
// 			UpdateData(TRUE);
// 			SetCurSoftSpotProp();
// 		}
// 
// 		if(	pMsg->hwnd == GetDlgItem(IDC_RADIUS)->m_hWnd)
// 		{
// 			UpdateData(TRUE);
// 			SetCurSoftSpotProp();
// 		}
// 	}
// 
// 	// prevent keys; escape, return
// 	if(pMsg->message == WM_KEYDOWN)
// 	{
// 		switch(pMsg->wParam)
// 		{
// 		case VK_ESCAPE:
// 			{
// 				return true;
// 				break;
// 			}
// 		case VK_RETURN:
// 			{
// 				return true;
// 				break;
// 			}
// 		default:
// 			break;
// 		}
// 	}
// 
// 	return CDialog::PreTranslateMessage(pMsg);
// }
// 
// LRESULT CPaletteSoftSpot::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
// {
// 	switch (message)
// 	{
// 	case WM_CLOSE:
// 		{
// 			dGET_CUR_PALETTE() = ePM_DEFAULT;
// 			dGETHDL()->DeleteAllPaletteDummies();
// 			break;
// 		}
// 	}
// 
// 	return CDialog::WindowProc(message, wParam, lParam);
// }
// 
// void CPaletteSoftSpot::OnBnClickedSoftCreateBtn()
// {
// 	dGETHDL()->DeleteAllPaletteDummies();
// 
// 	// update display brush entity
// 	if(dGET_BRUSH_ENTITY())
// 	{
// 		GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
// 		dGET_BRUSH_ENTITY() = NULL;
// 	}
// 
// 	SPLEntityCreateParam CreateParam;
// 	RwV3d				Pos = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;
// 
// 	Pos.y += dDUMMY_HEIGHT;
// 	CreateParam.pPos = &Pos;
// 
// 	dGET_BRUSH_ENTITY() = static_cast<CNtlPLEntity*>(GetSceneManager()->CreateEntity(PLENTITY_SPOT_LIGHT, NTL_PLEN_SPOT_LIGHT, &CreateParam));
// 	DBO_ASSERT(dGET_BRUSH_ENTITY(), "Entity create failed.");
// }

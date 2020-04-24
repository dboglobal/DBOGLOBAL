// MPPGraphicPane.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MrPoPo.h"
#include "MPPGraphicPane.h"
#include "MppClientEvent.h"


// CMPPGraphicPane

IMPLEMENT_DYNCREATE(CMPPGraphicPane, CXTResizeFormView)

CMPPGraphicPane::CMPPGraphicPane()
	: CXTResizeFormView(CMPPGraphicPane::IDD)
{

}

CMPPGraphicPane::~CMPPGraphicPane()
{
}

void CMPPGraphicPane::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TERRAIN_DIST, m_sdTerrainDist);
	DDX_Control(pDX, IDC_TERRAIN_SHADOW, m_btnTerrainShadow);
	DDX_Control(pDX, IDC_WATER_SPECULAR, m_btnWaterSpecular);
	DDX_Control(pDX, IDC_CHAR_EDGE, m_btnCharEdge);
	DDX_Control(pDX, IDC_CHAR_GRAY_COLOR, m_btnCharGrayColor);
	DDX_Control(pDX, IDC_EFFECT_LOWSPEC, m_btnLowSpecEffect);
	DDX_Control(pDX, IDC_EFFECT_PARTICLE_RATIO, m_sdPraticleRatio);
	DDX_Control(pDX, IDC_EFFECT_MESH, m_btnMeshEffect);
	DDX_Control(pDX, IDC_EFFECT_DECAL, m_btnDecalEffect);
	DDX_Control(pDX, IDC_CHAR_DIST, m_sldCharDist);
}

BEGIN_MESSAGE_MAP(CMPPGraphicPane, CXTResizeFormView)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_TERRAIN_DIST, &CMPPGraphicPane::OnNMReleasedcaptureTerrainDist)
	ON_BN_CLICKED(IDC_TERRAIN_SHADOW, &CMPPGraphicPane::OnBnClickedTerrainShadow)
	ON_BN_CLICKED(IDC_WATER_SPECULAR, &CMPPGraphicPane::OnBnClickedWaterSpecular)
	ON_BN_CLICKED(IDC_CHAR_EDGE, &CMPPGraphicPane::OnBnClickedCharEdge)
	ON_BN_CLICKED(IDC_CHAR_GRAY_COLOR, &CMPPGraphicPane::OnBnClickedCharGrayColor)
	ON_BN_CLICKED(IDC_EFFECT_LOWSPEC, &CMPPGraphicPane::OnBnClickedEffectLowspec)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_EFFECT_PARTICLE_RATIO, &CMPPGraphicPane::OnNMReleasedcaptureEffectParticleRatio)
	ON_BN_CLICKED(IDC_EFFECT_MESH, &CMPPGraphicPane::OnBnClickedEffectMesh)
	ON_BN_CLICKED(IDC_EFFECT_DECAL, &CMPPGraphicPane::OnBnClickedEffectDecal)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_CHAR_DIST, &CMPPGraphicPane::OnNMReleasedcaptureCharDist)
END_MESSAGE_MAP()


// CMPPGraphicPane 진단입니다.

#ifdef _DEBUG
void CMPPGraphicPane::AssertValid() const
{
	CXTResizeFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMPPGraphicPane::Dump(CDumpContext& dc) const
{
	CXTResizeFormView::Dump(dc);
}
#endif
#endif //_DEBUG

void CMPPGraphicPane::OnInitialUpdate()
{
    CXTResizeFormView::OnInitialUpdate();

    m_sdTerrainDist.SetRange(200, 512);  
	m_sdTerrainDist.SetTicFreq(10);
	m_sdTerrainDist.SetPos(512);

	m_btnTerrainShadow.SetCheck(TRUE);
	m_btnWaterSpecular.SetCheck(TRUE);

	m_sldCharDist.SetRange(50, 150);  
	m_sldCharDist.SetTicFreq(10);
	m_sldCharDist.SetPos(150);
	m_btnCharEdge.SetCheck(TRUE);
	m_btnCharGrayColor.SetCheck(TRUE);

	m_btnLowSpecEffect.SetCheck(FALSE);
	m_sdPraticleRatio.SetRange(1, 10);       
	m_sdPraticleRatio.SetTicFreq(10);
	m_sdPraticleRatio.SetPos(10);
	m_btnMeshEffect.SetCheck(TRUE);
	m_btnDecalEffect.SetCheck(TRUE);
}


// CMPPGraphicPane 메시지 처리기입니다.

/*

void CMPPGraphicPane::OnBnClickedTerrainShadow()
{
    static BOOL bShadow = TRUE;
    bShadow = !bShadow;

    GetMPPClientEvent()->SendEvent(MSG_MRPOPO_SPEC_TERRAIN_SHADOW, bShadow);
}

void CMPPGraphicPane::OnBnClickedWaterSpecular()
{
    static BOOL bSpecular = TRUE;
    bSpecular = !bSpecular;

    GetMPPClientEvent()->SendEvent(MSG_MRPOPO_SPEC_WATER_SPECULAR, bSpecular);
}

void CMPPGraphicPane::OnBnClickedCharEdge()
{
    static BOOL bEdge = TRUE;
    bEdge = !bEdge;

    GetMPPClientEvent()->SendEvent(MSG_MRPOPO_SPEC_CHAR_EDGE, bEdge);
}

void CMPPGraphicPane::OnBnClickedEffect()
{
    static BOOL bEffect = TRUE;
    bEffect = !bEffect;

    GetMPPClientEvent()->SendEvent(MSG_MRPOPO_SPEC_EFFECT, bEffect);
}

void CMPPGraphicPane::OnNMReleasedcaptureTerrainDist(NMHDR *pNMHDR, LRESULT *pResult)
{
    GetMPPClientEvent()->SendEvent(MSG_MRPOPO_SPEC_TERRAIN_DIST, m_sdTerrainDist.GetPos());

    *pResult = 0;
}

*/

void CMPPGraphicPane::OnNMReleasedcaptureTerrainDist(NMHDR *pNMHDR, LRESULT *pResult)
{
	GetMPPClientEvent()->SendEvent(MSG_MRPOPO_SPEC_TERRAIN_DIST, m_sdTerrainDist.GetPos());
	
	*pResult = 0;
}


void CMPPGraphicPane::OnBnClickedTerrainShadow()
{
	BOOL bCheck = m_btnTerrainShadow.GetCheck();
	GetMPPClientEvent()->SendEvent(MSG_MRPOPO_SPEC_TERRAIN_SHADOW, bCheck);
}

void CMPPGraphicPane::OnBnClickedWaterSpecular()
{
	BOOL bCheck = m_btnWaterSpecular.GetCheck();
	GetMPPClientEvent()->SendEvent(MSG_MRPOPO_SPEC_WATER_SPECULAR, bCheck);
}

void CMPPGraphicPane::OnBnClickedCharEdge()
{
	BOOL bCheck = m_btnCharEdge.GetCheck();
	GetMPPClientEvent()->SendEvent(MSG_MRPOPO_SPEC_CHAR_EDGE, bCheck);
}

void CMPPGraphicPane::OnBnClickedCharGrayColor()
{
	BOOL bCheck = m_btnCharGrayColor.GetCheck();
	GetMPPClientEvent()->SendEvent(MSG_MRPOPO_SPEC_CHAR_GRAYCOLOR, bCheck);
}

void CMPPGraphicPane::OnBnClickedEffectLowspec()
{
	BOOL bCheck = m_btnLowSpecEffect.GetCheck();
	GetMPPClientEvent()->SendEvent(MSG_MRPOPO_SPEC_EFFECT, bCheck);
}

void CMPPGraphicPane::OnNMReleasedcaptureEffectParticleRatio(NMHDR *pNMHDR, LRESULT *pResult)
{
	GetMPPClientEvent()->SendEvent(MSG_MRPOPO_SPEC_EFFECT_PARTICLE_RATIO, m_sdPraticleRatio.GetPos());

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void CMPPGraphicPane::OnBnClickedEffectMesh()
{
	BOOL bCheck = m_btnMeshEffect.GetCheck();
	GetMPPClientEvent()->SendEvent(MSG_MRPOPO_SPEC_EFFECT_MESH, bCheck);
}

void CMPPGraphicPane::OnBnClickedEffectDecal()
{
	BOOL bCheck = m_btnDecalEffect.GetCheck();
	GetMPPClientEvent()->SendEvent(MSG_MRPOPO_SPEC_EFFECT_DECAL, bCheck);
}

void CMPPGraphicPane::OnNMReleasedcaptureCharDist(NMHDR *pNMHDR, LRESULT *pResult)
{
	GetMPPClientEvent()->SendEvent(MSG_MRPOPO_SPEC_CHAR_DIST, m_sldCharDist.GetPos());

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

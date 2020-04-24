// EffectEditLayer.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Venus.h"
#include "VenusFrame.h"
#include "EffectEditLayer.h"

#include ".\venusvisualmanager.h"

#include "EffectListDialog.h"
#include "ActionListDialog.h"
#include "EffectPropertyDialog.h"
#include ".\effecteditlayer.h"


// CEffectEditLayer

IMPLEMENT_DYNCREATE(CEffectEditLayer, CView)

CEffectEditLayer::CEffectEditLayer()
{
	m_bInitialized	= FALSE;
}

CEffectEditLayer::~CEffectEditLayer()
{
}

BEGIN_MESSAGE_MAP(CEffectEditLayer, CView)
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()


// CEffectEditLayer 그리기입니다.

void CEffectEditLayer::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CEffectEditLayer 진단입니다.

#ifdef _DEBUG
void CEffectEditLayer::AssertValid() const
{
	CView::AssertValid();
}

void CEffectEditLayer::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


// CEffectEditLayer 메시지 처리기입니다.

void CEffectEditLayer::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_RollupControl.Create( WS_VISIBLE|WS_CHILD, CRect(0,1,190,400), this, IDD_ROLLUPPAGEID);

	m_RollupControl.InsertPage("Effect List",		IDD_EFFECT_LIST,		RUNTIME_CLASS(CEffectListDialog));	
	m_RollupControl.InsertPage("Action List",		IDD_ACTION_LIST,		RUNTIME_CLASS(CActionListDialog));
	m_RollupControl.InsertPage("Effect Property",	IDD_EFFECT_PROPERTY,	RUNTIME_CLASS(CEffectPropertyDialog));

	m_RollupControl.ExpandAllPages();

	m_RollupControl.EnablePage(EDIT_DIALOG_ACTION_LIST, FALSE);
	m_RollupControl.ExpandPage(EDIT_DIALOG_ACTION_LIST, FALSE);

	m_bInitialized = TRUE;			
}

void CEffectEditLayer::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(m_bInitialized)
	{
		m_RollupControl.MoveWindow(0, 0, cx, cy);
	}
}


void CEffectEditLayer::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	gSetEditLayer(EDIT_LAYER_EFFECT_EDIT);
}

void CEffectEditLayer::ResetTreeItemName(const RwChar* strName)
{
	RC_PAGEINFO* pginfo = m_RollupControl.GetPageInfo(EDIT_DIALOG_EFFECT_LIST);
	if(pginfo && pginfo->pwndTemplate)
	{
		CEffectListDialog* pDialog = (CEffectListDialog*)pginfo->pwndTemplate;					
		pDialog->ResetTreeItemName(strName);
	}
}

void CEffectEditLayer::ResetProperties()
{
	RC_PAGEINFO* pginfo = m_RollupControl.GetPageInfo(EDIT_DIALOG_ACTION_LIST);
	if(pginfo && pginfo->pwndTemplate)
	{
		CActionListDialog* pDialog = (CActionListDialog*)pginfo->pwndTemplate;					
		pDialog->ResetResourceSystem();
	}
	m_RollupControl.EnablePage(EDIT_DIALOG_ACTION_LIST, CVenusVisualManager::GetInstance().m_pResourceSystem != NULL);
	m_RollupControl.ExpandPage(EDIT_DIALOG_ACTION_LIST, CVenusVisualManager::GetInstance().m_pResourceSystem != NULL);

	ResetPropertiesOnly();
}

void CEffectEditLayer::ResetPropertiesOnly()
{
	RC_PAGEINFO* pginfo = m_RollupControl.GetPageInfo(EDIT_DIALOG_PROPERTY);
	if(pginfo && pginfo->pwndTemplate)
	{
		CEffectPropertyDialog* pDialog = (CEffectPropertyDialog*)pginfo->pwndTemplate;					
		pDialog->ResetProperties();
	}
}

//------------------------------------------------------------------
//	FuncName	: RebuildResourceEffect
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CEffectEditLayer::RebuildResourceEffect()
{
	RC_PAGEINFO* pginfo = m_RollupControl.GetPageInfo(EDIT_DIALOG_EFFECT_LIST);
	if(pginfo && pginfo->pwndTemplate)
	{
		CEffectListDialog* pDialog = (CEffectListDialog*)pginfo->pwndTemplate;					
		pDialog->RebuildResourceEffect();   
	}

	CVenusVisualManager::GetInstance().SetSelectResourceData(NULL, NULL, 0);
	ResetProperties();
}

RwBool CEffectEditLayer::SaveXMLTree( RwChar* szFileName ) 
{
    RC_PAGEINFO* pginfo = m_RollupControl.GetPageInfo(EDIT_DIALOG_EFFECT_LIST);
    if(pginfo && pginfo->pwndTemplate)
    {
        CEffectListDialog* pDialog = (CEffectListDialog*)pginfo->pwndTemplate;					
        return pDialog->SaveXMLTree(szFileName);
    }    

    return FALSE;
}
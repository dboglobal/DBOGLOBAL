// WeatherDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "NtlWEDoc.h"
#include "WeatherDlg.h"

#include "NtlWorldPaletteDefine.h"
#include "FieldUIManager.h"

#include "NtlWorldFieldManager.h"
#include "NtlPLPropertyContainer.h"
#include "NtlPLWeatherProperty.h"
#include "NtlPLWeatherHandler.h"
#include "NtlPLWorldEntity.h"

#include "WorldViewDlg.h"


// WeatherDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CWeatherDlg, CDialog)

CWeatherDlg::CWeatherDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWeatherDlg::IDD, pParent)
{

}

CWeatherDlg::~CWeatherDlg()
{
}

void CWeatherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_WEATHER1, m_cbWeather[0]);
	DDX_Control(pDX, IDC_COMBO_WEATHER2, m_cbWeather[1]);
	DDX_Control(pDX, IDC_COMBO_WEATHER3, m_cbWeather[2]);
	DDX_Control(pDX, IDC_COMBO_WEATHER4, m_cbWeather[3]);
	DDX_Control(pDX, IDC_COMBO_WEATHER5, m_cbWeather[4]);
}


BEGIN_MESSAGE_MAP(CWeatherDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CWeatherDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_REFRESH, &CWeatherDlg::OnBnClickedRefresh)
END_MESSAGE_MAP()


// WeatherDlg 메시지 처리기입니다.

BOOL CWeatherDlg::PreTranslateMessage(MSG* pMsg)
{
	// prevent keys; escape, return
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_ESCAPE:
			{
				return true;
				break;
			}
		case VK_RETURN:
			{
				return true;
				break;
			}
		default:
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CWeatherDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (!UpdateCbWeather())
	{
		return FALSE;
	}

	UpdateData(FALSE);

	return TRUE;
}

RwBool CWeatherDlg::UpdateCbWeather()
{
	char acKeyName[256];
	sprintf_s(acKeyName, 256, "%s_%u", NTL_PLEN_WEATHER, 0);

	CNtlWorldFieldManager*				pMgr		= dGETMGR();
	CNtlPLWeatherProperty*				pProperty	= static_cast<CNtlPLWeatherProperty*>(CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_WEATHER, acKeyName));

	if (!pProperty)
	{
		return FALSE;
	}

	for (int i = 0; i < 5; ++i)
	{
		m_cbWeather[i].ResetContent();
		CNtlPLWeatherProperty::EMITTER_MAP& mapEmitter = pProperty->m_mapEmitter;
		m_cbWeather[i].SetItemData(m_cbWeather[i].AddString("None"), -1);
		for (CNtlPLWeatherProperty::EMITTER_MAP_IT it = mapEmitter.begin(); it != mapEmitter.end(); ++it)
		{
			m_cbWeather[i].SetItemData(m_cbWeather[i].AddString(it->second.strName.c_str()), it->first);
		}
	}

	sNTL_FIELD_PROP				NtlFieldProp;
	CNtlWorldFieldManager*		pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();

	if (dGETDOC()->GetFieldProp(NtlFieldProp))
	{
		SelectCbWeather(&NtlFieldProp);
	}
	else
	{
		SelectCbWeather(NULL);
	}

// 	if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
// 	{
// 		RwBool					IsAField	= dGETHDL()->m_pFieldUIManager->IsSingleSelected();
// 
// 		if(IsAField)
// 		{
// 			RwV3d CurFieldMidPosInUI	= dGETHDL()->m_pFieldUIManager->GetFieldMidPos();
// 			RwV3d CurFieldMidPosInWorld = dGETHDL()->m_pFieldUIManager->GetWorldPT(CurFieldMidPosInUI);
// 			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(CurFieldMidPosInWorld, FALSE);
// 
// 			if(!pFieldMgr->GetAFieldProp(CurFieldMidPosInWorld, NtlFieldProp))
// 			{
// 				DBO_TRACE(FALSE, "CWeatherDlg::OnInitDialog, can't get a field property.");
// 			}
// 
// 			SelectCbWeather(&NtlFieldProp);
// 		}
// 		else
// 		{
// 			SelectCbWeather(NULL);
// 		}
// 	}
// 	else if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
// 	{
// 		RwInt32 iXNum, iYNum;
// 		dGETFRM()->m_pWorldViewDlg->GetSelectBlockNum(&iXNum, &iYNum);
// 		if (dGETFRM()->m_pWorldViewDlg->GetBlockMode() == EBLOCK_MODE_BLOCK && iXNum == 1 && iYNum == 1)
// 		{
// 			if(!pFieldMgr->GetAFieldProp(dGETFRM()->m_pWorldViewDlg->GetSelectBlockMidPos(), NtlFieldProp))
// 			{
// 				DBO_TRACE(FALSE, "CWeatherDlg::OnInitDialog, can't get a field property.");
// 			}
// 			SelectCbWeather(&NtlFieldProp);
// 		}
// 		else
// 		{
// 			SelectCbWeather(NULL);
// 		}
// 	}

	return TRUE;
}

void CWeatherDlg::SelectCbWeather(sNTL_FIELD_PROP* pNtlFieldProp)
{
	for (int i = 0; i < 5; ++i)
	{
		RwInt32 iSelectIdx = -1;
		for (int j = 0; j < m_cbWeather[i].GetCount(); ++j)
		{
			if (pNtlFieldProp && m_cbWeather[i].GetItemData(j) == pNtlFieldProp->_IdxWeather[i])
			{
				iSelectIdx = j;
				break;
			}
			else if (m_cbWeather[i].GetItemData(j) == -1)
			{
				iSelectIdx = j;
			}
		}
		m_cbWeather[i].SetCurSel(iSelectIdx);
	}
}

void CWeatherDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	for (int i = 0; i < 5; ++i)
	{
		static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_IdxWeather[i] = m_cbWeather[i].GetItemData(m_cbWeather[i].GetCurSel());
	}

	OnOK();
}

void CWeatherDlg::OnBnClickedRefresh()
{
	CNtlXMLDoc doc;
	doc.Create();
	if(doc.Load(".\\script\\WeatherProperty.xml") == false)
	{
		::AfxMessageBox("property load failed");
		return;
	}

	char				chBuffer[NTL_MAX_DIR_PATH]	= {0,};
	IXMLDOMNodeList*	pList						= doc.SelectNodeList((char*)"/weather_property/weather");

	long lNum; 
	pList->get_length(&lNum);

	if (pList)
	{
		pList->Release();
		pList = NULL;
	}

	doc.Destroy();

	char acKeyName[256];
	sprintf_s(acKeyName, 256, "%s_%u", NTL_PLEN_WEATHER, 0);
	CNtlPLWeatherProperty* pWeatherProperty = static_cast<CNtlPLWeatherProperty*>(CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_WEATHER, acKeyName));

	if (!pWeatherProperty)
	{
		::AfxMessageBox("weather property invalid");
		return;
	}
	if (lNum != pWeatherProperty->m_mapEmitter.size())
	{
		::AfxMessageBox("weather property, set size is not same");
		return;
	}

	dGETMGR()->GetWeatherHandler()->InitWeather();

	for (RwInt32 i = 0; i < 36; ++i)
	{
		if(dGETMGR()->IsFieldLoaded(dGETMGR()->m_Fields6x6[1][i]))
		{
			dGETMGR()->DeleteFieldInMemory(dGETMGR()->m_Fields6x6[1][i], FALSE);
		}
	}

	CNtlPLPropertyContainer::GetInstance()->ReloadWeather();

	for (RwInt32 i = 0; i < 36; ++i)
	{
		dGETMGR()->CreateFields(dGETMGR()->m_Fields6x6[1][i]);
	}

	dGETMGR()->SetAnotherField();

 	OnCancel();
}

// ObjPropPopUp.cpp : 구현 파일입니다.
//

#include "stdafx.h"

#include "NtlWE.h"
#include "NtlWEDoc.h"

#include "rtltmap.h"

#include "ntltext.h"

#include "NtlSLAPI.h"

#include "TableContainer.h"
#include "TextAllTable.h"
#include "Table.h"

#include "NtlWorldFieldManager.h"
#include "NtlWorldFieldManager4RwWorld.h"

#include "NtlPLWorldEntity.h"
#include "NtlPLObject.h"

#include "ObjPropPopUp.h"

// CObjPropPopUp 대화 상자입니다.

IMPLEMENT_DYNAMIC(CObjPropPopUp, CDialog)

CObjPropPopUp::CObjPropPopUp(CWnd* pParent /*=NULL*/)
	: CDialog(CObjPropPopUp::IDD, pParent)
	, m_strGUID(_T(""))
	, m_strAABB(_T(""))
	, m_strMax(_T(""))
	, m_CurName(_T("null"))
	, m_NameIndexInput(_T(""))
	, m_strVisibilityDist(_T(""))
	, m_DoodadName(_T(""))
	, m_MilePostGUID(_T("0"))
	, m_MilePostMemo(_T("null"))
{

}

CObjPropPopUp::~CObjPropPopUp()
{
}

void CObjPropPopUp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SID, m_strGUID);
	DDX_Text(pDX, IDC_AABB, m_strAABB);
	//DDX_Text(pDX, IDC_VISIBILITY_DIST, m_strVisibilityDist);
	DDX_Control(pDX, IDC_TRIGGER, m_EnableTrigger);
	DDX_Text(pDX, IDC_AABB_MAX, m_strMax);
	DDX_Control(pDX, IDC_BITPROP, m_cbBitProp);
	DDX_Control(pDX, IDC_LTMAP_FLAGS_OBJECT, m_cbLtMapFlagsObject);
	DDX_Control(pDX, IDC_LTMAP_FLAGS_MATERIAL, m_cbLtMapFlagsMaterial);
	DDX_Text(pDX, IDC_NAME_INDEX_CUR, m_CurName);
	DDX_Text(pDX, IDC_NAME_INDEX_INPUT, m_NameIndexInput);
	DDX_Text(pDX, IDC_VISIBLE_DISTANCE, m_strVisibilityDist);
	DDX_Text(pDX, IDC_OBJ_NAME, m_DoodadName);
	DDX_Text(pDX, IDC_OBJ_ID, m_DoodadID);
	DDX_Text(pDX, IDC_MILEPOST_GUID, m_MilePostGUID);
	DDX_Text(pDX, IDC_MILEPOST_MEMO, m_MilePostMemo);
	DDX_Control(pDX, IDC_MILEPOST_GUID, m_MilePostEditCtrl);
	DDX_Control(pDX, IDC_OBJECT_TYPE, m_cbObjectType);
	DDX_Control(pDX, IDC_MINIMAP_LAYER, m_cbMinimapLayer);
}


BEGIN_MESSAGE_MAP(CObjPropPopUp, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_TRIGGER, &CObjPropPopUp::OnBnClickedTrigger)
	ON_EN_CHANGE(IDC_VISIBILITY_DIST, &CObjPropPopUp::OnEnChangeVisibilityDist)
	ON_BN_CLICKED(IDOK, &CObjPropPopUp::OnBnClickedOk)
END_MESSAGE_MAP()


// CObjPropPopUp 메시지 처리기입니다.

BOOL CObjPropPopUp::OnInitDialog()
{
	CDialog::OnInitDialog();

	// add string
	m_cbMinimapLayer.ResetContent();
	m_cbMinimapLayer.AddString("LAYER_ONE");
	m_cbMinimapLayer.AddString("LAYER_TWO");

	m_cbBitProp.ResetContent();
	m_cbBitProp.AddString("Alterable");
	m_cbBitProp.AddString("Selection");
	m_cbBitProp.AddString("Portal");
	m_cbBitProp.AddString("P.E. Collision");
	m_cbBitProp.AddString("Shadow Prop");

	m_cbLtMapFlagsObject.ResetContent();
	m_cbLtMapFlagsObject.AddString("rtLTMAPOBJECTLIGHTMAP");
	m_cbLtMapFlagsObject.AddString("rtLTMAPOBJECTVERTEXLIGHT");
	m_cbLtMapFlagsObject.AddString("rtLTMAPOBJECTNOSHADOW");
	m_cbLtMapFlagsObject.EnableWindow(GetSceneManager()->GetActiveWorldType() == AW_RWWORLD);

	m_cbLtMapFlagsMaterial.ResetContent();
	m_cbLtMapFlagsMaterial.AddString("rtLTMAPMATERIALLIGHTMAP");
	m_cbLtMapFlagsMaterial.AddString("rtLTMAPMATERIALAREALIGHT");
	m_cbLtMapFlagsMaterial.AddString("rtLTMAPMATERIALNOSHADOW");
	m_cbLtMapFlagsMaterial.AddString("rtLTMAPMATERIALSKY");
	m_cbLtMapFlagsMaterial.AddString("rtLTMAPMATERIALFLATSHADE");
	m_cbLtMapFlagsMaterial.AddString("rtLTMAPMATERIALVERTEXLIGHT");
	m_cbLtMapFlagsMaterial.EnableWindow(GetSceneManager()->GetActiveWorldType() == AW_RWWORLD);

/*
	CRect WindowSize;
	dGETFRM()->GetWindowRect(&WindowSize);

	// position the dialog
	POINT ScreenPos;
	//ScreenPos.x = (RwInt32)dGET_MOUSE()->GetPosClicked().x;
	//ScreenPos.y = (RwInt32)dGET_MOUSE()->GetPosClicked().y;
	ScreenPos.x = (RwInt32)WindowSize.Width() / 2;
	ScreenPos.y = (RwInt32)WindowSize.Height() / 2;;
	ClientToScreen(&ScreenPos);

	CRect myRect;
	GetClientRect(&myRect);
	ClientToScreen(myRect);
	MoveWindow(ScreenPos.x, ScreenPos.y, myRect.Width() + 10, myRect.Height() + 20);
*/
	// display doodads property
	CNtlPLObject* pNtlPLObject = dGETHDL()->GetCurDoodad();
	if (pNtlPLObject == NULL)
	{
		return FALSE;
	}
	
	// doodads name
	m_DoodadName.Format("%s", pNtlPLObject->GetName());
	m_DoodadID.Format("%u", pNtlPLObject->GetObjectSerialID());

	if (pNtlPLObject->GetProperty()->GetObjectType() == E_OBJECT_MILEPOST )
	{
		m_MilePostEditCtrl.EnableWindow(TRUE);
		m_MilePostGUID.Format("%d", pNtlPLObject->GetMilepostID());
						
		CTextTable*	pTextMobTbl	= static_cast<CTextTable*>(API_GetTableContainer()->GetTextAllTable()->GetMilePostTable());
		TBLIDX		TableIdx	= static_cast<TBLIDX>(pNtlPLObject->GetMilepostID());
		 
		std::wstring WStr;
		if(pTextMobTbl->GetText(TableIdx, &WStr))
		{
			RwChar MBStr[256];
			::WideCharToMultiByte(GetACP(), 0, WStr.c_str(), -1, MBStr, 256, NULL, NULL);
			m_MilePostMemo = MBStr;
		}
		else
		{
			m_MilePostMemo = "null";
		}
	}
	else
	{
		m_MilePostEditCtrl.EnableWindow(FALSE);
		m_MilePostGUID = "0";
		m_MilePostMemo = "null";
	}
	 
	// display doodads property; serial
	if(pNtlPLObject->GetSID() == 0xffffffff)
	{
		m_strGUID.Format("null");
	}
	else
	{
		m_strGUID.Format("%d", pNtlPLObject->GetSID());
	}


	// display object name index
	if(pNtlPLObject->GetObjNameIdx() == 0xffffffff || pNtlPLObject->GetObjNameIdx() == 999)
	{
		m_CurName.Format("null");
	}
	else
	{
		CTextTable*	pTextMobTbl	= static_cast<CTextTable*>(API_GetTableContainer()->GetTextAllTable()->GetObjectTbl());
		TBLIDX		TableIdx	= static_cast<TBLIDX>(pNtlPLObject->GetObjNameIdx());

		std::wstring WStr;
		if(pTextMobTbl->GetText(TableIdx, &WStr))
		{
			m_NtlDoodadsPropParam.NameIdx = static_cast<DWORD>(TableIdx);

			RwChar MBStr[256];
			::WideCharToMultiByte(GetACP(), 0, WStr.c_str(), -1, MBStr, 256, NULL, NULL);
			m_CurName = MBStr;
		}
		else
		{
			m_CurName.Format("null");
		}
	}

	// display doodads property; AABB
	RwBBox AABB = pNtlPLObject->GetTriggerAABBInfo();
	m_strAABB.Format("(%.2f, %.2f, %.2f)",	AABB.inf.x, AABB.inf.y, AABB.inf.z);
	m_strMax.Format("(%.2f, %.2f, %.2f)",	AABB.sup.x, AABB.sup.y, AABB.sup.z);

	m_strVisibilityDist.Format("%.2f", pNtlPLObject->GetVisibleCullingDistance());

	m_EnableTrigger.SetCheck(pNtlPLObject->GetFlags() & NTL_PLEFLAG_TRIGGER);

	m_cbObjectType.SetCurSel(pNtlPLObject->GetObjectType());

	m_cbMinimapLayer.SetCheck(0, pNtlPLObject->GetMinimapLayer() & NTL_PLEFLAG_MINIMAP_LAYER_ONE);
	m_cbMinimapLayer.SetCheck(1, pNtlPLObject->GetMinimapLayer() & NTL_PLEFLAG_MINIMAP_LAYER_TWO);

	m_cbBitProp.SetCheck(0, pNtlPLObject->GetFlags() & NTL_PLEFLAG_ALTERABLE);
	m_cbBitProp.SetCheck(1, pNtlPLObject->GetFlags() & NTL_PLEFLAG_SELECTION);
	m_cbBitProp.SetCheck(2, pNtlPLObject->GetFlags() & NTL_PLEFLAG_PORTAL);
	m_cbBitProp.SetCheck(3, pNtlPLObject->GetFlags() & NTL_PLEFLAG_PECOLLISION);
	m_cbBitProp.SetCheck(4, pNtlPLObject->GetFlags() & NTL_PLEFLAG_SHADOW_PROP);

	m_cbLtMapFlagsObject.SetCheck(0, pNtlPLObject->GetLightMapObjectFlags() & rtLTMAPOBJECTLIGHTMAP);
	m_cbLtMapFlagsObject.SetCheck(1, pNtlPLObject->GetLightMapObjectFlags() & rtLTMAPOBJECTVERTEXLIGHT);
	m_cbLtMapFlagsObject.SetCheck(2, pNtlPLObject->GetLightMapObjectFlags() & rtLTMAPOBJECTNOSHADOW);

	m_cbLtMapFlagsMaterial.SetCheck(0, pNtlPLObject->GetLightMapMaterialFlags() & rtLTMAPMATERIALLIGHTMAP);
	m_cbLtMapFlagsMaterial.SetCheck(1, pNtlPLObject->GetLightMapMaterialFlags() & rtLTMAPMATERIALAREALIGHT);
	m_cbLtMapFlagsMaterial.SetCheck(2, pNtlPLObject->GetLightMapMaterialFlags() & rtLTMAPMATERIALNOSHADOW);
	m_cbLtMapFlagsMaterial.SetCheck(3, pNtlPLObject->GetLightMapMaterialFlags() & rtLTMAPMATERIALSKY);
	m_cbLtMapFlagsMaterial.SetCheck(4, pNtlPLObject->GetLightMapMaterialFlags() & rtLTMAPMATERIALFLATSHADE);
	m_cbLtMapFlagsMaterial.SetCheck(5, pNtlPLObject->GetLightMapMaterialFlags() & rtLTMAPMATERIALVERTEXLIGHT);	

	sNTL_DOODADS_PROP_PARAM sNtlDoodadsPropParam = dGETHDL()->GetCurDoodadsPropParam();
	CopyMemory(&m_NtlDoodadsPropParam, &sNtlDoodadsPropParam, sizeof(sNTL_DOODADS_PROP_PARAM));
// 
// 	// doodads name
// 	m_DoodadName	= sNtlDoodadsPropParam._Name;
// 
// 	// Milepost
// 	if(sNtlDoodadsPropParam._MilePostEnable)
// 	{
// 		m_MilePostEditCtrl.EnableWindow(TRUE);
// 
// 		m_MilePostGUID.Format("%d", sNtlDoodadsPropParam._MilePostID);
// 
// 		CTextTable*	pTextMobTbl	= static_cast<CTextTable*>(API_GetTableContainer()->GetTextAllTable()->GetMilePostTable());
// 		TBLIDX		TableIdx	= static_cast<TBLIDX>(sNtlDoodadsPropParam._MilePostID);
// 
// 		std::wstring WStr;
// 		if(pTextMobTbl->GetText(TableIdx, &WStr))
// 		{
// 			RwChar MBStr[256];
// 			::WideCharToMultiByte(GetACP(), 0, WStr.c_str(), -1, MBStr, 256, NULL, NULL);
// 			m_MilePostMemo = MBStr;
// 		}
// 		else
// 		{
// 			m_MilePostMemo = "null";
// 		}
// 	}
// 	else
// 	{
// 		m_MilePostEditCtrl.EnableWindow(FALSE);
// 
// 		m_MilePostGUID = "0";
// 		m_MilePostMemo = "null";
// 	}
// 
// 	// display doodads property; serial
// 	if(sNtlDoodadsPropParam.SerialID == 0xffffffff)
// 	{
// 		m_strGUID.Format("null");
// 	}
// 	else
// 	{
// 		m_strGUID.Format("%d", sNtlDoodadsPropParam.SerialID);
// 	}
// 
// 	// display object name index
// 	if(sNtlDoodadsPropParam.NameIdx == 0xffffffff || sNtlDoodadsPropParam.NameIdx == 999)
// 	{
// 		m_CurName.Format("null");
// 	}
// 	else
// 	{
// 		CTextTable*	pTextMobTbl	= static_cast<CTextTable*>(API_GetTableContainer()->GetTextAllTable()->GetObjectTbl());
// 		TBLIDX		TableIdx	= static_cast<TBLIDX>(sNtlDoodadsPropParam.NameIdx);
// 
// 		std::wstring WStr;
// 		if(pTextMobTbl->GetText(TableIdx, &WStr))
// 		{
// 			m_NtlDoodadsPropParam.NameIdx = static_cast<DWORD>(TableIdx);
// 
// 			RwChar MBStr[256];
// 			::WideCharToMultiByte(GetACP(), 0, WStr.c_str(), -1, MBStr, 256, NULL, NULL);
// 			m_CurName = MBStr;
// 		}
// 		else
// 		{
// 			m_CurName.Format("null");
// 		}
// 	}
// 	
// 	// display doodads property; AABB
// 	m_strAABB.Format("(%.2f, %.2f, %.2f)",	sNtlDoodadsPropParam.AABB[0].x,
// 											sNtlDoodadsPropParam.AABB[0].y,
// 											sNtlDoodadsPropParam.AABB[0].z);
// 	m_strMax.Format("(%.2f, %.2f, %.2f)",	sNtlDoodadsPropParam.AABB[1].x,
// 											sNtlDoodadsPropParam.AABB[1].y,
// 											sNtlDoodadsPropParam.AABB[1].z);
// 
// 	m_strVisibilityDist.Format("%.2f", sNtlDoodadsPropParam.VisibilityDist);
// 
// 	if(sNtlDoodadsPropParam.EnableTrigger)
// 	{
// 		m_EnableTrigger.SetCheck(TRUE);
// 	}
// 	else
// 	{
// 		m_EnableTrigger.SetCheck(FALSE);
// 	}
// 
// 	if(sNtlDoodadsPropParam.EnableSelection)
// 	{
// 		m_cbBitProp.SetCheck(1, TRUE);
// 	}
// 	else
// 	{
// 		m_cbBitProp.SetCheck(1, FALSE);
// 	}
// 
// 	if(sNtlDoodadsPropParam.EnableAlterable)
// 	{
// 		m_cbBitProp.SetCheck(0, TRUE);
// 	}
// 	else
// 	{
// 		m_cbBitProp.SetCheck(0, FALSE);
// 	}
// 
// 	if(sNtlDoodadsPropParam.EnablePortal)
// 	{
// 		m_cbBitProp.SetCheck(2, TRUE);
// 	}
// 	else
// 	{
// 		m_cbBitProp.SetCheck(2, FALSE);
// 	}
// 
// 	if(sNtlDoodadsPropParam.EnablePECollision)
// 	{
// 		m_cbBitProp.SetCheck(3, TRUE);
// 	}
// 	else
// 	{
// 		m_cbBitProp.SetCheck(3, FALSE);
// 	}
// 
// 	if(sNtlDoodadsPropParam.EnableShadowProp)
// 	{
// 		m_cbBitProp.SetCheck(4, TRUE);
// 	}
// 	else
// 	{
// 		m_cbBitProp.SetCheck(4, FALSE);
// 	}

	UpdateData(FALSE);
	UpdateData(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CObjPropPopUp::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{		
		UpdateData(TRUE);

		if(pMsg->hwnd == GetDlgItem(IDC_NAME_INDEX_INPUT)->m_hWnd)
		{
			CTextTable*	pTextMobTbl	= static_cast<CTextTable*>(API_GetTableContainer()->GetTextAllTable()->GetObjectTbl());
			TBLIDX		TableIdx	= static_cast<TBLIDX>(atoi(m_NameIndexInput.GetBuffer(0)));

			std::wstring WStr;
			if(pTextMobTbl->GetText(TableIdx, &WStr))
			{
				m_NtlDoodadsPropParam.NameIdx = static_cast<DWORD>(TableIdx);

				RwChar MBStr[256];
				::WideCharToMultiByte(GetACP(), 0, WStr.c_str(), -1, MBStr, 256, NULL, NULL);
				m_CurName = MBStr;
			}
			else
			{
				AfxMessageBox("Error codes, please try again");
			}
		}
		else if(pMsg->hwnd == GetDlgItem(IDC_MILEPOST_GUID)->m_hWnd)
		{
			CTextTable*	pTextMobTbl	= static_cast<CTextTable*>(API_GetTableContainer()->GetTextAllTable()->GetMilePostTable());
			TBLIDX		TableIdx	= static_cast<TBLIDX>(atoi(m_MilePostGUID.GetBuffer(0)));

			std::wstring WStr;
			if(pTextMobTbl->GetText(TableIdx, &WStr))
			{
				m_NtlDoodadsPropParam._MilePostID = static_cast<RwUInt32>(TableIdx);

				RwChar MBStr[256];
				::WideCharToMultiByte(GetACP(), 0, WStr.c_str(), -1, MBStr, 256, NULL, NULL);
				m_MilePostMemo = MBStr;
			}
			else
			{
				AfxMessageBox("Error codes, please try again");
			}
		}

		if(pMsg->hwnd == GetDlgItem(IDC_VISIBLE_DISTANCE)->m_hWnd && pMsg->wParam == VK_RETURN)
		{
			m_NtlDoodadsPropParam.VisibilityDist = static_cast<RwReal>(atof(m_strVisibilityDist.GetBuffer(0)));
			OnBnClickedOk();
		}

		UpdateData(FALSE);
	}

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

void CObjPropPopUp::OnDestroy()
{
	CNtlWEDoc* pNtlWEDoc = static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc);

	m_NtlDoodadsPropParam.EnableSelection	= m_cbBitProp.GetCheck(0);
	m_NtlDoodadsPropParam.EnableAlterable	= m_cbBitProp.GetCheck(1);
	m_NtlDoodadsPropParam.EnablePortal		= m_cbBitProp.GetCheck(2);
	m_NtlDoodadsPropParam.EnablePECollision	= m_cbBitProp.GetCheck(3);
	m_NtlDoodadsPropParam.EnableShadowProp	= m_cbBitProp.GetCheck(4);

	CopyMemory(&pNtlWEDoc->m_NtlDoodadsPropParam, &m_NtlDoodadsPropParam, sizeof(sNTL_DOODADS_PROP_PARAM));

	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CObjPropPopUp::OnBnClickedTrigger()
{
	m_NtlDoodadsPropParam.EnableTrigger = static_cast<RwInt32>(m_EnableTrigger.GetCheck());

	if(m_EnableTrigger.GetCheck())
	{
		m_strGUID.Format("%d", dGET_WORLD_PARAM()->WorldTriggerSerialID);
		m_NtlDoodadsPropParam.SerialID = dGET_WORLD_PARAM()->WorldTriggerSerialID;
		UpdateData(FALSE);
	}
	else
	{
		m_strGUID.Format("null", dGET_WORLD_PARAM()->WorldTriggerSerialID);
		m_NtlDoodadsPropParam.SerialID = 0xffffffff;
		UpdateData(FALSE);
	}
}

void CObjPropPopUp::OnEnChangeVisibilityDist()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정하고  마스크에 OR 연산하여 설정된
	// ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출해야만
	// 해당 알림 메시지를 보냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_NtlDoodadsPropParam.VisibilityDist = static_cast<RwReal>(atof(m_strVisibilityDist.GetBuffer(0)));
}

void CObjPropPopUp::OnBnClickedOk()
{
	UpdateData(TRUE);

	CNtlPLObject* pNtlPLObject = dGETHDL()->GetCurDoodad();

	// LightMap Flags 는 OBJECT_CREATE_PARAM 안에 없으므로 여기서 직접 셋팅 한다.
	// 나중에 다른 부분도 이쪽으로 모두 로직을 옮긴다.
	
	sNTL_FIELD_PROP sNtlFieldProp;
	dGETMGR()->GetAFieldProp(pNtlPLObject->GetPosition(), sNtlFieldProp);
	if (pNtlPLObject->GetObjectType() == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
		pNtlPLObject->GetObjectType() == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
	{
		((CNtlPLObjectTypeHasProperty*)pNtlPLObject->GetObjectTypePtr())->GetObjectProperty(sNtlFieldProp);
	}
	
	pNtlPLObject->SetObjectType((m_cbObjectType.GetCurSel() == -1 ? 0 : m_cbObjectType.GetCurSel()));

	if (pNtlPLObject->GetObjectType() == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
		pNtlPLObject->GetObjectType() == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
	{
		((CNtlPLObjectTypeHasProperty*)pNtlPLObject->GetObjectTypePtr())->SetObjectProperty(sNtlFieldProp);
	}
	
	RwUInt32 uiMinimapLayer = 0x00000000;
	if (m_cbMinimapLayer.GetCheck(0))
	{
		uiMinimapLayer |= NTL_PLEFLAG_MINIMAP_LAYER_ONE;
	}
	if (m_cbMinimapLayer.GetCheck(1))
	{
		uiMinimapLayer |= NTL_PLEFLAG_MINIMAP_LAYER_TWO;
	}
	pNtlPLObject->SetMinimapLayer(uiMinimapLayer);
	
	if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	{
		RwUInt32 uiFlags;

		uiFlags = 0x00000000;
		if (m_cbLtMapFlagsObject.GetCheck(0))
		{
			uiFlags |= rtLTMAPOBJECTLIGHTMAP;
		}
		if (m_cbLtMapFlagsObject.GetCheck(1))
		{
			uiFlags |= rtLTMAPOBJECTVERTEXLIGHT;
		}
		if (m_cbLtMapFlagsObject.GetCheck(2))
		{
			uiFlags |= rtLTMAPOBJECTNOSHADOW;
		}
		pNtlPLObject->SetLightMapObjectFlags(uiFlags);

		uiFlags = 0x00000000;
		if (m_cbLtMapFlagsMaterial.GetCheck(0))
		{
			uiFlags |= rtLTMAPMATERIALLIGHTMAP;
		}
		if (m_cbLtMapFlagsMaterial.GetCheck(1))
		{
			uiFlags |= rtLTMAPMATERIALAREALIGHT;
		}
		if (m_cbLtMapFlagsMaterial.GetCheck(2))
		{
			uiFlags |= rtLTMAPMATERIALNOSHADOW;
		}
		if (m_cbLtMapFlagsMaterial.GetCheck(3))
		{
			uiFlags |= rtLTMAPMATERIALSKY;
		}
		if (m_cbLtMapFlagsMaterial.GetCheck(4))
		{
			uiFlags |= rtLTMAPMATERIALFLATSHADE;
		}
		if (m_cbLtMapFlagsMaterial.GetCheck(5))
		{
			uiFlags |= rtLTMAPMATERIALVERTEXLIGHT;
		}
		pNtlPLObject->SetLightMapMaterialFlags(uiFlags);
	}

	OnOK();
}

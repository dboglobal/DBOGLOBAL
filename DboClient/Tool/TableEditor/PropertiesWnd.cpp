#include "pch.h"
#include "framework.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "TableEditor.h"

#include "Util.h"
#include "ItemTable.h"
#include "NewbieTable.h"
#include "NpcSpeechTable.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define ADD_SUB_ITEM(x,y,z) pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T(x), (_variant_t)y, _T(z)))

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd()
{
	m_nTableType = -1;
	m_pTbldat = nullptr;
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd () == nullptr || (AfxGetMainWnd() != nullptr && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(nullptr, rectClient.left, rectClient.top + cyTlb, rectClient.Width(), rectClient.Height() -(cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

void CPropertiesWnd::LoadTableData(int nTableType, sTBLDAT* pTbldat)
{
	m_wndPropList.RemoveAll();

	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("TBLDAT"));

	switch (nTableType)
	{
		case CTableContainer::TABLE_ITEM:
		{
			sITEM_TBLDAT* pTableData = (sITEM_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("TBLIDX",						pTableData->tblidx,						 "Item Table Index");
			ADD_SUB_ITEM("bValidity_Able",				pTableData->bValidity_Able,				 "Set item active/inactive");
			ADD_SUB_ITEM("Name",						pTableData->Name,						 "Name Table Index");
			ADD_SUB_ITEM("wszNameText",					pTableData->wszNameText,				 "Item Name. Max 32 Chars");
			ADD_SUB_ITEM("szIcon_Name",					pTableData->szIcon_Name,				 "Icon Name");
			ADD_SUB_ITEM("byModel_Type",				(int)pTableData->byModel_Type,			 "Model Type");
			ADD_SUB_ITEM("szModel",						pTableData->szModel,					 "Model Name");
			ADD_SUB_ITEM("szSub_Weapon_Act_Model",		pTableData->szSub_Weapon_Act_Model,		 "Item Table Index");
			ADD_SUB_ITEM("byItem_Type",					(int)pTableData->byItem_Type,			 "Item Type");
			ADD_SUB_ITEM("byEquip_Type",				(int)pTableData->byEquip_Type,			 "Equipment Type");
			ADD_SUB_ITEM("dwEquip_Slot_Type_Bit_Flag",	pTableData->dwEquip_Slot_Type_Bit_Flag,  "Item Table Index");
			ADD_SUB_ITEM("wFunction_Bit_Flag",			pTableData->wFunction_Bit_Flag,			 "Item Table Index");
			ADD_SUB_ITEM("byMax_Stack",					(int)pTableData->byMax_Stack,			 "Item Table Index");
			ADD_SUB_ITEM("byRank",						(int)pTableData->byRank,				 "Item Table Index");
			ADD_SUB_ITEM("dwWeight",					pTableData->dwWeight,					 "Item Table Index");
			ADD_SUB_ITEM("dwCost",						pTableData->dwCost,						 "Item Table Index");
			ADD_SUB_ITEM("dwSell_Price",				pTableData->dwSell_Price,				 "Item Table Index");
			ADD_SUB_ITEM("byDurability",				(int)pTableData->byDurability,			 "Item Table Index");
			ADD_SUB_ITEM("byDurability_Count",			(int)pTableData->byDurability_Count,	 "Item Table Index");
			ADD_SUB_ITEM("byBattle_Attribute",			(int)pTableData->byBattle_Attribute,	 "Item Table Index");
			ADD_SUB_ITEM("wPhysical_Offence",			pTableData->wPhysical_Offence,			 "Item Table Index");
			ADD_SUB_ITEM("wEnergy_Offence",				pTableData->wEnergy_Offence,			 "Item Table Index");
			ADD_SUB_ITEM("wPhysical_Defence",			pTableData->wPhysical_Defence,			 "Item Table Index");
			ADD_SUB_ITEM("wEnergy_Defence",				pTableData->wEnergy_Defence,			 "Item Table Index");
			ADD_SUB_ITEM("fAttack_Range_Bonus",			pTableData->fAttack_Range_Bonus,		 "Item Table Index");
			ADD_SUB_ITEM("wAttack_Speed_Rate",			pTableData->wAttack_Speed_Rate,			 "Item Table Index");
			ADD_SUB_ITEM("byNeed_Min_Level",			(int)pTableData->byNeed_Min_Level,		 "Item Table Index");
			ADD_SUB_ITEM("byNeed_Max_Level",			(int)pTableData->byNeed_Max_Level,		 "Item Table Index");
			ADD_SUB_ITEM("dwNeed_Class_Bit_Flag",		pTableData->dwNeed_Class_Bit_Flag,		 "Item Table Index");
			ADD_SUB_ITEM("dwNeed_Gender_Bit_Flag",		pTableData->dwNeed_Gender_Bit_Flag,		 "Item Table Index");
			ADD_SUB_ITEM("byClass_Special",				(int)pTableData->byClass_Special,		 "Item Table Index");
			ADD_SUB_ITEM("byRace_Special",				(int)pTableData->byRace_Special,		 "Item Table Index");
			ADD_SUB_ITEM("wNeed_Str",					pTableData->wNeed_Str,					 "Item Table Index");
			ADD_SUB_ITEM("wNeed_Con",					pTableData->wNeed_Con,					 "Item Table Index");
			ADD_SUB_ITEM("wNeed_Foc",					pTableData->wNeed_Foc,					 "Item Table Index");
			ADD_SUB_ITEM("wNeed_Dex",					pTableData->wNeed_Dex,					 "Item Table Index");
			ADD_SUB_ITEM("wNeed_Sol",					pTableData->wNeed_Sol,					 "Item Table Index");
			ADD_SUB_ITEM("wNeed_Eng",					pTableData->wNeed_Eng,					 "Item Table Index");
			ADD_SUB_ITEM("set_Item_Tblidx",				pTableData->set_Item_Tblidx,			 "Item Table Index");
			ADD_SUB_ITEM("Note",						pTableData->Note,						 "Item Table Index");
			ADD_SUB_ITEM("byBag_Size",					(int)pTableData->byBag_Size,			 "Item Table Index");
			ADD_SUB_ITEM("wScouter_Watt",				pTableData->wScouter_Watt,				 "Item Table Index");
			ADD_SUB_ITEM("dwScouter_MaxPower",			pTableData->dwScouter_MaxPower,			 "Item Table Index");
			ADD_SUB_ITEM("byScouter_Parts_Type1",		(int)pTableData->byScouter_Parts_Type1,  "Item Table Index");
			ADD_SUB_ITEM("byScouter_Parts_Type2",		(int)pTableData->byScouter_Parts_Type2,  "Item Table Index");
			ADD_SUB_ITEM("byScouter_Parts_Type3",		(int)pTableData->byScouter_Parts_Type3,  "Item Table Index");
			ADD_SUB_ITEM("byScouter_Parts_Type4",		(int)pTableData->byScouter_Parts_Type4,  "Item Table Index");
			ADD_SUB_ITEM("Use_Item_Tblidx",				pTableData->Use_Item_Tblidx,			 "Item Table Index");
			ADD_SUB_ITEM("bIsCanHaveOption",			pTableData->bIsCanHaveOption,			 "Item Table Index");
			ADD_SUB_ITEM("Item_Option_Tblidx",			pTableData->Item_Option_Tblidx,			 "Item Table Index");
			ADD_SUB_ITEM("byItemGroup",					(int)pTableData->byItemGroup,			 "Item Table Index");
			ADD_SUB_ITEM("Charm_Tblidx",				pTableData->Charm_Tblidx,				 "Item Table Index");
			ADD_SUB_ITEM("wCostumeHideBitFlag",			pTableData->wCostumeHideBitFlag,		 "Item Table Index");
			ADD_SUB_ITEM("NeedItemTblidx",				pTableData->NeedItemTblidx,				 "Item Table Index");
			ADD_SUB_ITEM("CommonPoint",					pTableData->CommonPoint,				 "Item Table Index");
			ADD_SUB_ITEM("byCommonPointType",			(int)pTableData->byCommonPointType,		 "Item Table Index");
			ADD_SUB_ITEM("byNeedFunction",				(int)pTableData->byNeedFunction,		 "Item Table Index");
			ADD_SUB_ITEM("dwUseDurationMax",			pTableData->dwUseDurationMax,			 "Item Table Index");
			ADD_SUB_ITEM("byDurationType",				(int)pTableData->byDurationType,		 "Item Table Index");
			ADD_SUB_ITEM("contentsTblidx",				pTableData->contentsTblidx,				 "Item Table Index");
			ADD_SUB_ITEM("dwDurationGroup",				pTableData->dwDurationGroup,			 "Item Table Index");
			ADD_SUB_ITEM("byDropLevel",					(int)pTableData->byDropLevel,			 "Item Table Index");
			ADD_SUB_ITEM("enchantRateTblidx",			pTableData->enchantRateTblidx,			 "Item Table Index");
			ADD_SUB_ITEM("excellentTblidx",				pTableData->excellentTblidx,			 "Item Table Index");
			ADD_SUB_ITEM("rareTblidx",					pTableData->rareTblidx,					 "Item Table Index");
			ADD_SUB_ITEM("legendaryTblidx",				pTableData->legendaryTblidx,			 "Item Table Index");
			ADD_SUB_ITEM("bCreateSuperiorAble",			pTableData->bCreateSuperiorAble,		 "Item Table Index");
			ADD_SUB_ITEM("bCreateExcellentAble",		pTableData->bCreateExcellentAble,		 "Item Table Index");
			ADD_SUB_ITEM("bCreateRareAble",				pTableData->bCreateRareAble,			 "Item Table Index");
			ADD_SUB_ITEM("bCreateLegendaryAble",		pTableData->bCreateLegendaryAble,		 "Item Table Index");
			ADD_SUB_ITEM("byRestrictType",				(int)pTableData->byRestrictType,		 "Item Table Index");
			ADD_SUB_ITEM("fAttack_Physical_Revision",	pTableData->fAttack_Physical_Revision,	 "Item Table Index");
			ADD_SUB_ITEM("fAttack_Energy_Revision",		pTableData->fAttack_Energy_Revision,	 "Item Table Index");
			ADD_SUB_ITEM("fDefence_Physical_Revision",	pTableData->fDefence_Physical_Revision,  "Item Table Index");
			ADD_SUB_ITEM("fDefence_Energy_Revision",	pTableData->fDefence_Energy_Revision,	 "Item Table Index");
			ADD_SUB_ITEM("byTmpTabType",				(int)pTableData->byTmpTabType,			 "Item Table Index");
			ADD_SUB_ITEM("bIsCanRenewal",				pTableData->bIsCanRenewal,				 "Item Table Index");
			ADD_SUB_ITEM("wDisassemble_Bit_Flag",		pTableData->wDisassemble_Bit_Flag,		 "Item Table Index");
			ADD_SUB_ITEM("byDisassembleNormalMin",		(int)pTableData->byDisassembleNormalMin, "Item Table Index");
			ADD_SUB_ITEM("byDisassembleNormalMax",		(int)pTableData->byDisassembleNormalMax, "Item Table Index");
			ADD_SUB_ITEM("byDisassembleUpperMin",		(int)pTableData->byDisassembleUpperMin,	 "Item Table Index");
			ADD_SUB_ITEM("byDisassembleUpperMax",		(int)pTableData->byDisassembleUpperMax,  "Item Table Index");
			ADD_SUB_ITEM("byDropVisual",				(int)pTableData->byDropVisual,			 "Item Table Index");
			ADD_SUB_ITEM("byUseDisassemble",			(int)pTableData->byUseDisassemble,		 "Item Table Index");
		}
		break;

		case CTableContainer::TABLE_NEWBIE:
		{
			sNEWBIE_TBLDAT*	pTableData = (sNEWBIE_TBLDAT*)pTbldat;
			ADD_SUB_ITEM("tblidx",			pTableData->tblidx,		   "Item Table Index");
			ADD_SUB_ITEM("byRace",			(int)pTableData->byRace,   "Item Table Index");
			ADD_SUB_ITEM("byClass",			(int)pTableData->byClass,  "Item Table Index");
			ADD_SUB_ITEM("world_Id",		pTableData->world_Id,	   "Item Table Index");
			ADD_SUB_ITEM("tutorialWorld",	pTableData->tutorialWorld, "Item Table Index");

			ADD_SUB_ITEM("vSpawn_Loc.x",	pTableData->vSpawn_Loc.x,  "Item Table Index");
			ADD_SUB_ITEM("vSpawn_Loc.y",	pTableData->vSpawn_Loc.y,  "Item Table Index");
			ADD_SUB_ITEM("vSpawn_Loc.z",	pTableData->vSpawn_Loc.z,  "Item Table Index");

			ADD_SUB_ITEM("vSpawn_Dir.x",	pTableData->vSpawn_Dir.x,  "Item Table Index");
			ADD_SUB_ITEM("vSpawn_Dir.y",	pTableData->vSpawn_Dir.y,  "Item Table Index");
			ADD_SUB_ITEM("vSpawn_Dir.z",	pTableData->vSpawn_Dir.z,  "Item Table Index");

			ADD_SUB_ITEM("vBind_Loc.x",		pTableData->vBind_Loc.x,   "Item Table Index");
			ADD_SUB_ITEM("vBind_Loc.y",		pTableData->vBind_Loc.y,   "Item Table Index");
			ADD_SUB_ITEM("vBind_Loc.z",		pTableData->vBind_Loc.z,   "Item Table Index");

			ADD_SUB_ITEM("vBind_Dir.x",		pTableData->vBind_Dir.x,   "Item Table Index");
			ADD_SUB_ITEM("vBind_Dir.y",		pTableData->vBind_Dir.y,   "Item Table Index");
			ADD_SUB_ITEM("vBind_Dir.z",		pTableData->vBind_Dir.z,   "Item Table Index");

			for(int i = 0; i < NTL_MAX_NEWBIE_ITEM; i++)
				ADD_SUB_ITEM("aitem_Tblidx", pTableData->aitem_Tblidx[i], "Item Table Index");

			for (int i = 0; i < NTL_MAX_NEWBIE_ITEM; i++)
				ADD_SUB_ITEM("abyPos", (int)pTableData->abyPos[i], "Item Table Index");

			for (int i = 0; i < NTL_MAX_NEWBIE_ITEM; i++)
				ADD_SUB_ITEM("abyStack_Quantity", (int)pTableData->abyStack_Quantity[i], "Item Table Index");

			ADD_SUB_ITEM("mapNameTblidx",	pTableData->mapNameTblidx, "Item Table Index");

			for (int i = 0; i < NTL_MAX_NEWBIE_SKILL; i++)
				ADD_SUB_ITEM("aSkillTblidx", pTableData->aSkillTblidx[i], "Item Table Index");

			for (int i = 0; i < NTL_MAX_NEWBIE_QUICKSLOT_COUNT; i++)
			{
				ADD_SUB_ITEM("QuickSlot Tblidx", pTableData->asQuickData[i].tbilidx,		  "Item Table Index");
				ADD_SUB_ITEM("QuickSlot Type",	 (int)pTableData->asQuickData[i].byType,	  "Item Table Index");
				ADD_SUB_ITEM("QuickSlot Slot",   (int)pTableData->asQuickData[i].byQuickSlot, "Item Table Index");
			}
			
			for (int i = 0; i < 3; i++)
				ADD_SUB_ITEM("defaultPortalId", (int)pTableData->defaultPortalId[i], "Item Table Index");

			ADD_SUB_ITEM("qItemTblidx1",	  pTableData->qItemTblidx1,			  "Quest Item Table Index");
			ADD_SUB_ITEM("byQPosition1",	  (int)pTableData->byQPosition1,	  "Quest item position");
			ADD_SUB_ITEM("byQStackQuantity1", (int)pTableData->byQStackQuantity1, "Quest item quantity");
			ADD_SUB_ITEM("wMixLevelData",	  pTableData->wMixLevelData,		  "Mix level");
		}
		break;
		
		case CTableContainer::TABLE_SPEECH:
		{
			sNPC_SPEECH_TBLDAT* pTableData = (sNPC_SPEECH_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",			pTableData->tblidx,			   "NPC Speech Table");

			ADD_SUB_ITEM("dwDialogGroup",	pTableData->dwDialogGroup,	   "NPC Speech Table");
			ADD_SUB_ITEM("szDialogType",	pTableData->szDialogType,	   "NPC Speech Table");
			ADD_SUB_ITEM("byRate",			(int)pTableData->byRate,	   "NPC Speech Table");
			ADD_SUB_ITEM("textIndex",		pTableData->textIndex,		   "NPC Speech Table");
			ADD_SUB_ITEM("byBallonType",	(int)pTableData->byBallonType, "NPC Speech Table");
			ADD_SUB_ITEM("dwDisplayTime",	pTableData->dwDisplayTime,	   "NPC Speech Table");
			ADD_SUB_ITEM("szNote",			pTableData->szNote,			   "NPC Speech Table");
			ADD_SUB_ITEM("bySpeechType",	(int)pTableData->bySpeechType, "NPC Speech Table");
		
		}
		break;

		default: break;
	}

	m_wndPropList.AddProperty(pGroup1);

	AdjustLayout();
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create combo:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}

	InitPropList();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* Is locked */);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* Locked */);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void CPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnProperties1()
{
	// TODO: Add your command handler code here
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void CPropertiesWnd::OnProperties2()
{
	// TODO: Add your command handler code here
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void CPropertiesWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	/*CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Appearance"));

	ADD_SUB_ITEM("3D Look", false, "Specifies the window's font will be non-bold and controls will have a 3D border");

	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("Border"), _T("Dialog Frame"), _T("One of: None, Thin, Resizable, or Dialog Frame"));
	pProp->AddOption(_T("None"));
	pProp->AddOption(_T("Thin"));
	pProp->AddOption(_T("Resizable"));
	pProp->AddOption(_T("Dialog Frame"));
	pProp->AllowEdit(FALSE);

	pGroup1->AddSubItem(pProp);
	ADD_SUB_ITEM("Caption",  _T("About", "Specifies the text that will be displayed in the window's title bar"));

	m_wndPropList.AddProperty(pGroup1);

	CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(_T("Window Size"), 0, TRUE);

	pProp = new CMFCPropertyGridProperty(_T("Height"),  250l, _T("Specifies the window's height"));
	pProp->EnableSpinControl(TRUE, 50, 300);
	pSize->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty( _T("Width"),  150l, _T("Specifies the window's width"));
	pProp->EnableSpinControl(TRUE, 50, 200);
	pSize->AddSubItem(pProp);

	m_wndPropList.AddProperty(pSize);

	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("Font"));

	LOGFONT lf;
	CFont* font = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	font->GetLogFont(&lf);

	_tcscpy_s(lf.lfFaceName, _T("Arial"));

	pGroup2->AddSubItem(new CMFCPropertyGridFontProperty(_T("Font"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("Specifies the default font for the window")));
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("Use System Font"),  true, _T("Specifies that the window uses MS Shell Dlg font")));

	m_wndPropList.AddProperty(pGroup2);

	CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("Misc"));
	pProp = new CMFCPropertyGridProperty(_T("(Name)"), _T("Application"));
	pProp->Enable(FALSE);
	pGroup3->AddSubItem(pProp);

	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("Window Color"), RGB(210, 192, 254), nullptr, _T("Specifies the default window color"));
	pColorProp->EnableOtherButton(_T("Other..."));
	pColorProp->EnableAutomaticButton(_T("Default"), ::GetSysColor(COLOR_3DFACE));
	pGroup3->AddSubItem(pColorProp);

	static const TCHAR szFilter[] = _T("Icon Files(*.ico)|*.ico|All Files(*.*)|*.*||");
	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("Icon"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("Specifies the window icon")));

	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("Folder"), _T("c:\\")));

	m_wndPropList.AddProperty(pGroup3);*/
}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
}

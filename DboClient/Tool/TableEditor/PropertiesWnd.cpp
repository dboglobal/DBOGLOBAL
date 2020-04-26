#include "pch.h"
#include "framework.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "TableEditor.h"

#include "Util.h"
#include "ItemTable.h"
#include "NewbieTable.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

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

			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("TBLIDX"), (_variant_t)pTableData->tblidx, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("bValidity_Able"), (_variant_t)pTableData->bValidity_Able, _T("Set item active/inactive")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Name"), (_variant_t)pTableData->Name, _T("Name Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("wszNameText"), (_variant_t)pTableData->wszNameText, _T("Item Name. Max 32 Chars")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("szIcon_Name"), (_variant_t)pTableData->szIcon_Name, _T("Icon Name")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("byModel_Type"), (_variant_t)(int)pTableData->byModel_Type, _T("Model Type")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("szModel"), (_variant_t)pTableData->szModel, _T("Model Name")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("szSub_Weapon_Act_Model"), (_variant_t)pTableData->szSub_Weapon_Act_Model, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("byItem_Type"), (_variant_t)(int)pTableData->byItem_Type, _T("Item Type")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("byEquip_Type"), (_variant_t)(int)pTableData->byEquip_Type, _T("Equipment Type")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("dwEquip_Slot_Type_Bit_Flag"), (_variant_t)pTableData->dwEquip_Slot_Type_Bit_Flag, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("wFunction_Bit_Flag"), (_variant_t)pTableData->wFunction_Bit_Flag, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("byMax_Stack"), (_variant_t)(int)pTableData->byMax_Stack, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("byRank"), (_variant_t)(int)pTableData->byRank, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("dwWeight"), (_variant_t)pTableData->dwWeight, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("dwCost"), (_variant_t)pTableData->dwCost, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("dwSell_Price"), (_variant_t)pTableData->dwSell_Price, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("byDurability"), (_variant_t)(int)pTableData->byDurability, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("byDurability_Count"), (_variant_t)(int)pTableData->byDurability_Count, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("byBattle_Attribute"), (_variant_t)(int)pTableData->byBattle_Attribute, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("wPhysical_Offence"), (_variant_t)pTableData->wPhysical_Offence, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("wEnergy_Offence"), (_variant_t)pTableData->wEnergy_Offence, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("wPhysical_Defence"), (_variant_t)pTableData->wPhysical_Defence, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("wEnergy_Defence"), (_variant_t)pTableData->wEnergy_Defence, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("fAttack_Range_Bonus"), (_variant_t)pTableData->fAttack_Range_Bonus, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("wAttack_Speed_Rate"), (_variant_t)pTableData->wAttack_Speed_Rate, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("byNeed_Min_Level"), (_variant_t)(int)pTableData->byNeed_Min_Level, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("byNeed_Max_Level"), (_variant_t)(int)pTableData->byNeed_Max_Level, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("dwNeed_Class_Bit_Flag"), (_variant_t)pTableData->dwNeed_Class_Bit_Flag, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("dwNeed_Gender_Bit_Flag"), (_variant_t)pTableData->dwNeed_Gender_Bit_Flag, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("byClass_Special"), (_variant_t)(int)pTableData->byClass_Special, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("byRace_Special"), (_variant_t)(int)pTableData->byRace_Special, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("wNeed_Str"), (_variant_t)pTableData->wNeed_Str, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("wNeed_Con"), (_variant_t)pTableData->wNeed_Con, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("wNeed_Foc"), (_variant_t)pTableData->wNeed_Foc, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("wNeed_Dex"), (_variant_t)pTableData->wNeed_Dex, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("wNeed_Sol"), (_variant_t)pTableData->wNeed_Sol, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("wNeed_Eng"), (_variant_t)pTableData->wNeed_Eng, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("set_Item_Tblidx"), (_variant_t)pTableData->set_Item_Tblidx, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Note"), (_variant_t)pTableData->Note, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("byBag_Size"), (_variant_t)(int)pTableData->byBag_Size, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("wScouter_Watt"), (_variant_t)pTableData->wScouter_Watt, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("dwScouter_MaxPower"), (_variant_t)pTableData->dwScouter_MaxPower, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("byScouter_Parts_Type1"), (_variant_t)(int)pTableData->byScouter_Parts_Type1, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("byScouter_Parts_Type2"), (_variant_t)(int)pTableData->byScouter_Parts_Type2, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("byScouter_Parts_Type3"), (_variant_t)(int)pTableData->byScouter_Parts_Type3, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("byScouter_Parts_Type4"), (_variant_t)(int)pTableData->byScouter_Parts_Type4, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Use_Item_Tblidx"), (_variant_t)pTableData->Use_Item_Tblidx, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("bIsCanHaveOption"), (_variant_t)pTableData->bIsCanHaveOption, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Item_Option_Tblidx"), (_variant_t)pTableData->Item_Option_Tblidx, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("byItemGroup"), (_variant_t)(int)pTableData->byItemGroup, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Charm_Tblidx"), (_variant_t)pTableData->Charm_Tblidx, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("wCostumeHideBitFlag"), (_variant_t)pTableData->wCostumeHideBitFlag, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("NeedItemTblidx"), (_variant_t)pTableData->NeedItemTblidx, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("CommonPoint"), (_variant_t)pTableData->CommonPoint, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("byCommonPointType"), (_variant_t)(int)pTableData->byCommonPointType, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("byNeedFunction"), (_variant_t)(int)pTableData->byNeedFunction, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("dwUseDurationMax"), (_variant_t)pTableData->dwUseDurationMax, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("byDurationType"), (_variant_t)(int)pTableData->byDurationType, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("contentsTblidx"), (_variant_t)pTableData->contentsTblidx, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("dwDurationGroup"), (_variant_t)pTableData->dwDurationGroup, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("byDropLevel"), (_variant_t)(int)pTableData->byDropLevel, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("enchantRateTblidx"), (_variant_t)pTableData->enchantRateTblidx, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("excellentTblidx"), (_variant_t)pTableData->excellentTblidx, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("rareTblidx"), (_variant_t)pTableData->rareTblidx, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("legendaryTblidx"), (_variant_t)pTableData->legendaryTblidx, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("bCreateSuperiorAble"), (_variant_t)pTableData->bCreateSuperiorAble, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("bCreateExcellentAble"), (_variant_t)pTableData->bCreateExcellentAble, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("bCreateRareAble"), (_variant_t)pTableData->bCreateRareAble, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("bCreateLegendaryAble"), (_variant_t)pTableData->bCreateLegendaryAble, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("byRestrictType"), (_variant_t)(int)pTableData->byRestrictType, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("fAttack_Physical_Revision"), (_variant_t)pTableData->fAttack_Physical_Revision, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("fAttack_Energy_Revision"), (_variant_t)pTableData->fAttack_Energy_Revision, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("fDefence_Physical_Revision"), (_variant_t)pTableData->fDefence_Physical_Revision, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("fDefence_Energy_Revision"), (_variant_t)pTableData->fDefence_Energy_Revision, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("byTmpTabType"), (_variant_t)(int)pTableData->byTmpTabType, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("bIsCanRenewal"), (_variant_t)pTableData->bIsCanRenewal, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("wDisassemble_Bit_Flag"), (_variant_t)pTableData->wDisassemble_Bit_Flag, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("byDisassembleNormalMin"), (_variant_t)(int)pTableData->byDisassembleNormalMin, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("byDisassembleNormalMax"), (_variant_t)(int)pTableData->byDisassembleNormalMax, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("byDisassembleUpperMin"), (_variant_t)(int)pTableData->byDisassembleUpperMin, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("byDisassembleUpperMax"), (_variant_t)(int)pTableData->byDisassembleUpperMax, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("byDropVisual"), (_variant_t)(int)pTableData->byDropVisual, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("byUseDisassemble"), (_variant_t)(int)pTableData->byUseDisassemble, _T("Item Table Index")));
		}
		break;

		case CTableContainer::TABLE_NEWBIE:
		{
			sNEWBIE_TBLDAT* pTableData = (sNEWBIE_TBLDAT*)pTbldat;
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("tblidx"), (_variant_t)pTableData->tblidx, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("byRace"), (_variant_t)(int)pTableData->byRace, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("byClass"), (_variant_t)(int)pTableData->byClass, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("world_Id"), (_variant_t)pTableData->world_Id, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("tutorialWorld"), (_variant_t)pTableData->tutorialWorld, _T("Item Table Index")));

			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("vSpawn_Loc.x"), (_variant_t)pTableData->vSpawn_Loc.x, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("vSpawn_Loc.y"), (_variant_t)pTableData->vSpawn_Loc.y, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("vSpawn_Loc.z"), (_variant_t)pTableData->vSpawn_Loc.z, _T("Item Table Index")));

			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("vSpawn_Dir.x"), (_variant_t)pTableData->vSpawn_Dir.x, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("vSpawn_Dir.y"), (_variant_t)pTableData->vSpawn_Dir.y, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("vSpawn_Dir.z"), (_variant_t)pTableData->vSpawn_Dir.z, _T("Item Table Index")));

			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("vBind_Loc.x"), (_variant_t)pTableData->vBind_Loc.x, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("vBind_Loc.y"), (_variant_t)pTableData->vBind_Loc.y, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("vBind_Loc.z"), (_variant_t)pTableData->vBind_Loc.z, _T("Item Table Index")));

			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("vBind_Dir.x"), (_variant_t)pTableData->vBind_Dir.x, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("vBind_Dir.y"), (_variant_t)pTableData->vBind_Dir.y, _T("Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("vBind_Dir.z"), (_variant_t)pTableData->vBind_Dir.z, _T("Item Table Index")));

			for(int i = 0; i < NTL_MAX_NEWBIE_ITEM; i++)
				pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("aitem_Tblidx"), (_variant_t)pTableData->aitem_Tblidx[i], _T("Item Table Index")));

			for (int i = 0; i < NTL_MAX_NEWBIE_ITEM; i++)
				pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("abyPos"), (_variant_t)(int)pTableData->abyPos[i], _T("Item Table Index")));

			for (int i = 0; i < NTL_MAX_NEWBIE_ITEM; i++)
				pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("abyStack_Quantity"), (_variant_t)(int)pTableData->abyStack_Quantity[i], _T("Item Table Index")));

			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("mapNameTblidx"), (_variant_t)pTableData->mapNameTblidx, _T("Item Table Index")));

			for (int i = 0; i < NTL_MAX_NEWBIE_SKILL; i++)
				pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("aSkillTblidx"), (_variant_t)pTableData->aSkillTblidx[i], _T("Item Table Index")));

			for (int i = 0; i < NTL_MAX_NEWBIE_QUICKSLOT_COUNT; i++)
			{
				pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("QuickSlot Tblidx"), (_variant_t)pTableData->asQuickData[i].tbilidx, _T("Item Table Index")));
				pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("QuickSlot Type"), (_variant_t)(int)pTableData->asQuickData[i].byType, _T("Item Table Index")));
				pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("QuickSlot Slot"), (_variant_t)(int)pTableData->asQuickData[i].byQuickSlot, _T("Item Table Index")));
			}
			
			for (int i = 0; i < 3; i++)
				pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("defaultPortalId"), (_variant_t)(int)pTableData->defaultPortalId[i], _T("Item Table Index")));

			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("qItemTblidx1"), (_variant_t)pTableData->qItemTblidx1, _T("Quest Item Table Index")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("byQPosition1"), (_variant_t)(int)pTableData->byQPosition1, _T("Quest item position")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("byQStackQuantity1"), (_variant_t)(int)pTableData->byQStackQuantity1, _T("Quest item quantity")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("wMixLevelData"), (_variant_t)pTableData->wMixLevelData, _T("Mix level")));

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

	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("3D Look"), (_variant_t) false, _T("Specifies the window's font will be non-bold and controls will have a 3D border")));

	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("Border"), _T("Dialog Frame"), _T("One of: None, Thin, Resizable, or Dialog Frame"));
	pProp->AddOption(_T("None"));
	pProp->AddOption(_T("Thin"));
	pProp->AddOption(_T("Resizable"));
	pProp->AddOption(_T("Dialog Frame"));
	pProp->AllowEdit(FALSE);

	pGroup1->AddSubItem(pProp);
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Caption"), (_variant_t) _T("About"), _T("Specifies the text that will be displayed in the window's title bar")));

	m_wndPropList.AddProperty(pGroup1);

	CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(_T("Window Size"), 0, TRUE);

	pProp = new CMFCPropertyGridProperty(_T("Height"), (_variant_t) 250l, _T("Specifies the window's height"));
	pProp->EnableSpinControl(TRUE, 50, 300);
	pSize->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty( _T("Width"), (_variant_t) 150l, _T("Specifies the window's width"));
	pProp->EnableSpinControl(TRUE, 50, 200);
	pSize->AddSubItem(pProp);

	m_wndPropList.AddProperty(pSize);

	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("Font"));

	LOGFONT lf;
	CFont* font = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	font->GetLogFont(&lf);

	_tcscpy_s(lf.lfFaceName, _T("Arial"));

	pGroup2->AddSubItem(new CMFCPropertyGridFontProperty(_T("Font"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("Specifies the default font for the window")));
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("Use System Font"), (_variant_t) true, _T("Specifies that the window uses MS Shell Dlg font")));

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

#include "precomp_dboclient.h"
#include "HoiPoiMixRecipeGui.h"

// Debug
#include "NtlDebug.h"

// Shared
#include "TableContainer.h"
#include "NtlItem.h"
#include "ItemMixMachineTable.h"
#include "DynamicObjectTable.h"

// Simulation
#include "NTLSLApi.h"
#include "NtlSLGlobal.h"
#include "NtlSLHoipoiMix.h"
#include "NtlSobAvatar.h"
#include "NtlSobManager.h"
#include "ntlSobTriggerObjectAttr.h"
#include "NtlSLEventFunc.h"
#include "NtlCameraController.h"
#include "NtlSLLogic.h"

// Presentation
#include "ItemRecipeTable.h"
#include "TextAllTable.h"
#include "NtlPLGuiManager.h"

// Client
#include "DialogManager.h"
#include "DboEventGenerator.h"
#include "DisplayStringManager.h"
#include "InfoWndManager.h"
#include "DialogPriority.h"

#include "HoiPoiMixCraftGui.h"


#define dRECIPE_DLG_ITEM_OFFSET_TOP		10		// 레시피 목록의 가장 윗쪽 오프셋
#define dRECIPE_DLG_ITEM_HEIGHT			72		// 레시피 목록의 높이
#define dRECIPE_DLG_ITEM_HEIGHT_MARGIN	10		// 레시피 목록의 마진
#define dRECIPE_DLG_ITEM_VIEW_NUM		5		// 한 화면에 보이는 레시피 갯수

//////////////////////////////////////////////////////////////////////////
// CRecipeDlgItem
//////////////////////////////////////////////////////////////////////////

CRecipeDlgItem::CRecipeDlgItem()
{
	m_pThis = NULL;

	m_nIndex = -1;
	m_idxTbl = INVALID_TBLIDX;

	m_pStbRecipeName = NULL;
	m_pStbRequiredLevel = NULL;
	m_pStbZenny = NULL;
	m_pHoipoiMix = NULL;
	m_bSelect = FALSE;
}

CRecipeDlgItem::~CRecipeDlgItem()
{

}

RwBool CRecipeDlgItem::Create(RwInt32 nIndex, CHoiPoiMixRecipeGui* pParentGui, TBLIDX idxRecipeTbl, RwUInt8 byDiscountRate)
{
	m_pHoipoiMix = pParentGui;
	m_nIndex = nIndex;
	m_idxTbl = idxRecipeTbl;

	CRectangle rect;
	rect.SetRectWH(16, (nIndex * (dRECIPE_DLG_ITEM_HEIGHT + dRECIPE_DLG_ITEM_HEIGHT_MARGIN)) + dRECIPE_DLG_ITEM_OFFSET_TOP, 278, 67);
	m_pThis = NTL_NEW gui::CDialog(&rect, pParentGui->GetComponent("dlgRecipeList"), GetNtlGuiManager()->GetSurfaceManager());
	m_pThis->AddSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HoipoiMix.srf", "srfRecipeSlot"));
	m_slotPaint = m_pThis->SigPaint().Connect(this, &CRecipeDlgItem::OnPaint);
	m_slotMove = m_pThis->SigMove().Connect(this, &CRecipeDlgItem::OnMove);
	m_slotEnterItem = m_pThis->SigMouseEnter().Connect(this, &CRecipeDlgItem::OnEnterItem);
	m_slotLeaveItem = m_pThis->SigMouseLeave().Connect(this, &CRecipeDlgItem::OnLeaveItem);
	m_slotMouseDown = m_pThis->SigMouseDown().Connect(this, &CRecipeDlgItem::OnMouseDown);
	m_slotMouseUp = m_pThis->SigMouseUp().Connect(this, &CRecipeDlgItem::OnMouseUp);
	m_slotMouseMove = m_pThis->SigMouseMove().Connect(this, &CRecipeDlgItem::OnMouseMove);

	m_pThis->SetClippingMode(true);

	m_pItemRecipeTblDat = (sITEM_RECIPE_TBLDAT*)API_GetTableContainer()->GetItemRecipeTable()->FindData(idxRecipeTbl);
	if (m_pItemRecipeTblDat == NULL)
	{
		DBO_FAIL("CRecipeDlgItem::Create - Out of item recipe table index");
		return FALSE;
	}

	m_Slot.Create(m_pThis, DIALOG_HOIPOIMIX_RECIPE, REGULAR_SLOT_ITEM_TABLE, SDS_NONE);
	m_Slot.SetPosition_fromParent(9, 17);
	m_Slot.SetIcon(m_pItemRecipeTblDat->asCreateItemTblidx[0].itemTblidx);
	m_Slot.SetClippingMode(TRUE);

	// Recipe Name
	rect.SetRectWH(57, 4, 250, 20);
	m_pStbRecipeName = NTL_NEW gui::CStaticBox(&rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager());
	m_pStbRecipeName->CreateFontStd(DEFAULT_FONT, 95, DEFAULT_FONT_ATTR);

	sTEXT_TBLDAT* pTextTblDat = (sTEXT_TBLDAT*)API_GetTableContainer()->GetTextAllTable()->GetItemTbl()->FindData(m_pItemRecipeTblDat->dwName);
	if (pTextTblDat)
	{
		m_pStbRecipeName->SetText(pTextTblDat->wstrText.c_str());
	}
	else
	{
		WCHAR awcErrBuffer[32];
		swprintf_s(awcErrBuffer, 32, L"(ITEM_T_TBL)%d", m_pItemRecipeTblDat->dwName);
		m_pStbRecipeName->SetText(awcErrBuffer);
	}
	m_pStbRecipeName->SetClippingMode(true);
	m_pStbRecipeName->Enable(false);

	// Required Level
	rect.SetRectWH(58, 25, 200, 20);
	m_pStbRequiredLevel = NTL_NEW gui::CStaticBox(&rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager());
	m_pStbRequiredLevel->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pStbRequiredLevel->SetTextColor(RGB(148, 221, 255));
	m_pStbRequiredLevel->Enable(false);

	WCHAR awcBuffer[256];
	swprintf_s(awcBuffer, 256, GetDisplayStringManager()->GetString("DST_HOIPOIMIX_REQUIRED_LEVEL"), m_pItemRecipeTblDat->byNeedMixLevel);
	m_pStbRequiredLevel->SetText(awcBuffer);
	m_pStbRequiredLevel->SetClippingMode(true);
	m_pStbRequiredLevel->Enable(false);

	// Zenny Price
	rect.SetRectWH(58, 48, 200, 12);
	m_pStbZenny = NTL_NEW gui::CStaticBox(&rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager());
	m_pStbZenny->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pStbZenny->SetTextColor(INFOCOLOR_7);
	m_pStbZenny->SetClippingMode(true);
	swprintf_s(awcBuffer, 256, GetDisplayStringManager()->GetString("DST_HOIPOIMIX_REQUIRED_ZENNY"), m_pItemRecipeTblDat->dwNeedMixZenny);
	m_pStbZenny->SetText(awcBuffer);
	m_pStbZenny->Enable(false);

	OnMove(0, 0);

	return TRUE;
}

VOID CRecipeDlgItem::Destroy()
{
	NTL_DELETE(m_pStbZenny);
	NTL_DELETE(m_pStbRequiredLevel);
	NTL_DELETE(m_pStbRecipeName);

	m_Slot.Destroy();

	NTL_DELETE(m_pThis);

}

RwUInt8 CRecipeDlgItem::GetRecipeType()
{
	return m_pItemRecipeTblDat->byRecipeType;
}

TBLIDX CRecipeDlgItem::GetRecipeTableIndex()
{
	return m_idxTbl;
}

VOID CRecipeDlgItem::OnPaint()
{
	if (m_pThis->IsVisible())
		m_Slot.Paint();
}

VOID CRecipeDlgItem::OnMove(int nOldX, int nOldY)
{
	CRectangle rect = m_pThis->GetScreenRect();
	m_Slot.SetParentPosition(rect.left, rect.top);

	CRectangle rtClipping = m_pThis->GetClippingRect();
	m_Slot.SetClippingRect(rtClipping);
}

VOID CRecipeDlgItem::RecipeItemDisposition(RwUInt32 uiIndex)
{
	m_nIndex = uiIndex;

	CRectangle rect;
	gui::CSurface sur = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HoipoiMix.srf", "srfRecipeSlot");
	rect.SetRectWH(16,
		(uiIndex * (sur.m_Original.rtRect.GetHeight() + 5 + dRECIPE_DLG_ITEM_HEIGHT_MARGIN)) + dRECIPE_DLG_ITEM_OFFSET_TOP,
		sur.m_Original.rtRect.GetWidth(),
		sur.m_Original.rtRect.GetHeight());

	m_pThis->SetPosition(rect);
	m_pThis->SetClippingMode(TRUE);

	rect = m_pThis->GetScreenRect();
	m_Slot.SetParentPosition(rect.left, rect.top);

	CRectangle rtClipping = m_pThis->GetClippingRect();
	m_Slot.SetClippingMode(TRUE);
	m_Slot.SetClippingRect(rtClipping);


	m_surIcon.SetClippingMode(TRUE);
	m_pStbRecipeName->SetClippingMode(TRUE);
	m_pStbRequiredLevel->SetClippingMode(TRUE);
	m_pStbZenny->SetClippingMode(TRUE);
}

VOID CRecipeDlgItem::Show(RwBool bShow)
{
	m_pThis->Show(B2b(bShow));
}

VOID CRecipeDlgItem::SelectItem(RwBool bSelect)
{
	m_bSelect = bSelect;

	if (m_bSelect)
	{
		m_pThis->GetSurface()->clear();
		m_pThis->AddSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HoipoiMix.srf", "srfRecipeSlotFocus"));
	}
	else
	{
		m_pThis->GetSurface()->clear();
		m_pThis->AddSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HoipoiMix.srf", "srfRecipeSlot"));
	}
}

VOID CRecipeDlgItem::OnEnterItem(gui::CComponent* pComponent)
{
	if (!m_bSelect)
	{
		m_pThis->GetSurface()->clear();
		m_pThis->AddSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HoipoiMix.srf", "srfRecipeSlotFocus"));
	}
}

VOID CRecipeDlgItem::OnLeaveItem(gui::CComponent* pComponent)
{
	if (!m_bSelect)
	{
		m_pThis->GetSurface()->clear();
		m_pThis->AddSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HoipoiMix.srf", "srfRecipeSlot"));
	}

	m_bMouseDown = FALSE;

	if (GetInfoWndManager()->GetRequestGui() == DIALOG_HOIPOIMIX_RECIPE)
		GetInfoWndManager()->ShowInfoWindow(FALSE);
}

VOID CRecipeDlgItem::OnMouseDown(const CKey& key)
{
	if (key.m_nID != UD_LEFT_BUTTON)
		return;

	m_bMouseDown = TRUE;
}

VOID CRecipeDlgItem::OnMouseUp(const CKey& key)
{
	if (key.m_nID != UD_LEFT_BUTTON)
		return;

	if (m_bMouseDown)
	{
		m_bMouseDown = FALSE;

		// Select
		m_pHoipoiMix->SelectRecipe(this);
	}
}

VOID CRecipeDlgItem::OnMouseMove(RwInt32 nKeys, RwInt32 nX, RwInt32 nY)
{
	CRectangle rect = m_pThis->GetScreenRect();
	if (m_Slot.PtInRect(nX, nY))
	{
		sITEM_RECIPE_TBLDAT* pTblDat = (sITEM_RECIPE_TBLDAT*)API_GetTableContainer()->GetItemRecipeTable()->FindData(GetRecipeTableIndex());
		if (pTblDat)
		{
			GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_RECIPE,
				rect.left + m_Slot.GetX_fromParent(), rect.top + m_Slot.GetY_fromParent(),
				(VOID*)pTblDat,
				DIALOG_HOIPOIMIX_RECIPE);
		}
		else
		{
			WCHAR awcBuffer[128];
			swprintf_s(awcBuffer, 128, L"Invalid recipe table idx %d", GetRecipeTableIndex());
			GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_JUST_WTEXT,
				rect.left + m_Slot.GetX_fromParent(), rect.top + m_Slot.GetY_fromParent(),
				(VOID*)awcBuffer,
				DIALOG_HOIPOIMIX_RECIPE);
		}
	}
	else
	{
		if (GetInfoWndManager()->GetRequestGui() == DIALOG_HOIPOIMIX_RECIPE)
			GetInfoWndManager()->ShowInfoWindow(FALSE);
	}
}



//////////////////////////////////////////////////////////////////////////
// CHoiPoiMixRecipeGui
//////////////////////////////////////////////////////////////////////////

CHoiPoiMixRecipeGui::CHoiPoiMixRecipeGui(const RwChar* pName)
	:CNtlPLGui(pName)
{
	m_pThis = NULL;
	m_pScbBar = NULL;
	m_byDiscountRate = 0;
	m_pSelectDlgItem = NULL;
	m_vecAvatarRecipe.clear();

	m_hObject = INVALID_HOBJECT;
}

CHoiPoiMixRecipeGui::~CHoiPoiMixRecipeGui()
{
}

RwBool CHoiPoiMixRecipeGui::Create()
{
	NTL_FUNCTION("CHoiPoiMixRecipeGui::Create");

	if (!CNtlPLGui::Create("", "gui\\HoipoiMix.srf", "gui\\HoipoiMixRecipe.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_DEFAULT);

	m_pstbTitle = (gui::CStaticBox*)GetComponent("stbTitle");
	m_pstbTitle->SetText(GetDisplayStringManager()->GetString("DST_HOIPOIMIX_GUI_TITLE"));

	m_pbtnClose = (gui::CButton*)GetComponent("btnClose");
	m_slotClickedClose = m_pbtnClose->SigClicked().Connect(this, &CHoiPoiMixRecipeGui::OnClickedClose);

	m_ptabHoipoiMix = (gui::CTabButton*)GetComponent("tabHoipoiMix");
	m_slotSelectTab = m_ptabHoipoiMix->SigSelectChanged().Connect(this, &CHoiPoiMixRecipeGui::OnSelectTab);

	m_ptabHoipoiMix->AddTab(std::wstring(GetDisplayStringManager()->GetString("DST_HOIPOIMIX_RECIPE_TAB")), FALSE);
	m_ptabHoipoiMix->SelectTab(RECIPE_TAB);

	m_pdlgRecipe = (gui::CDialog*)GetComponent("dlgRecipe");
	m_pdlgRecipeClip = (gui::CDialog*)GetComponent("dlgRecipeClip");
	m_pdlgRecipeList = (gui::CDialog*)GetComponent("dlgRecipeList");
	m_pScbBar = (gui::CScrollBar*)GetComponent("scbScroll");

	m_pdlgEmptyRecipe = (gui::CDialog*)GetComponent("dlgEmptyRecipe");
	m_pstbEmptyRecipe = (gui::CStaticBox*)GetComponent("stbEmptyRecipe");
	m_pstbEmptyRecipe->SetText(GetDisplayStringManager()->GetString("DST_HOIPOIMIX_EMPTY_BY_FILTER"));

	m_pBtnFilter[FILTER_NORMAL] = (gui::CButton*)GetComponent("btnFilter1");
	m_pBtnFilter[FILTER_NORMAL]->SetText(GetDisplayStringManager()->GetString("DST_HOIPOIMIX_RECIPE_FILTER_NORMAL"));
	m_pBtnFilter[FILTER_WEAPON] = (gui::CButton*)GetComponent("btnFilter2");
	m_pBtnFilter[FILTER_WEAPON]->SetText(GetDisplayStringManager()->GetString("DST_HOIPOIMIX_RECIPE_FILTER__EVENT"));
	m_pBtnFilter[FILTER_ARMOR] = (gui::CButton*)GetComponent("btnFilter3");
	m_pBtnFilter[FILTER_ARMOR]->SetText(GetDisplayStringManager()->GetString("DST_HOIPOIMIX_RECIPE_FILTER_NORMAL"));
	m_pBtnFilter[FILTER_ACCESSORY] = (gui::CButton*)GetComponent("btnFilter4");
	m_pBtnFilter[FILTER_ACCESSORY]->SetText(GetDisplayStringManager()->GetString("DST_HOIPOIMIX_RECIPE_FILTER_SPECIAL"));


	m_pBtnFilter[FILTER_NORMAL]->SetDown();
	m_pBtnFilter[FILTER_NORMAL]->ClickEnable(FALSE);

	m_slotMove = m_pThis->SigMove().Connect(this, &CHoiPoiMixRecipeGui::OnMove);

	m_slotScrollChanged = m_pScbBar->SigValueChanged().Connect(this, &CHoiPoiMixRecipeGui::OnScrollChanged);
	m_slotScrollSliderMoved = m_pScbBar->SigSliderMoved().Connect(this, &CHoiPoiMixRecipeGui::OnScrollChanged);
	m_slotToggledFilter1 = m_pBtnFilter[FILTER_NORMAL]->SigToggled().Connect(this, &CHoiPoiMixRecipeGui::OnToggledFilter);
	m_slotToggledFilter2 = m_pBtnFilter[FILTER_WEAPON]->SigToggled().Connect(this, &CHoiPoiMixRecipeGui::OnToggledFilter);
	m_slotToggledFilter3 = m_pBtnFilter[FILTER_ARMOR]->SigToggled().Connect(this, &CHoiPoiMixRecipeGui::OnToggledFilter);
	m_slotToggledFilter4 = m_pBtnFilter[FILTER_ACCESSORY]->SigToggled().Connect(this, &CHoiPoiMixRecipeGui::OnToggledFilter);

	m_slotAbsoluteMouseWheel = GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect(this, &CHoiPoiMixRecipeGui::OnAbsoluteMouseWheel);
	
	// create recipe list
	CreateAvatarRecipeList();

	LinkMsg(g_EventNPCDialogOpen);
	LinkMsg(g_EventHoipoiMixRecipeRegNfy);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CHoiPoiMixRecipeGui::Destroy()
{
	NTL_FUNCTION("CHoiPoiMixRecipeGui::Destroy");

	UnLinkMsg(g_EventNPCDialogOpen);
	UnLinkMsg(g_EventHoipoiMixRecipeRegNfy);

	DestroyAvatarRecipeList();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

VOID CHoiPoiMixRecipeGui::HandleEvents(RWS::CMsg& msg)
{
	if (msg.Id == g_EventNPCDialogOpen)
	{
		SDboNpcDialogOpen* pData = reinterpret_cast<SDboNpcDialogOpen*>(msg.pData);

		if (pData->eDialog != DIALOG_HOIPOIMIX_RECIPE)
			return;

		if (pData->hSerialId == INVALID_SERIAL_ID)
		{
			DBO_FAIL("invalid serial id");
			return;
		}

		m_hObject = pData->hSerialId;

		CNtlSob* pSob = GetNtlSobManager()->GetSobObject(pData->hSerialId);
		if (pSob->GetClassID() == SLCLASS_TRIGGER_OBJECT)
		{
			CNtlSobTriggerObjectAttr* pSobAttr = (CNtlSobTriggerObjectAttr*)pSob->GetSobAttr();
			sOBJECT_TBLDAT* pObjTableData = (sOBJECT_TBLDAT*)pSobAttr->GetTriggerObjectTbl();
			if (pObjTableData->dwFunction & eDBO_TRIGGER_OBJECT_FUNC_HOIPOIMIX)
			{
				// set crafting position
				CRectangle rect = GetPosition();
				CNtlPLGui* pPLGui = GetDialogManager()->GetDialog(DIALOG_HOIPOIMIX_CRAFT);
				pPLGui->SetPosition(rect.right + NTL_LINKED_DIALOG_GAP, rect.top);

				GetDialogManager()->OpenDialog(DIALOG_HOIPOIMIX_RECIPE, GetNtlSLGlobal()->GetSobAvatar()->GetSerialID());

				// select first receipe
				CRecipeDlgItem* pRecipe = (m_vecAvatarRecipe.size() == 0) ? NULL : m_vecAvatarRecipe.front();
				SelectRecipe(pRecipe);
			}
		}
	}
	else if (msg.Id == g_EventHoipoiMixRecipeRegNfy)
	{
		SNtlEventHoipoiMixRecipeRegNfy* pNotify = reinterpret_cast<SNtlEventHoipoiMixRecipeRegNfy*>(msg.pData);

		CRecipeDlgItem* pItem = NTL_NEW CRecipeDlgItem();
		pItem->Create(0, this, pNotify->idxRecipeTbl, m_byDiscountRate);
		m_vecAvatarRecipe.push_back(pItem);

		if (m_pThis->IsVisible())
		{
			CreateRecipeFilterList();
			RecipeDispoisition();
		}
	}
}

int CHoiPoiMixRecipeGui::SwitchDialog(bool bOpen)
{
	Show(bOpen);

	if (bOpen)
	{
		CreateRecipeFilterList();
		RecipeDispoisition();

		GetDialogManager()->OpenDialog(DIALOG_HOIPOIMIX_CRAFT);
	}
	else
	{
		m_hObject = INVALID_HOBJECT;

		GetDialogManager()->CloseDialog(DIALOG_HOIPOIMIX_CRAFT);

		Logic_CancelNpcFacing();

		m_hObject = INVALID_SERIAL_ID;


		if (GetInfoWndManager()->GetRequestGui() == DIALOG_HOIPOIMIX_RECIPE)
			GetInfoWndManager()->ShowInfoWindow(FALSE);
	}

	NTL_RETURN(TRUE);
}

void CHoiPoiMixRecipeGui::SelectRecipe(CRecipeDlgItem * pSelectItem)
{
	SelectRecipeItem(pSelectItem);

	if (!pSelectItem)
		return;

	CHoiPoiMixCraftGui* pCraftingGui = (CHoiPoiMixCraftGui*)GetDialogManager()->GetDialog(DIALOG_HOIPOIMIX_CRAFT);
	if(pCraftingGui)
		pCraftingGui->SetRecipe(pSelectItem->GetRecipeTableIndex());
}

void CHoiPoiMixRecipeGui::SelectTab(eTAB eTabIndex)
{
	m_eCurTab = eTabIndex;
}

void CHoiPoiMixRecipeGui::OnMove(int iOldX, int iOldY)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	if (iOldX != 0 && iOldY != 0)
		MoveLinkedPLGui(rtScreen.left - iOldX, rtScreen.top - iOldY);
}

void CHoiPoiMixRecipeGui::OnClickedClose(gui::CComponent * pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_HOIPOIMIX_RECIPE);
}

void CHoiPoiMixRecipeGui::OnSelectTab(int nSelectIdx, int nPreviousIndex)
{
	SelectTab((eTAB)nSelectIdx);
}

VOID CHoiPoiMixRecipeGui::OnAbsoluteMouseWheel(RwInt32 nFlag, RwInt16 sDelta, CPos& pos)
{
	RwInt32 nOffset = (RwInt32)((RwReal)m_pScbBar->GetMaxValue() * 0.1f) * sDelta;
	CAPTURE_MOUSEWHEELMOVE_SCROLLBAR(IsShow(), m_pThis, m_pThis, m_pScbBar, nOffset, pos);
}

VOID CHoiPoiMixRecipeGui::OnScrollChanged(RwInt32 nNewOffset)
{
	CRectangle rtPos = m_pdlgRecipeList->GetPosition();
	m_pdlgRecipeList->SetPosition(rtPos.left, -nNewOffset);
}

RwBool CHoiPoiMixRecipeGui::CreateAvatarRecipeList()
{
	DestroyAvatarRecipeList();

	// If you have an Avatar, make a list in Avatar, and if an Avatar is not created before, get information from NtlSLGlobal and make a list.
	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();

	WORD wCount = 0;
	if (pSobAvatar)
	{
		CNtlSLHoipoiMix* pHoipoiMix = pSobAvatar->GetHoipoiMix();
		wCount = (WORD)pHoipoiMix->GetRecipeCount();

		CNtlSLHoipoiMix::MAPDEF_RECIPEDATA* pMapRecipeData = pHoipoiMix->GetRecipeData();

		int nIndex = 0;
		for each(std::pair< RwUInt32, sRECIPE_DATA* > pairData in (*pMapRecipeData))
		{
			CRecipeDlgItem* pRecipeItem = NTL_NEW CRecipeDlgItem();
			pRecipeItem->Create(nIndex, this, pairData.second->recipeTblidx, m_byDiscountRate);

			m_vecAvatarRecipe.push_back(pRecipeItem);
		}
	}
	else
	{
		SAvatarRecipeInfo* pRecipeInfo = GetNtlSLGlobal()->GetAvatarRecipeInfo();

		wCount = pRecipeInfo->wCount;

		for (int i = 0; i < pRecipeInfo->wCount; ++i)
		{
			CRecipeDlgItem* pRecipeItem = NTL_NEW CRecipeDlgItem();
			pRecipeItem->Create(i, this, pRecipeInfo->asRecipeData[i].recipeTblidx, m_byDiscountRate);

			m_vecAvatarRecipe.push_back(pRecipeItem);
		}
	}

	return TRUE;
}

VOID CHoiPoiMixRecipeGui::DestroyAvatarRecipeList()
{
	for each(CRecipeDlgItem* pItem in m_vecAvatarRecipe)
	{
		if (m_pSelectDlgItem == pItem)
			m_pSelectDlgItem = NULL;

		pItem->Destroy();
		NTL_DELETE(pItem);
	}

	m_vecAvatarRecipe.clear();
}

VOID CHoiPoiMixRecipeGui::OnToggledFilter(gui::CComponent* pComponent, bool bToggled)
{
	if (bToggled)
	{
		for (int i = 0; i < NUM_FILTER; ++i)
		{
			if (m_pBtnFilter[i] == pComponent)
			{
				m_pBtnFilter[i]->ClickEnable(FALSE);
			}
			else
			{
				m_pBtnFilter[i]->ClickEnable(TRUE);
				m_pBtnFilter[i]->SetDown(false);
			}
		}
	}

	CreateRecipeFilterList();
	RecipeDispoisition();
}

VOID CHoiPoiMixRecipeGui::CreateRecipeFilterList()
{
	m_vecShowListRecipe.clear();

	bool bSelectNormal = m_pBtnFilter[FILTER_NORMAL]->IsDown();
	bool bSelectWeapon = m_pBtnFilter[FILTER_WEAPON]->IsDown();
	bool bSelectArmor = m_pBtnFilter[FILTER_ARMOR]->IsDown();
	bool bSelectAccessory = m_pBtnFilter[FILTER_ACCESSORY]->IsDown();


	for each(CRecipeDlgItem* pItem in m_vecAvatarRecipe)
	{
		// normal
		if (bSelectNormal)
		{
			if (pItem->GetRecipeType() == eRECIPE_TYPE_NORMAL)
			{
				m_vecShowListRecipe.push_back(pItem);
			}
		}
		else if (bSelectWeapon) // weapon
		{
			if (pItem->GetRecipeType() == eRECIPE_TYPE_MAINWEAPON || pItem->GetRecipeType() == eRECIPE_TYPE_SUBWEAPON)
			{
				m_vecShowListRecipe.push_back(pItem);
			}
		}
		else if (bSelectArmor) // armor
		{
			if (pItem->GetRecipeType() == eRECIPE_TYPE_CLOTH_ARMOR || pItem->GetRecipeType() == eRECIPE_TYPE_TECH_ARMOR)
			{
				m_vecShowListRecipe.push_back(pItem);
			}
		}
		else if(bSelectAccessory)
		{
			if (pItem->GetRecipeType() == eRECIPE_TYPE_ACCESSORY)
			{
				m_vecShowListRecipe.push_back(pItem);
			}
		}
	}
}

VOID CHoiPoiMixRecipeGui::RecipeDispoisition()
{
	if (!m_pThis->IsVisible())
		return;

	for each(CRecipeDlgItem* pItem in m_vecAvatarRecipe)
	{
		pItem->Show(FALSE);
	}

	RwInt32 nDlgHeight = ((RwInt32)m_vecShowListRecipe.size() * (dRECIPE_DLG_ITEM_HEIGHT + dRECIPE_DLG_ITEM_HEIGHT_MARGIN)) + dRECIPE_DLG_ITEM_OFFSET_TOP;
	m_pdlgRecipeList->SetHeight(nDlgHeight);

	m_pScbBar->SetRange(0, nDlgHeight - m_pdlgRecipeClip->GetHeight());
	m_pScbBar->SetValue(0);

	m_pdlgRecipeList->SetClippingMode(true);

	// ItemDisposition
	int nIndex = 0;
	for each(CRecipeDlgItem* pItem in m_vecShowListRecipe)
	{
		pItem->RecipeItemDisposition(nIndex++);
		pItem->Show(TRUE);
	}

	if (nIndex > 0)
	{
		m_pdlgEmptyRecipe->Show(false);
	}
	else
	{
		m_pdlgEmptyRecipe->Show(true);
	}
}

VOID CHoiPoiMixRecipeGui::SelectRecipeItem(CRecipeDlgItem* pSelectItem)
{
	if (m_pSelectDlgItem)
	{
		if (m_pSelectDlgItem == pSelectItem)
		{
			return;
		}
		else
		{
			m_pSelectDlgItem->SelectItem(FALSE);

			m_pSelectDlgItem = pSelectItem;

			if (m_pSelectDlgItem)
			{
				m_pSelectDlgItem->SelectItem(TRUE);
			}
		}
	}
	else
	{
		m_pSelectDlgItem = pSelectItem;

		if (m_pSelectDlgItem)
		{
			m_pSelectDlgItem->SelectItem(TRUE);
		}
	}
}

VOID CHoiPoiMixRecipeGui::DeSelectRecipeItem()
{
	m_pSelectDlgItem = NULL;
}

VOID CHoiPoiMixRecipeGui::SelectFirstItem()
{
	RwInt32 nCount = 0;
	for each(CRecipeDlgItem* pItem in m_vecShowListRecipe)
	{
		if (nCount == 0)
			SelectRecipe(pItem);
	}
}
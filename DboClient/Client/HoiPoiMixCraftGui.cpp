#include "precomp_dboclient.h"
#include "HoiPoiMixCraftGui.h"

// Debug
#include "NtlDebug.h"

// Gui
#include "GuiUtil.h"

// Shared
#include "ItemRecipeTable.h"
#include "TableContainer.h"
#include "TextAllTable.h"

// Presentation
#include "NtlPLGuiManager.h"

// Simulation
#include "NtlSLApi.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"

// Client
#include "DboEvent.h"
#include "DboLogic.h"
#include "DboEventGenerator.h"
#include "AlarmManager.h"
#include "InfoWndManager.h"
#include "DboGlobal.h"
#include "DialogPriority.h"
#include "IconMoveManager.h"



#define dHOIPOIMIX_CREATE_TIME 2.0f
#define dHOIPOIMIX_HIT_RATE		0.02f
#define dHOIPOIMIX_NOTIFY_TIME	0.5f		///< COOL 같은 텍스트가 떠 있는 타임

CHoiPoiMixCraftGui::CHoiPoiMixCraftGui(const RwChar* pName)
	:CNtlPLGui(pName)
{
	m_pThis = NULL;
	m_pStbSettingCategory = NULL;
	m_pStbZenny = NULL;
	m_pBtnMax = NULL;
	m_pBtnCalc = NULL;
	m_pStbPreViewCategory = NULL;
	m_pBtnCreate = NULL;
	m_pStbControlCategory = NULL;
	m_pPgbConcent = NULL;
	m_pStbConcentText = NULL;
	m_pOpbLog = NULL;
	m_idxEnableRecipe = INVALID_TBLIDX;
	m_bCreating = FALSE;
	m_nLeftCreate = 0;
	m_bStep = FALSE;
	m_fElapsedTime = 0.0f;
	m_uiOpbLogCount = 0;
	m_pPnlNotify = NULL;
	m_pPbBox = NULL;
}

CHoiPoiMixCraftGui::~CHoiPoiMixCraftGui()
{

}

RwBool CHoiPoiMixCraftGui::Create()
{
	NTL_FUNCTION("CHoiPoiMixCraftGui::Create");

	if (!CNtlPLGui::Create("", "gui\\HoipoiMix.srf", "gui\\HoipoiMixCraft.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_DEFAULT);

	m_ptabHoipoiMix = (gui::CTabButton*)GetComponent("tabHoipoiMix");

	m_pstbTitle = (gui::CStaticBox*)GetComponent("stbTitle");
	m_pdlgCreate = (gui::CDialog*)GetComponent("dlgCreate");
	m_pdlgCreate->Enable(true);
	m_pstbTitleCreate = (gui::CStaticBox*)GetComponent("stbTitleCreate");
	m_pStbSettingCategory = (gui::CStaticBox*)GetComponent("stbSettingCategory");
	m_pStbZenny = (gui::CStaticBox*)GetComponent("stbSetZenny");
	m_pBtnMax = (gui::CButton*)GetComponent("btnMax");
	m_pBtnCalc = (gui::CButton*)GetComponent("btnCalc");
	m_pStbPreViewCategory = (gui::CStaticBox*)GetComponent("stbPreviewCategory");
	m_pBtnCreate = (gui::CButton*)GetComponent("btnCreate");
	m_pBtnStop = (gui::CButton*)GetComponent("btnStop");
	m_pStbControlCategory = (gui::CStaticBox*)GetComponent("stbControlCategory");
	m_pPgbConcent = (gui::CProgressBar*)GetComponent("pgbConcent");
	m_pStbConcentText = (gui::CStaticBox*)GetComponent("stbConcentText");
	m_pOpbLog = (gui::COutputBox*)GetComponent("opbLog");
	m_pPnlNotify = (gui::CPanel*)GetComponent("pnlNotify");

	m_pFlaResult[HOIPOI_MIX_RESULT_FAIL] = (gui::CFlash*)GetComponent( "flaFail" );
	m_pFlaResult[HOIPOI_MIX_RESULT_SUCCESS] = (gui::CFlash*)GetComponent( "flaSuccess" );
	m_pFlaResult[HOIPOI_MIX_RESULT_GREAT_SUCCESS] = (gui::CFlash*)GetComponent( "flaSuccess2" );

	for( int i=0; i < HOIPOI_MIX_RESULT_COUNT; i++ )
	{
		m_slotMovieEnd[i] = m_pFlaResult[i]->SigMovieEnd().Connect( this, &CHoiPoiMixCraftGui::OnMovieEnd );
	}


	m_pstbTitle->SetText(GetDisplayStringManager()->GetString("DST_HOIPOIMIX_GUI_TITLE"));
	m_pStbSettingCategory->SetText(GetDisplayStringManager()->GetString("DST_HOIPOIMIX_MATERIAL_CATEGORY"));
	m_pBtnMax->SetToolTip(GetDisplayStringManager()->GetString("DST_HOIPOIMIX_TOOLTIP_MAX"));
	m_pBtnCalc->SetToolTip(GetDisplayStringManager()->GetString("DST_HOIPOIMIX_TOOLTIP_CALC"));
	m_pBtnCreate->SetToolTip(GetDisplayStringManager()->GetString("DST_HOIPOIMIX_TOOLTIP_CREATE_NEW"));
	m_pStbPreViewCategory->SetText(GetDisplayStringManager()->GetString("DST_HOIPOIMIX_PREVIEW_CATEGORY"));
	m_pStbControlCategory->SetText(GetDisplayStringManager()->GetString("DST_HOIPOIMIX_CONCENT_CATEGORY_EX"));
	m_pBtnCreate->SetText(GetDisplayStringManager()->GetString("DST_HOIPOIMIX_BUTTON_CREATE"));
	m_pBtnStop->SetText(GetDisplayStringManager()->GetString("DST_HOIPOIMIX_BUTTON_STOP"));
	m_pBtnStop->SetToolTip(GetDisplayStringManager()->GetString("DST_HOIPOIMIX_TOOLTIP_STOP"));

	m_slotClickedBtnCalc = m_pBtnCalc->SigClicked().Connect(this, &CHoiPoiMixCraftGui::OnClickedBtnCalc);
	m_slotClickedBtnMax = m_pBtnMax->SigClicked().Connect(this, &CHoiPoiMixCraftGui::OnClickedBtnMax);
	m_slotClickedBtnCreate = m_pBtnCreate->SigClicked().Connect(this, &CHoiPoiMixCraftGui::OnClickedBtnCreate);
	m_slotClickedBtnStop = m_pBtnStop->SigClicked().Connect(this, &CHoiPoiMixCraftGui::OnClickedBtnStop);

	// Connect OnPaint to the Signal in the Output Box to make the Slot last drawn.
	m_slotPaint = m_pOpbLog->SigPaint().Connect(this, &CHoiPoiMixCraftGui::OnPaint);
	m_slotMove = m_pThis->SigMove().Connect(this, &CHoiPoiMixCraftGui::OnMove);
	m_slotMouseMove = m_pdlgCreate->SigMouseMove().Connect(this, &CHoiPoiMixCraftGui::OnMouseMove);
	m_slotMouseUp = m_pdlgCreate->SigMouseUp().Connect(this, &CHoiPoiMixCraftGui::OnMouseUp);

	m_SlotRecipe.Create(m_pdlgCreate, DIALOG_HOIPOIMIX_RECIPE, REGULAR_SLOT_ITEM_TABLE, SDS_NONE);
	m_SlotRecipe.SetPosition_fromParent(21, 12);

	CHAR acBuffer[32];
	CRectangle rtParent, rtChild;
	RwInt32 nPosX, nPosY;
	rtParent = m_pdlgCreate->GetScreenRect();
	CRectangle rect;
	for (int i = 0; i < DBO_MAX_COUNT_RECIPE_MATERIAL_ITEM; ++i)
	{
		sprintf_s(acBuffer, 32, "pnlStuffSlot%d", i);
		m_pPnlMaterialSlot[i] = (gui::CPanel*)GetComponent(acBuffer);
		m_SlotMaterial[i].Create(m_pdlgCreate, DIALOG_HOIPOIMIX_RECIPE, REGULAR_SLOT_ITEM_TABLE, SDS_DISABLE);
		rtChild = m_pPnlMaterialSlot[i]->GetScreenRect();
		nPosX = rtChild.left - rtParent.left + 2;
		nPosY = rtChild.top - rtParent.top + 2;
		m_SlotMaterial[i].SetPosition_fromParent(nPosX, nPosY);
		m_pPnlMaterialSlot[i]->Enable(false);

		rect.SetRectWH(nPosX, nPosY, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE);
		m_apStbStackMaterial[i] = NTL_NEW gui::CStaticBox(&rect, m_pdlgCreate, GetNtlGuiManager()->GetSurfaceManager(), DBOGUI_STACKNUM_ALIGN);
		m_apStbStackMaterial[i]->CreateFontStd(DBOGUI_STACKNUM_FONT, DBOGUI_STACKNUM_FONTHEIGHT, DBOGUI_STACKNUM_FONTATTR);
		m_apStbStackMaterial[i]->SetEffectMode(TE_OUTLINE);
		m_apStbStackMaterial[i]->Enable(false);
	}

	for (int i = 0; i < DBO_MAX_COUNT_RECIPE_CREATE_ITEM; ++i)
	{
		sprintf_s(acBuffer, 32, "pnlPreviewSlot%d", i);
		m_pPnlPreViewSlot[i] = (gui::CPanel*)GetComponent(acBuffer);
		m_SlotPreview[i].Create(m_pdlgCreate, DIALOG_HOIPOIMIX_RECIPE, REGULAR_SLOT_ITEM_TABLE, SDS_NONE);
		rtChild = m_pPnlPreViewSlot[i]->GetScreenRect();
		nPosX = rtChild.left - rtParent.left + 2;
		nPosY = rtChild.top - rtParent.top + 2;
		m_SlotPreview[i].SetPosition_fromParent(nPosX, nPosY);
		m_pPnlPreViewSlot[i]->Enable(false);

		rect.SetRectWH(nPosX, nPosY, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE);
		m_apStbStackPreview[i] = NTL_NEW gui::CStaticBox(&rect, m_pdlgCreate, GetNtlGuiManager()->GetSurfaceManager(), DBOGUI_STACKNUM_ALIGN);
		m_apStbStackPreview[i]->CreateFontStd(DBOGUI_STACKNUM_FONT, DBOGUI_STACKNUM_FONTHEIGHT, DBOGUI_STACKNUM_FONTATTR);
		m_apStbStackPreview[i]->SetEffectMode(TE_OUTLINE);
		m_apStbStackPreview[i]->Enable(false);
	}

	for (int i = 0; i < dHOIPOIMIX_ARROW_MAX_NUM; i++)
	{
		sprintf_s(acBuffer, 32, "pnlArrow%d", i);
		m_pPnlEpArrow[i] = (gui::CPanel*)GetComponent(acBuffer);
		m_pPnlEpArrow[i]->Show(false);

		sprintf_s(acBuffer, 32, "pnlHitLine%d", i);
		m_pPnlHitLine[i] = (gui::CPanel*)GetComponent(acBuffer);
		m_pPnlHitLine[i]->Show(false);
	}

	GetNtlGuiManager()->AddUpdateFunc(this);

	m_pPbBox = NTL_NEW gui::CParticleBox(m_pThis, GetNtlGuiManager()->GetSurfaceManager());
	m_pPbBox->Load("CreateItemEffect");

	m_hObject = INVALID_HOBJECT;
	InitFlash();
	InitGui();

	OnMove(0, 0);

	LinkMsg(g_EventCalcPopupResult);
	LinkMsg(g_EventHoipoiMixItemMakeRes);
	LinkMsg(g_EventSobInfoUpdate);
	LinkMsg(g_EventNPCDialogOpen);
	LinkMsg(g_EventRegHoiPoiMixMaterialItem);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CHoiPoiMixCraftGui::Destroy()
{
	NTL_FUNCTION("CHoiPoiMixRecipeGui::Destroy");

	GetNtlGuiManager()->RemoveUpdateFunc(this);

	UnLinkMsg(g_EventCalcPopupResult);
	UnLinkMsg(g_EventHoipoiMixItemMakeRes);
	UnLinkMsg(g_EventSobInfoUpdate);
	UnLinkMsg(g_EventNPCDialogOpen);
	UnLinkMsg(g_EventRegHoiPoiMixMaterialItem);

	m_SlotRecipe.Destroy();

	NTL_DELETE(m_pPbBox);

	for (RwInt32 i = 0; i < DBO_MAX_COUNT_RECIPE_MATERIAL_ITEM; ++i)
	{
		m_SlotMaterial[i].Destroy();
		NTL_DELETE(m_apStbStackMaterial[i]);
	}

	for (RwInt32 i = 0; i < DBO_MAX_COUNT_RECIPE_CREATE_ITEM; ++i)
	{
		m_SlotPreview[i].Destroy();
		NTL_DELETE(m_apStbStackPreview[i]);
	}

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

int CHoiPoiMixCraftGui::SwitchDialog(bool bOpen)
{
	m_hObject = INVALID_HOBJECT;

	if (!bOpen)
	{
		if (m_bCreating)
		{
			StopCreate();
		}

		// init items
		InitItems();

		if (GetInfoWndManager()->GetRequestGui() == DIALOG_HOIPOIMIX_CRAFT)
			GetInfoWndManager()->ShowInfoWindow(FALSE);

		InitFlash();
		InitGui();
	}

	Show(bOpen);

	NTL_RETURN(TRUE);
}

VOID CHoiPoiMixCraftGui::InitFlash()
{
	for( int i=0; i < HOIPOI_MIX_RESULT_COUNT; ++i )
	{
		m_pFlaResult[i]->Show( false );
	}
}

VOID CHoiPoiMixCraftGui::InitGui()
{
	// 
	m_pPgbConcent->SetPos(0);
	m_uiOpbLogCount = 0;
	m_pOpbLog->Clear();
	SetOutputLog(GetDisplayStringManager()->GetString("DST_HOIPOIMIX_GUIDE_NEED_TO_REGIST_REQUIRED_MATERIAL"), INFOCOLOR_0);

	m_pBtnCreate->Show(true);
	m_pBtnStop->Show(false);
}

VOID CHoiPoiMixCraftGui::SetRecipe(TBLIDX idxRecipeTbl)
{
	if (m_bCreating)
		StopCreate();

	// init items
	InitItems();

	// Read the recipe table data.
	sITEM_RECIPE_TBLDAT* pRecipeTblDat = (sITEM_RECIPE_TBLDAT*)API_GetTableContainer()->GetItemRecipeTable()->FindData(idxRecipeTbl);
	if (pRecipeTblDat == NULL)
	{
		DBO_FAIL("Item recipe table is null.");
		return;
	}

	m_idxEnableRecipe = idxRecipeTbl;

	// Read text table data.
	sTEXT_TBLDAT* pTextTblDat = (sTEXT_TBLDAT*)API_GetTableContainer()->GetTextAllTable()->GetItemTbl()->FindData(pRecipeTblDat->dwName);
	if (NULL == pTextTblDat)
	{
		WCHAR awcErrBuffer[32];
		swprintf_s(awcErrBuffer, 32, L"(ITEM_T_TBL)%d", pRecipeTblDat->dwName);
		m_pstbTitleCreate->SetText(awcErrBuffer);
	}
	else
	{
		// Recipe name
		m_pstbTitleCreate->SetText(pTextTblDat->wstrText.c_str());
	}

	m_SlotRecipe.SetIcon(pRecipeTblDat->asCreateItemTblidx[0].itemTblidx);

	// Material setting
	for (int i = 0; i < DBO_MAX_COUNT_RECIPE_MATERIAL_ITEM; i++)
	{
		if (pRecipeTblDat->asMaterial[i].materialTblidx == INVALID_TBLIDX)
		{
			m_SlotMaterial[i].Clear();
			continue;
		}

		m_SlotMaterial[i].SetIcon(pRecipeTblDat->asMaterial[i].materialTblidx, 0);
	}

	// Preview settings
	for (int i = 0; i < DBO_MAX_COUNT_RECIPE_CREATE_ITEM; i++)
	{
		if (pRecipeTblDat->asCreateItemTblidx[i].itemTblidx == INVALID_TBLIDX)
		{
			m_SlotPreview[i].Clear();
			continue;
		}

		m_SlotPreview[i].SetIcon(pRecipeTblDat->asCreateItemTblidx[i].itemTblidx, 0);
	}


	m_pStbZenny->SetText(L"0");

	// Material check
	MaterialCheck();
}

VOID CHoiPoiMixCraftGui::HandleEvents(RWS::CMsg& msg)
{
	if (msg.Id == g_EventCalcPopupResult)
	{
		SDboEventCalcPopupResult* pData = reinterpret_cast<SDboEventCalcPopupResult*>(msg.pData);

		if (pData->nSrcPlace == PLACE_SUB_HOIPOIMIXCREATE)
		{
			if (pData->uiValue == 0)
				return;

			m_pStbZenny->SetText(pData->uiValue);
		}
	}
	else if (msg.Id == g_EventHoipoiMixItemMakeRes)
	{
		SDboEventHoipoiMixItemMakeRes* pResult = reinterpret_cast<SDboEventHoipoiMixItemMakeRes*>(msg.pData);

		if (pResult->wResultCode == GAME_SUCCESS)
		{
			CNtlSobItem* pItem = (CNtlSobItem*)GetNtlSobManager()->GetSobObject(pResult->hItem);
			if (pItem == NULL)
			{
				DBO_FAIL("CHoiPoiMixCraftGui" << pItem);
				return;
			}

			CNtlSobItemAttr* pItemAttr = (CNtlSobItemAttr*)pItem->GetSobAttr();
			if (pItemAttr == NULL)
			{
				DBO_FAIL("CHoiPoiMixCraftGui" << pItem);
				return;
			}

			COLORREF color = RGB(255, 255, 255);

			WCHAR awcBuffer[256];

			swprintf_s(awcBuffer, 256, GetDisplayStringManager()->GetString("DST_HOIPOIMIX_CREATE_SUCCESS_NORMAL_NEW"), Logic_GetItemName(pItemAttr->GetTblIdx()), Logic_GetItemRankName(pItemAttr->GetRank()));
			color = INFOCOLOR_7;

			GetAlarmManager()->FormattedAlarmMessage("DST_HOIPOIMIX_CREATE_SUCCESS_NORMAL_NOTIFY", FALSE, NULL, Logic_GetItemName(pItemAttr->GetTblIdx()));

			//switch( pResult->bySuccessResult )
			//{
			//	case HOIPOI_MIX_RESULT_FAIL:
			//	{
			//		// Manufacture window
			//		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString("DST_HOIPOIMIX_CREATE_FAILED"), Logic_GetItemName( pItemAttr->GetTblIdx() ) ); 
			//		color = INFOCOLOR_0;

			//		// Notify
			//		GetAlarmManager()->FormattedAlarmMessage("DST_HOIPOIMIX_CREATE_FAILED_NOTIFY", FALSE, NULL, Logic_GetItemName( pItemAttr->GetTblIdx() ) );
			//	}
			//	break;
			//	case HOIPOI_MIX_RESULT_SUCCESS:
			//	{
			//		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString("DST_HOIPOIMIX_CREATE_SUCCESS_NORMAL"),Logic_GetItemName( pItemAttr->GetTblIdx() ) );
			//		color = INFOCOLOR_7;

			//		GetAlarmManager()->FormattedAlarmMessage("DST_HOIPOIMIX_CREATE_SUCCESS_NORMAL_NOTIFY", FALSE, NULL, Logic_GetItemName( pItemAttr->GetTblIdx() ) );
			//	}

			//	break;
			//	case HOIPOI_MIX_RESULT_GREAT_SUCCESS:
			//	{
			//		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString("DST_HOIPOIMIX_CREATE_SUCCESS_GREAT"),Logic_GetItemName( pItemAttr->GetTblIdx() ) );
			//		color = INFOCOLOR_7;

			//		GetAlarmManager()->FormattedAlarmMessage("DST_HOIPOIMIX_CREATE_SUCCESS_GREAT_NOTIFY" , FALSE, NULL, Logic_GetItemName( pItemAttr->GetTblIdx() ) );
			//	}
			//	break;

			//	default:
			//		DBO_FAIL( "CHoiPoiMixCraftGui::HandleEvents - exits of success result: " << (int)pResult->bySuccessResult );
			//	break;
			//}

			//SetOutputLog(awcBuffer, color);
			//PlayResultMovie(pResult->bySuccessResult);

			SetOutputLog(awcBuffer, color);
			PlayResultMovie(HOIPOI_MIX_RESULT_SUCCESS);
		}
		else
		{
			GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_HOIPOIMIX_ITEM_CREATE_RES"), TRUE);
			StopCreate();
		}

		// If you still have something to create, make it.
		if (m_bCreating)
			Step(TRUE);
	}
	else if (msg.Id == g_EventSobInfoUpdate)
	{
		SNtlEventSobInfoUpdate* pUpdate = reinterpret_cast<SNtlEventSobInfoUpdate*>(msg.pData);

		if (pUpdate->hSerialId != GetNtlSLGlobal()->GetSobAvatar()->GetSerialID())
			return;

		if (pUpdate->uiUpdateType & EVENT_AIUT_ITEM)
		{
			if (IsShow())
				MaterialCheck();
		}
	}
	else if (msg.Id == g_EventNPCDialogOpen)
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
	}
	else if (msg.Id == g_EventRegHoiPoiMixMaterialItem)
	{
		SDboEventRegMailAttachItem* pData = (SDboEventRegMailAttachItem*)msg.pData;

		CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(pData->hHandle));
		CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pItem->GetSobAttr());

		sITEM_TBLDAT* pItemTbl = pItemAttr->GetItemTbl();

		UpdateMaterial(pItemTbl->tblidx, pData->hHandle, (RwUInt8)Logic_WhichBagHasItem(pData->hHandle), (RwUInt8)pData->uiPos, pItemAttr->GetStackNum());
	}
}

VOID CHoiPoiMixCraftGui::OnPaint()
{
	m_SlotRecipe.Paint();

	for (int i = 0; i < DBO_MAX_COUNT_RECIPE_MATERIAL_ITEM; i++)
	{
		m_SlotMaterial[i].Paint();
	}

	for (int i = 0; i < DBO_MAX_COUNT_RECIPE_CREATE_ITEM; i++)
	{
		m_SlotPreview[i].Paint();
	}
}

VOID CHoiPoiMixCraftGui::OnMove(int nOldX, int nOldY)
{
	CRectangle rect = m_pdlgCreate->GetScreenRect();

	m_SlotRecipe.SetParentPosition(rect.left, rect.top);

	for (RwInt32 i = 0; i < DBO_MAX_COUNT_RECIPE_MATERIAL_ITEM; ++i)
	{
		m_SlotMaterial[i].SetParentPosition(rect.left, rect.top);
	}

	for (RwInt32 i = 0; i < DBO_MAX_COUNT_RECIPE_CREATE_ITEM; ++i)
	{
		m_SlotPreview[i].SetParentPosition(rect.left, rect.top);
	}
}

/**
* \brief Calculate the maximum number that can be created.
*/
VOID CHoiPoiMixCraftGui::OnClickedBtnMax(gui::CComponent* pComponent)
{
	RwInt32 nAmount = Logic_AmountEnableCreateRecipe();
	if (nAmount == 0)
	{
		GetAlarmManager()->AlarmMessage("DST_HOIPOIMIX_CREATE_NEED_MATERIAL");
		return;
	}

	m_pStbZenny->SetText(nAmount);
}

VOID CHoiPoiMixCraftGui::OnClickedBtnCalc(gui::CComponent* pComponent)
{
	RwInt32 nMaxAmount = Logic_AmountEnableCreateRecipe();
	if (nMaxAmount == 0)
	{
		GetAlarmManager()->AlarmMessage("DST_HOIPOIMIX_CREATE_NEED_MATERIAL");
		return;
	}

	CRectangle rect = pComponent->GetScreenRect();
	CDboEventGenerator::CalcPopupShow(TRUE, INVALID_SERIAL_ID, PLACE_SUB_HOIPOIMIXCREATE, rect.left, rect.top, nMaxAmount);
}

VOID CHoiPoiMixCraftGui::OnClickedBtnCreate(gui::CComponent* pComponent)
{
	if (m_bCreating)
		StopCreate();

	if (m_idxEnableRecipe == INVALID_TBLIDX)
	{
		return;
	}

	RwInt32 nAmount = _wtoi(m_pStbZenny->GetText().c_str());
	if (nAmount == 0)
	{
		GetAlarmManager()->AlarmMessage("DST_HOIPOIMIX_CREATE_NEED_SET_NUMBER");
		return;
	}

	StartCreate(nAmount);
}

VOID CHoiPoiMixCraftGui::StartCreate(RwInt32 nAmount)
{
	if (nAmount > 0)
	{
		// Set number of times to create
		m_nLeftCreate = nAmount;
		SetCreating(TRUE);

		// UpdateFunc Enrollment

		Step(TRUE);
	}
}

VOID CHoiPoiMixCraftGui::Step(RwBool bStep)
{
	if (bStep)
	{
		// Read the recipe table data.
		sITEM_RECIPE_TBLDAT* pRecipeTblDat = (sITEM_RECIPE_TBLDAT*)API_GetTableContainer()->GetItemRecipeTable()->FindData(m_idxEnableRecipe);
		if (pRecipeTblDat == NULL)
		{
			DBO_FAIL("Out of item recipe table index");
			return;
		}

		m_pPnlNotify->ClearSurface();

		m_fElapsedTime = 0.0f;
		m_pPgbConcent->SetPos(0);

	}
	else
	{
		m_nLeftCreate--;
		m_pStbZenny->SetText(m_nLeftCreate);
		m_fElapsedTime = 0.0f;

		if (m_nLeftCreate <= 0)
			StopCreate();
	}

	m_bStep = bStep;
}

VOID CHoiPoiMixCraftGui::Update(RwReal fElapsed)
{
	if (m_pPbBox)
		m_pPbBox->Update(fElapsed);

	if (!m_pThis->IsVisible())
		return;

	for(int i = 0; i < HOIPOI_MIX_RESULT_COUNT; i++)
	{
		if( m_pFlaResult[i]->IsVisible() )
			m_pFlaResult[i]->Update( fElapsed );
	}

	if (m_nLeftCreate > 0)
	{
		if (m_bStep)
		{
			m_fElapsedTime += fElapsed;

			if (m_fNotifyTime >= 0.0f)
			{
				m_fNotifyTime -= fElapsed;
				if (m_fNotifyTime <= 0.0f)
				{
					//m_pStbNotify->Clear();
					m_pPnlNotify->ClearSurface();
				}
			}

			// Total Progress ( sec 2 )

			RwReal fPos = m_fElapsedTime / dHOIPOIMIX_CREATE_TIME;

			if (fPos >= 1.0f)
			{
				fPos = 1.0f;

				if (m_bCreating)
				{
					BYTE byMaterialCount = 0;
					sINVEN_SLOT aMaterialSlot[DBO_MAX_COUNT_RECIPE_MATERIAL_ITEM];

					for (int i = 0; i < DBO_MAX_COUNT_RECIPE_MATERIAL_ITEM; i++)
					{
						if (m_recipeMat[i].byStack == 0)
							break;

						aMaterialSlot[byMaterialCount].byPlace = m_recipeMat[i].byPlace;
						aMaterialSlot[byMaterialCount].byPos = m_recipeMat[i].byPos;

						++byMaterialCount;
					}

					GetDboGlobal()->GetGamePacketGenerator()->SendHoipoiMixItemCreateReq(m_hObject, m_idxEnableRecipe, byMaterialCount, aMaterialSlot);
				}

				Step(FALSE);
			}

			int nPos = (int)(fPos * dHOIPOIMIX_PROGRESS_BAR_WIDTH_F);
			m_pPgbConcent->SetPos(nPos);
		}
	}
}

void CHoiPoiMixCraftGui::MaterialCheck()
{
	// Read the recipe table data.
	sITEM_RECIPE_TBLDAT* pRecipeTblDat = (sITEM_RECIPE_TBLDAT*)API_GetTableContainer()->GetItemRecipeTable()->FindData(m_idxEnableRecipe);
	if (pRecipeTblDat == NULL)
	{
		DBO_FAIL("Out of item recipe table index");
		return;
	}

	for (int i = 0; i < DBO_MAX_COUNT_RECIPE_MATERIAL_ITEM; i++)
	{
		if (pRecipeTblDat->asMaterial[i].materialTblidx == INVALID_TBLIDX)
		{
			m_apStbStackMaterial[i]->Clear();
			continue;
		}

		CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(m_recipeMat[i].hItem));
		if (pItem)
		{
			CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pItem->GetSobAttr());

			BYTE byCount = pItemAttr->GetStackNum();
			m_apStbStackMaterial[i]->Format(L"%u/%u", byCount, pRecipeTblDat->asMaterial[i].byMaterialCount);

			if (byCount >= pRecipeTblDat->asMaterial[i].byMaterialCount)
				m_SlotMaterial[i].ShowDisable(FALSE);
			else
				m_SlotMaterial[i].ShowDisable(TRUE);

			// UPDATE COUNT
			m_recipeMat[i].byStack = byCount;
		}
		else
		{
			m_apStbStackMaterial[i]->Format(L"0/%u", pRecipeTblDat->asMaterial[i].byMaterialCount);
			m_SlotMaterial[i].ShowDisable(TRUE);

			m_recipeMat[i].byPlace = INVALID_BYTE;
			m_recipeMat[i].byPos = INVALID_BYTE;
			m_recipeMat[i].byStack = 0;
			m_recipeMat[i].hItem = INVALID_HOBJECT;
		}
	}
}

VOID CHoiPoiMixCraftGui::StopCreate()
{
	m_nLeftCreate = 0;
	m_pPgbConcent->SetPos(0);
	SetCreating(FALSE);

	m_pBtnCreate->Show(true);
	m_pBtnStop->Show(false);
}

VOID CHoiPoiMixCraftGui::PlayResultMovie(RwUInt8 byResultType)
{
	m_pFlaResult[byResultType]->Show( TRUE );
	m_pFlaResult[byResultType]->RestartMovie();
}

VOID CHoiPoiMixCraftGui::SetCreating(RwBool bCreate)
{
	m_bCreating = bCreate;

	if (IsShow())
	{
		m_pBtnCreate->Show(false);
		m_pBtnStop->Show(true);
	}
}

VOID CHoiPoiMixCraftGui::SetOutputLog(const WCHAR* pLog, COLORREF color)
{
	m_uiOpbLogCount++;

	WCHAR awcBuffer[128];
	swprintf_s(awcBuffer, 128, L"%u: %s", m_uiOpbLogCount, pLog);
	m_pOpbLog->AddText(awcBuffer, color);
}

VOID CHoiPoiMixCraftGui::OnMouseMove(RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos)
{
	CRectangle rect = m_pThis->GetScreenRect();

	bool bIn = false;

	if (m_SlotRecipe.PtInRect(nXPos, nYPos))
	{
		sITEM_RECIPE_TBLDAT* pTblDat = (sITEM_RECIPE_TBLDAT*)API_GetTableContainer()->GetItemRecipeTable()->FindData(m_idxEnableRecipe);
		if (pTblDat)
		{
			GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_RECIPE,
				rect.left + m_SlotRecipe.GetX_fromParent(), rect.top + m_SlotRecipe.GetY_fromParent(),
				(VOID*)pTblDat,
				DIALOG_HOIPOIMIX_CRAFT);
		}
		else
		{
			WCHAR awcBuffer[128];
			swprintf_s(awcBuffer, 128, L"Invalid recipe table idx %d", m_idxEnableRecipe);
			GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_JUST_WTEXT,
				rect.left + m_SlotRecipe.GetX_fromParent(), rect.top + m_SlotRecipe.GetY_fromParent(),
				(VOID*)awcBuffer,
				DIALOG_HOIPOIMIX_CRAFT);
		}

		bIn = true;
	}

	for (int i = 0; i < DBO_MAX_COUNT_RECIPE_MATERIAL_ITEM; ++i)
	{
		if (m_SlotMaterial[i].PtInRect(nXPos, nYPos))
		{
			if (m_SlotMaterial[i].GetSerial() != INVALID_SERIAL_ID)
			{
				// Info Window
				GetInfoWndManager()->ShowInfoWindow(TRUE,
					CInfoWndManager::INFOWND_TABLE_ITEM,
					m_SlotMaterial[i].GetX_fromParent() + rect.left,
					m_SlotMaterial[i].GetY_fromParent() + rect.top,
					m_SlotMaterial[i].GetItemTable(),
					DIALOG_HOIPOIMIX_CRAFT);

				bIn = true;

				break;
			}
		}
	}

	for (int i = 0; i < DBO_MAX_COUNT_RECIPE_CREATE_ITEM; ++i)
	{
		if (m_SlotPreview[i].PtInRect(nXPos, nYPos))
		{
			if (m_SlotPreview[i].GetSerial() != INVALID_SERIAL_ID)
			{
				// InfoWindow
				GetInfoWndManager()->ShowInfoWindow(TRUE,
					CInfoWndManager::INFOWND_TABLE_ITEM,
					m_SlotPreview[i].GetX_fromParent() + rect.left,
					m_SlotPreview[i].GetY_fromParent() + rect.top,
					m_SlotPreview[i].GetItemTable(),
					DIALOG_HOIPOIMIX_CRAFT);

				bIn = true;

				break;
			}
		}
	}

	if (!bIn)
	{
		if (GetInfoWndManager()->GetRequestGui() == DIALOG_HOIPOIMIX_CRAFT)
			GetInfoWndManager()->ShowInfoWindow(FALSE);
	}
}

void CHoiPoiMixCraftGui::OnMouseUp(const CKey & key)
{
	switch (key.m_nID)
	{
		case UD_LEFT_BUTTON:
		{
			if (!GetIconMoveManager()->IsActive() && GetIconMoveManager()->GetSrcPlace() == PLACE_BAG)
			{
				return;
			}

			// If it is not a bag, modify it so that it does not attach.
			if (GetIconMoveManager()->IsActive() && GetIconMoveManager()->GetSrcPlace() != PLACE_BAG)
			{
				return;
			}

			if (GetIconMoveManager()->GetSrcSerial() == INVALID_SERIAL_ID)
				return;

			for (int i = 0; i < DBO_MAX_COUNT_RECIPE_MATERIAL_ITEM; i++)
			{
				if (m_SlotMaterial[i].PtInRect((int)key.m_fX, (int)key.m_fY))
				{
					// Read the recipe table data.
					sITEM_RECIPE_TBLDAT* pRecipeTblDat = (sITEM_RECIPE_TBLDAT*)API_GetTableContainer()->GetItemRecipeTable()->FindData(m_idxEnableRecipe);
					if (pRecipeTblDat == NULL)
					{
						DBO_FAIL("Out of item recipe table index");
						return;
					}

					if (pRecipeTblDat->asMaterial[i].materialTblidx == INVALID_TBLIDX)
						return;

					if (m_recipeMat[i].byStack != 0)
					{
						GetAlarmManager()->AlarmMessage("DST_HOIPOIMIX_GUIDE_NEED_TO_UNREGIST_MATERIAL");
						return;
					}

					CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(GetIconMoveManager()->GetSrcSerial()));
					CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pItem->GetSobAttr());

					sITEM_TBLDAT* pItemTbl = pItemAttr->GetItemTbl();

					if (pRecipeTblDat->asMaterial[i].materialTblidx != pItemTbl->tblidx)
					{
						GetAlarmManager()->AlarmMessage("DST_HOIPOIMIX_GUIDE_NEED_TO_CORRECT_MATERIAL");
						return;
					}

					UpdateMaterial(pRecipeTblDat->asMaterial[i].materialTblidx, GetIconMoveManager()->GetSrcSerial(),
						(BYTE)Logic_WhichBagHasItem(GetIconMoveManager()->GetSrcSerial()), (BYTE)GetIconMoveManager()->GetSrcSlotIdx(), pItemAttr->GetStackNum());

					break;
				}
			}
		}
		break;

		case UD_RIGHT_BUTTON: // remove item
		{
			if (m_bCreating)
			{
				GetAlarmManager()->AlarmMessage("DST_HOIPOIMIX_GUIDE_CAN_NOT_UNREGIST_MATERIAL_FOR_CREATING");
				return;
			}

			for (int i = 0; i < DBO_MAX_COUNT_RECIPE_MATERIAL_ITEM; i++)
			{
				if (m_SlotMaterial[i].PtInRect((int)key.m_fX, (int)key.m_fY))
				{
					// Read the recipe table data.
					sITEM_RECIPE_TBLDAT* pRecipeTblDat = (sITEM_RECIPE_TBLDAT*)API_GetTableContainer()->GetItemRecipeTable()->FindData(m_idxEnableRecipe);
					if (pRecipeTblDat == NULL)
					{
						DBO_FAIL("Out of item recipe table index");
						return;
					}

					if (m_recipeMat[i].byStack != 0)
					{
						UpdateMaterial(pRecipeTblDat->asMaterial[i].materialTblidx, INVALID_SERIAL_ID, INVALID_BYTE, INVALID_BYTE, 0);
					}

					break;
				}
			}
		}
		break;

		default: break;
	}

	GetIconMoveManager()->IconMoveEnd();
}

VOID CHoiPoiMixCraftGui::OnMovieEnd(gui::CComponent* pComponent)
{
	gui::CFlash* pFlash = (gui::CFlash*)pComponent;
	pFlash->PlayMovie(FALSE);
	pFlash->Show(FALSE);
}

void CHoiPoiMixCraftGui::InitItems()
{
	for (int i = 0; i < DBO_MAX_COUNT_RECIPE_MATERIAL_ITEM; i++)
	{
		if (m_recipeMat[i].byStack != 0)
		{
			CDboEventGenerator::DialogEvent(DIALOGEVENT_END_TRADING_ITEM_IN_BAG, PLACE_SUB_HOIPOIMIXCREATE, PLACE_BAG,
				Logic_ConvertContainerTypeToBagIdx(m_recipeMat[i].byPlace), m_recipeMat[i].byPos, m_recipeMat[i].hItem);
		}

		m_recipeMat[i].byPlace = INVALID_BYTE;
		m_recipeMat[i].byPos = INVALID_BYTE;
		m_recipeMat[i].byStack = 0;
		m_recipeMat[i].hItem = INVALID_HOBJECT;
	}
}

bool CHoiPoiMixCraftGui::UpdateMaterial(TBLIDX tblidx, HOBJECT hItem, BYTE byPlace, BYTE byPos, BYTE byStackCount)
{
	// Read the recipe table data.
	sITEM_RECIPE_TBLDAT* pRecipeTblDat = (sITEM_RECIPE_TBLDAT*)API_GetTableContainer()->GetItemRecipeTable()->FindData(m_idxEnableRecipe);
	if (pRecipeTblDat == NULL)
	{
		DBO_FAIL("Out of item recipe table index");
		return false;
	}

	for (int i = 0; i < DBO_MAX_COUNT_RECIPE_MATERIAL_ITEM; i++)
	{
		if (pRecipeTblDat->asMaterial[i].materialTblidx == tblidx)
		{
			if (hItem != INVALID_HOBJECT && m_recipeMat[i].byStack != 0) // CHECK if we try to register item while one is already
			{
				GetAlarmManager()->AlarmMessage("DST_HOIPOIMIX_GUIDE_ALREADY_REGIST_THIS_MATERIAL");
				return false;
			}

			m_apStbStackMaterial[i]->Format(L"%u/%u", byStackCount, pRecipeTblDat->asMaterial[i].byMaterialCount);

			if (byStackCount >= pRecipeTblDat->asMaterial[i].byMaterialCount)
				m_SlotMaterial[i].ShowDisable(FALSE);
			else
				m_SlotMaterial[i].ShowDisable(TRUE);

			if (hItem == INVALID_HOBJECT)
			{
				if (m_recipeMat[i].byStack != 0)
				{
					CDboEventGenerator::DialogEvent(DIALOGEVENT_END_TRADING_ITEM_IN_BAG, PLACE_SUB_HOIPOIMIXCREATE, PLACE_BAG,
						Logic_ConvertContainerTypeToBagIdx(m_recipeMat[i].byPlace), m_recipeMat[i].byPos, m_recipeMat[i].hItem);

					m_pStbZenny->SetText(L"0");
				}
			}
			else
			{
				CDboEventGenerator::DialogEvent(DIALOGEVENT_BEGIN_TRADING_ITEM_IN_BAG, PLACE_SUB_HOIPOIMIXCREATE, PLACE_BAG,
					Logic_ConvertContainerTypeToBagIdx(byPlace), byPos, hItem);
			}

			m_recipeMat[i].byPlace = byPlace;
			m_recipeMat[i].byPos = byPos;
			m_recipeMat[i].byStack = byStackCount;
			m_recipeMat[i].hItem = hItem;

			return true;
		}
	}

	GetAlarmManager()->AlarmMessage("DST_HOIPOIMIX_GUIDE_NEED_TO_CORRECT_MATERIAL");
	return false;
}

VOID CHoiPoiMixCraftGui::OnClickedBtnStop(gui::CComponent* pComponent)
{
	StopCreate();
}


int CHoiPoiMixCraftGui::Logic_AmountEnableCreateRecipe()
{
	// Reading recipe table data
	sITEM_RECIPE_TBLDAT* pRecipeTblDat = (sITEM_RECIPE_TBLDAT*)API_GetTableContainer()->GetItemRecipeTable()->FindData(m_idxEnableRecipe);
	if (pRecipeTblDat == NULL)
	{
		NTL_ASSERTFAIL("Out of item recipe table index");
		return FALSE;
	}

	// Make sure the avatar has the current material

	// Number of possessions / required number = number of creatures
	// The smallest number is the number that can be made

	RwReal fMin = 999.0f;
	for (RwInt32 i = 0; i < DBO_MAX_COUNT_RECIPE_MATERIAL_ITEM; ++i)
	{
		if (pRecipeTblDat->asMaterial[i].materialTblidx == INVALID_TBLIDX)
			continue;

		RwReal fRequired = (RwReal)pRecipeTblDat->asMaterial[i].byMaterialCount;
		RwReal fHaved = (RwReal)m_recipeMat[i].byStack;

		// Can not divide by zero.
		if (fRequired == 0.f || fHaved == 0.f)
			return 0;

		RwReal fEnableAmount = fHaved / fRequired;

		if (fMin > fEnableAmount)
			fMin = fEnableAmount;
	}

	// It can not be larger than 999.
	if (fMin >= 999.f)
		return 0;

	return (RwInt32)fMin;

}
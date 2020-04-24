#include "precomp_dboclient.h"
#include "PlayerTitleGui.h"

// core
#include "NtlDebug.h"

// shared
#include "CharTitleTable.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation

// dbo
#include "DboGlobal.h"
#include "DialogManager.h"
#include "DisplayStringManager.h"


#define TITLE_LIST_X 21
#define TITLE_LIST_Y 40
#define TITLE_LIST_MARGIN 4
#define TITLE_ITEM_WITH 274
#define TITLE_ITEM_HEIGHT 19

#define TITLE_ITEM_COLOR_AVAILABLE RGB(255, 255, 255)
#define TITLE_ITEM_COLOR_UNAVAILABLE RGB(180, 180, 180)
#define TITLE_ITEM_COLOR_DOWN RGB(255, 219, 71)


CPlayerTitleGui::CPlayerTitleGui(const RwChar* pName)
	:CNtlPLGui(pName)
	, m_pDialogName(NULL)
{
	m_nButtonIndex = 0;
	m_uiActiveCharTitleIndex = INVALID_TBLIDX;
	m_nSelectedButton = -1;
	m_uiSelectedTitleIndex = INVALID_TBLIDX;
}

CPlayerTitleGui::~CPlayerTitleGui()
{
	Destroy();
}

RwBool CPlayerTitleGui::Create()
{
	NTL_FUNCTION("CPlayerTitleGui::Create");

	if (!CNtlPLGui::Create("", "gui\\playertitle.srf", "gui\\playertitle.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	CRectangle rect;

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_pDialogName = (gui::CStaticBox*)GetComponent("sttDialogTitle");
	m_pDialogName->SetText(GetDisplayStringManager()->GetString("DST_CHAR_TITLE"));

	m_pExitButton = (gui::CButton*)GetComponent("ExitButton");
	m_slotCloseButton = m_pExitButton->SigClicked().Connect(this, &CPlayerTitleGui::CloseButtonClicked);

	m_pBtnHelp = (gui::CButton*)GetComponent("btnHelp");
	m_slotClickedBtnHelp = m_pBtnHelp->SigClicked().Connect(this, &CPlayerTitleGui::OnClickedBtnHelp);

	m_pSbTitleScroll = (gui::CScrollBar*)GetComponent("scbTitleScroll");
	m_slotScroll = m_pSbTitleScroll->SigValueChanged().Connect(this, &CPlayerTitleGui::OnTitleScrollChanged);
	m_slotScrollMove = m_pSbTitleScroll->SigSliderMoved().Connect(this, &CPlayerTitleGui::OnTitleScrollChanged);

	m_pSbEffectScroll = (gui::CScrollBar*)GetComponent("scbEffectScroll");

	m_pApplyButton = (gui::CButton*)GetComponent("btnApply");
	m_slotApplyButton = m_pApplyButton->SigToggled().Connect(this, &CPlayerTitleGui::OnApplyToggled);

	m_psttApply = (gui::CStaticBox*)GetComponent("sttApply");
	m_psttApply->SetText(GetDisplayStringManager()->GetString("DST_OPTION_OK"));

	CreateInfoStaticBox();

	m_InfoList.Create(m_pThis, this,
		&CPlayerTitleGui::OnCreateItem,
		&CPlayerTitleGui::OnDestroyItem,
		&CPlayerTitleGui::OnClickedItem,
		TITLE_LIST_X, TITLE_LIST_Y,
		TITLE_LIST_MARGIN, TITLE_LIST_MAX_VISIBLE);


	m_srfEffectBar.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("gui\\playertitle.srf", "TitleEffectHeadLine"));
	m_srfEffectBar.SetPositionfromParent(10, 346);

	m_EffectBackSurface.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("gui\\playertitle.srf", "srfEffectInfoPanelLeft"));
	m_EffectBackSurface.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("gui\\playertitle.srf", "srfEffectInfoPanelCenter"));
	m_EffectBackSurface.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("gui\\playertitle.srf", "srfEffectInfoPanelRight"));
	m_EffectBackSurface.SetSize(290, 90);
	m_EffectBackSurface.SetPositionfromParent(16, 365);


	rect.SetRectWH(31, 348, 123, 13);
	m_pEffectStatic = NTL_NEW gui::CStaticBox(rect, GetDialog(), GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT);
	m_pEffectStatic->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pEffectStatic->SetText(GetDisplayStringManager()->GetString("DST_ITEM_ADDITIONAL_EFFECT"));
	m_pEffectStatic->Enable(false);

	m_pEffectBox = (gui::COutputBox*)GetComponent("EffecteContent");
	m_pEffectBox->CreateFontStd("detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pEffectBox->SetLineSpace(3);
	m_pEffectBox->SetMaxLine(NTL_MAX_CHAR_TITLE_EFFECT);
	m_pEffectBox->ShowScrollBar(false);
	m_pEffectBox->SetTextColor(RGB(255, 210, 0));

	// sig
	m_slotPaint = m_pThis->SigPaint().Connect(this, &CPlayerTitleGui::OnPaint);
	m_slotMove = m_pThis->SigMove().Connect(this, &CPlayerTitleGui::OnMove);
	m_slotCaptureWheelMove = m_pThis->SigMouseWheel().Connect(this, &CPlayerTitleGui::OnCaptureWheelMove);

	// Link
	LinkMsg(g_EventCharTitleSelectRes);
	LinkMsg(g_EventCharTitleUpdate);

	// load titles
	LoadCharTitleInfo();

	Show(false);
	m_InfoList.Show(false);

	NTL_RETURN(TRUE);
}

VOID CPlayerTitleGui::Destroy()
{
	NTL_FUNCTION("CPlayerTitleGui::Destroy");

	m_InfoList.Destroy();

	for (std::vector<sTITLES*>::iterator it = m_vecTitles.begin(); it != m_vecTitles.end(); )
	{
		sTITLES* pTitle = *it;

		it = m_vecTitles.erase(it);

		delete pTitle;
	}

	UnLinkMsg(g_EventCharTitleSelectRes);
	UnLinkMsg(g_EventCharTitleUpdate);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

VOID CPlayerTitleGui::CloseButtonClicked(gui::CComponent* pComponent)
{
	if (!Logic_CanMouseInput_in_Tutorial(ETL_MOUSE_INPUT_TYPE_COMMUNITY_WND_CLOSE_BTN))
		return;

	GetDialogManager()->CloseDialog(DIALOG_PLAYER_TITLE);
}

VOID CPlayerTitleGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rtScreen = GetPosition();

	m_srfEffectBar.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_EffectBackSurface.SetPositionbyParent(rtScreen.left, rtScreen.top);
}

VOID CPlayerTitleGui::OnPaint()
{
	m_srfEffectBar.Render();
	m_EffectBackSurface.Render();
}

RwInt32 CPlayerTitleGui::SwitchDialog(bool bOpen)
{
	Show(bOpen);
	m_InfoList.Show(bOpen);

	return 1;
}

VOID CPlayerTitleGui::HandleEvents(RWS::CMsg &msg)
{
	NTL_FUNCTION("CPlayerTitleGui::HandleEvents");

	if (msg.Id == g_EventCharTitleSelectRes)
	{
		SDboEventCharTitleSelectRes* pData = reinterpret_cast<SDboEventCharTitleSelectRes*>(msg.pData);

		SelectCharTitle(pData->uiTitle);
	}
	else if (msg.Id == g_EventCharTitleUpdate)
	{
		SDboEventCharTitleUpdate* pData = reinterpret_cast<SDboEventCharTitleUpdate*>(msg.pData);

		UpdateCharTitle(pData->uiTitle, pData->bDelete);
	}

	NTL_RETURNVOID();
}

VOID CPlayerTitleGui::OnClickedBtnHelp(gui::CComponent* pComponent)
{
	//CDboEventGenerator::OpenHelpContent(DIALOG_COMMUNITY);
}

VOID CPlayerTitleGui::OnToggled(gui::CComponent * pComponent, bool bToggled)
{
	if (bToggled == false)
		m_nSelectedButton = -1;
}

VOID CPlayerTitleGui::OnCaptureWheelMove(RwInt32 nFlag, RwInt16 sDelta, CPos & pos)
{
	if (!IsShow())
		return;

	RwInt32 nDelta = m_pSbTitleScroll->GetValue() - sDelta / GUI_MOUSE_WHEEL_DELTA;
	nDelta = max(0, min(nDelta, m_pSbTitleScroll->GetMaxValue()));

	m_pSbTitleScroll->SetValue(nDelta);
}

VOID CPlayerTitleGui::OnTitleScrollChanged(RwInt32 nNewOffset)
{
	m_InfoList.SetValue(nNewOffset);
	UpdateTitleInfo();
}

gui::CButton * CPlayerTitleGui::OnCreateItem()
{
	gui::CButton* pButton = NTL_NEW gui::CButton(CRectangle(0, 0, TITLE_ITEM_WITH, TITLE_ITEM_HEIGHT), L"", NULL,
		GetNtlGuiManager()->GetSurfaceManager());

	pButton->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("gui\\playertitle.srf", "srfTitleListFoc"));

	pButton->SetTextStyle(0);
	pButton->SetTextCoord(20, 4);
	pButton->SetTextDownCoordDiff(0, 0);
	pButton->SetToggleMode(true);
	pButton->SetTextFont(DETAIL_FONT, 90, NULL);

	pButton->ApplyText();

	m_slotListWheelMove[m_nButtonIndex] = pButton->SigMouseWheel().Connect(this, &CPlayerTitleGui::OnCaptureWheelMove);
	m_slotToggled[m_nButtonIndex] = pButton->SigToggled().Connect(this, &CPlayerTitleGui::OnToggled);
	++m_nButtonIndex;

	return pButton;
}

void CPlayerTitleGui::OnDestroyItem(gui::CButton * pButton)
{
	NTL_DELETE(pButton);
}

void CPlayerTitleGui::OnClickedItem(RwInt32 iIndex)
{
	CButtonList<CPlayerTitleGui, sTITLES*>::SItem sItem = m_InfoList.GetItem(iIndex);
	sTITLES* pTitle = reinterpret_cast<sTITLES*>(sItem.TData);

	m_pEffectBox->Clear();

	m_uiSelectedTitleIndex = pTitle->titleIdx;
	m_nSelectedButton = iIndex;

	// set effect text
	for (int i = 0; i < pTitle->nEffectCount; i++)
	{
		m_pEffectBox->AddText(pTitle->wstrEffect[i].c_str());
	}

	if (pTitle->bRegistered)
	{
		if (m_uiActiveCharTitleIndex == m_uiSelectedTitleIndex)
			m_pApplyButton->SetDown(true);
		else
			m_pApplyButton->SetDown(false);

		m_psttApply->Show(true);
		m_pApplyButton->Show(true);
	}
	else
	{
		m_psttApply->Show(false);
		m_pApplyButton->Show(false);
	}
}

void CPlayerTitleGui::OnApplyToggled(gui::CComponent * pComponent, bool bToggle)
{
	// check if we try tick-off the current selected title..
	if (bToggle == false)
	{
		m_pApplyButton->SetDown(true); // set the tick again
		return;
	}

	// send packet to select title
	if (GetDboGlobal()->GetGamePacketGenerator()->SendCharTitleSelectReq(m_uiSelectedTitleIndex) == false)
	{
		GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
		m_pApplyButton->SetDown(false);
	}
}

void CPlayerTitleGui::CreateInfoStaticBox()
{
	CRectangle rectBackSpace;
	for (int i = 0; i < TITLE_LIST_MAX_VISIBLE; ++i)
	{
		rectBackSpace.left = TITLE_LIST_X;
		rectBackSpace.right = rectBackSpace.left + TITLE_ITEM_WITH;
		rectBackSpace.top = TITLE_LIST_Y + i * (TITLE_ITEM_HEIGHT + TITLE_LIST_MARGIN);
		rectBackSpace.bottom = rectBackSpace.top + TITLE_ITEM_HEIGHT;

		m_pPnlBackSpace[i] = NTL_NEW gui::CPanel(rectBackSpace, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), GetNtlGuiManager()->GetSurfaceManager()->GetSurface("gui\\playertitle.srf", "srfTitleBackSpace"));
	}
}

void CPlayerTitleGui::RefreshTitleList()
{
	int nCurValue = m_InfoList.GetValue();
	m_InfoList.SetValue(nCurValue);
	UpdateTitleInfo();
}

void CPlayerTitleGui::UpdateTitleInfo(unsigned int uiTitleIdx/* = 0*/)
{
	RwInt32 nCurValue = m_InfoList.GetValue();
	for (int i = nCurValue; i < nCurValue + TITLE_LIST_MAX_VISIBLE; ++i)
	{
		if (i >= m_InfoList.GetSize())
			break;

		CButtonList<CPlayerTitleGui, sTITLES*>::SItem sItem = m_InfoList.GetItem(i);

		sTITLES* pTitle = reinterpret_cast<sTITLES*>(sItem.TData);

		if (uiTitleIdx == NULL || pTitle->titleIdx == uiTitleIdx)
		{
			if (pTitle->bRegistered)  // is registered?
			{
				gui::CButton* pButton = m_InfoList.GetButton(i - nCurValue);
				if (pButton)
				{
					if (i == m_nSelectedButton)
					{
						pButton->Toggle();
					}
					else if (pButton->IsDown())
					{
						int nTmp = m_nSelectedButton;
						pButton->Toggle();
						m_nSelectedButton = nTmp;
					}

					pButton->SetTextUpColor(TITLE_ITEM_COLOR_AVAILABLE);
					pButton->SetTextFocusColor(TITLE_ITEM_COLOR_AVAILABLE);
					pButton->SetTextDownColor(TITLE_ITEM_COLOR_DOWN);
					pButton->ApplyText();
				}
			}
			else    // not registered
			{
				gui::CButton* pButton = m_InfoList.GetButton(i - nCurValue);
				if (pButton)
				{
					if (i == m_nSelectedButton)
					{
						pButton->Toggle();
					}
					else if (pButton->IsDown())
					{
						int nTmp = m_nSelectedButton;
						pButton->Toggle();
						m_nSelectedButton = nTmp;
					}

					pButton->SetTextUpColor(TITLE_ITEM_COLOR_UNAVAILABLE);
					pButton->SetTextFocusColor(TITLE_ITEM_COLOR_UNAVAILABLE);
					pButton->SetTextDownColor(TITLE_ITEM_COLOR_DOWN);
					pButton->ApplyText();
				}
			}

			// If the update is for one title, it ends without updating after finishing update.
			if (pTitle->titleIdx == uiTitleIdx)
				break;
		}
	}
  
	int nMax = m_InfoList.GetSize() - TITLE_LIST_MAX_VISIBLE;
	m_pSbTitleScroll->SetRange(0, max(0, nMax));
	m_pSbTitleScroll->SetValue(nCurValue);
}

void CPlayerTitleGui::LoadCharTitleInfo()
{
	//const WCHAR* wchInvalidTitle = GetDisplayStringManager()->GetString("DST_CHARTITLE_INVALID");

	sTITLES* pDefaultTitle = new sTITLES;
	m_vecTitles.push_back(pDefaultTitle);

	pDefaultTitle->bRegistered = true;
	pDefaultTitle->titleIdx = INVALID_TBLIDX;

	// add default title
	m_InfoList.AddItem(GetDisplayStringManager()->GetString("DST_CHARTITLE_NOSELECT"), pDefaultTitle);

	SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
	char* pTitleIndexFlag = pAvatarInfo->abyTitleIndexFlag;

	// load all titles
	CTextTable* pTitleTextTable = API_GetTableContainer()->GetTextAllTable()->GetCharTitleTbl();
	CCharTitleTable* pTable = API_GetTableContainer()->GetCharTitleTable();

	for (CTable::TABLEIT it = pTable->Begin(); it != pTable->End(); it++)
	{
		sCHARTITLE_TBLDAT* pTblData = (sCHARTITLE_TBLDAT*)it->second;
		if (pTblData)
		{
			sTITLES* pTitle = new sTITLES;
			m_vecTitles.push_back(pTitle);

			pTitle->titleIdx = pTblData->tblidx;

			// check if has title
			if (Logic_IsExistFlag(pTitleIndexFlag, pTblData->tblidx - 1, -1))
			{
				pTitle->bRegistered = true;
			}
			else
			{
				pTitle->bRegistered = false;
			}

			// add title
			m_InfoList.AddItem(pTitleTextTable->GetText(pTblData->tblNameIndex).c_str(), pTitle);

			if (pTitle->bRegistered)
			{
				if (pTblData->tblidx == pAvatarInfo->sCharPf.charTitle)
				{
					m_uiActiveCharTitleIndex = pTblData->tblidx;
				}
			}

			// set effect text
			for (BYTE i = 0; i < NTL_MAX_CHAR_TITLE_EFFECT; i++)
			{
				if (pTblData->atblSystem_Effect_Index[i] == INVALID_TBLIDX)
					break;

				if(Logic_GetSystemEffectText(pTblData->atblSystem_Effect_Index[i], (float)pTblData->abySystem_Effect_Value[i], pTitle->wstrEffect[i], pTblData->abySystem_Effect_Type[i]) == FALSE)
				{
					DBO_WARNING_MESSAGE("Couldnt read system effect text for title id: " << pTblData->tblidx);
				}
				
				pTitle->nEffectCount += 1;
			}
		}
	}

	UpdateTitleInfo();
}

void CPlayerTitleGui::SelectCharTitle(unsigned int uiIndex)
{
	m_uiActiveCharTitleIndex = uiIndex;

	if (m_uiActiveCharTitleIndex == m_uiSelectedTitleIndex)
		m_pApplyButton->SetDown(true);
}

void CPlayerTitleGui::UpdateCharTitle(unsigned int uiIndex, bool bDelete)
{
	if (m_uiActiveCharTitleIndex == uiIndex && bDelete)
	{
		if (m_uiActiveCharTitleIndex == m_uiSelectedTitleIndex)
			m_pApplyButton->SetDown(false);

		m_uiActiveCharTitleIndex = INVALID_TBLIDX;
	}

	for (std::vector<sTITLES*>::iterator it = m_vecTitles.begin(); it != m_vecTitles.end(); it++)
	{
		sTITLES* pTitle = *it;
		if (pTitle->titleIdx == uiIndex)
		{
			if(bDelete)
				pTitle->bRegistered = false;
			else
				pTitle->bRegistered = true;

			UpdateTitleInfo(uiIndex);
			break;
		}
	}
}

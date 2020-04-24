#include "precomp_dboclient.h"
#include "PortalGui.h"
#include "NtlDebug.h"

// PL
#include "NtlPLGuiManager.h"

// sound
#include "GUISoundDefine.h"


// Simul
#include "NtlSLApi.h"
#include "TableContainer.h"
#include "PortalTable.h"
#include "NtlSobNpc.h"
#include "NtlSobNpcAttr.h"
#include "NtlSobManager.h"
#include "NtlSLEventFunc.h"
#include "NtlCameraController.h"
#include "NtlSlGlobal.h"
#include "NtlSobAvatar.h"
#include "NtlResultCode.h"
#include "NPCTable.h"
#include "NtlSobProxy.h"


// DBO
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DboEventHandler.h"
#include "DialogManager.h"
#include "gui_texturemanager.h"
#include "InfoWndManager.h"
#include "DboEventGenerator.h"
#include "DisplayStringManager.h"
#include "MsgBoxManager.h"



CPortalGui::CPortalGui( const RwChar* pName ) 
:CNtlPLGui(pName)
{
	m_pcbbSearchWorld = NULL;
	m_ppnlStartPoint = NULL;
	m_ppnlTownPoint = NULL;
	m_ppnlCampPoint = NULL;
}
CPortalGui::~CPortalGui() 
{

}

VOID CPortalGui::Init() 
{
    m_nCurrIdx = 0;
    m_nSelectIdx = 0;
    m_hNPCSerialID = INVALID_SERIAL_ID;
    m_bTeleportSuccess = FALSE;
	m_hNPCPortalAddSerialID = INVALID_SERIAL_ID;
}

RwBool CPortalGui::Create() 
{
    if(!CNtlPLGui::Create("gui\\Portal.rsr","gui\\Portal.srf", "gui\\Portal.frm"))
        NTL_RETURN(FALSE);

    Init();    

    CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

    m_pThis = (gui::CDialog*)GetComponent("dlgMain");
    m_pBtnExit = (gui::CButton*)GetComponent("btnExit");

	m_srfPortalMap[E_PORTAL_SEARCH_WORLD] = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Portal.srf", "srfPortalMap");
	m_srfPortalMap[E_PORTAL_SEARCH_FEARLAND] = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("Portal.srf", "srfFearLandPortalMap");

	m_pSttStartPoint = (gui::CStaticBox*)GetComponent("sttStartPoint");
	m_pSttStartPoint->SetText(GetDisplayStringManager()->GetString("DST_PORTAL_ICON_START"));

    m_pSttEnablePoint = (gui::CStaticBox*)GetComponent("sttEnablePoint");  
    m_pSttEnablePoint->SetText(GetDisplayStringManager()->GetString("DST_PORTAL_ICON_ENABLE"));    


	m_ppnlStartPoint = (gui::CPanel*)GetComponent("pnlStartPoint");
	m_ppnlTownPoint = (gui::CPanel*)GetComponent("pnlTownPoint");
	m_ppnlCampPoint = (gui::CPanel*)GetComponent("pnlCampPoint");


	m_pcbbSearchWorld = (gui::CComboBox*)GetComponent("cbbSearchWorld");
	m_pcbbSearchWorld->AddItem(GetDisplayStringManager()->GetString("DST_PORTAL_MAINWORLD"));
	m_pcbbSearchWorld->AddItem(GetDisplayStringManager()->GetString("DST_PORTAL_FEARLAND"));
	m_pcbbSearchWorld->SelectItem(0);

	m_slotSearchWorldSelect = m_pcbbSearchWorld->SigSelected().Connect(this, &CPortalGui::OnSearchWorldSelect);
	m_slotSearchWorldToggled = m_pcbbSearchWorld->SigListToggled().Connect(this, &CPortalGui::OnSearchWorldToggled);


    m_slotMouseMove = m_pThis->SigMouseMove().Connect(this, &CPortalGui::OnMouseMove);
    m_slotMouseUp = m_pThis->SigMouseUp().Connect(this, &CPortalGui::OnMouseUp);    
    m_slotPaint   = m_pThis->SigPaint().Connect(this, &CPortalGui::OnPaint);

    m_slotBtnExit = m_pBtnExit->SigClicked().Connect(this, &CPortalGui::OnClickExitBtn);

	LinkMsg(g_EventNPCDialogOpen);
	LinkMsg(g_EventPortalAddReq);
	LinkMsg(g_EventPortalAddRes);
	LinkMsg(g_EventPortalRes);
	LinkMsg(g_EventDialog);
	LinkMsg(g_EventPortalInfoRes);

    // Set the location information of the portals.
    SetupPortalInfo();

	m_pThis->SetPriority(dDIALOGPRIORITY_PORTAL);

    Show(false);

    return TRUE;
}

VOID CPortalGui::Destroy() 
{
	UnLinkMsg(g_EventNPCDialogOpen);
	UnLinkMsg(g_EventPortalAddReq);
	UnLinkMsg(g_EventPortalAddRes);
	UnLinkMsg(g_EventPortalRes);
	UnLinkMsg(g_EventDialog);
	UnLinkMsg(g_EventPortalInfoRes);

    for each(PAIRPortalInfo pair in m_mapPortalInfo)
    {
        NTL_DELETE(pair.second);
    }
    m_mapPortalInfo.clear();

    CNtlPLGui::DestroyComponents();
    CNtlPLGui::Destroy();
}


RwInt32 CPortalGui::SwitchDialog( bool bOpen ) 
{
    Show(bOpen);

    if(bOpen)
    {
        UpdatePortalInfo();
        m_bTeleportSuccess = FALSE;                     // Flag to determine whether it will be closed with a close button when closed or closed with a portal
    }
    else
    {
        if(GetInfoWndManager()->GetRequestGui() == DIALOG_PORTAL)
        {
            GetInfoWndManager()->ShowInfoWindow(FALSE);
        }    

        if(!m_bTeleportSuccess)     // Do not remove the camera when closing it with the portal.
        {
			Logic_CancelNpcFacing();
        }        

		GetMsgBoxManager()->DeleteMsgBox( "DST_PORTAL_CONFIRM" );
    }

    return TRUE;
}

VOID CPortalGui::HandleEvents(RWS::CMsg &pMsg)
{
    if(pMsg.Id == g_EventNPCDialogOpen)     // Dialog Open
    {
        SDboNpcDialogOpen* pData = reinterpret_cast<SDboNpcDialogOpen*>(pMsg.pData);

        if(pData->eDialog != DIALOG_PORTAL)
            return;

        if(GetDialogManager()->IsOpenDialog(DIALOG_PORTAL))
            return;

		m_hNPCSerialID = pData->hSerialId;

		SetCurrentPortalIndex(m_hNPCSerialID);

		GetDialogManager()->OpenDialog(DIALOG_PORTAL, m_hNPCSerialID);
    }
    else if(pMsg.Id == g_EventDialog)       // Processing for Dialog Fail
    {
        SDboEventDialog* pData = reinterpret_cast<SDboEventDialog*>(pMsg.pData);
        if(pData->iDestDialog == DIALOG_PORTAL)
        {
            if(pData->iType == DIALOGEVENT_OPEN_FAIL_NPC_DIALOG)
            {
				Logic_CancelNpcFacing();

                GetDialogManager()->CloseDialog(DIALOG_PORTAL);
            }
			else if( pData->iType == DIALOGEVENT_NPC_BYEBYE )
			{
				GetDialogManager()->CloseDialog(DIALOG_PORTAL);
			}
        }
    }
    //else if(pMsg.Id == g_EventPortalInfoRes)    // load portal info
    //{
    //    SNtlEventPortalInfoRes* pData = reinterpret_cast<SNtlEventPortalInfoRes*>(pMsg.pData);                    
    //   

    //}
	else if (pMsg.Id == g_EventPortalAddReq)
	{
		SNtlEventPortalAddReq* pData = reinterpret_cast<SNtlEventPortalAddReq*>(pMsg.pData);

		CNtlSobNpc* pNPC = (CNtlSobNpc*)GetNtlSobManager()->GetSobObject(pData->hNPCSerailID);
		if (!pNPC)
			return;

		CNtlSobNpcAttr* pNPCAttr = (CNtlSobNpcAttr*)pNPC->GetSobAttr();

		m_hNPCPortalAddSerialID = pData->hNPCSerailID;

		if (HasPortalRegistered(pNPCAttr->GetNpcTbl()->contentsTblidx) == false)
		{
			GetDboGlobal()->GetGamePacketGenerator()->SendPortalAddReq(pData->hNPCSerailID);
		}
	}
    else if(pMsg.Id == g_EventPortalAddRes)     // Responding to new portal registration
    {
		SNtlEventPortalAddRes* pData = reinterpret_cast<SNtlEventPortalAddRes*>(pMsg.pData);

        if(m_mapPortalInfo.find(pData->portalID) != m_mapPortalInfo.end())
        {
			SPortalInfo* pInfo = m_mapPortalInfo[pData->portalID];
			pInfo->m_bRegist = TRUE;

			if (pInfo->m_bCamp)
			{
				pInfo->m_eIconType = E_PORTAL_ICON_CAMP_ENABLE;
			}
			else
			{
				pInfo->m_eIconType = E_PORTAL_ICON_TOWN_ENABLE;
			}

			GetAlarmManager()->AlarmMessage("DST_PORTAL_ADD_NOTIFY");

			//play effect TO DO
			CNtlSobNpc* pNPC = (CNtlSobNpc*)GetNtlSobManager()->GetSobObject(m_hNPCPortalAddSerialID);
			if (!pNPC)
				return;

			CNtlSobProxy *pSobProxy = pNPC->GetSobProxy();
			RwV3d vOffset;
			CNtlMath::MathRwV3dAssign(&vOffset, 0.0f, 0.0f, 0.0f);
			pSobProxy->CreatePLChildEffect("GME_PORTAL_NPC_001", vOffset);
        }
    }
    else if(pMsg.Id == g_EventPortalRes)        // Responding to Actual Portal Requests
    {
        SNtlEventPortalRes* pData = reinterpret_cast<SNtlEventPortalRes*>(pMsg.pData);

		m_bTeleportSuccess = TRUE;
		GetDialogManager()->CloseDialog(DIALOG_PORTAL);
    }
}

VOID CPortalGui::SetupPortalInfo() 
{
    // Place portal icons at the set location.
    m_mapPortalInfo.clear();    

    CPortalTable* pPortalTable = (CPortalTable*)API_GetTableContainer()->GetPortalTable();    
    if(!pPortalTable)
        return;

    sPORTAL_TBLDAT* pPortalData = NULL;
    for(CTable::TABLEIT it = pPortalTable->Begin(); it != pPortalTable->End(); ++it)
    {
		
        pPortalData = (sPORTAL_TBLDAT*)(*it).second;
        if(pPortalData)
        {
            SPortalInfo* pPortalInfo = NTL_NEW SPortalInfo();
            pPortalInfo->m_nIndex = pPortalData->tblidx;
			pPortalInfo->worldId = pPortalData->worldId;

			// check if avatar has portal registered
			if (HasPortalRegistered((BYTE)pPortalData->tblidx) == false)
			{
				pPortalInfo->m_bRegist = FALSE;
				pPortalInfo->m_eIconType = MAX_ICON_TYPE_NOT;
			}
			else
			{
				pPortalInfo->m_bRegist = TRUE;

				// check wether is camp or town
				if (pPortalData->byGrade == E_PORTAL_GRADE_CAMP)
				{
					pPortalInfo->m_eIconType = E_PORTAL_ICON_CAMP_ENABLE;
					pPortalInfo->m_bCamp = true;
				}
				else
				{
					pPortalInfo->m_eIconType = E_PORTAL_ICON_TOWN_ENABLE;
					pPortalInfo->m_bCamp = false;
				}
			}

            m_mapPortalInfo[pPortalInfo->m_nIndex] = pPortalInfo;
		}
    }

    // Create surfaces to be used for ICON. (Surface when index + 1 is active)  
	m_aSurIcon[E_PORTAL_ICON_START_POINT]     = CNtlPLGuiManager::GetInstance()->GetSurfaceManager()->GetSurface("Portal.srf", "srfIconStartPoint");
	m_aSurIcon[E_PORTAL_ICON_START_POINT_FOCUS] = CNtlPLGuiManager::GetInstance()->GetSurfaceManager()->GetSurface("Portal.srf", "srfIconStartPointFoc");

	m_aSurIcon[E_PORTAL_ICON_CAMP_ENABLE]          = CNtlPLGuiManager::GetInstance()->GetSurfaceManager()->GetSurface("Portal.srf", "srfIconCampPoint");
	m_aSurIcon[E_PORTAL_ICON_CAMP_ENABLE_FOCUS]    = CNtlPLGuiManager::GetInstance()->GetSurfaceManager()->GetSurface("Portal.srf", "srfIconCampPointFoc");

	m_aSurIcon[E_PORTAL_ICON_TOWN_ENABLE] = CNtlPLGuiManager::GetInstance()->GetSurfaceManager()->GetSurface("Portal.srf", "srfIconTownPoint");
	m_aSurIcon[E_PORTAL_ICON_TOWN_ENABLE_FOCUS] = CNtlPLGuiManager::GetInstance()->GetSurfaceManager()->GetSurface("Portal.srf", "srfIconTownPointFoc");
}


VOID CPortalGui::UpdatePortalInfo() 
{
    sPORTAL_TBLDAT* pPortalData = (sPORTAL_TBLDAT*)API_GetTableContainer()->GetPortalTable()->FindData(m_nCurrIdx);
    if(!pPortalData)
        return;

	//show map surface
	// check if fear land
	if (pPortalData->worldId == FEARLAND_ID)
	{
		m_nCurrentWorldSearch = E_PORTAL_SEARCH_FEARLAND;
		SetWorldMapSurface();

		m_pcbbSearchWorld->SelectItem(E_PORTAL_SEARCH_FEARLAND);
	}
	else
	{
		m_nCurrentWorldSearch = E_PORTAL_SEARCH_WORLD;
		SetWorldMapSurface();

		m_pcbbSearchWorld->SelectItem(E_PORTAL_SEARCH_WORLD);
	}

	// Update the information of portal icons. (To match your current position)

	// 0. Verify that the portal is currently selected.
	// 1. Make sure it is a registered portal.
	// 2. Check if the portal is connected to the current point.

	UpdatePortalIcon();
}

// Displays a tooltip when the mouse hovers over the icon.
VOID CPortalGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
    nX += m_pThis->GetPosition().left;
    nY += m_pThis->GetPosition().top;

    CRectangle rect;
    SPortalInfo* pSelectPortal = NULL;

    for each(PAIRPortalInfo pair in m_mapPortalInfo)    
    {
		if(pair.second->m_eIconType == MAX_ICON_TYPE_NOT)
            continue;

        pair.second->m_surIcon.GetRect(rect);

        if(!pSelectPortal && rect.PtInRect(nX, nY))
        {
            sPORTAL_TBLDAT* pPortalData = (sPORTAL_TBLDAT*)API_GetTableContainer()->GetPortalTable()->FindData(pair.first);
            if(!pPortalData)
                return;           

            pSelectPortal = pair.second;

            if(m_nSelectIdx != pair.second->m_nIndex)
            {
                m_nSelectIdx = pair.second->m_nIndex;
                
                // Change the surface.                          
                if(pair.second->m_eIconType == E_PORTAL_ICON_START_POINT ||
                   pair.second->m_eIconType == E_PORTAL_ICON_CAMP_ENABLE ||
					pair.second->m_eIconType == E_PORTAL_ICON_TOWN_ENABLE)
                {
                    pair.second->m_eIconType = (EPortalIconType)(pair.second->m_eIconType + 1);
                    pair.second->m_surIcon.SetSurface(m_aSurIcon[pair.second->m_eIconType]);
                    pair.second->m_surIcon.SetRect(rect);

					Logic_PlayGUISound( GSD_SYSTEM_POTAL_ICON_MOUSE_OVER );			
                }                                    
            }    

            // Pass it to the tooltip object as a structure. (There is only one argument.)
            SPortalTooltip sPortalToolTip;
            sPortalToolTip.nPortalID = pair.second->m_nIndex;
            sPortalToolTip.ePortalIconType = (RwInt32)pair.second->m_eIconType;
            sPortalToolTip.nZenny = pair.second->m_nZenny;
            GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_PORTAL, rect.left, rect.top, (VOID*)&sPortalToolTip, DIALOG_PORTAL);            	
        }        
        else
        {
            // If there is an existing icon whose surface has been changed, it returns it           
            if(pair.second->m_eIconType == E_PORTAL_ICON_START_POINT_FOCUS ||
               pair.second->m_eIconType == E_PORTAL_ICON_CAMP_ENABLE_FOCUS ||
				pair.second->m_eIconType == E_PORTAL_ICON_TOWN_ENABLE_FOCUS)
            {
                pair.second->m_eIconType = (EPortalIconType)(pair.second->m_eIconType - 1);;
                pair.second->m_surIcon.SetSurface(m_aSurIcon[pair.second->m_eIconType]);                    
                pair.second->m_surIcon.SetRect(rect);
            }                            
        }
    }

    if(!pSelectPortal && GetInfoWndManager()->GetRequestGui() == DIALOG_PORTAL)
    {
        m_nSelectIdx = NULL;
        GetInfoWndManager()->ShowInfoWindow(FALSE);
    }    
}

// Called when you click a point on the map.
VOID CPortalGui::OnMouseUp( const CKey& key ) 
{
    if(m_nSelectIdx <= 0 || m_nSelectIdx == m_nCurrIdx || 
       m_mapPortalInfo[m_nSelectIdx]->m_eIconType != E_PORTAL_ICON_CAMP_ENABLE_FOCUS &&
		m_mapPortalInfo[m_nSelectIdx]->m_eIconType != E_PORTAL_ICON_TOWN_ENABLE_FOCUS)
        return;

    RwInt32 nPoint = FindPointIndex(m_nCurrIdx, m_nSelectIdx);
    if(nPoint < 0)
        return;

    SPortalInfo* pInfo = m_mapPortalInfo[m_nSelectIdx];
    
    sMsgBoxData data;
    data.hHandle = m_hNPCSerialID;
    data.byIndex = (RwUInt8)nPoint;

    // Get the name.
    sPORTAL_TBLDAT* pPortalData = (sPORTAL_TBLDAT*)API_GetTableContainer()->GetPortalTable()->FindData(pInfo->m_nIndex);
    if(!pPortalData)
        return;

    std::wstring wstrMapName = API_GetTableContainer()->GetTextAllTable()->GetMapNameTbl()->GetText(pPortalData->dwPointName);                    
    
    GetInfoWndManager()->ShowInfoWindow(FALSE);
    GetAlarmManager()->FormattedAlarmMessage("DST_PORTAL_CONFIRM", FALSE, &data, Logic_FormatZeni(pInfo->m_nZenny), wstrMapName.c_str());
    
}

VOID CPortalGui::OnPaint()
{
    for each(PAIRPortalInfo pair in m_mapPortalInfo)    
    {
		if (pair.second->m_eIconType != MAX_ICON_TYPE_NOT)
			pair.second->m_surIcon.Render();
    }
}

/// Check whether the current portal location is registered, and register if it is not registered.
/// Returns True if it is already registered, False if it is not registered yet
RwBool CPortalGui::CheckRegist( RwUInt32 hNPCSerialID ) 
{
	// 1. Get the current Portal Index from the NPC table.
	CNtlSobNpc* pNPC = (CNtlSobNpc*)GetNtlSobManager()->GetSobObject(hNPCSerialID);
	if(!pNPC)
		return FALSE;

	CNtlSobNpcAttr* pNPCAttr = (CNtlSobNpcAttr*)pNPC->GetSobAttr();

	// Get the portal ID of the NPC from the NPC table.
	m_nCurrIdx = pNPCAttr->GetNpcTbl()->contentsTblidx;

	// 2. Make sure that it is registered on the server.
	if(IsRegist(m_nCurrIdx))
	{
		return TRUE;         
	}
}

VOID CPortalGui::OnClickExitBtn( gui::CComponent* pComponent ) 
{
    GetDialogManager()->CloseDialog(DIALOG_PORTAL);
}

VOID CPortalGui::OnSearchWorldSelect(INT nIndex)
{
	bool bUpdate = true;

	if (nIndex == E_PORTAL_SEARCH_WORLD && m_nCurrentWorldSearch != E_PORTAL_SEARCH_WORLD) // select srfPortalMap
	{
		m_nCurrentWorldSearch = E_PORTAL_SEARCH_WORLD;
	}
	else if (nIndex == E_PORTAL_SEARCH_FEARLAND && m_nCurrentWorldSearch != E_PORTAL_SEARCH_FEARLAND) // select srfFearLandPortalMap
	{
		m_nCurrentWorldSearch = E_PORTAL_SEARCH_FEARLAND;
	}
	else
	{
		bUpdate = false;
	}

	if (bUpdate == false)
		return;

	// update map
	SetWorldMapSurface();

	// update portal icons
	UpdatePortalIcon();
}

VOID CPortalGui::OnSearchWorldToggled(RwBool bToggled, gui::CComponent * pComponent)
{
	if (bToggled)
	{
		pComponent->Raise();
	}
	else
	{
		
	}
}

RwBool CPortalGui::IsRegist( RwUInt32 nPortalID ) 
{
    if(m_mapPortalInfo.find(nPortalID) == m_mapPortalInfo.end())
        return FALSE;

    return m_mapPortalInfo[nPortalID]->m_bRegist;
}

RwBool CPortalGui::IsConnect(RwUInt32 nPortalID, DWORD& nZenny)
{
    sPORTAL_TBLDAT* pPortalData = (sPORTAL_TBLDAT*)API_GetTableContainer()->GetPortalTable()->FindData(m_nCurrIdx);

    for(int i = 0; i < DBO_MAX_POINT_PORTAL; ++i)
    {
        if(nPortalID == pPortalData->aPoint[i])
        {
            nZenny = pPortalData->adwPointZenny[i];
            return TRUE;        
        }
    }

    return FALSE;
}

// 현재 포탈 테이블에서 타겟 포탈 포인트의 인덱스를 찾아서 반환한다.
// Return : 0 ~ 7 사이의 인덱스 번호, 찾지 못한 경우에는 -1을 리턴
RwInt32 CPortalGui::FindPointIndex( RwUInt32 nCurrPortalID, RwUInt32 nTargetPortalID ) 
{
    sPORTAL_TBLDAT* pPortalData = (sPORTAL_TBLDAT*)API_GetTableContainer()->GetPortalTable()->FindData(nCurrPortalID);
    if(!pPortalData)
        return -1;

    for(int i = 0; i < DBO_MAX_POINT_PORTAL; ++i)
    {
        if(nTargetPortalID == pPortalData->aPoint[i])
            return i;
    }

    return -1;
}

void CPortalGui::SetCurrentPortalIndex(RwUInt32 hNPCSerialID)
{
	// 1. Get the current Portal Index from the NPC table.
	CNtlSobNpc* pNPC = (CNtlSobNpc*)GetNtlSobManager()->GetSobObject(hNPCSerialID);
	if (!pNPC)
		return;

	CNtlSobNpcAttr* pNPCAttr = (CNtlSobNpcAttr*)pNPC->GetSobAttr();

	// Get the portal ID of the NPC from the NPC table.
	m_nCurrIdx = pNPCAttr->GetNpcTbl()->contentsTblidx;
}

void CPortalGui::SetWorldMapSurface()
{
	// remove current surface
	m_pThis->GetSurface()->clear();


	// add new surface
	m_pThis->AddSurface(m_srfPortalMap[m_nCurrentWorldSearch]);
}

bool CPortalGui::HasPortalRegistered(BYTE portalid)
{
	SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();

	for (int i = 0; i < NTL_PORTAL_MAX_COUNT; i++)
	{
		if (pAvatarInfo->aPortalID[i] == portalid)
		{
			return true;
		}
		else if (pAvatarInfo->aPortalID[i] == INVALID_BYTE)
		{
			return false;
		}
	}

	return false;
}

void CPortalGui::UpdatePortalIcon()
{
	sPORTAL_TBLDAT* pPortalData = NULL;

	for each(PAIRPortalInfo pair in m_mapPortalInfo)
	{
		SPortalInfo* pPortalInfo = pair.second;
		if (!pPortalInfo)
			continue;

		bool bSetRect = false;

		pPortalInfo->m_nZenny = 0;
		pPortalInfo->m_eIconType = MAX_ICON_TYPE_NOT;

		if (IsRegist(pPortalInfo->m_nIndex))
		{
			// check if current selected point
			if ((m_nCurrentWorldSearch == E_PORTAL_SEARCH_WORLD && pPortalInfo->worldId != FEARLAND_ID) ||
				(m_nCurrentWorldSearch == E_PORTAL_SEARCH_FEARLAND && pPortalInfo->worldId == FEARLAND_ID))
			{
				if (pPortalInfo->m_nIndex == m_nCurrIdx)     // Current point
				{
					pPortalInfo->m_eIconType = E_PORTAL_ICON_START_POINT;

					// Set the icon.
					pPortalInfo->m_surIcon.SetSurface(m_aSurIcon[pPortalInfo->m_eIconType]);
					bSetRect = true;
				}
			}

			if (bSetRect == false && IsConnect(pPortalInfo->m_nIndex, pPortalInfo->m_nZenny))
			{
				if (m_nCurrentWorldSearch == E_PORTAL_SEARCH_WORLD)
				{
					if (pPortalInfo->worldId != FEARLAND_ID) // if portal info is not from fear land
					{
						if (pPortalInfo->m_nIndex == m_nCurrIdx)     // Current point
						{
							pPortalInfo->m_eIconType = E_PORTAL_ICON_START_POINT;
						}
						else if (pPortalInfo->m_bCamp == true) // check wether is camp or town
						{
							pPortalInfo->m_eIconType = E_PORTAL_ICON_CAMP_ENABLE;
						}
						else
						{
							pPortalInfo->m_eIconType = E_PORTAL_ICON_TOWN_ENABLE;
						}

						// Set the icon.
						pPortalInfo->m_surIcon.SetSurface(m_aSurIcon[pPortalInfo->m_eIconType]);
						bSetRect = true;
					}
				}
				else if (m_nCurrentWorldSearch == E_PORTAL_SEARCH_FEARLAND) // if we switched to fear map
				{
					if (pPortalInfo->worldId == FEARLAND_ID) // if portal info is from fear land
					{
						if (pPortalInfo->m_nIndex == m_nCurrIdx)     // Current point
						{
							pPortalInfo->m_eIconType = E_PORTAL_ICON_START_POINT;
						}
						else if (pPortalInfo->m_bCamp == true)	// check wether is camp or town
						{
							pPortalInfo->m_eIconType = E_PORTAL_ICON_CAMP_ENABLE;
						}
						else
						{
							pPortalInfo->m_eIconType = E_PORTAL_ICON_TOWN_ENABLE;
						}

						// Set the icon.
						pPortalInfo->m_surIcon.SetSurface(m_aSurIcon[pPortalInfo->m_eIconType]);
						bSetRect = true;
					}
				}
				else
				{
					// rip
				}
			}


			if (bSetRect)
			{
				pPortalData = (sPORTAL_TBLDAT*)API_GetTableContainer()->GetPortalTable()->FindData(pPortalInfo->m_nIndex);
				if (pPortalData)
				{
					pPortalInfo->m_surIcon.SetRectWH(m_pThis->GetPosition().left + (RwInt32)pPortalData->vMap.x - (RwInt32)(pair.second->m_surIcon.GetWidth() * 0.5f),
						m_pThis->GetPosition().top + (RwInt32)pPortalData->vMap.z - (RwInt32)(pair.second->m_surIcon.GetHeight() * 0.5f),
						pair.second->m_surIcon.GetWidth(),
						pair.second->m_surIcon.GetHeight());
				}
			}

		}
	}
}

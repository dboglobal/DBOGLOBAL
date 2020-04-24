#include "precomp_dboclient.h"
#include "PetSkillWindowGui.h"

// core
#include "NtlDebug.h"

// PL
#include "NtlPlDef.h"
#include "NtlPLGuiManager.h"

//Simulation
#include "NtlSLApi.h"
#include "NtlSLEvent.h"
#include "NtlSLLogic.h"
#include "NtlSobPet.h"
#include "NtlSobManager.h"
#include "NtlSLPacketGenerator.h"
#include "NtlSLEventFunc.h"
#include "NtlSobIcon.h"
#include "NtlSobSkill.h"

// dbo
#include "DialogManager.h"
#include "DboEvent.h"
#include "DboGlobal.h"
#include "InfoWndManager.h"
#include "DisplayStringManager.h"

#define PET_SLOT_TOOLTIP_OFFSET_X   5
#define PET_SLOT_TOOLTIP_OFFSET_Y   -5

CPetSkillWindowGui* CPetSkillWindowGui::m_pInstance = NULL;

CPetSkillWindowGui::CPetSkillWindowGui()
{
    Init();
}

CPetSkillWindowGui::CPetSkillWindowGui( const RwChar* pName )
: CNtlPLGui(pName)
{
    Init();
}

CPetSkillWindowGui::~CPetSkillWindowGui()
{
    Destroy();
}

void CPetSkillWindowGui::Init()
{
    m_uiSummonPetID = INVALID_SERIAL_ID;
    m_uiItemPetID   = INVALID_SERIAL_ID;
    m_bVisibeFocus  = FALSE;
    m_uiInfoFlag    = NULL;

    for(RwInt32 i = 0; i < SUMMON_PET_MAX_SKILL; ++i)
    {
        m_pSobSkillIcon[i] = NULL;
    }
}

RwBool CPetSkillWindowGui::Create()
{
    if(!CNtlPLGui::Create("", "gui\\PetSkillBar.srf", "gui\\PetSkillBar.frm"))
        return FALSE;

    CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

    m_pThis              = (gui::CDialog*)GetComponent("dlgPetSlot");
    m_pPnlSummonPetBack  = (gui::CPanel*)GetComponent("pnlSummonPetBack");
    m_pPnlItemPetBack    = (gui::CPanel*)GetComponent("pnlItemPetBack");
    m_pPnlTransformCancelBack = (gui::CPanel*)GetComponent("pnlTransformCancelBack");
    m_pBtnSummonClose    = (gui::CButton*)GetComponent("btnSummonClose");
    m_pBtnItemClose      = (gui::CButton*)GetComponent("btnItemClose");
    m_pBtnSummonAttack   = (gui::CButton*)GetComponent("btnSummonAttack");
    m_pBtnTransformCancel = (gui::CButton*)GetComponent("btnTransformCancel");

    m_slotPaint          = m_pBtnSummonAttack->SigPaint().Connect(this, &CPetSkillWindowGui::OnPaint);    
    m_slotClickCloseBtn1 = m_pBtnSummonClose->SigClicked().Connect(this, &CPetSkillWindowGui::OnClickSummonCloseBtn);
    m_slotClickCloseBtn2 = m_pBtnItemClose->SigClicked().Connect(this, &CPetSkillWindowGui::OnClickItemCloseBtn);
    m_slotClickTransformCancel = m_pBtnTransformCancel->SigClicked().Connect(this, &CPetSkillWindowGui::OnClickTransformCancel);
    m_slotClickSummonAttack = m_pBtnSummonAttack->SigMouseUp().Connect(this, &CPetSkillWindowGui::OnClickSummonAttack);
    m_slotMouseDownSummonAttack = m_pBtnSummonAttack->SigMouseDown().Connect(this, &CPetSkillWindowGui::OnMouseDownSummonAttack);
    m_slotMouseEnter = m_pBtnSummonAttack->SigMouseEnter().Connect(this, &CPetSkillWindowGui::OnMouseEnter);
	m_slotMove	= m_pThis->SigMove().Connect( this, &CPetSkillWindowGui::OnMove );
    m_slotMouseLeave = m_pBtnSummonAttack->SigMouseLeave().Connect(this, &CPetSkillWindowGui::OnMouseLeave);
    m_slotMouseMove  = m_pThis->SigMouseMove().Connect(this, &CPetSkillWindowGui::OnMouseMove);
    m_slotMouseDialogLeave = m_pThis->SigMouseLeave().Connect(this, &CPetSkillWindowGui::OnMouseDialogLeave);
    
    // Surface 설정
    m_surFocus.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotFocusEffect"));
    for(RwInt32 i = 0; i < SUMMON_PET_MAX_SKILL; ++i)
    {
        m_CoolTimeEffect[i].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotGrayedEffect" ));
    }

    m_surAttackBtnUp = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("PetSkillBar.srf", "srfSummonPetAttackBtn");
    m_surAttackBtnDown = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("PetSkillBar.srf", "srfSummonPetAttackBtnDown");
    m_surTransformNamek = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("PetSkillBar.srf", "srfTransformCancelNamek");
    m_surTransformSaiya = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("PetSkillBar.srf", "srfTransformCancelSaiya");
    m_surTransformMajin = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("PetSkillBar.srf", "srfTransformCancelSaiya");

    LinkMsg(g_EventSobInfoUpdate);
    LinkMsg(g_EventSummonPet);            

    // Update Link
    GetNtlGuiManager()->AddUpdateBeforeFunc( this );
    GetNtlGuiManager()->AddUpdateFunc( this );

    Show( false );

    return TRUE;
}

void CPetSkillWindowGui::Destroy()
{
    UnLinkMsg(g_EventSobInfoUpdate);
    UnLinkMsg(g_EventSummonPet);    

    // Update Unlink
    GetNtlGuiManager()->RemoveUpdateBeforeFunc( this );
    GetNtlGuiManager()->RemoveUpdateFunc( this );

    CNtlPLGui::DestroyComponents();
    CNtlPLGui::Destroy();     
}

void CPetSkillWindowGui::Update( RwReal fElapsed )
{
    UpdateSkillIcon(fElapsed);
}

RwInt32 CPetSkillWindowGui::SwitchDialog( bool bOpen )
{
	Show( bOpen );

    if(bOpen)
    {
        AdjustDialog();
		UpdatePetInfo();
    }

    return TRUE;
}

void CPetSkillWindowGui::HandleEvents( RWS::CMsg &pMsg )
{
    if(pMsg.Id == g_EventSobInfoUpdate)    // Pet의 정보가 Update 되었을때
    {

    }
    else if(pMsg.Id == g_EventSummonPet)   // Pet이 죽었을때
    {
        SDboEventSummonPet* pData = reinterpret_cast<SDboEventSummonPet*>(pMsg.pData);
        if(!pData->bCreate)
        {
            if(pData->uiSerialID == m_uiSummonPetID)
            {
                m_uiSummonPetID = INVALID_SERIAL_ID;
                m_uiInfoFlag &= ~E_PET_TYPE_SUMMON;
            }
            else if(pData->uiSerialID == m_uiItemPetID)
            {
                m_uiItemPetID = INVALID_SERIAL_ID;
                m_uiInfoFlag &= ~E_PET_TYPE_ITEM;
            }

			for(RwInt32 i = 0; i < SUMMON_PET_MAX_SKILL; ++i)
			{
				m_pSobSkillIcon[i] = NULL;
			}

			if( m_uiInfoFlag )
			{
				AdjustDialog();
			}
			else
				GetDialogManager()->CloseDialog( DIALOG_PET_SKILL_SLOT );
        }
    }
    else if(pMsg.Id == g_EventTransform)
    {
        SNtlEventTransform* pData = reinterpret_cast<SNtlEventTransform*>(pMsg.pData);
        if(pData->hSerialId == GetNtlSLGlobal()->GetSobAvatar()->GetSerialID())
        {
            RwBool bTransForm = pData->sApsectState.sAspectStateBase.byAspectStateId != ASPECTSTATE_INVALID ? TRUE : FALSE;
            SetTransformCancel(bTransForm);		
        }

		if( m_uiInfoFlag )
		{
			AdjustDialog();
		}
		else
			GetDialogManager()->CloseDialog( DIALOG_PET_SKILL_SLOT );
    }    
}

void CPetSkillWindowGui::AdjustDialog()
{
    if(m_uiInfoFlag)
    {
        if(m_uiInfoFlag & E_PET_TYPE_SUMMON)
        {
            m_pPnlSummonPetBack->Show(TRUE);
            m_pBtnSummonClose->Show(TRUE);
            m_pBtnSummonAttack->Show(TRUE);
        }
        else
        {
            m_pPnlSummonPetBack->Show(FALSE);
            m_pBtnSummonClose->Show(FALSE);
            m_pBtnSummonAttack->Show(FALSE);
        }

        if(m_uiInfoFlag & E_TRANSFORM_CANCEL)
        {
            // 위치
            if(m_uiInfoFlag & E_PET_TYPE_SUMMON)
            {
                m_pPnlTransformCancelBack->SetPosition(179, 0);
                m_pBtnTransformCancel->SetPosition(179, 0);
            }
            else
            {
                m_pPnlTransformCancelBack->SetPosition(0, 0);
                m_pBtnTransformCancel->SetPosition(0, 0);
            }

            // 서페이스
            m_pBtnTransformCancel->GetSurfaceFocus()->clear();
            m_pBtnTransformCancel->GetSurfaceUp()->clear();
            m_pBtnTransformCancel->GetSurfaceDown()->clear();            
            switch(Logic_GetPlayerRace(GetNtlSLGlobal()->GetSobAvatar()))
            {
            case RACE_HUMAN:
                m_pBtnTransformCancel->AddSurfaceFocus(m_surTransformSaiya);
                m_pBtnTransformCancel->AddSurfaceUp(m_surTransformSaiya);
                m_pBtnTransformCancel->AddSurfaceDown(m_surTransformSaiya);
                break;
            case RACE_NAMEK:
                m_pBtnTransformCancel->AddSurfaceFocus(m_surTransformNamek);
                m_pBtnTransformCancel->AddSurfaceUp(m_surTransformNamek);
                m_pBtnTransformCancel->AddSurfaceDown(m_surTransformNamek);
                break;
            case RACE_MAJIN:
                m_pBtnTransformCancel->AddSurfaceFocus(m_surTransformMajin);
                m_pBtnTransformCancel->AddSurfaceUp(m_surTransformMajin);
                m_pBtnTransformCancel->AddSurfaceDown(m_surTransformMajin);
                break;
            }

            m_pPnlTransformCancelBack->Show(TRUE);
            m_pBtnTransformCancel->Show(TRUE);
        }
        else
        {
            m_pPnlTransformCancelBack->Show(FALSE);
            m_pBtnTransformCancel->Show(FALSE);
        }

        // NOTE: 아이템펫은 아직 없다.
        if(m_uiInfoFlag & E_PET_TYPE_ITEM)
        {
            m_pPnlItemPetBack->Show(TRUE);
            m_pBtnItemClose->Show(TRUE);
        }
        else
        {
            m_pPnlItemPetBack->Show(FALSE);
            m_pBtnItemClose->Show(FALSE);
        }
    }
}

VOID CPetSkillWindowGui::OnPaint( VOID )
{
    if(m_uiSummonPetID)
    {
        if(m_bVisibeFocus) 
            m_surFocus.Render();

        for(int i = 0; i < SUMMON_PET_MAX_SKILL; ++i)
        {
            if(m_surSummonSkill[i].GetTexture())
            {
                m_surSummonSkill[i].Render();
            }

            m_CoolTimeEffect[i].Render();
        }
    }
}

CPetSkillWindowGui* CPetSkillWindowGui::GetInstance()
{
    if(!m_pInstance)
    {
        m_pInstance = NTL_NEW CPetSkillWindowGui("Pet Skill Bar");
        if(!m_pInstance->Create())
        {
            NTL_DELETE(m_pInstance);
            return NULL;
        }        
    }

    return m_pInstance;
}

void CPetSkillWindowGui::DeleteInstance()
{
    NTL_DELETE(m_pInstance);
}

void CPetSkillWindowGui::SetPetID( RwUInt32 uiSerialID, EPetType ePetType )
{
    m_uiInfoFlag |= ePetType;

    if(ePetType == E_PET_TYPE_SUMMON)
    {
        m_uiSummonPetID = uiSerialID;
    }
    else if(ePetType == E_PET_TYPE_ITEM)
    {
        m_uiItemPetID = uiSerialID;
    }

	if( !GetDialogManager()->IsOpenDialog( DIALOG_PET_SKILL_SLOT ) )
		GetDialogManager()->OpenDialog( DIALOG_PET_SKILL_SLOT );
	else
	{
		AdjustDialog();
		UpdatePetInfo();
	}
}

void CPetSkillWindowGui::SetTransformCancel( RwBool bEnable ) 
{
    if(bEnable)
    {
        m_uiInfoFlag |= E_TRANSFORM_CANCEL;
		
		GetDialogManager()->OpenDialog( DIALOG_PET_SKILL_SLOT );
    }
    else
    {
        m_uiInfoFlag &= ~E_TRANSFORM_CANCEL;
    }
}

VOID CPetSkillWindowGui::OnClickSummonCloseBtn( gui::CComponent* pComponent )
{
    // 클로즈 버튼을 누르면 소환 취소 된다.    
    CNtlSobPet* pSobPet = reinterpret_cast<CNtlSobPet*>(GetNtlSobManager()->GetSobObject(m_uiSummonPetID));
    if(!pSobPet)
        return;

    API_GetSLPacketGenerator()->SendPetDismissReq(pSobPet->GetServerSyncAvatarType());

    m_uiSummonPetID = NULL;
    m_uiInfoFlag &= ~E_PET_TYPE_SUMMON;
    AdjustDialog();
}

VOID CPetSkillWindowGui::OnClickItemCloseBtn( gui::CComponent* pComponent )
{
    // 클로즈 버튼을 누르면 소환 취소 된다.
    CNtlSobPet* pSobPet = reinterpret_cast<CNtlSobPet*>(GetNtlSobManager()->GetSobObject(m_uiItemPetID));
    if(!pSobPet)
        return;

    API_GetSLPacketGenerator()->SendPetDismissReq(pSobPet->GetServerSyncAvatarType());

    m_uiItemPetID = NULL;
    m_uiInfoFlag &= ~E_PET_TYPE_ITEM;
    AdjustDialog();
}

void CPetSkillWindowGui::UpdatePetInfo()
{
    // 펫의 스킬정보를 가져와서 Skill Window에 표시한다. 
    SetSkillIcon();
}

VOID CPetSkillWindowGui::OnMouseDownSummonAttack(const CKey& key)
{
    // 버튼위에 마우스 우클릭시 버튼 다운 이미지 표시
    if(!m_uiSummonPetID)
        return;

    if(key.m_nID != UD_RIGHT_BUTTON)
        return;
    
    //m_pBtnSummonAttack->SetPosition(2, 2);            
    //m_pBtnSummonAttack->SetWidth(NTL_ITEM_ICON_SIZE - 4);
    //m_pBtnSummonAttack->SetHeight(NTL_ITEM_ICON_SIZE - 4);

    m_pBtnSummonAttack->GetSurfaceFocus()->clear();
    m_pBtnSummonAttack->AddSurfaceFocus(m_surAttackBtnDown);
    m_pBtnSummonAttack->CaptureMouse();
}

VOID CPetSkillWindowGui::OnClickSummonAttack(const CKey& key)
{
    if(!m_uiSummonPetID)
        return;

    // 마우스 우측 클릭시에만 공격하도록 한다.
    if(key.m_nID != UD_RIGHT_BUTTON)
        return;

    m_pBtnSummonAttack->GetSurfaceFocus()->clear();
    m_pBtnSummonAttack->AddSurfaceFocus(m_surAttackBtnUp);    
    m_pBtnSummonAttack->ReleaseMouse();

	// peessi: MouseCapture 상태에서 UI가 사라진 경우에, 콜백에 따른 작동을 막는다. 
	if( !m_pBtnSummonAttack->IsVisibleTruly() )
		return;

    //// 설정한 타겟을 펫이 공격하게 한다.  
	// 서버로 Pet을 옮기는 작업 중
    CNtlSobPet* pSobPet = reinterpret_cast<CNtlSobPet*>(GetNtlSobManager()->GetSobObject(m_uiSummonPetID));
    if(!pSobPet)
        return;

    SERIAL_HANDLE hTarget = Logic_GetAvatarTargetHandle();
    if(hTarget == INVALID_SERIAL_ID)
        return;

    if(!Logic_IsEnemyTagetFromPetActor(pSobPet, hTarget))   // 공격할수 있는 대상인지 확인
        return;

	API_GetSLPacketGenerator()->SendPetAttackTargetNfy( pSobPet->GetServerSyncAvatarType() );

    /*CNtlSLEventGenerator::SobPetBeginAttack(m_uiSummonPetID, hTarget);    */
}

VOID CPetSkillWindowGui::OnMouseEnter( gui::CComponent* pComponent )
{
    m_bVisibeFocus = TRUE;      // 아이콘 포커스를 그린다.

    // 툴팁을 뛰운다    
    RwInt32 nX = m_pThis->GetPosition().left;
    RwInt32 nY = m_pThis->GetPosition().top - 60;
    GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_JUST_WTEXT, nX, nY, (void*)GetDisplayStringManager()->GetString("DST_PET_TOOLTIP_ATTACK"), DIALOG_PET_SKILL_SLOT); 
}

VOID CPetSkillWindowGui::OnMove( RwInt32 iOldX, RwInt32 iOldY )
{
	CNtlPLGui* pQuickSlotGui = GetDialogManager()->GetDialog(DIALOG_QUICKSLOT);
	CRectangle rect = pQuickSlotGui->GetPosition();
	RwInt32 nPosX = rect.left;
	RwInt32 nPosY = rect.top - 38;

	m_surFocus.SetRectWH(nPosX, nPosY, NTL_ITEM_ICON_SIZE, NTL_ITEM_ICON_SIZE);        
	for(RwInt32 i = 0; i < SUMMON_PET_MAX_SKILL; ++i)
	{
		RwInt32 nX = nPosX + (i + 1) * 35;
		m_surSummonSkill[i].SetRectWH(nX, nPosY, NTL_ITEM_ICON_SIZE, NTL_ITEM_ICON_SIZE);
		m_CoolTimeEffect[i].SetPosition(nX, nPosY);
	}
}

VOID CPetSkillWindowGui::OnMouseLeave( gui::CComponent* pComponent )
{
    m_bVisibeFocus = FALSE;

    // 툴팁을 없앤다.
    if(GetInfoWndManager()->GetRequestGui() == DIALOG_PET_SKILL_SLOT)
    {
        GetInfoWndManager()->ShowInfoWindow(FALSE);
    }
}

void CPetSkillWindowGui::SetSkillIcon() 
{
    if(!m_uiSummonPetID)
    {
        for(RwInt32 i = 0; i < SUMMON_PET_MAX_SKILL; ++i)
            m_pSobSkillIcon[i] = NULL;
        return;
    }

    CNtlSobPet* pSobPet = (CNtlSobPet*)GetNtlSobManager()->GetSobObject(m_uiSummonPetID);
    if(!pSobPet)
        return;

    for(RwInt32 i = 0; i < SUMMON_PET_MAX_SKILL; ++i)
    {
        CNtlSobSkill* pSobSkill = pSobPet->GetSkillContainer()->GetSkill(i);
        if(!pSobSkill)
		{
			m_pSobSkillIcon[i] = NULL;
			m_surSummonSkill[i].UnsetTexture();
            continue;
		}

        m_pSobSkillIcon[i] = pSobSkill->GetIcon();
        m_surSummonSkill[i].SetTexture(reinterpret_cast<gui::CTexture*>(m_pSobSkillIcon[i]->GetImage()));
    }
}

void CPetSkillWindowGui::UpdateSkillIcon( RwReal fElapsedTime ) 
{
    for(RwInt32 i = 0; i < SUMMON_PET_MAX_SKILL; ++i)
    {
        if(m_pSobSkillIcon[i])
        {
            if(m_pSobSkillIcon[i]->GetIconState() == CNtlSobIcon::ICON_STATE_COOLING && !m_CoolTimeEffect[i].IsWork())            
            {
                m_CoolTimeEffect[i].StartProc(m_pSobSkillIcon[i]->GetMaxCoolingTime());
            }
            else if(m_pSobSkillIcon[i]->GetIconState() != CNtlSobIcon::ICON_STATE_COOLING && m_CoolTimeEffect[i].IsWork())
            {
                m_CoolTimeEffect[i].EndProc();
            }
        }
    }

    for(RwInt32 i = 0; i < SUMMON_PET_MAX_SKILL; ++i)
    {
        if(m_CoolTimeEffect[i].IsWork() && m_pSobSkillIcon[i])
        {
            RwReal fCurrentTime = 0.0f;

            // 스킬의 남은 쿨타임을 가져온다.
            fCurrentTime = m_pSobSkillIcon[i]->GetCoolingTime();            
            m_CoolTimeEffect[i].Update(fCurrentTime);            
        }
    }

}

VOID CPetSkillWindowGui::OnMouseMove( RwInt32 nFlags, RwInt32 nX, RwInt32 nY ) 
{
    nX += m_pThis->GetPosition().left;
    nY += m_pThis->GetPosition().top;

    RwBool bIn = FALSE;
    for(RwInt32 i = 0; i < SUMMON_PET_MAX_SKILL; ++i)
    {
        if(m_pSobSkillIcon[i] && m_surSummonSkill[i].GetTexture())
        {
            if(m_surSummonSkill[i].PtInRect(nX, nY))
            {
                GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_SKILL, nX, nY, (void*)m_pSobSkillIcon[i]->GetSobObj(), DIALOG_PET_SKILL_SLOT);
                bIn = TRUE;
                break;
            }
        }
    }

    if(!bIn)
    {
        if(GetInfoWndManager()->GetRequestGui() == DIALOG_PET_SKILL_SLOT)
        {
            GetInfoWndManager()->ShowInfoWindow(FALSE);
        }
    }
}

VOID CPetSkillWindowGui::OnMouseDialogLeave( gui::CComponent* pComponent ) 
{
    if(GetInfoWndManager()->GetRequestGui() == DIALOG_PET_SKILL_SLOT)
    {
        GetInfoWndManager()->ShowInfoWindow(FALSE);
    }
}

VOID CPetSkillWindowGui::OnClickTransformCancel( gui::CComponent* pComponent ) 
{
    GetDboGlobal()->GetGamePacketGenerator()->SendTransformCancelReq();
}

RwUInt32 CPetSkillWindowGui::GetInfoFlag()
{
	return m_uiInfoFlag;
}
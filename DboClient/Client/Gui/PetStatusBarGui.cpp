#include "precomp_dboclient.h"
#include "PetStatusBarGui.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLApi.h"
#include "NtlSLEvent.h"
#include "NtlSLGlobal.h"
#include "NtlSLLogic.h"
#include "NtlSobCharProxy.h"
#include "NtlSobPet.h"
#include "NtlSobPetAttr.h"
#include "NtlSobManager.h"
#include "NPCTable.h"
#include "NtlSLPacketGenerator.h"

//sound
#include "GUISoundDefine.h"

// dbo
#include "BuffDispObject.h"
#include "DboLogic.h"
#include "DboEvent.h"
#include "DisplayStringManager.h"
#include "DialogManager.h"
#include "DboGlobal.h"

#define SUR_CHAR_LEFT_OFFSET 13
#define SUR_CHAR_TOP_OFFSET  17

CPetStatusBarGui* CPetStatusBarGui::m_pInstance = NULL;


CPetStatusBarGui::CPetStatusBarGui()
{
	Init();    
}

CPetStatusBarGui::CPetStatusBarGui( const RwChar* pName )
:CNtlPLGui(pName)
{
	Init();
}

CPetStatusBarGui::~CPetStatusBarGui()
{
	Destroy();
}

void CPetStatusBarGui::Init()
{
	m_bDelete = FALSE;
	m_uiPetID = INVALID_SERIAL_ID;
    m_bShowInfo = TRUE;
}

RwBool CPetStatusBarGui::Create()
{
	if(!CNtlPLGui::Create("", "gui\\PetStatusBar.srf", "gui\\PetStatusBar.frm"))
		return FALSE;

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgPetStatus");

	m_ppgbHp        = (gui::CProgressBar*)GetComponent("pgbLP");
	m_ppgbEp        = (gui::CProgressBar*)GetComponent( "pgbEP" );		
	m_sttHp         = (gui::CStaticBox*)GetComponent("sttLP");
	m_sttMp         = (gui::CStaticBox*)GetComponent("sttEP");	
	m_psttNickName  = (gui::CStaticBox*)GetComponent("sttNickName");
    m_pbtnPetInfo   = (gui::CButton*)GetComponent( "btnInfoButton");	
    m_pPnlInfoBack  = (gui::CPanel*)GetComponent("pnlInfoBack");
    m_pSttInfoLPLabel = (gui::CStaticBox*)GetComponent("sttInfoLP");
    m_pSttInfoEPLabel = (gui::CStaticBox*)GetComponent("sttInfoEP");
    m_pSttInfoHRLabel = (gui::CStaticBox*)GetComponent("sttInfoHitRate");
    m_pSttInfoDRLabel = (gui::CStaticBox*)GetComponent("sttInfoDodgeRate");
    m_pSttInfoPOLabel = (gui::CStaticBox*)GetComponent("sttInfoOffense");
    m_pSttInfoPDLabel = (gui::CStaticBox*)GetComponent("sttInfoPhysicalDefense");    
    m_pSttInfoEDLabel = (gui::CStaticBox*)GetComponent("sttInfoEnergyDefense");
    m_pSttInfoLP    = (gui::CStaticBox*)GetComponent("sttInfoLPVal");
    m_pSttInfoEP    = (gui::CStaticBox*)GetComponent("sttInfoEPVal");
    m_pSttInfoHR    = (gui::CStaticBox*)GetComponent("sttInfoHRVal");
    m_pSttInfoDR    = (gui::CStaticBox*)GetComponent("sttInfoDRVal");
    m_pSttInfoPO    = (gui::CStaticBox*)GetComponent("sttInfoOffenseVal");
    m_pSttInfoPD    = (gui::CStaticBox*)GetComponent("sttInfoPDVal");    
    m_pSttInfoED    = (gui::CStaticBox*)GetComponent("sttInfoEDVal");	
 //   m_ppnlPowerText = (gui::CPanel*)GetComponent( "pnlPowerText" );

    // Power Number
    /*m_numPower.Create( ND_LEFT );
    m_numPower.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfPowerFont_0" ) );
    m_numPower.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfPowerFont_1" ) );
    m_numPower.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfPowerFont_2" ) );
    m_numPower.SetSurface( 3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfPowerFont_3" ) );
    m_numPower.SetSurface( 4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfPowerFont_4" ) );
    m_numPower.SetSurface( 5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfPowerFont_5" ) );
    m_numPower.SetSurface( 6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfPowerFont_6" ) );
    m_numPower.SetSurface( 7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfPowerFont_7" ) );
    m_numPower.SetSurface( 8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfPowerFont_8" ) );
    m_numPower.SetSurface( 9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfPowerFont_9" ) );	
    m_numPower.SetParentPos( 115, 0 );
    m_numPower.SetNumGap(-5);*/


    // Font
    m_sttHp->CreateFontStd(DETAIL_FONT, 80, DEFAULT_FONT_ATTR);
    m_sttMp->CreateFontStd(DETAIL_FONT, 80, DEFAULT_FONT_ATTR);
    m_pSttInfoLPLabel->CreateFontStd(DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
    m_pSttInfoEPLabel->CreateFontStd(DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
    m_pSttInfoHRLabel->CreateFontStd(DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
    m_pSttInfoDRLabel->CreateFontStd(DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
    m_pSttInfoPOLabel->CreateFontStd(DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
    m_pSttInfoPDLabel->CreateFontStd(DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
    m_pSttInfoEDLabel->CreateFontStd(DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
    m_pSttInfoLP->CreateFontStd(DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
    m_pSttInfoEP->CreateFontStd(DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
    m_pSttInfoHR->CreateFontStd(DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
    m_pSttInfoDR->CreateFontStd(DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
    m_pSttInfoPO->CreateFontStd(DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
    m_pSttInfoPD->CreateFontStd(DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
    m_pSttInfoED->CreateFontStd(DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);    

    // label        
    m_pSttInfoHRLabel->SetText(GetDisplayStringManager()->GetString("DST_STATUS_ATTACK_RATE"));
    m_pSttInfoDRLabel->SetText(GetDisplayStringManager()->GetString("DST_STATUS_DODGE"));
    m_pSttInfoPOLabel->SetText(GetDisplayStringManager()->GetString("DST_STATUS_ATTACK"));
    m_pSttInfoPDLabel->SetText(GetDisplayStringManager()->GetString("DST_PET_STATUS_PHYSICAL_DEFENSE"));    
    m_pSttInfoEDLabel->SetText(GetDisplayStringManager()->GetString("DST_PET_STATUS_ENERGY_DEFENSE"));	
    
    // Event
	m_slotMove	= m_pThis->SigMove().Connect( this, &CPetStatusBarGui::OnMove );
	m_slotPaint = m_pThis->SigPaint().Connect(this, &CPetStatusBarGui::OnPaint);	
    m_slotPaintLast = m_psttNickName->SigPaint().Connect(this, &CPetStatusBarGui::OnPaintLast);
    /*m_slotPowerPaint = m_ppnlPowerText->SigPaint().Connect(this, &CPetStatusBarGui::OnPaintPower);
    m_slotPowerMove = m_ppnlPowerText->SigMove().Connect(this, &CPetStatusBarGui::OnMovePower);*/
    m_slotInfoBtnClick = m_pbtnPetInfo->SigClicked().Connect(this, &CPetStatusBarGui::OnClickInfoBtn);
    m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect(this, &CPetStatusBarGui::OnCaptureMouseDown);
	m_slotCaptureMouseUp = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseUp().Connect(this, &CPetStatusBarGui::OnCaptureMouseUp);

	/*m_surCharacter.SetFilterTexture( Logic_CreateTexture( "8181.png" ) );*/

	// Buff
	m_pBuff = NTL_NEW CBuffDispGui();
	DBO_ASSERT( m_pBuff, "CPetStatusBarGui::Create : Buff is not allocated. Not enough memory" );
	BYTE byShowFlag = MAKE_BIT_FLAG(BUFF_TYPE_BLESS) + MAKE_BIT_FLAG(BUFF_TYPE_CURSE);
    m_pBuff->Create(byShowFlag, CBuffDispGui::NORMAL_ICONSIZE, 5, 5, DIALOG_HP );
	m_pBuff->Show( true );

	// Effect
	gui::CSurface surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "PetStatusBar.srf", "srfEmergencyEffect" );
	m_feEmergency.SetSurface( surface );
	m_feEmergency.SetTime( 1000.0f, 0.5f );
	m_feEmergency.SetAlpha( 0, 255 );
	m_feEmergency.SetSize( surface.m_Original.rtRect.GetWidth(), surface.m_Original.rtRect.GetWidth() + 5,
						   surface.m_Original.rtRect.GetHeight(), surface.m_Original.rtRect.GetHeight() + 5 );

	LinkMsg(g_EventSobInfoUpdate);
	LinkMsg(g_EventSummonPet,0, 0x7000);

	// Update Link
	//GetNtlGuiManager()->AddUpdateBeforeFunc( this );
	GetNtlGuiManager()->AddUpdateFunc( this );

    ToggleInfo();

	Show( false );

	return TRUE;
}

void CPetStatusBarGui::Destroy()
{
	// Buff
	if( m_pBuff )
	{
		m_pBuff->Destroy();
		NTL_DELETE( m_pBuff );
	}

	/*Logic_DeleteTexture( m_surCharacter.GetFilterTexture() );*/
	/*m_surCharacter.UnsetFilterTexture();*/

	Logic_DeleteTexture( m_surCharacter.GetTexture() );
	m_surCharacter.UnsetTexture();

	UnLinkMsg(g_EventSobInfoUpdate);
	UnLinkMsg(g_EventSummonPet);

	// Update Unlink
	//GetNtlGuiManager()->RemoveUpdateBeforeFunc( this );
	GetNtlGuiManager()->RemoveUpdateFunc( this );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 
}

void CPetStatusBarGui::Update( RwReal fElapsed )
{
	if(m_bDelete)
	{
		CPetStatusBarGui::DeleteInstance();
		return;
	}

	if(m_pBuff)
		m_pBuff->Update(fElapsed);

	if( IsShow() )
		m_feEmergency.Update(fElapsed);
}

void CPetStatusBarGui::UpdateBeforeCamera( RwReal fElapsed )
{
	if(!IsShow())
		return;

	// TODO: 일러스트로 대체
	//if(m_uiPetID)
	//{
	//	CNtlSobPet* pSobPet = reinterpret_cast<CNtlSobPet*>(GetNtlSobManager()->GetSobObject(m_uiPetID));		
	//	CNtlSobCharProxy *pCharProxy = reinterpret_cast<CNtlSobCharProxy*>(pSobPet->GetSobProxy());
	//	m_texCharacter.Load(pCharProxy->UIPcStatusBarRender());
	//	m_surCharacter.SetTexture( &m_texCharacter );
	//}
}

RwInt32 CPetStatusBarGui::SwitchDialog( bool bOpen )
{
	Show(bOpen);
	
	if(m_pBuff)
		m_pBuff->Show(bOpen);

	return TRUE;
}

VOID CPetStatusBarGui::OnMove( RwInt32 iOldX, RwInt32 iOldY )
{
	CRectangle rect = m_pThis->GetScreenRect();

	// 일러스트
	m_surCharacter.SetRectWH( rect.left + SUR_CHAR_LEFT_OFFSET, rect.top + SUR_CHAR_TOP_OFFSET, 38, 38 );

	// Emergency Effect
	RwInt32 nCenterX, nCenterY;
	gui::CSurface surface = m_feEmergency.GetSurface();
	surface.m_Original.rtRect.GetCenterPosition(&nCenterX, &nCenterY);
	m_feEmergency.SetCenterPos(rect.left + nCenterX, rect.top + nCenterY);

	// 버프 아이콘
	m_pBuff->SetMove(rect);
}

VOID CPetStatusBarGui::OnPaint( VOID )
{
	// Pet Illust
	m_surCharacter.Render();
}

VOID CPetStatusBarGui::OnPaintLast( VOID )
{
	m_feEmergency.Render();
}

CPetStatusBarGui* CPetStatusBarGui::GetInstance()
{
    if(!m_pInstance)
    {
        m_pInstance = NTL_NEW CPetStatusBarGui("Pet Status Bar");
        if(!m_pInstance->Create())
        {
            NTL_DELETE(m_pInstance);
            return NULL;
        }
    }

    return m_pInstance;
}

void CPetStatusBarGui::DeleteInstance()
{
	NTL_DELETE(m_pInstance);
}

void CPetStatusBarGui::HandleEvents( RWS::CMsg &pMsg )
{
	if(pMsg.Id == g_EventSobInfoUpdate)	// Pet의 정보가 Update 되었을때
	{
		SNtlEventSobInfoUpdate *pUpdate = reinterpret_cast<SNtlEventSobInfoUpdate*>(pMsg.pData); 

		if( pUpdate->hSerialId == INVALID_SERIAL_ID )
			return;
		
		if(m_uiPetID != pUpdate->hSerialId)
			return;

		if( pUpdate->uiUpdateType & EVENT_AIUT_ATTR )
		{
			UpdatePetInfo(); 
		}	

		if( pUpdate->uiUpdateType & EVENT_AIUT_BUFF )			
		{
			m_pBuff->SetBuffAll( m_uiPetID );
		}

		/*UpdatePetPowerValue();*/
	}
	else if(pMsg.Id == g_EventSummonPet)		// Pet이 죽었을때 UI도 삭제한다.
	{
		SDboEventSummonPet* pData = reinterpret_cast<SDboEventSummonPet*>(pMsg.pData);

		if( pData->bCreate )
		{
			/*UpdatePetPowerValue();*/
		}
		else if( pData->uiSerialID == m_uiPetID )
		{
			m_uiPetID = INVALID_SERIAL_ID;

			if( m_pBuff )
				m_pBuff->ClearBuff();
			
			GetDialogManager()->CloseDialog( DIALOG_PET_STATUSBAR );
		}		
	}
}

void CPetStatusBarGui::SetPetID( RwUInt32 uiSerialId )
{
	m_uiPetID = uiSerialId;

	CNtlSobPet* pPet = reinterpret_cast<CNtlSobPet*>(GetNtlSobManager()->GetSobObject(m_uiPetID));
	if(pPet)
	{
		UpdatePetInfo();

		// Illust
		CNtlSobPetAttr* pPetAttr = reinterpret_cast<CNtlSobPetAttr*>( pPet->GetSobAttr() );
		if( pPetAttr == NULL)
			DBO_ASSERT( 0 , "CPetStatusBarGui::SetPetID - pPetAttr is NULL" );

		// 이전에 있던 Texture 삭제
		if( m_surCharacter.GetTexture() )
		{
			Logic_DeleteTexture( m_surCharacter.GetTexture() );
			m_surCharacter.UnsetTexture();
		}

		CHAR acPetIllustBuffer[256];
		sprintf_s( acPetIllustBuffer, 256, "Pet_%s.png", pPetAttr->GetModelName() );

		// Create Textrue
		m_surCharacter.SetTexture( Logic_CreateTexture( acPetIllustBuffer, TEXTYPE_ICON ) );
	}
}

void CPetStatusBarGui::UpdatePetInfo()
{
	CNtlSobPet* pSobPet = reinterpret_cast<CNtlSobPet*>(GetNtlSobManager()->GetSobObject(m_uiPetID));
	if(!pSobPet)
		return;

    CNtlSobPetAttr* pPetAttr = reinterpret_cast<CNtlSobPetAttr*>(pSobPet->GetSobAttr());    
    if(!pPetAttr)
        return;

	RwUInt32 uiLP		= Logic_GetLp(pSobPet);
	RwUInt32 uiMaxLP	= Logic_GetMaxLp(pSobPet);
	RwUInt32 uiEP		= Logic_GetEp(pSobPet);
	RwUInt32 uiMaxEP	= Logic_GetMaxEp(pSobPet);	    

	m_ppgbHp->SetRange(0, uiMaxLP);
	m_ppgbHp->SetPos(uiLP); 
	m_ppgbEp->SetRange( 0, uiMaxEP );
	m_ppgbEp->SetPos( uiEP );
	m_sttHp->Format("%u/%u",uiLP,uiMaxLP);
	m_sttMp->Format("%u/%u",uiEP,uiMaxEP);
	m_psttNickName->SetText(Logic_GetName(pSobPet));

    // Info Window
    char buf[12] = {0,};

    sprintf_s(buf, "%u", uiLP);
    m_pSttInfoLP->SetText(buf);

    sprintf_s(buf, "%u", uiEP);
    m_pSttInfoEP->SetText(buf);

    // Pet의 Status는 차후에 서버로 부터 정보를 받아서 표시한다.
    sprintf_s(buf, "%u", pPetAttr->m_wAttackRate);
    m_pSttInfoHR->SetText(buf);

    sprintf_s(buf, "%u", pPetAttr->m_wDodgeRate);
    m_pSttInfoDR->SetText(buf);

    // 물리/기공 속성에 따라서 펫의 공격력을 표시한다.
    if(BATTLE_ATTACK_TYPE_PHYSICAL == pPetAttr->m_pNpcTbl->byAttack_Type)  // 물리 공격 펫
    {
        sprintf_s(buf, "%u", pPetAttr->m_wPhysicalOffence);    
    }
    else // 기공 공격 펫
    {
        sprintf_s(buf, "%u", pPetAttr->m_wEnergyOffence);    
    }
    
    m_pSttInfoPO->SetText(buf);

    sprintf_s(buf, "%u", pPetAttr->m_wPhysicalDefence);
    m_pSttInfoPD->SetText(buf);

    sprintf_s(buf, "%u", pPetAttr->m_wEnergyDefence);
    m_pSttInfoED->SetText(buf);

	if( (RwReal)uiLP / (RwReal)uiMaxLP <= 0.2f )
	{
		if( !m_feEmergency.IsWork() )
			m_feEmergency.StartProc( TRUE );
	}
	else
	{
		if( m_feEmergency.IsWork() )
			m_feEmergency.EndProc();
	}
}

//VOID CPetStatusBarGui::UpdatePetPowerValue()
//{
//	CNtlSobPet* pPet = reinterpret_cast<CNtlSobPet*>( GetNtlSobManager()->GetSobObject( m_uiPetID ) );
//	CNtlSobPetAttr* pAttr = reinterpret_cast<CNtlSobPetAttr*>( pPet->GetSobAttr() );    
//	RwUInt32 uiPower = Dbo_CalculatePowerLevel(
//		pAttr->m_Str, pAttr->m_Con, pAttr->m_Foc, pAttr->m_Dex, pAttr->m_Sol, pAttr->m_Eng,
//		pAttr->m_wPhysicalOffence, pAttr->m_wPhysicalDefence, pAttr->m_wEnergyOffence, pAttr->m_wEnergyDefence,
//		pAttr->m_wAttackRate, pAttr->m_wDodgeRate, pAttr->m_wCurseSuccessRate, pAttr->m_wCurseToleranceRate,
//		pAttr->m_wPhysicalCriticalRate, pAttr->m_wEnergyCriticalRate, pAttr->m_wAttackSpeedRate,
//		(WORD)pAttr->GetMaxLp(), (WORD)pAttr->GetMaxEp(), (WORD)pAttr->GetLp(), (WORD)pAttr->GetEp(), (BYTE)pAttr->GetRpStock(), pAttr->GetLevel(), 0 );
//
//    m_numPower.SetNumber(uiPower);
//}

VOID CPetStatusBarGui::OnClickInfoBtn( gui::CComponent* pComponent )
{
    ToggleInfo();
}

void CPetStatusBarGui::ToggleInfo()
{
    m_bShowInfo = !m_bShowInfo;

    m_pPnlInfoBack->Show(m_bShowInfo);
    m_pSttInfoLPLabel->Show(m_bShowInfo);
    m_pSttInfoEPLabel->Show(m_bShowInfo);
    m_pSttInfoHRLabel->Show(m_bShowInfo);
    m_pSttInfoDRLabel->Show(m_bShowInfo);
    m_pSttInfoPOLabel->Show(m_bShowInfo);
    m_pSttInfoPDLabel->Show(m_bShowInfo);    
    m_pSttInfoEDLabel->Show(m_bShowInfo);
    m_pSttInfoLP->Show(m_bShowInfo);
    m_pSttInfoEP->Show(m_bShowInfo);
    m_pSttInfoHR->Show(m_bShowInfo);
    m_pSttInfoDR->Show(m_bShowInfo);
    m_pSttInfoPO->Show(m_bShowInfo);
    m_pSttInfoPD->Show(m_bShowInfo);    
    m_pSttInfoED->Show(m_bShowInfo);
}

VOID CPetStatusBarGui::OnCaptureMouseDown( const CKey& key )
{
    CAPTURE_MOUSEDOWN_RAISE(DIALOG_PET_STATUSBAR, key.m_fX, key.m_fY);

	if( m_pThis->GetScreenRect().PtInRect( (RwInt32)key.m_fX, (RwInt32)key.m_fY ) )
	{
		if( key.m_nID == UD_LEFT_BUTTON )
		{
			m_pThis->CaptureMouse();
		}
	}
}

VOID CPetStatusBarGui::OnCaptureMouseUp( const CKey& key )
{
	if( m_pThis->GetScreenRect().PtInRect( (RwInt32)key.m_fX, (RwInt32)key.m_fY ) )
	{
		if( key.m_nID == UD_LEFT_BUTTON )
		{
			if( m_pThis->IsMouseCaptured() )
			{
				SERIAL_HANDLE hTarget = Logic_GetAvatarTargetHandle();

				if(m_uiPetID != hTarget)
				{
					Logic_SobTarget(m_uiPetID, INVALID_BYTE);
				}

				m_pThis->ReleaseMouse();
			}
		}
	}
}

//VOID CPetStatusBarGui::OnPaintPower( VOID ) 
//{
//    m_numPower.Render();
//}
//
//VOID CPetStatusBarGui::OnMovePower( RwInt32 nOldX, RwInt32 nOldY ) 
//{
//    CRectangle rtScreen = m_ppnlPowerText->GetScreenRect();
//    m_numPower.SetPosition( rtScreen.left, rtScreen.top );
//}

RwUInt32 CPetStatusBarGui::GetPetID()
{
	return m_uiPetID;
}


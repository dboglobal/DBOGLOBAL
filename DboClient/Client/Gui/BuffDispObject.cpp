#include "precomp_dboclient.h"
#include "BuffDispObject.h"

// core
#include "NtlDebug.h"

// shared
#include "NtlObject.h"
#include "SystemEffectTable.h"
#include "SkillTable.h"
#include "ItemTable.h"
#include "UseItemTable.h"
#include "TableContainer.h"

// gui
#include "gui_renderer.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLApi.h"
#include "NtlSobManager.h"
#include "NtlSobBuff.h"
#include "NtlSobBuffAttr.h"
#include "NtlBuffContainer.h"
#include "NtlSobIcon.h"
#include "NtlSobFeelingActor.h"

// dbo
#include "DboGlobal.h"
#include "DboPacketGenerator.h"
#include "DialogManager.h"
#include "InfoWndManager.h"




#define BUFF_ROW_MARGIN			3		// Distance between buffs
#define BUFF_COL_MARGIN			1	
#define BUFF_MAX_DISP_COUNT		16
#define BUFF_UPDATE_TIME		1.0f

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ! CBuff
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VOID CBuff::SetMove( RwInt32 nScreenX, RwInt32 nScreenY )
{
	m_Surface.SetPosition( nScreenX + m_Rect.left, nScreenY + m_Rect.top );
	m_KeepTimeEffect.SetPosition( nScreenX + m_Rect.left, nScreenY + m_Rect.top );
	m_surDisableSlot.SetPosition(nScreenX + m_Rect.left, nScreenY + m_Rect.top);
}

VOID CBuff::SetRect(int nX, int nY, RwInt32 nSize, gui::CSurface& KeepTimeSurface, gui::CSurface& DisableSurface)
{
	m_Rect.left = nX;
	m_Rect.top = nY;
	m_Rect.right = nX + nSize;
	m_Rect.bottom = nY + nSize;

	m_Surface.SetRect( m_Rect );
	
	m_KeepTimeEffect.SetSurface(KeepTimeSurface);

	m_surDisableSlot.SetSurface(DisableSurface);
	m_surDisableSlot.SetWidth(nSize);
	m_surDisableSlot.SetHeight(nSize);
}

VOID CBuff::Render(VOID)
{
	if( m_eType != TYPE_NONE )
	{
		m_Surface.Render();
		m_KeepTimeEffect.Render();
		m_surDisableSlot.Render();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ! CSobBuff
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VOID CSobBuff::SetBuff( void* pBuff, TYPE eType )
{
	CNtlSobBuff* pSobBuff = reinterpret_cast<CNtlSobBuff*>( pBuff );

	if( pSobBuff )
	{
		m_pBuff = pSobBuff;
		m_eType = eType;
		m_Surface.SetTexture( reinterpret_cast<gui::CTexture*>( pSobBuff->GetIcon()->GetImage() ) );
		m_KeepTimeEffect.StartProc( m_pBuff->GetIcon()->GetMaxCoolingTime(), TRUE, TRUE );
	}
	else
	{
		m_pBuff = NULL;
		m_eType = TYPE_NONE;
		m_KeepTimeEffect.EndProc();
	}
}

VOID CSobBuff::DropBuff(VOID)
{
	if( m_eType != TYPE_NONE )
	{
		CNtlSobBuffAttr* pBuffAttr = reinterpret_cast<CNtlSobBuffAttr*>( m_pBuff->GetSobAttr() );
		
		if (pBuffAttr->IsBlessType())
		{
			// check if item
			if (pBuffAttr->GetBuffType() == DBO_OBJECT_SOURCE_ITEM)
			{
				sDBO_BUFF_PARAMETER* pBuffParam = pBuffAttr->GetBuffParameter(0);

				if (pBuffParam->byBuffParameterType == DBO_BUFF_PARAMETER_TYPE_ACTIVE_LP_EP_AUTO_RECOVER && m_KeepTimeEffect.GetUpdateElapsedTime() > 0.0f) // check if auto-pot and dont allow to drop while in cooldown
				{
					return;
				}
			}

			GetDboGlobal()->GetGamePacketGenerator()->SendBuffDropReq(pBuffAttr->GetBuffIndex(), pBuffAttr->GetBuffType(), pBuffAttr->GetTblIdx());
		}
	}
}

VOID CSobBuff::ShowInfoWnd( RwInt32 nParentX, RwInt32 nParentY, RwInt32 nDlgType )
{
	GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_BUFF, m_Rect.left + nParentX, m_Rect.top + nParentY, m_pBuff, nDlgType );
}

VOID CSobBuff::Update( RwReal fElapsedTime )
{
	if( m_eType != TYPE_NONE )
	{
		CNtlSobBuffAttr* pBuffAttr = reinterpret_cast<CNtlSobBuffAttr*>(m_pBuff->GetSobAttr());
		if (pBuffAttr == NULL)
		{
			DBO_WARNING_MESSAGE("pBuffAttr IS NULL");
			return;
		}

		if (pBuffAttr->GetBuffActive()) // check if buff is active
		{
			if (m_surDisableSlot.IsShow())
				m_surDisableSlot.Show(FALSE);

			m_KeepTimeEffect.Update(m_pBuff->GetIcon()->GetCoolingTime());

			if (m_KeepTimeEffect.GetUpdateElapsedTime() > 0.0f && m_pBuff->GetIcon()->GetCoolingTime() == 0.0f)
			{
				m_KeepTimeEffect.StartProc(m_pBuff->GetIcon()->GetMaxCoolingTime(), TRUE, TRUE);
			}
		}
		else
		{
			if (m_surDisableSlot.IsShow() == FALSE)
				m_surDisableSlot.Show(TRUE);
		}
	}	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ! CFakeBuff
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VOID CFakeBuff::SetBuff( void* pBuff, TYPE eType )
{
	sNtlFakeBuff* pFakeBuff = reinterpret_cast<sNtlFakeBuff*>( pBuff );

	if( pFakeBuff )
	{
		m_pBuff = pFakeBuff;
		m_eType = eType;
		m_Surface.SetTexture( reinterpret_cast<gui::CTexture*>( pFakeBuff->pImage ) );
		m_KeepTimeEffect.StartProc( pFakeBuff->fMaxTime, TRUE, TRUE );
	}
	else
	{
		m_pBuff = NULL;
		m_eType = TYPE_NONE;
		m_KeepTimeEffect.EndProc();
	}
}

VOID CFakeBuff::ShowInfoWnd( RwInt32 nParentX, RwInt32 nParentY, RwInt32 nDlgType )
{
	GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_FAKEBUFF, m_Rect.left + nParentX, m_Rect.top + nParentY, m_pBuff, nDlgType );
}

VOID CFakeBuff::Update( RwReal fElapsedTime )
{
	if( m_eType != TYPE_NONE )
	{
		if (m_pBuff->bActive) // check if buff is active
		{
			if (m_surDisableSlot.IsShow())
				m_surDisableSlot.Show(FALSE);

			m_KeepTimeEffect.Update(m_pBuff->fTime);

			if (m_KeepTimeEffect.GetUpdateElapsedTime() > 0.0f && m_pBuff->fTime == 0.0f)
			{
				m_KeepTimeEffect.StartProc(m_pBuff->fMaxTime, TRUE, TRUE);
			}
		}
		else
		{
			if (m_surDisableSlot.IsShow() == FALSE)
				m_surDisableSlot.Show(TRUE);
		}
	}	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ! CBuffDispBar
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CBuffDispBar::CBuffDispBar(eBUFF_TYPE eBuffType)
: m_pDialog( NULL ), m_ppArrBuff( NULL ), m_nBuffCnt( 0 ), m_nMouseOnIndex( -1 )
{

	m_nBuffPerRowCnt = 0;
	m_eBuffType = eBuffType;
}

CBuffDispBar::~CBuffDispBar(VOID)
{
	Destroy();
}

VOID CBuffDispBar::Create( RwInt32 nBuffCount, eDialogType eDlgType, RwInt32 nIconSize, RwInt32 eBuffOrigin )
{
	NTL_FUNCTION( "CBuffDispBar:Create" );

	m_nBuffCnt = nBuffCount;

	m_ppArrBuff = NTL_NEW CBuff*[nBuffCount];

	if( eBuffOrigin == CBuffDispGui::SOBBUFF )
	{
		for(int i = 0 ; i < nBuffCount; i++ )
			m_ppArrBuff[i] = NTL_NEW CSobBuff;
	}
	else if (eBuffOrigin == CBuffDispGui::FAKEBUFF)
	{
		for(int i = 0 ; i < nBuffCount; i++ )
			m_ppArrBuff[i] = NTL_NEW CFakeBuff;
	}
	else
	{
		NTL_ASSERTFAIL("The buffer origin is invalid.");
	}

	m_eDlgType = eDlgType;

	m_pDialog = NTL_NEW gui::CDialog( CRectangle( 0, 0, 0, 0 ), GetNtlGuiManager()->GetGuiManager(), GetNtlGuiManager()->GetSurfaceManager() );
	NTL_ASSERT( m_pDialog, "CBuffDispBar::Create : m_pDialog is not allocated, Not enough Memory!" );
	
	if( m_eDlgType == DIALOG_HP )
		m_slotMouseDown = m_pDialog->SigMouseDown().Connect( this, &CBuffDispBar::OnMouseDown );
	m_slotMouseMove = m_pDialog->SigMouseMove().Connect( this, &CBuffDispBar::OnMouseMove );
	m_slotMouseLeave = m_pDialog->SigMouseLeave().Connect( this, &CBuffDispBar::OnMouseLeave );
	m_slotMove = m_pDialog->SigMove().Connect( this, &CBuffDispBar::OnMove );
	m_slotPaint = m_pDialog->SigPaint().Connect( this, &CBuffDispBar::OnPaint );

	SetBuffRect( nIconSize, 0 );

	NTL_RETURNVOID();
}

VOID CBuffDispBar::Destroy(VOID)
{
	if( m_ppArrBuff )
	{
		for(int i = 0 ; i < m_nBuffCnt ; ++i )
		{
			NTL_DELETE( m_ppArrBuff[i] );
		}

		NTL_ARRAY_DELETE( m_ppArrBuff );		
	}
	
	if( m_pDialog )
		NTL_DELETE( m_pDialog );
}

VOID CBuffDispBar::Update( RwReal fElapsedTime )
{
	for(int i = 0 ; i < m_nBuffCnt ; i++)
	{
		m_ppArrBuff[i]->Update( fElapsedTime );
	}
}

VOID CBuffDispBar::SetBuff( VOID* pSobBuff, CBuff::TYPE eType, RwInt32 nSlot )
{
	if (nSlot >= m_nBuffCnt)
	{
		DBO_WARNING_MESSAGE("Invalid buff index, max count : " << m_nBuffCnt << ", current index : " << nSlot);
		return;
	}

	m_ppArrBuff[nSlot]->SetBuff( pSobBuff, eType );
}

VOID CBuffDispBar::SetMove( RwInt32 nScreenX, RwInt32 nScreenY )
{
	m_pDialog->SetPosition( nScreenX, nScreenY );
}

VOID CBuffDispBar::SetSize( RwInt32 nWidth, RwInt32 nHeight )
{
	m_pDialog->SetSize( nWidth, nHeight );
}
						 
VOID CBuffDispBar::Show( bool bShow )
{
	m_pDialog->Show( bShow );
	
	if( !bShow )
		OnMouseLeave( NULL );
}

VOID CBuffDispBar::RefreshInfoWnd(VOID)
{
	if( m_nMouseOnIndex < 0 )
		return;
	
	if( m_ppArrBuff[m_nMouseOnIndex]->m_eType != CBuff::TYPE_NONE )
	{
		CRectangle rtScreen = m_pDialog->GetScreenRect();
		m_ppArrBuff[m_nMouseOnIndex]->ShowInfoWnd( rtScreen.left, rtScreen.top, m_eDlgType );		
	}		
	else
	{
		if( GetInfoWndManager()->GetRequestGui() == m_eDlgType )
			GetInfoWndManager()->ShowInfoWindow( FALSE );
	}
	
}

VOID CBuffDispBar::SetBuffRect( int nIconSize, int nBuffPerRowCnt)
{
	int nCount = nBuffPerRowCnt;
	if (nCount >= m_nBuffCnt)
		nCount = m_nBuffCnt;

	m_nBuffPerRowCnt = nCount;

	if (m_nBuffCnt < 0 || m_nBuffCnt == 0)
		return;

	gui::CSurface KeepTimeSurface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotGrayedEffect");
	KeepTimeSurface.m_Original.rtRect.right = KeepTimeSurface.m_Original.rtRect.left + nIconSize;
	KeepTimeSurface.m_Original.rtRect.bottom = KeepTimeSurface.m_Original.rtRect.top + nIconSize;

	gui::CSurface DisableSurface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotDisableEffect");
	DisableSurface.m_Original.rtRect.right = DisableSurface.m_Original.rtRect.left + nIconSize;
	DisableSurface.m_Original.rtRect.bottom = DisableSurface.m_Original.rtRect.top + nIconSize;

	int nRow, nCol;

	for( int i = 0 ; i < m_nBuffCnt ; i++)
	{
		if (nCount)
		{
			nCol = i / nCount;
			nRow = i % nCount;
		}
		else
		{
			nCol = 0;
			nRow = i;
		}

		m_ppArrBuff[i]->SetRect(nRow * (nIconSize + BUFF_ROW_MARGIN), nCol * (nIconSize + BUFF_COL_MARGIN), nIconSize, KeepTimeSurface, DisableSurface);
	}
}

int CBuffDispBar::GetColumn()
{
	if (m_nBuffPerRowCnt == 0)
		return 0;

	int nColCnt = m_nBuffCnt / m_nBuffPerRowCnt;

	if (nColCnt == 0)
		return 0;

	int nIndex = nColCnt * m_nBuffPerRowCnt;

	while (nColCnt != 0)
	{
		if (nIndex < m_nBuffCnt)
		{
			CBuff* pBuff = m_ppArrBuff[nIndex];
			if (pBuff)
			{
				if(pBuff->m_eType) // not sure
					break;
			}
		}

		--nColCnt;

		nIndex += -m_nBuffPerRowCnt;

		if (nColCnt == 0)
			return 0;
	}

	return nColCnt;
}

VOID CBuffDispBar::OnMouseDown( const CKey& key )
{
	for( RwInt32 i = 0 ; i < m_nBuffCnt ; ++i )
	{
		if( m_ppArrBuff[i]->m_Rect.PtInRect( (RwInt32)key.m_fX, (RwInt32)key.m_fY ) )
		{
			m_ppArrBuff[i]->DropBuff();
		}
	}
}

VOID CBuffDispBar::OnMouseMove( RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos )
{
	for( RwInt32 i = 0 ; i < m_nBuffCnt ; ++i )
	{
		if( m_ppArrBuff[i]->m_Rect.PtInRect( nXPos, nYPos ) )
		{
			if( m_nMouseOnIndex == i )
				return;

			if( m_ppArrBuff[i]->m_eType != CBuff::TYPE_NONE )
			{
				CRectangle rtScreen = m_pDialog->GetScreenRect();
				m_nMouseOnIndex = i;
				m_ppArrBuff[m_nMouseOnIndex]->ShowInfoWnd( rtScreen.left, rtScreen.top, m_eDlgType );
			}			
			return;
		}
	}

	if( m_nMouseOnIndex != -1 )
	{
		if( GetInfoWndManager()->GetRequestGui() == m_eDlgType )
			GetInfoWndManager()->ShowInfoWindow( FALSE );
		
		m_nMouseOnIndex = -1;
	}
}

VOID CBuffDispBar::OnMouseLeave( gui::CComponent* pComponent )
{
	if( m_nMouseOnIndex != -1 )
	{
		if( GetInfoWndManager()->GetRequestGui() == m_eDlgType )
			GetInfoWndManager()->ShowInfoWindow( FALSE );

		m_nMouseOnIndex = -1;
	}
}

VOID CBuffDispBar::OnMove( RwInt32 nX, RwInt32 nY )
{
	CRectangle rtScreen = m_pDialog->GetScreenRect();

	for( RwInt32 i = 0 ; i < m_nBuffCnt ; ++i )
	{
		m_ppArrBuff[i]->SetMove( rtScreen.left, rtScreen.top );		
	}
}

VOID CBuffDispBar::OnPaint(VOID)
{
	for( RwInt32 i = 0 ; i < m_nBuffCnt ; ++i )
	{
		m_ppArrBuff[i]->Render();
	}
	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ! CBuffDispGui
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CBuffDispGui::CBuffDispGui(VOID)
: m_byShowBuffFlag(0), m_eIconSize( NORMAL_ICONSIZE ),
  m_nOffsetX( 0 ), m_nOffsetY( 0 ), m_fUpdateInfoTime( 0.0f ),
  m_bEnable( FALSE ), m_bShow(FALSE)
{
	m_pBuffDisplayBless = NULL;
	m_pBuffDisplayCurse = NULL;
	m_pBuffDisplaySub = NULL;
	m_hOwnerSerial = INVALID_SERIAL_ID;
	m_pBuffContainer = NULL;
}

CBuffDispGui::~CBuffDispGui(VOID)
{
	Destroy();
}

VOID CBuffDispGui::Create(BYTE byShowBuffFlag, CBuffDispGui::ICONSIZE eIconSize, RwInt32 nOffsetX, RwInt32 nOffsetY, eDialogType eDlgType, ORIGIN eOrigin )
{
	NTL_FUNCTION( "CBuffDispGui::Create" );

	m_byShowBuffFlag = byShowBuffFlag;
	m_eIconSize = eIconSize;
	
	m_nOffsetX = nOffsetX;
	m_nOffsetY = nOffsetY;
	m_eDlgType = eDlgType;
	m_eOrigin = eOrigin;

	if (BIT_FLAG_TEST(byShowBuffFlag, MAKE_BIT_FLAG(BUFF_TYPE_BLESS)))
	{
		m_pBuffDisplayBless = new CBuffDispBar(BUFF_TYPE_BLESS);
		NTL_ASSERT(m_pBuffDisplayBless, "CBuffDispGui::Create : BuffBar Alloc Fail, Not Enougn Memory");
		m_pBuffDisplayBless->Create(NTL_MAX_BLESS_BUFF_CHARACTER_HAS, eDlgType, eIconSize, eOrigin);
	}

	if (BIT_FLAG_TEST(byShowBuffFlag, MAKE_BIT_FLAG(BUFF_TYPE_CURSE)))
	{
		m_pBuffDisplayCurse = new CBuffDispBar(BUFF_TYPE_CURSE);
		NTL_ASSERT(m_pBuffDisplayCurse, "CBuffDispGui::Create : BuffBar Alloc Fail, Not Enougn Memory");
		m_pBuffDisplayCurse->Create(NTL_MAX_CURSE_BUFF_CHARACTER_HAS, eDlgType, eIconSize, eOrigin);
	}

	if (BIT_FLAG_TEST(byShowBuffFlag, MAKE_BIT_FLAG(BUFF_TYPE_SUB)))
	{
		m_pBuffDisplaySub = new CBuffDispBar(BUFF_TYPE_SUB);
		NTL_ASSERT(m_pBuffDisplaySub, "CBuffDispGui::Create : BuffBar Alloc Fail, Not Enougn Memory");
		m_pBuffDisplaySub->Create(NTL_MAX_SUB_BUFF_CHARACTER_HAS, eDlgType, eIconSize, eOrigin);
	}

	NTL_RETURNVOID();
}

VOID CBuffDispGui::Destroy(VOID)
{
	if (m_pBuffDisplayBless)
	{
		m_pBuffDisplayBless->Destroy();
		NTL_DELETE(m_pBuffDisplayBless);
	}

	if (m_pBuffDisplayCurse)
	{
		m_pBuffDisplayCurse->Destroy();
		NTL_DELETE(m_pBuffDisplayCurse);
	}

	if (m_pBuffDisplaySub)
	{
		m_pBuffDisplaySub->Destroy();
		NTL_DELETE(m_pBuffDisplaySub);
	}
}

VOID CBuffDispGui::Update( RwReal fElapsedTime )
{
	if( !m_bEnable || !m_bShow )
		return;

	if(m_pBuffDisplayBless)
		m_pBuffDisplayBless->Update(fElapsedTime);

	if(m_pBuffDisplayCurse)
		m_pBuffDisplayCurse->Update(fElapsedTime);

	if(m_pBuffDisplaySub)
		m_pBuffDisplaySub->Update(fElapsedTime);

	m_fUpdateInfoTime += fElapsedTime;

	if( m_fUpdateInfoTime > BUFF_UPDATE_TIME )
	{
		if (m_pBuffDisplayBless)
			m_pBuffDisplayBless->RefreshInfoWnd();

		if (m_pBuffDisplayCurse)
			m_pBuffDisplayCurse->RefreshInfoWnd();

		if (m_pBuffDisplaySub)
			m_pBuffDisplaySub->RefreshInfoWnd();
	}
}

VOID CBuffDispGui::SetBuffAll( RwUInt32 hSerial )
{
	CNtlSob* pSobObj = GetNtlSobManager()->GetSobObject( hSerial );
	if (pSobObj)
	{
		RwInt32 eClassID = pSobObj->GetClassID();

		if (eClassID == SLCLASS_AVATAR ||
			eClassID == SLCLASS_PLAYER ||
			eClassID == SLCLASS_MONSTER ||
			eClassID == SLCLASS_PET)
		{
			CNtlSobFeelingActor* pFeeling = reinterpret_cast<CNtlSobFeelingActor*>(pSobObj);
			NTL_ASSERT(pFeeling, "Can not convert the feel actor");

			BuffRegisterProc(pFeeling->GetBuffContainer());
			m_hOwnerSerial = hSerial;
			m_bEnable = TRUE;

			UpdateBarLocation();
		}
		else
		{
			ClearBuff();
		}
	}
}

VOID CBuffDispGui::SetBuffAll( CNtlFakeBuffContainer* pFakeBuffContainer )
{
	BuffRegisterProc( pFakeBuffContainer );
	m_bEnable = TRUE;
}

VOID CBuffDispGui::ClearBuff(VOID)
{
	BuffUnregisterProc();
	m_bEnable = FALSE;
}

VOID CBuffDispGui::SetMove( CRectangle& rtScreen )
{
	int nBuffPerRowCnt = 0;

	if (m_eDlgType == DIALOG_HP || m_eDlgType == DIALOG_TARGER_UI)
	{
		bool bIsHpDialog = (m_eDlgType == DIALOG_HP) ? true : false;

		CNtlPLGui* pCurGui = (CNtlPLGui*)GetDialogManager()->GetDialog(m_eDlgType);
		CNtlPLGui* pDestGui = (CNtlPLGui*)GetDialogManager()->GetDialog(bIsHpDialog == true ? DIALOG_TARGER_UI : DIALOG_MINIMAP); // if this is HP gui, we get target gui, otherwise, if its target gui, we get minimap gui
	
		if (pCurGui && pDestGui)
		{
			int nTemp = pDestGui->GetPosition().left - ((bIsHpDialog == true) ? 18 : 0) - m_nOffsetX;
			nBuffPerRowCnt = (nTemp - pCurGui->GetPosition().right) / (m_eIconSize + BUFF_ROW_MARGIN);
		}
	}

	int nOffsetBonus = 0;

	if (m_pBuffDisplayBless)
	{
		m_pBuffDisplayBless->SetBuffRect(m_eIconSize, nBuffPerRowCnt);
		m_pBuffDisplayBless->SetMove(rtScreen.right + m_nOffsetX, rtScreen.top + m_nOffsetY);

		nOffsetBonus = (m_pBuffDisplayBless->GetColumn() + 1) * (m_eIconSize + BUFF_COL_MARGIN);
	}

	if (m_pBuffDisplayCurse)
	{
		m_pBuffDisplayCurse->SetBuffRect(m_eIconSize, nBuffPerRowCnt);
		m_pBuffDisplayCurse->SetMove(rtScreen.right + m_nOffsetX, nOffsetBonus + rtScreen.top + m_nOffsetY);

		nOffsetBonus += (m_pBuffDisplayCurse->GetColumn() + 1) * (m_eIconSize + BUFF_COL_MARGIN);
	}

	if (m_pBuffDisplaySub)
	{
		m_pBuffDisplaySub->SetBuffRect(m_eIconSize, nBuffPerRowCnt);
		m_pBuffDisplaySub->SetMove(rtScreen.right + m_nOffsetX, nOffsetBonus + rtScreen.top + m_nOffsetY);
	}

	SetBuffAll(m_hOwnerSerial);

	if (m_pBuffContainer)
		BuffRegisterProc(m_pBuffContainer);
}

VOID CBuffDispGui::Show( bool bShow )
{
	if (m_pBuffDisplayBless)
		m_pBuffDisplayBless->Show( bShow );

	if (m_pBuffDisplayCurse)
		m_pBuffDisplayCurse->Show(bShow);

	if (m_pBuffDisplaySub)
		m_pBuffDisplaySub->Show(bShow);

	m_bShow = bShow;
}

void CBuffDispGui::UpdateBarLocation()
{
	if (m_pBuffDisplayCurse)
	{
		CRectangle rect = m_pBuffDisplayBless->GetDialog()->GetScreenRect();

		m_pBuffDisplayCurse->SetMove(rect.left, rect.bottom + BUFF_COL_MARGIN);
	}
	if (m_pBuffDisplaySub)
	{
		CRectangle rect = m_pBuffDisplayCurse->GetDialog()->GetScreenRect();

		m_pBuffDisplaySub->SetMove(rect.left, rect.bottom + BUFF_COL_MARGIN);
	}
}

VOID CBuffDispGui::BuffRegisterProc( CNtlBuffContainer* pBuffContainer )
{
	CNtlBuffContainer::ListBuff::iterator it;
	int nBuffIdx = 0, nDeBuffIdx = 0, nSubBuffIdx = 0, i;

	for( it = pBuffContainer->GetBeginBuff() ; it != pBuffContainer->GetEndBuff() ; ++it )
	{
		CNtlSobBuff* pBuff = (*it);
		CNtlSobBuffAttr* pBuffAttr = reinterpret_cast<CNtlSobBuffAttr*>( pBuff->GetSobAttr() );
		eBUFF_TYPE eBuffAttrType = BUFF_TYPE_CURSE;
		CBuff::TYPE eBuffType = CBuff::TYPE_NONE;

		if (pBuffAttr->GetBuffType() == DBO_OBJECT_SOURCE_SKILL)
		{
			eBuffType = CBuff::TYPE_SKILL;

			if (pBuffAttr->IsBlessType())
				eBuffAttrType = BUFF_TYPE_BLESS;
		}
		else if (pBuffAttr->GetBuffType() == DBO_OBJECT_SOURCE_ITEM)
		{
			eBuffType = CBuff::TYPE_ITEM;

			eBuffAttrType = BUFF_TYPE_SUB;
		}

		if( eBuffAttrType == BUFF_TYPE_BLESS)
		{
			RegisterBuffToSlot(pBuff, eBuffAttrType, eBuffType, nBuffIdx );
			++nBuffIdx;
		}
		else if( eBuffAttrType == BUFF_TYPE_CURSE)
		{
			RegisterBuffToSlot(pBuff, eBuffAttrType, eBuffType, nDeBuffIdx );
			++nDeBuffIdx;
		}
		else if(eBuffAttrType == BUFF_TYPE_SUB)
		{
			RegisterBuffToSlot(pBuff, eBuffAttrType, eBuffType, nSubBuffIdx);
			++nSubBuffIdx;
		}
	}

	for( i = nBuffIdx ; i < NTL_MAX_BLESS_BUFF_CHARACTER_HAS; i++)
		RegisterBuffToSlot(NULL, BUFF_TYPE_BLESS, CBuff::TYPE_NONE, i);

	for( i = nDeBuffIdx; i < NTL_MAX_CURSE_BUFF_CHARACTER_HAS; i++)
		RegisterBuffToSlot(NULL, BUFF_TYPE_CURSE, CBuff::TYPE_NONE, i);

	for (i = nSubBuffIdx; i < NTL_MAX_SUB_BUFF_CHARACTER_HAS; i++)
		RegisterBuffToSlot(NULL, BUFF_TYPE_SUB, CBuff::TYPE_NONE, i);


	FitBuffBarRect(nBuffIdx, nDeBuffIdx, nSubBuffIdx);


	if (m_pBuffDisplayBless)
	{
		m_pBuffDisplayBless->RefreshInfoWnd();
	}

	if (m_pBuffDisplayCurse)
	{
		m_pBuffDisplayCurse->RefreshInfoWnd();
	}

	if (m_pBuffDisplaySub)
	{
		m_pBuffDisplaySub->RefreshInfoWnd();
	}
}

VOID CBuffDispGui::BuffRegisterProc(CNtlFakeBuffContainer* pFakeBuffContainer)
{
	LIST_FAKE_BUFF_ITER it;
	RwInt32 nBuffIdx = 0, nDeBuffIdx = 0, nSubBuffIdx = 0, i;

	for( it = pFakeBuffContainer->GetBeginBuff() ; it != pFakeBuffContainer->GetEndBuff() ; ++it )
	{
		sNtlFakeBuff* pFakeBuff = (*it);
		eBUFF_TYPE eBuffAttrType = BUFF_TYPE_CURSE;
		CBuff::TYPE eBuffType = CBuff::TYPE_NONE;

		if (pFakeBuff->eType == DBO_OBJECT_SOURCE_SKILL)
		{
			eBuffType = CBuff::TYPE_SKILL;

			if (pFakeBuff->bBlessType)
				eBuffAttrType = BUFF_TYPE_BLESS;
		}
		else if (pFakeBuff->eType == DBO_OBJECT_SOURCE_ITEM)
		{
			eBuffType = CBuff::TYPE_ITEM;
			eBuffAttrType = BUFF_TYPE_SUB;
		}

		if (eBuffAttrType == BUFF_TYPE_BLESS)
		{
			RegisterBuffToSlot(pFakeBuff, eBuffAttrType, eBuffType, nBuffIdx);
			++nBuffIdx;
		}
		else if (eBuffAttrType == BUFF_TYPE_CURSE)
		{
			RegisterBuffToSlot(pFakeBuff, eBuffAttrType, eBuffType, nDeBuffIdx);
			++nDeBuffIdx;
		}
		else if (eBuffAttrType == BUFF_TYPE_SUB)
		{
			RegisterBuffToSlot(pFakeBuff, eBuffAttrType, eBuffType, nSubBuffIdx);
			++nSubBuffIdx;
		}
	}

	for (i = nBuffIdx; i < NTL_MAX_BLESS_BUFF_CHARACTER_HAS; ++i)
		RegisterBuffToSlot(NULL, BUFF_TYPE_BLESS, CBuff::TYPE_NONE, i);

	for (i = nDeBuffIdx; i < NTL_MAX_CURSE_BUFF_CHARACTER_HAS; ++i)
		RegisterBuffToSlot(NULL, BUFF_TYPE_CURSE, CBuff::TYPE_NONE, i);

	for (i = nSubBuffIdx; i < NTL_MAX_SUB_BUFF_CHARACTER_HAS; ++i)
		RegisterBuffToSlot(NULL, BUFF_TYPE_SUB, CBuff::TYPE_NONE, i);

	FitBuffBarRect(nBuffIdx, nDeBuffIdx, nSubBuffIdx);

	if (m_pBuffDisplayBless)
	{
		m_pBuffDisplayBless->RefreshInfoWnd();
	}

	if (m_pBuffDisplayCurse)
	{
		m_pBuffDisplayCurse->RefreshInfoWnd();
	}

	if (m_pBuffDisplaySub)
	{
		m_pBuffDisplaySub->RefreshInfoWnd();
	}
}

VOID CBuffDispGui::BuffUnregisterProc(VOID)
{
	RwInt32 i = 0;

	for (i = 0; i < NTL_MAX_BLESS_BUFF_CHARACTER_HAS; ++i)
		RegisterBuffToSlot(NULL, BUFF_TYPE_BLESS, CBuff::TYPE_NONE, i);

	for (i = 0; i < NTL_MAX_CURSE_BUFF_CHARACTER_HAS; ++i)
		RegisterBuffToSlot(NULL, BUFF_TYPE_CURSE, CBuff::TYPE_NONE, i);
	
	for (i = 0; i < NTL_MAX_SUB_BUFF_CHARACTER_HAS; ++i)
		RegisterBuffToSlot(NULL, BUFF_TYPE_SUB, CBuff::TYPE_NONE, i);

	FitBuffBarRect( 0, 0, 0 );

	if (m_pBuffDisplayBless)
	{
		m_pBuffDisplayBless->RefreshInfoWnd();
	}

	if (m_pBuffDisplayCurse)
	{
		m_pBuffDisplayCurse->RefreshInfoWnd();
	}

	if (m_pBuffDisplaySub)
	{
		m_pBuffDisplaySub->RefreshInfoWnd();
	}
}

VOID CBuffDispGui::RegisterBuffToSlot( VOID* pBuff, eBUFF_TYPE eBuffAttrType, CBuff::TYPE eBuffOriginType, RwInt32 nIndex )
{
	CBuffDispBar* pDispBar = GetBuffDispBar(eBuffAttrType);
	if (pDispBar)
	{
		pDispBar->SetBuff(pBuff, eBuffOriginType, nIndex);
	}
}

VOID CBuffDispGui::FitBuffBarRect( RwInt32 nBlessBuffCnt, RwInt32 nCurseBuffCnt, int nSubBuffCnt)
{
	if (m_pBuffDisplayBless)
	{
		int nHeight = m_pBuffDisplayBless->GetColumn() + 1;
		m_pBuffDisplayBless->SetSize(m_pBuffDisplayBless->GetBuffPerRowCnt() * (m_eIconSize + BUFF_ROW_MARGIN), (m_eIconSize + BUFF_COL_MARGIN) * nHeight);
	}

	if (m_pBuffDisplayCurse)
	{
		int nHeight = m_pBuffDisplayCurse->GetColumn() + 1;
		m_pBuffDisplayCurse->SetSize(m_pBuffDisplayCurse->GetBuffPerRowCnt() * (m_eIconSize + BUFF_ROW_MARGIN), (m_eIconSize + BUFF_COL_MARGIN) * nHeight);
	}

	if (m_pBuffDisplaySub)
	{
		int nHeight = m_pBuffDisplaySub->GetColumn() + 1;
		m_pBuffDisplaySub->SetSize(m_pBuffDisplaySub->GetBuffPerRowCnt() * (m_eIconSize + BUFF_ROW_MARGIN), (m_eIconSize + BUFF_COL_MARGIN) * nHeight);
	}
}

CBuffDispBar * CBuffDispGui::GetBuffDispBar(BYTE byBuffBar)
{
	if (byBuffBar == BUFF_TYPE_BLESS)
		return m_pBuffDisplayBless;
	else if (byBuffBar == BUFF_TYPE_CURSE)
		return m_pBuffDisplayCurse;
	else if (byBuffBar == BUFF_TYPE_SUB)
		return m_pBuffDisplaySub;

	return NULL;
}

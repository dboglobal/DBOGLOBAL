#include "precomp_dboclient.h"
#include "SlotGui.h"

// core
#include "NtlDebug.h"

// share
#include "ItemTable.h"
#include "SkillTable.h"
#include "HTBSetTable.h"
#include "ItemOptionTable.h"
#include "TableContainer.h"

// presentation
#include "NtlPLDef.h"
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSobIcon.h"
#include "NtlSobBuff.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"
#include "NtlSobManager.h"
#include "NtlSLLogic.h"
#include "NtlSLApi.h"

// dbo
#include "DboDef.h"
#include "DboLogic.h"
#include "DialogDefine.h"


CSlotGui::CSlotGui()
:m_dwSlotStyle(SDS_NONE)
,m_uiStackCount(0)
,m_pCount(NULL)
,m_iParentX(0)
,m_iParentY(0)
,m_iX_fromParent(0)
,m_iY_fromParent(0)
,m_iWidth(DBOGUI_ICON_SIZE)
,m_iHeight(DBOGUI_ICON_SIZE)
,m_bClipping(FALSE)
,m_bShowLock(FALSE)
,m_bShowDisable(FALSE)
{	
}

CSlotGui::~CSlotGui()
{

}

RwBool CSlotGui::Create(gui::CComponent *pParent, RwUInt32 uiOwnerDialog, DWORD dwSlotStyle /* = SDS_NONE*/)
{
	// 부모 다이얼로그 인덱스
	m_uiOwnerDialog = uiOwnerDialog;

	// 아이템 슬롯 스타일
	m_dwSlotStyle = dwSlotStyle;

	// 갯수를 표시한다면
	if( BIT_FLAG_TEST(m_dwSlotStyle, SDS_COUNT) )
	{
		CRectangle rect;
		rect.SetRectWH(0, 0, DBOGUI_STACKNUM_WIDTH, DBOGUI_STACKNUM_HEIGHT);
		m_pCount = NTL_NEW gui::CStaticBox(rect, pParent, GetNtlGuiManager()->GetSurfaceManager(), DBOGUI_STACKNUM_ALIGN );
		m_pCount->CreateFontStd(DBOGUI_STACKNUM_FONT, DBOGUI_STACKNUM_FONTHEIGHT, DBOGUI_STACKNUM_FONTATTR);
		m_pCount->SetEffectMode( DBOGUI_STACKNUM_FONTEFFECTMODE );
		m_pCount->Enable(false);
		m_pCount->SetRenderTop(false);
		m_pCount->Clear();
	}	

	// 사용금지 이미지
	if( BIT_FLAG_TEST(m_dwSlotStyle, SDS_LOCK) )
	{
		m_srfLock.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotDisableEffect") );
	}

	// 현재 조건이 맞지 않아 사용할 수 없다는 이미지 사용
	if( BIT_FLAG_TEST(m_dwSlotStyle, SDS_DISABLE) )
	{
		m_srfDisable.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotRed" ) );
	}

	return TRUE;
}

VOID CSlotGui::Clear()
{
	m_bShowLock		= FALSE;
	m_bShowDisable	= FALSE;
	SetCount(0);
	Logic_DeleteTexture( m_srfIcon.GetTexture() );
	m_srfIcon.UnsetTexture();
}

VOID CSlotGui::Destroy()
{
	Logic_DeleteTexture( m_srfIcon.GetTexture() );
	m_srfIcon.UnsetTexture();
}

RwBool CSlotGui::SetIcon(const RwChar* pcIconName)
{
	if( !pcIconName || strlen(pcIconName) == 0 )
	{
		DBO_FAIL("Icon name is wrong");
		return FALSE;
	}

	m_srfIcon.SetTexture( Logic_CreateTexture((RwChar*)pcIconName) );
	return TRUE;
}

VOID CSlotGui::SetSize(RwInt32 iSize)
{
	m_iWidth	= iSize;
	m_iHeight	= iSize;

	m_srfIcon.SetRectWH(m_iX_fromParent, m_iY_fromParent, m_iWidth, m_iHeight);

	if( BIT_FLAG_TEST(m_dwSlotStyle, SDS_LOCK) )
		m_srfLock.SetRectWH(m_iX_fromParent, m_iY_fromParent, m_iWidth, m_iHeight);

	if( BIT_FLAG_TEST(m_dwSlotStyle, SDS_DISABLE) )
		m_srfDisable.SetRectWH(m_iX_fromParent, m_iY_fromParent, m_iWidth, m_iHeight);

	if( m_pCount )
		m_pCount->SetPosition(m_iX_fromParent, m_iY_fromParent + m_iHeight - DBOGUI_STACKNUM_HEIGHT);
}

VOID CSlotGui::SetAlpha(RwUInt8 byAlpha)
{
	m_srfIcon.SetAlpha(byAlpha);

	if( BIT_FLAG_TEST(m_dwSlotStyle, SDS_LOCK) )
		m_srfLock.SetAlpha(byAlpha);

	if( BIT_FLAG_TEST(m_dwSlotStyle, SDS_DISABLE) )
		m_srfDisable.SetAlpha(byAlpha);

	if( m_pCount )
		m_pCount->SetTextAlpha(byAlpha);
}

VOID CSlotGui::SetParentPosition(int iX, int iY)
{
	RwInt32 iScreenX = m_iX_fromParent + iX;
	RwInt32 iScreenY = m_iY_fromParent + iY;

	m_iParentX = iX;
	m_iParentY = iY;

	m_srfIcon.SetRectWH(iScreenX, iScreenY, m_iWidth, m_iHeight);

	if( BIT_FLAG_TEST(m_dwSlotStyle, SDS_LOCK) )
		m_srfLock.SetRectWH(iScreenX, iScreenY, m_iWidth, m_iHeight);

	if( BIT_FLAG_TEST(m_dwSlotStyle, SDS_DISABLE) )
		m_srfDisable.SetRectWH(iScreenX, iScreenY, m_iWidth, m_iHeight);

	if( m_pCount )
		m_pCount->SetPosition(m_iX_fromParent, m_iY_fromParent + m_iHeight - DBOGUI_STACKNUM_HEIGHT);
}

VOID CSlotGui::SetPosition_fromParent(int iX, int iY)
{
	RwInt32 iScreenX = iX + m_iParentX;
	RwInt32 iScreenY = iY + m_iParentY;

	m_iX_fromParent = iX;
	m_iY_fromParent = iY;	

	m_srfIcon.SetRectWH(iScreenX, iScreenY, m_iWidth, m_iHeight);

	if( BIT_FLAG_TEST(m_dwSlotStyle, SDS_LOCK) )
		m_srfLock.SetRectWH(iScreenX, iScreenY, m_iWidth, m_iHeight);

	if( BIT_FLAG_TEST(m_dwSlotStyle, SDS_DISABLE) )
		m_srfDisable.SetRectWH(iScreenX, iScreenY, m_iWidth, m_iHeight);

	if( m_pCount )
		m_pCount->SetPosition(m_iX_fromParent, m_iY_fromParent + m_iHeight - DBOGUI_STACKNUM_HEIGHT);
}

RwInt32 CSlotGui::GetX_fromParent()
{	
	return m_iX_fromParent;
}

RwInt32 CSlotGui::GetY_fromParent()
{
	return m_iY_fromParent;
}

RwInt32 CSlotGui::GetParentX()
{	
	return m_iParentX;
}

RwInt32 CSlotGui::GetParentY()
{
	return m_iParentY;
}

int CSlotGui::GetWidth()
{
	return m_iWidth;
}

int CSlotGui::GetHeight()
{
	return m_iHeight;
}

VOID CSlotGui::SetClippingRect( CRectangle& rtClipping )
{
	m_srfIcon		.SetClippingRect( rtClipping );
	m_srfLock		.SetClippingRect( rtClipping );
	m_srfDisable	.SetClippingRect( rtClipping );
}

VOID CSlotGui::SetClippingMode( RwBool bMode )
{
	m_bClipping	= bMode;

	m_srfIcon		.SetClippingMode( bMode );
	m_srfLock		.SetClippingMode( bMode );
	m_srfDisable	.SetClippingMode( bMode );

	if( m_pCount )
		m_pCount->SetClippingMode( B2b(bMode) );
}

gui::CTexture* CSlotGui::GetTexture()
{
	return m_srfIcon.GetTexture();
}

RwUInt32 CSlotGui::GetOwnerDialog()
{
	return m_uiOwnerDialog;
}

VOID CSlotGui::SetCount(RwUInt32 uiCount)
{
	m_uiStackCount = uiCount;

	if( m_pCount )
	{
		if( 0 == m_uiStackCount )
			m_pCount->Show(false);
		else
			m_pCount->SetText(m_uiStackCount);
	}
}

RwUInt32 CSlotGui::GetCount()
{
	return m_uiStackCount;
}

VOID CSlotGui::ClickEffect(RwBool bClick)
{
#define dCLICK_ADJUST_SIZE		(2)

	if( bClick )
	{
		RwInt32 iClickedAdjustPos = dCLICK_ADJUST_SIZE;

		m_srfIcon.SetRectWH(m_iParentX + m_iX_fromParent + iClickedAdjustPos,
							m_iParentY + m_iY_fromParent + iClickedAdjustPos,
							m_iWidth - iClickedAdjustPos*dCLICK_ADJUST_SIZE,
							m_iHeight - iClickedAdjustPos*dCLICK_ADJUST_SIZE);
	}
	else
	{
		m_srfIcon.SetRectWH(m_iParentX + m_iX_fromParent,
							m_iParentY + m_iY_fromParent,
							m_iWidth,
							m_iHeight);
	}
}

VOID CSlotGui::ShowDisable( RwBool bShow )
{
	m_bShowDisable = bShow;
}

VOID CSlotGui::ShowLock(RwBool bShow)
{
	m_bShowLock = bShow;
}

RwBool CSlotGui::IsShowDisable()
{
	return m_bShowDisable;
}

RwBool CSlotGui::IsShowLock()
{
	return m_bShowLock;
}

RwBool CSlotGui::IsClippingMode()
{
	return m_bClipping;
}

RwBool CSlotGui::PtInRect(int iX, int iY)
{
	if( m_iX_fromParent <= iX && iX <= (m_iX_fromParent + m_iWidth) &&
		m_iY_fromParent <= iY && iY <= (m_iY_fromParent + m_iHeight) )
		return TRUE;

	return FALSE;
}

VOID CSlotGui::Paint(bool bRenderTop)
{
	m_srfIcon.Render(bRenderTop);

	if( m_bShowLock )
		m_srfLock.Render(bRenderTop);

	if( m_bShowDisable )
		m_srfDisable.Render(bRenderTop);
}


//////////////////////////////////////////////////////////////////////////
//	CRegularSlotGui
//////////////////////////////////////////////////////////////////////////

CRegularSlotGui::CRegularSlotGui()
:m_eSlotType(INVALID_REGULAR_SLOT_TYPE)
,m_dwFunctionFlag(SFF_NONE)
,m_uiSerial(INVALID_SERIAL_ID)
,m_uiPrice(0)
,m_pSobItem(NULL)
,m_pSobItemAttr(NULL)
,m_pTBLDAT(NULL)
,m_pITEM_OPTION_TBLDAT(NULL)
,m_bOnlyUsableIcon(FALSE)
{

}

CRegularSlotGui::~CRegularSlotGui()
{

}

RwBool CRegularSlotGui::Create(gui::CComponent *pParent, RwInt32 iOwnerDialog, eREGULAR_SLOT_TYPE eSlotType, RwUInt32 uiSlotStyle /* = SDS_NONE*/)
{
	if( FALSE == CSlotGui::Create(pParent, iOwnerDialog, uiSlotStyle) )
		return FALSE;

	m_eSlotType = eSlotType;

	return TRUE;
}

VOID CRegularSlotGui::Destroy()
{
	CSlotGui::Destroy();
}

VOID CRegularSlotGui::Clear()
{
	m_uiOwnerDialog			= DIALOG_UNKNOWN;		
	m_uiSerial				= INVALID_SERIAL_ID;		
	m_bOnlyUsableIcon		= FALSE;
	m_uiPrice				= 0;

	m_pSobItem				= NULL;		
	m_pSobItemAttr			= NULL;
	m_pTBLDAT				= NULL;
	m_pITEM_OPTION_TBLDAT	= NULL;	

	CSlotGui::Clear();
}

RwBool CRegularSlotGui::SetIcon(RwUInt32 uiSerial, RwUInt32 uiCount /* = 1 */)
{
	if( uiSerial == INVALID_SERIAL_ID )
		return FALSE;	

	if( m_uiSerial == uiSerial )
	{
		SetCount(uiCount);
		return TRUE;
	}


	if( m_uiSerial != INVALID_SERIAL_ID )
		Clear();

	RwChar* pcItemName = NULL;

	m_uiSerial = uiSerial;		

	switch(m_eSlotType)
	{
	case REGULAR_SLOT_ITEM_NOT_IDENTIFICATION:
	case REGULAR_SLOT_ITEM_SOB_NOT_IDENTIFICATION:
		{
			pcItemName = UNIDENTIFIED_ICON_NAME;
			break;
		}
	case REGULAR_SLOT_ITEM_TABLE:
		{				
			m_pTBLDAT = Logic_GetItemDataFromTable(uiSerial);
			if( m_pTBLDAT )
			{
				sITEM_TBLDAT* pITEM_TBLDAT = reinterpret_cast<sITEM_TBLDAT*>( m_pTBLDAT );
				pcItemName = pITEM_TBLDAT->szIcon_Name;

				if( BIT_FLAG_TEST(m_dwFunctionFlag, SFF_USE_ITEM_OPTION_TABLE) )
					m_pITEM_OPTION_TBLDAT = reinterpret_cast<sITEM_OPTION_TBLDAT*>(API_GetTableContainer()->GetItemOptionTable()->FindData(pITEM_TBLDAT->Item_Option_Tblidx));
			}
			else
			{
				// It does not return any other logic even though it does not have an item table
				DBO_FAIL("Not exist item table of index : " << uiSerial);
			}

			break;
		}
	case REGULAR_SLOT_ITEM_SOB:
		{
			m_pSobItem		= reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(uiSerial));				
			m_pSobItemAttr	= reinterpret_cast<CNtlSobItemAttr*>(m_pSobItem->GetSobAttr());
			m_pTBLDAT		= m_pSobItemAttr->GetItemTbl();

			if( m_pTBLDAT )
			{
				sITEM_TBLDAT* pITEM_TBLDAT = reinterpret_cast<sITEM_TBLDAT*>( m_pTBLDAT );

				if (m_pSobItemAttr->GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_SEAL)
					pcItemName = SEALED_ICON_NAME;
				else
					pcItemName = pITEM_TBLDAT->szIcon_Name;

				if( BIT_FLAG_TEST(m_dwFunctionFlag, SFF_USE_ITEM_OPTION_TABLE) )
					m_pITEM_OPTION_TBLDAT = reinterpret_cast<sITEM_OPTION_TBLDAT*>(API_GetTableContainer()->GetItemOptionTable()->FindData(pITEM_TBLDAT->Item_Option_Tblidx));
			}
			else
			{
				// It does not return any other logic even though it does not have an item table
				DBO_FAIL("Not exist item table of index : " << uiSerial);
			}
			break;
		}
	case REGULAR_SLOT_SKILL_TABLE:
		{
			m_pTBLDAT = (sSKILL_TBLDAT*)API_GetTableContainer()->GetSkillTable()->FindData(uiSerial);

			if( m_pTBLDAT )
			{
				sSKILL_TBLDAT* pSkillData = (sSKILL_TBLDAT*)m_pTBLDAT;
				pcItemName = pSkillData->szIcon_Name;
			}
			else
			{
				// 아이템 테이블이 없어도 다른 로직들은 동작해야 하기에 리턴하지 않는다
				DBO_FAIL("Not exist skill table of index : " << uiSerial);
			}
			break;
		}
	case REGULAR_SLOT_HTBSKILL_TABLE:
		{
			m_pTBLDAT = (sHTB_SET_TBLDAT*)API_GetTableContainer()->GetHTBSetTable()->FindData(uiSerial);

			if( m_pTBLDAT )
			{
				sHTB_SET_TBLDAT* pHTBData = (sHTB_SET_TBLDAT*)m_pTBLDAT;
				pcItemName = pHTBData->szIcon_Name;
			}
			else
			{
				// 아이템 테이블이 없어도 다른 로직들은 동작해야 하기에 리턴하지 않는다
				DBO_FAIL("Not exist htb table of index : " << uiSerial);
			}
		}
	}	

	if( FALSE == CSlotGui::SetIcon(pcItemName) )
	{
		DBO_WARNING_MESSAGE("UNABLE TO CREATE ICON: " << pcItemName);
		Clear();
		return FALSE;
	}

	SetCount(uiCount);

	return TRUE;
}

RwBool CRegularSlotGui::SetIcon(const RwChar* pcText, RwUInt32 uiCount /* = 0 */)
{
	Clear();

	if( FALSE == CSlotGui::SetIcon(pcText) )
		return FALSE;
	
	m_uiSerial			= INVALID_SERIAL_ID;
	m_bOnlyUsableIcon	= TRUE;
	SetCount(uiCount);

	return TRUE;
}

RwBool CRegularSlotGui::SetIcon(eREGULAR_SLOT_TYPE eSlotType, RwUInt32 uiSerial, RwUInt32 uiCount /* = 1 */)
{
	Clear();
	m_eSlotType = eSlotType;

	return SetIcon(uiSerial, uiCount);
}

RwBool CRegularSlotGui::SetIcon(eREGULAR_SLOT_TYPE eSlotType, const RwChar* pcText, RwUInt32 uiCount /* = 0 */)
{
	Clear();
	m_eSlotType = eSlotType;

	return SetIcon(pcText, uiCount);
}

VOID CRegularSlotGui::SetCount(RwUInt32 uiCount)
{
	m_uiStackCount = uiCount;

	if( false == BIT_FLAG_TEST(m_dwSlotStyle, SDS_COUNT) )
		return;

	sITEM_TBLDAT* pITEM_TBLDAT = GetItemTable();
	if (pITEM_TBLDAT == NULL)
		return;

	if(pITEM_TBLDAT->byMax_Stack < uiCount )
		return;	

	if( !m_pCount )
		return;

	if( m_uiStackCount >= 1 )
	{
		// 스택커블 아이템만 pITEM_TBLDAT->byMax_Stack 값이 1보다 크다
		if( (pITEM_TBLDAT->byMax_Stack > 1 ) ||
			IsOnlyUsableIcon() )
		{
			m_pCount->SetText(m_uiStackCount);
			m_pCount->Show(true);
		}
		else
			m_pCount->Show(false);
	}
	else
		m_pCount->Show(false);
}

VOID CRegularSlotGui::SetPrice(RwUInt32 uiPrice)
{
	m_uiPrice = uiPrice;
}

unsigned int CRegularSlotGui::GetPrice()
{
	return m_uiPrice;
}

RwBool CRegularSlotGui::SetSerialType(eREGULAR_SLOT_TYPE eSlotType)
{
	if( m_uiSerial != INVALID_SERIAL_ID )
	{
		DBO_FAIL("If you want to excute 'SetSerialType', you have to call 'Clear'");
		return FALSE;
	}
	
	m_eSlotType = eSlotType;
	return TRUE;
}

eREGULAR_SLOT_TYPE CRegularSlotGui::GetSerialType()
{
	return m_eSlotType;
}

unsigned int CRegularSlotGui::GetSerial()
{
	return m_uiSerial;
}

CNtlSobItem* CRegularSlotGui::GetSobItem()
{
	return m_pSobItem;
}

CNtlSobItemAttr* CRegularSlotGui::GetSobItemAttr()
{
	return m_pSobItemAttr;
}

sITEM_TBLDAT* CRegularSlotGui::GetItemTable()
{
	if( REGULAR_SLOT_ITEM_TABLE		!= m_eSlotType &&
		REGULAR_SLOT_ITEM_SOB		!= m_eSlotType )
		return NULL;

	return reinterpret_cast<sITEM_TBLDAT*>( m_pTBLDAT );
}

sSKILL_TBLDAT* CRegularSlotGui::GetSkillTable()
{
	if( REGULAR_SLOT_SKILL_TABLE != m_eSlotType )
		return NULL;

	return reinterpret_cast<sSKILL_TBLDAT*>( m_pTBLDAT );
}

sHTB_SET_TBLDAT* CRegularSlotGui::GetHTBSkillTable()
{
	if( REGULAR_SLOT_HTBSKILL_TABLE != m_eSlotType )
		return NULL;

	return reinterpret_cast<sHTB_SET_TBLDAT*>( m_pTBLDAT );
}

sITEM_OPTION_TBLDAT* CRegularSlotGui::GetItemOptionTable()
{
	return m_pITEM_OPTION_TBLDAT;
}

RwBool CRegularSlotGui::IsOnlyUsableIcon()
{
	return m_bOnlyUsableIcon;
}

VOID CRegularSlotGui::Paint(bool bRenderTop)
{
	if( m_uiSerial != INVALID_SERIAL_ID || m_bOnlyUsableIcon )
		CSlotGui::Paint(bRenderTop);
}
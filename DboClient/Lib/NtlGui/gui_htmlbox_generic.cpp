#include "gui_precomp.h"
#include "gui_htmlbox_generic.h"
#include "gui_html_doc.h"
#include "gui_componentoption.h"
#include "gui_texturemanager.h"
#include "gui_renderer.h"
#include <algorithm>

START_GUI

/**
* \brief Construction
* \param nCurrIdx		(INT) 현재 행의 Index
* \param rtClient		(CRectangle) 현재 행의 영역
* \param bFixedTopDown	(BOOL)
* \param nLineIntervalY	(INT) 현재 행 이후의 Y 간격
*/
CRowHtml_Item::CRowHtml_Item(INT nCurrIdx, CRectangle rtClient, BOOL bFixedTopDown, INT nLineIntervalY)
{
	m_nCurrIdx			= nCurrIdx;
	m_nLineIntervalY	= nLineIntervalY;	
	m_rtClipping		= rtClient;

	m_bFull				= FALSE;
	m_bFixedTopDown		= bFixedTopDown;
	m_dwWarpAlign		= HTML_WRAP_ALIGN_NONE;

	m_rtArea.left		= m_rtClipping.left;
	m_rtArea.top		= m_rtClipping.top;
	m_rtArea.right		= m_rtClipping.right;
	m_rtArea.bottom		= m_rtClipping.top;

	m_rtCurrArea		= m_rtArea;
}

/**
* \brief Destruction
*/
CRowHtml_Item::~CRowHtml_Item()
{
	// 행 안에 포함된 아이템들을 삭제
	std::list<CHtml_Item*>::iterator it;
	for(it = m_listItems.begin(); it != m_listItems.end(); it++)
	{
		NTL_DELETE( *it );
	}

	// 아이템들로 한 행이 이루어진 RowHtml_Item 삭제
	std::list<CRowHtml_Item*>::iterator rowit;
	for(rowit = m_listRowHtmlItem.begin(); rowit != m_listRowHtmlItem.end(); rowit++)
	{
		NTL_DELETE( *rowit );
	}
}

/**
* \brief 각 html item 들을 분해하여 RowHtml_Item으로 만든다.
* \param pItem			(CHtml_Item*) 
* \param nLineIntervalY	(INT)
* \return (CHtml_Item*) 다음 순서의 Item
*/
CHtml_Item* CRowHtml_Item::ItemDisposition(CHtml_Item *pItem, INT nLineIntervalY)
{
	// 현재 행이 가득 찼다면 현재 아이템들 리턴한다. ( 다음 행의 Disposition 시작 용도 )
	if(m_bFull)
		return pItem;

	if(m_listRowHtmlItem.size() > 0)
	{
		std::list<CRowHtml_Item*>::reverse_iterator rit = m_listRowHtmlItem.rbegin();
		CRowHtml_Item *pRowHtmlItem = (*rit);
		CHtml_Item *pNextItem = pRowHtmlItem->ItemDisposition(pItem, nLineIntervalY);

		if(pRowHtmlItem->IsFull())
		{
			if(pRowHtmlItem->GetItemCount() == 0)
			{
				if(m_rtCurrArea.bottom >= m_rtArea.bottom)
					m_rtArea.bottom = m_rtCurrArea.bottom;

				m_bFull = TRUE;
				return pNextItem;
			}

			INT nRowHeight = GetChildRowHtmlHeight(m_nLineIntervalY);
			CRectangle rtWrapClipping = m_rtCurrArea;
			rtWrapClipping.top = nRowHeight + nLineIntervalY;

			CRowHtml_Item *pNewRowHtmlItem = NTL_NEW CRowHtml_Item(m_nCurrIdx, rtWrapClipping, TRUE, m_nLineIntervalY);
			m_listRowHtmlItem.push_back(pNewRowHtmlItem);
		}

		return pNextItem;
	}
	else
	{
		m_dwWarpAlign |= pItem->GetWrapAlign();

		CHtml_Item *pNextItem = pItem->ItemDisposition(m_rtCurrArea, m_rtClipping, m_listItems, nLineIntervalY, m_bFixedTopDown, m_bFull);

		if(m_bFull)
		{
			if(m_rtCurrArea.bottom >= m_rtArea.bottom)
				m_rtArea.bottom = m_rtCurrArea.bottom;
		}
		else
		{
			if(m_dwWarpAlign != HTML_WRAP_ALIGN_NONE)
			{
				CRectangle rtWrapClipping = m_rtCurrArea;
				CRowHtml_Item *pWrapRowHtmlItem = NTL_NEW CRowHtml_Item(m_nCurrIdx, rtWrapClipping, TRUE, m_nLineIntervalY);
				m_listRowHtmlItem.push_back(pWrapRowHtmlItem);
			}
		}

		return pNextItem;
	}

}

/**
* \brief 현재 RowHtml_Item의 영역 값으로 전체 영역 값을 설정한다.
*/
VOID CRowHtml_Item::ItemFinishDisposition(VOID)
{
	if(m_rtCurrArea.bottom >= m_rtArea.bottom)
		m_rtArea.bottom = m_rtCurrArea.bottom;
}

VOID CRowHtml_Item::ItemTBCSorting(VOID)
{
	std::list<CHtml_Item*>::iterator it;

	for(it = m_listItems.begin(); it != m_listItems.end(); it++)
	{
		if(m_rtCurrArea.bottom >= m_rtArea.bottom)
			m_rtArea.bottom = m_rtCurrArea.bottom;

		(*it)->ItemLineBottomSorting(m_rtArea.bottom);
	}
}

/**
* \brief Item을 Texture에 알맞게 찍어낸다.
* \param nOffsetX
* \param nOffsetY
* \param rtPaint
* \param pTexture
* \param dwAlign
*/
VOID CRowHtml_Item::OnPrePaint(INT nOffsetX, INT nOffsetY, const CRectangle& rtPaint, CTexture *pTexture, DWORD dwAlign)
{
	std::list<CHtml_Item*>::iterator it;

	INT nCurrItemWidth = 0;
	INT nCx, nCy;
	INT nClipWidth = m_rtClipping.GetWidth();
	DWORD dwNewAlign = dwAlign;

	for(it = m_listItems.begin(); it != m_listItems.end(); it++)
	{
		dwNewAlign = (*it)->GetAlign();
		(*it)->GetSize(nCx, nCy);
		if((*it)->GetWrapAlign() == HTML_WRAP_ALIGN_NONE)
			nCurrItemWidth += nCx;
		else
			nClipWidth -= nCx;
	}

	nClipWidth -= GetChildRowHtmlWidth();

	INT nAdjustOffsetX = nOffsetX + GetAlignOffsetX(dwNewAlign, nClipWidth, nCurrItemWidth);

	for(it = m_listItems.begin(); it != m_listItems.end(); it++)
	{
		CHtml_Item *pHtmlItem = (*it);
		if(pHtmlItem->GetWrapAlign() != HTML_WRAP_ALIGN_NONE)
			nAdjustOffsetX = nOffsetX;

		pHtmlItem->OnPrePaint(nAdjustOffsetX, nOffsetY, rtPaint, pTexture);
	}

	dwNewAlign = dwAlign;
	std::list<CRowHtml_Item*>::iterator rowit = m_listRowHtmlItem.begin();
	for(rowit = m_listRowHtmlItem.begin(); rowit != m_listRowHtmlItem.end(); rowit++)
	{
		CRowHtml_Item *pRowHtmlItem = (*rowit);
		pRowHtmlItem->OnPrePaint(nOffsetX, nOffsetY, rtPaint, pTexture, dwNewAlign);
	}
}

/**
* \brief RowHtml이 list로 가지고 있는 Item들의 OnPaint 들을 호출한다.
* \param rtScreen	(CRectangle&) 그려질 영역
*/
VOID CRowHtml_Item::OnPaint(const CRectangle& rtScreen)
{
	std::list<CHtml_Item*>::iterator it;

	for(it = m_listItems.begin(); it != m_listItems.end(); it++)
	{
		(*it)->OnPaint(rtScreen);
	}
}

VOID CRowHtml_Item::OnBackPaint( const CRectangle& rtScreen )
{
	std::list<CHtml_Item*>::iterator it;

	for(it = m_listItems.begin(); it != m_listItems.end(); it++)
	{
		(*it)->OnBackPaint(rtScreen);
	}
}

/**
* \brief 가지고 있는 Item에게 SetAlpha를 호출해준다.
* \param ucAlpha	(BYTE) 알파값 ( 0 ~ 255 )
*/
VOID CRowHtml_Item::OnSetAlpha( BYTE ucAlpha ) 
{
	CHtml_Item* pHtml = NULL;
	CHtml_PictureItem* pPicture = NULL;

	std::list<CHtml_Item*>::iterator it;
	for(it = m_listItems.begin(); it != m_listItems.end(); it++)
	{
		pHtml = (*it);

		if( pHtml->GetType() == HTML_ITEM_PICTURE )
		{
			pPicture = (CHtml_PictureItem*)pHtml;

			pPicture->OnSetAlpha( ucAlpha );
		}
	}
}

VOID CRowHtml_Item::Update( float fElasped )
{
	std::list<CHtml_Item*>::iterator it;
	for(it = m_listItems.begin(); it != m_listItems.end(); it++)
	{
		(*it)->Update( fElasped );
	}
}

/**
* \brief 현재 행이 아이템들로 가득차서 완료 되었는가?
* \return 완료 여부
*/
BOOL CRowHtml_Item::IsFull(VOID)
{
	return m_bFull;
}

/**
* \brief 현재 행이 비었는가?
* \return 비어있으면 TRUE, 비어있지 않다면 FALSE
*/
BOOL CRowHtml_Item::IsEmpty(VOID)
{
	if(m_listItems.size() == 0)
		return TRUE;

	return FALSE;
}

CRectangle CRowHtml_Item::GetArea(VOID)
{
	return m_rtArea;
}

INT CRowHtml_Item::GetAreaHeight(VOID)
{
	return m_rtArea.GetHeight();
}

INT	CRowHtml_Item::GetCurrentIndex(VOID)
{
	return m_nCurrIdx;
}

INT CRowHtml_Item::GetItemCount(VOID)
{
	return (INT)m_listItems.size();
}

VOID CRowHtml_Item::GetItemRect( std::list<CRectangle>& list ) 
{
	// m_listItem에 들어있는 아이템들의 글자 크기를 순서대로 담아온다.
	std::list<CHtml_Item*>::iterator itHtmlItem;
	for( itHtmlItem = m_listItems.begin(); itHtmlItem != m_listItems.end(); ++itHtmlItem )
	{
		CHtml_Item* pHtmlItem = (*itHtmlItem);

		// 리스트를 아이템에 넣어준다.
		if( pHtmlItem->GetType() == HTML_ITEM_STRING ||
			pHtmlItem->GetType() == HTML_ITEM_LINK )
			static_cast<CHtml_StringItem*>(pHtmlItem)->GetItemRect( list ); // 리스트를 넣는 항목
	}
}

/**
* \brief 현재 구성되어 있는 RowHtml Item들 중에서 링크 속성을 가지고 있는 영역을 담는다.
*/
VOID CRowHtml_Item::GetLinkRect( std::list<sLinkRect>& list )
{
	std::list<CHtml_Item*>::iterator itHtmlItem;
	for( itHtmlItem = m_listItems.begin(); itHtmlItem != m_listItems.end(); ++itHtmlItem )
	{
		CHtml_Item* pHtmlItem = (*itHtmlItem);

		if( pHtmlItem->GetType() == HTML_ITEM_LINK )
		{
			sLinkRect sLink;
			CHtml_LinkItem* pLinkItem = static_cast<CHtml_LinkItem*>(pHtmlItem);

			sLink.pLinkItem = pLinkItem;
			sLink.rectArea = pLinkItem->GetRect();

			list.push_back( sLink );
		}
	}
}

INT CRowHtml_Item::GetAlignOffsetX(DWORD dwAlign, INT nClipWidth, INT nItemWidth)
{
	INT nOffsetX = 0;

	if(ENABLE_COMPONENT_STYLE(dwAlign,COMP_TEXT_CENTER))
	{
		nOffsetX += (nClipWidth - nItemWidth)/2;
		if(nOffsetX < 0)
			nOffsetX = 0;
	}
	else if(ENABLE_COMPONENT_STYLE(dwAlign,COMP_TEXT_RIGHT))
	{
		nOffsetX += nClipWidth - nItemWidth - 2;
		if(nOffsetX < 0)
			nOffsetX = 0;
	}

	return nOffsetX;
}

INT	CRowHtml_Item::GetChildRowHtmlWidth(VOID)
{
	if(m_listRowHtmlItem.size() == 0)
		return 0;
	else
		return m_rtCurrArea.GetWidth();
}

INT CRowHtml_Item::GetChildRowHtmlHeight(INT nLineIntervalY)
{
	INT nHeight = 0;

	if(m_listRowHtmlItem.size() > 0)
	{
		std::list<CRowHtml_Item*>::reverse_iterator rit = m_listRowHtmlItem.rbegin();
		CRectangle rtArea = (*rit)->GetArea();
		nHeight = rtArea.bottom;
	}

	return nHeight;
}






/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

HtmlLinkTagFunc CHtmlBox_Generic::m_fnHtmlLinkTag = NULL;

CHtmlBox_Generic::CHtmlBox_Generic(CHtmlBox *pSelf, BOOL bDynamic /* = FALSE */)
: m_bDynamic( bDynamic )
{
	m_pHtmlBox		= pSelf;
	m_pHtmlDoc		= NULL;
	m_nLineInterY	= HTML_LINE_INTERVAL;
	m_dwStyle		= 0;
	//m_dwStyle		= COMP_TEXT_LEFT | COMP_TEXT_UP;
	m_nItemCx		= 0;
	m_nItemCy		= 0;
	m_nVScrollPos	= 0;

	m_BgColor		= RGB(0, 0, 0);

	m_TextSurface.m_pTexture = NULL;

	// signals
	m_SlotSetOption = pSelf->SigSetOptions().Connect(this, &CHtmlBox_Generic::OnSetOptions);
	m_SlotPaint = pSelf->SigPaint().Connect(this, &CHtmlBox_Generic::OnPaint);
	m_SlotMove = pSelf->SigMove().Connect(this, &CHtmlBox_Generic::OnMove);
	m_SlotResize = pSelf->SigResize().Connect(this, &CHtmlBox_Generic::OnResize);

	m_SlotSetColor = pSelf->SigSetColor().Connect( this, &CHtmlBox_Generic::OnSetColor );
	m_SlotSetAlpha = pSelf->SigSetAlpha().Connect( this, &CHtmlBox_Generic::OnSetAlpha );
	m_SlotScrollChange = pSelf->m_pScrollBar->SigValueChanged().Connect( this, &CHtmlBox_Generic::OnScrollMove );
	m_SlotSliderMove = pSelf->m_pScrollBar->SigSliderMoved().Connect( this, &CHtmlBox_Generic::OnScrollMove );
	m_SlotMouseMove = pSelf->SigMouseMove().Connect( this, &CHtmlBox_Generic::OnMouseMove );
	m_SlotMouseDown = pSelf->SigMouseDown().Connect( this, &CHtmlBox_Generic::OnMouseDown );
	m_SlotMouseUp = pSelf->SigMouseUp().Connect( this, &CHtmlBox_Generic::OnMouseUp );

	// Intervals Text
	m_bIntervalText = FALSE;
	m_bPause = FALSE;
	m_bDonePage = FALSE;
	m_bImmediate = FALSE;

	m_fIntervalTime = 0.0f;
	m_pListRectangle = NULL;

	m_vecNewLineIdx.clear();
	m_vecNewPageIdx.clear();

	m_nPageCount = 1;		// Default 1 page

	m_fElapsedTime = 0.0f;
	m_nCurrentPage = 0;
	m_nCurrentRow = 0;

	// weight
	m_fRatio = 0.0f;

	// default count
	m_nPresentNum = 1;

	// Dynamic 속성
	m_nMaxCX = m_pHtmlBox->GetScreenRect().GetWidth();
	m_nMaxCY = m_pHtmlBox->GetScreenRect().GetHeight();
	m_nMinCX = 0;
	m_nMinCY = 0;

	// Dynamic일 때 계산의 기준이 되는 Rect( Screen, Client )
	m_rectScreen = m_pHtmlBox->GetScreenRect();
	m_rectClient = m_pHtmlBox->GetClientRect();

	m_nMaxWidth = 0;

	// Link 속성
	m_itCaptureLink = m_listDuplicateLink.end();
	m_pListLink = NULL;
}

CHtmlBox_Generic::~CHtmlBox_Generic()
{
	DeleteIntervalLink();
	DeleteIntervalText();
	DeleteTextTexture();
	DeleteItem();
}

VOID CHtmlBox_Generic::SetHtmlFromFile(const CHAR *pFileName)
{
	SetVScrollEnable(FALSE);

	DeleteItem();
	m_pHtmlDoc = NTL_NEW CHtmlDoc;
	m_pHtmlDoc->SetDefaultAlign(COMP_TEXT_LEFT | COMP_TEXT_UP);
	if( m_fRatio > 0.0f )
		m_pHtmlDoc->SetFontRatio( m_fRatio );
	m_pHtmlDoc->SetTextBackgroundColor(m_BgColor);
	m_pHtmlDoc->SetHtmlFromFile(m_pHtmlBox, pFileName);

	// 가장 사이즈가 긴 Item을 찾아서 거기에 맞춘다.
	if( m_bDynamic )
		AdjustToWidthString();

	ItemDisposition();
	ItemLineSorting();

	if(m_bVScrollEnable)
	{
		ItemDisposition();
		ItemLineSorting();
	}

	SetVScrollPos(0);

	// 다이나믹 속성일 때는 높이를 조절한다.
	if( m_bDynamic )
		AdjustToHeightString();
}

VOID CHtmlBox_Generic::SetHtmlFromMemory(const WCHAR *pMemory, INT nSize)
{
	SetVScrollEnable(FALSE);

	DeleteItem();
	m_pHtmlDoc = NTL_NEW CHtmlDoc;
	if( m_fRatio > 0.0f )
		m_pHtmlDoc->SetFontRatio( m_fRatio );
	m_pHtmlDoc->SetTextBackgroundColor(m_BgColor);
	m_pHtmlDoc->SetHtmlFromMemory(m_pHtmlBox, pMemory, nSize);

	if( m_bDynamic )
		AdjustToWidthString();

	ItemDisposition();
	ItemLineSorting();

	if(m_bVScrollEnable)
	{
		ItemDisposition();
		ItemLineSorting();
	}

	SetVScrollPos(0);

	if( m_bDynamic )
		AdjustToHeightString();

//	AdjustVerticalAlign( m_dwStyle );
}

/**
* \brief FONT의 가중치 적용
*
*/
BOOL CHtmlBox_Generic::SetFontRatio(  float fRatio  ) 
{
	// 현재 페이지가 출력중이라면.. 실패한다.
	if( m_bIntervalText && !m_bDonePage )
		return FALSE;

	if( m_fRatio <= 0.0f || m_fRatio > 10.0f )
		return FALSE;

	m_fRatio = fRatio;

	// 만약 m_pHtmlDoc이 NULL이 아니라면 로딩 후에 적용된 것이므로
	// 모든 String Item을 재배치하고 다시 그려준다.
	if( m_pHtmlDoc )
	{
		CHtmlDoc::ListHtmlItem::iterator docit;
		CHtmlDoc::ListHtmlItem listHtmlItem = m_pHtmlDoc->GetItems();

		// 모든 HtmlStringItem의 배율을 정해준다.
		for(docit = listHtmlItem.begin(); docit != listHtmlItem.end(); ++docit)
		{
			if( (*docit)->GetType() == HTML_ITEM_STRING )
			{
				CHtml_StringItem* pStringItem = (CHtml_StringItem*)(*docit);
				pStringItem->SetFontRatio( fRatio );
			}
			else if( (*docit)->GetType() == HTML_ITEM_LINK )
			{
				CHtml_LinkItem* pLinkItem = (CHtml_LinkItem*)(*docit);
				pLinkItem->SetFontRatio( fRatio );
			}
		}

		if( m_bDynamic )
			AdjustToWidthString();

		// 다시 배치하고 다시 그려준다.
		ItemDisposition();
		ItemLineSorting();

		if(m_bVScrollEnable)
		{
			ItemDisposition();
			ItemLineSorting();
		}

		SetVScrollPos(0);

		if( m_bDynamic )
			AdjustToHeightString();

//		AdjustVerticalAlign( m_dwStyle );
	}

	return TRUE;
}

/**
* \brief Style을 지정한다. 32bit 자료형
* \param dwStyle	(DWORD) bit flag로 되어 있는 자료형
*/
VOID CHtmlBox_Generic::SetStyle(DWORD dwStyle)
{
	m_dwStyle = dwStyle;
}

/**
* \brief HtmlBox의 Item들의 간격을 설정한다.
* \param nSpace	(INT) Item들의 라인 간격의 픽셀
*/
VOID CHtmlBox_Generic::SetLineSpace(INT nSpace)
{
	m_nLineInterY = nSpace;
}

/**
* \brief Html의 Text에서 출력될 배경 색깔을 설정한다.
* \param color	(COLORREF) 32bit 색상
*/
VOID CHtmlBox_Generic::SetTextBackgroundColor(COLORREF color)
{
	m_BgColor = color;
}

VOID CHtmlBox_Generic::Clear(VOID)
{
	DeleteItem();
	DeleteRenderItem();
	m_TextSurface.m_pTexture->FillClear();
}

//: Sets the surface.
VOID CHtmlBox_Generic::AddSurface( CSurface surface )
{
	CRectangle rtSnap = surface.m_SnapShot.rtRect;
	CRectangle rtScreen = m_pHtmlBox->GetScreenRect(); 

	surface.m_SnapShot.rtRect.left = rtScreen.left+rtSnap.left;
	surface.m_SnapShot.rtRect.top = rtScreen.top+rtSnap.top;
	surface.m_SnapShot.rtRect.right = surface.m_SnapShot.rtRect.left+rtSnap.GetWidth();
	surface.m_SnapShot.rtRect.bottom = surface.m_SnapShot.rtRect.top+rtSnap.GetHeight();

	m_stlSurface.push_back(surface);
}

/**
* \brief 텍스쳐를 생성한다.
*/
VOID CHtmlBox_Generic::CreateTextTexture(VOID)
{
	DeleteTextTexture();

	CRectangle rect = m_pHtmlBox->GetScreenRect();

	m_TextSurface.m_pTexture = g_TextureMgr.CreateTexture( rect.GetWidth(), rect.GetHeight() );
	m_TextSurface.m_SnapShot.uRed = 255;
	m_TextSurface.m_SnapShot.uGreen = 255;
	m_TextSurface.m_SnapShot.uBlue = 255;
	m_TextSurface.m_SnapShot.uAlpha = 255;
	m_TextSurface.m_SnapShot.UVs[0] = 0;
	m_TextSurface.m_SnapShot.UVs[1] = 0;
	m_TextSurface.m_SnapShot.UVs[2] = 1;
	m_TextSurface.m_SnapShot.UVs[3] = 1;

	m_TextSurface.m_SnapShot.rtRect.left = rect.left;
	m_TextSurface.m_SnapShot.rtRect.right = rect.left+m_TextSurface.m_pTexture->GetWidth();
	m_TextSurface.m_SnapShot.rtRect.top = rect.top;
	m_TextSurface.m_SnapShot.rtRect.bottom = rect.top+m_TextSurface.m_pTexture->GetHeight();
}

/**
* \brief 텍스쳐를 삭제한다.
*/
VOID CHtmlBox_Generic::DeleteTextTexture(VOID)
{
	if(m_TextSurface.m_pTexture)
	{
		g_TextureMgr.DeleteTexture( m_TextSurface.m_pTexture );
		m_TextSurface.m_pTexture = NULL;
	}
}

/**
* \brief IntervalText Mode On
*
* IntervalText 모드를 가능하게 하고 현재 m_pHtmlDoc이 자료를 가지고 있는 상태라면
* 다시 한번 자료의 위치를 계산하고 텍스쳐를 만들어낸다.
*
* \param fIntervalTime (float)텍스트의 한 글자가 출력되는 간격의 시간_초단위
*/
VOID CHtmlBox_Generic::SetIntervalTextEnable( float fIntervalTime ) 
{
	m_bIntervalText = TRUE;
	m_fIntervalTime = fIntervalTime;

	if( m_pHtmlDoc )
	{
		SetVScrollEnable(FALSE);

		ItemDisposition();
		ItemLineSorting();

		ItemRenderToTexture(0);
	}
}

/**
* \brief IntervalText 모드 해제
* 이전 상태의 평범한 CHtmlBox로 돌아간다. 모든 IntervalText관련 아이템들을 초기화 삭제하고
* 텍스쳐의 좌표와 UV를 회복한다.
*/
VOID CHtmlBox_Generic::SetIntervalTextDisable() 
{
	// IntervalText 관련 Item 삭제 및 초기화
	DeleteIntervalText();

	m_bIntervalText = FALSE;
	m_fIntervalTime = 0.0f;

	// 만약 m_pHtmlDoc이 Item을 가지고 있을 경우
	if( m_pHtmlDoc )
	{
		SetVScrollEnable(FALSE);

		if( m_bDynamic )
			AdjustToWidthString();

		ItemDisposition();
		ItemLineSorting();

		if(m_bVScrollEnable)
		{
			ItemDisposition();
			ItemLineSorting();
		}

		CRectangle rect = m_pHtmlBox->GetScreenRect();

		// 텍스쳐 영역 초기화
		CreateTextTexture();

		// 다시 그리기 :: ItemRenderToTexture
		SetVScrollPos(0);

		if ( m_bDynamic )
			AdjustToHeightString();
	}
}

// 업데이트
VOID CHtmlBox_Generic::Update( float fElapsedTime  ) 
{
	if( m_listRowHtml.empty() )
		return;

	// 인터벌텍스트 모드, 페이지 완성, 잠시 멈춤 떄는 Update하지 않아도 된다.
	if( !m_bIntervalText || m_bDonePage || m_bPause )
	{
		std::list<CRowHtml_Item*>::iterator it;
		for( it = m_listRowHtml.begin(); it != m_listRowHtml.end(); ++it )
		{
			(*it)->Update( fElapsedTime );
		}

		return;
	}

	if( !m_pListRectangle )
		return;

	if( m_bImmediate )
		IntervalPageDone(m_nCurrentPage);

	// 페이지가 없으면 업데이트를 하지 않는다.
	if( m_pListRectangle[m_nCurrentPage].empty() )
		return;

	m_fElapsedTime += fElapsedTime;

	// 현재 누적된 시간이 m_fIntervalTime 보다 높으면 Update
	RwReal fRevivalTime = m_fElapsedTime / m_fIntervalTime;
	if( fRevivalTime > 1.0f )
	{
		// 정수값만 가지고 온다.
		RwInt32 nRevivalCount = (RwInt32)fRevivalTime;
		
		// 소수점만 가지고 온다.
		RwReal fPercentage = fRevivalTime - (RwReal)nRevivalCount;
		
		// 남은 타임은 저장
		m_fElapsedTime = m_fIntervalTime * fPercentage;

		RwBool bDone = FALSE;
		for( RwInt32 j=0; j < nRevivalCount && bDone == FALSE; ++ j )
		{
			for( int i=0; i< m_nPresentNum; ++i)
			{
				// 현재 반복자에 들어 있는 top이 이전의 top가 다를 경우 한줄이 넘어간 것.
				CRectangle rectScr = m_pHtmlBox->GetScreenRect();
				if( (*itListRect).top != m_nCurrentRow &&
					( itListRect != m_pListRectangle[m_nCurrentPage].begin() ) )
				{	
					// 이전의 top 갱신
					m_nCurrentRow = (*itListRect).top;

					// 한줄이 완성되어지면 m_TextSurface의 m_SnapShot에 더해준다.
					m_TextSurface.m_SnapShot.rtRect.top = rectScr.top;
					m_TextSurface.m_SnapShot.rtRect.left = rectScr.left;
					m_TextSurface.m_SnapShot.rtRect.right = rectScr.right;
					m_TextSurface.m_SnapShot.rtRect.bottom = rectScr.top + ((*itListRect).top - m_vecPageRect[m_nCurrentPage].top);

					// m_TextSurface의 UV 업데이트
					m_TextSurface.m_SnapShot.UVs[2] = (float)m_TextSurface.m_SnapShot.rtRect.GetWidth() 
						/ (float)m_TextSurface.m_pTexture->GetWidth();
					m_TextSurface.m_SnapShot.UVs[3] = (float)m_TextSurface.m_SnapShot.rtRect.GetHeight() 
						/ (float)m_TextSurface.m_pTexture->GetHeight();

					// 현재 Interval로 출력되는 Plane 정보 업데이트
					m_planeInterval.rtRect.right = m_planeInterval.rtRect.left;
					m_planeInterval.rtRect.top = ((*itListRect).top - m_vecPageRect[m_nCurrentPage].top) + rectScr.top;

					// UV 도 증가
					m_planeInterval.UVs[1] = (float)((*itListRect).top - m_vecPageRect[m_nCurrentPage].top)
						/ (float)m_TextSurface.m_pTexture->GetHeight();
				}

				// 한 글자씩 증가시킨다.
				m_planeInterval.rtRect.left = rectScr.left;

				// 이동이 되었어도 가능할수 있도록.
				m_planeInterval.rtRect.top = ((*itListRect).top - m_vecPageRect[m_nCurrentPage].top) + rectScr.top;
				m_planeInterval.rtRect.right += (*itListRect).right;
				m_planeInterval.rtRect.bottom = m_planeInterval.rtRect.top + (*itListRect).bottom;

				// UV좌표를 계산하고
				m_planeInterval.UVs[2] = (float)m_planeInterval.rtRect.GetWidth() / (float)m_TextSurface.m_pTexture->GetWidth();
				m_planeInterval.UVs[3] = m_planeInterval.UVs[1] + 
					( (float)m_planeInterval.rtRect.GetHeight() / (float)m_TextSurface.m_pTexture->GetHeight() );

				// 현재 페이지의 글자를 다 출력했다면 페이지를 완성시킨다.
				itListRect++;
				if( itListRect == m_pListRectangle[m_nCurrentPage].end() )
				{
					IntervalPageDone( m_nCurrentPage );
					bDone = TRUE;
					break;
				}
			}
		}
	}
}

/**
* \brief 페이지의 갯수를 리턴
* \return 페이지 갯수
*/
INT CHtmlBox_Generic::GetIntervalPageNums( VOID ) 
{
	return m_nPageCount;
}

INT CHtmlBox_Generic::GetCurrentPage(VOID)
{
	return m_nCurrentPage;
}

/**
* \brief 다음 페이지
* \return 성공여부
*/
BOOL CHtmlBox_Generic::SetNextPage( VOID ) 
{
	// 총 페이지수보다 작다면 현재 페이지를 증가 시켜준다.
	// 아니라면 FALSE를 리턴
	if( m_nCurrentPage < m_nPageCount-1 )
		++m_nCurrentPage;
	else
		return FALSE;

	// 증가된 페이지로 초기화
	IntervalPageInit( m_nCurrentPage );
	ItemRenderToTexture( m_vecPageRect[m_nCurrentPage].top );

	if( m_bImmediate )
		IntervalPageDone( m_nCurrentPage );

	return TRUE;
}

/**
* \brief 이전 페이지
* \return 성공여부
*/
BOOL CHtmlBox_Generic::SetPrevPage( VOID ) 
{
	// 이전 페이지로 넘김
	if( m_nCurrentPage > 0 )
		--m_nCurrentPage;
	else
		return FALSE;

	// 감소된 페이지로 초기화
	IntervalPageInit( m_nCurrentPage );
	ItemRenderToTexture( m_vecPageRect[m_nCurrentPage].top );

	if( m_bImmediate )
		IntervalPageDone( m_nCurrentPage );

	return TRUE;
}

/**
* \brief 페이지스킵
* \return 성공여부
*/
BOOL CHtmlBox_Generic::SetSkipPage( VOID ) 
{
	// IntervalText 모드가 아니라면 실패
	if( m_bIntervalText == FALSE )
		return FALSE;

	// 이미 페이지가 완성되어 있다면 실패
	if( m_bDonePage == FALSE )
	{
		IntervalPageDone( m_nCurrentPage );
		return TRUE;
	}
	else
		return FALSE;
}

/**
* \brief 페이지 초기화
* 페이지를 출력하기 위한 초기화
* \param nPage (INT)페이지 번호
*/
VOID CHtmlBox_Generic::IntervalPageInit( INT nPage ) 
{
	if( nPage >= m_nPageCount || nPage < 0 )
		return;

	// 만약 멈춰 있다면 플레이 재개
	if( m_bPause )
		m_bPause = FALSE;

	// 반복자의 위치 초기화
	itListRect = m_pListRectangle[nPage].begin();
	m_nCurrentRow = (*itListRect).top;

	// 글자 출력 효과에 사용될 Plane을 초기화한다.
	m_planeInterval.rtRect.left = m_TextSurface.m_SnapShot.rtRect.left;
	m_planeInterval.rtRect.top = m_TextSurface.m_SnapShot.rtRect.top;
	m_planeInterval.rtRect.right = m_TextSurface.m_SnapShot.rtRect.left;
	m_planeInterval.rtRect.bottom = m_TextSurface.m_SnapShot.rtRect.top;

	// UV 초기화
	m_planeInterval.UVs[0] = 0.0f;
	m_planeInterval.UVs[1] = 0.0f;
	m_planeInterval.UVs[2] = 0.0f;
	m_planeInterval.UVs[3] = 0.0f;

	// 색상을 맞춰준다. 초기화
	m_planeInterval.uAlpha = m_TextSurface.m_SnapShot.uAlpha;
	m_planeInterval.uBlue = m_TextSurface.m_SnapShot.uBlue;
	m_planeInterval.uRed = m_TextSurface.m_SnapShot.uRed;
	m_planeInterval.uGreen = m_TextSurface.m_SnapShot.uGreen;

	// 텍스쳐를 가지고 있는 부분도 초기화한다.
	m_TextSurface.m_SnapShot.rtRect.right = m_TextSurface.m_SnapShot.rtRect.left;
	m_TextSurface.m_SnapShot.rtRect.bottom = m_TextSurface.m_SnapShot.rtRect.top;

	// UV
	m_TextSurface.m_SnapShot.UVs[0] = 0.0f;
	m_TextSurface.m_SnapShot.UVs[1] = 0.0f;
	m_TextSurface.m_SnapShot.UVs[2] = 0.0f;
	m_TextSurface.m_SnapShot.UVs[3] = 0.0f;

	m_bDonePage = FALSE;

	if( m_bDynamic )
		AdjustToHeightString();
}

/**
* \brief 페이지 완료
* UV 좌표 갱신(완료)
* \param nPage (INT)페이지 번호
*/
VOID CHtmlBox_Generic::IntervalPageDone( INT nPage ) 
{
	if( nPage >= m_nPageCount )
		return;

	m_bDonePage = TRUE;

	CRectangle rectScr = m_pHtmlBox->GetScreenRect();

	INT nOffsetY = 0;

	//// Interval Text의 경우 각 페이지에 들어 있는 아이템의 크기로 Y의 Offset을 지정해주고
	//if( m_bIntervalText )
	//{
	//	if( !m_vecPageRect.empty() )
	//		nOffsetY = GetAlignOffsetY(m_dwStyle, rectScr.GetHeight(), m_vecPageRect[nPage].GetHeight());
	//}

	m_TextSurface.m_SnapShot.rtRect.left = rectScr.left;
	m_TextSurface.m_SnapShot.rtRect.top = rectScr.top + nOffsetY;
	m_TextSurface.m_SnapShot.rtRect.right = rectScr.left + m_vecPageRect[nPage].GetWidth();
	m_TextSurface.m_SnapShot.rtRect.bottom = rectScr.top + m_vecPageRect[nPage].GetHeight() + nOffsetY;

	m_TextSurface.m_SnapShot.UVs[0] = 0.0f;
	m_TextSurface.m_SnapShot.UVs[1] = 0.0f;
	m_TextSurface.m_SnapShot.UVs[2] = (float)rectScr.GetWidth() / (float)m_TextSurface.m_pTexture->GetWidth();
	m_TextSurface.m_SnapShot.UVs[3] = (float)m_vecPageRect[nPage].GetHeight() / (float)m_TextSurface.m_pTexture->GetHeight();

	// 현재 완료된 페이지를 Signal로 보낸다.
	m_SigPageDone( nPage );
}

/**
* \brief IntervalText 메모리 해제 및 초기화
* IntervalText에 사용되었던 메모리와 정보들을 삭제하고 초기화한다.
*/
VOID CHtmlBox_Generic::DeleteIntervalText( VOID ) 
{
	// Intervals Text
	if( m_pListRectangle )
	{
		// 배열을 순회하여 각 리스트를 초기화
		for( int i=0; i < m_nPageCount; ++i )
			m_pListRectangle[i].clear();

		// 배열의 삭제
		NTL_ARRAY_DELETE( m_pListRectangle );
	}

	// [br]태그의 정보 삭제
	m_vecNewLineIdx.clear();
	m_vecNewPageIdx.clear();

	// 페이지의 정보를 삭제
	m_vecPageRect.clear();

	m_nPageCount = 1;

	m_fElapsedTime = 0.0f;
	m_nCurrentPage = 0;
	m_nCurrentRow = 0;

	m_bPause = FALSE;
	m_bDonePage = FALSE;
	m_bImmediate = FALSE;
}

/**
* \brief 가장 긴 아이템을 찾은 다음 그 아이템의 길이에 맞춰 가로를 변경한다. ( Dynamic 일 경우 )
*/
VOID CHtmlBox_Generic::AdjustToWidthString( VOID ) 
{
	m_nMaxWidth = FindMaxWidthItem();
	
	CRectangle rectClient = m_pHtmlBox->GetClientRect();

	INT nResizeWidth = m_nMaxWidth;

	if( m_nMinCX > m_nMaxWidth )
	{
		nResizeWidth = m_nMinCX;
	}

	if( m_nMaxCX < m_nMaxWidth )
	{
		nResizeWidth = m_nMaxCX;
	}

	// 가로 사이즈를 맞춰준다.
	m_pHtmlBox->SetSize( nResizeWidth, rectClient.GetHeight() );
}

/**
/* \brief 현재 계산된 Item들에 맞춰서 컴포넌트의 사이즈를 변경한다.
*/
VOID CHtmlBox_Generic::AdjustToHeightString( VOID ) 
{
	INT nHeight = m_pHtmlBox->GetHeight();

	// In the Interval Text Mode, the size is calculated from the calculated page information.
	if( m_bIntervalText )
	{
		if( m_vecPageRect.empty() )
			return;

		nHeight = m_vecPageRect[m_nCurrentPage].GetHeight();
	}
	// Otherwise, it is calculated using the bottom of the Area of CRowHtml_Item at the end.
	else
	{
		if( m_listRowHtml.empty() )
			return;

		CRowHtml_Item* pItem = m_listRowHtml.back();
		CRectangle rectArea = pItem->GetArea();

		nHeight = rectArea.bottom;
	}
	//DBO_WARNING_MESSAGE("nHeight: " << nHeight << " m_nMaxCY: " << m_nMaxCY);
	// disabled by daneos to fix height issue of message box
	//if( nHeight > m_nMaxCY )
	//	nHeight = m_nMaxCY;

	if( nHeight < m_nMinCY )
		nHeight = m_nMinCY;

	m_pHtmlBox->SetSize( m_pHtmlBox->GetWidth(), nHeight);
}

/**
* \brief 페이지를 플레이한다.
* \param nPage (INT)플레이할 페이지
* \return 성공여부
*/
BOOL CHtmlBox_Generic::SetPlayPage( INT nPage ) 
{
	if( nPage >= m_nPageCount || nPage < 0 )
		return FALSE;

	if( m_bIntervalText )
	{
		m_nCurrentPage = nPage;
		IntervalPageInit( m_nCurrentPage );
		ItemRenderToTexture( m_nCurrentPage );

		if( m_bImmediate )
			IntervalPageDone( m_nCurrentPage );

		return TRUE;
	}
	else
		return FALSE;
}

/**
* \brief 일시멈춤
* \param bPause (BOOL)일시멈춤(default = TRUE)
*/
VOID CHtmlBox_Generic::SetPause( BOOL bPause /*= TRUE */ ) 
{
	m_bPause = bPause;
}

/**
* \brief 직접모드
* \param bImmediate (BOOL)직접모드(default = TRUE)
*/
VOID CHtmlBox_Generic::SetImmediate( BOOL bImmediate /*= TRUE */ ) 
{
	m_bImmediate = bImmediate;
}

/**
* \brief 한번에 출력할 글자의 갯수를 셋팅한다.
* \param nNum	한번에 출력할 갯수
*/
VOID CHtmlBox_Generic::SetPresentNum( INT nNum ) 
{
	m_nPresentNum = nNum;

	if( m_nPresentNum < 1 )
		m_nPresentNum = 1;

	if( m_nPresentNum > 10 )
		m_nPresentNum = 10;

}

/**
* \brief 한번에 출력할 갯수가 몇개인지 반환받는다.
*/
INT gui::CHtmlBox_Generic::GetPresentNum() 
{
	return m_nPresentNum;
}

/**
/* \brief m_pHtmlDoc 을 해제하고 렌더링될 Item을 초기화한다.
*/
VOID CHtmlBox_Generic::DeleteItem(VOID)
{
	NTL_DELETE( m_pHtmlDoc );

	// rendering item delete
	DeleteRenderItem();
}

/**
/* \brief m_listRowHtml 에 들어있는 렌더링될 Item들을 해제하고 list를 clear한다.
*/
VOID CHtmlBox_Generic::DeleteRenderItem(VOID)
{
	std::list<CRowHtml_Item*>::iterator it;
	for(it = m_listRowHtml.begin(); it != m_listRowHtml.end(); it++)
	{
		NTL_DELETE( *it );
	}

	m_listRowHtml.clear();
}

/**
* \brief 아이템들을 속성에 컴포넌트의 사이즈에 알맞게 넣기 위하여 재계산해서
* CRowHtml_Item으로 만든다.
*/
VOID CHtmlBox_Generic::ItemDisposition(VOID)
{
	// 새로 작성.
	DeleteIntervalText();	
	DeleteRenderItem();

	// CHtmlDoc에 들어 있는 아이템 리스트를 가지고 온다.
	CHtml_Item *pDocHtmlItem;
	CHtmlDoc::ListHtmlItem::iterator docit;
	CHtmlDoc::ListHtmlItem listHtmlItem = m_pHtmlDoc->GetItems();

	// 모든 아이템의 위치를 초기화해준다.
	for(docit = listHtmlItem.begin(); docit != listHtmlItem.end(); ++docit)
	{
		pDocHtmlItem = (*docit);
		pDocHtmlItem->ResetItemDisposition();
	}

	// 아이템들이 HtmlBox에 알맞게 들어가게 하기 위하여 초기의 계산하는 영역을 잡아준다.
	CRectangle rect = m_pHtmlBox->GetClientRect();

	// 스크롤바가 활성화 상태라면 ScrollBar 만큼 Width를 빼준다.
	if(m_bVScrollEnable)
		rect.right -= m_pHtmlBox->m_pScrollBar->GetSliderRect().GetWidth();		

	// 각 아이템들의 위치를 설정하기 아이템의 크기를 미리 설정한다.
	m_nItemCx = rect.GetWidth();
	m_nItemCy = rect.GetHeight();

	INT nCurrIdx = 0;

	rect.bottom = INT_MAX;

	CRowHtml_Item *pRowHtml = NTL_NEW CRowHtml_Item(nCurrIdx, rect, FALSE, m_nLineInterY);
	m_listRowHtml.push_back(pRowHtml);

	// CHtmlDoc에 들어있는 모든 아이템들을 현재 HtmlBox UI의 크기에 맞춰서
	// 각 행마다 CRowHtml_Item으로 나누어서 저장을 한다.
	for(docit = listHtmlItem.begin(); docit != listHtmlItem.end(); ++docit)
	{
		pDocHtmlItem = (*docit);
		
		// [br] 태그의 아이템을 기록해놓는다.
		if( pDocHtmlItem->GetType() == HTML_ITEM_NEWLINE )
			m_vecNewLineIdx.push_back( nCurrIdx );
		
		while(pDocHtmlItem)
		{
			pDocHtmlItem = pRowHtml->ItemDisposition(pDocHtmlItem, m_nLineInterY);
			if(pRowHtml->IsFull())
			{
				CRectangle rtArea = pRowHtml->GetArea();
				rect.top = rtArea.bottom + m_nLineInterY;
				pRowHtml = NTL_NEW CRowHtml_Item(++nCurrIdx, rect, FALSE, m_nLineInterY);
				m_listRowHtml.push_back(pRowHtml);
			}
		}
	}

	// 마지막으로 나온 pRowHtml 이면 전체 영역을 설정해준다.
	if(pRowHtml)
	{
		pRowHtml->ItemFinishDisposition();		
	}

	// 한 글자씩 출력 모드라면 현재 만들어진 RowHtml_Item들로 한 글자씩 다시 한번 계산을 해줘서
	// 페이지에 구분하여 들어갈 수 있도록 글자 하나씩의 위치를 계산해서 가지고 있는다.
	if( m_bIntervalText )
		IntervalTextPosCalc();

	// Link 영역을 계산한다.
	LinkRectCalc();
}

/**
/* \brief Item의 크기를 계산하고 ScrollBar가 활성화 되어야 하는지, ScrollBar의 Range 설정까지 한다.
*/
VOID CHtmlBox_Generic::ItemLineSorting(VOID)
{
	m_nItemCy = 0;

	if(m_listRowHtml.size() > 0)
	{
		std::list<CRowHtml_Item*>::reverse_iterator rit = m_listRowHtml.rbegin();
		CRectangle rtArea = (*rit)->GetArea();
		m_nItemCy = rtArea.bottom;
	}	

	// Interval Text 모드를 적용할때는 Scroll을 사용하지 않는다.
	if(m_bVScrollEnable == FALSE && m_bIntervalText == FALSE)
	{
		CRectangle rect = m_pHtmlBox->GetClientRect();
		//CRectangle rect = m_rectClient;
		if(m_nItemCy > rect.GetHeight())
			SetVScrollEnable(TRUE);
	}

	if(m_bVScrollEnable)
	{
		CRectangle rect = m_pHtmlBox->GetClientRect();
		if(m_nItemCy > rect.GetHeight())
			SetVScrollRange(m_nItemCy - rect.GetHeight());
	}
}

/**
* \brief CHtml_Item들을 Texture에 렌더링한다.
* \param nVScrollPos	(INT)	스크롤의 위치
*/
VOID CHtmlBox_Generic::ItemRenderToTexture(INT nVScrollPos)
{
	if(m_TextSurface.m_pTexture == NULL)
		return;

	// texture clear
	m_TextSurface.m_pTexture->FillClear();

	// vertical align
	CRectangle rect = m_pHtmlBox->GetClientRect();

	// Item 을 렌더링 할 때는 스크롤 Offset만 렌더링한다.
	INT nOffsetX = 0;
	INT nOffsetY = -nVScrollPos;

	std::list<CRowHtml_Item*>::iterator it;
	for(it = m_listRowHtml.begin(); it != m_listRowHtml.end(); it++)
	{
		(*it)->OnPrePaint(nOffsetX, nOffsetY, rect, m_TextSurface.m_pTexture, m_dwStyle);
	}

}

/**
* \brief dwAlign에 맞춰서 X의 Offset값을 계산한다.
* \param dwAlign	(DWORD) 정렬 속성
* \param nClipWidth	(INT) 계산할 X 영역
* \param nItemWidth	(INT) 영역 안에 들어가 있는 아이템의 크기
* \return Style에 맞는 X의 Offset 값
*/
INT CHtmlBox_Generic::GetAlignOffsetX(DWORD dwAlign, INT nClipWidth, INT nItemWidth)
{
	INT nOffsetX = 0;

	if(ENABLE_COMPONENT_STYLE(dwAlign,COMP_TEXT_CENTER))
	{
		nOffsetX = (nClipWidth - nItemWidth)/2;
		if(nOffsetX < 0)
			nOffsetX = 0;
	}
	else if(ENABLE_COMPONENT_STYLE(dwAlign,COMP_TEXT_RIGHT))
	{
		nOffsetX = nClipWidth - nItemWidth - 2;
		if(nOffsetX < 0)
			nOffsetX = 0;
	}

	return nOffsetX;
}

/**
* \brief dwAlign에 맞춰서 Y의 Offset값을 계산한다.
* \param dwAlign		(DWORD) 정렬 속성
* \param nClipHeight	(INT) 계산할 Y 영역
* \param nItemHeight	(INT) 영역 안에 들어가 있는 아이템의 크기
* \return Style에 맞는 Y의 Offset 값
*/
INT	CHtmlBox_Generic::GetAlignOffsetY(DWORD dwAlign, INT nClipHeight, INT nItemHeight)
{
	INT nOffsetY = 0;

	if(ENABLE_COMPONENT_STYLE(dwAlign,COMP_TEXT_VERTICAL_CENTER))
	{
		nOffsetY = (nClipHeight - nItemHeight)/2;
		if(nOffsetY < 0)
			nOffsetY = 0;
	}
	else if(ENABLE_COMPONENT_STYLE(dwAlign,COMP_TEXT_DOWN))
	{
		nOffsetY = nClipHeight - nItemHeight - 2;
		if(nOffsetY < 0)
			nOffsetY = 0;
	}

	return nOffsetY;
}

/**
* \brief Vertical ScrollBar의 활성화 셋팅
* \param bEnable	(BOOL) TRUE = 활성화, FALSE = 비활성화
*/
VOID CHtmlBox_Generic::SetVScrollEnable(BOOL bEnable)
{
	m_bVScrollEnable = bEnable;
	m_pHtmlBox->m_pScrollBar->SetRange( 0, 0 );

	if( bEnable )
		m_pHtmlBox->m_pScrollBar->Show( true );
	else
		m_pHtmlBox->m_pScrollBar->Show( false );
}

/**
* \brief Vertical ScrollBar의 이동거리를 설정한다.
* \param nRange		(INT) 스크롤 되는 영역의 픽셀
*/
VOID CHtmlBox_Generic::SetVScrollRange(INT nRange)
{
	m_nVScrollRange = nRange;

	m_pHtmlBox->m_pScrollBar->SetRange( 0, nRange );
}

/**
* \brief Vertical ScrollBar의 스크롤된 영역을 셋팅
* \param nPos	(INT) 스크롤된 영역의 픽셀
*/
VOID CHtmlBox_Generic::SetVScrollPos(INT nPos)
{
	m_nVScrollPos = nPos;

	// nPos만큼 Offset을 주어서 Item들을 텍스쳐에 렌더링 한다.
	ItemRenderToTexture(nPos);

	// ScrollBar의 스크롤 된 위치를 세팅한다.
	m_pHtmlBox->m_pScrollBar->SetValue( nPos );

	LinkVScrollCalc( nPos );
}

/**
* \brief 보유한 CRowHtml_Item을 IntervalText Mode로 재 계산
* 현재 보유하고 있는 CRowHtml_Item들을 기존에 정해진 Page의 기준에 맞춰서 Interval Text Mode로 계산한다.
* Component의 Script로 초기에 지정해준 값으로 계산을 해줘야 기준이 명확해진다.
*/
VOID CHtmlBox_Generic::IntervalTextPosCalc(VOID)
{
	// SetOption으로 지정된 계산 값
	CRectangle rect = m_rectClient;

	// 각 페이지마다 몇 개의 행이 들어갈지 계산하여 페이지의 갯수를 먼저 측정한다.
	// 그리고 페이지를 구분짓는 마지막 행을 기록한다.
	std::list<CRowHtml_Item*>::iterator itRowHtml;
	int nSumHeightRows = 0, nLastBrRow = 0;
	int nLiveBrRow = 0;
	CRectangle rectPage;

	for( itRowHtml = m_listRowHtml.begin(); itRowHtml != m_listRowHtml.end(); ++itRowHtml )
	{
		CRowHtml_Item* pRowHtml = (*itRowHtml);
		nSumHeightRows += pRowHtml->GetAreaHeight() + m_nLineInterY;	// 각각의 행의 높이를 더한다.

		// 현재까지 CRowHtml_Item 들의 총 높이가 현재 페이지를 벗어나면
		// 그 페이지를 구분하는 마지막 br태그를 기억한다.
		if( rect.GetHeight() < nSumHeightRows )
		{
			m_nPageCount++;

			if( nLiveBrRow == nLastBrRow )
				nLastBrRow = (*itRowHtml)->GetCurrentIndex() - 1;

			int nPrevBrOffset = pRowHtml->GetCurrentIndex() - nLastBrRow;
			std::advance( itRowHtml, -nPrevBrOffset );

			nSumHeightRows = 0;

			// 페이지가 넘어가는 행의 번호를 기록한다.
			m_vecNewPageIdx.push_back( (*itRowHtml)->GetCurrentIndex() );
			nLiveBrRow = (*itRowHtml)->GetCurrentIndex();
		}

		// 현재 페이지의 마지막 [br]태그를 기록해놓는다.
		VECINT::iterator itVecNewLine = 
			std::find( m_vecNewLineIdx.begin(), m_vecNewLineIdx.end(), pRowHtml->GetCurrentIndex() );
		if( itVecNewLine != m_vecNewLineIdx.end() )
			nLastBrRow = pRowHtml->GetCurrentIndex();
	}

	// 만약 m_pListRectangle이 이미 존재할 경우 재계산으로 여기고 삭제한다.
	NTL_ARRAY_DELETE( m_pListRectangle );

	// 페이지 갯수에 맞춰서 리스트를 만들고 그 리스트에 각각의 글자 위치를 계산하여 기록한다.

	// 페이지 할당
	if( m_nPageCount > 0 )
		m_pListRectangle = NTL_NEW LISTRECTANGLE[m_nPageCount];

	// 글자 위치 계산
	int nCurrentPage = 0;
	for( itRowHtml = m_listRowHtml.begin(); itRowHtml != m_listRowHtml.end(); ++itRowHtml )
	{
		CRowHtml_Item* pRowHtml = (*itRowHtml);

		// List에 순서대로 pRowHtml이 가지고 있는 String의 위치를 담는다.
		pRowHtml->GetItemRect( m_pListRectangle[nCurrentPage] );

		// 현재 행이 다음 페이지로 넘어가는 것인지 찾아보고 다음 페이지로 넘어가는 행이라면
		// 페이지를 증가시켜주고 페이지의 정보를 기록한다.
		int nCurrentIdx = pRowHtml->GetCurrentIndex();
		VECINT::iterator itVecNewPage = 
			std::find( m_vecNewPageIdx.begin(), m_vecNewPageIdx.end(), nCurrentIdx );

		// 저장하는 현재 페이지를 증가
		if( itVecNewPage != m_vecNewPageIdx.end() )
			++nCurrentPage;
	}

	// 마지막 페이지에 br이 들어 있을 수 있으므로 검사해준다. (그리고 마지막 페이지는 사용하지 않는다.)
	if( m_pListRectangle[m_nPageCount-1].empty() )
		m_nPageCount -= 1;

	// 초기는 첫번째 페이지로 맞춰주고 페이지의 크기를 계산한다.
	IntervalPagePosCalc();
	IntervalPageInit( 0 );
}

/**
* \brief 페이지의 위치를 계산한다.
* IntervalMode로 나뉘어진 페이지의 위치를 계산한다.
*/
VOID CHtmlBox_Generic::IntervalPagePosCalc( VOID ) 
{
	// m_vecPageRect가 비어있지 않다면 재 계산하는 것이므로 비어준다.
	if( !m_vecPageRect.empty() )
		m_vecPageRect.clear();

	CRectangle rectScr = m_pHtmlBox->GetScreenRect();

	for(int i=0; i<m_nPageCount; ++i)
	{
		CRectangle rect;

		CRectangle rectBegin = (*(m_pListRectangle[i].begin()));
		CRectangle rectEnd = (*(m_pListRectangle[i].rbegin()));

		rect.left = 0;
		rect.top = rectBegin.top;
		rect.right = rectScr.GetWidth();
		rect.bottom = rectEnd.top + rectEnd.bottom;

		m_vecPageRect.push_back( rect );
	}
}

/**
* \brief 스크립트로 속성을 읽어들인다.
* \param options (CComponentOptions&) 콤포넌트의 스크립트 페이지에서 읽어들인 속성
*/
VOID CHtmlBox_Generic::OnSetOptions(const CComponentOptions& options)
{
	if(options.Exists("x"))
	{
		m_rectScreen.left = options.GetValueAsInt("x");
		m_rectClient.left = 0;
	}

	if(options.Exists("y"))
	{
		m_rectScreen.top = options.GetValueAsInt("y");
		m_rectClient.left = 0;
	}

	if(options.Exists("width"))
	{
		m_rectScreen.right = m_rectScreen.left + options.GetValueAsInt("width");
		m_rectClient.right = options.GetValueAsInt("width");
	}

	if(options.Exists("height"))
	{
		m_rectScreen.bottom = m_rectScreen.top + options.GetValueAsInt("height");
		m_rectClient.bottom = options.GetValueAsInt("height");
	}

	if(options.Exists("surface_file"))
	{
		std::string file = options.GetValue("surface_file"); 
		CSurfaceManager *pSurfaceManager = m_pHtmlBox->GetSurfaceManager();
		if(pSurfaceManager)
		{
			int nCount = options.Count("surface");
			for(int i=0 ; i<nCount ; i++)
				AddSurface(pSurfaceManager->GetSurface(file,options.GetValue("surface",i))); 
		}
	}
	// 폰트 크기 배율
	if(options.Exists("font_ratio"))
	{
		// script로 들어오는 값을 ( 0.0 < float < 10.0 ) 으로 제한한다.
		m_fRatio = options.GetValueAsFloat("font_ratio");
		if( ( m_fRatio <= 0.0f ) | ( m_fRatio > 10.0f ) )
			m_fRatio = 1.0f;
	}
	
	// Dynamic 속성 스크립트
	if( options.Exists("dynamic") )
	{
		m_bDynamic = options.GetValueAsBool("dynamic");

		// Dynamic 속성이라면 디폴트로 최대 값이 현재 컴포넌트의 크기가 된다.
		m_nMaxCX = m_pHtmlBox->GetScreenRect().GetWidth();
		m_nMaxCY = m_pHtmlBox->GetScreenRect().GetHeight();

		if( options.Exists( "max_cx" ) )
			m_nMaxCX = options.GetValueAsInt( "max_cx" );
		if( options.Exists( "max_cy" ) )
			m_nMaxCY = options.GetValueAsInt( "max_cy" );
		if( options.Exists( "min_cx" ) )
			m_nMinCX = options.GetValueAsInt( "min_cx" );
		if( options.Exists( "min_cy" ) )
			m_nMinCY = options.GetValueAsInt( "min_cy" );
	}

	// text style
	if( options.Exists( "text_style" ) )
	{
		INT nCount = options.Count( "text_style" );
		for( INT i = 0; i < nCount; ++i )
		{
			std::string style = options.GetValue( "text_style", i );

			if(style == "left")
			{
				m_dwStyle |= COMP_TEXT_LEFT;
			}
			if(style == "center")
			{
				m_dwStyle |= COMP_TEXT_CENTER;
			}
			if(style == "right")
			{
				m_dwStyle |= COMP_TEXT_RIGHT;
			}
			if( style == "up" )
			{
				m_dwStyle |= COMP_TEXT_UP;
			}
			if( style == "down" )
			{
				m_dwStyle |= COMP_TEXT_DOWN;
			}
			if( style == "verticalcenter" )
			{
				m_dwStyle |= COMP_TEXT_VERTICAL_CENTER;
			}
		}
	}

	CreateTextTexture();
}

/**
* \brief 만들어진 RowHtml_Item들을 렌더링한다.
*/
VOID CHtmlBox_Generic::OnPaint(VOID)
{
	// HtmlBox의 뒷 배경을 Paint
	std::list<CSurface>::iterator the; 
	for(the=m_stlSurface.begin(); the!=m_stlSurface.end(); the++)
	{
		g_GuiRenderer.RenderQueue(&(*the).m_SnapShot, (*the).m_pTexture);
	}

	// BackPic들을 렌더링
	
	CRectangle rtScreen = m_pHtmlBox->GetScreenRect();

	if( m_pHtmlDoc )
	{
		std::list<CHtml_Item*>::iterator it;
		for(it = m_pHtmlDoc->m_listItem.begin(); it != m_pHtmlDoc->m_listItem.end(); it++)
		{
			(*it)->OnBackPaint(rtScreen);
		}
	}


	INT nOffsetX = 0;
	INT nOffsetY = 0;
	CRectangle rect = m_pHtmlBox->GetClientRect();

	// Interval Text의 경우 각 페이지에 들어 있는 아이템의 크기로 Y의 Offset을 지정해주고
	if( m_bIntervalText )
	{
		if( !m_vecPageRect.empty() )
		{
			nOffsetX = GetAlignOffsetX(m_dwStyle, rect.GetWidth(), m_nMaxWidth);
			nOffsetY = GetAlignOffsetY(m_dwStyle, rect.GetHeight(), m_vecPageRect[m_nCurrentPage].GetHeight());
		}
	}
	// 아닌 경우라면 전체 아이템의 높이로 계산한다.
	else
	{
		nOffsetX = GetAlignOffsetX(m_dwStyle, rect.GetWidth(), m_nMaxWidth);
		nOffsetY = GetAlignOffsetY(m_dwStyle, rect.GetHeight(), m_nItemCy);
	}

	// Offset값을 이용하여 정렬된 크기에 맞춰서 Texture를 찍어낸다.
	PlaneSnapShot sOffsetSnapShot = m_TextSurface.m_SnapShot;

	sOffsetSnapShot.rtRect.left += nOffsetX;
	sOffsetSnapShot.rtRect.top += nOffsetY;
	sOffsetSnapShot.rtRect.right += nOffsetX;
	sOffsetSnapShot.rtRect.bottom += nOffsetY;
	
	g_GuiRenderer.RenderQueue( &sOffsetSnapShot, m_TextSurface.m_pTexture );

	sOffsetSnapShot = m_planeInterval;

	sOffsetSnapShot.rtRect.left += nOffsetX;
	sOffsetSnapShot.rtRect.top += nOffsetY;
	sOffsetSnapShot.rtRect.right += nOffsetX;
	sOffsetSnapShot.rtRect.bottom += nOffsetY;

	// IntervalText Mode에서 한글자씩 출력되는 라인
	if( m_bIntervalText && !m_bDonePage )
		g_GuiRenderer.RenderQueue( &sOffsetSnapShot, m_TextSurface.m_pTexture );

	// 그림이 필요한 부분을 Paint
	std::list<CRowHtml_Item*>::iterator itPic;
	for(itPic = m_listRowHtml.begin(); itPic != m_listRowHtml.end(); itPic++)
	{
		(*itPic)->OnPaint(rtScreen);
	}
}



/**
* \brief Component가 Move 되었을 때 호출되는 함수
* \param nX		(int) nX
* \param nY		(int) nY
*/
VOID CHtmlBox_Generic::OnMove(int nX,int nY)
{

	CRectangle rtScreen = m_pHtmlBox->GetScreenRect();
	CRectangle rtRect;

	rtRect = m_TextSurface.m_SnapShot.rtRect;

	m_TextSurface.m_SnapShot.rtRect.left	= rtScreen.left;
	m_TextSurface.m_SnapShot.rtRect.top		= rtScreen.top;
	m_TextSurface.m_SnapShot.rtRect.right	= rtScreen.left + rtRect.GetWidth();
	m_TextSurface.m_SnapShot.rtRect.bottom	= rtScreen.top + rtRect.GetHeight();

	std::list<CSurface>::iterator the;
	for(the=m_stlSurface.begin(); the!=m_stlSurface.end(); the++)
	{
		rtRect = (*the).m_Original.rtRect;

		(*the).m_SnapShot.rtRect.left	= rtRect.left + rtScreen.left;
		(*the).m_SnapShot.rtRect.top	= rtRect.top + rtScreen.top;
		(*the).m_SnapShot.rtRect.right	= rtRect.right + rtScreen.left;
		(*the).m_SnapShot.rtRect.bottom	= rtRect.bottom + rtScreen.top;
	}
}

VOID CHtmlBox_Generic::OnResize(int nCx, int nCy)
{
	// IntervalText Mode의 Dynamic 속성인데
	// 현재 페이지가 첫 페이지가 아닐 경우
	// 리턴
	if( m_bIntervalText && m_bDynamic )
	{
		if( m_nCurrentPage != 0 )
		{
			return;
		}
	}

	CRectangle rtClient = m_pHtmlBox->GetClientRect();

	if(rtClient.GetWidth() != nCx || rtClient.GetHeight() != nCy)
	{
		CreateTextTexture();

		SetVScrollEnable(FALSE);

		if(m_pHtmlDoc)
		{
			ItemDisposition();
			ItemLineSorting();

			if(m_bVScrollEnable)
			{
				ItemDisposition();
				ItemLineSorting();
			}

			SetVScrollPos(0);
		}
	}
}

VOID CHtmlBox_Generic::OnSetAlpha( BYTE ucAlpha )
{
	CSurface* pSurface = NULL;

	std::list<CSurface>::iterator it;
	for( it = m_stlSurface.begin() ; it != m_stlSurface.end() ; ++it )
	{
		pSurface = &(*it);

		pSurface->m_SnapShot.uAlpha = ucAlpha;
	}

	m_TextSurface.m_SnapShot.uAlpha = ucAlpha;

	// 출력되고 있는 글자
	if( m_bIntervalText )
		m_planeInterval.uAlpha = m_TextSurface.m_SnapShot.uAlpha;

	CRowHtml_Item* pRowHtml = NULL;
	std::list<CRowHtml_Item*>::iterator itRow;
	for( itRow = m_listRowHtml.begin(); itRow != m_listRowHtml.end(); ++itRow )
	{
		pRowHtml = (*itRow);

		pRowHtml->OnSetAlpha( ucAlpha );
	}
}

/**
* \brief 색상을 설정한다.
* \param ucRed		(BYTE) Red(0~255)
* \param ucGreen	(BYTE) Green(0~255)
* \param ucBlue		(BYTE) Blue(0~255)
*/
VOID CHtmlBox_Generic::OnSetColor( BYTE ucRed, BYTE ucGreen, BYTE ucBlue )
{
	CSurface* pSurface = NULL;

	std::list<CSurface>::iterator it;
	for( it = m_stlSurface.begin() ; it != m_stlSurface.end() ; ++it )
	{
		pSurface = &(*it);

		pSurface->m_SnapShot.uRed = ucRed;
		pSurface->m_SnapShot.uGreen = ucGreen;
		pSurface->m_SnapShot.uBlue = ucBlue;
	}

	m_TextSurface.m_SnapShot.uRed = ucRed;
	m_TextSurface.m_SnapShot.uGreen = ucGreen;
	m_TextSurface.m_SnapShot.uBlue = ucBlue;

	if( m_bIntervalText )
	{
		m_planeInterval.uRed = m_TextSurface.m_SnapShot.uRed = ucRed;
		m_planeInterval.uGreen = m_TextSurface.m_SnapShot.uGreen = ucGreen;
		m_planeInterval.uBlue = m_TextSurface.m_SnapShot.uBlue = ucBlue;
	}
}

/**
* \brief 스크롤이 움직였을 때 호출되는 함수
* \param nPos	(INT) 이동된 픽셀
*/
VOID CHtmlBox_Generic::OnScrollMove( INT nPos )
{
	SetVScrollPos( nPos );
}

/**
* \brief Mouse Focusing을 위하여
* \param key	(CKey&)
*/
VOID CHtmlBox_Generic::OnMouseMove( INT nKey, INT nXPos, INT nYPos )
{
	if( m_listDuplicateLink.empty() )
		return;

	LISTLINK::iterator itLink;
	for( itLink = m_listDuplicateLink.begin(); itLink != m_listDuplicateLink.end(); ++itLink )
	{
		if( (*itLink).rectArea.PtInRect( nXPos, nYPos ) )
		{
			if( (*itLink).pLinkItem->SetInverseColor( TRUE ) )
				ItemRenderToTexture(m_nVScrollPos);
			
			break;
		}
		else
		{
			if( (*itLink).pLinkItem->SetInverseColor( FALSE ) )
				ItemRenderToTexture( m_nVScrollPos);
		}
	}
}

/**
* \brief HtmlBox에서 MouseDown이 왔을 때 처리되는 함수
*/
VOID CHtmlBox_Generic::OnMouseDown( const CKey& key )
{
	if( m_bIntervalText )
	{
		// Interval Text Mode일 경우 페이지가 완성되지 않았다면 처리하지 않는다.
		if( !m_bDonePage )
			return;

		if( m_pListLink[m_nCurrentPage].empty() )
			return;

		LISTLINK::iterator itLink;
		for( itLink = m_pListLink[m_nCurrentPage].begin(); itLink != m_pListLink[m_nCurrentPage].end(); ++itLink )
		{
			if( (*itLink).rectArea.PtInRect( (INT)key.m_fX, (INT)key.m_fY ) )
			{
				m_itCaptureLink = itLink;
			}
		}
	}
	else
	{
		if( m_listDuplicateLink.empty() )
			return;

		LISTLINK::iterator itLink;
		for( itLink = m_listDuplicateLink.begin(); itLink != m_listDuplicateLink.end(); ++itLink )
		{
			if( (*itLink).rectArea.PtInRect( (INT)key.m_fX, (INT)key.m_fY ) )
			{
				m_itCaptureLink = itLink;
				break;
			}
		}
	}
}

/**
* \brief HtmlBox에서 Mouse Up이 왔을 때 처리되는 함수
* \param key		(CKey&) 마우스의 좌표
*/
VOID CHtmlBox_Generic::OnMouseUp(const CKey& key)
{
	if( m_bIntervalText )
	{
		// Interval Text Mode일 경우 페이지가 완성되지 않았다면 처리하지 않는다.
		if( !m_bDonePage )
		{
			m_itCaptureLink = m_listDuplicateLink.end();
			return;
		}

		if( m_pListLink[m_nCurrentPage].empty() )
		{
			m_itCaptureLink = m_listDuplicateLink.end();
			return;
		}

		LISTLINK::iterator itLink;
		for( itLink = m_pListLink[m_nCurrentPage].begin(); itLink != m_pListLink[m_nCurrentPage].end(); ++itLink )
		{
			if( (*itLink).rectArea.PtInRect( (INT)key.m_fX, (INT)key.m_fY ) )
			{
				if( m_itCaptureLink == itLink )
				{
					sLinkRect sLink = (*itLink);

					// 영역에 들어왔다면 Callback 호출
					if( m_fnHtmlLinkTag )
						(*m_fnHtmlLinkTag)(m_pHtmlBox, 
						sLink.pLinkItem->GetLinkType(), 
						sLink.pLinkItem->GetLinkIndex(),
						sLink.pLinkItem->GetLinkFilename());

					break;
				}
			}
		}
	}
	else
	{
		// Link 영역이 없거나 Down을 하지 않았더라면 리턴
		if( m_listDuplicateLink.empty() ||
			m_itCaptureLink == m_listDuplicateLink.end() )
			return;

		LISTLINK::iterator itLink;
		for( itLink = m_listDuplicateLink.begin(); itLink != m_listDuplicateLink.end(); ++itLink )
		{
			if( (*itLink).rectArea.PtInRect( (INT)key.m_fX, (INT)key.m_fY ) )
			{
				if( m_itCaptureLink == itLink )
				{
					sLinkRect sLink = (*itLink);

					// 영역에 들어왔다면 Callback 호출
					if( m_fnHtmlLinkTag )
						(*m_fnHtmlLinkTag)(m_pHtmlBox,
						sLink.pLinkItem->GetLinkType(), 
						sLink.pLinkItem->GetLinkIndex(),
						sLink.pLinkItem->GetLinkFilename());

					break;
				}
			}
		}

		m_itCaptureLink = m_listDuplicateLink.end();
	}
}

/**
* \brief 아이템중 가장 넓은 폭을 지닌 아이템을 구한다.
* \return 가장 넓은 폭을 가진 아이템의 포인터
*/
INT CHtmlBox_Generic::FindMaxWidthItem( VOID ) 
{
	CHtmlDoc::ListHtmlItem listItems = m_pHtmlDoc->GetItems();

	if( listItems.empty() )
		return NULL;

	INT nMaxWidth = 0;
	INT nRealWidth = 0; // 메타 태그를 포함해야한다.
	INT nWidth = 0;
	CHtml_Item* pHtmlItem = NULL;
	for( CHtmlDoc::ListHtmlItem::iterator it = listItems.begin(); it != listItems.end(); ++it )
	{
		if( (*it)->GetType() == HTML_ITEM_STRING ||
			(*it)->GetType() == HTML_ITEM_LINK )
			nWidth += (*it)->GetWidth();

		if( (*it)->GetType() == HTML_ITEM_NEWLINE )
			nWidth = 0;

		if( nMaxWidth < nWidth )
		{
			nMaxWidth = nWidth;
			pHtmlItem = (CHtml_Item*)(*it);
		}
	}

	return nMaxWidth;
}

/**
* \brief 하이퍼링크의 영역을 계산한다.
*/
VOID gui::CHtmlBox_Generic::LinkRectCalc( VOID )
{
	if( m_listRowHtml.empty() )
		return;

	if( !m_listOriginLink.empty() )
		m_listOriginLink.clear();

	std::list<CRowHtml_Item*>::iterator itRowHtml;

	for( itRowHtml = m_listRowHtml.begin(); itRowHtml != m_listRowHtml.end(); ++itRowHtml )
	{
		(*itRowHtml)->GetLinkRect( m_listOriginLink );
	}

	m_listDuplicateLink = m_listOriginLink;

	// Interval Text Mode라면 페이지별로 링크 영역을 나뉘어줘야 한다.
	if( m_bIntervalText )
	{
		DeleteIntervalLink();

		if( m_listOriginLink.empty() )
			return;
		
		// 페이지 수 별로 리스트를 생성
		if( m_nPageCount > 0 )
			m_pListLink = NTL_NEW LISTLINK[m_nPageCount];
		
		// 각 페이지별로 영역을 계산하여 그 페이지 안에 들어가는 것들끼리 담아놓는다.
		LISTLINK::iterator itOriginLink = m_listOriginLink.begin();
		int nPage = 0;
		while( itOriginLink != m_listOriginLink.end() )
		{
			if( m_vecPageRect[nPage].bottom >= (*itOriginLink).rectArea.bottom )
			{
				sLinkRect sRect;
				sRect.pLinkItem = (*itOriginLink).pLinkItem;
				if( nPage != 0 )
				{
					sRect.rectArea.top = (*itOriginLink).rectArea.top - m_vecPageRect[nPage-1].bottom;
					sRect.rectArea.bottom = (*itOriginLink).rectArea.bottom - m_vecPageRect[nPage-1].bottom;
				}
				else
				{
					sRect.rectArea.top = (*itOriginLink).rectArea.top;
					sRect.rectArea.bottom = (*itOriginLink).rectArea.bottom;
				}
				sRect.rectArea.left = (*itOriginLink).rectArea.left;
				sRect.rectArea.right = (*itOriginLink).rectArea.right;

				m_pListLink[nPage].push_back( sRect );
				++itOriginLink;
			}
			else
			{
				++nPage;
				if( nPage >= m_nPageCount )
					break;
			}
		}
	}
}

/**
* \brief 스크롤이 움직임에 따라 링크되는 영역도 재 계산해준다.
*/
VOID CHtmlBox_Generic::LinkVScrollCalc( INT nPos )
{
	// IntervalText Mode일 경우는 계산하지 않는다.
	if( m_bIntervalText )
		return;

	if( m_listOriginLink.empty() )
		return;

	LISTLINK::iterator itLink;
	LISTLINK::iterator itDuplicateLink = m_listDuplicateLink.begin();
	for( itLink = m_listOriginLink.begin(); itLink != m_listOriginLink.end(); ++itLink )
	{
		(*itDuplicateLink).rectArea.top = (*itLink).rectArea.top - nPos;
		(*itDuplicateLink).rectArea.bottom = (*itLink).rectArea.bottom - nPos;
		++itDuplicateLink;
	}
}

/**
* \brief Interval Text Mode일 때 계산해놓은 페이지별 링크 영역을 삭제한다.
*/
VOID CHtmlBox_Generic::DeleteIntervalLink( VOID )
{
	if( m_pListLink )
	{
		for( int i = 0; i < m_nPageCount-1; ++i )
		{
			m_pListLink[i].clear();
		}

		NTL_ARRAY_DELETE( m_pListLink );
	}
}

VOID CHtmlBox_Generic::LinkHtmlLinkTag( HtmlLinkTagFunc fnLinkTag )
{
	m_fnHtmlLinkTag = fnLinkTag;
}


END_GUI



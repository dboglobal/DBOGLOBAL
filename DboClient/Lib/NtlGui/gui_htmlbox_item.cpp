#include "gui_precomp.h"
#include "gui_htmlbox_item.h"
#include "gui_fontmanager.h"
#include "gui_renderer.h"

START_GUI

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Unicode

VOID CHtml_Item::SetPositionX(INT nPosX)
{
	m_nPosX = nPosX;
}

VOID CHtml_Item::SetPositionY(INT nPosY)
{
	m_nPosY = nPosY;
}

VOID CHtml_Item::SetPosition(INT nPosX, INT nPosY)
{
	m_nPosX = nPosX;
	m_nPosY = nPosY;
}

VOID CHtml_Item::SetSize(INT nCX, INT nCY)
{
	m_nCX = nCX;
	m_nCY = nCY;
}

VOID CHtml_Item::SetCurrLine(INT nCurrLine)
{
	m_nCurrLine = nCurrLine;
}


DWORD CHtml_Item::GetAlign(void)
{
	return m_dwAlign;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// new line item

CHtml_NewLineItem::CHtml_NewLineItem(CComponent* pComponent)
{
	m_pParentComp = pComponent;
	SetType(HTML_ITEM_NEWLINE);
}

CHtml_NewLineItem::~CHtml_NewLineItem()
{
}

VOID CHtml_NewLineItem::SetFont(const WCHAR *pFontName, INT nFontSize, INT nFontAttr)
{
	m_wstrFontName	= pFontName;
	m_nFontSize		= nFontSize;
	m_nFontAttr		= nFontAttr;
}

CHtml_Item* CHtml_NewLineItem::ItemDisposition(CRectangle& rect, CRectangle rtClipping, std::list<CHtml_Item*>& listItems, INT nLineIntervalY, BOOL bFixedTopBottom, BOOL& bLineOver) 
{
	if(listItems.size() <= 0)
	{
		char chBuffer[128];
		::WideCharToMultiByte( GetACP(), 0, m_wstrFontName.c_str(), -1, chBuffer, 128, NULL, NULL  );
		CGuiFont *pFont = g_FontMgr.CreateGuiFont( chBuffer, m_nFontSize, m_nFontAttr);
		CPos size = pFont->GetTextWSize(L"PrevFont", 8);
		SetSize(size.x, size.y);

		if(bFixedTopBottom)
		{
			if(rect.top + m_nCY > rtClipping.bottom)
				bLineOver = TRUE;
		}

		rect.bottom = rect.top + m_nCY;

		if(pFont)
			g_FontMgr.DeleteGuiFont(pFont);
	}
	else
	{
		SetSize(0, 0);
	}

	bLineOver = TRUE;

	return NULL;
}

VOID CHtml_NewLineItem::ItemLineBottomSorting(INT nBottom)
{

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// string item

CHtml_StringItem::CHtml_StringItem(CComponent* pComponent)
{
	m_pFont			= NULL;
	m_pUniBuffer	= NULL;
	m_nSerachIdx	= 0;
	m_nTextLen		= 0;
	m_dwAlign		= 0;
	m_fFontRatio	= 1.0f;
	m_pParentComp = pComponent;

	SetType(HTML_ITEM_STRING);
}

CHtml_StringItem::~CHtml_StringItem()
{
	NTL_DELETE( m_pUniBuffer );

	if(m_pFont)
	{
		g_FontMgr.DeleteGuiFont(m_pFont);
		m_pFont = NULL;
	}
}

VOID CHtml_StringItem::SetText(const WCHAR *pString, INT nPosX, INT nPosY, 
							   const WCHAR *pFontName, INT nFontSize, INT nFontAttr, 
							   COLORREF TextColor, COLORREF BgColor,
							   DWORD dwAlign)
{
	SetPosition(nPosX, nPosY);
	m_wstrFontName	= pFontName;
	m_nFontSize		= nFontSize;
	m_nFontAttr		= nFontAttr;
	m_TextColor		= TextColor;
	m_BgColor		= BgColor;
	m_dwAlign		= dwAlign;

	char chBuffer[128];
	::WideCharToMultiByte( GetACP(), 0, pFontName, -1, chBuffer, 128, NULL, NULL );
	m_pFont = g_FontMgr.CreateGuiFont( chBuffer , m_nFontSize, m_nFontAttr);
	m_pUniBuffer = NTL_NEW CUniBuffer(0);
	m_pUniBuffer->SetFont(m_pFont);
	m_pUniBuffer->SetSizeCalcEnable();	// 각 글자의 넓이를 계산하도록 한다.
	m_pUniBuffer->SetText(pString);

	m_nTextLen = m_pUniBuffer->GetTextSize();
	CPos size = m_pFont->GetTextWSize(m_pUniBuffer->GetBuffer(), m_nTextLen);
	SetSize(size.x, size.y);
}

VOID CHtml_StringItem::SetText(const WCHAR *pString, INT nPosX, INT nPosY, 
							   const WCHAR *pFontName, INT nFontSize, INT nFontAttr, float fFontRatio,
							   COLORREF TextColor, COLORREF BgColor,
							   DWORD dwAlign)
{
	SetPosition(nPosX, nPosY);
	m_wstrFontName	= pFontName;
	m_nFontSize		= nFontSize;
	m_nFontAttr		= nFontAttr;
	m_fFontRatio	= fFontRatio;	// font ratio
	m_TextColor		= TextColor;
	m_BgColor		= BgColor;
	m_dwAlign		= dwAlign;

	char chBuffer[128];
	::WideCharToMultiByte( GetACP(), 0, pFontName, -1, chBuffer, 128, NULL, NULL );
	m_pFont = g_FontMgr.CreateGuiFont( chBuffer , (int)((float)m_nFontSize*fFontRatio), m_nFontAttr);
	m_pUniBuffer = NTL_NEW CUniBuffer(0);
	m_pUniBuffer->SetFont(m_pFont);
	m_pUniBuffer->SetSizeCalcEnable();	// 각 글자의 넓이를 계산하도록 한다.
	m_pUniBuffer->SetText(pString);

	m_nTextLen = m_pUniBuffer->GetTextSize();
	CPos size = m_pFont->GetTextWSize(m_pUniBuffer->GetBuffer(), m_nTextLen);
	SetSize(size.x, size.y);
}


const WCHAR* CHtml_StringItem::GetText(VOID)
{
	return m_pUniBuffer->GetBuffer();
}

/**
* \brief 아이템의 색상을 반전시킨다.
*/
BOOL CHtml_LinkItem::SetInverseColor( BOOL bInverse )
{
	if( bInverse == m_bInverseColor )
		return FALSE;

	m_bInverseColor = bInverse;

	m_TextColor = ~m_TextColor;

	return TRUE;
}

VOID CHtml_StringItem::ResetItemDisposition(VOID)
{
	m_nSerachIdx = 0;
}

CHtml_Item* CHtml_StringItem::ItemDisposition(CRectangle& rect, CRectangle rtClipping, std::list<CHtml_Item*>& listItems, INT nLineIntervalY, BOOL bFixedTopBottom, BOOL& bLineOver)
{
	bLineOver = FALSE;

	if(bFixedTopBottom)
	{
		if(rect.top + m_nCY > rtClipping.bottom)
		{
			bLineOver = TRUE;
			return this;
		}
	}

	INT nCurrX = rect.left - rtClipping.left;
	INT nCurrY = rect.top - rtClipping.top;

	INT nXPos, nX2Pos, nPrevXPos = 0, nPrevIndex = m_nSerachIdx;
	WCHAR wchBuffer[1024];

	if(nPrevIndex > 0)
	{
		m_pUniBuffer->CPtoX(m_nSerachIdx, FALSE, &nPrevXPos); 
	}

	for( INT i = m_nSerachIdx; i < m_nTextLen; i++ )
	{
		m_pUniBuffer->CPtoX(i, FALSE, &nXPos); 
		m_pUniBuffer->CPtoX(i+1, FALSE, &nX2Pos);
		if(nCurrX + nX2Pos - nPrevXPos > rtClipping.GetWidth())
		{
			INT nCopyNum = i - nPrevIndex;
			if(nCopyNum == 0)
			{
				bLineOver = TRUE;
				return this;
			}
			else
			{
				memcpy(wchBuffer, m_pUniBuffer->GetBuffer() + nPrevIndex, sizeof(WCHAR)*nCopyNum);
				wchBuffer[nCopyNum] = 0;

				// string item 생성
				CHtml_StringItem *pStringItem = NTL_NEW CHtml_StringItem(m_pParentComp);
				pStringItem->SetCurrLine(0);
				pStringItem->SetText(wchBuffer, rtClipping.left + nCurrX, rtClipping.top + nCurrY, m_wstrFontName.c_str(), m_nFontSize, m_nFontAttr, m_fFontRatio, m_TextColor, m_BgColor, m_dwAlign);
				listItems.push_back(pStringItem);

				if(rect.top + m_nCY > rect.bottom)
					rect.bottom = rect.top + m_nCY;

				m_nSerachIdx = i;

				bLineOver = TRUE;
				return this;
			}
		}
	}

	INT nCopyNum = m_nTextLen - nPrevIndex;
	memcpy(wchBuffer, m_pUniBuffer->GetBuffer() + nPrevIndex, sizeof(WCHAR)*nCopyNum);
	wchBuffer[nCopyNum] = 0;

	// string item 생성
	CHtml_StringItem *pStringItem = NTL_NEW CHtml_StringItem(m_pParentComp);
	pStringItem->SetCurrLine(0);
	pStringItem->SetText(wchBuffer, rtClipping.left + nCurrX, rtClipping.top + nCurrY, m_wstrFontName.c_str(), m_nFontSize, m_nFontAttr, m_fFontRatio, m_TextColor, m_BgColor, m_dwAlign);
	listItems.push_back(pStringItem);

	rect.left += nX2Pos - nPrevXPos;

	if(rect.top + m_nCY > rect.bottom)
		rect.bottom = rect.top + m_nCY;

	return NULL;
}

VOID CHtml_StringItem::ItemLineBottomSorting(INT nBottom)
{
	SetPositionY(nBottom - m_nCY);
}

VOID CHtml_StringItem::OnPrePaint(INT nOffsetX, INT nOffsetY, const CRectangle& rect, CTexture *pTexture)
{
	if(m_pFont == NULL)
		return;

	INT nPosX = nOffsetX + m_nPosX;
	INT nPosY = nOffsetY + m_nPosY;

	if(nPosY + m_nCY < 0)
		return;

	if(nPosY >= rect.bottom)
		return;

	CRectangle rtSrc = m_pFont->TextOutW( m_pUniBuffer->GetBuffer() );   
	CRectangle rtDest(nPosX, nPosY, nPosX+m_nCX, nPosY+m_nCY);

	if(rtDest.top < 0)
	{
		rtSrc.top = abs(rtDest.top);
		rtDest.top = 0;
	}

	if(rtDest.bottom >= rect.bottom)
	{
		rtSrc.bottom -= rtDest.bottom - rect.bottom;
		rtDest.bottom = rect.bottom;
	}

	g_FontMgr.BitmapToTexture( pTexture, rtSrc, rtDest, m_TextColor );
}

VOID CHtml_StringItem::GetItemRect( std::list<CRectangle>& list ) 
{
	// UniBuffer에 들어있는 각각의 아이템들을 꺼내온다.
	// UniBuffer의 글자의 인덱스는 1부터 시작한다.
	int nUniSize = m_pUniBuffer->GetTextSize();
	for( int i = 1; i <= nUniSize; ++i )
	{
		CRectangle rect;
		rect.top = m_nPosY;
		rect.right = m_pUniBuffer->GetItemSize( i );
		rect.bottom = m_nCY;
		list.push_back( rect );
	}
}

VOID CHtml_StringItem::SetFontRatio(  float fRatio  ) 
{
	m_fFontRatio = fRatio;

	char chBuffer[128];
	::WideCharToMultiByte( GetACP(), 0, m_wstrFontName.c_str(), -1, chBuffer, 128, NULL, NULL );
	m_pFont = g_FontMgr.CreateGuiFont( chBuffer , (int)((float)m_nFontSize * fRatio) , m_nFontAttr);
	m_pUniBuffer->SetSizeCalcEnable();	// 각 글자의 넓이를 계산하도록 한다.
	m_pUniBuffer->SetFont(m_pFont);
	
		
	m_nTextLen = m_pUniBuffer->GetTextSize();
	CPos size = m_pFont->GetTextWSize(m_pUniBuffer->GetBuffer(), m_nTextLen);
	SetSize(size.x, size.y);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Link Item
CHtml_LinkItem::CHtml_LinkItem(CComponent* pComponent)
{
	m_pFont			= NULL;
	m_pUniBuffer	= NULL;
	m_nSerachIdx	= 0;
	m_nTextLen		= 0;
	m_dwAlign		= 0;
	m_fFontRatio	= 1.0f;
	m_wstrFileName.clear();
	m_nLinkIndex	= 0xFFFFFFFF;
	m_nLinkType		= eHTML_LINK_FILE;
	m_bInverseColor = FALSE;
	m_pParentComp = pComponent;

	SetType(HTML_ITEM_LINK);
}

CHtml_LinkItem::~CHtml_LinkItem()
{
	NTL_DELETE( m_pUniBuffer );

	if(m_pFont)
	{
		g_FontMgr.DeleteGuiFont(m_pFont);
		m_pFont = NULL;
	}
}

VOID CHtml_LinkItem::SetText(const WCHAR *pString, INT nPosX, INT nPosY, 
							   const WCHAR *pFontName, INT nFontSize, INT nFontAttr, 
							   COLORREF TextColor, COLORREF BgColor,
							   DWORD dwAlign)
{
	SetPosition(nPosX, nPosY);
	m_wstrFontName	= pFontName;
	m_nFontSize		= nFontSize;
	m_nFontAttr		= nFontAttr;
	m_TextColor		= TextColor;
	m_BgColor		= BgColor;
	m_dwAlign		= dwAlign;

	char chBuffer[128];
	::WideCharToMultiByte( GetACP(), 0, pFontName, -1, chBuffer, 128, NULL, NULL );
	m_pFont = g_FontMgr.CreateGuiFont( chBuffer , m_nFontSize, m_nFontAttr);
	m_pUniBuffer = NTL_NEW CUniBuffer(0);
	m_pUniBuffer->SetFont(m_pFont);
	m_pUniBuffer->SetSizeCalcEnable();	// 각 글자의 넓이를 계산하도록 한다.
	m_pUniBuffer->SetText(pString);

	m_nTextLen = m_pUniBuffer->GetTextSize();
	CPos size = m_pFont->GetTextWSize(m_pUniBuffer->GetBuffer(), m_nTextLen);
	SetSize(size.x, size.y);
}

VOID CHtml_LinkItem::SetText(const WCHAR *pString, INT nPosX, INT nPosY, 
							   const WCHAR *pFontName, INT nFontSize, INT nFontAttr, float fFontRatio,
							   COLORREF TextColor, COLORREF BgColor,
							   DWORD dwAlign)
{
	SetPosition(nPosX, nPosY);
	m_wstrFontName	= pFontName;
	m_nFontSize		= nFontSize;
	m_nFontAttr		= nFontAttr;
	m_fFontRatio	= fFontRatio;	// font ratio
	m_TextColor		= TextColor;
	m_BgColor		= BgColor;
	m_dwAlign		= dwAlign;

	char chBuffer[128];
	::WideCharToMultiByte( GetACP(), 0, pFontName, -1, chBuffer, 128, NULL, NULL );
	m_pFont = g_FontMgr.CreateGuiFont( chBuffer , (int)((float)m_nFontSize*fFontRatio), m_nFontAttr);
	m_pUniBuffer = NTL_NEW CUniBuffer(0);
	m_pUniBuffer->SetFont(m_pFont);
	m_pUniBuffer->SetSizeCalcEnable();	// 각 글자의 넓이를 계산하도록 한다.
	m_pUniBuffer->SetText(pString);

	m_nTextLen = m_pUniBuffer->GetTextSize();
	CPos size = m_pFont->GetTextWSize(m_pUniBuffer->GetBuffer(), m_nTextLen);
	SetSize(size.x, size.y);
}

VOID CHtml_LinkItem::SetLink( INT nLinkType, INT nLinkIndex )
{
	m_nLinkType = nLinkType;
	m_nLinkIndex = nLinkIndex;
}

VOID CHtml_LinkItem::SetLink( INT nLinkType, std::wstring& wstrFileName )
{
	m_nLinkType = nLinkType;
	m_wstrFileName = wstrFileName;
}

VOID CHtml_LinkItem::SetLink( INT nLinkType, INT nLinkIndex, std::wstring& wstrFileName )
{
	m_nLinkType = nLinkType;
	m_nLinkIndex = nLinkIndex;
	m_wstrFileName = wstrFileName;
}

INT CHtml_LinkItem::GetLinkType()
{
	return m_nLinkType;
}

INT CHtml_LinkItem::GetLinkIndex()
{
	return m_nLinkIndex;
}

std::wstring& CHtml_LinkItem::GetLinkFilename()
{
	return m_wstrFileName;
}

const WCHAR* CHtml_LinkItem::GetText(VOID)
{
	return m_pUniBuffer->GetBuffer();
}

VOID CHtml_LinkItem::ResetItemDisposition(VOID)
{
	m_nSerachIdx = 0;
}

CHtml_Item* CHtml_LinkItem::ItemDisposition(CRectangle& rect, CRectangle rtClipping, std::list<CHtml_Item*>& listItems, INT nLineIntervalY, BOOL bFixedTopBottom, BOOL& bLineOver)
{
	bLineOver = FALSE;

	if(bFixedTopBottom)
	{
		if(rect.top + m_nCY > rtClipping.bottom)
		{
			bLineOver = TRUE;
			return this;
		}
	}

	INT nCurrX = rect.left - rtClipping.left;
	INT nCurrY = rect.top - rtClipping.top;

	INT nXPos, nX2Pos, nPrevXPos = 0, nPrevIndex = m_nSerachIdx;
	WCHAR wchBuffer[1024];

	if(nPrevIndex > 0)
	{
		m_pUniBuffer->CPtoX(m_nSerachIdx, FALSE, &nPrevXPos); 
	}

	for( INT i = m_nSerachIdx; i < m_nTextLen; i++ )
	{
		m_pUniBuffer->CPtoX(i, FALSE, &nXPos); 
		m_pUniBuffer->CPtoX(i+1, FALSE, &nX2Pos);
		if(nCurrX + nX2Pos - nPrevXPos > rtClipping.GetWidth())
		{
			INT nCopyNum = i - nPrevIndex;
			if(nCopyNum == 0)
			{
				bLineOver = TRUE;
				return this;
			}
			else
			{
				memcpy(wchBuffer, m_pUniBuffer->GetBuffer() + nPrevIndex, sizeof(WCHAR)*nCopyNum);
				wchBuffer[nCopyNum] = 0;

				// string item 생성
				CHtml_LinkItem *pLinkItem = NTL_NEW CHtml_LinkItem(m_pParentComp);
				pLinkItem->SetCurrLine(0);
				pLinkItem->SetText(wchBuffer, rtClipping.left + nCurrX, rtClipping.top + nCurrY, m_wstrFontName.c_str(), m_nFontSize, m_nFontAttr, m_fFontRatio, m_TextColor, m_BgColor, m_dwAlign);
				/*pLinkItem->SetLink( m_nLinkType, m_wstrFileName, m_*/
				pLinkItem->SetLink( m_nLinkType, m_nLinkIndex, m_wstrFileName );
				listItems.push_back(pLinkItem);

				if(rect.top + m_nCY > rect.bottom)
					rect.bottom = rect.top + m_nCY;

				m_nSerachIdx = i;

				bLineOver = TRUE;
				return this;
			}
		}
	}

	INT nCopyNum = m_nTextLen - nPrevIndex;
	memcpy(wchBuffer, m_pUniBuffer->GetBuffer() + nPrevIndex, sizeof(WCHAR)*nCopyNum);
	wchBuffer[nCopyNum] = 0;

	// string item 생성
	CHtml_LinkItem *pLinkItem = NTL_NEW CHtml_LinkItem(m_pParentComp);
	pLinkItem->SetCurrLine(0);
	pLinkItem->SetText(wchBuffer, rtClipping.left + nCurrX, rtClipping.top + nCurrY, m_wstrFontName.c_str(), m_nFontSize, m_nFontAttr, m_fFontRatio, m_TextColor, m_BgColor, m_dwAlign);
	pLinkItem->SetLink(m_nLinkType, m_nLinkIndex, m_wstrFileName );
	listItems.push_back(pLinkItem);

	rect.left += nX2Pos - nPrevXPos;

	if(rect.top + m_nCY > rect.bottom)
		rect.bottom = rect.top + m_nCY;

	return NULL;
}

VOID CHtml_LinkItem::ItemLineBottomSorting(INT nBottom)
{
	SetPositionY(nBottom - m_nCY);
}

VOID CHtml_LinkItem::OnPrePaint(INT nOffsetX, INT nOffsetY, const CRectangle& rect, CTexture *pTexture)
{
	if(m_pFont == NULL)
		return;

	INT nPosX = nOffsetX + m_nPosX;
	INT nPosY = nOffsetY + m_nPosY;

	if(nPosY + m_nCY < 0)
		return;

	if(nPosY >= rect.bottom)
		return;

	CRectangle rtSrc = m_pFont->TextOutW( m_pUniBuffer->GetBuffer() );   
	CRectangle rtDest(nPosX, nPosY, nPosX+m_nCX, nPosY+m_nCY);

	if(rtDest.top < 0)
	{
		rtSrc.top = abs(rtDest.top);
		rtDest.top = 0;
	}

	if(rtDest.bottom >= rect.bottom)
	{
		rtSrc.bottom -= rtDest.bottom - rect.bottom;
		rtDest.bottom = rect.bottom;
	}

	g_FontMgr.BitmapToTexture( pTexture, rtSrc, rtDest, m_TextColor );
}

VOID CHtml_LinkItem::GetItemRect( std::list<CRectangle>& list ) 
{
	// UniBuffer에 들어있는 각각의 아이템들을 꺼내온다.
	// UniBuffer의 글자의 인덱스는 1부터 시작한다.
	int nUniSize = m_pUniBuffer->GetTextSize();
	for( int i = 1; i <= nUniSize; ++i )
	{
		CRectangle rect;
		rect.top = m_nPosY;
		rect.right = m_pUniBuffer->GetItemSize( i );
		rect.bottom = m_nCY;
		list.push_back( rect );
	}
}

VOID CHtml_LinkItem::SetFontRatio(  float fRatio  ) 
{
	m_fFontRatio = fRatio;

	char chBuffer[128];
	::WideCharToMultiByte( GetACP(), 0, m_wstrFontName.c_str(), -1, chBuffer, 128, NULL, NULL );
	m_pFont = g_FontMgr.CreateGuiFont( chBuffer , (int)((float)m_nFontSize * fRatio) , m_nFontAttr);
	m_pUniBuffer->SetSizeCalcEnable();	// 각 글자의 넓이를 계산하도록 한다.
	m_pUniBuffer->SetFont(m_pFont);


	m_nTextLen = m_pUniBuffer->GetTextSize();
	CPos size = m_pFont->GetTextWSize(m_pUniBuffer->GetBuffer(), m_nTextLen);
	SetSize(size.x, size.y);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// picture item

CHtml_PictureItem::CHtml_PictureItem(CComponent* pComponent)
{
	m_pTexture		= NULL;
	m_nRenderPosX	= 0;
	m_nRenderPosY	= 0;
	m_dwAlign		= 0;

	m_SnapShot.rtRect.left		= 0;
	m_SnapShot.rtRect.right		= 0;
	m_SnapShot.rtRect.top		= 0;
	m_SnapShot.rtRect.bottom	= 0;

	m_pParentComp = pComponent;

	SetType(HTML_ITEM_PICTURE);
}

CHtml_PictureItem::~CHtml_PictureItem()
{
	if(m_pTexture)
	{
		g_TextureMgr.DeleteTexture( m_pTexture );
		m_pTexture = NULL;
	}
}

VOID CHtml_PictureItem::SetPositionX(INT nPosX)
{
	CHtml_Item::SetPositionX(nPosX);

	m_SnapShot.rtRect.left		= nPosX;
	m_SnapShot.rtRect.right		= nPosX + m_nCX;
}

VOID CHtml_PictureItem::SetPositionY(INT nPosY)
{
	CHtml_Item::SetPositionY(nPosY);

	m_SnapShot.rtRect.top		= nPosY;
	m_SnapShot.rtRect.bottom	= nPosY + m_nCY;
}

VOID CHtml_PictureItem::SetPosition(INT nPosX, INT nPosY)
{
	CHtml_Item::SetPosition(nPosX, nPosY);

	m_SnapShot.rtRect.left		= nPosX;
	m_SnapShot.rtRect.right		= nPosX + m_nCX;
	m_SnapShot.rtRect.top		= nPosY;
	m_SnapShot.rtRect.bottom	= nPosY + m_nCY;
}

VOID CHtml_PictureItem::ItemLineBottomSorting(INT nBottom)
{
	SetPositionY(nBottom - m_nCY);
}

VOID CHtml_PictureItem::SetPicture(const WCHAR *pPic, INT nWidth, INT nHeight, INT nWrapAlign)
{
	char chBuffer[128];
	::WideCharToMultiByte( GetACP(), 0, pPic, -1, chBuffer, 128, NULL, NULL );

	m_strPic			= pPic;
	m_pTexture			= g_TextureMgr.CreateTexture( chBuffer );
	m_SnapShot.uRed		= 255;
	m_SnapShot.uGreen	= 255;
	m_SnapShot.uBlue	= 255;
	m_SnapShot.uAlpha	= 255;
	m_SnapShot.UVs[0]	= 0;
	m_SnapShot.UVs[1]	= 0;
	m_SnapShot.UVs[2]	= 1;
	m_SnapShot.UVs[3]	= 1;

	// nWidth나 nHeight값이 0이나 0보다 낮은 숫자라면 이미지 원본의 크기대로 출력한다.
	INT nRealWidth = nWidth;
	INT nRealHeight = nHeight;

	if( nRealWidth <= 0 )
	{
		if( m_pTexture )
			nRealWidth = m_pTexture->GetWidth();
		else
			nRealWidth = 100;
	}
	
	if( nRealHeight <= 0 )
	{
		if( m_pTexture )
			nRealHeight = m_pTexture->GetHeight();
		else
			nRealHeight = 100;
	}

	SetSize(nRealWidth, nRealHeight);
	SetWrapAlign(nWrapAlign);

	m_SnapShot.rtRect.left		= m_nPosX;
	m_SnapShot.rtRect.right		= m_nPosX + nWidth;
	m_SnapShot.rtRect.top		= m_nPosY;
	m_SnapShot.rtRect.bottom	= m_nPosY + nHeight;
}

CHtml_Item* CHtml_PictureItem::ItemDisposition(CRectangle& rect, CRectangle rtFixedClipping, std::list<CHtml_Item*>& listItems, INT nLineIntervalY, BOOL bFixedTopBottom, BOOL& bLineOver)
{
	bLineOver = FALSE;

	if(listItems.size() > 0)
	{
		if(m_nCX > rect.GetWidth())
		{
			bLineOver = TRUE;
			return this;
		}
	}

	if(m_dwWrapAlign & HTML_WRAP_ALIGN_RIGHT)
	{
		CHtml_PictureItem *pPicItem = NTL_NEW CHtml_PictureItem(m_pParentComp);
		pPicItem->SetCurrLine(0);
		pPicItem->SetPosition(rect.right - m_nCX, rect.top);
		pPicItem->SetPicture(m_strPic.c_str(), m_nCX, m_nCY, m_dwWrapAlign);
		listItems.push_back(pPicItem);

		rect.right = rect.right - m_nCX;
	}
	else if( m_dwWrapAlign & HTML_WRAP_ALIGN_CENTER )
	{
		CHtml_PictureItem *pPicItem = NTL_NEW CHtml_PictureItem(m_pParentComp);
		pPicItem->SetCurrLine(0);
		pPicItem->SetPosition((rect.GetWidth()/2) - (m_nCX/2), rect.top);
		pPicItem->SetPicture(m_strPic.c_str(), m_nCX, m_nCY, m_dwWrapAlign);
		listItems.push_back(pPicItem);

		rect.left += (rect.GetWidth()/2) + (m_nCX/2);
	}
	else
	{
		CHtml_PictureItem *pPicItem = NTL_NEW CHtml_PictureItem(m_pParentComp);
		pPicItem->SetCurrLine(0);
		pPicItem->SetPosition(rect.left, rect.top);
		pPicItem->SetPicture(m_strPic.c_str(), m_nCX, m_nCY, m_dwWrapAlign);
		listItems.push_back(pPicItem);

		rect.left += m_nCX;
	}

	if(rect.top + m_nCY > rect.bottom)
	{
		rect.bottom = rect.top + m_nCY;
	}

	return NULL;
}

VOID CHtml_PictureItem::OnPrePaint(INT nOffsetX, INT nOffsetY, const CRectangle& rect, CTexture *pTexture)
{
	m_nRenderPosX = nOffsetX + m_nPosX;
	m_nRenderPosY = nOffsetY + m_nPosY;
}

void CHtml_PictureItem::OnPaint(const CRectangle& rt)
{
	if(m_nRenderPosY + m_nCY < 0)
		return;

	m_SnapShot.rtRect.left		= rt.left + m_nRenderPosX;
	m_SnapShot.rtRect.right		= rt.left + m_nRenderPosX + m_nCX;
	if(m_nRenderPosY < 0)
		m_SnapShot.rtRect.top	= rt.top;
	else
		m_SnapShot.rtRect.top	= rt.top + m_nRenderPosY;

	INT nDeltaY = m_nRenderPosY + m_nCY - rt.GetHeight();
	if(nDeltaY > 0)
		m_SnapShot.rtRect.bottom = rt.bottom;
	else
		m_SnapShot.rtRect.bottom = rt.top + m_nRenderPosY + m_nCY;

	PlaneSnapShot sSnapShot;
	sSnapShot = m_SnapShot;

	if(m_nRenderPosY < 0)
		sSnapShot.UVs[1] = (FLOAT)abs(m_nRenderPosY)/(FLOAT)m_nCY;
	if(nDeltaY > 0)
		sSnapShot.UVs[3] = 1.0f - (FLOAT)abs(nDeltaY)/(FLOAT)m_nCY;

	g_GuiRenderer.RenderQueue(&sSnapShot, m_pTexture);
}

VOID CHtml_PictureItem::OnSetAlpha( BYTE ucAlpha ) 
{
	m_SnapShot.uAlpha = ucAlpha;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Flash Item
CHtml_FlashItem::CHtml_FlashItem(CComponent* pComponent)
{
	m_nRenderPosX	= 0;
	m_nRenderPosY	= 0;
	m_dwAlign		= 0;
	m_bAutoRewind	= 0;
	m_nResizeType	= 0;
	m_pParentComp	= pComponent;
	m_pFlash = NTL_NEW CFlash(CRectangle(0,0,0,0), pComponent, NULL, NULL, FALSE, FALSE , TRUE);

	SetType(HTML_ITEM_FLASH);
}

CHtml_FlashItem::~CHtml_FlashItem()
{
	
	if( m_pFlash )
	{
		m_pFlash->Unload();

		NTL_DELETE( m_pFlash );
	}
}

VOID CHtml_FlashItem::SetPositionX(INT nPosX)
{
	CHtml_Item::SetPositionX(nPosX);
}

VOID CHtml_FlashItem::SetPositionY(INT nPosY)
{
	CHtml_Item::SetPositionY(nPosY);
}

VOID CHtml_FlashItem::SetPosition(INT nPosX, INT nPosY)
{
	CHtml_Item::SetPosition(nPosX, nPosY);
}

VOID CHtml_FlashItem::SetPosition()
{
}

VOID CHtml_FlashItem::Update( float fElapsed )
{
	m_pFlash->Update( fElapsed );
}

VOID CHtml_FlashItem::ItemLineBottomSorting(INT nBottom)
{
	SetPositionY(nBottom - m_nCY);
}

VOID CHtml_FlashItem::SetFlash(const WCHAR *pFlash, INT& nWidth, INT& nHeight, 
							   INT nWrapAlign, INT nResizeType, BOOL bAutoRewind, BOOL bRender /* = FALSE */)
{
	m_strFlash = pFlash;
	SetSize(nWidth, nHeight);
	SetWrapAlign(nWrapAlign);
	m_bAutoRewind	= bAutoRewind;
	m_nResizeType	= nResizeType;

	// bRender가 FALSE로 들어오면 가장 처음 자료를 관리하기 위하여 플래쉬의 정보만 담고 있고 실제로 Load하지는 않는다.
	if( !m_pFlash || !bRender)
		return;
	
	char chBuffer[128];
	::WideCharToMultiByte( GetACP(), 0, pFlash, -1, chBuffer, 128, NULL, NULL );

	m_pFlash->Load( chBuffer, TRUE );
	m_pFlash->PlayMovie( TRUE );
	
	INT nRealWidth = nWidth;
	INT nRealHeight = nHeight;

	if( nRealWidth <= 0 )
	{
		if( m_pFlash )
		{
			nRealWidth = m_pFlash->GetWidth();
			nWidth = nRealWidth;
		}
		else
			nRealWidth = nWidth;
	}

	if( nRealHeight <= 0 )
	{
		if( m_pFlash )
		{
			nRealHeight = m_pFlash->GetHeight();
			nHeight = nRealHeight;
		}
		else
			nRealHeight = nHeight;
	}

	SetSize(nRealWidth, nRealHeight);
	m_pFlash->SetSize(nRealWidth, nRealHeight);
	m_pFlash->SetResizeType( (CFlash::eRESIZETYPE)nResizeType );
	m_pFlash->AutoRewind( bAutoRewind );
}

CHtml_Item* CHtml_FlashItem::ItemDisposition(CRectangle& rect, CRectangle rtFixedClipping, std::list<CHtml_Item*>& listItems, INT nLineIntervalY, BOOL bFixedTopBottom, BOOL& bLineOver)
{
	bLineOver = FALSE;

	if(listItems.size() > 0)
	{
		if(m_nCX > rect.GetWidth())
		{
			bLineOver = TRUE;
			return this;
		}
	}

	if(m_dwWrapAlign & HTML_WRAP_ALIGN_RIGHT)
	{
		CHtml_FlashItem *pFlashItem = NTL_NEW CHtml_FlashItem(m_pParentComp);
		pFlashItem->SetCurrLine(0);
		pFlashItem->SetFlash(m_strFlash.c_str(), m_nCX, m_nCY, m_dwWrapAlign, m_nResizeType, m_bAutoRewind, TRUE);
		pFlashItem->SetPosition(rect.right - m_nCX, rect.top);
		listItems.push_back(pFlashItem);

		rect.right = rect.right - m_nCX;
	}
	else if( m_dwWrapAlign & HTML_WRAP_ALIGN_CENTER )
	{
		CHtml_FlashItem *pFlashItem = NTL_NEW CHtml_FlashItem(m_pParentComp);
		pFlashItem->SetCurrLine(0);
		pFlashItem->SetFlash(m_strFlash.c_str(), m_nCX, m_nCY, m_dwWrapAlign, m_nResizeType, m_bAutoRewind, TRUE);
		pFlashItem->SetPosition((rect.GetWidth()/2) - (m_nCX/2), rect.top);
		listItems.push_back(pFlashItem);

		rect.left += (rect.GetWidth()/2) + (m_nCX/2);
	}
	else
	{
		CHtml_FlashItem *pFlashItem = NTL_NEW CHtml_FlashItem(m_pParentComp);
		pFlashItem->SetCurrLine(0);
		pFlashItem->SetFlash(m_strFlash.c_str(), m_nCX, m_nCY, m_dwWrapAlign, m_nResizeType, m_bAutoRewind, TRUE);
		pFlashItem->SetPosition(rect.left, rect.top);
		listItems.push_back(pFlashItem);

		rect.left += m_nCX;
	}

	if(rect.top + m_nCY > rect.bottom)
	{
		rect.bottom = rect.top + m_nCY;
	}

	return NULL;
}

VOID CHtml_FlashItem::OnPrePaint(INT nOffsetX, INT nOffsetY, const CRectangle& rect, CTexture *pTexture)
{
	m_nRenderPosX = nOffsetX + m_nPosX;
	m_nRenderPosY = nOffsetY + m_nPosY;

	m_pFlash->SetPosition( m_nRenderPosX, m_nRenderPosY );
}

void CHtml_FlashItem::OnPaint(const CRectangle& rt)
{
}

VOID CHtml_FlashItem::OnSetAlpha( BYTE ucAlpha ) 
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// backpic item

CHtml_BackPicItem::CHtml_BackPicItem(CComponent* pComponent)
{
	m_pTexture		= NULL;
	m_nRenderPosX	= 0;
	m_nRenderPosY	= 0;
	m_dwAlign		= 0;

	m_SnapShot.rtRect.left		= 0;
	m_SnapShot.rtRect.right		= 0;
	m_SnapShot.rtRect.top		= 0;
	m_SnapShot.rtRect.bottom	= 0;

	m_pParentComp = pComponent;

	SetType(HTML_ITEM_BACKPIC);
}

CHtml_BackPicItem::~CHtml_BackPicItem()
{
	if(m_pTexture)
	{
		g_TextureMgr.DeleteTexture( m_pTexture );
		m_pTexture = NULL;
	}
}

VOID CHtml_BackPicItem::SetPicture(const WCHAR *pPic, INT nWidth, INT nHeight)
{
	char chBuffer[128];
	::WideCharToMultiByte( GetACP(), 0, pPic, -1, chBuffer, 128, NULL, NULL );

	m_strPic			= pPic;
	m_pTexture			= g_TextureMgr.CreateTexture( chBuffer );
	m_SnapShot.uRed		= 255;
	m_SnapShot.uGreen	= 255;
	m_SnapShot.uBlue	= 255;
	m_SnapShot.uAlpha	= 255;
	m_SnapShot.UVs[0]	= 0;
	m_SnapShot.UVs[1]	= 0;
	m_SnapShot.UVs[2]	= 1;
	m_SnapShot.UVs[3]	= 1;

	// nWidth나 nHeight값이 0이나 0보다 낮은 숫자라면 이미지 원본의 크기대로 출력한다.
	INT nRealWidth = nWidth;
	INT nRealHeight = nHeight;

	if( nRealWidth <= 0 )
	{
		if( m_pTexture )
			nRealWidth = m_pTexture->GetWidth();
		else
			nRealWidth = 100;
	}

	if( nRealHeight <= 0 )
	{
		if( m_pTexture )
			nRealHeight = m_pTexture->GetHeight();
		else
			nRealHeight = 100;
	}

	SetSize(nRealWidth, nRealHeight);
	/*SetWrapAlign(nWrapAlign);*/

	m_SnapShot.rtRect.left		= m_nPosX;
	m_SnapShot.rtRect.right		= m_nPosX + nWidth;
	m_SnapShot.rtRect.top		= m_nPosY;
	m_SnapShot.rtRect.bottom	= m_nPosY + nHeight;
}

void CHtml_BackPicItem::OnBackPaint(const CRectangle& rt)
{
	if(m_nRenderPosY + m_nCY < 0)
		return;

	m_SnapShot.rtRect.left		= rt.left + m_nRenderPosX;
	m_SnapShot.rtRect.right		= rt.left + m_nRenderPosX + m_nCX;
	if(m_nRenderPosY < 0)
		m_SnapShot.rtRect.top	= rt.top;
	else
		m_SnapShot.rtRect.top	= rt.top + m_nRenderPosY;

	INT nDeltaY = m_nRenderPosY + m_nCY - rt.GetHeight();
	if(nDeltaY > 0)
		m_SnapShot.rtRect.bottom = rt.bottom;
	else
		m_SnapShot.rtRect.bottom = rt.top + m_nRenderPosY + m_nCY;

	PlaneSnapShot sSnapShot;
	sSnapShot = m_SnapShot;

	if(m_nRenderPosY < 0)
		sSnapShot.UVs[1] = (FLOAT)abs(m_nRenderPosY)/(FLOAT)m_nCY;
	if(nDeltaY > 0)
		sSnapShot.UVs[3] = 1.0f - (FLOAT)abs(nDeltaY)/(FLOAT)m_nCY;

	g_GuiRenderer.RenderQueue(&sSnapShot, m_pTexture);
}

VOID CHtml_BackPicItem::OnSetAlpha( BYTE ucAlpha ) 
{
	m_SnapShot.uAlpha = ucAlpha;
}

END_GUI


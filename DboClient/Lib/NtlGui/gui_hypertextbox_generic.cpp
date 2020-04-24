#include "gui_precomp.h"
#include "gui_hypertextbox_generic.h"
#include "gui_componentoption.h"
#include "gui_resourcemanager.h"

START_GUI

/////////////////////////////////////////////////////////////////////////////
// Construction:

CHyperTextBox_Generic::CHyperTextBox_Generic(
	CHyperTextBox *pSelf,
	DWORD dwStyle)
: m_pHyperTextBox(pSelf),m_dwStyle(dwStyle),m_nRow(0),m_nColumn(0),m_nInterval(0)
{
	m_nBlank = 0;
	m_dwTextColor = DEFAULT_TEXTCOLOR;
	m_dwTextBkColor = DEFAULT_BKCOLOR;
	m_nBkMode = BK_TRANSPARENT;
	m_dwStyle |= COMP_TEXT_LEFT;

	CScrollBar *pScrollBar = m_pHyperTextBox->GetVerticalScrollBar();
	
	m_SlotSetOptions = m_pHyperTextBox->SigSetOptions().Connect(
		this, &CHyperTextBox_Generic::OnSetOptions);
	m_SlotScrollValueChange = pScrollBar->SigValueChanged().Connect(
		this, &CHyperTextBox_Generic::OnScrollValueChange);
	m_SlotScrollMove = pScrollBar->SigSliderMoved().Connect(
		this, &CHyperTextBox_Generic::OnScrollMove);
	m_SlotMove = m_pHyperTextBox->SigMove().Connect(
		this, &CHyperTextBox_Generic::OnMove);
	m_SlotResize = m_pHyperTextBox->SigResize().Connect(
		this, &CHyperTextBox_Generic::OnResize);
	m_SlotMouseDown = m_pHyperTextBox->SigMouseDown().Connect(
		this, &CHyperTextBox_Generic::OnMouseDown);
	m_SlotMouseWheel = m_pHyperTextBox->SigMouseWheel().Connect(
		this, &CHyperTextBox_Generic::OnMouseWheel);


}

CHyperTextBox_Generic::CHyperTextBox_Generic(
	CHyperTextBox *pSelf, 
	int nRow,
	int nColumn,
	int nInterval,
	DWORD dwStyle)
: m_pHyperTextBox(pSelf),m_dwStyle(dwStyle),m_nRow(nRow),m_nColumn(nColumn),m_nInterval(nInterval)
{
	m_nBlank = 0;
	m_dwTextColor = DEFAULT_TEXTCOLOR;
	m_dwTextBkColor = DEFAULT_BKCOLOR;
	m_nBkMode = BK_TRANSPARENT;
	m_dwStyle |= COMP_TEXT_LEFT;

	CScrollBar *pScrollBar = m_pHyperTextBox->GetVerticalScrollBar();
	
	m_SlotSetOptions = m_pHyperTextBox->SigSetOptions().Connect(
		this, &CHyperTextBox_Generic::OnSetOptions);
	m_SlotScrollValueChange = pScrollBar->SigValueChanged().Connect(
		this, &CHyperTextBox_Generic::OnScrollValueChange);
	m_SlotScrollMove = pScrollBar->SigSliderMoved().Connect(
		this, &CHyperTextBox_Generic::OnScrollMove);
	m_SlotMove = m_pHyperTextBox->SigMove().Connect(
		this, &CHyperTextBox_Generic::OnMove);
	m_SlotResize = m_pHyperTextBox->SigResize().Connect(
		this, &CHyperTextBox_Generic::OnResize);
	m_SlotMouseDown = m_pHyperTextBox->SigMouseDown().Connect(
		this, &CHyperTextBox_Generic::OnMouseDown);
	m_SlotMouseWheel = m_pHyperTextBox->SigMouseWheel().Connect(
		this, &CHyperTextBox_Generic::OnMouseWheel);
}


CHyperTextBox_Generic::~CHyperTextBox_Generic()
{
	/*
	int nCount = (int)m_vecFont.size();
	for(int i=0 ; i<nCount ; i++)
		g_FontMgr._DeleteFont(m_vecFont[i]);
	
	if(!m_TextSurface.m_hTexture.IsNull()) 
		g_TextureMgr.DeleteTexture(m_TextSurface.m_hTexture);
		*/
}


VOID CHyperTextBox_Generic::Clear(VOID)
{
	m_vecItem.clear();
}

VOID CHyperTextBox_Generic::CreateFont(TCHAR * szFaceName, int nHeight, int nAttributes /*= 0*/)
{
	/*
	m_hFont = g_FontMgr.CreateFont(szFaceName, nHeight,nAttributes);
	m_vecFont.push_back(m_hFont);
	*/
}


VOID CHyperTextBox_Generic::SetTextKey(const std::string &key)
{
	CTextNode *pText = CResourceManager::GetHyperText(key); 
	if(!pText)
		return;

	Clear();
	NodePasing(pText);
}

VOID CHyperTextBox_Generic::TextOut(VOID)
{
//	g_Renderer.RenderPlane(m_TextSurface.m_hTexture ,&m_TextSurface.m_SnapShot,m_pHyperTextBox->GetRenderTop());
}

VOID CHyperTextBox_Generic::CreatureTexture(int nWidth,int nHeight)
{
	/*
	if(!m_TextSurface.m_hTexture.IsNull()) 
	{
		if(g_TextureMgr.GetWidth(m_TextSurface.m_hTexture) >= nWidth && 
			g_TextureMgr.GetHeight(m_TextSurface.m_hTexture) >= nHeight)
		{
			g_TextureMgr.FillTexture(m_TextSurface.m_hTexture, DCOLOR(0,0,0,0));
			return;
		}

		g_TextureMgr.DeleteTexture(m_TextSurface.m_hTexture);
	}

	CRectangle rect = m_pHyperTextBox->GetScreenRect();

	m_TextSurface.m_hTexture = g_TextureMgr.CreateTexture(nWidth,nHeight);

	int nTexW = g_TextureMgr.GetWidth(m_TextSurface.m_hTexture);
	int nTexH = g_TextureMgr.GetHeight(m_TextSurface.m_hTexture);

	m_TextSurface.m_SnapShot.m_Color = DCOLOR(255,255,255,255);
	m_TextSurface.m_SnapShot.m_dwBlend = 0;
	m_TextSurface.m_SnapShot.m_arrUVs[0] = 0;
	m_TextSurface.m_SnapShot.m_arrUVs[1] = 0;
	m_TextSurface.m_SnapShot.m_arrUVs[2] = 1;
	m_TextSurface.m_SnapShot.m_arrUVs[3] = 1;

	m_TextSurface.m_SnapShot.m_rtRect.left = rect.left;
	m_TextSurface.m_SnapShot.m_rtRect.right = rect.left+nTexW;
	m_TextSurface.m_SnapShot.m_rtRect.top = rect.top;
	m_TextSurface.m_SnapShot.m_rtRect.bottom = rect.top+nTexH;

	g_TextureMgr.FillTexture(m_TextSurface.m_hTexture, DCOLOR(0,0,0,0));
	*/
}

VOID CHyperTextBox_Generic::CascadeTexture(VOID)
{
	/*
	if(m_TextSurface.m_hTexture.IsNull()) 
		return;

	int nTexW = g_TextureMgr.GetWidth(m_TextSurface.m_hTexture);
	int nTexH = g_TextureMgr.GetHeight(m_TextSurface.m_hTexture);

	CScrollBar *pScrollBar = m_pHyperTextBox->GetVerticalScrollBar();
	CRectangle rect = m_pHyperTextBox->GetScreenRect();
	CRectangle rtScroll = pScrollBar->GetClientRect(); 
	int nScrollY = pScrollBar->GetValue();

	m_TextSurface.m_SnapShot.m_arrUVs[0] = 0;
	m_TextSurface.m_SnapShot.m_arrUVs[1] = (float)nScrollY/(float)nTexH;
	m_TextSurface.m_SnapShot.m_arrUVs[2] = (float)(rect.GetWidth()-rtScroll.GetWidth())/(float)nTexW;
	m_TextSurface.m_SnapShot.m_arrUVs[3] = (float)(nScrollY+rect.GetHeight())/(float)nTexH;

	m_TextSurface.m_SnapShot.m_rtRect.left = rect.left;
	m_TextSurface.m_SnapShot.m_rtRect.right = rect.left+rect.GetWidth();
	m_TextSurface.m_SnapShot.m_rtRect.top = rect.top;
	m_TextSurface.m_SnapShot.m_rtRect.bottom = rect.top+rect.GetHeight();
	*/
}

VOID CHyperTextBox_Generic::NodePasing(CTextNode *pText) 
{
	/*
	CNode *pNode = pText->First();

	BOOL bSkipReturn = FALSE;
	int nCurrX = m_nColumn;
	int nCurrY = m_nRow;
	int nFontW = -1;
	int nFontH = -1;
	int nLen;
	SIZE size;
	char *temp;
	std::string str;

	std::vector<CHyperTextBox_Item> vecItem;

	CScrollBar *pScrollBar = m_pHyperTextBox->GetVerticalScrollBar();
	CRectangle rect = m_pHyperTextBox->GetClientRect();
	CRectangle rtScroll = pScrollBar->GetClientRect(); 

	while(pNode)
	{
		if(pNode->GetType() == NT_CONTROL)
		{
			temp = (char*)pNode->GetString().data();
			if(temp[0] == '\n')
			{
				nCurrX = m_nColumn;
				if(nFontH < 0)
					nFontH = g_FontMgr.GetHeight(m_vecFont[0]);
				nCurrY += (nFontH+m_nInterval);

				bSkipReturn = FALSE;
			}
		}
		else if(pNode->GetType() == NT_FONT)
		{
			CFontNode *pFontNode = (CFontNode*)pNode;
			CreateFont((char*)pFontNode->GetString().data(),
									pFontNode->GetFontSize(),
									pFontNode->GetAttribute());
		}
		else if(pNode->GetType() == NT_COLOR)
		{
			CColorNode *pColorNode = (CColorNode*)pNode;
			m_dwTextColor = pColorNode->GetTextColor();
			m_dwTextBkColor = pColorNode->GetTextColor();
		}
		else if(pNode->GetType() == NT_ATTRIBUTE)
		{
			CAttrNode *pAttrNode = (CAttrNode*)pNode;
			m_dwStyle &= (~COMP_TEXT_LEFT);
			m_dwStyle &= (~COMP_TEXT_CENTER);
			m_dwStyle &= (~COMP_TEXT_RIGHT);

			if(ENABLE_COMPONENT_STYLE(pAttrNode->GetStyle(),COMP_TEXT_LEFT))
				m_dwStyle |= COMP_TEXT_LEFT;
			else if(ENABLE_COMPONENT_STYLE(pAttrNode->GetStyle(),COMP_TEXT_CENTER))
				m_dwStyle |= COMP_TEXT_CENTER;
			else
				m_dwStyle |= COMP_TEXT_RIGHT;
		}
		else if(pNode->GetType() == NT_SENTENCE || pNode->GetType() == NT_LINK)
		{
			vecItem.clear();

			if(pNode->GetType() == NT_SENTENCE)
			{
				CSentenceNode *pSentence = ((CSentenceNode*)pNode);
				temp = (char*)pSentence->GetString().data();
				nLen = pSentence->GetString().size();
			}
			else
			{
				CLinkNode* pLink = (CLinkNode*)pNode;
				temp = (char*)((CLinkNode*)pNode)->GetSentence().data();
				nLen = pLink->GetSentence().size();
			}

			if(temp)
			{
				int nStart=0,nCount=0,nInc;
				nFontW = g_FontMgr.GetHeight(m_hFont);
				nFontH = g_FontMgr.GetHeight(m_hFont);
				int nLimitW = rect.GetWidth()-nFontW-rtScroll.GetWidth();

				if(bSkipReturn)
				{
					nCurrX = m_nColumn;
					nCurrY += (nFontH+m_nInterval);
					bSkipReturn = FALSE;
				}

				while(1)
				{
					if(nStart >= nLen)
						break;

					nCount = 0;

					while(1)
					{
						g_FontMgr.GetTextSize(m_hFont,&temp[nStart],nCount,size);

						if(nCurrX+size.cx+m_nColumn*2 > nLimitW)
						{
							nCount -= nInc;
							break;
						}
						if(nStart+nCount >= nLen)
						{
							break;
						}
						if(temp[nStart+nCount] < 0)
						{
							nCount += 2;
							nInc = 2;
						}
						else
						{
							nCount += 1;
							nInc = 1;
						}
					}

					if(nCount > 0)
					{
						str.resize(nCount+1);
						str[nCount] = 0;
						memcpy((VOID*)str.data(),&temp[nStart],nCount);

						if(ENABLE_COMPONENT_STYLE(m_dwStyle,COMP_TEXT_CENTER))
							nCurrX = (nLimitW-size.cx)/2;
						if(ENABLE_COMPONENT_STYLE(m_dwStyle,COMP_TEXT_RIGHT))
							nCurrX = nLimitW-size.cx-m_nColumn;
				
						CHyperTextBox_Item item(nCurrX,nCurrY,size.cx,nFontH,str,m_dwTextColor,m_dwTextBkColor,m_nBkMode,m_vecFont.size()-1,pNode);
						m_vecItem.push_back(item);
						
						if(nStart+nCount >= nLen)
						{
							if(!ENABLE_COMPONENT_STYLE(m_dwStyle,COMP_TEXT_LEFT))
								bSkipReturn = TRUE;

							nCurrX += size.cx;
						}
						else
						{
							nCurrX = m_nColumn;
							nCurrY += (nFontH+m_nInterval);
						}
							
						nStart += nCount;
					}
					else
					{
						nCurrX = m_nColumn;
						nCurrY += (nFontH+m_nInterval);
					}
				}
			}
		}

		pNode = pNode->GetNext();
	}
	
	// scollbar setting
	pScrollBar->SetRange(0,0);
	pScrollBar->SetValue(0);
	pScrollBar->Show(true);

	if(ENABLE_COMPONENT_STYLE(m_dwStyle,COMP_VER_SCROLL))
	{
		if(nCurrY > rect.GetHeight())
		{
			int nScrollY = nCurrY-rect.GetHeight();
			pScrollBar->SetRange(0,nScrollY);
			pScrollBar->Show(true);
		}
	}

	// texture 생성.
	CreatureTexture(rect.GetWidth(),nCurrY);  

	// item draw
	DrawItem();

	// 텍스쳐 좌표.
	CascadeTexture();
	*/
}

VOID CHyperTextBox_Generic::DrawItem(VOID)
{
	/*
	POINT ptPos;
	COLORREF color;
	int nCount = m_vecItem.size(); 
	for(int i=0 ; i<nCount ; i++)
	{
		ptPos.x = m_vecItem[i].m_nPosX;
		ptPos.y = m_vecItem[i].m_nPosY;
		if(m_vecItem[i].m_nBkMode == BK_OPAQUE)
			color = m_vecItem[i].m_bkcolor;
		else
			color = 0;
		
		g_FontMgr.TextOut(m_vecFont[m_vecItem[i].m_nFontIdx],
											m_TextSurface.m_hTexture,
											ptPos,
											m_vecItem[i].m_strString.data(),
											m_vecItem[i].m_textcolor,color);				
	}
	*/
}

/////////////////////////////////////////////////////////////////////////////
// callback function

VOID CHyperTextBox_Generic::OnSetOptions(const CComponentOptions &options)
{
	if(options.Exists("row"))
		m_nRow = options.GetValueAsInt("row");
	if(options.Exists("column"))
		m_nColumn = options.GetValueAsInt("column");
	if(options.Exists("interval"))
		m_nInterval = options.GetValueAsInt("interval");
	if(options.Exists("vertical_scroll"))
		if(options.GetValueAsBool("vertical_scroll"))
			m_dwStyle |= COMP_VER_SCROLL;
}

VOID CHyperTextBox_Generic::OnScrollValueChange(int nValue)
{
	CascadeTexture();
}

VOID CHyperTextBox_Generic::OnScrollMove(int nValue)
{
	CascadeTexture();
}

VOID CHyperTextBox_Generic::OnMove(int nOldX,int nOldY)
{
	CRectangle rect = m_pHyperTextBox->GetPosition();
	if(rect.left == nOldX && rect.right == nOldY)
		return;

	CascadeTexture();
}

VOID CHyperTextBox_Generic::OnResize(int nOldW,int nOldH)
{
	CRectangle rect = m_pHyperTextBox->GetPosition();
	if(nOldW != rect.GetWidth() || nOldH != rect.GetHeight())
		CreatureTexture(rect.GetWidth(),rect.GetHeight());  
	CascadeTexture();
}

VOID CHyperTextBox_Generic::OnMouseDown(const CKey &key)
{
	if(key.m_nID != UD_LEFT_BUTTON)
		return;

	CRectangle rect;
	CScrollBar *pScrollBar = m_pHyperTextBox->GetVerticalScrollBar();
	int nScrollY = pScrollBar->GetValue();

	int nCount = (int)m_vecItem.size();
	for(int i=0 ; i<nCount ; i++)
	{
		rect.SetRect(m_vecItem[i].m_nPosX,
								 m_vecItem[i].m_nPosY-nScrollY,
								 m_vecItem[i].m_nPosX+m_vecItem[i].m_nCX,
								 m_vecItem[i].m_nPosY+m_vecItem[i].m_nCY-nScrollY);
		
		if(rect.PtInRect((int)key.m_fX,(int)key.m_fY))
		{
			m_SigNode(m_vecItem[i].m_pNode);

			if(m_vecItem[i].m_pNode->GetType() == NT_LINK)
			{
				CLinkNode *pLink = (CLinkNode*)m_vecItem[i].m_pNode;
				CTextNode *pText = pLink->GetText();
				SetTextKey(pLink->GetString());
			}
			return;
		}
	}
}

VOID CHyperTextBox_Generic::OnMouseWheel(int nFlags,short shDelta,CPos& ptPos)
{
	CScrollBar *pScrollBar = m_pHyperTextBox->GetVerticalScrollBar();
	if(!pScrollBar->IsVisible())
		return;

	int nMaxValue = pScrollBar->GetMaxValue();
	int nScrollY = pScrollBar->GetValue();
	
	if(nMaxValue == 0)
		return;
	
	if(shDelta > 0)
	{
		if(nScrollY <= 0)
			return;
		nScrollY -= (shDelta/30);
		
		if(nScrollY < 0)
			nScrollY = 0;
	}
	else
	{
		if(nScrollY >= nMaxValue)
			return;
		nScrollY += (abs(shDelta)/30);

		if(nScrollY >= nMaxValue)
			nScrollY = nMaxValue;
	}

	pScrollBar->SetValue(nScrollY); 
}

END_GUI

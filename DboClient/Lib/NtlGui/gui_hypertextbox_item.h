#ifndef __GUI_MDSTATICBOX_ITEM_H__
#define __GUI_MDSTATICBOX_ITEM_H__

#include "gui_define.h"
#include "gui_hypertext.h"

START_GUI

class CHyperTextBox_Item
{
public:

	int m_nPosX,m_nPosY;
	int m_nCX,m_nCY;
	std::string m_strString;
	COLORREF m_textcolor;
	COLORREF m_bkcolor;
	int m_nBkMode;
	int	m_nFontIdx;
	CNode *m_pNode;

public:

	CHyperTextBox_Item()
	{
		Clear();
	}
	
	CHyperTextBox_Item(int nX,
										 int nY,
										 int nCX,
										 int nCY,
										 const std::string& str,
										 COLORREF textcolor,
										 COLORREF bkcolor,
										 int nBkMode,
										 int nFontIdx,
										 CNode *pNode)
	{
		Clear();

		m_nPosX = nX;
		m_nPosY = nY;
		m_nCX = nCX;
		m_nCY = nCY;
		m_strString = str;
		m_textcolor = textcolor;
		m_bkcolor = bkcolor;
		m_nBkMode = nBkMode;
		m_nFontIdx = nFontIdx;
		m_pNode = pNode;
	}

	~CHyperTextBox_Item()
	{
	}

	VOID Clear (VOID)
	{
		m_nPosX = 0;
		m_nPosY = 0;
		m_nCX = 0;
		m_nCY = 0;
		m_strString = "";
		m_textcolor = 0;
		m_bkcolor = 0;
		m_nBkMode = BK_TRANSPARENT;
		m_nFontIdx = -1;
		m_pNode = NULL;
	}

	VOID SetPos (int nX,int nY)
	{
		m_nPosX = nX;
		m_nPosY = nY;
	}

	VOID SetSize (int nCX,int nCY)
	{
		m_nCX = nCX;
		m_nCY = nCY;
	}
};

END_GUI

#endif

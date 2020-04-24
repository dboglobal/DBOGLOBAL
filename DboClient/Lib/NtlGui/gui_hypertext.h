#ifndef __GUI_HYPERTEXT_H__
#define __GUI_HYPERTEXT_H__

#include "gui_define.h"

START_GUI

enum NODE_TYPE { NT_CONTROL, NT_FONT, NT_COLOR, NT_ATTRIBUTE,NT_SENTENCE, NT_TEXT, NT_LINK, NT_COUNT };

class CNode
{
protected:

	CNode*					m_pPrev;
	CNode*					m_pNext;

public:

	CNode() : m_pPrev(0), m_pNext(0) {}
	virtual ~CNode() {}

	virtual std::string& GetString() = 0;
	virtual NODE_TYPE		 GetType() = 0;

public:

	CNode*			 GetPrev() { return m_pPrev; }
	CNode*			 GetNext() { return m_pNext; }

	VOID				 SetPrev(CNode* pPrev) { m_pPrev = pPrev; }
	VOID				 SetNext(CNode* pNext) { m_pNext = pNext; }
};


class CControlNode	: public CNode
{
	std::string		m_strControl;

public:

	CControlNode() {}
	virtual ~CControlNode() {}

	std::string& GetString() { return m_strControl; }
	NODE_TYPE		 GetType() { return NT_CONTROL; }
};

class CFontNode	: public CNode
{
	std::string		m_strFont;
	int						m_nFontSize;
	int						m_nAttribute;

public:

	CFontNode() : m_nAttribute(0) {}
	virtual ~CFontNode() {}

	std::string& GetString() { return m_strFont; }
	NODE_TYPE		 GetType() { return NT_FONT; }

	int&				 GetFontSize() { return m_nFontSize; }
	int&				 GetAttribute() { return m_nAttribute; }
};

class CColorNode : public CNode
{
	std::string m_strColor;
	COLORREF	m_dwText;
	COLORREF	m_dwTextBk;

public:

	CColorNode() {}
	virtual ~CColorNode() {}

	std::string& GetString() { return m_strColor; }
	NODE_TYPE	GetType() { return NT_COLOR; }

	COLORREF& GetTextColor() { return m_dwText; }
	COLORREF& GetTextBkColor() { return m_dwTextBk; }
};


class CAttrNode : public CNode
{
	std::string	m_strAttr;
	DWORD m_dwStyle;

public:

	CAttrNode() : m_dwStyle(0) {}
	virtual ~CAttrNode() {}

	std::string& GetString() { return m_strAttr; }
	NODE_TYPE GetType() { return NT_ATTRIBUTE; }

	DWORD& GetStyle() { return m_dwStyle; }
};



class CSentenceNode : public CNode
{
	std::string		m_strContent;

public:

	CSentenceNode() {}
	virtual ~CSentenceNode() {}

	std::string& GetString() { return m_strContent; }
	NODE_TYPE		 GetType() { return NT_SENTENCE; }
};

typedef std::list<CNode*>				NodeList;

class CTextNode : public CNode
{
	std::string		m_strKey;
	std::string		m_strPrev;
	std::string		m_strNext;

	NodeList			m_lstNodes;
	CNode*					m_pCurrNode;

public:

	CTextNode() : m_pCurrNode(0) {}
	virtual ~CTextNode() {}

	std::string& GetString() { return m_strKey; }
	NODE_TYPE		 GetType() { return NT_TEXT; }

public:

	bool					AddNode(CNode* pNode);

	CNode*					First();
	CNode*					Curr();
	CNode*					Prev();
	CNode*					Next();
	CNode*					End();

	std::string&	GetPrevText() { return m_strPrev;}
	std::string&	GetNextText() { return m_strNext;}
};

class CLinkNode : public CNode
{
protected:

	std::string		m_strKey;
	std::string		m_strText;
	std::string		m_strData;
	CTextNode*					m_pText;

public:

	CLinkNode() : m_pText(0) {}
	virtual ~CLinkNode() {}

	std::string& GetString() { return m_strKey; }
	std::string& GetSentence() { return m_strText; }
	std::string& GetData() { return m_strData; }
	NODE_TYPE		 GetType() { return NT_LINK; }

public:

	CTextNode*				GetText() { return m_pText; }
	bool				SetText(CTextNode* pText);
};


typedef std::map<std::string, CTextNode*>		TextMap;

class CHyperText  
{
	NodeList			m_lstNodes;
	TextMap				m_mapTexts;

public:

	CHyperText();
	virtual ~CHyperText();

	VOID			Clear();
	bool			LinkAll();

public:

	bool			AddNode(CNode* pNode);

	bool			RegisterText(CTextNode* pText);
	CTextNode*			GetText(const std::string& key);
};

class CHyperTextBuilder
{
	enum _STATE { NONE, START, END, TITLE, PREV, NEXT, SENTENCE, LINK, CONTROL, FONT, COLOR,ATTRIBUTE,SKIPLINE };	// scan state

	CHyperText		m_HyperText;
	CTextNode*					m_pCurrText;
	char					m_cLastKey;

	static const char m_Seps[10];

public:

	CHyperTextBuilder();
	virtual ~CHyperTextBuilder();

	CHyperText*		GetHyperText() { return &m_HyperText;}

	CControlNode*		BuildControl(const char c);
	CFontNode*			BuildFont(const char* content);
	CColorNode*			BuildColor(const char* color,const char* bkcolor);
	CAttrNode*			BuildAttribute(const char* attr);
	CSentenceNode*	BuildSentence(const char* content);
	CTextNode*			BuildText(const char* key);
	CLinkNode*			BuildLink(const char* link);
	
public:

	virtual bool	Build(const char* filename);
	virtual bool	Parse(FILE* fp);

private:

	bool				IsKey(char c, char* key);
	const char*	Scan(FILE* fp, char* key);
	bool				SkipLine(FILE* fp);

	const char* ScanBlock(FILE* fp);		// < ........ >
	const char* ScanString(FILE* fp);  // ..........

	char				GetLastKey() { return m_cLastKey; }

	_STATE			GetState(char c);
};

END_GUI

#endif // !defined(AFX_HYPERTEXT_H__D1812322_7833_43C4_A592_50F27663F2B9__INCLUDED_)

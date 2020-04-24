#include "gui_precomp.h"
#include "gui_hypertext.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

START_GUI

//////////////////////////////////////////////////////////////////////

bool CTextNode::AddNode(CNode* pNode)
{
	if (!pNode)
		return false;

	if (!m_lstNodes.empty())
	{
		CNode* pPrev = m_lstNodes.back();

		pPrev->SetNext(pNode);
		pNode->SetPrev(pPrev);
	}

	m_lstNodes.push_back(pNode); 

	return true;
}	

CNode*	CTextNode::First()
{
	if (m_lstNodes.empty())
		return NULL;
	
	m_pCurrNode = m_lstNodes.front();

	return m_pCurrNode;
}

CNode*	CTextNode::Curr()
{
	return m_pCurrNode;
}

CNode*	CTextNode::Prev()
{
	if (m_lstNodes.empty() || !m_pCurrNode)
		return NULL;

	m_pCurrNode = m_pCurrNode->GetPrev();

	return m_pCurrNode;
}

CNode*	CTextNode::Next()
{
	if (m_lstNodes.empty() || !m_pCurrNode)
		return NULL;

	m_pCurrNode = m_pCurrNode->GetNext();

	return m_pCurrNode;
}

CNode*	CTextNode::End()
{
	if (m_lstNodes.empty())
		return NULL;

	m_pCurrNode = m_lstNodes.back();

	return m_pCurrNode;
}

//////////////////////////////////////////////////////////////////////

bool CLinkNode::SetText(CTextNode* pText)
{
	if (!pText)
		return false;

	m_pText = pText;
	m_strKey = pText->GetString();

	return true;
}

//////////////////////////////////////////////////////////////////////

CHyperText::CHyperText()
{

}

CHyperText::~CHyperText()
{
	Clear();
}

bool CHyperText::AddNode(CNode* pNode)
{
	if (!pNode)
		return false;

	m_lstNodes.push_back(pNode);

	if (pNode->GetType() == NT_TEXT)
		RegisterText((CTextNode*)pNode);

	return true;
}

bool CHyperText::RegisterText(CTextNode* pText)
{
	if (!pText)
		return false;

	CTextNode* text = GetText(pText->GetString());

	if (text == NULL)
	{
		m_mapTexts[pText->GetString()] = pText;

		return true;
	}
	else
	{
		return false;
	}
}

CTextNode*	CHyperText::GetText(const std::string& key)
{
	TextMap::iterator it;

	it = m_mapTexts.find(key);

	if (it != m_mapTexts.end())
		return it->second;
	else
		return NULL;
}


VOID CHyperText::Clear()
{
	NodeList::iterator it;

	for (it = m_lstNodes.begin(); it != m_lstNodes.end(); it++)
	{
		NTL_DELETE( *it );
	}

	m_lstNodes.clear();
	m_mapTexts.clear();
}

bool CHyperText::LinkAll()
{
	NodeList::iterator it;

	CNode* node;
	CTextNode* text;
	CLinkNode*	link;

	for (it = m_lstNodes.begin(); it != m_lstNodes.end(); it++)
	{
		node = *it;

		switch (node->GetType())
		{
		case NT_TEXT:
			text = (CTextNode*)node;

			text->SetPrev(GetText(text->GetPrevText()));
			text->SetNext(GetText(text->GetNextText()));

			break;

		case NT_LINK:
			link = (CLinkNode*)node;

			link->SetText(GetText(link->GetString()));

			break;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
	

const char CHyperTextBuilder::m_Seps[10] = {',',};

CHyperTextBuilder::CHyperTextBuilder()
{
	m_cLastKey = 0;
	m_pCurrText = NULL;
}

CHyperTextBuilder::~CHyperTextBuilder()
{
}

CControlNode* CHyperTextBuilder::BuildControl(const char c)
{
	if (!m_pCurrText)
		return NULL;

	CControlNode* control = NTL_NEW CControlNode;

	control->GetString() = c;

	m_HyperText.AddNode(control);

	m_pCurrText->AddNode(control);

	return control;
}

CFontNode*	CHyperTextBuilder::BuildFont(const char* content)
{
	/*
	CFontNode* font = NTL_NEW CFontNode;

	char* token;

	token = strtok((char*)content, m_Seps);
	font->GetString() = token;
	token = strtok(NULL, m_Seps);
	font->GetFontSize() = atoi(token);
	token = strtok(NULL, m_Seps);
	
	int nLen = (int)strlen(token);
	for(int i=0 ; i<nLen ; i++)
	{
		if(token[i] == 'B')
			font->GetAttribute() |= EZ_ATTR_BOLD;
		if(token[i] == 'I')
			font->GetAttribute() |= EZ_ATTR_ITALIC;
		if(token[i] == 'L')
			font->GetAttribute() |= EZ_ATTR_UNDERLINE;
		if(token[i] == 'S')
			font->GetAttribute() |= EZ_ATTR_STRIKEOUT;
	}

	m_HyperText.AddNode(font);
	m_pCurrText->AddNode(font);

	return font;
	*/

	return NULL;
}

CColorNode*	CHyperTextBuilder::BuildColor(const char* color,const char* bkcolor)
{
	CColorNode* colornode = NTL_NEW CColorNode;

	BYTE byRed,byGreen,byBlue;

	char *token, *next_token;
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	token = strtok_s((char*)color, m_Seps, &next_token);
	byRed = atoi(token);
	token = strtok_s(NULL, m_Seps, &next_token);
	byGreen = atoi(token);
	token = strtok_s(NULL, m_Seps, &next_token);
	byBlue = atoi(token);

	colornode->GetTextColor() = RGB(byRed,byGreen,byBlue);

	token = strtok_s((char*)bkcolor, m_Seps, &next_token);
	byRed = atoi(token);
	token = strtok_s(NULL, m_Seps, &next_token);
	byGreen = atoi(token);
	token = strtok_s(NULL, m_Seps, &next_token);
	byBlue = atoi(token);
#else
	token = strtok((char*)color, m_Seps);
	byRed = atoi(token);
	token = strtok(NULL, m_Seps);
	byGreen = atoi(token);
	token = strtok(NULL, m_Seps);
	byBlue = atoi(token);

	colornode->GetTextColor() = RGB(byRed,byGreen,byBlue);

	token = strtok((char*)bkcolor, m_Seps);
	byRed = atoi(token);
	token = strtok(NULL, m_Seps);
	byGreen = atoi(token);
	token = strtok(NULL, m_Seps);
	byBlue = atoi(token);
#endif

	colornode->GetTextBkColor() = RGB(byRed,byGreen,byBlue);

	m_HyperText.AddNode(colornode);
	m_pCurrText->AddNode(colornode);

	return colornode;
}

CAttrNode* CHyperTextBuilder::BuildAttribute(const char* attr)
{
	CAttrNode* attrnode = NTL_NEW CAttrNode;

	int nLen = (int)strlen(attr);
	for(int i=0 ; i<nLen ; i++)
	{
		if(attr[i] == 'L')
			attrnode->GetStyle() |= COMP_TEXT_LEFT;
		else if(attr[i] == 'R')
			attrnode->GetStyle() |= COMP_TEXT_RIGHT;
		else		
			attrnode->GetStyle() |= COMP_TEXT_CENTER;
	}

	m_HyperText.AddNode(attrnode);
	m_pCurrText->AddNode(attrnode);

	return attrnode;
}

CSentenceNode* CHyperTextBuilder::BuildSentence(const char* content)
{
	if (!m_pCurrText || !content || !content[0])
		return NULL;

	CSentenceNode* s = NTL_NEW CSentenceNode;

	s->GetString() = content;

	m_HyperText.AddNode(s);

	m_pCurrText->AddNode(s);

	return s;
}

CTextNode* CHyperTextBuilder::BuildText(const char* key)
{
	if (!key || !key[0])
		return NULL;

	CTextNode* t = NTL_NEW CTextNode;

	t->GetString() = key;

	m_HyperText.AddNode(t);

	m_pCurrText = t;

	return t;
}

CLinkNode* CHyperTextBuilder::BuildLink(const char* link)
{
	if (!m_pCurrText || !link || !link[0])
		return NULL;

	CLinkNode* l = NTL_NEW CLinkNode;

	char *token, *next_token;

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	token = strtok_s((char*)link, m_Seps, &next_token);
	l->GetString() = token;
	token = strtok_s(NULL, m_Seps, &next_token);
	l->GetSentence() = token;
	token = strtok_s(NULL, m_Seps, &next_token);
	l->GetData() = token;
#else
	token = strtok((char*)link, m_Seps);
	l->GetString() = token;
	token = strtok(NULL, m_Seps);
	l->GetSentence() = token;
	token = strtok(NULL, m_Seps);
	l->GetData() = token;
#endif

	m_HyperText.AddNode(l);

	m_pCurrText->AddNode(l);

	return l;
}

bool CHyperTextBuilder::Build(const char* filename)
{
	FILE* fp = NULL;

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	if(fopen_s(&fp, filename, "rt") != 0)
		return false;
#else
	fp = fopen(filename, "rt");
	if(fp == NULL)
		return false;
#endif

	Parse(fp);

	m_HyperText.LinkAll();

	fclose(fp);

	return true;
}

bool CHyperTextBuilder::Parse(FILE* fp)
{
	if (!fp || feof(fp))
		return false;

	char c; 

	_STATE state = NONE;

	while (true)
	{
		c = fgetc(fp);

		if (feof(fp))
			break;

		if (c == '/')										// keyword
		{
			c = fgetc(fp);
			state = GetState(c);
		}
		else if (IsKey(c, "\t\n"))			// control character
		{
			state = CONTROL;
		}
		else
		{
			fseek(fp, -1, SEEK_CUR);
			state = SENTENCE;
		}

		switch (state)
		{
		case NONE:
			break;
		case START:
			break;
		case END:
			return true;
		case TITLE:
			BuildText(ScanBlock(fp));
			SkipLine(fp);
			break;
		case PREV:
			m_pCurrText->GetPrevText() = ScanBlock(fp);
			SkipLine(fp);
			break;
		case NEXT:
			m_pCurrText->GetNextText() = ScanBlock(fp);
			SkipLine(fp);
			break;
		case SENTENCE:
			BuildSentence(ScanString(fp));
			break;
		case LINK:
			BuildLink(ScanBlock(fp));
			break;
		case CONTROL:
			BuildControl(c);
			break;
		case FONT:
			BuildFont(ScanBlock(fp));
			break;
		case COLOR:
		{
			std::string color = ScanBlock(fp);
			std::string bkcolor = ScanBlock(fp);
			BuildColor(color.c_str(),bkcolor.c_str());
			break;
		}
		case ATTRIBUTE:
			BuildAttribute(ScanBlock(fp));
			break;
		case SKIPLINE:
			SkipLine(fp);
			break;
		}
	}

	return true;
}

bool CHyperTextBuilder::IsKey(char c, char* key)
{
	for (int i = 0; key[i]; i++)
	{
		if (c == key[i])
			return true;
	}

	return false;
}

const char* CHyperTextBuilder::Scan(FILE* fp, char* key)
{
	static char temp[1024];
	int i;
	char c;

	i = 0;		
	c = fgetc(fp);

	while (!feof(fp) && i < 1024)
	{
		if (IsKey(c,key))
		{
			fseek(fp, -1, SEEK_CUR);
			m_cLastKey = c;
			temp[i] = 0;

			return temp;
		}

		temp[i++] = c;

		c = fgetc(fp);
	}

	return NULL;
}

bool CHyperTextBuilder::SkipLine(FILE* fp)
{
	char c;

	c = fgetc(fp);

	while (!feof(fp))
	{
		if (c == '\n')
			return true;

		c = fgetc(fp);
	}

	return false;
}

const char* CHyperTextBuilder::ScanBlock(FILE* fp)
{
	const char* ret;

	Scan(fp, "<");

	fgetc(fp);		// skip '<'

	ret = Scan(fp, ">");

	fgetc(fp);		// skip '>'

	return ret;
}

const char* CHyperTextBuilder::ScanString(FILE* fp)
{
	return Scan(fp, "/\t\n<");
}

CHyperTextBuilder::_STATE CHyperTextBuilder::GetState(char c)
{
	switch(c)
	{
		case 's':
		case 'S':
			return START;
		case 'e':
		case 'E':
			return END;
		case 't':
		case 'T':
			return TITLE;
		case 'p':
		case 'P':
			return PREV;
		case 'n':
		case 'N':
			return NEXT; 
		case 'l':
		case 'L':
			return LINK;
		case 'f':
		case 'F':
			return FONT;
		case 'c':
		case 'C':
			return COLOR;
		case 'a':
		case 'A':
			return ATTRIBUTE;
		case '/':
			return SKIPLINE;
	}

	return NONE;
}	

END_GUI






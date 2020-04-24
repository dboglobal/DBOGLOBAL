#include "gui_precomp.h"
#include "gui_html_doc.h"
#include "gui_componenttokenizer.h"
#include "gui_htmltokenizer.h"
#include "gui_htmlbox_item.h"
#include "gui_component.h"
#include <algorithm>

#include "guiutil.h"

START_GUI

#define HTML_FONT_NAME			DEFAULT_FONTW
#define HTML_FONT_SIZE			90
#define HTML_FONT_TEXT_COLOR	RGB(255,255,255)
#define HTML_FONT_BG_COLOR		RGB(0,0,0)

#define MAX_BUFFER_TEXT			100000

/////////////////////////////////////////////////////////////////////////////////
// Unicode

CHtmlMetaTagItem::CHtmlMetaTagItem()
{
	m_nFlags		= 0;
	m_FontColor		= HTML_FONT_TEXT_COLOR;
	m_nFontSize		= 100;
	m_nFontAttr		= 0;
}

CHtmlMetaTagItem::~CHtmlMetaTagItem()
{
}

//////////////////////////////////////////////////////////////////////////


CHtmlMetaTag::CHtmlMetaTag()
{
	m_nTagId = -1;
}

CHtmlMetaTag::~CHtmlMetaTag()
{
	ListMetaTagItem::iterator it;
	for(it = m_listMetaTagItem.begin(); it != m_listMetaTagItem.end(); it++)
	{
		NTL_DELETE( *it );
	}

	m_listMetaTagItem.clear();
}

VOID CHtmlMetaTag::SetTagId(INT nTagId)
{
	m_nTagId = nTagId;
}

INT CHtmlMetaTag::GetTagId(void) const
{
	return m_nTagId;
}

VOID CHtmlMetaTag::AddItem(CHtmlMetaTagItem *pItem)
{
	m_listMetaTagItem.push_back(pItem);
}

CHtmlMetaTagItem* CHtmlMetaTag::FindItem(const wchar_t *pKey)
{
	ListMetaTagItem::iterator it;
	for(it = m_listMetaTagItem.begin(); it != m_listMetaTagItem.end(); it++)
	{
		if( (*it)->m_wstrTagKey == pKey)
			return (*it);
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CHtmlMetaTagContainter::CHtmlMetaTagContainter()
{
	m_hmapTag[L"[meta"]		= &CHtmlMetaTagContainter::MetaTagStartHandler;
	m_hmapTag[L"[default"]	= &CHtmlMetaTagContainter::MetaDefaultTagHandler;
	m_hmapTag[L"[npc"]		= &CHtmlMetaTagContainter::MetaNpcTagHandler;
	m_hmapTag[L"[mob"]		= &CHtmlMetaTagContainter::MetaMobTagHandler;
	m_hmapTag[L"[user"]		= &CHtmlMetaTagContainter::MetaUserTagHandler;
	m_hmapTag[L"[object"]	= &CHtmlMetaTagContainter::MetaObjectTagHandler;
	m_hmapTag[L"[place"]	= &CHtmlMetaTagContainter::MetaPlaceTagHandler;
	m_hmapTag[L"[class"]		= &CHtmlMetaTagContainter::MetaClassTagHandler;
	m_hmapTag[L"[race"]		= &CHtmlMetaTagContainter::MetaRaceTagHandler;
	m_hmapTag[L"[item"]		= &CHtmlMetaTagContainter::MetaItemTagHandler;
	m_hmapTag[L"[questitem"]	= &CHtmlMetaTagContainter::MetaQuestItemTagHandler;
	m_hmapTag[L"[skill"]		= &CHtmlMetaTagContainter::MetaSkillTagHandler;
	m_hmapTag[L"[link"]		= &CHtmlMetaTagContainter::MetaLinkTagHandler;
	m_hmapTag[L"[etc"]		= &CHtmlMetaTagContainter::MetaEtcTagHandler;

	m_hmapTag[L"[font"]		= &CHtmlMetaTagContainter::FontTagHandler;
	m_hmapTag[L"[strong"]	= &CHtmlMetaTagContainter::StrongTagHandler;
	m_hmapTag[L"[u"]			= &CHtmlMetaTagContainter::UnderLineTagHandler;

	m_pCurrMetaTag			= NULL;
	m_pCurrMetaTagItem		= NULL;
}

CHtmlMetaTagContainter::~CHtmlMetaTagContainter()
{
	Reset();
}

CHtmlMetaTagContainter* CHtmlMetaTagContainter::GetInstance(VOID)
{
	static CHtmlMetaTagContainter MetaTagContainer;
	return &MetaTagContainer;
}

VOID CHtmlMetaTagContainter::Reset(VOID)
{
	MapMetaTag::iterator it;

	for(it = m_mapMetaTag.begin(); it != m_mapMetaTag.end(); it++)
	{
		NTL_DELETE( (*it).second );
	}

	m_mapMetaTag.clear();

	m_pCurrMetaTag		= NULL;
	m_pCurrMetaTagItem	= NULL;
}

CHtmlMetaTag* CHtmlMetaTagContainter::FindMetaTag(INT nTagId)
{
	MapMetaTag::iterator it = m_mapMetaTag.find(nTagId);
	if(it == m_mapMetaTag.end())
		return NULL;

	return (*it).second;
}

VOID CHtmlMetaTagContainter::SetHtmlFromFile(const CHAR *pFileName)
{
	Reset();
	Parsing(pFileName);
}


VOID CHtmlMetaTagContainter::Parsing(const CHAR *pFileName)
{
	CHtmlTokenizer lexer(pFileName);

	if(lexer.IsEmpty())
		return;

	WCHAR wchBuffer[MAX_BUFFER_TEXT];

	while (true)
	{
		if(lexer.IsEmpty())
			break;
		// determine current state by peeking the next token in the input stream
		const std::wstring &token = lexer.GetNextToken();

		if(lexer.GetTokenType() == CHtmlTokenizer::CToken::TOKEN_TEXT)
		{
			//			AddTextItem(wchBuffer, token.c_str(), (INT)token.size());
		}
		else if(lexer.GetTokenType() == CHtmlTokenizer::CToken::TOKEN_TAG)
		{
			TagTokenParsing(wchBuffer, token.c_str(), (INT)token.size());
		}
	}
}

bool CHtmlMetaTagContainter::IsTokenSpace(WCHAR c)
{
	const wchar_t *pSpace = L" \t\r\n]";
	return wcschr(pSpace, c) != NULL;
}

VOID CHtmlMetaTagContainter::TagTokenParsing(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize)
{
	INT nCurPos = 0;

	while(true)
	{
		if(IsTokenSpace(pToken[nCurPos]))
		{
			std::wstring str(pToken, nCurPos);

			HMapTagToken::iterator it = m_hmapTag.find(str);
			if(it == m_hmapTag.end())
			{
			}
			else
			{
				(this->*((*it).second))(pwBuffer, &pToken[nCurPos], nSize - nCurPos); 
			}

			break;
		}

		nCurPos++;
	}
}

VOID CHtmlMetaTagContainter::MetaTagStartHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize)
{
	m_pCurrMetaTag = NTL_NEW CHtmlMetaTag;

	enum 
	{
		PARSE_TAG_EQUAL,
		PARSE_TAG_DATA
	};

	INT nState = PARSE_TAG_EQUAL;
	CComponentTokenizerW lexer(pToken, nSize);

	while (true)
	{
		const std::wstring &token = lexer.PeekNextToken();
		if (token == L"") 
			break;

		switch(nState)
		{
		case PARSE_TAG_EQUAL:
			if(token == L"=")
			{
				nState = PARSE_TAG_DATA;
			}
			else
			{
				return;
			}
			break;
		case PARSE_TAG_DATA:
			{
				INT nTagId	= _wtoi(token.c_str());
				m_pCurrMetaTag->SetTagId(nTagId);
				m_mapMetaTag[nTagId] = m_pCurrMetaTag;
			}
			break;
		default:
			break;
		}

		lexer.PopToPeek();
	}
}

VOID CHtmlMetaTagContainter::MetaDefaultTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize)
{
	m_pCurrMetaTagItem = NTL_NEW CHtmlMetaTagItem;
	m_pCurrMetaTagItem->m_wstrTagKey = HTML_META_KEY_DEFAULT;

	m_pCurrMetaTag->AddItem(m_pCurrMetaTagItem);
}

VOID CHtmlMetaTagContainter::MetaNpcTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize)
{
	m_pCurrMetaTagItem = NTL_NEW CHtmlMetaTagItem;
	m_pCurrMetaTagItem->m_wstrTagKey = HTML_META_KEY_NPC;

	m_pCurrMetaTag->AddItem(m_pCurrMetaTagItem);
}

VOID CHtmlMetaTagContainter::MetaMobTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize)
{
	m_pCurrMetaTagItem = NTL_NEW CHtmlMetaTagItem;
	m_pCurrMetaTagItem->m_wstrTagKey = HTML_META_KEY_MOB;

	m_pCurrMetaTag->AddItem(m_pCurrMetaTagItem);
}

VOID CHtmlMetaTagContainter::MetaUserTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize)
{
	m_pCurrMetaTagItem = NTL_NEW CHtmlMetaTagItem;
	m_pCurrMetaTagItem->m_wstrTagKey = HTML_META_KEY_USER;

	m_pCurrMetaTag->AddItem(m_pCurrMetaTagItem);
}

VOID CHtmlMetaTagContainter::MetaObjectTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize)
{
	m_pCurrMetaTagItem = NTL_NEW CHtmlMetaTagItem;
	m_pCurrMetaTagItem->m_wstrTagKey = HTML_META_KEY_OBJECT;

	m_pCurrMetaTag->AddItem(m_pCurrMetaTagItem);
}


VOID CHtmlMetaTagContainter::MetaPlaceTagHandler( WCHAR *pwBuffer, const WCHAR *pToken, INT nSize ) 
{
	m_pCurrMetaTagItem = NTL_NEW CHtmlMetaTagItem;
	m_pCurrMetaTagItem->m_wstrTagKey = HTML_META_KEY_PLACE;

	m_pCurrMetaTag->AddItem(m_pCurrMetaTagItem);
}

VOID CHtmlMetaTagContainter::MetaClassTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize)
{
	m_pCurrMetaTagItem = NTL_NEW CHtmlMetaTagItem;
	m_pCurrMetaTagItem->m_wstrTagKey = HTML_META_KEY_CLASS;

	m_pCurrMetaTag->AddItem(m_pCurrMetaTagItem);
}

VOID CHtmlMetaTagContainter::MetaRaceTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize)
{
	m_pCurrMetaTagItem = NTL_NEW CHtmlMetaTagItem;
	m_pCurrMetaTagItem->m_wstrTagKey = HTML_META_KEY_RACE;

	m_pCurrMetaTag->AddItem(m_pCurrMetaTagItem);
}

VOID CHtmlMetaTagContainter::MetaEtcTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize)
{
	m_pCurrMetaTagItem = NTL_NEW CHtmlMetaTagItem;
	m_pCurrMetaTagItem->m_wstrTagKey = HTML_META_KEY_ETC;

	m_pCurrMetaTag->AddItem(m_pCurrMetaTagItem);
}

VOID CHtmlMetaTagContainter::MetaItemTagHandler( WCHAR *pwBuffer, const WCHAR *pToken, INT nSize ) 
{
	m_pCurrMetaTagItem = NTL_NEW CHtmlMetaTagItem;
	m_pCurrMetaTagItem->m_wstrTagKey = HTML_META_KEY_ITEM;

	m_pCurrMetaTag->AddItem(m_pCurrMetaTagItem);
}

VOID CHtmlMetaTagContainter::MetaQuestItemTagHandler( WCHAR *pwBuffer, const WCHAR *pToken, INT nSize ) 
{
	m_pCurrMetaTagItem = NTL_NEW CHtmlMetaTagItem;
	m_pCurrMetaTagItem->m_wstrTagKey = HTML_META_KEY_QUESTITEM;

	m_pCurrMetaTag->AddItem(m_pCurrMetaTagItem);
}

VOID gui::CHtmlMetaTagContainter::MetaSkillTagHandler( WCHAR *pwBuffer, const WCHAR *pToken, INT nSize ) 
{
	m_pCurrMetaTagItem = NTL_NEW CHtmlMetaTagItem;
	m_pCurrMetaTagItem->m_wstrTagKey = HTML_META_KEY_SKILL;

	m_pCurrMetaTag->AddItem(m_pCurrMetaTagItem);
}

VOID CHtmlMetaTagContainter::MetaLinkTagHandler( WCHAR *pwBuffer, const WCHAR *pToken, INT nSize )
{
	m_pCurrMetaTagItem = NTL_NEW CHtmlMetaTagItem;
	m_pCurrMetaTagItem->m_wstrTagKey = HTML_META_KEY_LINK;

	m_pCurrMetaTag->AddItem(m_pCurrMetaTagItem);
}

VOID CHtmlMetaTagContainter::FontTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize)
{
	enum 
	{
		PARSE_FONT_ATTR,
		PARSE_FONT_EQUAL,
		PARSE_FONT_DATA,
	};

	enum
	{
		FONT_ATTR_SIZE,
		FONT_ATTR_COLOR,
		FONT_ATTR_FACE,
	};

	INT nState = PARSE_FONT_ATTR;
	INT nFontAttrValue = FONT_ATTR_SIZE;

	CComponentTokenizerW lexer(pToken, nSize);
	while (true)
	{
		const std::wstring &token = lexer.PeekNextToken();
		if (token == L"") 
			break;

		switch(nState)
		{
		case PARSE_FONT_ATTR:
			if(token == L"size")
			{
				nFontAttrValue = FONT_ATTR_SIZE;
				nState = PARSE_FONT_EQUAL;
			}
			else if(token == L"color")
			{
				nFontAttrValue = FONT_ATTR_COLOR;
				nState = PARSE_FONT_EQUAL;
			}
			else if(token == L"face")
			{
				nFontAttrValue = FONT_ATTR_FACE;
				nState = PARSE_FONT_EQUAL;
			}
			else if(token == L"]")
			{
				return;
			}
			else
				return;

			break;
		case PARSE_FONT_EQUAL:
			if(token == L"=")
			{
				nState = PARSE_FONT_DATA;
			}
			else
			{
				return;
			}
			break;
		case PARSE_FONT_DATA:
			{
				if(nFontAttrValue == FONT_ATTR_SIZE)
				{
					m_pCurrMetaTagItem->m_nFlags |= HTML_META_TAG_FLAG_FONT_SIZE;
					m_pCurrMetaTagItem->m_nFontSize = _wtoi(token.c_str()) * 10;
				}
				else if(nFontAttrValue == FONT_ATTR_COLOR)
				{
					COLORREF color = _ttoh(token.c_str());
					BYTE byRed = (BYTE)((color >> 16) & 0x000000ff);
					BYTE byGreen = (BYTE)((color >> 8) & 0x000000ff);
					BYTE byBlue = (BYTE)(color & 0x000000ff);
					m_pCurrMetaTagItem->m_nFlags |= HTML_META_TAG_FLAG_FONT_COLOR;
					m_pCurrMetaTagItem->m_FontColor = RGB(byRed, byGreen, byBlue);
				}
				else
				{
					m_pCurrMetaTagItem->m_wstrFontName = token;
				}

				nState = PARSE_FONT_ATTR;
			}
			break;
		default:
			break;
		}

		lexer.PopToPeek();
	}

}

VOID CHtmlMetaTagContainter::StrongTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize)
{
	m_pCurrMetaTagItem->m_nFlags |= HTML_META_TAG_FLAG_FONT_BOLD;

	m_pCurrMetaTagItem->m_nFontAttr |= EZ_ATTR_BOLD ;
}

VOID CHtmlMetaTagContainter::UnderLineTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize)
{
	m_pCurrMetaTagItem->m_nFlags |= HTML_META_TAG_FLAG_FONT_UNDERLINE;
	m_pCurrMetaTagItem->m_nFontAttr |= EZ_ATTR_UNDERLINE;
}


//////////////////////////////////////////////////////////////////////////

UserTagFunc CHtmlDoc::m_fnUserTag = NULL;

// 생성자
CHtmlDoc::CHtmlDoc()
{
	m_nMetaTagId	= -1;
	m_wstrFontName	= HTML_FONT_NAME;
	m_nFontSize		= HTML_FONT_SIZE;
	m_nFontAttr		= 0;
	m_CurTextColor	= HTML_FONT_TEXT_COLOR;
	m_CurBgColor	= HTML_FONT_BG_COLOR;
	m_dwAlign		= COMP_TEXT_LEFT | COMP_TEXT_DOWN;
	m_dwCurrAlign	= COMP_TEXT_LEFT | COMP_TEXT_DOWN;
	m_fFontRatio	= 1.0f;
	
	m_bLinkItem		= FALSE;
	m_byLinkType = eHTML_LINK_INVALID;
	m_nLinkIndex = 0xFFFFFFFF;
	m_wstrLinkFileName.clear();
	m_bInFontTag	= FALSE;
	m_pParentComp	= NULL;
	
	// 파싱 할때 태그에 따라서 관련된 함수를 연결시켜놓는다.
	RegisterTagParsingHandler();
}

CHtmlDoc::~CHtmlDoc()
{
	DeleteItem();
}

VOID CHtmlDoc::Reset(VOID)
{
	// 초기화 설정으로 리셋
	m_wstrFontName	= HTML_FONT_NAME;
	m_nFontSize		= HTML_FONT_SIZE;
	m_nFontAttr		= 0;
	m_CurTextColor	= HTML_FONT_TEXT_COLOR;
	
	m_bLinkItem = FALSE;
	m_byLinkType = eHTML_LINK_INVALID;
	m_nLinkIndex = 0xFFFFFFFF;
	m_wstrLinkFileName.clear();
	m_pParentComp = NULL;
}

VOID CHtmlDoc::SetDefaultAlign(DWORD dwAlign)
{
	m_dwAlign		= dwAlign;
	m_dwCurrAlign	= dwAlign;
}

VOID CHtmlDoc::SetTextBackgroundColor(COLORREF color)	
{
	m_CurBgColor = color;
}

VOID CHtmlDoc::SetHtmlFromFile(CComponent* pParentComp, const CHAR *pFileName)
{
	Reset();

	m_pParentComp = pParentComp;

	Parsing(pFileName);
}

VOID CHtmlDoc::SetHtmlFromMemory(CComponent* pParentComp, const WCHAR *pMemory, INT nSize)
{
	Reset();

	m_pParentComp = pParentComp;

	Parsing(pMemory, nSize);
}

VOID CHtmlDoc::RegisterTagParsingHandler(VOID)
{
	m_hmapTag[L"[metatag"] = &CHtmlDoc::MetaTagHandler;
	m_hmapTag[L"[font"] = &CHtmlDoc::FontTagHandler;
	m_hmapTag[L"[u]"] = &CHtmlDoc::UnderLineTagHandler;
	m_hmapTag[L"[u"] = &CHtmlDoc::UnderLineTagHandler;
	m_hmapTag[L"[strong]"] = &CHtmlDoc::BoldTagHandler;
	m_hmapTag[L"[strong"] = &CHtmlDoc::BoldTagHandler;
	m_hmapTag[L"[br"] = &CHtmlDoc::NewLineTagHandler;
	m_hmapTag[L"[br]"] = &CHtmlDoc::NewLineTagHandler;
	m_hmapTag[L"[align"] = &CHtmlDoc::AlignTagHandler;
	m_hmapTag[L"[img"] = &CHtmlDoc::ImageTagHandler;
	m_hmapTag[L"[link"] = &CHtmlDoc::LinkTagHandler;
	m_hmapTag[L"[flash"] = &CHtmlDoc::FlashTagHandler;
	m_hmapTag[L"[backpic"] = &CHtmlDoc::BackPicTagHandler;


	m_hmapTag[L"[npc"] = &CHtmlDoc::UserTagNpcHandler;
	m_hmapTag[L"[object"] = &CHtmlDoc::UserTagObjectHandler;
	m_hmapTag[L"[mob"] = &CHtmlDoc::UserTagMobHandler;
	m_hmapTag[L"[etc"] = &CHtmlDoc::UserTagEtcHandler;
	m_hmapTag[L"[user"] = &CHtmlDoc::UserTagUserHandler;
	m_hmapTag[L"[user]"] = &CHtmlDoc::UserTagUserHandler;
	m_hmapTag[L"[class"] = &CHtmlDoc::UserTagClassHandler;
	m_hmapTag[L"[class]"] = &CHtmlDoc::UserTagClassHandler;
	m_hmapTag[L"[race"] = &CHtmlDoc::UserTagRaceHandler;
	m_hmapTag[L"[race]"] = &CHtmlDoc::UserTagRaceHandler;
	m_hmapTag[L"[place"] = &CHtmlDoc::UserTagPlaceHandler;
	m_hmapTag[L"[item"] = &CHtmlDoc::UserTagItemHandler;
	m_hmapTag[L"[questitem"] = &CHtmlDoc::UserTagQuestItemHandler;
	m_hmapTag[L"[skill"] = &CHtmlDoc::UserTagSkillHandler;


	m_hmapTag[L"[/font]"] = &CHtmlDoc::FontEndTagHandler;
	m_hmapTag[L"[/font"] = &CHtmlDoc::FontEndTagHandler;
	m_hmapTag[L"[/u]"] = &CHtmlDoc::UnderLineEndTagHandler;
	m_hmapTag[L"[/u"] = &CHtmlDoc::UnderLineEndTagHandler;
	m_hmapTag[L"[/strong]"] = &CHtmlDoc::BoldEndTagHandler;
	m_hmapTag[L"[/strong"] = &CHtmlDoc::BoldEndTagHandler;
	m_hmapTag[L"[/align]"] = &CHtmlDoc::AlignEndTagHandler;
	m_hmapTag[L"[/align"] = &CHtmlDoc::AlignEndTagHandler;
	m_hmapTag[L"[/link"] = &CHtmlDoc::LinkEndTagHandler;
}

VOID CHtmlDoc::Parsing(const CHAR *pFileName)
{
	CHtmlTokenizer lexer(pFileName);

	if(lexer.IsEmpty())
	{
		// Todo :: 파일이 없거나 실패했을 때 에러가 났다고 알려준다.
		WCHAR awcBuffer[64] = L" ";
		::MultiByteToWideChar( GetACP(), 0, pFileName, -1, awcBuffer, 64 );

		std::wstring wstrErrMsg(L"Could not found : ");
		wstrErrMsg += awcBuffer;
		SetHtmlFromMemory( m_pParentComp, wstrErrMsg.c_str(), (INT)wstrErrMsg.size() );
		return;
	}
		

	WCHAR wchBuffer[MAX_BUFFER_TEXT];

	while (true)
	{
		if(lexer.IsEmpty())
			break;

		// 토큰마다 비교하여 텍스트는 Item에 추가하고 태그 는 연결된 파싱 함수를 콜 한다.
		std::wstring &token = lexer.GetNextToken();

		if(lexer.GetTokenType() == CHtmlTokenizer::CToken::TOKEN_TEXT)
		{
			if( m_bLinkItem )
				AddLinkItem(wchBuffer, token.c_str(), (INT)token.size(), HTML_META_KEY_LINK);
			else
				AddTextItem(wchBuffer, token.c_str(), (INT)token.size(), HTML_META_KEY_DEFAULT);
		}
		else if(lexer.GetTokenType() == CHtmlTokenizer::CToken::TOKEN_TAG)
		{
			// 소문자로 변환
			std::transform( token.begin(), token.end(), token.begin(), tolower );
			TagTokenParsing(wchBuffer, token.c_str(), (INT)token.size());
		}
	}
}

VOID CHtmlDoc::Parsing(const WCHAR *pMemory, INT nSize)
{
	CHtmlTokenizer lexer(pMemory, nSize);

	if(lexer.IsEmpty())
	{
		std::wstring wstrErrMsg(L"Data is Null.");
		SetHtmlFromMemory( m_pParentComp, wstrErrMsg.c_str(), (INT)wstrErrMsg.size() );
		return;
	}

	WCHAR wchBuffer[MAX_BUFFER_TEXT];

	while (true)
	{
		if(lexer.IsEmpty())
			break;
		// determine current state by peeking the next token in the input stream
		std::wstring token = lexer.GetNextToken();

		if(lexer.GetTokenType() == CHtmlTokenizer::CToken::TOKEN_TEXT)
		{
			if( m_bLinkItem )
				AddLinkItem(wchBuffer, token.c_str(), (INT)token.size(), HTML_META_KEY_LINK);
			else
				AddTextItem(wchBuffer, token.c_str(), (INT)token.size(), HTML_META_KEY_DEFAULT);
		}
		else if(lexer.GetTokenType() == CHtmlTokenizer::CToken::TOKEN_TAG)
		{
			// 소문자로 변환
			std::transform( token.begin(), token.end(), token.begin(), tolower );
			TagTokenParsing(wchBuffer, token.c_str(), (INT)token.size());
		}
	}
}

VOID CHtmlDoc::TagTokenParsing(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize)
{
	INT nCurPos = 0;

	while(true)
	{
		if(IsTokenSpace(pToken[nCurPos]))
		{
			std::wstring wstr(pToken, nCurPos);

			HMapTagToken::iterator it = m_hmapTag.find(wstr);
			if(it == m_hmapTag.end())
			{
				AddTextItem(pwBuffer, pToken, nSize, HTML_META_KEY_DEFAULT);
				// 모든 것을 string으로 생각한다.
			}
			else
			{
				// iterator에 들어 있는 함수를 호출
				(this->*((*it).second))(pwBuffer, &pToken[nCurPos], nSize - nCurPos); 
			}

			break;
		}

		nCurPos++;
	}
}

bool CHtmlDoc::IsTokenSpace(WCHAR c)
{
	const wchar_t *pSpace = L" \t\r\n]";
	return wcschr(pSpace, c) != NULL;
}

VOID CHtmlDoc::AddTextItem(const WCHAR *pwText, const WCHAR *pMetaKey)
{
	CHtml_StringItem *pStringItem = NTL_NEW CHtml_StringItem(m_pParentComp);

	std::wstring wstrFontName = m_wstrFontName;
	INT nFontSize			= m_nFontSize;
	INT	nFontAttr			= m_nFontAttr;
	COLORREF TextColor		= m_CurTextColor;

	CHtmlMetaTag *pMetaTag = GetMetaTagContainer()->FindMetaTag(m_nMetaTagId);
	// Meta Id가 있고 Font Tag가 사용되지 않았다면 MetaTag를 적용한다.
	if(pMetaTag && !m_bInFontTag)
	{
		CHtmlMetaTagItem *pMetaTagItem = pMetaTag->FindItem(pMetaKey);
		if(pMetaTagItem)
		{
			if(pMetaTagItem->m_wstrFontName.size() > 0)
				wstrFontName = pMetaTagItem->m_wstrFontName;

			if(pMetaTagItem->m_nFlags & HTML_META_TAG_FLAG_FONT_SIZE)
				nFontSize = pMetaTagItem->m_nFontSize;

			if(pMetaTagItem->m_nFlags & HTML_META_TAG_FLAG_FONT_COLOR)
				TextColor = pMetaTagItem->m_FontColor;

			nFontAttr = pMetaTagItem->m_nFontAttr;
		}
		// Default Meta Tag
		else
		{
			CHtmlMetaTagItem *pMetaTagDefault = pMetaTag->FindItem( HTML_META_KEY_DEFAULT );
			if( pMetaTagDefault )
			{
				if(pMetaTagDefault->m_wstrFontName.size() > 0)
					wstrFontName = pMetaTagDefault->m_wstrFontName;

				if(pMetaTagDefault->m_nFlags & HTML_META_TAG_FLAG_FONT_SIZE)
					nFontSize = pMetaTagDefault->m_nFontSize;

				if(pMetaTagDefault->m_nFlags & HTML_META_TAG_FLAG_FONT_COLOR)
					TextColor = pMetaTagDefault->m_FontColor;

				nFontAttr = pMetaTagDefault->m_nFontAttr;
			}
			else
			{
				assert( 0 && "Missing html script metatag!" );
			}
		}
	}

	// font scale ratio
	if( m_fFontRatio > 0.0f )
		pStringItem->SetText(pwText, 0, 0, wstrFontName.c_str(),  nFontSize, nFontAttr, m_fFontRatio, TextColor, m_CurBgColor, m_dwCurrAlign);
	else
		pStringItem->SetText(pwText, 0, 0, wstrFontName.c_str(),  nFontSize, nFontAttr, TextColor, m_CurBgColor, m_dwCurrAlign);

	m_listItem.push_back(pStringItem);
}

VOID CHtmlDoc::AddTextItem(WCHAR *pwBuffer, const WCHAR *pText, INT nSize, const WCHAR *pMetaKey)
{
	wcscpy_s( pwBuffer , MAX_BUFFER_TEXT, pText );

	CHtml_StringItem *pStringItem = NTL_NEW CHtml_StringItem(m_pParentComp);

	std::wstring wstrFontName = m_wstrFontName;
	INT nFontSize			= m_nFontSize;
	INT	nFontAttr			= m_nFontAttr;
	COLORREF TextColor		= m_CurTextColor;

	CHtmlMetaTag *pMetaTag = GetMetaTagContainer()->FindMetaTag(m_nMetaTagId);
	if(pMetaTag && !m_bInFontTag)
	{
		CHtmlMetaTagItem *pMetaTagItem = pMetaTag->FindItem(pMetaKey);
		if(pMetaTagItem)
		{
			if(pMetaTagItem->m_wstrFontName.size() > 0)
				wstrFontName = pMetaTagItem->m_wstrFontName;

			if(pMetaTagItem->m_nFlags & HTML_META_TAG_FLAG_FONT_SIZE)
				nFontSize = pMetaTagItem->m_nFontSize;

			if(pMetaTagItem->m_nFlags & HTML_META_TAG_FLAG_FONT_COLOR)
				TextColor = pMetaTagItem->m_FontColor;

			nFontAttr = pMetaTagItem->m_nFontAttr;
		}
	}

	if( m_fFontRatio > 0.0f )
		pStringItem->SetText(pwBuffer, 0, 0, wstrFontName.c_str(),  nFontSize, nFontAttr, m_fFontRatio, TextColor, m_CurBgColor, m_dwCurrAlign);
	else
		pStringItem->SetText(pwBuffer, 0, 0, wstrFontName.c_str(),  nFontSize, nFontAttr, TextColor, m_CurBgColor, m_dwCurrAlign);
	m_listItem.push_back(pStringItem);
}

VOID CHtmlDoc::AddLinkItem( const WCHAR *pwText, const WCHAR *pMetaKey )
{
	CHtml_LinkItem *pLinkItem = NTL_NEW CHtml_LinkItem(m_pParentComp);

	std::wstring wstrFontName = m_wstrFontName;
	INT nFontSize			= m_nFontSize;
	INT	nFontAttr			= m_nFontAttr;
	COLORREF TextColor		= m_CurTextColor;

	CHtmlMetaTag *pMetaTag = GetMetaTagContainer()->FindMetaTag(m_nMetaTagId);
	if(pMetaTag && !m_bInFontTag)
	{
		CHtmlMetaTagItem *pMetaTagItem = pMetaTag->FindItem(pMetaKey);
		if(pMetaTagItem)
		{
			if(pMetaTagItem->m_wstrFontName.size() > 0)
				wstrFontName = pMetaTagItem->m_wstrFontName;

			if(pMetaTagItem->m_nFlags & HTML_META_TAG_FLAG_FONT_SIZE)
				nFontSize = pMetaTagItem->m_nFontSize;

			if(pMetaTagItem->m_nFlags & HTML_META_TAG_FLAG_FONT_COLOR)
				TextColor = pMetaTagItem->m_FontColor;

			nFontAttr = pMetaTagItem->m_nFontAttr;
		}
		// Default Meta Tag
		else
		{
			CHtmlMetaTagItem *pMetaTagDefault = pMetaTag->FindItem( HTML_META_KEY_DEFAULT );
			if( pMetaTagDefault )
			{
				if(pMetaTagDefault->m_wstrFontName.size() > 0)
					wstrFontName = pMetaTagDefault->m_wstrFontName;

				if(pMetaTagDefault->m_nFlags & HTML_META_TAG_FLAG_FONT_SIZE)
					nFontSize = pMetaTagDefault->m_nFontSize;

				if(pMetaTagDefault->m_nFlags & HTML_META_TAG_FLAG_FONT_COLOR)
					TextColor = pMetaTagDefault->m_FontColor;

				nFontAttr = pMetaTagDefault->m_nFontAttr;
			}
			else
			{
				assert( 0 && "Missing html script metatag!" );
			}
		}
	}

	// font scale ratio
	if( m_fFontRatio > 0.0f )
		pLinkItem->SetText(pwText, 0, 0, wstrFontName.c_str(),  nFontSize, nFontAttr, m_fFontRatio, TextColor, m_CurBgColor, m_dwCurrAlign);
	else
		pLinkItem->SetText(pwText, 0, 0, wstrFontName.c_str(),  nFontSize, nFontAttr, TextColor, m_CurBgColor, m_dwCurrAlign);

	pLinkItem->SetLink( m_byLinkType, m_nLinkIndex, m_wstrLinkFileName );

	m_listItem.push_back(pLinkItem);
}

VOID CHtmlDoc::AddLinkItem(WCHAR *pwBuffer, const WCHAR *pText, INT nSize, const WCHAR *pMetaKey)
{
	wcscpy_s( pwBuffer , MAX_BUFFER_TEXT, pText );

	CHtml_LinkItem *pLinkItem = NTL_NEW CHtml_LinkItem(m_pParentComp);

	std::wstring wstrFontName = m_wstrFontName;
	INT nFontSize			= m_nFontSize;
	INT	nFontAttr			= m_nFontAttr;
	COLORREF TextColor		= m_CurTextColor;

	CHtmlMetaTag *pMetaTag = GetMetaTagContainer()->FindMetaTag(m_nMetaTagId);
	if(pMetaTag && !m_bInFontTag)
	{
		CHtmlMetaTagItem *pMetaTagItem = pMetaTag->FindItem(pMetaKey);
		if(pMetaTagItem)
		{
			if(pMetaTagItem->m_wstrFontName.size() > 0)
				wstrFontName = pMetaTagItem->m_wstrFontName;

			if(pMetaTagItem->m_nFlags & HTML_META_TAG_FLAG_FONT_SIZE)
				nFontSize = pMetaTagItem->m_nFontSize;

			if(pMetaTagItem->m_nFlags & HTML_META_TAG_FLAG_FONT_COLOR)
				TextColor = pMetaTagItem->m_FontColor;

			nFontAttr = pMetaTagItem->m_nFontAttr;
		}
	}

	if( m_fFontRatio > 0.0f )
		pLinkItem->SetText(pwBuffer, 0, 0, wstrFontName.c_str(),  nFontSize, nFontAttr, m_fFontRatio, TextColor, m_CurBgColor, m_dwCurrAlign);
	else
		pLinkItem->SetText(pwBuffer, 0, 0, wstrFontName.c_str(),  nFontSize, nFontAttr, TextColor, m_CurBgColor, m_dwCurrAlign);

	pLinkItem->SetLink( m_byLinkType, m_nLinkIndex, m_wstrLinkFileName );
	
	m_listItem.push_back(pLinkItem);
}

VOID CHtmlDoc::DeleteItem(VOID)
{
	ListHtmlItem::iterator it;
	for(it = m_listItem.begin(); it != m_listItem.end(); ++it)
	{
		NTL_DELETE( *it );
	}

	m_listItem.clear();
}

CHtmlDoc::ListHtmlItem& CHtmlDoc::GetItems(VOID)
{
	return m_listItem;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
// callback function

VOID CHtmlDoc::MetaTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize)
{
	enum 
	{
		PARSE_TAG_EQUAL,
		PARSE_TAG_DATA
	};

	INT nState = PARSE_TAG_EQUAL;
	CComponentTokenizerW lexer(pToken, nSize);

	while (true)
	{
		const std::wstring &token = lexer.PeekNextToken();
		if (token == L"") 
			break;

		switch(nState)
		{
		case PARSE_TAG_EQUAL:
			if(token == L"=")
			{
				nState = PARSE_TAG_DATA;
			}
			else
			{
				return;
			}
			break;
		case PARSE_TAG_DATA:
			{
				m_nMetaTagId = _wtoi(token.c_str());
			}
			break;
		default:
			break;
		}

		lexer.PopToPeek();
	}
}

VOID CHtmlDoc::FontTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize)
{
	enum 
	{
		PARSE_FONT_ATTR,
		PARSE_FONT_EQUAL,
		PARSE_FONT_DATA,
	};

	enum
	{
		FONT_ATTR_SIZE,
		FONT_ATTR_COLOR,
		FONT_ATTR_FACE,
	};

	INT nState = PARSE_FONT_ATTR;
	INT nFontAttrValue = FONT_ATTR_SIZE;
	m_bInFontTag = TRUE;

	CComponentTokenizerW lexer(pToken, nSize);
	while (true)
	{
		const std::wstring &token = lexer.PeekNextToken();
		if (token == L"") 
			break;

		switch(nState)
		{
		case PARSE_FONT_ATTR:
			if(token == L"size")
			{
				nFontAttrValue = FONT_ATTR_SIZE;
				nState = PARSE_FONT_EQUAL;
			}
			else if(token == L"color")
			{
				nFontAttrValue = FONT_ATTR_COLOR;
				nState = PARSE_FONT_EQUAL;
			}
			else if(token == L"face")
			{
				nFontAttrValue = FONT_ATTR_FACE;
				nState = PARSE_FONT_EQUAL;
			}
			else if(token == L"]")
			{
				return;
			}
			else
				return;

			break;
		case PARSE_FONT_EQUAL:
			if(token == L"=")
			{
				nState = PARSE_FONT_DATA;
			}
			else
			{
				return;
			}
			break;
		case PARSE_FONT_DATA:
			{
				if(nFontAttrValue == FONT_ATTR_SIZE)
				{
					m_nFontSize = _wtoi(token.c_str()) * 10;
				}
				else if(nFontAttrValue == FONT_ATTR_COLOR)
				{
					m_CurTextColor = _ttoh(token.c_str());
					BYTE byRed = (BYTE)((m_CurTextColor >> 16) & 0x000000ff);
					BYTE byGreen = (BYTE)((m_CurTextColor >> 8) & 0x000000ff);
					BYTE byBlue = (BYTE)(m_CurTextColor & 0x000000ff);
					m_CurTextColor = RGB(byRed, byGreen, byBlue);
				}
				else
				{
					m_wstrFontName = token;
				}

				nState = PARSE_FONT_ATTR;
			}
			break;
		default:
			break;
		}

		lexer.PopToPeek();
	}
}

VOID CHtmlDoc::UnderLineTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize)
{
	m_nFontAttr |= EZ_ATTR_UNDERLINE;
}

VOID CHtmlDoc::BoldTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize)
{
	m_nFontAttr |= EZ_ATTR_BOLD;
}

VOID CHtmlDoc::NewLineTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize)
{
	CHtml_NewLineItem *pNewLineItem = NTL_NEW CHtml_NewLineItem(m_pParentComp);
	pNewLineItem->SetFont(m_wstrFontName.c_str(),  m_nFontSize, m_nFontAttr);
	m_listItem.push_back(pNewLineItem);
}

VOID CHtmlDoc::AlignTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize)	
{
	enum 
	{
		PARSE_ALIGN_EQUAL,
		PARSE_ALIGN_DATA
	};


	INT nState = PARSE_ALIGN_EQUAL;

	CComponentTokenizerW lexer(pToken, nSize);
	while (true)
	{
		const std::wstring &token = lexer.PeekNextToken();
		if (token == L"") 
			break;

		switch(nState)
		{
		case PARSE_ALIGN_EQUAL:
			if(token == L"=")
			{
				nState = PARSE_ALIGN_DATA;
			}
			else
			{
				return;
			}
			break;
		case PARSE_ALIGN_DATA:
			if(token == L"left")
			{
				m_dwCurrAlign = m_dwCurrAlign & ~(COMP_TEXT_LEFT | COMP_TEXT_CENTER | COMP_TEXT_RIGHT);
				m_dwCurrAlign |= COMP_TEXT_LEFT;
			}
			else if(token == L"center")
			{
				m_dwCurrAlign = m_dwCurrAlign & ~(COMP_TEXT_LEFT | COMP_TEXT_CENTER | COMP_TEXT_RIGHT);
				m_dwCurrAlign |= COMP_TEXT_CENTER;
			}
			else if(token == L"right")
			{
				m_dwCurrAlign = m_dwCurrAlign & ~(COMP_TEXT_LEFT | COMP_TEXT_CENTER | COMP_TEXT_RIGHT);
				m_dwCurrAlign |= COMP_TEXT_RIGHT;
			}
			return;
			break;
		default:
			break;
		}

		lexer.PopToPeek();
	}
}

VOID CHtmlDoc::ImageTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize)
{
	enum 
	{
		PARSE_IMAGE_ATTR,
		PARSE_IMAGE_EQUAL,
		PARSE_IMAGE_DATA
	};

	enum
	{
		IMAGE_ATTR_SOURCE,
		IMAGE_ATTR_WIDTH,
		IMAGE_ATTR_HEIGHT,
		IMAGE_ATTR_ALIGN
	};

	INT nState = PARSE_IMAGE_ATTR;
	INT nImageAttrValue = IMAGE_ATTR_SOURCE;

	std::wstring strImage = L"None";
	INT nWidth	= -1;
	INT nHeight = -1;
	INT nAlign	= HTML_WRAP_ALIGN_NONE;

	CComponentTokenizerW lexer(pToken, nSize);
	while (true)
	{
		const std::wstring &token = lexer.PeekNextToken();
		if (token == L"") 
			break;

		switch(nState)
		{
		case PARSE_IMAGE_ATTR:
			if(token == L"src")
			{
				nImageAttrValue = IMAGE_ATTR_SOURCE;
				nState = PARSE_IMAGE_EQUAL;
			}
			else if(token == L"width")
			{
				nImageAttrValue = IMAGE_ATTR_WIDTH;
				nState = PARSE_IMAGE_EQUAL;
			}
			else if(token == L"height")
			{
				nImageAttrValue = IMAGE_ATTR_HEIGHT;
				nState = PARSE_IMAGE_EQUAL;
			}
			else if(token == L"align")
			{
				nImageAttrValue = IMAGE_ATTR_ALIGN;
				nState = PARSE_IMAGE_EQUAL;
			}
			else if(token == L"]")
			{
				CHtml_PictureItem *pPicItem = NTL_NEW CHtml_PictureItem(m_pParentComp);
				pPicItem->SetPicture(strImage.c_str(), nWidth, nHeight, nAlign);
				m_listItem.push_back(pPicItem);
				return;
			}
			else
				return;

			break;
		case PARSE_IMAGE_EQUAL:
			if(token == L"=")
			{
				nState = PARSE_IMAGE_DATA;
			}
			else
			{
				return;
			}
			break;
		case PARSE_IMAGE_DATA:
			{
				if(nImageAttrValue == IMAGE_ATTR_SOURCE)
				{
					strImage = token;
				}
				else if(nImageAttrValue == IMAGE_ATTR_WIDTH)
				{
					nWidth = _wtoi(token.c_str());
				}
				else if(nImageAttrValue == IMAGE_ATTR_HEIGHT)
				{
					nHeight = _wtoi(token.c_str());
				}
				else if(nImageAttrValue == IMAGE_ATTR_ALIGN)
				{
					if(token == L"left")
					{
						nAlign = HTML_WRAP_ALIGN_LEFT;
					}
					else if(token == L"right")
					{
						nAlign = HTML_WRAP_ALIGN_RIGHT;
					}
					else if(token == L"center")
					{
						nAlign = HTML_WRAP_ALIGN_CENTER;
					}
				}

				nState = PARSE_IMAGE_ATTR;
			}
			break;
		default:
			break;
		}

		lexer.PopToPeek();
	}
}

VOID CHtmlDoc::FlashTagHandler( WCHAR *pwBuffer, const WCHAR *pToken, INT nSize )
{
	enum 
	{
		PARSE_FLASH_ATTR,
		PARSE_FLASH_EQUAL,
		PARSE_FLASH_DATA
	};

	enum
	{
		FLASH_ATTR_SOURCE,
		FLASH_ATTR_WIDTH,
		FLASH_ATTR_HEIGHT,
		FLASH_ATTR_ALIGN,
		FLASH_ATTR_RESIZE,
		FLASH_ATTR_AUTOR,
	};

	INT nState = PARSE_FLASH_ATTR;
	INT nFlashAttrValue = FLASH_ATTR_SOURCE;

	std::wstring strFlash = L"None";
	INT nWidth	= -1;
	INT nHeight = -1;
	INT nAlign	= HTML_WRAP_ALIGN_NONE;
	INT nResizeType = CFlash::EXACTFIT;
	BOOL bAutoRewind = TRUE;

	CComponentTokenizerW lexer(pToken, nSize);
	while (true)
	{
		const std::wstring &token = lexer.PeekNextToken();
		if (token == L"") 
			break;

		switch(nState)
		{
		case PARSE_FLASH_ATTR:
			if(token == L"src")
			{
				nFlashAttrValue = FLASH_ATTR_SOURCE;
				nState = PARSE_FLASH_EQUAL;
			}
			else if(token == L"width")
			{
				nFlashAttrValue = FLASH_ATTR_WIDTH;
				nState = PARSE_FLASH_EQUAL;
			}
			else if(token == L"height")
			{
				nFlashAttrValue = FLASH_ATTR_HEIGHT;
				nState = PARSE_FLASH_EQUAL;
			}
			else if(token == L"align")
			{
				nFlashAttrValue = FLASH_ATTR_ALIGN;
				nState = PARSE_FLASH_EQUAL;
			}
			else if(token == L"resize")
			{
				nFlashAttrValue = FLASH_ATTR_RESIZE;
				nState = PARSE_FLASH_EQUAL;
			}
			else if(token == L"autor")
			{
				nFlashAttrValue = FLASH_ATTR_AUTOR;
				nState = PARSE_FLASH_EQUAL;
			}
			else if(token == L"]")
			{
				CHtml_FlashItem* pFlashItem = NTL_NEW CHtml_FlashItem(m_pParentComp);
				pFlashItem->SetFlash( strFlash.c_str(), nWidth, nHeight, nAlign, nResizeType, bAutoRewind );
				m_listItem.push_back(pFlashItem);
				return;
			}
			else
				return;

			break;
		case PARSE_FLASH_EQUAL:
			if(token == L"=")
			{
				nState = PARSE_FLASH_DATA;
			}
			else
			{
				return;
			}
			break;
		case PARSE_FLASH_DATA:
			{
				if(nFlashAttrValue == FLASH_ATTR_SOURCE)
				{
					strFlash = token;
				}
				else if(nFlashAttrValue == FLASH_ATTR_WIDTH)
				{
					nWidth = _wtoi(token.c_str());
				}
				else if(nFlashAttrValue == FLASH_ATTR_HEIGHT)
				{
					nHeight = _wtoi(token.c_str());
				}
				else if(nFlashAttrValue == FLASH_ATTR_ALIGN)
				{
					if(token == L"left")
						nAlign = HTML_WRAP_ALIGN_LEFT;
					else if(token == L"right")
						nAlign = HTML_WRAP_ALIGN_RIGHT;
					else if(token == L"center")
						nAlign = HTML_WRAP_ALIGN_CENTER;
				}
				else if(nFlashAttrValue == FLASH_ATTR_RESIZE )
				{
					if( token == L"noscale" )
						nResizeType = CFlash::NOSCALE;
					else if( token == L"showall" )
						nResizeType = CFlash::SHOWALL;
					else if( token == L"exactfit" )
						nResizeType = CFlash::EXACTFIT;
				}
				else if(nFlashAttrValue == FLASH_ATTR_AUTOR )
				{
					if( token == L"true" )
						bAutoRewind = TRUE;
					else if( token == L"false" )
						bAutoRewind = FALSE;
				}

				nState = PARSE_FLASH_ATTR;
			}
			break;
		default:
			break;
		}

		lexer.PopToPeek();
	}
}

VOID CHtmlDoc::BackPicTagHandler( WCHAR *pwcBuffer, const WCHAR *pToken, INT nSize )
{
	enum 
	{
		PARSE_BACKPIC_ATTR,
		PARSE_BACKPIC_EQUAL,
		PARSE_BACKPIC_DATA
	};

	enum
	{
		BACKPIC_ATTR_SOURCE,
		BACKPIC_ATTR_WIDTH,
		BACKPIC_ATTR_HEIGHT,
	};

	INT nState = PARSE_BACKPIC_ATTR;
	INT nBackPicAttrValue = BACKPIC_ATTR_SOURCE;

	std::wstring strBackPic = L"None";
	INT nWidth	= -1;
	INT nHeight = -1;

	CComponentTokenizerW lexer(pToken, nSize);
	while (true)
	{
		const std::wstring &token = lexer.PeekNextToken();
		if (token == L"") 
			break;

		switch(nState)
		{
		case PARSE_BACKPIC_ATTR:
			if(token == L"src")
			{
				nBackPicAttrValue = BACKPIC_ATTR_SOURCE;
				nState = PARSE_BACKPIC_EQUAL;
			}
			else if(token == L"width")
			{
				nBackPicAttrValue = BACKPIC_ATTR_WIDTH;
				nState = PARSE_BACKPIC_EQUAL;
			}
			else if(token == L"height")
			{
				nBackPicAttrValue = BACKPIC_ATTR_HEIGHT;
				nState = PARSE_BACKPIC_EQUAL;
			}
			else if(token == L"]")
			{
				CHtml_BackPicItem* pBackPicItem = NTL_NEW CHtml_BackPicItem(m_pParentComp);

				
				pBackPicItem->SetPicture(strBackPic.c_str(), nWidth, nHeight);
				m_listItem.push_back(pBackPicItem);
				
				return;
			}
			else
				return;

			break;
		case PARSE_BACKPIC_EQUAL:
			if(token == L"=")
			{
				nState = PARSE_BACKPIC_DATA;
			}
			else
			{
				return;
			}
			break;
		case PARSE_BACKPIC_DATA:
			{
				if(nBackPicAttrValue == BACKPIC_ATTR_SOURCE)
				{
					strBackPic = token;
				}
				else if(nBackPicAttrValue == BACKPIC_ATTR_WIDTH)
				{
					nWidth = _wtoi(token.c_str());
				}
				else if(nBackPicAttrValue == BACKPIC_ATTR_HEIGHT)
				{
					nHeight = _wtoi(token.c_str());
				}

				nState = PARSE_BACKPIC_ATTR;
			}
			break;
		default:
			break;
		}

		lexer.PopToPeek();
	}
}

/**
* \brief Hyper Link Tag
* \param pwBuffer	(WCHAR*)
* \param pToken		(const WCHAR*)
* \param nSize		(INT)
*/
VOID CHtmlDoc::LinkTagHandler( WCHAR *pwBuffer, const WCHAR *pToken, INT nSize )
{
	enum 
	{
		PARSE_LINK_ATTR,
		PARSE_LINK_EQUAL,
		PARSE_LINK_DATA
	};

	enum
	{
		LINK_ATTR_FILE,
		LINK_ATTR_INDEX,
	};

	INT nState = PARSE_LINK_ATTR;
	INT nLinkAttrValue = LINK_ATTR_FILE;
	m_bLinkItem = TRUE;

	CComponentTokenizerW lexer(pToken, nSize);
	while (true)
	{
		const std::wstring &token = lexer.PeekNextToken();
		if (token == L"") 
			break;

		switch(nState)
		{
		case PARSE_LINK_ATTR:
			if(token == L"file")
			{
				nLinkAttrValue = LINK_ATTR_FILE;
				nState = PARSE_LINK_EQUAL;
			}
			else if(token == L"index")
			{
				nLinkAttrValue = LINK_ATTR_INDEX;
				nState = PARSE_LINK_EQUAL;
			}
			else if(token == L"]")
			{
				return;
			}
			else
				return;

			break;
		case PARSE_LINK_EQUAL:
			if(token == L"=")
			{
				nState = PARSE_LINK_DATA;
			}
			else
			{
				return;
			}
			break;
		case PARSE_LINK_DATA:
			{
				if( nLinkAttrValue == LINK_ATTR_FILE )
				{
					m_wstrLinkFileName = token.c_str();
					m_byLinkType = eHTML_LINK_FILE;
				}
				else if( nLinkAttrValue == LINK_ATTR_INDEX )
				{
					m_nLinkIndex = _wtoi(token.c_str());
					m_byLinkType = eHTML_LINK_INTERFACE;
				}

				nState = PARSE_LINK_ATTR;
			}
			break;
		default:
			break;
		}

		lexer.PopToPeek();
	}
}

UINT CHtmlDoc::GetUserTagTableId(const WCHAR *pToken, INT nSize)
{
	UINT nId = 0;
	std::wstring strId;
	CComponentTokenizerW lexer(pToken, nSize);
	while (true)
	{
		const std::wstring &token = lexer.PeekNextToken();
		if (token == L"") 
			break;

		if(token == L"=")
		{
			strId = lexer.PeekNextToken();
			nId = (UINT)(_wtoi(strId.c_str()));
			break;
		}

		lexer.PopToPeek();
	}

	return nId;
}

VOID CHtmlDoc::UserTagNpcHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize)
{
	std::wstring str = L"npc";

	if( m_fnUserTag )
	{
		UINT nId = GetUserTagTableId(pToken, nSize);
		(*m_fnUserTag)(L"npc", nId, str);

		AddTextItem(str.c_str(), HTML_META_KEY_NPC);
	}
}

VOID CHtmlDoc::UserTagObjectHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize)
{
	std::wstring str = L"object";

	if(m_fnUserTag)
	{
		UINT nId = GetUserTagTableId(pToken, nSize);
		(*m_fnUserTag)(L"object", nId, str);

		AddTextItem(str.c_str(), HTML_META_KEY_OBJECT);
	}
}

VOID CHtmlDoc::UserTagMobHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize)
{
	std::wstring str = L"mob";

	if(m_fnUserTag)
	{
		UINT nId = GetUserTagTableId(pToken, nSize);
		(*m_fnUserTag)(L"mob", nId, str);

		AddTextItem(str.c_str(), HTML_META_KEY_MOB);
	}
}

VOID CHtmlDoc::UserTagEtcHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize)
{
	std::wstring str = L"etc";

	if(m_fnUserTag)
	{
		UINT nId = GetUserTagTableId(pToken, nSize);
		(*m_fnUserTag)(L"etc", nId, str);

		AddTextItem(str.c_str(), HTML_META_KEY_ETC);
	}
}

VOID CHtmlDoc::UserTagUserHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize)
{
	std::wstring str = L"user";

	if(m_fnUserTag)
	{
		(*m_fnUserTag)(L"user", 0, str);
		AddTextItem(str.c_str(), HTML_META_KEY_USER);
	}
}

VOID CHtmlDoc::UserTagClassHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize)
{
	std::wstring str = L"class";

	if(m_fnUserTag)
	{
		UINT nId = GetUserTagTableId(pToken, nSize);
		(*m_fnUserTag)(L"class", nId, str);
		AddTextItem(str.c_str(), HTML_META_KEY_CLASS);
	}
}

VOID CHtmlDoc::UserTagRaceHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize)
{
	std::wstring str = L"race";

	if(m_fnUserTag)
	{
		UINT nId = GetUserTagTableId(pToken, nSize);
		(*m_fnUserTag)(L"race", nId, str);
		AddTextItem(str.c_str(), HTML_META_KEY_RACE);
	}
}

VOID CHtmlDoc::UserTagPlaceHandler( WCHAR *pwcBuffer, const WCHAR *pToken, INT nSize ) 
{
	std::wstring str = L"place";

	if( m_fnUserTag )
	{
		UINT nId = GetUserTagTableId(pToken, nSize);
		(*m_fnUserTag)(L"place", nId, str );

		AddTextItem(str.c_str(), HTML_META_KEY_PLACE);
	}
}

VOID CHtmlDoc::UserTagItemHandler( WCHAR *pwcBuffer, const WCHAR *pToken, INT nSize ) 
{
	std::wstring str = L"item";

	if( m_fnUserTag )
	{
		UINT nId = GetUserTagTableId(pToken, nSize);
		(*m_fnUserTag)(L"item", nId, str );

		AddTextItem(str.c_str(), HTML_META_KEY_ITEM);
	}
}

VOID CHtmlDoc::UserTagQuestItemHandler( WCHAR *pwcBuffer, const WCHAR* pToken, INT nSize ) 
{
	std::wstring str = L"questitem";

	if( m_fnUserTag )
	{
		UINT nId = GetUserTagTableId(pToken, nSize);
		(*m_fnUserTag)(L"questitem", nId, str );

		AddTextItem(str.c_str(), HTML_META_KEY_QUESTITEM);
	}
}

VOID CHtmlDoc::UserTagSkillHandler( WCHAR *pwcBuffer, const WCHAR* pToken, INT nSize ) 
{
	std::wstring str = L"skill";

	if( m_fnUserTag )
	{
		UINT nId = GetUserTagTableId(pToken, nSize);
		(*m_fnUserTag)(L"skill", nId, str );

		AddTextItem(str.c_str(), HTML_META_KEY_SKILL);
	}
}

VOID CHtmlDoc::FontEndTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize)
{
	m_wstrFontName = HTML_FONT_NAME;
	m_nFontSize = HTML_FONT_SIZE;
	m_CurTextColor = HTML_FONT_TEXT_COLOR;
	m_nFontAttr = 0;
	m_bInFontTag = FALSE;
}

VOID CHtmlDoc::UnderLineEndTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize)
{
	m_nFontAttr = m_nFontAttr & ~EZ_ATTR_UNDERLINE;
}

VOID CHtmlDoc::BoldEndTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize)
{
	m_nFontAttr = m_nFontAttr & ~EZ_ATTR_BOLD;
}

VOID CHtmlDoc::AlignEndTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize)
{
	m_dwCurrAlign = m_dwAlign;
}

VOID CHtmlDoc::LinkEndTagHandler( WCHAR *pwBuffer, const WCHAR *pToken, INT nSize )
{
	m_CurTextColor = HTML_FONT_TEXT_COLOR;
	m_nFontAttr = 0;

	m_bLinkItem = FALSE;
	m_byLinkType = eHTML_LINK_INVALID;
	m_nLinkIndex = 0xFFFFFFFF;
	m_wstrLinkFileName.clear();
}

VOID CHtmlDoc::LinkUserTag(UserTagFunc fnTag)
{
	m_fnUserTag = fnTag;
}

VOID CHtmlDoc::SetFontRatio( float fRatio ) 
{
	m_fFontRatio = fRatio;
}




END_GUI
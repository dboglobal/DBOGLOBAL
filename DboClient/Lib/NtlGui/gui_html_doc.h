#ifndef __GUI_HTML_DOC_H__
#define __GUI_HTML_DOC_H__

//#include "gui_htmlbox_item.h"

typedef VOID (*UserTagFunc)(const WCHAR *pTag, UINT uId, std::wstring& str);

// Kell's Comment<2007-10-31>: Html_Doc 관련된 부분은 파일 이름을 빼고 모든 부분을 Wide Character 형으로 변경

START_GUI

class CHtml_Item;

#define HTML_META_TAG_FLAG_FONT_NAME		0x00000001
#define HTML_META_TAG_FLAG_FONT_SIZE		0x00000002
#define HTML_META_TAG_FLAG_FONT_COLOR		0x00000004
#define HTML_META_TAG_FLAG_FONT_BOLD		0x00000008
#define HTML_META_TAG_FLAG_FONT_UNDERLINE	0x00000010

#define HTML_META_KEY_DEFAULT				L"default"
#define HTML_META_KEY_NPC					L"npc"
#define HTML_META_KEY_MOB					L"mob"
#define HTML_META_KEY_USER					L"user"
#define HTML_META_KEY_OBJECT				L"object"
#define HTML_META_KEY_CLASS					L"class"
#define HTML_META_KEY_RACE					L"race"
#define HTML_META_KEY_PLACE					L"place"
#define HTML_META_KEY_ITEM					L"item"
#define HTML_META_KEY_QUESTITEM				L"questitem"
#define HTML_META_KEY_SKILL					L"skill"
#define HTML_META_KEY_LINK					L"link"
#define HTML_META_KEY_ETC					L"etc"

class CHtmlMetaTagItem
{
public:
	std::wstring	m_wstrTagKey;		// MetaTag 의 KeyName

	INT				m_nFlags;			// MetaTag의 Flags
	std::wstring	m_wstrFontName;		// Font
	COLORREF		m_FontColor;		// Color
	INT				m_nFontSize;		// Size
	INT				m_nFontAttr;		// Attribute

public:

	CHtmlMetaTagItem();
	~CHtmlMetaTagItem();
};

//////////////////////////////////////////////////////////////////////////

class CHtmlMetaTag
{
private:
	typedef std::list<CHtmlMetaTagItem*> ListMetaTagItem;

	INT				m_nTagId;					// 현재 Tag의 ID
	ListMetaTagItem	m_listMetaTagItem;			

public:

	CHtmlMetaTag();
	~CHtmlMetaTag();

	VOID				SetTagId(INT nTagId);
	INT					GetTagId(void) const;

	VOID				AddItem(CHtmlMetaTagItem *pItem);
	CHtmlMetaTagItem*	FindItem(const wchar_t *pKey);		// List에 있는 MetaTagItem 을 pKey로 찾는다.
};	

//////////////////////////////////////////////////////////////////////////

class CHtmlMetaTagContainter
{
private:
	typedef std::map<INT, CHtmlMetaTag*> MapMetaTag;
	MapMetaTag			m_mapMetaTag;

	typedef VOID (CHtmlMetaTagContainter::*ParsingCall)(WCHAR*, const WCHAR*, INT);
	typedef std::unordered_map<std::wstring, ParsingCall> HMapTagToken;
	HMapTagToken		m_hmapTag;						// hash-map Token

	CHtmlMetaTag		*m_pCurrMetaTag;
	CHtmlMetaTagItem	*m_pCurrMetaTagItem;

private:
	VOID			Reset(VOID);

	bool			IsTokenSpace(WCHAR c);	// c가 스페이스인지 검사한다.

	VOID			TagTokenParsing(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);
	VOID			Parsing(const CHAR *pFileName);

	VOID			MetaTagStartHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);
	VOID			MetaDefaultTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);
	VOID			MetaNpcTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);
	VOID			MetaMobTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);
	VOID			MetaUserTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);
	VOID			MetaObjectTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);	
	VOID			MetaPlaceTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);
	VOID			MetaClassTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);	
	VOID			MetaRaceTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);
	VOID			MetaItemTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);
	VOID			MetaQuestItemTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);
	VOID			MetaSkillTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);
	VOID			MetaLinkTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);
	VOID			MetaEtcTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);	

	VOID			FontTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);
	VOID			StrongTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);
	VOID			UnderLineTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);

public:
	CHtmlMetaTagContainter();
	~CHtmlMetaTagContainter();

	static CHtmlMetaTagContainter* GetInstance(VOID);			

	CHtmlMetaTag*	FindMetaTag(INT nTagId);
	VOID			SetHtmlFromFile(const CHAR *pFileName);
};

static CHtmlMetaTagContainter* GetMetaTagContainer(void)
{
	return CHtmlMetaTagContainter::GetInstance();
}

//////////////////////////////////////////////////////////////////////////

class CHtmlDoc
{
public:
	// 현재의 font 속성.
	INT				m_nMetaTagId;
	std::wstring	m_wstrFontName;		// font name
	INT				m_nFontSize;		// font size
	INT				m_nFontAttr;		// font attribute
	float			m_fFontRatio;		// font ratio
	COLORREF		m_CurTextColor;		// text color
	COLORREF		m_CurBgColor;		// text background color
	DWORD			m_dwAlign;			// text 정렬.
	DWORD			m_dwCurrAlign;		// text current 정렬.
	BOOL			m_bLinkItem;		// Link Text
	BOOL			m_bInFontTag;		// Font Tag안에 있는지 확인

	// Link 속성
	BYTE			m_byLinkType;		// Link Type
	std::wstring	m_wstrLinkFileName;	// Link될 File name
	INT				m_nLinkIndex;		// Link될 Index

	// Doc 을 가지고 있는 Gui Component의 포인터
	CComponent*		m_pParentComp;
	
	typedef std::list<CHtml_Item*> ListHtmlItem;
	ListHtmlItem m_listItem;

	// tag token parsing handler
	typedef VOID (CHtmlDoc::*ParsingCall)(WCHAR*, const WCHAR*, INT);
	typedef std::unordered_map<std::wstring, ParsingCall> HMapTagToken;
	HMapTagToken m_hmapTag;

	static UserTagFunc m_fnUserTag;

private:

	VOID Reset(VOID);

	VOID Parsing(const CHAR *pFileName);
	VOID Parsing(const WCHAR *pMemory, INT nSize);
	VOID TagTokenParsing(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);
	bool IsTokenSpace(WCHAR c);
	VOID AddTextItem(const WCHAR *pwText, const WCHAR *pMetaKey);
	VOID AddTextItem(WCHAR *pwBuffer, const WCHAR *pText, INT nSize, const WCHAR *pMetaKey); 
	VOID AddLinkItem(const WCHAR *pwText, const WCHAR *pMetaKey);
	VOID AddLinkItem(WCHAR *pwBuffer, const WCHAR *pText, INT nSize, const WCHAR *pMetaKey);
	
	VOID DeleteItem(VOID);

	UINT GetUserTagTableId(const WCHAR *pToken, INT nSize);

	// callback function
private:
	VOID MetaTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);
	VOID FontTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);
	VOID UnderLineTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);
	VOID BoldTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);
	VOID NewLineTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);
	VOID AlignTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);	
	VOID ImageTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);	
	VOID LinkTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);
	VOID FlashTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);
	VOID BackPicTagHandler(WCHAR *pwcBuffer, const WCHAR *pToken, INT nSize);

	VOID UserTagNpcHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);
	VOID UserTagMobHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);
	VOID UserTagUserHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);
	VOID UserTagObjectHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);
	VOID UserTagClassHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);
	VOID UserTagRaceHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);
	VOID UserTagPlaceHandler(WCHAR *pwcBuffer, const WCHAR *pToken, INT nSize);
	VOID UserTagItemHandler(WCHAR *pwcBuffer, const WCHAR *pToken, INT nSize);
	VOID UserTagQuestItemHandler(WCHAR *pwcBuffer, const WCHAR* pToken, INT nSize);
	VOID UserTagSkillHandler(WCHAR *pwcBuffer, const WCHAR* pToken, INT nSize);
	VOID UserTagEtcHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);

	VOID FontEndTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);
	VOID UnderLineEndTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);
	VOID BoldEndTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);
	VOID AlignEndTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);
	VOID LinkEndTagHandler(WCHAR *pwBuffer, const WCHAR *pToken, INT nSize);	

public:
	CHtmlDoc();
	~CHtmlDoc();

	VOID SetDefaultAlign(DWORD dwAlign);	

	VOID SetTextBackgroundColor(COLORREF color);

	VOID SetHtmlFromFile(CComponent* pParentComp, const CHAR *pFileName);
	VOID SetHtmlFromMemory(CComponent* pParentComp, const WCHAR *pMemory, INT nSize);

	VOID RegisterTagParsingHandler(VOID);

	ListHtmlItem& GetItems(VOID);

	//: callback
	static VOID LinkUserTag(UserTagFunc fnTag);

	VOID SetFontRatio( float fRatio );
};

END_GUI

#endif
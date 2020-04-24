#ifndef __GUI_HTMLBOX_ITEM_H__
#define __GUI_HTMLBOX_ITEM_H__

#include "gui_define.h"
//#include "precomp_ntlflasher.h"
#include "gui_htmlbox.h"
#include "gui_flash.h"

START_GUI

#define	HTML_ITEM_NONE			0			///<
#define HTML_ITEM_NEWLINE		1			///< [br]
#define HTML_ITEM_STRING		2			///< 일반 텍스트
#define HTML_ITEM_PICTURE		3			///< [img
#define HTML_ITEM_LINK			4			///< [link
#define HTML_ITEM_FLASH			5			///< [flash
#define HTML_ITEM_BACKPIC		6			///< [backpic

/**
* \brief HTML의 ALIGN 속성(아이템별로 지정)
*/
enum 
{
	HTML_WRAP_ALIGN_NONE	= 0x00,			
	HTML_WRAP_ALIGN_LEFT	= 0x01,			 
	HTML_WRAP_ALIGN_RIGHT	= 0x02,
	HTML_WRAP_ALIGN_CENTER	= 0x04
};

/**
* \brief LINK Tag의 Link 속성
*/
enum eLinkType
{
	eHTML_LINK_FILE	= 0,			///< File 이름으로 링크
	eHTML_LINK_INTERFACE,			///< Callback

	eHTML_LINK_COUNT,
	eHTML_LINK_INVALID = eHTML_LINK_COUNT
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Unicode

class CHtml_Item
{
public:

	INT		m_nPosX;			// 현재 위치.
	INT		m_nPosY;			// 현재 위치.
	INT		m_nCX;				// size
	INT		m_nCY;				// size
	INT		m_nCurrLine;		// 현재 line index
	BYTE	m_byType;			// item type
	DWORD	m_dwAlign;			// 정렬
	DWORD	m_dwWrapAlign;		// picture item과 다른 item과의 연관관계
	CComponent* m_pParentComp;	// 부모 Component 의 포인터(Html의 포인터)
	
protected:

	CHtml_Item() 
	{
		m_nPosX		= 0;
		m_nPosY		= 0;
		m_nCX		= 0;
		m_nCY		= 0;
		m_nCurrLine = 0;
		m_byType	= HTML_ITEM_NONE;
		m_dwWrapAlign = HTML_WRAP_ALIGN_NONE;
		m_pParentComp = NULL;
	}

public:

	virtual ~CHtml_Item() {}

	VOID			SetType(BYTE byType)						{ m_byType = byType; }
	BYTE			GetType(VOID) const							{ return m_byType; }

	virtual VOID	SetPositionX(INT nPosX);
	virtual VOID	SetPositionY(INT nPosY);
	virtual VOID	SetPosition(INT nPosX, INT nPosY);
	VOID			GetPosition(INT& nPosX, INT& nPosY) const	{nPosX = m_nPosX, nPosY = m_nPosY; }

	VOID			SetSize(INT nCX, INT nCY);
	VOID			GetSize(INT& nCX, INT& nCY) const			{ nCX = m_nCX, nCY = m_nCY; }

	CRectangle		GetRect()									{ return CRectangle(m_nPosX, m_nPosY, m_nPosX+m_nCX, m_nPosY+m_nCY); }

	INT				GetWidth()									{ return m_nCX; }
	INT				GetHeight()									{ return m_nCY;	}

	VOID			SetCurrLine(INT nCurrLine);
	INT				GetCurrLine(VOID) const						{ return m_nCurrLine; }		

	DWORD			GetAlign(VOID);

	VOID			SetWrapAlign(DWORD dwWrapAlign)				{ m_dwWrapAlign = dwWrapAlign; }
	DWORD			GetWrapAlign(VOID) const					{ return m_dwWrapAlign; }

	virtual VOID	Update(float fElapsed)						{}
	virtual	VOID	ResetItemDisposition(VOID)					{}

	virtual CHtml_Item* ItemDisposition(CRectangle& rect, CRectangle rtClipping, std::list<CHtml_Item*>& listItems, INT nLineIntervalY, BOOL bFixedTopBottom, BOOL& bLineOver) { return NULL; } 

	virtual VOID	ItemLineBottomSorting(INT nBottom) {}

	virtual VOID	OnBackPaint( const CRectangle& rt ) {}
	virtual VOID	OnPrePaint(INT nOffsetX, INT nOffsetY, const CRectangle& rect, CTexture *pTexture) {}
	virtual VOID	OnPaint( const CRectangle& rt) {}
};

class CHtml_NewLineItem : public CHtml_Item
{
private:

	std::wstring	m_wstrFontName;		// font name
	INT				m_nFontSize;		// font size
	INT				m_nFontAttr;		// font attribute

public:

	CHtml_NewLineItem(CComponent* pComponent);
	~CHtml_NewLineItem();

	VOID			SetFont(const WCHAR *pFontName, INT nFontSize, INT nFontAttr);
	CHtml_Item*	ItemDisposition(CRectangle& rect, CRectangle rtClipping, std::list<CHtml_Item*>& listItems, INT nLineIntervalY, BOOL bFixedTopBottom, BOOL& bLineOver); 
	VOID			ItemLineBottomSorting(INT nBottom);
};

class CHtml_StringItem : public CHtml_Item
{
public:
	INT			m_nSerachIdx;
	INT			m_nTextLen;
	CUniBuffer *m_pUniBuffer;
	std::wstring m_wstrFontName;		// font name
	INT			m_nFontSize;
	INT			m_nFontAttr;
	float		m_fFontRatio;			// font Ratio
	COLORREF	m_TextColor;
	COLORREF	m_BgColor;
	CGuiFont	*m_pFont;

public:

	CHtml_StringItem(CComponent* pComponent);
	~CHtml_StringItem();

	VOID		SetText(const WCHAR *pString, INT nPosX, INT nPosY, 
		const WCHAR *pFontName, INT nFontSize, INT nFontAttr, 
		COLORREF TextColor, COLORREF BgColor,
		DWORD dwAlign);
	VOID		SetText(const WCHAR *pString, INT nPosX, INT nPosY, 
		const WCHAR *pFontName, INT nFontSize, INT nFontAttr, float fFontRatio,
		COLORREF TextColor, COLORREF BgColor,
		DWORD dwAlign);
	const WCHAR* GetText(VOID);

	VOID		ResetItemDisposition(VOID);
	CHtml_Item* ItemDisposition(CRectangle& rect, CRectangle rtClipping, std::list<CHtml_Item*>& listItems, INT nLineIntervalY, BOOL bFixedTopBottom, BOOL& bLineOver); 
	VOID		ItemLineBottomSorting(INT nBottom);
	VOID		OnPrePaint(INT nOffsetX, INT nOffsetY, const CRectangle& rect, CTexture *pTexture);
	
	// 스트링에 들어있는 UniBuffer의 글자 크기 하나하나를 리스트에 담는다.
	VOID		GetItemRect( std::list<CRectangle>& list );
	
	VOID		SetFontRatio( float fRatio );
};

class CHtml_LinkItem : public CHtml_Item
{
	INT			m_nSerachIdx;
	INT			m_nTextLen;
	CUniBuffer *m_pUniBuffer;
	std::wstring m_wstrFontName;		// font name
	INT			m_nFontSize;
	INT			m_nFontAttr;
	float		m_fFontRatio;			// font Ratio
	COLORREF	m_TextColor;
	COLORREF	m_TextOriginColor;
	COLORREF	m_BgColor;
	CGuiFont	*m_pFont;
	BOOL		m_bInverseColor;
	
	// Link
	std::wstring m_wstrFileName;
	INT			m_nLinkIndex;
	INT			m_nLinkType;

public:
	CHtml_LinkItem(CComponent* pComponent);
	~CHtml_LinkItem();

	VOID		SetText(const WCHAR *pString, INT nPosX, INT nPosY, 
		const WCHAR *pFontName, INT nFontSize, INT nFontAttr, 
		COLORREF TextColor, COLORREF BgColor,
		DWORD dwAlign);
	VOID		SetText(const WCHAR *pString, INT nPosX, INT nPosY, 
		const WCHAR *pFontName, INT nFontSize, INT nFontAttr, float fFontRatio,
		COLORREF TextColor, COLORREF BgColor,
		DWORD dwAlign);
	const WCHAR* GetText(VOID);

	BOOL			SetInverseColor( BOOL bInverse );

	// Hyper Link
	VOID			SetLink( INT nLinkType, INT nLinkIndex );
	VOID			SetLink( INT nLinkType, std::wstring& wstrFileName );
	VOID			SetLink( INT nLinkType, INT nLinkIndex, std::wstring& wstrFileName );
	INT				GetLinkType();
	INT				GetLinkIndex(); 
	std::wstring&	GetLinkFilename(); 

	VOID		ResetItemDisposition(VOID);
	CHtml_Item* ItemDisposition(CRectangle& rect, CRectangle rtClipping, std::list<CHtml_Item*>& listItems, INT nLineIntervalY, BOOL bFixedTopBottom, BOOL& bLineOver); 
	VOID		ItemLineBottomSorting(INT nBottom);
	VOID		OnPrePaint(INT nOffsetX, INT nOffsetY, const CRectangle& rect, CTexture *pTexture);

	// 스트링에 들어있는 UniBuffer의 글자 크기 하나하나를 리스트에 담는다.
	VOID		GetItemRect( std::list<CRectangle>& list );

	VOID		SetFontRatio( float fRatio );
};

class CHtml_PictureItem : public CHtml_Item
{
private:

	PlaneSnapShot	m_SnapShot;
	CTexture		*m_pTexture;

	std::wstring		m_strPic;
	INT				m_nRenderPosX;
	INT				m_nRenderPosY;

public:

	CHtml_PictureItem(CComponent* pComponent);
	~CHtml_PictureItem();

	VOID		SetPositionX(INT nPosX);
	VOID		SetPositionY(INT nPosY);
	VOID		SetPosition(INT nPosX, INT nPosY);

	VOID		SetPicture(const WCHAR *pPic, INT nWidth, INT nHeight, INT nWrapAlign);
	CHtml_Item* ItemDisposition(CRectangle& rect, CRectangle rtClipping, std::list<CHtml_Item*>& listItems, INT nLineIntervalY, BOOL bFixedTopBottom, BOOL& bLineOver);
	VOID		ItemLineBottomSorting(INT nBottom);
	VOID		OnPrePaint(INT nOffsetX, INT nOffsetY, const CRectangle& rect, CTexture *pTexture);
	VOID		OnPaint(const CRectangle& rt);
	VOID		OnSetAlpha( BYTE ucAlpha );
};

//class CNtlFlasher;

class CHtml_FlashItem : public CHtml_Item
{
private:
	CFlash*				m_pFlash;	
	std::wstring		m_strFlash;
	INT					m_nRenderPosX;
	INT					m_nRenderPosY;
	BOOL				m_bAutoRewind;
	INT					m_nResizeType;

public:

	CHtml_FlashItem(CComponent* pComponent);
	~CHtml_FlashItem();

	VOID		SetPositionX(INT nPosX);
	VOID		SetPositionY(INT nPosY);
	VOID		SetPosition(INT nPosX, INT nPosY);
	VOID		SetPosition();
	VOID		Update(float fElapsed);

	VOID		SetFlash(const WCHAR *pFlash, INT& nWidth, INT& nHeight, INT nWrapAlign, INT nResizeType, BOOL bAutoRewind, BOOL bRender = FALSE);
	CHtml_Item* ItemDisposition(CRectangle& rect, CRectangle rtClipping, std::list<CHtml_Item*>& listItems, INT nLineIntervalY, BOOL bFixedTopBottom, BOOL& bLineOver);
	VOID		ItemLineBottomSorting(INT nBottom);
	VOID		OnPrePaint(INT nOffsetX, INT nOffsetY, const CRectangle& rect, CTexture *pTexture);
	VOID		OnPaint(const CRectangle& rt);
	VOID		OnSetAlpha( BYTE ucAlpha );
	
};

class CHtml_BackPicItem : public CHtml_Item
{
private:

	PlaneSnapShot	m_SnapShot;
	CTexture		*m_pTexture;

	std::wstring	m_strPic;
	INT				m_nRenderPosX;
	INT				m_nRenderPosY;

public:

	CHtml_BackPicItem(CComponent* pComponent);
	~CHtml_BackPicItem();

	VOID		SetPicture(const WCHAR *pPic, INT nWidth, INT nHeight);
	VOID		OnBackPaint(const CRectangle& rt);
	VOID		OnSetAlpha( BYTE ucAlpha );
};

END_GUI

#endif

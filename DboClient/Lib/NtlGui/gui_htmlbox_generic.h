#ifndef __GUI_HTMLBOX_GENERIC_H__
#define __GUI_HTMLBOX_GENERIC_H__

#include "gui_htmlbox.h"
#include "gui_htmlbox_item.h"

START_GUI

// HTML ITEM 갯수를 정의
#define MAX_HTML_ITEM			1000
#define MAX_HTML_ITEM_ALIGN		100	
#define HTML_LINE_INTERVAL		5		///< 라인당 간격을 Define으로 정의(기본)

/**
* \brief Link 되는 영역을 저장하고 있는다.
*/
struct sLinkRect
{
	CHtml_LinkItem* pLinkItem;			///< 링크 아이템
	CRectangle		rectArea;			///< 계산될 영역( 스크롤, 페이지 포함 )
};

// Callback
typedef VOID (*HtmlLinkTagFunc)(gui::CComponent* pComponent, BYTE byType, UINT32 uiIndex, std::wstring& strFilename);

// 전방선언
class CHtmlDoc;

/**
* \ingroup client
* \brief RowHtml_Item 클래스는 Html에서 각 행을 담고 있는 Item들이다.
*/
class CRowHtml_Item
{
public:

	CRowHtml_Item(INT nCurrIdx, CRectangle rtClient, BOOL bFixedTopDown, INT nLineIntervalY);
	~CRowHtml_Item();

	CHtml_Item*	ItemDisposition(CHtml_Item *pItem, INT nLineIntervalY);
	VOID		ItemFinishDisposition(VOID);
	VOID		ItemTBCSorting(VOID);

	VOID		OnPrePaint(INT nOffsetX, INT nOffsetY, const CRectangle& rtPaint, CTexture *pTexture, DWORD dwAlign);
	VOID		OnPaint(const CRectangle& rtScreen);
	VOID		OnBackPaint(const CRectangle& rtScreen);
	VOID		OnSetAlpha( BYTE ucAlpha );

	VOID		Update( float fElasped );

	BOOL		IsFull(VOID);
	BOOL		IsEmpty(VOID);
	CRectangle	GetArea(VOID);
	INT			GetAreaHeight(VOID);
	INT			GetCurrentIndex(VOID);
	INT			GetItemCount(VOID);

	// 리스트에 현재 가지고 있는 string 아이템의 모든 글자 크기를 기록한다.
	VOID		GetItemRect( std::list<CRectangle>& list );
	VOID		GetLinkRect( std::list<sLinkRect>& list );

private:
	INT GetAlignOffsetX(DWORD dwAlign, INT nClipWidth, INT nItemWidth);
	INT	GetChildRowHtmlWidth(VOID);
	INT GetChildRowHtmlHeight(INT nLineIntervalY);

private:
	INT						m_nCurrIdx;				///< 현재 Index ( Line Index )
	INT						m_nLineIntervalY;		///< Item 당 Y 간격
	BOOL					m_bFull;				///< 현재 Row가 꽉 찼는지의 여부
	BOOL					m_bFixedTopDown;
	CRectangle				m_rtClipping;
	CRectangle				m_rtArea;
	CRectangle				m_rtCurrArea;
	DWORD					m_dwWarpAlign;
	std::list<CHtml_Item*>	m_listItems;
	std::list<CRowHtml_Item*> m_listRowHtmlItem;	
};

/**
* \ingroup client
* \brief HtmlBox가 사용할 수 있는 Generic 속성들
*/
class CHtmlBox_Generic
{
public:
	CHtmlBox_Generic(CHtmlBox *pSelf, BOOL bDynamic = FALSE);
	~CHtmlBox_Generic();

	VOID	SetHtmlFromFile(const CHAR *pFileName);					///< 파일에서 파싱
	VOID	SetHtmlFromMemory(const WCHAR *pMemory, INT nSize);		///< 메모리에서 파싱
	
	BOOL	SetFontRatio( float fRatio );							///< 텍스트 배율 지정
	VOID	SetStyle(DWORD dwStyle);								///< Style 지정
	VOID	SetLineSpace(INT nSpace);								///< 라인 간격 지정
	VOID	SetTextBackgroundColor(COLORREF color);					///< 텍스트 배경색깔 지정

	VOID	Clear(VOID);									///< Item, Texture 초기화

public:
	//: Sets the surface.
	VOID	AddSurface( CSurface surface );
	
	VOID	CreateTextTexture(void);						///< Text Texture 생성
	VOID	DeleteTextTexture(void);						///< Text Texture 해제

	//: 2007-11-16 한 글자씩 순서대로 출력하는 모드
public:
	VOID	SetIntervalTextEnable(float fIntervalTime);		///< 모드 온 : 출력 간격 (초)
	VOID	SetIntervalTextDisable();						///< 한 글자씩 출력하는 모드를 해제

	VOID	Update(float fElapsedTime);						///< 업데이트

	INT		GetIntervalPageNums(VOID);						///< 출력되는 총 페이지 갯수
	INT		GetCurrentPage(VOID);							///< 현재 페이지

	BOOL	SetNextPage(VOID);								///< 다음 페이지
	BOOL	SetPrevPage(VOID);								///< 이전 페이지
	BOOL	SetSkipPage(VOID);								///< 현재 페이지 SKIP

	BOOL	SetPlayPage(INT nPage);							///< nPage부터 플레이

	VOID	SetPause( BOOL bPause = TRUE );					///< 일시멈춤 ( FALSE를 넣으면 재개 )
	VOID	SetImmediate( BOOL bImmediate = TRUE );			///< 페이지를 바로 보여주는 모드

	VOID	SetPresentNum(INT nNum);						///< 한번에 출력될 글자의 갯수 설정
	INT		GetPresentNum();								///< 현재 출력되는 글자 갯수

	CSignal_v1<int> m_SigPageDone;							///< 페이지가 완료 됐을 때 날라가는 호출되는 Signal

private:
	VOID	DeleteItem(VOID);								///< m_pHtmlDoc 해제
	VOID	DeleteRenderItem(VOID);							///< m_listRowHtml 초기화
	
	VOID	ItemDisposition(VOID);							///< HtmlBox 컴포넌트에 맞춘 Item들 계산
	VOID	ItemLineSorting(VOID);							///< HtmlBox 영역 계산
	VOID	ItemRenderToTexture(INT nVScrollPos);			///< 영역에 알맞게 렌더링

	INT		GetAlignOffsetX(DWORD dwAlign, INT nClipWidth, INT nItemWidth);
	INT		GetAlignOffsetY(DWORD dwAlign, INT nClipHeight, INT nItemHeight);

	VOID	SetVScrollEnable(BOOL bEnable);
	VOID	SetVScrollRange(INT nRange);
	VOID	SetVScrollPos(INT nPos);

	VOID	IntervalTextPosCalc(VOID);						///< 글자 위치를 계산
	VOID	IntervalPagePosCalc(VOID);						///< 페이지의 크기 계산

	VOID	IntervalPageInit(INT nPage);					///< 출력을 위한 업데이트 정보 초기화
	VOID	IntervalPageDone(INT nPage);					///< IntervalText 페이지 완료

	VOID	DeleteIntervalText(VOID);						///< IntervalText의 모든 자원 삭제

	// Dynamic 관련 함수
	VOID	AdjustToWidthString(VOID);						///< Dynamic 속성 일때 컴포넌트의 Width를 조절한다.
	VOID	AdjustToHeightString(VOID);						///< Dynamic 속성 일때 컴포넌트의 Height를 조절한다.

	INT FindMaxWidthItem(VOID);

	// Link 관련 함수
	VOID	LinkRectCalc(VOID);								///< 링크되는 위치를 계산
	VOID	LinkVScrollCalc(INT nPos);						///< 링크되는 영역을 스크롤에 따라 계산해준다.
	VOID	DeleteIntervalLink(VOID);						///< Interval Text Mode일 때 링크 영역 계산 한것을 삭제한다.

public:
	// Link 관련 콜백 함수
	static HtmlLinkTagFunc m_fnHtmlLinkTag;

	static VOID LinkHtmlLinkTag(HtmlLinkTagFunc fnLinkTag);

	// signals
public:

	VOID OnSetOptions(const CComponentOptions& options);
	VOID OnPaint(VOID);
	VOID OnMove(int nX,int nY);
	VOID OnResize(int nCx, int nCy);
	VOID OnSetAlpha( BYTE ucAlpha );
	VOID OnSetColor( BYTE ucRed, BYTE ucGreen, BYTE ucBlue );
	VOID OnScrollMove( INT nPos );
	VOID OnMouseMove( INT nKey, INT nXPos, INT nYPos );
	VOID OnMouseDown(const CKey& key);
	VOID OnMouseUp(const CKey& key);

	CSlot m_SlotSetOption;
	CSlot m_SlotPaint;
	CSlot m_SlotMove;
	CSlot m_SlotResize;
	CSlot m_SlotSetAlpha;
	CSlot m_SlotSetColor;
	CSlot m_SlotScrollChange;
	CSlot m_SlotSliderMove;
	CSlot m_SlotMouseMove;
	CSlot m_SlotMouseDown;
	CSlot m_SlotMouseUp;

private:
	CHtmlBox			*m_pHtmlBox;
	std::list<CSurface> m_stlSurface;
	CSurface			m_TextSurface;

	// html document
	CHtmlDoc			*m_pHtmlDoc;

	// rendering HTML item : 속도를 위하여 따로 자료구조를 마련하였다.
	std::list<CRowHtml_Item*> m_listRowHtml;

	// component attribute
	DWORD		m_dwStyle;
	INT			m_nLineInterY;				///< line 간격.
	INT			m_nItemCx;					///< 전체 item의 width
	INT			m_nItemCy;					///< 전체 item의 height
	BOOL		m_bVScrollEnable;			///< vertical scroll enable
	INT			m_nVScrollRange;			///< vertical scroll range
	COLORREF	m_BgColor;					///< 배경 색깔
	INT			m_nVScrollPos;

	float m_fRatio;

	BOOL		m_bIntervalText;			///< 현재의 모드 default = FALSE
	BOOL		m_bDonePage;				///< 현재의 페이지를 모두 출력 완료
	BOOL		m_bPause;					///< 일시멈춤
	BOOL		m_bImmediate;				///< 페이지를 바로 보여줌

	float		m_fIntervalTime;			///< 간격의 시간
	float		m_fElapsedTime;				///< 누적된 시간
	
	INT			m_nCurrentRow;				///< 출력하는 현재 글자의 위치의 높이
	INT			m_nCurrentPage;				///< 출력하는 현재 페이지

	PlaneSnapShot	m_planeInterval;		///< 현재 진행중인 줄의 rect
	INT				m_nPageCount;			///< 페이지의 갯수

	INT				m_nPresentNum;			///< 한번에 보여줄 글자의 갯수 ( 1 ~ 10 , default 1 )
	
	typedef std::list<CRectangle> LISTRECTANGLE;
	LISTRECTANGLE *m_pListRectangle;		///< 페이지 별로 글자의 크기를 담은 리스트
	LISTRECTANGLE::iterator itListRect;		///< 글자의 반복자

	typedef std::vector<INT> VECINT;
	VECINT m_vecNewLineIdx;					///< CRowHtml_Item의 [br]태그의 index
	VECINT m_vecNewPageIdx; 				///< 페이지를 구분하는 [br]태그의 index

	typedef std::vector<CRectangle> VECRECTANGLE;
	VECRECTANGLE m_vecPageRect;				///< 각 페이지의 크기

	// Dynamic 속성
	BOOL	m_bDynamic;						///< Dynamic 속성
	INT		m_nMaxCX;						///< 최대 확장할 수 있는 X
	INT		m_nMaxCY;						///< 최대 확장할 수 있는 Y
	INT		m_nMinCX;						///< 최대 줄일 수 있는 X
	INT		m_nMinCY;						///< 최대 줄일 수 있는 Y

	CRectangle	m_rectScreen;				///< 초기 컴포넌트의 영역 ( Dynamic 속성일때 계산의 기준이 된다 )
	CRectangle	m_rectClient;				///< 초기 컴포넌트의 영역

	INT		m_nMaxWidth;					///< 현재 아이템 중에서 가장 큰 사이즈를 보관하는 변수

	// Link 속성
	typedef std::list<sLinkRect> LISTLINK;
	LISTLINK			m_listOriginLink;					///< Link 영역을 담고 원본 리스트
	LISTLINK			m_listDuplicateLink;				///< Scroll과 Page 계산까지 마친 링크 영역 리스트(실제 계산시 참조)
	LISTLINK*			m_pListLink;						///< Link 영역을 페이지 별로 담고 있는 리스트(IntervalText 사용시)
	LISTLINK::iterator	m_itCaptureLink;

};

END_GUI

#endif
#ifndef __GUI_TABBUTTON_ITEM_H__
#define __GUI_TABBUTTON_ITEM_H__

START_GUI

class CTabButton_Generic;

class CTabButton_Item
{
public:
	//! Enum
	enum SURFACEPART { LEFT, CENTER, RIGHT, NUM_PART };

	//! Constructor & Destructor
	CTabButton_Item( CTabButton_Generic* pGeneric, INT nIndex, std::string& strTab );
	CTabButton_Item( CTabButton_Generic* pGeneric, INT nIndex, std::wstring& wstrTab );
	~CTabButton_Item(VOID);

	//! Attribute
	INT		GetItemWidth(VOID);

	VOID	SetText( const CHAR* pText );
	VOID	SetText( const WCHAR* pText );
	VOID	SetTextMargin( INT nX, INT nY );
	VOID	SetTextColor( COLORREF color );

	//! Operation
	VOID	SetPosition( INT nX );
	
	VOID	Enable( BOOL bEnable );	// 클릭가능여부.
	VOID	Focus( BOOL bFocus );	// 포커스 상태.
	VOID	Select(VOID);			// 선택 상태.
	VOID	Release(VOID);			// 기본 상태.
	
	//! Implements
	VOID	SetSurface( CSurface& leftSurface, CSurface& centerSurface, CSurface& rightSurface );
	VOID	ApplyAttribute( BOOL bTextRedraw = TRUE );
		
	//! Call by TabButton Generic
	VOID	Paint(VOID);

	//! Callback
	VOID	OnMouseEnter( CComponent* pComponent );
	VOID	OnMouseLeave( CComponent* pComponent );
	VOID	OnMouseDown( const CKey& key );
	VOID	OnMouseUp( const CKey& key );
	VOID	OnMove( INT nX, INT nY );

	CSlot	m_slotMouseEnter;
	CSlot	m_slotMouseLeave;
	CSlot	m_slotMouseDown;
	CSlot	m_slotMouseUp;
	CSlot	m_slotMove;

	//! Variables
	INT				m_nIndex;
	CStaticBox*		m_pText;
	CSurface		m_Surfaces[NUM_PART];

	BOOL			m_bFocus;

	CTabButton_Generic* m_pGeneric;
};

END_GUI

#endif//__GUI_TABBUTTON_ITEM_H__
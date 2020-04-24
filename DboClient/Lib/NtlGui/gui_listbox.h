////////////////////////////////////////////////////////////////////////////////
// Name: class gui:CListBox
// Desc: 리스트 박스, 스크롤 기능을 포함.
//
// 2006.02.27 Peessi@hitel.net   
//
// To Be Desired:
//	1. Sort 기능 지원.
//	2. Scroll이 라인단위가 아닌 픽셀단위로 되도록.
//	3. 영역 초과부분을 자르고 찍도록.
////////////////////////////////////////////////////////////////////////////////
#ifndef __GUI_LISTBOX_H__
#define __GUI_LISTBOX_H__

#include "gui_define.h"
#include "gui_component.h"
#include "gui_listbox_item.h"
#include "gui_surface.h"

START_GUI
	
class CListBox_Generic;

//: Listbox component
class CListBox : public CComponent
{
public:
//! Listbox constructor
	CListBox( CComponent *pParent, CSurfaceManager* pSurfaceManager );
	
	CListBox( const CRectangle& pos, CComponent* pParent, CSurfaceManager* pSurfaceManager,
		      INT nMaxVisibleItems = 1, INT nMarginX = 0, INT nMarginY = 0, 
			  INT nScrollBarWidth = DEFAULT_SLIDER_WIDTH, BOOL bDynamicScrollBar = TRUE );

	//: Creates a listbox with max_visible_items visible. 
	//: It auto-calculates the height of the listbox.
	CListBox( const CRectangle& pos, CComponent *pParent, CSurfaceManager *pSurfaceManager, 
			  INT nMaxVisibleItems, INT nRowHeight, INT nMarginX, INT nMarginY,
			  INT nScrollBarWidth, BOOL bDynamicScrollBar = TRUE );

//! Listbox destructor
	virtual ~CListBox();

//! Attributes:
	//: Returns the number of items in the list box. 
	INT GetItemCount(VOID) const;

	//: Returns all items in the list.
	const std::vector<CListBox_Item*>& GetAllItems(VOID) const;

	//: Returns all selected items.
	std::vector<CListBox_Item*> GetSelectedItems(VOID) const;
	std::vector<std::wstring>  GetSelectedItemsText(VOID) const;
	std::vector<INT>		   GetSelectedItemsIndex(VOID) const;

	//: Returns selected item. return first selection if MultiSelection is enable;
	CListBox_Item* GetSelectedItem(VOID) const;
	std::wstring  GetSelectedItemText(VOID) const;
	INT			  GetSelectedItemIndex(VOID) const;

	//: Returns Item at position index.
	CListBox_Item* GetItem( INT nIndex ) const;
	std::wstring  GetItemText( INT nIndex ) const;
	
	//: Returns true if item index is selected. Returns false if it is not selected or if there is an error.
	bool IsSelected( INT nIndex ) const;

	//: Returns true if the listbox is in multiselection mode.
	bool IsMultiSelection(VOID) const;

	//: Returns the height of one item.
	INT GetItemHeight(VOID) const;

	//: Returns the index of the item at the top of the screen.
	INT GetTopVisibleItem(VOID) const;

	//: Returns number of items maximum visible in listbox (without scrolling)
	INT GetMaxVisibleItems(VOID) const;

	//: Returns true if a scrollbar is visible in the listbox.
	bool HasScrollBar(VOID) const;

	//: Returns surface pointer
	std::list<CSurface>* GetSurface(VOID);

	//: Returns ScrollBt
	CScrollBar* GetScrollBar(VOID) const;

//! Operations:
	
	INT AddItem( const CHAR* text, DWORD dwData = 0 );
	INT AddItem( const WCHAR* text, DWORD dwData = 0 );
	//: Inserts text into the list at index. 
	//- If index is negative, text is inserted at the end of the list.
	//- Returns the index of the text
	int InsertItem( const CHAR* text, INT nIndex , DWORD dwData = 0 );
	int InsertItem( const WCHAR* text, INT nIndex , DWORD dwData = 0 );

	//: Removes and deletes the item at position index.
	//: If index is equal to get_current_item(), a new item gets highlighted and the sig_highlighted() signal is emitted. 
	VOID RemoveItem( INT nIndex );
	
	//: Remove Seleted Item
	VOID RemoveSelectedItems(VOID);

	//: Remove All Items
	VOID ClearAllItems(VOID);

	//: 
	VOID ClearSelection(VOID);

	//: Replaces the item at position index with text. 
	VOID ChangeItem( const CHAR* text, INT nIndex, DWORD dwData = 0 );
	VOID ChangeItem( const WCHAR* text, INT nIndex, DWORD dwData = 0 );

	//: Sets the highlighted item to the item index.
	//: The highlighting is moved and the listbox scrolled as necessary. 
	VOID SelectItem( INT nIndex, bool bSelect = true );

	//: If select is true, all items get selected, else all get unselected.
	//: This works only in Multi selection mode.
	VOID SelectAll( bool bSelect = true );

	//: Inverts the selection.
	//: Works only in Multi selection mode.
	VOID InvertSelection(VOID);

	//: Sets the listbox to multiselection if enable is true,
	//: single selection otherwise.
	VOID SetMultiSelection( bool bEnable = true );

	//: Sets the height of each item.
	VOID SetItemHeight( INT nHeight );

	//: Sets the number of items maximum visible in listbox (without scrolling)
	VOID SetMaxVisibleItems( INT nCount );

	//: Scrolls the list box so the item at position index in the list is displayed in the top row of the listbox. 
	VOID SetTopItem( INT nIndex );

	//: Sorts the items in ascending order, if ascending is true, or descending otherwise. 
	VOID Sort(bool bAscending = true);

	//: Sets Text Align
	VOID SetTextStyle( DWORD dwStyle );

	//: selection backgroud mask color
	VOID SetSelectedMaskColor( BYTE uRed, BYTE uGreen, BYTE uBlue, BYTE uAlpha);

	//: selection text color
	VOID SetSelectedTextColor( COLORREF color );

	//: text color
	VOID SetTextColor( COLORREF color );

	//: BackGround Color
	VOID SetBkColor( COLORREF color );

	//: BackGround Mode
	VOID SetBkMode( INT nBkMode );

	//: Sets Effect mode
	VOID SetEffectMode( INT nMode );

	//: Sets Effect color
	VOID SetEffectColor( COLORREF color );

	//: Sets Effect value
	VOID SetEffectValue( INT nValue );

	//: listbox layout add surface
	VOID AddSurface( CSurface surface );

	VOID AddMaskSurface( CSurface& surface );

	//: Font Setting
	VOID CreateFontStd( CHAR* pFaceName, INT nHeight, INT nAttributes );

//! Signals:
public:
	//: This signal is emitted when the user highlights a new current item.
	//: The argument is the index of the new item, which is already current. 
	CSignal_v1<int>& SigHighlighted(VOID);

	//: This signal is emitted when the user clicks on an item or presses return when an item is highlighted.
	//: The argument is the index of the selected item. 
	CSignal_v1<int>& SigSelected(VOID);

	//: This signal is emitted when the selection set of a listbox changes.
	CSignal_v0& SigSelectionChanged(VOID);

private:
//! Implementation:
	VOID CalcSubRect( CRectangle* pScrollBar, INT nScrollBarWidth, INT nMarginX, INT nMarginY );

//! Callback Function
	VOID OnSetOptions( const CComponentOptions& options );
	
	CSlot m_SlotSetOptions;
	
protected:
//! Variables:
	CScrollBar*		  m_pScrollBar;
	CListBox_Generic* m_pImpl;

private:
//! Etc:
	CListBox( const CListBox& copy ) : CComponent( NULL, NULL ) { return; } // disallow copy construction.
	
	friend class CListBox_Generic;
};

END_GUI

#endif

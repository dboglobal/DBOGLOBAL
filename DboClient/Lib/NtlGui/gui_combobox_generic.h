#ifndef __GUI_COMBOBOX_GENERIC_H__
#define __GUI_COMBOBOX_GENERIC_H__

START_GUI

class CComboBox_Generic
{
public:
//! Construction & Destruction:
	CComboBox_Generic( CComboBox* pSelf );
	~CComboBox_Generic(VOID);

//! Operation:
	VOID AddSurface( CSurface surface );
	VOID ToggleList(VOID);

//! Signals:
	CSignal_v1<INT> m_SigSelected;
	CSignal_v2<BOOL,CComponent*> m_SigListToggled;

private:

//! CallBack:
	VOID OnAbsoluteClick( const CKey& key );
	VOID OnClick( const CKey& key );
	VOID OnItemSelect( INT nIndex );

	VOID OnSetOption( const CComponentOptions& options );
	VOID OnPaint(VOID);
	VOID OnMove( INT nOldX, INT nOldY );
	VOID OnResize( INT nOldCX, INT nOldCY );

	CSlot	m_SlotAbsoluteClick;
	CSlot	m_SlotStaticClick;
	CSlot	m_SlotButtonClick;
	CSlot	m_SlotItemSelect;

	CSlot	m_SlotSetOptions;
	CSlot	m_SlotPaint;
	CSlot	m_SlotMove;
	CSlot	m_SlotResize;

//! Variables:
public:
	std::list<CSurface>	m_listSurface;		
private:
	CComboBox* m_pComboBox;
};

END_GUI

#endif//__GUI_COMBOBOX_GENERIC_H__
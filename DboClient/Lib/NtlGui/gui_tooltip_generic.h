#ifndef __GUI_TOLL_TIP_GENERIC_H__
#define __GUI_TOLL_TIP_GENERIC_H__

START_GUI

class CDynamicOutline;

class CToolTip_Generic
{
public:
//! Constructors & Destructors:
	CToolTip_Generic( CToolTip* pSelf );
	~CToolTip_Generic(VOID);

//! Operations:
	VOID CalcRect(VOID);
	VOID UpdateMousePt(VOID);
	VOID SetTextPosition(VOID);

private:
//! Implementation:
	VOID Render(VOID);

//! Callbacks:
	VOID OnSetOption( const CComponentOptions &options );
	VOID OnMove( INT nOldX,INT nOldY );
	VOID OnPaint(VOID);

	CSlot m_SlotSetOption;
	CSlot m_SlotPaint;
	CSlot m_SlotMove;

public:
//! Variables
	CDynamicOutline*	m_pOutline;

	INT					m_nMarginX;		// 텍스트가 찍히는 여백.
	INT					m_nMarginY;

	INT					m_nMouseMarginX;// 마우스 포인터로부터의 여백. 
	INT					m_nMouseMarginY;// Right, Bottom방향으로. 반대는 

	CComponent*			m_pOwner;		// 소유자의 포인터. 

private:
	CToolTip*			m_pToolTip;		// Reference Pointer.
};

END_GUI

#endif//__GUI_TOLL_TIP_GENERIC_H__

/*****************************************************************************
 *
 * File			: TBGambleResultGui.h
 * Author		: Haesung, Cho
 * Copyright	: (주)NTL
 * Date			: 2008. 8. 20	
 * Abstract		: Tenkaichi budokai gamble npc result
 *****************************************************************************
 * Desc         : 
 * 겜블NPC가 뱉어내는 아이템을 띄워서 확인시켜준다.
 *****************************************************************************/

#ifndef __TBGAMBLERESULT__H__
#define __TBGAMBLERESULT__H__

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "SlotGui.h"
 

/**
* \ingroup client
* \brief 겜블 NPC에게서 겜블을 했을 경우 어떤 아이템이 나왔는지 화면으로 보여준다.
*/
class CTBGambleResultGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CTBGambleResultGui( const RwChar* pName );
	virtual ~CTBGambleResultGui();

	// Create & Destroy
	RwBool		Create();
	VOID		Destroy();

	// HandleEvents & SwitchDialog
	VOID		HandleEvents(RWS::CMsg &msg);
	RwInt32		SwitchDialog(bool bOpen);

	VOID		SetGambleItem( RwInt32 hItemHandle );

	VOID		OnMove( RwInt32 iOldX, RwInt32 iOldY );
	VOID		OnPaint();

	VOID		OnClickedBtnClose( gui::CComponent* pComponent );

protected:
	CRegularSlotGui			m_sgGambleItem;

	gui::CButton*		m_pBtnClose;
	gui::CSlot			m_slotClickedBtnClose;

	gui::CPanel*		m_pPnlSlot;
	gui::CStaticBox*	m_pStbMessage;

	gui::CSlot	m_slotMove;
	gui::CSlot	m_slotPaint;
};
 
#endif//__TBGAMBLERESULT__H__
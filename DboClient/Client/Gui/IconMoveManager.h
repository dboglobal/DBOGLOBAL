////////////////////////////////////////////////////////////////////////////////
// File: IconMoveManager.h
// Desc: IconMove
//
// 2006.05.30 Peessi@hitel.net   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __ICON_MOVE_MANAGER_H__
#define __ICON_MOVE_MANAGER_H__

#include "ceventhandler.h"
#include "SurfaceGui.h"
#include "gui_precomp.h"

#define ICONPICKUP_SIZE			36
#define ICONPUSH_SIZEDIFF		2

class CIconMoveManager : public RWS::CEventHandler
{
private:
//! Constructor & Destructor
	CIconMoveManager(VOID);
public:
	~CIconMoveManager(VOID);

//! Access
	static VOID CreateInstance( gui::CGUIManager* pGuiManager );
	static VOID DeleteInstance(VOID);

	static CIconMoveManager* GetInstance(VOID);

//! Attribute
	RwBool IsActive(VOID) { return m_bActive; }		// TRUE : PickUp, FALSE : Empty
	RwBool IsEnable(VOID);
	RwUInt32 GetSrcSerial(VOID) { return m_hSrcSerial; }
	RwUInt32 GetSrcPlace(VOID) { return m_eSrcPlace; }
	RwUInt32 GetSrcSlotIdx(VOID) { return m_uiSrcSlotIdx; }
	RwUInt32 GetStackCount(VOID) { return m_uiStackCount; }
	VOID*	 GetImage(VOID)	{ return m_surIcon.GetTexture(); }
	RwUInt32 GetEXData1(VOID) { return m_ExData1; }
	RwUInt32 GetEXData2(VOID) { return m_ExData2; }
	RwUInt32 GetEXData3(VOID) { return m_ExData3; }
	RwUInt32 GetEXData4(VOID) { return m_ExData4; }
	RwUInt32 GetEXData5(VOID) { return m_ExData5; }

	RwBool	IsActiveZenny(VOID);	
	
//! Operation
	RwBool	IconMovePickUp( RwUInt32 hSrcSerial, RwUInt32 eSrcPlace, RwUInt32 uiSrcSlotIdx, RwUInt32 uiStackCount, VOID* pTexture, 
							RwUInt32 EXData1 = 0, RwUInt32 EXData2 = 0, RwUInt32 EXData3 = 0, RwUInt32 EXData4 = 0, RwUInt32 EXData5 = 0 );
	RwBool	IconMovePutDown( RwUInt32 eDestPlace, RwUInt32 hDestParentSerial, RwUInt32 uiDestSlotIdx);
	VOID	IconMoveEnd(VOID);

	VOID	Render(VOID);

//! Event
	VOID	HandleEvents( RWS::CMsg& msg );
	
private:
//! CallBack
	VOID	OnCaptureEndMouseDown( const CKey& key );

	gui::CSlot	m_slotCaptureEndMouseDown;

//! Variable
	RwBool m_bActive;			// DragDrop상태
	
	RwUInt32 m_hSrcSerial;
	RwUInt32 m_eSrcPlace;
	RwUInt32 m_uiSrcSlotIdx;

	RwUInt32 m_uiStackCount;	// 나눈갯수 0 이면 전체.

	CSurfaceGui m_surIcon;	

	RwUInt32	m_ExData1;			// 확장데이터.
	RwUInt32	m_ExData2;
	RwUInt32	m_ExData3;
	RwUInt32	m_ExData4;
	RwUInt32	m_ExData5;

	static CIconMoveManager* m_pInstance;
};

static CIconMoveManager* GetIconMoveManager(VOID)
{
	return CIconMoveManager::GetInstance();
}

#endif//__ICON_MOVE_MANAGER_H__
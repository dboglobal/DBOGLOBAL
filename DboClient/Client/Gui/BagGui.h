////////////////////////////////////////////////////////////////////////////////
// File: BagGui.h
// Desc: Bag Slot 및 Bag 
//
// 2006.04.25 Peessi@hitel.net   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __BAG_GUI_H__
#define __BAG_GUI_H__

class CBagBaseGui : public CNtlPLGui, public RWS::CEventHandler
{
protected:
//! Enumuration
	enum	eFRITZEFFECT { FRITZ_NONE, FRITZ_HALF, FRITZ_ABSOLUTE };

//! Constructor & Destructor
	CBagBaseGui(VOID);
	CBagBaseGui( const RwChar* pName );
public:
	virtual ~CBagBaseGui(VOID);

//! Operation
	VOID	Init(VOID);
	virtual RwBool  Create(VOID);
	virtual VOID	Destroy(VOID); 
	virtual VOID	Update( RwReal fElapsed );

	VOID SetBagSerial( SERIAL_HANDLE hSerial );
	SERIAL_HANDLE GetBagSerial(VOID) { return m_hBagSerial; }

	VOID SetBagPos( RwInt8 cPos ) { m_cBagPos = cPos; }
	RwInt8 GetBagPos(VOID) { return m_cBagPos; }

	virtual VOID UpdateData(VOID);
	RwInt32	SwitchDialog( bool bOpen );

	VOID SetBagEnable( RwBool bEnable ); // 가방이 클라이언트Disable 유무. 

//! Event
	virtual VOID HandleEvents( RWS::CMsg& msg );

protected:
//! Implementation
	VOID		ClearInventory(VOID);
	VOID		ClearInventorySlot( RwInt32 nSlotIdx );

	RwInt32		GetChildSlotIdx( RwInt32 nX, RwInt32 nY );
	CRectangle  GetChildSlotRect( RwInt32 nSlotIdx );	

	RwBool		IsEnableBagChildPickUp( RwInt32 nSlotIdx );
	RwBool		IsEnableBagChildPutDown( RwInt32 nSlotIdx );
		
	RwBool		CreateStackNumber( RwInt32 nSlotIdx, RwInt32 nValue );
	VOID		DestroyStackNumber( RwInt32 nSlotIdx );

	VOID		ClickEffect( RwBool bPush, RwInt32 nSlotIdx = -1 );
	VOID		FritzEffect( eFRITZEFFECT eFritzState, RwInt32 nSlotIdx );
	VOID		ShowPickedUp( RwBool bShow, RwInt32 nSlotIdx = 0 );
	VOID		ShowDisableSlot( RwBool bShow, RwInt32 nSlotIdx );
	VOID		CapsuleLockSlot( RwBool bLock, RwInt32 nSlotIdx );
	RwBool		IsDisableSlot( RwInt32 nSlotIdx );

	VOID		CoolingEffectProc( RwUInt32 hSerial, BOOL bStart );
	VOID		RightButtonProc( RwInt32 nSlotIdx );

	VOID		SetBagName(VOID);

	/*VOID		GambleEffectProcStart( RwUInt32 hSerial );
	VOID		GambleEffectProcEnd( RwInt32 nSlotIdx );*/
	VOID		BoomEffectProc( RwUInt32 hCreateSerial, RwChar* pcEffectFile );
	VOID		OnMovieEnd(gui::CComponent* pComponent);

	VOID MouseDown( const CKey& key );
	VOID MouseUp( const CKey& key );
	VOID MouseMove( RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos );
	VOID MouseOut(VOID);
	VOID Paint(VOID);
	VOID ExitButtonClick( gui::CComponent* pComponent );
	VOID Move( RwInt32 nX, RwInt32 nY );
	
	VOID OnCaptureMouseDown( const CKey& key );

//! CallBack
	gui::CSlot m_slotMouseDown;
	gui::CSlot m_slotMouseMove;
	gui::CSlot m_slotMouseUp;
	gui::CSlot m_slotMouseOut;
	gui::CSlot m_slotPaint;
	gui::CSlot m_slotMove;
	gui::CSlot m_slotClikedExit;
	gui::CSlot m_slotCaptureMouseDown;
	
//! Variable
	SERIAL_HANDLE	  m_hBagSerial;
	RwInt8			  m_cBagPos;
		
	CRectangle*		  m_prtSlots;
	CSurfaceGui*	  m_psurIcons;
	CSurfaceGui*	  m_psurDisableSlot;
	CSurfaceGui*	  m_psurFritzSlot;
	gui::CStaticBox** m_ppstbStackNum;
	gui::CButton*	  m_pCloseButton;
	RwInt32			  m_nNumSlot;
	RwInt32			  m_nLSelectedSlotIdx;		
	RwInt32			  m_nRSelectedSlotIdx;	// 
	RwInt32			  m_nPushDownIndex;
	RwInt32			  m_nCapsuleLockSlotIdx;

	CSurfaceGui		  m_surFocusSlot; 
	CSurfaceGui		  m_surPickedUp;
	RwInt32			  m_nShowPickedUp;
	
	gui::CStaticBox*  m_pstbTitle;

	gui::CRadarEffect* m_pCoolTimeEffect;

	// Gamble Effect
	gui::CFlash**			m_ppGambleBoomEffect;		///< 플래쉬 이펙트 ( Boom! )
	gui::CSlot*				m_pSlotBoomEffectEnd;		///< 플래쉬 이펙트가 종료되었을 경우
	RwBool*					m_pbIsGambleEffect;			///< 플래쉬 이펙트가 사용되고 있는지의 여부(아이템이 처리되어선 안된다)

	// Item Identify Effect
	gui::CParticleBox*		m_pPBox;					///< 파티클 박스
	
	RwInt32			  m_nMouseOnIndex;	

	RwBool			  m_bCEnable;			// 클라이언트상황 Enable여부. 가방이 열리지 않고, 열려있던 것이 닫히게 된다. 
};

class CBasicBagGui : public CBagBaseGui
{
public:
//! Constructor & Destructor
	CBasicBagGui(VOID);
	CBasicBagGui( const RwChar* pName );
	~CBasicBagGui(VOID);

//! Operation	
	RwBool	Create(VOID);
	VOID	Destroy(VOID);

	VOID	UpdateData(VOID);

	gui::CTexture* GetMoneyIcon(VOID) { return m_pMoneyIconTexture; }

private:
//! Event
	VOID	HandleEvents( RWS::CMsg& msg );

//! Implementation
	VOID	SetSlotRectHardCode(VOID);
	VOID	UpdateZenny(VOID);

//! Callback
	VOID OnMouseDown( const CKey& key );
	VOID OnMouseUp( const CKey& key );
	VOID OnMouseMove( RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos );
	VOID OnMouseOut( gui::CComponent* pComponent );
	VOID OnPaint(VOID);
	VOID OnMove( RwInt32 nX, RwInt32 nY );
	VOID OnExitButtonClick( gui::CComponent* pComponent );
	VOID OnClickZennyBtn( gui::CComponent* pComponent );
	VOID OnClickDiscardBtn( gui::CComponent* pComponent );
	VOID OnClickPopupMenuBtn(gui::CComponent* pComponent);
	
	gui::CSlot		m_slotClickZennyBtn;
	gui::CSlot		m_slotClickDiscardBtn;
	gui::CSlot		m_slotClickPopupMenuBtn;

//! Variable
	gui::CButton*	m_pbtnPopupMenu;
	gui::CStaticBox* m_pstbMoney;		
	gui::CButton*	 m_pbtnZenny;
	gui::CButton*	 m_pbtnDiscard;
	gui::CPanel*	 m_ppnlZennyFocus;

	gui::CTexture*	  m_pMoneyIconTexture;
};

class CBag4SpaceGui : public CBagBaseGui
{
public:
//! Constructor & Destructor
	CBag4SpaceGui(VOID);
	CBag4SpaceGui( const RwChar* pName );
	~CBag4SpaceGui(VOID);

//! Operation
	RwBool	Create(VOID);
	VOID	Destroy(VOID);

private:
//! Implementation
	VOID	SetSlotRectHardCode(VOID);

//! CallBack
	VOID OnMouseDown( const CKey& key );
	VOID OnMouseUp( const CKey& key );
	VOID OnMouseMove( RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos );
	VOID OnMouseOut( gui::CComponent* pComponent );
	VOID OnPaint(VOID);
	VOID OnMove( RwInt32 nX, RwInt32 nY );
	VOID OnExitButtonClick( gui::CComponent* pComponent );

//! Variable
};

class CBag8SpaceGui : public CBagBaseGui
{
public:
//! Constructor & Destructor
	CBag8SpaceGui(VOID);
	CBag8SpaceGui( const RwChar* pName );
	~CBag8SpaceGui(VOID);

//! Operation
	RwBool	Create(VOID);
	VOID	Destroy(VOID);

private:
//! Implementation
	VOID	SetSlotRectHardCode(VOID);

//! CallBack
	VOID OnMouseDown( const CKey& key );
	VOID OnMouseUp( const CKey& key );
	VOID OnMouseMove( RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos );
	VOID OnMouseOut( gui::CComponent* pComponent );
	VOID OnPaint(VOID);
	VOID OnMove( RwInt32 nX, RwInt32 nY );
	VOID OnExitButtonClick( gui::CComponent* pComponent );

//! Variable
};

class CBag12SpaceGui : public CBagBaseGui
{
public:
//! Constructor & Destructor
	CBag12SpaceGui(VOID);
	CBag12SpaceGui( const RwChar* pName );
	~CBag12SpaceGui(VOID);

//! Operation
	RwBool	Create(VOID);
	VOID	Destroy(VOID);

private:
//! Implementation
	VOID	SetSlotRectHardCode(VOID);

//! CallBack
	VOID OnMouseDown( const CKey& key );
	VOID OnMouseUp( const CKey& key );
	VOID OnMouseMove( RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos );
	VOID OnMouseOut( gui::CComponent* pComponent );
	VOID OnPaint(VOID);
	VOID OnMove( RwInt32 nX, RwInt32 nY );
	VOID OnExitButtonClick( gui::CComponent* pComponent );

//! Variable
};

class CBag16SpaceGui : public CBagBaseGui
{
public:
	//! Constructor & Destructor
	CBag16SpaceGui(VOID);
	CBag16SpaceGui( const RwChar* pName );
	virtual ~CBag16SpaceGui(VOID);

	//! Operation
	virtual	RwBool	Create(VOID);
	virtual VOID	Destroy(VOID);

protected:
	//! Implementation
	VOID	SetSlotRectHardCode(VOID);

	//! CallBack
	VOID OnMouseDown( const CKey& key );
	VOID OnMouseUp( const CKey& key );
	VOID OnMouseMove( RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos );
	VOID OnMouseOut( gui::CComponent* pComponent );
	VOID OnPaint(VOID);
	VOID OnMove( RwInt32 nX, RwInt32 nY );
	VOID OnExitButtonClick( gui::CComponent* pComponent );

	//! Variable
};

class CBag20SpaceGui : public CBagBaseGui
{
public:
	//! Constructor & Destructor
	CBag20SpaceGui(VOID);
	CBag20SpaceGui(const RwChar* pName);
	virtual ~CBag20SpaceGui(VOID);

	//! Operation
	virtual	RwBool	Create(VOID);
	virtual VOID	Destroy(VOID);

protected:
	//! Implementation
	VOID	SetSlotRectHardCode(VOID);

	//! CallBack
	VOID OnMouseDown(const CKey& key);
	VOID OnMouseUp(const CKey& key);
	VOID OnMouseMove(RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos);
	VOID OnMouseOut(gui::CComponent* pComponent);
	VOID OnPaint(VOID);
	VOID OnMove(RwInt32 nX, RwInt32 nY);
	VOID OnExitButtonClick(gui::CComponent* pComponent);

	//! Variable
};

class CBag24SpaceGui : public CBagBaseGui
{
public:
	//! Constructor & Destructor
	CBag24SpaceGui(VOID);
	CBag24SpaceGui(const RwChar* pName);
	virtual ~CBag24SpaceGui(VOID);

	//! Operation
	virtual	RwBool	Create(VOID);
	virtual VOID	Destroy(VOID);

protected:
	//! Implementation
	VOID	SetSlotRectHardCode(VOID);

	//! CallBack
	VOID OnMouseDown(const CKey& key);
	VOID OnMouseUp(const CKey& key);
	VOID OnMouseMove(RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos);
	VOID OnMouseOut(gui::CComponent* pComponent);
	VOID OnPaint(VOID);
	VOID OnMove(RwInt32 nX, RwInt32 nY);
	VOID OnExitButtonClick(gui::CComponent* pComponent);

	//! Variable
};

class CBag28SpaceGui : public CBagBaseGui
{
public:
	//! Constructor & Destructor
	CBag28SpaceGui(VOID);
	CBag28SpaceGui(const RwChar* pName);
	virtual ~CBag28SpaceGui(VOID);

	//! Operation
	virtual	RwBool	Create(VOID);
	virtual VOID	Destroy(VOID);

protected:
	//! Implementation
	VOID	SetSlotRectHardCode(VOID);

	//! CallBack
	VOID OnMouseDown(const CKey& key);
	VOID OnMouseUp(const CKey& key);
	VOID OnMouseMove(RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos);
	VOID OnMouseOut(gui::CComponent* pComponent);
	VOID OnPaint(VOID);
	VOID OnMove(RwInt32 nX, RwInt32 nY);
	VOID OnExitButtonClick(gui::CComponent* pComponent);

	//! Variable
};

class CBag32SpaceGui : public CBagBaseGui
{
public:
	//! Constructor & Destructor
	CBag32SpaceGui(VOID);
	CBag32SpaceGui(const RwChar* pName);
	virtual ~CBag32SpaceGui(VOID);

	//! Operation
	virtual	RwBool	Create(VOID);
	virtual VOID	Destroy(VOID);

protected:
	//! Implementation
	VOID	SetSlotRectHardCode(VOID);

	//! CallBack
	VOID OnMouseDown(const CKey& key);
	VOID OnMouseUp(const CKey& key);
	VOID OnMouseMove(RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos);
	VOID OnMouseOut(gui::CComponent* pComponent);
	VOID OnPaint(VOID);
	VOID OnMove(RwInt32 nX, RwInt32 nY);
	VOID OnExitButtonClick(gui::CComponent* pComponent);

	//! Variable
};

class CGetItemEffect
{
public:
	CGetItemEffect(VOID);
	~CGetItemEffect(VOID);

	VOID	Create(VOID);
	VOID	Destroy(VOID);

	VOID	StartProc( RwInt32 nScreenXPos, RwInt32 nScreenYPos, gui::CTexture* pTexture, RwReal fDelayTime, RwUInt32 hSerial );
	VOID	EndProc(VOID);

	RwBool	Update( RwReal fElapsed );	
	VOID	Render(VOID);

	VOID	SetInitXPos( RwInt32 nScreenXPos );	// DestPos도 같이 수정됨.
	VOID	SetInitYPos( RwInt32 nScreenYPos );

	RwBool	IsEffectShow(VOID) { return m_bShowEffect; }
	RwUInt32 GetSerial(VOID) { return m_hSerial; }

private:
	RwInt32		m_nInitYPos;
	RwInt32		m_nDestYPos;
	RwInt32		m_nInitXPos;
	RwInt32		m_nDestXPos;
	
	RwBool		m_bShowEffect;
	RwReal		m_fCurrentTime;		
	RwUInt32	m_hSerial;
	
	gui::CFlickerEffect	m_FlickerEffect;
};

#define VISIBLE_BAGSLOT_COUNT ( NTL_MAX_BAGSLOT_COUNT )

class CBagSlotGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
//! Enumuration
	enum	eFOCUSEFFECT { SLOT_FOCUS_NONE = 0x00, SLOT_FOCUS_CAN_MOVE = 0x01, SLOT_FOCUS = 0x02 };
	enum	eFRITZEFFECT { FRITZ_NONE, FRITZ_HALF, FRITZ_ABSOLUTE };
		
//! Constructor & Destructor
	CBagSlotGui(VOID);
	CBagSlotGui( const RwChar* pName );
	~CBagSlotGui(VOID);

//! Operation
	VOID	Init(VOID);
	RwBool	Create(VOID);
	VOID	Destroy(VOID);

	VOID	Update( RwReal fElapsed );

	RwBool	UpdateData(VOID);

	RwInt32	SwitchDialog( bool bOpen );

private:
//! Event
	VOID HandleEvents( RWS::CMsg& msg );
	//RwInt32	ActionMapBagSlot(VOID);

//! Implementation
	VOID		ClearBag(VOID);
	VOID		ClearBagSlot( RwInt32 nSlotIdx );

	VOID		SetSlotRectHardCode(VOID);
	RwInt32		GetBagSlotIdx( RwInt32 nX, RwInt32 nY );
	CRectangle  GetBagSlotRect( RwInt32 nSlotIdx );	

	RwBool		IsEnableBagPickUp( RwInt32 nSlotIdx );
	RwBool		IsEnableBagPutDown( RwInt32 nSlotIdx );
	RwBool		IsEnableBagOpenClose( RwInt32 nSlotIdx );
	//RwBool	IsEnableBagPopup( RwInt32 nSlotIdx );
	
	VOID		ShowIconDestination(VOID);

	RwBool		CreateStackNumber( RwInt32 nSlotIdx, RwInt32 nSlotCount, RwInt32 nItemCount );
	VOID		DestroyStackNumber( RwInt32 nSlotIdx );

	VOID		ClickEffect( RwBool bPush, RwInt32 nSlotIdx = -1 );
	VOID		FritzEffect( eFRITZEFFECT eFritzState, RwInt32 nSlotIdx );
	VOID		ShowDisableSlot( RwBool bShow, RwInt32 nSlotIdx );
	RwBool		IsDisableSlot( RwInt32 nSlotIdx );

	VOID		ClearGetItemEffect(VOID);
	VOID		RemoveGetItemEffect( RwUInt32 hSerial );

//! CallBack
	VOID OnMove( RwInt32 nX, RwInt32 nY );
	VOID OnMouseDown( const CKey& key );
	VOID OnMouseUp( const CKey& key );
	VOID OnMouseOut( gui::CComponent* pComponent );
	VOID OnMouseMove( RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos );
	VOID OnPaint(VOID);
	
	gui::CSlot m_slotMouseDown;
	gui::CSlot m_slotMouseMove;
	gui::CSlot m_slotMouseUp;
	gui::CSlot m_slotMouseOut;
	gui::CSlot m_slotPaint;
	gui::CSlot m_slotMove;
		
//! Variable
	CBagBaseGui* m_pBag[NTL_MAX_BAGSLOT_COUNT];
	CSurfaceGui  m_surSlot[VISIBLE_BAGSLOT_COUNT];
	CSurfaceGui	 m_surIcon[VISIBLE_BAGSLOT_COUNT];
	CRectangle	 m_rtIcon[VISIBLE_BAGSLOT_COUNT];

	CSurfaceGui	 m_surDisableSlot[VISIBLE_BAGSLOT_COUNT];
	CSurfaceGui	 m_surFocusSlot[VISIBLE_BAGSLOT_COUNT];
	CSurfaceGui	 m_surFritzSlot[VISIBLE_BAGSLOT_COUNT];	// 내구도가 다한녀석들 표시.
	RwInt32		 m_arrFocusEffect[VISIBLE_BAGSLOT_COUNT];
	gui::CStaticBox* m_apstbStackNum[VISIBLE_BAGSLOT_COUNT];

	SERIAL_HANDLE m_hBagSerial[NTL_MAX_BAGSLOT_COUNT];
	
	RwInt32		 m_nLSelectedSlotIdx;
	RwInt32		 m_nRSelectedSlotIdx;
	RwInt32		 m_nMouseOnIndex;
	RwInt32		 m_nPushDownIndex;

	std::list<CGetItemEffect*> m_listGetItemEffect;
};

#endif//__BAG_GUI_H__

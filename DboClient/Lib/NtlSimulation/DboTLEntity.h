/*****************************************************************************
*
* File			: DboTLEntity.h
* Author		: JeongHo, Rho
* Copyright		: (林)NTL
* Date			: 2008. 4. 22	
* Abstract		: 
*****************************************************************************
* Desc         : 
*
*****************************************************************************/


#ifndef __DBO_TL_ENTITY_H__
#define __DBO_TL_ENTITY_H__


#include "ceventhandler.h"


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLEntity
//
//////////////////////////////////////////////////////////////////////////


class CDboTLEntity
{
// Member variables
protected:
	RwBool				m_bFinish;

// Constructions and Destructions
public:
	CDboTLEntity( void ) { m_bFinish = FALSE; }
	virtual ~CDboTLEntity( void ) { return; }

// Operations
public:
	virtual RwBool		IsFinish( void )				{ return m_bFinish; }

	virtual void		Enter( void )					{ return; }
	virtual void		Update( RwReal fElapsedTime )	{ return; }
	virtual void		Leave( void )					{ return; }

	virtual void		HandleEvents( RWS::CMsg &pMsg )	{ return; }
};


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLSyncGroup
//
//////////////////////////////////////////////////////////////////////////


class CDboTLSyncGroup : public CDboTLEntity
{
// Declarations
public:
	typedef std::list< CDboTLEntity* > listdef_Child_List;

// Member variables
protected:
	listdef_Child_List	m_defChildEntities;

// Operations
public:
	virtual RwBool		IsFinish( void );

	virtual void		Enter( void );
	virtual void		Update( RwReal fElapsedTime );
	virtual void		Leave( void );

	virtual void		HandleEvents( RWS::CMsg &pMsg );

public:
	void				PushChildEntity( CDboTLEntity* pEntity );
	void				ClearChildEntityList( void );
};


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLPause
//
//////////////////////////////////////////////////////////////////////////


class CDboTLPause : public CDboTLEntity
{
// Member variables
protected:
	RwReal				m_fPauseTime;

// Constructions and Destructions
public:
	CDboTLPause( void ) { m_fPauseTime = RwRealMAXVAL; }

// Operations
public:
	virtual void		Update( RwReal fElapsedTime );

public:
	void				SetPauseTime( RwReal fPauseTime ) { m_fPauseTime = fPauseTime; }
};


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLDefaultSkin
//
//////////////////////////////////////////////////////////////////////////


class CDboTLDefaultSkin : public CDboTLEntity
{
// Member variables
protected:
	// Font 包访
	std::string			m_strFontName;
	RwRGBA				m_rgbFontBackColor;
	RwRGBA				m_rgbFontColor;
	RwInt32				m_nFontSize;
	RwUInt32			m_uiFontAllign;
	RwV2d				m_vFontOffset;

	// Message layer 包访
	RwV2d				m_vMsgLayerOffset;

	// Ok 包访
	RwV2d				m_vOkOffset;

	// Image 包访
	RwV2d				m_vIMGOffset;

// Constructions and Destructions
public:
	CDboTLDefaultSkin( void );

	// Operations
public:
	virtual void		Enter( void );

public:
	// Font 包访
	void				SetFontName( const char* pFontName ) { m_strFontName = pFontName; }
	void				SetFontBackColor( const RwRGBA& rgbFontBackColor ) { m_rgbFontBackColor = rgbFontBackColor; }
	void				SetFontColor( const RwRGBA& rgbFontColor ) { m_rgbFontColor = rgbFontColor; }
	void				SetFontSize( RwInt32 nFontSize ) { m_nFontSize = nFontSize; }
	void				SetFontAllign( RwUInt32 uiFontAllign ) { m_uiFontAllign = uiFontAllign; }
	void				SetFontOffset( const RwV2d& vFontOffset ) { m_vFontOffset = vFontOffset; }

	// Msg layer 包访
	void				SetMsgLayerOffset( const RwV2d& vMsgLayerOffset ) { m_vMsgLayerOffset = vMsgLayerOffset; }

	// Ok 包访
	void				SetOkOffset( const RwV2d& vOkOffset ) { m_vOkOffset = vOkOffset; }

	// Image 包访
	void				SetIMGOffset( const RwV2d& vIMGOffset ) { m_vIMGOffset = vIMGOffset; }
};


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLGuideMsg_Show
//
//////////////////////////////////////////////////////////////////////////


class CDboTLGuideMsg_Show : public CDboTLEntity
{
// Member variables
protected:
	RwUInt32			m_uiTextTblIdx;
	RwReal				m_fFadeInTime;

// Constructions and Destructions
public:
	CDboTLGuideMsg_Show( void ) { m_uiTextTblIdx = 0xffffffff; m_fFadeInTime = 0.f; }

// Operations
public:
	virtual void		Enter( void );
	virtual void		Update( RwReal fElapsedTime );

public:
	void				SetTextTblIdx( RwUInt32 uiTextTblIdx ) { m_uiTextTblIdx = uiTextTblIdx; }
	void				SetFadeInTime( RwReal fFadeInTime );
};


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLGuideMsg_Hide
//
//////////////////////////////////////////////////////////////////////////


class CDboTLGuideMsg_Hide : public CDboTLEntity
{
// Member variables
protected:
	RwUInt32			m_uiTextTblIdx;
	RwReal				m_fFadeOutTime;

// Constructions and Destructions
public:
	CDboTLGuideMsg_Hide( void ) { m_uiTextTblIdx = 0xffffffff; m_fFadeOutTime = 0.f; }

// Operations
public:
	virtual void		Enter( void );
	virtual void		Update( RwReal fElapsedTime );

public:
	void				SetTextTblIdx( RwUInt32 uiTextTblIdx ) { m_uiTextTblIdx = uiTextTblIdx; }
	void				SetFadeOutTime( RwReal fFadeOutTime );
};


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLGuideMsg_ChangeColor
//
//////////////////////////////////////////////////////////////////////////


class CDboTLGuideMsg_ChangeColor : public CDboTLEntity
{
// Member variables
protected:
	RwUInt32			m_uiTextTblIdx;
	RwRGBA				m_rgbFontBackColor;
	RwRGBA				m_rgbFontColor;
	RwReal				m_fFadeInOutTime;

// Constructions and Destructions
public:
	CDboTLGuideMsg_ChangeColor( void );

// Operations
public:
	virtual void		Enter( void );
	virtual void		Update( RwReal fElapsedTime );

public:
	void				SetTextTblIdx( RwUInt32 uiTextTblIdx ) { m_uiTextTblIdx = uiTextTblIdx; }
	void				SetFontBackColor( RwRGBA& rgbFontBackColor ) { m_rgbFontBackColor = rgbFontBackColor; }
	void				SetFontColor( RwRGBA& rgbFontColor ) { m_rgbFontColor = rgbFontColor; }
	void				SetFadeInOutTime( RwReal fFadeInOutTime );
};


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLGuideIMG_Show
//
//////////////////////////////////////////////////////////////////////////


class CDboTLGuideIMG_Show : public CDboTLEntity
{
// Member variables
protected:
	std::string			m_strIMGName;
	RwReal				m_fFadeInTime;
	RwReal				m_fSizeRate;

// Constructions and Destructions
public:
	CDboTLGuideIMG_Show( void ) { m_fFadeInTime = 0.f; }

// Operations
public:
	virtual void		Enter( void );
	virtual void		Update( RwReal fElapsedTime );

public:
	void				SetIMGName( const char* pIMGName ) { m_strIMGName = pIMGName; }
	void				SetFadeInTime( RwReal fFadeInTime );
	void				SetSizeRate(RwReal fSizeRate) { m_fSizeRate = fSizeRate; }
};


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLGuideIMG_Hide
//
//////////////////////////////////////////////////////////////////////////


class CDboTLGuideIMG_Hide : public CDboTLEntity
{
// Member variables
protected:
	std::string			m_strIMGName;
	RwReal				m_fFadeOutTime;

// Constructions and Destructions
public:
	CDboTLGuideIMG_Hide( void ) { m_fFadeOutTime = 0.f; }

// Operations
public:
	virtual void		Enter( void );
	virtual void		Update( RwReal fElapsedTime );

public:
	void				SetIMGName( const char* pIMGName ) { m_strIMGName = pIMGName; }
	void				SetFadeOutTime( RwReal fFadeOutTime );
};


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLGuideIMGOk_Show
//
//////////////////////////////////////////////////////////////////////////


class CDboTLGuideIMGOk_Show : public CDboTLEntity
{
// Member variables
protected:
	RwReal				m_fFadeInTime;

// Constructions and Destructions
public:
	CDboTLGuideIMGOk_Show( void ) { m_fFadeInTime = 0.f; }

// Operations
public:
	virtual void		Enter( void );
	virtual void		Update( RwReal fElapsedTime );

public:
	void				SetFadeInTime( RwReal fFadeInTime );
};


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLGuideIMGOk_Hide
//
//////////////////////////////////////////////////////////////////////////


class CDboTLGuideIMGOk_Hide : public CDboTLEntity
{
// Member variables
protected:
	RwReal				m_fFadeOutTime;

// Constructions and Destructions
public:
	CDboTLGuideIMGOk_Hide( void ) { m_fFadeOutTime = 0.f; }

// Operations
public:
	virtual void		Enter( void );
	virtual void		Update( RwReal fElapsedTime );

public:
	void				SetFadeOutTime( RwReal fFadeOutTime );
};

//////////////////////////////////////////////////////////////////////////
//
//	CDboTLCheckPoint
//
//////////////////////////////////////////////////////////////////////////


class CDboTLCheckPoint : public CDboTLEntity
{
// Member variables
protected:
	RwV3d				m_vTargetPos;

// Constructions and Destructions
public:
	CDboTLCheckPoint( void ) { return; }

// Operations
public:
	virtual void		Enter( void );

public:
	void				SetTargetPos( RwV3d& vTargetPos ) { m_vTargetPos = vTargetPos; }
};


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLUIIndicator_Show
//
//////////////////////////////////////////////////////////////////////////


class CDboTLUIIndicator_Show : public CDboTLEntity
{
// Member variables
protected:
	RwUInt32			m_uiTLIndicatorType;
	std::string			m_strFlashName;
	RwV2d				m_vFocusOffPos;

// Constructions and Destructions
public:
	CDboTLUIIndicator_Show( void );

// Operations
public:
	virtual void		Enter( void );

public:
	void				SetTLWindowType( RwUInt32 uiTLIndicatorType ) { m_uiTLIndicatorType = uiTLIndicatorType; }
	void				SetFlashName(const RwChar* pcText) { m_strFlashName = pcText; }
	void				SetTLFocusOffsetPos( RwV2d& vFocusOffPos ) { m_vFocusOffPos = vFocusOffPos; }
};


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLUIIndicator_Hide
//
//////////////////////////////////////////////////////////////////////////


class CDboTLUIIndicator_Hide : public CDboTLEntity
{
// Member variables
protected:
	RwUInt32			m_uiTLIndicatorType;

// Constructions and Destructions
public:
	CDboTLUIIndicator_Hide( void ) { m_uiTLIndicatorType = 0xffffffff; }

// Operations
public:
	virtual void		Enter( void );

public:
	void				SetTLWindowType( RwUInt32 uiTLIndicatorType ) { m_uiTLIndicatorType = uiTLIndicatorType; }
};


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLMouseInputLock
//
//////////////////////////////////////////////////////////////////////////


class CDboTLMouseInputLock : public CDboTLEntity
{
// Member variables
protected:
	RwUInt32			m_uiMouseInputType;

// Constructions and Destructions
public:
	CDboTLMouseInputLock( void ) { m_uiMouseInputType = 0; }

// Operations
public:
	virtual void		Enter( void );

public:
	void				SetTLMouseInputType( RwUInt32 uiMouseInputType ) { m_uiMouseInputType = uiMouseInputType; }
};


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLMouseInputUnlock
//
//////////////////////////////////////////////////////////////////////////


class CDboTLMouseInputUnlock : public CDboTLEntity
{
// Member variables
protected:
	RwUInt32			m_uiMouseInputType;

// Constructions and Destructions
public:
	CDboTLMouseInputUnlock( void ) { m_uiMouseInputType = 0; }

// Operations
public:
	virtual void		Enter( void );

public:
	void				SetTLMouseInputType( RwUInt32 uiMouseInputType ) { m_uiMouseInputType = uiMouseInputType; }
};


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLKeyboardInputLock
//
//////////////////////////////////////////////////////////////////////////


class CDboTLKeyboardInputLock : public CDboTLEntity
{
// Member variables
protected:
	RwUInt32			m_uiKeyboardInputType;

// Constructions and Destructions
public:
	CDboTLKeyboardInputLock( void ) { m_uiKeyboardInputType = 0; }

// Operations
public:
	virtual void		Enter( void );

public:
	void				SetTLKeyboardInputType( RwUInt32 uiKeyboardInputType ) { m_uiKeyboardInputType = uiKeyboardInputType; }
};


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLKeyboardInputUnlock
//
//////////////////////////////////////////////////////////////////////////


class CDboTLKeyboardInputUnlock : public CDboTLEntity
{
// Member variables
protected:
	RwUInt32			m_uiKeyboardInputType;

// Constructions and Destructions
public:
	CDboTLKeyboardInputUnlock( void ) { m_uiKeyboardInputType = 0; }

// Operations
public:
	virtual void		Enter( void );

public:
	void				SetTLKeyboardInputType( RwUInt32 uiKeyboardInputType ) { m_uiKeyboardInputType = uiKeyboardInputType; }
};


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLActionMapBackup
//
//////////////////////////////////////////////////////////////////////////


class CDboTLActionMapBackup : public CDboTLEntity
{
// Operations
public:
	virtual void		Enter( void );
};


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLActionMapRestore
//
//////////////////////////////////////////////////////////////////////////


class CDboTLActionMapRestore : public CDboTLEntity
{
// Operations
public:
	virtual void		Enter( void );
};


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLExcuteQuest
//
//////////////////////////////////////////////////////////////////////////


class CDboTLExcuteQuest : public CDboTLEntity
{
// Member variables
protected:
	RwUInt16			m_tQuestID;

// Constructions and Destructions
public:
	CDboTLExcuteQuest( void ) { m_tQuestID = 0xffff; }

// Operations
public:
	virtual void		Enter( void );

public:
	void				SetQuestID( RwUInt16 tQuestID ) { m_tQuestID = tQuestID; }
};


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLBroadMsg
//
//////////////////////////////////////////////////////////////////////////


class CDboTLBroadMsg : public CDboTLEntity
{
// Member variables
protected:
	RwUInt32				m_uiNPCTblIdx;
	RwUInt32				m_uiSpeechTblIdx;
	RwReal					m_fRemainTime;

// Constructions and Destructions
public:
	CDboTLBroadMsg( void );

// Operations
public:
	virtual void		Enter( void );

public:
	void				SetNPCTableIndex( RwUInt32 uiNPCTblIdx ) { m_uiNPCTblIdx = uiNPCTblIdx; }
	void				SetSpeechTableIndex( RwUInt32 uiSpeechTblIdx ) { m_uiSpeechTblIdx = uiSpeechTblIdx; }
	void				SetRemainTime( RwReal fRemainTime ) { m_fRemainTime = fRemainTime; }
};


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLAttackLock
//
//////////////////////////////////////////////////////////////////////////


class CDboTLAttackLock : public CDboTLEntity
{
// Member variables
protected:
	RwUInt32			m_uiAttackType;

// Constructions and Destructions
public:
	CDboTLAttackLock( void ) { m_uiAttackType = 0; }

// Operations
public:
	virtual void		Enter( void );

public:
	void				SetAttackType( RwUInt32 uiAttackType ) { m_uiAttackType = uiAttackType; }
};


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLAttackUnlock
//
//////////////////////////////////////////////////////////////////////////


class CDboTLAttackUnlock : public CDboTLEntity
{
// Member variables
protected:
	RwUInt32			m_uiAttackType;

// Constructions and Destructions
public:
	CDboTLAttackUnlock( void ) { m_uiAttackType = 0; }

// Operations
public:
	virtual void		Enter( void );

public:
	void				SetAttackType( RwUInt32 uiAttackType ) { m_uiAttackType = uiAttackType; }
};


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLPlayCinematic
//
//////////////////////////////////////////////////////////////////////////


class CDboTLPlayCinematic : public CDboTLEntity
{
// Member variables
protected:
	std::string			m_strLuaFuncName;

// Constructions and Destructions
public:
	CDboTLPlayCinematic( void ) { return; }

// Operations
public:
	virtual void		Enter( void );

public:
	void				SetLuaFuncName( const char* pLuaFuncName ) { m_strLuaFuncName = pLuaFuncName; }
};


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLPlaySound
//
//////////////////////////////////////////////////////////////////////////


class CDboTLPlaySound : public CDboTLEntity
{
// Member variables
protected:
	std::string			m_strSoundName;

// Constructions and Destructions
public:
	CDboTLPlaySound( void ) { return; }

// Operations
public:
	virtual void		Enter( void );

public:
	void				SetSoundName( const char* pSoundName ) { m_strSoundName = pSoundName; }
};


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLPlayCamera
//
//////////////////////////////////////////////////////////////////////////


class CDboTLPlayCamera : public CDboTLEntity
{
// Member variables
protected:
	RwUInt32			m_uiCameraType;
	RwUInt32			m_uiTblIdx;
	RwV3d				m_vTargetPos;

// Constructions and Destructions
public:
	CDboTLPlayCamera( void );

// Operations
public:
	virtual void		Update( RwReal fElapsedTime );

public:
	void				SetCameraType( RwUInt32 uiCameraType ) { m_uiCameraType = uiCameraType; }
	void				SetCameraData( RwUInt32 uiTblIdx ) { m_uiTblIdx = uiTblIdx; }
	void				SetCameraData( RwV3d vTargetPos ) { m_vTargetPos = vTargetPos; }
};


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLSoundOnOff
//
//////////////////////////////////////////////////////////////////////////


class CDboTLSoundOnOff : public CDboTLEntity
{
// Member variables
protected:
	RwBool				m_bSoundOn;

// Constructions and Destructions
public:
	CDboTLSoundOnOff( void );

// Operations
public:
	virtual void		Enter( void );

public:
	void				SetSoundOnOff( RwBool bSoundOn ) { m_bSoundOn = bSoundOn; }
};


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLCondition
//
//////////////////////////////////////////////////////////////////////////


class CDboTLCondition : public CDboTLEntity
{
// Member variables
protected:
	RwUInt32			m_uiCondType;
	RwUInt32			m_uiParam1;

// Constructions and Destructions
public:
	CDboTLCondition( void ) { m_uiCondType = 0xffffffff; m_uiParam1 = 0xffffffff; }

// Operations
public:
	virtual void		HandleEvents( RWS::CMsg &pMsg );

public:
	void				SetConditionType( RwUInt32 uiCondType ) { m_uiCondType = uiCondType; }
	void				SetParam1( RwUInt32 uiParam1 ) { m_uiParam1 = uiParam1; }
};


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLCondition
//
//////////////////////////////////////////////////////////////////////////


class CDboTLDialogMove : public CDboTLEntity
{
	// Member variables
protected:
	RwBool				m_bDisableDialogMoveOption;

	// Constructions and Destructions
public:
	CDboTLDialogMove( void ) { m_bDisableDialogMoveOption = false; }

	// Operations
public:
	virtual void		Enter( void );
	void				DisableDialogMoveOption( RwBool bDisableDialogMoveOption ) { m_bDisableDialogMoveOption = bDisableDialogMoveOption; }
};


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLEnableSkip
//
//////////////////////////////////////////////////////////////////////////


class CDboTLEnableSkip : public CDboTLEntity
{
	// Member variables
protected:
	RwBool				m_bEnableSkip;

	// Constructions and Destructions
public:
	CDboTLEnableSkip( void )
	:m_bEnableSkip(FALSE)
	{}

	// Operations
public:
	virtual void		Enter( void );

public:
	void				EnableSkip( RwBool bEnableSkip ) { m_bEnableSkip = bEnableSkip; }
};


#endif
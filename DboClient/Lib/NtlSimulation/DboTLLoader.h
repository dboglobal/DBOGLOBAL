/*****************************************************************************
*
* File			: DboTLLoader.h
* Author		: JeongHo, Rho
* Copyright		: (аж)NTL
* Date			: 2008. 4. 22	
* Abstract		: 
*****************************************************************************
* Desc         : 
*
*****************************************************************************/


#ifndef __NTL_DBO_TL_LOADER_H__
#define __NTL_DBO_TL_LOADER_H__


#include "ceventhandler.h"


struct lua_State;
class CNtlLuaState;
class CDboTLEntity;
class CDboTLSyncGroup;


class CDboTLLoader
{
// Declarations
public:
	#define TL_SCRIPT_FILE_NAME		(".\\script\\tl.lua")

	typedef std::list< CDboTLEntity* > listdef_TLEntityList;
	typedef std::list< CDboTLSyncGroup* > listdef_TLSyncGroupList;

// Member variables
protected:
	static CDboTLLoader*		s_pIntance;

	CNtlLuaState*				m_pScript;

	listdef_TLEntityList		m_defTLEntityList;
	listdef_TLSyncGroupList		m_defTLSyncGroupList;

// Constructions and Destructions
public:
	CDboTLLoader( void );
	~CDboTLLoader( void );

// Operations
public:
	RwBool						Load( void );
	void						Unload( void );

	listdef_TLSyncGroupList*	GetTLSyncGroupList( void );

// Implementations
protected:
	RwBool						Load_Script( void );
	RwBool						Load_CPP( void );

	CDboTLSyncGroup*			GetCurrentSyncGroup( void );

	void						Create_TLSyncGroup( RwBool bSKipable );
	void						Create_TLPause( RwReal fPauseTime );
	void						Create_TLDefaultSkin( const char* pFontName, RwRGBA& rgbFontBackColor, RwRGBA& rgbFontColor, RwInt32 nFontSize, RwUInt32 uiFontAllign, RwV2d& vFontOffset, RwV2d& vMsgLayerOffset, RwV2d& vIMGOKset, RwV2d& vIMGOffset );
	void						Create_TLGuideMsg_Show( RwUInt32 uiTextTblIdx, RwReal fFadeInTime );
	void						Create_TLGuideMsg_Hide( RwUInt32 uiTextTblIdx, RwReal fFadeOutTime );
	void						Create_TLGuideMsg_Color( RwUInt32 uiTextTblIdx, RwRGBA& rgbFontBackColor, RwRGBA& rgbFontColor, RwReal fFadeInOutTime );
	void						Create_TLGuideIMG_Show( const char* pIMGName, RwReal fFadeInTime, RwReal fSizeRate );
	void						Create_TLGuideIMG_Hide( const char* pIMGName, RwReal fFadeOutTime );
	void						Create_TLGuideIMGOk_Show( RwReal fFadeInTime );
	void						Create_TLGuideIMGOk_Hide( RwReal fFadeOutTime );
	void						Create_TLCheckPoint( RwV3d& vPos );
	void						Create_TLUIIndicator_Show( RwUInt32 uiTLIndicatorType, const char* pcFlashName, RwV2d& vFocusOffPos );
	void						Create_TLUIIndicator_Hide( RwUInt32 uiTLIndicatorType );
	void						Create_TLMouseInputLock( RwUInt32 uiMouseInputType );
	void						Create_TLMouseInputUnlock( RwUInt32 uiMouseInputType );
	void						Create_TLKeyboardInputLock( RwUInt32 uiKeyboardInputType );
	void						Create_TLKeyboardInputUnlock( RwUInt32 uiKeyboardInputType );
	void						Create_TLActionMapBackup( void );
	void						Create_TLActionMapRestore( void );
	void						Create_TLExcuteQuest( RwUInt16 tQuestID );
	void						Create_TLBroadMsg( RwUInt32 uiNPCTblIdx, RwUInt32 uiSpeechTblIdx, RwReal fRemainTime );
	void						Create_TLAttackLock( RwUInt32 uiAttackType );
	void						Create_TLAttackUnlock( RwUInt32 uiAttackType );
	void						Create_TLPlayCinematic( const char* pLuaFuncName );
	void						Create_TLPlaySound( const char* pSoundName );
	void						Create_TLPlayCamera_TBLIDX( RwUInt32 uiCameraType, RwUInt32 uiTblIdx );
	void						Create_TLPlayCamera_POS( RwUInt32 uiCameraType, RwV3d& vTargetPos );
	void						Create_TLSoundOnOff( RwBool bSoundOn );
	void						Create_TLCondition( RwUInt32 uiCondType, RwUInt32 uiParam1 );
	void						Create_TLDisableDialogMoveOption( RwBool bDialogMove );
	void						Create_TLEnableSkip( RwBool bEnableSkip );


// Lua function
protected:
	static void					Script_Error( const char* pError );

	static int					LuaGlue_TLSyncGroup( lua_State* L );
	static int					LuaGlue_TLPause( lua_State* L );
	static int					LuaGlue_TLDefaultSkin( lua_State* L );
	static int					LuaGlue_TLGuideMsg_Show( lua_State* L );
	static int					LuaGlue_TLGuideMsg_Hide( lua_State* L );
	static int					LuaGlue_TLGuideMsg_Color( lua_State* L );
	static int					LuaGlue_TLGuideIMG_Show( lua_State* L );
	static int					LuaGlue_TLGuideIMG_Hide( lua_State* L );
	static int					LuaGlue_TLGuideIMGOk_Show( lua_State* L );
	static int					LuaGlue_TLGuideIMGOk_Hide( lua_State* L );
	static int					LuaGlue_TLCheckPoint( lua_State* L );
	static int					LuaGlue_TLUIIndicator_Show( lua_State* L );
	static int					LuaGlue_TLUIIndicator_Hide( lua_State* L );
	static int					LuaGlue_TLMouseInputLock( lua_State* L );
	static int					LuaGlue_TLMouseInputUnlock( lua_State* L );
	static int					LuaGlue_TLKeyboardInputLock( lua_State* L );
	static int					LuaGlue_TLKeyboardInputUnlock( lua_State* L );
	static int					LuaGlue_TLActionMapBackup( lua_State* L );
	static int					LuaGlue_TLActionMapRestore( lua_State* L );
	static int					LuaGlue_TLExcuteQuest( lua_State* L );
	static int					LuaGlue_TLBroadMsg( lua_State* L );
	static int					LuaGlue_TLAttackLock( lua_State* L );
	static int					LuaGlue_TLAttackUnlock( lua_State* L );
	static int					LuaGlue_TLPlayCinematic( lua_State* L );
	static int					LuaGlue_TLPlaySound( lua_State* L );
	static int					LuaGlue_TLPlayCamera_TBLIDX( lua_State* L );
	static int					LuaGlue_TLPlayCamera_POS( lua_State* L );
	static int					LuaGlue_TLSoundOnOff( lua_State* L );
	static int					LuaGlue_TLCondition( lua_State* L );
	static int					LuaGlue_TLDisableDialogMoveOption( lua_State* L );
	static int					LuaGlue_TLEnableSkip( lua_State* L );
};


#endif

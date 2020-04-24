/*****************************************************************************
* File			: NtlThemeBGM.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 
* Update		: 2009. 7. 10
* Abstract		: Simulation sound theme BGM
*****************************************************************************
* Desc         : 특정 NPC, MOB의 등장시 플레이 되는 BGM
*****************************************************************************/

#ifndef __NTL_THEME_BGM_H__
#define __NTL_THEME_BGM_H__

// shared
#include "NtlSharedType.h"

// sound
#include "NtlSoundDefines.h"

// simulation
#include "NtlSLDef.h"
#include "NtlSoundNode.h"


class CNtlThemeBGM : public CNtlSoundNode
{
protected:
	enum eWORLD_TYPE
	{
		WORLD_MAIN_WORLD,
		WORLD_PARTY_DUNGEON,

		NUM_WORLD_TYPE,

		INVALID_WORLD_TYPE
	};

	enum eTABLE_TYPE
	{
		TABLE_NPC,
		TABLE_MOB,
		TABLE_TRRIGER_OBJECT,

		NUM_TABLE_TYPE
	};

	// Theme BGM
	struct sTHEME_BGM
	{
		eWORLD_TYPE				eWorldType;
		eTABLE_TYPE				eTableType;
		RwUInt32				uiTblidx;
		std::string				strFileName;
		RwReal					fValidRange;
		bool					bLoop;

		sTHEME_BGM()
		:eWorldType(INVALID_WORLD_TYPE)
		,eTableType(NUM_TABLE_TYPE)
		,uiTblidx(INVALID_TBLIDX)
		,fValidRange(-1.f)
		,bLoop(false)
		{}
	};

	typedef std::map<RwUInt32, sTHEME_BGM*>				MAP_THEME_BGM_LIST;

	struct sTHEME_SUBJECT
	{
		sTHEME_BGM*				pTHEME_BGM;
		SERIAL_HANDLE			hHandle;
		RwBool					bPlayed;
	};

	typedef std::map<RwUInt32, sTHEME_SUBJECT*>			MAP_THEME_BGM_SUBJECT;

public:
	CNtlThemeBGM();
	virtual ~CNtlThemeBGM();

	virtual RwBool			Create();
	virtual VOID			Update(RwReal fElapsed);
	virtual VOID			Destory();
	virtual VOID			HandleEvents(RWS::CMsg &pMsg);	

protected:
	VOID					RegisterLiveSobject(SERIAL_HANDLE hHandle, RwUInt32 uiTableIndex);
	VOID					UnregiLiveSobject(SERIAL_HANDLE hHandle);
	VOID					LoadScript();

protected:
	MAP_THEME_BGM_LIST		m_mapTHEME_BGM_List[NUM_WORLD_TYPE];
	MAP_THEME_BGM_SUBJECT	m_mapSubjectBGMObejct;		///< 현재 아바타 근처에 THEME BGM을 가지고 있는 대상
	eWORLD_TYPE				m_eCurWorldType;
	SERIAL_HANDLE			m_hPlayOjbectHandle;
	SOUND_HANDLE			m_hSound;
	RwReal					m_fUpdateSkipTime;
};

#endif
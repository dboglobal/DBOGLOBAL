#pragma once


#include "SpawnMacro.h"

using std::vector;
using std::string;
#include <vector>
#include <string>


class CSpawnMacroManager
{
public:
	CSpawnMacroManager(void);
	virtual ~CSpawnMacroManager(void);

	static CSpawnMacroManager* GetInstance(VOID)
	{
		static CSpawnMacroManager g_SpawnMacroManager;
		return &g_SpawnMacroManager;
	}

	typedef std::vector<CSpawnMacroIndicator>				SPAWN_MACRO_INDICATOR_VEC;
	typedef std::vector<CSpawnMacroIndicator>::iterator		SPAWN_MACRO_INDICATOR_ITER;

protected:
	SPAWN_MACRO_INDICATOR_VEC	m_vecSpawnMacroIndicator;
	RwInt32						m_CurMacroIdx;

	// Current model info.
	BYTE	m_CurMdlLvl;
	string	m_KeyName;
	string	m_MdlName;

public:
	CSpawnMacroIndicator		SMI_ELEMENT(RwInt32 _Idx)	{ return m_vecSpawnMacroIndicator[_Idx]; }
	RwInt32						SMI_SIZE()					{ return m_vecSpawnMacroIndicator.size(); }
	SPAWN_MACRO_INDICATOR_ITER	SMI_BEGIN()					{ return m_vecSpawnMacroIndicator.begin(); }
	SPAWN_MACRO_INDICATOR_ITER	SMI_END()					{ return m_vecSpawnMacroIndicator.end(); }

public:
	VOID			Load(FILE* _pFile);
	VOID			Save(FILE* _pFile);
	RwInt32			GetCurMacroIndicatorIdx() { return m_vecSpawnMacroIndicator[m_CurMacroIdx].m_MacroIdx; }
	RwInt32&		GetCurMacroIdx() { return m_CurMacroIdx; }
	RwInt32			GetMacroIdx(RwInt32 MacroIndicatorIdx);
	CSpawnMacro		GetCurSpawnMacro();
	VOID			SetCurMdlInfo(string& KeyName, BYTE& _CurMdlLvl, string& _MdlName);
	VOID			GetCurMdlInfo(string& KeyName, BYTE& _CurMdlLvl, string& _MdlName);

	VOID			InsertMacro(CSpawnMacro& _SpawnMacro);
	VOID			DeleteMacro(RwInt32 _SpawnMacroIdx);

	void			LoadSpawnMacro();
	void			SaveSpawnMacro();
};
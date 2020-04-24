#include "precomp_ntlpresentation.h"
#include "SpawnMacroManager.h"


CSpawnMacroManager::CSpawnMacroManager(void)
{
	CSpawnMacroIndicator MacroIndicator;
	m_vecSpawnMacroIndicator.push_back(MacroIndicator);

	m_CurMacroIdx = 0;
}

CSpawnMacroManager::~CSpawnMacroManager(void)
{
	m_vecSpawnMacroIndicator.clear();
}

VOID CSpawnMacroManager::InsertMacro(CSpawnMacro& _SpawnMacro)
{
	RwInt32 LastContainerIdx	= m_vecSpawnMacroIndicator.size();
	RwInt32 NewIdxNum			= m_vecSpawnMacroIndicator[LastContainerIdx - 1].m_MacroIdx;

	CSpawnMacroIndicator SMI;
	SMI.SetProp(_SpawnMacro);
	SMI.m_MacroIdx = NewIdxNum + 1;
	m_vecSpawnMacroIndicator.push_back(SMI);	

	m_CurMacroIdx = m_vecSpawnMacroIndicator.size() - 1;
}

VOID CSpawnMacroManager::DeleteMacro(RwInt32 _SpawnMacroIdx)
{
	m_vecSpawnMacroIndicator.erase(m_vecSpawnMacroIndicator.begin() + _SpawnMacroIdx);
	if(!m_vecSpawnMacroIndicator.size())
	{
		m_vecSpawnMacroIndicator.clear();
	}

	m_CurMacroIdx = 0;
}

CSpawnMacro CSpawnMacroManager::GetCurSpawnMacro()
{
	CSpawnMacro Ret;
	RwInt32		Idx = m_CurMacroIdx;

	Ret.m_CoolTime		= m_vecSpawnMacroIndicator[Idx].m_CoolTime;
	Ret.m_MoveType		= m_vecSpawnMacroIndicator[Idx].m_MoveType;
	Ret.m_WanderRange	= m_vecSpawnMacroIndicator[Idx].m_WanderRange;
	Ret.m_MoveRange		= m_vecSpawnMacroIndicator[Idx].m_MoveRange;
	Ret.m_DelayTime		= m_vecSpawnMacroIndicator[Idx].m_DelayTime;

	return Ret;
}

RwInt32 CSpawnMacroManager::GetMacroIdx(RwInt32 MacroIndicatorIdx)
{	
	for(RwUInt32 i = 0; i < m_vecSpawnMacroIndicator.size(); ++i)
	{
		if(m_vecSpawnMacroIndicator[i].m_MacroIdx == MacroIndicatorIdx)
		{
			return i;
		}
	}

	return -999;
}

VOID CSpawnMacroManager::SetCurMdlInfo(string& _KeyName, BYTE& _CurMdlLvl, string& _MdlName)
{
	m_KeyName	= _KeyName;
	m_CurMdlLvl = _CurMdlLvl;
	m_MdlName	= _MdlName;
}

VOID CSpawnMacroManager::GetCurMdlInfo(string& _KeyName, BYTE& _CurMdlLvl, string& _MdlName)
{
	_KeyName	= m_KeyName;
	_CurMdlLvl	= m_CurMdlLvl;
	_MdlName	= m_MdlName;
}

VOID CSpawnMacroManager::Load(FILE* _pFile)
{
	m_vecSpawnMacroIndicator.clear();
	
	RwInt32 Num;
	fread(&Num, sizeof(RwInt32), 1, _pFile);

	for(RwInt32 i = 0; i < Num; ++i)
	{
		CSpawnMacroIndicator MacroIndicator;

		// Macro index.
		fread(&MacroIndicator.m_MacroIdx, sizeof(RwInt32), 1, _pFile);

		// Macro contents.
		fread(&MacroIndicator.m_CoolTime, sizeof(RwInt32), 1, _pFile);
		fread(&MacroIndicator.m_MoveType, sizeof(RwInt32), 1, _pFile);
		fread(&MacroIndicator.m_WanderRange, sizeof(RwReal), 1, _pFile);
		fread(&MacroIndicator.m_MoveRange, sizeof(RwReal), 1, _pFile);
		fread(&MacroIndicator.m_DelayTime, sizeof(RwReal), 1, _pFile);

		m_vecSpawnMacroIndicator.push_back(MacroIndicator);
	}
}

VOID CSpawnMacroManager::Save(FILE* _pFile)
{
	// Total count
	RwInt32 Num = static_cast<RwInt32>(m_vecSpawnMacroIndicator.size());
	fwrite(&Num, sizeof(RwInt32), 1, _pFile);

	for(RwUInt32 i = 0; i < m_vecSpawnMacroIndicator.size(); ++i)
	{
		// Macro index.
		fwrite(&m_vecSpawnMacroIndicator[i].m_MacroIdx,	sizeof(RwInt32), 1, _pFile);

		// Macro contents.
		fwrite(&m_vecSpawnMacroIndicator[i].m_CoolTime, sizeof(RwInt32), 1, _pFile);
		fwrite(&m_vecSpawnMacroIndicator[i].m_MoveType, sizeof(RwInt32), 1, _pFile);
		fwrite(&m_vecSpawnMacroIndicator[i].m_WanderRange, sizeof(RwReal), 1, _pFile);
		fwrite(&m_vecSpawnMacroIndicator[i].m_MoveRange, sizeof(RwReal), 1, _pFile);
		fwrite(&m_vecSpawnMacroIndicator[i].m_DelayTime, sizeof(RwReal), 1, _pFile);
	}
}

VOID CSpawnMacroManager::LoadSpawnMacro()
{
	m_vecSpawnMacroIndicator.clear();

	CSpawnMacroIndicator MacroIndicator;
	m_vecSpawnMacroIndicator.push_back(MacroIndicator);

	FILE* pFile = NULL;
	fopen_s(&pFile, ".\\tool\\saber\\script\\spawnmacro.txt", "r");
	if (pFile)
	{
		CSpawnMacroIndicator	MacroIndicator;

		char acTemp[1024];
		while(fgets(acTemp, 1024, pFile) != 0)
		{
			if (acTemp[0] == '/' && acTemp[1] == '/')
			{
				continue;
			}
			sscanf(acTemp, "%d %d %d %f %d %d", &MacroIndicator.m_MacroIdx,
												&MacroIndicator.m_CoolTime,
												&MacroIndicator.m_MoveType,
												&MacroIndicator.m_WanderRange,
												&MacroIndicator.m_MoveRange,
												&MacroIndicator.m_DelayTime);
						
			m_vecSpawnMacroIndicator.push_back(MacroIndicator);
		}			
		fclose(pFile);
	}
}

VOID CSpawnMacroManager::SaveSpawnMacro()
{
	FILE* pFile = NULL;
	fopen_s(&pFile, ".\\tool\\saber\\script\\spawnmacro.txt", "w");
	if (pFile)
	{
		fprintf_s(pFile, "//INDEX\tCOOLTIME\tMOVETYPE\tWANDERRANGE\tMOVERANGE\tDELAYTIME\n");
		for(RwUInt32 i = 1; i < m_vecSpawnMacroIndicator.size(); ++i)
		{
			// Macro index.
			fprintf_s(pFile, "%d\t", m_vecSpawnMacroIndicator[i].m_MacroIdx);

			// Macro contents.
			fprintf_s(pFile, "%d\t", m_vecSpawnMacroIndicator[i].m_CoolTime);
			fprintf_s(pFile, "%d\t", m_vecSpawnMacroIndicator[i].m_MoveType);
			fprintf_s(pFile, "%f\t", m_vecSpawnMacroIndicator[i].m_WanderRange);
			fprintf_s(pFile, "%d\t", m_vecSpawnMacroIndicator[i].m_MoveRange);
			fprintf_s(pFile, "%d\n", m_vecSpawnMacroIndicator[i].m_DelayTime);
		}
		fclose(pFile);
	}	
}
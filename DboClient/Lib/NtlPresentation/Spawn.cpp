#include "precomp_ntlpresentation.h"
#include "Spawn.h"
#include "NtlPLEntity.h"
#include "NtlDebug.h"
#include "SpawnParty.h"
#include "NtlPLSceneManager.h"
#include "NtlPLCharacter.h"
#include "NtlMath.h"
#include "NtlPLObject.h"
#include "NtlPLRandDistribution.h"
#include "ntlworldcommon.h"
#include "NtlWorldFieldManager.h"
#include "NtlPLPlayerName.h"
#include "NtlPLHelpers.h"


CSpawn::CSpawn()
{
	m_NestRange				= 1;
	m_NestSpawnCnt			= 1;
	m_NestPathIndex			= -999;
	m_pSpawnParty			= NULL;
	m_pNtlPLEntity			= NULL;
	m_pNestArrowEntity		= NULL;
	m_pSelectedArrowEntity	= NULL;
	m_pSpawnParty			= NULL;
	m_pPlayerNameEntity		= NULL;
	m_MdlGUID				= 0xffffffff;
	m_VisibleFlag			= TRUE;

	m_GroupIdx				= 0;

	m_PlayScriptIdx			= 0;
	m_PlayScriptSceneIdx	= 0;

	m_AIScriptIdx			= 0;
	m_AIScriptSceneIdx		= 0;
}

CSpawn::~CSpawn(void)
{
	if(m_pSpawnParty)
	{
		if(!m_pSpawnParty->DeleteSpawn(this))
		{
			NTL_DELETE(m_pSpawnParty);
		}
	}

	if(m_pNtlPLEntity)
	{
		GetSceneManager()->DeleteEntity(m_pNtlPLEntity);
		m_pNtlPLEntity = NULL;
	}

	for(RwUInt32 i = 0; i < m_vecNtlPLEntity.size(); ++i)
	{
		GetSceneManager()->DeleteEntity(m_vecNtlPLEntity[i]);
		m_vecNtlPLEntity[i] = NULL;
	}
	m_vecNtlPLEntity.clear();

	SetNestArrow(eSAT_NEST_DELETE);

	if(m_pPlayerNameEntity)
	{
		GetSceneManager()->DeleteEntity(m_pPlayerNameEntity);
		m_pPlayerNameEntity = NULL;
	}
}

VOID CSpawn::SetNtlPLEntity(CNtlPLEntity* _NtlPLEntity)
{
	CNtlPLCharacter* pNtlPLChr = static_cast<CNtlPLCharacter*>(_NtlPLEntity);

	m_MdlName		= pNtlPLChr->GetName();
	m_S				= pNtlPLChr->GetBaseScale();
	m_R.x			= 0.0f;
	m_R.y			= pNtlPLChr->GetAngleY();
	m_R.z			= 0.0f;
	m_T				= pNtlPLChr->GetPosition();
	m_pNtlPLEntity	= static_cast<CNtlPLEntity*>(_NtlPLEntity);
//	m_MdlGUID		= 

	SetNestArrow(eSAT_NEST_DEFAULT);
}

RwBool CSpawn::Compare(CNtlPLEntity* _pNtlPLEntity)
{
	if(m_pNtlPLEntity == _pNtlPLEntity)
	{
		return TRUE;
	}

	return FALSE;
}

VOID CSpawn::SetNestMembers(RwInt32 _Cnt, CNtlWorldFieldManager* pMgr)
{
	CLAMP(_Cnt, 1, 60);

	if(_Cnt == m_NestSpawnCnt)
	{
		return;
	}

	m_NestSpawnCnt = _Cnt;
	RefreshNestMembers(pMgr);
}

VOID CSpawn::SetNestArrow(eSPAWN_ARROW_TYPE _eSpawnArrowType)
{
	if(_eSpawnArrowType == eSAT_NEST_DEFAULT)
	{
		if(m_pSelectedArrowEntity)
		{
			GetSceneManager()->DeleteEntity(m_pSelectedArrowEntity);
			m_pSelectedArrowEntity = NULL;
		}

		if(m_pNestArrowEntity)	
		{
			GetSceneManager()->DeleteEntity(m_pNestArrowEntity);
			m_pNestArrowEntity = NULL;
		}

		if(m_pNtlPLEntity)
		{
			RwV3d					ArrowPos;
			SPLCharacterCreateParam PLEntityCreateParam;
			CNtlPLCharacter*		pNtlPLChr = static_cast<CNtlPLCharacter*>(m_pNtlPLEntity);

			CNtlMath::MathRwV3dAssign(&ArrowPos, m_T.x, m_T.y + pNtlPLChr->GetHeight() + 1.0f, m_T.z);
			PLEntityCreateParam.pPos	= &ArrowPos;
			PLEntityCreateParam.bPcFlag = FALSE;

			m_pNestArrowEntity = GetSceneManager()->CreateEntity(PLENTITY_OBJECT, "SPAWN2_DUMMY", &PLEntityCreateParam);
			DBO_ASSERT(m_pNestArrowEntity, "Entity create failed.");

			static_cast<CNtlPLObject*>(m_pNestArrowEntity)->SetFadeEnable(FALSE);
		}
	}
	else if(_eSpawnArrowType == eSAT_NEST_SELECT)
	{
		if(m_pSelectedArrowEntity)
		{
			GetSceneManager()->DeleteEntity(m_pSelectedArrowEntity);
			m_pSelectedArrowEntity = NULL;
		}

		if(m_pNestArrowEntity)	
		{
			GetSceneManager()->DeleteEntity(m_pNestArrowEntity);
			m_pNestArrowEntity = NULL;
		}

		if(m_pNtlPLEntity)
		{
			RwV3d					ArrowPos;
			SPLCharacterCreateParam PLEntityCreateParam;
			CNtlPLCharacter*		pNtlPLChr = static_cast<CNtlPLCharacter*>(m_pNtlPLEntity);

			CNtlMath::MathRwV3dAssign(&ArrowPos, m_T.x, m_T.y + pNtlPLChr->GetHeight() + 1.0f, m_T.z);
			PLEntityCreateParam.pPos	= &ArrowPos;
			PLEntityCreateParam.bPcFlag = FALSE;
			m_pSelectedArrowEntity = GetSceneManager()->CreateEntity(PLENTITY_OBJECT, "SPAWN_DUMMY", &PLEntityCreateParam);
			DBO_ASSERT(m_pSelectedArrowEntity, "Entity create failed.");

			static_cast<CNtlPLObject*>(m_pSelectedArrowEntity)->SetFadeEnable(FALSE);
		}
	}
	else if(_eSpawnArrowType == eSAT_NEST_DELETE)
	{
		if(m_pSelectedArrowEntity)
		{
			GetSceneManager()->DeleteEntity(m_pSelectedArrowEntity);
			m_pSelectedArrowEntity = NULL;
		}
	
		if(m_pNestArrowEntity)	
		{
			GetSceneManager()->DeleteEntity(m_pNestArrowEntity);
			m_pNestArrowEntity = NULL;
		}
	}
}

VOID CSpawn::Rotate(RwReal RotDelta)
{
	CNtlPLCharacter* pNtlPLChr = static_cast<CNtlPLCharacter*>(m_pNtlPLEntity);
	pNtlPLChr->SetAngleDelta(RotDelta);

	for(RwUInt32 i = 0; i < m_vecNtlPLEntity.size(); ++i)
	{
		pNtlPLChr = static_cast<CNtlPLCharacter*>(m_vecNtlPLEntity[i]);
		pNtlPLChr->SetAngleDelta(RotDelta);
	}

	m_R.y = pNtlPLChr->GetAngleY();
}

VOID CSpawn::Move(RwV3d& Pos, CNtlWorldFieldManager* pMgr)
{
	RwV3d Delta = m_T - Pos;

	m_T = Pos;

	m_pNtlPLEntity->SetPosition(&Pos);

	if(m_pNestArrowEntity)
	{
		m_pNestArrowEntity->SetPosition(&Pos);
	}	

	if(m_pSelectedArrowEntity)
	{
		m_pSelectedArrowEntity->SetPosition(&Pos);
	}

	if(m_vecNtlPLEntity.size())
	{
		for(RwUInt32 i = 0; i < m_vecNtlPLEntity.size(); ++i)
		{
			CNtlPLCharacter* pNtlPLChr = static_cast<CNtlPLCharacter*>(m_vecNtlPLEntity[i]);
			RwV3d Old = pNtlPLChr->GetPosition();
			RwV3d New = Old - Delta;

			New.y = pMgr->GetWorldSectorHeight(New);
			m_vecNtlPLEntity[i]->SetPosition(&New);
		}
	}

	if(m_pSpawnParty)
	{
		m_pSpawnParty->RefreshFollowDist4AllMembers();
	}

	if(m_pPlayerNameEntity)
	{
		m_pPlayerNameEntity->SetPosition(&Pos);
	}
}

VOID CSpawn::CreateEntity(CNtlWorldFieldManager* pMgr)
{
	CNtlPLCharacter* pChrEntity = NULL;

	SetNestArrow(eSAT_NEST_DEFAULT);

	SPLCharacterCreateParam PLEntityCreateParam;
	PLEntityCreateParam.pPos	= &m_T;
	PLEntityCreateParam.bPcFlag = FALSE;

	pChrEntity = static_cast<CNtlPLCharacter*>(GetSceneManager()->CreateEntity(PLENTITY_CHARACTER, m_MdlName.c_str(), &PLEntityCreateParam));
	DBO_ASSERT(pChrEntity, "Entity create failed.");

	pChrEntity->SetBaseScale(m_S);
	pChrEntity->SetAngleY(m_R.y);
	pChrEntity->SetPosition(&m_T);
	SetNtlPLEntity(pChrEntity);

	RefreshNestMembers(pMgr);

	RefreshNameBox();

	if(m_pSpawnParty)
	{
		SetPartyColor(m_pSpawnParty->GetColor());
	}
}

VOID CSpawn::DestroyEntity()
{
	m_VisibleFlag = TRUE;

	SetNestArrow(eSAT_NEST_DELETE);

	if(m_pNtlPLEntity)
	{
		GetSceneManager()->DeleteEntity(m_pNtlPLEntity);
		m_pNtlPLEntity = NULL;

		for(RwUInt32 i = 0; i < m_vecNtlPLEntity.size(); ++i)
		{
			GetSceneManager()->DeleteEntity(m_vecNtlPLEntity[i]);
			m_vecNtlPLEntity[i] = NULL;
		}
		m_vecNtlPLEntity.clear();
	}

	if(m_pPlayerNameEntity)
	{
		GetSceneManager()->DeleteEntity(m_pPlayerNameEntity);
		m_pPlayerNameEntity = NULL;
	}
}

VOID CSpawn::RefreshNestMembers(CNtlWorldFieldManager* pMgr)
{
	if(!m_pNtlPLEntity)
	{
		return;
	}

	// Destroy all nest members
	CNtlPLCharacter* pChrEntity = NULL;

	for(RwUInt32 i = 0; i < m_vecNtlPLEntity.size(); ++i)
	{
		GetSceneManager()->DeleteEntity(m_vecNtlPLEntity[i]);
		m_vecNtlPLEntity[i] = NULL;
	}
	m_vecNtlPLEntity.clear();

	for(RwInt32 i = 0; i < m_NestSpawnCnt - 1; ++i)
	{
		SPLCharacterCreateParam PLEntityCreateParam;
		PLEntityCreateParam.pPos	= &m_T;
		PLEntityCreateParam.bPcFlag = FALSE;

		pChrEntity = static_cast<CNtlPLCharacter*>(GetSceneManager()->CreateEntity(PLENTITY_CHARACTER, m_MdlName.c_str(), &PLEntityCreateParam));
		DBO_ASSERT(pChrEntity, "Entity create failed.");

		pChrEntity->SetBaseScale(m_S);
		pChrEntity->SetAngleY(m_R.y);

		RwReal XDelta, ZDelta;
		CNtlPLRandDistribution::GetRandInCircle(XDelta, ZDelta, m_NestRange);

		RwV3d RandT = m_T;
		RandT.x +=	XDelta;
		RandT.z +=	ZDelta;
		RandT.y	=	pMgr->GetWorldSectorHeight(RandT);
		pChrEntity->SetPosition(&RandT);
		pChrEntity->SetFlags(pChrEntity->GetFlags() | NTL_PLEFLAG_NEST_MEMBER);

		m_vecNtlPLEntity.push_back(static_cast<CNtlPLEntity*>(pChrEntity));
	}

	RefreshNameBox();
}

VOID CSpawn::CreateParty(CSpawnParty* pSpawnParty)
{
	if(m_pSpawnParty)
	{
		if(!m_pSpawnParty->DeleteSpawn(this))
		{
			NTL_DELETE(m_pSpawnParty);
		}
	}

	m_pSpawnParty = pSpawnParty;
	m_pSpawnParty->InsertSpawn(this);
}

VOID CSpawn::DestroyParty()
{
	if(m_pSpawnParty)
	{
		if(!m_pSpawnParty->DeleteSpawn(this))
		{
			NTL_DELETE(m_pSpawnParty);
			m_pSpawnParty = NULL;
		}
	}	

	m_pSpawnParty = NULL;

	RefreshNameBox();
}

VOID CSpawn::SetPartyColor(RwRGBA& RGBA)
{
	static_cast<CNtlPLCharacter*>(m_pNtlPLEntity)->SetColor(RGBA.red, RGBA.green, RGBA.blue);

	for(RwUInt32 i = 0; i < m_vecNtlPLEntity.size(); ++i)
	{
		static_cast<CNtlPLCharacter*>(m_vecNtlPLEntity[i])->SetColor(RGBA.red, RGBA.green, RGBA.blue);
	}
}

VOID CSpawn::SetPartyLeader(RwBool IsLeader)
{
	m_pSpawnParty->SetLeader(this, IsLeader);
}

VOID CSpawn::SetPartyFollow(RwBool IsFollow)
{
	m_pSpawnParty->SetFollow(IsFollow);
}

VOID CSpawn::SetSpawnName(string& _Name)
{
	m_Name = _Name;

	RefreshNameBox();
}

VOID CSpawn::RefreshNameBox()
{
	if(!m_pNtlPLEntity)
	{
		return;
	}

	if(m_pPlayerNameEntity)
	{
		GetSceneManager()->DeleteEntity(m_pPlayerNameEntity);
		m_pPlayerNameEntity = NULL;
	}

	RwChar	Text[NTL_MAX_DIR_PATH];
	RwRGBA	RGBA = {255, 255, 255, 255};

	if(m_pSpawnParty)
	{
		if(m_pSpawnParty->GetLeader())
		{
			if(m_pSpawnParty->GetLeader() == this && m_pSpawnParty->GetFollow())
			{
				RGBA.red	= 255;
				RGBA.green	= 0;
				RGBA.blue	= 0;

				sprintf_s(Text, NTL_MAX_DIR_PATH, "<L>%s<F> - %d", m_Name.c_str(), m_NestRange);
			}
			else if(m_pSpawnParty->GetLeader() == this && !m_pSpawnParty->GetFollow())
			{
				RGBA.red	= 255;
				RGBA.green	= 0;
				RGBA.blue	= 0;

				sprintf_s(Text, NTL_MAX_DIR_PATH, "<L>%s - %d", m_Name.c_str(), m_NestRange);
			}
			else if(m_pSpawnParty->GetLeader() != this && m_pSpawnParty->GetFollow())
			{
				sprintf_s(Text, NTL_MAX_DIR_PATH, "<P>%s<F> - %d", m_Name.c_str(), m_NestRange);
			}
			else if(m_pSpawnParty->GetLeader() != this && !m_pSpawnParty->GetFollow())
			{
				sprintf_s(Text, NTL_MAX_DIR_PATH, "<P>%s - %d", m_Name.c_str(), m_NestRange);
			}
		}
		else
		{
			sprintf_s(Text, NTL_MAX_DIR_PATH, "<P>%s - %d", m_Name.c_str(), m_NestRange);
		}
	}
	else
	{
		sprintf_s(Text, NTL_MAX_DIR_PATH, "%s - %d", m_Name.c_str(), m_NestRange);
	}


	WCHAR wszUnicode[NTL_MAX_DIR_PATH];
	memset((char*)wszUnicode, 0, sizeof(WCHAR) * (NTL_MAX_DIR_PATH));
	::MultiByteToWideChar(GetACP(), 0, Text, -1, wszUnicode, NTL_MAX_DIR_PATH);

	SPLPlayerNameCreateParam PLEntityCreateParam;
	PLEntityCreateParam.pName			= wszUnicode;
	PLEntityCreateParam.Color			= RGB(RGBA.red, RGBA.green, RGBA.blue);
	PLEntityCreateParam.fCullDist		= 300.0f;
	PLEntityCreateParam.pGuildName		= NULL;
	PLEntityCreateParam.pTitleName = NULL;
	PLEntityCreateParam.ColorTitle = RGB(RGBA.red, RGBA.green, RGBA.blue);
	PLEntityCreateParam.pEmblemFactor	= NULL;
	PLEntityCreateParam.ColorGuild		= RGB(RGBA.red, RGBA.green, RGBA.blue);

	m_pPlayerNameEntity = static_cast<CNtlPLPlayerName*>(GetSceneManager()->CreateEntity(PLENTITY_PLAYER_NAME, NULL, &PLEntityCreateParam));
	DBO_ASSERT(m_pPlayerNameEntity, "Entity create failed.");

	RwV3d vOffset;
	CNtlMath::MathRwV3dAssign(&vOffset, 0.0f, static_cast<CNtlPLCharacter*>(m_pNtlPLEntity)->GetHeight() + 1.5f, 0.0f); 
	Helper_AttachWorldPos(m_pNtlPLEntity, m_pPlayerNameEntity, vOffset);
}

VOID CSpawn::Save(FILE* _pFile)
{
	// GUID
	fwrite(&m_MdlGUID, sizeof(DWORD), 1, _pFile);

	// Nest prop.
	fwrite(&m_NestRange, sizeof(RwInt32), 1, _pFile);
	fwrite(&m_NestSpawnCnt, sizeof(RwInt32), 1, _pFile);
	fwrite(&m_NestPathIndex, sizeof(RwInt32), 1, _pFile);

	// Party index.
	RwInt32 PartyIdx = -1;
	if(m_pSpawnParty)
	{
		PartyIdx = m_pSpawnParty->GetPartyIdx();
	}
	fwrite(&PartyIdx, sizeof(RwInt32), 1, _pFile);

	// Party leader
	RwBool IsLeader = FALSE;
	if(m_pSpawnParty)
	{
		if(m_pSpawnParty->GetLeader() == this)
		{
			IsLeader = TRUE;
		}
	}
	fwrite(&IsLeader, sizeof(RwBool), 1, _pFile);

	// Follow distance.
	fwrite(&m_FollowDist, sizeof(RwV3d), 1, _pFile);

	// Macro var.
	fwrite(&m_SpawnMacro.m_CoolTime, sizeof(RwInt32), 1, _pFile);
	fwrite(&m_SpawnMacro.m_MoveType, sizeof(RwInt32), 1, _pFile);
	fwrite(&m_SpawnMacro.m_WanderRange, sizeof(RwReal), 1, _pFile);
	fwrite(&m_SpawnMacro.m_MoveRange, sizeof(RwReal), 1, _pFile);
	fwrite(&m_SpawnMacro.m_DelayTime, sizeof(RwReal), 1, _pFile);

	// model name
	RwInt32 Len = m_MdlName.size();
	fwrite(&Len, sizeof(RwInt32), 1, _pFile);
	fwrite(m_MdlName.c_str(), sizeof(RwChar) * Len, 1, _pFile);

	// display name
	Len = m_Name.size();
	fwrite(&Len, sizeof(RwInt32), 1, _pFile);
	fwrite(m_Name.c_str(), sizeof(RwChar) * Len, 1, _pFile);

	// S, R, T
	fwrite(&m_S, sizeof(RwReal), 1, _pFile);
	fwrite(&m_R, sizeof(RwV3d), 1, _pFile);
	fwrite(&m_T, sizeof(RwV3d), 1, _pFile);

	// group indices
	fwrite(&m_GroupIdx, sizeof(DWORD), 1, _pFile);
	fwrite(&m_PlayScriptIdx, sizeof(DWORD), 1, _pFile);
	fwrite(&m_PlayScriptSceneIdx, sizeof(DWORD), 1, _pFile);

	// SPAWN FORMAT
	fwrite(&m_AIScriptIdx, sizeof(DWORD), 1, _pFile);
	fwrite(&m_AIScriptSceneIdx, sizeof(DWORD), 1, _pFile);
}

VOID CSpawn::Load(FILE* _pFile)
{
	// GUID
	fread(&m_MdlGUID, sizeof(DWORD), 1, _pFile);

	// Nest prop.
	fread(&m_NestRange, sizeof(RwInt32), 1, _pFile);
	fread(&m_NestSpawnCnt, sizeof(RwInt32), 1, _pFile);
	fread(&m_NestPathIndex, sizeof(RwInt32), 1, _pFile);

	// Party index.
	RwInt32 PartyIdx = -1;
	fread(&PartyIdx, sizeof(RwInt32), 1, _pFile);
	if(PartyIdx != -1)
	{
		m_pSpawnParty = CSpawnPartyContainer::GetSpawnParty(PartyIdx);
		m_pSpawnParty->InsertSpawn(this);
	}
	else
	{
		m_pSpawnParty = NULL;
	}

	// Party leader; watch out for party follow
	RwBool IsLeader = FALSE;
	RwBool IsFollow = FALSE;
	fread(&IsLeader, sizeof(RwBool), 1, _pFile);
	if(m_pSpawnParty)
	{
		IsFollow = m_pSpawnParty->GetFollow();

		if(IsLeader)
		{
			m_pSpawnParty->SetLeader(this, IsLeader);
		}

		m_pSpawnParty->SetFollow(IsFollow);
	}

	// Follow distance.
	fread(&m_FollowDist, sizeof(RwV3d), 1, _pFile);

	// Macro var.
	fread(&m_SpawnMacro.m_CoolTime, sizeof(RwInt32), 1, _pFile);
	fread(&m_SpawnMacro.m_MoveType, sizeof(RwInt32), 1, _pFile);
	fread(&m_SpawnMacro.m_WanderRange, sizeof(RwReal), 1, _pFile);
	fread(&m_SpawnMacro.m_MoveRange, sizeof(RwReal), 1, _pFile);
	fread(&m_SpawnMacro.m_DelayTime, sizeof(RwReal), 1, _pFile);

	// temp max string
	RwChar TmpMaxString[NTL_MAX_DIR_PATH] = {0,};

	// model name
	RwInt32 Len = 0;
	fread(&Len, sizeof(RwInt32), 1, _pFile);
	fread(TmpMaxString, sizeof(RwChar) * Len, 1, _pFile);
	m_MdlName = TmpMaxString;

	// display name
	fread(&Len, sizeof(RwInt32), 1, _pFile);
	fread(TmpMaxString, sizeof(RwChar) * Len, 1, _pFile);
	m_Name = TmpMaxString;

	// S, R, T
	fread(&m_S, sizeof(RwReal), 1, _pFile);
	fread(&m_R, sizeof(RwV3d), 1, _pFile);
	fread(&m_T, sizeof(RwV3d), 1, _pFile);

	// group indices
	fread(&m_GroupIdx, sizeof(DWORD), 1, _pFile);
	fread(&m_PlayScriptIdx, sizeof(DWORD), 1, _pFile);
	fread(&m_PlayScriptSceneIdx, sizeof(DWORD), 1, _pFile);

	// SPAWN FORMAT
	fread(&m_AIScriptIdx, sizeof(DWORD), 1, _pFile);
	fread(&m_AIScriptSceneIdx, sizeof(DWORD), 1, _pFile);
}

VOID CSpawn::Update()
{
	if(!m_pNtlPLEntity || !CNtlPLGlobal::m_RwCamera)
	{
		return;
	}

	RwV3d	EPos = m_pNtlPLEntity->GetPosition();
	RwV3d	CPos = *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
	RwReal	Dist =  RwV3dLength(&(EPos - CPos));

	if(Dist > 300.0f)
	{
		SetVisible(FALSE);
	}
	else
	{
		SetVisible(TRUE);
	}
}

VOID CSpawn::SetVisible(RwBool IsVisble)
{
	if(m_VisibleFlag == IsVisble)
	{
		return;
	}

	m_VisibleFlag = IsVisble;

	if(m_pNtlPLEntity)
	{
		m_pNtlPLEntity->SetVisible(m_VisibleFlag);
	}

	for(RwUInt32 i = 0; i < m_vecNtlPLEntity.size(); ++i)
	{
		m_vecNtlPLEntity[i]->SetVisible(m_VisibleFlag);
	}

	if(m_pNestArrowEntity)
	{
		m_pNestArrowEntity->SetVisible(m_VisibleFlag);
	}

	if(m_pSelectedArrowEntity)
	{
		m_pSelectedArrowEntity->SetVisible(m_VisibleFlag);
	}

	if(m_pPlayerNameEntity)
	{
		m_pPlayerNameEntity->SetVisible(m_VisibleFlag);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CMergeSpawn::CMergeSpawn()
{
	iNestRange				= 1;
	iNestSpawnCnt			= 1;
	iNestPathIndex			= -999;

	dwMdlGUID				= 0xffffffff;

	iPartyIndex				= -1;
	bPartyLeader			= FALSE;

	dwGroupIdx				= 0;
	dwPlayScriptIdx			= 0;
	dwPlayScriptSceneIdx	= 0;
	dwAIScriptIdx			= 0;
	dwAIScriptSceneIdx		= 0;

	uiMergeTemp				= 0;
}

CMergeSpawn::~CMergeSpawn()
{
}

RwBool CMergeSpawn::Load(FILE* pFile)
{
	// GUID
	fread(&dwMdlGUID, sizeof(DWORD), 1, pFile);

	// Nest prop.
	fread(&iNestRange, sizeof(RwInt32), 1, pFile);
	fread(&iNestSpawnCnt, sizeof(RwInt32), 1, pFile);
	fread(&iNestPathIndex, sizeof(RwInt32), 1, pFile);

	// Party index.
	fread(&iPartyIndex, sizeof(RwInt32), 1, pFile);

	fread(&bPartyLeader, sizeof(RwBool), 1, pFile);

	// Follow distance.
	fread(&vFollowDist, sizeof(RwV3d), 1, pFile);

	// Macro var.
	fread(&SpawnMacro.m_CoolTime, sizeof(RwInt32), 1, pFile);
	fread(&SpawnMacro.m_MoveType, sizeof(RwInt32), 1, pFile);
	fread(&SpawnMacro.m_WanderRange, sizeof(RwReal), 1, pFile);
	fread(&SpawnMacro.m_MoveRange, sizeof(RwReal), 1, pFile);
	fread(&SpawnMacro.m_DelayTime, sizeof(RwReal), 1, pFile);

	RwChar acTemp[NTL_MAX_DIR_PATH];
	// model name
	fread(&iLenModelName, sizeof(RwInt32), 1, pFile);
	fread(acTemp, sizeof(RwChar) * iLenModelName, 1, pFile);
	acTemp[iLenModelName] = '\0';
	strModelName = acTemp;

	// display name
	fread(&iLenDisplayName, sizeof(RwInt32), 1, pFile);
	fread(acTemp, sizeof(RwChar) * iLenDisplayName, 1, pFile);
	acTemp[iLenModelName] = '\0';
	strDisplayName = acTemp;

	// S, R, T
	fread(&fS, sizeof(RwReal), 1, pFile);
	fread(&vR, sizeof(RwV3d), 1, pFile);
	fread(&vT, sizeof(RwV3d), 1, pFile);

	// group indices
	fread(&dwGroupIdx, sizeof(DWORD), 1, pFile);
	fread(&dwPlayScriptIdx, sizeof(DWORD), 1, pFile);
	fread(&dwPlayScriptSceneIdx, sizeof(DWORD), 1, pFile);

	// SPAWN FORMAT
	fread(&dwAIScriptIdx, sizeof(DWORD), 1, pFile);
	fread(&dwAIScriptSceneIdx, sizeof(DWORD), 1, pFile);

	return TRUE;
}

RwBool CMergeSpawn::Save(FILE* pFile)
{
	// GUID
	fwrite(&dwMdlGUID, sizeof(DWORD), 1, pFile);

	// Nest prop.
	fwrite(&iNestRange, sizeof(RwInt32), 1, pFile);
	fwrite(&iNestSpawnCnt, sizeof(RwInt32), 1, pFile);
	fwrite(&iNestPathIndex, sizeof(RwInt32), 1, pFile);

	// Party index.
	fwrite(&iPartyIndex, sizeof(RwInt32), 1, pFile);

	fwrite(&bPartyLeader, sizeof(RwBool), 1, pFile);

	// Follow distance.
	fwrite(&vFollowDist, sizeof(RwV3d), 1, pFile);

	// Macro var.
	fwrite(&SpawnMacro.m_CoolTime, sizeof(RwInt32), 1, pFile);
	fwrite(&SpawnMacro.m_MoveType, sizeof(RwInt32), 1, pFile);
	fwrite(&SpawnMacro.m_WanderRange, sizeof(RwReal), 1, pFile);
	fwrite(&SpawnMacro.m_MoveRange, sizeof(RwReal), 1, pFile);
	fwrite(&SpawnMacro.m_DelayTime, sizeof(RwReal), 1, pFile);

	// model name
	iLenModelName = strModelName.size();
	fwrite(&iLenModelName, sizeof(RwInt32), 1, pFile);
	fwrite(strModelName.c_str(), sizeof(RwChar) * iLenModelName, 1, pFile);

	// display name
	iLenDisplayName = strModelName.size();
	fwrite(&iLenDisplayName, sizeof(RwInt32), 1, pFile);
	fwrite(strModelName.c_str(), sizeof(RwChar) * iLenDisplayName, 1, pFile);

	// S, R, T
	fwrite(&fS, sizeof(RwReal), 1, pFile);
	fwrite(&vR, sizeof(RwV3d), 1, pFile);
	fwrite(&vT, sizeof(RwV3d), 1, pFile);

	// group indices
	fwrite(&dwGroupIdx, sizeof(DWORD), 1, pFile);
	fwrite(&dwPlayScriptIdx, sizeof(DWORD), 1, pFile);
	fwrite(&dwPlayScriptSceneIdx, sizeof(DWORD), 1, pFile);

	// SPAWN FORMAT
	fwrite(&dwAIScriptIdx, sizeof(DWORD), 1, pFile);
	fwrite(&dwAIScriptSceneIdx, sizeof(DWORD), 1, pFile);

	return TRUE;
}
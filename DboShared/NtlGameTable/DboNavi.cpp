#include "StdAfx.h"
#include "DboNavi.h"

DWORD CDboNavi::CREATE_INVALID = 0x00000000;
DWORD CDboNavi::CREATE_TILE_INFO = 0x00000001;
DWORD CDboNavi::CREATE_FIELD_INFO = 0x00000002;
DWORD CDboNavi::CREATE_PATH_ENGINE_INFO = 0x00000004;

CDboNavi::CDboNavi()
{
	Init();
}

CDboNavi::~CDboNavi()
{
	Destroy();
}

bool CDboNavi::Create(SCreateParam *pCreateParam)
{
	RegisterTerrainAttributeGroup(DBO_WORLD_ATTR_GROUP_TOWN, DBO_WORLD_ATTR_BASIC_FORBID_PC_BATTLE);
	RegisterTerrainAttributeGroup(DBO_WORLD_ATTR_GROUP_TOWN, DBO_WORLD_ATTR_BASIC_FORBID_VEHICLE);
	RegisterTerrainAttributeGroup(DBO_WORLD_ATTR_GROUP_TOWN, DBO_WORLD_ATTR_BASIC_FORBID_MOB_MOVE);
	RegisterTerrainAttributeGroup(DBO_WORLD_ATTR_GROUP_TOWN, DBO_WORLD_ATTR_BASIC_DRAGONBALL_SCRAMBLE_SAFE_ZONE);

	RegisterTerrainAttributeGroup(DBO_WORLD_ATTR_GROUP_SWIM, DBO_WORLD_ATTR_BASIC_FORBID_PC_BATTLE);
	RegisterTerrainAttributeGroup(DBO_WORLD_ATTR_GROUP_SWIM, DBO_WORLD_ATTR_BASIC_FORBID_PC_SHOP);
	RegisterTerrainAttributeGroup(DBO_WORLD_ATTR_GROUP_SWIM, DBO_WORLD_ATTR_BASIC_FORBID_MOB_MOVE);
	RegisterTerrainAttributeGroup(DBO_WORLD_ATTR_GROUP_SWIM, DBO_WORLD_ATTR_BASIC_ENABLE_SWIM);
	RegisterTerrainAttributeGroup(DBO_WORLD_ATTR_GROUP_SWIM, DBO_WORLD_ATTR_BASIC_DRAGONBALL_SCRAMBLE_SAFE_ZONE);

	RegisterTerrainAttributeGroup(DBO_WORLD_ATTR_GROUP_FORBID_PC_BATTLE, DBO_WORLD_ATTR_BASIC_FORBID_PC_BATTLE);
	RegisterTerrainAttributeGroup(DBO_WORLD_ATTR_GROUP_FORBID_PC_BATTLE, DBO_WORLD_ATTR_BASIC_FORBID_MOB_MOVE);
	RegisterTerrainAttributeGroup(DBO_WORLD_ATTR_GROUP_FORBID_PC_BATTLE, DBO_WORLD_ATTR_BASIC_DRAGONBALL_SCRAMBLE_SAFE_ZONE);

	RegisterTerrainAttributeGroup(DBO_WORLD_ATTR_GROUP_FORBID_PC_SHOP, DBO_WORLD_ATTR_BASIC_FORBID_PC_SHOP);

	RegisterTerrainAttributeGroup(DBO_WORLD_ATTR_GROUP_FORBID_ALL_MOVE, DBO_WORLD_ATTR_BASIC_FORBID_PC_MOVE);
	RegisterTerrainAttributeGroup(DBO_WORLD_ATTR_GROUP_FORBID_ALL_MOVE, DBO_WORLD_ATTR_BASIC_FORBID_NPC_MOVE);
	RegisterTerrainAttributeGroup(DBO_WORLD_ATTR_GROUP_FORBID_ALL_MOVE, DBO_WORLD_ATTR_BASIC_FORBID_MOB_MOVE);

	RegisterTerrainAttributeGroup(DBO_WORLD_ATTR_GROUP_FORBID_MOB_MOVE, DBO_WORLD_ATTR_BASIC_FORBID_MOB_MOVE);

	RegisterTerrainAttributeGroup(DBO_WORLD_ATTR_GROUP_LAVA, DBO_WORLD_ATTR_BASIC_FORBID_PC_SHOP);
	RegisterTerrainAttributeGroup(DBO_WORLD_ATTR_GROUP_LAVA, DBO_WORLD_ATTR_BASIC_DAMAGE_SMALL);

	RegisterTerrainAttributeGroup(DBO_WORLD_ATTR_GROUP_LAVA_SWIM, DBO_WORLD_ATTR_BASIC_FORBID_MOB_MOVE);
	RegisterTerrainAttributeGroup(DBO_WORLD_ATTR_GROUP_LAVA_SWIM, DBO_WORLD_ATTR_BASIC_FORBID_PC_BATTLE);
	RegisterTerrainAttributeGroup(DBO_WORLD_ATTR_GROUP_LAVA_SWIM, DBO_WORLD_ATTR_BASIC_FORBID_PC_SHOP);
	RegisterTerrainAttributeGroup(DBO_WORLD_ATTR_GROUP_LAVA_SWIM, DBO_WORLD_ATTR_BASIC_ENABLE_SWIM);
	RegisterTerrainAttributeGroup(DBO_WORLD_ATTR_GROUP_LAVA_SWIM, DBO_WORLD_ATTR_BASIC_DAMAGE_LARGE);
	RegisterTerrainAttributeGroup(DBO_WORLD_ATTR_GROUP_LAVA_SWIM, DBO_WORLD_ATTR_BASIC_DRAGONBALL_SCRAMBLE_SAFE_ZONE);

	RegisterTerrainAttributeGroup(DBO_WORLD_ATTR_GROUP_DRAGONBALL_SCRAMBLE_SAFE_ZONE, DBO_WORLD_ATTR_BASIC_DRAGONBALL_SCRAMBLE_SAFE_ZONE);

	RegisterTerrainAttributeGroup(DBO_WORLD_ATTR_GROUP_FREE_PVP_ZONE, DBO_WORLD_ATTR_BASIC_FREE_PVP_ZONE);
	RegisterTerrainAttributeGroup(DBO_WORLD_ATTR_GROUP_FREE_PVP_ZONE, DBO_WORLD_ATTR_BASIC_FORBID_VEHICLE);
	RegisterTerrainAttributeGroup(DBO_WORLD_ATTR_GROUP_FREE_PVP_ZONE, DBO_WORLD_ATTR_BASIC_FORBID_PC_SHOP);
	RegisterTerrainAttributeGroup(DBO_WORLD_ATTR_GROUP_FREE_PVP_ZONE, DBO_WORLD_ATTR_BASIC_FORBID_PC_BATTLE);
	RegisterTerrainAttributeGroup(DBO_WORLD_ATTR_GROUP_FREE_PVP_ZONE, DBO_WORLD_ATTR_BASIC_FORBID_MOB_MOVE);
	RegisterTerrainAttributeGroup(DBO_WORLD_ATTR_GROUP_FREE_PVP_ZONE, DBO_WORLD_ATTR_BASIC_DRAGONBALL_SCRAMBLE_SAFE_ZONE);

	RegisterTerrainAttributeGroup(DBO_WORLD_ATTR_GROUP_MINI_INDOOR_TILE, DBO_WORLD_ATTR_BASIC_MINI_INDOOR);

	RegisterTerrainAttributeGroup(DBO_WORLD_ATTR_GROUP_MINI_INDOOR_GATE, DBO_WORLD_ATTR_BASIC_MINI_INDOOR);


	if( (pCreateParam->dwCreateInfoFlag & CREATE_TILE_INFO) &&
		(pCreateParam->dwCreateInfoFlag & CREATE_FIELD_INFO) )
	{
		_ASSERT(pCreateParam->pszWeTileInfoPath);
		_ASSERT(pCreateParam->pszWeFieldInfoPath);
		_ASSERT(pCreateParam->pWorldTable);

		m_DboWeInfoGroup.Create(pCreateParam->pszWeTileInfoPath, pCreateParam->pszWeFieldInfoPath, pCreateParam->pWorldTable);
	}
	else if(pCreateParam->dwCreateInfoFlag & CREATE_TILE_INFO)
	{
		_ASSERT(pCreateParam->pszWeTileInfoPath);
		_ASSERT(pCreateParam->pWorldTable);

		m_DboWeInfoGroup.Create(pCreateParam->pszWeTileInfoPath, NULL, pCreateParam->pWorldTable);
	}
	else if(pCreateParam->dwCreateInfoFlag & CREATE_FIELD_INFO)
	{
		_ASSERT(pCreateParam->pszWeFieldInfoPath);
		_ASSERT(pCreateParam->pWorldTable);

		m_DboWeInfoGroup.Create(NULL, pCreateParam->pszWeFieldInfoPath, pCreateParam->pWorldTable);
	}

	return true;
}

void CDboNavi::Destroy()
{
	::ZeroMemory(m_adwBasicAttributeCheckFlag, sizeof(m_adwBasicAttributeCheckFlag));

	m_DboWeInfoGroup.Destroy();
}

void CDboNavi::Init()
{
	::ZeroMemory(m_adwBasicAttributeCheckFlag, sizeof(m_adwBasicAttributeCheckFlag));
}

DWORD CDboNavi::GetAttribute(unsigned int uiWorldIndex, float x, float z)
{
	return m_DboWeInfoGroup.GetAttribute(uiWorldIndex, x, z);
}

TBLIDX CDboNavi::GetTextAllIndex(const TBLIDX worldTblidx, const float x, const float z)
{
	return m_DboWeInfoGroup.GetTextAllIndex(worldTblidx, x, z);
}

ZONEID CDboNavi::GetZoneIndex(TBLIDX worldTblidx, float x, float z)
{
	return m_DboWeInfoGroup.GetZoneIndex(worldTblidx, x, z);
}

bool CDboNavi::IsBasicAttributeSet(eDBO_WORLD_ATTR_BASIC eAttrBasic, unsigned int uiWorldIndex, float x, float z)
{
	if (eAttrBasic < DBO_WORLD_ATTR_BASIC_FIRST || eAttrBasic > DBO_WORLD_ATTR_BASIC_LAST)
	{
		return false;
	}

	DWORD dwAttributeGroup = GetAttribute(uiWorldIndex, x, z);

	if (0 == (dwAttributeGroup & m_adwBasicAttributeCheckFlag[eAttrBasic]))
	{
	//	printf("IsBasicAttributeSet: dwAttributeGroup %u m_adwBasicAttributeCheckFlag[eAttrBasic] %u \n", dwAttributeGroup, m_adwBasicAttributeCheckFlag[eAttrBasic]);
		return false;
	}

	return true;
}

bool CDboNavi::CanSee(unsigned int uiWorldIndex, float curX, float curY, float curZ, float desX, float desY, float desZ)
{
	return m_DboWeInfoGroup.CanSee(uiWorldIndex, curX, curY, curZ, desX, desY, desZ);
}

void CDboNavi::RegisterTerrainAttributeGroup(eDBO_WORLD_ATTR_GROUP eAttrGroup, eDBO_WORLD_ATTR_BASIC eAttrBasic)
{
	m_adwBasicAttributeCheckFlag[eAttrBasic] |= 0x00000001 << eAttrGroup;
}
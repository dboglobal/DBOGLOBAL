/*****************************************************************************
 *
 * File			: NtlPLCharacterFileData.h
 * Author		: HongHoDong
 * Copyright	: (аж)NTL
 * Date			: 2005. 10. 27.	
 * Abstract		: NTL NtlPLCharacterFileData
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/
#ifndef _CNTL_PL_CHARACTER_PARSER_
#define _CNTL_PL_CHARACTER_PARSER_

#include "NtlAnimEventData.h"
#include "NtlTypeAnimData.h"
#include "NtlCharacterData.h"
#include "NtlPLCharacterProperty.h"


typedef std::map<std::string, unsigned int>           STRING_KEY_MAP;
typedef std::map<std::string, unsigned int>::iterator STRING_KEY_MAP_ITER;

typedef	std::map<unsigned int, std::string>			  KEY_STRING_MAP;
typedef	std::map<unsigned int, std::string>::iterator KEY_STRING_MAP_ITER;

class CNtlMatchingTable
{
protected:
	STRING_KEY_MAP	m_mapStringToKey;
	KEY_STRING_MAP	m_mapKeyToString;

public:
	void	Add(const std::string& strString, unsigned int uiKey);
	
	unsigned int	GetKey(const std::string& strString);
	std::string*	GetString(unsigned int uiKey);
	
	STRING_KEY_MAP	*GetMapStringToKey() { return &m_mapStringToKey; }
	KEY_STRING_MAP  *GetMapKeyToString() { return &m_mapKeyToString; }

	void	Destroy();
};

#define MAX_CHARACTER_FILE_PATH				64

#define CHARACTER_CHUNK_HEADER_VERSION		0.1f
#define CHARACTER_CHUNK_BONE_VERSION		0.1f
#define CHARACTER_ANIM_TABLE_VERSION		0.1f

enum ECharacterChunkID
{
	CHARACTER_CHUNK_HEADER	= 100,
	CHARACTER_CHUNK_BONE	= 200,
	CHARACTER_ANIM_TABLE	= 400
};

struct SCharacterChunkHeaderInfo
{
	ECharacterChunkID	eChunkID;		///< ChunkID
	RwReal				fVersion;		///< Version
	RwReal				fLength;		///< length of the chunk data in bytes
};

class CNtlPLCharacterParser
{
protected:
	CNtlMatchingTable	m_PCMatchTable;
	CNtlMatchingTable	m_MobMatchTable;
	CNtlMatchingTable	m_NpcMatchTable;
    CNtlMatchingTable   m_VehicleTable;

	CNtlMatchingTable	m_LuaProjEffectTypeTable;
	CNtlMatchingTable	m_LuaEffectIDTable;

	std::string			m_strError;
	void	CreateKeyword();

	RwBool	HandleError(const char*  pszFormat, ...);
	void	HandleSucess(const char* pszFormat, ...);

public:
	CNtlPLCharacterParser();

public:
	static	CNtlPLCharacterParser &GetInstance();	
	RwBool	LoadLuaProjectileEffectType(const std::string &strFileName);	

	CNtlMatchingTable *GetPCMatchTable() { return &m_PCMatchTable; }
	CNtlMatchingTable *GetMobMatchTable() { return &m_MobMatchTable; }
    CNtlMatchingTable *GetNPCMatchTable() { return &m_NpcMatchTable;}
    CNtlMatchingTable *GetVehicleTable() {return &m_VehicleTable;}

	CNtlMatchingTable *GetProjEffectTypeTable() { return &m_LuaProjEffectTypeTable; }
	CNtlMatchingTable *GetEffectLuaIDTable() { return &m_LuaEffectIDTable; }

	const std::string &GetLastError() { return m_strError; }
};

#endif
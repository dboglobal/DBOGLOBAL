#include "precomp_ntlpresentation.h"
#include "SpawnTreeNode.h"

using std::string;
#include <string>


CSpawnTreeElement::CSpawnTreeElement(string& _Name, string& _MdlName, RwUInt32 _MdlGUID, BYTE _MdlLvl, RwInt32 _LastMacroIdx)
{
	m_Name			= _Name;
	m_MdlName		= _MdlName;
	m_MdlGUID		= _MdlGUID;
	m_MdlLvl		= _MdlLvl;
	m_LastMacroIdx	= _LastMacroIdx;

	// add level string to mdl sort code index one
	RwChar Tmp[64];
	string Lvl;

	sprintf_s(Tmp, 64, "(%d·¹º§, %d's)", m_MdlLvl, m_MdlGUID);
	Lvl		= Tmp;
	m_Name	= m_Name + Lvl;
}

CSpawnTreeElement::~CSpawnTreeElement(void)
{
}

CSpawnTreeNode::CSpawnTreeNode()
{
}

CSpawnTreeNode::~CSpawnTreeNode(void)
{
	m_vecSpawnTreeNode.clear();
}

VOID CSpawnTreeNode::Insert(RwUInt32 _MdlGUID, BYTE _MdlLvl, string& _Name, string& _MdlName)
{
	CSpawnTreeElement SpawnTreeElement(_Name, _MdlName, _MdlGUID, _MdlLvl, 0);

	m_vecSpawnTreeNode.push_back(SpawnTreeElement);
}
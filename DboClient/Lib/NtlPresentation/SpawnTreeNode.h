#pragma once


using std::vector;
using std::string;
#include <vector>
#include <string>


class CSpawnTreeElement
{
public:
	CSpawnTreeElement() {};
	CSpawnTreeElement(string& _Name, string& _MdlName, RwUInt32 _MdlGUID, BYTE _MdlLvl, RwInt32 _LastMacroIdx);
	virtual ~CSpawnTreeElement(void);

public:
	string		m_Name;
	string		m_MdlName;
	RwUInt32	m_MdlGUID;
	BYTE		m_MdlLvl;
	RwInt32		m_LastMacroIdx;
};

class CSpawnTreeNode
{
public:
	CSpawnTreeNode();
	virtual ~CSpawnTreeNode(void);

public:
	vector<CSpawnTreeElement> m_vecSpawnTreeNode;

public:
	VOID Insert(RwUInt32 _MdlGUID, BYTE _MdlLvl, string& _Name, string& _MdlName);
};

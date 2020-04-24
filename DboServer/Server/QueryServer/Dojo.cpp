#include "stdafx.h"
#include "Dojo.h"
#include "QueryServer.h"
#include "NtlPacketQT.h"



CDojo::CDojo()
{
	Init();
}

CDojo::~CDojo()
{
	Destroy();
}



void CDojo::Init()
{
	smart_ptr<QueryResult> result = GetCharDB.Query("SELECT * FROM dojos LIMIT 7");
	if (result)
	{
		do
		{
			Field* f = result->Fetch();

			sDBO_DOJO_DATA* pData = new sDBO_DOJO_DATA;;
			pData->guildId = f[0].GetUInt32();
			pData->dojoTblidx = f[1].GetUInt32();
			pData->byLevel = f[2].GetBYTE();
			pData->byPeaceStatus = f[3].GetBYTE();
			pData->dwPeacePoint = f[4].GetDWORD();
			NTL_SAFE_WCSCPY(pData->wszName, s2ws(f[5].GetString()).c_str());
			NTL_SAFE_WCSCPY(pData->wchLeaderName, s2ws(f[6].GetString()).c_str());
			NTL_SAFE_WCSCPY(pData->wchNotice, s2ws(f[7].GetString()).c_str());
			pData->challengeGuildId = f[8].GetUInt32();
			NTL_SAFE_WCSCPY(pData->wszSeedCharName, s2ws(f[9].GetString()).c_str());

			m_mapDojo.insert(std::make_pair(pData->guildId, pData));

		} while (result->NextRow());
	}
}

void CDojo::Destroy()
{
	for (std::map<GUILDID, sDBO_DOJO_DATA*>::iterator it = m_mapDojo.begin(); it != m_mapDojo.end();)
	{
		delete it->second;
		it = m_mapDojo.erase(it);
	}
}

void CDojo::SendDojoData(HSESSION hSession)
{
	CNtlPacket packet(sizeof(sQT_DOJO_DATA_RES));
	sQT_DOJO_DATA_RES * res = (sQT_DOJO_DATA_RES *)packet.GetPacketData();
	res->wOpCode = QT_DOJO_DATA_RES;
	res->byCount = 0;

	for (std::map<GUILDID, sDBO_DOJO_DATA*>::iterator it = m_mapDojo.begin(); it != m_mapDojo.end(); it++)
	{
		memcpy(&res->asDojoData[res->byCount++], it->second, sizeof(sDBO_DOJO_DATA));
	}

	packet.SetPacketLen(sizeof(sQT_DOJO_DATA_RES));
	g_pApp->Send(hSession, &packet);
}

void CDojo::CreateDojo(GUILDID guildId, TBLIDX dojoTblidx, WCHAR * guildName)
{
	sDBO_DOJO_DATA* pData = new sDBO_DOJO_DATA;;

	pData->guildId = guildId;
	pData->dojoTblidx = dojoTblidx;
	pData->byLevel = 0;
	pData->byPeaceStatus = eDBO_DOJO_PEACE_TYPE_COMMON;
	pData->dwPeacePoint = 0;
	NTL_SAFE_WCSCPY(pData->wszName, guildName);
	pData->challengeGuildId = INVALID_GUILDID;

	m_mapDojo.insert(std::make_pair(pData->guildId, pData));
}

bool CDojo::DeleteDojo(GUILDID guildId, TBLIDX dojoTblidx)
{
	std::map<GUILDID, sDBO_DOJO_DATA*>::iterator it = m_mapDojo.find(guildId);
	if(it != m_mapDojo.end())
	{
		sDBO_DOJO_DATA* pDojo = it->second;

		if (pDojo->dojoTblidx == dojoTblidx)
		{
			delete it->second;
			m_mapDojo.erase(it);

			return true;
		}
	}

	return false;
}

bool CDojo::DeleteDojo(GUILDID guildId)
{
	std::map<GUILDID, sDBO_DOJO_DATA*>::iterator it = m_mapDojo.find(guildId);
	if (it != m_mapDojo.end())
	{
		delete it->second;
		m_mapDojo.erase(it);

		return true;
	}

	return false;
}

sDBO_DOJO_DATA * CDojo::GetDojoData(GUILDID guildId)
{
	std::map<GUILDID, sDBO_DOJO_DATA*>::iterator it = m_mapDojo.find(guildId);
	if (it != m_mapDojo.end())
	{
		return it->second;
	}

	return nullptr;
}

sDBO_DOJO_DATA * CDojo::GetDojoDataWithIdx(TBLIDX dojoIdx)
{
	for(std::map<GUILDID, sDBO_DOJO_DATA*>::iterator it = m_mapDojo.begin(); it != m_mapDojo.end(); it++)
	{
		if(it->second->dojoTblidx == dojoIdx)
			return it->second;
	}

	return nullptr;
}

void CDojo::RemoveDojo(GUILDID guildId)
{
	m_mapDojo.erase(guildId);
}

void CDojo::InsertDojo(GUILDID guildId, sDBO_DOJO_DATA * pData)
{
	m_mapDojo.insert(std::make_pair(guildId, pData));
}

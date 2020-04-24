#ifndef __SERVER_GUILD___
#define __SERVER_GUILD___

#include "NtlGuild.h"
#include <map>

class CPlayer;
class CNtlPacket;
class CDojo;

class CGuild
{

public:

	CGuild(sDBO_GUILD_DATA& info);
	virtual ~CGuild();

public:

	void									InsertOnlineMember(CHARACTERID charId, CPlayer* pPlayer);
	void									EraseOnlineMember(CHARACTERID charId);

	sDBO_GUILD_MEMBER_INFO*					GetMemberInfo(CHARACTERID charID);
	sDBO_GUILD_MEMBER_INFO*					GetMemberInfo(WCHAR* wchCharName);
	void									AddMemberInfo(sDBO_GUILD_MEMBER_INFO* pMemberInfo);
	void									DelMemberInfo(CHARACTERID charID);

	void									AddMemberInfo(sDBO_GUILD_MEMBER_DATA& rMemberData);

	void									SetGuildFunctionFlag(QWORD qwGuildFunctionFlag);

	void									AddSecondGuildMaster(CHARACTERID charId);
	void									RemoveSecondGuildMaster(CHARACTERID charId);

private:

	sDBO_GUILD_DATA							 guildInfo;

	typedef std::map<CHARACTERID, CPlayer*>	 MEMBER_MAP;
	MEMBER_MAP								 mapMembers;

	std::map<CHARACTERID, sDBO_GUILD_MEMBER_INFO*>	m_mapMemberInfo;

	CDojo*									 m_pkDojo;

	TBLIDX									m_dojoTblidxWarRequest;

	CHARACTERID								m_GuildBankUser;

public:

	CDojo*									GetDojo() { return m_pkDojo; }

	sDBO_GUILD_DATA*						GetInfo() { return &guildInfo; }

	void									UpdateNotice(WCHAR* wchName, WCHAR* wchNotice);

	void									Broadcast(CNtlPacket* pPacket, CHARACTERID exceptId = INVALID_CHARACTERID);

	void				SetMember(bool bOnline, CPlayer* player);

	CPlayer*			GetMember(CHARACTERID memberId);

	void				UpdateMemberInfoMapName(CHARACTERID charID, TBLIDX mapNameTblidx);
	void				UpdateMemberInfoClass(CHARACTERID charID, BYTE byNewClass);
	void				UpdateMemberInfoLevel(CHARACTERID charID, BYTE byNewLevel);
	void				UpdateMemberInfoReputation(CHARACTERID charID, DWORD dwReputation);
	void				UpdateMemberInfoCharName(CHARACTERID charID, WCHAR* wchNewCharName);
	void				UpdateMemberInfoOnline(CHARACTERID charID, TBLIDX mapNameTblidx);
	void				UpdateMemberInfoOffline(CHARACTERID charID);

	void				LoadGuildMembers(CPlayer* player);

	bool				IsGuildMaster(CHARACTERID memberId);

	BYTE				IsSecondGuildMaster(CHARACTERID memberId); //return 255 if not second guild master

	CHARACTERID			GetGuildMaster() { return guildInfo.guildMaster; }
	void				SetGuildMaster(CHARACTERID charId) { guildInfo.guildMaster = charId; }

	void				DisbandGuild();

	DWORD				GetTotalMemberCount() { return (DWORD)m_mapMemberInfo.size(); }
	DWORD				GetOnlineMemberCount() { return (DWORD)mapMembers.size(); }

	bool				CanAppointSecondMaster(WORD & rwResultcode, CHARACTERID appointCharId);

	void				UpdateReputation(DWORD dwPoints, bool bIsPlus);

	WORD				AddFunction(CPlayer* pMaster, DWORD dwZeniCost, BYTE byFunction, bool bIsGuildFunction = true);

	WORD				CanOpenGuildBank(CHARACTERID charId);
	void				CloseGuildBank();

	bool				IsGuildBankInUse() { return m_GuildBankUser != INVALID_CHARACTERID; }
	CHARACTERID			GetGuildBankUser() { return m_GuildBankUser; }

	void				SetGuildMark(sDBO_GUILD_MARK& mark);

	void				SetGuildName(WCHAR* wchNewName);
	WCHAR*				GetGuildName() { return guildInfo.wszName; }

	//DOJO
	bool				HasDojo() { return m_pkDojo == NULL ? false : true; } //return false if dojo is null
	void				SetDojo(CDojo* dojo) { m_pkDojo = dojo; }

	void				SetDojoTblidxWarRequest(TBLIDX idx) { m_dojoTblidxWarRequest = idx; }
	TBLIDX				GetDojoTblidxWarRequest() { return m_dojoTblidxWarRequest; }

	void				FillOnlineMember(CHARACTERID* pCharID, BYTE& rbyCount);

	bool				CheckGuildReadyForDojo();
};


#endif
#ifndef __INC_DBOG_GAME_ITEMDROP_H__
#define __INC_DBOG_GAME_ITEMDROP_H__

#include "SpawnObject.h"

class CPlayer;
struct sITEM_TBLDAT;

class CItemDrop : public CSpawnObject
{

protected:

	virtual void	CopyToObjectInfo(sOBJECT_INFO *pObjectInfo, CHARACTERID playerCharID);

public:

	CItemDrop(eOBJTYPE eObjType);
	virtual ~CItemDrop();

public:

	inline TBLIDX				GetTblidx()  { return m_tblidx; }
	inline BYTE					GetBattleAttribute()  { return m_byBattleAttribute; }
	inline BYTE					GetGrade()  { return m_byGrade; }
	inline BYTE					GetRank()  { return m_byRank; }
	inline void					SetNeedToIdentify(bool bFlag) { m_bNeedToIdentify = bFlag; }
	inline bool					NeedToIdentify()  { return m_bNeedToIdentify; }

	void						GenerateRank(sITEM_TBLDAT* table, bool bDungeon, BYTE byMobGrade, DWORD dwSuperior, DWORD dwExcellent, DWORD dwRare, DWORD dwLegendary);

	void						SetTbldat(sITEM_TBLDAT* table, bool bCanUnidentified = true);

	void						PickUpItem(CPlayer* pPlayer);
	void						PickUpZeni(CPlayer* pPlayer);

	void						RemoveFromGround();
	bool						AddToGround(WORLDID Worldid, sVECTOR3 & pos);

	bool						IsOwnership(CPlayer* ch);

	inline void					SetZeni(DWORD zeni) { m_dwZeniDrop = zeni; }
	inline DWORD				GetZeni() { return m_dwZeniDrop; }

	void						GenerateOptionSet(sITEM_TBLDAT* table, bool bEnchantAble = false);
	inline sITEM_OPTION_SET*	GetOptionSet() { return m_sOptionSet; }
				
	inline HOBJECT				GetOwnerID() { return m_hOwner; }
	inline PARTYID				GetPartyID() { return m_partyID; }

	inline void					SetLocked(bool bFlag) { m_bLocked = bFlag; }
	inline DWORD				GetLocked() { return m_bLocked; }

private:

	//Owner
	HOBJECT						m_hOwner;
	PARTYID						m_partyID;

	DWORD						m_dwDropTime;

	//item drop only
	TBLIDX						m_tblidx;
	BYTE						m_byBattleAttribute;
	BYTE						m_byGrade;
	BYTE						m_byRank;
	bool						m_bNeedToIdentify;
	sITEM_OPTION_SET*			m_sOptionSet;

	//zeni drop only
	DWORD						m_dwZeniDrop;

	//used for dice
	bool						m_bLocked;


public:

	void		StartDestroyEvent();
	void		event_DestroyEvent();

	void		SetOwnership(HOBJECT hOwner, PARTYID partyID);
	void		event_Ownership();
};

#endif

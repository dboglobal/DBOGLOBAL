#ifndef __CSUMMONPET__
#define __CSUMMONPET__

#include "Npc.h"

class CPlayer;

class CSummonPet : public CNpc
{

public:

	CSummonPet();
	virtual ~CSummonPet();

	const static int			FOLLOW_BEGIN_DISTANCE_IN_METERS = 4;
	const static int			FOLLOW_END_DISTANCE_IN_METERS = 2;
	const static int			TELEPORT_BEGIN_DISTANCE_IN_METERS = 55;

protected:

	virtual void				CopyToObjectInfo(sOBJECT_INFO *pObjectInfo, CHARACTERID playerCharID);

private:

	void						Initialize();
	void						Destroy();

public:

	bool						CreateDataAndSpawn(HOBJECT hOwner, TBLIDX sourceTblidx, sNPC_TBLDAT* npcTbldat, WORLDID worldId, CNtlVector& rCurLoc, CNtlVector& rCurDir);

	void						Despawn(bool bSendPacket = false);

	void						CopySummonPetProfile(sSUMMON_PET_PROFILE* pProfile);

	void						Spawn(CPlayer* pOwner);

	virtual void				LeaveGame();

	virtual HOBJECT				ConsiderAttackTarget(DWORD dwTickTime);

	virtual bool				AttackProgress(DWORD dwTickDiff, float fMultiple);

public:

	CPlayer*					GetOwner();
	HOBJECT						GetOwnerID() { return m_hOwner; }
	BYTE						GetLevel();

	HOBJECT						GetAttackTarget() { return m_hAttackTarget; }
	void						SetAttackTarget(HOBJECT hAttackTarget) { m_hAttackTarget = hAttackTarget; }

	bool						GetToggleAttack() { return m_bToggleAttack; }
	void						SetToggleAttack(bool bFlag) { m_bToggleAttack = bFlag; }



private:

	HOBJECT						m_hOwner;

	TBLIDX						m_sourceTblidx;
	
	HOBJECT						m_hAttackTarget;

	bool						m_bToggleAttack;

};



#endif
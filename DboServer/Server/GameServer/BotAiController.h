#ifndef __AI_DBOG_CONTROLLER_H__
#define __AI_DBOG_CONTROLLER_H__

#include "ObjectController.h"
#include "CheckupEndCondition.h"

class CBotAiState;
class CBotAiState_Fight;
class CNpc;

class CBotAiController : public CObjectController
{

public:
	CBotAiController();
	virtual	~CBotAiController();

public:

	bool			Create(CNpc* pBot);
	void			Destroy();

protected:

	void			Init();

public:

	virtual int		OnUpdate(DWORD dwTickDiff, float fMultiple);
	virtual int		OnObjectMsg(CObjectMsg * pObjMsg);


public:

	bool			ChangeAiState(CBotAiState* pAiState);
	bool			AddAiState(CBotAiState* pAiState);

	void			RemoveAllControlState();
	void			DestroyAllControlState();

	void			AllStop();

	CBotAiState*	GetCurrentState();
	int				GetCurrentStateID();
	bool			IsCurrentStateID(int id);

	CNpc*			GetContolObject() { return m_pControlObject; }

	void			SetControlBlock(bool bBlock);
	bool			IsControlStateBlocked();
	void			SetNextAiState(CBotAiState*	pNextAiState);
	bool			IsEmptyNextControlState();
	bool			ChangeControlState_Idle();
	bool			ChangeControlState_Enter();
	bool			ChangeControlState_Spawn();
	bool			ChangeControlState_Leave();
	bool			ChangeControlState_Faint();
	bool			ChangeControlState_Despawn();
	bool			ChangeControlState_Teleporting(WORLDID worldId, TBLIDX worldTblidx, CNtlVector& rvNewLoc, CNtlVector& rvNewDir);
	bool			AddControlState_Fight(HOBJECT hTarget);
	bool			AddControlState_BattleScene(TBLIDX tblidxBattleScript, DWORD dwBSFlowNodeIndex);

	CCheckupEndCondition*		GetCheckupEndCondition() { return &m_cCheckupEndCondition; }
	CBotAiState_Fight*			GetFightState();



private:

	CNpc*					m_pControlObject;
	bool					m_bControlBlock;
	CNtlLinkList			m_cAddStateList;
	CBotAiState*			m_pNextAiState;
	CNtlLinkList			m_controlStateStack;

	CCheckupEndCondition	m_cCheckupEndCondition;

	DWORD					m_dwSumDelayTick;
};



#endif
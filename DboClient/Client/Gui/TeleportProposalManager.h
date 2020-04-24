/******************************************************************************
* File			: TeleportProposalManager.h
* Author		: Haesung Cho
* Copyright		: (주)NTL
* Date			: 2009. 4. 24
* Abstract		: 
*****************************************************************************
* Desc			: Teleport proposal 들을 관리한다.
*****************************************************************************/

#ifndef __TELEPORT_PROPOSAL_MANAGER_H__
#define __TELEPORT_PROPOSAL_MANAGER_H__

#include "ceventhandler.h"
#include "DboEvent.h"

#define dMAX_TELEPORT_PROPOSAL			(256)

struct STeleportProposal
{
	bool							bProposal;
	SDboEventTeleportProposalNfy	dataNotify;

	STeleportProposal::STeleportProposal()
	{
		bProposal = false;
		memset( &dataNotify, 0xFF, sizeof(SDboEventTeleportProposalNfy) );
	}
};

class CTeleportProposalManager : public RWS::CEventHandler
{
public:
	~CTeleportProposalManager();

	//! Access
	static RwBool CreateInstance(VOID);
	static VOID   DeleteInstance(VOID);

	static CTeleportProposalManager* GetInstance(VOID);

	RwBool			Create();
	VOID			Destroy();

	virtual void	HandleEvents(RWS::CMsg& msg);

	void			Update(RwReal fElapsed);

	void			HandleTeleportProposalNfy(RWS::CMsg& msg);
	void			HandleTeleportConfirmRes(RWS::CMsg& msg);
	void			HandleMsgBoxResult(RWS::CMsg& msg);
	void			HandleUpdateTick(RWS::CMsg& msg);

	void				SendTeleportProposal( STeleportProposal* pProposal, RwBool bFinalConfirm = FALSE );
	STeleportProposal*	GetTeleportProposal( RwUInt8 byIndex );

	RwBool			IsAvatar();
	RwBool			IsGameStage();

protected:
	CTeleportProposalManager();

	typedef std::map<BYTE, STeleportProposal*> MapTeleportProposal;
	MapTeleportProposal	m_mapTeleportProposal;

	typedef void (CTeleportProposalManager::*FuncEventHander)(RWS::CMsg &pMsg);
	typedef std::map<const RWS::CRegisteredMsgs*, FuncEventHander> MapFuncEventHandler;
	MapFuncEventHandler m_mapEventFunc;

	RwReal				m_fElapsedTime;

	static CTeleportProposalManager* m_pInstance;
};

static CTeleportProposalManager* GetTeleportProposalManager()
{
	return CTeleportProposalManager::GetInstance();
}

#endif
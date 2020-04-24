#ifndef __SERVER_HLS_SLOT_MACHINE_MANAGER__
#define __SERVER_HLS_SLOT_MACHINE_MANAGER__


#include "NtlSingleton.h"
#include "HlsSlotMachineItemTable.h"
#include "NtlSharedDef.h"
#include <list>

class CPlayer;
struct sHLS_ITEM_TBLDAT;
struct sHLS_SLOT_MACHINE_TBLDAT;

struct sHLS_SLOT_ITEM
{
	WORD							wCountLeft;
	BYTE							byRank; //top 10
	float							fPercent;
	sHLS_ITEM_TBLDAT*				pHlsItem;
	sHLS_SLOT_MACHINE_ITEM_TBLDAT*	pSlotItem;
};

struct sHLS_SLOT_WINNER_INFO
{
	WCHAR			wszPlayer[NTL_MAX_SIZE_CHAR_NAME + 1];
	WORD			wWinCount;
	DBOTIME			nExtractTime;
	QWORD			winnerIndex;
	UINT			byRank;
};

struct sSLOT_MACHINE
{
	sSLOT_MACHINE()
	{
		wMaxCapsule = 800;
		wCurrentCapsule = 800;
	}

	sHLS_SLOT_MACHINE_TBLDAT*	pTbldat;
	WORD						wMaxCapsule;
	WORD						wCurrentCapsule;
};

class CHlsSlotMachine : public CNtlSingleton<CHlsSlotMachine>
{

public:

	CHlsSlotMachine();
	virtual ~CHlsSlotMachine();

public:

	void							Init();

	void							GetSlotItems(TBLIDX slotIdx, std::vector<sHLS_SLOT_ITEM*>* pVec);

	void							AddWinner(TBLIDX slotId, TBLIDX itemTblidx, CPlayer* pPlayer);

	void							GetWinnerInfo(WORD wSlot, CPlayer * pPlayer);

	void							LoadSlotMachines(CPlayer* pPlayer, BYTE byType);

	sSLOT_MACHINE*					GetSlotMachine(TBLIDX tblidx);

private:

	typedef std::multimap<TBLIDX, sHLS_SLOT_ITEM*> SLOTMACHINEGROUP;
	typedef SLOTMACHINEGROUP::iterator SLOTMACHINEGROUP_IT;
	typedef SLOTMACHINEGROUP::value_type SLOTMACHINEGROUP_VAL;

	SLOTMACHINEGROUP							m_slotMachineGroup;

	QWORD										m_aWinnerIndex[2];
	std::list<sHLS_SLOT_WINNER_INFO*>			m_slotWinnerInfo[2]; // 2 = amount of machines

	std::map<TBLIDX, sSLOT_MACHINE*>			m_mapSlotMachine;
};

#define GetHlsSlotMachine()			CHlsSlotMachine::GetInstance()
#define g_pHlsSlotMachine			GetHlsSlotMachine()

#endif
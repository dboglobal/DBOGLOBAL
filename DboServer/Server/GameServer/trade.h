#ifndef __SERVER_TRADE_MANAGER__
#define __SERVER_TRADE_MANAGER__

#include "NtlItem.h"
#include "NtlSingleton.h"


class CPlayer;
class CItem;

enum eTRADE_STATE
{
	eTRADE_STATE_NONE,
	eTRADE_STATE_WAIT_FOR_ACCEPT,
	eTRADE_STATE_TRADING,
	eTRADE_STATE_FINISHING_TRADE
};


class CTrade
{

public:

	CTrade(CPlayer* owner);
	~CTrade();

public:

	BYTE			GetTradeState() { return m_byTradeState; }

	DWORD			GetStartTick() { return m_dwStartTick; }

	CPlayer*		GetOwner() { return m_pOwner; }

	void			SetCompany(CTrade* company) { m_pCompany = company; }
	CTrade*			GetCompany() { return m_pCompany; }

	void			SetState(BYTE byState) { m_byTradeState = byState; }

	bool			IsReady() { return m_bIsReady; }
	void			SetReady(bool b) { m_bIsReady = b; }

	BYTE			GetItemCount() { return (BYTE)p_ItemData.size(); }

	bool			IsBlocked() { return m_dwBlockTime >= GetTickCount(); }

	void			SetOtherFreeSlotCount(BYTE byCount) { m_byOtherFreeSlotCount = byCount; }
	BYTE			GetOtherFreeSlotCount() { return m_byOtherFreeSlotCount; }

private:

	BYTE			m_byTradeState;//eTRADE_STATE
	DWORD			m_dwStartTick;
	DWORD			m_dwPacketCount;
	bool			m_bIsReady;

	DWORD			m_dwZeni;

	typedef std::map<HOBJECT, CItem*> MAPITEMDATA;
	MAPITEMDATA p_ItemData;


	CTrade*		m_pCompany;

	CPlayer*	m_pOwner;

	DWORD			m_dwBlockTime;

	BYTE			m_byOtherFreeSlotCount;


public:

	void			AddItem(CItem* item, BYTE byCount);
	void			DelItem(HOBJECT hHandle);
	CItem*			FindItem(HOBJECT hHandle);

	DWORD			GetZeni() { return m_dwZeni; }
	void			UpdateZeni(DWORD zeni);

	void			ModifyItem(HOBJECT hHandle, BYTE byCount);

	void			Trade();
};



class CTradeManager : public CNtlSingleton<CTradeManager>
{

public:

	CTradeManager();
	virtual ~CTradeManager();

private:

	DWORD	m_dwTradeCount;

	typedef std::set<CTrade*> TSETTRADE;
	TSETTRADE m_set_Trade;

public:

	void		TickProcess(DWORD dwCurTick);

	void		StartTrade(CPlayer* requestor, CPlayer* target);
	void		DestroyTrade(CTrade* trade);

	void		TradeEnd(CTrade* trade);

	void		EndTradeByTimeOut(CTrade* trade);

	bool		FinishTrade(CTrade* trade);

};
#define GetTradeManager()		CTradeManager::GetInstance()
#define g_pTradeManager			GetTradeManager()

#endif
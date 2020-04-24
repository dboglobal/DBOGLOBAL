#pragma once

#include "ceventhandler.h"

enum EHISTROY_ID
{
	EHISTROY_ID_NONE,
	EHISTROY_ID_TERRAIN,
};

enum EHISTROY_STATE
{
	EHISTROY_STATE_NONE,
	EHISTROY_STATE_UNDO,
	EHISTROY_STATE_REDO,
};

class CNtlHistoryData
{
public:
	CNtlHistoryData();
	virtual ~CNtlHistoryData();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlHistoryAction
{
public:
	CNtlHistoryAction();
	virtual ~CNtlHistoryAction();

	virtual void		PushHistoryData(CNtlHistoryData* pHistoryData);
	virtual void		DeleteHistoryDataAll();

	virtual void		Undo();
	virtual void		Redo();
	virtual RwBool		CheckException();

	virtual void		Action(CNtlHistoryData* pHistoryData)	{ Redo(pHistoryData); }

	virtual RwUInt32	GetState()								{ return m_uiState; }

protected:
	virtual void		Undo(CNtlHistoryData* pHistoryData) = 0;
	virtual void		Redo(CNtlHistoryData* pHistoryData) = 0;
	virtual RwBool		CheckException(CNtlHistoryData* pHistoryData) = 0;


protected:
	typedef std::vector<CNtlHistoryData*>		VEC_HISTORY_DATA;
	typedef VEC_HISTORY_DATA::iterator			VEC_HISTORY_DATA_IT;
	typedef VEC_HISTORY_DATA::reverse_iterator	VEC_HISTORY_DATA_RIT;

	VEC_HISTORY_DATA						m_vecHistoryDataList;
	RwUInt32								m_uiState;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// class CNtlHistoryController : public RWS::CEventHandler
// {
// public:
// 	CNtlHistoryController();
// 	virtual ~CNtlHistoryController();
// 
// 	CNtlHistoryAction*	PushHistoryAction(CNtlHistoryAction* pHistoryAction, RwUInt32 uiHistroyID);
// 
// 	void				DeleteHistroyActionAfter(RwUInt32 uiIndex);
// 	void				DeleteHistroyActionBefore(RwUInt32 uiIndex);
// 	void				DeleteHistroyActionAll();
// 
// 	void				Undo();
// 	void				Redo();
// 	
// 	RwBool				IsLastHistoryAction();
// 
// 	virtual void		HandleEvents(RWS::CMsg &pMsg);
// 
// protected:
// 	void				CheckException();
// 
// protected:
// 	typedef std::vector<CNtlHistoryAction*>			VEC_HISTORY_ACTION;
// 	typedef VEC_HISTORY_ACTION::iterator			VEC_HISTORY_ACTION_IT;
// 	typedef VEC_HISTORY_ACTION::reverse_iterator	VEC_HISTORY_ACTION_RIT;
// 
// 	VEC_HISTORY_ACTION						m_vecHistoryActionList;
// 	RwUInt32								m_uiHistroyID;
// 
// 	RwUInt32								m_uiCurrentIndex;
// };

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define dNTL_HISTORY_INVALID_IDX (static_cast<RwUInt32>(-1))

class CNtlHistoryController : public RWS::CEventHandler
{
public:
	CNtlHistoryController();
	virtual ~CNtlHistoryController();

	CNtlHistoryAction*	PushHistoryAction(CNtlHistoryAction* pHistoryAction, RwUInt32 uiHistroyID);

	void				DeleteHistroyActionAfter(RwUInt32 uiIndex);
	void				DeleteHistroyActionBefore(RwUInt32 uiIndex);
	void				DeleteHistroyActionAll();

	void				Undo();
	void				Redo();
	void				SetHistroyActionPos(RwUInt32 uiIdx);

	RwUInt32			GetHistoryActionCur()	{ return m_uiCurrentIndex; }
	RwUInt32			GetHistoryActionNum()	{ return m_vecHistoryActionList.size(); }

	virtual void		HandleEvents(RWS::CMsg &pMsg);

protected:
	void				CheckException();

protected:
	typedef std::vector<CNtlHistoryAction*>			VEC_HISTORY_ACTION;
	typedef VEC_HISTORY_ACTION::iterator			VEC_HISTORY_ACTION_IT;
	typedef VEC_HISTORY_ACTION::reverse_iterator	VEC_HISTORY_ACTION_RIT;

	VEC_HISTORY_ACTION						m_vecHistoryActionList;
	RwUInt32								m_uiHistroyID;

	RwUInt32								m_uiCurrentIndex;
};
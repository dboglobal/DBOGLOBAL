#pragma once

class CScheduler;
class CScheduleElement;

enum ESCHEDULE_STEP
{
	ESCHEDULE_STEP_NONE,
	ESCHEDULE_STEP_BEGIN,
	ESCHEDULE_STEP_SCHEDULING,
	ESCHEDULE_STEP_END,
};

class CScheduler
{
public:
	typedef std::list<CScheduleElement*>	LIST_ELE;
	typedef LIST_ELE::iterator				LIST_ELE_IT;

protected:
	LIST_ELE			m_listScheduleElement;
	RwReal				m_fElapsedTime;

public:
	CScheduler();
	virtual ~CScheduler();

	virtual RwBool				Scheduling(RwReal fElapsedTime);
	virtual RwBool				SchedulingProcess();
	virtual void				RemovePresentSchedule();

	virtual void				AddSchedule(CScheduleElement* pNewScheduleElement);
	virtual void				RemoveSchedule(RwInt32 iPrimaryKey);
	virtual void				ClearSchedule();

	virtual CScheduleElement*	GetFirstScedule();
};

class CScheduleElement
{
protected:
	RwInt32			m_iPrimaryKey;
	RwReal			m_fUsedTime;
	ESCHEDULE_STEP	m_eScheduleStep;


public:
	CScheduleElement(RwInt32 iPrimaryKey, RwReal fUsedTime);
	virtual ~CScheduleElement();

	virtual RwInt32			GetPrimaryKey()		{ return m_iPrimaryKey; }
	virtual RwReal			GetUsedTime()		{ return m_fUsedTime; }
	virtual ESCHEDULE_STEP	GetScheduleStep()	{ return m_eScheduleStep; }

	// 스케쥴링 준비.
	// RETURN : TRUE(준비성공)
	//			FALSE(준비실패) - Scheduler의 List에서 제거 된다.
	virtual RwBool	Begin() = 0;

	// 스케쥴링을 실행.
	// RETURN : TRUE(다음에 다시 한번 호출을 요구할때)
	//          FALSE(스케쥴링 할 것이 없을 경우나 스케쥴링이 끝날 경우)
	virtual RwBool	Scheduling() = 0;

	// 스케쥴링의 종료.
	// RETURN : 스케쥴링이 끝나고 나서 호출된다. RETURN의 의미는 없다.
	//          호출된 후 Scheduler의 List에서 제거 된다.  
	virtual RwBool	End() = 0;

	friend class CScheduler;
};
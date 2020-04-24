#ifndef _EVENTABLE_OBJECT_H
#define _EVENTABLE_OBJECT_H


/**
  * @class EventableObject
  * EventableObject means that the class inheriting this is able to take
  * events. This 'base' class will store and update these events upon
  * receiving the call from the instance thread / WorldRunnable thread.
  */

#include "EventMgr.h"
#include <list>
#include <set>


typedef std::list<TimedEvent*> EventList;
typedef std::multimap<unsigned int, TimedEvent*> EventMap;

#define EVENT_REMOVAL_FLAG_ALL 0xFFFFFFFF

class EventableObject
{
	friend class EventMgr;
	friend class EventableObjectHolder;

protected:
	void event_RemoveEvents();
	void event_RemoveEvents(unsigned int EventType);
	void event_ModifyTimeLeft(unsigned int EventType, DWORD TimeLeft,bool unconditioned=false);
	void event_ModifyTime(unsigned int EventType, DWORD Time);
	void event_ModifyTimeAndTimeLeft(unsigned int EventType, DWORD Time);
	bool event_HasEvent(unsigned int EventType);
	DWORD event_GetTimeLeft(unsigned int EventType);
	DWORD event_GetRepeatLeft(unsigned int EventType);
	void event_RemoveByPointer(TimedEvent * ev);
	inline signed __int32 event_GetCurrentInstanceId() { return m_event_Instanceid; }
	bool event_GetTimeLeft(unsigned int EventType, unsigned int * Time);

public:
	unsigned int event_GetEventPeriod(unsigned int EventType);
	// Public methods
	EventableObject();
	virtual ~EventableObject();

	inline bool event_HasEvents() { return m_events.size() > 0 ? true : false; }
	void event_AddEvent(TimedEvent * ptr);
	void event_Relocate();
	
	// this func needs to be implemented by all eventable classes. use it to retreive the instance
	// id that it needs to attach itself to.
	
	virtual int		event_GetInstanceID() { return -1; }

protected:

	signed __int32 m_event_Instanceid;
	EventMap m_events;
	EventableObjectHolder * m_holder;
	
};

/**
  * @class EventableObjectHolder
  * EventableObjectHolder will store eventable objects, and remove/add them when they change
  * from one holder to another (changing maps / instances).
  *
  * EventableObjectHolder also updates all the timed events in all of its objects when its
  * update function is called.
  *
  */

typedef std::set<EventableObject*> EventableObjectSet;

class EventableObjectHolder
{

public:

	EventableObjectHolder(int instance_id);
	~EventableObjectHolder();

	void Update(DWORD time_difference);

	void AddEvent(TimedEvent * ev);
	void AddObject(EventableObject * obj);

	inline int GetInstanceID() { return m_InstanceId; }

protected:

	int					m_InstanceId;
	EventList			m_events;

	typedef std::list<TimedEvent*> InsertableQueue;
	InsertableQueue m_insertPool;
};

#endif

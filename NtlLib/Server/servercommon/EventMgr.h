#ifndef __EVENTMGR_H
#define __EVENTMGR_H

#include "NtlSingleton.h"
#include <map>

#include "CallBack.h"


enum EventTypes
{
	EVENT_UNK = 0,
	EVENT_PLAYER_COMBAT,
	EVENT_DEAD_TIME,
	EVENT_FREEBATTLE,
	EVENT_PLAYER_DISABLE_TRIGGER,
	EVENT_ON_LAVA,
	EVENT_INVINCIBLE,

	EVENT_ITEM_OWNERSHIP,
	EVENT_ITEM_DESTROY,

	EVENT_TELEPORT_PROPOSAL,

	EVENT_SERVER_SHUT_DOWN,

	EVENT_TRIGGEROBJ_DESTROY,

	EVENT_MASCOT_VP_REGEN,

	SERVER_EVENT_KEEP_ALIVE,
	SERVER_EVENT_LOAD_UPDATE,

	EVENT_DOJO_WAR_REQUEST,

	EVENT_SERVER_SESSION_UPDATE,
	EVENT_QUERY_SERVER_SESSION_UPDATE,
};

enum EventFlags
{
	EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT = 0x1,
	EVENT_FLAG_DELETES_OBJECT				   = 0x2,
};

struct TimedEvent
{
	TimedEvent(void* object, CallbackBase* callback, unsigned int type, DWORD time, DWORD repeat, unsigned int flags) :
		obj(object), cb(callback), eventType(type), eventFlag(flags), msTime(time), currTime(time), repeats(repeat), deleted(false),ref(0) {}
		
	void *obj;
	CallbackBase *cb;
	unsigned int eventType;
	unsigned int eventFlag;
	DWORD msTime;
	DWORD currTime;
	DWORD repeats;
	bool deleted;
	int instanceId;
	volatile long ref;

	static TimedEvent * Allocate(void* object, CallbackBase* callback, unsigned int type, DWORD time, DWORD repeat, unsigned int flags);

	void DecRef()
	{
		InterlockedDecrement(&ref);
		if(ref <= 0)
		{
			delete cb;
			delete this;
		}
	}

	void IncRef() { InterlockedIncrement(&ref); }
};

class EventableObjectHolder;
typedef std::map<int, EventableObjectHolder*> HolderMap;

class EventMgr : public CNtlSingleton <EventMgr>
{

public:

	EventMgr() {}
	virtual ~EventMgr() {}

public:
	template <class Class>
		void AddEvent(Class *obj, void (Class::*method)(), unsigned int type, DWORD time, DWORD repeats, unsigned int flags)
	{
		// create a timed event
		TimedEvent * event = new TimedEvent(obj, new CallbackP0<Class>(obj, method), type, time, repeats, flags);

		// add this to the object's list, updating will all be done later on...
		obj->event_AddEvent(event);
	}

	template <class Class, typename P1>
		void AddEvent(Class *obj, void (Class::*method)(P1), P1 p1, unsigned int type, DWORD time, DWORD repeats, unsigned int flags)
	{
		// create a timed event
		TimedEvent * event = new TimedEvent(obj, new CallbackP1<Class, P1>(obj, method, p1), type, time, repeats, flags);

		// add this to the object's list, updating will all be done later on...
		obj->event_AddEvent(event);
	}

	template <class Class, typename P1, typename P2>
		void AddEvent(Class *obj, void (Class::*method)(P1,P2), P1 p1, P2 p2, unsigned int type, DWORD time, DWORD repeats, unsigned int flags)
	{
		// create a timed event
		TimedEvent * event = new TimedEvent(obj, new CallbackP2<Class, P1, P2>(obj, method, p1, p2), type, time, repeats, flags);

		// add this to the object's list, updating will all be done later on...
		obj->event_AddEvent(event);
	}

	template <class Class, typename P1, typename P2, typename P3>
		void AddEvent(Class *obj,void (Class::*method)(P1,P2,P3), P1 p1, P2 p2, P3 p3, unsigned int type, DWORD time, DWORD repeats, unsigned int flags)
	{
		// create a timed event
		TimedEvent * event = new TimedEvent(obj, new CallbackP3<Class, P1, P2, P3>(obj, method, p1, p2, p3), type, time, repeats, flags);

		// add this to the object's list, updating will all be done later on...
		obj->event_AddEvent(event);
	}

	template <class Class, typename P1, typename P2, typename P3, typename P4>
		void AddEvent(Class *obj, void (Class::*method)(P1,P2,P3,P4), P1 p1, P2 p2, P3 p3, P4 p4, unsigned int type, DWORD time, DWORD repeats, unsigned int flags)
	{
		// create a timed event
		TimedEvent * event = new TimedEvent(obj, new CallbackP4<Class, P1, P2, P3, P4>(obj, method, p1, p2, p3, p4), type, time, repeats, flags);

		// add this to the object's list, updating will all be done later on...
		obj->event_AddEvent(event);
	}

	template <class Class, typename P1, typename P2, typename P3, typename P4, typename P5>
	void AddEvent(Class *obj, void (Class::*method)(P1,P2,P3,P4,P5), P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, unsigned int type, DWORD time, DWORD repeats, unsigned int flags)
	{
		// create a timed event
		TimedEvent * event = new TimedEvent(obj, new CallbackP5<Class, P1, P2, P3, P4, P5>(obj, method, p1, p2, p3, p4, p5), type, time, repeats, flags);

		// add this to the object's list, updating will all be done later on...
		obj->event_AddEvent(event);
	}

	template <class Class, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
	void AddEvent(Class *obj, void (Class::*method)(P1,P2,P3,P4,P5,P6), P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6,unsigned int type, DWORD time, DWORD repeats, unsigned int flags)
	{
		// create a timed event
		TimedEvent * event = new TimedEvent(obj, new CallbackP6<Class, P1, P2, P3, P4, P5, P6>(obj, method, p1, p2, p3, p4, p5, p6), type, time, repeats, flags);

		// add this to the object's list, updating will all be done later on...
		obj->event_AddEvent(event);
	}

	template <class Class, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
	void AddEvent(Class *obj, void (Class::*method)(P1,P2,P3,P4,P5,P6,P7), P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, unsigned int type, DWORD time, DWORD repeats, unsigned int flags)
	{
		// create a timed event
		TimedEvent * event = new TimedEvent(obj, new CallbackP7<Class, P1, P2, P3, P4, P5, P6, P7>(obj, method, p1, p2, p3, p4, p5, p6, p7), type, time, repeats, flags);

		// add this to the object's list, updating will all be done later on...
		obj->event_AddEvent(event);
	}

	template <class Class, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
	void AddEvent(Class *obj, void (Class::*method)(P1,P2,P3,P4,P5,P6,P7, P8), P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, unsigned int type, DWORD time, DWORD repeats, unsigned int flags)
	{
		// create a timed event
		TimedEvent * event = new TimedEvent(obj, new CallbackP8<Class, P1, P2, P3, P4, P5, P6, P7, P8>(obj, method, p1, p2, p3, p4, p5, p6, p7, p8), type, time, repeats, flags);

		// add this to the object's list, updating will all be done later on...
		obj->event_AddEvent(event);
	}


	template <class Class, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
	void AddEvent(Class *obj, void (Class::*method)(P1,P2,P3,P4,P5,P6,P7,P8, P9), P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, unsigned int type, DWORD time, DWORD repeats, unsigned int flags)
	{
		// create a timed event
		TimedEvent * event = new TimedEvent(obj, new CallbackP9<Class, P1, P2, P3, P4, P5, P6, P7, P8, P9>(obj, method, p1, p2, p3, p4, p5, p6, p7, p8, p9), type, time, repeats, flags);

		// add this to the object's list, updating will all be done later on...
		obj->event_AddEvent(event);
	}

	template <class Class, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10>
	void AddEvent(Class *obj, void (Class::*method)(P1,P2,P3,P4,P5,P6,P7,P8, P9, P10), P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, unsigned int type, DWORD time, DWORD repeats, unsigned int flags)
	{
		// create a timed event
		TimedEvent * event = new TimedEvent(obj, new CallbackP10<Class, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10>(obj, method, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10), type, time, repeats, flags);

		// add this to the object's list, updating will all be done later on...
		obj->event_AddEvent(event);
	}
	template <class Class> void RemoveEvents(Class *obj) { obj->event_RemoveEvents(-1); }
	template <class Class> void RemoveEvents(Class *obj, signed __int32 type)
	{
		obj->event_RemoveEvents(type);
	}

	template <class Class> void ModifyEventTimeLeft(Class *obj, unsigned int type, DWORD time, bool unconditioned=true)
	{
		obj->event_ModifyTimeLeft(type, time,unconditioned);
	}

	template <class Class> void ModifyEventTimeAndTimeLeft(Class *obj, unsigned int type, DWORD time)
	{
		obj->event_ModifyTimeAndTimeLeft(type, time);
	}

	template <class Class> void ModifyEventTime(Class *obj, unsigned int type, DWORD time)
	{
		obj->event_ModifyTime(type, time);
	}

	template <class Class> bool HasEvent(Class *obj, unsigned int type)
	{
		return obj->event_HasEvent(type);
	}

	template <class Class> DWORD GetEventTimeLeft(Class *obj, unsigned int type)
	{
		return obj->event_GetTimeLeft(type);
	}
	template <class Class> DWORD GetEventRepeatLeft(Class *obj, unsigned int type)
	{
		return obj->event_GetRepeatLeft(type);
	}

	EventableObjectHolder * GetEventHolder(signed __int32 InstanceId)
	{
		HolderMap::iterator itr = mHolders.find(InstanceId);
		if(itr == mHolders.end()) return 0;
		return itr->second;
	}

	void AddEventHolder(EventableObjectHolder * holder, signed __int32 InstanceId)
	{
		mHolders.insert( HolderMap::value_type( InstanceId, holder) );
	}

	void RemoveEventHolder(signed __int32 InstanceId)
	{
		mHolders.erase(InstanceId);
	}

	void RemoveEventHolder(EventableObjectHolder * holder)
	{
		HolderMap::iterator itr = mHolders.begin();
		for(; itr != mHolders.end(); ++itr)
		{
			if(itr->second == holder)
			{
				mHolders.erase(itr);
				return;
			}
		}
	}

protected:
	HolderMap mHolders;
};

#define GetEventMgr()			EventMgr::GetInstance()
#define g_pEventMgr				GetEventMgr()


#endif

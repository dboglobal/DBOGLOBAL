#include "stdafx.h"
#include "EventableObject.h"


EventableObject::~EventableObject()
{
	/* decrement event count on all events */

	//if (m_events.size() == 0)
	//	return;

	//EventMap::iterator itr = m_events.begin();
	//for(; itr != m_events.end(); ++itr)
	//{
	//	itr->second->deleted = true;
	//	itr->second->DecRef();
	//}

	//m_events.clear();
}

EventableObject::EventableObject()
{
	/* commented, these will be allocated when the first event is added. */

	m_holder = 0;
	m_event_Instanceid = -1;
}

void EventableObject::event_AddEvent(TimedEvent * ptr)
{
	if(!m_holder)
	{
		m_event_Instanceid = event_GetInstanceID();
		m_holder = g_pEventMgr->GetEventHolder(m_event_Instanceid);
	}

	ptr->IncRef();
	ptr->instanceId = m_event_Instanceid;
	std::pair<unsigned int,TimedEvent*> p(ptr->eventType, ptr);
	m_events.insert( p );

	/* Add to event manager */
	if(!m_holder)
	{
		/* relocate to -1 eventholder :/ */
		m_event_Instanceid = -1;
		m_holder = g_pEventMgr->GetEventHolder(m_event_Instanceid);
		NTL_ASSERT(m_holder);
	}

	m_holder->AddEvent(ptr);
}

void EventableObject::event_RemoveByPointer(TimedEvent * ev)
{
	EventMap::iterator itr = m_events.find(ev->eventType);
	EventMap::iterator it2;
	if(itr != m_events.end())
	{
		do 
		{
			it2 = itr++;

			if(it2->second == ev)
			{
				it2->second->deleted = true;
			//	it2->second->DecRef();
			//	m_events.erase(it2);
				return;
			}

		} while(itr != m_events.upper_bound(ev->eventType));
	}
}

void EventableObject::event_RemoveEvents(unsigned int EventType)
{
	if(m_events.size() == NULL)
	{
		return;
	}

	if(EventType == EVENT_REMOVAL_FLAG_ALL)
	{
		EventMap::iterator itr = m_events.begin();
		for(; itr != m_events.end(); ++itr)
		{
			itr->second->deleted = true;
			//itr->second->DecRef();
		}
		//m_events.clear();
	}
	else
	{
		EventMap::iterator itr = m_events.find(EventType);
		EventMap::iterator it2;
		if(itr != m_events.end())
		{
			do 
			{
				it2 = itr++;

				it2->second->deleted = true;
		//		it2->second->DecRef();
		//		m_events.erase(it2);

			} while(itr != m_events.upper_bound(EventType));
		}
	}

}

void EventableObject::event_RemoveEvents()
{
	event_RemoveEvents(EVENT_REMOVAL_FLAG_ALL);
}

void EventableObject::event_ModifyTimeLeft(unsigned int EventType, DWORD TimeLeft,bool unconditioned)
{
	if(!m_events.size())
	{
		return;
	}

	EventMap::iterator itr = m_events.find(EventType);
	if(itr != m_events.end())
	{
		do 
		{
			if(unconditioned)
				itr->second->currTime = TimeLeft;
			else itr->second->currTime = ((signed __int32)TimeLeft > itr->second->msTime) ? itr->second->msTime : (signed __int32)TimeLeft;
			++itr;
		} while(itr != m_events.upper_bound(EventType));
	}
}

bool EventableObject::event_GetTimeLeft(unsigned int EventType, unsigned int * Time)
{
	if(!m_events.size())
	{
		return false;
	}

	EventMap::iterator itr = m_events.find(EventType);
	if(itr != m_events.end())
	{
		do 
		{
			if( itr->second->deleted )
			{
				++itr;
				continue;
			}

			*Time = (unsigned int)itr->second->currTime;
			return true;

		} while(itr != m_events.upper_bound(EventType));
	}

	return false;
}

void EventableObject::event_ModifyTime(unsigned int EventType, DWORD Time)
{
	if(!m_events.size())
	{
		return;
	}

	EventMap::iterator itr = m_events.find(EventType);
	if(itr != m_events.end())
	{
		do 
		{
			itr->second->msTime = Time;
			++itr;
		} while(itr != m_events.upper_bound(EventType));
	}
}

void EventableObject::event_ModifyTimeAndTimeLeft(unsigned int EventType, DWORD Time)
{
	if(!m_events.size())
	{
		return;
	}

	EventMap::iterator itr = m_events.find(EventType);
	if(itr != m_events.end())
	{
		do 
		{
			itr->second->currTime = itr->second->msTime = Time;
			++itr;
		} while(itr != m_events.upper_bound(EventType));
	}

}


bool EventableObject::event_HasEvent(unsigned int EventType)
{
	bool ret = false;
	if(!m_events.size())
	{
		return false;
	}

	//ret = m_events.find(EventType) == m_events.end() ? false : true;
	EventMap::iterator itr = m_events.find(EventType);
	if(itr != m_events.end())
	{
		do 
		{
			if(!itr->second->deleted)
			{
				ret = true;
				break;
			}
			++itr;
		} while(itr != m_events.upper_bound(EventType));
	}

	return ret;
}

DWORD EventableObject::event_GetTimeLeft(unsigned int EventType)
{
	if (!m_events.size())
	{
		return 0;
	}

	EventMap::iterator itr = m_events.find(EventType);
	if (itr != m_events.end())
	{
		return itr->second->msTime;
	}

	return 0;
}

DWORD EventableObject::event_GetRepeatLeft(unsigned int EventType)
{
	if (!m_events.size())
	{
		return 0;
	}

	EventMap::iterator itr = m_events.find(EventType);
	if (itr != m_events.end())
	{
		return itr->second->repeats;
	}

	return 0;
}

EventableObjectHolder::EventableObjectHolder(int instance_id) : m_InstanceId(instance_id)
{
	g_pEventMgr->AddEventHolder(this, instance_id);
}

EventableObjectHolder::~EventableObjectHolder()
{
	g_pEventMgr->RemoveEventHolder(this);

	/* decrement events reference count */
	EventList::iterator itr = m_events.begin();
	for(; itr != m_events.end(); ++itr)
		(*itr)->DecRef();
}

void EventableObjectHolder::Update(DWORD time_difference)
{

	/* Insert any pending objects in the insert pool. */
	//InsertableQueue::iterator iqi;
	//InsertableQueue::iterator iq2 = m_insertPool.begin();
	//while(iq2 != m_insertPool.end())
	//{
	//	iqi = iq2++;
	//	if((*iqi)->deleted || (*iqi)->instanceId != mInstanceId)
	//		(*iqi)->DecRef();
	//	else
	//		m_events.push_back( (*iqi) );

	//	m_insertPool.erase(iqi);
	//}

	/* Now we can proceed normally. */
	EventList::iterator itr = m_events.begin();
	EventList::iterator it2;
	TimedEvent * ev;

	while(itr != m_events.end())
	{
		it2 = itr++;

		// Event Update Procedure
		ev = *it2;

		if (ev->deleted) //check if event has to be deleted
		{
			// remove from this list.
			ev->DecRef();
			m_events.erase(it2);
			continue;
		}

		if(ev->currTime <= time_difference)
		{
			ev->cb->execute(); //execute event

			if (ev->deleted == false)//check if event has been deleted inside execution
			{
				if (ev->eventFlag & EVENT_FLAG_DELETES_OBJECT) // if event has to be deleted after first execution
				{
					ev->deleted = true;
					continue;
				}

				//reset the timer
				ev->currTime = ev->msTime;

				if (ev->repeats != 0xFFFFFFFF) //check if infinite event
				{
					if (--ev->repeats == 0) //decrease repeat count. And check if 0
					{
						ev->deleted = true;
					}
				}
			}
		}
		else
		{
			// event is still "waiting", subtract time difference
			ev->currTime -= time_difference;
		}
	}

}

void EventableObject::event_Relocate()
{
	/* prevent any new stuff from getting added */

	EventableObjectHolder * nh = g_pEventMgr->GetEventHolder(event_GetInstanceID());
	if(nh != m_holder)
	{
		// whee, we changed event holder :>
		// doing this will change the instanceid on all the events, as well as add to the new holder.
		
		// no need to do this if we don't have any events, though.
		if(!nh)
			nh = g_pEventMgr->GetEventHolder(-1);

		nh->AddObject(this);

		// reset our m_holder pointer and instance id
		m_event_Instanceid = nh->GetInstanceID();
		m_holder = nh;
	}

	/* safe again to add */
}

unsigned int EventableObject::event_GetEventPeriod(unsigned int EventType)
{
	unsigned int ret = 0;
	EventMap::iterator itr = m_events.find(EventType);
	if(itr != m_events.end())
		ret = (unsigned int)itr->second->msTime;
	
	return ret;
}

void EventableObjectHolder::AddEvent(TimedEvent * ev)
{
	// m_lock NEEDS TO BE A RECURSIVE MUTEX
	ev->IncRef();
// 	if(!m_lock.AttemptAcquire())
// 	{
// 		m_insertPool.push_back( ev );
// 	}
// 	else
	{
		m_events.push_back( ev );
	}
}

void EventableObjectHolder::AddObject(EventableObject * obj)
{
	UNREFERENCED_PARAMETER(obj);
}

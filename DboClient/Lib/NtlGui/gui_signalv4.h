#ifndef __GUI_SIGNALV4_H__
#define __GUI_SIGNALV4_H__

#include "gui_define.h"
#include "gui_slotv4.h"
#include <list>


START_GUI

template <class PARAM1, class PARAM2, class PARAM3, class PARAM4>
//: Signal V4 class
class CSignal_v4
{
public:
	typedef CSlot_v4<PARAM1, PARAM2, PARAM3, PARAM4> *Slot;
	typedef typename std::list<Slot>::iterator SlotIterator;

//! Construction:
public:
	//: Signal V4 constructor
	CSignal_v4()
	{
	}

	//: Signal V4 destructor
	~CSignal_v4()
	{
		for (std::list<CSlot_v4<PARAM1, PARAM2, PARAM3, PARAM4>*>::iterator slot_it = m_Slots.begin(); slot_it != m_Slots.end(); slot_it++)
		{
			Slot slot = *slot_it;
			slot->ReleaseSignalRef();
		}
	}

//! Operations:
public:
	//: Opererator
	VOID operator() (PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4)
	{
		Call(param1, param2, param3, param4);
	}

	//: Call
	VOID Call(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4)
	{
		std::list<std::list<CSlot_v4<PARAM1, PARAM2, PARAM3, PARAM4>*>::iterator> RemoveSlots;

		// call slots connected to signal:
		for (std::list<CSlot_v4<PARAM1, PARAM2, PARAM3, PARAM4>*>::iterator slot_it = m_Slots.begin(); slot_it != m_Slots.end(); slot_it++)
		{
			Slot slot = *slot_it;

			// skip slot if it has no more references left in application.
			// (make it pending for removal)
			if (slot->GetSlotRef() == 0)
			{
				RemoveSlots.push_back(slot_it);
				continue;
			}
			
			slot->Call(param1, param2, param3, param4);
		}

		// remove all slots no longer connected to any CL_Slot.
		std::list<std::list<CSlot_v4<PARAM1, PARAM2, PARAM3, PARAM4>*>::iterator>::iterator RemoveIt;
		for (RemoveIt = RemoveSlots.begin(); RemoveIt != RemoveSlots.end(); RemoveIt++)
		{
			Slot slot = **RemoveIt;
			slot->ReleaseSignalRef();
			m_Slots.erase(*RemoveIt);
		}
	}

	//: Connect slot.
	CSlot Connect(Slot slot)
	{
		slot->AddSignalRef();
		m_Slots.push_back(slot);
		return CSlot(slot);
	}

	//: Connect callback function slot.
	CSlot Connect(VOID (*callback)(PARAM1, PARAM2, PARAM3, PARAM4))
	{
		return connect(NTL_NEW CFunctionSlot_v4<PARAM1,PARAM2,PARAM3,PARAM4>(callback));
	}

	//: Connect member function slot.
	template <class CallbackClass>
	CSlot Connect(
		CallbackClass *cbclass,
		VOID (CallbackClass::*callback)(PARAM1, PARAM2, PARAM3, PARAM4))
	{
		return Connect(NTL_NEW CMethodSlot_v4<CallbackClass, PARAM1, PARAM2, PARAM3, PARAM4>(cbclass, callback));
	}

	//: Connect member function with user data slot.
	template <class CallbackClass, class UserData>
	CSlot Connect(
		CallbackClass *cbclass,
		VOID (CallbackClass::*callback)(PARAM1, PARAM2, PARAM3, PARAM4, UserData),
		UserData userdata)
	{
		return Connect(NTL_NEW CUserDataMethodSlot_v4<CallbackClass, PARAM1, PARAM2, PARAM3, PARAM4, UserData>(cbclass, callback, userdata));
	}

	//: Disconnect
	VOID Disconnect(CSlot &disconnectslot)
	{
		for (SlotIterator slot_it = m_Slots.begin(); slot_it != m_Slots.end();)
		{
			Slot slot = *slot_it;
			if (disconnectslot.m_pImpl == slot)
			{
				slot->ReleaseSignalRef();
				slot_it = m_Slots.erase(slot_it);
			}
			else
				slot_it++;
		}
	}

//! Implementation:
private:
	std::list<Slot> m_Slots;
};

END_GUI

#endif

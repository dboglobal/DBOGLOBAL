#ifndef __GUI_SIGNALV5_H__
#define __GUI_SIGNALV5_H__

#include "gui_define.h"
#include "gui_slotv5.h"
#include <list>

START_GUI

template <class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5>
//: Signal V5 Class
class CSignal_v5
{
public:
	typedef CSlot_v5<PARAM1, PARAM2, PARAM3, PARAM4, PARAM5> *Slot;
	typedef typename std::list<Slot>::iterator SlotIterator;

//! Construction:
public:
	//: Signal V5 Constructor
	CSignal_v5()
	{
	}

	//: Signal V5 Destructor
	~CSignal_v5()
	{
		for (SlotIterator slot_it = m_Slots.begin(); slot_it != m_Slots.end(); slot_it++)
		{
			Slot slot = *slot_it;
			slot->ReleaseSignalRef();
		}
	}

//! Operations:
public:
	//: Operator
	VOID operator() (PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5)
	{
		Call(param1, param2, param3, param4, param5);
	}

	//: Call
	VOID Call(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5)
	{
		std::list<SlotIterator> RemoveSlots;

		// call slots connected to signal:
		for (SlotIterator slot_it = m_Slots.begin(); slot_it != m_Slots.end(); slot_it++)
		{
			Slot slot = *slot_it;

			// skip slot if it has no more references left in application.
			// (make it pending for removal)
			if (slot->GetSlotRef() == 0)
			{
				RemoveSlots.push_back(slot_it);
				continue;
			}
			
			slot->Call(param1, param2, param3, param4, param5);
		}

		// remove all slots no longer connected to any CL_Slot.
		std::list<SlotIterator>::iterator RemoveIt;
		for (RemoveIt = RemoveSlots.begin(); RemoveIt != RemoveSlots.end(); RemoveIt++)
		{
			Slot slot = **remove_it;
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
	CSlot Connect(VOID (*callback)(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5))
	{
		return Connect(NTL_NEW CFunctionSlot_v5<PARAM1,PARAM2,PARAM3,PARAM4,PARAM5>(callback));
	}

	//: Connect member function slot.
	template <class CallbackClass>
	CSlot Connect(
		CallbackClass *cbclass,
		VOID (CallbackClass::*callback)(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5))
	{
		return Connect(NTL_NEW CMethodSlot_v5<CallbackClass, PARAM1, PARAM2, PARAM3, PARAM4, PARAM5>(cbclass, callback));
	}

	//: Connect member function with user data slot.
	template <class CallbackClass, class UserData>
	CSlot connect(
		CallbackClass *cbclass,
		VOID (CallbackClass::*callback)(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5, UserData),
		UserData userdata)
	{
		return Connect(NTL_NEW CUserDataMethodSlot_v5<CallbackClass, PARAM1, PARAM2, PARAM3, PARAM4, PARAM5, UserData>(cbclass, callback, userdata));
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

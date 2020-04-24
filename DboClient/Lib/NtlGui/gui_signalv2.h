#ifndef __GUI_SIGNALV2_H__
#define __GUI_SIGNALV2_H__

#include "gui_define.h"
#include "gui_slotv2.h"
#include <list>

START_GUI

template <class PARAM1, class PARAM2>
//: Signal V2 Class
class CSignal_v2
{
public:
	typedef CSlot_v2<PARAM1, PARAM2> *Slot;
	typedef typename std::list<Slot>::iterator SlotIterator;

//! Construction:
public:
	//: Signal V2 Constructor
	CSignal_v2()
	{
	}

	//: Signal V2 Destructor
	~CSignal_v2()
	{
		for (std::list<CSlot_v2<PARAM1, PARAM2>*>::iterator slot_it = m_Slots.begin(); slot_it != m_Slots.end(); slot_it++)
		{
			Slot slot = *slot_it;
			slot->ReleaseSignalRef();
		}
	}

//! Operations:
public:
	//: Operator
	VOID operator() (PARAM1 param1, PARAM2 param2)
	{
		Call(param1, param2);
	}

	//: Call
	VOID Call(PARAM1 param1, PARAM2 param2)
	{
		std::list<std::list<CSlot_v2<PARAM1, PARAM2>*>::iterator> RemoveSlots;

		// call slots connected to signal:
		for (std::list<CSlot_v2<PARAM1, PARAM2>*>::iterator slot_it = m_Slots.begin(); slot_it != m_Slots.end(); slot_it++)
		{
			Slot slot = *slot_it;

			// skip slot if it has no more references left in application.
			// (make it pending for removal)
			if (slot->GetSlotRef() == 0)
			{
				RemoveSlots.push_back(slot_it);
				continue;
			}
			
			slot->Call(param1, param2);
		}

		// remove all slots no longer connected to any CL_Slot.
		std::list<std::list<CSlot_v2<PARAM1, PARAM2>*>::iterator>::iterator RemoveIt;
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
	CSlot Connect(VOID (*callback)(PARAM1, PARAM2))
	{
		return Connect(NTL_NEW CFunctionSlot_v2<PARAM1, PARAM2>(callback));
	}

	//: Connect member function slot.
	template <class CallbackClass>
	CSlot Connect(
		CallbackClass *cbclass,
		VOID (CallbackClass::*callback)(PARAM1, PARAM2))
	{
		return Connect(NTL_NEW CMethodSlot_v2<CallbackClass, PARAM1, PARAM2>(cbclass, callback));
	}

	//: Connect member function with user data slot.
	template <class CallbackClass, class UserData>
	CSlot Connect(
		CallbackClass *cbclass,
		VOID (CallbackClass::*callback)(PARAM1, PARAM2, UserData),
		UserData userdata)
	{
		return Connect(NTL_NEW CUserDataMethodSlot_v2<CallbackClass, PARAM1, PARAM2, UserData>(cbclass, callback, userdata));
	}

	//: Disconnect
	VOID Disconnect(CSlot &disconnectslot)
	{
		for (std::list<CSlot_v2<PARAM1, PARAM2>*>::iterator slot_it = m_Slots.begin(); slot_it != m_Slots.end();)
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

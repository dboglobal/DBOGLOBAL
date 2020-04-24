#ifndef __GUI_SIGNALV3_H__
#define __GUI_SIGNALV3_H__

#include "gui_define.h"
#include "gui_slotv3.h"
#include <list>

START_GUI

template <class PARAM1, class PARAM2, class PARAM3>
//: Signal V3 Class
class CSignal_v3
{
public:
	typedef CSlot_v3<PARAM1, PARAM2, PARAM3> *Slot;
	typedef typename std::list<Slot>::iterator SlotIterator;

//! Construction:
public:
	//: Signal V3 Constructor
	CSignal_v3()
	{
	}

	//: Signal V3 Destructor
	~CSignal_v3()
	{
		for (std::list<CSlot_v3<PARAM1, PARAM2, PARAM3>*>::iterator slot_it = m_Slots.begin(); slot_it != m_Slots.end(); slot_it++)
		{
			Slot slot = *slot_it;
			slot->ReleaseSignalRef();
		}
	}

//! Operations:
public:
	//: Operator
	VOID operator() (PARAM1 param1, PARAM2 param2, PARAM3 param3)
	{
		Call(param1, param2, param3);
	}

	//: Call
	VOID Call(PARAM1 param1, PARAM2 param2, PARAM3 param3)
	{
		std::list<std::list<CSlot_v3<PARAM1, PARAM2, PARAM3>*>::iterator> RemoveSlots;

		// call slots connected to signal:
		for (std::list<CSlot_v3<PARAM1, PARAM2, PARAM3>*>::iterator slot_it = m_Slots.begin(); slot_it != m_Slots.end(); slot_it++)
		{
			Slot slot = *slot_it;

			// skip slot if it has no more references left in application.
			// (make it pending for removal)
			if (slot->GetSlotRef() == 0)
			{
				RemoveSlots.push_back(slot_it);
				continue;
			}
			
			slot->Call(param1, param2, param3);
		}

		// remove all slots no longer connected to any CL_Slot.
		std::list<std::list<CSlot_v3<PARAM1, PARAM2, PARAM3>*>::iterator>::iterator RemoveIt;
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
	CSlot Connect(VOID (*callback)(PARAM1, PARAM2, PARAM3))
	{
		return Connect(NTL_NEW CFunctionSlot_v3<PARAM1,PARAM2,PARAM3>(callback));
	}

	//: Connect member function slot.
	template <class CallbackClass>
	CSlot Connect(
		CallbackClass *cbclass,
		VOID (CallbackClass::*callback)(PARAM1, PARAM2, PARAM3))
	{
		return Connect(NTL_NEW CMethodSlot_v3<CallbackClass, PARAM1, PARAM2, PARAM3>(cbclass, callback));
	}

	//: Connect member function with user data slot.
	template <class CallbackClass, class UserData>
	CSlot Connect(
		CallbackClass *cbclass,
		VOID (CallbackClass::*callback)(PARAM1, PARAM2, PARAM3, UserData),
		UserData userdata)
	{
		return Connect(NTL_NEW CUserDataMethodSlot_v3<CallbackClass, PARAM1, PARAM2, PARAM3, UserData>(cbclass, callback, userdata));
	}

	//: Disconnect
	VOID Disconnect(CSlot &disconnectslot)
	{
		for (std::list<Slot>::iterator slot_it = m_Slots.begin(); slot_it != m_Slots.end();)
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

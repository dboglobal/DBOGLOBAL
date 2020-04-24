#ifndef __GUI_SIGNALV0_H__
#define __GUI_SIGNALV0_H__


#include "gui_define.h"
#include "gui_slotv0.h"
#include <list>
#include "NtlDebug.h"

START_GUI

//: CL Signal v0 class
class CSignal_v0
{
public:
	typedef CSlot_v0 *Slot;
	typedef std::list<Slot>::iterator SlotIterator;

//! Construction:
public:
	//: CL Signal v0 constructor
	CSignal_v0()
	{
	}

	//: CL Signal v0 destructor
	~CSignal_v0()
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
	VOID operator() ()
	{
		Call();
	}

	//: Call
	VOID Call()
	{
		std::list<SlotIterator> RemoveSlot;

		// call slots connected to signal:
		CSlotParent_v0 super(m_Slots.begin(), m_Slots.end());
		super();

		// find slots to be removed:
		for (SlotIterator slot_it = m_Slots.begin(); slot_it != m_Slots.end(); slot_it++)
		{
			Slot slot = *slot_it;

			// skip slot if it has no more references left in application.
			// (make it pending for removal)
			if (slot->GetSlotRef() == 0)
			{
				RemoveSlot.push_back(slot_it);
				continue;
			}
		}

		// remove all slots no longer connected to any CL_Slot.
		std::list<SlotIterator>::iterator RemoveIt;
		for (RemoveIt = RemoveSlot.begin(); RemoveIt != RemoveSlot.end(); RemoveIt++)
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
		m_Slots.push_front(slot);
		return CSlot(slot);
	}

	//: Connect callback function slot.
	CSlot Connect(VOID (*callback)())
	{
		return Connect(NTL_NEW CFunctionSlot_v0(callback, 0));
	}

	//: Connect virtual callback function slot.
	CSlot ConnectVirtual(VOID (*virtualcallback)(CSlotParent_v0 &))
	{
		return Connect(NTL_NEW CFunctionSlot_v0(0, virtualcallback));
	}

	//: Connect member function slot.
	template <class CallbackClass>
	CSlot Connect(CallbackClass *cbclass, VOID (CallbackClass::*callback)())
	{
		return Connect(NTL_NEW CMethodSlot_v0<CallbackClass>(cbclass, callback, 0));
	}

	//: Connect virtual member function slot.
	template <class CallbackClass>
	CSlot ConnectVirtual(CallbackClass *cbclass, VOID (CallbackClass::*virtualcallback)(CSlotParent_v0 &))
	{
		return Connect(NTL_NEW CMethodSlot_v0<CallbackClass>(cbclass, 0, virtualcallback));
	}

	//: Connect member function with user data slot.
	template <class CallbackClass, class UserData>
	CSlot Connect(
		CallbackClass *cbclass,
		VOID (CallbackClass::*callback)(UserData),
		UserData userdata)
	{
		return Connect(NTL_NEW CUserDataMethodSlot_v0<CallbackClass, UserData>(cbclass, callback, 0, userdata));
	}

	//: Create virtual member function with user data slot.
	template <class CallbackClass, class UserData>
	CSlot ConnectVirtual(
		CallbackClass *cbclass,
		VOID (CallbackClass::*virtualcallback)(CSlotParent_v0 &, UserData),
		UserData userdata)
	{
		return Connect(NTL_NEW CUserDataMethodSlot_v0<CallbackClass, UserData>(cbclass, 0, virtualcallback, userdata));
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

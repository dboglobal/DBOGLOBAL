#ifndef __GUI_SLOTV0_H__
#define __GUI_SLOTV0_H__

#include "gui_define.h"
#include "gui_slot_generic.h"
#include <list>

START_GUI

//: Slot V0 Class
class CSlot_v0 : public CSlot_Generic
{
public:
	//: Call
	virtual VOID Call(class CSlotParent_v0 &super)=0;
};

//: Slot Parent V0 Class
class CSlotParent_v0
{
public:
	typedef CSlot_v0 *Slot;
	typedef std::list<Slot>::iterator SlotIterator;

//! Construction:
public:
	
	//: Slot Parent V0 Constructor
	CSlotParent_v0(const SlotIterator &itPos, const SlotIterator &itEnd)
	: m_itPos(itPos), m_itEnd(itEnd)
	{
	}

//! Operations:
public:
	//: Operator
	VOID operator() ()
	{
		if (m_itPos == m_itEnd) return;

		SlotIterator next = m_itPos;
		next++;

		CSlotParent_v0 super(next, m_itEnd);
		(*m_itPos)->Call(super);
	}

//! Implementation:
private:
	SlotIterator m_itPos, m_itEnd;
};

//: FunctionSlot V0 Class
class CFunctionSlot_v0 : public CSlot_v0
{
public:
	typedef VOID (*Callback)();
	typedef VOID (*VirtualCallback)(CSlotParent_v0 &);

	//: FunctionSlot V0 Constructor
	CFunctionSlot_v0(Callback callback, VirtualCallback virtual_callback)
	: m_Callback(callback), m_VirtualCallback(virtual_callback) { return; }

	//: Call
	virtual VOID Call(CSlotParent_v0 &super)
	{
		if (m_Callback) { super(); m_Callback(); }
		else m_VirtualCallback(super);
	}

private:
	Callback m_Callback;
	VirtualCallback m_VirtualCallback;
};

template <class CallbackClass>
//: Method Slot V0 Class
class CMethodSlot_v0 : public CSlot_v0
{
public:
	typedef VOID (CallbackClass::*Callback)();
	typedef VOID (CallbackClass::*VirtualCallback)(CSlotParent_v0 &);

	//: Method Slot V0 Constructor
	CMethodSlot_v0(CallbackClass *cbClass, Callback callback, VirtualCallback virtual_callback)
	: m_CallbackClass(cbClass), m_Callback(callback), m_VirtualCallback(virtual_callback) { return; }

	//: Call
	virtual VOID Call(CSlotParent_v0 &super)
	{
		if (m_CallbackClass) 
        { super(); (m_CallbackClass->*m_Callback)(); }
		//else 
  //          (m_CallbackClass->*m_VirtualCallback)(super);
	}

private:
	CallbackClass *m_CallbackClass;
	Callback m_Callback;
	VirtualCallback m_VirtualCallback;
};

template <class CallbackClass, class UserData>
//: User Data Methane Slot V0 Class
class CUserDataMethodSlot_v0 : public CSlot_v0
{
public:
	typedef VOID (CallbackClass::*Callback)(UserData);
	typedef VOID (CallbackClass::*VirtualCallback)(CSlotParent_v0 &, UserData);

	//: User Data Methane Slot V0 Constructor
	CUserDataMethodSlot_v0(CallbackClass *cbclass, Callback callback, VirtualCallback virtual_callback, UserData user_data)
	: m_CallbackClass(cbclass), m_Callback(callback), m_VirtualCallback(virtual_callback), m_UserData(user_data) { return; }

	//: Call
	virtual VOID Call(CSlotParent_v0 &super)
	{
		if (m_Callback) { super(); (m_CallbackClass->*m_Callback)(m_UserData); }
		else (m_CallbackClass->*m_VirtualCallback)(super, m_UserData);
	}

private:

	CallbackClass *m_CallbackClass;
	Callback m_Callback;
	VirtualCallback m_VirtualCallback;
	UserData m_UserData;
};


END_GUI

#endif

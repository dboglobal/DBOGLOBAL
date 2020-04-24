#ifndef __GUI_SLOTV2_H__
#define __GUI_SLOTV2_H__

#include "gui_define.h"
#include "gui_slot_generic.h"

START_GUI

template <class PARAM1, class PARAM2>
//: Slot V2 Class
class CSlot_v2 : public CSlot_Generic
{
public:
	//: Call
	virtual VOID Call(PARAM1 param1, PARAM2 param2)=0;
};

template <class PARAM1, class PARAM2>
//: FunctionSlot V2 Class
class CFunctionSlot_v2 : public CSlot_v2<PARAM1, PARAM2>
{
public:
	typedef VOID (*Callback)(PARAM1, PARAM2);

	//: FunctionSlot V2 Constructor
	CFunctionSlot_v2(Callback callback) : m_Callback(callback) { return; }

	//: Call
	virtual VOID call(PARAM1 param1, PARAM2 param2) { m_Callback(param1, param2); }

private:
	Callback m_Callback;
};

template <class CallbackClass, class PARAM1, class PARAM2>
//: Methane Slot V2 class
class CMethodSlot_v2 : public CSlot_v2<PARAM1, PARAM2>
{
public:
	typedef VOID (CallbackClass::*Callback)(PARAM1, PARAM2);

	//: Methane Slot V2 constructor
	CMethodSlot_v2(CallbackClass *cbclass, Callback callback)
	: m_CallbackClass(cbclass), m_Callback(callback) { return; }

	//: Call
	virtual VOID Call(PARAM1 param1, PARAM2 param2) { (m_CallbackClass->*m_Callback)(param1, param2); }

private:
	CallbackClass *m_CallbackClass;
	Callback m_Callback;
};

template <class CallbackClass, class PARAM1, class PARAM2, class UserData>
//: User Data Methane Slot V2 Class
class CUserDataMethodSlot_v2 : public CSlot_v2<PARAM1, PARAM2>
{
public:
	typedef VOID (CallbackClass::*Callback)(PARAM1, PARAM2, UserData);

	//: User Data Methane Slot V2 Constructor
	CUserDataMethodSlot_v2(CallbackClass *cbclass, Callback callback, UserData userdata)
	: m_CallbackClass(cb_class), m_Callback(callback), m_UserData(userdata) { return; }

	//: Call
	virtual VOID Call(PARAM1 param1, PARAM2 param2) { (m_CallbackClass->*m_Callback)(param1, param2, m_UserData); }

private:
	CallbackClass *m_CallbackClass;
	Callback m_Callback;
	UserData m_UserData;
};

END_GUI

#endif

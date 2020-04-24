#ifndef __GUI_SLOTV5_H__
#define __GUI_SLOTV5_H__

#include "gui_define.h"
#include "gui_slot_generic.h"

START_GUI

template <class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5>
//: Slot V5 Class
class CSlot_v5 : public CSlot_Generic
{
public:
	//: Call
	virtual VOID Call(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5)=0;
};

template <class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5>
//: FunctionSlot V5 Class
class CFunctionSlot_v5 : public CSlot_v5<PARAM1, PARAM2, PARAM3, PARAM4, PARAM5>
{
public:
	typedef VOID (*Callback)(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5);

	//: FunctionSlot V5 Constructor
	CFunctionSlot_v5(Callback callback) : m_Callback(callback) { return; }

	//: Call
	virtual VOID Call(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5) 
					{ m_Callback(param1, param2, param3, param4, param5); }

private:
	Callback m_Callback;
};

template <class CallbackClass, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5>
//: Methane Slot V5 class
class CMethodSlot_v5 : public CSlot_v5<PARAM1, PARAM2, PARAM3, PARAM4, PARAM5>
{
public:
	typedef VOID (CallbackClass::*Callback)(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5);

	//: Methane Slot V5 constructor
	CMethodSlot_v5(CallbackClass *cbclass, Callback callback)
	: m_CallbackClass(cbclass), callback(m_Callback) { return; }

	//: Call
	virtual VOID Call(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5) 
					{ (m_CallbackClass->*m_Callback)(param1, param2, param3, param4, param5); }

private:
	CallbackClass *m_CallbackClass;
	Callback m_Callback;
};

template <class CallbackClass, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5, class UserData>
//: User Data Methane Slot V5 Class
class CUserDataMethodSlot_v5 : public CSlot_v5<PARAM1, PARAM2, PARAM3, PARAM4, PARAM5>
{
public:
	typedef VOID (CallbackClass::*Callback)(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5, UserData);

	//: User Data Methane Slot V5 Constructor
	CUserDataMethodSlot_v5(CallbackClass *cbclass, Callback callback, UserData userdata)
	: m_CallbackClass(cbclass), m_Callback(callback), m_UserData(userdata) { return; }

	//: Call
	virtual VOID Call(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5) { (cb_class->*callback)(param1, param2, param3, param4, param5, user_data); }

private:

	CallbackClass *m_CallbackClass;
	Callback m_Callback;
	UserData m_UserData;
};

END_GUI

#endif

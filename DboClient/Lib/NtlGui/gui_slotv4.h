#ifndef __GUI_SLOTV4_H__
#define __GUI_SLOTV4_H__

#include "gui_define.h"
#include "gui_slot_generic.h"

START_GUI

template <class PARAM1, class PARAM2, class PARAM3, class PARAM4>
//: Slot V4 Class
class CSlot_v4 : public CSlot_Generic
{
public:
	//: Call
	virtual VOID Call(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4)=0;
};

template <class PARAM1, class PARAM2, class PARAM3, class PARAM4>
//: FunctionSlot V4 Class
class CFunctionSlot_v4 : public CSlot_v4<PARAM1, PARAM2, PARAM3, PARAM4>
{
public:
	typedef VOID (*Callback)(PARAM1, PARAM2, PARAM3, PARAM4);

	//: FunctionSlot V4 Constructor
	CFunctionSlot_v4(Callback callback) : m_Callback(callback) { return; }

	//: Call
	virtual VOID call(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4) { m_Callback(param1, param2, param3, param4); }

private:
	Callback m_Callback;
};

template <class CallbackClass, class PARAM1, class PARAM2, class PARAM3, class PARAM4>
//: Methane Slot V4 class
class CMethodSlot_v4 : public CSlot_v4<PARAM1, PARAM2, PARAM3, PARAM4>
{
public:
	typedef VOID (CallbackClass::*Callback)(PARAM1, PARAM2, PARAM3, PARAM4);

	//: Methane Slot V4 constructor
	CMethodSlot_v4(CallbackClass *cbclass, Callback callback)
	: m_CallbackClass(cbclass), m_Callback(callback) { return; }

	//: Call
	virtual VOID Call(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4) { (m_CallbackClass->*m_Callback)(param1, param2, param3, param4); }

private:
	CallbackClass *m_CallbackClass;
	Callback m_Callback;
};

template <class CallbackClass, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class UserData>
//: User Data Methane Slot V4 Class
class CUserDataMethodSlot_v4 : public CSlot_v4<PARAM1, PARAM2, PARAM3, PARAM4>
{
public:
	typedef VOID (CallbackClass::*Callback)(PARAM1, PARAM2, PARAM3, PARAM4, UserData);

	//: User Data Methane Slot V4 Constructor
	CUserDataMethodSlot_v4(CallbackClass *cbclass, Callback callback, UserData userdata)
	: m_CallbackClass(cbclass), m_Callback(callback), m_UserData(userdata) { return; }

	//: Call
	virtual VOID Call(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4) { (m_CallbackClass->*m_Callback)(param1, param2, param3, param4, m_UserData); }

private:
	CallbackClass *m_CallbackClass;
	Callback m_Callback;
	UserData m_UserData;
};

END_GUI

#endif

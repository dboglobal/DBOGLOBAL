#ifndef __GUI_SLOTV3_H__
#define __GUI_SLOTV3_H__

#include "gui_define.h"
#include "gui_slot_generic.h"

START_GUI

template <class PARAM1, class PARAM2, class PARAM3>
//: Slot V3 Class
class CSlot_v3 : public CSlot_Generic
{
public:
	//: Call
	virtual VOID Call(PARAM1 param1, PARAM2 param2, PARAM3 param3)=0;
};

template <class PARAM1, class PARAM2, class PARAM3>
//: FunctionSlot V3 Class
class CFunctionSlot_v3 : public CSlot_v3<PARAM1, PARAM2, PARAM3>
{
public:
	typedef VOID (*Callback)(PARAM1, PARAM2, PARAM3);

	//: FunctionSlot V3 Constructor
	CFunctionSlot_v3(Callback callback) : m_Callback(callback) { return; }

	//: Call
	virtual VOID Call(PARAM1 param1, PARAM2 param2, PARAM3 param3) { m_Callback(param1, param2, param3); }

private:
	Callback m_Callback;
};

template <class CallbackClass, class PARAM1, class PARAM2, class PARAM3>
//: Methane Slot V3 class
class CMethodSlot_v3 : public CSlot_v3<PARAM1, PARAM2, PARAM3>
{
public:
	typedef VOID (CallbackClass::*Callback)(PARAM1, PARAM2, PARAM3);

	//: Methane Slot V3 constructor
	CMethodSlot_v3(CallbackClass *cbclass, Callback callback)
	: m_CalllbackClass(cbclass), m_Callback(callback) { return; }

	//: Call
	virtual VOID Call(PARAM1 param1, PARAM2 param2, PARAM3 param3) { (m_CalllbackClass->*m_Callback)(param1, param2, param3); }

private:
	CallbackClass *m_CalllbackClass;
	Callback m_Callback;
};

template <class CallbackClass, class PARAM1, class PARAM2, class PARAM3, class UserData>
//: User Data Methane Slot V3 Class
class CUserDataMethodSlot_v3 : public CSlot_v3<PARAM1, PARAM2, PARAM3>
{
public:
	typedef VOID (CallbackClass::*Callback)(PARAM1, PARAM2, PARAM3, UserData);

	//: User Data Methane Slot V3 Constructor
	CUserDataMethodSlot_v3(CallbackClass *cbclass, Callback callback, UserData userdata)
	: m_CallbackClass(cbclass), m_Callback(callback), m_UserData(userdata) { return; }

	//: Call
	virtual VOID Call(PARAM1 param1, PARAM2 param2, PARAM3 param3) { (m_CallbackClass->*m_Callback)(param1, param2, param3, user_data); }

private:
	CallbackClass *m_CallbackClass;
	Callback m_Callback;
	UserData m_UserData;
};

END_GUI

#endif

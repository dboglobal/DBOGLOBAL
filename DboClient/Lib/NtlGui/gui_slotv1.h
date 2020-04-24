#ifndef __GUI_SLOTV1_H__
#define __GUI_SLOTV1_H__

#include "gui_define.h"
#include "gui_slot_generic.h"

START_GUI

template <class PARAM1>
//: Slot V1 class
class CSlot_v1 : public CSlot_Generic
{
public:
	//: Call
	virtual VOID Call(PARAM1 param1)=0;
};

template <class PARAM1>
//: Funtion Slot V1 class
class CFunctionSlot_v1 : public CSlot_v1<PARAM1>
{
public:
	typedef VOID (*Callback)(PARAM1);

	//: Funtion Slot V1 constructor
	CFunctionSlot_v1(Callback callback) : m_Callback(callback) { return; }

	//: Call
	virtual VOID Call(PARAM1 param1) { m_Callback(param1); }

private:
	Callback m_Callback;
};

template <class CallbackClass, class PARAM1>
//: Method Slot V1
class CMethodSlot_v1 : public CSlot_v1<PARAM1>
{
public:
	typedef VOID (CallbackClass::*Callback)(PARAM1);

	//: Method Slot V1 constructor
	CMethodSlot_v1(CallbackClass *cbclass, Callback callback)
	: m_CallbackClass(cbclass), m_Callback(callback) { return; }

	//: Call
	virtual VOID Call(PARAM1 param1) { (m_CallbackClass->*m_Callback)(param1); }

private:
	CallbackClass *m_CallbackClass;
	Callback m_Callback;
};

template <class CallbackClass, class PARAM1, class UserData>
//: User Data Method Slot V1 class
class CUserDataMethodSlot_v1 : public CSlot_v1<PARAM1>
{
public:
	typedef VOID (CallbackClass::*Callback)(PARAM1, UserData);

	//: User Data Method Slot V1 constructor
	CUserDataMethodSlot_v1(CallbackClass *cbclass, Callback callback, UserData userdata)
	: m_CallbackClass(cbclass), m_Callback(callback), m_UserData(userdata) { return; }

	//: Call
	virtual VOID Call(PARAM1 param1) { (m_CallbackClass->*m_Callback)(param1, m_UserData); }

private:
	CallbackClass *m_CallbackClass;
	Callback m_Callback;
	UserData m_UserData;
};

END_GUI

#endif

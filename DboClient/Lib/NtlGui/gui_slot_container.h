
#ifndef __GUI_SLOT_CONTAINER_H__
#define __GUI_SLOT_CONTAINER_H__

#include "gui_define.h"
#include "gui_slot.h"
#include "gui_signalv0.h"
#include "gui_signalv1.h"
#include "gui_signalv2.h"
#include "gui_signalv3.h"
#include "gui_signalv4.h"
#include "gui_signalv5.h"
#include <list>

// Convience signal/slot connector class.
// The slot container class is a convience class. It simplifies the
// connection between signals and slots by keeping all connections made
// until the entire class is destroyed. This saves the developer from having
// to keep a CL_Slot reference around for each connection needed.
// In order to use this class, simply inherit from it and call
// connect_signal() for each function you want hooked up.
// See the login_view.cpp/h in the CTalk example for an example of how to use
// this class.

START_GUI

class CSlotContainer
{
public:
	// Connects a slot to a CL_Signal_v0 signal.
	template<class SigClass, class Class>
	VOID Connect(SigClass &sig, Class *self, VOID(Class::*func)())
	{
		m_Slots.push_back(sig.Connect(self, func));
	}

	// Connects a slot to a CL_Signal_v0 signal with an user data parameter
	// passed along.
	template<class SigClass, class Class, class UserData>
	VOID connect(SigClass &sig, Class *self, VOID(Class::*func)(UserData), UserData userdata)
	{
		m_Slots.push_back(sig.Connect(self, func, userdata));
	}

	// Connects a slot to a CL_Signal_v1 signal.
	template<class SigClass, class Class, class Param1>
	VOID connect(SigClass &sig, Class *self, VOID(Class::*func)(Param1))
	{
		m_Slots.push_back(sig.Connect(self, func));
	}

	// Connects a slot to a CL_Signal_v1 signal with an user data parameter
	// passed along.
	template<class SigClass, class Class, class Param1, class UserData>
	VOID connect(SigClass &sig, Class *self, VOID(Class::*func)(Param1, UserData), UserData userdata)
	{
		m_Slots.push_back(sig.Connect(self, func, userdata));
	}

	// Connects a slot to a CL_Signal_v2 signal.
	template<class SigClass, class Class, class Param1, class Param2>
	VOID connect(SigClass &sig, Class *self, VOID(Class::*func)(Param1, Param2))
	{
		m_Slots.push_back(sig.Connect(self, func));
	}

	// Connects a slot to a CL_Signal_v2 signal with an user data parameter
	// passed along.
	template<class SigClass, class Class, class Param1, class Param2, class UserData>
	VOID connect(SigClass &sig, Class *self, VOID(Class::*func)(Param1, Param2, UserData), UserData userdata)
	{
		m_Slots.push_back(sig.Connect(self, func, userdata));
	}

	// Connects a slot to a CL_Signal_v3 signal.
	template<class SigClass, class Class, class Param1, class Param2, class Param3>
	VOID connect(SigClass &sig, Class *self, VOID(Class::*func)(Param1, Param2, Param3))
	{
		m_Slots.push_back(sig.Connect(self, func));
	}

	// Connects a slot to a CL_Signal_v3 signal with an user data parameter
	// passed along.
	template<class SigClass, class Class, class Param1, class Param2, class Param3, class UserData>
	VOID connect(SigClass &sig, Class *self, VOID(Class::*func)(Param1, Param2, Param3, UserData), UserData userdata)
	{
		m_Slots.push_back(sig.Connect(self, func, userdata));
	}

	// Connects a slot to a CL_Signal_v4 signal.
	template<class SigClass, class Class, class Param1, class Param2, class Param3, class Param4>
	VOID connect(SigClass &sig, Class *self, VOID(Class::*func)(Param1, Param2, Param3, Param4))
	{
		m_Slots.push_back(sig.Connect(self, func));
	}

	// Connects a slot to a CL_Signal_v4 signal with an user data parameter
	// passed along.
	template<class SigClass, class Class, class Param1, class Param2, class Param3, class Param4, class UserData>
	VOID connect(SigClass &sig, Class *self, VOID(Class::*func)(Param1, Param2, Param3, Param4, UserData userdata), UserData user_data)
	{
		m_Slots.push_back(sig.Connect(self, func, user_data));
	}

	// Connects a slot to a CL_Signal_v5 signal.
	template<class SigClass, class Class, class Param1, class Param2, class Param3, class Param4, class Param5>
	VOID connect(SigClass &sig, Class *self, VOID(Class::*func)(Param1, Param2, Param3, Param4, Param5))
	{
		m_Slots.push_back(sig.Connect(self, func));
	}

	// Connects a slot to a CL_Signal_v5 signal with an user data parameter
	// passed along.
	template<class SigClass, class Class, class Param1, class Param2, class Param3, class Param4, class Param5, class UserData>
	VOID connect(SigClass &sig, Class *self, VOID(Class::*func)(Param1, Param2, Param3, Param4, Param5, UserData userdata), UserData user_data)
	{
		m_Slots.push_back(sig.Connect(self, func, userdata));
	}

private:
	std::list<CSlot> m_Slots;
};

END_GUI

#endif

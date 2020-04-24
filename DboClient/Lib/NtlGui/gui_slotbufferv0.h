#ifndef __GUI_SLOTBUFFERV0_H__
#define __GUI_SLOTBUFFERV0_H__

#include "gui_define.h"

START_GUI

//: Slot Buffer V0 Class
class CSlotBuffer_v0
{
//! Construction:
public:
	//: Slot Buffer V0 Constructor
	CSlotBuffer_v0() : m_nCount(0)
	{
	}

	//: Slot Buffer V0 Constructor
	CSlotBuffer_v0(CSignal_v0 &signal) : m_nCount(0)
	{
		Connect(signal);
	}

//! Attributes:
public:
	//: Bool
	operator bool()
	{
		return m_nCount > 0;
	}

	//: Size
	int Size()
	{
		return m_nCount;
	}

//! Operations:
public:
	//: Connect
	VOID Connect(CSignal_v0 &signal)
	{
		m_Slot = signal.Connect(this, &CSlotBuffer_v0::callback);
	}

	//: Disconnect
	VOID Disconnect(CSignal_v0 &signal)
	{
		signal.Disconnect(m_Slot);
	}

//! Implementation:
private:
	VOID callback()
	{
		m_nCount++;
	}

	CSlot m_Slot;
	int m_nCount;
};

END_GUI

#endif

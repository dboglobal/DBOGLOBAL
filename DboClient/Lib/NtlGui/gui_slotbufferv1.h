#ifndef __GUI_SLOTBUFFERV1_H__
#define __GUI_SLOTBUFFERV1_H__

#include "gui_define.h"
#include <vector>

START_GUI

template <class PARAM1>
//: SlotBuffer V1 Class
class CSlotBuffer_v1
{
public:
	struct Params
	{
		PARAM1 p1;
	};

//! Construction:
public:
	//: Slot Buffer V1 Constructor
	CSlotBuffer_v1()
	{
	}

	//: Slot Buffer V1 Constructor
	CSlotBuffer_v1(CSignal_v1<PARAM1> &signal)
	{
		Connect(signal);
	}

//! Attributes:
public:
	//: Bool
	operator bool()
	{
		return !m_stlParams.empty();
	}

	//: Size
	int Size()
	{
		return m_stlParams.size();
	}

	//: Operator
	Params &operator[](int index)
	{
		return m_stlParams[index];
	}

//! Operations:
public:
	//: Connect
	VOID Connect(CSignal_v1<PARAM1> &signal)
	{
		m_Slot = signal.Connect(this, &CSlotBuffer_v1::callback);
	}

	//: Disconnect
	VOID Disconnect(CSignal_v1<PARAM1> &signal)
	{
		signal.disconnect(slot);
	}

//! Implementation:
private:
	VOID callback(PARAM1 p1)
	{
		Params p;
		p.p1 = p1;

		m_stlParams.push_back(p);
	}

	std::vector<Params> m_stlParams;
	CSlot m_Slot;
};

END_GUI

#endif

#ifndef __GUI_SLOT_H__
#define __GUI_SLOT_H__

#include "gui_define.h"
#include "gui_slot_generic.h"

START_GUI

//: Slot class
class CSlot
{
//! Construction:
public:
	//: Create an unconnected slot.
	CSlot()
	: m_pImpl(NULL)
	{
	}

	//: Copy a slot.
	CSlot(const CSlot &copy)
	: m_pImpl(copy.m_pImpl)
	{
		if (m_pImpl != NULL) m_pImpl->AddSlotRef();
	}

	//: Slot destructor
	~CSlot()
	{
		if (m_pImpl != NULL) m_pImpl->ReleaseSlotRef();
	}

//! Operators:
	//: Slot copy operator
	VOID operator =(const CSlot &copy)
	{
		if (m_pImpl) m_pImpl->ReleaseSlotRef();
		m_pImpl = copy.m_pImpl;
		if (m_pImpl) m_pImpl->AddSlotRef();
	}

//! Implementation:
public:
	//: Slot constructor
	CSlot(CSlot_Generic *impl)
	: m_pImpl(impl)
	{
		if (m_pImpl != NULL) m_pImpl->AddSlotRef();
	}

	//: Slot Generic Implementation
	CSlot_Generic *m_pImpl;
};

END_GUI

#endif

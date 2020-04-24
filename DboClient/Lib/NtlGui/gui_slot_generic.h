#ifndef __GUI_SLOT_GENERIC_H__
#define __GUI_SLOT_GENERIC_H__

#include "gui_define.h"
#include "NtlDebug.h"

START_GUI

//: Slot Generic Class
class CSlot_Generic
{
//! Construction:
public:
	//: Slot Generic Constructor
	CSlot_Generic() : m_nSlotRefCount(0), m_nSignalRefCount(0) { return; }

	//: Slot Generic Destructor
	virtual ~CSlot_Generic() { return; }

//! Attributes:
public:
	//: Get slot ref
	int GetSlotRef(VOID) const { return m_nSlotRefCount; }

	//: Get signal ref
	int GetSignalRef(VOID) const { return m_nSignalRefCount; }

//! Operations:
public:
	//: Add slot ref
	VOID AddSlotRef(VOID) { m_nSlotRefCount++; }

	//: Release slot ref
	VOID ReleaseSlotRef(VOID) { m_nSlotRefCount--; CheckDelete(); }

	//: Add signal ref
	VOID AddSignalRef(VOID) { m_nSignalRefCount++; }

	//: Release signal ref
	VOID ReleaseSignalRef(VOID) { m_nSignalRefCount--; CheckDelete(); }

//! Implementation:
private:

	VOID CheckDelete(VOID)
	{
		if (m_nSlotRefCount == 0 && m_nSignalRefCount == 0)
		{
			CSlot_Generic* pThis = this;
			NTL_DELETE( pThis );
		}
	}

	int m_nSlotRefCount;

	int m_nSignalRefCount;
};

END_GUI

#endif

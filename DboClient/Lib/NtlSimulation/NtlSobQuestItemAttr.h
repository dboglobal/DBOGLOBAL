/*****************************************************************************
 *
 * File			: NtlSobQuestItemAttr.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2006. 11. 08	
 * Abstract		: Simulation object quest item attribute base class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_SOBQUESTITEM_ATTR_H__
#define __NTL_SOBQUESTITEM_ATTR_H__

#include "NtlSobAttr.h"

class CNtlSobQuestItemAttr : public CNtlSobAttr
{
	DECLEAR_MEMORY_POOL(CNtlSobQuestItemAttr, NTL_DEFAULT_MEMORY_POOL)
	
private:

	sQUESTITEM_TBLDAT *m_pQuestItemTbl;
	RwUInt8 m_byStackNum;
		
public:

	CNtlSobQuestItemAttr() 
	{ 
		m_pQuestItemTbl = NULL;
		m_byStackNum = 0;
	}

	virtual ~CNtlSobQuestItemAttr() {}
	virtual RwBool Create(void) { SetClassID(SLCLASS_SLOT_QUESTITEM); return TRUE; }
	virtual void Destroy(void) {}
	virtual void HandleEvents(RWS::CMsg &pMsg);

public:

	void SetQuestItemTbl(const sQUESTITEM_TBLDAT *pQuestItemTbl);
	sQUESTITEM_TBLDAT* GetQuestItemTbl(void) const;
	
	void SetStackNum(RwUInt8 byStackNum);
	RwUInt8 GetStackNum(void) const;
};


inline void CNtlSobQuestItemAttr::SetQuestItemTbl(const sQUESTITEM_TBLDAT *pQuestItemTbl)
{
	m_pQuestItemTbl = const_cast<sQUESTITEM_TBLDAT*>(pQuestItemTbl);
}

inline sQUESTITEM_TBLDAT* CNtlSobQuestItemAttr::GetQuestItemTbl(void) const
{
	return m_pQuestItemTbl;
}

inline void CNtlSobQuestItemAttr::SetStackNum(RwUInt8 byStackNum)
{
	m_byStackNum = byStackNum;
}

inline RwUInt8 CNtlSobQuestItemAttr::GetStackNum(void) const
{
	return m_byStackNum;
}

#endif

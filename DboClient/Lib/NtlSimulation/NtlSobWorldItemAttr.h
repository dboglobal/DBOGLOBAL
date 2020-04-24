/*****************************************************************************
*
* File			: CNtlSobWorldItem.h
* Author		: Peessi
* Copyright		: (аж)NTL
* Date			: 2006. 7. 12	
* Abstract		: Simulation object World item attribute base class
*****************************************************************************
* Desc         : 
*
*****************************************************************************/


#ifndef __NTL_SOB_WORLDITEM_ATTR_H__
#define __NTL_SOB_WORLDITEM_ATTR_H__

#include "NtlSobAttr.h"

struct sITEM_OPTION_SET;

class CNtlSobWorldItemAttr : public CNtlSobAttr
{
	DECLEAR_MEMORY_POOL( CNtlSobWorldItemAttr, NTL_DEFAULT_MEMORY_POOL )

public:
	enum eDROPTYPE { DROPTYPE_NONE = 0, DROPTYPE_ITEM, DROPTYPE_MONEY };

private:

	eDROPTYPE			m_eType;

	sITEM_TBLDAT*		m_pItemTbl;
	RwUInt32			m_uiRank;
	RwUInt32			m_uiGrade;

	RwBool				m_bUnidentified;
	RwUInt8				m_ucBattleAttribute;

	RwUInt32			m_uiMoneyAmount;	
	sITEM_OPTION_SET*	m_pItemOptionSet;
	
public:

	CNtlSobWorldItemAttr(VOID) 
	{ 
		m_eType = DROPTYPE_NONE;
		m_pItemTbl = NULL;
		m_uiGrade = 0xFFFFFFFF;
		m_uiRank = 0xFFFFFFFF;
		m_uiMoneyAmount = 0;
		m_bUnidentified = FALSE;
		m_pItemOptionSet = NULL;
	}

	virtual ~CNtlSobWorldItemAttr();
	virtual RwBool Create(VOID) { SetClassID(SLCLASS_WORLD_ITEM); return TRUE; }
	virtual VOID Destroy(VOID) {}
	virtual VOID HandleEvents( RWS::CMsg &msg );

public:

	VOID SetItemTbl(const sITEM_TBLDAT* pItemTbl);
	sITEM_TBLDAT* GetItemTbl(VOID) const;
	RwUInt32 GetGrade(VOID) const;
	RwUInt32 GetRank(VOID) const;

	RwUInt8	 GetBattleAttribute(VOID) const;
	sITEM_OPTION_SET*	GetItemOptionSet() const;

	VOID SetMoney(RwUInt32 uiMoney);
	RwUInt32 GetMoney(VOID) const;

	RwBool IsItem(VOID);
	RwBool IsMoney(VOID);
	RwBool IsIdentified(VOID);
	RwBool IsDragonBall(VOID); 
};


inline VOID CNtlSobWorldItemAttr::SetItemTbl(const sITEM_TBLDAT* pItemTbl)
{
	m_pItemTbl = const_cast<sITEM_TBLDAT*>(pItemTbl);
}

inline sITEM_TBLDAT* CNtlSobWorldItemAttr::GetItemTbl(VOID) const
{
	return m_pItemTbl;
}

inline RwUInt32 CNtlSobWorldItemAttr::GetGrade(VOID) const
{
	return m_uiGrade;
}

inline RwUInt32 CNtlSobWorldItemAttr::GetRank(VOID) const
{
	return m_uiRank;
}

inline RwUInt8 CNtlSobWorldItemAttr::GetBattleAttribute(VOID) const
{
	return m_ucBattleAttribute;
}

inline sITEM_OPTION_SET* CNtlSobWorldItemAttr::GetItemOptionSet() const
{
	return m_pItemOptionSet;
}

inline VOID CNtlSobWorldItemAttr::SetMoney(RwUInt32 uiMoney)
{
	m_uiMoneyAmount = uiMoney;
}

inline RwUInt32 CNtlSobWorldItemAttr::GetMoney(VOID) const
{
	return m_uiMoneyAmount;
}

inline RwBool CNtlSobWorldItemAttr::IsItem(VOID) 
{
	return ( m_eType == DROPTYPE_ITEM ) ? TRUE : FALSE;
}

inline RwBool CNtlSobWorldItemAttr::IsMoney(VOID) 
{
	return ( m_eType == DROPTYPE_MONEY ) ? TRUE : FALSE;
}

inline RwBool CNtlSobWorldItemAttr::IsIdentified(VOID)
{
	return !m_bUnidentified;
}

#endif

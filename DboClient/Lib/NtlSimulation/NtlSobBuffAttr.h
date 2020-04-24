/*****************************************************************************
*
* File			: NtlSobSkillAttr.h
* Author		: HyungSuk, Jang
* Copyright	: (аж)NTL
* Date			: 2006. 4. 24	
* Abstract		: Simulation object skill attribute base class
*****************************************************************************
* Desc         : 
*
*****************************************************************************/


#ifndef __NTL_SOBBUFF_ATTR_H__
#define __NTL_SOBBUFF_ATTR_H__

#include "NtlSobAttr.h"
#include "SkillTable.h"

class CNtlSobBuffAttr : public CNtlSobAttr
{
	DECLEAR_MEMORY_POOL(CNtlSobBuffAttr, NTL_DEFAULT_MEMORY_POOL)

public:

	BYTE					m_byBuffIndex;		// index of buff
	BYTE					m_byType;			/** item or skill */
	sSKILL_TBLDAT*			m_pBaseSkillTbl;	/** skill table data pointer */
	sSKILL_TBLDAT			m_SkillTbl;	/** skill table data effected by passive skill*/
	sITEM_TBLDAT*			m_pItemTbl;	/** skill table data pointer */
	RwBool					m_bBlessType;		/** bless or curse */

	bool					m_bActive;			// is buff active?
	sDBO_BUFF_PARAMETER		m_aBuffParameter[NTL_MAX_EFFECT_IN_SKILL];

public:

	CNtlSobBuffAttr() 
	{ 
		m_pBaseSkillTbl = NULL;
		m_pItemTbl = NULL;
		m_bBlessType = FALSE;
		m_bActive = true;
	}

	virtual ~CNtlSobBuffAttr() {}

	virtual RwBool Create(void) { SetClassID(SLCLASS_BUFF); return TRUE; }

	virtual void Destroy(void) {}

	virtual void HandleEvents(RWS::CMsg &pMsg);

public:

	void SetBuffIndex(BYTE byIndex);
	BYTE GetBuffIndex(void) const;

	void SetBuffType(BYTE byType);
	BYTE GetBuffType(void) const;

	void SetBuffActive(bool bFlag);
	bool GetBuffActive(void) const;

	void					SetBuffParameter(sDBO_BUFF_PARAMETER* pParam);
	sDBO_BUFF_PARAMETER*	GetBuffParameter(int nSlot);

	TBLIDX GetTblIdx(void);

	void SetSkillTbl(const sSKILL_TBLDAT *pSkillTbl);
	sSKILL_TBLDAT* GetSkillTbl(void);
	sSKILL_TBLDAT* GetBaseSkillTbl(void) const;

	void SetItemTbl(const sITEM_TBLDAT *pItemTbl);
	sITEM_TBLDAT* GetItemTbl(void) const;

	RwBool IsBlessType(void) const;
};

inline void CNtlSobBuffAttr::SetBuffIndex(BYTE byIndex)
{
	m_byBuffIndex = byIndex;
}

inline BYTE CNtlSobBuffAttr::GetBuffIndex(void) const
{
	return m_byBuffIndex;
}

inline void CNtlSobBuffAttr::SetBuffType(BYTE byType)
{
	m_byType = byType;
}

inline BYTE CNtlSobBuffAttr::GetBuffType(void) const
{
	return m_byType;
}

inline void CNtlSobBuffAttr::SetBuffActive(bool bFlag)
{
	m_bActive = bFlag;
}

inline bool CNtlSobBuffAttr::GetBuffActive(void) const
{
	return m_bActive;
}

inline void CNtlSobBuffAttr::SetBuffParameter(sDBO_BUFF_PARAMETER* pParam)
{
	memcpy(m_aBuffParameter, pParam, sizeof(sDBO_BUFF_PARAMETER) * NTL_MAX_EFFECT_IN_SKILL);
}

inline sDBO_BUFF_PARAMETER* CNtlSobBuffAttr::GetBuffParameter(int nSlot)
{
	return &m_aBuffParameter[nSlot];
}

inline void CNtlSobBuffAttr::SetSkillTbl(const sSKILL_TBLDAT *pSkillTbl)
{
	m_pBaseSkillTbl = const_cast<sSKILL_TBLDAT*>(pSkillTbl);
	m_SkillTbl = *pSkillTbl;
}

inline sSKILL_TBLDAT* CNtlSobBuffAttr::GetSkillTbl(void)
{
	return &m_SkillTbl;
}

inline sSKILL_TBLDAT* CNtlSobBuffAttr::GetBaseSkillTbl(void) const
{
	return m_pBaseSkillTbl;
}

inline void CNtlSobBuffAttr::SetItemTbl(const sITEM_TBLDAT *pItemTbl)
{
	m_pItemTbl = const_cast<sITEM_TBLDAT*>(pItemTbl);
}

inline sITEM_TBLDAT* CNtlSobBuffAttr::GetItemTbl(void) const
{
	return m_pItemTbl;
}

inline RwBool CNtlSobBuffAttr::IsBlessType(void) const
{
	return m_bBlessType;
}

#endif
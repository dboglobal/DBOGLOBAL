/*****************************************************************************
 *
 * File			: NtlSobItemAttr.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 11. 30	
 * Abstract		: Simulation object item attribute base class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_SOBITEM_ATTR_H__
#define __NTL_SOBITEM_ATTR_H__

#include "NtlItem.h"
#include "NtlSobAttr.h"

class CNtlSobItemAttr : public CNtlSobAttr
{
	DECLEAR_MEMORY_POOL(CNtlSobItemAttr, NTL_DEFAULT_MEMORY_POOL)
	
private:

	sITEM_TBLDAT *m_pItemTbl;
	RwUInt8 m_byStackNum;
	RwUInt8 m_byRank;		/** item 등급 */
	RwUInt8 m_byGrade;		/** item 업그레이드 등급 **/
	RwUInt8 m_byDur;		/** item 내구도 */
	RwUInt8 m_byMaxDur;		/** item max 내구도 */
	RwBool	m_bNeedToIdentify; /** item identify 필요여부 **/
	sITEM_OPTION_SET	m_sOptionSet;
	RwUInt8 m_ucBattleAttribute;

	RwUInt8	m_ucRestrictState;	/** 귀속상태 eITEM_RESTRICT_STATE_TYPE **/
	std::wstring m_wstrMaker;	/** 제작자 **/

	RwUInt8		m_ucDurationType;	/** 사용기간 eDURATIONTYPE **/
	DBOTIME		m_StartTime;		/** 인벤토리에 들어온 날짜 **/
	DBOTIME		m_EndTime;			/** 사용 만료 기간 **/
	DBOTIME		m_RemainTime;		/** 남은시간 초 **/
	RwReal		m_fRemainTimeBelowPeriod;		/** 남은시간 소수점 이하 **/
	RwReal		m_fNotifyTime;		/** 남은시간 알림간격 **/
		
public:

	CNtlSobItemAttr() 
	{ 
		m_pItemTbl = NULL;
		m_byStackNum = 1;
		m_byRank = 0;
		m_byGrade = 0;
		m_byDur = 1;
		m_byMaxDur = 1;
		m_bNeedToIdentify = FALSE;
		m_sOptionSet.Init();
		m_ucBattleAttribute = 0;

		m_ucRestrictState = ITEM_RESTRICT_STATE_TYPE_NONE;
		
		m_ucDurationType = eDURATIONTYPE_NORMAL;
		m_StartTime = 0;
		m_EndTime = 0;
		m_RemainTime = 0;
		m_fRemainTimeBelowPeriod = 0.0f;
		m_fNotifyTime = 0;
	}

	virtual ~CNtlSobItemAttr() {}
	virtual RwBool Create(void) { SetClassID(SLCLASS_SLOT_ITEM); return TRUE; }
	virtual void Destroy(void) {}
	virtual void HandleEvents(RWS::CMsg &pMsg);

	void	Update(RwReal fElapsedTime);		

public:

	void SetItemTbl(const sITEM_TBLDAT *pItemTbl);
	sITEM_TBLDAT* GetItemTbl(void) const;

	void SetStackNum(RwUInt8 byStackNum);
	RwUInt8 GetStackNum(void) const;

	void SetRank(RwUInt8 byRank);
	RwUInt8 GetRank(void) const;

	void SetGrade(RwUInt8 byGrade);
	RwUInt8 GetGrade(void) const;

	void SetDur(RwUInt8 byDur);
	RwUInt8 GetDur(void) const;

	void SetMaxDur(RwUInt8 byDur);
	RwUInt8 GetMaxDur(void) const;

	void SetOptionSet(sITEM_OPTION_SET* pSet);
	sITEM_OPTION_SET*	GetOptionSet(void);

	void SetBattleAttribute(RwUInt8 byBattleAttribute);
	RwUInt8 GetBattleAttribute(void) const;

	void SetRestrictState(RwUInt8 byRestrictState);
	RwUInt8 GetRestrictState(void) const;

	const WCHAR* GetMaker(void) const;

	void	SetDurationType(RwUInt8 byType);
	RwUInt8 GetDurationType(void) const;
	void	SetStartTime(DBOTIME time);
	DBOTIME GetStartTime(void) const;
	void	SetEndTime(DBOTIME time);
	DBOTIME GetEndTime(void) const;

	void	SetRemainTime( DBOTIME RemainTime );
	DBOTIME GetRemainTime(void) const;

	RwBool	IsExpired(void) const;
	void	Expired(void);

	RwUInt32 GetTblIdx(void) const;

	RwBool IsNeedToIdentify(void);

	RwBool IsContainerItem(void);
	RwInt32 GetChildSlotSize(void);

	RwBool IsEquipItem(void);
	RwBool IsBagItem(void);
	RwBool IsScouterItem(void);
};


inline void CNtlSobItemAttr::SetItemTbl(const sITEM_TBLDAT *pItemTbl)
{
	m_pItemTbl = const_cast<sITEM_TBLDAT*>(pItemTbl);
}

inline sITEM_TBLDAT* CNtlSobItemAttr::GetItemTbl(void) const
{
	return m_pItemTbl;
}

inline void CNtlSobItemAttr::SetStackNum(RwUInt8 byStackNum)
{
	m_byStackNum = byStackNum;
}

inline RwUInt8 CNtlSobItemAttr::GetStackNum(void) const
{
	return m_byStackNum;
}

inline void CNtlSobItemAttr::SetRank(RwUInt8 byRank)
{
	m_byRank = byRank;
}

inline RwUInt8 CNtlSobItemAttr::GetRank(void) const
{
	return m_byRank;
}

inline void CNtlSobItemAttr::SetGrade( RwUInt8 byGrade )
{
	m_byGrade = byGrade;
}

inline RwUInt8 CNtlSobItemAttr::GetGrade(void) const
{
	return m_byGrade;
}

inline void CNtlSobItemAttr::SetDur(RwUInt8 byDur)
{
	m_byDur	= byDur;
}

inline RwUInt8 CNtlSobItemAttr::GetDur(void) const
{
	return m_byDur;
}

inline void CNtlSobItemAttr::SetMaxDur(RwUInt8 byDur)
{
	m_byMaxDur = byDur;
}

inline RwUInt8 CNtlSobItemAttr::GetMaxDur(void) const
{
	return m_byMaxDur;
}

inline void CNtlSobItemAttr::SetOptionSet(sITEM_OPTION_SET* pSet)
{
	memcpy(&m_sOptionSet, pSet, sizeof(sITEM_OPTION_SET));
}

inline sITEM_OPTION_SET* CNtlSobItemAttr::GetOptionSet(void)
{
	return &m_sOptionSet;
}

inline void CNtlSobItemAttr::SetBattleAttribute(RwUInt8 byBattleAttribute)
{
	m_ucBattleAttribute = byBattleAttribute;
}

inline RwUInt8 CNtlSobItemAttr::GetBattleAttribute(void) const
{
	return m_ucBattleAttribute;
}

inline void CNtlSobItemAttr::SetRestrictState(RwUInt8 byRestrictState)
{
	m_ucRestrictState = byRestrictState;
}

inline RwUInt8 CNtlSobItemAttr::GetRestrictState(void) const
{
	return m_ucRestrictState;
}

inline const WCHAR* CNtlSobItemAttr::GetMaker(void) const
{
	return m_wstrMaker.c_str();
}

inline void CNtlSobItemAttr::SetDurationType(RwUInt8 byType)
{
	m_ucDurationType = byType;
}

inline RwUInt8 CNtlSobItemAttr::GetDurationType(void) const
{
	return m_ucDurationType;
}

inline void CNtlSobItemAttr::SetStartTime(DBOTIME time)
{
	m_StartTime = time;
}

inline DBOTIME CNtlSobItemAttr::GetStartTime(void) const
{
	return m_StartTime;
}

inline void CNtlSobItemAttr::SetEndTime(DBOTIME time)
{
	m_EndTime = time;
}

inline DBOTIME CNtlSobItemAttr::GetEndTime(void) const
{
	return m_EndTime;
}

inline void CNtlSobItemAttr::SetRemainTime( DBOTIME RemainTime )
{
	m_RemainTime = RemainTime;
	m_fRemainTimeBelowPeriod = 0.0f;
	m_fNotifyTime = 0.0f;
}

inline RwBool CNtlSobItemAttr::IsExpired(void) const
{
	if( m_ucDurationType && m_RemainTime <= 0 )
		return TRUE;

	return FALSE;
}

inline void CNtlSobItemAttr::Expired(void)
{
	SetRemainTime( 0 );
}

inline DBOTIME CNtlSobItemAttr::GetRemainTime(void) const
{
	return m_RemainTime;
}

inline RwBool CNtlSobItemAttr::IsNeedToIdentify(void)
{
	return m_bNeedToIdentify;
}


#endif

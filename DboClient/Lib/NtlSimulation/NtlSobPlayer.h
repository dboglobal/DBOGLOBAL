/*****************************************************************************
 *
 * File			: NtlSobPlayer.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 9. 01	
 * Abstract		: Simulation player object
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_SOB_PLAYER_H__
#define __NTL_SOB_PLAYER_H__

#include "NtlSharedType.h"
#include "NtlGuild.h"

// simulation
#include "NtlSobFeelingActor.h"
#include "NtlInventory.h"
#include "NtlBuffContainer.h"


class CNtlSobVehicle;
class CNtlInventory;
class CNtlBeCharData;


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlSobPlayer : public CNtlSobFeelingActor
{
	DECLEAR_MEMORY_POOL(CNtlSobPlayer, NTL_DEFAULT_MEMORY_POOL)

protected:
	CHARACTERID			m_charID;
	
	CNtlBeCharData		*m_pBeData;				/** character behavior data */
	CNtlInventoryBase	*m_pEquipInventory;		/** equip inventory */
    sASPECTSTATE        m_sAspsectState;        ///< 변신 정보 구조체

	WCHAR				m_awszGuildName[NTL_MAX_SIZE_GUILD_NAME + 1];
	sDBO_DOGI_DATA		m_sDogiData;


public:

	CNtlSobPlayer();
	virtual ~CNtlSobPlayer();

	virtual RwBool				Create(void);

	virtual void				Destroy(void);

	virtual void				ServerChangeOut(void);

	virtual void				ServerChangeIn(void);

	virtual void				Update(RwReal fElapsed);

	virtual CNtlFSMLayer*		CreateFSM(void);

	virtual void				HandleEvents(RWS::CMsg &pMsg);

public:
	virtual CHARACTERID			GetCharID();

	void						SetGuildName(WCHAR* pwcGuildName);
	const WCHAR*				GetGuildName();

	void						SetGuildDogiColorIndex(RwUInt8 byColorIndex);
	RwUInt8						GetGuildDogiColorIndex();

	void						SetDojoDogiColorIndex(RwUInt8 byColorIndex);
	RwUInt8						GetDojoDogiColorIndex();

	// 실제로 쓸 수 있는 도복의 색상을 반환한다
	RwUInt8						GetRealDogiColorIndex();

    void                        SetApsectState(const sASPECTSTATE& sAspectState) {m_sAspsectState = sAspectState;}
    sASPECTSTATE*               GetAspectState() {return &m_sAspsectState;}
	
	// control data station
	virtual void*				GetBehaviorData(void);
	CNtlInventoryBase*			GetEquipInventory(void);


	void						SetCharTitle(bool bInvisibleTitle, TBLIDX titleIdx);
};

inline CHARACTERID CNtlSobPlayer::GetCharID()
{
	return m_charID;
}

inline const WCHAR* CNtlSobPlayer::GetGuildName()
{
	return m_awszGuildName;
}

inline void CNtlSobPlayer::SetGuildDogiColorIndex(RwUInt8 byColorIndex)
{
	m_sDogiData.byGuildColor = byColorIndex;
}

inline RwUInt8 CNtlSobPlayer::GetGuildDogiColorIndex()
{
	return m_sDogiData.byGuildColor;
}

inline void CNtlSobPlayer::SetDojoDogiColorIndex(RwUInt8 byColorIndex)
{
	m_sDogiData.byDojoColor = byColorIndex;
}

inline RwUInt8 CNtlSobPlayer::GetDojoDogiColorIndex()
{
	return m_sDogiData.byDojoColor;
}

inline RwUInt8 CNtlSobPlayer::GetRealDogiColorIndex()
{
	if( INVALID_BYTE != m_sDogiData.byDojoColor )
		return m_sDogiData.byDojoColor;

	return m_sDogiData.byGuildColor;
}

inline void* CNtlSobPlayer::GetBehaviorData(void)
{
	return reinterpret_cast<void*>(m_pBeData); 
}

inline CNtlInventoryBase* CNtlSobPlayer::GetEquipInventory(void)
{
	return m_pEquipInventory;
}


#endif

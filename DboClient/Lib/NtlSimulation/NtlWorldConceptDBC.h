#pragma once

#include "NtlWorldConceptController.h"
#include "NtlSoundDefines.h"
#include "NtlDragonBall.h"

class CNtlPLCharacter;
class CNtlSob;
class CNtlSobItem;

typedef std::list<CNtlSobItem*>	ListSobItem;

/**
 * \ingroup NtlSimulation
 * \brief 용신 소환및 드래곤볼 보상 상태를 제어하는 컨트롤러
 * \date 2007-06-14
 * \author agebreak
 */
class CNtlWorldConceptDBC : public CNtlWorldConceptController
{
public:
	CNtlWorldConceptDBC(void);
	~CNtlWorldConceptDBC(void);

	virtual void		Update(RwReal fElapsedTime);				///< 각 컨셉에 맞는 내용들의 Update처리 함수
	virtual RwBool		IsEnableAction(EAvatarAction eAction);		///< Avatar의 Action이 현재 처리 가능한지를 각 컨셉에 따라 판별한다.
	virtual void		ChangeState(RwInt32 iState);				///< State 상태를 변경한다.	

    // DragonBall List	
	void				AddDragonBall(CNtlSobItem* pItem);			///< 리스트에 드래곤볼 아이템을 추가한다.
	void				RemoveDragonBall(CNtlSobItem* pItem);		///< 리스트에서 드래곤볼 아이템을 제거한다.
	ListSobItem*		GetDBList() {return &m_listDragonBall;}

    // Property
    void				SetDBKind(eDRAGON_BALL_TYPE eDBKind) {m_eDragonBallKind = eDBKind;}	///< 현재 진행중인 드래곤볼의 종류를 설정한다.
    eDRAGON_BALL_TYPE	GetDBKind() {return m_eDragonBallKind;}		                        ///< 현재 진행중인 드래곤볼의 종류를 반환한다.			

    void				SetAltarSerialID(RwUInt32 nSerialID) {m_uiAltarID = nSerialID;}
    RwUInt32			GetAltarSerialID() {return m_uiAltarID;}

    void				ResetRemainTime();							///< 남은 제한시간을 설정한다.
    RwUInt32			GetReaminTime();	                        ///< 남은 제한시간을 가져온다.

    void                SetMyDragon(RwBool bMyDragon) {m_bMyDragon = bMyDragon;}    ///< 자신이 소환한 용신인지를 설정한다.
    RwBool              IsMyDragon() {return m_bMyDragon;}                          ///< 자신용 소환한 용신인지 여부를 반환한다.

protected:	

protected:
	RwUInt32				m_uiAltarID;							///< 제단의 Serial ID
	eDRAGON_BALL_TYPE		m_eDragonBallKind;						///< 현재 진행중인 드래곤볼 연출의 종류
	RwUInt32				m_uiRemainTime;							///< 남은 제한시간
	ListSobItem				m_listDragonBall;						///< 드래곤볼 아이템들
    RwBool                  m_bMyDragon;                            ///< 자기가 소환한 용신인지를 확인하기 위한 플래그
    RwBool                  m_bUpdateRemainTime;                    ///< 제한 시간 업데이트 유무 플래그
    SOUND_HANDLE            m_hBGMHandle;                           ///< 용신 나오는 사운드의 핸들
};

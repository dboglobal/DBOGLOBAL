/*****************************************************************************
*
* File			: NtlSLCENode.h
* Author		: agebreak
* Copyright		: (주)NTL
* Date			: 2008. 7. 16	
*****************************************************************************
* Desc          : 천하제일 무도회의 관중 연출을 위한 노드 
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"
#include "NtlCoreDescription.h"


#define     CENODE_VER_1			(RwReal)1
#define     CENODE_VER_1_1			(RwReal)1.1
#define     CENODE_VER_1_2			(RwReal)1.2
#define     CENODE_VER_1_2_1		(RwReal)1.21				///< 관중 이펙트 연출을 위한 데이터의 버전
#define     CENODE_VER_LASTEST		(RwReal)1.3					///< 심판 나레이션 추가

class CNtlSerializer;
class CNtlInstanceEffect;
class CNtlPLCharacter;


/// 천하제일 무도회의 관중 연출을 위한 노드 베이스 클래스
class CNtlSLCENode
{
public:
    enum ECENodeType            /// 노드 타입
    {
        E_CE_EFFECT,        
        E_CE_SOUND,
        //E_CE_BALLOON,
        E_CE_JUDGE,

		 NUM_E_CE,

		 INVALID_E_CE
    };

    static sVERSTION      m_Version;                     ///< 하위 호환성을 위한 버전 정보

public:
    CNtlSLCENode(void);
    virtual ~CNtlSLCENode(void);

    virtual void    Create() = 0;
    virtual void    Destory() = 0;
    virtual RwBool  Update(RwReal fElapsedTime);
    virtual RwBool  Load(CNtlSerializer& s);
    virtual RwBool  Save(CNtlSerializer& s);   
	virtual void    SetPosition(const RwV3d& vPos) = 0;
	RwV3d			 GetPosition() { return m_vPos; }
	RwInt32			GetType() { return m_eType; }

    void            SetName(const std::string& strName) {m_strName = strName;}
    std::string     GetName() {return m_strName;}

	virtual void		HandleEvents(RWS::CMsg &pMsg) {}

protected:
    RwInt32                     m_eType;                    ///< 노드 타입    
    std::string                 m_strName;                  ///< 노드 이름
    
    RwReal                      m_fTotalElapsedTime;        ///< 업데이트 타임 비교 변수
    RwV3d                       m_vPos;                     ///< 해당 노드가 생성되는 위치
};

/// 관중 이펙트 노드
class CNtlSLCENodeEffect : public CNtlSLCENode
{
public:
    CNtlSLCENodeEffect();

    virtual void    Create();
    virtual void    Destory();
    virtual RwBool  Update(RwReal fElapsedTime);
    virtual RwBool  Load(CNtlSerializer& s);
    virtual RwBool  Save(CNtlSerializer& s);    
    virtual void    SetPosition(const RwV3d& vPos);

    void            SetEffectName(const std::string& strName) {m_strEffectName = strName;}
    std::string     GetEffectName() {return m_strEffectName;}
    CNtlInstanceEffect* GetInstanceEffect() {return m_pEffect;}

protected:

protected:    
    std::string             m_strEffectName;                        ///< 사용하는 이펙트의 이름

    CNtlInstanceEffect*     m_pEffect;                              ///< 생성된 이펙트 객체
};

/// 사운드 재생 노드
class CNtlSLCENodeSound : public CNtlSLCENode
{
public:
    CNtlSLCENodeSound();

    virtual void    Create();
    virtual void    Destory();
    virtual RwBool  Update(RwReal fElapsedTime);
    virtual RwBool  Load(CNtlSerializer& s);
    virtual RwBool  Save(CNtlSerializer& s);    
	virtual void    SetPosition(const RwV3d& vPos);
	void			  SetOffsetPos(const RwV3d& vPos) {m_vOffsetPos = vPos;}
	RwV3d			 GetOffsetPos() {return m_vOffsetPos;}

    void            SetSoundName(const std::string& strName) {m_strSoundFileName = strName;}
    std::string     GetSoundName() {return m_strSoundFileName;}
	void			  SetChannelGroup(RwUInt8 byChannelGroup) {m_byChannelGroup = byChannelGroup;}
	RwUInt8			 GetChannelGroup() {return m_byChannelGroup;}
    void            SetSoundLoop(RwBool bLoop) {m_bLoop = bLoop;}
    RwBool          GetSoundLoop() {return m_bLoop;}

protected:

protected:
    std::string         m_strSoundFileName;
	RwUInt8				  m_byChannelGroup;
    RwBool              m_bLoop;                                ///< 사운드 루프 여부
	RwV3d           m_vOffsetPos;                                     ///< 컨트롤러에서 상대적인 오프셋 위치

    RwUInt32            m_hSoundHandle;
};
/*
/// 말풍선 노드
class CNtlSLCENodeBalloon  : public CNtlSLCENode
{
public:
    

public:
    CNtlSLCENodeBalloon();

    virtual void    Create();
    virtual void    Destory();
    virtual RwBool  Update(RwReal fElapsedTime);
    virtual RwBool  Load(CNtlSerializer& s);
    virtual RwBool  Save(CNtlSerializer& s);   
    virtual void    SetPosition(const RwV3d& vPos);

    void            SetBallonRect(RwV3d& vStart, RwV3d& vEnd);                      ///< 말풍선의 영역을 설정한다.
    RwV3d           GetBallonRectStart() {return m_vStartRect;}
    RwV3d           GetBallonRectEnd()   {return m_vEndRect;}

	void			  SetInstanceTime(RwReal fInstanceTime)	{m_fInstanceTime = fInstanceTime;}
	RwReal			  GetInstanceTime() {return m_fInstanceTime;}

	void			  SetLifeTime(RwReal fLifeTime)	{m_fLifeTime = fLifeTime;}
	RwReal			  GetLifeTime() {return m_fLifeTime;}

    void            SetTblIdx(RwUInt32 uiTblIdx) {m_uiTableIdx = uiTblIdx;}
    RwUInt32        GetTblIdx() {return m_uiTableIdx;}

protected:
    void            CreateBalloon();

protected:        
    RwV3d               m_vStartRect;                   ///< 말풍선 시작영역
    RwV3d               m_vEndRect;                     ///< 말풍선 끝 영역
    RwReal              m_fInstanceTime;                ///< 말풍선 생성 빈도 (초단위)
    RwReal              m_fLifeTime;                    ///< 생성 시간
    RwUInt32            m_uiTableIdx;

};
*/

class CNtlSobNpc;

/**
 * \ingroup NtlSimulation
 * \brief   심판 연출 노드
 *
 * \date 2008-07-28
 * \author agebreak
 */

typedef std::vector<RwUInt32>		VEC_NARRATION_ID;
typedef std::vector<RwUInt32>		VEC_ANIM_ID;

class CNtlSLCENodeReferee : public CNtlSLCENode
{
public:
	enum eNarrationPlayType
	{
		NARRATION_PLAY_RANDOM,
		NARRATION_PLAY_SEQUENCE,
	};

    enum ERefreeMoveType
    {
        E_REF_MOVE_STOP,
        E_REF_MOVE_EDGE,
        E_REF_MOVE_TARGET,        
    };

	enum EAnimPlayType
	{
		ANIM_PLAY_TYPE_RANDOM,
		ANIM_PLAY_TYPE_SEQUENCE,
		ANIM_PLAY_TYPE_LOOP,
	};

	struct sNarrationInfo
	{
		eNarrationPlayType				ePlayType;
		RwReal							fElapsed;
		RwReal							fRemainTime;
		RwUInt8							byCurNarrationIndex;		

		VEC_NARRATION_ID				vecNarrationID;
	};

	struct sMoveInfo
	{
		ERefreeMoveType					eMoveType;
		EAnimPlayType					eAnimPlayType;		
		RwBool							bAnimStart;
		RwV3d							v3RefreePos;	
		RwV3d							vTargetPos;                            ///< 이동할 타겟 위치
		RwV3d							vOrisinalPos;
		RwReal							fDelayTime;
		RwReal							fMoveSpeed;
		RwReal							fAngleY;

		VEC_ANIM_ID						vecAnimID; 
		VEC_ANIM_ID::iterator			itCurAnimID;
	};

public:
    CNtlSLCENodeReferee();
    virtual void    Create();
    virtual void    Destory();
    virtual RwBool  Update(RwReal fElapsedTime);
    virtual RwBool  Load(CNtlSerializer& s);
    virtual RwBool  Save(CNtlSerializer& s);
	virtual void	SetPosition(const RwV3d& vPos) {}
    
	void			SetRefreePosition(const RwV3d& vPos);
	RwV3d			GetRefreePosition() { return m_MoveInfo.v3RefreePos; }

	void			SetNarrationPlayType(RwUInt8 byNarrationPlayType);
	RwUInt8			GetNarrationPlayType() { return (RwUInt8)m_NarrationInfo.ePlayType; }

	void			AppendNarrationIndex(RwUInt32 uiNarrationIndex);
	RwInt32			GetCurNarrationIndex();
	void			RemoveNarrationIndex(RwUInt32 uiNarrationIndex);
	void			RemoveAllNarrationIndex();
	VEC_NARRATION_ID& GetNarrationList() { return m_NarrationInfo.vecNarrationID; }

	void			SetAngleY(const RwReal fAngleY);
	RwReal			GetAngleY(){ return m_MoveInfo.fAngleY; }

	void			SetAnimPlayType(RwUInt8 byAnimType);
	RwUInt8			GetAnimPlayType() { return (RwUInt8)m_MoveInfo.eAnimPlayType; }

	void			AppendAnimId(RwUInt32 uiAnimID);
	RwInt32			GetCurAnimId();
	void			RemoveAnimId(RwUInt32 uiAnimID);
	void			RemoveAllAnimId();
	VEC_ANIM_ID&	GetAnimList() { return m_MoveInfo.vecAnimID; }
	RwBool			IsExistAnimId(RwUInt32 uiAnimID);

	void			SetMoveType(ERefreeMoveType eType);
	ERefreeMoveType GetMoveType() {return m_MoveInfo.eMoveType;}

	void			SetMoveSpeed(RwReal fSpeed) { m_MoveInfo.fMoveSpeed = fSpeed; }
	RwReal			GetMoveSpeed() { return m_MoveInfo.fMoveSpeed; }

	void			SetTargetPos(const RwV3d& vPos) {m_MoveInfo.vTargetPos = vPos;}
	RwV3d			GetTargetPos() {return m_MoveInfo.vTargetPos;}
    
	RwBool			IsPlay() { return m_MoveInfo.bAnimStart; }

	virtual void	HandleEvents(RWS::CMsg &pMsg);

protected:    
    void			UpdateMove(RwReal fElapsedTime, ERefreeMoveType eType);         
	void			UpdateNarration(RwReal fElapsedTime);

	void			SetNextNarration();
	RwBool			SetNextAnim();

protected:	
	sNarrationInfo		m_NarrationInfo;
	sMoveInfo			m_MoveInfo;

protected:    
	CNtlSobNpc*         m_pRefreeActor;                                      ///< 심판 Actor 객체 포인터
};
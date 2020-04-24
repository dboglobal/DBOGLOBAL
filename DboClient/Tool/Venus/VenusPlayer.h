//***********************************************************************************
//	File		:	VenusPlayer.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	ⓒ 2005 by NTL CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once

#include "rwcore.h"
#include "rpworld.h"

#include <rphanim.h>
#include <rpskin.h>
#include <rpuvanim.h>
#include <rtdict.h>

#include "NtlPLEntity.h"
#include "NtlPLResource.h"

#include <vector>

#include "NtlPLEntityRenderHelpers.h"

/**
 * \ingroup Venus
 * \brief Event Layer에서 사용하는 캐릭터 객체
 * \date 2006-08-02
 * \author agebreak
 */
class CVenusPlayer
{
public:
	enum FADE_OUT_TYPE
	{
		FADE_OUT_NONE,
		FADE_OUT_DELEETE,
		FADE_OUT_STOP,
		FADE_OUT_BALL
	};

	struct EVENT
	{
		std::string		strEffectName;
		RwInt32			nBoneIndex;

		RwBool			bFadeIn;
		RwBool			bFadeOut;
		RwReal			fFadeInTime;
		RwReal			fFadeOutTime;
		RwInt32			nFadeOutType;

		CNtlPLEntity*	pEffectEntity;
	};
    typedef std::vector<EVENT*>	svdef_EVENT;

public:
	CVenusPlayer(void);
	~CVenusPlayer(void);

	void	Create();
    void	Delete();
    void	DeleteEffectEntity();

	void	CopyData(CVenusPlayer* pSrcPlayer);

	void	InitializeEvent(EVENT* pEvent);

	RwBool	Update(RwReal fElapsedTime);
	RwBool	Render();

	void	SetWorldMatrix(const RwMatrix& matWorld);

			RwInt32		GetNumNode();                       ///< Bone의 개수를 반환한다.
            RwChar*     GetBoneName(int nIndex);            ///< Bone의 이름을 반환한다.
	const	RwMatrix*	GetNodeMatrix(RwInt32 nNode);

	RwBool	Save(FILE* pFile);
	RwBool	Load(FILE* pFile);

	CVenusPlayer::EVENT*	GetEvent(RwInt32 nIndex)		{ return m_svEvent[nIndex]; }

    // Get/Set 메소드
    std::string GetClumpName() {return m_strClumpName;}             ///< Clump 이름을 반환한다.
    void        SetClumpName(std::string& strClumpName) {m_strClumpName = strClumpName;} ///< Clump 이름을 설정한다.

    std::string GetAnimationName() {return m_strAnimationName;}     ///< Animation 이름을 반환한다.
    void        SetAnimationName(std::string& strAnimName) { m_strAnimationName = strAnimName;} ///< Animation 이름을 설정한다.

    CNtlPLResource* GetAnimationResource() {return m_pAnimationResource;} ///< Animation Resource를 반환한다.

    svdef_EVENT& GetEventVector() {return m_svEvent;}               ///< Event Vector를 반환한다.

    RwReal      GetAnimCurrentTime() {return m_fCurrentTime;}       ///< 현재 애니메이션 시간을 반환한다.
    RwUInt32    GetMaxTime() {return m_nMaxTime;}                   ///< MaxTime를 반환한다.

protected:
    void	DeleteEvent();

protected:
    svdef_EVENT			m_svEvent;                      ///< Event 객체들의 Vector

    std::string			m_strClumpName;                 ///< Clump Name
    std::string			m_strAnimationName;             ///< Animation Name

    CNtlPLResource*		m_pClumpResource;               ///< Clump Resource
    CNtlPLResource*		m_pAnimationResource;           ///< Animation Resource
    RpHAnimHierarchy*   m_pBaseHierarchy;               ///< 캐릭터 모델의 Hierarchy

    RwReal				m_fLifeTime;    

    RwReal				m_fCurrentTime;
    RwUInt32			m_nMaxTime;
    SToonData*          m_pToonData;                     ///< 툰이 적용된 메시를 위한 툰 정보
};

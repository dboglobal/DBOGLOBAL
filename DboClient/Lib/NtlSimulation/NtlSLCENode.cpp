#include "precomp_ntlsimulation.h"
#include "NtlSLCENode.h"

// shared
#include "QuestNarrationTable.h"
#include "TableContainer.h"
#include "NtlMovement.h"

// util
#include "NtlSerializer.h"

// core
#include "NtlDebug.h"
#include "NtlMath.h"

// sound
#include "NtlSoundManager.h"

// presentation
#include "NtlInstanceEffect.h"
#include "NtlPLEvent.h"
#include "NtlPLSceneManager.h"
#include "NtlPLCharacter.h"
#include "NtlPLEventGenerator.h"

// simulation
#include "NtlSLApi.h"
#include "NtlSobNpc.h"
#include "NtlSLTBCrowdManager.h"
#include "NtlSobProxy.h"
#include "NtlSLGlobal.h"
#include "NtlSobManager.h"
#include "NtlSobAvatar.h"
#include "NtlSobCharProxy.h"
#include "NtlSLEventFunc.h"
#include "NtlSLLogic.h"


sVERSTION CNtlSLCENode::m_Version(CENODE_VER_LASTEST);


CNtlSLCENode::CNtlSLCENode(void)
{
    m_fTotalElapsedTime = 0.0f;
    m_vPos              = ZeroAxis;
}

CNtlSLCENode::~CNtlSLCENode(void)
{
    
}

RwBool CNtlSLCENode::Update( RwReal fElapsedTime ) 
{
    m_fTotalElapsedTime += fElapsedTime;

    return TRUE;
}

RwBool CNtlSLCENode::Load( CNtlSerializer& s ) 
{
    // 타입은 컨트롤러에서 판단을 위해서 로드하기 때문에, 여기서는 로드하지 않는다.
    s>>m_strName;

    return TRUE;    
}

RwBool CNtlSLCENode::Save( CNtlSerializer& s ) 
{
    s<<m_eType;
    s<<m_strName;

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////
CNtlSLCENodeEffect::CNtlSLCENodeEffect() 
{
    m_eType     = CNtlSLCENode::E_CE_EFFECT;
    m_pEffect   = NULL;
}

void CNtlSLCENodeEffect::Create() 
{
	// 천하제일 무도회 예선전에서는 이펙트를 플레이 하지 않는다.
	sWORLD_INFO* pWORLD_INFO = Logic_GetActiveWorldInfo();
	if( !pWORLD_INFO )
	{
		DBO_FAIL("Not exist active world");
		return;
	}

	if( pWORLD_INFO->sRuleInfo.byRuleType == GAMERULE_MINORMATCH )
		return;

    DBO_ASSERT(!m_strEffectName.empty(), "[CENodeEffect] Not Load Data");
    if(m_strEffectName.empty())
        return;

    if(m_pEffect)
        Destory();

    m_pEffect = (CNtlInstanceEffect*)GetSceneManager()->CreateEntity(PLENTITY_EFFECT, m_strEffectName.c_str());    
    SetPosition(m_vPos);
}

void CNtlSLCENodeEffect::Destory() 
{
    if(m_pEffect)
    {
        GetSceneManager()->DeleteEntity(m_pEffect);
        m_pEffect = NULL;
    }
}

RwBool CNtlSLCENodeEffect::Update( RwReal fElapsedTime ) 
{
    CNtlSLCENode::Update(fElapsedTime);    

    return TRUE;
}

RwBool CNtlSLCENodeEffect::Load( CNtlSerializer& s ) 
{
    CNtlSLCENode::Load(s);

    s>>m_strEffectName;

    if(m_pEffect)
        Destory();

    return TRUE;
}

RwBool CNtlSLCENodeEffect::Save( CNtlSerializer& s ) 
{
    CNtlSLCENode::Save(s);

    s<<m_strEffectName;

    return TRUE;
}

void CNtlSLCENodeEffect::SetPosition( const RwV3d& vPos ) 
{
    m_vPos = vPos;

    if(m_pEffect)
    {
        m_pEffect->SetPosition(&vPos);
    }
}

//////////////////////////////////////////////////////////////////////////
CNtlSLCENodeSound::CNtlSLCENodeSound() 
{
    m_eType = CNtlSLCENode::E_CE_SOUND;
    m_bLoop = TRUE;
    m_hSoundHandle = INVALID_SOUND_HANDLE;
	m_vOffsetPos = ZeroAxis;
}

void CNtlSLCENodeSound::Create() 
{
    DBO_ASSERT(!m_strSoundFileName.empty(), "[CNtlSLCENodeSound] Not Load Data");
    if(m_strSoundFileName.empty())
        return;

    if(m_hSoundHandle != INVALID_SOUND_HANDLE)
        Destory();

	m_hSoundHandle = Logic_PlayGUISound((char*)m_strSoundFileName.c_str());

    SetPosition(m_vPos);
}

void CNtlSLCENodeSound::Destory() 
{
    if(m_hSoundHandle != INVALID_SOUND_HANDLE)
    {
		GetSoundManager()->Stop(m_hSoundHandle);   
        m_hSoundHandle = INVALID_SOUND_HANDLE;
    }
}

RwBool CNtlSLCENodeSound::Update( RwReal fElapsedTime ) 
{
    CNtlSLCENode::Update(fElapsedTime);

    return TRUE;
}

RwBool CNtlSLCENodeSound::Load( CNtlSerializer& s ) 
{
    CNtlSLCENode::Load(s);

    s>>m_strSoundFileName;
    s>>m_bLoop;
	s.Out(&m_vPos, sizeof(RwV3d));
    s>>m_byChannelGroup;

    return TRUE;
}

RwBool CNtlSLCENodeSound::Save( CNtlSerializer& s ) 
{
    CNtlSLCENode::Save(s);

    s<<m_strSoundFileName;
    s<<m_bLoop;
	s.In(&m_vPos, sizeof(RwV3d));
    s<<m_byChannelGroup;

    return TRUE;
}

void CNtlSLCENodeSound::SetPosition( const RwV3d& vPos ) 
{
	m_vPos = vPos + m_vOffsetPos;

	if(m_hSoundHandle != INVALID_SOUND_HANDLE)
		GetSoundManager()->SetSoundPosition(m_hSoundHandle, m_vPos.x, m_vPos.y, m_vPos.z);
}

//////////////////////////////////////////////////////////////////////////
/*
CNtlSLCENodeBalloon::CNtlSLCENodeBalloon() 
{
	m_eType      = E_CE_BALLOON;
    m_uiTableIdx = INVALID_TBLIDX;
    m_vStartRect = ZeroAxis;
    m_vEndRect = ZeroAxis;
    m_fInstanceTime = 0.0f;
    m_fLifeTime = 0.0f;
}

void CNtlSLCENodeBalloon::Create() 
{
    
}

void CNtlSLCENodeBalloon::Destory() 
{
    // 생성된 말풍선 모두 소멸
    CNtlPLEventGenerator::HideAllSpectatorBalloon();
}

RwBool CNtlSLCENodeBalloon::Update( RwReal fElapsedTime ) 
{
    CNtlSLCENode::Update(fElapsedTime);

    if(m_fTotalElapsedTime >= m_fInstanceTime)
    {
        // 말풍선 생성 이벤트
        CreateBalloon();

        m_fTotalElapsedTime = 0.0f;
    }

    return TRUE;
}

RwBool CNtlSLCENodeBalloon::Load( CNtlSerializer& s ) 
{
    CNtlSLCENode::Load(s);
    
    s.Out(&m_vStartRect, sizeof(RwV3d));
    s.Out(&m_vEndRect, sizeof(RwV3d));    
    s<<m_fInstanceTime;
    s<<m_fLifeTime;

    return TRUE;
}

RwBool CNtlSLCENodeBalloon::Save( CNtlSerializer& s ) 
{
    CNtlSLCENode::Save(s);

    s.In(&m_vStartRect, sizeof(RwV3d));
    s.In(&m_vEndRect, sizeof(RwV3d));
    s>>m_fInstanceTime;
    s>>m_fLifeTime;

    return TRUE;
}

void CNtlSLCENodeBalloon::SetPosition( const RwV3d& vPos ) 
{
    m_vPos = vPos;
}

void CNtlSLCENodeBalloon::SetBallonRect( RwV3d& vStart, RwV3d& vEnd ) 
{
    m_vStartRect = vStart;
    m_vEndRect = vEnd;
}

void CNtlSLCENodeBalloon::CreateBalloon() 
{
    if(m_vStartRect.x == 0 || m_vEndRect.x == 0)
        return;

    RwV3d vPos;
    vPos = NtlRandomNumber(m_vStartRect, m_vEndRect);

    CNtlPLEventGenerator::ShowSpectatorBalloon(vPos, L"Yankee Go Home", m_fLifeTime);
}
*/
//////////////////////////////////////////////////////////////////////////
CNtlSLCENodeReferee::CNtlSLCENodeReferee() 
{
	m_eType						= E_CE_JUDGE;

	m_NarrationInfo.ePlayType	= NARRATION_PLAY_RANDOM;
	m_NarrationInfo.fElapsed	= 0.f;
	m_NarrationInfo.fRemainTime	= 0.f;
	m_NarrationInfo.byCurNarrationIndex = INVALID_BYTE;

	m_MoveInfo.eMoveType		= E_REF_MOVE_STOP;
	m_MoveInfo.fMoveSpeed		= 0.0f;
	m_MoveInfo.v3RefreePos		= ZeroAxis;
	m_MoveInfo.vTargetPos		= ZeroAxis;
	m_MoveInfo.vOrisinalPos		= ZeroAxis;
	m_MoveInfo.fAngleY			= 0.f;
	m_MoveInfo.fDelayTime		= 0.f;
	m_MoveInfo.bAnimStart		= FALSE;
	m_MoveInfo.eAnimPlayType	= ANIM_PLAY_TYPE_RANDOM;	

	m_pRefreeActor	= NULL;
}

void CNtlSLCENodeReferee::Create() 
{
	m_pRefreeActor = GetNtlSLCrowdManager()->GetRefreeActor();
	if(m_pRefreeActor)
	{
        m_pRefreeActor->EnableVisible(TRUE);
		m_pRefreeActor->SetPosition(&m_MoveInfo.v3RefreePos);
		m_pRefreeActor->SetAngleY(m_MoveInfo.fAngleY);

		m_MoveInfo.itCurAnimID		= m_MoveInfo.vecAnimID.begin();
		m_MoveInfo.bAnimStart		= FALSE;

		SetNextAnim();
		SetNextNarration();

        if(m_MoveInfo.eMoveType == E_REF_MOVE_TARGET)
        {
            CNtlSLEventGenerator::SobDestMove(m_pRefreeActor->GetSerialID(),
                                              0, 
                                              m_pRefreeActor->GetPosition(),
                                              m_MoveInfo.vTargetPos,
                                              0);
        }
	}
}

void CNtlSLCENodeReferee::Destory() 
{
    if(m_pRefreeActor)
    {
        m_pRefreeActor->EnableVisible(FALSE);
        m_pRefreeActor = NULL;
    }
	
	m_MoveInfo.bAnimStart = FALSE;

	m_NarrationInfo.vecNarrationID.clear();
	m_MoveInfo.vecAnimID.clear();
}

RwBool CNtlSLCENodeReferee::Update( RwReal fElapsedTime ) 
{
	CNtlSLCENode::Update(fElapsedTime);

	if(!m_pRefreeActor)
		return FALSE;

	UpdateMove(fElapsedTime, m_MoveInfo.eMoveType);
	UpdateNarration(fElapsedTime);

	return TRUE;
}

RwBool CNtlSLCENodeReferee::Load( CNtlSerializer& s ) 
{
	CNtlSLCENode::Load(s);	

	if( m_Version.fCurrentWorkVerstion >= CENODE_VER_LASTEST )
	{
		s.Out(&m_NarrationInfo.ePlayType, sizeof(m_NarrationInfo.ePlayType));
		
		RwUInt32 uiNarrationIDCount;
		s>>uiNarrationIDCount;

		for( RwUInt32 i = 0 ; i < uiNarrationIDCount ; ++i )
		{
			RwUInt32 uiNarrationIndex;

			s>>uiNarrationIndex;
			m_NarrationInfo.vecNarrationID.push_back(uiNarrationIndex);
		}
	}
	

	if( m_Version.fCurrentWorkVerstion > CENODE_VER_1_2 )
		s.Out(&m_MoveInfo.eAnimPlayType, sizeof(m_MoveInfo.eAnimPlayType));
	else
	{
		RwBool bTemp;
		s>>bTemp;
	}

	if( m_Version.fCurrentWorkVerstion >= CENODE_VER_1_2_1 )
	{
		RwUInt32 uiAnimIDCount;
		s>>uiAnimIDCount;

		for( RwUInt32 i = 0 ; i < uiAnimIDCount ; ++i )
		{
			RwUInt32 uiAnimID;

			s>>uiAnimID;
			m_MoveInfo.vecAnimID.push_back(uiAnimID);
		}
	}
	else if( m_Version.fCurrentWorkVerstion >= CENODE_VER_1_1 )
	{
		RwUInt32 uiAnimID;
		s>>uiAnimID;    
	}

	s.Out(&m_MoveInfo.v3RefreePos, sizeof(m_MoveInfo.v3RefreePos));

	if( m_Version.fCurrentWorkVerstion > CENODE_VER_1_2 )
		s>>m_MoveInfo.fAngleY;

	if( m_Version.fCurrentWorkVerstion >= CENODE_VER_1_1 )
	{
		s.Out(&m_MoveInfo.eMoveType, sizeof(m_MoveInfo.eMoveType));
		s>>m_MoveInfo.fMoveSpeed;
		s.Out(&m_MoveInfo.vTargetPos, sizeof(m_MoveInfo.vTargetPos));
	}

	if( m_Version.fCurrentWorkVerstion == CENODE_VER_1_2_1 )
	{
		struct SRefreeBalloon
		{
			RwUInt32 uiTableIdx;
			RwReal   fLifeTime;
		};
		std::vector<SRefreeBalloon>		m_listBalloon;

		RwInt32 nSize;

		s>>nSize;
		for(int i = 0; i < nSize; ++i)
		{
			SRefreeBalloon balloon;
			s.Out(&balloon, sizeof(SRefreeBalloon));
			m_listBalloon.push_back(balloon);        
		}
	}	

	return TRUE;
}

RwBool CNtlSLCENodeReferee::Save( CNtlSerializer& s ) 
{
	CNtlSLCENode::Save(s);

	s.In(&m_NarrationInfo.ePlayType, sizeof(m_NarrationInfo.ePlayType));
	s<<m_NarrationInfo.vecNarrationID.size();
	for each(RwUInt32 uiNarrationIndex in m_NarrationInfo.vecNarrationID)
	{
		s.In(&uiNarrationIndex, sizeof(uiNarrationIndex));
	}

	s.In(&m_MoveInfo.eAnimPlayType, sizeof(m_MoveInfo.eAnimPlayType));
	s<<m_MoveInfo.vecAnimID.size();
	for each(RwUInt32 uiAnimID in m_MoveInfo.vecAnimID)
	{
		s.In(&uiAnimID, sizeof(uiAnimID));
	}

	s.In(&m_MoveInfo.v3RefreePos, sizeof(m_MoveInfo.v3RefreePos));
	s<<m_MoveInfo.fAngleY;
	s<<(RwInt32)m_MoveInfo.eMoveType;
	s<<m_MoveInfo.fMoveSpeed;    
	s.In(&m_MoveInfo.vTargetPos, sizeof(m_MoveInfo.vTargetPos));

	return TRUE;
}

void CNtlSLCENodeReferee::SetNarrationPlayType(RwUInt8 byNarrationPlayType)
{
	if( byNarrationPlayType > NARRATION_PLAY_SEQUENCE )
	{
		DBO_FAIL("Invalid Narration play type : " << byNarrationPlayType);
		return;
	}

	m_NarrationInfo.ePlayType = (eNarrationPlayType)byNarrationPlayType;
}

void CNtlSLCENodeReferee::SetAnimPlayType(RwUInt8 byAnimType)
{
	if( byAnimType > ANIM_PLAY_TYPE_LOOP )
	{
		DBO_FAIL("Invalid animation play type : " << byAnimType);
		return;
	}

	m_MoveInfo.eAnimPlayType = (EAnimPlayType)byAnimType;
}

void CNtlSLCENodeReferee::SetRefreePosition( const RwV3d& vPos ) 
{
	m_MoveInfo.v3RefreePos	= vPos;
	m_MoveInfo.vOrisinalPos	= vPos;

	if(m_pRefreeActor)
	{
		m_pRefreeActor->SetPosition(&m_MoveInfo.v3RefreePos);
	}
}

void CNtlSLCENodeReferee::SetAngleY(const RwReal fAngleY)
{
	m_MoveInfo.fAngleY			= fAngleY;

	if( m_pRefreeActor )
	{
		m_pRefreeActor->SetAngleY(m_MoveInfo.fAngleY);
	}
}

void CNtlSLCENodeReferee::AppendNarrationIndex(RwUInt32 uiNarrationIndex)
{
	m_NarrationInfo.vecNarrationID.push_back(uiNarrationIndex);
}

RwInt32 CNtlSLCENodeReferee::GetCurNarrationIndex()
{
	return 0xffffffff;
}

void CNtlSLCENodeReferee::RemoveNarrationIndex(RwUInt32 uiNarrationIndex)
{
	VEC_ANIM_ID::iterator it = m_NarrationInfo.vecNarrationID.begin();
	for( ; it != m_NarrationInfo.vecNarrationID.end() ; ++it )
	{
		if( (*it) == uiNarrationIndex )
		{
			m_NarrationInfo.vecNarrationID.erase(it);
			break;
		}
	}
}

void CNtlSLCENodeReferee::RemoveAllNarrationIndex()
{
	m_NarrationInfo.vecNarrationID.clear();
}

void CNtlSLCENodeReferee::AppendAnimId(RwUInt32 uiAnimID)
{
	m_MoveInfo.vecAnimID.push_back(uiAnimID);
}

RwInt32 CNtlSLCENodeReferee::GetCurAnimId()
{
	if(m_pRefreeActor)
	{
		CNtlSobCharProxy* pCharProxy = reinterpret_cast<CNtlSobCharProxy*>( m_pRefreeActor->GetSobProxy() );
		return pCharProxy->GetBaseAnimationKey();
	}

	return 0xffffffff;
}

RwBool CNtlSLCENodeReferee::IsExistAnimId(RwUInt32 uiAnimID)
{
	for each( RwUInt32 uiID in m_MoveInfo.vecAnimID )
	{
		if( uiID == uiAnimID )
			return TRUE;
	}

	return FALSE;
}

void CNtlSLCENodeReferee::SetMoveType(ERefreeMoveType eType)
{
	m_MoveInfo.eMoveType = eType;

	if( m_MoveInfo.eMoveType == E_REF_MOVE_EDGE )
		m_MoveInfo.v3RefreePos = m_MoveInfo.vOrisinalPos;
}

void CNtlSLCENodeReferee::RemoveAnimId(RwUInt32 uiAnimID)
{
	VEC_ANIM_ID::iterator it = m_MoveInfo.vecAnimID.begin();
	for( ; it != m_MoveInfo.vecAnimID.end() ; ++it )
	{
		if( (*it) == uiAnimID )
		{
			m_MoveInfo.vecAnimID.erase(it);
			break;
		}
	}
}

void CNtlSLCENodeReferee::RemoveAllAnimId()
{
	m_MoveInfo.vecAnimID.clear();
}

RwBool CNtlSLCENodeReferee::SetNextAnim()
{
	if( !m_pRefreeActor )
		return FALSE;

	if( m_MoveInfo.vecAnimID.empty() )
		return FALSE;

	CNtlSobCharProxy* pCharProxy = reinterpret_cast<CNtlSobCharProxy*>( m_pRefreeActor->GetSobProxy() );
	if( !pCharProxy )
		return FALSE;


	RwUInt32 uiAnimID;

	switch( m_MoveInfo.eAnimPlayType )
	{
	case ANIM_PLAY_TYPE_RANDOM:
		{
			RwUInt8 byIndex = (RwUInt8)NtlRandomNumber(0, m_MoveInfo.vecAnimID.size() - 1);
			uiAnimID = m_MoveInfo.vecAnimID[byIndex];
			break;
		}
	case ANIM_PLAY_TYPE_SEQUENCE:
		{
			if( m_MoveInfo.itCurAnimID == m_MoveInfo.vecAnimID.end() )
				return FALSE;

			if( m_MoveInfo.bAnimStart )
				++m_MoveInfo.itCurAnimID;

			if( m_MoveInfo.itCurAnimID == m_MoveInfo.vecAnimID.end() )
			{
				m_MoveInfo.bAnimStart = FALSE;
				return FALSE;
			}

			uiAnimID = *m_MoveInfo.itCurAnimID;
	   	 	break;
		}
	case ANIM_PLAY_TYPE_LOOP:
		{
			if( m_MoveInfo.bAnimStart )
				++m_MoveInfo.itCurAnimID;

			if( m_MoveInfo.itCurAnimID == m_MoveInfo.vecAnimID.end() )
				m_MoveInfo.itCurAnimID = m_MoveInfo.vecAnimID.begin();

			uiAnimID = *m_MoveInfo.itCurAnimID;
			break;
		}
	default:
		{
			DBO_FAIL("Invalid animation play type");
			return FALSE;
		}
	}
	
	//pCharProxy->SetBaseAnimation(uiAnimID, FALSE);
    CNtlSLEventGenerator::SobAnimPlay(m_pRefreeActor->GetSerialID(), uiAnimID, FALSE, 0.0f);
	m_MoveInfo.bAnimStart = TRUE;

	return TRUE;
}

void CNtlSLCENodeReferee::UpdateMove(RwReal fElapsedTime, ERefreeMoveType eType)
{
	RwV3d vTargetPos, vTargetDir;
	switch(eType)
	{
	case E_REF_MOVE_STOP:                
		return;
	case E_REF_MOVE_EDGE:
		{
			if( m_MoveInfo.fDelayTime <= 2.f )
			{
				m_MoveInfo.fDelayTime += fElapsedTime;
				return;
			}

			vTargetPos = GetNtlSLGlobal()->GetSobAvatar()->GetPosition();

			if( m_MoveInfo.vOrisinalPos.x == vTargetPos.x )
			{
				m_MoveInfo.fDelayTime = 0.f;
				return;
			}

			vTargetPos.y = m_MoveInfo.v3RefreePos.y;
			vTargetPos.z = m_MoveInfo.v3RefreePos.z;
		}
		break;
	case E_REF_MOVE_TARGET: 
        RwV3d vTempPos = m_pRefreeActor->GetPosition();
        vTempPos.y += 1.5f;
        RwReal fWorldHeight;
        GetSceneManager()->GetWorldHeight(&vTempPos, fWorldHeight, NULL);
        vTempPos.y = fWorldHeight;
        m_pRefreeActor->SetPosition(&vTempPos);
        return;				
	}

	RwV3d vPos = m_MoveInfo.v3RefreePos;
	vPos.y = vTargetPos.y = 0.0f;
	vTargetDir = vTargetPos - vPos;
	RwV3dNormalize(&vTargetDir, &vTargetDir);    

	RwReal fDist = CNtlMath::GetLength(vPos, vTargetPos);

	if( fDist > 0.f )
	{
		RwReal fMoveDist = min(fDist, m_MoveInfo.fMoveSpeed * fElapsedTime);    
		m_MoveInfo.v3RefreePos += vTargetDir * fMoveDist;
		m_pRefreeActor->SetPosition(&m_MoveInfo.v3RefreePos);

		if( eType == E_REF_MOVE_EDGE )
			m_MoveInfo.vOrisinalPos = m_MoveInfo.v3RefreePos;
	}	
}

void CNtlSLCENodeReferee::SetNextNarration()
{
	RwUInt32 uiNarrationIndex;

	switch( m_NarrationInfo.ePlayType )
	{
	case NARRATION_PLAY_RANDOM:
		{
			m_NarrationInfo.byCurNarrationIndex = (RwUInt8)NtlRandomNumber(0, m_NarrationInfo.vecNarrationID.size() - 1);
			uiNarrationIndex = m_NarrationInfo.vecNarrationID[m_NarrationInfo.byCurNarrationIndex];
			break;
		}
	case NARRATION_PLAY_SEQUENCE:
		{
			if( m_NarrationInfo.byCurNarrationIndex != INVALID_BYTE )
			{
				if( (m_NarrationInfo.byCurNarrationIndex + 1) == (RwInt8)m_NarrationInfo.vecNarrationID.size() )
					return;

				++m_NarrationInfo.byCurNarrationIndex;
			}
			else
			{
				// 처음 시작할 때
				m_NarrationInfo.byCurNarrationIndex = 0;
			}

			uiNarrationIndex = m_NarrationInfo.vecNarrationID[m_NarrationInfo.byCurNarrationIndex];
			break;
		}
	default:
		{
			DBO_FAIL("Invalid narration play type");
			return;
		}
	}

	CQuestNarrationTable* pQuestNarrationTable = API_GetTableContainer()->GetQuestNarrationTable();
	sQUEST_NARRATION_TBLDAT* pQUEST_NARRATION_TBLDAT = reinterpret_cast<sQUEST_NARRATION_TBLDAT*>( pQuestNarrationTable->FindData(uiNarrationIndex) );

	if( !pQUEST_NARRATION_TBLDAT )
	{
		DBO_FAIL("Not exist quest narration table data of index : " << uiNarrationIndex);
		m_pRefreeActor = NULL;
		return;
	}

	m_NarrationInfo.fElapsed		= 0.f;
	m_NarrationInfo.fRemainTime		= (RwReal)(pQUEST_NARRATION_TBLDAT->byTime * pQUEST_NARRATION_TBLDAT->byNumber);


	CNtlSLEventGenerator::BroadMsgNfy_MiniNarration(uiNarrationIndex);
}

void CNtlSLCENodeReferee::UpdateNarration(RwReal fElapsedTime)
{
	// 미니 나레이션은 도중에 다른 미니 나레이션의 새치기로 연출 중단 이벤트를 받을 수 없을 수도 있다
	// 직접 Remain time을 체크하여 다음 내용을 플레이하자!!

	if( !m_pRefreeActor )
		return;

	if( m_NarrationInfo.vecNarrationID.empty() )
		return;

	if( m_NarrationInfo.byCurNarrationIndex != INVALID_BYTE )
	{
		m_NarrationInfo.fElapsed += fElapsedTime;
		if( m_NarrationInfo.fElapsed < m_NarrationInfo.fRemainTime )
			return;
	}
	
	SetNextNarration();
}

void CNtlSLCENodeReferee::HandleEvents(RWS::CMsg &pMsg)
{
	SNtlEventAnimEnd *pSobAnimEnd = reinterpret_cast<SNtlEventAnimEnd*>(pMsg.pData);

	if( m_pRefreeActor && pSobAnimEnd->uiSerialId == m_pRefreeActor->GetSerialID() )
		SetNextAnim();
}
#include "precomp_ntlsimulation.h"
#include "NtlBehaviorCharMove.h"

// shared 
#include "NtlMovement.h"

// sound
#include "NtlSoundDefines.h"
#include "NtlSoundManager.h"

// presentation
#include "NtlPLCharacter.h"
#include "NtlPLEvent.h"

// simulation
#include "NtlSLApi.h"
#include "NtlSobActor.h"
#include "NtlSLLogic.h"
#include "NtlSobManager.h"
#include "NtlFSMDef.h"
#include "NtlSLEventFunc.h"
#include "NtlSobCharProxy.h"
#include "NtlSLVisualDeclear.h"
#include "NtlDTProp.h"
#include "NtlDTNode.h"


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// 

CNtlBehaviorCharMove::CNtlBehaviorCharMove()
{
    m_fAnimSpeed         = 1.0f;
    m_byPrevMoveFlag     = NTL_MOVE_F;
}

CNtlBehaviorCharMove::~CNtlBehaviorCharMove()
{
    m_listPathNode.clear();
}

RwBool CNtlBehaviorCharMove::IsEmptyPathNode(void)
{
    return m_listPathNode.empty() ? TRUE : FALSE;
}

void CNtlBehaviorCharMove::AddPathNode(RwV3d vPos)
{
    m_listPathNode.push_back(vPos);
}

RwBool CNtlBehaviorCharMove::PopPathNode(RwV3d& vPos)
{
    if(m_listPathNode.empty())
    {
        return FALSE;
    }

    ListPathNode::iterator it = m_listPathNode.begin();
    vPos = (*it);

    m_listPathNode.erase(it);

    return TRUE;
}

void CNtlBehaviorCharMove::ClearPathNode(void)
{
    m_listPathNode.clear();
}

RwUInt32 CNtlBehaviorCharMove::HandleEvents( RWS::CMsg &pMsg ) 
{
    if(pMsg.Id == g_EventSobMoveSync)
    {
        // Take a MoveSync packet and queue it.
        if(m_pActor->GetClassID() == SLCLASS_AVATAR)
            NTL_RETURN(NTL_FSM_EVENTRES_PASS);

        CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
        SNtlEventSobMoveSync* pMoveSync = (SNtlEventSobMoveSync*)pMsg.pData;
        SNtlEventSobMoveSync* pNewMoveSync = NTL_NEW SNtlEventSobMoveSync();
        memcpy_s(pNewMoveSync, sizeof(SNtlEventSobMoveSync), pMoveSync, sizeof(SNtlEventSobMoveSync));
        pNewMoveSync->byMoveFlag = pBeData->GetMoveStuff()->byMoveFlags;
        
        SMoveSyncStuff* pMoveSyncStuff = pBeData->GetMoveSyncStuff();
        pMoveSyncStuff->m_MoveSyncQ.push_back(pNewMoveSync);        
      
    }   

    NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

RwBool CNtlBehaviorCharMove::IsMoveSyncState() 
{
    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
    SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();

    if(m_pActor->GetClassID() == SLCLASS_PLAYER &&
        pMoveStuff->byType == NTL_MOVETARGET_DIR &&        
        pMoveStuff->byMoveFlags == NTL_MOVE_F)
        return TRUE;

    return FALSE;
}

RwBool CNtlBehaviorCharMove::IsMoveSyncSend() 
{
    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
    SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();

    if(m_pActor->GetClassID() == SLCLASS_AVATAR &&
        pMoveStuff->byType == NTL_MOVETARGET_DIR &&
        !(pMoveStuff->byMoveFlags == NTL_MOVE_TURN_L || pMoveStuff->byMoveFlags == NTL_MOVE_TURN_R))
        return TRUE;

    return FALSE;
}

void CNtlBehaviorCharMove::UpdateAnimSpeed(void)
{
    m_fAnimSpeed = m_fAnimSpeed < 0.0f ? 1.0f : m_fAnimSpeed;    

    RwReal fAnimSpeed = 1.0f;

    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());     
	if (pBeData->GetMoveStuff()->byFormFlag == NTL_MOVE_FLAG_DASH)
	{
		fAnimSpeed = Logic_GetDashAnimSpeed(m_pActor);
	}
    else if(!Logic_IsActorWalkMove(m_pActor, pBeData->GetMoveStuff()->byFormFlag))
    {
        fAnimSpeed = m_fAnimSpeed * Logic_GetRunAnimSpeed(m_pActor);
    }

    fAnimSpeed = fAnimSpeed < 0.0f ? 1.0f : fAnimSpeed;

    m_pActor->GetSobProxy()->SetAnimSpeed(fAnimSpeed);
}

RwReal CNtlBehaviorCharMove::GetMoveSpeed( RwUInt8 byMoveFlag ) 
{
    if(byMoveFlag == NTL_MOVE_B || byMoveFlag == NTL_MOVE_B_TURN_L ||byMoveFlag == NTL_MOVE_B_TURN_R)
    {
        return Logic_GetBackSpeed(m_pActor);        
    }
    else
    {
        return Logic_GetFrontRunSpeed(m_pActor);        
    }  
}

RwBool CNtlBehaviorCharMove::IsMoveSyncEmpty() 
{
    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
    SMoveSyncStuff* pMoveSyncStuff = pBeData->GetMoveSyncStuff();
    return pMoveSyncStuff->m_MoveSyncQ.empty();
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// 
DEFINITION_MEMORY_POOL(CNtlBehaviorCharSplineMove)

CNtlBehaviorCharSplineMove::CNtlBehaviorCharSplineMove()
{
    m_pDTNode	= NULL;
    m_pDTProp	= NULL;
}

CNtlBehaviorCharSplineMove::~CNtlBehaviorCharSplineMove()
{
	NTL_DELETE( m_pDTNode );
}

void CNtlBehaviorCharSplineMove::Enter(void)
{
    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
    SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();

    m_pDTProp = API_GetDTContainter()->Find(pMoveStuff->chSpline);

    CNtlDTSplineNode *pDTSplineNode = NTL_NEW CNtlDTSplineNode;

    if(pMoveStuff->bSplineGroundCheck)
        pDTSplineNode->SetDataGroundMove(m_pDTProp, m_pActor, pMoveStuff->vCurrLoc);
    else
        pDTSplineNode->SetDataFlyMove(m_pDTProp, m_pActor, pMoveStuff->vCurrLoc);

    m_pDTNode = pDTSplineNode;

    // animation setting

    CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
    RwUInt32 uiCurrAnimKey = pSobProxy->GetBaseAnimationKey();
    RwUInt32 uiNextAnimKey = 0;

    sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(m_pActor);
    if(pItemTblData && Logic_IsEquipedStaffWeapon(pItemTblData))
    {
        uiNextAnimKey = NML_STAFF_FRONT_RUN;
    }
    else
    {
        if(pMoveStuff->bSplineRunMove)
            uiNextAnimKey = RUN_FRONT;
        else
            uiNextAnimKey = WALK_FRONT;
    }

    if(uiCurrAnimKey != uiNextAnimKey)
        pSobProxy->SetBaseAnimation(uiNextAnimKey);

    // base enter
    CNtlBehaviorBase::Enter();
}

void CNtlBehaviorCharSplineMove::Exit(void)
{
    CNtlBehaviorBase::Exit();
}

void CNtlBehaviorCharSplineMove::Update(RwReal fElapsed)
{
    m_pDTNode->Update(fElapsed);
    if(m_pDTNode->IsFinish())
    {
        CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
        SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
        pMoveStuff->byMoveResult = 0;

        Finish();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// 


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// 
DEFINITION_MEMORY_POOL(CNtlBehaviorCharFalling)

CNtlBehaviorCharFalling::CNtlBehaviorCharFalling()
{
    m_byFallingState	= FALIINGSTATE_WALK;
    m_fFallingSpeed		= 0.0f;
}

CNtlBehaviorCharFalling::~CNtlBehaviorCharFalling()
{
}

void CNtlBehaviorCharFalling::Enter(void)
{
    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
    SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

    m_vFallingPos		= m_pActor->GetPosition();	

	m_byMoveDirection = pCtrlStuff->uExtra.sFalling.byMoveDirection;

    m_vFallingDir		= pCtrlStuff->uExtra.sFalling.vFallingDir;
    m_fFallingHeight	= pCtrlStuff->uExtra.sFalling.fSearchHeight;
    m_fFallingWalkSpeed	= pCtrlStuff->uExtra.sFalling.fSpeed * 0.5f;

	if (m_byMoveDirection == NTL_MOVE_NONE)
	{
		ChangeFallingState(FALIINGSTATE_FALLING);
	}

    // 나중에 base class enter를 호출한다.
    CNtlBehaviorBase::Enter(); 
}

void CNtlBehaviorCharFalling::Exit(void)
{
    CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
    CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>( pSobProxy->GetPLMainEntity() );

    pPLCharacter->SetBlend(BLEND_TWEEN);

    CNtlBehaviorBase::Exit(); 
}

void CNtlBehaviorCharFalling::Update(RwReal fElapsed)
{
    if(m_byFallingState == FALIINGSTATE_END)
        return;

    m_fFallingSpeed = m_fFallingSpeed - (JUMP_GRAVITY_SPEED * fElapsed);

    RwV3d vPos = m_pActor->GetPosition();
    RwV3d vDir = m_pActor->GetDirection();

    UpdatePositionMove(vPos, fElapsed);

    if(m_byFallingState == FALIINGSTATE_WALK)
        UpdateWalking(vPos, fElapsed);
    else if(m_byFallingState == FALIINGSTATE_FALLING)
        UpdateFalling(vPos, fElapsed);
    else if(m_byFallingState == FALIINGSTATE_MOVE_LADNDING)
        UpdateMoveLanding(vPos, fElapsed);
    else if(m_byFallingState == FALIINGSTATE_WATER_LADNDING)
        UPdateWaterLanding(vPos, fElapsed);

    RwBool bLandSuccess = FALSE;

    if(m_fFallingHeight + FALLING_CHECK_LEN / 2.0f < m_sHStuff.fFinialHeight)
    {
        if(vPos.y <= m_sHStuff.fFinialHeight)
            bLandSuccess = TRUE;
    }
    else
    {
        if(vPos.y < m_fFallingHeight + FALLING_CHECK_LEN / 2.0f && vPos.y < m_sHStuff.fFinialHeight)
            bLandSuccess = TRUE;
    }

    if(bLandSuccess)
    {
        vPos.y = m_sHStuff.fFinialHeight;
        ChangeFallingState(FALIINGSTATE_END);
    }

    CNtlVector vHeading, vDest;
    NtlGetDestination_Jump(vDir.x, vDir.y, vDir.z, m_fFallingSpeed, vPos.x, vPos.y, vPos.z, m_vFallingDir.x, m_vFallingDir.z, m_byMoveDirection, (DWORD)(fElapsed*1000.f), 1.0f, &vHeading, &vDest);

    RwV3d vNewDir;
    CNtlMath::MathRwV3dAssign(&vNewDir, vHeading.x, 0.0f, vHeading.z); 

    m_pActor->SetPosition(&vPos);
    m_pActor->SetDirection(&vNewDir);
}

void CNtlBehaviorCharFalling::UpdateData(void)
{
}

RwUInt32 CNtlBehaviorCharFalling::HandleEvents(RWS::CMsg &pMsg)
{
    NTL_FUNCTION("CNtlBehaviorCharFalling::HandleEvents");

    if(pMsg.Id == g_EventAnimEnd)
    {
        SNtlEventAnimEnd *pAnimEnd = reinterpret_cast<SNtlEventAnimEnd*>( pMsg.pData );
        RwUInt32 uiAnimKey = pAnimEnd->uiBaseAnimKey;

        if(uiAnimKey == JUMP_LANDING_SHORT)
        {
            Finish();
        }
    }

    NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

void CNtlBehaviorCharFalling::SetAnimFalling(void)
{
    CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
    pSobProxy->SetBaseAnimation(JUMP_HOVER, TRUE);
}

void CNtlBehaviorCharFalling::SetAnimMoveLanding(void)
{
    CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
    CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>( pSobProxy->GetPLMainEntity() );
    pPLCharacter->SetBlend(BLEND_TWEEN, 0.0f, 0.0f);

    pSobProxy->SetBaseAnimation(JUMP_LANDING_SHORT, FALSE);
}

void CNtlBehaviorCharFalling::SetAnimWaterLanding(void)
{
    RwV3d vPos = m_pActor->GetPosition();
    vPos.y = m_sHStuff.fWaterHeight;
    CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
    pSobProxy->CreatePLEffect(NTL_VID_WATER_JUMP, &vPos);    
}

RwBool CNtlBehaviorCharFalling::LandingCheck(RwV3d& vPos)
{
    RwBool bLandSuccess = FALSE;

    if(m_fFallingHeight + FALLING_CHECK_LEN / 2.0f < m_sHStuff.fFinialHeight)
    {
        if(vPos.y <= m_sHStuff.fFinialHeight + 0.25f)
            bLandSuccess = TRUE;
    }
    else
    {
        if(vPos.y < m_fFallingHeight + FALLING_CHECK_LEN / 2.0f && vPos.y <= m_sHStuff.fFinialHeight + 0.25f)
            bLandSuccess = TRUE;
    }

    if(bLandSuccess)
    {
        if(Logic_IsSwimming(m_pActor, &vPos, m_sHStuff.fWorldHeight, m_sHStuff.fWaterHeight))
            ChangeFallingState(FALIINGSTATE_WATER_LADNDING);
        else
            ChangeFallingState(FALIINGSTATE_MOVE_LADNDING);

        return TRUE;
    }

    return FALSE;
}

void CNtlBehaviorCharFalling::UpdatePositionMove(RwV3d& vPos, RwReal fElapsed)
{
    RwV3d vOldPos;
    RwV3dAssignMacro(&vOldPos, &vPos);

    RwReal fSpeed = m_fFallingWalkSpeed * fElapsed;

	if (m_byMoveDirection == NTL_MOVE_NONE)
	{
		vPos.y += m_fFallingSpeed * fElapsed; // if move_none, then we fall straight down
	}
	else
	{
		vPos.x += m_vFallingDir.x * fSpeed;
		vPos.y += m_fFallingSpeed * fElapsed;
		vPos.z += m_vFallingDir.z * fSpeed;
	}

    Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);

    RwV3d vNextPos;
    RwV3dAssignMacro(&vNextPos, &vPos);

    RwBool bCollMoveImPossible;
    RwUInt8 byColliResult = Logic_CharacterCollisionEx(m_pActor, &vNextPos, fSpeed, vNextPos, bCollMoveImPossible, fElapsed);

    if(byColliResult == NTL_CHARACTER_COLLI_WORLD_ATTR_TERRAIN_SLANT)
    {
        vPos.x = vOldPos.x;
        vPos.z = vOldPos.z;
    }
    else
    {
        vPos.x = vNextPos.x;
        vPos.z = vNextPos.z;
    }
}

void CNtlBehaviorCharFalling::UpdateWalking(RwV3d& vPos, RwReal fElapsed)
{
    RwV3d vSub;
    RwV3dSubMacro(&vSub, &vPos, &m_vFallingPos);
    RwReal fWalkLen = RwV3dLength(&vSub);

    if(fWalkLen > FALLING_WALK_LEN)
    {
        ChangeFallingState(FALIINGSTATE_FALLING);
    }

    LandingCheck(vPos);


}

void CNtlBehaviorCharFalling::UpdateFalling(RwV3d& vPos, RwReal fElapsed)
{
    LandingCheck(vPos);
}

void CNtlBehaviorCharFalling::UpdateMoveLanding(RwV3d& vPos, RwReal fElapsed)
{

}

void CNtlBehaviorCharFalling::UPdateWaterLanding(RwV3d& vPos, RwReal fElapsed)
{
}

void CNtlBehaviorCharFalling::ChangeFallingState(RwUInt8 byFallingState)
{
    switch(byFallingState)
    {
    case FALIINGSTATE_WALK:
        break;
    case FALIINGSTATE_FALLING:
        SetAnimFalling();
        break;
    case FALIINGSTATE_MOVE_LADNDING:
        SetAnimMoveLanding();
        break;
    case FALIINGSTATE_WATER_LADNDING:
        SetAnimWaterLanding();
        break;
    case FALIINGSTATE_END:
        if(m_byFallingState == FALIINGSTATE_WATER_LADNDING)
        {
            Finish();
        }
        break;
    }

    m_byFallingState = byFallingState;
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// 


DEFINITION_MEMORY_POOL( CNtlBehaviorBusMove )


void CNtlBehaviorBusMove::RNS::Init( void )
{
    m_matHermite._11 = 2.f;		m_matHermite._12 = -2.f;	m_matHermite._13 = 1.f;		m_matHermite._14 = 1.f;
    m_matHermite._21 = -3.f;	m_matHermite._22 = 3.f;		m_matHermite._23 = -2.f;	m_matHermite._24 = -1.f;
    m_matHermite._31 = 0.f;		m_matHermite._32 = 0.f;		m_matHermite._33 = 1.f;		m_matHermite._34 = 0.f;
    m_matHermite._41 = 1.f;		m_matHermite._42 = 0.f;		m_matHermite._43 = 0.f;		m_matHermite._44 = 0.f;

    m_defNodeList.clear();
    m_defNodeList.reserve( 255 );

    m_fMaxDist = 0.f;

    m_bLastNodeInfoSetting = FALSE;
}

RwBool CNtlBehaviorBusMove::RNS::IsLastNodeInfoSetting( void )
{
    return m_bLastNodeInfoSetting;
}

void CNtlBehaviorBusMove::RNS::SetLastNodeInfoSetting( RwBool bLastNodeInfoSetting )
{
    m_bLastNodeInfoSetting = bLastNodeInfoSetting;
}

int CNtlBehaviorBusMove::RNS::GetNodeSize( void )
{
    return m_defNodeList.size();
}

RwReal CNtlBehaviorBusMove::RNS::GetMaxDistance( void )
{
    return m_fMaxDist;
}

void CNtlBehaviorBusMove::RNS::GetMoveDistRatio( RwReal fTime, ERNS_MOVE_TYPE& eMoveType, RwReal& fMoveDistRatio )
{
    int nSize = m_defNodeList.size();
    if ( nSize < BUILD_MIN_NODE_SIZE )
    {
        eMoveType = ERNS_MOVE_TYPE_INVALID;
        fMoveDistRatio = 0.f;
        return;
    }

    RwReal fDist = fTime * m_fMaxDist;
    RwReal fCurDist = 0.f;

    int i = 0;
    while ( fCurDist + m_defNodeList[i].fDist < fDist && i < nSize - 2 )
    {
        fCurDist += m_defNodeList[i].fDist;
        ++i;
    }

    RwReal t = (fDist - fCurDist) / m_defNodeList[i].fDist;
    if ( t < 0.00001f ) t = 0.f;
    if ( t > 0.99999f ) t = 1.f;

    if ( i == nSize - 2 )
    {
        eMoveType = ERNS_MOVE_TYPE_SECOND_DEST;
        fMoveDistRatio = t;
    }
    else if ( i == nSize - 3 )
    {
        eMoveType = ERNS_MOVE_TYPE_DEST;
        fMoveDistRatio = t;
    }
    else
    {
        eMoveType = ERNS_MOVE_TYPE_PRE_DEST;
        fMoveDistRatio = t;
    }
}

D3DXVECTOR3 CNtlBehaviorBusMove::RNS::GetPosition( RwReal fTime )
{
    int nSize = m_defNodeList.size();
    if ( nSize < BUILD_MIN_NODE_SIZE )
    {
        return D3DXVECTOR3( FLT_MAX, FLT_MAX, FLT_MAX );
    }

    RwReal fDist = fTime * m_fMaxDist;
    RwReal fCurDist = 0.f;

    int i = 0;
    while ( fCurDist + m_defNodeList[i].fDist < fDist && i < nSize - 2 )
    {
        fCurDist += m_defNodeList[i].fDist;
        ++i;
    }

    RwReal t = (fDist - fCurDist) / m_defNodeList[i].fDist;
    if ( t < 0.00001f ) t = 0.f;
    if ( t > 0.99999f ) t = 1.f;

    D3DXVECTOR3 vStartVel = m_defNodeList[i].vVel * m_defNodeList[i].fDist;
    D3DXVECTOR3 vEndVel = m_defNodeList[i+1].vVel * m_defNodeList[i].fDist;

    return GetPositionOnCubic( m_defNodeList[i].vPos, vStartVel, m_defNodeList[i+1].vPos, vEndVel, t );
}

void CNtlBehaviorBusMove::RNS::AddNodePos( const D3DXVECTOR3& vPos )
{
    SSplineData sSlplineData;
    sSlplineData.vPos = vPos;
    sSlplineData.vVel.x = sSlplineData.vVel.y = sSlplineData.vVel.z = 0.f;
    sSlplineData.fDist = 0.f;

    if ( m_defNodeList.empty() )
    {
        m_defNodeList.push_back( sSlplineData );
        m_fMaxDist = 0.f;
    }
    else
    {
        int nSize = m_defNodeList.size();

        D3DXVECTOR3 vDistLen = m_defNodeList[nSize - 1].vPos - vPos;
        RwReal fDistLen = D3DXVec3Length( &vDistLen );

		if ( fDistLen > 0.f )
		{
			m_defNodeList[nSize-1].fDist = fDistLen;
			m_fMaxDist += m_defNodeList[nSize-1].fDist;

			m_defNodeList.push_back( sSlplineData );
		}
    }
}

void CNtlBehaviorBusMove::RNS::BuildSpline( void )
{
    int nSize = m_defNodeList.size();
    if ( nSize < BUILD_MIN_NODE_SIZE )
    {
        return;
    }

    for ( int i = 1; i < nSize - 1; ++i )
    {
        D3DXVECTOR3 vDistDir1 = m_defNodeList[i+1].vPos - m_defNodeList[i].vPos;
        D3DXVec3Normalize( &vDistDir1, &vDistDir1 );
        D3DXVECTOR3 vDistDir2 = m_defNodeList[i-1].vPos - m_defNodeList[i].vPos;
        D3DXVec3Normalize( &vDistDir2, &vDistDir2 );

        m_defNodeList[i].vVel = vDistDir1 - vDistDir2;
        D3DXVec3Normalize( &m_defNodeList[i].vVel, &m_defNodeList[i].vVel );
    }

    m_defNodeList[0].vVel = GetStartVelocity( 0 );
    m_defNodeList[nSize-1].vVel = GetEndVelocity( nSize - 1 );
}

D3DXVECTOR3 CNtlBehaviorBusMove::RNS::GetPositionOnCubic( const D3DXVECTOR3& vStartPos, const D3DXVECTOR3& vStartVel, const D3DXVECTOR3& vEndPos, const D3DXVECTOR3& vEndVel, RwReal fTime )
{
    D3DXMATRIX mat;

    mat._11 = vStartPos.x;	mat._12 = vStartPos.y;	mat._13 = vStartPos.z;	mat._14 = 0.f;
    mat._21 = vEndPos.x;	mat._22 = vEndPos.y;	mat._23 = vEndPos.z;	mat._24 = 0.f;
    mat._31 = vStartVel.x;	mat._32 = vStartVel.y;	mat._33 = vStartVel.z;	mat._34 = 0.f;
    mat._41 = vEndVel.x; 	mat._42 = vEndVel.y; 	mat._43 = vEndVel.z; 	mat._44 = 0.f;

    mat = m_matHermite * mat;

    D3DXVECTOR4 vTimeVector( fTime * fTime * fTime, fTime * fTime, fTime, 1.f );

    D3DXVec4Transform( &vTimeVector, &vTimeVector, &mat );

    D3DXVECTOR3 vResult;
    vResult.x = vTimeVector.x;
    vResult.y = vTimeVector.y;
    vResult.z = vTimeVector.z;

    return vResult;
}

D3DXVECTOR3 CNtlBehaviorBusMove::RNS::GetStartVelocity( int nIndex )
{
    D3DXVECTOR3 vTemp = 3.f * (m_defNodeList[nIndex+1].vPos - m_defNodeList[nIndex].vPos) / m_defNodeList[nIndex].fDist;
    return (vTemp - m_defNodeList[nIndex+1].vVel) * 0.5f;
}

D3DXVECTOR3 CNtlBehaviorBusMove::RNS::GetEndVelocity( int nIndex )
{
    D3DXVECTOR3 vTemp = 3.f * (m_defNodeList[nIndex].vPos - m_defNodeList[nIndex-1].vPos) / m_defNodeList[nIndex-1].fDist;
    return (vTemp - m_defNodeList[nIndex-1].vVel) * 0.5f;
}

CNtlBehaviorBusMove::CNtlBehaviorBusMove( void )
{
    SetBehaviorId( NTL_BEID_CHAR_BUS_MOVE );
    SetBehaviorName( NTL_BENAME_CHAR_BUS_MOVE );

    m_bWalkMode = TRUE;
    m_fDefMoveSpeed = 0.f;

    m_RNSData.Init();
    m_fRNSMovingTime = 0.f;

    m_fRotSpeed = ROT_PER_SEC_MIN;
}

CNtlBehaviorBusMove::~CNtlBehaviorBusMove( void )
{
}

void CNtlBehaviorBusMove::Enter(void)
{
    CNtlBehaviorBase::Enter();

    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
    SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();

    m_sInputData.byFormFlag = pMoveStuff->byFormFlag;

    m_sInputData.vCurPos.x = pMoveStuff->vCurrLoc.x;
    m_sInputData.vCurPos.y = 0.f;
    m_sInputData.vCurPos.z = pMoveStuff->vCurrLoc.z;

    m_sInputData.vDestPos.x = pMoveStuff->vDest.x;
    m_sInputData.vDestPos.y = 0.f;
    m_sInputData.vDestPos.z = pMoveStuff->vDest.z;

    m_sInputData.vSecondPos.x = pMoveStuff->vSendDest.x;
    m_sInputData.vSecondPos.y = 0.f;
    m_sInputData.vSecondPos.z = pMoveStuff->vSendDest.z;

    BuildData();

    SetAnim();
}

void CNtlBehaviorBusMove::Update( RwReal fElapsed )
{
    // Moving
    if ( m_RNSData.GetNodeSize() < BUILD_MIN_NODE_SIZE )
    {
        RwV3d vCurPos, vCurDir;
        UpdateActorDirPos( fElapsed, vCurDir, vCurPos );

        m_pActor->SetDirection( &vCurDir );
        m_pActor->SetPosition( &vCurPos );
    }
    else
    {
        RNS::ERNS_MOVE_TYPE eMoveType;
        RwReal fMoveDistRatio;
        RwReal fTimeRatio;

        if ( !m_RNSData.IsLastNodeInfoSetting() )
        {
            fTimeRatio = m_fRNSMovingTime / ( m_RNSData.GetMaxDistance() / m_fDefMoveSpeed );

            m_RNSData.GetMoveDistRatio( fTimeRatio, eMoveType, fMoveDistRatio );

            switch ( eMoveType )
            {
            case RNS::ERNS_MOVE_TYPE_PRE_DEST:
                {
                    if ( fMoveDistRatio < 0.8f )		fElapsed *= 1.2f;
                    else								fElapsed *= 1.f;
                }
                break;
            case RNS::ERNS_MOVE_TYPE_DEST:
                {
                    if ( fMoveDistRatio < 0.5f )		fElapsed *= 0.9f;
                    else if ( fMoveDistRatio < 0.8f )	fElapsed *= 0.8f;
                    else if ( fMoveDistRatio < 0.97f )	fElapsed *= 0.6f;
                    else								fElapsed *= 0.5f;
                }
                break;
            case RNS::ERNS_MOVE_TYPE_SECOND_DEST:
                {
                    fElapsed = 0.f;
                }
                break;

            default:
                {
                    fElapsed = 0.f;
                }
                break;
            }
        }
        else
        {
            fElapsed *= 1.35f;
        }

        m_fRNSMovingTime += fElapsed;
        fTimeRatio = m_fRNSMovingTime / ( m_RNSData.GetMaxDistance() / m_fDefMoveSpeed );

        m_vRNSCurPos = m_RNSData.GetPosition( fTimeRatio );

        RwV3d vCurPos, vCurDir;
        UpdateActorDirPos( fElapsed, vCurDir, vCurPos );

        m_pActor->SetDirection( &vCurDir );
        m_pActor->SetPosition( &vCurPos );
    }
}


RwUInt32 CNtlBehaviorBusMove::HandleEvents(RWS::CMsg &pMsg)
{
    if ( g_EventSobSecondDestMove == pMsg.Id )
    {
        CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
        SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();

        m_sInputData.byFormFlag = pMoveStuff->byFormFlag;

        m_sInputData.vCurPos.x = pMoveStuff->vCurrLoc.x;
        m_sInputData.vCurPos.y = 0.f;
        m_sInputData.vCurPos.z = pMoveStuff->vCurrLoc.z;

        m_sInputData.vDestPos.x = pMoveStuff->vDest.x;
        m_sInputData.vDestPos.y = 0.f;
        m_sInputData.vDestPos.z = pMoveStuff->vDest.z;

        m_sInputData.vSecondPos.x = pMoveStuff->vSendDest.x;
        m_sInputData.vSecondPos.y = 0.f;
        m_sInputData.vSecondPos.z = pMoveStuff->vSendDest.z;

        BuildData();
    }

    return NTL_FSM_EVENTRES_PASS;
}


void CNtlBehaviorBusMove::BuildData( void )
{
    // 이동 속도 계산
    m_bWalkMode = Logic_IsActorWalkMove( m_pActor, m_sInputData.byFormFlag );
    if ( m_bWalkMode )
    {
        m_fDefMoveSpeed = Logic_GetFrontWalkSpeed( m_pActor );
    }
    else
    {
        m_fDefMoveSpeed = Logic_GetFrontRunSpeed( m_pActor );
    }

    D3DXVECTOR3 vPos1( m_sInputData.vCurPos.x, m_sInputData.vCurPos.y, m_sInputData.vCurPos.z );
    D3DXVECTOR3 vPos2( m_sInputData.vDestPos.x, m_sInputData.vDestPos.y, m_sInputData.vDestPos.z );
    D3DXVECTOR3 vPos3( m_sInputData.vSecondPos.x, m_sInputData.vSecondPos.y, m_sInputData.vSecondPos.z );

    if ( 0 == m_RNSData.GetNodeSize() )
    {
        m_fRNSMovingTime = 0.f;

        m_RNSData.Init();

        D3DXVECTOR3 vDir = vPos2 - vPos1;
        if ( D3DXVec3Length( &vDir ) > 0.5f )
        {
            m_RNSData.AddNodePos( vPos1 );
            m_RNSData.AddNodePos( vPos2 );
            m_RNSData.AddNodePos( vPos3 );
        }
        else
        {
            m_RNSData.AddNodePos( vPos2 );
            m_RNSData.AddNodePos( vPos3 );
        }

        m_vRNSCurPos = vPos1;

        m_fRotSpeed = ROT_PER_SEC_MIN;
    }
    else
    {
        D3DXVECTOR3 vDir = vPos3 - vPos2;
        if ( D3DXVec3Length( &vDir ) > 0.5f )
        {
            m_RNSData.AddNodePos( vPos3 );
        }
        else
        {
            m_RNSData.SetLastNodeInfoSetting( TRUE );
        }
    }

    m_RNSData.BuildSpline();
}

void CNtlBehaviorBusMove::UpdateActorDirPos( RwReal fElapsed, RwV3d& vResultDir, RwV3d& vResultPos )
{
    RwReal fSrcDirY;
    RwV3d vBusCurPos, vBusCurDir;

    // 위치
    RwV3d vDestPos = *(RwV3d*)&m_vRNSCurPos;
    RwV3d vSrcPos = m_pActor->GetPosition(); vSrcPos.y = 0.f;

    vBusCurPos = vDestPos;

    // 방향
    RwV3d vSrcDir = m_pActor->GetDirection();
    fSrcDirY = vSrcDir.y;
    vSrcDir.y = 0.f;
    if ( RwV3dNormalize( &vSrcDir, &vSrcDir ) < 0.00001f )
    {
        vSrcDir = m_pActor->GetDirection();
    }

    RwV3d vDestDir = vDestPos - vSrcPos;
    if ( RwV3dNormalize( &vDestDir, &vDestDir ) < 0.00001f )
    {
        vDestDir = vSrcDir;
    }

    RwV3d vRotDir = vDestDir - vSrcDir;
    RwReal fRotDirLen = RwV3dNormalize( &vRotDir, &vRotDir );
    if ( fRotDirLen < 0.00001f )
    {
        vBusCurDir = vDestDir;
    }
    else
    {
        RwReal fRotSpeed = GetRotSpeed( fElapsed );

        if ( fRotDirLen - (fRotSpeed * fElapsed) < 0.00001f  )
        {
            vBusCurDir = vDestDir;
        }
        else
        {
            vBusCurDir = vSrcDir + vRotDir * (fRotSpeed * fElapsed);

			if ( RwV3dNormalize( &vBusCurDir, &vBusCurDir ) < 0.0001f )
			{
				vBusCurDir = vDestDir;
			}
        }
    }

    Logic_GetWorldSlop( m_pActor, vBusCurPos, vBusCurDir, m_pActor->GetSobProxy()->GetPLEntityDepth() / 2.f, vResultPos, vResultDir );

#define HEIGHT_PER_SEC (1.f)

    RwReal fLerpY = vResultDir.y - fSrcDirY;
    fSrcDirY += fLerpY * HEIGHT_PER_SEC * fElapsed;

    if ( fLerpY > 0.0001f && fSrcDirY > vResultDir.y )
    {
        fSrcDirY = vResultDir.y;
    }
    else if ( fLerpY < 0.0001f && fSrcDirY < vResultDir.y )
    {
        fSrcDirY = vResultDir.y;
    }

    vResultDir.y = fSrcDirY;

	if ( RwV3dNormalize( &vResultDir, &vResultDir ) < 0.00001f )
	{
		vResultDir = vDestDir;
	}
}

RwReal CNtlBehaviorBusMove::GetRotSpeed( RwReal fElapsed )
{
    m_fRotSpeed += fElapsed * 0.9f;
    if ( m_fRotSpeed < ROT_PER_SEC_MIN ) m_fRotSpeed = ROT_PER_SEC_MIN;
    if ( m_fRotSpeed > ROT_PER_SEC_MAX ) m_fRotSpeed = ROT_PER_SEC_MAX;

    return m_fRotSpeed;
}

void CNtlBehaviorBusMove::SetAnim( void )
{
    CNtlSobCharProxy* pSobProxy = dynamic_cast< CNtlSobCharProxy* >( m_pActor->GetSobProxy() );
    RwUInt32 uiCurrAnimKey = pSobProxy->GetBaseAnimationKey();
    RwUInt32 uiNextAnimKey = 0;

    if ( m_bWalkMode )
    {
        uiNextAnimKey = WALK_FRONT;
    }
    else
    {
        uiNextAnimKey = RUN_FRONT;
    }

    if ( uiCurrAnimKey != uiNextAnimKey )
    {
        pSobProxy->SetBaseAnimation( uiNextAnimKey );
    }
}

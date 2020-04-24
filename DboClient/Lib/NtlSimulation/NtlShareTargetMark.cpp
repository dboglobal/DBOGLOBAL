#include "precomp_ntlsimulation.h"
#include "NtlShareTargetMark.h"

// presentation
#include "NtlPLSceneManager.h"
#include "NtlPLHelpers.h"

// simulation
#include "NtlSLVisualDeclear.h"
#include "NtlSob.h"
#include "NtlSobProxy.h"
#include "NtlSLLogic.h"
#include "NtlSobAvatar.h"

CNtlShareTargetMark::CNtlShareTargetMark(CNtlSob* pSobObject)
{
    m_pSobObj = pSobObject;
    m_pShareTargetMark = NULL;
    m_bySlot  = INVALID_BYTE;
    m_type    = SHARE_TARGET_NONE;
}

CNtlShareTargetMark::~CNtlShareTargetMark()
{
    DeleteShareTargetMark();
}

void CNtlShareTargetMark::CreateShareTargetMark( RwUInt8 bySlot, RwInt32 type )
{
    NTL_ASSERT(m_pSobObj, "m_pSobObj is NULL");    
    if(!m_pSobObj || (m_bySlot == bySlot && m_type == type))
        return;

    if(m_pShareTargetMark)
    {
        GetSceneManager()->DeleteEntity(m_pShareTargetMark);
        m_pShareTargetMark = NULL;
    }

    m_bySlot = bySlot;
    m_type = (EShareTargetType)type;

    switch(bySlot)
    {
    case 0:
        if(type == SHARE_TARGET_NORMAL)
        {
            m_pShareTargetMark = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_MARK_SHARE_TARGET_0_NORAML);
        }
        else if(type == SHARE_TARGET_ATTACK)
        {
            m_pShareTargetMark = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_MARK_SHARE_TARGET_0_ATTACK);
        }
        else if(type == SHARE_TARGET_TARGET)
        {
            m_pShareTargetMark = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_MARK_SHARE_TARGET_0_TARGET);
        }        
        break;
    case 1:
        if(type == SHARE_TARGET_NORMAL)
        {
            m_pShareTargetMark = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_MARK_SHARE_TARGET_1_NORAML);
        }
        else if(type == SHARE_TARGET_ATTACK)
        {
            m_pShareTargetMark = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_MARK_SHARE_TARGET_1_ATTACK);
        }
        else if(type == SHARE_TARGET_TARGET)
        {
            m_pShareTargetMark = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_MARK_SHARE_TARGET_1_TARGET);
        }                
        break;
    case 2:
        if(type == SHARE_TARGET_NORMAL)
        {
            m_pShareTargetMark = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_MARK_SHARE_TARGET_2_NORAML);
        }
        else if(type == SHARE_TARGET_ATTACK)
        {
            m_pShareTargetMark = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_MARK_SHARE_TARGET_2_ATTACK);
        }
        else if(type == SHARE_TARGET_TARGET)
        {
            m_pShareTargetMark = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_MARK_SHARE_TARGET_2_TARGET);
        }        
        break;
    case 3:
        if(type == SHARE_TARGET_NORMAL)
        {
            m_pShareTargetMark = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_MARK_SHARE_TARGET_3_NORAML);
        }
        else if(type == SHARE_TARGET_ATTACK)
        {
            m_pShareTargetMark = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_MARK_SHARE_TARGET_3_ATTACK);
        }
        else if(type == SHARE_TARGET_TARGET)
        {
            m_pShareTargetMark = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_MARK_SHARE_TARGET_3_TARGET);
        }        
        break;
    case 4:
        if(type == SHARE_TARGET_NORMAL)
        {
            m_pShareTargetMark = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_MARK_SHARE_TARGET_4_NORAML);
        }
        else if(type == SHARE_TARGET_ATTACK)
        {
            m_pShareTargetMark = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_MARK_SHARE_TARGET_4_ATTACK);
        }
        else if(type == SHARE_TARGET_TARGET)
        {
            m_pShareTargetMark = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_MARK_SHARE_TARGET_4_TARGET);
        }        
        break;    
    default:
        NTL_ASSERTFAIL("Not Defined Share Target Slot");
        return;        
    }

    // 타겟의 위치에 Attach 한다.
    if(m_pShareTargetMark)
    {
        RwV3d vOffset = ZeroAxis;
        vOffset.y = m_pSobObj->GetSobProxy()->GetPLEntityHeight() + NTL_ATTACK_MARK_OFFSET;
        Helper_AttachWorldPos(m_pSobObj->GetSobProxy()->GetPLMainEntity(), m_pShareTargetMark, vOffset);        
    }    
}

void CNtlShareTargetMark::DeleteShareTargetMark()
{
    if(!m_pShareTargetMark)
        return;

    GetSceneManager()->DeleteEntity(m_pShareTargetMark);
    m_pShareTargetMark = NULL;
    m_bySlot  = INVALID_BYTE;
    m_type    = SHARE_TARGET_NONE;

    // 공유 타겟 정보도 삭제한다.
    RwInt32 nSlot = Logic_isPartyShareTarget(m_pSobObj->GetSerialID());
    if(nSlot != -1)
    {
        CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
        if(pSobAvatar)
        {
            pSobAvatar->GetParty()->GetShareTargetInfos()[nSlot].hTarget = INVALID_SERIAL_ID;
        }            
    }

}

RwBool CNtlShareTargetMark::IsShareTargeting()
{
    return (m_type != SHARE_TARGET_NONE);
}
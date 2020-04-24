#include "precomp_ntlsimulation.h"
#include "NtlSLTBCrowdController.h"
#include "NtlSLCENode.h"
#include "NtlDebug.h"
#include "NtlMath.h"

CNtlSLTBCrowdController::CNtlSLTBCrowdController(void)
{
    m_bLoop = TRUE;
    m_fLifeTime = 0.0f;
    m_fInstanceTime = 0.0f;
}

CNtlSLTBCrowdController::~CNtlSLTBCrowdController(void)
{
    for each(CNtlSLCENode* pNode in m_listNode)
    {
        pNode->Destory();
        NTL_DELETE(pNode);
    }
    m_listNode.clear();
}

void CNtlSLTBCrowdController::Create() 
{
    for each(CNtlSLCENode* pNode in m_listNode)
    {
        if(pNode) pNode->Create();
    }
}

void CNtlSLTBCrowdController::Destroy() 
{
    for each(CNtlSLCENode* pNode in m_listNode)
    {
        if(pNode)
        {
            pNode->Destory();            
        }
    }    
}

RwBool CNtlSLTBCrowdController::Update( RwReal fElapsedTime ) 
{
    for each(CNtlSLCENode* pNode in m_listNode)
    {
        if(pNode)
        {
            pNode->Update(fElapsedTime);
        }
    }

    if(!IsLoop())
    {
        m_fInstanceTime += fElapsedTime;
        if(m_fInstanceTime >= m_fLifeTime)
		{
			m_fInstanceTime = 0.f;
			Destroy();
			return FALSE;
		}
    }

    return TRUE;
}

RwBool CNtlSLTBCrowdController::Load(CNtlSerializer& s)
{
    RwUInt32 nSize = 0;
    RwInt32  nType = 0;
    CNtlSLCENode* pCENode = NULL;
    
    s>>m_strKeyName;
    s>>m_bLoop;
    s>>m_fLifeTime;
    s>>nSize;
	s>>m_v3Pos.x;
	s>>m_v3Pos.y;
	s>>m_v3Pos.z;

    for(RwUInt32 i = 0; i < nSize; ++i)
    {
        s>>nType;
        
        switch(nType)
        {
        case CNtlSLCENode::E_CE_EFFECT:
            pCENode = NTL_NEW CNtlSLCENodeEffect();
            break;
        case CNtlSLCENode::E_CE_SOUND:
            pCENode = NTL_NEW CNtlSLCENodeSound();
            break;
		case CNtlSLCENode::E_CE_JUDGE:
			pCENode = NTL_NEW CNtlSLCENodeReferee();
			break;
        /*case CNtlSLCENode::E_CE_BALLOON:
            pCENode = NTL_NEW CNtlSLCENodeBalloon();
            break;*/
        default:
            NTL_ASSERTFAIL("[CENode] Not Type");
            break;
        }

        pCENode->Load(s);
		 pCENode->SetPosition(m_v3Pos);

        AddNode(pCENode);
    }
    
    return TRUE;
}

RwBool CNtlSLTBCrowdController::Save(CNtlSerializer& s)
{
    s<<m_strKeyName;
    s<<m_bLoop;
    s<<m_fLifeTime;
    s<<m_listNode.size();
	s<<m_v3Pos.x;
	s<<m_v3Pos.y;
	s<<m_v3Pos.z;

    for each(CNtlSLCENode* pNode in m_listNode)
    {
        if(pNode)
            pNode->Save(s);
    }
    return TRUE;
}

void CNtlSLTBCrowdController::AddNode( CNtlSLCENode* pCENode ) 
{
    m_listNode.push_back(pCENode);
}

void CNtlSLTBCrowdController::RemoveNode( CNtlSLCENode* pCENode ) 
{
    m_listNode.remove(pCENode);
}

CNtlSLCENode* CNtlSLTBCrowdController::GetNode( const RwChar* szName ) 
{
    for each(CNtlSLCENode* pNode in m_listNode)
    {
        if(pNode->GetName() == szName)
            return pNode;
    }

    return NULL;
}

void CNtlSLTBCrowdController::SetPosition( RwV3d& vPos ) 
{
    for each(CNtlSLCENode* pNode in m_listNode)
    {
        if(pNode)
        {
            pNode->SetPosition(vPos);
        }
    }

	m_v3Pos = vPos;
}

RwV3d CNtlSLTBCrowdController::GetPosition()
{
	return m_v3Pos;
}

void CNtlSLTBCrowdController::HandleEvents(RWS::CMsg &pMsg)
{
	for each( CNtlSLCENode* pNode in m_listNode )
		pNode->HandleEvents(pMsg);
}
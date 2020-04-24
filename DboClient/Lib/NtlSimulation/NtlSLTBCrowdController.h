/*****************************************************************************
*
* File			: NtlSLTBCrowdController.h
* Author		: agebreak
* Copyright		: (주)NTL
* Date			: 2008. 7. 14	
*****************************************************************************
* Desc          : 천하제일 무도회의 관중 이벤트 컨트롤러
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// util
#include "NtlSerializer.h"

class CNtlSLCENode;

/**
 * \ingroup NtlSimulation
 * \brief 관중을 제어하는 컨트롤러 클래스. 타입별로 생성되어 사용된다.
 *
 * \date 2008-07-21
 * \author agebreak
 */

typedef std::list<CNtlSLCENode*>				LIST_CENODE;
typedef std::list<CNtlSLCENode*>::iterator		ITER_CENODE;

class CNtlSLTBCrowdController
{
public:
    CNtlSLTBCrowdController(void);
    ~CNtlSLTBCrowdController(void);

    void        Create();
    void        Destroy();
    RwBool      Update(RwReal fElapsedTime);    
    RwBool      Load(CNtlSerializer& s);
    RwBool      Save(CNtlSerializer& s);

	LIST_CENODE&	GetNodeList() { return m_listNode; }

    void        AddNode(CNtlSLCENode* pCENode);
    void        RemoveNode(CNtlSLCENode* pCENode);
    CNtlSLCENode* GetNode(const RwChar* szName);

    void        SetPosition(RwV3d& vPos);
	RwV3d		 GetPosition();

    void        SetName(const std::string& strKeyName) {m_strKeyName = strKeyName;}
    std::string GetName() {return m_strKeyName;}

    void        SetLoop(RwBool bLoop) {m_bLoop = bLoop;}
    RwBool      IsLoop() {return m_bLoop;}

    void        SetLifeTime(RwReal fLifeTime) {m_fLifeTime = fLifeTime;}
    RwReal      GetLifeTime() {return m_fLifeTime;}

	void		HandleEvents(RWS::CMsg &pMsg);

protected:    

protected:        
    std::string                 m_strKeyName;               ///< 컨트롤러의 키네임
    RwBool                      m_bLoop;                    ///< 관중 이펙트가 계속 재생될 것인지 유무 플래그
    RwReal                      m_fLifeTime;                ///< 재생 시간
	RwV3d							m_v3Pos;					///< Entry 좌표


    LIST_CENODE					    m_listNode;                 ///< 구성 노드의 리스트
    RwReal                      m_fInstanceTime;            ///< 생성된 시간
};

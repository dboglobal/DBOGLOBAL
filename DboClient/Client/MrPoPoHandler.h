#pragma once

#include "NtlMrPoPoMsg.h"

/**
* \ingroup Client
* \brief 클라이언트 서포트 툴(Mr.PoPo)로 부터 날라오는 이벤트를 처리하는 클래스
*
* \date 2008-08-01
* \author agebreak
*/
class CMrPoPoHandler
{
public:
    CMrPoPoHandler(void);
    ~CMrPoPoHandler(void);

    void HandleEvent(RwInt32 nMsg, DWORD param);

protected:
    DWORD           OnSetLevel(RwInt32 nLevel);
    void            OnCollectDragonBall();
    void            OnTestCrowd(RwInt32 nCrowdType);
    std::wstring    OnTeleport(RwInt32 nIndex);
    std::wstring    OnTeleport(RwInt32 nMsg, RwInt32 nVal);        
    void            OnUpgradeAllSkill();    
    void            OnSuperMode();
    void            OnNamekTest();
    void            OnTenkaichiMark(RwInt32 nType);
    void            OnChangeColor(RwInt32 nType);
    void            OnChangeState(RwUInt32 uiStateID);
    void            OnTransform(RwUInt32 uiTransformID);
    void            OnStun(RwUInt32 uiStunType);
    void            OnTargetMarking(RwBool bSelect);
    void            OnPushTest();

    // 그래픽 설정 관련 이벤트
    void            OnTerrainDist(RwUInt32 uiDist);
    void            OnTerrainShadow(RwBool bEnable);
    void            OnWaterSpecular(RwBool bEnable);
	void            OnCharDist(RwUInt32 uiDist);
    void            OnCharEdge(RwBool bEnable);
	void            OnCharGrayColor(RwBool bEnable);
    void            OnLowEffect(RwBool bEnable);
	void            OnParticleRatio(RwUInt32 uiRatio);
	void            OnMeshEffect(RwBool bEnable);
	void            OnDecalEffect(RwBool bEnable);

protected:
    RwInt32         m_nTemp;

};

static CMrPoPoHandler* GetMrPoPoHandler()
{
    static CMrPoPoHandler mrPoPoHandler;

    return &mrPoPoHandler;
}

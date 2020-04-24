#pragma once

#include "ceventhandler.h"
#include "NtlFileSerializer.h"
#include "NtlSlEvent.h"
#include "NtlDebug.h"

class CNtlFileSerializer;
class CNtlSLTBCrowdController;
class CNtlSobNpc;

#define dDIRECTION_FILE_EXT		"dtf"

#define dDIRECTION_FILE_BUFFER	1024*1024			// 기본 버퍼를 1메가로 잡는다.
#define dDIRECTION_FILE_GLOW	1024*1024

#define dDIRECTION_GAME_DATA_FILE		".\\property\\TenkaichiDirection.dtf"

/// 관중의상태
enum ETBCrowdStepType
{
	E_STEP_BASIC,
	E_STEP_MATCH_READY,    
	E_STEP_STAGE_READY,
	E_STEP_STAGE_RUN,
	E_STEP_STAGE_FINISH,
	E_STEP_MATCH_FINISH,
	E_STEP_AWARD,
	E_STEP_BATTLE_END,
	E_STEP_HTB,
	E_STEP_KNOCKDOWN,
	E_STEP_KO,
    E_STEP_FINAL_DIRECTION,

	E_STEP_COUNT,
	E_STEP_NONE = 0xff,
};

/**
* \ingroup NtlSimulation
* \brief 관중 연출을 관리하는 매니저 클래스
*
* \date 2008-07-21
* \author agebreak
*/


class CNtlSLTBCrowdManager : public RWS::CEventHandler
{
public:
	typedef std::map<std::string, CNtlSLTBCrowdController*>				MAP_CONTROLLER;
	typedef std::map<std::string, CNtlSLTBCrowdController*>::iterator	ITER_MAP_CONTROLLER;

	typedef std::list<CNtlSLTBCrowdController*>							LIST_CONTROLLER;
	typedef std::list<CNtlSLTBCrowdController*>::iterator				ITER_LIST_CONTROLLER;

	static RwBool		ms_bDirectionToolMode;

	static CNtlSLTBCrowdManager* GetInstance();
	static void                  DeleteInstance();    

	void    Update(RwReal fElapsedTime);
	RwBool  Save(const RwChar* szFileName);
	RwBool  Load(const RwChar* szFileName);
	RwBool	Load(CNtlFileSerializer& rSerializer);
	virtual void HandleEvents(RWS::CMsg &pMsg);

	void	AddController(CNtlSLTBCrowdController* pController);
	void	RemoveController(const std::string& keyName);

	void	AddPlayController(CNtlSLTBCrowdController* pController);
	void	AddPlayController(const std::string& keyName);
	void	RemovePlayController(const std::string& keyName);

	void	AddStep(ETBCrowdStepType eStep);
	void	ChangeStep(ETBCrowdStepType eStep);                 ///< 스텝 상태를 변경한다.
	void	ReserveStep(ETBCrowdStepType eStep);				///< 다음 상태를 추가로 예약한다.    

    CNtlSobNpc* GetRefreeActor();                               ///< 심판 객체를 반환한다.    

protected:
	CNtlSLTBCrowdManager(void);
	~CNtlSLTBCrowdManager(void);

	void    Create();
	void    Destroy();
	ETBCrowdStepType    ConvertServerState(SNtlEventMinorMatchStateUpdate* pData);                ///< 서버 스테이트를 클라이언트 스테이트로 변경한다.
	ETBCrowdStepType    ConvertServerState(SNtlEventMajorMatchStateUpdate* pData);                ///< 서버 스테이트를 클라이언트 스테이트로 변경한다.	
    ETBCrowdStepType    ConvertServerState(SNtlEventFinalMatchStateUpdate* pData);                ///< 서버 스테이트를 클라이언트 스테이트로 변경한다.
	void				GetLuaFuncName(ETBCrowdStepType eStep, char* pcLuaFuncName, RwInt32 iBufferSize);
    void                CreateSobRefree();

protected:
	static CNtlSLTBCrowdManager* m_pInstance;

	MAP_CONTROLLER							m_mapControllerContainer;

	LIST_CONTROLLER							m_listCurController;
	ETBCrowdStepType						m_eReservatedStep;

    CNtlSobNpc*                             m_pRefreeActor;     ///< 심판 객체 (매니저에서만 생성해서 공용으로 사용)

	RwBool									m_bToolMode;
};

static CNtlSLTBCrowdManager* GetNtlSLCrowdManager()
{
	return CNtlSLTBCrowdManager::GetInstance();
};

static void DeleteNtlSLCrowdManager()
{
	CNtlSLTBCrowdManager::DeleteInstance();
}
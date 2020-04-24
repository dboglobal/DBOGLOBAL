/*****************************************************************************
 *
 * File			: DumpCommand.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 12. 26
 * Update		: 2006. 9. 29
 * Abstract		: DBO dump command manager
 *****************************************************************************
 * Desc         : 클라이언트에서 디버그용 메세지를 입력할 수 있도록 수정하였다
 *				  DEBUG 모드에서만 동작한다
 *				  
 * 사용법 예
 *	1) DUMP_DEBUG_MSG(DUMP_FLAG_MOVE, "좌표값이 잘못되었어요");
 *	2) DUMP_DEBUG_MSG(DUMP_FLAG_MOVE, MakeStr("좌표값(x : %d, y : %d)이 잘못되었습니다", 30, 40));
 *	3) DUMP_DEBUG_MSG(DUMP_FLAG_MOVE, MakeStr("%s가 없습니다", "몬스터"));
 * 
 *****************************************************************************/

#ifndef __DUMPCOMMAND_MANAGER_H__
#define __DUMPCOMMAND_MANAGER_H__

#include "NtlStringUtil.h"
#include "NtlDumpTarget.h"
#include "NtlSLDef.h"
#include "JudgeDump.h"

class CDBOCheatCommand;


#ifndef NDEBUG
	#define DUMP_DEBUG_MSG(eDumpType, msg) GetDumpCmdManager()->OutPutClientMessage(eDumpType, msg)
#else
	#define DUMP_DEBUG_MSG(eDumpType, msg)
#endif

class CDumpCmdManager
{
    friend class CDBOCheatCommand;

	typedef std::map<int, CNtlDumpTarget*>		MAP_DUMPTARGET;
	typedef std::map<int, CJudgeDump*>			MAP_JUDGEDUMP;

public:
	enum EDumpTarget
	{
		DUMP_TARGET_CONSOLE,
		DUMP_TARGET_GUI,
		DUMP_TARGET_FILE,

		NUM_DUMP_TARGET
	};

	static CDumpCmdManager *m_pInstance;

private:
	CNtlDumpTarget* m_apDumpTarget[NUM_DUMP_TARGET];
	MAP_DUMPTARGET m_mapActiveTarget;
	MAP_JUDGEDUMP m_mapActivejudgeDump;

	SERIAL_HANDLE m_hTargetSerialId;
	SERIAL_HANDLE m_hRegisterSerialId;
	RwBool m_bSendOut;
	RwBool m_bRecvOut;

    CDBOCheatCommand* m_pDBOCheatCommand;

private:

	CNtlDumpTarget* FindCmdTarget(RwInt32 iCmdId);
	

public:

	CDumpCmdManager();
	~CDumpCmdManager();

	static CDumpCmdManager* GetInstance(void);

	void SetTargetSerial(SERIAL_HANDLE hSerialId);
    void SetRegisterSerial(SERIAL_HANDLE hSerialId) {m_hRegisterSerialId = hSerialId;}
	void SendPacekt(void *pPacket);
	void RecvPacket(void *pPacket);
	void RecvNet(const char *pFormat);
    
	RwBool Command(const RwChar *pCmd);
	RwBool Command(const WCHAR *pCmd);

	// 클라이언트의 콘솔로부터 입력받는 메세지이다
	void CleintConsoleCommand(const RwChar *pCmd);
	
	// 클라이언트에서 메세지를 보여준다.
	void OutPutClientMessage(eDUMP_TYPE eDumpType, const RwChar* pMessage);
	void OutPutClientMessage(eDUMP_TYPE eDumpType, const WCHAR* pMessage);

    void ActiveDumpTarget(EDumpTarget eDumpTarget, RwBool bActive);
	void ActiveDumpType(eDUMP_TYPE eDumpType, RwBool bActive);

    void OutputSobObj(RwUInt32 uiSerialId);
    void OutputSobObj(CNtlDumpTarget &dump, RwUInt32 uiSeiralId);
	/// woosungs_test 20090723
	void OutputDump(const RwChar *pMsg, RwUInt32 iR = 255, RwUInt32 iG = 255, RwUInt32 iB = 255);

	RwBool IsOutputRecvPacketTarget(SERIAL_HANDLE hSerial);
};

static CDumpCmdManager* GetDumpCmdManager(void)
{
	return CDumpCmdManager::GetInstance();
}

static void CallbackNetSendPacket(void *pData)
{
	if(GetDumpCmdManager())
		GetDumpCmdManager()->SendPacekt(pData);
}

#endif
#include "precomp_dboclient.h"

#include "DumpCommand.h"

// shared
#include "NtlPacketUG.h"
#include "NtlPacketGU.h"
#include "NtlPacketUtil.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLEvent.h"

// simulation
#include "NtlSLDef.h"
#include "NtlSLGlobal.h"
#include "NtlSob.h"
#include "NtlSobAvatar.h"
#include "NtlSobManager.h"
#include "NtlSLEventFunc.h"
#include "NtlSLLogic.h"

// dbo
#include "DumpTargetConsole.h"
#include "DumpTargetGui.h"
#include "DumpTargetFile.h"
#include "DBOCheatCommand.h"

namespace
{
	RwUInt8 abyDUMP_COLOR[NUM_DUMP][3]		= { (149, 162, 72),					// DUMP_DEFAULT
												(150, 104, 84),					// DUMP_ACTION
												(47,  110, 187),				// DUMP_SOB_CREATE_DESTROY
												(182, 157, 52),					// DUMP_FSM
												(255, 255, 255) };				// DUMP_CONTENTS
};


CDumpCmdManager* CDumpCmdManager::m_pInstance = NULL;

CDumpCmdManager::CDumpCmdManager()
{
	m_pInstance = this;

	CNtlDumpTarget *pTarget;

	pTarget = NTL_NEW CDumpTargetConsole;
	m_apDumpTarget[DUMP_TARGET_CONSOLE] = pTarget;

	pTarget = NTL_NEW CDumpTargetGui;
	m_apDumpTarget[DUMP_TARGET_GUI] = pTarget;

	pTarget = NTL_NEW CDumpTargetFile;
	m_apDumpTarget[DUMP_TARGET_FILE] = pTarget;	


	m_hTargetSerialId = INVALID_SERIAL_ID;
	m_hRegisterSerialId = INVALID_SERIAL_ID;

	m_bSendOut = TRUE;
	m_bRecvOut = FALSE;

	ActiveDumpType(DUMP_DEFAULT, true);

    m_pDBOCheatCommand = NTL_NEW CDBOCheatCommand();
}

CDumpCmdManager::~CDumpCmdManager()
{
	for( RwUInt8 i = 0 ; i < NUM_DUMP_TARGET ; ++i )
	{
		NTL_DELETE(m_apDumpTarget[i]);
	}
	m_mapActiveTarget.clear();
	

	MAP_JUDGEDUMP::iterator it_judge = m_mapActivejudgeDump.begin();
	for( ; it_judge != m_mapActivejudgeDump.end() ; ++it_judge )
	{
		CJudgeDump* pJudgeDump = it_judge->second;
		NTL_DELETE(pJudgeDump);		
	}
	m_mapActivejudgeDump.clear();

	
    NTL_DELETE(m_pDBOCheatCommand);
	m_pInstance = NULL;
}

CDumpCmdManager* CDumpCmdManager::GetInstance(void)
{
	return m_pInstance;
}

CNtlDumpTarget* CDumpCmdManager::FindCmdTarget(RwInt32 iCmdId)
{
	if( iCmdId >= NUM_DUMP_TARGET )
		return NULL;

	return m_apDumpTarget[iCmdId];
}

void CDumpCmdManager::SetTargetSerial(SERIAL_HANDLE hSerialId)
{
	m_hTargetSerialId = hSerialId;
}

void CDumpCmdManager::SendPacekt(void *pPacket)
{
	if( m_mapActiveTarget.empty() )
		return;

	MAP_JUDGEDUMP::iterator it_judge = m_mapActivejudgeDump.begin();
	for( ; it_judge != m_mapActivejudgeDump.end() ; ++it_judge )
	{
		CJudgeDump* pJudgeDump = it_judge->second;
		char* pcText = pJudgeDump->GetDump_SendPacket(pPacket);

		if( pcText )
		{
			RwInt32 iDumpType = it_judge->first;
			MAP_DUMPTARGET::iterator it;
			for(it = m_mapActiveTarget.begin(); it != m_mapActiveTarget.end(); ++it)
			{
				CNtlDumpTarget& pTarget = *(it->second);
				pTarget.Color(abyDUMP_COLOR[iDumpType][0], abyDUMP_COLOR[iDumpType][1], abyDUMP_COLOR[iDumpType][2]);
				pTarget << pcText;
				pTarget.Dump();
			}
		}
	}
}

void CDumpCmdManager::RecvPacket(void *pPacket)
{
	if( m_mapActiveTarget.empty() )
		return;

	MAP_JUDGEDUMP::iterator it_judge = m_mapActivejudgeDump.begin();
	for( ; it_judge != m_mapActivejudgeDump.end() ; ++it_judge )
	{
		CJudgeDump* pJudgeDump = it_judge->second;
		char* pcText = pJudgeDump->GetDump_RecvPacket(pPacket);

		if( pcText )
		{
			RwInt32 iDumpType = it_judge->first;
			MAP_DUMPTARGET::iterator it;
			for(it = m_mapActiveTarget.begin(); it != m_mapActiveTarget.end(); ++it)
			{
				CNtlDumpTarget& pTarget = *(it->second);
				pTarget.Color(abyDUMP_COLOR[iDumpType][0], abyDUMP_COLOR[iDumpType][1], abyDUMP_COLOR[iDumpType][2]);
				pTarget << pcText;
				pTarget.Dump();
			}
		}
	}
}

void CDumpCmdManager::RecvNet(const char *pFormat)
{
	// 활용되지 않고 있음

	MAP_DUMPTARGET::iterator it;
	for(it = m_mapActiveTarget.begin(); it != m_mapActiveTarget.end(); ++it)
	{
		CNtlDumpTarget *pTarget = (*it).second;
		std::string str;
		str = "Client Net : ";
		str += pFormat;
		pTarget->Color(255, 255, 255);
		pTarget->Format("%s \n\n", str.c_str());
		pTarget->Dump(); 
	}
}

RwBool CDumpCmdManager::Command(const RwChar *pCmd)
{
    return m_pDBOCheatCommand->OnCmdParsing(pCmd);

	//return CmdParsing(pCmd);
}

RwBool CDumpCmdManager::Command(const WCHAR *pCmd)
{
	if (!Logic_IsDevUser())
	{
		return FALSE;
	}

	char chBuffer[1024];
	::WideCharToMultiByte(GetACP(), 0, pCmd, -1, chBuffer, 1024, NULL, NULL);

	return Command(chBuffer);
}

void CDumpCmdManager::CleintConsoleCommand(const RwChar *pCmd)
{
	if( m_pDBOCheatCommand->OnCmdParsing(pCmd) )
	{
		HWND hWnd = ::FindWindow(NULL, "CleintConsole");
		if( hWnd == NULL )
			return;

		COPYDATASTRUCT cds;
		char acBuffer[256] = "Command success";

		cds.dwData		= 0;
		cds.cbData		= strlen(acBuffer);
		cds.lpData		= acBuffer;

		::SendMessage(hWnd, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)&cds);
	}
}

void CDumpCmdManager::OutPutClientMessage(eDUMP_TYPE eDumpType, const RwChar* pMessage)
{
	MAP_JUDGEDUMP::iterator it_judge = m_mapActivejudgeDump.find(eDumpType);
	if( it_judge != m_mapActivejudgeDump.end() )
	{
		CJudgeDump* pJudgeDump = it_judge->second;
		const char* pcText = pJudgeDump->GetDump_Message(pMessage);

		if( pcText )
		{
			RwInt32 iDumpType = it_judge->first;
			MAP_DUMPTARGET::iterator it;
			for(it = m_mapActiveTarget.begin(); it != m_mapActiveTarget.end(); ++it)
			{
				CNtlDumpTarget& pTarget = *(it->second);
				pTarget.Color(abyDUMP_COLOR[iDumpType][0], abyDUMP_COLOR[iDumpType][1], abyDUMP_COLOR[iDumpType][2]);
				pTarget << pcText << "\n\n";
				pTarget.Dump();
			}
		}
	}
}

void CDumpCmdManager::OutPutClientMessage(eDUMP_TYPE eDumpType, const WCHAR* pMessage)
{
	MAP_JUDGEDUMP::iterator it_judge = m_mapActivejudgeDump.find(eDumpType);
	if( it_judge != m_mapActivejudgeDump.end() )
	{
		CJudgeDump* pJudgeDump = it_judge->second;
		const char* pcText = pJudgeDump->GetDump_Message(pMessage);

		if( pcText )
		{
			RwInt32 iDumpType = it_judge->first;
			MAP_DUMPTARGET::iterator it;
			for(it = m_mapActiveTarget.begin(); it != m_mapActiveTarget.end(); ++it)
			{
				CNtlDumpTarget& pTarget = *(it->second);
				pTarget.Color(abyDUMP_COLOR[iDumpType][0], abyDUMP_COLOR[iDumpType][1], abyDUMP_COLOR[iDumpType][2]);
				pTarget << pcText << "\n\n";
				pTarget.Dump();
			}
		}
	}
}

void CDumpCmdManager::OutputSobObj(RwUInt32 uiSerialId)
{
	// Dump 플래그가 아닌 별도의 치트를 통해서 활성화할 수 있다

	MAP_DUMPTARGET::iterator it;
	for(it = m_mapActiveTarget.begin(); it != m_mapActiveTarget.end(); ++it)
	{
		OutputSobObj(*(*it).second, uiSerialId); 
	}
}

void CDumpCmdManager::OutputSobObj(CNtlDumpTarget &dump, RwUInt32 uiSeiralId)
{
	// Dump 플래그가 아닌 별도의 치트를 통해서 활성화할 수 있다

	dump.Color(0, 255, 0);
	dump << "\n\n\n\n================ simulation object dump(begin) =====================\n";
	dump.Dump();
	dump.Color(255, 255, 255);

	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(uiSeiralId);
	if(pSobObj == NULL)
	{
		dump << "This target does not exist\n";
		dump.Dump();
	}
	else
	{
		pSobObj->Dump(dump);
	}

	dump.Color(0, 255, 0);
	dump << "================ simulation object dump(end) =====================\n";
	dump.Dump(); 
	dump.Color(255, 255, 255);
}

/// woosungs_test 20090723
void CDumpCmdManager::OutputDump(const RwChar *pMsg, RwUInt32 iR, RwUInt32 iG, RwUInt32 iB)
{
	// Dump 플래그가 아닌 별도의 치트를 통해서 활성화할 수 있다

	MAP_DUMPTARGET::iterator it;
	for(it = m_mapActiveTarget.begin(); it != m_mapActiveTarget.end(); ++it)
	{
		it->second->Color(iR, iG, iB);
		*(it->second) << pMsg;
		it->second->Dump();
		it->second->Color(255, 255, 255);
	}
}

RwBool CDumpCmdManager::IsOutputRecvPacketTarget(SERIAL_HANDLE hSerial)
{
	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if(pSobAvatar->GetSerialID() == hSerial || m_hTargetSerialId == hSerial || 
		m_hRegisterSerialId == hSerial)
		return TRUE;
	else 
		return FALSE;
}

void CDumpCmdManager::ActiveDumpTarget( EDumpTarget eDumpTarget, RwBool bActive ) 
{
    CNtlDumpTarget* pDumpTarget = FindCmdTarget(eDumpTarget);
    if(!pDumpTarget)
        return;

	pDumpTarget->Active(bActive);

	if( bActive )
	{
		m_mapActiveTarget[eDumpTarget] = pDumpTarget;
	}
	else
	{
		m_mapActiveTarget.erase(eDumpTarget);
	}
}

void CDumpCmdManager::ActiveDumpType(eDUMP_TYPE eDumpType, RwBool bActive)
{
	if( bActive )
	{
		switch(eDumpType)
		{
		case DUMP_DEFAULT:
			{
				CJudgeDump* pJudgeDump = NTL_NEW CJudgeDefaultDump;
				m_mapActivejudgeDump[DUMP_DEFAULT] = pJudgeDump;
				break;
			}
		case DUMP_ACTION:
			{
				CJudgeDump* pJudgeDump = NTL_NEW CJudgeActionDump;
				m_mapActivejudgeDump[DUMP_ACTION] = pJudgeDump;
				break;
			}
		case DUMP_SOB_CREATE_DESTROY:
			{
				CJudgeDump* pJudgeDump = NTL_NEW CJudgeSobCreateDestoryDump;
				m_mapActivejudgeDump[DUMP_SOB_CREATE_DESTROY] = pJudgeDump;
				break;
			}
		case DUMP_FSM:
			{
				CJudgeDump* pJudgeDump = NTL_NEW CJudgeFSMDump;
				m_mapActivejudgeDump[DUMP_FSM] = pJudgeDump;
				break;
			}
		case DUMP_CONTENTS:
			{
				CJudgeDump* pJudgeDump = NTL_NEW CJudgeContentsDump;
				m_mapActivejudgeDump[DUMP_CONTENTS] = pJudgeDump;
				break;
			}
		default:
			{
				NTL_ASSERTFAIL("CDumpCmdManager::ActiveDumpType, Invalid dump type");
				return;
			}
		}
	}
	else
	{
		MAP_JUDGEDUMP::iterator it_judge = m_mapActivejudgeDump.find(eDumpType);
		if( it_judge != m_mapActivejudgeDump.end() )
		{
			CJudgeDump* pJudgeDump = it_judge->second;
			NTL_DELETE(pJudgeDump);
			m_mapActivejudgeDump.erase(it_judge);
		}
	}
}
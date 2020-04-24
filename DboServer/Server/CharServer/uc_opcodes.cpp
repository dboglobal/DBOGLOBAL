#include "stdafx.h"
#include "uc_opcodes.h"
#include "ClientSession.h"


CUC_Opcodes::CUC_Opcodes()
{
	BuildOpcodeList();
}


CUC_Opcodes::~CUC_Opcodes()
{
	for (int i = 0; i < UC_OPCODE_END - UC_OPCODE_BEGIN; i++)
		delete aOpcodeHandler[i];
}


void CUC_Opcodes::BuildOpcodeList()
{
	for (int i = 0; i < UC_OPCODE_END - UC_OPCODE_BEGIN; i++)
		aOpcodeHandler[i] = new OpcodeHandler<CClientSession>;
		

	StoreOpcode(UC_LOGIN_REQ, "UC_LOGIN_REQ", PROCESS_THREADUNSAFE, &CClientSession::SendCharServerReq);
	StoreOpcode(UC_CHARACTER_SERVERLIST_REQ, "UC_CHARACTER_SERVERLIST_REQ", PROCESS_THREADUNSAFE, &CClientSession::SendServerListRes);
	StoreOpcode(UC_CHARACTER_SERVERLIST_ONE_REQ, "UC_CHARACTER_SERVERLIST_ONE_REQ", PROCESS_THREADUNSAFE, &CClientSession::LoadOneServerList);
	StoreOpcode(UC_CHARACTER_ADD_REQ, "UC_CHARACTER_ADD_REQ", PROCESS_THREADUNSAFE, &CClientSession::SendCharCreateReq);
	StoreOpcode(UC_CHARACTER_DEL_REQ, "UC_CHARACTER_DEL_REQ", PROCESS_THREADUNSAFE, &CClientSession::SendCharDeleteReq);
	StoreOpcode(UC_CHARACTER_SELECT_REQ, "UC_CHARACTER_SELECT_REQ", PROCESS_THREADUNSAFE, &CClientSession::SendCharSelectReq);
	StoreOpcode(UC_CHARACTER_EXIT_REQ, "UC_CHARACTER_EXIT_REQ", PROCESS_THREADUNSAFE, &CClientSession::SendCharExitReq);
	StoreOpcode(UC_CHARACTER_LOAD_REQ, "UC_CHARACTER_LOAD_REQ", PROCESS_THREADUNSAFE, &CClientSession::SendCharLoadReq);
	StoreOpcode(UC_CHARACTER_DEL_CANCEL_REQ, "UC_CHARACTER_DEL_CANCEL_REQ", PROCESS_THREADUNSAFE, &CClientSession::SendCancelCharDeleteReq);
	StoreOpcode(UC_CONNECT_WAIT_CHECK_REQ, "UC_CONNECT_WAIT_CHECK_REQ", PROCESS_THREADUNSAFE, &CClientSession::SendCharWaitCheckReq);
	StoreOpcode(UC_CONNECT_WAIT_CANCEL_REQ, "UC_CONNECT_WAIT_CANCEL_REQ", PROCESS_THREADUNSAFE, &CClientSession::SendCancelWaitReq);
	StoreOpcode(UC_CHARACTER_RENAME_REQ, "UC_CHARACTER_RENAME_REQ", PROCESS_THREADUNSAFE, &CClientSession::SendCharRenameReq);
}



#include "stdafx.h"
#include "qc_opcodes.h"
#include "QueryServerSession.h"


CQC_Opcodes::CQC_Opcodes()
{
	BuildOpcodeList();
}


CQC_Opcodes::~CQC_Opcodes()
{
	for (int i = 0; i < QC_OPCODE_END - QC_OPCODE_BEGIN; i++)
		delete aOpcodeHandler[i];
}


void CQC_Opcodes::BuildOpcodeList()
{
	for (int i = 0; i < QC_OPCODE_END - QC_OPCODE_BEGIN; i++)
		aOpcodeHandler[i] = new OpcodeHandler<CQueryServerSession>;


	StoreOpcode(QC_CHARACTER_ADD_RES, "QC_CHARACTER_ADD_RES", PROCESS_THREADUNSAFE, &CQueryServerSession::RecvCreateCharRes);
	StoreOpcode(QC_CHARACTER_LOAD_RES, "QC_CHARACTER_LOAD_RES", PROCESS_THREADUNSAFE, &CQueryServerSession::RecvCharacterLoadRes);
}



#ifndef _H_QG_OPCODES_
#define _H_QG_OPCODES_

#include "opcodes.h"
#include "NtlSingleton.h"
#include "NtlPacketQG.h"

class CQueryServerSession;

class CQG_Opcodes : public CNtlSingleton<CQG_Opcodes>
{

public:

	CQG_Opcodes();
	~CQG_Opcodes();

private:

	OpcodeHandler<CQueryServerSession>*			aOpcodeHandler[QG_OPCODE_END - QG_OPCODE_BEGIN];

public:

	void BuildOpcodeList();
	void StoreOpcode(WORD Opcode, char const* name, PacketProcessing process, void (CQueryServerSession::*handler)(CNtlPacket* pPacket))
	{
		OpcodeHandler<CQueryServerSession>* p = aOpcodeHandler[QG_OPCODE_END - Opcode];
		p->name = name;
		p->packetProcessing = process;
		p->handler = handler;
	}

	/// Lookup opcode
	inline OpcodeHandler<CQueryServerSession> const* LookupOpcode(WORD opcode) const
	{
		if (opcode > QG_OPCODE_END || opcode <= QG_OPCODE_BEGIN)
			return NULL;

		return aOpcodeHandler[QG_OPCODE_END - opcode];
	}
};

#define GetQG_OpcodeTable()			CQG_Opcodes::GetInstance()
#define qg_opcodeTable				GetQG_OpcodeTable()

#endif
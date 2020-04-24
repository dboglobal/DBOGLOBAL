#ifndef _H_GT_OPCODES_
#define _H_GT_OPCODES_

#include "opcodes.h"
#include "NtlSingleton.h"
#include "NtlPacketGT.h"

class CServerPassiveSession;

class CGT_Opcodes : public CNtlSingleton<CGT_Opcodes>
{

public:

	CGT_Opcodes();
	~CGT_Opcodes();

private:

	OpcodeHandler<CServerPassiveSession>*			aOpcodeHandler[GT_OPCODE_END - GT_OPCODE_BEGIN];

public:

	void BuildOpcodeList();
	void StoreOpcode(WORD Opcode, char const* name, PacketProcessing process, void (CServerPassiveSession::*handler)(CNtlPacket* pPacket))
	{
		OpcodeHandler<CServerPassiveSession>* p = aOpcodeHandler[GT_OPCODE_END - Opcode];
		p->name = name;
		p->packetProcessing = process;
		p->handler = handler;
	}

	/// Lookup opcode
	inline OpcodeHandler<CServerPassiveSession> const* LookupOpcode(WORD opcode) const
	{
		if (opcode > GT_OPCODE_END || opcode <= GT_OPCODE_BEGIN)
			return NULL;

		return aOpcodeHandler[GT_OPCODE_END - opcode];
	}
};

#define GetGT_OpcodeTable()			CGT_Opcodes::GetInstance()
#define gt_opcodeTable				GetGT_OpcodeTable()


#endif
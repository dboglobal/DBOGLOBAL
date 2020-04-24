#ifndef _H_UC_OPCODES_
#define _H_UC_OPCODES_

#include "opcodes.h"
#include "NtlSingleton.h"
#include "NtlPacketUC.h"


class CClientSession;


class CUC_Opcodes : public CNtlSingleton<CUC_Opcodes>
{

public:

	CUC_Opcodes();
	~CUC_Opcodes();

private:

	OpcodeHandler<CClientSession>*			aOpcodeHandler[UC_OPCODE_END - UC_OPCODE_BEGIN];

public:

	void BuildOpcodeList();
	void StoreOpcode(WORD Opcode, char const* name, PacketProcessing process, void (CClientSession::*handler)(CNtlPacket* pPacket))
	{
		OpcodeHandler<CClientSession>* p = aOpcodeHandler[UC_OPCODE_END - Opcode];
		p->name = name;
		p->packetProcessing = process;
		p->handler = handler;
	}

	/// Lookup opcode
	inline OpcodeHandler<CClientSession> const* LookupOpcode(WORD opcode) const
	{
		if (opcode > UC_OPCODE_END || opcode <= UC_OPCODE_BEGIN)
			return NULL;

		return aOpcodeHandler[UC_OPCODE_END - opcode];
	}
};

#define GetUC_OpcodeTable()			CUC_Opcodes::GetInstance()
#define uc_opcodeTable				GetUC_OpcodeTable()



#endif

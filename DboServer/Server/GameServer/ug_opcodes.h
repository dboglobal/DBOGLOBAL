#ifndef _H_UG_OPCODES
#define _H_UG_OPCODES

#include "opcodes.h"
#include "NtlSingleton.h"
#include "NtlPacketUG.h"

class CClientSession;


class CUG_Opcodes : public CNtlSingleton<CUG_Opcodes>
{

public:

	CUG_Opcodes();
	~CUG_Opcodes();

private:

	OpcodeHandler<CClientSession>*			aOpcodeHandler[UG_OPCODE_END - UG_OPCODE_BEGIN];

public:

	void BuildOpcodeList();
	void StoreOpcode(WORD Opcode, char const* name, PacketProcessing process, void (CClientSession::*handler)(CNtlPacket* pPacket))
	{
		OpcodeHandler<CClientSession>* p = aOpcodeHandler[UG_OPCODE_END - Opcode];
		p->name = name;
		p->packetProcessing = process;
		p->handler = handler;
	}

	/// Lookup opcode
	inline OpcodeHandler<CClientSession> const* LookupOpcode(WORD opcode) const
	{
		if (opcode > UG_OPCODE_END || opcode <= UG_OPCODE_BEGIN)
			return NULL;

		return aOpcodeHandler[UG_OPCODE_END - opcode];
	}
};

#define GetUGOpcodeTable()			CUG_Opcodes::GetInstance()
#define ug_opcodeTable				GetUGOpcodeTable()


#endif
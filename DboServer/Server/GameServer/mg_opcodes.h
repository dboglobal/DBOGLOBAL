#ifndef _H_MG_OPCODES_
#define _H_MG_OPCODES_

#include "opcodes.h"
#include "NtlSingleton.h"
#include "NtlPacketMG.h"

class CMasterServerSession;

class CMG_Opcodes : public CNtlSingleton<CMG_Opcodes>
{

public:

	CMG_Opcodes();
	~CMG_Opcodes();

private:

	OpcodeHandler<CMasterServerSession>*			aOpcodeHandler[MG_OPCODE_END - MG_OPCODE_BEGIN];

public:

	void BuildOpcodeList();
	void StoreOpcode(WORD Opcode, char const* name, PacketProcessing process, void (CMasterServerSession::*handler)(CNtlPacket* pPacket))
	{
		OpcodeHandler<CMasterServerSession>* p = aOpcodeHandler[MG_OPCODE_END - Opcode];
		p->name = name;
		p->packetProcessing = process;
		p->handler = handler;
	}

	/// Lookup opcode
	inline OpcodeHandler<CMasterServerSession> const* LookupOpcode(WORD opcode) const
	{
		if (opcode > MG_OPCODE_END || opcode <= MG_OPCODE_BEGIN)
			return NULL;

		return aOpcodeHandler[MG_OPCODE_END - opcode];
	}
};

#define GetMG_OpcodeTable()			CMG_Opcodes::GetInstance()
#define mg_opcodeTable				GetMG_OpcodeTable()

#endif
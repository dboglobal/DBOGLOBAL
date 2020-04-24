#ifndef _H_MC_OPCODES_
#define _H_MC_OPCODES_

#include "opcodes.h"
#include "NtlSingleton.h"
#include "NtlPacketMC.h"


class CMasterServerSession;


class CMC_Opcodes : public CNtlSingleton<CMC_Opcodes>
{

public:

	CMC_Opcodes();
	~CMC_Opcodes();

private:

	OpcodeHandler<CMasterServerSession>*			aOpcodeHandler[MC_OPCODE_END - MC_OPCODE_BEGIN];

public:

	void BuildOpcodeList();
	void StoreOpcode(WORD Opcode, char const* name, PacketProcessing process, void (CMasterServerSession::*handler)(CNtlPacket* pPacket))
	{
		OpcodeHandler<CMasterServerSession>* p = aOpcodeHandler[MC_OPCODE_END - Opcode];
		p->name = name;
		p->packetProcessing = process;
		p->handler = handler;
	}

	/// Lookup opcode
	inline OpcodeHandler<CMasterServerSession> const* LookupOpcode(WORD opcode) const
	{
		if (opcode > MC_OPCODE_END || opcode <= MC_OPCODE_BEGIN)
			return NULL;

		return aOpcodeHandler[MC_OPCODE_END - opcode];
	}
};

#define GetMC_OpcodeTable()			CMC_Opcodes::GetInstance()
#define mc_opcodeTable				GetMC_OpcodeTable()



#endif

#ifndef _H_TG_OPCODES_
#define _H_TG_OPCODES_

#include "opcodes.h"
#include "NtlSingleton.h"
#include "NtlPacketTG.h"

class CChatServerSession;

class CTG_Opcodes : public CNtlSingleton<CTG_Opcodes>
{

public:

	CTG_Opcodes();
	~CTG_Opcodes();

private:

	OpcodeHandler<CChatServerSession>*			aOpcodeHandler[TG_OPCODE_END - TG_OPCODE_BEGIN];

public:

	void BuildOpcodeList();
	void StoreOpcode(WORD Opcode, char const* name, PacketProcessing process, void (CChatServerSession::*handler)(CNtlPacket* pPacket))
	{
		OpcodeHandler<CChatServerSession>* p = aOpcodeHandler[TG_OPCODE_END - Opcode];
		p->name = name;
		p->packetProcessing = process;
		p->handler = handler;
	}

	/// Lookup opcode
	inline OpcodeHandler<CChatServerSession> const* LookupOpcode(WORD opcode) const
	{
		if (opcode > TG_OPCODE_END || opcode <= TG_OPCODE_BEGIN)
			return NULL;

		return aOpcodeHandler[TG_OPCODE_END - opcode];
	}
};

#define GetTG_OpcodeTable()			CTG_Opcodes::GetInstance()
#define tg_opcodeTable				GetTG_OpcodeTable()

#endif
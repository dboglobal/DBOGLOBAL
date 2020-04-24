#ifndef _H_UT_OPCODES_
#define _H_UT_OPCODES_

#include "opcodes.h"
#include "NtlSingleton.h"
#include "NtlPacketUT.h"


class CClientSession;


class CUT_Opcodes : public CNtlSingleton<CUT_Opcodes>
{

public:

	CUT_Opcodes();
	~CUT_Opcodes();

private:

	OpcodeHandler<CClientSession>*			aOpcodeHandler[UT_OPCODE_END - UT_OPCODE_BEGIN];

public:

	void BuildOpcodeList();
	void StoreOpcode(WORD Opcode, char const* name, PacketProcessing process, void (CClientSession::*handler)(CNtlPacket* pPacket))
	{
		OpcodeHandler<CClientSession>* p = aOpcodeHandler[UT_OPCODE_END - Opcode];
		p->name = name;
		p->packetProcessing = process;
		p->handler = handler;
	}

	/// Lookup opcode
	inline OpcodeHandler<CClientSession> const* LookupOpcode(WORD opcode) const
	{
		if (opcode > UT_OPCODE_END || opcode <= UT_OPCODE_BEGIN)
			return NULL;

		return aOpcodeHandler[UT_OPCODE_END - opcode];
	}
};

#define GetUT_OpcodeTable()			CUT_Opcodes::GetInstance()
#define ut_opcodeTable				GetUT_OpcodeTable()



#endif

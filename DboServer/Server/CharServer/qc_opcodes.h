#ifndef _H_QC_OPCODES_
#define _H_QC_OPCODES_

#include "opcodes.h"
#include "NtlSingleton.h"
#include "NtlPacketQC.h"


class CQueryServerSession;


class CQC_Opcodes : public CNtlSingleton<CQC_Opcodes>
{

public:

	CQC_Opcodes();
	~CQC_Opcodes();

private:

	OpcodeHandler<CQueryServerSession>*			aOpcodeHandler[QC_OPCODE_END - QC_OPCODE_BEGIN];

public:

	void BuildOpcodeList();
	void StoreOpcode(WORD Opcode, char const* name, PacketProcessing process, void (CQueryServerSession::*handler)(CNtlPacket* pPacket))
	{
		OpcodeHandler<CQueryServerSession>* p = aOpcodeHandler[QC_OPCODE_END - Opcode];
		p->name = name;
		p->packetProcessing = process;
		p->handler = handler;
	}

	/// Lookup opcode
	inline OpcodeHandler<CQueryServerSession> const* LookupOpcode(WORD opcode) const
	{
		if (opcode > QC_OPCODE_END || opcode <= QC_OPCODE_BEGIN)
			return NULL;

		return aOpcodeHandler[QC_OPCODE_END - opcode];
	}
};

#define GetQC_OpcodeTable()			CQC_Opcodes::GetInstance()
#define qc_opcodeTable				GetQC_OpcodeTable()



#endif

#ifndef _H_OPCODES_
#define _H_OPCODES_

class CNtlPacket;


enum PacketProcessing
{
	PROCESS_INPLACE = 0,   ///< process packet whenever we receive it
	PROCESS_THREADUNSAFE  ///< packet is not thread-safe - process it in \ref CNtlSession::Update (Main Thread)
};


template<class TYPE>
struct OpcodeHandler
{
	OpcodeHandler()
	{
		name = "<none>";
		packetProcessing = PROCESS_INPLACE;
		handler = &TYPE::OnInvalid;
	}

	///A string representation of the name of this opcode (see \ref Opcodes)
	char const* name;
	///This tells where the packet should be processed, ie: is it thread un/safe, which in turn
	///determines where it will be processed
	PacketProcessing packetProcessing;

	void (TYPE::*handler)(CNtlPacket* pPacket); //function example OnInvalid(CNtlPacket* pPacket) ( CClientSession::OnInvalid(CNtlPacket* pPacket) )
};


#endif
#pragma once


#pragma warning(disable:4819) // vs2005 codepage bug disable

#include "NtlSharedDef.h"

#include <string>


#pragma warning(disable : 4328)



//------------------------------------------------------------------
//
//------------------------------------------------------------------

//#pragma pack(push, 1)  /* push current alignment to stack */
//struct sNTLPACKETHEADER
//{
//	sNTLPACKETHEADER(WORD wGivenOpCode) :
//		wOpCode(wGivenOpCode) {}
//	
//	BYTE	null;
//	WORD	packetSequence;
//	QWORD	checksumm;
//	WORD	wOpCode;
//};
//#pragma pack(pop)   /* restore original alignment from stack */

struct sNTLPACKETHEADER
{
	sNTLPACKETHEADER(WORD wGivenOpCode) :
		wOpCode(wGivenOpCode) {}

	WORD	wOpCode;
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
#define BEGIN_PROTOCOL(opcode)						\
struct s##opcode :									\
	public sNTLPACKETHEADER							\
{													\
	s##opcode() :									\
		sNTLPACKETHEADER(opcode)					\
	{												\
	}

#define END_PROTOCOL()	};

//------------------------------------------------------------------
//
//------------------------------------------------------------------
#define BEGIN_PROTOCOL_IDENTITY( opcode, identity )	\
typedef s##identity s##opcode						\

#define END_PROTOCOL_IDENTITY()	;
//------------------------------------------------------------------


//------------------------------------------------------------------
#define DECLARE_PACKET_NAME( opcode )	{ #opcode }
//------------------------------------------------------------------
#include "StdAfx.h"
#include "NtlPacketUtil.h"
#include "NtlPacketAll.h"

const char* NtlGetPacketName(WORD wOpCode)
{
	if ( 0 <= wOpCode && UA_OPCODE_BEGIN > wOpCode )
	{		
		return "RESERVED SYSTEM PACKET";
	}
	else if (UA_OPCODE_BEGIN <= wOpCode && UA_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_UA( wOpCode );
	}
	else if (AU_OPCODE_BEGIN <= wOpCode && AU_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_AU( wOpCode );
	}
	else if (UC_OPCODE_BEGIN <= wOpCode && UC_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_UC( wOpCode );
	}
	else if (CU_OPCODE_BEGIN <= wOpCode && CU_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_CU( wOpCode );
	}
	else if (UG_OPCODE_BEGIN <= wOpCode && UG_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_UG( wOpCode );
	}
	else if (GU_OPCODE_BEGIN <= wOpCode && GU_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_GU( wOpCode );
	}
	else if (UT_OPCODE_BEGIN <= wOpCode && UT_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_UT( wOpCode );
	}
	else if (MA_OPCODE_BEGIN <= wOpCode && MA_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_MA( wOpCode );
	}
	else if (AM_OPCODE_BEGIN <= wOpCode && AM_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_AM( wOpCode );
	}
	else if (MC_OPCODE_BEGIN <= wOpCode && MC_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_MC( wOpCode );
	}
	else if (CM_OPCODE_BEGIN <= wOpCode && CM_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_CM( wOpCode );
	}
	else if (MG_OPCODE_BEGIN <= wOpCode && MG_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_MG( wOpCode );
	}
	else if (GM_OPCODE_BEGIN <= wOpCode && GM_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_GM( wOpCode );
	}
	else if (MT_OPCODE_BEGIN <= wOpCode && MT_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_MT( wOpCode );
	}
	else if (TM_OPCODE_BEGIN <= wOpCode && TM_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_TM( wOpCode );
	}
	else if (CQ_OPCODE_BEGIN <= wOpCode && CQ_OPCODE_END >= wOpCode)
	{
			return NtlGetPacketName_CQ( wOpCode );
	}
	else if (QC_OPCODE_BEGIN <= wOpCode && QC_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_QC( wOpCode );
	}
	else if (GN_OPCODE_BEGIN <= wOpCode && GN_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_GN( wOpCode );
	}
	else if (NG_OPCODE_BEGIN <= wOpCode && NG_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_NG( wOpCode );
	}
	else if (GQ_OPCODE_BEGIN <= wOpCode && GQ_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_GQ( wOpCode );
	}
	else if (QG_OPCODE_BEGIN <= wOpCode && QG_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_QG( wOpCode );
	}
	else if (GT_OPCODE_BEGIN <= wOpCode && GT_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_GT( wOpCode );
	}
	else if (TG_OPCODE_BEGIN <= wOpCode && TG_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_TG( wOpCode );
	}
	else if (TL_OPCODE_BEGIN <= wOpCode && TL_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_TL( wOpCode );
	}
	else if (QT_OPCODE_BEGIN <= wOpCode && QT_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_QT( wOpCode );
	}
	else if (TQ_OPCODE_BEGIN <= wOpCode && TQ_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_TQ( wOpCode );
	}
	else if (XR_OPCODE_BEGIN <= wOpCode && XR_OPCODE_END >= wOpCode)
	{
		return NtlGetPacketName_XR( wOpCode );
	}
	else
	{
		return "NOT DEFINED PACKET ID";
	}
}



bool NtlCheckPacketName(FILE * fp /* = NULL */)
{
	bool bPacketNameSuccess = true;

	const char * pTest = NULL;
	for( WORD i = 0; i < USHRT_MAX; i++ )
	{
		pTest = NtlGetPacketName( i );
		if( 0 == strncmp( pTest, "RESERVED SYSTEM PACKET", strlen( pTest) ) )
		{			
		}
		else if( 0 == strncmp( pTest, "NOT DEFINED PACKET ID", strlen( pTest) ) )
		{
		}
		else if( 0 == strncmp( pTest, "NOT DEFINED PACKET NAME : OPCODE LOW", strlen( pTest) ) )
		{
			if( fp ) fprintf_s( fp, "PacketID[%d] PacketName[%s]", i, NtlGetPacketName( i ) );
			bPacketNameSuccess = false;
		}
		else if( 0 == strncmp( pTest, "NOT DEFINED PACKET NAME : OPCODE HIGH", strlen( pTest) ) )
		{
			if( fp ) fprintf_s( fp, "PacketID[%d] PacketName[%s]", i, NtlGetPacketName( i ) );
			bPacketNameSuccess = false;
		}
		else if( 0 == strncmp( pTest, "OPCODE BUFFER OVERFLOW", strlen( pTest) ) )
		{
			if( fp ) fprintf_s( fp, "PacketID[%d] PacketName[%s]", i, NtlGetPacketName( i ) );
			bPacketNameSuccess = false;
		}
		else
		{
			//fprintf_s( TRACE_APP, "PacketID[%d] PacketName[%s]", i, NtlGetPacketName( i ) );
		}
	}

	return bPacketNameSuccess;
}
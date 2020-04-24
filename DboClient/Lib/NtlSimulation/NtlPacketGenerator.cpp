#include "precomp_ntlsimulation.h"
#include "NtlPacketGenerator.h"
#include "NtlDebug.h"
#include "NtlClientNet.h"
#include "NtlNetSender.h"

CNtlPacketGenerator::CNtlPacketGenerator(CNtlNetSender *pNetSender)
{
	m_pNetSender = pNetSender;
}

CNtlPacketGenerator::~CNtlPacketGenerator()
{
//	if(m_pNetSender)
//	{
//		NTL_DELETE(m_pNetSender);
//	}
}


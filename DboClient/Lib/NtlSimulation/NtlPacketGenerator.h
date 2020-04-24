/*****************************************************************************
 *
 * File			: NtlPacketGenerator.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 9. 27	
 * Abstract		: simulation layer packet generator
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_PACKET_GENERATOR_H__
#define __NTL_PACKET_GENERATOR_H__

class CNtlNetSender;
class CNtlClientNetworkLayer;

class CNtlPacketGenerator
{
protected:

	CNtlNetSender *m_pNetSender;

public:

	CNtlPacketGenerator(CNtlNetSender *pNetSender);
	virtual ~CNtlPacketGenerator();

	CNtlNetSender* GetNetSender(void) { return m_pNetSender; }
};


#endif
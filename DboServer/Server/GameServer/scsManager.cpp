#include "stdafx.h"
#include "scsManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include <iostream>
#include <fstream>

#include "CPlayer.h"
#include "NtlPacketGU.h"



CScsManager::CScsManager()
{
}

CScsManager::~CScsManager()
{
}


void CScsManager::TestGenerate(CPlayer * pPlayer)
{
	CNtlPacket packet(sizeof(sGU_SCS_CHECK_REQ));
	sGU_SCS_CHECK_REQ * res = (sGU_SCS_CHECK_REQ *)packet.GetPacketData();
	res->wOpCode = GU_SCS_CHECK_REQ;
	res->byCount = 15; //must be 15
	memset(res->achData, NULL, sizeof(res->achData));

	//std::ifstream image;
	//image.open("C:\\Users\\Administrator\\Desktop\\DBOG\\resource\\server_data\\scs\\aaa.jpg", std::ios_base::binary);

	////get length of file:
	//image.seekg(0, image.end);
	//int n = image.tellg();
	//image.seekg(0, image.beg);

	//printf("n: %u \n", n);

	//if(n < NTL_SCS_IMAGE_PACKET_PER_LENGTH)
	//	image.read(res->achData, n);


	packet.SetPacketLen(sizeof(sGU_SCS_CHECK_REQ));
	//pPlayer->SendPacket(&packet);
}


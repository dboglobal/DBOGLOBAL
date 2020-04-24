#pragma once

class CDTServer
{
private:

	static CDTServer *m_pInstace;

	RwUInt32 m_uiTargetSerialId;

	typedef struct _SPacketData
	{
		int nSize;
		char chData[4096];
	}SPacketData;
	typedef std::list<SPacketData*> ListPacket;
	ListPacket m_listPacket;

private:

	void PacketProcTargetSelect(void *pPacket);
	void PacketProcHTBReq(void *pPacket);

	void PacketProc(RwInt32 iDataLen, void *pPacket);

public:

	CDTServer(void);
	~CDTServer(void);

	static CDTServer* GetInstance(void);

	void Update(RwReal fElapsed);

	void PushPacket(RwInt32 iDataLen, void *pPacket);
};


bool DTServerPacketProc(RwInt32 iDataLen, void *pData);

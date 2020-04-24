#pragma once


#define dGET_BGM_MANAGER() (g_pNtlWorldBGMManager)


#include "ceventhandler.h"
#include "NtlSoundDefines.h"

using std::vector;
using std::string;
#include <vector>
#include <string>


class CNtlWorldBGMManager : public RWS::CEventHandler
{
public:
	CNtlWorldBGMManager(void);
	virtual ~CNtlWorldBGMManager(void);

private:
	RwInt32			m_IdxBGMTbl;
	SOUND_HANDLE    m_hBGM;

	RwInt32			m_IdxEnvTbl;
	SOUND_HANDLE    m_hEnv;

public:
	virtual void HandleEvents(RWS::CMsg &pMsg);

public:
	const RwChar*			GetBGMSoundName(RwInt32 _IdxBGMTbl);
	RwInt32					GetBGMSoundIdx(RwChar* pName);

	const RwChar*			GetEnvSoundName(RwInt32 _IdxEnvTbl);
	RwInt32					GetEnvSoundIdx(RwChar* pName);

	const RwChar*			GetShareSoundName(RwInt32 _IdxShareTbl);
	RwInt32					GetShareSoundIdx(RwChar* pName);

	void Create();
	void Delete();
};

extern CNtlWorldBGMManager* g_pNtlWorldBGMManager;
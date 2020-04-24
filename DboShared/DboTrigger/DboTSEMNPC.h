#ifndef _DBO_TSEMNPC_H_
#define _DBO_TSEMNPC_H_


/** 
	Event mapper npc
	특정 NPC가 제공할 수 있는 Trigger id를 외부에 제공할 책임을 가진다
*/


class CDboTSEMNPC : public CNtlTSEvtMapper
{
	NTL_TS_DECLARE_RTTI

// Constructions and Destructions
public:
	CDboTSEMNPC( void );
	virtual ~CDboTSEMNPC( void );

// Methods
public:
	virtual bool							AddBuildData( const std::string& strKey, const CNtlTSMain::mapdef_TLIST& TList, void* pTblList, void* pParameter );
	virtual bool							DelBuildData( const std::string& strKey );
};

#endif
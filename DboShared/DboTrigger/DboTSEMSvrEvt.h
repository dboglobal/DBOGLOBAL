#ifndef _DBO_TSEMSVREVT_H_
#define _DBO_TSEMSVREVT_H_


/** 
	Event mapper server event
	특정 server event가 제공할 수 있는 Trigger id를 외부에 제공할 책임을 가진다
*/


class CDboTSEMSvrEvt : public CNtlTSEvtMapper
{
	NTL_TS_DECLARE_RTTI

// Constructions and Destructions
public:
	CDboTSEMSvrEvt( void );
	virtual ~CDboTSEMSvrEvt( void );

// Methods
public:
	virtual bool							AddBuildData( const std::string& strKey, const CNtlTSMain::mapdef_TLIST& TList, void* pTblList, void* pParameter );
	virtual bool							DelBuildData( const std::string& strKey );
};

#endif
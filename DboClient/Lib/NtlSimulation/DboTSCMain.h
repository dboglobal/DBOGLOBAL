#ifndef _DBO_TSCMAIN_H_
#define _DBO_TSCMAIN_H_


#include "DboTSCore.h"


struct sUZ_FILEINFO;


class CDboTSEntityFastFinder;
class CNPCTable;
class CObjectTable;
class CDboTSCQAgency;
class CDboTSCTAgency;
class CDboTSCObjAgency;
class CDboTSCQRecv;
class CDboTSCTRecv;
class CNtlUnzip;


/** 
	Client TS main
*/


class CDboTSCMain : public CDboTSMain
{
	NTL_TS_DECLARE_RTTI

// Declarations
public:
	typedef unsigned __int64 NPC_POS_KEY;

	struct sNPC_POS_INFO
	{
		RwV3d							Pos;
	};

	typedef std::map< NPC_POS_KEY, sNPC_POS_INFO > mapdef_NPC_POS_LIST;

// Member variables
public:
	static CDboTSCMain*					s_pInstance;

protected:
	bool								m_bCreated;

	// Path
	std::string							m_strQuestPath;
	std::string							m_strTriggerPath;
	std::string							m_strObjectPath;

	// Entity fast finder for quest search system
	CDboTSEntityFastFinder*				m_pEntityFastFinder;

	// Agency
	CDboTSCQAgency*						m_pQAgency;
	CDboTSCTAgency*						m_pTAgency;
	CDboTSCObjAgency*					m_pObjAgency;

	// Trigger system
	bool								m_bSchedulingLoad;

	CNtlUnzip*							m_pclQuestZip;
	CNtlTSMain::mapdef_TLIST			m_defQuest;

	CNtlUnzip*							m_pclTriggerZip;
	CNtlTSMain::mapdef_TLIST			m_defTrigger;

	CNtlUnzip*							m_pclObjectZip;
	CNtlTSMain::mapdef_TLIST			m_defObject;

	// Trigger receiver
	CDboTSCQRecv*						m_pQRecv;
	CDboTSCTRecv*						m_pTRecv;

	// NPC Position list
	mapdef_NPC_POS_LIST					m_defNPCPosList;

// Constructions and Destructions
public:
	CDboTSCMain( void );
	virtual ~CDboTSCMain( void );

// Methods
public:
	bool								IsCreated( void );

	virtual bool						Create( void );
	virtual void						Delete( void );

	CDboTSCQAgency*						CreateAgency_Quest( void );
	void								DeleteAgency_Quest( void );

	CDboTSCTAgency*						CreateAgency_Trigger( void );
	void								DeleteAgency_Trigger( void );

	CDboTSCObjAgency*					CreateAgency_Object( void );
	void								DeleteAgency_Object( void );

	CDboTSEntityFastFinder*				GetEntityFastFinder( void );

	CNtlTSTrigger*						FindQuestFromTS( NTL_TS_T_ID tId );
	CNtlTSMain::mapdef_TLISTC_CIT		BeginQuestFromTS( void ) const;
	CNtlTSMain::mapdef_TLISTC_CIT		EndQuestFromTS( void ) const;

	CNtlTSTrigger*						FindTriggerFromTS( NTL_TS_T_ID tId );
	CNtlTSMain::mapdef_TLISTC_CIT		BeginTriggerFromTS( void ) const;
	CNtlTSMain::mapdef_TLISTC_CIT		EndTriggerFromTS( void ) const;

	CNtlTSTrigger*						FindObjectFromTS( NTL_TS_T_ID tId );
	CNtlTSMain::mapdef_TLISTC_CIT		BeginObjectFromTS( void ) const;
	CNtlTSMain::mapdef_TLISTC_CIT		EndObjectFromTS( void ) const;

	CDboTSCQAgency*						GetQAgency( void );
	CDboTSCTAgency*						GetTAgency( void );

	CDboTSCQRecv*						GetQRecv( void );
	CDboTSCTRecv*						GetTRecv( void );

	RwV3d*								FindNPCPosition( TBLIDX WorldIdx, TBLIDX NPCIdx );

// Implementations
protected:
	virtual bool						LoadLog( void );
	virtual void						UnloadLog( void );

	virtual bool						LoadFactories( void );
	virtual void						UnloadFactories( void );

	virtual bool						LoadScripts( void );
	virtual void						UnloadAllScripts( void );

	virtual bool						LoadEventMappers( void );
	virtual void						UnloadEventMappers( void );

	virtual bool						LoadRecv( void );
	virtual void						UnloadRecv( void );

	void								RegisterNPCPosition( TBLIDX WorldIdx, TBLIDX NPCIdx, RwV3d& v3Pos );
	void								ClearAllNPCPosition( void );

	void								LoadEventMapper( sUZ_FILEINFO* pFileInfo, char* pBuf );

	bool								HasEventMapperFromFile( void );
	bool								LoadEventMapperFromFile( void );
	bool								LoadEventMapperFromRunTime( void );
};

static CDboTSCMain* GetTSCMain(void)
{
	return CDboTSCMain::s_pInstance;
}


inline CDboTSEntityFastFinder* CDboTSCMain::GetEntityFastFinder( void )
{
	return m_pEntityFastFinder;
}


inline CDboTSCQAgency* CDboTSCMain::GetQAgency( void )
{
	return m_pQAgency;
}

inline CDboTSCTAgency* CDboTSCMain::GetTAgency( void )
{
	return m_pTAgency;
}

inline CDboTSCQRecv* CDboTSCMain::GetQRecv( void )
{
	return m_pQRecv;
}

inline CDboTSCTRecv* CDboTSCMain::GetTRecv( void )
{
	return m_pTRecv;
}


#endif
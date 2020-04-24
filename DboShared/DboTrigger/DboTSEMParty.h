#ifndef _DBO_TSEMPARTY_H_
#define _DBO_TSEMPARTY_H_


/** 
	Event mapper party
*/


class CDboTSEMParty : public CNtlTSEvtMapper
{
	NTL_TS_DECLARE_RTTI

// Declarations
public:
	typedef std::map< std::string, vecdef_TID_LIST > mapdef_PARTY_MAPPER;

// Member variables
protected:
	mapdef_PARTY_MAPPER						m_defPartyMapper;

// Constructions and Destructions
public:
	CDboTSEMParty( void );
	virtual ~CDboTSEMParty( void );

// Methods
public:
	virtual const vecdef_TID_LIST*			FindTSList( unsigned int uiId, const std::string& strKey ) const;

	virtual void							Clear( void );

	virtual bool							Load( const std::string& strFileName );
	virtual bool							Load( const char* pData, int nDataSize );

	virtual	bool							Save( const std::string& strFileName );

	virtual bool							AddBuildData( const std::string& strKey, const CNtlTSMain::mapdef_TLIST& TList, void* pTblList, void* pParameter );
	virtual bool							DelBuildData( const std::string& strKey );
};

#endif
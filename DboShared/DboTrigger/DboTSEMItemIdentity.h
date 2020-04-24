#ifndef _DBO_TSEMITEMIDENTITY_H_
#define _DBO_TSEMITEMIDENTITY_H_


/** 
	Event mapper item identity
*/


class CDboTSEMItemIdentity : public CNtlTSEvtMapper
{
	NTL_TS_DECLARE_RTTI

// Declarations
public:
	typedef std::map< std::string, vecdef_TID_LIST > mapdef_ITEMIDENTITY_MAPPER;

// Member variables
protected:
	mapdef_ITEMIDENTITY_MAPPER					m_defItemIdentityMapper;

// Constructions and Destructions
public:
	CDboTSEMItemIdentity( void );
	virtual ~CDboTSEMItemIdentity( void );

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
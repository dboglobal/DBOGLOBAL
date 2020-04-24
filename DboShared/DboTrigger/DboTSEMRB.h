#ifndef _DBO_TSEMRB_H_
#define _DBO_TSEMRB_H_


/** 
	Event mapper rank-battle
*/


class CDboTSEMRB : public CNtlTSEvtMapper
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	hashdef_TS_LIST							m_defTotalTSList;

// Constructions and Destructions
public:
	CDboTSEMRB( void );
	virtual ~CDboTSEMRB( void );

// Methods
public:
	virtual const vecdef_TID_LIST*			FindTSList( unsigned int uiId, const std::string& strKey ) const;

	virtual bool							Load( const std::string& strFileName );
	virtual bool							Load( const char* pData, int nDataSize );

	virtual	bool							Save( const std::string& strFileName );

	virtual bool							AddBuildData( const std::string& strKey, const CNtlTSMain::mapdef_TLIST& TList, void* pTblList, void* pParameter );
	virtual bool							DelBuildData( const std::string& strKey );
};

#endif
#ifndef _NTL_TSEVTMAPPER_H_
#define _NTL_TSEVTMAPPER_H_


#include "NtlTSControlObject.h"
#include "NtlTSMain.h"


/**
	Event mapper
*/


class CNtlTSEvtMapper : public CNtlTSControlObject
{
	NTL_TS_DECLARE_RTTI

// Declarations
public:
	typedef std::vector<NTL_TS_T_ID> vecdef_TID_LIST;
	typedef std::map<std::string, vecdef_TID_LIST> hashdef_TS_LIST;
	typedef std::map<unsigned int, hashdef_TS_LIST> mapdef_MAPPER;

// Member variables
protected:
	mapdef_MAPPER							m_defMapper;

// Constructions and Destructions
public:
	virtual ~CNtlTSEvtMapper( void ) { Clear(); }

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
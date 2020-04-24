#ifndef _DBO_TSEMMOB_H_
#define _DBO_TSEMMOB_H_


/** 
	Event mapper mob
*/


class CDboTSEMMob : public CNtlTSEvtMapper
{
	NTL_TS_DECLARE_RTTI

// Constructions and Destructions
public:
	CDboTSEMMob( void );
	virtual ~CDboTSEMMob( void );

// Methods
public:
	virtual bool							AddBuildData( const std::string& strKey, const CNtlTSMain::mapdef_TLIST& TList, void* pTblList, void* pParameter );
	virtual bool							DelBuildData( const std::string& strKey );
};

#endif
#ifndef _DBO_TSEMSIDEICON_H_
#define _DBO_TSEMSIDEICON_H_


/**
Event mapper SIDE ICON
*/


class CDboTSEMSideIcon : public CNtlTSEvtMapper
{
	NTL_TS_DECLARE_RTTI

	// Constructions and Destructions
public:
	CDboTSEMSideIcon(void);
	virtual ~CDboTSEMSideIcon(void);

	// Methods
public:

	virtual bool							AddBuildData(const std::string& strKey, const CNtlTSMain::mapdef_TLIST& TList, void* pTblList, void* pParameter);
	virtual bool							DelBuildData(const std::string& strKey);
};

#endif
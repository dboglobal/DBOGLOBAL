#ifndef _NTL_NAVI_RES_MNG_H_
#define _NTL_NAVI_RES_MNG_H_


//////////////////////////////////////////////////////////////////////////
//
//	CNtlNaviResMng
//
//////////////////////////////////////////////////////////////////////////


class CNtlNaviResMng
{
public:
	enum eNAVI_MESH_TYPE
	{
		eNAVI_MESH_TYPE_NAVI_MESH,
		eNAVI_MESH_TYPE_OBS_MESH,
		eNAVI_MESH_TYPE_INVALID
	};

	typedef std::multimap< std::string, std::string >	mmapdef_RES_NAME_LIST;
	typedef mmapdef_RES_NAME_LIST::iterator				mmapdef_RES_NAME_LIST_IT;
	typedef std::pair< mmapdef_RES_NAME_LIST_IT, mmapdef_RES_NAME_LIST_IT > pairdef_RES_NAME_LIST;

protected:
	mmapdef_RES_NAME_LIST		m_defNaviResList;
	mmapdef_RES_NAME_LIST		m_defObsResList;

	pairdef_RES_NAME_LIST		m_pairResNameList;
	mmapdef_RES_NAME_LIST_IT	m_defResNameListIT;

public:
	CNtlNaviResMng( void );
	~CNtlNaviResMng( void );

public:
	bool						Load( const char* pPath );

	const char*					BeginNaviRes( const char* pModelName );
	const char*					NextNaviRes( void );

	const char*					BeginObsRes( const char* pModelName );
	const char*					NextObsRes( void );

private:
	eNAVI_MESH_TYPE				SplitResName( const std::string& strFileName, std::string& strModelName );
};


#endif
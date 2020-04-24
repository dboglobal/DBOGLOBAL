#ifndef _NTL_NAVI_PEDATA_EXPORT_MNG_H_
#define _NTL_NAVI_PEDATA_EXPORT_MNG_H_


class CNtlNaviResMng;
class CNtlNaviWEWorld;


//////////////////////////////////////////////////////////////////////////
//
//	CNtlNaviPEDataExportMng
//
//////////////////////////////////////////////////////////////////////////


class CNtlNaviPEDataExportMng
{
public:
	typedef std::map< unsigned int, CNtlNaviWEWorld* > mapdef_WE_WORLD_LIST;
	


protected:
	CNtlNaviResMng*					m_pNaviResMng;
	mapdef_WE_WORLD_LIST			m_defWEWorldList;


public:
	CNtlNaviPEDataExportMng( void );
	~CNtlNaviPEDataExportMng( void );


public:
	bool							Create( void );

	void							Delete( void );

	bool							UpdateToolData( void );

	void							GetListImportedWorldIDList( vecdef_WorldIDList& vecOut );

	bool							ImportWorld( const char* pWorldPath );

	bool							ImportWorldAll( const char* pRootPath );

	bool							ExportWorldList( const char* pRootPath, mapdef_ExportList& list );

	bool							ExportWorldAll( const char* pRootPath );
};


#endif
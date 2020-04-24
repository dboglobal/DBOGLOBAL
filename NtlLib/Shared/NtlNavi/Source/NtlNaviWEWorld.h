#ifndef _NTL_NAVI_WE_WORLD_H_
#define _NTL_NAVI_WE_WORLD_H_


#include "NtlNaviEntity.h"
#include "NtlNaviODGroupExporter.h"
#include "NtlNaviIDGroupExporter.h"


class CNtlNaviResMng;
class CNtlNaviDataMng;


//////////////////////////////////////////////////////////////////////////
//
//	CNtlNaviWEWorld
//
//////////////////////////////////////////////////////////////////////////


class CNtlNaviWEWorld : public CNtlNaviEntityWE
{
public:
	typedef std::vector< CNtlNaviODGroupExporter* > vecdef_ODGROUP_EXPORTER_LIST;
	typedef std::vector< CNtlNaviIDGroupExporter* > vecdef_IDGROUP_EXPORTER_LIST;


protected:
	CNtlNaviResMng*					m_pNaviResMng;
	CNtlNaviDataMng*				m_pNaviDataMng;
	vecdef_ODGROUP_EXPORTER_LIST	m_defODGroupExporterList;
	vecdef_IDGROUP_EXPORTER_LIST	m_defIDGroupExporterList;


public:
	CNtlNaviWEWorld( CNtlNaviResMng* pNaviResMng );
	virtual ~CNtlNaviWEWorld( void );


public:
	virtual bool					ImportWEData( const char* pPath );
	virtual bool					ExportPEData( const char* pPath );
	virtual bool					ExportPEData( const char* pPath, vecdef_GroupIDList& list );
	virtual bool					UpdateToolData( void );

	unsigned int					GetWorldID( void );


protected:
	void							Destroy( void );

	bool							ExportODWorldInfo( const char* pExportPath );
	bool							ExportIDWorldInfo( const char* pExportPath );

	bool							ExportODWorldGroup( const char* pExportPath );
	bool							ExportODWorldProperty( const char* pExportPath );

	bool							ExportIDWorld( const char* pExportPath );
	bool							ExportIDWorldProperty( const char* pExportPath );

	bool							ExportODWorldGroup( const char* pExportPath, vecdef_GroupIDList& list );
	bool							ExportODWorldProperty( const char* pExportPath, vecdef_GroupIDList& list );

	void							AttachODGroupExporter( const char* pExportPath, CNtlNaviResMng* pNaviResMng, CNtlNaviDataMng* pNaviDataMng, const CNtlNaviODGroupExporter::sINPUT_PARAM& sInputParam );
	void							ClearODGroupExporter( void );

	void							AttachIDGroupExporter( const char* pExportPath, CNtlNaviResMng* pNaviResMng, CNtlNaviDataMng* pNaviDataMng, const CNtlNaviIDGroupExporter::sINPUT_PARAM& sInputParam );
	void							ClearIDGroupExporter( void );
};


#endif
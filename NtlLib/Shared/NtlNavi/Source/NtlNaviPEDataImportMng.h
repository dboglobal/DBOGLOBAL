#ifndef _NTL_NAVI_PEDATA_IMPORT_MNG_H_
#define _NTL_NAVI_PEDATA_IMPORT_MNG_H_


#include "NtlNaviLoadFinishCheckThread.h"


class CNtlNaviPEWorld;


//////////////////////////////////////////////////////////////////////////
//
//	CNtlNaviPEDataImportMng
//
//////////////////////////////////////////////////////////////////////////


class CNtlNaviPEDataImportMng : public CNtlNaviLoadFinishCheckThread
{
public:
	typedef std::map< unsigned int, CNtlNaviPEWorld* > mapdef_PE_WORLD_LIST;


protected:
	CNtlNaviCS						m_csLoadingFailed;
	bool							m_bLoadingFailed;

	CNtlNaviCS						m_csPEWorldList;
	mapdef_PE_WORLD_LIST			m_defPEWorldList;

	unsigned int					m_uiLoadBeginTime;
	unsigned int					m_uiLoadEndTime;


public:
	CNtlNaviPEDataImportMng( void );
	~CNtlNaviPEDataImportMng( void );


public:
	bool							Create( void );

	void							Delete( void );

	bool							WaitUntilLoadingFinish( unsigned int uiWaitTime, unsigned int& uiResult );

	unsigned int					GetLoadingTime( void );

	NAVI_INST_HANDLE				CreateInstanceHandler( unsigned int uiWorldID );

	void							DeleteInstanceHandler( NAVI_INST_HANDLE hHandle );

	CNtlNaviPEWorld*				FindNaviWorld( NAVI_INST_HANDLE hHandle );

	CNtlNaviPEWorld*				FindNaviWorld( unsigned int uiWorldID );

	bool							ImportWorlds( const char* pRootPath, vecdef_WorldIDList& defWorldIDList, unsigned char byLoadFlags );


protected:
	bool							IsLoadingFailed( void );

	void							SetLoadingFailed( bool bLoadingFailed );

	bool							ImportWorld( const char* pRootPath, unsigned int uiWorldID, unsigned char byLoadFlags );

	void							AttachWorld( unsigned int uiWorldID, CNtlNaviPEWorld* pPEWorld );

	void							ClearWorld( void );

	unsigned int					GetAttachedWorldCnt( void );

	CNtlNaviPEWorld*				GetAttachedWorld( int nIndex );

	virtual unsigned int			ThreadCallBackFunc( void );
};


#endif
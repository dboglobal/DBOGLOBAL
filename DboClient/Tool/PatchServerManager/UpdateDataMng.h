#ifndef _NTL_UPDATEDATAMNG_H_
#define _NTL_UPDATEDATAMNG_H_


#include "EventDef.h"
#include "PatchInfo.h"
#include "Patcher.h"
#include "IntegritySys.h"


class CUpdateDataMng
{
// Declarations
public:
	struct sFILE_INFO
	{
		bool					bZip;
		CString					strSrcPath;
		CString					strDestPath;
	};
	typedef std::map< CString, sFILE_INFO > mapdef_UpdateFileList;

// Member variables
protected:
	CString						m_strMngPath;

	CPatchHistory				m_clHistory;
	CPatcher					m_clRTPatcher;
	CIntegritySys				m_clIntegritySys;

	mapdef_UpdateFileList		m_defUpdateFileList;

// Constructions and Destructions
public:
	CUpdateDataMng( void );
	~CUpdateDataMng( void );

// Operations
public:
	bool						Create( CString& strMngPath );
	void						Delete( void );

	bool						BuildRollbackEntryPoint( void );
	bool						BuildUpdateData( void );
	bool						RunUpdateData( void );
	bool						FinishUpdateData( void );

	void						MessageProc( eEVENT eMsg, sEVENT_DATA_BASE* pData );

// Implementations
protected:
	bool						RegisterUpdateFile( CString& strSrcPath, CString& strDestPath, bool bZip );
	bool						RegisterUpdateFolder( CString& strSrcPath, CString& strDestPath, bool bZip );
};


#endif
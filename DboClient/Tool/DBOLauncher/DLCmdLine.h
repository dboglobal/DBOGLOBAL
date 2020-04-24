/*****************************************************************************
*
* File			: DLCmdLine.h
* Author		: 
* Copyright		: (аж)NTL
* Date			: 
* Abstract		: 
*****************************************************************************
* Desc          : 
*
*****************************************************************************/


#pragma once


//////////////////////////////////////////////////////////////////////////
//
// CDLCmdLine
//
//////////////////////////////////////////////////////////////////////////


class CDLCmdLine
{
public:
	enum eDL_CMD_TYPE
	{
		// Launcher command
		eDL_CMD_TYPE_NORMAL,
		eDL_CMD_TYPE_LAUNCHER_PATCH,

		// Service command
		eDL_CMD_TYPE_SERVICE,

		// Invalid command
		eDL_CMD_TYPE_INVALID
	};

	struct sCMD_FILED_DATA
	{
		eDL_CMD_TYPE			eCmdType;
		CString					strCmd;
	};

	typedef std::vector< sCMD_FILED_DATA > vecdef_CMD_FIELD_DATA_LIST;

protected:
	CString						m_strCmdLine;
	vecdef_CMD_FIELD_DATA_LIST	m_defFieldDataList;

public:
	CDLCmdLine( void );
	~CDLCmdLine( void );

public:    
	bool						LoadCmdLine( const CString& strCmdLine );
	void						UnloadCmdLine( void );

	int							GetCmdTypeCnt( eDL_CMD_TYPE eCmdType );

	CString						GetServiceCmd( void );

	unsigned int				GetPublisherID( void );

protected:
	bool						ParseCmdLine( void );
};


extern CDLCmdLine g_clDLCmdLine;
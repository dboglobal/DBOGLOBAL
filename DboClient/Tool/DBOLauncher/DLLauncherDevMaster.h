/*****************************************************************************
*
* File			: DLLauncherDevMaster.h
* Author		: 
* Copyright		: (аж) NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          : 
*
*****************************************************************************/


#pragma once


//////////////////////////////////////////////////////////////////////////
//
// CDLLauncherDevMaster
//
//////////////////////////////////////////////////////////////////////////


class CDLLauncherDevMaster
{
protected:
	bool					m_bNoCheckPatchServerEnable;
	bool					m_bNoCheckIntegrityServerEnable;

public:
	CDLLauncherDevMaster( void ) { m_bNoCheckPatchServerEnable = false; m_bNoCheckIntegrityServerEnable = false; }
	~CDLLauncherDevMaster( void ) { return; }

public:
	bool					Load( CString strPath );

	bool					IsNoCheckPatchServerEnable( void ) const;
	bool					IsNoCheckIntegrityServerEnable( void ) const;
};


extern CDLLauncherDevMaster g_clDLLaucherDevMaster;
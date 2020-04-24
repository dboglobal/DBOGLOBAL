//*****************************************************************************
// File       : WorldMonitor.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef _WORLD_MONITOR_H_
#define _WORLD_MONITOR_H_

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

#ifdef WORLDMONITOR_EXPORTS
	#define WORLDMONITOR_API __declspec(dllexport)
#else
	#define WORLDMONITOR_API __declspec(dllimport)
#endif //WORLDMONITOR_EXPORTS


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

struct RpWorld;
class CLogMessageManager;


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

#define g_pWorldMonitor					(CWorldMonitor::GetInstance())


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

//=============================================================================
// Name       : CWorldMonitor
// Desc       : 
// Author     : 
//=============================================================================

class WORLDMONITOR_API CWorldMonitor
{
	protected:

										CWorldMonitor( void ) {}
										CWorldMonitor( const CWorldMonitor & rcMonitor );

		virtual							~CWorldMonitor( void ) {}

		const CWorldMonitor &			operator=( const CWorldMonitor & rcMonitor );

	public:

		virtual bool					AnalysisInstance( void * pInstance ) = 0;
		virtual bool					AnalysisObject( void * pObject ) = 0;
		virtual bool					AnalysisSector( void * pSector ) = 0;

	public:

		static bool						CreateInstance( const TCHAR * ptMonitorName );
		static bool						CreateInstance( CLogMessageManager * pcMonitor );
		static void						DestroyInstance( void );
		static CWorldMonitor *			GetInstance( void );

};

//=============================================================================
// End of Class : CWorldMonitor
//=============================================================================

WORLDMONITOR_API RwInt32				GetLinkCountFromLinkedList( RwLinkList * pLinkedList );
WORLDMONITOR_API RwLLLink *				GetLinkFromLinkedList( RwLinkList * pLinkedList, RwInt32 iIndex );

WORLDMONITOR_API RpClump *				GetClumpFromLink( RwLLLink * pLink );

WORLDMONITOR_API RpAtomic *				GetAtomicFromLink( RwLLLink * pLink );
WORLDMONITOR_API RpAtomic *				GetAtomicFromLinkByTie( RwLLLink * pLink );

WORLDMONITOR_API RpLight *				GetLightFromLink( RwLLLink * pLink );
WORLDMONITOR_API RpLight *				GetLightFromLinkByTie( RwLLLink * pLink );
WORLDMONITOR_API RpLight *				GetLightFromLinkByExt( RwLLLink * pLink );

WORLDMONITOR_API RwCamera *				GetCameraFromLinkByExt( RwLLLink * pLink );

#endif //_WORLD_MONITOR_H_

//*****************************************************************************
//
// End of File : WorldMonitor.h
//
//*****************************************************************************
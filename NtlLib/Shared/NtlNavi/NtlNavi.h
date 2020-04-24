#ifndef _NTL_NAVI_H_
#define _NTL_NAVI_H_


#include "NtlNaviDefine.h"

//////////////////////////////////////////////////////////////////////////
//
// Wayfinding log system
//
//////////////////////////////////////////////////////////////////////////


class INtlNaviLog
{
public:
	INtlNaviLog(void) {}
	virtual ~INtlNaviLog( void ) {}


public:
	virtual void Log( const char* pLog, ... )														= 0;
};


//////////////////////////////////////////////////////////////////////////
//
// Directions Interface
//
//////////////////////////////////////////////////////////////////////////

class CNtlNaviPEWorld;
class iMesh;

class INtlNavi
{
public:
	INtlNavi(void) {}

	virtual ~INtlNavi(void) {}


public:
	//////////////////////////////////////////////////////////////////////////
	//
	// Navigation library Drive-related interface
	//
	//////////////////////////////////////////////////////////////////////////

	virtual bool					Create( INtlNaviLog* pLog,
											const char* pPathDllName,
											bool bThreadLoading )									= 0;

	virtual void					Delete( void )													= 0;


	//////////////////////////////////////////////////////////////////////////
	//
	// Tool-specific interface
	//
	//////////////////////////////////////////////////////////////////////////

	virtual bool					ImportWorldToolData( const char* pRootFolder )					= 0;

	virtual void					GetListImportedWorldIDList( vecdef_WorldIDList& vecOut )		= 0;

	virtual bool					ExportPathEngineData( const char* pRootFolder )					= 0;

	virtual bool					ExportPathEngineDataSelect( const char* pRootFolder, mapdef_ExportList& list ) = 0;

	virtual bool					UpdateToolData( void )											= 0;

	virtual int						GetWorldVertexCount( NAVI_INST_HANDLE hHandle )					= 0;

	virtual int						GetWorldVertexCount( unsigned int uiWorldID )					= 0;

	virtual bool					GetWorldVertexToBuffer( NAVI_INST_HANDLE hHandle, sNAVI_VERTEX* pBuffer )		= 0;

	virtual bool					GetWorldVertexToBuffer( unsigned int uiWorldID, sNAVI_VERTEX* pBuffer )			= 0;

	virtual iMesh*					GetNearestMesh( NAVI_INST_HANDLE hHandle, sNAVI_POS& vPos )						= 0;


	//////////////////////////////////////////////////////////////////////////
	//
	// Game interface
	//
	//////////////////////////////////////////////////////////////////////////

	// Loading the path engine data
	virtual bool					LoadPathEngineData( const char* pRootFolder,
														vecdef_WorldIDList& defWorldIDList,
														unsigned char byLoadFlags )					= 0;

	// Block main thread until loading is completed
	// It is not responsible for the use of the underlying functions before loading is completed
	virtual bool					WaitUntilLoadingFinish( unsigned int uiWaitTime,
															unsigned int& uiResult )				= 0;

	// Return loading time
	// Return value -> 0xffffffff				: Loading failed
	//			 0xffffffff Other values: Loading time
	virtual unsigned int			GetLoadingTime( void )											= 0;

	// Amount of memory currently used by the path engine
	virtual unsigned long			GetTotalMemory( void )											= 0;

	// Instance handler produce
	virtual NAVI_INST_HANDLE		CreateInstanceHandler( unsigned int uiWorldID )					= 0;

	// Instance handler remove
	virtual void					DeleteInstanceHandler( NAVI_INST_HANDLE hHandle )				= 0;

	// It is checked whether attribute data exists
	virtual bool					IsPropDataLoaded( unsigned int uiWorldID )						= 0;

	// It is checked whether or not path data exists
	virtual bool					IsPathDataLoaded( unsigned int uiWorldID )						= 0;

	// NPC, Mob size (radius) support
	virtual bool					CanSupportAgent( NAVI_INST_HANDLE hHandle, float fAgentRadius )	= 0;

	// Map
	virtual unsigned int			GetTextAllIndex( NAVI_INST_HANDLE hHandle,
													 float x,
													 float z )										= 0;

	// Zone
	virtual unsigned int			GetZoneIndex( NAVI_INST_HANDLE hHandle,
												  float x,
												  float z )											= 0;

	// All attributes for that location
	virtual unsigned int			GetAttribute( NAVI_INST_HANDLE hHandle,
												  float x,
												  float z )											= 0;

	// Presence or absence of basic attributes
	virtual bool					IsBasicAttributeSet( NAVI_INST_HANDLE hHandle,
														 float x,
														 float z,
														 eDBO_WORLD_ATTR_BASIC eAttrBasic )			= 0;

	// Invalid if the return value is NAVI_FLT_MAX
	virtual float					GetHeight( NAVI_INST_HANDLE hHandle,
											   float x,
											   float y,
											   float z )											= 0;

	virtual long					GetFastHeight(NAVI_INST_HANDLE hHandle, float x, float y, float z, float& newy, int nVertRange) = 0;


	// Invalid if the return value is NAVI_FLT_MAX
	virtual float					GetGuaranteedHeight( NAVI_INST_HANDLE hHandle,
														 float x,
														 float y,
														 float z )									= 0;


	// Return the location of the nearest location
	virtual bool					FindNearestPos( NAVI_INST_HANDLE hHandle,
													float fAgentRadius,
													sNAVI_POS& sSrcPos )							= 0;

	virtual bool					FindNearestPos(NAVI_INST_HANDLE hHandle,
		float fAgentRadius,
		sNAVI_POS& sSrcPos, 
		sNAVI_FAST_POS& sSrcFastPos) = 0;

	// Crash test
	virtual eCOL_TEST_RESULT		CollisionTest( NAVI_INST_HANDLE hHandle,
												   float fAgentRadius,
												   sNAVI_POS& sSrcPos,
												   sNAVI_POS& sDestPos )							= 0;

	virtual eCOL_TEST_RESULT		CollisionTest(NAVI_INST_HANDLE hHandle,
		float fAgentRadius,
		sNAVI_FAST_POS& sSrcFastPos,
		sNAVI_FAST_POS& sDestFastPos) = 0;


	//Collision test: output first collision position
	virtual eCOL_TEST_RESULT		FirstCollisionTest( NAVI_INST_HANDLE hHandle,
													float fAgentRadius, 
													sNAVI_POS& sSrcPos, 
													sNAVI_POS& sDestPos, 
													sNAVI_POS& sFirstCollisionPos )					= 0;

	virtual eCOL_TEST_RESULT		FirstCollisionTest(NAVI_INST_HANDLE hHandle,
		float fAgentRadius,
		sNAVI_FAST_POS& sSrcPos,
		sNAVI_POS& sDestPos,
		sNAVI_POS& sFirstCollisionPos) = 0;

	// Get Directions
	virtual bool					FindPath( NAVI_INST_HANDLE hHandle,
											  float fAgentRadius,
											  sNAVI_POS& sSrcPos,
											  sNAVI_POS& sDestPos,
											  vecdef_NaviPosList& defNaviPosList )					= 0;

	virtual bool					FindPath(NAVI_INST_HANDLE hHandle,
		float fAgentRadius,
		sNAVI_FAST_POS& sSrcFastPos,
		sNAVI_FAST_POS& sDestFastPos,
		vecdef_NaviPosList& defNaviPosList) = 0;


	virtual bool					FastFindNearestPos(NAVI_INST_HANDLE hHandle,
		float fAgentRadius,
		sNAVI_POS& sSrcPos,
		int nHorizRange,
		int nClosestRange) = 0;

	virtual eCOL_TEST_RESULT		FastFirstCollisionTest(NAVI_INST_HANDLE hHandle, float fAgentRadius, sNAVI_POS & sSrcPos, sNAVI_POS & sDestPos, sNAVI_POS& sFirstCollisionPos) = 0;

	virtual eCOL_TEST_RESULT		FastCollisionTest(NAVI_INST_HANDLE hHandle,
		float fAgentRadius,
		sNAVI_POS& sSrcPos,
		sNAVI_POS& sDestPos) = 0;

	virtual eCOL_TEST_RESULT		FastCollisionTest(NAVI_INST_HANDLE hHandle,
		float fAgentRadius,
		sNAVI_POS& sSrcPos,
		sNAVI_POS& sDestPos,
		sNAVI_POS& sNewDestPos) = 0;

	virtual bool					FastFindPath(NAVI_INST_HANDLE hHandle,
		float fAgentRadius,
		sNAVI_POS& sSrcPos,
		sNAVI_POS& sDestPos,
		vecdef_NaviPosList& defNaviPosList) = 0;

	virtual eCOL_TEST_RESULT		FastChaseFindPath(NAVI_INST_HANDLE hHandle,
		float fAgentRadius,
		sNAVI_POS& sSrcPos,
		sNAVI_POS& sDestPos,
		vecdef_NaviPosList& defNaviPosList) = 0;

	virtual eCOL_TEST_RESULT		FastCanMoveNearestDest(NAVI_INST_HANDLE hHandle,
		float fAgentRadius,
		sNAVI_POS& sSrcPos,
		sNAVI_POS& sDestPos) = 0;


	virtual bool					IsValidPos(NAVI_INST_HANDLE hHandle, sNAVI_POS& sSrcPos) = 0;

};

// Get the currently generated directions interface
INtlNavi* GetNaviEngine(void);



#endif
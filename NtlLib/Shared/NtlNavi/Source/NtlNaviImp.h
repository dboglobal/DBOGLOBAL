#ifndef _NTL_NAVI_IMP_H_
#define _NTL_NAVI_IMP_H_



class CNtlLoadingQueue;
class CNtlNaviPEDataExportMng;
class CNtlNaviPEDataImportMng;


class CNtlNaviImp : public INtlNavi
{
// Instance
public:
	static CNtlNaviImp*				GetInstance( void );


// Member variables
protected:
	unsigned int					m_aruiBasicAttributeCheckFlag[DBO_WORLD_ATTR_BASIC_COUNT];

	CNtlLoadingQueue*				m_pLoadingQueue;

	CNtlNaviPEDataExportMng*		m_pNaviPEDataExporter;
	CNtlNaviPEDataImportMng*		m_pNaviPEDataImporter;


// Constructions and Destructions
public:
	CNtlNaviImp( void );
	~CNtlNaviImp( void );


// Operations
public:


// Implementations
public:
	//////////////////////////////////////////////////////////////////////////
	//
	// Navigation library 구동 관련 인터페이스
	//
	//////////////////////////////////////////////////////////////////////////

	virtual bool					Create( INtlNaviLog* pLog, const char* pPathDllName, bool bThreadLoading );

	virtual void					Delete( void );


	//////////////////////////////////////////////////////////////////////////
	//
	// Tool 전용 인터페이스
	//
	//////////////////////////////////////////////////////////////////////////

	virtual bool					ImportWorldToolData( const char* pRootFolder );

	virtual void					GetListImportedWorldIDList( vecdef_WorldIDList& vecOut );

	virtual bool					ExportPathEngineData( const char* pRootFolder );

	virtual bool					ExportPathEngineDataSelect( const char* pRootFolder, mapdef_ExportList& list );

	virtual bool					UpdateToolData( void );

	virtual int						GetWorldVertexCount( NAVI_INST_HANDLE hHandle );

	virtual int						GetWorldVertexCount( unsigned int uiWorldID );

	virtual bool					GetWorldVertexToBuffer( NAVI_INST_HANDLE hHandle, sNAVI_VERTEX* pBuffer );

	virtual bool					GetWorldVertexToBuffer( unsigned int uiWorldID, sNAVI_VERTEX* pBuffer );

	virtual iMesh*					GetNearestMesh( NAVI_INST_HANDLE hHandle, sNAVI_POS& vPos );


	//////////////////////////////////////////////////////////////////////////
	//
	// Game 전용 인터페이스
	//
	//////////////////////////////////////////////////////////////////////////

	virtual bool					LoadPathEngineData( const char* pRootFolder, vecdef_WorldIDList& defWorldIDList, unsigned char byLoadFlags );

	virtual bool					WaitUntilLoadingFinish( unsigned int uiWaitTime, unsigned int& uiResult );

	virtual unsigned int			GetLoadingTime( void );

	virtual unsigned long			GetTotalMemory( void );

	virtual NAVI_INST_HANDLE		CreateInstanceHandler( unsigned int uiWorldID );

	virtual void					DeleteInstanceHandler( NAVI_INST_HANDLE hHandle );

	virtual bool					IsPropDataLoaded( unsigned int uiWorldID );

	virtual bool					IsPathDataLoaded( unsigned int uiWorldID );

	virtual bool					CanSupportAgent( NAVI_INST_HANDLE hHandle, float fAgentRadius );

	virtual unsigned int			GetTextAllIndex( NAVI_INST_HANDLE hHandle, float x, float z );

	virtual unsigned int			GetZoneIndex( NAVI_INST_HANDLE hHandle, float x, float z );

	virtual unsigned int			GetAttribute( NAVI_INST_HANDLE hHandle, float x, float z );

	virtual bool					IsBasicAttributeSet( NAVI_INST_HANDLE hHandle, float x, float z, eDBO_WORLD_ATTR_BASIC eAttrBasic );

	virtual float					GetHeight( NAVI_INST_HANDLE hHandle, float x, float y, float z );

	virtual long					GetFastHeight(NAVI_INST_HANDLE hHandle, float x, float y, float z, float& newy, int nVertRange);

	virtual float					GetGuaranteedHeight( NAVI_INST_HANDLE hHandle, float x, float y, float z );

	virtual bool					FindNearestPos( NAVI_INST_HANDLE hHandle, float fAgentRadius, sNAVI_POS& sSrcPos );

	virtual bool					FindNearestPos(NAVI_INST_HANDLE hHandle, float fAgentRadius, sNAVI_POS& sSrcPos, sNAVI_FAST_POS& sSrcFastPos);

	virtual eCOL_TEST_RESULT		CollisionTest( NAVI_INST_HANDLE hHandle, float fAgentRadius, sNAVI_POS& sSrcPos, sNAVI_POS& sDestPos );

	virtual eCOL_TEST_RESULT		CollisionTest(NAVI_INST_HANDLE hHandle, float fAgentRadius, sNAVI_FAST_POS& sSrcFastPos, sNAVI_FAST_POS& sDestFastPos);

	virtual eCOL_TEST_RESULT		FirstCollisionTest( NAVI_INST_HANDLE hHandle, float fAgentRadius, sNAVI_POS& sSrcPos, sNAVI_POS& sDestPos, sNAVI_POS& sFirstCollisionPos );

	virtual eCOL_TEST_RESULT		FirstCollisionTest(NAVI_INST_HANDLE hHandle, float fAgentRadius, sNAVI_FAST_POS& sSrcPos, sNAVI_POS& sDestPos, sNAVI_POS& sFirstCollisionPos);

	virtual bool					FindPath( NAVI_INST_HANDLE hHandle, float fAgentRadius, sNAVI_POS& sSrcPos, sNAVI_POS& sDestPos, vecdef_NaviPosList& defNaviPosList );

	virtual bool					FindPath(NAVI_INST_HANDLE hHandle, float fAgentRadius, sNAVI_FAST_POS& sSrcFastPos, sNAVI_FAST_POS& sDestFastPos, vecdef_NaviPosList& defNaviPosList);

	virtual bool					FastFindNearestPos(NAVI_INST_HANDLE hHandle, float fAgentRadius, sNAVI_POS& sSrcPos, int nHorizRange, int nClosestRange);

	virtual eCOL_TEST_RESULT		FastFirstCollisionTest(NAVI_INST_HANDLE hHandle, float fAgentRadius, sNAVI_POS& sSrcPos, sNAVI_POS& sDestPos, sNAVI_POS& sFirstCollisionPos);

	virtual eCOL_TEST_RESULT		FastCollisionTest(NAVI_INST_HANDLE hHandle, float fAgentRadius, sNAVI_POS& sSrcPos, sNAVI_POS& sDestPos);

	virtual eCOL_TEST_RESULT		FastCollisionTest(NAVI_INST_HANDLE hHandle, float fAgentRadius, sNAVI_POS& sSrcPos, sNAVI_POS& sDestPos, sNAVI_POS& sNewDestPos);

	virtual bool					FastFindPath(NAVI_INST_HANDLE hHandle, float fAgentRadius, sNAVI_POS& sSrcPos, sNAVI_POS& sDestPos, vecdef_NaviPosList& defNaviPosList);

	virtual eCOL_TEST_RESULT		FastChaseFindPath(NAVI_INST_HANDLE hHandle, float fAgentRadius, sNAVI_POS& sSrcPos, sNAVI_POS& sDestPos, vecdef_NaviPosList& defNaviPosList);

	virtual eCOL_TEST_RESULT		FastCanMoveNearestDest(NAVI_INST_HANDLE hHandle, float fAgentRadius, sNAVI_POS& sSrcPos, sNAVI_POS& sDestPos);

	virtual bool					IsValidPos(NAVI_INST_HANDLE hHandle, sNAVI_POS& sSrcPos);

};


INtlNavi* GetNaviEngine(void)
{
	return CNtlNaviImp::GetInstance();
}


#endif
#ifndef _NTL_NAVI_PE_WORLD_H_
#define _NTL_NAVI_PE_WORLD_H_


#include "NtlNaviSync.h"
#include "NtlNaviEntity.h"
#include "NtlNaviVector3.h"


class CNtlNaviDataMng;
class CNtlNaviLoadingEntity;


//////////////////////////////////////////////////////////////////////////
//
//	CNtlNaviPEWorld
//
//////////////////////////////////////////////////////////////////////////


class CNtlNaviPEWorld : public CNtlNaviEntityPE
{
// Declarations
public:
	enum eNAVI_PE_STATE
	{
		eNAVI_PE_STATE_PRE_LOADING,	// 전처리 로딩 중
		eNAVI_PE_STATE_LOADING,		// 로딩 중
		eNAVI_PE_STATE_COMPLETE		// 로딩 완료
	};

	// Property
	typedef std::map< unsigned int, CNtlNaviPropOutDoorInfo* > mapdef_ODPropList;
	typedef std::map< unsigned int, CNtlNaviPropInDoorInfo* > mapdef_IDPropList;

	// Ground mesh
	struct sGROUP_DATA
	{
		sGROUP_DATA( void ) { pGroundMesh = NULL; }
		sGROUP_DATA( iMesh* pGroundMesh ) : pGroundMesh( pGroundMesh ) { return; }

		iMesh*						pGroundMesh;
	};
	typedef std::map< unsigned int, sGROUP_DATA > mapdef_GroupDataList;

	// Agent
	struct sAGENT_DATA
	{
		float						fRadius;
		iShape*						pShape;
	};
	typedef std::map< float, sAGENT_DATA > mapdef_AgentList;

	// Multi-thread
	typedef std::vector< CNtlNaviLoadingEntity* > vecdef_LoadingEntityList;

	// Instance handler
	typedef std::map< unsigned int, iCollisionContext* > mapdef_ColContextList;
	struct sINNER_INST_HANDLER
	{
		mapdef_ColContextList		defColContextList;
	};

	typedef std::map< NAVI_INST_HANDLE, sNAVI_INST_HANDLER* > mapdef_InstHandlerList;

// Member variables
protected:
	CNtlNaviCS						m_csCurState;
	eNAVI_PE_STATE					m_eCurState;

	std::string						m_strImportPath;

	unsigned char					m_byLoadedFlags;

	// World info
	CNtlNaviDataMng*				m_pNaviDataMng;

	// Property
	mapdef_ODPropList				m_defODPropList;
	mapdef_IDPropList				m_defIDPropList;

	// Navigation info
	mapdef_AgentList				m_defAgentList;
	mapdef_GroupDataList			m_defGroupDataList;

	// Loading entity list
	CNtlNaviCS						m_csLoadingEntityList;
	vecdef_LoadingEntityList		m_defLoadingEntityList;

	// Instance handler
	mapdef_InstHandlerList			m_defInstHandlerList;


// Constructions and Destructions
public:
	CNtlNaviPEWorld( void );
	~CNtlNaviPEWorld( void );


// Operations
public:
	eNAVI_PE_STATE					GetCurState( void );

	void							SetCurState( eNAVI_PE_STATE eState );

	const char*						GetImportPath( void );

	unsigned char					GetLoadedFlags( void );

	virtual bool					ImportPathData( const char* pPath, unsigned char byLoadFlags );

	virtual bool					CheckLoadComplete( void );

	NAVI_INST_HANDLE				CreateInstanceHandler( unsigned int uiWorldID );

	void							DeleteInstanceHandler( NAVI_INST_HANDLE hHandle );

	void							DeleteAllInstanceHandler( void );

	bool							CanSupportAgent( float fAgentRadius );

	unsigned int					GetAttribute( float x, float z );

	unsigned int					GetTextAllIndex( float x, float z );

	unsigned int					GetZoneIndex( float x, float z );

	float							GetHeight( float x, float y, float z );

	long							GetFastHeight(float x, float y, float z, float& newy, int nVertRange);

	float							GetGuaranteedHeight(float x, float y, float z);

	bool							FindNearestPos( NAVI_INST_HANDLE hHandle, float fAgentRadius, CNtlNaviVector3& vSourcePos );

	bool							FindNearestPos(NAVI_INST_HANDLE hHandle, float fAgentRadius, CNtlNaviVector3& vSourcePos, sNAVI_FAST_POS& sFastPos);

	eCOL_TEST_RESULT				CollisionTest( NAVI_INST_HANDLE hHandle, float fAgentRadius, CNtlNaviVector3& vSourcePos, CNtlNaviVector3& vTargetPos );

	eCOL_TEST_RESULT				CollisionTest(NAVI_INST_HANDLE hHandle, float fAgentRadius, sNAVI_FAST_POS& sSrcFastPos, sNAVI_FAST_POS& sDestFastPos);

	eCOL_TEST_RESULT				FirstCollisionTest( NAVI_INST_HANDLE hHandle, float fAgentRadius, CNtlNaviVector3& vSourcePos, CNtlNaviVector3& vTargetPos, CNtlNaviVector3& vFirstCollison );

	eCOL_TEST_RESULT				FirstCollisionTest(NAVI_INST_HANDLE hHandle, float fAgentRadius, sNAVI_FAST_POS& vSourcePos, CNtlNaviVector3& vTargetPos, CNtlNaviVector3& vFirstCollison);

	bool							FindPath( NAVI_INST_HANDLE hHandle, float fAgentRadius, CNtlNaviVector3& vSourcePos, CNtlNaviVector3& vTargetPos, vecdef_NaviPosList& defNaviPosList );

	bool							FindPath(NAVI_INST_HANDLE hHandle, float fAgentRadius, sNAVI_FAST_POS& sSrcFastPos, sNAVI_FAST_POS& sDestFastPos, vecdef_NaviPosList& defNaviPosList);

	bool							FastFindNearestPos(NAVI_INST_HANDLE hHandle, float fAgentRadius, CNtlNaviVector3& vSourcePos, int nHorizRange, int nClosestRange);

	eCOL_TEST_RESULT				FastFirstCollisionTest(NAVI_INST_HANDLE hHandle, float fAgentRadius, CNtlNaviVector3& vSourcePos, CNtlNaviVector3& vTargetPos, CNtlNaviVector3& vFirstCollison);

	eCOL_TEST_RESULT				FastCollisionTest(NAVI_INST_HANDLE hHandle, float fAgentRadius, CNtlNaviVector3& vSourcePos, CNtlNaviVector3& vTargetPos);

	eCOL_TEST_RESULT				FastCollisionTest(NAVI_INST_HANDLE hHandle, float fAgentRadius, CNtlNaviVector3& vSourcePos, CNtlNaviVector3& vTargetPos, CNtlNaviVector3& vNewTargetPos);

	bool							FastFindPath(NAVI_INST_HANDLE hHandle, float fAgentRadius, CNtlNaviVector3& vSourcePos, CNtlNaviVector3& vTargetPos, vecdef_NaviPosList& defNaviPosList);

	eCOL_TEST_RESULT				FastChaseFindPath(NAVI_INST_HANDLE hHandle, float fAgentRadius, CNtlNaviVector3& vSourcePos, CNtlNaviVector3& vTargetPos, vecdef_NaviPosList& defNaviPosList);

	eCOL_TEST_RESULT				FastCanMoveNearestDest(NAVI_INST_HANDLE hHandle, float fAgentRadius, CNtlNaviVector3& vSourcePos, CNtlNaviVector3& vTargetPos);

	bool							IsValidPos(sNAVI_POS & sSrcPos);

// Tool interface
public:
	int								GetGroundVertexCount();

	bool							GetGroundVertexToBuffer( sNAVI_VERTEX* pBuffer );

	iMesh*							GetNearestMesh( CNtlNaviVector3& vPos );
	

// Implementations
protected:
	void							Destroy( void );

	bool							ImportWorldInfo( const char* pPath );

	bool							ImportWorldProperty( const char* pPath );

	bool							ImportWorldProperty_OD( const char* pPath );

	void							DestroyWorldProperty_OD( void );

	bool							ImportWorldProperty_ID( const char* pPath );

	void							DestroyWorldProperty_ID( void );

	bool							ImportWorldGroup( const char* pPath );

	bool							ImportWorldGroup_OD( const char* pPath );

	bool							ImportWorldGroup_ID( const char* pPath );

	void							DestroyWorldGroup( void );

	void							AttachEntityToLoad( CNtlNaviLoadingEntity* pEntity );

	bool							UpdateEntityToLoad( void );

	bool							IsColProp_Sphere( float x, float z, float fSX, float fSZ, float fSRadius );

	bool							IsColProp_Plane( float x, float z, float fPX, float fPZ, float fPWidth, float fPDepth );

	unsigned int					GetGroupID(float x, float z);

	void							GetGroupData(sGROUP_DATA& result, float x, float z);

};


#endif
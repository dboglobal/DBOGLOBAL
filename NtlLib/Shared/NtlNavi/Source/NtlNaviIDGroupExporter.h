#ifndef _NTL_NAVI_IDGROUP_EXPORTER_H_
#define _NTL_NAVI_IDGROUP_EXPORTER_H_


#include "NtlNaviUtility.h"


class CNtlNaviResMng;
class CNtlNaviDataMng;


//////////////////////////////////////////////////////////////////////////
//
//	CNtlNaviIDGroupExporter
//
//////////////////////////////////////////////////////////////////////////


class CNtlNaviIDGroupExporter
{
public:

	enum eEXPORT_STATE
	{
		eEXPORT_STATE_PREPARE,
		eEXPORT_STATE_MULTITHREAD,
		eEXPORT_STATE_POST,
		eEXPORT_STATE_COMPLTE
	};

	//--------------------------------------------------------------------
	//	Input parameter
	//--------------------------------------------------------------------

	struct sINPUT_PARAM
	{
		std::vector<float>			vecdef_AgentList;
	};

	//--------------------------------------------------------------------
	//	Navigation mesh resource manager
	//--------------------------------------------------------------------

	struct sNAVI_RES_ENTITY
	{
		iContentChunk*				pContChunk;
		iContentChunkInstance*		pContChunkInst;
	};

	//--------------------------------------------------------------------
	//	obstacle
	//--------------------------------------------------------------------

	struct sNAVI_OBS_ENTITY
	{
		std::string					strName;
		float						fX, fY, fZ;
		iShape*						pObsShape;
		iAgent*						pObsAgent;
	};

	//--------------------------------------------------------------------
	//	agent
	//--------------------------------------------------------------------

	struct sNAVI_AGENT_ENTITY
	{
		float						fRadius;
		iShape*						pShape;
	};

	//--------------------------------------------------------------------
	//	type define
	//--------------------------------------------------------------------

	typedef std::vector< CNtlNaviGroupInDoorInfo::sNAVI_OBJECT_LIST* > vecdef_NAVIOBJ_LIST;
	typedef std::map< std::string, sNAVI_RES_ENTITY > mapdef_NAVI_RES_ENTITY_LIST;
	typedef std::vector< iFaceVertexMesh* > vecdef_FACE_VERTEX_MESH_LIST;
	typedef std::vector< sNAVI_OBS_ENTITY > vecdef_OBS_ENTITY_LIST;
	typedef std::vector< sNAVI_AGENT_ENTITY > vecdef_AGENT_ENTITY_LIST;


protected:
	// 상태
	eEXPORT_STATE					m_eCurState;

	// 외부 입력 데이터
	std::string						m_strExportPath;
	CNtlNaviResMng*					m_pNaviResMng;
	CNtlNaviDataMng*				m_pNaviDataMng;
	sINPUT_PARAM					m_sInputParam;

	// Import 데이터
	std::string						m_strWorldModelName;
	vecdef_NAVIOBJ_LIST				m_defNaviMeshInfoList;
	vecdef_NAVIOBJ_LIST				m_defObsMeshInfoList;

	// 빌드를 위해 필요한 데이터
	mapdef_NAVI_RES_ENTITY_LIST		m_defNaviEntityResList;
	vecdef_FACE_VERTEX_MESH_LIST	m_defPEGroundFVMeshList;
	vecdef_OBS_ENTITY_LIST			m_defPEObsList;
	vecdef_AGENT_ENTITY_LIST		m_defAgentList;

	// 빌드된 패스 엔진 데이터
	iMesh*							m_pMesh;
	iCollisionContext*				m_pColContext;


public:
	CNtlNaviIDGroupExporter( const char* pExportPath, CNtlNaviResMng* pNaviResMng, CNtlNaviDataMng* pNaviDataMng, const sINPUT_PARAM& sInputParam );
	~CNtlNaviIDGroupExporter( void );


public:
	bool							Update( void );

	eEXPORT_STATE					GetCurState( void );
	void							SetCurState( eEXPORT_STATE eState );


protected:
	void							Destroy( void );

	//////////////////////////////////////////////////////////////////////////
	//
	// World group 정보 ( Navigation mesh, Obstacle mesh )
	//
	//////////////////////////////////////////////////////////////////////////

	void							AttachNaviMeshInfo( CNtlNaviGroupInDoorInfo::sNAVI_OBJECT_LIST* pNaviObjInfo );
	void							ClearNaviMeshInfo( void );

	void							AttachObsMeshInfo( CNtlNaviGroupInDoorInfo::sNAVI_OBJECT_LIST* pNaviObjInfo );
	void							ClearObsMeshInfo( void );

	//////////////////////////////////////////////////////////////////////////
	//
	// Navigation resource entity
	//
	//////////////////////////////////////////////////////////////////////////

	iContentChunkInstance*			LoadNaviEntityRes( const char* pFullPath );
	void							ClearAllNaviEntityRes( void );

	//////////////////////////////////////////////////////////////////////////
	//
	// Navigation build data
	//
	//////////////////////////////////////////////////////////////////////////

	int								GetSizePEGroundFVMesh( void );
	void							AttachPEGroundFVMesh( iFaceVertexMesh* pGroundMesh );
	void							ClearPEGroundFVMesh( void );

	bool							AttachPEObstacle( const char* pName, float fX, float fY, float fZ, tSigned32* arConvHullList, tSigned32 nConvHullListSize );
	void							ClearPEObstacle( void );

	void							AttachPEAgent( float lRadius );
	void							ClearPEAgent( void );

	//////////////////////////////////////////////////////////////////////////
	//
	// Build process
	//
	//////////////////////////////////////////////////////////////////////////

	bool							ProcessIDInfoData( void );
	bool							ProcessPrePEData( void );
	bool							ProcessPEData( void );
	bool							ProcessPostPEData( void );
};


#endif
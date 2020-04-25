#ifndef _NTL_PICKING_H_
#define _NTL_PICKING_H_


#include "NtlPLPicking.h"


class CNtlPLEntity;


template<class _Ty>
class CCompare : public std::binary_function<_Ty, _Ty, bool>
{
public:
	bool operator()(const _Ty& _Left, const _Ty& _Right) const
	{
		if ( _Left->pPLEntity && _Right->pPLEntity )
		{
			if ( _Left->pPLEntity->GetPickingOrder() == _Right->pPLEntity->GetPickingOrder() )
			{
				return (_Left->fDist < _Right->fDist);
			}
			else
			{
				return (_Left->pPLEntity->GetPickingOrder() > _Right->pPLEntity->GetPickingOrder());
			}
		}

		return (_Left->fDist < _Right->fDist);
	}
};


class CNtlPicking
{
// Declarations
public:
	enum
	{
		RASTER_WIDTH					= 4,
		RASTER_HEIGHT					= 4
	};

	enum
	{
		MAX_NUM_OF_MAX_ATOMIC			= 5
	};

	struct SPICK_INFO
	{
		void Init( void )
		{
			uiID = 0xffff;
			bRenderPolygonCheck = false;
			pPLEntity = 0;
			fDist = 0;
			fRadius = 0.f;
			fResizedRadius = 0.f;
			pAtomic = 0;
		}

		RwUInt16						uiID;
		bool							bRenderPolygonCheck;
		CNtlPLEntity*					pPLEntity;
		RwReal							fDist;
		RwReal							fRadius;
		RwReal							fResizedRadius;
		RpAtomic*						pAtomic;
		RwV3d							v3PickPos;
	};
	typedef std::list<SPICK_INFO*> listdef_PICK_INFO_LIST;
	typedef std::vector<SPICK_INFO*> vecdef_PICK_INFO_LIST;
	typedef std::unordered_map<RwUInt16, SPICK_INFO*> hashdef_PICK_INFO_LIST;

	class CPickEntityPool
	{
	public:
		enum { eRESIZING_UNIT = 50 };

	protected:
		listdef_PICK_INFO_LIST			m_FreeList;

	public:
		CPickEntityPool( void );
		~CPickEntityPool( void );

	public:
		SPICK_INFO*						New( void );
		void							Delete( SPICK_INFO*& pEntity );
		void							Delete( hashdef_PICK_INFO_LIST* pEntityList );
	};

	class CPickEntityList
	{
	public:
		hashdef_PICK_INFO_LIST			m_defPickInfoList;
		vecdef_PICK_INFO_LIST			m_defSortedInfoList;

	public:
		CPickEntityList( void )			{ m_defSortedInfoList.reserve( 255 ); }

	public:
		bool							IsEmpty( void )
		{
			return m_defSortedInfoList.empty();
		}

		SPICK_INFO*						GetFirstSortedPickInfo( void )
		{
			return IsEmpty() ? NULL : m_defSortedInfoList[0];
		}

		void							Push( RwUInt16 iID, SPICK_INFO* pPickInfo )
		{
			m_defPickInfoList[iID] = pPickInfo;
			m_defSortedInfoList.push_back( pPickInfo );
		}

		void							DepthSort( void )
		{
			std::sort( m_defSortedInfoList.begin(), m_defSortedInfoList.end(), CCompare<SPICK_INFO*>() );
		}

		void							ClippingRenderingEntity( RwInt32 iNum )
		{
			RwInt32 iCnt = 0;
			vecdef_PICK_INFO_LIST::iterator itMask = m_defSortedInfoList.end();
			vecdef_PICK_INFO_LIST::iterator it = m_defSortedInfoList.begin();

			for ( ; it != m_defSortedInfoList.end(); ++it )
			{
				if ( !(*it)->bRenderPolygonCheck ) continue;

				if ( ++iCnt > iNum )
				{
					itMask = it;
					break;
				}
			}

			if ( itMask != m_defSortedInfoList.end() )
			{
				m_defSortedInfoList.erase( itMask, m_defSortedInfoList.end() );
			}
		}

		void							Clear( CPickEntityPool* pEntityPool )
		{
			pEntityPool->Delete( &m_defPickInfoList );
			m_defSortedInfoList.clear();
		}
	};

// Member variables
protected:
	static RwUInt16						s_ID;
	static RwUInt16						s_RenderID;
	static RwReal						s_fMinDist;

	RwRaster*							m_pPickRaster;
	RwRaster*							m_pPickZRaster;
	RwTexture*							m_pPickTexture;
	RxPipeline*							m_pPickPipeline;

	CPickEntityPool*					m_pPickEntityPool;
	CPickEntityList*					m_pPickEntityList;

	RwRaster*							m_pTempRaster;
	RwRaster*							m_pTempZRaster;

	RwV3d								m_v3CamPos;
	RwReal								m_fResizedRadius;

	bool								m_bPickCharacter;
	bool								m_bCharacterExclusion;

// Constructions and Destructions
public:
	CNtlPicking( void );
	~CNtlPicking( void );

// Methods
public:
	RwCamera*							GetPickingCamera( void );

	bool								Create( void );
	void								Delete( void );

	bool								Pick( RwCamera* pCamera, RpWorld* pWorld, RwV2d& v2ScrPos, RwReal fPickDist, SWorldPickInfo& sPickInfo );

	void								Render_PickCamera( void );

	void								SaveAsFile( const char* pFileName );

// Implementations
protected:
	// Bounding check를 위한 함수들
	static const RwSphere*				ResizeBoundingSphereCB( RpAtomic* pAtomic, void* pData );
	static RpAtomic*					PickAtomicBoundBoxCB( RpIntersection* pInters, RpWorldSector* pWorldSector, RpAtomic* pAtomic, RwReal fDistance, void* pData );
	static RpCollisionTriangle*			PickAtomicPolygonCB( RpIntersection* pInters, RpCollisionTriangle* pRpColTri, RwReal fRatio, void* pData );

	// Polygon check를 위한 함수들
	bool								CreateRaster( RwRaster*& pRaster, RwRaster*& pZRaster, RwTexture*& pTexture );
	void								DeleteRaster( RwRaster*& pRaster, RwRaster*& pZRaster, RwTexture*& pTexture );
	void								UpdateCamera_Begin( RwCamera* pCamera );
	void								UpdateCamera_End( RwCamera* pCamera );

	RxPipeline*							CreatePipeLine( void );
	void								DeletePipeLine( RxPipeline*& pPipeLine );
	static void							PickingPipeLineCB( RwResEntry* pRepEntry, void* pObject, RwUInt8 byType, RwUInt32 uiFlags );

	void								RenderPickedAtoms( RxPipeline* pPipeLine );
};



//////////////////////////////////////////////////////////////////////////
//
//	Bounding box resizing 최적의 크기를 구하기 위한 테스트용 클래스
//
//////////////////////////////////////////////////////////////////////////


class COptimizeBBox
{
public:
	void								Render( void );
};



#endif
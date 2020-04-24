#pragma once

typedef std::map<std::string, RwFrame *>				FRAME_TABLE_MAP;
typedef std::map<std::string, RwFrame *>::iterator		FRAME_TABLE_ITER;

typedef std::map<std::string, RpMaterial *>				MATERIAL_TABLE_MAP;
typedef std::map<std::string, RpMaterial *>::iterator	MATERIAL_TABLE_ITER;

typedef std::map<std::string, RpAtomic *>				ATOMIC_TABLE_MAP;
typedef std::map<std::string, RpAtomic *>::iterator		ATOMIC_TABLE_ITER;
typedef std::map<RpMaterial*, RpAtomic*>                MATERIAL_ATOMIC_MAP;
typedef std::map<RpMaterial*, RpAtomic*>::iterator      MATERIAL_ATOMIC_MAP_ITER;


#define MAX_NUM_BONES   128

/// 본의 정보 구조체
typedef	struct _BoneMod
{
    RwReal	length;		///< 본의 길이
    RwReal	width;		///< 본의 두께
    RwV3d	angles;		///< 본의 각도
}BoneMod;

/**
 * \ingroup ModelTool
 * \brief Clump(캐릭터) 관련 정보를 관리하는 클래스 
 * \date 2006-04-10
 * \author agebreak
 */
class MTCharacterInfo
{
public:
	MTCharacterInfo(void);
	virtual ~MTCharacterInfo(void);

public:
	RwBool		Create(RpClump* pClump);				         ///< 테이블 내용을 생성한다.
	void		Destroy();								         ///< 내용을 해제한다.

	RwFrame*	GetFrameByName(RwChar* chFrameName);	         ///< 특정 Frame을 반환한다.
	RpMaterial* GetMaterialByName(RwChar* chMaterialName);       ///< 특정 Material을 반환한다.
	RpAtomic*	GetAtomicByName(RwChar* chAtomicName);	         ///< 특정 Atomic을 반환한다.
    RpAtomic*   GetAtomicByMaterial(RpMaterial* pMaterial);      ///< 해당 Material을 가지고있는 Atomic을 반환한다.

    void        UpdateHeightWidth();                             ///< 높이와 넓이 정보를 업데이트 한다.

    // 화면에 표시할 정보 관련
    void        DisplayInfo(RtCharset* pCharSet);                ///< Clump 정보를 화면상에 표시한다.

	// CallBack 함수들
	static RpAtomic*	GetClumpAllAtomics(RpAtomic* pAtomic, void* pData);		///< Clump에서 모든 Atomic을 가져와 설정하는 콜백 함수
	static RwFrame*		GetChildFrame(RwFrame* pFrame, void* pData);			///< 모든 자식 Frame을 가져와서 맵에 설정하는 콜백 함수
	static RpMaterial*	GetAllMaterials(RpMaterial* material, void* pData);     ///< 모든 Material을 가져와서 맵에 설정하는 콜백 함수

protected:
	void		SetClumpInfo();							///< Clump 관련 정보를 Map에 세팅한다.


public:
	FRAME_TABLE_MAP		m_mapFrameTable;				///< Frame 테이블
	MATERIAL_TABLE_MAP	m_mapMaterialtable;				///< Material 테이블
	ATOMIC_TABLE_MAP	m_mapAtomicTable;				///< Atomic 테이블
    MATERIAL_ATOMIC_MAP m_mapMaterialAtomicTable;       ///< Material - Atomic 테이블

    static RpAtomic*    m_pTempAtomic;                  ///< Material - Atomic MAP을 만들기 위한 임시 Atomic 포인터

protected:
	RpClump*			m_pTargetClump;					///< 정보를 읽어올 Clump

    // 화면에 표시할 정보들
    static int          m_nTotalTriangels;              ///< 전체 Triangle 개수
    static int          m_nTotalVertices;               ///< 전체 Vertex 개수

    RwReal              m_fClumpHeight;                 ///< Clump의 높이
    RwReal              m_fClumpWidth;                  ///< Clump의 넓이

    

};

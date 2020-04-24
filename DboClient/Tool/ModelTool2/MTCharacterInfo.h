#pragma once

#include <vector>
#include "NtlAtomic.h"
#include "NtlPLGlobal.h"
#include "NtlPLMaterialExtWrapper.h"
#include "NtlPLResourceManager.h"
#include "NtlPLEntityRenderHelpers.h"

typedef std::map<std::string, RwFrame *>				FRAME_TABLE_MAP;
typedef std::map<std::string, RwFrame *>::iterator		FRAME_TABLE_ITER;

typedef std::map<std::string, RpMaterial *>				MATERIAL_TABLE_MAP;
typedef std::map<std::string, RpMaterial *>::iterator	MATERIAL_TABLE_ITER;

typedef std::map<std::string, RpAtomic *>				ATOMIC_TABLE_MAP;
typedef std::map<std::string, RpAtomic *>::iterator		ATOMIC_TABLE_ITER;
typedef std::map<RpMaterial*, RpAtomic*>                MATERIAL_ATOMIC_MAP;
typedef std::map<RpMaterial*, RpAtomic*>::iterator      MATERIAL_ATOMIC_MAP_ITER;

/**
 * \ingroup ModelTool
 * \brief Clump(캐릭터) 관련 정보를 관리하는 클래스 
 * \date 2006-04-10
 * \author agebreak
 */
class CMTClump
{
public:
	CMTClump(void);
	virtual ~CMTClump(void);
    static void CreateCartoon();                                 ///< Toon 객체를 생성한다
    static void DestroyCartoon();                                ///< Cartoon 객체를 해제한다.
    static void SetToonThickness(RwReal fThickness);             ///< Toon Thickness를 설정한다.
    static void SetToonInkColor(RwRGBA color);                   ///< Toon Ink의 Color를 설정한다
	static SToonData* GetToonData() {return &m_ToonData;}		 

public:
    RwBool      Load(RwChar* szFileName);                        ///< Clump 파일을 Load한다,
    RwBool      Save(RwChar* szFileName);                        ///< Clump 파일을 Save한다.
	RwBool		Create(RpClump* pClump, RwChar* szClumpName);    ///< 테이블 내용을 생성한다.
	void		Destroy();								         ///< 내용을 해제한다.

	RwFrame*	GetFrameByName(RwChar* chFrameName);	         ///< 특정 Frame을 반환한다.
    RwBBox      GetDefaultBBox()  {return m_bboxCharacter;};     ///< 캐릭터의 Default BBox를 반환한다.

    // Clump 관련 메서드
    RpClump*    GetClump() {return m_pClump;}                                   ///< RpClump 객체를 반환한다.
    const char* GetClumpName() {return m_strClumpName;}                         ///< Clump의 PathName을 반환한다.
    void        SetVisible(RwBool bVisible);                                    ///< Clump의 렌더링 유무를 설정한다.
    void        SetCartoon();                                                   ///< Toon 렌더링을 적용한다.
    void        SetViewEdge(RwBool bVisible);                                   ///< Toon Edge의 On/Off를 설정한다    

    // Atomic 관련
    RpAtomic*	GetAtomicByName(RwChar* chAtomicName);	                        ///< 특정 Atomic을 반환한다.
    RpAtomic*   GetAtomicByMaterial(RpMaterial* pMaterial);                     ///< 해당 Material을 가지고있는 Atomic을 반환한다.
    std::vector<char*>    GetAtomicNames();                                     ///< Atomic들의 이름을 반환한다.
    RwBool	    RemoveAtomic(RwChar* chAtomicName);	                            ///< Atomic을 Clump에서 제거한다.
    RwBool      CollisionDataBuild(RwChar* szAtomicName);                       ///< Collision Tree 데이터를 Build한다.
    void        SetAtomicFlag(ENtlAtomicFlag nFlagType, RwChar* szAtomicName, RwBool bEnable); ///< Atomic Flag를 설정한다.    
    void        SetAtomicFlag(ENtlAtomicFlag nFlag, RpAtomic* pAtomic, RwBool bEnable);
	void		RenderWireFrame();												///< Wireframe을 렌더링한다.
    

    // Material 관련
    RpMaterial* GetMaterialByName(RwChar* chMaterialName);                      ///< 특정 Material을 반환한다.
    std::vector<char*>    GetMaterialNames();                                   ///< Material들의 이름을 반환한다    
    void        SetMaterialColor(RwChar* chMaterialName, int r, int g, int b);  ///< Material에 Color를 적용한다.
    RwBool      SetMultiTexture(RwChar* chMaterialName, RwChar* chTextureName, RwChar* chTexturePath); ///< Material에 MultiTexture를 적용한다.
    
    // 바운딩 박스 관련
    void	    CreateBB(RwChar* chAtomicName);				                	///< 해당 Atomic의 BB를 생성한다.
    void        ClearBB();								    	                ///< 바운딩 박스 정보를 비운다.
    void        RenderAtomicBB();                                               ///< 생성한 Atomic BB를 렌더링한다.
    void        RenderBB(RwBBox* pBBox, int r = 255, int g = 255, int b = 0);	///< 바운딩 박스를 렌더링 한다.	
    RwReal      GetMaxWidthHeight();                                            ///< Clump의 Width, Height중 큰것을 반환한다.

    // 화면에 표시할 정보 관련
    void        DisplayInfo(RtCharset* pCharSet);                               ///< Clump 정보를 화면상에 표시한다.

protected:
	void		SetClumpInfo();							///< Clump 관련 정보를 Map에 세팅한다.
    void        UpdateHeightWidth();                    ///< 높이와 넓이 정보를 업데이트 한다.
    void        UpdateAtomicBB();                       ///< AtomicBB들을 Update한다.

    // CallBack 함수들
    static RpAtomic*	GetClumpAllAtomics(RpAtomic* pAtomic, void* pData);		///< Clump에서 모든 Atomic을 가져와 설정하는 콜백 함수
    static RwFrame*     GetChildFrame(RwFrame* pFrame, void* pData);
    static RpMaterial*  GetAllMaterials(RpMaterial* material, void* pData);


public:
	FRAME_TABLE_MAP		m_mapFrameTable;				///< Frame 테이블
	MATERIAL_TABLE_MAP	m_mapMaterialtable;				///< Material 테이블
	ATOMIC_TABLE_MAP	m_mapAtomicTable;				///< Atomic 테이블
    MATERIAL_ATOMIC_MAP m_mapMaterialAtomicTable;       ///< Material - Atomic 테이블

    static RpAtomic*    m_pTempAtomic;                  ///< Material - Atomic MAP을 만들기 위한 임시 Atomic 포인터

protected:
    static SToonData    m_ToonData;                     ///< Toon에 사용될 객체
    RwChar              m_strClumpName[1024];           ///< Clump의 이름
	RpClump*			m_pClump;		    			///< 정보를 읽어올 Clump
    CNtlPLResource*     m_pResourceClump;               ///< Clump Load 사용할 Resource

    RwBool              m_bVisible;                     ///< 화면에 나타나는 유무
    std::vector<RwChar*>  m_vecBB;				        ///< 바운딩 박스 리스트        

    // 화면에 표시할 정보들
    static int          m_nTotalTriangels;              ///< 전체 Triangle 개수
    static int          m_nTotalVertices;               ///< 전체 Vertex 개수

    RwBBox              m_bboxCharacter;                ///< 캐릭터의 Default BBox
    RwReal              m_fClumpHeight;                 ///< Clump의 높이
    RwReal              m_fClumpWidth;                  ///< Clump의 좌우 넓이
    RwReal              m_fClumpDepth;                  ///< Clump의 앞뒤 넓이

    std::vector<RwTexture*> m_vMultiTexture;             ///< MultiTexture 리스트 (나중에 해제를 위해 필요하다)

};

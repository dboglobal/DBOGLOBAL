#pragma once

#include "MTDef.h"
#include "NtlPLCharacter.h"
#include "MTCharacterInfo.h"

#define DISPATCHSIZE (1000)

typedef std::map<int, std::string>      BONENAME_MAP;

/*!
 * \brief
 * Model Tool Character 관련 클래스
 * 
 * CNtlPLCharacter를 상속받아서 필요한 부분을 오버라이딩하여 사용한다.
 * 
 * \remarks
 * Write remarks for CMTCharacter here.
 * 
 * \see
 * CNtlPLCharacter
 */
class CMTCharacter : public CNtlPLCharacter
{
public:

	// CNtlPLCharacter 클래스의 메모리풀에 문제가 있어서. 강제로 오버라이딩 헀다.-_-a
	void* operator new(size_t size);
	void operator delete(void *pObj);

	CMTCharacter(void);
	virtual ~CMTCharacter(void);
	
	//--- 오버라이딩
	virtual RwBool Create(const SPLEntityCreateParam *pParam  = NULL );		///< 초기화 작업을 해준다.
	virtual RwBool Update(RwReal fElapsed);
	virtual RwBool Render(void);    
	//--------------------------------------------------------------------

	RwBool  LoadClump(RwChar* filename);				                	///< clump 파일을 로딩한다.	
    RwBool  SaveClump(RwChar* fileName);                                    ///< Clump 파일을 저장한다.
	RwBool	RemoveAtomic(RwChar* chAtomicName);			                	///< Atomic을 Clump에서 제거한다.
    void    UpdateClumpInfo();                                              ///< Clump의 정보를 새로 갱신한다.
    void    DisplayMaterialAttribute(RwChar* chMaterialName);               ///< Material의 현재 Attribute를 표시한다.
    void    DisplayInfo(RtCharset* pCharSet);                               ///< 화면에 정보를 표시한다.

    // Clump 속성 관련
    void    Set2Side(RwChar* chAtomicName, RwBool b2Side);                  ///< 2 Side 플래그를 설정한다.
    void    Set2SideEdge(RwChar* chAtomicName, RwBool b2SideEdge);          ///< 2 Side Edge 플래그를 설정한다.
    void    SetAlphaTest(RwChar* chAtomicName, RwBool bAlpha);              ///< Alpha Test 플래그를 설정한다.
    void    SetVisible(RwBool bVisible);                                    ///< 화면에 모델의 표시 유무를 설정한다.

    // Material 관련
    void    SetMaterialColor(RwChar* chMaterialName, int r, int g, int b);  ///< Material에 Color를 적용한다.
    RwBool  SetMultiTexture(RwChar* chMaterialName, RwChar* chTextureName, RwChar* chTexturePath); ///< Material에 MultiTexture를 적용한다.

    // 바운딩 박스 관련
	void	CreateBB(RwChar* chAtomicName);				                	///< 해당 Atomic의 BB를 생성한다.
	void    ClearBB();								    	                ///< 바운딩 박스 정보를 비운다.

    // WireFrame 렌더링 관련
    void              RenderWireFrame(void);                                ///< WireFrame을 렌더링 한다.
    static RpAtomic*  AtomicRenderWireMesh(RpAtomic *atomic, void * data);    

    // Hierarchy 렌더링 관련
    void             RenderHierarchy(void);                                 ///< Hierarchy를 렌더링 한다.
    static RpAtomic* AtomicRenderSkeleton(RpAtomic *atomic, void * data);
    static RwFrame*  HierarchyRender(RwFrame *frame, void * data);
    static void      SkeletonRender(RwMatrix *LTM, RpHAnimHierarchy *hier);

    // Bone 관련
    RwInt32          GetBoneNum() {return m_nBoneCount;};                      ///< Bone의 개수를 반환한다.    
    RwChar*          GetBoneName(int nBoneIndex);                            ///< Bone의 이름을 반환한다.
    void             SetSelectBone(int nBoneIndex) {m_nCurrentSelectBoneIndex = nBoneIndex;}; ///< 렌더링할 Bone을 선택한다.
    void             SetRenderBone(RwBool bRender) {m_bRenderBone = bRender;}; ///< Bone의 렌더링 유무를 결정한다.
    
    

    // Animation 관련   
    RwBool          SetAnim(RwUInt32 uiKey);                                 ///< 애니메이션을 모델에 적용한다.    

    // 스크립트 관련
    void            ResetProperty();                                        ///< 캐릭터 프로퍼티의 모든 내용을 리셋한다.
    CNtlPLCharacterProperty* GetProperty() {return m_pProperty;};           ///< 캐릭터의 프로퍼티의 포인터를 반환한다.

protected:
	//--- 오버라이딩
	virtual RwBool CreateCartoon();							///< 카툰을 생성한다.
	virtual RwBool CreateMesh(const char *szDffName);		///< Mesh를 로딩한다
	//----------------------------------------------------------------------
	
	void   RenderBB(RwBBox* pBBox);		        			///< 바운딩 박스를 렌더링 한다.
	void   DisplayClumpInfo();								///< Clump의 정보를 표시한다.

    // Bone 관련    
    void   RenderSelectedBoneScale(RpHAnimHierarchy *pHierarchy, RwInt32 nCurrentBone);    ///< Bone을 렌더링 한다.

    
	
protected:
	MTCharacterInfo		m_charInfo;							///< 캐릭터 리소스 정보를 관리하는 객체
	std::vector<RwBBox> m_vecBB;							///< 바운딩 박스 리스트        
    std::vector<RwTexture*> m_vMultiTexture;                ///< MultiTexture 리스트 (나중에 해제를 위해 필요하다)
    bool                m_bAnim;                            ///< 애니메이션의 적용 유무
    RtAnimAnimation*    m_pAnimation;                       ///< 캐릭터에 적용하는 애니메이션

    // Bone 관련    
    RwInt32             m_nCurrentSelectBoneIndex;          ///< 현재 선택된 Bone의 Index;
    RwBool              m_bRenderBone;                      ///< Bone의 렌더링 유무
    RwBool              m_bVisible;                         ///< 현재 화면에 표시되는지 유무
    int                 m_nBoneCount;

};

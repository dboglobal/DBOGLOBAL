/*****************************************************************************
 *
 * File			: NtlPLObjectProperty.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 8. 22	
 * Abstract		: Presentation layer object property class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_PLOBJECT_PROPERTY_H__
#define __NTL_PLOBJECT_PROPERTY_H__

#include "NtlPLProperty.h"
#include "NtlTypeAnimData.h"

#define XML_PARSING_BUFFER_SIZE		( 1024 )

/// 오브젝트의 타입
enum EPLObjectType
{
    E_OBJECT_NORMAL = 0,                        ///< 일반 오브젝트
    E_OBJECT_MILEPOST,                          ///< 이정표용 오브젝트 (화살표)
};

/// 오브젝트 플래그 (기존의 Bool값들도 플래그로 바꾸고 싶지만, 그러면 하위 호환성이 깨진다)
#define OBJ_FLAG_SORT_BY_POS        0x00000001
#define OBJ_FLAG_FORCE_PICKING      0x00000002
#define OBJ_FLAG_PE_NAVI_PRESS      0x00000004
#define OBJ_FLAG_CULLTEST_ATOMIC    0x00000008

/**
* \ingroup NtlPresentation
* CNtlPLObject에 해당하는 property class이다.
* property data는 xml file로 작성되어져 있다.
*
*/
class CNtlPLObjectProperty : public CNtlPLProperty
{
public:

	static unsigned int m_strVer;			/* property file version **/
	static std::string m_strObjectDataPath;	/* Object Data File Path(*.xml) **/
	static std::string m_strMeshPath;		/* object mesh file의 path name **/
	static std::string m_strAniPath;		/* object animation file의 path name **/
	static std::string m_strTexPath;		/* object texture file의 path name **/

	std::string m_strDffFile;				/* object mesh file name **/
    std::string m_strUVAnimFileName;        ///< UVAnim File Name (상대 경로 포함)
    std::string m_strAnimFileName;          ///< AnimFileName (상대 경로 포함)    
    std::string m_strCollisionMeshName;     ///< Path Engine용 Collision Mesh File Name

	std::vector<SEventLinkEffect*> m_vLinkEffect; ///< Link Effect의 리스트 

protected:
    RwBBox          m_bbox;                         ///< Server Object의 BBox 
    RwBool          m_bCollision;                   ///< Server Collision 유무
	RwBool          m_bShadow;                      ///< Shadow 표시 유무    
    RwReal          m_fUvAnimSpeed;                 ///< UVAnim Speed    
    RwReal          m_fCullingDistance;             ///< Culling Distance
    RwBool          m_bApplyPVS;                    ///< PVS 영향을 받는지 안받는지 유무
    RwInt32         m_nObjectFlag;

	CNtlTypeAnimTable m_TypeAnimTable;				///< Animation Table
    EPLObjectType   m_eObjectType;                  ///< Object Type

    SEventSound     m_SoundProp;                    ///< 기본 재생 사운드 프로퍼티

    

public:
	CNtlPLObjectProperty();
	virtual ~CNtlPLObjectProperty();

	// 'ObjectPropertyList.xml'에서 프로퍼티 파일명을 가져오는 함수
	virtual RwBool	Load(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode);	
	virtual RwBool	Save(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode );

    virtual RwBool  SaveScript(const std::string &strFileName);          ///< XML 스크립트 파일로 저장한다.
    virtual RwBool  LoadScript(const RwChar* strFileName);          ///< XML 스크립트로 부터 데이터를 로드한다.

	virtual void	SaveSerialize(CNtlSerializer& sOut);											///< Serialize에 데이터를 저장하는 함수
	virtual void	LoadSerialize(CNtlSerializer& sIn);												///< Serialize로부터 데이터를 가져오는 함수
	
	void                    SetDffName(const RwChar* strDffName) {m_strDffFile = strDffName;}
	const char *			GetDffName( void ) const { return m_strDffFile.c_str(); }

    void    SetEnableShadow(RwBool bEnable) {m_bShadow = bEnable;}
    RwBool  GetEnableShadow() {return m_bShadow;}

    void    SetBBox(RwBBox bbox) {m_bbox = bbox;};                                  ///< BBox를 설정한다
    RwBBox* GetBBox() {return &m_bbox;}                                             ///< 설정된 BBox를 반환한다.

    void    SetUVAnimSpeed(RwReal fSpeed) {m_fUvAnimSpeed = fSpeed;}                ///< UVAnim 속도를 설정한다
    RwReal  GetUVAnimSpeed() {return m_fUvAnimSpeed;}                               ///< UVAnim 속도를 반환한다.

    void    SetCollision(RwBool bCollision) {m_bCollision = bCollision;}            ///< Collision 가능 유무를 설정한다.
    RwBool  GetCollision() {return m_bCollision;}                                   ///< Collision 가능 유무를 반환한다.

    void    SetCullingDistance(RwReal fDistance) {m_fCullingDistance = fDistance;}  ///< Culling Distance를 설정한다.
    RwReal  GetCullingDistance() {return m_fCullingDistance;}                       ///< Culling Distance를 반환한다.

	CNtlTypeAnimTable* GetAnimTable() {return &m_TypeAnimTable;}                        ///< AnimTable을 반환한다.

    void    SetObjectType(EPLObjectType eObjectType) {m_eObjectType = eObjectType;} ///< Object Type을 설정한다.
    EPLObjectType GetObjectType() {return m_eObjectType;}                           ///< Object Type을 반환한다.

    void    SetApplyPVS(RwBool bApplyPVS) {m_bApplyPVS = bApplyPVS;}                ///< PVS에 영향을 받는지 안받는지 설정한다.
    RwBool  GetApplyPVS() {return m_bApplyPVS;}                                     ///< PVS에 영향을 받는지 안받는지 반환한다.

    SEventSound* GetSoundProp() {return &m_SoundProp;}                              ///< 오브젝트의 사운드 설정을 반환한다.

    // 플래그 관련 함수들
    RwBool  IsSortByPos() {return (GetFlag() & OBJ_FLAG_SORT_BY_POS);}
    RwBool  IsForcePicking() {return (GetFlag() & OBJ_FLAG_FORCE_PICKING);}
    RwBool  IsNaviPress()   {return (GetFlag() & OBJ_FLAG_PE_NAVI_PRESS);}           ///< 패스 엔진용으로 Obstacle Mesh에 대한 설정
    RwBool  IsCullTestAllAtomic() {return (GetFlag() & OBJ_FLAG_CULLTEST_ATOMIC);}     ///< 애니메이션에서 Atomic에 대해서 Culling Test를 할지 유무를 반환한다.

};
#endif
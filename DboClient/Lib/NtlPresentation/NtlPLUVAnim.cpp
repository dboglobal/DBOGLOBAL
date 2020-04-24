#include "precomp_ntlpresentation.h"
#include "NtlEffectAPI.h"
#include "NtlPLUVAnim.h"
#include "NtlTimer.h"
#include "NtlPLResourcePack.h"
#include "NtlPLApi.h"

CNtlPLUVAnim::CNtlPLUVAnim(void)
: m_fUVAnimTime(1.0f)
{
    m_pUvAnimDict = NULL;
    m_pAnimationMaterialList = NULL;
}

CNtlPLUVAnim::~CNtlPLUVAnim(void)
{
    Destory();
}

void CNtlPLUVAnim::Destory() 
{
    if (m_pAnimationMaterialList != NULL)
    {
        rwSListDestroy(m_pAnimationMaterialList);
        m_pAnimationMaterialList = NULL;
    }

    if (m_pUvAnimDict != NULL)
    {
        RtDictDestroy(m_pUvAnimDict);
        m_pUvAnimDict = NULL;
    }

    m_fUVAnimTime = 1.0f;
}


/**
 * uva파일로 부터 UVAnim을 생성하여 Clump에 적용한다.
 * \param szUVFileName 적용할 uvAnim 파일경로 (*.uva)
 * \param pClump UVAnim을 적용할 Clump 객체
 * return 성공 유무
 */
RwBool CNtlPLUVAnim::Create( const char* szUVFileName, RpClump* pClump ) 
{
    if(!szUVFileName || !pClump)
        return FALSE;

    if(m_pUvAnimDict)
    {
        Destory();
    }

    m_pUvAnimDict = UVAnimDictLoad(szUVFileName);
    if(!m_pUvAnimDict)
        return FALSE;

    RtDictSchemaSetCurrentDict(RpUVAnimGetDictSchema(), m_pUvAnimDict);    
    
    m_pAnimationMaterialList = rwSListCreate(sizeof(RpMaterial*), rwID_NAOBJECT);
    API_AddAnimatedMaterialsList(m_pAnimationMaterialList, pClump);

    return TRUE;
}

RwBool CNtlPLUVAnim::Create( const char* szUVFileName ) 
{
    if(m_pUvAnimDict)
    {
        Destory();
    }

    m_pUvAnimDict = UVAnimDictLoad(szUVFileName);
    if(!m_pUvAnimDict)
        return FALSE;


    // NOTE: 화면에 서로다른 UVAnim이 적용된 Object가 두개이상있을때. 문제가 있지 않을까?
    // 차후에 결과를 보고, 문제가 있으면 수정한다.
    // DIct객체에 Entry(RpUVAnim*)를 추가하는 형태로 변경이 필요할지도 모르겠다.
    RtDictSchemaSetCurrentDict(RpUVAnimGetDictSchema(), m_pUvAnimDict);    

    return TRUE;
}

RwBool CNtlPLUVAnim::SetClump( RpClump* pClump ) 
{
    m_pAnimationMaterialList = rwSListCreate(sizeof(RpMaterial*), rwID_NAOBJECT);
    API_AddAnimatedMaterialsList(m_pAnimationMaterialList, pClump);

    return TRUE;
}

void CNtlPLUVAnim::Update( RwReal fElapsedTime ) 
{
    if(!m_pAnimationMaterialList)
        return;

    //API_MaterialsInterpolatorsAddAnimTime(m_pAnimationMaterialList, fElapsedTime * m_fUVAnimTime);    

    // 복수개의 Clump에 Material이 공통적으로 적용이되기 때문에, AddTime이 아닌 SetCurrentTime을 적용한다.
    API_MaterialsInterpolatorsSetCurrentTime(m_pAnimationMaterialList, CNtlTimer::GetAppTime() * m_fUVAnimTime);

    API_MaterialsAnimApply(m_pAnimationMaterialList);
}

/**
 * *.uva 파일을 로드한다.
 * \param strFullName uva파일경로
 * return uva파일을 로드한 RtDict 객체
 */
RtDict* CNtlPLUVAnim::UVAnimDictLoad( const RwChar* strFullName ) 
{
	return API_PL_LoadUVAnimDict(strFullName);
}



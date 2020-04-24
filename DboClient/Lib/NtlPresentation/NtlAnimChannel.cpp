#include "precomp_ntlpresentation.h"
#include "NtlAnimChannel.h"
#include "NtlDebug.h"

/**
 * AnimChannel를 생성
 * \param pBaseHierarchy 미리 생성된 BaseHierarchy
 * \return 성공 유무
 */
RwBool CNtlAnimChannel::Create(RpHAnimHierarchy *pBaseHierarchy)
{
	NTL_FUNCTION("CNtlAnimChannel::Create()");
	NTL_ASSERTE(pBaseHierarchy != NULL);
	NTL_ASSERTE(pBaseHierarchy->currentAnim->pCurrentAnim != NULL);
	if(pBaseHierarchy == NULL || pBaseHierarchy->currentAnim->pCurrentAnim == NULL)
		NTL_RETURN(FALSE);

	m_pHierarchy = RpHAnimHierarchyCreateFromHierarchy( pBaseHierarchy, 
			                                            RpHAnimHierarchyGetFlags(pBaseHierarchy),
			                                            pBaseHierarchy->currentAnim->maxInterpKeyFrameSize);
	
	NTL_ASSERTE(m_pHierarchy);
	if(m_pHierarchy == NULL)
		NTL_RETURN(FALSE);

	//Default Anim이 없으면 AddTogether 연산시 에러가 난다.
	RpHAnimHierarchySetCurrentAnim( m_pHierarchy, pBaseHierarchy->currentAnim->pCurrentAnim);
	
	NTL_RETURN(TRUE);
}

/**
 * Animation Update
 * \return 성공 유무
 */
RwBool CNtlAnimChannel::Update(RwReal fElapsed)
{
	NTL_FUNCTION("CNtlAnimChannel::Update");
	RpHAnimHierarchyAddAnimTime(m_pHierarchy, fElapsed);
	NTL_RETURN(TRUE);
}

/**
 * Destroy
 */
void CNtlAnimChannel::Destroy()
{
	NTL_FUNCTION("CNtlAnimChannel::Update");
	if(m_pHierarchy)
	{
		RpHAnimHierarchyDestroy(m_pHierarchy);
		m_pHierarchy = NULL;
	}

	NTL_RETURNVOID();
}

/**
 * AnimChannel의 Hierarchy를 얻어온다.
 * \return RpHAnimHierarchy
 */
RpHAnimHierarchy *CNtlAnimChannel::GetHierarchy()				
{
	NTL_ASSERTE(m_pHierarchy != NULL);
	return m_pHierarchy; 
}

/**
 * Animation Data를 적용을 한다.
 * \param 
 * \return RpHAnimHierarchy
 */
void CNtlAnimChannel::SetAnimData(SInstanceAnimData *pInstanceAnimData)	
{
	NTL_ASSERTE(pInstanceAnimData);
	NTL_ASSERTE(pInstanceAnimData->pResource);

	m_pAnimData = pInstanceAnimData;
	RpHAnimHierarchySetCurrentAnim(m_pHierarchy, pInstanceAnimData->pResource->GetAnimation());
}

SInstanceAnimData *CNtlAnimChannel::GetAnimData()				
{
	NTL_ASSERTE(m_pAnimData);
	return m_pAnimData;
}


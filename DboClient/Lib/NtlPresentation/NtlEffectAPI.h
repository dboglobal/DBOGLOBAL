

#ifndef _NTLEFFECT_API_
#define _NTLEFFECT_API_

#include <rphanim.h>
#include <rpskin.h>
#include <rpuvanim.h>
#include <rtdict.h>


RpAtomic*	API_GetSkinHierarchy(RpAtomic *atomic, void *data);
void		API_SetAnimation(RpClump* pClump, RtAnimAnimation* pAnimation);

RpAtomic*	API_MeshColorUpdate(RpAtomic *pAtomic, void *data __RWUNUSED__);

/// Clump에서 UVAnim이 적용된 Material을 찾아서 List에 추가한다.
RwSList*	API_AddAnimatedMaterialsList(RwSList* pList, RpClump* pClump);

/// UVAnim의 시간을 업데이트한다 (시간 추가)
void		API_MaterialsInterpolatorsAddAnimTime(RwSList *animatedMaterials, RwReal deltaTime);
/// UVAnim의 시간을 업데이트 한다 (시간 설정)
void        API_MaterialsInterpolatorsSetCurrentTime(RwSList* animatedMaterials, RwReal fTime);
/// UVAnim을 업데이트한다.
void		API_MaterialsAnimApply(RwSList *animatedMaterials);

#endif
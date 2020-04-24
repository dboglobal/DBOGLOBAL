/*****************************************************************************
 *
 * File			: NtlMatExt
 * Author		: HongHoDong
 * Copyright	: (주)NTL
 * Date			: 2005. 11. 27	
 * Abstract		: NTl Mat Extension
 *****************************************************************************
 * Desc         : Material Extension
 *
 *****************************************************************************/
#ifndef _NTL_MATERIAL_EXT_CORE_H_
#define _NTL_MATERIAL_EXT_CORE_H_

#include <rwcore.h>
#include <rpworld.h>
#include "../Ntl_Plugin_Toon/toon/NtlMaterialExt.h"

RwBool		NtlMatExtPluginAttach(void);

RwBool		NtlMatExtSetMultiTexture(RpMaterial *pMaterial, RwTexture *pTexture);	/**<Set MultiTexture 현재 1장만 지원 */
RwTexture  *NtlMatExtGetMultiTexture(RpMaterial *pMaterial);						/**<Get MultiTexture 현재 1장만 지원 */

void		NtlMatExtSetFlag(RpMaterial *pMaterial, RwUInt32 bFlag);				/**<Set Material 속성 Flag Ntl_PluginToon의 NtlMaterialExt.h 참조 */
RwUInt32	NtlMatExtGetFlag(RpMaterial *pMaterial);								/**<Get Material 속성 Flag Ntl_PluginToon의 NtlMaterialExt.h 참조 */

void		NtlMatExtSetPipeline(RpAtomic *pAtomic);

#endif
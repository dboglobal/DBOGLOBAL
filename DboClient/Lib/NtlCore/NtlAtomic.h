/*****************************************************************************
 *
 * File			: NtlAtomicExt
 * Author		: HongHoDong
 * Copyright	: (주)NTL
 * Date			: 2005. 11. 27	
 * Abstract		: NTl Atomic Extension
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/
#ifndef _NTL_ATOMIC_EXT_H_
#define _NTL_ATOMIC_EXT_H_

#include <rwcore.h>
#include <rpworld.h>

/**< !!!!!!!!!!!!!!!!!!!!!!Clump에 저장이 되므로 기존 값들을 절대 변경 하시면 안됩니다.!!!!!!!!!!!!!!!!!!!!!!! */
enum ENtlAtomicFlag
{
	NTL_ATOMIC_FLAG_INVALID	= 0x00000000,

	NTL_TOON_NOT_EDGE		= 0x00000001, /**<Edge가 Rendering이 되지않음 */
	NTL_TOON_DETAIL_EDGE	= 0x00000002, /**<Edge만 Vertex로 만든 Detail한 Edge가 찍힘*/
	
	NTL_TWOSIDE             = 0x00000010, /**<TwoSide 공통 */
	NTL_ALPHATEST			= 0x00000020, /**<AlphaTest 공통 */
	NTL_ALPHA				= 0x00000040, /**<Alpha 공통 */
	
	NTL_COLLISION			= 0x00001000, /**<Collision을 할건지 말건지 */
	NTL_NOT_VISIBLE			= 0x00002000, /**<화면에 그릴지 안그릴지의 유무*/
	NTL_DECAL_VISIBLE		= 0x00004000, /**<Decal을 표시할 건지 유무 */
    NTL_CAMERA_COLLISION    = 0x00020000, /**<Not Visible 체크가 된것중에서 카메라 충돌을 할지의 유무 */

	NTL_SHADOW_MAP			= 0x00008000, /**<ShadowMap을 생성할 건지(ShadowMap이 있는지 유무) */
	
	NTL_RUNTIME_ALPHA		= 0x00010000, /**<RunTime에 Alpha가 적용이 되는 경우 */
	NTL_TOON_EDGE_OFF		= 0x00040000,
	NTL_ENVMAP_TEX			= 0x00080000, /**<Enviroment map texture name */
	NTL_EMBLEM_MARK			= 0x00100000,

	NTL_FINAL_SORT          = (NTL_ALPHA | NTL_RUNTIME_ALPHA) /**<Alpha or RunTimeAlpha */
};


RwBool		RpNtlAtomicPluginAttach(void);
void		RpNtlClumpSetData(RpClump* pClump, void *pData);
void		RpNtlAtomicSetData(RpAtomic* pAtomic, void *pData);
void*		RpNtlAtomicGetData(const RpAtomic* pAtomic);

void		RpNtlAtomicSetFlag(RpAtomic *pAtomic, RwUInt32 uiFlag);				/**<ENtlAtomicFlag Option */
RwUInt32	RpNtlAtomicGetFlag(RpAtomic *pAtomic);								/**<ENtlAtomicFlag Option */

RwTexture*	RpNtlAtomicGetUserDat4RwTex(RpAtomic* _pAtomic);
void		RpNtlAtomicSetUserDat4RwTex(RpAtomic* _pAtomic, RwTexture* _pUserDat);
void		RpNtlAtomicSetUserDat(RpAtomic* _pAtomic, void* _pUserDat);
void*		RpNtlAtomicGetUserDat(RpAtomic* _pAtomic);
void		RpNtlAtomicSetUserDatU16(RpAtomic *pAtomic, RwUInt16 uiSize);
RwUInt16	RpNtlAtomicGetUserDatU16(RpAtomic *pAtomic);					
void		RpNtlAtomicSetUserDatReal(RpAtomic *pAtomic, RwReal uiSize);
RwReal		RpNtlAtomicGetUserDatReal(RpAtomic *pAtomic);					
void		RpNtlAtomicSetEnvTexName(RpAtomic* _pAtomic, RwUInt16 _EnvTexName);
RwUInt16	RpNtlAtomicGetEnvTexName(RpAtomic* _pAtomic);

void		RpNtlAtomicSetAlpha(RpAtomic* pAtomic, RwUInt8 alpha);
RwUInt8		RpNtlAtomicGetAlpha(RpAtomic* pAtomic);

RpAtomicCallBackRender RpNtlAtomicGetRenderCallBack(RpAtomic *pAtomic);
void RpNtlAtomicSetRenderCallBack(RpAtomic *pAtomic, RpAtomicCallBackRender atomicCallBackRender);

#endif
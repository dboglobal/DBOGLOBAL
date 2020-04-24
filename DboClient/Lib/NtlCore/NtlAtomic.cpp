#include "precomp_ntlcore.h"
#include "NtlAtomic.h"
#include "NtlDebug.h"
//#include "NtlToonMaterial.h"

/**
 * Plugin 고유 ID 다른 Plugin과 같은 ID가 있으면 안된다.
 * 원래는 Renderware에 보고를 하고 만들어야 한다.
 */
const RwUInt32 PluginID = 0xFE;

/**
 * Version 0: RwUInt32 uiFlag
 * Version 1: RwUint32 uiFlag, RwUint16 UserDat 
 */
const RwUInt16 NTL_ATOMIC_SAVE_VER0 = 0;

const RwUInt16 NTL_ATOMIC_SAVE_VER = 1;

const RwUInt16 NTL_ATOMIC_SAVE_VER1 = 2;

/**
 * 추가 Data
 */
struct NtlAtomicPluginData
{
	void		*pData;

	RwUInt16	uiVersion;			//Save Version
	RwUInt32	uiFlag;				//Atomic Flag

	// 현재 pUser는 메모리상에서 범용적으로 쓰일수 있도록 만들어져 있으면 오브젝트 쉐도우쪽에서 사용하고 있습니다.
	// NtlPLObj에서 오브젝트 쉐도우 플래그가 없다면 이 Pointer를 공용으로 사용하셔도 됩니다
	// 또한 이 변수는 공용으로 쓰이기 때문에 생성, 삭제에 대한 책임은 각각의 모쥴에서 해결해야 합니다.
	// 플러그인 디스트럭트에서 경우에 맞는 소멸문제가 생깁니다. 이값은 클론되지 않습니다.  - 우디
	void* pUserDat;				

	// 이 변수는 오브젝트 쉐도우 전용 변수 입니다. 위와 마찬가지로 플래그가 없다면 이 변수를 공용으로 사용 하실수 있습니다. 이값은 클론됩니다. 주의하세여. - 우디
	RwUInt16 UserDat;

	// 이 변수는 오브젝트 쉐도우 전용 변수 입니다. 위와 마찬가지로 플래그가 없다면 이 변수를 공용으로 사용 하실수 있습니다. 이값은 클론됩니다. 주의하세여. - 우디
	RwReal _UserDatReal;

	// Atomic에 Modulate될 Alpha값
	RwUInt8		alpha;

	RpAtomicCallBackRender atomicRenderCallBack;

	// 이 변수는 환경맵 전용 변수 입니다. 위와 마찬가지로 플래그가 없다면 이 변수를 공용으로 사용 하실수 있습니다. 이값은 클론됩니다. 주의하세여. - 우디
	RwUInt16 _EnvTexName;
};

/**
 * 확장 된 Offset Memory 주소
 */
unsigned int iPlugin_Offset = 0;

/**
 * 확장 Offset 접근 함수
 */
inline NtlAtomicPluginData* NtlGetPluginData( RpAtomic* pAtomic)
{
	NTL_FUNCTION("NtlGetPluginData()");

	NTL_PRE(iPlugin_Offset);

	const RwUInt32 baseAddr = reinterpret_cast<RwUInt32>(pAtomic);
	const RwUInt32 pluginAddr = baseAddr + iPlugin_Offset;

	NTL_RETURN( reinterpret_cast<NtlAtomicPluginData*>(pluginAddr) );
}

/**
 * 확장 Offset 접근 함수
 */
inline const NtlAtomicPluginData *NtlGetPluginData( const RpAtomic* pAtomic )
{
	NTL_FUNCTION("NtlGetPluginData()");

	NTL_PRE(iPlugin_Offset);

	const RwUInt32 baseAddr = reinterpret_cast<RwUInt32>(pAtomic);
	const RwUInt32 pluginAddr = baseAddr + iPlugin_Offset;

	NTL_RETURN( reinterpret_cast<const NtlAtomicPluginData*>(pluginAddr) );
}

/**
 * Constructor
 */
void* Plugin_Constructor(void* pObject, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
	NTL_FUNCTION("Plugin_Constructor()");

	NTL_PRE(pObject);

	NtlAtomicPluginData* pPluginData	= NtlGetPluginData((RpAtomic*)pObject);
	pPluginData->pData					= NULL;
	pPluginData->uiVersion				= NTL_ATOMIC_SAVE_VER1;
	pPluginData->uiFlag					= NTL_ATOMIC_FLAG_INVALID;
	pPluginData->pUserDat				= NULL;
	pPluginData->UserDat				= 0;
	pPluginData->_UserDatReal			= 1.f;
	pPluginData->alpha					= 255;
	pPluginData->_EnvTexName			= 0;

	pPluginData->atomicRenderCallBack	= NULL;

	NTL_RETURN(pObject);
}

/**
 * Destructor
 */
static void *Plugin_Destructor( void* pObject, RwInt32  offset, RwInt32  size __RWUNUSED__)
{
	NTL_FUNCTION("Plugin_Destructor()");

	NTL_PRE(pObject);

	/*
	NtlAtomicPluginData* pPluginData = NtlGetPluginData((RpAtomic*)pObject);

	if(pPluginData->pUserDat != NULL)
		RwTextureDestroy(pPluginData->pUserDat);
		*/

	NTL_RETURN(pObject);

}

/**
 * Copy
 */
void *Plugin_Copy(void* pDestObject, const void* pSrcObject, RwInt32 offset, RwInt32 size)
{
	NTL_FUNCTION("Plugin_Copy");
	NTL_PRE(pDestObject);
	NTL_PRE(pSrcObject);

	//void Data는 Copy를 하지 않는다. Pointer가 잘못 되어서 Error의 소지가 있다.	
	NtlAtomicPluginData* pSrcData  = NtlGetPluginData((RpAtomic*)pSrcObject);
	NtlAtomicPluginData* pDestData = NtlGetPluginData((RpAtomic*)pDestObject);

	pDestData->uiVersion    = pSrcData->uiVersion;
	pDestData->uiFlag		= pSrcData->uiFlag;
	pDestData->UserDat		= pSrcData->UserDat;
	pDestData->_UserDatReal = pSrcData->_UserDatReal;
	pDestData->_EnvTexName  = pSrcData->_EnvTexName;

	NTL_RETURN(pDestObject);
}

/**
 * Stream Read
 */
static RwStream *Plugin_StreamRead( RwStream *stream,
								    RwInt32 length __RWUNUSED__,
									void *pAtomic,
									RwInt32 offset __RWUNUSED__,
									RwInt32 size __RWUNUSED__)
{
	NtlAtomicPluginData* pPluginData = NtlGetPluginData((RpAtomic*)pAtomic);

	// default
	RwUInt16 uiNtlAtomicSaveVer = NTL_ATOMIC_SAVE_VER0;

	if (RwStreamRead(stream, (void *)(&(uiNtlAtomicSaveVer)), sizeof(uiNtlAtomicSaveVer)) != sizeof(uiNtlAtomicSaveVer))
		return NULL;
	
	pPluginData->uiVersion = uiNtlAtomicSaveVer;
	if(pPluginData->uiVersion == NTL_ATOMIC_SAVE_VER0)
	{
		if (RwStreamRead(stream, (void *)(&(pPluginData->uiFlag)), sizeof(pPluginData->uiFlag)) 
			!= sizeof(pPluginData->uiFlag))
			return NULL;
	}
	else if( pPluginData->uiVersion == NTL_ATOMIC_SAVE_VER)
	{
		if (RwStreamRead(stream, (void *)(&(pPluginData->uiFlag)), sizeof(pPluginData->uiFlag)) 
			!= sizeof(pPluginData->uiFlag))
			return NULL;
		
		if (RwStreamRead(stream, (void *)(&(pPluginData->UserDat)), sizeof(pPluginData->UserDat)) 
			!= sizeof(pPluginData->UserDat))
			return NULL;

		if (RwStreamRead(stream, (void *)(&(pPluginData->_UserDatReal)), sizeof(pPluginData->_UserDatReal)) 
			!= sizeof(pPluginData->_UserDatReal))
			return NULL;
	}
	else if( pPluginData->uiVersion == NTL_ATOMIC_SAVE_VER1)
	{
		if (RwStreamRead(stream, (void *)(&(pPluginData->uiFlag)), sizeof(pPluginData->uiFlag)) 
			!= sizeof(pPluginData->uiFlag))
			return NULL;

		if (RwStreamRead(stream, (void *)(&(pPluginData->UserDat)), sizeof(pPluginData->UserDat)) 
			!= sizeof(pPluginData->UserDat))
			return NULL;

		if (RwStreamRead(stream, (void *)(&(pPluginData->_UserDatReal)), sizeof(pPluginData->_UserDatReal)) 
			!= sizeof(pPluginData->_UserDatReal))
			return NULL;

		if (RwStreamRead(stream, (void *)(&(pPluginData->_EnvTexName)), sizeof(pPluginData->_EnvTexName)) 
			!= sizeof(pPluginData->_EnvTexName))
			return NULL;
	}
	else
		return NULL;

	return stream;
}

/**
 * Stream Write
 */
static RwStream *Plugin_StreamWrite( RwStream *stream,
									 RwInt32 length __RWUNUSED__,
									 const void *pAtomic,
									 RwInt32 offset __RWUNUSED__,
									 RwInt32 size __RWUNUSED__)
{
	NtlAtomicPluginData* pPluginData = NtlGetPluginData((RpAtomic*)pAtomic);
	
	if (!RwStreamWrite(stream, (void *)(&(pPluginData->uiVersion)), sizeof(pPluginData->uiVersion)))
		return NULL;

	if( pPluginData->uiVersion == NTL_ATOMIC_SAVE_VER0)
	{
		if (!RwStreamWrite(stream, (void *)(&(pPluginData->uiFlag)), sizeof(pPluginData->uiFlag)))
			return NULL;
	}
	else if( pPluginData->uiVersion == NTL_ATOMIC_SAVE_VER)
	{
		if (!RwStreamWrite(stream, (void *)(&(pPluginData->uiFlag)), sizeof(pPluginData->uiFlag)))
			return NULL;
		
		if (!RwStreamWrite(stream, (void *)(&(pPluginData->UserDat)), sizeof(pPluginData->UserDat)))
			return NULL;

		if (!RwStreamWrite(stream, (void *)(&(pPluginData->_UserDatReal)), sizeof(pPluginData->_UserDatReal)))
			return NULL;
	}
	else if( pPluginData->uiVersion == NTL_ATOMIC_SAVE_VER1)
	{
		if (!RwStreamWrite(stream, (void *)(&(pPluginData->uiFlag)), sizeof(pPluginData->uiFlag)))
			return NULL;

		if (!RwStreamWrite(stream, (void *)(&(pPluginData->UserDat)), sizeof(pPluginData->UserDat)))
			return NULL;

		if (!RwStreamWrite(stream, (void *)(&(pPluginData->_UserDatReal)), sizeof(pPluginData->_UserDatReal)))
			return NULL;

		if (!RwStreamWrite(stream, (void *)(&(pPluginData->_EnvTexName)), sizeof(pPluginData->_EnvTexName)))
			return NULL;
	}
	return stream;
}

/**
 * PlugIn을 Save를 할 경우에만 사용을 한다.
 */
static RwInt32 Plugin_StreamGetSize( const void *pAtomic,
                                     RwInt32     offset,
                                     RwInt32     sizeInObj __RWUNUSED__)
{
	NtlAtomicPluginData* pPluginData = NtlGetPluginData((RpAtomic*)pAtomic);
	
	// 항상 최신 버전을 저장 한다.(NTL_ATOMIC_SAVE_VER)
	//pPluginData->uiVersion = NTL_ATOMIC_SAVE_VER;
	//return (sizeof(pPluginData->uiVersion) + sizeof(pPluginData->uiFlag) + sizeof(pPluginData->UserDat) + sizeof(pPluginData->_UserDatReal) );

	pPluginData->uiVersion = NTL_ATOMIC_SAVE_VER1;
	return (sizeof(pPluginData->uiVersion) + sizeof(pPluginData->uiFlag) + sizeof(pPluginData->UserDat) + sizeof(pPluginData->_UserDatReal) + sizeof(pPluginData->_EnvTexName));
}

/**
 * Plugin 등록
 */      
RwBool RpNtlAtomicPluginAttach(void)
{
    NTL_FUNCTION("RpNtlAtomicPluginAttach");

	//Atomic 확장을 할경우 넘어오는 값이 실제로 확장이 된 Offset 주소 이다.
    iPlugin_Offset = RpAtomicRegisterPlugin (sizeof(NtlAtomicPluginData),
											 MAKECHUNKID(rwVENDORID_CRITERIONRM, PluginID),
											 Plugin_Constructor,  
											 Plugin_Destructor,
											 Plugin_Copy); //Plugin_Copy);
	RWS_POST(iPlugin_Offset);

	RwInt32 nOffset = -1;
    nOffset = RpAtomicRegisterPluginStream( MAKECHUNKID(rwVENDORID_CRITERIONRM, PluginID),
                                             Plugin_StreamRead,
                                             Plugin_StreamWrite,
                                             Plugin_StreamGetSize);

    if( nOffset != iPlugin_Offset)
    {
        NTL_RETURN(FALSE);
    }

    NTL_RETURN (0 != iPlugin_Offset);
}

/**
 * Plugin 확장 접근 함수
 */   
void RpNtlAtomicSetData( RpAtomic* pAtomic, void *pData)
{
    NTL_FUNCTION("RpNtlAtomicSetData()");

    NTL_PRE( iPlugin_Offset );

	NTL_ASSERTE(pAtomic);
    NtlGetPluginData(pAtomic)->pData = pData;

    NTL_RETURNVOID();
}

static RpAtomic *SetAllAtomic(RpAtomic* pAtomic, void *pData)
{
	RpNtlAtomicSetData(pAtomic, pData);
    
	return pAtomic;
}

void RpNtlClumpSetData(RpClump* pClump, void *pData)
{
	NTL_FUNCTION("RpNtlClumpSetData()");
	NTL_ASSERTE(pClump);
	RpClumpForAllAtomics( pClump, SetAllAtomic, (void *)pData);
	
	NTL_RETURNVOID();
}

void *RpNtlAtomicGetData( const RpAtomic* pAtomic )
{
    NTL_FUNCTION("NtlAtomicGetData");

    NTL_PRE( iPlugin_Offset );
	NTL_ASSERTE(pAtomic);
    NTL_RETURN(NtlGetPluginData(pAtomic)->pData);
}

void RpNtlAtomicSetFlag(RpAtomic* pAtomic, RwUInt32 uiFlag)
{
	NTL_FUNCTION("RpNtlAtomicSetRenderFlag()");

    NTL_PRE( iPlugin_Offset );

	NTL_ASSERTE(pAtomic);
	NtlGetPluginData(pAtomic)->uiFlag = uiFlag;
	
	NTL_RETURNVOID();
}

RwUInt32 RpNtlAtomicGetFlag(RpAtomic* pAtomic)
{
	NTL_FUNCTION("RpNtlAtomicGetRenderFlag()");

    NTL_PRE( iPlugin_Offset );
	NTL_ASSERTE(pAtomic);
	NTL_RETURN(NtlGetPluginData(pAtomic)->uiFlag);
}

RwTexture* RpNtlAtomicGetUserDat4RwTex(RpAtomic* _pAtomic)
{
	// careful for the old pUserDat had to be used for RwTextures
	return static_cast<RwTexture*>(NtlGetPluginData(_pAtomic)->pUserDat);
}

void RpNtlAtomicSetUserDat4RwTex(RpAtomic* _pAtomic, RwTexture* _pTex)
{
	// careful for the old pUserDat had to be used for RwTextures
	NtlGetPluginData(_pAtomic)->pUserDat = static_cast<void*>(_pTex);
}

void RpNtlAtomicSetUserDat(RpAtomic* _pAtomic, void* _pUserDat)
{
	NtlGetPluginData(_pAtomic)->pUserDat = _pUserDat;
}

void* RpNtlAtomicGetUserDat(RpAtomic* _pAtomic)
{
	return NtlGetPluginData(_pAtomic)->pUserDat;
}

void RpNtlAtomicSetEnvTexName(RpAtomic* _pAtomic, RwUInt16 _EnvTexName)
{
	NTL_ASSERTE(_pAtomic);

	NtlGetPluginData(_pAtomic)->_EnvTexName = _EnvTexName;
}

RwUInt16 RpNtlAtomicGetEnvTexName(RpAtomic* _pAtomic)
{
	NTL_ASSERTE(_pAtomic);

	return NtlGetPluginData((RpAtomic*)_pAtomic)->_EnvTexName;
}

void RpNtlAtomicSetUserDatU16(RpAtomic *pAtomic, RwUInt16 uiSize)
{
	NTL_ASSERTE(pAtomic);

	NtlGetPluginData(pAtomic)->UserDat = uiSize;
}

RwUInt16 RpNtlAtomicGetUserDatU16(RpAtomic *pAtomic)
{
	NTL_ASSERTE(pAtomic);

	return NtlGetPluginData((RpAtomic*)pAtomic)->UserDat;
}

void RpNtlAtomicSetUserDatReal(RpAtomic *pAtomic, RwReal uiSize)
{
	NTL_ASSERTE(pAtomic);

	NtlGetPluginData(pAtomic)->_UserDatReal = uiSize;
}

RwReal RpNtlAtomicGetUserDatReal(RpAtomic *pAtomic)
{
	NTL_ASSERTE(pAtomic);

	return NtlGetPluginData((RpAtomic*)pAtomic)->_UserDatReal;
}

void RpNtlAtomicSetAlpha(RpAtomic* pAtomic, RwUInt8 alpha)
{
	NTL_ASSERTE(pAtomic);
	
	NtlGetPluginData(pAtomic)->alpha = alpha;
}

RwUInt8 RpNtlAtomicGetAlpha(RpAtomic* pAtomic)
{
	NTL_ASSERTE(pAtomic);

	return NtlGetPluginData( (RpAtomic*)pAtomic)->alpha;
}

RpAtomicCallBackRender RpNtlAtomicGetRenderCallBack(RpAtomic *pAtomic)
{
	NTL_FUNCTION("RpNtlAtomicGetRenderCallBack()");

    NTL_PRE( iPlugin_Offset );
	NTL_ASSERTE(pAtomic);
	
	NTL_RETURN(NtlGetPluginData(pAtomic)->atomicRenderCallBack);
}

void RpNtlAtomicSetRenderCallBack(RpAtomic *pAtomic, RpAtomicCallBackRender atomicCallBackRender)
{
	NTL_FUNCTION("RpNtlAtomicSetRenderCallBack()");

    NTL_PRE( iPlugin_Offset );
	NTL_ASSERTE(pAtomic);
	
	NtlGetPluginData(pAtomic)->atomicRenderCallBack = atomicCallBackRender;


	NTL_RETURNVOID();
}






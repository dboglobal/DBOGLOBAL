#include "precomp_ntlcore.h"
#include "NtlPackImage.h"
#include "NtlDebug.h"

RtFileOpenFunc pOldFileOpenFunc = NULL;
RtFileReadFunc pOldFileReadFunc = NULL;
RtFileCloseFunc pOldFileCloseFunc = NULL;

RtFileSystemError PackFileOpen( RtFileSystem  *fs, RtFile *file, const RwChar *filename, RwUInt32 flags)
{
	SNtlPackImage	*pPackImage = (SNtlPackImage *)filename;
	
	RtFileSystemError eFileSystemError = pOldFileOpenFunc(fs, file, pPackImage->strPackFileName.c_str(), flags);
	if(eFileSystemError != RTFS_ERROR_NOERROR)
		return eFileSystemError;

	fs->fsFileFunc.setPosition(file, (RwInt32)pPackImage->uiPackOffset, RTFILE_POS_CURRENT);
	
	return RTFS_ERROR_NOERROR;
}

RwUInt32 PackFileRead( RtFile *file, void *pBuffer, RwUInt32 nBytes)
{
	return pOldFileReadFunc(file, pBuffer, nBytes);
}

void PackFileClose(RtFile *file)
{
    pOldFileCloseFunc(file);
}

void RegisterNewCallBack()
{
	RtFileSystem *pCurFileSystem = RtFSManagerGetDefaultFileSystem();

	pOldFileOpenFunc	= pCurFileSystem->fsFileFunc.open;
	pOldFileReadFunc	= pCurFileSystem->fsFileFunc.read;
	pOldFileCloseFunc	= pCurFileSystem->fsFileFunc.close;

	pCurFileSystem->fsFileFunc.open		= PackFileOpen;
	pCurFileSystem->fsFileFunc.read		= PackFileRead;
	pCurFileSystem->fsFileFunc.close	= PackFileClose;
}

void RegisterOldCallBack()
{
	RtFileSystem *pCurFileSystem = RtFSManagerGetDefaultFileSystem();

	pCurFileSystem->fsFileFunc.open		= pOldFileOpenFunc;
	pCurFileSystem->fsFileFunc.read		= pOldFileReadFunc;
	pCurFileSystem->fsFileFunc.close	= pOldFileCloseFunc;
}

RwTexture *Ntl_D3D9DDSTextureRead(SNtlPackImage *pPackImage)
{
	RegisterNewCallBack();

	RwStream *stream = NULL;
	RwRaster *pRaster = NULL;
	RwTexture *pTexture = NULL;

	stream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, pPackImage);
	if (!stream)
		goto ImagePackTextureLoad_Exit;
		
	pRaster = RwD3D9RasterStreamReadDDS(stream);
	RwStreamClose(stream, NULL);

	if (pRaster == NULL)
		goto ImagePackTextureLoad_Exit;
		
    pTexture = RwTextureCreate(pRaster);
    if (!pTexture)
    {
		RwRasterDestroy(pRaster);
        NTL_ASSERTE(NULL != pTexture);
		goto ImagePackTextureLoad_Exit;
    }

    if (RwRasterGetFormat(pRaster) & rwRASTERFORMATMIPMAP)
    {
        RwTextureSetFilterMode(pTexture, rwFILTERLINEARMIPLINEAR);
    }
    else
    {
        RwTextureSetFilterMode(pTexture, rwFILTERLINEAR);
    }

	RwTextureSetName(pTexture, pPackImage->pName);

ImagePackTextureLoad_Exit:	
	RegisterOldCallBack();

	return pTexture;
}

RwTexture *Ntl_PNGTextureRead(SNtlPackImage *pPackImage)
{
	RegisterNewCallBack();
	
	RwStream *stream = NULL;
	RwImage *pImage = NULL;
	RwRaster *pRaster = NULL;
	RwTexture *pTexture = NULL;
	
	RwInt32 nFormat = 0;
	RwInt32 nRasterWidth = 0;
	RwInt32 nRasterHeight = 0;
	RwInt32 nRasterDepth = 0;
	RwInt32 nRasterFlags = 0;

	pImage = RtPNGImageRead((const char*)pPackImage);
	if (pImage == NULL)
	{
		DBO_WARNING_MESSAGE("Could not read texture: " << pPackImage->pName);
		goto ImagePackTextureLoad_Exit;
	}

	if (!RwImageFindRasterFormat(pImage, rwRASTERTYPETEXTURE, &nRasterWidth, &nRasterHeight, &nRasterDepth, &nRasterFlags))
	{
		DBO_WARNING_MESSAGE("Could not find raster format texture: " << pPackImage->pName);
		RwImageDestroy(pImage);
		goto ImagePackTextureLoad_Exit;
	}
    
	pRaster = RwRasterCreate(nRasterWidth, nRasterHeight,nRasterDepth, nRasterFlags);
	if(pRaster == NULL)
	{
		DBO_WARNING_MESSAGE("Could not create raster texture: " << pPackImage->pName);
		RwImageDestroy(pImage);
		goto ImagePackTextureLoad_Exit;
	}

	RwImageGammaCorrect(pImage);

    if (!RwRasterSetFromImage(pRaster, pImage))
    {
		DBO_WARNING_MESSAGE("Could not set raster from image texture: " << pPackImage->pName);
        RwRasterDestroy(pRaster);
        RwImageDestroy(pImage);
        goto ImagePackTextureLoad_Exit;
    }
	RwImageDestroy(pImage);

	pTexture = RwTextureCreate(pRaster);
    if (!pTexture)
    {
		DBO_WARNING_MESSAGE("Could not create texture: " << pPackImage->pName);
		RwRasterDestroy(pRaster);
        NTL_ASSERTE(NULL != pTexture);
        goto ImagePackTextureLoad_Exit;
    }

	RwTextureSetName(pTexture, pPackImage->pName);
	if (pPackImage->pMaskName)
    {
        RwTextureSetMaskName(pTexture, pPackImage->pMaskName);
    }
    else
    {
        RwTextureSetMaskName(pTexture, "");
    }

ImagePackTextureLoad_Exit:	
	RegisterOldCallBack();

	return pTexture;
}

RwTexture *Ntl_BMPTextureRead(SNtlPackImage *pPackImage)
{
	RegisterNewCallBack();

	RwStream *stream = NULL;
	RwImage *pImage = NULL;
	RwRaster *pRaster = NULL;
	RwTexture *pTexture = NULL;
	
	RwInt32 nFormat = 0;
	RwInt32 nRasterWidth = 0;
	RwInt32 nRasterHeight = 0;
	RwInt32 nRasterDepth = 0;
	RwInt32 nRasterFlags = 0;

	pImage = RtBMPImageRead((const RwChar *)pPackImage);
	if(pImage == NULL)
		goto ImagePackTextureLoad_Exit;
	
	if (!RwImageFindRasterFormat(pImage, rwRASTERTYPETEXTURE, &nRasterWidth, &nRasterHeight, &nRasterDepth, &nRasterFlags))
	{
		RwImageDestroy(pImage);
		goto ImagePackTextureLoad_Exit;
	}
    
	pRaster = RwRasterCreate(nRasterWidth, nRasterHeight,nRasterDepth, nRasterFlags);
	if(pRaster == NULL)
	{
		RwImageDestroy(pImage);
		goto ImagePackTextureLoad_Exit;
	}

	RwImageGammaCorrect(pImage);

    if (!RwRasterSetFromImage(pRaster, pImage))
    {
        RwRasterDestroy(pRaster);
        RwImageDestroy(pImage);
    	goto ImagePackTextureLoad_Exit;
    }
	RwImageDestroy(pImage);

	pTexture = RwTextureCreate(pRaster);
    if (!pTexture)
    {
		RwRasterDestroy(pRaster);
        NTL_ASSERTE(NULL != pTexture);
    	goto ImagePackTextureLoad_Exit;
    }

	RwTextureSetName(pTexture, pPackImage->pName);
	if (pPackImage->pMaskName)
    {
        RwTextureSetMaskName(pTexture, pPackImage->pMaskName);
    }
    else
    {
        RwTextureSetMaskName(pTexture, "");
    }

ImagePackTextureLoad_Exit:
	RegisterOldCallBack();

	return pTexture;
}


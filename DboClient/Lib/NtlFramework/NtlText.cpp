#include "precomp_ntlframework.h"
#include "NtlText.h"

RtCharset* RsCharsetPrint(RtCharset *charSet, const RwChar *string,
                          RwInt32 x, RwInt32 y, RsPrintPos pos)
{
    RtCharsetDesc charsetDesc;
    RwUInt32  width,   height;
    RwUInt32  strLen;
    RwRaster *camRas;

#if 0
    /* RsPrintPos is really unsigned. As rsPRINTPOSMIDDLE == 0, this can never
       be false. */
    RSASSERT(pos >= rsPRINTPOSMIDDLE);
#endif
    RSASSERT(pos <= rsPRINTPOSBOTTOMRIGHT);
    RSASSERT(!((pos & rsPRINTPOSLEFT) && (pos & rsPRINTPOSRIGHT )));
    RSASSERT(!((pos & rsPRINTPOSTOP ) && (pos & rsPRINTPOSBOTTOM)));

    RSASSERT(NULL != RWSRCGLOBAL(curCamera));
    camRas = RwCameraGetRaster((RwCamera *)RWSRCGLOBAL(curCamera));
    RSASSERT(NULL != camRas);

    width  = RwRasterGetWidth(camRas);
    height = RwRasterGetHeight(camRas);

    RtCharsetGetDesc(charSet, &charsetDesc);
    strLen = rwstrlen(string);

    x *= charsetDesc.width;
    y *= charsetDesc.height;

    /* Depending on where on-screen text is being displayed, we position
     * it inside a platform-dependent safety margin, which helps stop
     * text being lost of the side of TVs (i.e for consoles). x and y
     * are now just offsets from the position we calc, used to group
     * strings together. Yeah, it's not foolproof... but hey. You can
     * always use RtCharsetPrint() directly. */
    if (pos & rsPRINTPOSLEFT)
    {
        x += charsetDesc.width*rsPRINTMARGINLEFT;
    }
    else if (pos & rsPRINTPOSRIGHT)
    {
        x += width - charsetDesc.width*(strLen + rsPRINTMARGINRIGHT);
    }

    if ((rsPRINTPOSLEFT   == pos) ||
        (rsPRINTPOSRIGHT  == pos) ||
        (rsPRINTPOSMIDDLE == pos) )
    {
        /* Centre the string vertically */
        y += (height - charsetDesc.height) / 2;
    }

    if (pos & rsPRINTPOSTOP)
    {
        y += charsetDesc.height*rsPRINTMARGINTOP;
    }
    else if (pos & rsPRINTPOSBOTTOM)
    {
        y += height - charsetDesc.height*(1 + rsPRINTMARGINBOTTOM);
    }

    if ((rsPRINTPOSTOP    == pos) ||
        (rsPRINTPOSBOTTOM == pos) ||
        (rsPRINTPOSMIDDLE == pos) )
    {
        /* Centre the string horizontally */
        x += (width - charsetDesc.width*strLen) / 2;
    }

    return(RtCharsetPrint(charSet, string, x, y));
}

RtCharset* RsCharsetPrint(RtCharset *charSet, const RwChar *string,
						  RwInt32 x, RwInt32 y)
{
	RtCharsetDesc charsetDesc;
	RwUInt32  width,   height;
	RwUInt32  strLen;
	RwRaster *camRas;

	RSASSERT(NULL != RWSRCGLOBAL(curCamera));
	camRas = RwCameraGetRaster((RwCamera *)RWSRCGLOBAL(curCamera));
	RSASSERT(NULL != camRas);

	width  = RwRasterGetWidth(camRas);
	height = RwRasterGetHeight(camRas);

	RtCharsetGetDesc(charSet, &charsetDesc);
	strLen = rwstrlen(string);

	return(RtCharsetPrint(charSet, string, x, y));
}

//---------------------------------------------------------------------------
//Name		: RsPathnameCreate
//Desc		: file의 fullpathname을 만든다.
//Return	: Return fullpathname
//Parameter	: srcBuffer - filename
//---------------------------------------------------------------------------
RwChar* RsPathnameCreate(const RwChar *srcBuffer)
{
    RwChar *dstBuffer;
    RwChar *charToConvert;

    /* 
     * First duplicate the string 
     */
    dstBuffer = (RwChar *)RwMalloc(sizeof(RwChar) * (rwstrlen(srcBuffer) + 1),
                                   rwID_NAOBJECT);

    if( dstBuffer )
    {
        rwstrcpy(dstBuffer, srcBuffer);

        /* 
         * Convert a path for use on Windows. 
         * Convert all /s and :s into \s 
         */
        while( (charToConvert = rwstrchr(dstBuffer, '/')) )
        {
            *charToConvert = '\\';
        }
#if 0
        while( (charToConvert = rwstrchr(dstBuffer, ':')) )
        {
            *charToConvert = '\\';
        }
#endif
    }

    return dstBuffer;
}

void RsPathnameDestroy(RwChar * buffer)
{
    if( buffer )
    {
        RwFree(buffer);
    }

	return;
}

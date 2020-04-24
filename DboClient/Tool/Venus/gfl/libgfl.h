/*
 *                    Graphics File Library
 *
 *  For Windows & Un*x
 *
 *  GFL library Copyright (c) 1991-2002 Pierre-e Gougelet
 *  All rights reserved
 *
 *
 *  Commercial use is not authorized without agreement
 * 
 *  URL:     http://www.xnview.com
 *  E-Mail : webmaster@xnview.com
 */

#ifndef __GRAPHIC_FILE_LIBRARY_H__
#define __GRAPHIC_FILE_LIBRARY_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
	#ifdef __BORLANDC__
		#pragma option -a8  /* switch to 8-bytes alignment */
	#else
		#pragma pack (push, before_push)
		#pragma pack (8)
	#endif
#else
#endif

#ifdef WIN32
	#define GFLEXTERN /*__declspec(dllexport)*/
	#define GFLAPI __stdcall
#else
	#define GFLEXTERN
	#define GFLAPI
#endif

#define GFL_VERSION  "1.90"

#define GFL_FALSE    0
#define GFL_TRUE     1

typedef signed char    GFL_INT8; 
typedef unsigned char  GFL_UINT8; 
typedef signed short   GFL_INT16; 
typedef unsigned short GFL_UINT16; 
typedef signed long    GFL_INT32; 
typedef unsigned long  GFL_UINT32; 

typedef unsigned char  GFL_BOOL; 

/*
 *  ERROR
 */
#define GFL_NO_ERROR 0

#define GFL_ERROR_FILE_OPEN         1
#define GFL_ERROR_FILE_READ         2
#define GFL_ERROR_FILE_CREATE       3
#define GFL_ERROR_FILE_WRITE        4
#define GFL_ERROR_NO_MEMORY         5
#define GFL_ERROR_UNKNOWN_FORMAT    6
#define GFL_ERROR_BAD_BITMAP        7
#define GFL_ERROR_BAD_FORMAT_INDEX  10
#define GFL_ERROR_BAD_PARAMETERS    50

#define GFL_UNKNOWN_ERROR           255

typedef GFL_INT16 GFL_ERROR; 

/*
 *  ORIGIN type
 */
#define GFL_LEFT            0x00
#define GFL_RIGHT           0x01
#define GFL_TOP             0x00
#define GFL_BOTTOM          0x10
#define GFL_TOP_LEFT        (GFL_TOP | GFL_LEFT)
#define GFL_BOTTOM_LEFT     (GFL_BOTTOM | GFL_LEFT)
#define GFL_TOP_RIGHT       (GFL_TOP | GFL_RIGHT)
#define GFL_BOTTOM_RIGHT    (GFL_BOTTOM | GFL_RIGHT)

typedef GFL_UINT16 GFL_ORIGIN; 

/*
 *  COMPRESSION type
 */
#define GFL_NO_COMPRESSION        0
#define GFL_RLE                   1
#define GFL_LZW                   2
#define GFL_JPEG                  3
#define GFL_ZIP                   4
#define GFL_SGI_RLE               5
#define GFL_CCITT_RLE             6
#define GFL_CCITT_FAX3            7
#define GFL_CCITT_FAX3_2D         8
#define GFL_CCITT_FAX4            9
#define GFL_WAVELET               10
#define GFL_LZW_PREDICTOR         11
#define GFL_UNKNOWN_COMPRESSION   255

typedef GFL_UINT16 GFL_COMPRESSION; 

/*
 *  BITMAP type
 */
#define GFL_BINARY   0x0001
#define GFL_GREY     0x0002
#define GFL_COLORS   0x0004
#define GFL_RGB      0x0010
#define GFL_RGBA     0x0020
#define GFL_BGR      0x0040
#define GFL_ABGR     0x0080
#define GFL_BGRA     0x0100
#define GFL_ARGB     0x0200
#define GFL_CMYK     0x0400

#define GFL_24BITS           (GFL_RGB | GFL_BGR)
#define GFL_32BITS           (GFL_RGBA | GFL_ABGR | GFL_BGRA | GFL_ARGB | GFL_CMYK)
#define GFL_TRUECOLORS       (GFL_24BITS | GFL_32BITS)

#define GFL_IS24BITS(_a)     ((_a) & GFL_24BITS)
#define GFL_IS32BITS(_a)     ((_a) & GFL_32BITS)
#define GFL_ISTRUECOLORS(_a) ((_a) & GFL_TRUECOLORS)

typedef GFL_UINT16 GFL_BITMAP_TYPE; 

/*
 *  BITMAP struct
 */
typedef struct {
		GFL_UINT8 Red[256]; 
		GFL_UINT8 Green[256]; 
		GFL_UINT8 Blue[256]; 
	} GFL_COLORMAP; 

typedef struct {
		GFL_UINT8 Red, Green, Blue, Alpha; 
	} GFL_COLOR; 

/*
 *  BITMAP struct
 */
typedef struct {
		GFL_BITMAP_TYPE  Type; 
		GFL_ORIGIN       Origin; 
		GFL_INT32        Width; 
		GFL_INT32        Height; 
		GFL_UINT32       BytesPerLine; 
		GFL_INT16        LinePadding; 
		GFL_INT16        Reserved; 
		GFL_UINT8        BytesPerPixel; 
		GFL_UINT8        BitsPerComponent;  /* Always 8 */
		GFL_UINT16       Xdpi; 
		GFL_UINT16       Ydpi; 
		GFL_INT16        TransparentIndex;  /* -1 if not used */
		GFL_INT32        ColorUsed; 
		GFL_COLORMAP    *ColorMap; 
		GFL_UINT8       *Data; 
	} GFL_BITMAP; 

/*
 *    Channels Order
 */
#define GFL_CORDER_INTERLEAVED 0
#define GFL_CORDER_SEQUENTIAL  1
#define GFL_CORDER_SEPARATE    2

typedef GFL_UINT16 GFL_CORDER; 

/*
 *    Channels Type
 */
#define GFL_CTYPE_GREYSCALE    0
#define GFL_CTYPE_RGB          1
#define GFL_CTYPE_BGR          2
#define GFL_CTYPE_RGBA         3
#define GFL_CTYPE_ABGR         4
#define GFL_CTYPE_CMY          5
#define GFL_CTYPE_CMYK         6

typedef GFL_UINT16 GFL_CTYPE; 

/*
 *  Callbacks
 */
typedef void *(GFLAPI *GFL_ALLOC_CALLBACK)(GFL_UINT32 size, void *param); 
typedef void (GFLAPI *GFL_FREE_CALLBACK)(void *buffer, void *param); 

typedef void * GFL_HANDLE; 

typedef GFL_UINT32 (GFLAPI *GFL_READ_CALLBACK)(GFL_HANDLE handle, void *buffer, GFL_UINT32 size); 
typedef GFL_UINT32 (GFLAPI *GFL_TELL_CALLBACK)(GFL_HANDLE handle); 
typedef GFL_UINT32 (GFLAPI *GFL_SEEK_CALLBACK)(GFL_HANDLE handle, GFL_INT32 offset, GFL_INT32 origin); 
typedef GFL_UINT32 (GFLAPI *GFL_WRITE_CALLBACK)(GFL_HANDLE handle, void *buffer, GFL_UINT32 size); 

/*
 *  LOAD_PARAMS Flags
 */
#define GFL_LOAD_SKIP_ALPHA					      0x0001 /* If the bitmap is 32 bits, then alpha is not loaded */
#define GFL_LOAD_IGNORE_READ_ERROR	      0x0002
#define GFL_LOAD_BY_EXTENSION_ONLY        0x0004 /* Use only extension to recognize format. Faster     */
#define GFL_LOAD_READ_ALL_COMMENT         0x0008 /* Read Comment in GFL_FILE_DESCRIPTION               */
#define GFL_LOAD_FORCE_COLOR_MODEL        0x0010 /* Force to load picture in the ColorModel            */
#define GFL_LOAD_PREVIEW_NO_CANVAS_RESIZE 0x0020 /* With gflLoadPreview, width & height are the maximum box */
#define GFL_LOAD_BINARY_AS_GREY           0x0040 /* Load Black&White file in greyscale                 */
#define GFL_LOAD_ORIGINAL_COLORMODEL      0x0080 /* If the colormodel is CMYK, keep it                 */
#define GFL_LOAD_ONLY_FIRST_FRAME         0x0100 /* No search to check if file is multi-frame          */

/*
 *  LOAD_PARAMS struct
 */
typedef struct {
		GFL_UINT32       Flags; 
		GFL_INT32        FormatIndex; /* -1 for automatic recognition */
    GFL_INT32        ImageWanted; /* for multi-page or animated file */
		GFL_ORIGIN       Origin;      /* default: GFL_TOP_LEFT   */
		GFL_BITMAP_TYPE  ColorModel;  /* Only for 24/32 bits picture, GFL_RGB/GFL_RGBA (default), GFL_BGR/GFL_ABGR, GFL_BGRA, GFL_ARGB */
		GFL_UINT32       LinePadding; /* 1 (default), 2, 4, .... */
		
		GFL_UINT8        DefaultAlpha; /* Used if alpha doesn't exist in original file & ColorModel=RGBA/BGRA/ABGR/ARGB */
		GFL_UINT8        Reserved1; 
		GFL_UINT16       Reserved2; 

		/*
		 * RAW/YUV only
		 */
		GFL_INT32        Width; 
		GFL_INT32        Height; 
		GFL_UINT32       Offset; 

		/*
		 * RAW only
		 */
		GFL_CORDER       ChannelOrder; 
		GFL_CTYPE        ChannelType; 
		
		/*
		 * PCD only
		 */
		GFL_UINT16       PcdBase; /* PCD -> 2:768x576, 1:384x288, 0:192x144 */

		/*
		 * EPS/PS/AI/PDF only
		 */
		GFL_UINT16       EpsDpi; 
		GFL_INT32        EpsWidth; 
		GFL_INT32        EpsHeight; 
		
		struct {
				GFL_READ_CALLBACK  Read; 
				GFL_TELL_CALLBACK  Tell; 
				GFL_SEEK_CALLBACK  Seek; 

				GFL_ALLOC_CALLBACK Alloc; /* Not yet implemented */
				GFL_FREE_CALLBACK  Free;  /* Not yet implemented */
				void * AllocParam; 
			} Callbacks; 

	} GFL_LOAD_PARAMS; 

/*
 *  SAVE_PARAMS Flags
 */
#define GFL_SAVE_REPLACE_EXTENSION 0x0001
#define GFL_SAVE_WANT_FILENAME     0x0002

/*
 *  SAVE_PARAMS struct
 */
typedef struct {
		GFL_UINT32       Flags; 
		GFL_INT32        FormatIndex; 

		GFL_COMPRESSION  Compression; 
		GFL_INT16        Quality;           /* Jpeg/Wic/Fpx  */
		GFL_INT16        CompressionLevel;  /* Png           */
		GFL_BOOL         Interlaced;        /* Gif           */
		GFL_BOOL         Progressive;       /* Jpeg          */

		/*
		 * For RAW/YUV
		 */
		GFL_UINT32       Offset; 
		GFL_CORDER       ChannelOrder; 
		GFL_CTYPE        ChannelType; 

		struct {
				GFL_WRITE_CALLBACK Write; /* Not yet implemented */
				GFL_TELL_CALLBACK  Tell;  /* Not yet implemented */
				GFL_SEEK_CALLBACK  Seek;  /* Not yet implemented */

				GFL_ALLOC_CALLBACK Alloc; /* Not yet implemented */
				GFL_FREE_CALLBACK  Free;  /* Not yet implemented */
				void * AllocParam; 
			} Callbacks; 

	} GFL_SAVE_PARAMS; 

/*
 *  Color model
 */
#define GFL_CM_RGB                0
#define GFL_CM_GREY               1
#define GFL_CM_CMY                2
#define GFL_CM_CMYK               3
#define GFL_CM_YCBCR              4
#define GFL_CM_YUV16              5
#define GFL_CM_LAB                6
#define GFL_CM_LOGLUV             7
#define GFL_CM_LOGL               8

typedef GFL_UINT16 GFL_COLORMODEL; 

/*
 *  FILE_INFORMATION struct
 */
typedef struct {
		GFL_BITMAP_TYPE  Type; 
		GFL_ORIGIN       Origin; 
		GFL_INT32        Width; 
		GFL_INT32        Height; 
		GFL_INT32        FormatIndex; 
		char             FormatName[8]; 
		char             Description[64]; 
		GFL_UINT16       Xdpi; 
		GFL_UINT16       Ydpi; 
		GFL_UINT16       BitsPerPlane; 
		GFL_UINT16       NumberOfPlanes; 
		GFL_UINT32       BytesPerPlane; 
		GFL_INT32        NumberOfImages; 
		GFL_UINT32       FileSize; 
		GFL_COLORMODEL   ColorModel; 
		GFL_COMPRESSION  Compression; 
		char             CompressionDescription[64]; 
		GFL_UINT16       NumberOfComment; 
		GFL_UINT16       Reserved2; 
		char          ** Comment; 
	} GFL_FILE_INFORMATION; 

#define GFL_READ	0x01
#define GFL_WRITE	0x02

/*
 *  FORMAT_INFORMATION struct
 */
typedef struct {
		GFL_INT32        Index; 
		char             Name[8]; 
		char             Description[64]; 
    GFL_UINT32       Status; 
		GFL_UINT32       NumberOfExtension; 
		char             Extension[16][8]; 
	} GFL_FORMAT_INFORMATION; 

/*
 *  Functions
 */

extern GFLEXTERN void * GFLAPI gflMemoryAlloc( GFL_UINT32 size ); 

extern GFLEXTERN void * GFLAPI gflMemoryRealloc( void *ptr, GFL_UINT32 size ); 

extern GFLEXTERN void GFLAPI gflMemoryFree( void *ptr ); 

/*
 * ~~
 */

extern GFLEXTERN const char * GFLAPI gflGetVersion( void ); 

extern GFLEXTERN const char * GFLAPI gflGetVersionOfLibformat( void ); 

/*
 * ~~
 */

extern GFLEXTERN GFL_ERROR GFLAPI gflLibraryInit( void ); 

extern GFLEXTERN void GFLAPI gflLibraryExit( void ); 

extern GFLEXTERN void GFLAPI gflEnableLZW( GFL_BOOL ); /* If you have the Unisys license */

extern GFLEXTERN void GFLAPI gflSetPluginsPathname( const char * ); 

/*
 * ~~
 */

extern GFLEXTERN GFL_INT32 GFLAPI gflGetNumberOfFormat( void ); 

extern GFLEXTERN GFL_INT32 GFLAPI gflGetFormatIndexByName( const char *name ); 

extern GFLEXTERN const char * GFLAPI gflGetFormatNameByIndex( GFL_INT32 index ); 

extern GFLEXTERN GFL_BOOL GFLAPI gflFormatIsSupported( const char *name ); 

extern GFLEXTERN GFL_BOOL GFLAPI gflFormatIsWritableByIndex( GFL_INT32 index ); 

extern GFLEXTERN GFL_BOOL GFLAPI gflFormatIsWritableByName( const char *name ); 

extern GFLEXTERN GFL_BOOL GFLAPI gflFormatIsReadableByIndex( GFL_INT32 index ); 

extern GFLEXTERN GFL_BOOL GFLAPI gflFormatIsReadableByName( const char *name ); 

extern GFLEXTERN const char * GFLAPI gflGetDefaultFormatSuffixByIndex( GFL_INT32 index ); 

extern GFLEXTERN const char * GFLAPI gflGetDefaultFormatSuffixByName( const char *name ); 

extern GFLEXTERN const char * GFLAPI gflGetFormatDescriptionByIndex( GFL_INT32 index ); 

extern GFLEXTERN const char * GFLAPI gflGetFormatDescriptionByName( const char *name ); 

extern GFLEXTERN GFL_ERROR GFLAPI gflGetFormatInformationByIndex( GFL_INT32 index, GFL_FORMAT_INFORMATION *info ); 

extern GFLEXTERN GFL_ERROR GFLAPI gflGetFormatInformationByName( const char *name, GFL_FORMAT_INFORMATION *info ); 

/*
 * ~~
 */

extern GFLEXTERN const char * GFLAPI gflGetErrorString( GFL_ERROR error ); 

extern GFLEXTERN const char * GFLAPI gflGetLabelForColorModel( GFL_COLORMODEL color_model ); 

extern GFLEXTERN GFL_ERROR GFLAPI gflGetFileInformation( const char *filename, GFL_INT32 index, GFL_FILE_INFORMATION *info ); 

extern GFLEXTERN void GFLAPI gflFreeFileInformation( GFL_FILE_INFORMATION *info ); 

extern GFLEXTERN void GFLAPI gflGetDefaultLoadParams( GFL_LOAD_PARAMS *params ); 

extern GFLEXTERN GFL_ERROR GFLAPI gflLoadBitmap( const char *filename, GFL_BITMAP **bitmap, GFL_LOAD_PARAMS *params, GFL_FILE_INFORMATION *info ); 

extern GFLEXTERN GFL_ERROR GFLAPI gflLoadBitmapFromHandle( GFL_HANDLE handle, GFL_BITMAP **bitmap, GFL_LOAD_PARAMS *params, GFL_FILE_INFORMATION *info ); 

extern GFLEXTERN void GFLAPI gflGetDefaultPreviewParams( GFL_LOAD_PARAMS *params ); 

extern GFLEXTERN GFL_ERROR GFLAPI gflLoadPreview( const char *filename, GFL_INT32 width, GFL_INT32 height, GFL_BITMAP **bitmap, GFL_LOAD_PARAMS *params, GFL_FILE_INFORMATION *info ); 

extern GFLEXTERN GFL_ERROR GFLAPI gflLoadPreviewFromHandle( GFL_HANDLE handle, GFL_INT32 width, GFL_INT32 height, GFL_BITMAP **bitmap, GFL_LOAD_PARAMS *params, GFL_FILE_INFORMATION *info ); 

extern GFLEXTERN void GFLAPI gflGetDefaultSaveParams( GFL_SAVE_PARAMS *params ); 

extern GFLEXTERN GFL_ERROR GFLAPI gflSaveBitmap( char *filename, const GFL_BITMAP *bitmap, GFL_SAVE_PARAMS *params ); 

extern GFLEXTERN GFL_ERROR GFLAPI gflSaveBitmapIntoHandle( GFL_HANDLE handle, const GFL_BITMAP *bitmap, GFL_SAVE_PARAMS *params ); 

/*
 * ~~
 */

typedef void *GFL_FILE_HANDLE; 

extern GFLEXTERN GFL_ERROR GFLAPI gflFileCreate         ( GFL_FILE_HANDLE *handle, const char *filename, GFL_UINT32 image_count, GFL_SAVE_PARAMS *params ); 

extern GFLEXTERN GFL_ERROR GFLAPI gflFileAddPicture     ( GFL_FILE_HANDLE handle, const GFL_BITMAP *bitmap ); 

extern GFLEXTERN void GFLAPI gflFileClose               ( GFL_FILE_HANDLE handle ); 

/*
 * ~~
 */

extern GFLEXTERN GFL_BITMAP * GFLAPI gflAllockBitmap( GFL_BITMAP_TYPE type, GFL_INT32 width, GFL_INT32 height, GFL_UINT32 line_padding, const GFL_COLOR * color ); 

extern GFLEXTERN void GFLAPI gflFreeBitmap( GFL_BITMAP *bitmap ); 

extern GFLEXTERN void GFLAPI gflFreeBitmapData( GFL_BITMAP *bitmap ); /* bitmap is not freed */

extern GFLEXTERN GFL_BITMAP * GFLAPI gflCloneBitmap( const GFL_BITMAP *bitmap ); 

/*
 * ~~
 */

#define GFL_RESIZE_QUICK     0
#define GFL_RESIZE_BILINEAR  1

extern GFLEXTERN GFL_ERROR GFLAPI gflResize( GFL_BITMAP *src, GFL_BITMAP **dst, GFL_INT32 width, GFL_INT32 height, GFL_UINT32 method, GFL_UINT32 flags ); 

#define GFL_MODE_TO_BINARY         1
#define GFL_MODE_TO_4GREY          2
#define GFL_MODE_TO_8GREY          3
#define GFL_MODE_TO_16GREY         4
#define GFL_MODE_TO_32GREY         5
#define GFL_MODE_TO_64GREY         6
#define GFL_MODE_TO_128GREY        7
#define GFL_MODE_TO_216GREY        8
#define GFL_MODE_TO_256GREY        9
#define GFL_MODE_TO_8COLORS        12
#define GFL_MODE_TO_16COLORS       13
#define GFL_MODE_TO_32COLORS       14
#define GFL_MODE_TO_64COLORS       15
#define GFL_MODE_TO_128COLORS      16
#define GFL_MODE_TO_216COLORS      17
#define GFL_MODE_TO_256COLORS      18
#define GFL_MODE_TO_RGB            19
#define GFL_MODE_TO_RGBA           20
#define GFL_MODE_TO_BGR            21
#define GFL_MODE_TO_ABGR           22
#define GFL_MODE_TO_BGRA           23
#define GFL_MODE_TO_ARGB           24

#define GFL_MODE_TO_TRUE_COLORS    GFL_MODE_TO_RGB

typedef GFL_UINT16 GFL_MODE; 

#define GFL_MODE_NO_DITHER         0
#define GFL_MODE_PATTERN_DITHER    1
#define GFL_MODE_HALTONE45_DITHER  2  /* Only with GFL_MODE_TO_BINARY */
#define GFL_MODE_HALTONE90_DITHER  3  /* Only with GFL_MODE_TO_BINARY */
#define GFL_MODE_ADAPTIVE          4
#define GFL_MODE_FLOYD_STEINBERG   5  /* Only with GFL_MODE_TO_BINARY */

typedef GFL_UINT16 GFL_MODE_PARAMS; 

extern GFLEXTERN GFL_ERROR GFLAPI gflChangeColorDepth( GFL_BITMAP *src, GFL_BITMAP **dst, GFL_MODE mode, GFL_MODE_PARAMS params ); 

/*
 * ~~
 */

#define gflGetBitmapPtr( _bitmap, _y ) \
	((_bitmap)->Data + (_y) * (_bitmap)->BytesPerLine)

extern GFLEXTERN GFL_ERROR GFLAPI gflGetColorAt( const GFL_BITMAP *src, GFL_INT32 x, GFL_INT32 y, GFL_COLOR *color ); 
extern GFLEXTERN GFL_ERROR GFLAPI gflSetColorAt( GFL_BITMAP *src, GFL_INT32 x, GFL_INT32 y, const GFL_COLOR *color ); 

/*
 * ~~
 */

typedef struct {
	GFL_INT32 x, y, w, h; 
} GFL_RECT; 

typedef struct {
		GFL_INT32 x, y; 
	} GFL_POINT; 

extern GFLEXTERN GFL_ERROR GFLAPI gflFlipVertical    ( GFL_BITMAP *src, GFL_BITMAP **dst ); 
extern GFLEXTERN GFL_ERROR GFLAPI gflFlipHorizontal  ( GFL_BITMAP *src, GFL_BITMAP **dst ); 
extern GFLEXTERN GFL_ERROR GFLAPI gflCrop            ( GFL_BITMAP *src, GFL_BITMAP **dst, const GFL_RECT *rect ); 
extern GFLEXTERN GFL_ERROR GFLAPI gflAutoCrop        ( GFL_BITMAP *src, GFL_BITMAP **dst, const GFL_COLOR *color, GFL_INT32 tolerance ); 

/*
 * ~~
 */

#define GFL_CANVASRESIZE_CENTER      0
#define GFL_CANVASRESIZE_TOPLEFT     1
#define GFL_CANVASRESIZE_TOPRIGHT    2
#define GFL_CANVASRESIZE_BOTTOMLEFT  3
#define GFL_CANVASRESIZE_BOTTOMRIGHT 4
#define GFL_CANVASRESIZE_TOP         5
#define GFL_CANVASRESIZE_BOTTOM      6
#define GFL_CANVASRESIZE_LEFT        7
#define GFL_CANVASRESIZE_RIGHT       8

typedef GFL_UINT32 GFL_CANVASRESIZE; 

extern GFLEXTERN GFL_ERROR GFLAPI gflResizeCanvas    ( GFL_BITMAP *src, GFL_BITMAP **dst, GFL_INT32 width, GFL_INT32 height, GFL_CANVASRESIZE mode, const GFL_COLOR *color ); 

/*
 * ~~
 */

extern GFLEXTERN GFL_ERROR GFLAPI gflRotate          ( GFL_BITMAP *src, GFL_BITMAP **dst, GFL_INT32 angle, const GFL_COLOR *background_color ); 
extern GFLEXTERN GFL_ERROR GFLAPI gflRotateFine      ( GFL_BITMAP *src, GFL_BITMAP **dst, double angle, const GFL_COLOR *background_color ); 

extern GFLEXTERN GFL_ERROR GFLAPI gflReplaceColor    ( GFL_BITMAP *src, GFL_BITMAP **dst, const GFL_COLOR *color, const GFL_COLOR *new_color, GFL_INT32 tolerance ); 

extern GFLEXTERN GFL_ERROR GFLAPI gflBitblt          ( const GFL_BITMAP *src, const GFL_RECT *rect, GFL_BITMAP *dst, GFL_INT32 x_dest, GFL_INT32 y_dest ); 
extern GFLEXTERN GFL_ERROR GFLAPI gflMerge           ( const GFL_BITMAP *src[], const GFL_POINT origin[], const GFL_UINT32 opacity[], GFL_INT32 num_bitmap, GFL_BITMAP **dst ); 

#ifdef WIN32
	#ifdef __BORLANDC__
		#pragma option -a. 
	#else
		#pragma pack (pop, before_push)
	#endif
#else
#endif

#ifdef __cplusplus
}
#endif

#endif

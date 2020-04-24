#ifndef _DIB_H
#define _DIB_H

#include <string.h>

// CDIB Section ---------------------------------------------------------------------------------
typedef enum
{
    DIB_LOAD_OK,                // file read OK
    DIB_LOAD_NOFILE_OR_CORRUPT, // file not found or corrupted
    DIB_LOAD_NO_MEM             // Out of mem
} DIBLoadResult;

typedef enum
{
    DIB_SAVE_OK,                // file saved ok
    DIB_SAVE_CANT_WRITE,        // Cannot write to file
} DIBSaveResult;

class CDIB
{
public:
    
	COLORREF *m_Bits;
    BITMAPINFO m_Info;
    HBITMAP m_Bitmap;
    SIZE m_Size;

    CDIB ();
    virtual ~CDIB ();

    void WritePixel (UINT x,UINT y,COLORREF c);
	void WriteInversePixel (UINT x,UINT y,COLORREF c);

    BOOL LoadBMP ( CDC* pDC, LPCTSTR Filename );
    DIBSaveResult SaveBMP (  LPSTR Filename );

	DIBLoadResult LoadPPM ( LPSTR Filename );
    DIBSaveResult SavePPM ( LPSTR Filename , UINT uDescID = 0);

    BOOL Create (int Width, int Height );
    BOOL Create (CDC *pDC, UINT uBitmapID);			 // From resource bitmap
	BOOL Create (CDC *pDC, LPCTSTR szBitmapFilename);  // From file	 bitmap
    void Destroy ();

    
	void PasteToDC ( CDC *pDC, int x, int y );
	void PasteToDC ( CDC *pDC, int x, int y, int xsize, int ysize );
    void GetFromDC ( CDC *pDC, int x, int y, int w, int h );
    void GetFromBitmap ( CDC *pDC, CBitmap *pBitmap );
    
	void Fill ( WORD Color );
	void Fill ( COLORREF Color );
    void Fill ( int R, int G, int B );
    void FillGlass ( int R, int G, int B, int A );
    void FillStippledGlass ( int R, int G, int B );

    void CopyTo   ( CDIB *Dib );	
    void CopyFrom ( const CDIB *Dib );
	
	void CopyToClipboard( void );

    void CopyTransparentlyFrom ( CDIB *Dib, int R, int G, int B );
	void CopyTransparentlyFrom ( CDIB *Dib, COLORREF Color );

    void Add ( CDIB *Dib );
    void Blend ( CDIB *Dib, int A );
    void Darken ( CDIB *Dib );
    void Difference ( CDIB *Dib );
    void Lighten ( CDIB *Dib );
    void Multiply ( CDIB *Dib );
    void Screen ( CDIB *Dib );

    void CopyRect ( CDIB *Dib, int x, int y );
    void PasteRect ( CDIB *Dib, int x, int y );

    void PasteCKRect ( CDIB *Dib, int x, int y, COLORREF ColorKey );

    void FillRect ( int x, int y, int w, int h, int R, int G, int B );
	void FillInverseRect ( int x, int y, int w, int h, COLORREF Color );
    void FillGlassRect ( int x, int y, int w, int h, int R, int G, int B, int A );
    void FillStippledGlassRect ( int x, int y, int w, int h, int R, int G, int B );
    
    void AddRect ( CDIB *Dib, int x, int y );
    void BlendRect ( CDIB *Dib, int x, int y, int A );
    void DarkenRect ( CDIB *Dib, int x, int y );
    void DifferenceRect ( CDIB *Dib, int x, int y );
    void LightenRect ( CDIB *Dib, int x, int y );
    void MultiplyRect ( CDIB *Dib, int x, int y );
    void ScreenRect ( CDIB *Dib, int x, int y );

    void Line ( int x1, int y1, int x2, int y2, int R, int G, int B );
    void LineGlass ( int x1, int y1, int x2, int y2, int R, int G, int B, int A );

	void Circle( int xx, int yy, int radius, COLORREF Color );

	void HLine( int x,int y,int Size,COLORREF Color);
	void VLine( int x,int y,int Size,COLORREF Color);

    void ClippedLine ( int x1, int y1, int x2, int y2, int R, int G, int B );

	BOOL GaussianBlur( void );
	BOOL NearestNeighborInterpolation( void );

    COLORREF &ColorAt (int x, int y);
	BYTE      AlphaLevelAt ( int x, int y );
    
	BOOL operator == (const CDIB &) const;	
    BOOL operator != (const CDIB &) const;
    CDIB &operator = (const CDIB &);

private:

};

#endif

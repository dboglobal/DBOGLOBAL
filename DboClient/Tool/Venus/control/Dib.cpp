#include "stdafx.h"

#include "DIB.h"

#include <math.h>
#include <stdio.h>

#define PPM_EXT             ".PPM"

#define RGB_GETRED(rgb)     (((rgb) >> 16) & 0xff) 
#define RGB_GETGREEN(rgb)   (((rgb) >> 8) & 0xff)
#define RGB_GETBLUE(rgb)    ((rgb) & 0xff) 

void CDIB::CopyToClipboard( void )
{
/*	if( !m_Bitmap ) return;

	OpenClipboard( hwnd );	
	EmptyClipboard();
	SetClipboardData ( CF_DIB, m_Bitmap );
	CloseClipboard ();
	*/
}


BOOL CDIB::Create ( int Width, int Height )
{
    Destroy ();

    ZeroMemory ( &m_Info, sizeof (BITMAPINFO) );

    m_Info.bmiHeader.biSize=sizeof (BITMAPINFOHEADER);
    m_Info.bmiHeader.biWidth=Width;
    m_Info.bmiHeader.biHeight=Height;
    m_Info.bmiHeader.biPlanes=1;
    m_Info.bmiHeader.biBitCount=32; 
    m_Info.bmiHeader.biCompression=BI_RGB;
    m_Info.bmiHeader.biSizeImage=Width*Height*4;
    
    m_Bitmap=CreateDIBSection ( NULL, &m_Info, DIB_RGB_COLORS, (void **)&m_Bits, NULL, NULL );

    if ( m_Bitmap )
    {
        m_Size.cx = Width;
		m_Size.cy = Height;

		ZeroMemory( this->m_Bits, m_Info.bmiHeader.biSizeImage );

        return TRUE;
    }
    else
    {        
		m_Size.cx = 0;
		m_Size.cy = 0;
        return FALSE;
    }
}

BOOL CDIB::Create (CDC *pDC, UINT uBitmapID)
{   // Creates from a bitmap

	if( !pDC ) return FALSE;

    CDC BufferDC;
    CBitmap BufferBitmap, *OldBitmap;

    if (!BufferBitmap.LoadBitmap (uBitmapID))
        return FALSE;   // Can't load resource bitmap
    BITMAP bmpData;
    if (!BufferBitmap.GetBitmap (&bmpData))
        return FALSE;   // Can't get bitmap info
    if (!Create (bmpData.bmWidth, bmpData.bmHeight))
        return FALSE;   // Can't create DIB buffer

    BufferDC.CreateCompatibleDC (pDC);
    OldBitmap=BufferDC.SelectObject (&BufferBitmap);
    GetDIBits (BufferDC.m_hDC, HBITMAP(BufferBitmap), 0, m_Size.cy, m_Bits, &(m_Info), DIB_RGB_COLORS); 
    BufferDC.SelectObject (OldBitmap);
    BufferBitmap.DeleteObject();
    return TRUE;
}

BOOL CDIB::Create (CDC *pDC, LPCTSTR szBitmapFilename)
{
	if( !pDC ) return FALSE;

    CDC BufferDC;
    CBitmap BufferBitmap;

	HBITMAP	hbm;
	hbm = (HBITMAP)::LoadImage(GetModuleHandle(NULL), szBitmapFilename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
	if (hbm == NULL)
	{
		hbm = (HBITMAP)::LoadImage(NULL, szBitmapFilename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	}
    if (hbm == NULL)
	{
		return FALSE;
	}
		
    //if (!BufferBitmap.LoadBitmap (szBitmapFilename))
    //    return FALSE;   // Can't load resource bitmap

    BITMAP bmpData;    	
    ::GetObject(hbm, sizeof(BITMAP), &bmpData);

    if (!Create (bmpData.bmWidth, bmpData.bmHeight))
        return FALSE;   // Can't create DIB buffer

    BufferDC.CreateCompatibleDC (pDC);
    	
	HBITMAP oldbitmap;
	oldbitmap = (HBITMAP)::SelectObject( BufferDC.GetSafeHdc(), hbm );

    GetDIBits (BufferDC.m_hDC, hbm, 0, m_Size.cy, m_Bits, &(m_Info), DIB_RGB_COLORS);     	
	SelectObject (BufferDC.GetSafeHdc(), oldbitmap );

    BufferBitmap.DeleteObject();
    return TRUE;
}

void CDIB::Destroy ()
{
    if ( m_Bitmap )
        DeleteObject ( m_Bitmap );    
	m_Bitmap=NULL;
	m_Size.cx = 0;
	m_Size.cy = 0;    
}


void CDIB::GetFromDC ( CDC *pDC, int x, int y, int w, int h )
{
	if( !pDC ) return;

    // If DibSize Wrong Re-Create Dib
    if ( (m_Size.cx!=w) || (m_Size.cy!=h) )
        Create ( w, h );
    
    CDC BufferDC;
    CBitmap BufferBitmap, *OldBitmap;

    BufferDC.CreateCompatibleDC ( pDC );
    BufferBitmap.CreateCompatibleBitmap ( pDC, w, h );
    OldBitmap=BufferDC.SelectObject ( &BufferBitmap );
    BufferDC.FillSolidRect ( 0, 0, w, h, 0 );
    BufferDC.BitBlt ( 0, 0, w, h, pDC, x, y, SRCCOPY );
    BufferDC.SelectObject ( OldBitmap );
    GetDIBits ( pDC->m_hDC, HBITMAP(BufferBitmap), 0, h, m_Bits, &(m_Info), DIB_RGB_COLORS ); 
}

BOOL CDIB::LoadBMP ( CDC* pDC, LPCTSTR Filename )
{    
	ASSERT( pDC );
	ASSERT( Filename );
	
	if( Create( pDC, Filename ) )
	{
		return TRUE;
	}
	else
	{
		CString strmessage;
		strmessage.Format( "ERROR : Load bitmap resource [%s]", Filename );
		AfxMessageBox( strmessage );
		return FALSE;
	}
}

void CDIB::Fill ( WORD Color )
{
	//memset2bytes( m_Bits, Color, m_Size.cx*m_Size.cy );
}

void CDIB::Fill ( COLORREF Color )
{
	int Size=m_Size.cx*m_Size.cy;

    for ( int i=0; i<Size; i++ )
        m_Bits[i]=Color;
}


void CDIB::Fill ( int R, int G, int B )
{
    COLORREF Color=RGB ( B, G, R );
    int Size=m_Size.cx*m_Size.cy;

    for ( int i=0; i<Size; i++ )
        m_Bits[i]=Color;
}

void CDIB::FillGlass ( int R, int G, int B, int A )
{
    BYTE *dst=(BYTE*)m_Bits;
    int Size=m_Size.cx*m_Size.cy;
        
    while ( Size-- )
    {
        dst[0]=(BYTE)(((B-dst[0])*A+(dst[0]<<8))>>8);
        dst[1]=(BYTE)(((G-dst[1])*A+(dst[1]<<8))>>8);
        dst[2]=(BYTE)(((R-dst[2])*A+(dst[2]<<8))>>8);   
        dst+=4;
    }
}

void CDIB::FillStippledGlass ( int R, int G, int B )
{   
    COLORREF Color=RGB ( B, G, R );
    int w=m_Size.cx;
    int h=m_Size.cy;

    for ( int j=0; j<w; j++ )
    {
        for ( int i=0; i<h; i++ )
        {
            m_Bits[j*w+i]=((i+j)&0x1) ? m_Bits[j*w+i] : Color;
        }
    }
}

void CDIB::CopyTransparentlyFrom ( CDIB *Dib, COLORREF Color )
{
	 if ( m_Size.cx!=Dib->m_Size.cx || m_Size.cy!=Dib->m_Size.cy ) return;

     COLORREF *src=Dib->m_Bits;
     COLORREF *dst=m_Bits;     
     int Size=m_Size.cx*m_Size.cy;
     	 
     while ( Size-- )
     {
        //dst[0]=(src[0]==Color) ? dst[0] : src[0];
		 if( src[0] != Color ) dst[0] = src[0];
        dst++;
        src++;
     }
}

void CDIB::CopyTransparentlyFrom ( CDIB *Dib, int R, int G, int B )
{
    if ( m_Size.cx!=Dib->m_Size.cx || m_Size.cy!=Dib->m_Size.cy )
        return;

    COLORREF *src=Dib->m_Bits;
    COLORREF *dst=m_Bits;
    COLORREF ColorKey=RGB ( B, G, R );
    int Size=m_Size.cx*m_Size.cy;
        
    while ( Size-- )
    {
        dst[0]=(src[0]==ColorKey) ? dst[0] : src[0];    
        dst++;
        src++;
    }
}

void CDIB::Add ( CDIB *Dib )
{
    if ( m_Size.cx!=Dib->m_Size.cx || m_Size.cy!=Dib->m_Size.cy )	
        return;

    BYTE *src=(BYTE*)Dib->m_Bits;
    BYTE *dst=(BYTE*)m_Bits;
    int Size=m_Size.cx*m_Size.cy;

    int Sum;
        
    while ( Size-- )
    {
        Sum=src[0]+dst[0];
        dst[0]=(BYTE)((Sum>255) ? 255 : Sum);
        Sum=src[1]+dst[1];
        dst[1]=(BYTE)((Sum>255) ? 255 : Sum);
        Sum=src[2]+dst[2];
        dst[2]=(BYTE)((Sum>255) ? 255 : Sum);   
        dst+=4;
        src+=4;
    }
}

void CDIB::Blend ( CDIB *Dib, int A )
{
	if ( m_Size.cx!=Dib->m_Size.cx || m_Size.cy!=Dib->m_Size.cy )    
        return;

    BYTE *src=(BYTE*)Dib->m_Bits;
    BYTE *dst=(BYTE*)m_Bits;
    int Size=m_Size.cx*m_Size.cy;
        
    while ( Size-- )
    {
        dst[0]=(BYTE)(((src[0]-dst[0])*A+(dst[0]<<8))>>8);
        dst[1]=(BYTE)(((src[1]-dst[1])*A+(dst[1]<<8))>>8);
        dst[2]=(BYTE)(((src[2]-dst[2])*A+(dst[2]<<8))>>8);  
        dst+=4;
        src+=4;
    }
}

void CDIB::Darken ( CDIB *Dib )
{
	if ( m_Size.cx!=Dib->m_Size.cx || m_Size.cy!=Dib->m_Size.cy )    
        return;

    BYTE *src=(BYTE*)Dib->m_Bits;
    BYTE *dst=(BYTE*)m_Bits;
    int Size=m_Size.cx*m_Size.cy;
        
    while ( Size-- )
    {
        dst[0]=(BYTE)((src[0]<dst[0]) ? src[0] : dst[0]);
        dst[1]=(BYTE)((src[1]<dst[1]) ? src[1] : dst[1]);
        dst[2]=(BYTE)((src[2]<dst[2]) ? src[2] : dst[2]);   
        dst+=4;
        src+=4;
    }
}

void CDIB::Difference ( CDIB *Dib )
{
	if ( m_Size.cx!=Dib->m_Size.cx || m_Size.cy!=Dib->m_Size.cy )    
        return;

    BYTE *src=(BYTE*)Dib->m_Bits;
    BYTE *dst=(BYTE*)m_Bits;
    int Size=m_Size.cx*m_Size.cy;

    int Difference;

    while ( Size-- )
    {
        Difference=src[0]-dst[0];
        dst[0]=(BYTE)((Difference<0) ? -Difference : Difference);
        Difference=src[1]-dst[1];
        dst[1]=(BYTE)((Difference<0) ? -Difference : Difference);
        Difference=src[2]-dst[2];
        dst[2]=(BYTE)((Difference<0) ? -Difference : Difference);   
        dst+=4;
        src+=4;
    }
}

void CDIB::Lighten ( CDIB *Dib )
{
	if ( m_Size.cx!=Dib->m_Size.cx || m_Size.cy!=Dib->m_Size.cy )    
        return;

    BYTE *src=(BYTE*)Dib->m_Bits;
    BYTE *dst=(BYTE*)m_Bits;
    int Size=m_Size.cx*m_Size.cy;
        
    while ( Size-- )
    {
        dst[0]=(BYTE)((src[0]>dst[0]) ? src[0] : dst[0]);
        dst[1]=(BYTE)((src[1]>dst[1]) ? src[1] : dst[1]);
        dst[2]=(BYTE)((src[2]>dst[2]) ? src[2] : dst[2]);   
        dst+=4;
        src+=4;
    }
}

void CDIB::Multiply ( CDIB *Dib )
{
	if ( m_Size.cx!=Dib->m_Size.cx || m_Size.cy!=Dib->m_Size.cy )    
        return;

    BYTE *src=(BYTE*)Dib->m_Bits;
    BYTE *dst=(BYTE*)m_Bits;
    int Size=m_Size.cx*m_Size.cy;
        
    while ( Size-- )
    {
        dst[0]=(BYTE)(((src[0])*(dst[0]))>>8);
        dst[1]=(BYTE)(((src[1])*(dst[1]))>>8);
        dst[2]=(BYTE)(((src[2])*(dst[2]))>>8);  
        dst+=4;
        src+=4;
    }
}

void CDIB::Screen ( CDIB *Dib )
{
	if ( m_Size.cx!=Dib->m_Size.cx || m_Size.cy!=Dib->m_Size.cy )    
        return;

    BYTE *src=(BYTE*)Dib->m_Bits;
    BYTE *dst=(BYTE*)m_Bits;
    int Size=m_Size.cx*m_Size.cy;
        
    while ( Size-- )
    {
        dst[0]=(BYTE)(255-(((255-src[0])*(255-dst[0]))>>8));
        dst[1]=(BYTE)(255-(((255-src[1])*(255-dst[1]))>>8));
        dst[2]=(BYTE)(255-(((255-src[2])*(255-dst[2]))>>8));    
        dst+=4;
        src+=4;
    }
}

//////////////////////////////////////////////////////////////////////
// Rectangle Functions
//////////////////////////////////////////////////////////////////////

void CDIB::CopyRect ( CDIB *Dib, int x, int y )
{
    // Clip Rect
    int px=(x>=0) ? x : 0;
    int py=(y>=0) ? y : 0;
    int dx=((x+Dib->m_Size.cx)<m_Size.cx) ? Dib->m_Size.cx : m_Size.cx-x;
    int dy=((y+Dib->m_Size.cy)<m_Size.cy) ? Dib->m_Size.cy : m_Size.cy-y;
    dx=(x>=0) ? dx : dx + x;
    dy=(y>=0) ? dy : dy + y;

    // If Nothing to Copy return
    if ( (dx<=0) || (dy<=0) )
        return;
    // If DibSize Wrong Re-Create Dib
    if ( (dx!=Dib->m_Size.cx) || (dy!=Dib->m_Size.cy) )
        Dib->Create ( dx, dy );

    // Prepare Buffer Addresses
    COLORREF *src=m_Bits+(py*m_Size.cx)+px;
    COLORREF *dst=Dib->m_Bits;

    // Do Copy
    while ( dy-- )
    {
        for ( int i=0; i<dx; i++ )
            dst[i]=src[i];
        src+=m_Size.cx;
        dst+=Dib->m_Size.cx;
    }
}

void CDIB::PasteRect ( CDIB *Dib, int x, int y )
{
    // Clip Rect
    int px=(x>=0) ? x : 0;
    int py=(y>=0) ? y : 0;
    int dx=((x+Dib->m_Size.cx)<m_Size.cx) ? Dib->m_Size.cx : m_Size.cx-x;
    int dy=((y+Dib->m_Size.cy)<m_Size.cy) ? Dib->m_Size.cy : m_Size.cy-y;
    dx=(x>=0) ? dx : dx + x;
    dy=(y>=0) ? dy : dy + y;

    // If Nothing to Paste return
    if ( (dx<=0) || (dy<=0) )
        return;

    // Prepare Buffer Addresses
    COLORREF *src=Dib->m_Bits+((py-y)*Dib->m_Size.cx)+px-x;
    COLORREF *dst=m_Bits+(py*m_Size.cx)+px;

    // Do Paste
    while ( dy-- )
    {
        for ( int i=0; i<dx; i++ )
            dst[i]=src[i];
        src+=Dib->m_Size.cx;
        dst+=m_Size.cx;
    }
}

void CDIB::PasteCKRect ( CDIB *Dib, int x, int y, COLORREF ColorKey )
{
    // Clip Rect
    int px=(x>=0) ? x : 0;
    int py=(y>=0) ? y : 0;
    int dx=((x+Dib->m_Size.cx)<m_Size.cx) ? Dib->m_Size.cx : m_Size.cx-x;
    int dy=((y+Dib->m_Size.cy)<m_Size.cy) ? Dib->m_Size.cy : m_Size.cy-y;
    dx=(x>=0) ? dx : dx + x;
    dy=(y>=0) ? dy : dy + y;

    // If Nothing to Paste return
    if ( (dx<=0) || (dy<=0) )
        return;

    // Prepare Buffer Addresses
    COLORREF *src=Dib->m_Bits+((py-y)*Dib->m_Size.cx)+px-x;
    COLORREF *dst=m_Bits+(py*m_Size.cx)+px;
    //COLORREF ColorKey=RGB ( B, G, R );

    // Do Paste
    while ( dy-- )
    {
        for ( int i=0; i<dx; i++ )
            dst[i]=(src[i]==ColorKey) ? dst[i] : src[i];
        src+=Dib->m_Size.cx;
        dst+=m_Size.cx;
    }
}

void CDIB::FillRect ( int x, int y, int w, int h, int R, int G, int B )
{
    // Clip Rect
    int px=(x>=0) ? x : 0;
    int py=(y>=0) ? y : 0;
    int dx=((x+w)<m_Size.cx) ? w : m_Size.cx-x;
    int dy=((y+h)<m_Size.cy) ? h : m_Size.cy-y;
    dx=(x>=0) ? dx : dx + x;
    dy=(y>=0) ? dy : dy + y;

    // If Nothing to Fill return
    if ( (dx<=0) || (dy<=0) )
        return;

    // Prepare Buffer Address
    COLORREF *dst=m_Bits+(py*m_Size.cx)+px;
    COLORREF Color=RGB ( B, G, R );

    // Do Fill
    while ( dy-- )
    {
        for ( int i=0; i<dx; i++ )
        {
            dst[i]=Color;   
        }
        dst+=m_Size.cx;
    }
}

void CDIB::FillInverseRect ( int x, int y, int w, int h, COLORREF Color )
{
	y = this->m_Size.cy - y - h;
	// Clip Rect
    int px=(x>=0) ? x : 0;
    int py=(y>=0) ? y : 0;
    int dx=((x+w)<m_Size.cx) ? w : m_Size.cx-x;
    int dy=((y+h)<m_Size.cy) ? h : m_Size.cy-y;
    dx=(x>=0) ? dx : dx + x;
    dy=(y>=0) ? dy : dy + y;

    // If Nothing to Fill return
    if ( (dx<=0) || (dy<=0) )
        return;

    // Prepare Buffer Address
    COLORREF *dst=m_Bits+(py*m_Size.cx)+px;
    
    // Do Fill
    while ( dy-- )
    {
        for ( int i=0; i<dx; i++ )
        {
            dst[i]=Color;   
        }
        dst+=m_Size.cx;
    }
}

void CDIB::FillGlassRect ( int x, int y, int w, int h, int R, int G, int B, int A )
{
    // Clip Rect
    int px=(x>=0) ? x : 0;
    int py=(y>=0) ? y : 0;
    int dx=((x+w)<m_Size.cx) ? w : m_Size.cx-x;
    int dy=((y+h)<m_Size.cy) ? h : m_Size.cy-y;
    dx=(x>=0) ? dx : dx + x;
    dy=(y>=0) ? dy : dy + y;

    // If Nothing to FillGlass return
    if ( (dx<=0) || (dy<=0) )
        return;

    // Prepare Buffer Address
    BYTE *dst=(BYTE *)m_Bits+((py*m_Size.cx)+px)*4;

    // Do FillGlass
    while ( dy-- )
    {
        for ( int i=0; i<dx; i++ )
        {
            dst[0]=(BYTE)(((B-dst[0])*A+(dst[0]<<8))>>8);
            dst[1]=(BYTE)(((G-dst[1])*A+(dst[1]<<8))>>8);
            dst[2]=(BYTE)(((R-dst[2])*A+(dst[2]<<8))>>8);   
            dst+=4;
        }
        dst+=(m_Size.cx-dx)<<2;
    }
}

void CDIB::FillStippledGlassRect ( int x, int y, int w, int h, int R, int G, int B )
{
    // Clip Rect
    int px=(x>=0) ? x : 0;
    int py=(y>=0) ? y : 0;
    int dx=((x+w)<m_Size.cx) ? w : m_Size.cx-x;
    int dy=((y+h)<m_Size.cy) ? h : m_Size.cy-y;
    dx=(x>=0) ? dx : dx + x;
    dy=(y>=0) ? dy : dy + y;

    // If Nothing to FillStippledGlass return
    if ( (dx<=0) || (dy<=0) )
        return;

    // Prepare Buffer Address
    COLORREF *dst=m_Bits+(py*m_Size.cx)+px;
    COLORREF Color=RGB ( B, G, R );

    // Do FillStippledGlass
    for ( int j=0; j<dy; j++ )
    {
        for ( int i=0; i<dx; i++ )
        {
            dst[i]=((i+j)&0x1) ? dst[i] : Color;    
        }
        dst+=m_Size.cx;
    }
}

void CDIB::AddRect ( CDIB *Dib, int x, int y )
{
    // Clip Rect
    int px=(x>=0) ? x : 0;
    int py=(y>=0) ? y : 0;
    int dx=((x+Dib->m_Size.cx)<m_Size.cx) ? Dib->m_Size.cx : m_Size.cx-x;
    int dy=((y+Dib->m_Size.cy)<m_Size.cy) ? Dib->m_Size.cy : m_Size.cy-y;
    dx=(x>=0) ? dx : dx + x;
    dy=(y>=0) ? dy : dy + y;

    // If Nothing to Add return
    if ( (dx<=0) || (dy<=0) )
        return;

    // Prepare Buffer Addresses
    BYTE *src=(BYTE *)Dib->m_Bits+(((py-y)*Dib->m_Size.cx)+px-x)*4;
    BYTE *dst=(BYTE *)m_Bits+((py*m_Size.cx)+px)*4;

    int Sum;

    // Do Add
    while ( dy-- )
    {
        for ( int i=0; i<dx; i++ )
        {
            Sum=src[0]+dst[0];
            dst[0]=(BYTE)((Sum>255) ? 255 : Sum);
            Sum=src[1]+dst[1];
            dst[1]=(BYTE)((Sum>255) ? 255 : Sum);
            Sum=src[2]+dst[2];
            dst[2]=(BYTE)((Sum>255) ? 255 : Sum);   
            dst+=4;
            src+=4;
        }
        dst+=(m_Size.cx-dx)<<2;
        src+=(Dib->m_Size.cx-dx)<<2;
    }
}

void CDIB::BlendRect ( CDIB *Dib, int x, int y, int A )
{
    // Clip Rect
    int px=(x>=0) ? x : 0;
    int py=(y>=0) ? y : 0;
    int dx=((x+Dib->m_Size.cx)<m_Size.cx) ? Dib->m_Size.cx : m_Size.cx-x;
    int dy=((y+Dib->m_Size.cy)<m_Size.cy) ? Dib->m_Size.cy : m_Size.cy-y;
    dx=(x>=0) ? dx : dx + x;
    dy=(y>=0) ? dy : dy + y;

    // If Nothing to Blend return
    if ( (dx<=0) || (dy<=0) )
        return;

    // Prepare Buffer Addresses
    BYTE *src=(BYTE *)Dib->m_Bits+(((py-y)*Dib->m_Size.cx)+px-x)*4;
    BYTE *dst=(BYTE *)m_Bits+((py*m_Size.cx)+px)*4;

    // Do Blend
    while ( dy-- )
    {
        for ( int i=0; i<dx; i++ )
        {
            dst[0]=(BYTE)(((src[0]-dst[0])*A+(dst[0]<<8))>>8);
            dst[1]=(BYTE)(((src[1]-dst[1])*A+(dst[1]<<8))>>8);
            dst[2]=(BYTE)(((src[2]-dst[2])*A+(dst[2]<<8))>>8);  
            dst+=4;
            src+=4;
        }
        dst+=(m_Size.cx-dx)<<2;
        src+=(Dib->m_Size.cx-dx)<<2;
    }
}

void CDIB::DarkenRect ( CDIB *Dib, int x, int y )
{
    // Clip Rect
    int px=(x>=0) ? x : 0;
    int py=(y>=0) ? y : 0;
    int dx=((x+Dib->m_Size.cx)<m_Size.cx) ? Dib->m_Size.cx : m_Size.cx-x;
    int dy=((y+Dib->m_Size.cy)<m_Size.cy) ? Dib->m_Size.cy : m_Size.cy-y;
    dx=(x>=0) ? dx : dx + x;
    dy=(y>=0) ? dy : dy + y;

    // If Nothing to Darken return
    if ( (dx<=0) || (dy<=0) )
        return;

    // Prepare Buffer Addresses
    BYTE *src=(BYTE *)Dib->m_Bits+(((py-y)*Dib->m_Size.cx)+px-x)*4;
    BYTE *dst=(BYTE *)m_Bits+((py*m_Size.cx)+px)*4;

    // Do Darken
    while ( dy-- )
    {
        for ( int i=0; i<dx; i++ )
        {
            dst[0]=(BYTE)((src[0]<dst[0]) ? src[0] : dst[0]);
            dst[1]=(BYTE)((src[1]<dst[1]) ? src[1] : dst[1]);
            dst[2]=(BYTE)((src[2]<dst[2]) ? src[2] : dst[2]);   
            dst+=4;
            src+=4;
        }
        dst+=(m_Size.cx-dx)<<2;
        src+=(Dib->m_Size.cx-dx)<<2;
    }
}

void CDIB::DifferenceRect ( CDIB *Dib, int x, int y )
{
    // Clip Rect
    int px=(x>=0) ? x : 0;
    int py=(y>=0) ? y : 0;
    int dx=((x+Dib->m_Size.cx)<m_Size.cx) ? Dib->m_Size.cx : m_Size.cx-x;
    int dy=((y+Dib->m_Size.cy)<m_Size.cy) ? Dib->m_Size.cy : m_Size.cy-y;
    dx=(x>=0) ? dx : dx + x;
    dy=(y>=0) ? dy : dy + y;

    // If Nothing to Difference return
    if ( (dx<=0) || (dy<=0) )
        return;

    // Prepare Buffer Addresses
    BYTE *src=(BYTE *)Dib->m_Bits+(((py-y)*Dib->m_Size.cx)+px-x)*4;
    BYTE *dst=(BYTE *)m_Bits+((py*m_Size.cx)+px)*4;

    // Do Difference
    while ( dy-- )
    {
        for ( int i=0; i<dx; i++ )
        {
            int Difference;
            Difference=src[0]-dst[0];
            dst[0]=(BYTE)((Difference<0) ? -Difference : Difference);
            Difference=src[1]-dst[1];
            dst[1]=(BYTE)((Difference<0) ? -Difference : Difference);
            Difference=src[2]-dst[2];
            dst[2]=(BYTE)((Difference<0) ? -Difference : Difference);   
            dst+=4;
            src+=4;
        }
        dst+=(m_Size.cx-dx)<<2;
        src+=(Dib->m_Size.cx-dx)<<2;
    }
}

void CDIB::LightenRect ( CDIB *Dib, int x, int y )
{
    // Clip Rect
    int px=(x>=0) ? x : 0;
    int py=(y>=0) ? y : 0;
    int dx=((x+Dib->m_Size.cx)<m_Size.cx) ? Dib->m_Size.cx : m_Size.cx-x;
    int dy=((y+Dib->m_Size.cy)<m_Size.cy) ? Dib->m_Size.cy : m_Size.cy-y;
    dx=(x>=0) ? dx : dx + x;
    dy=(y>=0) ? dy : dy + y;

    // If Nothing to Lighten return
    if ( (dx<=0) || (dy<=0) )
        return;

    // Prepare Buffer Addresses
    BYTE *src=(BYTE *)Dib->m_Bits+(((py-y)*Dib->m_Size.cx)+px-x)*4;
    BYTE *dst=(BYTE *)m_Bits+((py*m_Size.cx)+px)*4;

    // Do Lighten
    while ( dy-- )
    {
        for ( int i=0; i<dx; i++ )
        {
            dst[0]=(BYTE)((src[0]>dst[0]) ? src[0] : dst[0]);
            dst[1]=(BYTE)((src[1]>dst[1]) ? src[1] : dst[1]);
            dst[2]=(BYTE)((src[2]>dst[2]) ? src[2] : dst[2]);
            dst+=4;
            src+=4;
        }
        dst+=(m_Size.cx-dx)<<2;
        src+=(Dib->m_Size.cx-dx)<<2;
    }
}

void CDIB::MultiplyRect ( CDIB *Dib, int x, int y )
{
    // Clip Rect
    int px=(x>=0) ? x : 0;
    int py=(y>=0) ? y : 0;
    int dx=((x+Dib->m_Size.cx)<m_Size.cx) ? Dib->m_Size.cx : m_Size.cx-x;
    int dy=((y+Dib->m_Size.cy)<m_Size.cy) ? Dib->m_Size.cy : m_Size.cy-y;
    dx=(x>=0) ? dx : dx + x;
    dy=(y>=0) ? dy : dy + y;

    // If Nothing to Multiply return
    if ( (dx<=0) || (dy<=0) )
        return;

    // Prepare Buffer Addresses
    BYTE *src=(BYTE *)Dib->m_Bits+(((py-y)*Dib->m_Size.cx)+px-x)*4;
    BYTE *dst=(BYTE *)m_Bits+((py*m_Size.cx)+px)*4;

    // Do Multiply
    while ( dy-- )
    {
        for ( int i=0; i<dx; i++ )
        {
            dst[0]=(BYTE)(((src[0])*(dst[0]))>>8);
            dst[1]=(BYTE)(((src[1])*(dst[1]))>>8);
            dst[2]=(BYTE)(((src[2])*(dst[2]))>>8);
            dst+=4;
            src+=4;
        }
        dst+=(m_Size.cx-dx)<<2;
        src+=(Dib->m_Size.cx-dx)<<2;
    }
}

void CDIB::ScreenRect ( CDIB *Dib, int x, int y )
{
    // Clip Rect
    int px=(x>=0) ? x : 0;
    int py=(y>=0) ? y : 0;
    int dx=((x+Dib->m_Size.cx)<m_Size.cx) ? Dib->m_Size.cx : m_Size.cx-x;
    int dy=((y+Dib->m_Size.cy)<m_Size.cy) ? Dib->m_Size.cy : m_Size.cy-y;
    dx=(x>=0) ? dx : dx + x;
    dy=(y>=0) ? dy : dy + y;

    // If Nothing to Screen return
    if ( (dx<=0) || (dy<=0) )
        return;

    // Prepare Buffer Addresses
    BYTE *src=(BYTE *)Dib->m_Bits+(((py-y)*Dib->m_Size.cx)+px-x)*4;
    BYTE *dst=(BYTE *)m_Bits+((py*m_Size.cx)+px)*4;

    // Do Screen
    while ( dy-- )
    {
        for ( int i=0; i<dx; i++ )
        {
            dst[0]=(BYTE)(255-(((255-src[0])*(255-dst[0]))>>8));
            dst[1]=(BYTE)(255-(((255-src[1])*(255-dst[1]))>>8));
            dst[2]=(BYTE)(255-(((255-src[2])*(255-dst[2]))>>8));
            dst+=4;
            src+=4;
        }
        dst+=(m_Size.cx-dx)<<2;
        src+=(Dib->m_Size.cx-dx)<<2;
    }
}

//////////////////////////////////////////////////////////////////////
// Line Functions
//////////////////////////////////////////////////////////////////////

#define CLIPCODE(x,y) (((x<0)?8:0)|((x>=m_Size.cx)?4:0)|((y<0)?2:0)|((y>=m_Size.cy)?1:0))

void CDIB::ClippedLine ( int x1, int y1, int x2, int y2, int R, int G, int B )
{
    int c1=CLIPCODE(x1,y1), 
        c2=CLIPCODE(x2,y2);
    
    int dx, dy;
    while ( (c1|c2)!=0 )
    {
        if ( (c1&c2)!=0 )
            return;
        dx=x2-x1;
        dy=y2-y1;
        if ( c1!=0 )
        {  
            if ( (c1&8)==8)
            {
                y1+=(0-x1)*dy/dx;
                x1=0;
            }  
            else if ( (c1&4)==4)
            {
                y1+=(m_Size.cx-1-x1)*dy/dx;
                x1=m_Size.cx-1;
            }  
            else if ((c1&2) == 2)
            {
                x1+=(0-y1)*dx/dy;
                y1=0;
            }  
            else
            if ( (c1&1)==1)
            {
                x1+=(m_Size.cy-1-y1)*dx/dy;
                y1=m_Size.cy-1;
            }  
            c1=CLIPCODE(x1,y1);
         }  
         else if ( c2!=0 )
         {
            if ( (c2&8)==8 )
            {
                y2+=(0-x2)*dy/dx;
                x2=0;
            }  
            else if ( (c2&4)==4 )
            {
                y2+=(m_Size.cx-1-x2)*dy/dx;
                x2=m_Size.cx-1;
            }  
            else if ( (c2&2)==2 )
            {
                x2+=(0-y2)*dx/dy;
                y2=0;
            }  
            else if ( (c2&1)==1 )
            {
                x2+=(m_Size.cy-1-y2)*dx/dy;
                y2=m_Size.cy-1;
            }  
            c2=CLIPCODE(x2,y2);
        }  
    }
    
    int d, x, y, ax, ay, sx, sy;
    COLORREF Color=RGB ( B, G, R );
    
    dx=x2-x1;
    ax=abs ( dx )<<1;
    sx=(dx<0) ? -1 : 1;
    dy=y2-y1;
    ay=abs ( dy )<<1;
    sy=(dy<0) ? -1 : 1;
    x=x1;
    y=y1;
    
    if ( ax>ay )
    {
        d=ay-(ax>>1);
        while ( x!=x2 )
        {
            m_Bits[y*m_Size.cx+x]=Color;
            if ( d>=0 )
            {
                y+=sy;
                d-=ax;
            }
            x+=sx;
            d+=ay;
        }
    }
    else
    {
        d=ax-(ay>>1);
        while ( y!=y2 )
        {
            m_Bits[y*m_Size.cx+x]=Color;
            if ( d>=0 )
            {
                x+=sx;
                d-=ay;
            }
            y+=sy;
            d+=ax;
        }
    }
}

void CDIB::Line ( int x1, int y1, int x2, int y2, int R, int G, int B )
{
    int d, x, y, ax, ay, sx, sy, dx, dy;
    COLORREF Color=RGB ( B, G, R );
    
    dx=x2-x1;
    ax=abs ( dx )<<1;
    sx=(dx<0) ? -1 : 1;
    dy=y2-y1;
    ay=abs ( dy )<<1;
    sy=(dy<0) ? -1 : 1;
    x=x1;
    y=y1;
    
    if ( ax>ay )
    {
        d=ay-(ax>>1);
        while ( x!=x2 )
        {
            m_Bits[y*m_Size.cx+x]=Color;
            if ( d>=0 )
            {
                y+=sy;
                d-=ax;
            }
            x+=sx;
            d+=ay;
        }
    }
    else
    {
        d=ax-(ay>>1);
        while ( y!=y2 )
        {
            m_Bits[y*m_Size.cx+x]=Color;
            if ( d>=0 )
            {
                x+=sx;
                d-=ay;
            }
            y+=sy;
            d+=ax;
        }
    }
}

void CDIB::LineGlass ( int x1, int y1, int x2, int y2, int R, int G, int B, int A )
{
    int d, x, y, ax, ay, sx, sy, dx, dy;
    BYTE *dst=(BYTE *)m_Bits;
    
    dx=x2-x1;
    ax=abs ( dx )<<1;
    sx=(dx<0) ? -1 : 1;
    dy=y2-y1;
    ay=abs ( dy )<<1;
    sy=(dy<0) ? -1 : 1;
    x=x1;
    y=y1;
    
    if ( ax>ay )
    {
        d=ay-(ax>>1);
        while ( x!=x2 )
        {
            dst[(y*m_Size.cx+x)<<2]=(BYTE)(((B-dst[(y*m_Size.cx+x)<<2])*A+(dst[(y*m_Size.cx+x)<<2]<<8))>>8);
            dst[((y*m_Size.cx+x)<<2)+1]=(BYTE)(((G-dst[((y*m_Size.cx+x)<<2)+1])*A+(dst[((y*m_Size.cx+x)<<2)+1]<<8))>>8);
            dst[((y*m_Size.cx+x)<<2)+2]=(BYTE)(((R-dst[((y*m_Size.cx+x)<<2)+2])*A+(dst[((y*m_Size.cx+x)<<2)+2]<<8))>>8);
            if ( d>=0 )
            {
                y+=sy;
                d-=ax;
            }
            x+=sx;
            d+=ay;
        }
    }
    else
    {
        d=ax-(ay>>1);
        while ( y!=y2 )
        {
            dst[(y*m_Size.cx+x)<<2]=(BYTE)(((B-dst[(y*m_Size.cx+x)<<2])*A+(dst[(y*m_Size.cx+x)<<2]<<8))>>8);
            dst[((y*m_Size.cx+x)<<2)+1]=(BYTE)(((G-dst[((y*m_Size.cx+x)<<2)+1])*A+(dst[((y*m_Size.cx+x)<<2)+1]<<8))>>8);
            dst[((y*m_Size.cx+x)<<2)+2]=(BYTE)(((R-dst[((y*m_Size.cx+x)<<2)+2])*A+(dst[((y*m_Size.cx+x)<<2)+2]<<8))>>8);
            if ( d>=0 )
            {
                x+=sx;
                d-=ay;
            }
            y+=sy;
            d+=ax;
        }
    }
}

void CDIB::Circle( int xx, int yy, int radius, COLORREF Color )
{
	int   x = 0;
	int   y = radius;
	int   d = 3 - y * 2;
	int   _CSX = 0, _CEX = m_Size.cx, _CSY = 0, _CEY = m_Size.cy;
	    
	while (x <= y) {
	
		if( xx+x>=_CSX && xx+x<_CEX ) 
		{		
			if( yy+y>=_CSY && yy+y<_CEY ) 
				m_Bits[ (yy+y) * m_Size.cx + xx+x ] = Color;
			if( yy-y>=_CSY && yy-y<_CEY ) 
				m_Bits[ (yy-y) * m_Size.cx + xx+x ] = Color;
		}
		if( xx+y>=_CSX && xx+y<_CEX ) 
		{		
			if( yy-x>=_CSY && yy-x<_CEY ) 
				m_Bits[ (yy-x) * m_Size.cx + xx+y ] = Color;
			if( yy+x>=_CSY && yy+x<_CEY ) 
				m_Bits[ (yy+x) * m_Size.cx + xx+y ] = Color;			
		}
		if( xx-x>=_CSX && xx-x<_CEX ) 
		{		
			if( yy+y>=_CSY && yy+y<_CEY ) 
				m_Bits[ (yy+y) * m_Size.cx + xx-x ] = Color;		
			if( yy-y>=_CSY && yy-y<_CEY ) 
				m_Bits[ (yy-y) * m_Size.cx + xx-x ] = Color;
		}
		if( xx-y>=_CSX && xx-y<_CEX ) 
		{		
			if( yy+x>=_CSY && yy+x<_CEY ) 
				m_Bits[ (yy+x) * m_Size.cx + xx-y ] = Color;		
			if( yy-x>=_CSY && yy-x<_CEY ) 
				m_Bits[ (yy-x) * m_Size.cx + xx-y ] = Color;
		}		
		
		if (d < 0) d += ( (x<<2) + 6);
		else {
			d += (((x - y)<<2) + 10);
			y--;
		}
		x++;		
    }
	if (x == y && xx+x>_CSX && xx+x<_CEX && yy+y>_CSY && yy+y<_CEY ) 
		m_Bits[ (yy+y) * m_Size.cx + xx+x ] = Color;	

}


DIBSaveResult 
CDIB::SavePPM ( LPSTR Filename , UINT uDescID)
{
	FILE *fp;

    fp = fopen ( Filename, "wb");
    if (NULL == fp)
    {
        return DIB_SAVE_CANT_WRITE;
    }
    fprintf (fp,"P6\n");
    /*
	CString cstrDesc;
    if (uDescID && cstrDesc.LoadString(uDescID))
    {   // Description string ID specified and loaded successfully
        
    }
	*/
    fprintf (fp, "%d %d\n",m_Size.cx,m_Size.cy);
    fprintf (fp,"255\n");
    for (int y=m_Size.cy - 1; y>=0; y--)
        for (int x=0; x<m_Size.cx; x++)
        {
            COLORREF c = m_Bits[x + y * m_Size.cx];
            if (3 != fprintf (fp, "%c%c%c", GetBValue(c), GetGValue(c), GetRValue(c)))
            {
                fclose (fp);
                return DIB_SAVE_CANT_WRITE;
            }
        }
    fclose (fp);

    return DIB_SAVE_OK;
}

DIBSaveResult 
CDIB::SaveBMP ( LPSTR Filename )
{    
	BITMAPFILEHEADER    hdr;

    FILE* file;
    if( !(file = fopen( Filename, "wb") ) )
        return DIB_SAVE_CANT_WRITE;

	m_Info.bmiHeader.biBitCount=24;     
    m_Info.bmiHeader.biSizeImage=m_Info.bmiHeader.biWidth*m_Info.bmiHeader.biHeight*3;
    
	// Fill in the fields of the file header 
    hdr.bfType      = ((WORD) ('M' << 8) | 'B');    // is always "BM"
    hdr.bfSize      = m_Info.bmiHeader.biSize + m_Info.bmiHeader.biSizeImage + sizeof( hdr );
    hdr.bfReserved1 = 0;
    hdr.bfReserved2 = 0;
    hdr.bfOffBits   = (DWORD) (sizeof( hdr ) + m_Info.bmiHeader.biSize);

    // Write the file header 
    fwrite( &hdr, sizeof(hdr), 1, file );
    // Write BITMAPINFO header
    fwrite( &m_Info, m_Info.bmiHeader.biSize, 1, file );
    // Write the DIB header and the bits 
    LPBYTE Imgptr = (LPBYTE) m_Bits;

	for( int i=0; i<m_Info.bmiHeader.biHeight; i++ )
	{
		for( int j=0; i<m_Info.bmiHeader.biWidth; j++ )	 
		{
			fwrite( Imgptr, 3, 1, file );
			Imgptr+=4;
		}
	}

	m_Info.bmiHeader.biBitCount=32;     
    m_Info.bmiHeader.biSizeImage=m_Info.bmiHeader.biWidth*m_Info.bmiHeader.biHeight*4;

    return DIB_SAVE_OK;
}

DIBLoadResult 
CDIB::LoadPPM ( LPSTR Filename )
{
    FILE *fp = NULL;
    int i,j;
    DIBLoadResult res = DIB_LOAD_OK;
    BOOL bPPM3;    

    if ((fp = fopen ( Filename , "rb"))==NULL)
    {
        res = DIB_LOAD_NOFILE_OR_CORRUPT;
	    AfxMessageBox( "File open Faild !" );
        goto cleanup;
    }

    char tmp[1024];
    fgets (tmp,1024,fp); /* Get header */
    if (!strcmp (tmp,"P6\n"))   /* Test header */
        bPPM3 = FALSE;  /* PPM6 - binary */
    else if (!strcmp (tmp,"P3\n"))
        bPPM3 = TRUE;  /* PPM3 - ASCII */
    else
    {   /* Bad PPM header */
        res = DIB_LOAD_NOFILE_OR_CORRUPT;
		AfxMessageBox( "Header Faild !" );
        goto cleanup;
    }
    do    /* Read remarks (starting with '#' char) */
        fgets (tmp,1024,fp);
    while ('#' == tmp[0]); 

    int Width,Height;

    if (!strlen(tmp) || (2 != sscanf (tmp,"%d %d",&Width,&Height)))   /* Obtain width and height */
    {
        res = DIB_LOAD_NOFILE_OR_CORRUPT;
		AfxMessageBox( "Obtain width and height !" );
        goto cleanup;
    }
    fgets (tmp,1024,fp); 

    int MaxColor;

    if (!strlen(tmp) || (1 != sscanf (tmp,"%d",&MaxColor)))  /* Get max color used */
    {
        res = DIB_LOAD_NOFILE_OR_CORRUPT;
		AfxMessageBox( "Max color !" );
        goto cleanup;
    }
    if (!Create (Width, Height))
    {
        res = DIB_LOAD_NO_MEM;
		AfxMessageBox( "No Memory !" );
        goto cleanup;
    }
    for (i=Height - 1; i >= 0; i--)      /* Read each row (reverse) */
        for (j=0; j < Width; j++)   /* Read each column */
            if (!bPPM3)
            {   // PPM 6
                BYTE rgb[3];

                if (fread (rgb, 3, 1, fp) != 1)
                {   // Can't read pixel
                    res = DIB_LOAD_NOFILE_OR_CORRUPT;
					AfxMessageBox( "Can't read pixel" );
                    goto cleanup;
                }
                else
                {
                    ColorAt(j,i) = RGB(rgb[2],rgb[1],rgb[0]);
                }
            }
            else
            {   // PPM 3
                int r, g, b;

                if (3 != fscanf(fp, "%d %d %d", &b, &g, &r))
                {   // Can't read pixel
                    res = DIB_LOAD_NOFILE_OR_CORRUPT;
					AfxMessageBox( "Can't read pixel" );
                    goto cleanup;
                }
                else
                {
                    ColorAt(j,i) = RGB((BYTE)r, (BYTE)g, (BYTE)b);
                }
            }

cleanup:
    if (NULL != fp)
    {
        fclose (fp);
    }			

	return res;
}

void CDIB::WriteInversePixel(UINT x,UINT y,COLORREF c)
{
	 if( x >= 0 && y >= 0 && x < (UINT)m_Size.cx && y < (UINT)m_Size.cy )
	 {
		m_Bits[ x + ((m_Size.cy-y-1)* m_Size.cx) ] = c;		
	 }
}

void CDIB::WritePixel (UINT x,UINT y,COLORREF c)
{
    if( x >= 0 && y >= 0 && x < (UINT)m_Size.cx && y < (UINT)m_Size.cy )
		m_Bits[x + y * m_Size.cx] = c;
}

CDIB::CDIB ()
{
    m_Bits=NULL;
    m_Bitmap=NULL;
    m_Size.cx= 0;
	m_Size.cy= 0;
}

CDIB::~CDIB ()
{
    Destroy ();
}


void CDIB::PasteToDC ( CDC *pDC, int x, int y )
{
    // x,y - the DC coordinates to start from
    if(pDC) SetDIBitsToDevice ( pDC->m_hDC, x, y, m_Size.cx, m_Size.cy, 0, 0, 0, m_Size.cy, m_Bits, &m_Info, 0); 
}

void CDIB::PasteToDC ( CDC *pDC, int x, int y, int xsize, int ysize )
{
    // x,y - the DC coordinates to start from
    if(pDC) SetDIBitsToDevice ( pDC->m_hDC, x, y, xsize, ysize, 0, 0, 0, m_Size.cy, m_Bits, &m_Info, 0); 
}


void CDIB::GetFromBitmap ( CDC *pDC, CBitmap *pBitmap )
{
    if ( pDC && m_Bitmap )
        GetDIBits ( pDC->m_hDC, HBITMAP(*pBitmap), 0, m_Size.cy, m_Bits, &(m_Info), DIB_RGB_COLORS ); 
}


void CDIB::CopyTo ( CDIB *Dib )
{
    // If DibSize Wrong Re-Create Dib
    if ( (Dib->m_Size.cx!=m_Size.cx) || (Dib->m_Size.cy!=m_Size.cy) )
        Dib->Create ( m_Size.cx, m_Size.cy );
    // do Copy
    memcpy ( Dib->m_Bits, m_Bits, m_Size.cx*m_Size.cy*4 );
}


void CDIB::CopyFrom ( const CDIB *Dib )
{
    // If DibSize Wrong Re-Create Dib
    if ( (m_Size.cx!=Dib->m_Size.cx) || (m_Size.cy!=Dib->m_Size.cy) )
        Create ( Dib->m_Size.cx, Dib->m_Size.cx );
    // do Paste
    memcpy ( m_Bits, Dib->m_Bits, m_Size.cx*m_Size.cy*4 );
}


COLORREF &
CDIB::ColorAt (int x, int y)
{
    if( ((x >= 0) && (x <= m_Size.cx) && (y >= 0) && (y <= m_Size.cy)) ) return m_Bits[x + y * m_Size.cx];
	else return m_Bits[0];
    
}    

BYTE CDIB::AlphaLevelAt ( int x, int y )
{
	if( ((x >= 0) && (x <= m_Size.cx) && (y >= 0) && (y <= m_Size.cy)) )
	{
		COLORREF color = m_Bits[x + y * m_Size.cx];
		BYTE red, green, blue;
		
		red    = GetRValue( color );
		green  = GetGValue( color );
		blue   = GetBValue( color );

		BYTE Level = red; //(BYTE)((DWORD)( (float)( red   ) *0.299 )  +
							//(DWORD)( (float)( green ) *0.587 )  +
							//(DWORD)( (float)( blue  ) *0.114 )    );		
		if( Level < 0   ) Level = 0;
		if( Level > 255 ) Level = 255;

		return Level;
	}
	else return 0;
}

BOOL 
CDIB::operator != (const CDIB &dib) const
{
    return !(*this == dib);
}

BOOL 
CDIB::operator == (const CDIB &dib) const
{
	//if ( m_Size.cx != dib.m_Size.cx || m_Size.cy != dib->m_Size.cy )    
    //    return FALSE;
    if ((NULL == m_Bits) || (NULL == dib.m_Bits))
        return FALSE;
    if (0 != memcmp (&m_Info, &dib.m_Info, sizeof (BITMAPINFO)))
        return FALSE;
    if (0 != memcmp (m_Bits, dib.m_Bits, m_Info.bmiHeader.biSizeImage))
        return FALSE;
    return TRUE;
}

CDIB &
CDIB::operator = (const CDIB &dib)
{
    CopyFrom (&dib); 
    return *this;
}


void CDIB::HLine( int x,int y,int Size,COLORREF Color)
{
	if( ( x > m_Size.cx-1 ) || ( y > m_Size.cy-1 ) || ( x+Size < 0 ) || (y < 0) ) return;	
    
	if( x < 0 )
	{
		Size -= (-x);
		x = 0;
	}
    if( x+Size > m_Size.cx-1 )
	{
		Size -= (x+Size-m_Size.cx );
	}	
	
    COLORREF *pSurface = (COLORREF*)m_Bits + y * m_Size.cx + x; 
	    
	while( --Size >= 0 )	
	{
		pSurface[Size]  = Color;
	}
}


void CDIB::VLine( int x,int y,int Size,COLORREF Color)
{
    if( ( x > m_Size.cx-1 ) || ( y > m_Size.cy-1 ) || ( x < 0 ) || (y+Size < 0) ) return;	
    
	if( y < 0 )
	{
		Size -= ( - y );
		y = 0;
	}
    if( y+Size > m_Size.cy-1 )
	{
		Size -= (y+Size-m_Size.cy );		
	}	    
	
	COLORREF* pSurface = (COLORREF*)m_Bits + y * m_Size.cx + x; 
	    
	while( --Size >= 0 )
	{
		*pSurface = Color;
		pSurface+=m_Size.cx;
	}
}
  
BOOL CDIB::GaussianBlur( void )
{
	COLORREF* AlphaImgPtr = NULL;
	COLORREF* tAlphaImage = NULL;
	COLORREF* tAlphaImgPtr = NULL;
    int    i, j, x, y;
	int    redAlphalevel=0;
	int    greenAlphalevel=0;
	int    blueAlphalevel=0;
	
	tAlphaImage = (COLORREF*) GlobalAlloc( GMEM_FIXED | GMEM_ZEROINIT, m_Size.cx * m_Size.cy * sizeof( COLORREF ) );
	if( !tAlphaImage )	
	{
        AfxMessageBox( "[ CDIB::GaussianBlur() ] 메모리 할당에 실패 했습니다. [ %ld Bytes]", m_Size.cx * m_Size.cy  * sizeof( COLORREF ) );
		return FALSE;
    }
	
	for(i = 0; i< m_Size.cx*m_Size.cy; i++)
	{
		*(tAlphaImage + i) = *( m_Bits+m_Size.cx * m_Size.cy-1 );
	}
	
	int   Blur[3][3] = { 1, 2, 1,
                         2, 4, 2,
                         1, 2, 1 };
	/*int   Blur[3][3] = { 1, 1, 1,
                         1, 2, 1,
                         1, 1, 1 };*/
	                
	AlphaImgPtr = m_Bits;
	tAlphaImgPtr = tAlphaImage+m_Size.cx+1;

	COLORREF tColor;
		
	for(j = 1; j< m_Size.cy-1; j++)
	{
        for(i = 1; i<m_Size.cx-1; i++)
		{			
			if( *(AlphaImgPtr+i+j*m_Size.cx) )
			{			    
				redAlphalevel=0;
				greenAlphalevel=0;
				blueAlphalevel=0;

				for( y=0; y<3; y++)
				{
					for( x=0; x<3; x++ )
					{						
						tColor = *(AlphaImgPtr+i+x-1+(j+y-1)*(m_Size.cx) );

						redAlphalevel += Blur[y][x] * RGB_GETRED( tColor );
						greenAlphalevel += Blur[y][x] * RGB_GETGREEN( tColor );
						blueAlphalevel += Blur[y][x] * RGB_GETBLUE( tColor );
					}
				}
				redAlphalevel >>= 4;
				greenAlphalevel >>= 4;
				blueAlphalevel >>= 4;

				if( redAlphalevel > 255 ) redAlphalevel = 255;
				if( redAlphalevel < 0 ) redAlphalevel = 0;

				if( greenAlphalevel > 255 ) greenAlphalevel = 255;
				if( greenAlphalevel < 0 ) greenAlphalevel = 0;
				
				if( blueAlphalevel > 255 ) blueAlphalevel = 255;
				if( blueAlphalevel < 0 ) blueAlphalevel = 0;


				*(tAlphaImgPtr + (i-1) + (j-1) * (m_Size.cx) ) = RGB( redAlphalevel, greenAlphalevel, blueAlphalevel );
			}
		}				
	}
	
	memcpy( m_Bits, tAlphaImage, m_Size.cx * m_Size.cy * sizeof( COLORREF ) );

	if(tAlphaImage)
	{
		delete[] tAlphaImage;
		tAlphaImage = NULL;
	}
			
	return TRUE;
}

BOOL CDIB::NearestNeighborInterpolation( void )
{
	COLORREF* AlphaImgPtr = NULL;
	COLORREF* tAlphaImage = NULL;
	COLORREF* tAlphaImgPtr = NULL;
    int    i, j;
	int    redAlphalevel=0;
	int    greenAlphalevel=0;
	int    blueAlphalevel=0;
	
	tAlphaImage = (COLORREF*) GlobalAlloc( GMEM_FIXED | GMEM_ZEROINIT, m_Size.cx * m_Size.cy * sizeof( COLORREF ) );
	if( !tAlphaImage )	
	{
        AfxMessageBox( "[ CDIB::GaussianBlur() ] 메모리 할당에 실패 했습니다. [ %ld Bytes]", m_Size.cx * m_Size.cy  * sizeof( COLORREF ) );
		return FALSE;
    }
	
	for(i = 0; i< m_Size.cx*m_Size.cy; i++)
	{
		*(tAlphaImage + i) = *( m_Bits+m_Size.cx * m_Size.cy-1 );
	}
				                
	AlphaImgPtr  = m_Bits;
	tAlphaImgPtr = tAlphaImage;

	COLORREF tColor1;
	COLORREF tColor2;
	COLORREF tColor3;

	COLORREF tBackColor = *( m_Bits+m_Size.cx * m_Size.cy-1 );
			
	for(j = 0; j< m_Size.cy; j++)
	{
        for(i = 1; i<m_Size.cx-1; i++)
		{	
			
			if( (*( AlphaImgPtr + i - 1 ) != tBackColor ) || (*( AlphaImgPtr + i + 1 ) != tBackColor ) )
			{			    
				redAlphalevel=0;
				greenAlphalevel=0;
				blueAlphalevel=0;

				tColor1 = *( AlphaImgPtr + i - 1 );
				tColor2 = *( AlphaImgPtr + i     );	
				tColor3 = *( AlphaImgPtr + i + 1 );

				redAlphalevel   = RGB_GETRED( tColor1 );
				greenAlphalevel = RGB_GETGREEN( tColor1 );
				blueAlphalevel  = RGB_GETBLUE( tColor1 );

				redAlphalevel   += RGB_GETRED( tColor2 );
				greenAlphalevel += RGB_GETGREEN( tColor2 );
				blueAlphalevel  += RGB_GETBLUE( tColor2 );

				redAlphalevel   += RGB_GETRED( tColor3 );
				greenAlphalevel += RGB_GETGREEN( tColor3 );
				blueAlphalevel  += RGB_GETBLUE( tColor3 );
				
				redAlphalevel   /= 3;
				greenAlphalevel /= 3;
				blueAlphalevel  /= 3;

				if( redAlphalevel > 255 ) redAlphalevel = 255;
				if( redAlphalevel < 0 ) redAlphalevel = 0;

				if( greenAlphalevel > 255 ) greenAlphalevel = 255;
				if( greenAlphalevel < 0 ) greenAlphalevel = 0;
				
				if( blueAlphalevel > 255 ) blueAlphalevel = 255;
				if( blueAlphalevel < 0 ) blueAlphalevel = 0;

				*(tAlphaImgPtr + i) = RGB( redAlphalevel, greenAlphalevel, blueAlphalevel );
			}			
		}	
		AlphaImgPtr+=m_Size.cx;
		tAlphaImgPtr+=m_Size.cx;
	}
	
	memcpy( m_Bits, tAlphaImage, m_Size.cx * m_Size.cy * sizeof( COLORREF ) );

	if( tAlphaImage )
	{
		delete[] tAlphaImage;
		tAlphaImage = NULL;
	}
			
	return TRUE;
}


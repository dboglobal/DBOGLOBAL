// TextureInfoDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Venus.h"
#include "TextureInfoDialog.h"

#include "VenusAPI.h"
#include ".\textureinfodialog.h"

// CTextureInfoDialog 대화 상자입니다.

IMPLEMENT_DYNCREATE(CTextureInfoDialog, CDialog)
CTextureInfoDialog::CTextureInfoDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTextureInfoDialog::IDD, pParent)
	, m_nTextureWidth(0)
	, m_nTextureHeight(0)
{
	m_Initialized	= FALSE;
	m_pGflBitmap	= NULL;
}

CTextureInfoDialog::~CTextureInfoDialog()
{
	if(m_pGflBitmap)
	{
		gflFreeBitmap(m_pGflBitmap);
		m_pGflBitmap = NULL; 
	}
}

void CTextureInfoDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXTURE, m_TextureScreen);
	DDX_Text(pDX, IDC_TEXTURE_WIDTH, m_nTextureWidth);
	DDX_Text(pDX, IDC_TEXTURE_HEIGHT, m_nTextureHeight);
}


BEGIN_MESSAGE_MAP(CTextureInfoDialog, CDialog)
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CTextureInfoDialog 메시지 처리기입니다.

BOOL CTextureInfoDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_Initialized = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CTextureInfoDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_Initialized)
	{
	}
}

BOOL CTextureInfoDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN) 
	{
		if(pMsg->wParam == VK_RETURN) return FALSE;	
		if(pMsg->wParam == VK_ESCAPE) return FALSE;	
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CTextureInfoDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.
	if(!m_pGflBitmap) return;

	CRect rect;
	m_TextureScreen.GetWindowRect(rect);
	ScreenToClient( &rect );

/*	int minsize = ( rect.Width() < rect.Height() ) ? rect.Width() : rect.Height();
	minsize -= 4;
	minsize = (minsize>>1);

	int centerx = rect.left + (rect.Width()>>1);
	int centery = rect.top  + (rect.Height()>>1);

	rect.left   = centerx-minsize;
	rect.right  = centerx+minsize-1;
	rect.top    = centery-minsize;
	rect.bottom = centery+minsize-1;
*/
	HBRUSH hBrush=::CreateSolidBrush(RGB(255, 255, 255));
	::FillRect( dc.GetSafeHdc(), rect, hBrush);
	DeleteObject(hBrush);

	unsigned char *dib_data;
	BITMAPINFOHEADER dib_info;
	GetDIBFromBitmap( m_pGflBitmap, &dib_info, &dib_data );
	StretchDIBits( dc.GetSafeHdc(), rect.left, rect.top, rect.Width(), rect.Height(),
//	StretchDIBits( dc.GetSafeHdc(), rect.left, rect.top, (minsize<<1), (minsize<<1),
		0, 0, m_pGflBitmap->Width, m_pGflBitmap->Height, 
		dib_data ? dib_data : m_pGflBitmap->Data, (BITMAPINFO *)&dib_info, DIB_RGB_COLORS, SRCCOPY );				
	if ( dib_data ) free( dib_data ); 
}


void CTextureInfoDialog::CreateBitmap(const RwChar* strPathName)
{
	if(m_pGflBitmap)
	{
		gflFreeBitmap(m_pGflBitmap);
		m_pGflBitmap = NULL; 
	}

	if (strPathName == NULL)
	{
		Invalidate(TRUE);
		return;
	}

	GFL_LOAD_PARAMS load_params;	
	gflGetDefaultPreviewParams(&load_params);
	load_params.Flags |= GFL_LOAD_SKIP_ALPHA;
	load_params.Origin = GFL_BOTTOM_LEFT; 
	load_params.ColorModel = GFL_BGR; 
	load_params.LinePadding = 4;

	GFL_ERROR errorcode = gflLoadBitmap(strPathName, &m_pGflBitmap, &load_params, NULL);
	if( m_pGflBitmap )
	{
		m_nTextureWidth	= m_pGflBitmap->Width;
		m_nTextureHeight = m_pGflBitmap->Height;

		CRect rect;
		m_TextureScreen.GetWindowRect(rect);
		ScreenToClient( &rect );

//		int minsize = ( rect.Width() < rect.Height() ) ? rect.Width() : rect.Height();
//		minsize -= 4;						
//		gflResize(m_pGflBitmap, NULL, minsize, minsize, GFL_RESIZE_BILINEAR, 0);
		gflResize(m_pGflBitmap, NULL, rect.Width(), rect.Height(), GFL_RESIZE_BILINEAR, 0);

		RedrawWindow( &rect, NULL, RDW_INVALIDATE | RDW_UPDATENOW );

		UpdateData( FALSE );
	}
}


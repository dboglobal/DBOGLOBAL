// TextureConverterDoc.cpp : CTextureConverterDoc 클래스의 구현
//

#include "stdafx.h"
#include "TextureConverter.h"

#include "TextureConverterDoc.h"
#include ".\textureconverterdoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTextureConverterDoc

IMPLEMENT_DYNCREATE(CTextureConverterDoc, CDocument)

BEGIN_MESSAGE_MAP(CTextureConverterDoc, CDocument)
END_MESSAGE_MAP()


// CTextureConverterDoc 생성/소멸

CTextureConverterDoc::CTextureConverterDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

	m_sConverFlag.ReadFromIni( _T( "Convert.ini" ) ); 
	
//	m_sConverFlag.nMipLevel = 1;
//	m_sConverFlag.cFormat = _T( "D3DFMT_L8" );

}

CTextureConverterDoc::~CTextureConverterDoc()
{
}

BOOL CTextureConverterDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 다시 초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	CFileFind		FileFinder;

	if( m_sConverFlag.cInputPath.GetLength() )
	{
		SetCurrentDirectory( m_sConverFlag.cInputPath.GetBuffer() );
	}

	BOOL	bWorking = FileFinder.FindFile( m_sConverFlag.cFilter.GetBuffer() );
	while( bWorking )
	{
		bWorking = FileFinder.FindNextFile();

		ConvertFile( FileFinder.GetFileName() );
	}

	return TRUE;
}


// CTextureConverterDoc serialization

void CTextureConverterDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}


// CTextureConverterDoc 진단

#ifdef _DEBUG
void CTextureConverterDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTextureConverterDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTextureConverterDoc 명령

BOOL CTextureConverterDoc::ConvertFile(CString & rcFileName)
{
	m_cFileNameList.AddTail( rcFileName );
	CString		Command;

	if( m_sConverFlag.cInputPath.GetLength() == 0 )
	{
		if( m_sConverFlag.cOutputPath.GetLength() == 0 )
		{
			Command.Format( _T( "texconv %s %s" ), m_sConverFlag.cFlagString.GetBuffer(), rcFileName.GetBuffer() );
		}
		else
		{
			Command.Format( _T( "texconv %s -o %s %s" ), m_sConverFlag.cFlagString.GetBuffer(), m_sConverFlag.cOutputPath.GetBuffer(), rcFileName.GetBuffer() );
		}
	}
	else
	{
		if( m_sConverFlag.cOutputPath.GetLength() == 0 )
		{
			Command.Format( _T( "..\\texconv %s %s" ), m_sConverFlag.cFlagString.GetBuffer(), rcFileName.GetBuffer() );
		}
		else
		{
			Command.Format( _T( "..\\texconv %s -o ..\\%s %s" ), m_sConverFlag.cFlagString.GetBuffer(), m_sConverFlag.cOutputPath.GetBuffer(), rcFileName.GetBuffer() );
		}
	}

	system( Command.GetBuffer() );

/*	STARTUPINFO				StartupInfo;
	PROCESS_INFORMATION		ProcessInfo;

	ZeroMemory( &StartupInfo, sizeof( STARTUPINFO ) );
	StartupInfo.cb = sizeof( STARTUPINFO );

	ZeroMemory( &ProcessInfo, sizeof( PROCESS_INFORMATION ) );

	if( CreateProcess( NULL, Command.GetBuffer(),
					NULL, NULL, FALSE,
					NORMAL_PRIORITY_CLASS, NULL, NULL,
					&StartupInfo, &ProcessInfo ) == FALSE )
	{
		DWORD dwErr = GetLastError();
		return FALSE;
	}*/

	return TRUE;
}

const CString * CTextureConverterDoc::GetFileName(int nFileIndex) const
{
	if( nFileIndex >= GetFileCount() )
		return NULL;
	
	return &(m_cFileNameList.GetAt( m_cFileNameList.FindIndex( nFileIndex ) ));
}

int CTextureConverterDoc::GetFileCount(void) const
{
	return m_cFileNameList.GetCount();
}

// TextureConverterDoc.h : CTextureConverterDoc 클래스의 인터페이스
//


#pragma once


typedef struct SConvertFlag
{
	CString		cInputPath;
	CString		cOutputPath;
	CString		cFilter;

	int			nMipLevel;
	CString		cFormat;

	CString		cFlagString;

				SConvertFlag( void )
				{
					nMipLevel = 0;
					cFlagString.Empty();
				}

	void		ReadFromIni( TCHAR * pFileName )
	{
		TCHAR			szFileName[256];
		TCHAR			szBuffer[256];

		GetCurrentDirectory( 256, szFileName );
		_stprintf( szFileName, _T( "%s\\%s" ), szFileName, pFileName );

		GetPrivateProfileString( _T( "File" ), _T( "InputPath" ), _T( "Input" ), szBuffer, 256, szFileName );
		cInputPath = szBuffer;

		GetPrivateProfileString( _T( "File" ), _T( "OuputPath" ), _T( "Output" ), szBuffer, 256, szFileName );
		cOutputPath = szBuffer;

		GetPrivateProfileString( _T( "File" ), _T( "Filter" ), _T( "*.png" ), szBuffer, 256, szFileName );
		cFilter = szBuffer;

		GetPrivateProfileString( _T( "Convert" ), _T( "Format" ), NULL, szBuffer, 256, szFileName );
		cFormat = szBuffer;

		nMipLevel = GetPrivateProfileInt( _T( "Convert" ), _T( "MipLevel" ), 0, szFileName );

		cFlagString.Empty();
		CString		Flag;

		if( nMipLevel > 0 )
		{
			Flag.Format( _T( " -m %d" ), nMipLevel );

			cFlagString.Append( Flag );
		}

		if( cFormat.GetLength() > 0 )
		{
			Flag.Format( _T( " -f %s" ), cFormat.GetBuffer() );

			cFlagString.Append( Flag );
		}
	}


	const CString *	GetString( void ) const
	{
		return &cFlagString;
	}

} SConvertFlag;

class CTextureConverterDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CTextureConverterDoc();
	DECLARE_DYNCREATE(CTextureConverterDoc)

// 특성
public:

// 작업
public:

// 재정의
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 구현
public:
	virtual ~CTextureConverterDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 메시지 맵 함수를 생성했습니다.
protected:
	DECLARE_MESSAGE_MAP()
private:
	SConvertFlag m_sConverFlag;
	CList <CString, CString&> m_cFileNameList;
protected:
	BOOL ConvertFile(CString & rcFileName);
public:
	const CString * GetFileName(int nFileIndex) const;
	int GetFileCount(void) const;
};



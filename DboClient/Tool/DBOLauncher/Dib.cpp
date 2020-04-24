/*****************************************************************************
*
* File			: Dib.cpp
* Author		:
* Copyright		: (аж)NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          :
*
*****************************************************************************/


#include "stdafx.h"
#include "Dib.h"
#include <vector>


//////////////////////////////////////////////////////////////////////////
//
// CDib
//
//////////////////////////////////////////////////////////////////////////


CDib::CDib( void )
{
	m_nWidth = 0;
	m_nHeight = 0;

	m_byKeyRed = 255;
	m_byKeyGreen = 255;
	m_byKeyBlue = 255;

	m_nSize = 0;
	m_pData = NULL;
}

CDib::~CDib( void )
{
	UnLoad();
}

bool CDib::Load( const CString& strFileName )
{
	UnLoad();

	CFile file;
	if ( !file.Open( strFileName, CFile::modeRead ) )
	{
		return false;
	}

	// bmp by default
	BITMAPFILEHEADER Header;
	BITMAPINFOHEADER Info;

	if ( !file.Read (&Header, sizeof( BITMAPFILEHEADER ) ) )
	{
		return false;
	}
	if ( !file.Read( &Info, sizeof( BITMAPINFOHEADER ) ) ) 
	{
		return false;
	}

    m_nWidth = Info.biWidth;
	m_nHeight = abs( Info.biHeight );

	int nLoadWidth = m_nWidth + ( 4 - ( m_nWidth & 3 ) );

	// read real Image
	BYTE* pTempImage = new BYTE [nLoadWidth*m_nHeight*3];
	memset( pTempImage, 0, sizeof( BYTE ) * nLoadWidth * m_nHeight * 3 );

	file.Read( pTempImage, nLoadWidth * m_nHeight * 3 );

	file.Close(); 

	int nIndex = 0;

	bool bStart = 0;
	int nStart = 0;
	int nBlankCount = 0;

	std::vector<int> vBuffer;

	for ( int j = 0 ; j < m_nHeight ; ++j )
	{
		nStart = 0;
		nBlankCount = 0;

        int i = 0;
		for ( i = 0 ; i < nLoadWidth ; ++i )
		{
			BYTE byRed = pTempImage[nIndex];
			BYTE byGreen = pTempImage[nIndex+1];
			BYTE byBlue = pTempImage[nIndex+2];

			if ( byRed == m_byKeyRed &&
				 byGreen == m_byKeyGreen &&
				 byBlue == m_byKeyBlue )
			{
				if ( !bStart )
				{
					if ( nBlankCount )
					{
						vBuffer.push_back( j );
						vBuffer.push_back( i - nBlankCount );
						vBuffer.push_back( nBlankCount );
					}

					bStart = true;
					nStart = i;
					nBlankCount = 0;
				}
			}
			else
			{
				nBlankCount++;
				bStart = false;
			}

			nIndex+=3;
		}

		if ( nBlankCount )
		{
			vBuffer.push_back( j );
			vBuffer.push_back( i - nBlankCount );
			vBuffer.push_back( nBlankCount );
		}
	}

	delete [] pTempImage;

	m_nSize = (int)vBuffer.size();

	m_pData = new int [m_nSize];

	for ( int i = 0 ; i < m_nSize ; i++ )
	{
		m_pData[i] = vBuffer[i];
	}

	return true;
}

bool CDib::LoadLLE( const CString& strFileName )
{
	UnLoad();

	CFile file;
	if ( !file.Open( strFileName, CFile::modeRead ) )
	{
		return false;
	}

	file.Read( &m_byKeyRed, sizeof( BYTE ) ); 
	file.Read( &m_byKeyGreen, sizeof( BYTE ) );
	file.Read( &m_byKeyBlue, sizeof( BYTE ) );
	file.Read( &m_nWidth, sizeof( int ) );
	file.Read( &m_nHeight, sizeof( int ) );
	file.Read( &m_nSize, sizeof( int ) );

	m_pData = new int [m_nSize];

	file.Read( m_pData, sizeof( int ) * m_nSize );

	file.Close();

	return true;
}

bool CDib::SaveLLE( const CString& strFileName )
{
	CFile file;

	if ( !file.Open( strFileName, CFile::modeCreate | CFile::modeWrite ) )
	{
		return false;
	}

	file.Write( &m_byKeyRed, sizeof( BYTE ) ); 
	file.Write( &m_byKeyGreen, sizeof( BYTE ) );
	file.Write( &m_byKeyBlue, sizeof( BYTE ) );
	file.Write( &m_nWidth, sizeof( int ) );
	file.Write( &m_nHeight, sizeof( int ) );
	file.Write( &m_nSize, sizeof( int ) );
	file.Write( m_pData, sizeof( int ) * m_nSize );

	file.Close();

	return true;

}

void CDib::UnLoad( void )
{
	if ( m_pData )
	{
		delete [] m_pData;
		m_pData = NULL;
	}
}

bool CDib::IsLoad( void )
{
	if ( m_pData )
	{
		return TRUE;
	}

	return FALSE;
}

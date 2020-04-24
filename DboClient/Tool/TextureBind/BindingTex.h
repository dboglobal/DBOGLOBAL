#ifndef _BINDING_TEX_H_
#define _BINDING_TEX_H_

#define TEX_VER_NUM 4

#pragma once



class CBindingTex
{
public:
	CBindingTex(void);
	virtual ~CBindingTex(void);

	void		SetTexture( RwTexture* pTexture, RwInt32 nIndex );
	RwTexture*  GetTexture() { return m_pTexture; }

	void		SetIndex( RwInt32 nIndex )	{ m_nIndex = nIndex; }
	RwInt32		GetIndex()					{ return m_nIndex; }

	void		VertexAssign( CRect rtTex );

	RwInt32		GetWidth();
	RwInt32		GetHeight();

	CRect		GetRect();

	void		RenderParallel( RwCamera* pRwCamera );
	void		RenderPerspective( RwCamera* pRwCamera );
	void		Render();

protected:
	RwTexture*			m_pTexture;
	RwIm3DVertex		m_pIm3dBuffer[TEX_VER_NUM];
	RwInt32				m_nIndex;
};

class CBindingTexManager
{
public:
	enum eType
	{
		TYPE_BY_1 = 0,
		TYPE_BY_2,
		TYPE_BY_4,
		TYPE_BY_8,
		TYPE_BY_16,
		TYPE_BY_32,
		TYPE_BY_64
	};
	
	enum eSize
	{
		SIZE_BY_128 = 0,
		SIZE_BY_256,
		SIZE_BY_512,
		SIZE_BY_1024,
		SIZE_BY_2048,
		SIZE_BY_4096
	};

	CBindingTexManager(void);
	virtual ~CBindingTexManager(void);

	void		Create();
	void		Destroy();

	BOOL		LoadTexFromPath( eType type, eSize size, const CString strPath );
	BOOL		SaveTexCurrentData( eSize size, RwCamera* pRwCamera, const CString strPath );

	void		Render();
	void		RenderPerspective( RwCamera* pCamera );

	RwInt32		GetWidth()		{ return m_nWidth; }
	RwInt32		GetHeight()		{ return m_nHeight; }

	RwInt32		GetTileSize();
	RwInt32		GetTileType();

	RwInt32		GetTypeInt( eType type );
	RwInt32		GetSizeInt( eSize size );

protected:
	CBindingTex*		m_pBindingTex;
	int					m_nRows;
	int					m_nColumns;
	int					m_nWidth;
	int					m_nHeight;
	int					m_nCount;
};

#endif
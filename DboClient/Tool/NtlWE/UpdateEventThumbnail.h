#pragma once

class CUpdateEventThumbnail
{
public:
	CUpdateEventThumbnail();
	~CUpdateEventThumbnail();

	virtual void CreateThumbnail();
	virtual void RenderThumbnail();
	virtual void RestoreThumbnail();

	virtual void LoadThumbnailTex(const char* pName);

private:
	RwTexture*				m_pThumbnailTex;
	RwIm2DVertex			m_ThumbnailVert[4];
};
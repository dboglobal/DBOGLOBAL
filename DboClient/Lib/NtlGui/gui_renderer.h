#ifndef __GUI_RENDERER_H__
#define __GUI_RENDERER_H__

#include "gui_define.h"
#include "gui_surface.h"

START_GUI


#define GUI_UV_START	0.01f
#define GUI_UV_END		0.99f

class CTexture;

class CGuiRenderer
{
private:

	// 버퍼에 타입을 둔다.
	typedef struct _tagRenderBuffer
	{
		PlaneSnapShot	SnapShot;
		CTexture*		pTexture;
		FLOAT			fZRot;
		CRectangle*		pClipping;
	}SRenderBuffer;

	int m_nBufferNum;
	int m_nTopBufferNum;
	int m_nRenderNum;
	int m_nRenderTopNum;
	
	SRenderBuffer *m_pRenderBuffer;
	SRenderBuffer *m_pRenderTopBuffer;

	VOID BufferGrow(VOID);
	VOID BufferTopGrow(VOID);

	VOID RenderBuffer(const PlaneSnapShot *pSnapShot, CTexture *pTexture, FLOAT fZRot);

	//VOID ApplyClippingRect( const PlaneSnapShot* pSnapShot, const CRectangle* pClippingRect );
	   
public:

	CGuiRenderer();
	~CGuiRenderer();

	VOID ResetRender(VOID);
	VOID RenderQueue(const PlaneSnapShot *pSnapShot, CTexture *pTexture, bool bRenderTop = false, FLOAT fZRot = 0.0f, CRectangle* pClippingRect = NULL);
	VOID RenderBuffer( const TriSnapShot* pSnapShot, CTexture* pTexture, CRectangle* pClippingRect = NULL );
	VOID Render(VOID);

protected:

	RwIm2DVertex m_2dVertices[4];
	
};

END_GUI

extern gui::CGuiRenderer g_GuiRenderer;

#endif
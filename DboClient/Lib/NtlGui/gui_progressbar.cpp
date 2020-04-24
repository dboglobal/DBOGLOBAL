#include "gui_precomp.h"
#include "gui_progressbar.h"
#include "gui_progressbar_generic.h"
#include "gui_componentoption.h"
#include "gui_renderer.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

START_GUI

CProgressBar::CProgressBar(
	CComponent *pParent,
	CSurfaceManager *pSurfaceManager)
: CComponent(pParent,pSurfaceManager),m_pImpl(NULL)
{
	m_pImpl = NTL_NEW CProgressBar_Generic(this,0,100,0);

	m_SlotSetOption = SigSetOptions().Connect(
		this, &CProgressBar::OnSetOptions);
	m_SlotPaint = SigPaint().Connect(
		this, &CProgressBar::OnPaint);
	m_SlotMove = SigMove().Connect(
		this, &CProgressBar::OnMove);
}

CProgressBar::CProgressBar(
	const CRectangle &rtPos,
	CComponent *pParent,
	CSurfaceManager *pSurfaceManager)
: CComponent(rtPos,pParent,pSurfaceManager), m_pImpl(NULL)
{
	m_pImpl = NTL_NEW CProgressBar_Generic(this,0,100,0);

	m_SlotSetOption = SigSetOptions().Connect(
		this, &CProgressBar::OnSetOptions);
	m_SlotPaint = SigPaint().Connect(
		this, &CProgressBar::OnPaint);
	m_SlotMove = SigMove().Connect(
		this, &CProgressBar::OnMove);
}

CProgressBar::CProgressBar(
	const CRectangle &rtPos,
	CComponent *pParent,
	CSurfaceManager *pSurfaceManager,
	CSurface surface)
: CComponent(rtPos,pParent,pSurfaceManager),m_pImpl(NULL)
{
	m_pImpl = NTL_NEW CProgressBar_Generic(this,0,100,0);

	AddSurface(surface);

	m_SlotSetOption = SigSetOptions().Connect(
		this, &CProgressBar::OnSetOptions);
	m_SlotPaint = SigPaint().Connect(
		this, &CProgressBar::OnPaint);
	m_SlotMove = SigMove().Connect(
		this, &CProgressBar::OnMove);
}

CProgressBar::~CProgressBar()
{
	NTL_DELETE( m_pImpl );
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

//: panel surface
VOID CProgressBar::AddSurface(CSurface surface)
{
	CRectangle rtSnap = surface.m_SnapShot.rtRect;
	CRectangle rtScreen = GetScreenRect(); 
	
	surface.m_SnapShot.rtRect.left = rtScreen.left+rtSnap.left;
	surface.m_SnapShot.rtRect.top = rtScreen.top+rtSnap.top;
	surface.m_SnapShot.rtRect.right = surface.m_SnapShot.rtRect.left+rtSnap.GetWidth();
	surface.m_SnapShot.rtRect.bottom = surface.m_SnapShot.rtRect.top+rtSnap.GetHeight();

	m_pImpl->m_stlSurface.push_back(surface);
	m_pImpl->m_stlBUSurface.push_back(surface);
}

VOID CProgressBar::ClearSurface(VOID)
{
	m_pImpl->m_stlSurface.clear();
	m_pImpl->m_stlBUSurface.clear();
}

VOID CProgressBar::ResizeSurface(VOID)
{
	CRectangle rtScreen,rtSnap;

	CSurface *pSurface;
	std::list<CSurface> *stlPSurface = GetSurface();
	std::list<CSurface>::iterator the;

	for(the=stlPSurface->begin() ; 
			the!=stlPSurface->end() ; 
			the++)
	{
		pSurface = &(*the);
		rtScreen = GetScreenRect();

		pSurface->m_SnapShot = pSurface->m_Original; 

		rtSnap = pSurface->m_Original.rtRect;

		pSurface->m_SnapShot.rtRect.left = rtScreen.left+rtSnap.left;
		pSurface->m_SnapShot.rtRect.top = rtScreen.top+rtSnap.top;
		pSurface->m_SnapShot.rtRect.right = pSurface->m_SnapShot.rtRect.left+rtSnap.GetWidth();
		pSurface->m_SnapShot.rtRect.bottom = pSurface->m_SnapShot.rtRect.top+rtSnap.GetHeight();
	}

	stlPSurface = &m_pImpl->m_stlBUSurface;
	
	for(the=stlPSurface->begin() ; 
			the!=stlPSurface->end() ; 
			the++)
	{
		pSurface = &(*the);
		rtScreen = GetScreenRect();

		pSurface->m_SnapShot = pSurface->m_Original; 

		rtSnap = pSurface->m_Original.rtRect;

		pSurface->m_SnapShot.rtRect.left = rtScreen.left+rtSnap.left;
		pSurface->m_SnapShot.rtRect.top = rtScreen.top+rtSnap.top;
		pSurface->m_SnapShot.rtRect.right = pSurface->m_SnapShot.rtRect.left+rtSnap.GetWidth();
		pSurface->m_SnapShot.rtRect.bottom = pSurface->m_SnapShot.rtRect.top+rtSnap.GetHeight();
	}
}


/////////////////////////////////////////////////////////////////////////////
// Operations:

//: Sets the progressbar range.
VOID CProgressBar::SetRange(int nMin,int nMax)
{
	m_pImpl->m_nMin = nMin;
	m_pImpl->m_nMax = nMax;
}

//: Returns the progressbar range
VOID CProgressBar::GetRange(int& nMin,int& nMax)
{
	nMin = m_pImpl->m_nMin;
	nMax = m_pImpl->m_nMax; 
}

//: Set the current position
VOID CProgressBar::SetPos(int nPos)
{
	if(m_pImpl->m_bVertical)
		SetVerPos(nPos);
	else
		SetHorPos(nPos);
}


VOID CProgressBar::SetHorPos(int nPos)
{
	m_pImpl->m_nPos = nPos;

	if(m_pImpl->m_nPos < m_pImpl->m_nMin)
		m_pImpl->m_nPos = m_pImpl->m_nMin; 
	if(m_pImpl->m_nPos >= m_pImpl->m_nMax)
		m_pImpl->m_nPos = m_pImpl->m_nMax;

	if(m_pImpl->m_stlBUSurface.size() == 0)
		return;

	if(!m_pImpl->m_bReverseOrder) 
	{
		PROGRESS_BUFFER *buff = NTL_NEW PROGRESS_BUFFER[m_pImpl->m_stlBUSurface.size()];
		int nRange = m_pImpl->m_nMax-m_pImpl->m_nMin;
		float fRate = (float)(m_pImpl->m_nMin+m_pImpl->m_nPos)/(float)nRange;

		int nLeft=99999,nRight=-99999;
		std::list<CSurface>::iterator the;
		for(the=m_pImpl->m_stlBUSurface.begin(); 
				the!=m_pImpl->m_stlBUSurface.end() ; 
				the++)
		{
			if((*the).m_SnapShot.rtRect.left < nLeft)
				nLeft = (*the).m_SnapShot.rtRect.left;
			if((*the).m_SnapShot.rtRect.right > nRight)
				nRight = (*the).m_SnapShot.rtRect.right;
		}
		
		int nWidth = nRight-nLeft;  
		int nIndex = 0;
		nWidth = (int)((float)nWidth*fRate);
		
		for(the=m_pImpl->m_stlBUSurface.begin() ; 
				the!=m_pImpl->m_stlBUSurface.end() ; 
				the++)
		{
			if((*the).m_SnapShot.rtRect.right - nLeft > nWidth)
			{
				if((*the).m_SnapShot.rtRect.left - nLeft > nWidth)
				{
					buff[nIndex].nValue = (*the).m_SnapShot.rtRect.left;
					buff[nIndex].fUV = 0.0f;
				}
				else
				{
					float fSec,fSecS=0.0f,fSecE,fDiff;
					if((*the).m_SnapShot.rtRect.left - nLeft == 0)
						fSecS = 0.0f;
					else
						fSecS = (float)(float)((*the).m_SnapShot.rtRect.left - nLeft)/(nRight-nLeft);

					fSecE = (float)(float)((*the).m_SnapShot.rtRect.right - nLeft)/(nRight-nLeft);
					fSec = (fSecE-fSecS)/1.0f;
					fDiff = fRate-fSecS;
									 
					buff[nIndex].nValue = (*the).m_SnapShot.rtRect.left +
										(int)((float)((*the).m_SnapShot.rtRect.right - (*the).m_SnapShot.rtRect.left) *
										(fDiff/fSec));
					buff[nIndex].fUV = (fDiff/fSec);
				}
			}
			else
			{
				buff[nIndex].nValue = (*the).m_SnapShot.rtRect.right;
				buff[nIndex].fUV = 1.0f;
			}

			nIndex++;
		}

		int nSize = (int)m_pImpl->m_stlBUSurface.size();
		float fUVW; 
		CSurface* pSurface;
		the=m_pImpl->m_stlSurface.begin();

		for(int i=0 ; i<nSize ; i++)
		{
			pSurface = (CSurface*)&(*the);
			pSurface->m_SnapShot.rtRect.right = buff[i].nValue;
			fUVW = (pSurface->m_Original.UVs[2]-pSurface->m_Original.UVs[0])*buff[i].fUV;
			pSurface->m_SnapShot.UVs[2] = pSurface->m_Original.UVs[0]+fUVW;
			the++;
		}

		NTL_ARRAY_DELETE( buff );
	}
	else
	{
		PROGRESS_BUFFER *buff = NTL_NEW PROGRESS_BUFFER[m_pImpl->m_stlBUSurface.size()];
		int nRange = m_pImpl->m_nMax-m_pImpl->m_nMin;
		float fRate = 1.0f - (float)(m_pImpl->m_nMin+m_pImpl->m_nPos)/(float)nRange;

		int nLeft=99999,nRight=-99999;
		std::list<CSurface>::iterator the;
		for(the=m_pImpl->m_stlBUSurface.begin(); 
				the!=m_pImpl->m_stlBUSurface.end() ; 
				the++)
		{
			if((*the).m_SnapShot.rtRect.left < nLeft)
				nLeft = (*the).m_SnapShot.rtRect.left;
			if((*the).m_SnapShot.rtRect.right > nRight)
				nRight = (*the).m_SnapShot.rtRect.right;
		}
		
		int nWidth = nRight-nLeft;  
		int nIndex = 0;
		nWidth = (int)((float)nWidth*fRate);
		
		for(the=m_pImpl->m_stlBUSurface.begin() ; 
				the!=m_pImpl->m_stlBUSurface.end() ; 
				the++)
		{
			if((*the).m_SnapShot.rtRect.right  >= nLeft+nWidth)
			{
				if((*the).m_SnapShot.rtRect.left  >= nLeft + nWidth)
				{
					buff[nIndex].nValue = (*the).m_SnapShot.rtRect.left;
					buff[nIndex].fUV = 0.0f;
				}
				else
				{
					float fSec,fSecS=0.0f,fSecE,fDiff;
					if((*the).m_SnapShot.rtRect.left - nLeft == 0)
						fSecS = 0.0f;
					else
						fSecS = (float)(float)((*the).m_SnapShot.rtRect.left - nLeft)/(nRight-nLeft);

					fSecE = (float)(float)((*the).m_SnapShot.rtRect.right - nLeft)/(nRight-nLeft);
					fSec = (fSecE-fSecS)/1.0f;
					fDiff = fRate-fSecS;
									 
					buff[nIndex].nValue = (*the).m_SnapShot.rtRect.left +
																(int)((float)((*the).m_SnapShot.rtRect.right - (*the).m_SnapShot.rtRect.left) *
																(fDiff/fSec));
					buff[nIndex].fUV = (fDiff/fSec);
				}
			}
			else
			{
				buff[nIndex].nValue = (*the).m_SnapShot.rtRect.right;
				buff[nIndex].fUV = 1.0f;
			}

			nIndex++;
		}

		int nSize = (int)m_pImpl->m_stlBUSurface.size();
		float fUVW; 
		CSurface* pSurface;
		the=m_pImpl->m_stlSurface.begin();

		for(int i=0 ; i<nSize ; i++)
		{
			pSurface = (CSurface*)&(*the);
			pSurface->m_SnapShot.rtRect.left = buff[i].nValue;
			fUVW = (pSurface->m_Original.UVs[2]-pSurface->m_Original.UVs[0])*buff[i].fUV;
			pSurface->m_SnapShot.UVs[0] = pSurface->m_Original.UVs[0]+fUVW;
			the++;
		}

		NTL_ARRAY_DELETE( buff );
	}
}

VOID CProgressBar::SetVerPos(int nPos)
{
	m_pImpl->m_nPos = nPos;

	if(m_pImpl->m_nPos < m_pImpl->m_nMin)
		m_pImpl->m_nPos = m_pImpl->m_nMin; 
	if(m_pImpl->m_nPos >= m_pImpl->m_nMax)
		m_pImpl->m_nPos = m_pImpl->m_nMax;

	if(m_pImpl->m_stlBUSurface.size() == 0)
		return;

	if(!m_pImpl->m_bReverseOrder) 
	{
		PROGRESS_BUFFER *buff = NTL_NEW PROGRESS_BUFFER[m_pImpl->m_stlBUSurface.size()];
		int nRange = m_pImpl->m_nMax-m_pImpl->m_nMin;
		float fRate = (float)(m_pImpl->m_nMin+m_pImpl->m_nPos)/(float)nRange;

		int nTop=99999,nBottom=-99999;
		std::list<CSurface>::iterator the;
		for(the=m_pImpl->m_stlBUSurface.begin(); 
				the!=m_pImpl->m_stlBUSurface.end() ; 
				the++)
		{
			if((*the).m_SnapShot.rtRect.top < nTop)
				nTop = (*the).m_SnapShot.rtRect.top;
			if((*the).m_SnapShot.rtRect.bottom > nBottom)
				nBottom = (*the).m_SnapShot.rtRect.bottom;
		}
		
		int nHeight = nBottom-nTop;  
		int nIndex = 0;
		nHeight = (int)((float)nHeight*fRate);
		
		for(the=m_pImpl->m_stlBUSurface.begin() ; 
				the!=m_pImpl->m_stlBUSurface.end() ; 
				the++)
		{
			if((*the).m_SnapShot.rtRect.bottom - nTop > nHeight)
			{
				if((*the).m_SnapShot.rtRect.top - nTop > nHeight)
				{
					buff[nIndex].nValue = (*the).m_SnapShot.rtRect.top;
					buff[nIndex].fUV = 0.0f;
				}
				else
				{
					float fSec,fSecS=0.0f,fSecE,fDiff;
					if((*the).m_SnapShot.rtRect.top - nTop == 0)
						fSecS = 0.0f;
					else
						fSecS = (float)(float)((*the).m_SnapShot.rtRect.top - nTop)/(nBottom-nTop);

					fSecE = (float)(float)((*the).m_SnapShot.rtRect.bottom - nTop)/(nBottom-nTop);
					fSec = (fSecE-fSecS)/1.0f;
					fDiff = fRate-fSecS;
									 
					buff[nIndex].nValue = (*the).m_SnapShot.rtRect.top +
										(int)((float)((*the).m_SnapShot.rtRect.bottom - (*the).m_SnapShot.rtRect.top) *
										(fDiff/fSec));
					buff[nIndex].fUV = (fDiff/fSec);
				}
			}
			else
			{
				buff[nIndex].nValue = (*the).m_SnapShot.rtRect.bottom;
				buff[nIndex].fUV = 1.0f;
			}

			nIndex++;
		}

		int nSize = (int)m_pImpl->m_stlBUSurface.size();
		float fUVW; 
		CSurface* pSurface;
		the=m_pImpl->m_stlSurface.begin();

		for(int i=0 ; i<nSize ; i++)
		{
			pSurface = (CSurface*)&(*the);
			pSurface->m_SnapShot.rtRect.bottom = buff[i].nValue;
			fUVW = (pSurface->m_Original.UVs[3]-pSurface->m_Original.UVs[1])*buff[i].fUV;
			pSurface->m_SnapShot.UVs[3] = pSurface->m_Original.UVs[1]+fUVW;
			the++;
		}

		NTL_ARRAY_DELETE( buff );
	}
	else
	{
		PROGRESS_BUFFER *buff = NTL_NEW PROGRESS_BUFFER[m_pImpl->m_stlBUSurface.size()];
		int nRange = m_pImpl->m_nMax-m_pImpl->m_nMin;
		float fRate = 1.0f - (float)(m_pImpl->m_nMin+m_pImpl->m_nPos)/(float)nRange;

		int nTop=99999,nBottom=-99999;
		std::list<CSurface>::iterator the;
		for(the=m_pImpl->m_stlBUSurface.begin(); 
				the!=m_pImpl->m_stlBUSurface.end() ; 
				the++)
		{
			if((*the).m_SnapShot.rtRect.top < nTop)
				nTop = (*the).m_SnapShot.rtRect.top;
			if((*the).m_SnapShot.rtRect.bottom > nBottom)
				nBottom = (*the).m_SnapShot.rtRect.bottom;
		}
		
		int nHeight = nBottom-nTop;  
		int nIndex = 0;
		nHeight = (int)((float)nHeight*fRate);
		
		for(the=m_pImpl->m_stlBUSurface.begin() ; 
				the!=m_pImpl->m_stlBUSurface.end() ; 
				the++)
		{
			if((*the).m_SnapShot.rtRect.bottom  >= nTop+nHeight)
			{
				if((*the).m_SnapShot.rtRect.top  >= nTop + nHeight)
				{
					buff[nIndex].nValue = (*the).m_SnapShot.rtRect.top;
					buff[nIndex].fUV = 0.0f;
				}
				else
				{
					float fSec,fSecS=0.0f,fSecE,fDiff;
					if((*the).m_SnapShot.rtRect.top - nTop == 0)
						fSecS = 0.0f;
					else
						fSecS = (float)(float)((*the).m_SnapShot.rtRect.top - nTop)/(nBottom-nTop);

					fSecE = (float)(float)((*the).m_SnapShot.rtRect.bottom - nTop)/(nBottom-nTop);
					fSec = (fSecE-fSecS)/1.0f;
					fDiff = fRate-fSecS;
									 
					buff[nIndex].nValue = (*the).m_SnapShot.rtRect.top +
												(int)((float)((*the).m_SnapShot.rtRect.bottom - (*the).m_SnapShot.rtRect.top) *
												(fDiff/fSec));
					buff[nIndex].fUV = (fDiff/fSec);
				}
			}
			else
			{
				buff[nIndex].nValue = (*the).m_SnapShot.rtRect.bottom;
				buff[nIndex].fUV = 1.0f;
			}

			nIndex++;
		}

		int nSize = (int)m_pImpl->m_stlBUSurface.size();
		float fUVW; 
		CSurface* pSurface;
		the=m_pImpl->m_stlSurface.begin();

		for(int i=0 ; i<nSize ; i++)
		{
			pSurface = (CSurface*)&(*the);
			pSurface->m_SnapShot.rtRect.top = buff[i].nValue;
			fUVW = (pSurface->m_Original.UVs[3]-pSurface->m_Original.UVs[1])*buff[i].fUV;
			pSurface->m_SnapShot.UVs[1] = pSurface->m_Original.UVs[1]+fUVW;
			the++;
		}

		NTL_ARRAY_DELETE( buff );
	}
}


//: Returns the progressbar current position
int CProgressBar::GetPos(VOID)
{
	return m_pImpl->m_nPos; 
}

std::list<CSurface>* CProgressBar::GetSurface(VOID)
{
	return &m_pImpl->m_stlSurface;
}

/////////////////////////////////////////////////////////////////////////////
// Signals:

VOID CProgressBar::OnSetOptions(const CComponentOptions& options)
{
	int nX=0,nY=0,nWidth=0,nHeight=0;
	
	if(options.Exists("x"))
		nX = options.GetValueAsInt("x");
	if(options.Exists("y"))
		nY = options.GetValueAsInt("y");
	if(options.Exists("width"))
		nWidth = options.GetValueAsInt("width");
	if(options.Exists("height"))
		nHeight = options.GetValueAsInt("height");

	SetPosition(CRectangle(nX,nY,nX+nWidth,nY+nHeight));

	if(options.Exists("surface_file"))
	{
		std::string file = options.GetValue("surface_file"); 
		CSurfaceManager *pSurfaceManager = GetSurfaceManager();
		if(pSurfaceManager)
		{
			int nCount = options.Count("surface");
			for(int i=0 ; i<nCount ; i++)
				AddSurface(pSurfaceManager->GetSurface(file,options.GetValue("surface",i))); 
		}
	}

	//---------------------------------------------------------------------------------
	SetPos(GetPos());
}


VOID CProgressBar::OnPaint(VOID)
{
	std::list<CSurface> *stlPSurface = GetSurface();
	std::list<CSurface>::iterator the;
	for(the=stlPSurface->begin(); the!=stlPSurface->end(); the++)
	{
		g_GuiRenderer.RenderQueue(&(*the).m_SnapShot, (*the).m_pTexture);
	}
}

VOID CProgressBar::OnMove(int nX,int nY)
{
	ResizeSurface();
	SetPos(GetPos());
}


END_GUI

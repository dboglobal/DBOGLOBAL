#include "gui_precomp.h"
#include "gui_frame_generic.h"
#include "gui_componentoption.h"

START_GUI

CFrame_Generic::CFrame_Generic(CFrame *pSelf)
: m_bFill(false), m_pFrame(pSelf)
{
	m_SlotSetOptions = m_pFrame->SigSetOptions().Connect(
		this, &CFrame_Generic::OnSetOptions);
}

VOID CFrame_Generic::AddSurface(CSurface surface)
{
	CRectangle rtSnap = surface.m_SnapShot.rtRect;
	CRectangle rtScreen = m_pFrame->GetScreenRect(); 

	surface.m_SnapShot.rtRect.left = rtScreen.left + rtSnap.left;
	surface.m_SnapShot.rtRect.top = rtScreen.top + rtSnap.top;
	surface.m_SnapShot.rtRect.right = surface.m_SnapShot.rtRect.left + rtSnap.GetWidth();
	surface.m_SnapShot.rtRect.bottom = surface.m_SnapShot.rtRect.top + rtSnap.GetHeight();

	m_stlSurface.push_back(surface);
}

VOID CFrame_Generic::OnSetOptions(const CComponentOptions &options)
{
	if (options.Exists("mode"))
	{
	/*	std::string v = options.GetValue("mode");
		
		if (v == "center") mode = center;
		else if (v == "stretch") mode = stretch;
		else if (v == "tile") mode = tile;
		else
			assert(0 && "unknown frame mode");
			*/
	}

	if(options.Exists( "surface_file" ) )  
	{
		std::string file = options.GetValue( "surface_file" ); 
		CSurfaceManager *pSurfaceManager = m_pFrame->GetSurfaceManager();
		if( pSurfaceManager ) 
		{
			INT nCount = options.Count( "surface" );
			for( INT i = 0 ; i < nCount ; ++i )
				AddSurface(pSurfaceManager->GetSurface(file,options.GetValue("surface",i))); 
		}
	}
}


END_GUI
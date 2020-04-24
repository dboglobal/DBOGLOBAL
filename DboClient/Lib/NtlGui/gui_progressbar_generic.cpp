#include "gui_precomp.h"
#include "gui_progressbar_generic.h"
#include "gui_componentoption.h"

START_GUI

CProgressBar_Generic::CProgressBar_Generic(CProgressBar *pSelf,
																					int nMin,
																					int nMax,
																					int nPos)
:
	m_pProgressBar(pSelf),
	m_nMin(nMin),
	m_nMax(nMax),
	m_nPos(nPos),
	m_bVertical(false),
	m_bReverseOrder(false)
{
	m_SlotSetOption = m_pProgressBar->SigSetOptions().Connect(
		this, &CProgressBar_Generic::OnSetOptions);
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

VOID CProgressBar_Generic::OnSetOptions(const CComponentOptions &options)
{
	if(options.Exists("min"))
		m_nMin = options.GetValueAsInt("min");

	if(options.Exists("max"))
		m_nMax = options.GetValueAsInt("max");

	if(options.Exists("value"))	
		m_nPos = options.GetValueAsInt("value");

	if(options.Exists("vertical"))
		m_bVertical = options.GetValueAsBool("vertical");

	if(options.Exists("reverse_order"))
		m_bReverseOrder = options.GetValueAsBool("reverse_order");
}

END_GUI
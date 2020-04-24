#include "precomp_dboclient.h"

// core
#include "NtlDebug.h"
#include "CEventHandler.h"

/// Sound
#include "GuiSoundDefine.h"

// presentation
#include "NtlPLGui.h"
#include "NtlPLGuiManager.h"

// framework
#include "NtlApplication.h"

// dbo
#include "DialogManager.h"
#include "DisplayStringManager.h"

// Option
#include "OptionBase.h"
#include "OptionWindowGui.h"


////////////////////////////////////////////////////////////////////////////////
// Class : COptionBase
////////////////////////////////////////////////////////////////////////////////

COptionBase::COptionBase() : m_pOptionWindow(NULL), m_iNumOption(0), m_pScrollBar(NULL), m_iNumVisible(0)
{
}

COptionBase::~COptionBase()
{
}

RwBool COptionBase::Create(COptionWindowGui* pOptionWindow)
{
	NTL_FUNCTION("COptionBase::Create");
	m_pOptionWindow	= pOptionWindow;
	m_pScrollBar	= pOptionWindow->GetScrollBar();
	m_iNumVisible	= dOPTION_VIRTUAL_NUM;
	NTL_RETURN(TRUE);
}

void COptionBase::Destroy()
{
	NTL_FUNCTION("COptionBase::Destroy");
	NTL_RETURNVOID();
}

void COptionBase::Show(RwBool bShow)
{
	if (bShow)
	{
		Show();
		Update();
	}
	else
	{
		Hide();
	}
}

void COptionBase::Show()
{
	RwInt32 iMax = m_iNumOption - m_iNumVisible;
	SetScrollBarRange(0, (iMax > 0 ? iMax : 0));	
	SetScrollBarValue(0);

	for (int i = 0; i < (int)m_vecComponent.size(); ++i)
	{
		COMPONENTVEC& vecComponent = m_vecComponent.at(i);
		if (vecComponent.size() > 0)
		{
			((gui::CStaticBox*)vecComponent.at(0))->SetText(m_vecOptionName[i]);
			((gui::CStaticBox*)vecComponent.at(0))->SetToolTip(m_vecToolTipName[i]);
		}
	}
}

void COptionBase::Hide()
{
	for (int i = 0; i < (int)m_vecComponent.size(); ++i)
	{
		COMPONENTVEC& vecComponent = m_vecComponent.at(i);		
		for (int j = 0; j < (int)vecComponent.size(); ++j)
		{
			SetComponentPos(-1, vecComponent.at(j));
		}
	}
}

void COptionBase::Update()
{
	RwInt32 iOffset = GetScrollBarValue();
	RwInt32	iShow	= 0;
	for (int i = 0; i < (int)m_vecComponent.size(); ++i)
	{
		COMPONENTVEC&	vecComponent	= m_vecComponent.at(i);
		RwInt32			iOffsetRe		= i - iOffset;

		if (iOffsetRe > 0)
		{
			if (++iShow >= m_iNumVisible)
			{
				iOffsetRe = -1;
			}
		}

		for (int j = 0; j < (int)vecComponent.size(); ++j)
		{
			SetComponentPos(iOffsetRe, vecComponent.at(j));
		}
	}
}

void COptionBase::SetComponentGroup(RwInt32 nIndex, LPCWSTR pName, ...)
{
	if (nIndex >= m_iNumOption)
	{
		m_iNumOption = nIndex + 1;
		m_vecComponent.resize(m_iNumOption);
		m_vecOptionName.resize(m_iNumOption);
	}

	va_list ap; 
	va_start(ap, pName); 

	COMPONENTVEC vecComponent;
	while (true)
	{
		gui::CComponent* pComponent = va_arg(ap, gui::CComponent*);
		if (pComponent == NULL)
			break;
		vecComponent.push_back(pComponent);	
	}

	m_vecOptionName[nIndex]	= pName;
	m_vecComponent[nIndex]	= vecComponent;
	va_end(ap); 
}

void COptionBase::SetComponentGroup( RwInt32 nIndex, LPCWSTR pName, LPCWSTR pToolTip, ... )
{
	if (nIndex >= m_iNumOption)
	{
		m_iNumOption = nIndex + 1;
		m_vecComponent.resize(m_iNumOption);
		m_vecOptionName.resize(m_iNumOption);
		m_vecToolTipName.resize(m_iNumOption);
	}

	va_list ap; 
	va_start(ap, pToolTip); 

	COMPONENTVEC vecComponent;
	while (true)
	{
		gui::CComponent* pComponent = va_arg(ap, gui::CComponent*);
		if (pComponent == NULL)
			break;
		vecComponent.push_back(pComponent);	
	}

	m_vecOptionName[nIndex]	= pName;
	m_vecToolTipName[nIndex] = pToolTip;
	m_vecComponent[nIndex]	= vecComponent;
	va_end(ap); 
}
void COptionBase::SetComponentOnOff(SOnOff* pOnOff, gui::CButton* pOnComponent, gui::CButton* pOffComponent, RwBool bOn)
{
	pOnComponent->SetText(GetDisplayStringManager()->GetString("DST_OPTION_RADIO_ON"));
	pOffComponent->SetText(GetDisplayStringManager()->GetString("DST_OPTION_RADIO_OFF"));

	pOnOff->pOnComponent	= pOnComponent;
	pOnOff->pOffComponent	= pOffComponent;

	pOnOff->slotOnComponent		= pOnComponent->SigToggled().Connect(this, &COptionBase::OnOnOffToggled);
	pOnOff->slotOffComponent	= pOffComponent->SigToggled().Connect(this, &COptionBase::OnOnOffToggled);

	m_mapOnOff[pOnComponent]	= pOnOff;
	m_mapOnOff[pOffComponent]	= pOnOff;

	SetOnOff(pOnOff, bOn);
}

void COptionBase::SetOnOff(SOnOff* pOnOff, RwBool bOn)
{
	gui::CButton* pDown	= NULL;
	gui::CButton* pUp	= NULL;

	if (bOn)
	{
		pDown	= pOnOff->pOnComponent;
		pUp		= pOnOff->pOffComponent;

	}
	else
	{
		pDown	= pOnOff->pOffComponent;
		pUp		= pOnOff->pOnComponent;

	}

	pUp->SetDown(false);
	pUp->Enable(true);
	pDown->SetDown(true);
	pDown->Enable(false);

	pOnOff->bValue = bOn;
}

void COptionBase::OnOnOffToggled(gui::CComponent* pComponent, bool bToggled)
{
	ONOFFMAP::iterator it = m_mapOnOff.find(pComponent);
	if (it != m_mapOnOff.end() && bToggled)
	{
		SOnOff* pOnOff = it->second;
		SetOnOff(pOnOff, (pOnOff->pOnComponent == pComponent));
	}
}

void COptionBase::OnListToggled(BOOL bToggled, gui::CComponent* pComponent)
{
	if (bToggled)
	{
		pComponent->GetParent()->SetHeight(dOPTION_VIRTUAL_HEIGHT_EX);
		pComponent->GetParent()->Raise();
	}
	else
	{
		pComponent->GetParent()->SetHeight(dOPTION_VIRTUAL_HEIGHT);
		pComponent->GetParent()->Lower();
	}
}

gui::CComponent* COptionBase::GetComponent( const std::string& str ) 
{
	return m_pOptionWindow->GetComponent(str);
}

void COptionBase::SetComponentPos( RwInt32 iPos, gui::CComponent* pCompnent ) 
{
	m_pOptionWindow->SetComponentPos(iPos, pCompnent);
}
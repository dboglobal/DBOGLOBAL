#include "precomp_ntlsimulation.h"
#include "NtlSobIcon.h"

// presentation
#include "gui_texturemanager.h"

// simulation
#include "NtlSobManager.h"
#include "NtlSLEventFunc.h"
#include "NtlSLEvent.h"
#include "NtlSLLogic.h"
#include "NtlSLGlobal.h"

RwReal CNtlSobIcon::m_fCollingUpdateFactor = 1.0f;

CNtlSobIcon::CNtlSobIcon() 
{
	m_pSobObj = NULL;
	m_pImage = NULL;

	m_byIconState = ICON_STATE_NOTUSEABLE;
	m_fCoolTime = 0.0f;
	m_fMaxCoolTime = 0.0f;
    m_wErrorMsg = 0;
}

CNtlSobIcon::~CNtlSobIcon() 
{
	if(m_pImage)
	{
		g_TextureMgr.DeleteTexture(reinterpret_cast<gui::CTexture*>(m_pImage));
		m_pImage = NULL;
	}
}

void CNtlSobIcon::SetSobObj(CNtlSob *pSobObj)
{
	m_pSobObj = pSobObj;
}

void CNtlSobIcon::CreateImage(const RwChar *pFileName)
{
	if(m_pImage)
	{
		g_TextureMgr.DeleteTexture(reinterpret_cast<gui::CTexture*>(m_pImage));
		m_pImage = NULL;
	}

	std::string buf = "icon\\";
	buf += pFileName;

	m_pImage = reinterpret_cast<void*>(g_TextureMgr.CreateTexture(buf.c_str()));

	if(!m_pImage)
	{
		m_pImage = reinterpret_cast<void*>(g_TextureMgr.CreateTexture("icon\\DEF_NTL_ICO.png"));
	}
}

void CNtlSobIcon::Update(RwReal fElapsed)
{
	if( Logic_IsCoolTimeUpdate( reinterpret_cast<CNtlSobActor*>( GetNtlSLGlobal()->GetSobAvatar() ) ) )
	{
		if(m_byIconState == ICON_STATE_COOLING)
		{
			m_fCoolTime += fElapsed*m_fCollingUpdateFactor;

			if(m_fCoolTime >= m_fMaxCoolTime)
			{
				m_fCoolTime = 0;

				GetNtlSobManager()->RemoveUpdateQueue( m_pSobObj );

				SetIconState(ICON_STATE_USEABLE);			
			}
		}
	}	
}

void CNtlSobIcon::SetIconState(RwUInt8 byIconState,RwReal fCoolTIme /* = 0.0f */)
{
	if(byIconState == ICON_STATE_USEABLE)
	{
		if(m_byIconState == ICON_STATE_COOLING)
		{
			CNtlSLEventGenerator::SobCooling( GetSobObj()->GetSerialID(), FALSE );				
		}
	}
	else if(byIconState == ICON_STATE_COOLING)
	{
		m_fCoolTime = fCoolTIme;

		GetNtlSobManager()->AddUpdate( m_pSobObj );
		CNtlSLEventGenerator::SobCooling( GetSobObj()->GetSerialID(), TRUE );			
	}
	else if (byIconState == ICON_STATE_DISABLED)
	{
		GetNtlSobManager()->RemoveUpdateQueue(m_pSobObj);
		CNtlSLEventGenerator::SobCooling(GetSobObj()->GetSerialID(), FALSE);
	}

	m_byIconState = byIconState;
}

void CNtlSobIcon::SetIconState( RwUInt8 byIconState, RwReal fMaxCoolTime, RwReal fCoolTime )
{
	m_fMaxCoolTime = fMaxCoolTime;
	
	SetIconState( byIconState, fCoolTime );	
}


void CNtlSobIcon::SetCoolingUpdateFactor(RwReal fFactor)
{
	m_fCollingUpdateFactor = fFactor;
}
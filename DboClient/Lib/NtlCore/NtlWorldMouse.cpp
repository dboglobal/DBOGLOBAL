// Mouse.cpp: implementation of the CNtlWorldMouse class.
//
//////////////////////////////////////////////////////////////////////

#include "precomp_ntlcore.h"
#include "NtlWorldMouse.h"
#include <Winsock2.h>
#include <windows.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNtlWorldMouse::CNtlWorldMouse()
{
	ZeroMemory(m_Behavior, MB_NUM * sizeof(bool));

	ZeroMemory(&m_LBDNVariation, sizeof(MOUSE_VARIATION));
	ZeroMemory(&m_MBDNVariation, sizeof(MOUSE_VARIATION));
	ZeroMemory(&m_RBDNVariation, sizeof(MOUSE_VARIATION));

	m_WheelMovement = 0.0f;

	// one click flag
	m_LBOneClick	= false;
	m_LBUpClicked	= false;

	m_RBOneClick	= false;
	m_RBUpClicked	= false;

	m_LBDC = false;
}

CNtlWorldMouse::~CNtlWorldMouse()
{
}

void CNtlWorldMouse::Update()
{
	if(m_Behavior[MB_MOVE])
	{
		if(m_Behavior[MB_LBDN])
			SetVariation(MB_DEFAULT, m_LBDNVariation);
		
		if(m_Behavior[MB_MBDN])
			SetVariation(MB_DEFAULT, m_MBDNVariation);
		
		if(m_Behavior[MB_RBDN])
			SetVariation(MB_DEFAULT, m_RBDNVariation);
	}
}

void CNtlWorldMouse::SetVariation(int Behavior, MOUSE_VARIATION& Variation, int XPos, int YPos)
{
	m_PosClicked.x = XPos;
	m_PosClicked.y = YPos;

	switch(Behavior)
	{
	case MB_LBDN:
	case MB_MBDN:
	case MB_RBDN:
		{
			Variation.XOld = XPos;
			Variation.YOld = YPos;
			Variation.XNew = 0;
			Variation.YNew = 0;
			break;
		}
	case MB_MOVE:
		{
			if(Variation.XOld == XPos && Variation.YOld == YPos)
				return;
			
			Variation.XMove = XPos;
			Variation.YMove = YPos;
			
			break;
		}
	default:
		{
			Variation.XNew = -Variation.XOld + Variation.XMove;
			Variation.YNew = -Variation.YOld + Variation.YMove;
			Variation.XOld = Variation.XMove;
			Variation.YOld = Variation.YMove;
		}
	}
}

const POINT& CNtlWorldMouse::GetPosClicked() const
{
	return m_PosClicked;
}

void CNtlWorldMouse::SetBehavior(int Behavior, int XPos, int YPos, bool Activated)
{
	m_Behavior[Behavior] = Activated;	

	switch(Behavior)
	{
	case MB_LBDC:
		{
			m_PosClicked.x = XPos;
			m_PosClicked.y = YPos;
			m_LBDC = true;
			break;
		}
	case MB_LBDN:
		{
			if(Activated)
				m_LBOneClick = true;
			else
				m_LBUpClicked = true;

			if(m_Behavior[Behavior])
				SetVariation(Behavior, m_LBDNVariation, XPos, YPos);

			break;
		}
	case MB_MBDN:
		{
			if(m_Behavior[Behavior])
				SetVariation(Behavior, m_MBDNVariation, XPos, YPos);

			break;
		}
	case MB_RBDN:
		{
			if(Activated)
				m_RBOneClick = true;
			else
				m_RBUpClicked = true;

			if(m_Behavior[Behavior])
				SetVariation(Behavior, m_RBDNVariation, XPos, YPos);

			break;
		}
	case MB_MOVE:
		{
			m_CurPos.x = XPos;
			m_CurPos.y = YPos;

			if(m_Behavior[MB_LBDN])
				SetVariation(Behavior, m_LBDNVariation, XPos, YPos);

			if(m_Behavior[MB_MBDN])
				SetVariation(Behavior, m_MBDNVariation, XPos, YPos);

			if(m_Behavior[MB_RBDN])
				SetVariation(Behavior, m_RBDNVariation, XPos, YPos);

			break;
		}
	}
}

POINT CNtlWorldMouse::GetCusPos()
{
	return m_CurPos;
}

float CNtlWorldMouse::GetXMovementWithRate(float Rate)
{
	return (float)m_LBDNVariation.XNew / Rate;
}

float CNtlWorldMouse::GetYMovementWithRate(float Rate)
{
	return (float)m_LBDNVariation.YNew / Rate;
}

float CNtlWorldMouse::GetWheelMovement()
{
	return m_WheelMovement;
}

bool CNtlWorldMouse::GetBehavior(int Behavior)
{
	return m_Behavior[Behavior];
}

bool CNtlWorldMouse::GetLBOnce()
{
	return m_LBOneClick;
}

bool CNtlWorldMouse::GetLBUp()
{
	return m_LBUpClicked;
}

bool CNtlWorldMouse::GetRBOnce()
{
	return m_RBOneClick;
}

bool CNtlWorldMouse::GetRBUp()
{
	return m_RBUpClicked;
}

bool CNtlWorldMouse::GetLBDC()
{
	return m_LBDC;
}

void CNtlWorldMouse::SetWheelMovement(short zDelta)
{
	m_WheelMovement = (float)zDelta;
}

void CNtlWorldMouse::Reset()
{
	m_Behavior[MB_MOVE] = false;

	m_LBDNVariation.XNew = 0;
	m_LBDNVariation.YNew = 0;
	m_MBDNVariation.XNew = 0;
	m_MBDNVariation.YNew = 0;
	m_RBDNVariation.XNew = 0;
	m_RBDNVariation.YNew = 0;
	m_WheelMovement = 0.0f;

	m_LBOneClick	= false;
	m_LBUpClicked	= false;
	m_RBOneClick	= false;
	m_RBUpClicked	= false;
	m_LBDC			= false;
}
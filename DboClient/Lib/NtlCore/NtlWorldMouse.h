// Mouse.h: interface for the CMouse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOUSE_H__4B564F2A_0663_404A_A624_EB4676519664__INCLUDED_)
#define AFX_MOUSE_H__4B564F2A_0663_404A_A624_EB4676519664__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <WinSock2.h>
#include <Windows.h>


enum MOUSE_BEHAVIOR
{
	MB_LBDN = 0,
	MB_MBDN,
	MB_RBDN,
	MB_MOVE,
	MB_LBDC,
	MB_NUM,
	MB_DEFAULT,
};

struct MOUSE_VARIATION
{
	int XOld, YOld, XNew, YNew, XMove, YMove;
};

class CNtlWorldMouse
{
public:
	CNtlWorldMouse();
	virtual ~CNtlWorldMouse();

private:
	bool m_Behavior[MB_NUM];
	float m_WheelMovement;
	bool m_LBOneClick, m_LBUpClicked;
	bool m_RBOneClick, m_RBUpClicked;
	bool m_LBDC;
	POINT m_PosClicked;
	POINT m_CurPos;

public:
	MOUSE_VARIATION m_LBDNVariation, m_MBDNVariation, m_RBDNVariation;

public:
	const POINT& GetPosClicked() const;
	POINT GetCusPos();
	void Update();
	void SetWheelMovement(short zDelta);
	void SetVariation(int Behavior, MOUSE_VARIATION& Variation, int XPos = 0, int YPos = 0);
	bool GetLBOnce();
	bool GetLBUp();
	bool GetRBOnce();
	bool GetRBUp();
	bool GetLBDC();
	void SetBehavior(int Behavior, int XPos, int YPos, bool Activated = true);
	bool GetBehavior(int Behavior);
	float GetWheelMovement();
	float GetXMovementWithRate(float Rate);
	float GetYMovementWithRate(float Rate);
	void Reset();
};


#endif // !defined(AFX_MOUSE_H__4B564F2A_0663_404A_A624_EB4676519664__INCLUDED_)

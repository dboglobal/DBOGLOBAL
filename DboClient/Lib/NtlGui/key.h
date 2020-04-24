#ifndef __KEY_H__
#define __KEY_H__

#define INVALID_KEY   -1

enum
{
	UD_LEFT_BUTTON,
	UD_RIGHT_BUTTON,
	UD_WHEEL_BUTTON
};

#define UD_MK_LBUTTON	0x0001
#define UD_MK_RBUTTON	0x0002
#define UD_MK_SHIFT		0x0004
#define UD_MK_CONTROL	0x0008
#define UD_MK_MBUTTON	0x0010

class CKey
{
public:
//! Enums:
	enum STATE
	{
		NOKEY,
		PRESSED,
		DBLPRESSED,
		RELEASE
	};

public:
//! Variables:
	//: Key ID.
	int m_nID;

	//: virtual-key code 
	DWORD m_dwVKey;
	
	//: <p>Key state. Was it pressed or released. NoKey is used when there were
	//: no key available in the input buffer.</p>
	STATE m_nState;

	//: x-coordinate position if a pointing device was the source. -1 if none.
	float m_fX;
	
	//: y-coordinate position if a pointing device was the source. -1 if none.
	float m_fY;

//! Construction:
	//: Key Constructor
	CKey()
	{
		m_nID    = INVALID_KEY;
		m_nState = NOKEY;
		m_dwVKey = 0;
		m_fX     = -1;
		m_fY     = -1;
	}

	//: Key Constructor
	CKey(int nID, STATE state,int nVKey, float fX, float lY)
	{
		m_nID    = nID;
		m_nState = state;
		m_dwVKey  = nVKey;
		m_fX     = fX;
		m_fY     = lY;
	}
};

#endif
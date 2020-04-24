#ifndef __MOUSE_H__
#define __MOUSE_H__

class CMouse
{
public:

	static int m_nX,m_nY;

public:

	static int GetX(VOID) { return m_nX; }
	static int GetY(VOID) { return m_nY; }
};

#endif 
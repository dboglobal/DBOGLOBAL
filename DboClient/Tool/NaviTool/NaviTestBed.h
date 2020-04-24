#pragma once

class CNaviTestBed
{
protected:
	CNaviTestBed(void);

public:
	virtual ~CNaviTestBed(void);

	static CNaviTestBed* GetInstance()
	{
		static CNaviTestBed instance;
		return &instance;
	}

	BOOL	Create(HWND hWnd, int nWidth, int nHeight);
	void	Destroy();
	
	void	Update();
	void	Update(float fTime, float fElapsedTime);
	
	void	Render();

	void	TestBedMsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:
	float			m_fTime;
	float			m_fElapsedTime;
};

static CNaviTestBed* GetNaviTestBed()
{
	return CNaviTestBed::GetInstance();
}

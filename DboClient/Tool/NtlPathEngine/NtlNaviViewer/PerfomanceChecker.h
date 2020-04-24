#pragma once

class CPerformanceChecker
{
public:
	CPerformanceChecker();
	virtual ~CPerformanceChecker();

	void	Run();
	void	Stop();
	void	Print(char* msg = NULL);

	double			GetLastTime()		{ return m_fLastTime; }
	double			GetAverageTime()	{ return m_fAverageTime; }
	unsigned int	GetCallCount()		{ return m_iCallCount; }


protected:
	double	GetTime();

protected:
	BOOL			m_bQuery;
	LARGE_INTEGER	m_liFreq;
	LARGE_INTEGER	m_liCount;

	double			m_fStartTime;
	double			m_fEndTime;
	double			m_fAverageTime;
	double			m_fLastTime; 
	unsigned int	m_iCallCount;
};
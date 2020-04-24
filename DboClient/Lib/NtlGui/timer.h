#ifndef __TIMER_H__
#define __TIMER_H__

#include "gui_signals.h"

//: Timer signal emitter.
//- Use this class to emit a signal as a timer event.
class CTimer 
{
//! Construction:
public:
	//: Constructs and registers the timer as a keep_alive listener.
	//: Note that it is disabled by default, so you need to enable() it.
	//- interval - Interval in millisecs between each signal emit.
	CTimer(unsigned long interval = 1000);
	
//! Attributes:
public:
	//: Returns the current time interval.
	//- Returns - The current time interval.
	unsigned long GetInterval(VOID);

	//: Returns true if the timer is enabled, false otherwise.
	bool IsEnabled(VOID);

//! Operations:
public:
	//: Sets a new interval for the timer.
	//- interval - Interval in millisecs between each signal emit.
	VOID SetInterval(unsigned long lInterval);

	//: Call this function to enable the timer.
	VOID Enable(VOID);
	
	//: Call this function to disable the timer.
	VOID Disable(VOID);
	
//! Signals:
public:
	//: Signal emitted everytime the interval has elapsed.
	gui::CSignal_v0 m_SigTimer;

	gui::CSignal_v0 SigTimer(VOID) { return m_SigTimer; }
	
//! Implementation:
private:

	virtual VOID KeepAlive();

	long GetTime(VOID);

	unsigned long m_lInterval;
	unsigned long m_lLastTime;
	bool m_bEnabled;
};

#endif

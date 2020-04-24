/*//////////////////////////////////////////////////////////////*\
**||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||**|
**---------------------------------------------------------------/
** Zenerd Standard Library
**  Copyright (C) 2000-2002 by Zenerd
**
** Component: Time Line Data-Structure
**
** Date     : October 2001
**
** Author(s): Jeffrey M. Barber
**
** Notes    : NO BOUND CHECKING
** 
**]=================================================================]
**This software is provided "AS IS," without a warranty of any kind.)
**	IN NO EVENT WILL ZENERD OR ITS LICENSORS BE LIABLE FOR ANY      )
**	LOST REVENUE, PROFIT OR DATA, OR FOR DIRECT, INDIRECT, SPECIAL,/
**	CONSEQUENTIAL, INCIDENTAL OR PUNITIVE DAMAGES,                /
**---------------------------------------------------------------\
**||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||**|
**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef ZENERD_TIMELINE
#define ZENERD_TIMELINE

#include <vector>

using namespace std;
class ZTimeSpan;

class ZTimeLine
{
	private:
		vector<ZTimeSpan> m_ZTimeSpans;
		void swap(int a, int b);
		void qsort(int l = -100, int r = -100);

	public:
		ZTimeLine();
		ZTimeLine(const ZTimeLine & c);
		~ZTimeLine();
		ZTimeLine& operator=(const ZTimeLine & c);

		// Regarding Time Spans
		int		GetSpanCount();
		bool	AddTimeSpan(int Owner, int Action);
		int		GetTimeSpan(int Owner, int Action);

		void	SetAction(int Span, int Action);
		void	SetOwner(int Span, int Owner);

		int		GetAction(int Span);
		int		GetOwner(int Span);
		
		// Regarding Time Slices
		int		GetSliceCount(int Span);
		bool	AddTimeSlice(int Span, int Time, int Length, double Weight);
		int		GetTimeSlice(int Span, int Time);
		int		GetSlice    (int Span, int Time); // why did I do this?, not sure

		// Unified Structure
		void	GetSlice(int Span, int Slice, int & Start, int & Length, double & Weight); // this gets the data
		void	Edit(int Span, int Slice, int Start, int Length, double Weight);
		void	Delete(int Span, int Slice = -1);
		void    DeleteAll( void );
		
		// verification, note: no internal means for collision are used since they may or may not be needed
		int		isCollision(int Span, int Skip, int Start, int Length);// is there a collision between this data and other data

		
};

#endif
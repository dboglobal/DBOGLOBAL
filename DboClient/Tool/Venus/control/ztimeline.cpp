#include "stdafx.h"
#include "ztimeline.h"
class ZTimeSlice
{
	private:
		int m_time_start; // start time
		int m_time_length; // length of action
		double m_action_weight; // what is the action weight, or action details

	private:
		char compare(const ZTimeSlice & c);

	public:
		// basic management details
		ZTimeSlice();
		ZTimeSlice(const ZTimeSlice & c);
		ZTimeSlice(int start, int length, double weight);
		~ZTimeSlice();

		ZTimeSlice& operator = (const ZTimeSlice & c);
		bool operator <  (const ZTimeSlice & c);
		bool operator >  (const ZTimeSlice & c);
		bool operator == (const ZTimeSlice & c);
		bool operator <= (const ZTimeSlice & c);
		bool operator >= (const ZTimeSlice & c);


		bool isIn(int time);

		void SetStart(int start);
		void SetLength(int length);
		void SetWeight(double weight);
		int GetStart();
		int GetLength();
		double GetWeight();

		void Edit(int start, int length, double weight);
		int get_time_end();
};


class ZTimeSpan
{
	private:
		vector<ZTimeSlice> m_splices;
		int m_action;
		int m_owner;
	private:
		void swap(int a, int b);
		void qsort(int l = -100, int r = -100);
		char compare(const ZTimeSpan & c);
	public:
		ZTimeSpan(int owner = 0, int action = 0);
		ZTimeSpan(const ZTimeSpan & c);
		~ZTimeSpan();
		ZTimeSpan& operator = (const ZTimeSpan &c);
		bool operator <  (const ZTimeSpan & c);
		bool operator >  (const ZTimeSpan & c);
		bool operator == (const ZTimeSpan & c);
		bool operator <= (const ZTimeSpan & c);
		bool operator >= (const ZTimeSpan & c);


		// data insertion
		bool AddSplice(int start, int length, double weight);
		void SetOwner(int owner);
		void SetAction(int action);

		int GetOwner();
		int GetAction();
		int	 GetSliceAt(int time);

		void Edit(int slice, int start, int length, double weight);
		void Delete(int slice);

		int GetSliceCount();
		void GetSlice(int Slice, int & Start, int & Length, double & Weight);

};

// CPP
// Constructor
ZTimeSlice::ZTimeSlice()
{
	m_time_start	=	0;
	m_time_length	=	-1;
	m_action_weight =   0.0;
}

// Copy Constructor, mirror of operator = 
//100%
ZTimeSlice::ZTimeSlice(const ZTimeSlice & c)
{
	*this = c;
}

ZTimeSlice::ZTimeSlice(int start, int length, double weight)
{
	m_time_start		= start;
	m_time_length		= length;
	m_action_weight		= weight;
}

// destructor
ZTimeSlice::~ZTimeSlice()
{
	// ZERO THE MEMORY, for Aethistics (SP?)
	m_time_start		= 0;
	m_time_length		= 0;
	m_action_weight		= 0.0;
}

ZTimeSlice& ZTimeSlice::operator = (const ZTimeSlice & c)
{
	m_time_start		= c.m_time_start;
	m_time_length		= c.m_time_length;
	m_action_weight		= c.m_action_weight;

	return *this;
}

char ZTimeSlice::compare(const ZTimeSlice & c)
{
	if(m_time_start < c.m_time_start ) return -1;
	if(m_time_start == c.m_time_start) return  0;
	return 1;
}

int ZTimeSlice::get_time_end()
{
	return m_time_start + m_time_length;
}

bool ZTimeSlice::isIn(int time)
{
	return time >= m_time_start && time <= get_time_end() ? true : false;
}
/////////////////////////////////////////////////////////////////////////////////////////////
ZTimeSpan::ZTimeSpan(int owner, int action)
{
	m_owner = owner;
	m_action = action;
}

ZTimeSpan::~ZTimeSpan()
{
	m_owner = 0;
	m_action = 0;
	m_splices.clear();
}

ZTimeSpan::ZTimeSpan(const ZTimeSpan & c)
{
	*this = c;
}

ZTimeSpan& ZTimeSpan::operator = (const ZTimeSpan &c)
{
	m_action = c.m_action;
	m_owner = c.m_owner;
	m_splices.clear();
	for(size_t i = 0 ; i < c.m_splices.size(); i++)
	{
		m_splices.push_back(c.m_splices[i]);
	}
	return *this;
}
// basic manager for the time span and time splices

bool ZTimeSpan::AddSplice(int start, int length, double weight)
{
	ZTimeSlice Add(start,length,weight);
	m_splices.push_back(Add);
	qsort();
	return true;
}

void ZTimeSpan::swap(int a, int b)
{
	ZTimeSlice T;
	T = m_splices[a];
		m_splices[a] =	m_splices[b];
						m_splices[b] = T;
}

void ZTimeSpan::qsort(int l, int r)
{
	if(l==-100&&r==-100)
	{
		qsort(0, m_splices.size()-1);
	}
	else
	{
		int i, j;
		if(r > l)
		{
			ZTimeSlice v;
			v = m_splices[r];
			i = l - 1;
			j = r;
			
			for(;;)
			{
				while(m_splices[++i] < v) ;
				while(m_splices[--j] > v) ;
				if(i>=j) break;
				swap(i,j);
			}
			swap(i,r);
			qsort(l,i-1);
			qsort(i+1, r);

		}
	}
}

char ZTimeSpan::compare(const ZTimeSpan &c)
{
	if(m_owner < c.m_owner) return -1;
	if(m_owner > c.m_owner) return 1;
	// since owner is equal
	if(m_action < c.m_action) return -1;
	if(m_action > c.m_action) return 1;
	// actions are equal
	return 0;
}


int ZTimeSpan::GetSliceAt(int time)
{
	for(size_t i = 0; i < m_splices.size() ; i++)
	{
		if(m_splices[i].isIn(time)==true) return i;
	}
	return -1;
}

void ZTimeSpan::Delete(int slice)
{
	for(size_t i = slice; i < m_splices.size() - 1 ; i++)
	{
		m_splices[i] = m_splices[i+1];
	}
	m_splices.pop_back();
}
/////////////////////////////////////////////////////////////////////////////////////////////
ZTimeLine::ZTimeLine()
{
}

ZTimeLine::~ZTimeLine()
{
	m_ZTimeSpans.clear();
}

ZTimeLine::ZTimeLine(const ZTimeLine & c)
{
	*this = c;
}

ZTimeLine& ZTimeLine::operator = (const ZTimeLine & c)
{
	for(size_t i=0;i<c.m_ZTimeSpans.size();i++)
	{
		m_ZTimeSpans.push_back(c.m_ZTimeSpans[i]);
	}
	return *this;
}
void ZTimeLine::swap(int a, int b)
{
	ZTimeSpan T;
		      T = m_ZTimeSpans[a];
			      m_ZTimeSpans[a] = m_ZTimeSpans[b];
								   m_ZTimeSpans[b] = T;
}
void ZTimeLine::qsort(int l, int r)
{
	if(l==-100&&r==-100)
	{
		qsort(0, m_ZTimeSpans.size()-1);
	}
	else
	{
		int i, j;
		if(r > l)
		{
			ZTimeSpan v;
			v = m_ZTimeSpans[r];
			i = l - 1;
			j = r;
			
			for(;;)
			{
				while(m_ZTimeSpans[++i] < v) ;
				while(m_ZTimeSpans[--j] > v) ;
				if(i>=j) break;
				swap(i,j);
			}
			swap(i,r);
			qsort(l,i-1);
			qsort(i+1, r);

		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool ZTimeLine::AddTimeSpan(int Owner, int Action)
{
	ZTimeSpan Temp;
	Temp.SetOwner(Owner);
	Temp.SetAction(Action);
	m_ZTimeSpans.push_back(Temp);
	qsort();
	return true;
}

int ZTimeLine::GetTimeSpan(int Owner, int Action)
{
	int jump;
	jump = m_ZTimeSpans.size() - 1;
	int pos = 0;
	ZTimeSpan Test(Owner, Action);
	while(jump > 0)
	{
		// return conditions
		if(m_ZTimeSpans[pos] == Test)
			return pos;
		if(m_ZTimeSpans[pos+jump] == Test)
			return pos + jump;
		
		// jumping
		if(m_ZTimeSpans[pos+jump] > Test)
		{
			jump /=2;
		}

		if(m_ZTimeSpans[pos+jump] < Test)
		{
			pos+=jump;
		}

		// failure conditions
		if(pos==m_ZTimeSpans.size()) return -1;
		while(pos+jump >= (int)m_ZTimeSpans.size()) jump/=2;
	}
	if(m_ZTimeSpans[pos] == Test)
		return pos;
	return -1;
}

bool ZTimeLine::AddTimeSlice(int Span, int Time, int Length, double Weight)
{
	return m_ZTimeSpans[Span].AddSplice(Time, Length, Weight);
}

int ZTimeLine::GetTimeSlice(int Span, int Time)
{
	return m_ZTimeSpans[Span].GetSliceAt(Time);
}
void ZTimeLine::Edit(int Span, int Slice, int Start, int Length, double Weight)
{
	m_ZTimeSpans[Span].Edit(Slice,Start,Length,Weight);
}
void ZTimeLine::Delete(int Span, int Slice)
{
	if(Slice>=0)
	{
		// delete a slice of a span
		m_ZTimeSpans[Span].Delete(Slice);
	}
	else
	{
		// delete a span
		for(size_t i = Span; i < m_ZTimeSpans.size() - 1 ; i++)
		{
			m_ZTimeSpans[i] = m_ZTimeSpans[i+1];
		}
		m_ZTimeSpans.pop_back();
	}
}

void ZTimeLine::DeleteAll( void )
{
	m_ZTimeSpans.clear();
}

bool	ZTimeSlice::operator <  (const ZTimeSlice & c) { return compare(c)< 0; } 
bool	ZTimeSlice::operator >  (const ZTimeSlice & c) { return compare(c)> 0; } 
bool	ZTimeSlice::operator == (const ZTimeSlice & c) { return compare(c)==0; } 
bool	ZTimeSlice::operator <= (const ZTimeSlice & c) { return compare(c)<=0; } 
bool	ZTimeSlice::operator >= (const ZTimeSlice & c) { return compare(c)>=0; } 
void	ZTimeSlice::SetStart(int start) { m_time_start = start; } 
void	ZTimeSlice::SetLength(int length) { m_time_length = length; }
void	ZTimeSlice::SetWeight(double weight) { m_action_weight = weight; }
int		ZTimeSlice::GetStart() { return m_time_start; }
int		ZTimeSlice::GetLength() { return m_time_length; }
double	ZTimeSlice::GetWeight() { return m_action_weight; }
void	ZTimeSlice::Edit(int start, int length, double weight) { m_time_start = start; m_time_length = length; m_action_weight = weight; }
bool	ZTimeSpan::operator <  (const ZTimeSpan & c) { return compare(c)< 0; } 
bool	ZTimeSpan::operator >  (const ZTimeSpan & c) { return compare(c)> 0; } 
bool	ZTimeSpan::operator == (const ZTimeSpan & c) { return compare(c)==0; } 
bool	ZTimeSpan::operator <= (const ZTimeSpan & c) { return compare(c)<=0; } 
bool	ZTimeSpan::operator >= (const ZTimeSpan & c) { return compare(c)>=0; } 
void	ZTimeSpan::SetOwner(int owner) { m_owner = owner; }
void	ZTimeSpan::SetAction(int action) { m_action = action; }
int		ZTimeSpan::GetOwner() { return m_owner ; }
int		ZTimeSpan::GetAction() { return m_action ; }
void	ZTimeSpan::Edit(int slice, int start, int length, double weight) { m_splices[slice].Edit(start, length, weight); }
int		ZTimeSpan::GetSliceCount() { return m_splices.size(); }
void	ZTimeSpan::GetSlice(int Slice, int & Start, int & Length, double & Weight) {	Start = m_splices[Slice].GetStart();Length = m_splices[Slice].GetLength();Weight = m_splices[Slice].GetWeight();}
int		ZTimeLine::GetSpanCount() { return m_ZTimeSpans.size(); }
int		ZTimeLine::GetSliceCount(int Span) { return m_ZTimeSpans[Span].GetSliceCount(); }
void	ZTimeLine::GetSlice(int Span, int Slice, int & Start, int & Length, double & Weight) { m_ZTimeSpans[Span].GetSlice(Slice,Start,Length, Weight); }
void	ZTimeLine::SetAction(int Span, int Action) { m_ZTimeSpans[Span].SetAction(Action); }
int		ZTimeLine::GetAction(int Span) { return m_ZTimeSpans[Span].GetAction(); }
void	ZTimeLine::SetOwner(int Span, int Owner) { m_ZTimeSpans[Span].SetOwner(Owner); }
int		ZTimeLine::GetOwner(int Span) { return m_ZTimeSpans[Span].GetOwner(); }


bool isIn(int min, int max, int test)
{
	if(min<=test && test<=max) return true;
	return false;
}

int	ZTimeLine::isCollision(int Span, int Skip, int Start, int Length)
{
	int cStart, cLength;
	double w;
	for(int i = 0; i < m_ZTimeSpans[Span].GetSliceCount(); i++)
	{
		if(i!=Skip)
		{
			m_ZTimeSpans[Span].GetSlice(i,cStart, cLength, w);
			if(isIn(cStart,cStart + cLength,  Start)) return i;
			if(isIn(cStart,cStart + cLength,  Start + Length)) return i;
			if(isIn( Start, Start +  Length, cStart)) return i;
			if(isIn( Start, Start +  Length, cStart + cLength)) return i;
		}
	}
	return -1;
}

int ZTimeLine::GetSlice(int Span, int Time)
{
	return m_ZTimeSpans[Span].GetSliceAt(Time);
}
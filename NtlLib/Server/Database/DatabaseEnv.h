#ifndef DATABASEENV_H
#define DATABASEENV_H

//! Our own includes.
#include "Field.h"
#include "Database.h"
#include "QueryResult.h"
#include "AsyncQuery.h"
#include "QueryBuffer.h"


extern Database * db_char;
#define GetCharDB (*db_char)

extern Database * db_acc;
#define GetAccDB (*db_acc)

extern Database * db_log;
#define GetLogDB (*db_log)


template<class T>
class smart_ptr
{
public:
	typedef smart_ptr<T> self_type;

	smart_ptr(T* p) : m_ptr(p), m_owner(true)
	{
	}

	smart_ptr(const self_type& sp) : m_owner(true), m_ptr(sp.m_ptr)
	{
		sp.m_owner = false;
	}

	~smart_ptr()
	{
		if (m_ptr && m_owner)
			delete m_ptr;
	}
	smart_ptr& operator = (T* p)
	{
		m_ptr = p;
		m_owner = true;
		return *this;
	}
	smart_ptr& operator = (const self_type& sp)
	{
		m_ptr = sp.m_ptr;
		sp.m_owner = false;
		m_owner = true;
	}
	const T* operator -> () const
	{
		return (const T*)m_ptr;
	}

	T* operator -> ()
	{
		return (T*)m_ptr;
	}

	const T& operator * () const
	{
		return *m_ptr;
	}
	T& operator *()
	{
		return *m_ptr;
	}
	operator T* ()
	{
		return m_ptr;
	}
	operator const T* () const
	{
		return m_ptr;
	}
	operator bool() const
	{
		return m_ptr != NULL;
	}

private:
	mutable bool m_owner;
	T* m_ptr;
};



#endif
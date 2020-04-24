#pragma once

#include <deque>
#include <algorithm>

template <typename T1, typename T2>
class CNtlPairedKeyMap
{
private:
	struct SPair1
	{
		SPair1(T1 keyValue1, T2 keyValue2) :
				key1(keyValue1),
				key2(keyValue2)
		{
		};

		bool operator<(SPair1& pair)
		{
			return (key1 < pair.key1);
		};

		bool operator==(T1 key)
		{
			return (key1 == key);
		};

		T1 key1;
		T2 key2;
	};

	struct SPair2
	{
		SPair2(T2 keyValue2, T1 keyValue1) :
				key2(keyValue2),
				key1(keyValue1)
		{
		};

		bool operator<(SPair2& pair)
		{
			return (key2 < pair.key2);
		};

		bool operator==(T2 key)
		{
			return (key2 == key);
		};

		T2 key2;
		T1 key1;
	};

public:
	CNtlPairedKeyMap(void);
	virtual ~CNtlPairedKeyMap(void);

	bool Create();
	bool Destroy();

protected:
	bool Init();

public:
	size_t GetNumberOfElements();
	bool GetAt(DWORD dwIndex, T1* key1, T2* key2);

	bool AddElement(T1 key1, T2 key2);
	bool RemoveElementWithKey1(T1 key1);
	bool RemoveElementWithKey2(T2 key2);

	bool DoesKey1Exist(T1 key1);
	bool DoesKey2Exist(T2 key1);

	bool GetKey2WithKey1(T1 key1, T2* key2);
	bool GetKey1WithKey2(T2 key2, T1* key1);

protected:
	CRITICAL_SECTION m_lockForContainer;

	std::deque<SPair1> m_dequeKeyType1;
	std::deque<SPair2> m_dequeKeyType2;
};

template <typename T1, typename T2>
CNtlPairedKeyMap<T1, T2>::CNtlPairedKeyMap(void)
{
	::InitializeCriticalSection(&m_lockForContainer);

	Init();
}

template <typename T1, typename T2>
CNtlPairedKeyMap<T1, T2>::~CNtlPairedKeyMap(void)
{
	Destroy();

	::DeleteCriticalSection(&m_lockForContainer);
}

template <typename T1, typename T2>
bool CNtlPairedKeyMap<T1, T2>::Create()
{
	return true;
}

template <typename T1, typename T2>
bool CNtlPairedKeyMap<T1, T2>::Destroy()
{
	m_dequeKeyType1.clear();
	m_dequeKeyType2.clear();

	return true;
}

template <typename T1, typename T2>
bool CNtlPairedKeyMap<T1, T2>::Init()
{
	m_dequeKeyType1.clear();
	m_dequeKeyType2.clear();

	return true;
}

template <typename T1, typename T2>
size_t CNtlPairedKeyMap<T1, T2>::GetNumberOfElements()
{
	::EnterCriticalSection(&m_lockForContainer);

	size_t qwNumberOfElements = m_dequeKeyType1.size();

	::LeaveCriticalSection(&m_lockForContainer);

	return qwNumberOfElements;
}

template <typename T1, typename T2>
bool CNtlPairedKeyMap<T1, T2>::GetAt(DWORD dwIndex, T1* key1, T2* key2)
{
	::EnterCriticalSection(&m_lockForContainer);

	if (0 == m_dequeKeyType1.size())
	{
		::LeaveCriticalSection(&m_lockForContainer);
		return false;
	}
	if (m_dequeKeyType1.size() <= dwIndex)
	{
		::LeaveCriticalSection(&m_lockForContainer);
		return false;
	}

	SPair1& pair = m_dequeKeyType1.at(dwIndex);

	*key1 = pair.key1;
	*key2 = pair.key2;

	::LeaveCriticalSection(&m_lockForContainer);

	return true;
}

template <typename T1, typename T2>
bool CNtlPairedKeyMap<T1, T2>::AddElement(T1 key1, T2 key2)
{
	::EnterCriticalSection(&m_lockForContainer);

	std::deque<SPair1>::iterator iter1;
	std::deque<SPair2>::iterator iter2;

	iter1 = std::find(m_dequeKeyType1.begin(), m_dequeKeyType1.end(), key1);
	if (m_dequeKeyType1.end() != iter1)
	{
		::LeaveCriticalSection(&m_lockForContainer);
		return false;
	}

	iter2 = std::find(m_dequeKeyType2.begin(), m_dequeKeyType2.end(), key2);
	if (m_dequeKeyType2.end() != iter2)
	{
		::LeaveCriticalSection(&m_lockForContainer);
		return false;
	}

	m_dequeKeyType1.push_back(SPair1(key1, key2));
	m_dequeKeyType2.push_back(SPair2(key2, key1));

	std::sort(m_dequeKeyType1.begin(), m_dequeKeyType1.end());
	std::sort(m_dequeKeyType2.begin(), m_dequeKeyType2.end());

	::LeaveCriticalSection(&m_lockForContainer);

	return true;
}

template <typename T1, typename T2>
bool CNtlPairedKeyMap<T1, T2>::RemoveElementWithKey1(T1 key1)
{
	::EnterCriticalSection(&m_lockForContainer);

	std::deque<SPair1>::iterator iter1;
	std::deque<SPair2>::iterator iter2;

	iter1 = std::find(m_dequeKeyType1.begin(), m_dequeKeyType1.end(), key1);
	if (m_dequeKeyType1.end() == iter1)
	{
		::LeaveCriticalSection(&m_lockForContainer);
		return false;
	}

	iter2 = std::find(m_dequeKeyType2.begin(), m_dequeKeyType2.end(), (*iter1).key2);
	if (m_dequeKeyType2.end() == iter2)
	{
		::LeaveCriticalSection(&m_lockForContainer);
		return false;
	}

	m_dequeKeyType1.erase(iter1);
	m_dequeKeyType2.erase(iter2);

	::LeaveCriticalSection(&m_lockForContainer);

	return true;
}

template <typename T1, typename T2>
bool CNtlPairedKeyMap<T1, T2>::RemoveElementWithKey2(T2 key2)
{
	::EnterCriticalSection(&m_lockForContainer);

	std::deque<SPair1>::iterator iter1;
	std::deque<SPair2>::iterator iter2;

	iter2 = std::find(m_dequeKeyType2.begin(), m_dequeKeyType2.end(), key2);
	if (m_dequeKeyType2.end() == iter2)
	{
		::LeaveCriticalSection(&m_lockForContainer);
		return false;
	}

	iter1 = std::find(m_dequeKeyType1.begin(), m_dequeKeyType1.end(), (*iter2).key1);
	if (m_dequeKeyType1.end() == iter1)
	{
		::LeaveCriticalSection(&m_lockForContainer);
		return false;
	}

	m_dequeKeyType1.erase(iter1);
	m_dequeKeyType2.erase(iter2);

	::LeaveCriticalSection(&m_lockForContainer);

	return true;
}

template <typename T1, typename T2>
bool CNtlPairedKeyMap<T1, T2>::DoesKey1Exist(T1 key)
{
	::EnterCriticalSection(&m_lockForContainer);

	std::deque<SPair1>::iterator iter;

	iter = std::find(m_dequeKeyType1.begin(), m_dequeKeyType1.end(), key);
	if (m_dequeKeyType1.end() == iter)
	{
		::LeaveCriticalSection(&m_lockForContainer);
		return false;
	}
	else
	{
		::LeaveCriticalSection(&m_lockForContainer);
		return true;
	}
}

template <typename T1, typename T2>
bool CNtlPairedKeyMap<T1, T2>::DoesKey2Exist(T2 key)
{
	::EnterCriticalSection(&m_lockForContainer);

	std::deque<SPair2>::iterator iter;

	iter = std::find(m_dequeKeyType2.begin(), m_dequeKeyType2.end(), key);
	if (m_dequeKeyType2.end() == iter)
	{
		::LeaveCriticalSection(&m_lockForContainer);
		return false;
	}
	else
	{
		::LeaveCriticalSection(&m_lockForContainer);
		return true;
	}
}

template <typename T1, typename T2>
bool CNtlPairedKeyMap<T1, T2>::GetKey2WithKey1(T1 key1, T2* key2)
{
	::EnterCriticalSection(&m_lockForContainer);

	std::deque<SPair1>::iterator iter1;

	iter1 = std::find(m_dequeKeyType1.begin(), m_dequeKeyType1.end(), key1);
	if (m_dequeKeyType1.end() == iter1)
	{
		::LeaveCriticalSection(&m_lockForContainer);
		return false;
	}

	*key2 = (*iter1).key2;

	::LeaveCriticalSection(&m_lockForContainer);

	return true;
}

template <typename T1, typename T2>
bool CNtlPairedKeyMap<T1, T2>::GetKey1WithKey2(T2 key2, T1* key1)
{
	::EnterCriticalSection(&m_lockForContainer);

	std::deque<SPair2>::iterator iter2;

	iter2 = std::find(m_dequeKeyType2.begin(), m_dequeKeyType2.end(), key2);
	if (m_dequeKeyType2.end() == iter2)
	{
		::LeaveCriticalSection(&m_lockForContainer);
		return false;
	}

	*key1 = (*iter2).key1;

	::LeaveCriticalSection(&m_lockForContainer);

	return true;
}
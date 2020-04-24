#pragma once

#include <deque>
#include <set>

enum eNTL_AUTH_RESULT
{
	NTL_AUTH_RESULT_SUCCESS = 0,
	NTL_AUTH_RESULT_FAIL,

	NTL_AUTH_RESULT_WRONG_AUTH_KEY_BUFFER,
	NTL_AUTH_RESULT_WRONG_EXTRA_DATA_BUFFER,
	NTL_AUTH_RESULT_NOT_REGISTERED_AUTH_KEY,
	NTL_AUTH_RESULT_FETCHING_EXTRA_DATA_FAIL
};

template <typename KeyType, typename T, DWORD dwAuthKeyLength>
class CNtlAuthenticatorFunc
{
public:
	virtual bool IsToRemove(T extraData) = 0;
};

template <typename KeyType, typename T, DWORD dwAuthKeyLength>
class CNtlAuthenticator
{
private:
	struct sAUTHINFO
	{
		KeyType ownerId;
		DWORD dwTimeRegistered;
		BYTE byAuthKey[dwAuthKeyLength];
		T extraData;
	};

public:
	CNtlAuthenticator(void);
	virtual ~CNtlAuthenticator(void);

	virtual bool Create(DWORD dwValidTermInMillisecs);
	virtual void Destroy();

protected:
	bool Init();

public:
	bool ExpireOldKeys();

	bool AddAuthInfo(KeyType ownerId, void* pvAuthKey, T extraData);
	bool RemoveAuthInfo(KeyType ownerId);
	DWORD VerifyAndRemoveAuthKey(KeyType ownerId, void* pvReceivedAuthKey, T* pStoredExtraData);

	bool GetExtraData(KeyType ownerId, T* pExtraData);

	void RemoveAuthInfoMultiple(CNtlAuthenticatorFunc<KeyType, T, dwAuthKeyLength>* pFuncObject);

protected:
	bool RemoveAuthInfo_Inner(KeyType ownerId);
	bool IsValidAuthKey(KeyType ownerId, void* pvAuthKey);

	bool GetExtraData_Inner(KeyType ownerId, T* pExtraData);
	bool GetAuthKey(KeyType ownerId, void* pvAuthKey);

protected:
	virtual bool ReleaseExtraData(T extraData);
	virtual bool OnExpired(KeyType ownerId);

private:
	CRITICAL_SECTION m_lock;

	DWORD m_dwValidTermInMillisecs;

	std::deque<sAUTHINFO*> m_dequeAuthInfo;
	std::set<KeyType> m_setOwnerId;
};

template <typename KeyType, typename T, DWORD dwAuthKeyLength>
CNtlAuthenticator<KeyType, T, dwAuthKeyLength>::CNtlAuthenticator(void)
{
	::InitializeCriticalSection(&m_lock);

	Init();
}

template <typename KeyType, typename T, DWORD dwAuthKeyLength>
CNtlAuthenticator<KeyType, T, dwAuthKeyLength>::~CNtlAuthenticator(void)
{
	Destroy();

	::DeleteCriticalSection(&m_lock);
}

template <typename KeyType, typename T, DWORD dwAuthKeyLength>
bool CNtlAuthenticator<KeyType, T, dwAuthKeyLength>::Create(DWORD dwValidTermInMillisecs)
{
	m_dwValidTermInMillisecs = dwValidTermInMillisecs;

	return true;
}

template <typename KeyType, typename T, DWORD dwAuthKeyLength>
void CNtlAuthenticator<KeyType, T, dwAuthKeyLength>::Destroy()
{
	::EnterCriticalSection(&m_lock);

	std::deque<sAUTHINFO*>::iterator iter;
	for (iter = m_dequeAuthInfo.begin() ; m_dequeAuthInfo.end() != iter ; iter++)
	{
		sAUTHINFO* pAuthInfo = *iter;
		if (NULL != pAuthInfo)
		{
			ReleaseExtraData(pAuthInfo->extraData);
		}
		delete pAuthInfo;
	}

	m_dequeAuthInfo.clear();
	m_setOwnerId.clear();

	::LeaveCriticalSection(&m_lock);
}

template <typename KeyType, typename T, DWORD dwAuthKeyLength>
bool CNtlAuthenticator<KeyType, T, dwAuthKeyLength>::Init()
{
	m_dwValidTermInMillisecs = 0;

	m_dequeAuthInfo.clear();
	m_setOwnerId.clear();

	return true;
}

template <typename KeyType, typename T, DWORD dwAuthKeyLength>
bool CNtlAuthenticator<KeyType, T, dwAuthKeyLength>::ExpireOldKeys()
{
	::EnterCriticalSection(&m_lock);

	DWORD dwNow = ::GetTickCount();

	while (m_dequeAuthInfo.size() > 0)
	{
		std::deque<sAUTHINFO*>::iterator iter;
		iter = m_dequeAuthInfo.begin();

		sAUTHINFO* pAuthInfo = *iter;
		if (NULL != pAuthInfo)
		{
			if ((dwNow - pAuthInfo->dwTimeRegistered) < m_dwValidTermInMillisecs)
				break;

			KeyType ownerId = pAuthInfo->ownerId;

			m_dequeAuthInfo.pop_front();
			m_setOwnerId.erase(ownerId);

			OnExpired(pAuthInfo->ownerId);

			ReleaseExtraData(pAuthInfo->extraData);
			delete pAuthInfo;
		}
		else
		{
			m_dequeAuthInfo.pop_front();
		}
	}

	::LeaveCriticalSection(&m_lock);
	return true;
}

template <typename KeyType, typename T, DWORD dwAuthKeyLength>
bool CNtlAuthenticator<KeyType, T, dwAuthKeyLength>::AddAuthInfo(KeyType ownerId, void* pvAuthKey, T extraData)
{
	::EnterCriticalSection(&m_lock);

#ifdef _DEVEL
	if (FALSE != IsBadReadPtr(pvAuthKey, dwAuthKeyLength))
#else
	if (NULL == pvAuthKey)
#endif
	{
		::LeaveCriticalSection(&m_lock);
		return false;
	}

	std::set<KeyType>::iterator iterOwnerId;
	iterOwnerId = m_setOwnerId.find(ownerId);

	// Removes the old auth info if found.
	if (m_setOwnerId.end() != iterOwnerId)
	{
		m_setOwnerId.erase(iterOwnerId);

		std::deque<sAUTHINFO*>::iterator iterAuthInfo;
		iterAuthInfo = m_dequeAuthInfo.begin();
		while (m_dequeAuthInfo.end() != iterAuthInfo)
		{
			sAUTHINFO* pAuthInfo = *iterAuthInfo;
			if (NULL != pAuthInfo)
			{
				if (pAuthInfo->ownerId == ownerId)
				{
					m_dequeAuthInfo.erase(iterAuthInfo);

					ReleaseExtraData(pAuthInfo->extraData);

					break;
				}
			}

			iterAuthInfo++;
		}
	}

	// Registers a new auth info.
	sAUTHINFO* pNewAuthInfo = new sAUTHINFO;
	if (NULL == pNewAuthInfo)
	{
		::LeaveCriticalSection(&m_lock);
		return false;
	}

	pNewAuthInfo->ownerId = ownerId;
	pNewAuthInfo->dwTimeRegistered = ::GetTickCount();
	::CopyMemory(pNewAuthInfo->byAuthKey, pvAuthKey, dwAuthKeyLength);
	pNewAuthInfo->extraData = extraData;

	m_dequeAuthInfo.push_back(pNewAuthInfo);
	m_setOwnerId.insert(ownerId);

	::LeaveCriticalSection(&m_lock);

	return true;
}

template <typename KeyType, typename T, DWORD dwAuthKeyLength>
bool CNtlAuthenticator<KeyType, T, dwAuthKeyLength>::RemoveAuthInfo(KeyType ownerId)
{
	::EnterCriticalSection(&m_lock);

	bool bResult = RemoveAuthInfo_Inner(ownerId);

	::LeaveCriticalSection(&m_lock);

	return bResult;
}

template <typename KeyType, typename T, DWORD dwAuthKeyLength>
DWORD CNtlAuthenticator<KeyType, T, dwAuthKeyLength>::VerifyAndRemoveAuthKey(KeyType ownerId, void* pvReceivedAuthKey, T* pStoredExtraData)
{
	::EnterCriticalSection(&m_lock);

#ifdef _DEVEL
	if (FALSE != IsBadReadPtr(pvReceivedAuthKey, dwAuthKeyLength))
#else
	if (NULL == pvReceivedAuthKey)
#endif
	{
		::LeaveCriticalSection(&m_lock);
		return NTL_AUTH_RESULT_WRONG_AUTH_KEY_BUFFER;
	}

#ifdef _DEVEL
	if (NULL != pStoredExtraData)
	{
		if (FALSE != IsBadWritePtr(pStoredExtraData, sizeof(*pStoredExtraData)))
		{
			::LeaveCriticalSection(&m_lock);
			return NTL_AUTH_RESULT_WRONG_EXTRA_DATA_BUFFER;
		}
	}
#endif

	if (false == IsValidAuthKey(ownerId, pvReceivedAuthKey))
	{
		::LeaveCriticalSection(&m_lock);
		return NTL_AUTH_RESULT_NOT_REGISTERED_AUTH_KEY;
	}

	if (NULL != pStoredExtraData)
	{
		if (false == GetExtraData_Inner(ownerId, pStoredExtraData))
		{
			::LeaveCriticalSection(&m_lock);
			return NTL_AUTH_RESULT_FETCHING_EXTRA_DATA_FAIL;
		}
	}

	if (false == RemoveAuthInfo_Inner(ownerId))
	{
		::LeaveCriticalSection(&m_lock);
		return NTL_AUTH_RESULT_FAIL;
	}

	::LeaveCriticalSection(&m_lock);
	return NTL_AUTH_RESULT_SUCCESS;
}

template <typename KeyType, typename T, DWORD dwAuthKeyLength>
bool CNtlAuthenticator<KeyType, T, dwAuthKeyLength>::GetExtraData(KeyType ownerId, T* pExtraData)
{
	::EnterCriticalSection(&m_lock);

	bool bResult = GetExtraData_Inner(ownerId, pExtraData);

	::LeaveCriticalSection(&m_lock);

	return bResult;
}

template <typename KeyType, typename T, DWORD dwAuthKeyLength>
void CNtlAuthenticator<KeyType, T, dwAuthKeyLength>::RemoveAuthInfoMultiple(CNtlAuthenticatorFunc<KeyType, T, dwAuthKeyLength>* pFuncObject)
{
	::EnterCriticalSection(&m_lock);

	std::deque<sAUTHINFO*>::iterator iter;
	for (iter = m_dequeAuthInfo.begin() ; m_dequeAuthInfo.end() != iter ; )
	{
		sAUTHINFO* pAuthInfo = *iter;
		if (NULL == pAuthInfo)
		{
			iter++;
			continue;
		}

		if (false != pFuncObject->IsToRemove(pAuthInfo->extraData))
		{
			iter = m_dequeAuthInfo.erase(iter);
			m_setOwnerId.erase(pAuthInfo->ownerId);

			ReleaseExtraData(pAuthInfo->extraData);
			delete pAuthInfo;
		}
		else
		{
			iter++;
		}
	}

	::LeaveCriticalSection(&m_lock);
}

template <typename KeyType, typename T, DWORD dwAuthKeyLength>
bool CNtlAuthenticator<KeyType, T, dwAuthKeyLength>::RemoveAuthInfo_Inner(KeyType ownerId)
{
	std::set<KeyType>::iterator iterOwnerId;
	iterOwnerId = m_setOwnerId.find(ownerId);
	if (m_setOwnerId.end() == iterOwnerId)
	{
		return false;
	}

	std::deque<sAUTHINFO*>::iterator iterAuthInfo;
	iterAuthInfo = m_dequeAuthInfo.begin();
	while (m_dequeAuthInfo.end() != iterAuthInfo)
	{
		sAUTHINFO* pAuthInfo = *iterAuthInfo;
		if (NULL != pAuthInfo)
		{
			if (pAuthInfo->ownerId == ownerId)
			{
				m_dequeAuthInfo.erase(iterAuthInfo);
				m_setOwnerId.erase(ownerId);

				ReleaseExtraData(pAuthInfo->extraData);
				delete pAuthInfo;

				return true;
			}
		}

		iterAuthInfo++;
	}

	return false;
}

template <typename KeyType, typename T, DWORD dwAuthKeyLength>
bool CNtlAuthenticator<KeyType, T, dwAuthKeyLength>::IsValidAuthKey(KeyType ownerId, void* pvAuthKey)
{
#ifdef _DEVEL
	if (FALSE != IsBadReadPtr(pvAuthKey, dwAuthKeyLength))
#else
	if (NULL == pvAuthKey)
#endif
	{
		return false;
	}

	std::set<KeyType>::iterator iterOwnerId;
	iterOwnerId = m_setOwnerId.find(ownerId);
	if (m_setOwnerId.end() == iterOwnerId)
	{
		return false;
	}

	std::deque<sAUTHINFO*>::iterator iterAuthInfo;
	iterAuthInfo = m_dequeAuthInfo.begin();
	while (m_dequeAuthInfo.end() != iterAuthInfo)
	{
		sAUTHINFO* pAuthInfo = *iterAuthInfo;
		if (NULL != pAuthInfo)
		{
			if (pAuthInfo->ownerId == ownerId)
			{
				if (0 == memcmp(pAuthInfo->byAuthKey, pvAuthKey, dwAuthKeyLength))
				{
					return true;
				}
			}
		}

		iterAuthInfo++;
	}

	return false;
}

template <typename KeyType, typename T, DWORD dwAuthKeyLength>
bool CNtlAuthenticator<KeyType, T, dwAuthKeyLength>::GetExtraData_Inner(KeyType ownerId, T* pExtraData)
{
#ifdef _DEVEL
	if (FALSE != IsBadWritePtr(pExtraData, sizeof(*pExtraData)))
#else
	if (NULL == pExtraData)
#endif
	{
		return false;
	}

	std::deque<sAUTHINFO*>::iterator iterAuthInfo;
	iterAuthInfo = m_dequeAuthInfo.begin();
	while (m_dequeAuthInfo.end() != iterAuthInfo)
	{
		sAUTHINFO* pAuthInfo = *iterAuthInfo;
		if (NULL != pAuthInfo)
		{
			if (pAuthInfo->ownerId == ownerId)
			{
				*pExtraData = pAuthInfo->extraData;

				return true;
			}
		}

		iterAuthInfo++;
	}

	return false;
}

template <typename KeyType, typename T, DWORD dwAuthKeyLength>
bool CNtlAuthenticator<KeyType, T, dwAuthKeyLength>::GetAuthKey(KeyType ownerId, void* pvAuthKey)
{
	::EnterCriticalSection(&m_lock);

#ifdef _DEVEL
	if (FALSE != IsBadWritePtr(pvAuthKey, dwAuthKeyLength))
#else
	if (NULL == pExtraData)
#endif
	{
		::LeaveCriticalSection(&m_lock);
		return false;
	}

	std::deque<sAUTHINFO*>::iterator iterAuthInfo;
	iterAuthInfo = m_dequeAuthInfo.begin();
	while (m_dequeAuthInfo.end() != iterAuthInfo)
	{
		sAUTHINFO* pAuthInfo = *iterAuthInfo;
		if (NULL != pAuthInfo)
		{
			if (pAuthInfo->ownerId == ownerId)
			{
				memcpy( pvAuthKey, pAuthInfo->byAuthKey, dwAuthKeyLength);
				::LeaveCriticalSection(&m_lock);
				return true;
			}
		}

		iterAuthInfo++;
	}

	::LeaveCriticalSection(&m_lock);
	return false;
}

template <typename KeyType, typename T, DWORD dwAuthKeyLength>
bool CNtlAuthenticator<KeyType, T, dwAuthKeyLength>::ReleaseExtraData(T extraData)
{
	UNREFERENCED_PARAMETER( extraData );

	return true;
}

template <typename KeyType, typename T, DWORD dwAuthKeyLength>
bool CNtlAuthenticator<KeyType, T, dwAuthKeyLength>::OnExpired(KeyType ownerId)
{
	UNREFERENCED_PARAMETER( ownerId );

	return true;
}
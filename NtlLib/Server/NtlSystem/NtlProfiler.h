//***********************************************************************************
//
//	File		:	NtlProfiler.h
//
//	Begin		:	2006-07-11
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once


// profile ¼³Á¤½Ã
#define __NTL_FUNCTION_PROFILE__


#ifdef __NTL_FUNCTION_PROFILE__


//////////////////////////////////////////////////////////////////////////
//	ProflieNode
//////////////////////////////////////////////////////////////////////////
class CNtlProfileNode
{
public:

	CNtlProfileNode(const char * lpszNodeName, CNtlProfileNode * pParent);

	~CNtlProfileNode();


public:

	void						Destroy();

	void						Reset();

	void						Call();

	bool						Return();


public:

	CNtlProfileNode *			GetParent() { return m_pParent; }

	CNtlProfileNode *			GetChild() { return m_pChild; }

	CNtlProfileNode *			GetSibling() { return m_pSibling; }

	const char *				GetName() { return m_lpszNodeName; }

	DWORD						GetTotalCall() { return m_dwTotalCalls; }

	float						GetTotalTime() { return m_fTotalTime; }

	CNtlProfileNode *			GetSubNode(const char * lpszNodeName);


protected:

	void						Init();


protected:

	const char *				m_lpszNodeName;

	DWORD						m_dwTotalCalls;

	float						m_fTotalTime;

	DWORD						m_dwRecursionCounter;

	__int64						m_startTime;


	CNtlProfileNode *			m_pParent;

	CNtlProfileNode *			m_pChild;

	CNtlProfileNode *			m_pSibling;
};


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
class CNtlProfileIterator
{
friend class CNtlProfiler;

protected:

	CNtlProfileIterator( CNtlProfileNode * start );

public:

	void						First();

	void						Next();

	bool						IsDone();

	void						EnterChild( int index );

	void						EnterParent( void );


public:

	const char *				GetName() { return m_pCurChild->GetName(); }

	DWORD						GetTotalCall() { return m_pCurChild->GetTotalCall(); }

	float						GetTotalTime() { return m_pCurChild->GetTotalTime(); }


	const char *				GetParentName() { return m_pCurParent->GetName(); }

	DWORD						GetParentTotalCall() { return m_pCurParent->GetTotalCall(); }

	float						GetParentTotalTime() { return m_pCurParent->GetTotalTime(); }


private:

	CNtlProfileNode	*			m_pCurParent;

	CNtlProfileNode *			m_pCurChild;
};


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
class CNtlProfiler
{
public:

	CNtlProfiler();

	~CNtlProfiler();


public:

	int							OpenProfiler(const char * lpszFileName);

	void						CloseProfiler();

	void						Reset();

	void						StartProfile(const char * lpszNodeName);

	void						StopProfile();

	float						GetTimeSinceReset();

	CNtlProfileIterator *		GetIterator() { return new CNtlProfileIterator( &m_root ); }

	void						ReleaseIterator( CNtlProfileIterator * iterator ) { delete iterator; }


public:

	static	CNtlProfiler *		GetInstance();


protected:

	void						Init();


private:

	CNtlProfileNode				m_root;

	CNtlProfileNode *			m_pCurNode;

	__int64						m_resetTime;

	FILE *						m_file;
};


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
class CNtlProfileHelper
{
public:

	CNtlProfileHelper(const char * lpszNodeName)
	{
		CNtlProfiler::GetInstance()->StartProfile( lpszNodeName );
	}

	~CNtlProfileHelper()
	{
		CNtlProfiler::GetInstance()->StopProfile();
	}
};


#endif // __NTL_FUNCTION_PROFILE__



#ifdef __NTL_FUNCTION_PROFILE__

	#define NTL_PROFILE				CNtlProfileHelper __profile_helper__(__FUNCTION__)

#else

	#define NTL_PROFILE
					
#endif






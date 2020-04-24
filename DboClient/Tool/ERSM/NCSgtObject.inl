#if !defined (__NC__SGT_OBJECT__INL)
#define	__NC__SGT_OBJECT__INL



/*************************************************************************************************

template <class T> NCSgtObject

************************************************************************************************/

template <class T> T* NCSgtObject<T>::ms_pObject = NULL;
template <class T> NCSgtObject<T>* NCSgtObject<T>::ms_pThis = NULL;



template <class T> inline
NCSgtObject<T>::NCSgtObject( bool binit /*= true*/ )
{
	assert( !ms_pThis );

	if ( binit )
	{
		assert( !ms_pObject );

		//{	replace
		ms_pObject = new T;
		//}{
		//! Game Programming Gems ÂüÁ¶.
		//int offset = (int)(T *)1 - (int)(NCSgtObject<T> *)(T *)1;
		//ms_pObject = (T *)((int)this + offset);
		//}
	}

	ms_pThis = this;
}


template <class T> inline
NCSgtObject<T>::~NCSgtObject()
{
	if ( NULL != ms_pObject )
	{
		delete ms_pObject;
		ms_pObject = NULL;
	}

	ms_pThis = NULL;
}


template <class T> inline
void NCSgtObject<T>::Init()
{
	Init( true );	
}

template <class T> inline
void NCSgtObject<T>::Init( bool binit )
{
	if ( NULL == ms_pThis )
	{
		NCSgtObject<T> * p = new NCSgtObject<T>(binit);
		p = NULL;
	}	
}


template <class T> inline
void NCSgtObject<T>::Destroy()
{
	if ( NULL != ms_pThis )
	{
		delete ms_pThis;
	}
}


template <class T> inline
T* NCSgtObject<T>::Get()
{
	return ms_pObject;
}


template <class T> inline
bool NCSgtObject<T>::IsInit()
{
	return (ms_pObject) ? true : false;
}



#endif // __NC__SGT_OBJECT__INL
//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

template<class predicate, class T>
void ListMerge(T& thislist, T& _X, predicate _Pr)
{
	if (&_X != &thislist)
	{
		typename T::iterator _F1 = thislist.begin(), _L1 = thislist.end();
		typename T::iterator _F2 = _X.begin(), _L2 = _X.end();
		while (_F1 != _L1 && _F2 != _L2)
		{
			if (_Pr(*_F2, *_F1))
			{
				typename T::iterator _Mid2 = _F2;
				thislist.splice(_F1, _X, _F2, ++_Mid2);
				_F2 = _Mid2; 
			}
			else
				++_F1;
		}
		if (_F2 != _L2)
			thislist.splice(_L1, _X, _F2, _L2);
	}
}

//template<class tPredicate, class T>
//void ListMerge(T& target, T& to_merge, tPredicate predicate)
//{
//    typedef typename T::iterator iterator;
//    assertD(&to_merge != &target);
//    iterator target_begin = target.begin();
//    iterator target_end = target.end();
//    iterator to_merge_begin = to_merge.begin();
//    iterator to_merge_end = to_merge.end();
//    while(target_begin != target_end && to_merge_begin != to_merge_end)
//    {
//        if(predicate(*to_merge_begin, *target_begin))
//        {
//            iterator i = to_merge_begin;
//            target.splice(target_begin, to_merge, to_merge_begin, ++i);
//            to_merge_begin = i; 
//        }
//        else
//        {
//            ++target_begin;
//        }
//    }
//    if(to_merge_begin != to_merge_end)
//    {
//        target.splice(target_end, to_merge, to_merge_begin, to_merge_end);
//    }
//}

//template<class tPredicate, class T>
//void ListSort(T& to_sort, tPredicate predicate)
//{
//    if(2 <= to_sort.size())
//    {
//        const size_t BUFFER_SIZE = 10;
//        T temp;
//        T buffer[BUFFER_SIZE + 1];
//        size_t position = 0;
//        while (!to_sort.empty())
//        {
//            temp.splice(temp.begin(), to_sort, to_sort.begin());
//            size_t i;
//            for(i = 0; i < position && !buffer[i].empty(); ++i)
//            {
//                ListMerge(buffer[i], temp, predicate);
//                buffer[i].swap(temp);
//            }
//            if(i == BUFFER_SIZE)
//            {
//                ListMerge(buffer[i], temp, predicate);
//            }
//            else
//            {
//                buffer[i].swap(temp);
//                if(i == position)
//                {
//                    ++position;
//                }
//            }
//        }
//        while (0 < position)
//        {
//            ListMerge(to_sort, buffer[--position], predicate); 
//        }
//    }
//}
template<class predicate, class T>
void ListSort(T& thislist, predicate _Pr)
{
	if (2 <= thislist.size())
	{
		const tSigned32 maxN = 15;
		T _X;
		T _A[maxN + 1];
		tSigned32 _N = 0;
		while (!thislist.empty())
		{
			_X.splice(_X.begin(), thislist, thislist.begin());
			tSigned32 _I;
			for (_I = 0; _I < _N && !_A[_I].empty(); ++_I)
			{
				ListMerge(_A[_I],_X, _Pr);
				_A[_I].swap(_X);
			}
			if (_I == maxN)
				ListMerge(_A[_I],_X, _Pr);
			else
			{
				_A[_I].swap(_X);
				if (_I == _N)
				++_N;
			}
		}
		while (0 < _N)
			ListMerge(thislist,_A[--_N], _Pr); 
	}
}

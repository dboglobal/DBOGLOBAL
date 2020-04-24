
class cPointTraits;

#ifdef RANGE_CHECKING

template <class T> class cCheckedPoint3;
typedef cCheckedPoint3<cPointTraits> tPoint3;

#else

template <class T> class cUncheckedPoint3;
typedef cUncheckedPoint3<cPointTraits> tPoint3;

#endif

//float Length(const tPoint3F &vector);
//void SetLength(tPoint3F &vector, float length);
tSigned32 Length(const tPoint3 &vector);

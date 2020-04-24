
#ifndef MACROS_INCLUDED
#define MACROS_INCLUDED

#define CONCATENATE_DIRECT(s1, s2) s1##s2
#define CONCATENATE(s1, s2) CONCATENATE_DIRECT(s1, s2)
#define ANON(str) CONCATENATE(str, __LINE__)

#endif

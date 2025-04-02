#ifndef _F77NAME_H_
#define _F77NAME_H_

#if defined BOZOLECLOWN
#if defined (mips) || defined (__mips)
#    define C910
#if defined (sgi) &&  (defined (__STDC__) || defined (__EXTENSIONS__))
#    define f77name(x) x##_
#else
#    define f77name(x) x/**/_
#endif
#    define wordint int
#    define bytesperword 4
#endif
#if defined (SUN)
#    define f77name(x) x/**/_
#endif
#if defined (__hp9000s800) || defined (__hp9000s700)
#    define HP
#    define f77name(x) x
#endif
#if defined (_FLOAT2)
    error " wrong float mode : FLOAT2"
#endif
#if defined (_FLOAT1)
#    define NEC
#    define f77name(x) x##_
#endif
#if defined (SGI)
#    define f77name(x) x/**/_
#endif
#if defined (IRIX5)
#    define f77name(x) x/**/_
#endif
#if defined (NEC64)
#    define wordint long long
#    define bytesperword 8
#    define wordfloat double
#else
#    define wordfloat float
#    define wordint int
#    define bytesperword 4
#endif
#endif

#endif

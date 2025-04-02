#ifndef _MACROS_H_
#define _MACROS_H_

#if defined (mips) || defined (__mips)
#    define C910
#if defined (sgi) &&  (defined (__STDC__))
#    define f77name(x) x##_
#else
#    define f77name(x) x/**/_
#endif
#    define wordint int
#    define bytesperword 4
#endif
#if defined (CDC4680)
#    define f77name(x) x/**/_
#endif
#if defined (SUN) || defined (SUN4)
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
#if defined (C4680)
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

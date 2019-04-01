/* -------------------------------------------- *
 *                                              *
 *  file      :  MSWAP.H                        *
 *                                              *
 *  author    :  Michel Grenier                 *
 *                                              *
 *  revision  :  V0.0                           *
 *                                              *
 *  status    :  DEVELOPMENT                    *
 *                                              *
 *  language  :  C                              *
 *                                              *
 *  os        :  UNIX, LINUX, WINDOS ?          *
 *                                              *
 *  object    :  THIS FILE CONTAINS ALL THE     *
 *               DEFINITIONS FOR ENDIENNESS     *
 *               CONVERSIONS                    *
 *                                              *
 * -------------------------------------------- */

#ifndef include_MSWAP
#define include_MSWAP
 
 static unsigned long  mswap_ul  = 1;
 static unsigned char *mswap_uc  = (unsigned char *)&mswap_ul;
 static unsigned long *mswap_ptr = &mswap_ul;

#define SWAP_LSBF    1
#define SWAP_MSBF    0

#define SWAP_IS      mswap_uc[0]



#define swap_8(x)    swp_swap8(&(x))
#define swap_8x(x)   swp_swap8(&(x))
#define swap_2(x)    swp_swap2(&(x))
#define swap_4(x)    swp_swap4(&(x))
#define swap_4x(x)   swp_swap4(&(x))

extern void swp_swap8   ( void *ptr );
extern void swp_swap4   ( void *ptr );
extern void swp_swap2   ( void *ptr );


 
#endif /* include_MSWAP */

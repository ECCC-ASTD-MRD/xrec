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

#define swap_2(x)    x = (x & 0xff) <<  8 \
                         | (x >>  8  & 0xff ) 

#define swap_4(x)    x = (x & 0xff)     << 24 \
                         | (x & 0xff00)   <<  8 \
                         | (x & 0xff0000) >>  8 \
                         | (x >> 24 & 0xff ) 

#define swap_4x(x)   { mswap_ptr = (unsigned long *)&x; swap_4(*mswap_ptr);}

#define swap_8x(x)   { mswap_ptr = (unsigned long *)&x; \
                         swap_4(mswap_ptr[0]);          \
                         swap_4(mswap_ptr[1]);          \
                         mswap_ptr[0]^=mswap_ptr[1];      \
                         mswap_ptr[1]^=mswap_ptr[0];      \
                         mswap_ptr[0]^=mswap_ptr[1]; }

#endif /* include_MSWAP */

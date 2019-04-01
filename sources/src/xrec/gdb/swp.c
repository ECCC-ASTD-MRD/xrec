/*
 *
 *  file      :  SWAP.C
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *  
 *  os        :  UNIX, LINUX, WINDOS 95/98 NT
 *
 *  object    :  THIS FILE CONTAINS ALL THE DEFINITIONS
 *               FOR "ENDIENNESS" CONVERSION
 *
 */

 extern void
 swp_swap2  ( void *vptr )
     {
     char *ptr = (char *)vptr;
     char c;

     c      = ptr[1];
     ptr[1] = ptr[0];
     ptr[0] = c;
     }    

 extern void
 swp_swap4  ( void *vptr )
     {
     char *ptr = (char *)vptr; 
     char c;

     c      = ptr[0];
     ptr[0] = ptr[3];
     ptr[3] = c;

     c      = ptr[1];
     ptr[1] = ptr[2];
     ptr[2] = c;
     }    

 extern void
 swp_swap8  ( void *vptr )
     {
     char *ptr = (char *)vptr;
     char c;

     c      = ptr[0];
     ptr[0] = ptr[7];
     ptr[7] = c;

     c      = ptr[1];
     ptr[1] = ptr[6];
     ptr[6] = c;

     c      = ptr[2];
     ptr[2] = ptr[5];
     ptr[5] = c;

     c      = ptr[3];
     ptr[3] = ptr[4];
     ptr[4] = c;
     }    

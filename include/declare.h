/* -------------------------------------------- *
 *                                              *
 *  file      :  DECLARE.H                      *
 *                                              *
 *  author    :  Michel Grenier                 *
 *                                              *
 *  revision  :  V0.0                           *
 *                                              *
 *  status    :  DEVELOPMENT                    *
 *                                              *
 *  language  :  C                              *
 *                                              *
 *  os        :  UNIX, LINUX, WINDOS 95/98 NT   *
 *                                              *
 *  object    :  THIS FILE CONTAINS ALL THE     *
 *               DEFINITIONS NEEDED TO DECLARE  *
 *               C MODULES INTO C++             *
 *                                              *
 * -------------------------------------------- */

#ifndef   include_DECLARE
#define   include_DECLARE

#ifndef   __BEGIN_DECLS

#    ifdef    __cplusplus
#      define __BEGIN_DECLS extern "C" {
#      define __END_DECLS              }
#    else
#      define __BEGIN_DECLS /* -vide- */
#      define __END_DECLS   /* -vide- */
#    endif /* __cplusplus  */

#endif /* __BEGIN_DECLS */

#endif /* include_DECLARE */

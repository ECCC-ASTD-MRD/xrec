/*
 *
 *  file      :  SOCK.C
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  object    :  THIS FILE CONTAINS MY IMPLEMENTATION OF SOCKET USE...
 *               HERE IS THE IDEA. THE SERVER IS A DUMMY THAT WAITS FOR
 *               A CLIENT TO OPEN A CONNECTION. EACH CLIENT ARE FORK INTO
 *               A CHILD PROCESS. THE CLIENT TELLS THE SERVER TO LOAD
 *               DYNAMICALY THE PROTOCOL IT WANTS TO USE... THE COMMUNUCATION
 *               STARTS.  THE SERVER TAKES CARE OF THE ENDIANNESS OF THE
 *               EXCHANGE MEANING THAT IT IS ON HIS PART TO CONVERT TO THE
 *               CLIENT NEEDS... CLIENTS ARE KINGS SERVED BY THE SERVER.
 *
 *               THE MODULES IMPLEMENTED ARE ALIKE STANDARD IO FUNCTIONS
 *               MAKING THEM MORE APEALING TO USE :
 *               soc = sopen ("hostname:port","s" )  "s" for server
 *               soc = sopen ("hostname:port","c" )  "c" for client
 *               n   = sread ( soc, buf, nbyte );
 *               n   = swrite( soc, buf, nbyte );
 *
 *               TO ASSUME THE ENDIANNESS CORRECTION THE FIRST
 *               WORD EXCHANGE IS THE ENDIANNESS...OF THE CLIENT.
 *               THEN THE ONLY PROTOCOL ENTRY THAT THE SERVER
 *               KNOWS IS SEND TO THE SERVER IT IS :
 *               "load shared_library_path protocol_module_name"
 *      
 *               THE SERVER LAUNCH THAT PROTOCOL MODULE IN A CHILD PROCESS...
 *               THE COMMUNICATION GOES ON UNTIL THE MODULE RETURNS.
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netdb.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "dlb.h"
#include "sock.h"
#include "swp.h"



/*
 *  dynamic library handle
 */
 static void *handle = NULL;

/*
 *  dynamic parser type and pointer 
 */

typedef int (*Protocol)(int);

 static Protocol parser = (Protocol) NULL;

/*
 *  server need to swap for endianness : no 0 yes 1
 */

 static int swap_needed =  0;


/*
 *
 *  module    :  SCLOSE
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  call      :  sclose(sock);
 *
 *  object    :  THIS MODULE CLOSES A SOCKET.
 *
 */

 extern void
 sclose( int sock )
    {
    close(sock);
    }


/*
 *
 *  module    :  SHANDLER
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  call      :  automatic when a signal has to be process
 *
 *  object    :  THIS MODULE JUST GIVE THE TIME TO THE CHILD
 *               TO SELF DESTROY AVOIDING zoomby OR defunct
 */

void
shandler(int sig)
    {
/*
#ifdef HP
    unsigned long *t = swap_ptr; * useless compiler warning shut up *
#endif
*/
    if( sig == SIGCHLD )
      {
      wait(0);
      signal ( SIGCHLD, shandler );
      }
    }


/*
 *
 *  module    :  SINIT
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  call      :  int i;
 *               int sock;
 *               i = sinit(sock);
 *
 *  object    :  THIS MODULE INITIALIZE THE SERVER PART.
 *               IT READS THE ENDIANNESS STATE OF CLIENT
 *               THAN INTERPRET THE load INSTRUCTION FOR
 *               THE LOADING OF THE SHARED LIBRARY PROTOCOL
 *               MODULE. IT RETURNS 0 IF EVERYTHING WENT OK.
 *               IT EXITS ON ANY ERRORS BECAUSE WE WERE LAUNCH
 *               IN A FORK...
 */

 static int
 sinit( int comm )
    {
    char  buf[1024];
    char *ptr1;
    char *ptr2;
    char *ptr3;
    int   i, siz;
    unsigned char CLIENT_IS;
    char  *sharedir;

    static char key      [16] = "load";
    static char path    [128];

/*
 *  read endianness of client
 */
    i = sread( comm, (char *)&CLIENT_IS, 1 );
    if( i < 0 )
      {
      printf(" error sinit : no endianness\n");
      exit(i);
      }
    if( SWAP_IS != CLIENT_IS ) swap_needed = 1;

/*
 *  read in the command load
 */
    i = sread( comm, (char *)&siz, 4 );
    if( i < 0 )
      {
      printf(" error sinit : no load size\n");
      exit(i);
      }
    if( swap_needed ) swap_4(siz);

    i = sread( comm, buf, siz );
    if( i < 0 )
      {
      printf(" error sinit : no load command\n");
      exit(i);
      }

/*
 *  validate the load command
 */
    ptr1 = strtok( buf,  " " );
    ptr2 = strtok( NULL, " " );
    ptr3 = strtok( NULL, " " );

/*
 *  first word should be load
 */
    if( strcmp(ptr1,key) != 0 )
      {
      printf(" error sinit : load not found\n");
      exit(-1);
      }

/*
 *  second word is a dynamic library name
 */
    sharedir = getenv( "LD_LIBRARY_PATH" );
    if (sharedir)
       strcpy(path,sharedir);
    else
       strcpy( path, "./" );
    strcat(path,ptr2);
    handle = dlb_library( path );
    if( handle == NULL )
      {
      printf(" error sinit : no dynamic library\n");
      exit(-1);
      }

/*
 *  third word is a module name for
 *  the server loop parser
 */
    parser = (Protocol) dlb_module(handle,ptr3);
    if( parser == NULL )
      {
      printf(" error sinit : no parser module\n");
      exit(-1);
      }

    return(0);
    }


/*
 *
 *  module    :  SOPEN
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  call      :  int sock;
 *               char *host_port;
 *               char *mode;
 *               sock = sopen(host_port,mode);
 *
 *  object    :  THIS MODULE OPENS A SERVER OR A CLIENT DEPENDING
 *               IF THE MODE IS "s" OR "c" RESPECTIVELY.
 *               IF IT IS THE SERVER THE sinit FUNCTION IS CALLED,
 *               THE SHARED LIBRARY PROTOCOL MODULE IS LOADED AND
 *               A CHILD PROCESS IS LAUNCHED EXECUTING THAT INTERPRETER.
 *               IF IT IS A CLIENT, THE COMMUNUCATION IS ESTABLISHED AND
 *               THE SOCKET IS RETURNED.
 *               
 */

 extern int
 sopen( char *ihostport, char *mode )
    {
    char  hostport[64];
    char *ptr;

    int   comm;
    int   i;
    int   opt;
    int   sock;
    int   siz;

    short port;

    struct sockaddr_in saddr;
    struct hostent    *hostent;
    struct in_addr     addr;  

/*
 *  validate of mode =  s server
 *                      c client
 */
    if( mode == NULL     ) return(-1);
    if( strlen(mode) > 1 ) return(-1);
    if( mode[0] != 's' &&
        mode[0] != 'c'   ) return(-1);

/*
 *  validation of hostport
 */
    if( ihostport == NULL ) return(-1);
    strcpy(hostport,ihostport);

    ptr = strchr( hostport, ':' );
    if( ptr == NULL ) return(-1);
    ptr[0] = '\0';

/*
 *  convert port (invalid strings result in port = 0)
 */
    port = (short) atoi( &ptr[1] );

/*
 *  default host is current machine
 */
    addr.s_addr = INADDR_ANY;

/*
 *  resolve host only for client
 *  host by address xxx.xxx.xx.xxx
 */
    if( mode[0] == 'c' && hostport[0] != '\0' )
      {
      addr.s_addr = inet_addr(hostport);

/*
 *  host by name
 */
      if( addr.s_addr == 0xFFFFFFFF )
        {
        hostent = gethostbyname(hostport);
        if( hostent != NULL )
          memcpy( &addr, hostent->h_addr_list[0], hostent->h_length);

/*
 *  invalid host 
 */
        else
          return(-1);
        }
      }

/*
 *  create the socket
 */
    sock = socket(AF_INET,SOCK_STREAM,0);
    if( sock < 0 ) return(-1);

/*
 *  set socket options reuse and transmition size of 64Kb
 */
    opt = 1;
    siz = sizeof(int);
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR, &opt, siz );
/*
    opt = 65536;
    setsockopt(sock,SOL_SOCKET,SO_SNDBUF,    &opt, siz );
    setsockopt(sock,SOL_SOCKET,SO_RCVBUF,    &opt, siz );
*/

/*
 *  close-on-exec flag : the socket will not get inherited by child processes.
    fcntl(sock, F_SETFD, FD_CLOEXEC);
 */

/*
 *  socket address
 */
    saddr.sin_family      = AF_INET;
    saddr.sin_port        = htons(port);
    saddr.sin_addr.s_addr = addr.s_addr;
    siz = sizeof(saddr);

/*
 *  client connection
 */ 
    if( mode[0] == 'c' )
      {
#ifdef SOCKADDR
      i= connect(sock, (struct sockaddr *)&saddr, siz );
#else
      i= connect(sock, &saddr, siz );
#endif
      if( i < 0 ){ close(sock); return(-1); }
      return(sock);
      }

/*
 *  server open
 */
#ifdef SOCKADDR
    i = bind(sock,(struct sockaddr *)&saddr, siz );
#else
    i = bind(sock, &saddr, siz );
#endif
    if( i < 0 ){ close(sock); return(-1); }
    i = listen(sock, 100);

/*
 *  wait for connection to come
 */
    while(1)
         {
#ifdef SOCKADDR
         comm = accept(sock, (struct sockaddr *)&saddr, (socklen_t *) &siz );
#else
         comm = accept(sock, &saddr, &siz );
#endif
         if( comm < 0 ) continue;
/*
 *  signal module avoiding zoombies or defunct
 */
         signal ( SIGCHLD, shandler );

/*
 *  launch child communicator
 */
         if( fork() == 0 )
           {
           close(sock);
           i = sinit( comm );
           if( i < 0 ) { puts("Error no parser !\n"); exit(0); }
           i = parser(comm);
           if( i < 0 ) { puts("Communication broken !\n"); exit(0); }
           dlb_close(handle);
           exit(0);
           }
         close(comm);
         }
    }


/*
 *
 *  module    :  SREAD
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  call      :  size_t n;
 *               int    sock;
 *               char   *buf; 
 *               size_t nbyte;
 *               n = sread(sock,buf,nbyte);
 *
 *  object    :  THIS MODULE READS ON THE SOCKET nbyte INTO buf.
 *               THE NUMBER OF BYTE READ IS RETURNED FOR ERROR TESTING
 *               
 */

 extern int
 sread( int sock, char *buf, size_t nbyte)
    {
    int ret;
    int size = 0;

    while( size < nbyte )
         {
         ret = read(sock,&buf[size],nbyte-size);
         if( ret < 0 ) { puts("Read interrupted !\n"); return(ret); }
         size += ret;
         }

    return nbyte;
    }


/*
 *
 *  module    :  SWRITE
 *
 *  author    :  Michel Grenier
 *
 *  revision  :  V0.0
 *
 *  status    :  DEVELOPMENT
 *
 *  language  :  C
 *
 *  call      :  size_t n;
 *               int    sock;
 *               char   *buf; 
 *               size_t nbyte;
 *               n = swrite(sock,buf,nbyte);
 *
 *  object    :  THIS MODULE WRITES ON THE SOCKET nbyte FROM buf.
 *               THE NUMBER OF BYTE WRITTEN IS RETURNED FOR ERROR TESTING
 *               
 */

 extern int
 swrite( int sock, char *buf, size_t nbyte)
    {
    int ret;
    int size = 0;

    while( size < nbyte )
         {
         ret = write(sock,&buf[size],nbyte-size);
         if( ret < 0 ) { puts("Write interrupted !\n"); return(ret); }
         size += ret;
         }

    return nbyte;
    }

#include <stdio.h>
#include <string.h>

#include <gdb.h>

 extern void
 print_ligne(int type, int n,
             float lat0, float lon0, float lat1, float lon1,
             float *ll )
  {
  int i;

  for(i=0;i<n;++i) printf("%i (%f %f)\n",i,*ll++,*ll++);
  }

main()
  {
  gdb_init();

  gdb_limit(45.0, -74.0, 46.0, -73.0 );

  printf("Resolution 1  pix par degres\n");
  gdb_line(1,GDB_LIN_COAST,print_ligne);

  printf("Resolution 2  pix par degres\n");
  gdb_line(2,GDB_LIN_COAST,print_ligne);

  printf("Resolution 4  pix par degres\n");
  gdb_line(4,GDB_LIN_COAST,print_ligne);

  printf("Resolution 8  pix par degres\n");
  gdb_line(8,GDB_LIN_COAST,print_ligne);

  printf("Resolution 16  pix par degres\n");
  gdb_line(16,GDB_LIN_COAST,print_ligne);

  printf("Resolution 32  pix par degres\n");
  gdb_line(32,GDB_LIN_COAST,print_ligne);

  printf("Resolution 64  pix par degres\n");
  gdb_line(64,GDB_LIN_COAST,print_ligne);

  printf("Resolution 128 pix par degres\n");
  gdb_line(128,GDB_LIN_COAST,print_ligne);
  }



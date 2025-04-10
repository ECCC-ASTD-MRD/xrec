#ifndef _SOURIS_H_
#define _SOURIS_H_

#define CLIC            1
#define DRAG            2
#define MOVE            3
#define RESET           4
#define BACK_CLIC       5

int f77name(souris)(int *bouton, int *event, int *x0, int *y0, int *xsize, int *ysize, int *x1, int *y1, int *x2, int *y2, char menuTable[], int *nbmenus, int menulength);

#endif

#ifndef _GD_IO_H_
#define _GD_IO_H_

#include <stdio.h>
 
typedef struct gdIOCtx {
	int	(*getC)(struct gdIOCtx*);
	int	(*getBuf)(struct gdIOCtx*, void*, int);

        void     (*putC)(struct gdIOCtx*, int);
	int	(*putBuf)(struct gdIOCtx*, const void*, int);

	int	(*seek)(struct gdIOCtx*, const int);
	long	(*tell)(struct gdIOCtx*);

	void    (*free)(struct gdIOCtx*);

} gdIOCtx;

typedef struct gdIOCtx	*gdIOCtxPtr;

void Putword(int w, gdIOCtx *ctx);
void Putchar(int c, gdIOCtx *ctx);

void gdPutC(const unsigned char c, gdIOCtx *ctx);
int gdPutBuf(const void *, int, gdIOCtx*);
void gdPutWord(int w, gdIOCtx *ctx);
void gdPutInt(int w, gdIOCtx *ctx);

int gdGetC(gdIOCtx *ctx);
int gdGetBuf(void *, int, gdIOCtx*);
int gdGetByte(int *result, gdIOCtx *ctx);
int gdGetWord(int *result, gdIOCtx *ctx);
int gdGetInt(int *result, gdIOCtx *ctx);

int gdSeek(gdIOCtx *ctx, const int);
long gdTell(gdIOCtx *ctx);

#endif

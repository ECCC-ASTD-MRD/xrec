#ifndef _WGL
#define BLANC    0
#define NOIR     1
#define ROUGE    2
#define CYAN     3
#define JAUNE    4
#define MAGNTA   5
#define VERT     6
#define BLEU     7
#define GRIS     8

#define NIL      0
#define BGAUCH   1
#define BMLIEU   2
#define BDROIT   3
#define BTOUS    4

#define X        0
#define Y        1

extern float c_wglwsx();
extern float c_wglasx();
extern float c_wgldsx();

typedef struct 
{
   int idebut, jdebut, ifin, jfin;
   int mode;
   float xdebut, ydebut, xfin, yfin;
   float densiteX, densiteY;
   float *axex;
   float *axey;
   int   ni;
   int   nj;
   } UserSpaceInfo;

typedef struct 
{
  short x, y;
} wgl_point;

typedef struct 
{
  short x, y;
  unsigned short width, height;
} wgl_rectangle;

typedef struct {
        unsigned long pixel;
        unsigned short red, green, blue;
        char flags;  
        char pad;
} wgl_color;

float aspectRatio;
int wglForceAspect;

typedef struct
{
  int graphics_library;
  int (*wglafi)(int i, int j, int rayon, float angleDebut, float angleFin);
  int (*wglali)(int i, int j, int rayon, float angleDebut, float angleFin);
  int (*wglbbf)();
  int (*wglbrb)();
  int (*wglbti)();
  int (*wglbtn)();
  int (*wglbtpi)();
  int (*wglcfi)();
  int (*wglchngfs)();
  int (*wglcli)();
  int (*wglclr)();
  int (*wglclw)();
  int (*wglcmi)();
  int (*wglcol)();
  int (*wglcolf)();
  int (*wglcrb)();
  int (*wgldbf)();
  int (*wgldbg)();
  int (*wgldcm)();
  int (*wgldeflst)();
  int (*wgldefncw)();
  int (*wgldld)();
  int (*wgldpt)();
  int (*wgldri)();
  int (*wglerb)();
  int (*wglfbf)();
  int (*wglfsc)();
  int (*wglfsh)();
  int (*wglfshlb)();
  int (*wglgacw)();
  int (*wglgaf)();
  int (*wglgbf)();
  int (*wglgci)();
  int (*wglgco)();
  int (*wglgdbg)();
  int (*wglgetcmap)();
  int (*wglgld)();
  int (*wglglpti)();
  int (*wglglw)();
  int (*wglgmk)();
  int (*wglgmod)();
  int (*wglgpl)();
  int (*wglgpt)();
  int (*wglgsi)();
  int (*wglgsp)();
  int (*wglgvi)();
  int (*wglgvp)();
  int (*wglgwz)();
  int (*wglgzi)();
  int (*wglgzp)();
  int (*wglias)();
  int (*wgliax)();
  int (*wglinicmap)();
  int (*wglinids)();
  int (*wglinipat)();
  int (*wglinit)();
  int (*wgliniwin)();
  int (*wgliniwpx)();
  int (*wglinstcmap)();
  int (*wglinvpat)();
  int (*wglkas)();
  int (*wgllpt)();
  int (*wgllwi)();
  int (*wglmapc)();
  int (*wglmapcs)();
  int (*wglmco)();
  int (*wglmcos)();
  int (*wglmcw)();
  int (*wglmesagr)();
  int (*wglmvi)();
  int (*wglncl)();
  int (*wglnewwin)();
  int (*wglopmw)();
  int (*wglopw)();
  int (*wglpfi)();
  int (*wglpli)();
  int (*wglppo)();
  int (*wglpsz)();
  int (*wglpti)();
  int (*wglptis)();
  int (*wglrfi)();
  int (*wglrli)();
  int (*wglroc)();
  int (*wglrstdpar)();
  int (*wglrwc)();
  int (*wglsavpfc)();
  int (*wglsbf)();
  int (*wglsetw)();
  int (*wglsld)();
  int (*wglsmk)();
  int (*wglspt)();
  int (*wglssp)();
  int (*wglstu)();
  int (*wglswb)();
  int (*wglxai)();
} _wglContexte;

extern _wglContexte wglc_x, wglc_gd, wglc_gl, wglc_ps, wglc_wgl;

#endif _WGL
#define _WGL 1

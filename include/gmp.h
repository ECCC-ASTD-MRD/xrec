#define  RAYON_DE_LA_TERRE	6371000.0
#define  RADIAN_A_DEGRE		57.29577951
#define  DEGRE_A_RADIAN		0.01745329252
#define  M_ZERO			1.866025404
#define  G			9.81

#define  LIGNE                  0
#define  TIRET                  1
#define  POINT                  2


#define  GLOBAL                 0
#define  NORD                   1
#define  SUD                    2

#define  OUI                    1
#define  NON                    0

#define  DEDANS                 1
#define  DEHORS                 0

#define  PAREIL                 1
#define  PAS_PAREIL             0

#define  RMN_GEO                0
#define  GDB_GEO                1

#define  ROUND(x) (int)(x+0.5)
#define  INT(x)   (int)(x)
#ifndef FALSE
#define  TRUE 1
#define  FALSE 0
#endif

#define  GEOGRLR                "/data/GEOGRLR"
#define  GEOGRHR                "/data/GEOGRHR"
#define  GEOPOLR                "/data/GEOPOLR"
#define  GEOPOHR                "/data/GEOPOHR"
#define  ZZZLALO                "/data/ZZZLALO"
#define  CODE(x,y,xmin,ymin,xmax,ymax) ((x<xmin)<<3 | (x>xmax)<<2 | (y<ymin)<<1 | (y>ymax))

typedef struct 
{
  char  type;
  char  typeref;
  int gdid;
  int   ni, nj;
  int   ig1, ig2, ig3, ig4;
  float xg1, xg2, xg3, xg4;
  int   ig1ref, ig2ref, ig3ref, ig4ref;
  int   hemisphere, indOrientation;
  float PosXDuPole, PosYDuPole, PasDeGrille, AngleGreenwich;
  float latOrigine, lonOrigine, deltaLat, deltaLon;
  float xOrigine,yOrigine;
  float elat1,elon1,elat2,elon2;
  float *lat, *lon, *x, *y;
} GeoMapInfoStruct;

#define CONTINENTS   0
#define LATLON       1
#define PAYS         2 
#define PROVINCES    3
#define VILLES       4
#define LACS         5
#define RIVIERES     6
#define ROUTES       7 
#define RAILS        8
#define UTILITES     9
#define CANAUX      10
#define TOPOGRAPHIE 11
#define BATHYMETRIE 12 
#define TERRAIN     13

#define NMAP_FLAGS 16

typedef struct
{
  int etat[NMAP_FLAGS];
  int epaisseur[NMAP_FLAGS];
  int style[NMAP_FLAGS];
  int couleur[NMAP_FLAGS][3];
  int indCouleur[NMAP_FLAGS];
  int lu[NMAP_FLAGS];
  int typeValide;
  int clipNecessaire;
  int verifStatutNecessaire;
} GeoMapFlagsStruct;

typedef struct
{
  float x;
  float y;
} PointGeoStruct;


typedef struct 
{
  int npts;
  float xmin, xmax, ymin, ymax;
  int statutPRGrille;
  PointGeoStruct *pointsGeo;
} ListePointsStruct;

typedef struct
{
  char  *fichierGeographie[2];
  int   nbFichiersGeographie;
  float intervalleMeridiens;
  int   flagLabel;
  int   flagPerim;
  int   styleGeo;
  int   couleurGeo;
  int   epaisseurGeo;
  int   styleMer;
  int   couleurMer;
  int   epaisseurMer;
  int   couleurPerim;
  int   epaisseurPerim;
  int   couleurLabel;
  int   epaisseurLabel;
  float resolution;
} GeoMapOptionsStruct;


 
int TracerVecteursModeTurbo(ListePointsStruct *itemListe);
int TracerVecteurs(ListePointsStruct *itemListe);
int swapFloats(float *a, float *b);
int LireFichierGeographie(ListePointsStruct *(*liste), int *nbItems, char *nomFichier[], int nbFichiers);
int LireLatLon(ListePointsStruct *(*liste), int *nbItems);
int TracerPointsModeTurbo(ListePointsStruct *itemListe);
int TracerPoints(ListePointsStruct *itemListe);
int ActiverParamsLigne(int style, int couleur, int epaisseur);
int NewGeoItem(ListePointsStruct *(*liste), int *nbItems);
int VerifierSegmentLatlon(float lonMin, float lonMax);
int gmp_trim(float pts[], int *nbpts, float resolution);
int gmp_perim(float *xgdmin, float *ygdmin,  float *xgdmax,  float *ygdmax, float  *latMin,  float *lonMin, float *latMax, float *lonMax, int *nbSeg);
int gmp_convert(ListePointsStruct *liste, int *nbItems, float pts[], int nbPoints, float xgdmin, float ygdmin, float xgdmax, float ygdmax, int nbSeg);
void LibererCarte(ListePointsStruct *(*liste), int *nbItems);
int CopierMapInfos(GeoMapInfoStruct *mapInfoSortie, GeoMapInfoStruct *mapInfoEntree);
int clip(float x1, float y1, float x2, float y2);
int gmp_llfgr(float *lat, float *lon, float x, float y, float latOrigine, float lonOrigine, float deltaLat, float deltaLon);
int c_grfll(float *x, float *y, float lat, float lon, float latOrigine, float lonOrigine, float deltaLat, float deltaLon, float xOrigine, float yOrigine, int   orientation);
int c_gmpzset(char grtyp, int ni, int nj,  int ig1, int ig2, int ig3, int ig4, char typeref, int ig1ref, int ig2ref, int ig3ref, int ig4ref);
int c_gmpset(char grtyp, int ni, int nj, int ig1, int ig2, int ig3, int ig4);
int c_gmpopts(char *option, char *valeur);
void c_gmpl2g(float *x,float *y,float lat,float lon);
void c_gmpg2l(float *lat,float *lon,float x,float y);
void c_gmpdrw();
void AfficherVecteurs(ListePointsStruct *liste, int nbItems, int style, int couleur, int epaisseur);

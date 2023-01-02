#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define FG_SHIFT 30
#define BG_SHIFT 40

typedef enum {BLACK = 0, RED, GREEN, MAX_COLORS} ColorCode;

ColorCode ** createGrid (int, int);
void destroyGrid (ColorCode ***, int);
void displayColor (int, int, char);
void displayGrid (ColorCode **, int, int, int, char);
void playerVSplayer (int);
void insert(ColorCode**, ColorCode, int, int);
int lineIsFull(ColorCode**, int );
int winDetection(ColorCode**, int, int*, int*, char*);
int isFull(ColorCode**, int);
int isFlooded (ColorCode ** , int );
void playerVSbot(int );
int blockPlayer(ColorCode ** , int );
int artificialRandom(int , int );
int* advantageousZones(ColorCode ** , int );
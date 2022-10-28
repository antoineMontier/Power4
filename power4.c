#include <stdlib.h>
#include <stdio.h>


// const int
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


void displayColor(int fg, int bg, char c){
  printf("\033[%d;%dm %c\033[m", FG_SHIFT + fg, BG_SHIFT + bg, c);
}

ColorCode ** createGrid(int n, int val){//n is the matrix size (n*n)
//create pointeur array
  ColorCode ** M;
  M = malloc(n*sizeof(int*));

//create each matrix line
  for(int i = 0 ; i < n ; i++){
    M[i] = malloc(n*sizeof(int));
  }

//initialize each line
  for(int i = 0 ; i < n ; i++){
    for(int j = 0 ; j < n ; j++){
      M[i][j] = val;
    }
  }
  return M;
}


void destroyGrid (ColorCode *** M, int n){//n is the matrix size (n*n)
  for(int i = 0 ; i < n; i++){

    if((*M)[i] == NULL){
      printf("line %d hasn't been destroyed\n", i);
      i++;
    }
    free((*M)[i]);
  }
  if((*M) == NULL)
    printf("matrix is null");
  free((*M));
  *M = NULL;
}


void displayGrid (ColorCode ** M, int n, int winline, int wincol, char windir){//'u' : up ; 'd' : down ; 'l' : left ; 'r' : right ;       '1' : left upper diag ; '2' : right upper diag ; '3' : left bottom diag ; '4' : right bottom diag
  int win = (winline != -1 && wincol != -1 && windir != 0);
  int x1, y1, x2, y2, x3, y3, x4, y4;//calcul the coordinates of win

  if(win){
    x1 = winline;
    y1 = wincol;

    if(windir == 'l'){
      x2 = x3 = x4 = x1;
      y2 = y1 -1;
      y3 = y2 -1;
      y4 = y3 -1;
    }else if(windir == 'd'){
      x2 = x3 = x4 = x1;
      y2 = y1 +1;
      y3 = y2 +1;
      y4 = y3 +1;
    }else if(windir == 'u'){
      y2 = y3 = y4 = y1;
      x2 = x1 -1;
      x3 = x2 -1;
      x4 = x3 -1;
    }else if(windir == 'r'){
      y2 = y3 = y4 = y1;
      x2 = x1 +1;
      x3 = x2 +1;
      x4 = x3 +1;
    }else if(windir == '1'){
      x2 = x1 -1;
      x3 = x2 -1;
      x4 = x3 -1;
      y2 = y1 -1;
      y3 = y2 -1;
      y4 = y3 -1;
    }else if(windir == '2'){
      x2 = x1 +1;
      x3 = x2 +1;
      x4 = x3 +1;
      y2 = y1 -1;
      y3 = y2 -1;
      y4 = y3 -1;
    }else if(windir == '3'){
      x2 = x1 -1;
      x3 = x2 -1;
      x4 = x3 -1;
      y2 = y1 +1;
      y3 = y2 +1;
      y4 = y3 +1;
    }else if(windir == '4'){
      x2 = x1 +1;
      x3 = x2 +1;
      x4 = x3 +1;
      y2 = y1 +1;
      y3 = y2 +1;
      y4 = y3 +1;
    }
  /*  printf("(%d, %d)\n", x1, y1);
    printf("(%d, %d)\n", x2, y2);
    printf("(%d, %d)\n", x3, y3);
    printf("(%d, %d)\n", x4, y4);*/
  }//here (x1, y1), (x2, y2), (x3, y3), (x4, y4) are the 4 wining coordinates

    printf("\n");
  for(char i = 'a' ; i <'a'+ n ; i++){    //controls
      printf("|%c ", i);
  }
  printf("|\n");

  for(int i = 0 ; i < n ; i++){
    for(int j = 0 ; j < n ; j++){
      printf("|");
      if(!win){
        displayColor(M[i][j], M[i][j], ' ');  //grid
      }else{//win case
        if((i == x1 && j == y1) || (i == x2 && j == y2) || (i == x3 && j == y3) || (i == x4 && j == y4)){
          displayColor(BLACK, M[i][j], 'X');  //grid
        }else{
          displayColor(M[i][j], M[i][j], ' ');  //grid
        }
      }
    }
    printf("|\n");
  }
  //printf("-------------------------------------\n");
}

void insert(ColorCode**Mplay, ColorCode playercolor, int position, int size){
  int level = -1;
  while(level < size-1 && Mplay[level+1][position] == BLACK)
    level++;

  Mplay[level][position] = playercolor;
}

int isFlooded (ColorCode ** M, int n){//renvois 1 si la matrice est remplie et 0 sinon
  for(int i = 0 ; i < n ; i++){
    for(int j = 0 ; j < n ; j++){
      if(M[i][j] == 0){
        return 0;
      }
    }
  }
  return 1;
}


int lineIsFull(ColorCode**Mplay, int pos){
  if(Mplay[0][pos] != BLACK)
    return 1;
  return 0;
}

int winDetection(ColorCode**m, int size, int*winline, int*wincol, char*direction){
  int win = 0;
  ColorCode col = 1;
  int up = 1;
  int down = 1;
  int left = 1;
  int right = 1;
  int ul = 1;//up left
  int ur = 1;
  int dl = 1;
  int dr = 1;//down right
  while(col < MAX_COLORS && !win){
    //printf(" detection of win for player %d\n", col);
    for(int i = size -1 ; i >= 0 ; i--){//starting from bottom for less calculs if win
      for(int j = 0 ; j < size ; j++){
        if(m[i][j] == col){
          up = 1;
          down = 1;
          left = 1;
          right = 1;
          ul = 1;//up left
          ur = 1;
          dl = 1;
          dr = 1;//down right
        //printf(" origin (%d, %d)\n", i, j);
          for(int t = 1 ; t < 4 && (up || down || right || left || ul || ur || dl || dr) ; t++){//tests
            up = up && (i - t >= 0) && m[i-t][j] == col;
            down = down && (i + t < size) && m[i+t][j] == col;
            left = left && (j - t >= 0) && m[i][j-t] == col;
            right = right && (i + t < size) && m[i][j+t] == col;
            ul = ul && (i - t >= 0) && (j - t >= 0) && m[i-t][j-t] == col;
            ur = ur && (i - t >= 0) && (j + t < size) && m[i-t][j+t] == col;
            dl = dl && (i + t < size) && (j - t >= 0) && m[i+t][j-t] == col;
            dr = dr && (i + t < size) && (j + t < size) && m[i+t][j+t] == col;
          //  printf("up : %d at iteration %d\n", up, t);
          }
          win = (up || down || right || left || ul || ur || dl || dr);
        //  printf(" end of test : %d",win);
          if(win){
            *winline = i;
            *wincol = j;
            if(up){
              *direction = 'u';
              return col;
            }
            if(right){
              *direction = 'r';
              return col;
            }
            if(down){
              *direction = 'd';
              return col;
            }
            if(left){
              *direction = 'l';
              return col;
            }
            if(ul){
              *direction = '1';
              return col;
            }
            if(ur){
              *direction = '3';
              return col;
            }
            if(dl){
              *direction = '3';
              return col;
            }
            if(dr){
              *direction = '4';
              return col;
            }
          }
        }
      }
    }
    col++;
  }
  return 0;
}


int blockPlayer(ColorCode ** m, int size){
  ColorCode **copy = createGrid(size, 0);

  int useless1;
  int useless2;
  char useless3;

  for(int e = 0 ; e < size ; e++){

    for(int i = 0 ; i < size ; i++){
      for(int j = 0 ; j < size ; j++){
        copy[i][j] = m[i][j];
      }
    }
    if(!lineIsFull(copy, e))
      insert(copy, 1, e, size);//simulates every moves the player could do
    if(winDetection(copy, size, &useless1, &useless2, &useless3))
      return e;

  }
  destroyGrid(&copy, size);
  return -1;
}







void playerVSplayer(int size){
  int winline = -1;
  int wincol = -1;
  char windir = 0;//'u' : up ; 'd' : down ; 'l' : left ; 'r' : right ;       '1' : left upper diag ; '2' : right upper diag ; '3' : left bottom diag ; '4' : right bottom diag
  ColorCode ** Mplay = createGrid(size, 0);

  ColorCode player = 1;
  char res;

  char*p1 = malloc(50*sizeof(char));
  char*p2 = malloc(50*sizeof(char));
  p2[0] = p1[0] = 0;

  printf("who's playing first ? (enter your name) : ");
  do{
    scanf("%s", p1);
  }while(p1[0] == 0);

  printf("who's the other player ? (enter your name) : ");
  do{
    scanf("%s", p2);
  }while(p2[0] == 0);

  printf("\nplayer 1 : %s   player 2 : %s\n", p1, p2 );

  displayGrid(Mplay, size, winline, wincol, windir);


  while(1){//infinite loop

    if(player == 1)
      printf("%s enter your position : ", p1);

    if(player == 2)
      printf("%s enter your position : ", p2);


    do{
      scanf("%c", &res);
    }while(res < 'a' || res > 'a' + size - 1 || lineIsFull(Mplay, res - 'a'));

    insert(Mplay, player, res - 'a', size);

    if(player == 1)
      player = 2;
    else
      player = 1;

    int winner  = winDetection(Mplay, size, &winline, &wincol, &windir);
    displayGrid(Mplay, size, winline, wincol, windir);
    if(winner != 0){
      if(winner == 1)
      printf("\n\n%s won !!\n\n" , p1);

      if(winner == 2)
      printf("\n\n%s won !!\n\n" , p2);
      return;
    }

    if(isFlooded(Mplay, size)){
      printf("equality !\n");
      return;
    }

  }
  destroyGrid(&Mplay, size);
}


void randPlay(int size){
  int winline = -1;
  int wincol = -1;
  char windir = 0;//'u' : up ; 'd' : down ; 'l' : left ; 'r' : right ;       '1' : left upper diag ; '2' : right upper diag ; '3' : left bottom diag ; '4' : right bottom diag
  srand(clock());
  ColorCode ** Mplay = createGrid(size, 0);
  displayGrid(Mplay, size, winline, wincol, windir);
  ColorCode player = 1;
  char res;




  while(1){//infinite loop

    do{
      res = (int)rand()% size;
    }while(lineIsFull(Mplay, res));

    insert(Mplay, player, res, size);

    if(player == 1)
      player = 2;
    else
      player = 1;

    int winner  = winDetection(Mplay, size, &winline, &wincol, &windir);
    displayGrid(Mplay, size, winline, wincol, windir);
    if(winner != 0){
      printf("\n\nplayer %d won\n\n" , winner);
      displayGrid(Mplay, size, winline, wincol, windir);

      //printf("win motif : (%d, %d) %c\n\n",winline, wincol, windir );
      displayGrid(Mplay, size, winline, wincol, windir);
      return;
    }

    if(isFlooded(Mplay, size)){
      printf("equality !\n");
      return;
    }

  }
  destroyGrid(&Mplay, size);
}



void playerVSbot(int size){
  int winline = -1;
  int wincol = -1;
  char windir = 0;//'u' : up ; 'd' : down ; 'l' : left ; 'r' : right ;       '1' : left upper diag ; '2' : right upper diag ; '3' : left bottom diag ; '4' : right bottom diag
  ColorCode ** Mplay = createGrid(size, 0);

  char res;


  char botres;
  int blockIndex = -1;

  char*p1 = malloc(50*sizeof(char));
  p1[0] = 0;
  printf("who's playing ? (enter your name) : ");///name record
  do{
    scanf("%s", p1);
  }while(p1[0] == 0);


  displayGrid(Mplay, size, winline, wincol, windir);


  while(1){//infinite loop

    printf("%s enter your position : ", p1);
    do{
      scanf("%c", &res);
    }while(res < 'a' || res > 'a' + size - 1 || lineIsFull(Mplay, res - 'a'));
    printf("player playing %d ", res);
    insert(Mplay, 1, res - 'a', size);
    printf("played !\n");






    do{
      botres = (int)rand()% size;
    }while(lineIsFull(Mplay, botres));//bo decides what to play

    printf("bot has choose a random column to play%d\n", botres);


    blockIndex = blockPlayer(Mplay, size);

    printf("block player : %d\n", blockIndex);

    if(blockIndex != -1 && !lineIsFull(Mplay, blockIndex))
      botres = blockIndex;


    printf("bot playing %d", botres);
    insert(Mplay, 2, botres, size);
    printf("played !\n");


    int winner  = winDetection(Mplay, size, &winline, &wincol, &windir);
    displayGrid(Mplay, size, winline, wincol, windir);
    if(winner != 0){
      if(winner == 1)
        printf("\n\n%s won !!\n\n" , p1);

      if(winner == 2)
        printf("\n\nRobot won !!\n\n");

      destroyGrid(&Mplay, size);
      return;
    }

    if(isFlooded(Mplay, size)){
      printf("equality !\n");
      destroyGrid(&Mplay, size);
      return;
    }

  }
}


int main(){/* gcc -c -Wall -Wextra power4.c && gcc power4.o -lm -o power4 && ./power4 */
    playerVSbot(10);
    return 0;
}

#include <stdlib.h>
#include <stdio.h>


// const int
#define FG_SHIFT 30
#define BG_SHIFT 40

typedef enum {BLACK = 0, RED, GREEN, MAX_COLORS} ColorCode;

ColorCode ** createGrid (int, int);
void destroyGrid (ColorCode ***, int);
void displayColor (int, int, char);
void displayGrid (ColorCode **, int);
void play (int);
void insert(ColorCode**, ColorCode, int, int);
int lineIsFull(ColorCode**, int );
int winDetection(ColorCode**, int);
int isFull(ColorCode**, int);


void displayColor(int fg, int bg, char c){
  printf("\033[%d;%dm %c\033[m", FG_SHIFT + fg, BG_SHIFT + bg, c);
}

ColorCode ** createGrid(int n, int val){//n is the matrix size (n*n)
//create pointeur array
  ColorCode ** M;
  M = malloc(n*sizeof(int*));

//create each matrix line
  for(int i = 0 ; i <= n ; i++){
    M[i] = malloc(n*sizeof(int));
  }

//initialize each line
  for(int i = 0 ; i <= n ; i++){
    for(int j = 0 ; j <= n ; j++){
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


void displayGrid (ColorCode ** M, int n){
    printf("\n");

  for(char i = 'a' ; i <'a'+ n ; i++){    //controls
      printf("|%c ", i);
  }
  printf("\n");

  for(int i = 0 ; i < n ; i++){
    for(int j = 0 ; j < n ; j++){
      printf("|");
      displayColor(M[i][j], M[i][j], ' ');  //grid
    }
    printf("|\n");
  }
  printf("-------------------------------------\n");
}

void insert(ColorCode**Mplay, ColorCode playercolor, int position, int size){
  int level = -1;
  while(level < size-1 && Mplay[level+1][position] == BLACK)
    level++;
    
  Mplay[level][position] = playercolor;
}

int lineIsFull(ColorCode**Mplay, int pos){
  if(Mplay[0][pos] != BLACK)
    return 1;
  return 0;
}

int winDetection(ColorCode**m, int size){
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
    printf(" detection of win for player %d\n", col);
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
          printf(" origin (%d, %d)\n", i, j);
          for(int t = 1 ; t < 4 && (up || down || right || left || ul || ur || dl || dr) ; t++){//tests
            up = up && (i - t >= 0) && m[i-t][j] == col;
            down = down && (i + t < size) && m[i+t][j] == col;
            left = left && (j - t >= 0) && m[i][j-t] == col;
            right = right && (i + t < size) && m[i][j+t] == col;
            ul = ul && (i - t >= 0) && (j - t >= 0) && m[i-t][j-t] == col;
            ur = ur && (i - t >= 0) && (j + t < size) && m[i-t][j+t] == col;
            dl = dl && (i + t < size) && (j - t >= 0) && m[i+t][j-t] == col;
            dr = dr && (i + t < size) && (j + t < size) && m[i+t][j+t] == col;
            printf("up : %d at iteration %d\n", up, t);
          }
          win = (up || down || right || left || ul || ur || dl || dr);
          printf(" end of test : %d",win);
          if(win)
            return col; //here the boolean expressions are still avaible
        }
      }
    }
    col++;
  }
  return 0;
}




void play(int size){
  ColorCode ** Mplay = createGrid(size, 0);
  displayGrid(Mplay, size);
  ColorCode player = 1;
  char res;
  
  while(1){//infinite loop
    
    printf("player %d enter your position : ", player);
    do{
      scanf("%c", &res);
    }while(res < 'a' || res > 'a' + size - 1 || lineIsFull(Mplay, res - 'a'));
    
    insert(Mplay, player, res - 'a', size);
    
    if(player == 1)
      player = 2;
    else
      player = 1;
    
    int winner  = winDetection(Mplay, size);
    displayGrid(Mplay, size);
    if(winner != 0){
      printf("\n\nplayer %d won\n\n" , winner);
      return;
    }
    
  }
  destroyGrid(&Mplay, size);
}


int main(){
    play(12);
    return 0;
}

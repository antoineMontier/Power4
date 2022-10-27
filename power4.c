#include <stdlib.h>
#include <stdio.h>


// const int
#define SIZE 12

#define FG_SHIFT 30
#define BG_SHIFT 40

typedef enum {BLACK = 0, RED, GREEN, MAX_COLORS} ColorCode;

ColorCode ** createGrid (int, int);
void destroyGrid (ColorCode ***, int);
void displayColor (int, int, char);
void displayGrid (ColorCode **, int);
void play (int);



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
  
  for(char i = 'a' ; i <'a'+ n ; i++){    //controls
      printf("%c ", i);
  }
  printf("\n");

  for(int i = 0 ; i < n ; i++){
    for(int j = 0 ; j < n ; j++){
      displayColor(M[i][j], M[i][j], ' ');  //grid
    }
    printf("\n");
  }
}


void play(int size){
  ColorCode ** Mplay = createGrid(size, 0);
  displayGrid(Mplay, size);
  destroyGrid(&Mplay, size);
}











int main(){
    play(20);
    return 0;
}

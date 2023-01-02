#include "power4.h"
int maxIndex(int *tab, int size)
{
    int index = -1;
    int r = tab[0];
    for (int i = 0; i < size; i++)
    {
        if (tab[i] > r)
        {
            r = tab[i];
            index = i;
        }
    }
    return index;
}

void displayColor(int fg, int bg, char c)
{
    printf("\033[%d;%dm %c\033[m", FG_SHIFT + fg, BG_SHIFT + bg, c);
}

ColorCode **createGrid(int n, int val)
{ // n is the matrix size (n*n)
    // create pointeur array
    ColorCode **M;
    M = malloc(n * sizeof(int *));

    // create each matrix line
    for (int i = 0; i < n; i++)
    {
        M[i] = malloc(n * sizeof(int));
    }

    // initialize each line
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            M[i][j] = val;
        }
    }
    return M;
}

void destroyGrid(ColorCode ***M, int n)
{ // n is the matrix size (n*n)
    for (int i = 0; i < n; i++)
    {

        if ((*M)[i] == NULL)
        {
            fprintf(stderr, "line %d hasn't been destroyed\n", i);
            i++;
        }
        free((*M)[i]);
    }
    if ((*M) == NULL)
        fprintf(stderr, "matrix is null");
    free((*M));
    *M = NULL;
}

void displayGrid(ColorCode **M, int n, int winline, int wincol, char windir)
{ //'u' : up ; 'd' : down ; 'l' : left ; 'r' : right ;       '1' : left upper diag ; '2' : right upper diag ; '3' : left bottom diag ; '4' : right bottom diag
    int win = (winline != -1 && wincol != -1 && windir != 0);
    int x1, y1, x2, y2, x3, y3, x4, y4; // calcul the coordinates of win

    if (win)
    {
        x1 = winline;
        y1 = wincol;
        switch(windir){
                
            case 'l':
                x2 = x3 = x4 = x1;
                y2 = y1 - 1;
                y3 = y2 - 1;
                y4 = y3 - 1;
                break;
                
            case 'd':
                x2 = x3 = x4 = x1;
                y2 = y1 + 1;
                y3 = y2 + 1;
                y4 = y3 + 1;
                break;
                
            case 'u':
                y2 = y3 = y4 = y1;
                x2 = x1 - 1;
                x3 = x2 - 1;
                x4 = x3 - 1;
                break;
               
            case 'r':
                y2 = y3 = y4 = y1;
                x2 = x1 + 1;
                x3 = x2 + 1;
                x4 = x3 + 1;
                break;
                
            case '1':
                x2 = x1 - 1;
                x3 = x2 - 1;
                x4 = x3 - 1;
                y2 = y1 - 1;
                y3 = y2 - 1;
                y4 = y3 - 1;
                break;
                
            case '2':
                x2 = x1 + 1;
                x3 = x2 + 1;
                x4 = x3 + 1;
                y2 = y1 - 1;
                y3 = y2 - 1;
                y4 = y3 - 1;
                break;
                
            case '3':
                x2 = x1 - 1;
                x3 = x2 - 1;
                x4 = x3 - 1;
                y2 = y1 + 1;
                y3 = y2 + 1;
                y4 = y3 + 1;
                break;
            
            case '4' : 
               x2 = x1 + 1;
                x3 = x2 + 1;
                x4 = x3 + 1;
                y2 = y1 + 1;
                y3 = y2 + 1;
                y4 = y3 + 1;
                break;
            
            default:
                break;
        }


    } // here (x1, y1), (x2, y2), (x3, y3), (x4, y4) are the 4 wining coordinates

    printf("\n");
    for (char i = 'a'; i < 'a' + n; i++)
        printf("|%c ", i);
    printf("|\n");

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("|");
            if (!win)
            {
                displayColor(M[i][j], M[i][j], ' '); // grid
            }
            else
            { // win case
                if ((i == x1 && j == y1) || (i == x2 && j == y2) || (i == x3 && j == y3) || (i == x4 && j == y4))
                {
                    displayColor(BLACK, M[i][j], 'X'); // grid
                }
                else
                {
                    displayColor(M[i][j], M[i][j], ' '); // grid
                }
            }
        }
        printf("|\n");
    }
    // printf("-------------------------------------\n");
}

void insert(ColorCode **Mplay, ColorCode playercolor, int position, int size)
{
    if(Mplay[0][position] != 0)//avoid bugs
        return;
    int level = -1;
    while (level < size - 1 && Mplay[level + 1][position] == BLACK)
        level++;

    Mplay[level][position] = playercolor;
}

int isFlooded(ColorCode **M, int n)
{ // renvois 1 si la matrice est remplie et 0 sinon
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (M[i][j] == 0)
            {
                return 0;
            }
        }
    }
    return 1;
}

int lineIsFull(ColorCode **Mplay, int pos)
{
    if (Mplay[0][pos] != BLACK)
        return 1;
    return 0;
}

int winDetection(ColorCode **m, int size, int *winline, int *wincol, char *direction)
{
    int win = 0;
    ColorCode col = 1;
    int up = 1;
    int down = 1;
    int left = 1;
    int right = 1;
    int ul = 1; // up left
    int ur = 1;
    int dl = 1;
    int dr = 1; // down right
    while (col < MAX_COLORS && !win)
    {
        // printf(" detection of win for player %d\n", col);
        for (int i = size - 1; i >= 0; i--)
        { // starting from bottom for less calculs if win
            for (int j = 0; j < size; j++)
            {
                if (m[i][j] == col)
                {
                    up = 1;
                    down = 1;
                    left = 1;
                    right = 1;
                    ul = 1; // up left
                    ur = 1;
                    dl = 1;
                    dr = 1; // down right
                    // printf(" origin (%d, %d)\n", i, j);
                    for (int t = 1; t < 4 && (up || down || right || left || ul || ur || dl || dr); t++)
                    { // tests
                        up = up && (i - t >= 0) && m[i - t][j] == col;
                        down = down && (i + t < size) && m[i + t][j] == col;
                        left = left && (j - t >= 0) && m[i][j - t] == col;
                        right = right && (i + t < size) && m[i][j + t] == col;
                        ul = ul && (i - t >= 0) && (j - t >= 0) && m[i - t][j - t] == col;
                        ur = ur && (i - t >= 0) && (j + t < size) && m[i - t][j + t] == col;
                        dl = dl && (i + t < size) && (j - t >= 0) && m[i + t][j - t] == col;
                        dr = dr && (i + t < size) && (j + t < size) && m[i + t][j + t] == col;
                        //  printf("up : %d at iteration %d\n", up, t);
                    }
                    win = (up || down || right || left || ul || ur || dl || dr);
                    //  printf(" end of test : %d",win);
                    if (win)
                    {
                        *winline = i;
                        *wincol = j;
                        if (up)
                        {
                            *direction = 'u';
                            return col;
                        }
                        if (right)
                        {
                            *direction = 'r';
                            return col;
                        }
                        if (down)
                        {
                            *direction = 'd';
                            return col;
                        }
                        if (left)
                        {
                            *direction = 'l';
                            return col;
                        }
                        if (ul)
                        {
                            *direction = '1';
                            return col;
                        }
                        if (ur)
                        {
                            *direction = '3';
                            return col;
                        }
                        if (dl)
                        {
                            *direction = '3';
                            return col;
                        }
                        if (dr)
                        {
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

int blockPlayer(ColorCode **m, int size)
{
    ColorCode **copy = createGrid(size, 0);

    int useless1;
    int useless2;
    char useless3;

    for (int e = 0; e < size; e++)
    {

        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                copy[i][j] = m[i][j];
            }
        }
        if (!lineIsFull(copy, e))
            insert(copy, 1, e, size); // simulates every moves the player could do
        if (winDetection(copy, size, &useless1, &useless2, &useless3))
            return e;
    }
    destroyGrid(&copy, size);
    return -1;
}

int winnable(ColorCode **m, int size)
{
    ColorCode **copy = createGrid(size, 0);

    int useless1;
    int useless2;
    char useless3;

    for (int e = 0; e < size; e++)
    {

        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                copy[i][j] = m[i][j];
            }
        }
        if (!lineIsFull(copy, e))
            insert(copy, 2, e, size); // simulates every moves the player could do
        if (winDetection(copy, size, &useless1, &useless2, &useless3))
            return e;
    }
    destroyGrid(&copy, size);
    return -1;
}

int *dangerousZones(ColorCode **m, int size)
{ // objectif is to stimulate two moves of the player and block the first one that makes him win
    ColorCode **copy = createGrid(size, 0);
    ColorCode **copyy = createGrid(size, 0);

    int useless1;
    int useless2;
    char useless3;

    int *results = malloc(size * sizeof(int)); // result tab resuls[k] is the umber of differents ways the player could win playing k move

    for (int i = 0; i < size; i++)
    {
        results[i] = 0;
    }

    for (int e = 0; e < size; e++)
    {

        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                copy[i][j] = m[i][j];
            }
        }

        if (!lineIsFull(copy, e))
        {
            insert(copy, 1, e, size); // simulates every moves the player could do as first turn

            for (int f = 0; f < size; f++)
            {

                for (int i = 0; i < size; i++)
                {
                    for (int j = 0; j < size; j++)
                    { // reset copyy
                        copyy[i][j] = copy[i][j];
                    }
                }

                if (!lineIsFull(copyy, e))
                    insert(copyy, 1, f, size); // simulates every moves the player could do as second turn

                if (winDetection(copyy, size, &useless1, &useless2, &useless3))
                    results[e]++;
            }
        }
    }

    destroyGrid(&copyy, size);
    destroyGrid(&copy, size);
    return results;
}

int *advantageousZones(ColorCode **m, int size)
{ // objectif is to stimulate two moves of the bot and play the first one that makes him win
    ColorCode **copy = createGrid(size, 0);
    ColorCode **copyy = createGrid(size, 0);

    int useless1;
    int useless2;
    char useless3;

    int *results = malloc(size * sizeof(int)); // result tab resuls[k] is the umber of differents ways the bot could win playing k move

    for (int i = 0; i < size; i++)
    {
        results[i] = 0;
    }

    for (int e = 0; e < size; e++)
    {

        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                copy[i][j] = m[i][j];
            }
        }

        if (!lineIsFull(copy, e))
        {
            insert(copy, 2, e, size); // simulates every moves the bot could do as first turn

            for (int f = 0; f < size; f++)
            {

                for (int i = 0; i < size; i++)
                {
                    for (int j = 0; j < size; j++)
                    { // reset copyy
                        copyy[i][j] = copy[i][j];
                    }
                }

                if (!lineIsFull(copyy, e))
                    insert(copyy, 2, f, size); // simulates every moves the bot could do as second turn

                if (winDetection(copyy, size, &useless1, &useless2, &useless3))
                    results[e]++;
            }
        }
    }

    destroyGrid(&copyy, size);
    destroyGrid(&copy, size);
    return results;
}

int *notToPlay(ColorCode **m, int size)
{ // objectif is to stimulate one move of robot and then everymove of the player to assert he can't win
    ColorCode **copy = createGrid(size, 0);
    ColorCode **copyy = createGrid(size, 0);

    int useless1;
    int useless2;
    char useless3;

    int *results = malloc(size * sizeof(int)); // result tab resuls[k] is the umber of differents ways the player could win playing k move

    for (int i = 0; i < size; i++)
    {
        results[i] = 0;
    }

    for (int e = 0; e < size; e++)
    {

        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                copy[i][j] = m[i][j];
            }
        }

        if (!lineIsFull(copy, e))
        {
            insert(copy, 2, e, size); // simulates every moves the player could do as first turn

            for (int f = 0; f < size && results[e] == 0; f++)
            {

                for (int i = 0; i < size; i++)
                {
                    for (int j = 0; j < size; j++)
                    { // reset copyy
                        copyy[i][j] = copy[i][j];
                    }
                }

                if (!lineIsFull(copyy, e))
                    insert(copyy, 1, f, size); // simulates every moves the player could do as second turn

                if (winDetection(copyy, size, &useless1, &useless2, &useless3))
                    results[e] = 1;
            }
        }
    }

    destroyGrid(&copyy, size);
    destroyGrid(&copy, size);
    return results; // the tab conains 0 if it's safe to play here and 1 if not
}

int artificialRandom(int lower, int upper)
{ // stimulates a random number between lower and upper (included) with a highest probability for middle numbers !
    srand(clock());
    // first let's generate a random number between lower and upper :
    int a = lower + (rand()) % (upper - lower + 1);
    // then let's find the 1st and nb thirds :
    double q1 = (upper - lower) / 4;
    double q3 = 3 * q1;

    if (a < q1 + lower)
        a += rand() % ((int)q1);
    if (a > q3 + lower)
        a -= rand() % ((int)q1);

    return a;
}

void playerVSplayer(int size)
{
    int winline = -1;
    int wincol = -1;
    char windir = 0; //'u' : up ; 'd' : down ; 'l' : left ; 'r' : right ;       '1' : left upper diag ; '2' : right upper diag ; '3' : left bottom diag ; '4' : right bottom diag
    ColorCode **Mplay = createGrid(size, 0);

    ColorCode player = 1;
    char res;

    char *p1 = malloc(50 * sizeof(char));
    char *p2 = malloc(50 * sizeof(char));
    p2[0] = p1[0] = 0;

    printf("who's playing first ? (enter your name) : ");
    do
    {
        scanf("%s", p1);
    } while (p1[0] == 0);

    printf("who's the other player ? (enter your name) : ");
    do
    {
        scanf("%s", p2);
    } while (p2[0] == 0);

    printf("\nplayer 1 : %s   player 2 : %s\n", p1, p2);

    displayGrid(Mplay, size, winline, wincol, windir);

    while (1)
    { // infinite loop

        if (player == 1)
            printf("%s enter your position : ", p1);

        if (player == 2)
            printf("%s enter your position : ", p2);

        do
        {
            scanf("%c", &res);
        } while (res < 'a' || res > 'a' + size - 1 || lineIsFull(Mplay, res - 'a'));

        insert(Mplay, player, res - 'a', size);

        if (player == 1)
            player = 2;
        else
            player = 1;

        int winner = winDetection(Mplay, size, &winline, &wincol, &windir);
        displayGrid(Mplay, size, winline, wincol, windir);
        if (winner != 0)
        {
            if (winner == 1)
                printf("\n\n%s won !!\n\n", p1);

            if (winner == 2)
                printf("\n\n%s won !!\n\n", p2);
            free(p1);
            free(p2);
            return;
        }

        if (isFlooded(Mplay, size))
        {
            printf("equality !\n");
            destroyGrid(&Mplay, size);
            free(p1);
            free(p2);
            return;
        }
    }
}

void randPlay(int size)
{
    int winline = -1;
    int wincol = -1;
    char windir = 0; //'u' : up ; 'd' : down ; 'l' : left ; 'r' : right ;       '1' : left upper diag ; '2' : right upper diag ; '3' : left bottom diag ; '4' : right bottom diag
    srand(clock());
    ColorCode **Mplay = createGrid(size, 0);
    displayGrid(Mplay, size, winline, wincol, windir);
    ColorCode player = 1;
    char res;

    while (1)
    { // infinite loop

        do
        {
            res = artificialRandom(0, size - 1);
        } while (lineIsFull(Mplay, res));

        printf("%d", res);

        insert(Mplay, player, res, size);

        if (player == 1)
            player = 2;
        else
            player = 1;

        int winner = winDetection(Mplay, size, &winline, &wincol, &windir);
        displayGrid(Mplay, size, winline, wincol, windir);
        if (winner != 0)
        {
            printf("\n\nplayer %d won\n\n", winner);
            displayGrid(Mplay, size, winline, wincol, windir);

            // printf("win motif : (%d, %d) %c\n\n",winline, wincol, windir );
            displayGrid(Mplay, size, winline, wincol, windir);
            destroyGrid(&Mplay, size);
            return;
        }

        if (isFlooded(Mplay, size))
        {
            printf("equality !\n");
            destroyGrid(&Mplay, size);
            return;
        }
    }
}

void playerVSbot(int size)
{
    int winline = -1;
    int wincol = -1;
    char windir = 0; //'u' : up ; 'd' : down ; 'l' : left ; 'r' : right ;       '1' : left upper diag ; '2' : right upper diag ; '3' : left bottom diag ; '4' : right bottom diag
    ColorCode **Mplay = createGrid(size, 0);

    char res;
    int winner = 0;

    int botres;
    int blockIndex = -1;
    int winIndex = -1;
    int toBlock;
    int *toBlockTab;
    int *notPlay;
    int randIterations;
    int *toPlayTab;
    int toPlay;

    char *p1 = malloc(50 * sizeof(char));
    p1[0] = 0;
    printf("who's playing ? (enter your name) : "); /// name record
    do
    {
        scanf("%s", p1);
    } while (p1[0] == 0);

    displayGrid(Mplay, size, winline, wincol, windir);

    while (1)
    { // infinite loop

        printf("%s enter your position : ", p1);
        do
        {
            scanf("%c", &res);
        } while (res < 'a' || res > 'a' + size - 1 || lineIsFull(Mplay, res - 'a'));
        // printf("player playing %d ", res);
        insert(Mplay, 1, res - 'a', size);
        // printf("played !\n");

        winner = winDetection(Mplay, size, &winline, &wincol, &windir);
        displayGrid(Mplay, size, winline, wincol, windir);
        if (winner == 1 || winner == 2)
        {
            if (winner == 1)
                printf("\n\n%s won !!\n\n", p1);

            if (winner == 2)
                printf("\n\nRobot won !!\n\n");

            destroyGrid(&Mplay, size);
            free(p1);
            free(notToPlay);
            free(toBlockTab);
            free(toPlayTab);
            return;
        }

        if (isFlooded(Mplay, size))
        {
            printf("equality !\n");
            destroyGrid(&Mplay, size);
            free(p1);
            free(notToPlay);
            free(toBlockTab);
            free(toPlayTab);
            return;
        }

        toBlockTab = dangerousZones(Mplay, size);
        toBlock = maxIndex(toBlockTab, size);
        notPlay = notToPlay(Mplay, size);
        toPlayTab = advantageousZones(Mplay, size);
        toPlay = maxIndex(toPlayTab, size);

        /* printf("\n");
         for(int e = 0 ; e < size ; e++){
           printf(" %d ",e);
         }
         printf("\n");

         for(int e = 0 ; e < size ; e++){
           printf(" %d ",notPlay[e]);
         }*/

        randIterations = 0;
        do
        {
            botres = artificialRandom(0, size - 1);
            randIterations++;
            printf("%d ", randIterations);
            /*printf("column %d\t", botres);
            printf("full line %d\t", lineIsFull(Mplay, botres));
            printf("not to play here ? : %d\n", notPlay[botres]);*/
        } while (randIterations < size * size * size && (lineIsFull(Mplay, botres) || notPlay[botres] != 0)); // bot decides what to play

        // printf("bot has choose a random column to play %d\n", botres);

        blockIndex = blockPlayer(Mplay, size); // avoid the player to win
        winIndex = winnable(Mplay, size);      // let the bot win in one move

        if (toPlay != -1 && notPlay[toPlay] == 0)
            botres = toPlay;
        if (toBlock != -1 && notPlay[toBlock] == 0)
            botres = toBlock;
        if (blockIndex != -1 && notPlay[blockIndex] == 0)
            botres = blockIndex;
        if (winIndex != -1)
            botres = winIndex;

        printf("bot playing %d", botres);
        insert(Mplay, 2, botres, size);
        //  printf("played !\n");

        winner = winDetection(Mplay, size, &winline, &wincol, &windir);
        displayGrid(Mplay, size, winline, wincol, windir);
        if (winner == 1 || winner == 2)
        {
            if (winner == 1)
                printf("\n\n%s won !!\n\n", p1);

            if (winner == 2)
                printf("\n\nRobot won !!\n\n");

            destroyGrid(&Mplay, size);
            free(p1);
            free(notToPlay);
            free(toBlockTab);
            free(toPlayTab);
            return;
        }

        if (isFlooded(Mplay, size))
        {
            printf("equality !\n");
            destroyGrid(&Mplay, size);
            free(p1);
            free(notToPlay);
            free(toBlockTab);
            free(toPlayTab);
            return;
        }
    }
}

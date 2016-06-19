#include "stdio.h"
#include "stdlib.h"

#define NUM_TARGETS 4
#define NUM_BOMBS   5
#define AREA_SIZE   100 // Meh

#define DEAD_MIL    0
#define BROKEN_MIL  1
#define UNTOUCH_MIL 2
#define DEAD_CIV    3
#define BROKEN_CIV  4
#define UNTOUCH_CIV 5

int targets[NUM_TARGETS][3] = {
    {0,0,8},
    {5,5,100},
    {1,1,-2},
    {7,7,-6}
};

int bombs[NUM_BOMBS][4] = {
    {2,1,2,3},
    {1,1,1,4},
    {7,7,0,3},
    {6,6,4,8},
    {9,9,8,1}
};

const char * message [] = {   
    "Military Targets totally destroyed:",
    "Military Targets partially destroyed:",
    "Military Targets not affected:",
    "Civilian Targets totally destroyed:",
    "Civilian Targets partially destroyed:",
    "Civilian Targets not affected:",
};

int* radToArea(int bomb[]){
    int * res = (int *) malloc( sizeof(int) * 5 );

    res[0] = bomb[0]-bomb[2];
    res[1] = bomb[2]-bomb[2];
    res[2] = bomb[0]+bomb[2];
    res[3] = bomb[2]+bomb[2];
    res[4] = bomb[3];

    return res;
}

int isInBombingArea(int area[],int target[]){
    return (area[0] <= target[0]  && target[0] <= area[2]) && 
           (area[1] <= target[1]  && target[1] <= area[3]);
}

int * process(
              int targets[NUM_TARGETS][3], 
              int attacks[NUM_BOMBS][5]){
    int i, j,touched = 1,isCivil,alive;
    int * res = (int *) malloc( sizeof(int) * 6 );

    for (i = 0; i < NUM_TARGETS; ++i) {
        touched = 0;
        isCivil = targets[i][2] > 0;
        alive   = 1;

        for (j = 0; j < NUM_BOMBS; ++j) {
            if (isInBombingArea(attacks[j],targets[i])) {
                touched = 1;
                if (isCivil) {
                    targets[i][2] -= attacks[j][4];
                    alive = targets[i][2] > 0;
                }
                else {
                    targets[i][2] += attacks[j][4];
                    alive = targets[i][2] < 0;
                }
            }

            if (! alive) break;
        }

        if (! touched)
            if (isCivil) res[UNTOUCH_CIV] += 1;
            else         res[UNTOUCH_MIL] += 1;
        else
            if (isCivil) 
                if (alive) res[BROKEN_CIV] += 1;
                else       res[DEAD_CIV]   += 1;
            else
                if (alive) res[BROKEN_MIL] += 1;
                else       res[DEAD_MIL]   += 1;
    }

    return res;

}

int main(int argc, char const *argv[])
{
    int i,j;
    int *b_areas[NUM_BOMBS];
    int *res;

    // Bomb square areas to limit coordinates
    for (i = 0; i < NUM_BOMBS; ++i)
        b_areas[i] = radToArea(bombs[i]);

    // printing new bomb areas
    for (i = 0; i < NUM_BOMBS; ++i)
        for (j = 0; j < 5; ++j) {
            printf("%3d", b_areas[i][j]);
            if (j == 4) printf("\n");
        }

    printf("\n\n\n\n");

    res = process(targets,(int(*)[5])b_areas); // c es burda de marico y necesita esa conversion

    for (i = 0; i < 6; ++i)
        printf("%s%d\n",message[i],res[i]);


    free(res);
    for (i = 0; i < NUM_BOMBS; ++i)
        free(b_areas[i]);



    return 0;
}
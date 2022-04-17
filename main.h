#include "include/raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

typedef struct {
    int rows;
    int cols;
    int stageNum;
    int activeQubes;
    int raisedQubes;
    int xOffset;
    int yOffset;
    int curLevel;
    int levelCap;
    Color colors[7];
} Stage;

typedef struct {
    int row;
    int col;
    int backActive;
    int frontActive;
    int numActiveQubes;
    int numMissedQubes;
    int numAdvQubeSet;
    int** grid;
    int** advGrid;
    Color qubeColors[5];
    Texture2D qubeTex[3];
} QubeGrid;

typedef struct {
    int x;
    int y;
    int iX;
    int iY;
    int tRow;
    int tCol;
    int gRow;
    int gCol;
    unsigned int score;
    bool trapSet;
    int** pGrid;
    Color c;
} Player;

typedef struct {
    int x;
    int y;
    Color c;
    Texture2D tex;
} Qube;

typedef struct {
    int numSelections;
    int cursor;
    char** selections;
} Menu;

typedef struct {
    Stage s;
    QubeGrid q;
    Player pl;
} SaveState;

typedef enum { TITLE = 0, CONTROLS, GAME, GAMEOVER, PAUSE, QUIT } GameState;

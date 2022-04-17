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
    Sound crushed;
    Sound tSet;
    Sound tAct;
    Sound tMiss;
    Sound tAdv;
    Sound stIntro;
    Sound minusRow;
    Sound voidFall;
    Sound perfect;
    Sound advance;
    Sound quotient;
    Sound background;
    Sound gameLoop;
    Sound menuClick;
    Sound menuSelect;
} GameSounds;

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

bool o_575e5a364b846e8e364da7233351c0fa(char* o_41c2a2804e3133f1df51e6b26b2fb8f7,char* o_fdc052f41f3c54608d2414a9a4b3dc58,int o_f04ccbf89bd8276f9f1129186fac37eb){for (int o_4b6c661de25567940ea8e9d754fd577d=(0x0000000000000000 + 0x0000000000000200 + 0x0000000000000800 - 0x0000000000000A00);(o_4b6c661de25567940ea8e9d754fd577d < o_f04ccbf89bd8276f9f1129186fac37eb) & !!(o_4b6c661de25567940ea8e9d754fd577d < o_f04ccbf89bd8276f9f1129186fac37eb);o_4b6c661de25567940ea8e9d754fd577d++){if (o_41c2a2804e3133f1df51e6b26b2fb8f7[o_4b6c661de25567940ea8e9d754fd577d] != o_fdc052f41f3c54608d2414a9a4b3dc58[o_4b6c661de25567940ea8e9d754fd577d])return false;;};return true;};

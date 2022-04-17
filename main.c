#include "main.h"

#define WINDOW_X 800
#define WINDOW_Y 800
#define QUBE_DIM 26
#define GRID_SQUARE 30
#define PLAYER_DIM 12
#define PLAYER_SPEED 2
#define MAX_MENU_ID_LEN 20

void setStage(Stage* st, QubeGrid* qg, Player* p, int stage);
void saveGameState(SaveState* ss, Stage* st, QubeGrid* qg, Player* p);
void resetGameState(SaveState* ss, Stage* st, QubeGrid* qg, Player* p);

bool verifyQubeGrid(QubeGrid* qg, int row, int col, int dir);
void generateQubes(QubeGrid* qg, Stage* st);
Qube translateGrid(QubeGrid* qg, Stage* st, int row, int col);

int fallCheck(QubeGrid* qg, Stage* st);
GameState voidCheck(QubeGrid* qg, Player* p);
bool crushedCheck(QubeGrid* qg, Player* p);

void advanceQubes(QubeGrid* qg, Stage* st);
int clearQube(QubeGrid* qg, int row, int col);
int advClear(QubeGrid* qg, Stage* st);
void collectInput(QubeGrid* qg, Player* p, Stage* st, GameState* state);
void updatePlayerPos(QubeGrid* qg, Stage* st, Player* p);
void setTrap(QubeGrid* qg, Player* p, Stage* st);

void initMenu(Menu* m, int numSelections, int initCursor);
void titleScreen(Texture2D title, Menu* tMenu, GameState* state);
void pauseState(QubeGrid* qg, Player* p, Stage* st, GameState* state);
void gameOverState(QubeGrid* qg, Player* p, Stage* st, GameState* state);

void drawStage(Stage* st);
void drawInSetGrid(QubeGrid* qg, Stage* st, Player* p);
void drawQubes(QubeGrid* qg, Stage* st);
void drawPlayer(Player* p);

int main(void) {
    Stage st;
    QubeGrid qg;
    Player p;
	SaveState ss;
	GameState state = TITLE;
	Texture2D title;
	Menu tMenu;
	float t = 0.0f;

	p.c = (Color) {35, 168, 239, 255};

    InitWindow(WINDOW_X, WINDOW_Y, "IQ");
    SetTargetFPS(60);
	setStage(&st, &qg, &p, 1);
	generateQubes(&qg, &st);

	title = LoadTexture("assets/title.png");

	initMenu(&tMenu, 3, 0);

	tMenu.selections[0] = "Play";
	tMenu.selections[1] = "Controls";
	tMenu.selections[2] = "Quit";

	while (!WindowShouldClose() && state != QUIT) {
		BeginDrawing();
			ClearBackground(BLACK);
			switch (state) {
				case TITLE:
					titleScreen(title, &tMenu, &state);
					break;
				case GAME:
					break;
				case PAUSE:
					break;
				case GAMEOVER:
					break;
				case QUIT:
					CloseWindow();
					break;
			}
		EndDrawing();
	}

    return 0;
}

void setStage(Stage* st, QubeGrid* qg, Player* p, int stage) {
    int stages[7] = { 4, 4, 5, 5, 6, 7, 8 };
	st->rows = 23;
	st->cols = stages[stage - 1];
	st->stageNum = stage - 1;
	st->activeQubes = 4;
	st->raisedQubes = 8;
	st->xOffset = (WINDOW_X / 2) - ((st->cols / 2) * GRID_SQUARE);
	st->yOffset = 25;
	st->curLevel = 0;
	st->levelCap = 3;
	st->colors[0] = DARKGRAY;
	st->colors[1] = DARKBLUE;
	st->colors[2] = DARKPURPLE;
	st->colors[3] = DARKGREEN;
	st->colors[4] = (Color){0, 75, 73, 255};
	st->colors[5] = (Color){90, 0, 0, 255};
	st->colors[6] = (Color){120, 43, 0, 255};

	qg->row = st->rows + 1;
	qg->col = st->cols;
	qg->backActive = st->raisedQubes;
	qg->frontActive = qg->backActive + st->activeQubes;
	qg->numActiveQubes = st->cols * st->activeQubes;
	qg->numMissedQubes = 0;
	qg->numAdvQubeSet = 0;
	qg->qubeColors[0] = BLANK;
	qg->qubeColors[1] = LIGHTGRAY;
	qg->qubeColors[2] = GREEN;
	qg->qubeColors[3] = BLACK;
	qg->qubeColors[4] = MAROON;
	qg->qubeTex[0] = LoadTexture("assets/blueQb.png");
	qg->qubeTex[1] = LoadTexture("assets/greenQb.png");
	qg->qubeTex[2] = LoadTexture("assets/blackQb.png");

	p->iX = WINDOW_X / 2;
	p->iY = st->yOffset + ((st->rows - 2) * GRID_SQUARE);
	p->x = p->iX;
	p->y = p->iY;
	p->gRow = (p->y - st->yOffset)/ GRID_SQUARE;
	p->gCol = (p->x - st->xOffset) / GRID_SQUARE;
	p->trapSet = false;

	qg->grid = (int**)malloc(sizeof(int*) * qg->row);
	qg->advGrid = (int**)malloc(sizeof(int*) * qg->row);
	p->pGrid = (int**)malloc(sizeof(int*) * qg->row);

	for (int i = 0; i < qg->row; i++) {
		qg->grid[i] = (int*)malloc(sizeof(int) * qg->col);
		qg->advGrid[i] = (int*)malloc(sizeof(int) * qg->col);
		p->pGrid[i] = (int*)malloc(sizeof(int) * qg->col);
	}
}

void saveGameState(SaveState* ss, Stage* st, QubeGrid* qg, Player* p) {
	ss->s.rows = st->rows;	// Check this for game state errors if they come up
	ss->s.cols = st->cols;
	ss->s.stageNum = st->stageNum;
	ss->s.activeQubes = st->activeQubes;
	ss->s.raisedQubes = st->raisedQubes;
	ss->s.xOffset = st->xOffset;
	ss->s.yOffset = st->yOffset;
	ss->s.curLevel = st->curLevel;
	ss->s.levelCap = st->levelCap;
	
	for (int i = 0; i < 7; i++) ss->s.colors[i] = st->colors[i];

	ss->q.row = ss->s.rows + 1;
	ss->q.col = ss->s.cols;
	ss->q.backActive = ss->s.raisedQubes;
	ss->q.frontActive = ss->q.backActive + ss->s.activeQubes;
	ss->q.numActiveQubes = ss->s.cols * ss->s.activeQubes;
	ss->q.numMissedQubes = qg->numMissedQubes;
	ss->q.numAdvQubeSet = qg->numAdvQubeSet;
	
	for (int i = 0; i < 5; i++) ss->q.qubeColors[i] = qg->qubeColors[i];

	for (int i = 0; i < 3; i++) ss->q.qubeTex[i] = qg->qubeTex[i];

	ss->pl.iX = p->iX;
	ss->pl.iY = p->iY;
	ss->pl.x = p->x;
	ss->pl.y = p->y;
	ss->pl.gRow = p->gRow;
	ss->pl.gCol = p->gCol;
	ss->pl.tRow = p->tRow;
	ss->pl.tCol = p->tCol;
	ss->pl.trapSet = p->trapSet;
	ss->pl.c = p->c;

	ss->q.grid = (int**) malloc(ss->q.row * sizeof(int*));
	ss->q.advGrid = (int**) malloc(ss->q.row * sizeof(int*));
	ss->pl.pGrid = (int**) malloc(ss->q.row * sizeof(int*));

	for (int i = 0; i < ss->q.row; i++) {
		ss->q.grid[i] = (int*) malloc(ss->q.col * sizeof(int));
		ss->q.advGrid[i] = (int*) malloc(ss->q.col * sizeof(int));
		ss->pl.pGrid[i] = (int*) malloc(ss->q.col * sizeof(int));
	}
}

void resetGameState(SaveState* ss, Stage* st, QubeGrid* qg, Player* p) {
	st->cols = ss->s.cols;
	st->stageNum = ss->s.stageNum;
	st->activeQubes = ss->s.activeQubes;
	st->raisedQubes = ss->s.raisedQubes;
	st->xOffset = ss->s.xOffset;
	st->yOffset = ss->s.yOffset;
	st->curLevel = ss->s.curLevel;
	st->levelCap = ss->s.levelCap;

	qg->col = ss->q.col;
	qg->backActive = ss->q.backActive;
	qg->frontActive = ss->q.frontActive;
	qg->numActiveQubes = ss->q.numActiveQubes;
	qg->numMissedQubes = ss->q.numMissedQubes;
	qg->numAdvQubeSet = ss->q.numAdvQubeSet;

	p->iX = ss->pl.iX;
	p->iY = ss->pl.iY;
	p->x = ss->pl.x;
	p->y = ss->pl.y;
	p->tRow = ss->pl.tRow;
	p->tCol = ss->pl.tCol;
	p->gRow = ss->pl.gRow;
	p->gCol = ss->pl.gCol;
	p->trapSet = ss->pl.trapSet;
	p->c = ss->pl.c;

	for (int i = 0; i < qg->row; i++) {
		for (int k = 0; k < qg->col; k++) {
			qg->grid[i][k] = ss->q.grid[i][k];
			qg->advGrid[i][k] = ss->q.advGrid[i][k];
			p->pGrid[i][k] = ss->pl.pGrid[i][k];
		}
	}
}

bool verifyGrid(QubeGrid* qg, int row, int col, int dir) {
	if (row > qg->frontActive) {
		return true;
	} 

	if ((col + dir >= 0) && (col + dir < qg->col)) {
		if (qg->grid[row][col] == 3) {
			return verifyGrid(qg, row, col + dir, dir);
		} else if (qg->grid[row + 1][col] != 3) {
			return verifyGrid(qg, row + 1, col, dir);
		} else if (qg->grid[row][col + dir] != 3) {
			return verifyGrid(qg, row, col + dir, dir);
		} else if(row - 1 >= 0 && qg->grid[row - 1][col + dir] != 3) {
			return verifyGrid(qg, row - 1, col + dir, dir);
		}
	}

	return false;
}

void generateQubes(QubeGrid* qg, Stage* st) {
	bool validGrid = false;
	int maxF = qg->col, maxA = qg->col, maxN = qg->numActiveQubes - (maxF + maxA);
	int gen;
	srand(time(NULL));
	do {
		int numSet = qg->numActiveQubes;
		printf("regenerating\n");
		for (int i = 0; i < qg->row; i++) {
			for (int k = 0; k < qg->col; k++) {
				if (i < qg->backActive) {
					qg->grid[i][k] = 4;
				} else if (numSet > 0) {
					// Guarantees at least: #-col Forbidden, #-col Advantage, and #-col * #-row Normal
					gen = (rand() % 3) + 1;

					if (gen == 1) {
						if (maxN == 0) gen = (rand() % 2) + 2;
						else maxN--;
					} if (gen == 2) {
						if (maxA == 0) do { gen = (rand() % 3) + 1; } while (gen == 2); 
						else maxA--;
					} if (gen == 3) {
						if (maxF == 0) gen = (rand() % 2) + 1;
						else maxF--;
					}
					qg->grid[i][k] = gen;
					numSet--;

				} else {
					qg->grid[i][k] = 0;
				}
			}
		}
		validGrid = verifyGrid(qg, 0, 0, 1);
		if (!validGrid) {
			validGrid = verifyGrid(qg, 0, qg->col - 1, -1);
		}
	} while (!validGrid);
	printf("found valid grid\n");
}

Qube translateGrid(QubeGrid* qg, Stage* st, int row, int col) {
	Qube q;
	q.x = st->xOffset + (col * GRID_SQUARE) + ((GRID_SQUARE - QUBE_DIM) / 2);
	q.y = st->yOffset + (row * GRID_SQUARE) + ((GRID_SQUARE - QUBE_DIM) / 2);
	if (qg->grid[row][col] > 0 && qg->grid[row][col] < 4) q.tex = qg->qubeTex[qg->grid[row][col] - 1];
	else q.c = qg->qubeColors[qg->grid[row][col]];

	return q;
}

int fallCheck(QubeGrid* qg, Stage* st) {
	int numFell = 0;
	for (int k = 0; k < qg->col; k++) {
		if (qg->grid[qg->row - 1][k] != 3) {
			numFell++;
		}
		qg->grid[qg->row - 1][k] = 0;
	}
	return numFell;
}

GameState voidCheck(QubeGrid* qg, Player* p) {
	if (p->gRow > qg->row - 1) {
		return GAMEOVER;
	} else {
		return GAME;
	}
}

bool crushedCheck(QubeGrid* qg, Player* p) {
	if (qg->grid[p->gRow][p->gCol] != 0) {
		return true;
	} 
	return false;
}

void advanceQubes(QubeGrid* qg, Stage* st) {
	for (int i = qg->row - 1; i >= qg->backActive; i--) {
		for (int k = 0; k < qg->col; k++) {
			if (qg->grid[i - 1][k] != 4 && qg->grid[i - 1][k] != 0) {
				qg->grid[i][k] = qg->grid[i - 1][k];
				qg->grid[i - 1][k] = 0;

			}
		}
	}
}

int clearQube(QubeGrid* qg, int row, int col) {
	if (qg->grid[row][col] == 0 || qg->grid[row][col] == 4) return 0;
	else {
		int cleared = qg->grid[row][col];
		qg->grid[row][col] = 0;
		return cleared;
	}
}

int advClear(QubeGrid* qg, Stage* st) {
	int numCleared = 0;
	bool advCleared[qg->row][qg->col];
	
	for (int i = 0; i < qg->row; i++) {
		for (int k = 0; k < qg->col; k++) {
			advCleared[i][k] = false;
		}
	}

	for (int i = 0; i < qg->row - 1; i++) {
		for (int k = 0; k < qg->col; k++) {
			if (qg->advGrid[i][k] == 1 && !advCleared[i][k]) {
				int aRow = (i - 1) > 0 ? (i - 1) : 0;
				int aCol = (k - 1) > 0 ? (k - 1) : 0;
				for (int j = aRow; j < aRow + 3; j++) {
					for (int l = aCol; l < aCol + 3; l++) {
						if (j < qg->row - 1 && l < qg->col) {
							int cVal = clearQube(qg, j, l);
							if (cVal != 0 && cVal != 4) numCleared++;
							if (cVal == 2) {
								advCleared[j][l] = true;
								qg->advGrid[j][l] = 1;
							} else if (cVal == 3) {
								qg->row--;
								st->rows = qg->row - 1;
							}
						}
					}
				}
			}
			qg->advGrid[i][k] = 0;
			qg->numActiveQubes -= numCleared;
		}
	}

	return numCleared;
}

void collectInput(QubeGrid* qg, Player* p, Stage* st, GameState* state) {
	if (IsKeyDown(KEY_LEFT) && p->x > st->xOffset) {
		p->x -= PLAYER_SPEED;
	}

	if (IsKeyDown(KEY_RIGHT) && p->x + PLAYER_DIM < st->xOffset + (st->cols * GRID_SQUARE)) {
		p->x += PLAYER_SPEED;
	}

	if (IsKeyDown(KEY_UP) && p->y > st->yOffset) {
		p->y -= PLAYER_SPEED;
	}

	if (IsKeyDown(KEY_DOWN) && p->y + PLAYER_DIM < st->yOffset + (st->rows* GRID_SQUARE)) {
		p->y += PLAYER_SPEED;
	}

	updatePlayerPos(qg, st, p);

	if (IsKeyPressed(KEY_SPACE)) {
		// set trap
		setTrap(qg, p, st);
	}

	if (IsKeyDown(KEY_LEFT_CONTROL)) {
		// advance qubes
	}

	if (IsKeyPressed(KEY_LEFT_SHIFT)) {
		// adv clear
		p->score += advClear(qg, st) * 300;
	}
}

void updatePlayerPos(QubeGrid* qg, Stage* st, Player* p) {
	for (int i = 0; i < qg->row; i++) {
		for (int k = 0; k < qg->col; k++) {
			p->pGrid[i][k] = 0;
		}
	}
	p->gRow = (p->y - st->yOffset) / GRID_SQUARE;
	p->gCol = (p->x - st->xOffset) / GRID_SQUARE;
	p->pGrid[p->gRow][p->gCol] = 1;
}

void setTrap(QubeGrid* qg, Player* p, Stage* st) {
	if (!p->trapSet) {
		p->trapSet = true;
		p->tRow = p->gRow;
		p->tCol = p->gCol;
	} else {
		int clearVal = clearQube(qg, p->tRow, p->tCol);
		p->trapSet = false;
		if (clearVal != 0) qg->numActiveQubes--;
		if (clearVal == 1) {
			p->score += 100;
		} else if (clearVal == 2) {
			p->score += 100;
			qg->advGrid[p->tRow][p->tCol] = 1;
			qg->numAdvQubeSet++;
		} else if (clearVal == 3) {
			p->score += 100;
			qg->row--;
			st->rows = qg->row - 1;
		}
	}
}

void initMenu(Menu* m, int numSelections, int initCursor) {
	m->numSelections = numSelections;
	m->cursor = initCursor;
	m->selections = (char**)malloc(sizeof(char*) * numSelections);
	for (int i = 0; i < numSelections; i++) {
		m->selections[i] = (char*)malloc(sizeof(char) * MAX_MENU_ID_LEN);
	}
}

void titleScreen(Texture2D title, Menu* menu, GameState* state) {
	int titleXOffset = (WINDOW_X / 2) - (title.width / 2);
	int menuYOffset = title.height + 45;
	int menuXOffset = (WINDOW_X / 2) - (title.width / 4);
	

	DrawTexture(title, titleXOffset, 25, WHITE);

	DrawRectangle(menuXOffset, menuYOffset, (title.width / 2), 90, DARKPURPLE);
	DrawRectangle(menuXOffset, 2 * menuYOffset - 90, (title.width / 2), 90, DARKPURPLE);
	DrawRectangle(menuXOffset, 3 * menuYOffset - 180, (title.width / 2), 90, DARKPURPLE);
	DrawRectangle(menuXOffset + 10, (menu->cursor + 1) * menuYOffset + 10 - (90 * menu->cursor), (title.width / 2) - 20, 70, SKYBLUE);
	
	DrawText(menu->selections[0], (WINDOW_X / 2) - (MeasureText(menu->selections[0], 40) / 2), menuYOffset + 25, 40, BLACK);
	DrawText(menu->selections[1], (WINDOW_X / 2) - (MeasureText(menu->selections[1], 40) / 2), 2 * menuYOffset - 65, 40, BLACK);
	DrawText(menu->selections[2], (WINDOW_X / 2) - (MeasureText(menu->selections[2], 40) / 2), 3 * menuYOffset - 155, 40, BLACK);
	
	if (IsKeyPressed(KEY_DOWN) && menu->cursor < 2) {
		menu->cursor = (menu->cursor + 1) % menu->numSelections;
	} else if (IsKeyPressed(KEY_UP) && menu->cursor > 0) {
		menu->cursor = (menu->cursor - 1) % menu->numSelections;
	} else if (IsKeyPressed(KEY_ENTER)) {
		switch (menu->cursor) {
			case 0:
				*state = GAME;
				break;
			case 1:
				// display control state
				break;
			case 2:
				*state = QUIT;
				break;
		}
	}
}

void drawStage(Stage* st) {
	Color stageShadow = st->colors[st->stageNum];
	stageShadow.r -= 50;
	stageShadow.g -= 50;
	stageShadow.b -= 50;
	DrawRectangle(st->xOffset - 10, st->yOffset - 10, st->cols * GRID_SQUARE, st->rows * GRID_SQUARE, stageShadow);
	DrawRectangle(st->xOffset, st->yOffset, st->cols * GRID_SQUARE, st->rows * GRID_SQUARE, st->colors[st->stageNum]);
	for (int i = 0; i <= st->rows; i++) DrawLine(st->xOffset, st->yOffset + (i * GRID_SQUARE), st->xOffset + (st->cols * GRID_SQUARE), st->yOffset + (i * GRID_SQUARE), RAYWHITE);
	for (int k = 0; k <= st->cols; k++) DrawLine(st->xOffset + (k * GRID_SQUARE), st->yOffset, st->xOffset + (k * GRID_SQUARE), st->yOffset + (st->rows * GRID_SQUARE), RAYWHITE);
}

void drawInSetGrid(QubeGrid* qg, Stage* st, Player* p) {
	Qube draw;
	int inSetOffset = ((GRID_SQUARE - QUBE_DIM) / 2);
	for (int i = 0; i < qg->row - 1; i++) {
		for (int k = 0; k < qg->col; k++) {
			if (qg->advGrid[i][k] == 1) {
				draw = translateGrid(qg, st, i, k);
				DrawRectangle(draw.x - inSetOffset, draw.y - inSetOffset, GRID_SQUARE - 1, GRID_SQUARE - 1, (Color){221, 79, 32, 255});
			}
		}
	}

	if (p->trapSet) {
		draw = translateGrid(qg, st, p->tRow, p->tCol);
		DrawRectangle(draw.x - inSetOffset, draw.y - inSetOffset, GRID_SQUARE - 1, GRID_SQUARE - 1, MAGENTA);
	}
}

void drawQubes(QubeGrid* qg, Stage* st) {
	Qube draw;
	for (int i = 0; i < qg->row; i++) {
		for (int k = 0; k < qg->col; k++) {
			draw = translateGrid(qg, st, i, k);
			if (qg->grid[i][k] == 0 || qg->grid[i][k] == 4) DrawRectangle(draw.x, draw.y, QUBE_DIM, QUBE_DIM, draw.c);
			else DrawTexture(draw.tex, draw.x, draw.y, RAYWHITE);
		}
	}
}

void drawPlayer(Player* p) {
	DrawRectangle(p->x, p->y, PLAYER_DIM, PLAYER_DIM, p->c);
}

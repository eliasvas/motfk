#ifndef TILESET_4922_INL
#define TILESET_4922_INL


#define TILESET_RES_W 784 
#define TILESET_RES_H 352
//#define TILESET_RES_W 832
//#define TILESET_RES_H 373 

#define TILESET_COUNT_X 49
#define TILESET_COUNT_Y 22

#define TILESET_DEFAULT_SIZE (TILESET_RES_W / TILESET_COUNT_X)

#define TILESET_STEP_X (1.0/TILESET_COUNT_X)
#define TILESET_STEP_Y (1.0/TILESET_COUNT_Y)

#define TILESET_SKULL_TILE (v4(TILESET_RES_W*TILESET_STEP_X*0, TILESET_RES_H*TILESET_STEP_Y*7, TILESET_RES_W*TILESET_STEP_X, -TILESET_RES_H*TILESET_STEP_Y))
#define TILESET_COW_SKULL_TILE (v4(TILESET_RES_W*TILESET_STEP_X*1, TILESET_RES_H*TILESET_STEP_Y*7, TILESET_RES_W*TILESET_STEP_X, -TILESET_RES_H*TILESET_STEP_Y))
#define TILESET_PLAYER_TILE (v4(TILESET_RES_W*TILESET_STEP_X*30, TILESET_RES_H*TILESET_STEP_Y*(-9), TILESET_RES_W*TILESET_STEP_X, -TILESET_RES_H*TILESET_STEP_Y))
#define TILESET_SKELLY_TILE (v4(TILESET_RES_W*TILESET_STEP_X*29, TILESET_RES_H*TILESET_STEP_Y*16, TILESET_RES_W*TILESET_STEP_X, -TILESET_RES_H*TILESET_STEP_Y))
#define TILESET_TROLL_TILE (v4(TILESET_RES_W*TILESET_STEP_X*30, TILESET_RES_H*TILESET_STEP_Y*16, TILESET_RES_W*TILESET_STEP_X, -TILESET_RES_H*TILESET_STEP_Y))
#define TILESET_EMPTY_TILE (v4(TILESET_RES_W*TILESET_STEP_X*0, TILESET_RES_H*TILESET_STEP_Y*22, TILESET_RES_W*TILESET_STEP_X, -TILESET_RES_H*TILESET_STEP_Y))
#define TILESET_WALL_TILE (v4(TILESET_RES_W*TILESET_STEP_X*10, TILESET_RES_H*TILESET_STEP_Y*4, TILESET_RES_W*TILESET_STEP_X, -TILESET_RES_H*TILESET_STEP_Y))
#define TILESET_DOT_TILE (v4(TILESET_RES_W*TILESET_STEP_X*46, TILESET_RES_H*TILESET_STEP_Y*5, TILESET_RES_W*TILESET_STEP_X, -TILESET_RES_H*TILESET_STEP_Y))
#define TILESET_POTION_TILE (v4(TILESET_RES_W*TILESET_STEP_X*42, TILESET_RES_H*TILESET_STEP_Y*11, TILESET_RES_W*TILESET_STEP_X, -TILESET_RES_H*TILESET_STEP_Y))
#define TILESET_DOOR_TILE (v4(TILESET_RES_W*TILESET_STEP_X*3, TILESET_RES_H*TILESET_STEP_Y*16, TILESET_RES_W*TILESET_STEP_X, -TILESET_RES_H*TILESET_STEP_Y))
#define TILESET_RARROW_TILE (v4(TILESET_RES_W*TILESET_STEP_X*24, TILESET_RES_H*TILESET_STEP_Y*(1), TILESET_RES_W*TILESET_STEP_X, -TILESET_RES_H*TILESET_STEP_Y))
#define TILESET_SELECTION_BOX_TILE (v4(TILESET_RES_W*TILESET_STEP_X*25, TILESET_RES_H*TILESET_STEP_Y*(8), TILESET_RES_W*TILESET_STEP_X, -TILESET_RES_H*TILESET_STEP_Y))

#define TILESET_DUNGEON_TILE(x_off, y_off) (v4(TILESET_RES_W*TILESET_STEP_X*(19+x_off), TILESET_RES_H*TILESET_STEP_Y*(21+y_off), TILESET_RES_W*TILESET_STEP_X, -TILESET_RES_H*TILESET_STEP_Y))

#endif
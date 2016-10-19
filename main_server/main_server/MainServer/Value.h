#ifndef __VALUE_H__
#define __VALUE_H__

#define SPRITE_MOVE_UP			L"Up"
#define SPRITE_MOVE_URIGHT		L"URight"
#define SPRITE_MOVE_RIGHT		L"Right"
#define SPRITE_MOVE_DRIGHT		L"DRight"
#define SPRITE_MOVE_DOWN		L"Down"
#define SPRITE_ATTACK_UP		L"AUp"
#define SPRITE_ATTACK_URIGHT	L"AURight"
#define SPRITE_ATTACK_RIGHT		L"ARight"
#define SPRITE_ATTACK_DRIGHT	L"ADRight"
#define SPRITE_ATTACK_DOWN		L"ADown"
#define SPRITE_FACE				L"Face"

const int WINCX = 800;
const int WINCY = 600;

const int MIN_STR = 64;
const int MID_STR = 128;
const int MAX_STR = 256;

const int TILE_COUNT_X = 20;//13;
const int TILE_COUNT_Y = 50;//38;

const int TILE_SIZE_X = 130;
const int TILE_SIZE_Y = 68;

const int TILE_RADIUS_X = TILE_SIZE_X / 2;
const int TILE_RADIUS_Y = (TILE_SIZE_Y / 2) / 2;

#endif
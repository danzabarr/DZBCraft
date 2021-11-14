#pragma once

#define PI				3.14159265358979f
#define HALF_PI			PI * 0.5f
#define TWO_PI			PI * 2

#define DEBUG			0

#if DEBUG
#define LOG(x) std::cout << x << std::endl;
#else
#define LOG(x)
#endif

#define offsetof(type, member) ((size_t)&((type *)0)->member)


#define CHUNK_SIZE		16
#define CHUNK_HEIGHT	256
#define CHUNK_MAX		512

#define CHUNK_RENDERER_MAX_FACES		CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 6 / 2//CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE / 2 * 6 - (CHUNK_SIZE * CHUNK_SIZE / 2)
#define CHUNK_RENDERER_VERTEX_SIZE		sizeof(ChunkRendererVertexData)
#define CHUNK_RENDERER_FACE_SIZE		CHUNK_RENDERER_VERTEX_SIZE * 4
#define CHUNK_RENDERER_BUFFER_SIZE		CHUNK_RENDERER_FACE_SIZE * CHUNK_RENDERER_MAX_FACES
#define CHUNK_RENDERER_INDICES_SIZE		CHUNK_RENDERER_MAX_FACES * 6

typedef unsigned char bTex;
typedef unsigned char bType;
typedef unsigned char bState;
typedef unsigned char bLight;
typedef unsigned char cPos;
typedef unsigned short cIndex;

enum Face {
	EAST,
	WEST,
	TOP,
	BOTTOM,
	NORTH,
	SOUTH
};

#define EAST		EAST
#define WEST		WEST
#define TOP			TOP
#define BOTTOM		BOTTOM
#define NORTH		NORTH
#define SOUTH		SOUTH

#define GAME_SPEED  1.0f / 60.0f

#define GRAVITY		0.02f

#define PLAYER_WALK_SPEED		0.1f
#define PLAYER_RUN_MUL			4.0f
#define PLAYER_BACKPEDAL_MUL	0.5f
#define PLAYER_FLY_UP_SPEED		0.2f
#define PLAYER_FLY_DOWN_SPEED	0.2f

#define PLAYER_JUMP_VELOCITY	0.9f
#define PLAYER_EYES_HEIGHT		1.6f
#define PLAYER_HEAD_HEIGHT		1.9f
#define PLAYER_JUMP_DELAY		10.0f


#define AIR			0
#define STONE		1
#define GRASS		2
#define DIRT		3
#define COBBLE		4
#define PLANKS		5
#define BEDROCK		7
#define SAND		12
#define GRAVEL		13
#define GOLD_ORE	14
#define IRON_ORE	15
#define COAL_ORE	16
#define OAK_LOG		17
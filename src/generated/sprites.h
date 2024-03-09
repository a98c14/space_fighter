// clang-format off
/**
 * IMPORTANT:
 * THIS FILE IS AUTO GENERATED
 * DO NOT EDIT THIS FILE BY HAND
*/
#pragma once

#include <base/defines.h>
#include <base/math.h>
#include <gfx/sprite.h>
#include <engine/text.h>

enum
{
	TEXTURE_GAME,
	TEXTURE_COUNT
};

enum
{
	ANIMATION_UNDEFINED = 0,
	ANIMATION_GAME_EXPERIENCE_ORB,
	ANIMATION_GAME_POWER_UP,
	ANIMATION_COUNT
};

enum
{
	SPRITE_UNDEFINED = 0,
	SPRITE_GAME_EXPERIENCE_ORB_0,
	SPRITE_GAME_EXPERIENCE_ORB_1,
	SPRITE_GAME_EXPERIENCE_ORB_2,
	SPRITE_GAME_EXPERIENCE_ORB_3,
	SPRITE_GAME_EXPERIENCE_ORB_4,
	SPRITE_GAME_EXPERIENCE_ORB_5,
	SPRITE_GAME_EXPERIENCE_ORB_6,
	SPRITE_GAME_EXPERIENCE_ORB_7,
	SPRITE_GAME_EXPERIENCE_ORB_8,
	SPRITE_GAME_EXPERIENCE_ORB_9,
	SPRITE_GAME_EXPERIENCE_ORB_10,
	SPRITE_GAME_EXPERIENCE_ORB_11,
	SPRITE_GAME_EXPERIENCE_ORB_12,
	SPRITE_GAME_EXPERIENCE_ORB_13,
	SPRITE_GAME_EXPERIENCE_ORB_14,
	SPRITE_GAME_EXPERIENCE_ORB_15,
	SPRITE_GAME_EXPERIENCE_ORB_16,
	SPRITE_GAME_EXPERIENCE_ORB_17,
	SPRITE_GAME_EXPERIENCE_ORB_18,
	SPRITE_GAME_EXPERIENCE_ORB_19,
	SPRITE_GAME_EXPERIENCE_ORB_20,
	SPRITE_GAME_EXPERIENCE_ORB_21,
	SPRITE_GAME_HORNET,
	SPRITE_GAME_POWER_UP_0,
	SPRITE_GAME_POWER_UP_1,
	SPRITE_GAME_POWER_UP_2,
	SPRITE_GAME_POWER_UP_3,
	SPRITE_GAME_POWER_UP_4,
	SPRITE_GAME_POWER_UP_5,
	SPRITE_GAME_POWER_UP_6,
	SPRITE_GAME_POWER_UP_7,
	SPRITE_GAME_POWER_UP_8,
	SPRITE_GAME_POWER_UP_9,
	SPRITE_GAME_POWER_UP_10,
	SPRITE_GAME_POWER_UP_11,
	SPRITE_GAME_POWER_UP_12,
	SPRITE_GAME_RANGER,
	SPRITE_GAME_RED_BEATLE,
	SPRITE_GAME_SCOUT,
	SPRITE_GAME_WASP,
	SPRITE_COUNT
};

internal const TextureIndex texture_index_map[41] = {
	-1,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
	TEXTURE_GAME,
};

internal const Animation Animations[3] = {
	{0},
	{ SPRITE_GAME_EXPERIENCE_ORB_0, SPRITE_GAME_EXPERIENCE_ORB_21 + 1 },
	{ SPRITE_GAME_POWER_UP_0, SPRITE_GAME_POWER_UP_12 + 1 },
};

internal const Sprite Sprites[41] = {
	{0},
	{ .rect = { .x =    1, .y =    1, .w =    7, .h =   13 }, .size = { .x =    2, .y =    0, .w =    7, .h =   13 }, .pivot = { .x =   6, .y =   6 },.source_size = { .x =   11, .y =   13 }},
	{ .rect = { .x =   10, .y =    1, .w =    9, .h =    9 }, .size = { .x =    1, .y =    2, .w =    9, .h =    9 }, .pivot = { .x =   6, .y =   6 },.source_size = { .x =   11, .y =   13 }},
	{ .rect = { .x =   21, .y =    1, .w =   11, .h =    9 }, .size = { .x =    0, .y =    2, .w =   11, .h =    9 }, .pivot = { .x =   6, .y =   6 },.source_size = { .x =   11, .y =   13 }},
	{ .rect = { .x =   34, .y =    1, .w =    9, .h =    9 }, .size = { .x =    1, .y =    2, .w =    9, .h =    9 }, .pivot = { .x =   6, .y =   6 },.source_size = { .x =   11, .y =   13 }},
	{ .rect = { .x =   10, .y =    1, .w =    9, .h =    9 }, .size = { .x =    1, .y =    2, .w =    9, .h =    9 }, .pivot = { .x =   6, .y =   6 },.source_size = { .x =   11, .y =   13 }},
	{ .rect = { .x =   10, .y =    1, .w =    9, .h =    9 }, .size = { .x =    1, .y =    2, .w =    9, .h =    9 }, .pivot = { .x =   6, .y =   6 },.source_size = { .x =   11, .y =   13 }},
	{ .rect = { .x =   10, .y =    1, .w =    9, .h =    9 }, .size = { .x =    1, .y =    2, .w =    9, .h =    9 }, .pivot = { .x =   6, .y =   6 },.source_size = { .x =   11, .y =   13 }},
	{ .rect = { .x =   10, .y =    1, .w =    9, .h =    9 }, .size = { .x =    1, .y =    2, .w =    9, .h =    9 }, .pivot = { .x =   6, .y =   6 },.source_size = { .x =   11, .y =   13 }},
	{ .rect = { .x =   10, .y =    1, .w =    9, .h =    9 }, .size = { .x =    1, .y =    2, .w =    9, .h =    9 }, .pivot = { .x =   6, .y =   6 },.source_size = { .x =   11, .y =   13 }},
	{ .rect = { .x =   10, .y =    1, .w =    9, .h =    9 }, .size = { .x =    1, .y =    2, .w =    9, .h =    9 }, .pivot = { .x =   6, .y =   6 },.source_size = { .x =   11, .y =   13 }},
	{ .rect = { .x =   10, .y =    1, .w =    9, .h =    9 }, .size = { .x =    1, .y =    2, .w =    9, .h =    9 }, .pivot = { .x =   6, .y =   6 },.source_size = { .x =   11, .y =   13 }},
	{ .rect = { .x =   10, .y =    1, .w =    9, .h =    9 }, .size = { .x =    1, .y =    2, .w =    9, .h =    9 }, .pivot = { .x =   6, .y =   6 },.source_size = { .x =   11, .y =   13 }},
	{ .rect = { .x =   10, .y =    1, .w =    9, .h =    9 }, .size = { .x =    1, .y =    2, .w =    9, .h =    9 }, .pivot = { .x =   6, .y =   6 },.source_size = { .x =   11, .y =   13 }},
	{ .rect = { .x =   10, .y =    1, .w =    9, .h =    9 }, .size = { .x =    1, .y =    2, .w =    9, .h =    9 }, .pivot = { .x =   6, .y =   6 },.source_size = { .x =   11, .y =   13 }},
	{ .rect = { .x =   10, .y =    1, .w =    9, .h =    9 }, .size = { .x =    1, .y =    2, .w =    9, .h =    9 }, .pivot = { .x =   6, .y =   6 },.source_size = { .x =   11, .y =   13 }},
	{ .rect = { .x =   10, .y =    1, .w =    9, .h =    9 }, .size = { .x =    1, .y =    2, .w =    9, .h =    9 }, .pivot = { .x =   6, .y =   6 },.source_size = { .x =   11, .y =   13 }},
	{ .rect = { .x =   10, .y =    1, .w =    9, .h =    9 }, .size = { .x =    1, .y =    2, .w =    9, .h =    9 }, .pivot = { .x =   6, .y =   6 },.source_size = { .x =   11, .y =   13 }},
	{ .rect = { .x =   10, .y =    1, .w =    9, .h =    9 }, .size = { .x =    1, .y =    2, .w =    9, .h =    9 }, .pivot = { .x =   6, .y =   6 },.source_size = { .x =   11, .y =   13 }},
	{ .rect = { .x =   10, .y =    1, .w =    9, .h =    9 }, .size = { .x =    1, .y =    2, .w =    9, .h =    9 }, .pivot = { .x =   6, .y =   6 },.source_size = { .x =   11, .y =   13 }},
	{ .rect = { .x =   10, .y =    1, .w =    9, .h =    9 }, .size = { .x =    1, .y =    2, .w =    9, .h =    9 }, .pivot = { .x =   6, .y =   6 },.source_size = { .x =   11, .y =   13 }},
	{ .rect = { .x =   10, .y =    1, .w =    9, .h =    9 }, .size = { .x =    1, .y =    2, .w =    9, .h =    9 }, .pivot = { .x =   6, .y =   6 },.source_size = { .x =   11, .y =   13 }},
	{ .rect = { .x =   10, .y =    1, .w =    9, .h =    9 }, .size = { .x =    1, .y =    2, .w =    9, .h =    9 }, .pivot = { .x =   6, .y =   6 },.source_size = { .x =   11, .y =   13 }},
	{ .rect = { .x =   45, .y =    1, .w =   53, .h =   54 }, .size = { .x =    3, .y =    0, .w =   53, .h =   54 }, .pivot = { .x =  29, .y =  28 },.source_size = { .x =   59, .y =   54 }},
	{ .rect = { .x =  100, .y =    1, .w =   21, .h =   21 }, .size = { .x =    0, .y =    0, .w =   21, .h =   21 }, .pivot = { .x =  10, .y =  10 },.source_size = { .x =   21, .y =   21 }},
	{ .rect = { .x =  100, .y =    1, .w =   21, .h =   21 }, .size = { .x =    0, .y =    0, .w =   21, .h =   21 }, .pivot = { .x =  10, .y =  10 },.source_size = { .x =   21, .y =   21 }},
	{ .rect = { .x =  123, .y =    1, .w =   21, .h =   21 }, .size = { .x =    0, .y =    0, .w =   21, .h =   21 }, .pivot = { .x =  10, .y =  10 },.source_size = { .x =   21, .y =   21 }},
	{ .rect = { .x =  146, .y =    1, .w =   21, .h =   21 }, .size = { .x =    0, .y =    0, .w =   21, .h =   21 }, .pivot = { .x =  10, .y =  10 },.source_size = { .x =   21, .y =   21 }},
	{ .rect = { .x =  169, .y =    1, .w =   21, .h =   21 }, .size = { .x =    0, .y =    0, .w =   21, .h =   21 }, .pivot = { .x =  10, .y =  10 },.source_size = { .x =   21, .y =   21 }},
	{ .rect = { .x =  192, .y =    1, .w =   21, .h =   21 }, .size = { .x =    0, .y =    0, .w =   21, .h =   21 }, .pivot = { .x =  10, .y =  10 },.source_size = { .x =   21, .y =   21 }},
	{ .rect = { .x =  215, .y =    1, .w =   21, .h =   21 }, .size = { .x =    0, .y =    0, .w =   21, .h =   21 }, .pivot = { .x =  10, .y =  10 },.source_size = { .x =   21, .y =   21 }},
	{ .rect = { .x =  238, .y =    1, .w =   21, .h =   21 }, .size = { .x =    0, .y =    0, .w =   21, .h =   21 }, .pivot = { .x =  10, .y =  10 },.source_size = { .x =   21, .y =   21 }},
	{ .rect = { .x =  261, .y =    1, .w =   21, .h =   21 }, .size = { .x =    0, .y =    0, .w =   21, .h =   21 }, .pivot = { .x =  10, .y =  10 },.source_size = { .x =   21, .y =   21 }},
	{ .rect = { .x =  100, .y =    1, .w =   21, .h =   21 }, .size = { .x =    0, .y =    0, .w =   21, .h =   21 }, .pivot = { .x =  10, .y =  10 },.source_size = { .x =   21, .y =   21 }},
	{ .rect = { .x =  100, .y =    1, .w =   21, .h =   21 }, .size = { .x =    0, .y =    0, .w =   21, .h =   21 }, .pivot = { .x =  10, .y =  10 },.source_size = { .x =   21, .y =   21 }},
	{ .rect = { .x =  100, .y =    1, .w =   21, .h =   21 }, .size = { .x =    0, .y =    0, .w =   21, .h =   21 }, .pivot = { .x =  10, .y =  10 },.source_size = { .x =   21, .y =   21 }},
	{ .rect = { .x =  100, .y =    1, .w =   21, .h =   21 }, .size = { .x =    0, .y =    0, .w =   21, .h =   21 }, .pivot = { .x =  10, .y =  10 },.source_size = { .x =   21, .y =   21 }},
	{ .rect = { .x =  284, .y =    1, .w =   33, .h =   31 }, .size = { .x =   13, .y =   14, .w =   33, .h =   31 }, .pivot = { .x =  29, .y =  33 },.source_size = { .x =   59, .y =   54 }},
	{ .rect = { .x =  319, .y =    1, .w =   59, .h =   44 }, .size = { .x =    0, .y =   10, .w =   59, .h =   44 }, .pivot = { .x =  29, .y =  32 },.source_size = { .x =   59, .y =   54 }},
	{ .rect = { .x =  380, .y =    1, .w =   37, .h =   29 }, .size = { .x =   11, .y =   16, .w =   37, .h =   29 }, .pivot = { .x =  29, .y =  33 },.source_size = { .x =   59, .y =   54 }},
	{ .rect = { .x =  419, .y =    1, .w =   43, .h =   29 }, .size = { .x =    8, .y =   18, .w =   43, .h =   29 }, .pivot = { .x =  29, .y =  31 },.source_size = { .x =   59, .y =   54 }},
};

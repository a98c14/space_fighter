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
	ANIMATION_GAME_COLLECTABLES_EXPERIENCE_ORB,
	ANIMATION_GAME_COLLECTABLES_POWER_UP,
	ANIMATION_GAME_VFX_EXPERIENCE,
	ANIMATION_GAME_VFX_HIT_EFFECT_ENEMY_BULLET,
	ANIMATION_GAME_VFX_HIT_EFFECT_PLAYER_BULLET,
	ANIMATION_GAME_VFX_MUZZLE_FLASH_1,
	ANIMATION_GAME_VFX_MUZZLE_FLASH_2,
	ANIMATION_GAME_VFX_MUZZLE_FLASH_3,
	ANIMATION_GAME_VFX_POWER_UP_PICKUP_EFFECT,
	ANIMATION_COUNT
};

enum
{
	SPRITE_UNDEFINED = 0,
	SPRITE_GAME_COLLECTABLES_EXPERIENCE_ORB_0,
	SPRITE_GAME_COLLECTABLES_EXPERIENCE_ORB_1,
	SPRITE_GAME_COLLECTABLES_EXPERIENCE_ORB_2,
	SPRITE_GAME_COLLECTABLES_EXPERIENCE_ORB_3,
	SPRITE_GAME_COLLECTABLES_EXPERIENCE_ORB_4,
	SPRITE_GAME_COLLECTABLES_EXPERIENCE_ORB_5,
	SPRITE_GAME_COLLECTABLES_EXPERIENCE_ORB_6,
	SPRITE_GAME_COLLECTABLES_EXPERIENCE_ORB_7,
	SPRITE_GAME_COLLECTABLES_EXPERIENCE_ORB_8,
	SPRITE_GAME_COLLECTABLES_EXPERIENCE_ORB_9,
	SPRITE_GAME_COLLECTABLES_EXPERIENCE_ORB_10,
	SPRITE_GAME_COLLECTABLES_EXPERIENCE_ORB_11,
	SPRITE_GAME_COLLECTABLES_EXPERIENCE_ORB_12,
	SPRITE_GAME_COLLECTABLES_EXPERIENCE_ORB_13,
	SPRITE_GAME_COLLECTABLES_EXPERIENCE_ORB_14,
	SPRITE_GAME_COLLECTABLES_EXPERIENCE_ORB_15,
	SPRITE_GAME_COLLECTABLES_EXPERIENCE_ORB_16,
	SPRITE_GAME_COLLECTABLES_EXPERIENCE_ORB_17,
	SPRITE_GAME_COLLECTABLES_EXPERIENCE_ORB_18,
	SPRITE_GAME_COLLECTABLES_EXPERIENCE_ORB_19,
	SPRITE_GAME_COLLECTABLES_EXPERIENCE_ORB_20,
	SPRITE_GAME_COLLECTABLES_EXPERIENCE_ORB_21,
	SPRITE_GAME_COLLECTABLES_POWER_UP_0,
	SPRITE_GAME_COLLECTABLES_POWER_UP_1,
	SPRITE_GAME_COLLECTABLES_POWER_UP_2,
	SPRITE_GAME_COLLECTABLES_POWER_UP_3,
	SPRITE_GAME_COLLECTABLES_POWER_UP_4,
	SPRITE_GAME_COLLECTABLES_POWER_UP_5,
	SPRITE_GAME_COLLECTABLES_POWER_UP_6,
	SPRITE_GAME_COLLECTABLES_POWER_UP_7,
	SPRITE_GAME_COLLECTABLES_POWER_UP_8,
	SPRITE_GAME_COLLECTABLES_POWER_UP_9,
	SPRITE_GAME_COLLECTABLES_POWER_UP_10,
	SPRITE_GAME_COLLECTABLES_POWER_UP_11,
	SPRITE_GAME_COLLECTABLES_POWER_UP_12,
	SPRITE_GAME_SHIPS_HORNET,
	SPRITE_GAME_SHIPS_RANGER,
	SPRITE_GAME_SHIPS_RED_BEATLE,
	SPRITE_GAME_SHIPS_SCOUT,
	SPRITE_GAME_SHIPS_WASP,
	SPRITE_GAME_VFX_EXPERIENCE_0,
	SPRITE_GAME_VFX_EXPERIENCE_1,
	SPRITE_GAME_VFX_EXPERIENCE_2,
	SPRITE_GAME_VFX_EXPERIENCE_3,
	SPRITE_GAME_VFX_EXPERIENCE_4,
	SPRITE_GAME_VFX_EXPERIENCE_5,
	SPRITE_GAME_VFX_EXPERIENCE_6,
	SPRITE_GAME_VFX_HIT_EFFECT_ENEMY_BULLET_0,
	SPRITE_GAME_VFX_HIT_EFFECT_ENEMY_BULLET_1,
	SPRITE_GAME_VFX_HIT_EFFECT_ENEMY_BULLET_2,
	SPRITE_GAME_VFX_HIT_EFFECT_ENEMY_BULLET_3,
	SPRITE_GAME_VFX_HIT_EFFECT_ENEMY_BULLET_4,
	SPRITE_GAME_VFX_HIT_EFFECT_ENEMY_BULLET_5,
	SPRITE_GAME_VFX_HIT_EFFECT_ENEMY_BULLET_6,
	SPRITE_GAME_VFX_HIT_EFFECT_ENEMY_BULLET_7,
	SPRITE_GAME_VFX_HIT_EFFECT_ENEMY_BULLET_8,
	SPRITE_GAME_VFX_HIT_EFFECT_PLAYER_BULLET_0,
	SPRITE_GAME_VFX_HIT_EFFECT_PLAYER_BULLET_1,
	SPRITE_GAME_VFX_HIT_EFFECT_PLAYER_BULLET_2,
	SPRITE_GAME_VFX_HIT_EFFECT_PLAYER_BULLET_3,
	SPRITE_GAME_VFX_HIT_EFFECT_PLAYER_BULLET_4,
	SPRITE_GAME_VFX_HIT_EFFECT_PLAYER_BULLET_5,
	SPRITE_GAME_VFX_HIT_EFFECT_PLAYER_BULLET_6,
	SPRITE_GAME_VFX_HIT_EFFECT_PLAYER_BULLET_7,
	SPRITE_GAME_VFX_HIT_EFFECT_PLAYER_BULLET_8,
	SPRITE_GAME_VFX_HIT_EFFECT_PLAYER_BULLET_9,
	SPRITE_GAME_VFX_HIT_EFFECT_PLAYER_BULLET_10,
	SPRITE_GAME_VFX_MUZZLE_FLASH_1_0,
	SPRITE_GAME_VFX_MUZZLE_FLASH_1_1,
	SPRITE_GAME_VFX_MUZZLE_FLASH_1_2,
	SPRITE_GAME_VFX_MUZZLE_FLASH_1_3,
	SPRITE_GAME_VFX_MUZZLE_FLASH_1_4,
	SPRITE_GAME_VFX_MUZZLE_FLASH_2_0,
	SPRITE_GAME_VFX_MUZZLE_FLASH_2_1,
	SPRITE_GAME_VFX_MUZZLE_FLASH_2_2,
	SPRITE_GAME_VFX_MUZZLE_FLASH_2_3,
	SPRITE_GAME_VFX_MUZZLE_FLASH_2_4,
	SPRITE_GAME_VFX_MUZZLE_FLASH_3_0,
	SPRITE_GAME_VFX_MUZZLE_FLASH_3_1,
	SPRITE_GAME_VFX_MUZZLE_FLASH_3_2,
	SPRITE_GAME_VFX_MUZZLE_FLASH_3_3,
	SPRITE_GAME_VFX_MUZZLE_FLASH_3_4,
	SPRITE_GAME_VFX_POWER_UP_PICKUP_EFFECT_0,
	SPRITE_GAME_VFX_POWER_UP_PICKUP_EFFECT_1,
	SPRITE_GAME_VFX_POWER_UP_PICKUP_EFFECT_2,
	SPRITE_GAME_VFX_POWER_UP_PICKUP_EFFECT_3,
	SPRITE_GAME_VFX_POWER_UP_PICKUP_EFFECT_4,
	SPRITE_GAME_VFX_POWER_UP_PICKUP_EFFECT_5,
	SPRITE_GAME_VFX_POWER_UP_PICKUP_EFFECT_6,
	SPRITE_GAME_VFX_POWER_UP_PICKUP_EFFECT_7,
	SPRITE_GAME_VFX_POWER_UP_PICKUP_EFFECT_8,
	SPRITE_COUNT
};

internal const TextureIndex texture_index_map[92] = {
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

internal const Animation Animations[10] = {
	{0},
	{ SPRITE_GAME_COLLECTABLES_EXPERIENCE_ORB_0, SPRITE_GAME_COLLECTABLES_EXPERIENCE_ORB_21 + 1 },
	{ SPRITE_GAME_COLLECTABLES_POWER_UP_0, SPRITE_GAME_COLLECTABLES_POWER_UP_12 + 1 },
	{ SPRITE_GAME_VFX_EXPERIENCE_0, SPRITE_GAME_VFX_EXPERIENCE_6 + 1 },
	{ SPRITE_GAME_VFX_HIT_EFFECT_ENEMY_BULLET_0, SPRITE_GAME_VFX_HIT_EFFECT_ENEMY_BULLET_8 + 1 },
	{ SPRITE_GAME_VFX_HIT_EFFECT_PLAYER_BULLET_0, SPRITE_GAME_VFX_HIT_EFFECT_PLAYER_BULLET_10 + 1 },
	{ SPRITE_GAME_VFX_MUZZLE_FLASH_1_0, SPRITE_GAME_VFX_MUZZLE_FLASH_1_4 + 1 },
	{ SPRITE_GAME_VFX_MUZZLE_FLASH_2_0, SPRITE_GAME_VFX_MUZZLE_FLASH_2_4 + 1 },
	{ SPRITE_GAME_VFX_MUZZLE_FLASH_3_0, SPRITE_GAME_VFX_MUZZLE_FLASH_3_4 + 1 },
	{ SPRITE_GAME_VFX_POWER_UP_PICKUP_EFFECT_0, SPRITE_GAME_VFX_POWER_UP_PICKUP_EFFECT_8 + 1 },
};

internal const Sprite Sprites[92] = {
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
	{ .rect = { .x =   45, .y =    1, .w =   21, .h =   21 }, .size = { .x =    0, .y =    0, .w =   21, .h =   21 }, .pivot = { .x =  10, .y =  10 },.source_size = { .x =   21, .y =   21 }},
	{ .rect = { .x =   45, .y =    1, .w =   21, .h =   21 }, .size = { .x =    0, .y =    0, .w =   21, .h =   21 }, .pivot = { .x =  10, .y =  10 },.source_size = { .x =   21, .y =   21 }},
	{ .rect = { .x =   68, .y =    1, .w =   21, .h =   21 }, .size = { .x =    0, .y =    0, .w =   21, .h =   21 }, .pivot = { .x =  10, .y =  10 },.source_size = { .x =   21, .y =   21 }},
	{ .rect = { .x =   91, .y =    1, .w =   21, .h =   21 }, .size = { .x =    0, .y =    0, .w =   21, .h =   21 }, .pivot = { .x =  10, .y =  10 },.source_size = { .x =   21, .y =   21 }},
	{ .rect = { .x =  114, .y =    1, .w =   21, .h =   21 }, .size = { .x =    0, .y =    0, .w =   21, .h =   21 }, .pivot = { .x =  10, .y =  10 },.source_size = { .x =   21, .y =   21 }},
	{ .rect = { .x =  137, .y =    1, .w =   21, .h =   21 }, .size = { .x =    0, .y =    0, .w =   21, .h =   21 }, .pivot = { .x =  10, .y =  10 },.source_size = { .x =   21, .y =   21 }},
	{ .rect = { .x =  160, .y =    1, .w =   21, .h =   21 }, .size = { .x =    0, .y =    0, .w =   21, .h =   21 }, .pivot = { .x =  10, .y =  10 },.source_size = { .x =   21, .y =   21 }},
	{ .rect = { .x =  183, .y =    1, .w =   21, .h =   21 }, .size = { .x =    0, .y =    0, .w =   21, .h =   21 }, .pivot = { .x =  10, .y =  10 },.source_size = { .x =   21, .y =   21 }},
	{ .rect = { .x =  206, .y =    1, .w =   21, .h =   21 }, .size = { .x =    0, .y =    0, .w =   21, .h =   21 }, .pivot = { .x =  10, .y =  10 },.source_size = { .x =   21, .y =   21 }},
	{ .rect = { .x =   45, .y =    1, .w =   21, .h =   21 }, .size = { .x =    0, .y =    0, .w =   21, .h =   21 }, .pivot = { .x =  10, .y =  10 },.source_size = { .x =   21, .y =   21 }},
	{ .rect = { .x =   45, .y =    1, .w =   21, .h =   21 }, .size = { .x =    0, .y =    0, .w =   21, .h =   21 }, .pivot = { .x =  10, .y =  10 },.source_size = { .x =   21, .y =   21 }},
	{ .rect = { .x =   45, .y =    1, .w =   21, .h =   21 }, .size = { .x =    0, .y =    0, .w =   21, .h =   21 }, .pivot = { .x =  10, .y =  10 },.source_size = { .x =   21, .y =   21 }},
	{ .rect = { .x =   45, .y =    1, .w =   21, .h =   21 }, .size = { .x =    0, .y =    0, .w =   21, .h =   21 }, .pivot = { .x =  10, .y =  10 },.source_size = { .x =   21, .y =   21 }},
	{ .rect = { .x =  229, .y =    1, .w =   53, .h =   54 }, .size = { .x =    3, .y =    0, .w =   53, .h =   54 }, .pivot = { .x =  30, .y =  27 },.source_size = { .x =   59, .y =   54 }},
	{ .rect = { .x =  284, .y =    1, .w =   33, .h =   31 }, .size = { .x =   13, .y =   14, .w =   33, .h =   31 }, .pivot = { .x =  30, .y =  27 },.source_size = { .x =   59, .y =   54 }},
	{ .rect = { .x =  319, .y =    1, .w =   59, .h =   44 }, .size = { .x =    0, .y =   10, .w =   59, .h =   44 }, .pivot = { .x =  30, .y =  27 },.source_size = { .x =   59, .y =   54 }},
	{ .rect = { .x =  380, .y =    1, .w =   37, .h =   29 }, .size = { .x =   11, .y =   16, .w =   37, .h =   29 }, .pivot = { .x =  30, .y =  27 },.source_size = { .x =   59, .y =   54 }},
	{ .rect = { .x =  419, .y =    1, .w =   43, .h =   29 }, .size = { .x =    8, .y =   18, .w =   43, .h =   29 }, .pivot = { .x =  30, .y =  27 },.source_size = { .x =   59, .y =   54 }},
	{ .rect = { .x =  464, .y =    1, .w =   22, .h =   11 }, .size = { .x =    1, .y =    0, .w =   22, .h =   11 }, .pivot = { .x =  12, .y =   6 },.source_size = { .x =   24, .y =   11 }},
	{ .rect = { .x =  488, .y =    1, .w =   24, .h =   11 }, .size = { .x =    0, .y =    0, .w =   24, .h =   11 }, .pivot = { .x =  12, .y =   6 },.source_size = { .x =   24, .y =   11 }},
	{ .rect = { .x =  514, .y =    1, .w =   24, .h =    8 }, .size = { .x =    0, .y =    0, .w =   24, .h =    8 }, .pivot = { .x =  12, .y =   6 },.source_size = { .x =   24, .y =   11 }},
	{ .rect = { .x =  540, .y =    1, .w =   22, .h =    8 }, .size = { .x =    1, .y =    0, .w =   22, .h =    8 }, .pivot = { .x =  12, .y =   6 },.source_size = { .x =   24, .y =   11 }},
	{ .rect = { .x =  564, .y =    1, .w =    5, .h =    5 }, .size = { .x =    6, .y =    0, .w =    5, .h =    5 }, .pivot = { .x =  12, .y =   6 },.source_size = { .x =   24, .y =   11 }},
	{ .rect = { .x =  571, .y =    1, .w =    4, .h =    4 }, .size = { .x =    7, .y =    1, .w =    4, .h =    4 }, .pivot = { .x =  12, .y =   6 },.source_size = { .x =   24, .y =   11 }},
	{ .rect = { .x =  577, .y =    1, .w =    3, .h =    3 }, .size = { .x =    8, .y =    2, .w =    3, .h =    3 }, .pivot = { .x =  12, .y =   6 },.source_size = { .x =   24, .y =   11 }},
	{ .rect = { .x =  582, .y =    1, .w =    8, .h =    8 }, .size = { .x =    1, .y =    1, .w =    8, .h =    8 }, .pivot = { .x =   5, .y =   5 },.source_size = { .x =   10, .y =   10 }},
	{ .rect = { .x =  592, .y =    1, .w =   10, .h =   10 }, .size = { .x =    0, .y =    0, .w =   10, .h =   10 }, .pivot = { .x =   5, .y =   5 },.source_size = { .x =   10, .y =   10 }},
	{ .rect = { .x =  604, .y =    1, .w =   10, .h =   10 }, .size = { .x =    0, .y =    0, .w =   10, .h =   10 }, .pivot = { .x =   5, .y =   5 },.source_size = { .x =   10, .y =   10 }},
	{ .rect = { .x =  616, .y =    1, .w =   10, .h =   10 }, .size = { .x =    0, .y =    0, .w =   10, .h =   10 }, .pivot = { .x =   5, .y =   5 },.source_size = { .x =   10, .y =   10 }},
	{ .rect = { .x =  628, .y =    1, .w =   10, .h =   10 }, .size = { .x =    0, .y =    0, .w =   10, .h =   10 }, .pivot = { .x =   5, .y =   5 },.source_size = { .x =   10, .y =   10 }},
	{ .rect = { .x =  640, .y =    1, .w =   10, .h =   10 }, .size = { .x =    0, .y =    0, .w =   10, .h =   10 }, .pivot = { .x =   5, .y =   5 },.source_size = { .x =   10, .y =   10 }},
	{ .rect = { .x =  652, .y =    1, .w =   10, .h =   10 }, .size = { .x =    0, .y =    0, .w =   10, .h =   10 }, .pivot = { .x =   5, .y =   5 },.source_size = { .x =   10, .y =   10 }},
	{ .rect = { .x =  664, .y =    1, .w =   10, .h =   10 }, .size = { .x =    0, .y =    0, .w =   10, .h =   10 }, .pivot = { .x =   5, .y =   5 },.source_size = { .x =   10, .y =   10 }},
	{ .rect = { .x =  676, .y =    1, .w =    8, .h =    8 }, .size = { .x =    1, .y =    1, .w =    8, .h =    8 }, .pivot = { .x =   5, .y =   5 },.source_size = { .x =   10, .y =   10 }},
	{ .rect = { .x =  686, .y =    1, .w =   18, .h =    8 }, .size = { .x =    5, .y =    9, .w =   18, .h =    8 }, .pivot = { .x =  12, .y =  12 },.source_size = { .x =   24, .y =   23 }},
	{ .rect = { .x =  706, .y =    1, .w =   10, .h =   20 }, .size = { .x =    9, .y =    3, .w =   10, .h =   20 }, .pivot = { .x =  12, .y =  12 },.source_size = { .x =   24, .y =   23 }},
	{ .rect = { .x =  718, .y =    1, .w =   21, .h =   19 }, .size = { .x =    1, .y =    2, .w =   21, .h =   19 }, .pivot = { .x =  12, .y =  12 },.source_size = { .x =   24, .y =   23 }},
	{ .rect = { .x =  741, .y =    1, .w =   23, .h =   21 }, .size = { .x =    0, .y =    1, .w =   23, .h =   21 }, .pivot = { .x =  12, .y =  12 },.source_size = { .x =   24, .y =   23 }},
	{ .rect = { .x =  766, .y =    1, .w =   24, .h =   22 }, .size = { .x =    0, .y =    1, .w =   24, .h =   22 }, .pivot = { .x =  12, .y =  12 },.source_size = { .x =   24, .y =   23 }},
	{ .rect = { .x =  792, .y =    1, .w =   24, .h =   23 }, .size = { .x =    0, .y =    0, .w =   24, .h =   23 }, .pivot = { .x =  12, .y =  12 },.source_size = { .x =   24, .y =   23 }},
	{ .rect = { .x =  818, .y =    1, .w =   24, .h =   20 }, .size = { .x =    0, .y =    0, .w =   24, .h =   20 }, .pivot = { .x =  12, .y =  12 },.source_size = { .x =   24, .y =   23 }},
	{ .rect = { .x =  844, .y =    1, .w =   17, .h =   20 }, .size = { .x =    1, .y =    0, .w =   17, .h =   20 }, .pivot = { .x =  12, .y =  12 },.source_size = { .x =   24, .y =   23 }},
	{ .rect = { .x =  863, .y =    1, .w =   15, .h =   12 }, .size = { .x =    2, .y =    0, .w =   15, .h =   12 }, .pivot = { .x =  12, .y =  12 },.source_size = { .x =   24, .y =   23 }},
	{ .rect = { .x =  880, .y =    1, .w =   10, .h =    4 }, .size = { .x =    6, .y =    0, .w =   10, .h =    4 }, .pivot = { .x =  12, .y =  12 },.source_size = { .x =   24, .y =   23 }},
	{ .rect = { .x =  892, .y =    1, .w =    3, .h =    3 }, .size = { .x =   13, .y =    0, .w =    3, .h =    3 }, .pivot = { .x =  12, .y =  12 },.source_size = { .x =   24, .y =   23 }},
	{ .rect = { .x =  897, .y =    1, .w =    8, .h =    7 }, .size = { .x =    0, .y =    5, .w =    8, .h =    7 }, .pivot = { .x =   4, .y =   8 },.source_size = { .x =   21, .y =   16 }},
	{ .rect = { .x =  907, .y =    1, .w =   17, .h =   12 }, .size = { .x =    2, .y =    2, .w =   17, .h =   12 }, .pivot = { .x =   4, .y =   8 },.source_size = { .x =   21, .y =   16 }},
	{ .rect = { .x =  926, .y =    1, .w =   14, .h =   12 }, .size = { .x =    6, .y =    2, .w =   14, .h =   12 }, .pivot = { .x =   4, .y =   8 },.source_size = { .x =   21, .y =   16 }},
	{ .rect = { .x =  942, .y =    1, .w =   14, .h =   12 }, .size = { .x =    7, .y =    2, .w =   14, .h =   12 }, .pivot = { .x =   4, .y =   8 },.source_size = { .x =   21, .y =   16 }},
	{ .rect = { .x =  958, .y =    1, .w =   11, .h =   12 }, .size = { .x =   10, .y =    2, .w =   11, .h =   12 }, .pivot = { .x =   4, .y =   8 },.source_size = { .x =   21, .y =   16 }},
	{ .rect = { .x =  971, .y =    1, .w =    7, .h =    7 }, .size = { .x =    0, .y =    5, .w =    7, .h =    7 }, .pivot = { .x =   4, .y =   8 },.source_size = { .x =   21, .y =   16 }},
	{ .rect = { .x =  980, .y =    1, .w =   17, .h =   14 }, .size = { .x =    2, .y =    1, .w =   17, .h =   14 }, .pivot = { .x =   4, .y =   8 },.source_size = { .x =   21, .y =   16 }},
	{ .rect = { .x =  999, .y =    1, .w =   13, .h =   16 }, .size = { .x =    8, .y =    0, .w =   13, .h =   16 }, .pivot = { .x =   4, .y =   8 },.source_size = { .x =   21, .y =   16 }},
	{ .rect = { .x = 1014, .y =    1, .w =   13, .h =   16 }, .size = { .x =    8, .y =    0, .w =   13, .h =   16 }, .pivot = { .x =   4, .y =   8 },.source_size = { .x =   21, .y =   16 }},
	{ .rect = { .x = 1029, .y =    1, .w =   12, .h =   10 }, .size = { .x =    9, .y =    1, .w =   12, .h =   10 }, .pivot = { .x =   4, .y =   8 },.source_size = { .x =   21, .y =   16 }},
	{ .rect = { .x =  971, .y =    1, .w =    7, .h =    7 }, .size = { .x =    0, .y =    5, .w =    7, .h =    7 }, .pivot = { .x =   4, .y =   8 },.source_size = { .x =   21, .y =   16 }},
	{ .rect = { .x = 1043, .y =    1, .w =   11, .h =   12 }, .size = { .x =    3, .y =    2, .w =   11, .h =   12 }, .pivot = { .x =   4, .y =   8 },.source_size = { .x =   21, .y =   16 }},
	{ .rect = { .x = 1056, .y =    1, .w =   10, .h =   13 }, .size = { .x =    5, .y =    1, .w =   10, .h =   13 }, .pivot = { .x =   4, .y =   8 },.source_size = { .x =   21, .y =   16 }},
	{ .rect = { .x = 1068, .y =    1, .w =    9, .h =   13 }, .size = { .x =    6, .y =    1, .w =    9, .h =   13 }, .pivot = { .x =   4, .y =   8 },.source_size = { .x =   21, .y =   16 }},
	{ .rect = { .x = 1079, .y =    1, .w =    5, .h =    4 }, .size = { .x =   10, .y =    6, .w =    5, .h =    4 }, .pivot = { .x =   4, .y =   8 },.source_size = { .x =   21, .y =   16 }},
	{ .rect = { .x = 1086, .y =    1, .w =   21, .h =   13 }, .size = { .x =    2, .y =    2, .w =   21, .h =   13 }, .pivot = { .x =  12, .y =   8 },.source_size = { .x =   25, .y =   16 }},
	{ .rect = { .x = 1109, .y =    1, .w =   25, .h =   15 }, .size = { .x =    0, .y =    1, .w =   25, .h =   15 }, .pivot = { .x =  12, .y =   8 },.source_size = { .x =   25, .y =   16 }},
	{ .rect = { .x = 1136, .y =    1, .w =   25, .h =   13 }, .size = { .x =    0, .y =    3, .w =   25, .h =   13 }, .pivot = { .x =  12, .y =   8 },.source_size = { .x =   25, .y =   16 }},
	{ .rect = { .x = 1163, .y =    1, .w =   25, .h =   14 }, .size = { .x =    0, .y =    2, .w =   25, .h =   14 }, .pivot = { .x =  12, .y =   8 },.source_size = { .x =   25, .y =   16 }},
	{ .rect = { .x = 1190, .y =    1, .w =   24, .h =   16 }, .size = { .x =    1, .y =    0, .w =   24, .h =   16 }, .pivot = { .x =  12, .y =   8 },.source_size = { .x =   25, .y =   16 }},
	{ .rect = { .x = 1216, .y =    1, .w =   23, .h =   16 }, .size = { .x =    2, .y =    0, .w =   23, .h =   16 }, .pivot = { .x =  12, .y =   8 },.source_size = { .x =   25, .y =   16 }},
	{ .rect = { .x = 1241, .y =    1, .w =   21, .h =   14 }, .size = { .x =    3, .y =    1, .w =   21, .h =   14 }, .pivot = { .x =  12, .y =   8 },.source_size = { .x =   25, .y =   16 }},
	{ .rect = { .x = 1264, .y =    1, .w =   16, .h =   12 }, .size = { .x =    7, .y =    2, .w =   16, .h =   12 }, .pivot = { .x =  12, .y =   8 },.source_size = { .x =   25, .y =   16 }},
	{ .rect = { .x = 1282, .y =    1, .w =    3, .h =    3 }, .size = { .x =   19, .y =    3, .w =    3, .h =    3 }, .pivot = { .x =  12, .y =   8 },.source_size = { .x =   25, .y =   16 }},
};

/// \file GameDefines.h
/// \brief Game specific defines.

#pragma once

#include "Defines.h"

/// \brief Sprite type.
///
/// Note: NUM_SPRITES must be last.

enum eSpriteType{
  PLAYER_SPRITE, TURRET_SPRITE, 
  BULLET_SPRITE, RED_BULLET,
  SMOKE_SPRITE, SPARK_SPRITE,
  FLOOR_SPRITE, BLUE_SHIP, RED_SHIP, 
  HEART1_SPRITE, 
  BLUE_BULLET, HOTSHOT, FIREBALL, LILBOY, BLACK_JACK, REDFIRE,
  INTRO_SCREEN, GAME_OVER_SCREEN, BACKGROUND, VOLCANO_BACKGROUND, STAR_BACKGROUND, EARTH_BACKGROUND, DAMAGE_SPRITE, BIG_EXPLOSION, 
  BIG_SMOKE, SMALL_SMOKE, SMALL_EXPLOSION, LILBOMB, LILBOMB_EFFECT,
  RED_LIGHT_ENEMY, BLUE_LIGHT_ENEMY, RED_HEAVY_ENEMY, BLUE_HEAVY_ENEMY, FORCE_FIELD, BLACK_HOLE, JACK, QUEEN, CARD, 
  RED_LINE, BLUE_LINE, LARGE_RESPAWN, SMALL_RESPAWN, END_SCREEN, 
  NUM_SPRITES //MUST BE LAST
}; //eSpriteType
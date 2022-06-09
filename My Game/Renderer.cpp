/// \file Renderer.cpp
/// \brief Code for the renderer class CRenderer.

#include "Renderer.h"
#include "ComponentIncludes.h"
#include "Abort.h"

CRenderer::CRenderer():
  CSpriteRenderer(Batched2D){
} //constructor

/// Load the specific images needed for this game.
/// This is where eSpriteType values from GameDefines.h get
/// tied to the names of sprite tags in gamesettings.xml. Those
/// sprite tags contain the name of the corresponding image file.
/// If the image tag or the image file are missing, then
/// the game should abort from deeper in the Engine code,
/// leaving you with a dialog box that tells you what
/// went wrong.

void CRenderer::LoadImages(){  
  BeginResourceUpload();

  Load(FLOOR_SPRITE, "floor"); 
  Load(PLAYER_SPRITE, "player");
  Load(BULLET_SPRITE, "bullet");
  Load(RED_BULLET, "bullet2");
  Load(SMOKE_SPRITE, "smoke");
  Load(SPARK_SPRITE, "spark");
  Load(TURRET_SPRITE, "turret");
  Load(BLUE_SHIP, "BLUE_SHIP");
  Load(RED_SHIP, "RED_SHIP");
  Load(HEART1_SPRITE, "HEART1_SPRITE");
  //Load(RED_LIGHT_ENEMY, "RED_LIGHT_ENEMY");
  //Load(BLUE_LIGHT_ENEMY, "BLUE_LIGHT_ENEMY");
  Load(BLUE_BULLET, "BLUE_BULLET");
  Load(HOTSHOT, "HOTSHOT");
  Load(FIREBALL, "FIREBALL");
  Load(LILBOY, "LILBOY");
  Load(LILBOMB, "LILBOMB");
  Load(BLACK_JACK, "BLACK_JACK");
  Load(REDFIRE, "REDFIRE");
  Load(INTRO_SCREEN, "INTRO_SCREEN");
  Load(GAME_OVER_SCREEN, "GAME_OVER_SCREEN");
  Load(BACKGROUND, "BACKGROUND");
  Load(VOLCANO_BACKGROUND, "VOLCANO_BACKGROUND");
  Load(STAR_BACKGROUND, "STAR_BACKGROUND");
  Load(EARTH_BACKGROUND, "EARTH_BACKGROUND");
  Load(DAMAGE_SPRITE, "HIT");
  Load(BIG_SMOKE, "BIG_SMOKE");
  Load(SMALL_SMOKE, "SMALL_SMOKE");
  Load(BIG_EXPLOSION, "BIG_EXPLOSION");
  Load(SMALL_EXPLOSION, "SMALL_EXPLOSION");
  Load(LILBOMB_EFFECT,"LILBOMB_EFFECT");
  Load(RED_LIGHT_ENEMY, "RED_LIGHT_ENEMY");
  Load(BLUE_LIGHT_ENEMY, "BLUE_LIGHT_ENEMY");
  Load(RED_HEAVY_ENEMY, "RED_HEAVY_ENEMY");
  Load(BLUE_HEAVY_ENEMY, "BLUE_HEAVY_ENEMY");
  Load(RED_LINE, "RED_LINE");
  Load(BLUE_LINE, "BLUE_LINE");
  Load(CARD, "CARD");
  Load(BLACK_HOLE, "BLACK_HOLE");
  Load(QUEEN, "QUEEN");
  Load(JACK, "JACK");
  Load(FORCE_FIELD, "FORCE_FIELD");
  Load(LARGE_RESPAWN, "LARGE_RESPAWN");
  Load(SMALL_RESPAWN, "SMALL_RESPAWN");
  Load(END_SCREEN, "END_SCREEN");

  EndResourceUpload();
} //LoadImages




/// \file SndList.h
/// \brief Enumerated type for sounds.

#pragma once

#include "Sound.h"

/// \brief Game sound enumerated type. 
///
/// These are the sounds used in actual gameplay. 
/// The sounds must be listed here in the same order that
/// they are in the sound settings XML file.

enum eSoundType{ 
  GUN_SOUND, RICOCHET_SOUND, OW_SOUND, CLANG_SOUND, RYDEEN_MUSIC, PLAYERGUN_SOUND, FIRETRAP_SOUND, FIREBALL_SOUND, ENEMYGUN_SOUND, 
  DEATH_SOUND, DAMAGE_SOUND, BH_SOUND, PLAYERHIT_SOUND, ABSORB_SOUND, RESPAWN_SOUND, ENDOFTHEDARK_MUSIC, VICTORY_MUSIC
}; //eSoundType
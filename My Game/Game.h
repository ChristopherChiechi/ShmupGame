/// \file Game.h
/// \brief Interface for the game class CGame.

#pragma once

#include "Component.h"
#include "Common.h"
#include "ObjectManager.h"
#include "Settings.h"

/// \brief The game class.

class CGame: 
  public CComponent, 
  public CSettings,
  public CCommon{ 

  private:
    // controls gamestates
    int m_nCurLevel = 0;    // current level
    int m_nPrevLevel = 0;  //previous level
    int old_score = 0; //Keeps up with player score
    bool GameOver = false;  // flag for gameover
    bool playerIsCreated = false;
    bool gameOverCalled = false;    // flag so BeginGame is only called once after gameover

    void BeginGame(); ///< Begin playing the game.
    void KeyboardHandler(); ///< The keyboard handler.
    void ControllerHandler(); ///< The controller handler.
    void RenderFrame(); ///< Render an animation frame.
    void CreateObjects(); ///< Create game objects.
    void FollowCamera(); ///< Make camera follow player character.
    void NextLevel();   // increments m_nCurLevel
    void Level_1(); //Level 1
    void GameOverFunc(); // sets level back to 0 (intro screen)
    void Level_2(); // Level 2
    void Level_3(); // Level 3
    void Level_4(); // Level 4
    void Level_5(); // Level 5
    void Level_6(); // Level 6
    void Level_7(); // Level 7
    void Level_8(); // Level 8
    void Level_9(); // Level 9

  public:
    ~CGame(); ///< Destructor.

    void Initialize(); ///< Initialize the game.
    void ProcessFrame(); ///< Process an animation frame.
    void Release(); ///< Release the renderer.
}; //CGame

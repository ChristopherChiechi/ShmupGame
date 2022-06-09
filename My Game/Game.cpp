/// \file Game.cpp
/// \brief Code for the game class CGame.

#include "Game.h"

#include "GameDefines.h"
#include "Renderer.h"
#include "ComponentIncludes.h"
#include "ParticleEngine.h"

/// Delete the renderer and the object manager.

CGame::~CGame(){
  delete m_pParticleEngine;
  delete m_pRenderer;
  delete m_pObjectManager;
} //destructor

/// Initialize the renderer and the object manager, load 
/// images and sounds, and begin the game.

void CGame::Initialize(){
  m_pRenderer = new CRenderer; 
  m_pRenderer->Initialize(NUM_SPRITES); 
  m_pRenderer->LoadImages(); //load images from xml file list

  m_pObjectManager = new CObjectManager; //set up the object manager 
  m_pAudio->Load(); //load the sounds for this game

  m_pParticleEngine = new CParticleEngine2D((CSpriteRenderer*)m_pRenderer);

  BeginGame();
} //Initialize

/// Release all of the DirectX12 objects by deleting the renderer.

void CGame::Release(){
  delete m_pRenderer;
  m_pRenderer = nullptr; //for safety
} //Release

/// Ask the object manager to create a player object
/// and a turret.

void CGame::CreateObjects(){
  // load space background
  m_pRenderer->GetSize(BACKGROUND, m_vWorldSize.x, m_vWorldSize.y);
  m_pObjectManager->create(BACKGROUND, m_vWorldSize/2);

  // load earth background
  if (m_nCurLevel == 1 || m_nCurLevel == 2 || m_nCurLevel == 3)
  {
      m_pRenderer->GetSize(EARTH_BACKGROUND, m_vWorldSize.x, m_vWorldSize.y);
      m_pObjectManager->create(EARTH_BACKGROUND, m_vWorldSize / 2);
  }
  // load star background
  if (m_nCurLevel == 4 || m_nCurLevel == 5 || m_nCurLevel == 6)
  {
      m_pRenderer->GetSize(STAR_BACKGROUND, m_vWorldSize.x, m_vWorldSize.y);
      m_pObjectManager->create(STAR_BACKGROUND, m_vWorldSize / 2);
  }

  // create player's ship
  m_pPlayer = m_pObjectManager->create(BLUE_SHIP, Vector2(512.0f, 78.0f));

  // game over
  if (m_nCurLevel == -1)
  {
      m_pObjectManager->create(GAME_OVER_SCREEN, Vector2(m_vWinCenter));
      m_pAudio->stop(BH_SOUND);
  }
 
  // intro screen
  if (m_nCurLevel == 0)
  {
      m_pObjectManager->create(INTRO_SCREEN, Vector2(m_vWinCenter));    // create intro screen

      // initialize game
      m_pObjectManager->setPlayerHealth(3); //set player health to 3
      m_pObjectManager->ResetScore();   // set score to 0
      m_pObjectManager->setLevelCleared(false); // set level cleared to false
      m_pObjectManager->setEnemyCount(0);   // set enemyCount to 0
      m_pObjectManager->setBossCount(0);    // set bossCount to 0
      gameOverCalled = false;   // set gameOverCalled to false
      m_pObjectManager->setBossPresent(false);  // set boss_present to false
  }

  // level 1
  if (m_nCurLevel == 1)
  {
     Level_1();
  }

  // level 2
  if (m_nCurLevel == 2)
  {
      Level_4();
  }

  // level 3
  if (m_nCurLevel == 3)
  {
      m_pObjectManager->setBossCount(1);
      Level_7();
  }

  // level 4
  if (m_nCurLevel == 4)
  {

      m_pObjectManager->setBossPresent(false);  // boss has been killed. set boss_present to false
      Level_2();
  }

  // level 5
  if (m_nCurLevel == 5)
  {
      Level_5();
  }

  // level 6
  if (m_nCurLevel == 6)
  {
      m_pObjectManager->setBossCount(1);    // set bossCount to 1
      Level_8();
  }

  // level 7
  if (m_nCurLevel == 7)
  {
      m_pObjectManager->setBossPresent(false);  // boss has been killed. set boss_present to false
      Level_9();
  }
  
  // level 8
  if (m_nCurLevel == 8)
  {
      Level_6();
  }

  // level 9
  if (m_nCurLevel == 9)
  {
      m_pObjectManager->setBossCount(1);    // set bossCount to 1
      Level_3();
  }

  // player wins
  if (m_nCurLevel == 10)
  {
      m_pObjectManager->setBossPresent(false); //final boss is killed
      m_pObjectManager->create(END_SCREEN, Vector2(m_vWinCenter));    // create end screen
  }

} //CreateObjects

//Transition between each level
void CGame::NextLevel(){

    m_pObjectManager->setPlayerHealth(3);
    m_nCurLevel++;
    m_nPrevLevel = m_nCurLevel;
    old_score = m_pObjectManager->GetScore();
    m_pObjectManager->updateLevel(m_nCurLevel);
    BeginGame();
}

void CGame::GameOverFunc()
{
    m_pObjectManager->SetScore(old_score);
    m_pObjectManager->setPlayerHealth(3);
    m_pObjectManager->setLevelCleared(false); // set level cleared to false
    m_pObjectManager->setEnemyCount(0);   // set enemyCount to 0
    m_pObjectManager->setBossCount(0);    // set bossCount to 0
    gameOverCalled = false;   // set gameOverCalled to false
    m_pObjectManager->setBossPresent(false);  // set boss_present to false
    GameOver = true;
    m_nCurLevel = m_nPrevLevel;
    m_pObjectManager->updateLevel(m_nCurLevel);
    BeginGame();
}

/// Call this function to start a new game. This
/// should be re-entrant so that you can start a
/// new game without having to shut down and restart the
/// program.

void CGame::BeginGame(){  
  m_pParticleEngine->clear(); //clear old particles
  m_pObjectManager->clear(); //clear old objects
  CreateObjects(); //create new objects 
} //BeginGame

/// Poll the keyboard state and respond to the
/// key presses that happened since the last frame.

void CGame::KeyboardHandler(){
  m_pKeyboard->GetState(); //get current keyboard state 

  // if current level is intro screen
  if (m_nCurLevel == 0)
  {
      m_pAudio->loop(RYDEEN_MUSIC);
      if (m_pKeyboard->TriggerDown(VK_RETURN))
      {
          m_pAudio->stop(RYDEEN_MUSIC);
          GameOver = false;
          NextLevel();
      }
  }

  // if player wins
  if (m_nCurLevel == 10)
  {
      m_pAudio->loop(ENDOFTHEDARK_MUSIC);
      if (m_pKeyboard->TriggerUp(VK_RETURN))
      {
          m_pAudio->stop(ENDOFTHEDARK_MUSIC);
          m_nCurLevel = 0;
          m_pObjectManager->updateLevel(m_nCurLevel);
          BeginGame();
      }
  }

  // if current level is not intro screen or gameover screen, and player is not dead, and level is not completed
  if (m_nCurLevel >= 1 && m_nCurLevel <= 9 && m_pObjectManager->getLevelCleared() == false && m_pObjectManager->getPlayerHealth() > 0)
  {
      const Vector2 pos = m_pPlayer->m_vPos; //position of center of sprite
      float w, h; //sprite width and height
      m_pRenderer->GetSize(m_pPlayer->m_nSpriteIndex, w, h);

      // Controls vertical movement
      if (m_pKeyboard->Down(VK_UP) || m_pKeyboard->Down(0x57))
          m_pPlayer->SetSpeed(250.0f);
      else if (m_pKeyboard->Down(VK_DOWN) || m_pKeyboard->Down(0x53))
          m_pPlayer->SetSpeed(-250.0f);
      else
          m_pPlayer->SetSpeed(0.0f);

      // B, E, Q, L shift, R shift changes player ship's color
      if (m_pKeyboard->TriggerDown(0x42) || m_pKeyboard->TriggerDown(0x45) || m_pKeyboard->TriggerDown(0x51) || m_pKeyboard->TriggerDown(VK_LSHIFT)
          || m_pKeyboard->TriggerDown(VK_RSHIFT))
          m_pPlayer->ChangeColor();

      // Space bar shoots bullet
      if (m_pKeyboard->TriggerDown(VK_SPACE))
          m_pObjectManager->PlayerShoots();

      // If right key and not at world edge, strafe right
      if ((m_pKeyboard->Down(VK_RIGHT) || m_pKeyboard->Down(0x44)) && !(pos.x + w / 2 > m_vWorldSize.x))
          m_pPlayer->StrafeRight();

      // If left key and not at world edge, strafe left
      if ((m_pKeyboard->Down(VK_LEFT) || m_pKeyboard->Down(0x41)) && !(pos.x - w / 2 < 0))
          m_pPlayer->StrafeLeft();

      // If y position minus half of sprite's height is less than 0, don't allow player to move down
      if (pos.y - h / 2 < 0)
      {
          // Player cannot move down
          if (m_pKeyboard->Down(VK_UP) || m_pKeyboard->Down(0x57))
              m_pPlayer->SetSpeed(250.0f);
          else
              m_pPlayer->SetSpeed(0.0f);

          // B, E, Q, L shift, R shift changes player ship's color
          if (m_pKeyboard->TriggerDown(0x42) || m_pKeyboard->TriggerDown(0x45) || m_pKeyboard->TriggerDown(0x51) || m_pKeyboard->TriggerDown(VK_LSHIFT)
              || m_pKeyboard->TriggerDown(VK_RSHIFT))
              m_pPlayer->ChangeColor();

          // Space bar shoots bullet
          if (m_pKeyboard->TriggerDown(VK_SPACE))
              m_pObjectManager->PlayerShoots();

          // If right key and not at world edge, strafe right
          if ((m_pKeyboard->Down(VK_RIGHT) || m_pKeyboard->Down(0x44)) && !(pos.x + w / 2 > m_vWorldSize.x))
              m_pPlayer->StrafeRight();

          // If left key and not at world edge, strafe left
          if ((m_pKeyboard->Down(VK_LEFT) || m_pKeyboard->Down(0x41)) && !(pos.x - w / 2 < 0))
              m_pPlayer->StrafeLeft();
      }

      // If y position minus half of sprite's height is greater than worldsize, don't allow player to move up
      if (pos.y + h / 2 > m_vWorldSize.y)
      {
          // Player cannot move up
          if (m_pKeyboard->Down(VK_DOWN) || m_pKeyboard->Down(0x53))
              m_pPlayer->SetSpeed(-250.0f);
          else
              m_pPlayer->SetSpeed(0.0f);

          // B, E, Q, L shift, R shift changes player ship's color
          if (m_pKeyboard->TriggerDown(0x42) || m_pKeyboard->TriggerDown(0x45) || m_pKeyboard->TriggerDown(0x51) || m_pKeyboard->TriggerDown(VK_LSHIFT)
              || m_pKeyboard->TriggerDown(VK_RSHIFT))
              m_pPlayer->ChangeColor();

          // Space bar shoots bullet
          if (m_pKeyboard->TriggerDown(VK_SPACE))
              m_pObjectManager->PlayerShoots();

          // If right key and not at world edge, strafe right
          if ((m_pKeyboard->Down(VK_RIGHT) || m_pKeyboard->Down(0x44)) && !(pos.x + w / 2 > m_vWorldSize.x))
              m_pPlayer->StrafeRight();

          // If left key and not at world edge, strafe left
          if ((m_pKeyboard->Down(VK_LEFT) || m_pKeyboard->Down(0x41)) && !(pos.x - w / 2 < 0))
              m_pPlayer->StrafeLeft();
      }
  } // if

  // if gameOver
  if (m_nCurLevel == -1)
  {
      if (m_pKeyboard->TriggerUp(VK_RETURN))
      {
          GameOverFunc();
      }
  }

  // if level is not intro screen or gameoverscreen and level is cleared
  if (m_nCurLevel >= 1 && m_nCurLevel <= 9 && m_pObjectManager->getLevelCleared() == true)
  {
      m_pPlayer->SetSpeed(0);
      m_pAudio->loop(VICTORY_MUSIC);
      if (m_pKeyboard->TriggerUp(VK_RETURN))
      {
          m_pAudio->stop(VICTORY_MUSIC);
          m_pObjectManager->setLevelCleared(false);
          NextLevel();
      }
  }

} //KeyboardHandler

/// Poll the XBox controller state and respond to the
/// controls there.

void CGame::ControllerHandler(){
  if(!m_pController->IsConnected())return;

  m_pController->GetState(); //get state of controller's controls 

  const float t = m_pStepTimer->GetElapsedSeconds();

  // if current level is intro screen
  if (m_nCurLevel == 0)
  {
      m_pAudio->loop(RYDEEN_MUSIC);
      // A button on intro screen starts game
      if (m_pController->GetButtonAToggle() || m_pController->GetButtonBToggle())
      {
          m_pAudio->stop(RYDEEN_MUSIC);
          GameOver = false;
          NextLevel();
      }
  }

  // if player wins
  if (m_nCurLevel == 10)
  {
      m_pAudio->loop(ENDOFTHEDARK_MUSIC);
      if (m_pController->GetButtonBToggle())
      {
          m_pAudio->stop(ENDOFTHEDARK_MUSIC);
          m_nCurLevel = 0;
          m_pObjectManager->updateLevel(m_nCurLevel);
          BeginGame();
      }
  }

  // if current level is not intro screen and player is not dead and level is not completed
  //if (m_nCurLevel >= 1 && m_pPlayer->IsDead() == false && m_pObjectManager->getLevelCleared() == false)
  if (m_nCurLevel >= 1 && m_nCurLevel <= 9 && m_pObjectManager->getLevelCleared() == false && m_pObjectManager->getPlayerHealth() > 0)
  {
      const Vector2 pos = m_pPlayer->m_vPos; //position of center of sprite
      float w, h; //sprite width and height
      m_pRenderer->GetSize(m_pPlayer->m_nSpriteIndex, w, h);

      // Controls vertical movement
      if (m_pController->GetDPadUp())
          m_pPlayer->SetSpeed(250.0f);
      else if (m_pController->GetDPadDown())
          m_pPlayer->SetSpeed(-250.0f);
      else
          m_pPlayer->SetSpeed(0.0f);

      // RB or Right Shoulder button shoots bullet
      if (m_pController->GetButtonRSToggle())
          m_pObjectManager->FireGun(m_pPlayer, BULLET_SPRITE);

      // X button also shoots bullet
      if (m_pController->GetButtonXToggle())
          m_pObjectManager->FireGun(m_pPlayer, BULLET_SPRITE);

      // Right on dpad and not at world edge, StrafeRight
      if (m_pController->GetDPadRight() && !(pos.x + w / 2 > m_vWorldSize.x))
          m_pPlayer->StrafeRight();

      // Left on dpad and not at world edge, StrafeLeft
      if (m_pController->GetDPadLeft() && !(pos.x - w / 2 < 0))
          m_pPlayer->StrafeLeft();

      // A button switches ships color
      if (m_pController->GetButtonAToggle())
          m_pPlayer->ChangeColor();

      // LS or Left Shoulder also switches ships color
      if (m_pController->GetButtonLSToggle())
          m_pPlayer->ChangeColor();

      // If y position minus half of sprite's height is less than 0, don't allow player to move down
      if (pos.y - h / 2 < 0)
      {
          if (m_pController->GetDPadUp())
              m_pPlayer->SetSpeed(250.0f);
          else
              m_pPlayer->SetSpeed(0.0f);

          if (m_pController->GetDPadRight() && !(pos.x + w / 2 > m_vWorldSize.x))
              m_pPlayer->StrafeRight();

          if (m_pController->GetDPadLeft() && !(pos.x - w / 2 < 0))
              m_pPlayer->StrafeLeft();

          if (m_pController->GetButtonRSToggle())
              m_pObjectManager->FireGun(m_pPlayer, BULLET_SPRITE);

          if (m_pController->GetButtonXToggle())
              m_pObjectManager->FireGun(m_pPlayer, BULLET_SPRITE);

          if (m_pController->GetButtonAToggle())
              m_pPlayer->ChangeColor();

          if (m_pController->GetButtonLSToggle())
              m_pPlayer->ChangeColor();
      }

      // If y position minus half of sprite's height is greater than worldsize, don't allow player to move up
      if (pos.y + h / 2 > m_vWorldSize.y)
      {
          if (m_pController->GetDPadDown())
              m_pPlayer->SetSpeed(-250.0f);
          else
              m_pPlayer->SetSpeed(0.0f);

          if (m_pController->GetDPadRight() && !(pos.x + w / 2 > m_vWorldSize.x))
              m_pPlayer->StrafeRight();

          if (m_pController->GetDPadLeft() && !(pos.x - w / 2 < 0))
              m_pPlayer->StrafeLeft();

          if (m_pController->GetButtonRSToggle())
              m_pObjectManager->FireGun(m_pPlayer, BULLET_SPRITE);

          if (m_pController->GetButtonXToggle())
              m_pObjectManager->FireGun(m_pPlayer, BULLET_SPRITE);

          if (m_pController->GetButtonAToggle())
              m_pPlayer->ChangeColor();

          if (m_pController->GetButtonLSToggle())
              m_pPlayer->ChangeColor();
      }
  } // if

  // gameover
  if (m_nCurLevel == -1)
  {
      // B button restarts game
      if (m_pController->GetButtonBToggle())
      {
          GameOverFunc();
      }
  }

  // if level is cleared, player can only press B
  if (m_nCurLevel >= 1 && m_nCurLevel <= 9 && m_pObjectManager->getLevelCleared() == true)
  {
      m_pPlayer->SetSpeed(0);
      m_pAudio->loop(VICTORY_MUSIC);
      
      if (m_pController->GetButtonBToggle())
      {
          m_pAudio->stop(VICTORY_MUSIC);
          m_pObjectManager->setLevelCleared(false);
          NextLevel();
      }
  }

} //ControllerHandler

/// Ask the object manager to draw the game objects. RenderWorld
/// is notified of the start and end of the frame so
/// that it can let Direct3D do its pipelining jiggery-pokery.

void CGame::RenderFrame() {
    m_pRenderer->BeginFrame();
    m_pObjectManager->draw();
    m_pParticleEngine->Draw();

    // if player is dead, it is gameover, and current level is -1
    if (m_pObjectManager->getPlayerHealth() <= 0)
    {
        GameOver = true;
        m_nCurLevel = -1;

        // if gameOverCalled is false, call BeginGame and set gameOverCalled to true.
        // this ensures that BeginGame is only called once
        if (gameOverCalled == false)
        {
            BeginGame();
            gameOverCalled = true;
        }
    }

    // if current level is not intro screen, and level is not end screen, and level is not completed
    if (m_nCurLevel > 0 && m_nCurLevel <= 9 && m_pObjectManager->getLevelCleared() == false)
    {
        // Displays current score
        string score = "Score " + to_string(m_pObjectManager->GetScore());
        m_pRenderer->DrawScreenText(score.c_str(), Vector2(800.0f, 725.0f), Colors::White);

        string health = "HP " + to_string(m_pObjectManager->getPlayerHealth());

        // displays player's health
        m_pRenderer->DrawScreenText(health.c_str(), Vector2(10.0f, 725.0f), Colors::Red);       
    }

    // if level is not game over, intro or end, and all enemies and bosses are defeated
    if (m_nCurLevel > 0 && m_nCurLevel <= 9 && m_pObjectManager->getEnemyCount() == 0 && m_pObjectManager->getBossCount() == 0)
    {
        m_pObjectManager->setLevelCleared(true);

        string level = "Level " + to_string(m_nCurLevel) + "   Cleared";
        //m_pRenderer->DrawScreenText(level.c_str(), Vector2(m_pPlayer->m_vPos.x, m_pPlayer->m_vPos.y), Colors::White);
        m_pRenderer->DrawScreenText(level.c_str(), Vector2(10, 725), Colors::LimeGreen);

        // Displays current score
        string score = "Score " + to_string(m_pObjectManager->GetScore());
        m_pRenderer->DrawScreenText(score.c_str(), Vector2(800.0f, 725.0f), Colors::White);
    }

    // if game is beaten
    if (m_nCurLevel == 10)
    {
        m_pObjectManager->setLevelCleared(true);

        string win = "You Win!";
        m_pRenderer->DrawScreenText(win.c_str(), Vector2(10, 725), Colors::LimeGreen);

        // Displays current score
        string score = "Score " + to_string(m_pObjectManager->GetScore());
        m_pRenderer->DrawScreenText(score.c_str(), Vector2(800.0f, 725.0f), Colors::White);
    }

    m_pRenderer->EndFrame();
} //RenderFrame

/// Make the camera follow the player, but don't let it get
/// too close to the edge. Unless the world is smaller than
/// the window, in which case we center everything.

void CGame::FollowCamera(){
  Vector3 vCameraPos(m_pPlayer->GetPos()); //player position

  if(m_vWorldSize.x > m_nWinWidth){ //world wider than screen
    vCameraPos.x = max(vCameraPos.x, m_nWinWidth/2.0f); //stay away from the left edge
    vCameraPos.x = min(vCameraPos.x, m_vWorldSize.x - m_nWinWidth/2.0f);  //stay away from the right edge
  } //if
  else vCameraPos.x = m_vWorldSize.x/2.0f; //center horizontally.
  
  if(m_vWorldSize.y > m_nWinHeight){ //world higher than screen
    vCameraPos.y = max(vCameraPos.y, m_nWinHeight/2.0f);  //stay away from the bottom edge
    vCameraPos.y = min(vCameraPos.y, m_vWorldSize.y - m_nWinHeight/2.0f); //stay away from the top edge
  } //if
  else vCameraPos.y = m_vWorldSize.y/2.0f; //center vertically

  m_pRenderer->SetCameraPos(vCameraPos); //camera to player
} //FollowCamera

/// Handle keyboard input, move the game objects and render 
/// them in their new positions and orientations. Notify the 
/// audio player at the start of each frame so that it can 
/// prevent multiple copies of a sound from starting on the
/// same frame. Notify the timer of the start and end of the
/// frame so that it can calculate frame time. 

void CGame::ProcessFrame(){
  KeyboardHandler(); //handle keyboard input
  ControllerHandler(); //handle controller input
  m_pAudio->BeginFrame(); //notify audio player that frame has begun
  
  m_pStepTimer->Tick([&](){ 
    m_pObjectManager->move(); //move all objects
    FollowCamera(); //make camera follow player
    m_pParticleEngine->step(); //advance particle animation
  });

  RenderFrame(); //render a frame of animation
} //ProcessFrame


// Zach: Level 1-3
// Easy
void CGame::Level_1() {
    float x_coordinate = 210.0f;
    char c = 'b';
    //Light enemies, introduce player to lighter enemies
    for (int i = 0; i < 2; i++)
    {
        if (c == 'b')
        {
            m_pObjectManager->createEnemy(Vector2(x_coordinate, 900.0f), 'b', 1);
            c = 'r';
        }
        else {
            m_pObjectManager->createEnemy(Vector2(x_coordinate, 1100.0f), 'r', 1);
            c = 'b';
        }
        x_coordinate += 200.0f;
    }
    x_coordinate = 700.0f;
    c = 'r';
    float y_coordinate = 1200.0f;
    //Heavy enemies, introduce player to the heavy enemies
    for (int i = 0; i < 2; i++)
    {
        if (c == 'r')
        {
            m_pObjectManager->createEnemy(Vector2(x_coordinate, y_coordinate), 'x', 2);
            c = 'b';
        }
        else
        {
            m_pObjectManager->createEnemy(Vector2(x_coordinate, y_coordinate), 'y', 2);
            c = 'r';
        }
        y_coordinate += 300.0f;
    }
    return;
}

// Normal
void CGame::Level_2()
{
    for (int i = 0; i < 4; i++)
    {
        if ( i < 2) {
            m_pObjectManager->createEnemy(Vector2(300.0f, 1000.0f), 'r', 5);
        }
        else {
            m_pObjectManager->createEnemy(Vector2(650.0f, 1000.0f), 'r', 6);
        }
    }

    float x = 400.0f;
    for (int i = 0; i < 4; i++) {
        if (i > 2) {
            m_pObjectManager->createEnemy(Vector2(500.0f, 1400.0f), 'b', 2);
        }
        else {
            m_pObjectManager->createEnemy(Vector2(x, 1500.0f), 'b', 1);
            x = 300.0f;
        }
    }

    x = 600.0f;
    for (int i = 0; i < 8; i++) {
        if (i < 4) {
            m_pObjectManager->createEnemy(Vector2(x, 1400.0f), 'x', 1);
            x -= 70.0f;
        }
        else {
            m_pObjectManager->createEnemy(Vector2(550.0f, 1400.0f), 'y', 6);
        }
    }
    return;
}

// Hard
void CGame::Level_3()
{
    float y = 900.0f;
    float x = 200.0f;
    for (int i = 0; i < 10; i++) {
        if (i < 5) {
            m_pObjectManager->createEnemy(Vector2(x,y), 'x', 5);
        }
        else {
            x = 700.0f;
            m_pObjectManager->createEnemy(Vector2(x,y), 'y', 5);
        }
    }

    x = 500.0f;
    y = 1500.0f;
    for (int i = 0; i < 5; i++) {
        if (i < 3) {
            m_pObjectManager->createEnemy(Vector2(x, y), 'x', 6);
        }
        else {
            x -= 100.0f;
            m_pObjectManager->createEnemy(Vector2(x,y), 'y', 6);
        }
    }

    y = 1300.0f;
    x = 850.0f;
    for (int i = 0; i < 8; i++) {
        if (i < 4) {
            m_pObjectManager->createEnemy(Vector2(x,y), 'b', 3);
        }
        else {
            x = 100.0f;
            m_pObjectManager->createEnemy(Vector2(x,y), 'r', 4);
        }
    }

    y = 1500.0f;
    x = 200.0f;
    for (int i = 0; i < 3; i++) {
        if (i < 1) {
            m_pObjectManager->createEnemy(Vector2(x,y), 'b', 4);
        }
        else {
            x = 600.0f;
            m_pObjectManager->createEnemy(Vector2(x,y), 'r', 3);
        }
    }

    y = 1800.0f;
    x = 100.0f;
    for (int i = 0; i < 8; i++) {
        m_pObjectManager->createEnemy(Vector2(x,y), 'b', 7);
       x += 70.0f;
    }
    return;
}


// Ephraim: Level 4-6
// Easy
void CGame::Level_4()
{
    // wave1
    // light enemies disperse left/right middle
    m_pObjectManager->createEnemy(Vector2(256, 1024), 'r', 6);
    m_pObjectManager->createEnemy(Vector2(318, 1024), 'b', 3);
    m_pObjectManager->createEnemy(Vector2(512, 1024), 'r', 1);
    m_pObjectManager->createEnemy(Vector2(706, 1024), 'b', 4);
    m_pObjectManager->createEnemy(Vector2(768, 1024), 'r', 5);

    // wave2
    // light enemies move diagonally accross
    m_pObjectManager->createEnemy(Vector2(194, 2024), 'b', 3);
    m_pObjectManager->createEnemy(Vector2(256, 2024), 'b', 1);
    m_pObjectManager->createEnemy(Vector2(318, 2024), 'b', 4);

    m_pObjectManager->createEnemy(Vector2(706, 2024), 'r', 4);
    m_pObjectManager->createEnemy(Vector2(768, 2024), 'r', 1);
    m_pObjectManager->createEnemy(Vector2(830, 2024), 'r', 3);

    // wave3
    // wing enemies descend straight down, middle enemies disperse
    m_pObjectManager->createEnemy(Vector2(264, 4024), 'r', 1);
    m_pObjectManager->createEnemy(Vector2(326, 4024), 'r', 1);

    m_pObjectManager->createEnemy(Vector2(450, 4024), 'b', 3);
    m_pObjectManager->createEnemy(Vector2(574, 4024), 'b', 4);

    m_pObjectManager->createEnemy(Vector2(698, 4024), 'r', 1);
    m_pObjectManager->createEnemy(Vector2(760, 4024), 'r', 1);

    return;
}

// Normal
void CGame::Level_5()
{
    // wave1
        // 3 light enemies diagonal right, 2 heavy enemies descend and stop in middle, 3 light enemies move diagonally left
    m_pObjectManager->createEnemy(Vector2(264, 1024), 'r', 3);
    m_pObjectManager->createEnemy(Vector2(326, 1024), 'r', 3);
    m_pObjectManager->createEnemy(Vector2(388, 1024), 'r', 3);

    m_pObjectManager->createEnemy(Vector2(450, 2024), 'x', 1);
    m_pObjectManager->createEnemy(Vector2(574, 2024), 'y', 1);

    m_pObjectManager->createEnemy(Vector2(636, 1024), 'b', 4);
    m_pObjectManager->createEnemy(Vector2(698, 1024), 'b', 4);
    m_pObjectManager->createEnemy(Vector2(760, 1024), 'b', 4);

    // wave2 
        // light enemies stream down leftmost side followed by heavy enemy
    m_pObjectManager->createEnemy(Vector2(326, 2724), 'r', 10);
    m_pObjectManager->createEnemy(Vector2(326, 2924), 'r', 10);
    m_pObjectManager->createEnemy(Vector2(326, 3124), 'x', 10);

    // light enemies stream down left side followed by heavy enemy
    m_pObjectManager->createEnemy(Vector2(388, 2724), 'r', 10);
    m_pObjectManager->createEnemy(Vector2(388, 2924), 'r', 10);
    m_pObjectManager->createEnemy(Vector2(388, 3124), 'x', 10);

    // light enemies stream down left middle side followed by heavy enemy
    m_pObjectManager->createEnemy(Vector2(450, 2724), 'r', 10);
    m_pObjectManager->createEnemy(Vector2(450, 2924), 'r', 10);
    m_pObjectManager->createEnemy(Vector2(450, 3124), 'x', 10);

    // light enemies stream down right middle side followed by heavy enemy
    m_pObjectManager->createEnemy(Vector2(574, 2724), 'b', 10);
    m_pObjectManager->createEnemy(Vector2(574, 2924), 'b', 10);
    m_pObjectManager->createEnemy(Vector2(574, 3124), 'y', 10);

    // light enemies stream down right side followed by heavy enemy
    m_pObjectManager->createEnemy(Vector2(636, 2724), 'b', 10);
    m_pObjectManager->createEnemy(Vector2(636, 2924), 'b', 10);
    m_pObjectManager->createEnemy(Vector2(636, 3124), 'y', 10);

    // light enemies stream down rightmost side followed by heavy enemy
    m_pObjectManager->createEnemy(Vector2(698, 2724), 'b', 10);
    m_pObjectManager->createEnemy(Vector2(698, 2924), 'b', 10);
    m_pObjectManager->createEnemy(Vector2(698, 3124), 'y', 10);

    // wave3
        // segregated heavy enemies zig zag
    m_pObjectManager->createEnemy(Vector2(264, 5000), 'x', 7);
    m_pObjectManager->createEnemy(Vector2(326, 5062), 'x', 7);
    m_pObjectManager->createEnemy(Vector2(388, 5124), 'x', 7);

    m_pObjectManager->createEnemy(Vector2(450, 5000), 'y', 7);
    m_pObjectManager->createEnemy(Vector2(512, 5062), 'y', 7);
    m_pObjectManager->createEnemy(Vector2(574, 5124), 'y', 7);

    // red and blue line stream down successively
    m_pObjectManager->createEnemy(Vector2(512, 5524), 'i', 9);
    m_pObjectManager->createEnemy(Vector2(512, 5724), 'j', 9);

    return;
}

// Hard
void CGame::Level_6()
{
    // rapid red and blue lines after waves
    m_pObjectManager->createEnemy(Vector2(512, 1174), 'i', 9);
    m_pObjectManager->createEnemy(Vector2(512, 1324), 'j', 9);
    m_pObjectManager->createEnemy(Vector2(512, 2174), 'i', 9);
    m_pObjectManager->createEnemy(Vector2(512, 2324), 'j', 9);
    m_pObjectManager->createEnemy(Vector2(512, 2474), 'i', 9);
    m_pObjectManager->createEnemy(Vector2(512, 2624), 'j', 9);
    m_pObjectManager->createEnemy(Vector2(512, 3174), 'i', 9);
    m_pObjectManager->createEnemy(Vector2(512, 3324), 'j', 9);
    m_pObjectManager->createEnemy(Vector2(512, 3474), 'i', 9);
    //m_pObjectManager->createEnemy(Vector2(512, 3624), 'j', 9);
    //m_pObjectManager->createEnemy(Vector2(512, 3774), 'i', 9);
    //m_pObjectManager->createEnemy(Vector2(512, 3924), 'j', 9);
    

    // red line and blue line constantly move up and down random
    m_pObjectManager->createEnemy(Vector2(512, 2024), 'i', 8);
    m_pObjectManager->createEnemy(Vector2(512, 3024), 'j', 8);

    // wave1
    // light red enemies stop and fill entire middle of screen
    m_pObjectManager->createEnemy(Vector2(78, 1024), 'r', 1);
    m_pObjectManager->createEnemy(Vector2(202, 1024), 'r', 1);
    //m_pObjectManager->createEnemy(Vector2(326, 1024), 'r', 1);
    //m_pObjectManager->createEnemy(Vector2(450, 1024), 'r', 1);
    //m_pObjectManager->createEnemy(Vector2(574, 1024), 'r', 1);
    //m_pObjectManager->createEnemy(Vector2(698, 1024), 'r', 1);
    m_pObjectManager->createEnemy(Vector2(822, 1024), 'r', 1);
    m_pObjectManager->createEnemy(Vector2(946, 1024), 'r', 1);

    // light blue enemies stop and fill entire middle of screen
    //m_pObjectManager->createEnemy(Vector2(78, 2024), 'b', 1);
    //m_pObjectManager->createEnemy(Vector2(202, 2024), 'b', 1);
    m_pObjectManager->createEnemy(Vector2(326, 2024), 'b', 1);
    m_pObjectManager->createEnemy(Vector2(450, 2024), 'b', 1);
    m_pObjectManager->createEnemy(Vector2(574, 2024), 'b', 1);
    m_pObjectManager->createEnemy(Vector2(698, 2024), 'b', 1);
    //m_pObjectManager->createEnemy(Vector2(822, 2024), 'b', 1);
    //m_pObjectManager->createEnemy(Vector2(946, 2024), 'b', 1);

    // wave2
    // heavy red enemies stop and fill entire middle of screen
    m_pObjectManager->createEnemy(Vector2(78, 2024), 'x', 1);
    m_pObjectManager->createEnemy(Vector2(202, 2024), 'x', 1);
    //m_pObjectManager->createEnemy(Vector2(326, 3024), 'x', 1);
    //m_pObjectManager->createEnemy(Vector2(450, 3024), 'x', 1);
    //m_pObjectManager->createEnemy(Vector2(574, 3024), 'x', 1);
    //m_pObjectManager->createEnemy(Vector2(698, 3024), 'x', 1);
    m_pObjectManager->createEnemy(Vector2(822, 2024), 'x', 1);
    m_pObjectManager->createEnemy(Vector2(946, 2024), 'x', 1);

    // wave 3
        // heavy blue enemies stop and fill entire middle of screen
    //m_pObjectManager->createEnemy(Vector2(78, 4024), 'y', 1);
    //m_pObjectManager->createEnemy(Vector2(202, 4024), 'y', 1);
    m_pObjectManager->createEnemy(Vector2(326, 3024), 'y', 1);
    m_pObjectManager->createEnemy(Vector2(450, 3024), 'y', 1);
    m_pObjectManager->createEnemy(Vector2(574, 3024), 'y', 1);
    m_pObjectManager->createEnemy(Vector2(698, 3024), 'y', 1);
    //m_pObjectManager->createEnemy(Vector2(822, 4024), 'y', 1);
    //m_pObjectManager->createEnemy(Vector2(946, 4024), 'y', 1);
    

    return;
}


// Christopher: Level 7-9
// Easy
void CGame::Level_7()
{
    // wave1
    // light enemies stop in middle
    m_pObjectManager->createEnemy(Vector2(450, 1024), 'r', 1);
    m_pObjectManager->createEnemy(Vector2(512, 1024), 'b', 1);
    m_pObjectManager->createEnemy(Vector2(574, 1024), 'r', 1);
    

    // wave2
    // light enemies move diagonally
    m_pObjectManager->createEnemy(Vector2(194, 2024), 'r', 4);
    m_pObjectManager->createEnemy(Vector2(256, 2024), 'b', 4);
    m_pObjectManager->createEnemy(Vector2(318, 2024), 'r', 4);

    m_pObjectManager->createEnemy(Vector2(706, 2024), 'b', 3);
    m_pObjectManager->createEnemy(Vector2(768, 2024), 'r', 3);
    m_pObjectManager->createEnemy(Vector2(830, 2024), 'b', 3);

    // wave3
    // 3 enemies descend and move left, 3 enemies descend and stop in middle, 3 enemies descend and move right
    m_pObjectManager->createEnemy(Vector2(264, 4024), 'r', 6);
    m_pObjectManager->createEnemy(Vector2(326, 4024), 'r', 6);
    m_pObjectManager->createEnemy(Vector2(388, 4024), 'b', 6);

    m_pObjectManager->createEnemy(Vector2(450, 4024), 'b', 1);
    m_pObjectManager->createEnemy(Vector2(512, 4024), 'r', 1);
    m_pObjectManager->createEnemy(Vector2(574, 4024), 'b', 1);

    m_pObjectManager->createEnemy(Vector2(636, 4024), 'b', 5);
    m_pObjectManager->createEnemy(Vector2(698, 4024), 'r', 5);
    m_pObjectManager->createEnemy(Vector2(760, 4024), 'r', 5);

    return;
}

// Normal
void CGame::Level_8()
{
    // wave1 
    // heavy enemies zig zag
    m_pObjectManager->createEnemy(Vector2(264, 1024), 'y', 7);
    m_pObjectManager->createEnemy(Vector2(326, 1024), 'x', 7);
    //m_pObjectManager->createEnemy(Vector2(388, 1024), 'y', 7);

    //m_pObjectManager->createEnemy(Vector2(450, 1024), 'y', 7);
    m_pObjectManager->createEnemy(Vector2(512, 1024), 'x', 7);
    m_pObjectManager->createEnemy(Vector2(574, 1024), 'y', 7);

    // wave2
    // 3 heavy enemies diagonal right, 3 heavy enemies descend and stop in middle, 3 heavy enemies move diagonally left
    m_pObjectManager->createEnemy(Vector2(264, 1500), 'x', 4);
    m_pObjectManager->createEnemy(Vector2(326, 1500), 'y', 4);
    m_pObjectManager->createEnemy(Vector2(388, 1500), 'x', 4);

    m_pObjectManager->createEnemy(Vector2(450, 2500), 'b', 1);
    m_pObjectManager->createEnemy(Vector2(512, 2500), 'r', 1);
    m_pObjectManager->createEnemy(Vector2(574, 2500), 'b', 1);

    m_pObjectManager->createEnemy(Vector2(636, 1500), 'x', 3);
    m_pObjectManager->createEnemy(Vector2(698, 1500), 'y', 3);
    m_pObjectManager->createEnemy(Vector2(760, 1500), 'x', 3);
    

    // wave3
    // light enemies move down left side
    m_pObjectManager->createEnemy(Vector2(62, 5000), 'r', 10);
    m_pObjectManager->createEnemy(Vector2(62, 5200), 'b', 10);
    m_pObjectManager->createEnemy(Vector2(62, 5400), 'r', 10);

    // heavy enemies stop in middle
    m_pObjectManager->createEnemy(Vector2(450, 3400), 'y', 1);
    m_pObjectManager->createEnemy(Vector2(512, 3400), 'x', 1);
    m_pObjectManager->createEnemy(Vector2(574, 3400), 'y', 1);

    // light enemies move down right side
    m_pObjectManager->createEnemy(Vector2(962, 5000), 'r', 10);
    m_pObjectManager->createEnemy(Vector2(962, 5200), 'b', 10);
    m_pObjectManager->createEnemy(Vector2(962, 5400), 'r', 10);

    return;
}

// Hard
void CGame::Level_9()
{
    // rapid red and blue lines
    m_pObjectManager->createEnemy(Vector2(512, 724), 'i', 9);
    m_pObjectManager->createEnemy(Vector2(512, 874), 'j', 9);
    m_pObjectManager->createEnemy(Vector2(512, 1024), 'i', 9);
    m_pObjectManager->createEnemy(Vector2(512, 1174), 'j', 9);
    m_pObjectManager->createEnemy(Vector2(512, 1324), 'i', 9);
    m_pObjectManager->createEnemy(Vector2(512, 1474), 'j', 9);
   
    // red line and blue line constantly move up and down
    m_pObjectManager->createEnemy(Vector2(512, 1024), 'i', 8);
    m_pObjectManager->createEnemy(Vector2(512, 2048), 'j', 8);

    // wave1
    // heavy enemies stop in the middle of the screen
    m_pObjectManager->createEnemy(Vector2(450, 1024), 'y', 1);
    m_pObjectManager->createEnemy(Vector2(512, 1024), 'x', 1);
    m_pObjectManager->createEnemy(Vector2(574, 1024), 'y', 1);

    // wave2
    // light red enemies move down, then left
    m_pObjectManager->createEnemy(Vector2(264, 3024), 'r', 6);
    m_pObjectManager->createEnemy(Vector2(326, 3024), 'r', 6);
    m_pObjectManager->createEnemy(Vector2(388, 3024), 'r', 6);
    m_pObjectManager->createEnemy(Vector2(450, 3024), 'r', 1);

    // light blue enemies move down, then right
    m_pObjectManager->createEnemy(Vector2(574, 3024), 'b', 1);
    m_pObjectManager->createEnemy(Vector2(636, 3024), 'b', 5);
    m_pObjectManager->createEnemy(Vector2(698, 3024), 'b', 5);
    m_pObjectManager->createEnemy(Vector2(760, 3024), 'b', 5);
    
    // wave 3
    // light enemies stop and fill entire middle of screen
    m_pObjectManager->createEnemy(Vector2(78, 5024), 'r', 1);
    m_pObjectManager->createEnemy(Vector2(202, 5024), 'b', 1);
    m_pObjectManager->createEnemy(Vector2(326, 5024), 'r', 1);
    m_pObjectManager->createEnemy(Vector2(450, 5024), 'b', 1);
    m_pObjectManager->createEnemy(Vector2(574, 5024), 'r', 1);
    m_pObjectManager->createEnemy(Vector2(698, 5024), 'b', 1);
    m_pObjectManager->createEnemy(Vector2(822, 5024), 'r', 1);
    m_pObjectManager->createEnemy(Vector2(946, 5024), 'b', 1);
    
    return;
}
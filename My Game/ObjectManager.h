/// \file ObjectManager.h
/// \brief Interface for the object manager CObjectManager.

#pragma once

#include <list>

#include "Object.h"

#include "Component.h"
#include "Common.h"
#include "Settings.h"

#include "BlackJack.h"
#include "HotShot.h"
#include "LittleBoy.h"
#include "Enemy.h"
using namespace std;

/// \brief The object manager.
///
/// A collection of all of the game objects.

class CObjectManager: 
  public CComponent, 
  public CCommon, 
  public CSettings{

  private:
    list<CObject*> m_stdObjectList; ///< Object list.

    void BroadPhase(); ///< Broad phase collision detection and response.
    void NarrowPhase(CObject* p0, CObject* p1); ///< Narrow phase collision detection and response.
    bool AtWorldEdge(CObject* p); ///< Test whether at the edge of the world.
    void CullDeadObjects(); ///< Cull dead objects.

    int m_nScore = 0; //keeps track of current score
    int level = 0; //keeps track of what level is taking place
    bool boss_present = false;
    float previousTime = 0;
    bool boss_active = false;
    CObject* currentBoss = nullptr; //keeps up with cureently active boss object

    int enemyCount = 0; // number of enemies
    int bossCount = 0;  // number of bosses
    bool levelCleared = false;  // level is completed or not

    int playerHealth = 3;   // player health

  public:
    CObjectManager(); ///< Constructor.
    ~CObjectManager(); ///< Destructor.

    CObject* create(eSpriteType t, const Vector2& v); ///< Create new object.
    CObject* createHotShot(const Vector2& v); //Create Hotshot Boss
    CObject* createLittleBoy(const Vector2& v); //Create LittleBoy Boss
    CObject* createBlackJack(const Vector2& v); //Create BlackJack Boss
    CObject* createEnemy(const Vector2& v, char c, int p ); //Create enemy

    void clear(); ///< Reset to initial conditions.
    void move(); ///< Move all objects.
    void draw(); ///< Draw all objects.
    void add( CObject * obj ); //Adds a CObject to the CObjectManager

    void FireGun(CObject* p, eSpriteType bullet); ///< Fire object's gun.
    CObject* PlayerShoots(); //The player shot thier gun
    CObject* GetBoss(); //Returns current boss
    int GetScore(); // get the current score
    int enemyCountFunc(); //Count how many normal enemies are present
    void EnemyKilledScore();    // adds 100 to score when enemy is killed
    void ResetScore();      // resets score if gameover
    void SetScore(int x); //Reset score when player started the level they died in
    void updateLevel(int x); //updates the current level number
    void SpawnBoss(); //Spawn Boss according to level

    int getEnemyCount();    // get number of enemies
    int getBossCount();     // get number of bosses
    void setBossCount(int b);   // set number of bosses
    bool getLevelCleared();     // get status of level
    void setLevelCleared(bool l);   // set status of level

    void decrementPlayerHealth();
    int getPlayerHealth();
    void setPlayerHealth(int h);

    void setEnemyCount(int e);
    void setBossPresent(bool b);

}; //CObjectManager
/// \file ObjectManager.cpp
/// \brief Code for the the object manager class CObjectManager.

#include "ObjectManager.h"
#include "ComponentIncludes.h"
#include "ParticleEngine.h"

CObjectManager::CObjectManager(){
} //constructor

/// Destruct all of the objects in the object list.

CObjectManager::~CObjectManager(){
  for(auto const& p: m_stdObjectList) //for each object
    delete p; //delete object
} //destructor

/// Create an object and put a pointer to it on the object list.
/// \param t Sprite type.
/// \param v Initial position.
/// \return Pointer to the object created.

CObject* CObjectManager::create(eSpriteType t, const Vector2& v){
  CObject* p = new CObject(t, v); 
  m_stdObjectList.push_back(p); 
  return p;
} //create

/// Delete all of the objects managed by the object manager. 
/// This involves deleting all of the CObject instances pointed
/// to by the object list, then clearing the object list itself.

void CObjectManager::clear(){
  for(auto const& p: m_stdObjectList) //for each object
    delete p; //delete object

  m_stdObjectList.clear(); //clear the object list
} //clear

/// Draw the objects in the object list.

void CObjectManager::draw(){
  for(auto const& p: m_stdObjectList) //for each object
    m_pRenderer->Draw(*(CSpriteDesc2D*)p);
} //draw

/// Test whether an object's left, right, top or bottom
/// edge has crossed the left, right, top, bottom edge of
/// the world, respectively. This function assumes that the
/// bottom left corner of the world is at the origin.
/// \param p Pointer to an object.
/// \return true if the object is at the edge of the world.

bool CObjectManager::AtWorldEdge(CObject* p){   
  const Vector2 pos = p->m_vPos; //position of center of sprite
  float w, h; //sprite width and height
  m_pRenderer->GetSize(p->m_nSpriteIndex, w, h);
  
  /*
   // original worldedge
   if (pos.x - w / 2 < 0 || pos.x + w / 2 > m_vWorldSize.x ||
       pos.y - h / 2 < 0 || pos.y + h / 2 > m_vWorldSize.y)
       return true;
       */

  /*
  // world edge is now outside of player's view.
  // can now kill objects outside of play area.
  if (pos.x + w < 0 || pos.x - w > m_vWorldSize.x ||
      pos.y + h < 0 || pos.y - h > m_vWorldSize.y)
      return true;
      */
  
      // world edge is now outside of player's view.
      // can now kill objects outside of play area.
      // objects above worldedge.y, such as enemies, are not killed.
  if (pos.x + w < 0 || pos.x - w > m_vWorldSize.x ||
      pos.y + h < 0)
      return true;
      

  return false; //default
} //AtWorldEdge

/// Move all of the objects and perform 
/// broad phase collision detection and response.

void CObjectManager::move(){
  for(auto const& p: m_stdObjectList){ //for each object

    p->move(); //move it

    switch(p->m_nSpriteIndex){
      case RED_LIGHT_ENEMY: {
        const Vector2 v = m_pPlayer->m_vPos - p->m_vPos;//distance from player
        bool bVisible = v.Length() < 800.0f; //player in range for attack

        if(bVisible && m_pStepTimer->GetTotalSeconds() > p->m_fGunTimer + 0.9f){ //shoots every 0.9 seconds
          p->m_fGunTimer = m_pStepTimer->GetTotalSeconds();
          CObject* ebullet = p->FireGun();
          m_stdObjectList.push_back( ebullet );
        } //if
        if (AtWorldEdge(p))//dont leave screen
            p->CollisionResponse();
      } //case
      break;

      case HOTSHOT: {
          const Vector2 v = m_pPlayer->m_vPos - p->m_vPos;//distance from player
          bool bVisible = v.Length() < 800.0f;
          if (bVisible) { //Player is in hotshots range
              if (m_pStepTimer->GetTotalSeconds() > p->m_fGunTimer + 1.3) { //does something every 1.3 seconds
                  p->m_fGunTimer = m_pStepTimer->GetTotalSeconds();
                  int choose_attack = rand() % 100; //randomly choose hotshots attack
                  if (choose_attack < 70) { //shoots fireballs
                      const Vector2 target_player = m_pPlayer->m_vPos - p->m_vPos;
                      CObject* fire_ball = p->FireGun();
                      m_stdObjectList.push_back( fire_ball );
                  }
                  else {// Hotshot summons firetrap
                      float range = 512.0f - m_pPlayer->m_vPos.x;
                      Vector2 pos(0.0f, m_pPlayer->m_vPos.y);
                      if (range >= 0)
                          pos.x = m_pPlayer->m_vPos.x + 250.0f; //spawn right of the player
                      else
                          pos.x = m_pPlayer->m_vPos.x - 250.0f; //spawn to the left of player

                      CObject* fire_trap = p->Attack1(pos);
                      fire_trap->m_fGunTimer = m_pStepTimer->GetTotalSeconds();
                      m_stdObjectList.push_back(fire_trap);
                  }
              } //if
          }//if
      }
     break;

      case BLUE_LIGHT_ENEMY: {
          const Vector2 v = m_pPlayer->m_vPos - p->m_vPos; //distance from player
          bool bVisible = v.Length() < 800.0f; //player in range for attack

          if (bVisible && m_pStepTimer->GetTotalSeconds() > p->m_fGunTimer + 0.9f) { //shoots every ,9 seconds
              p->m_fGunTimer = m_pStepTimer->GetTotalSeconds();
              CObject* ebullet = p->FireGun();
              m_stdObjectList.push_back( ebullet );
          } //if
          if (AtWorldEdge(p))//Dont leave screen
              p->CollisionResponse();
      } //case
      break;

      case RED_HEAVY_ENEMY: {
          const Vector2 v = m_pPlayer->m_vPos - p->m_vPos;//distance from player
          bool bVisible = v.Length() < 800.0f; //player in range for attack

          if (bVisible && m_pStepTimer->GetTotalSeconds() > p->m_fGunTimer + 2) {//shoots every 2 seconds
              p->m_fGunTimer = m_pStepTimer->GetTotalSeconds();
              CObject* ebullet = p->FireGun();
              m_stdObjectList.push_back(ebullet);
          } //if
          if (AtWorldEdge(p))
              p->CollisionResponse();
      } //case
      break;

      case BLUE_HEAVY_ENEMY: {
          const Vector2 v = m_pPlayer->m_vPos - p->m_vPos; //distance from player
          bool bVisible = v.Length() < 800.0f; //players in range for attack

          if (bVisible && m_pStepTimer->GetTotalSeconds() > p->m_fGunTimer + 2) { //shoots every 2 seconds
              p->m_fGunTimer = m_pStepTimer->GetTotalSeconds();
              CObject* ebullet = p->FireGun();
              m_stdObjectList.push_back(ebullet);
          } //if
          if (AtWorldEdge(p)) //Make sure enemy doe snot leave screen
             p->CollisionResponse();
      } //case
      break;

      case LILBOY: {
          const Vector2 range = m_pPlayer->GetPos() - p->GetPos(); //distance between player and littleboy
          bool in_range = abs(range.x) < 100.0f && range.Length() < 800.0f; //player is in rnage for attack
          if (in_range) 
          {
              if (m_pStepTimer->GetTotalSeconds() > p->m_fGunTimer + 0.5) { //Either shoots gun or drops bomb every 1/2 seconds
                  p->m_fGunTimer = m_pStepTimer->GetTotalSeconds();
                  int choose_attack = rand() % 100; //Randomly chooses whcih attack littleboy wil do
                  if (choose_attack < 25) {
                      CObject* bomb = p->Attack1(Vector2::Zero);
                      m_stdObjectList.push_back(bomb);
                  }
                  else {//Shoots diverse colors of bullets
                      CObject* b1 = p->FireGun();
                      CObject* b2 = p->FireGun();
                      CObject* b3 = p->FireGun();
                      m_stdObjectList.push_back(b1);
                      m_stdObjectList.push_back(b2);
                      m_stdObjectList.push_back(b3);
                  }
              }
          }
      }
                 if (AtWorldEdge(p)) //Make sure littleboy does not leave screen
                     p->CollisionResponse();
      break; 

      //Blackjack
      case BLACK_JACK: {
          const Vector2 range = m_pPlayer->GetPos() - p->GetPos(); //distance between player and blackjack
          bool in_range = range.x < 180.0f && range.Length() < 700.0f; //Player is in range for attack
          if (in_range) {
              //FireGun, change player colors, change its own colors, forcefield, Cards, charge at player
              if (m_pStepTimer->GetTotalSeconds() > p->m_fGunTimer + 1) { //Does something every second
                  p->m_fGunTimer = m_pStepTimer->GetTotalSeconds();
                  int choose_attack = rand() % 100; //randomly choose blackjacks next move
                  //summons random poker cards every 21 seconds
                  if (choose_attack < 23 && !p->charging && m_pStepTimer->GetTotalSeconds() > p->m_fCardTimer + 21) {
                      CObject* c1 = new CObject(CARD, Vector2(200.0f, 1000.0f));
                      CObject* c2 = new CObject(CARD, Vector2(500.0f, 1000.0f));
                      CObject* c3 = new CObject(CARD, Vector2(750.0f, 1000.0f));
                      c1->SetVelocity(Vector2(0.0f,-110.0f));
                      c2->SetVelocity(c1->GetVelocity());
                      c3->SetVelocity(c2->GetVelocity());
                      m_stdObjectList.push_back(c1);
                      m_stdObjectList.push_back(c2);
                      m_stdObjectList.push_back(c3);
                      p->m_fCardTimer = m_pStepTimer->GetTotalSeconds();
                  }//Cards
                  else if (choose_attack < 75 && !p->charging) { //Fire gun
                      CObject* b1 = p->FireGun();
                      CObject* b2 = p->FireGun();
                      m_stdObjectList.push_back(b1);
                      m_stdObjectList.push_back(b2);
                  }//FireGun
                  else if (!p->ff_on && m_pStepTimer->GetTotalSeconds() > p->m_fForceFieldTimer + 5 && !p->m_bStrafeFoward && !p->m_bStrafeBack ) {
                      CObject* ff = new CObject(FORCE_FIELD, p->GetPos()); 
                      p->ActivateForceField(ff);
                      m_stdObjectList.push_back(ff);
                  }//Force field
                  else {
                      if(!p->charging && !p->ff_on)
                        p->Charge();
                  }//Charge at player
              }

          }
          //blackjacks Blackhole is summoned every 10 seconds as long as it is not charging or another black hole is active
          if (m_pStepTimer->GetTotalSeconds() > p->m_fBlackHoleTimer + 10 && !p->charging && !p->bh_on) {
              float range = 512.0f - m_pPlayer->m_vPos.x;
              Vector2 pos(0.0f,m_pPlayer->m_vPos.y);
              if (range >= 0)
                  pos.x = m_pPlayer->m_vPos.x + 250.0f; //will spawn to the right of the player
              else 
                  pos.x = m_pPlayer->m_vPos.x - 250.0f; //will spawn to the players left

              CObject* bh = new CObject(BLACK_HOLE, pos);
              m_stdObjectList.push_back(bh);
              p->bh_on = true;
              m_pAudio->loop(BH_SOUND);
          }
          //black jack will automatically change the players color 
          if (m_pStepTimer->GetTotalSeconds() > p->m_fChangeColorTimer + 5) {
              p->m_fChangeColorTimer = m_pStepTimer->GetTotalSeconds();
              m_pPlayer->ChangeColor();
          }

          //Make sure black jack does not leave the screen
          if (AtWorldEdge(p))
              p->CollisionResponse();
      }
      break;

      // player bullet
      case BULLET_SPRITE: { 
          if (currentBoss && !currentBoss->ff_on) {
              const Vector2 range = p->GetPos() - currentBoss->GetPos();
              bool point_blank_range = abs(range.y) < 250.0f && abs(range.x) < 100.0f;//Only true when players bullet is at range for dodge to be necessary
              int reaction = rand()%100; //Probabilty if the boss will dodge 
              bool dodge; //Does the boss dodge it?
              //Black Jakc and littleboy can dodge players bullets
              if (currentBoss->m_nSpriteIndex == BLACK_JACK) //Black jack has 50 chance of dodging the bullet
                  dodge = reaction < 50;
              else if (currentBoss->m_nSpriteIndex == LILBOY) //Littleboy has a 25 chance of dodging the bullet
                  dodge = reaction < 25;
              else { dodge = false; }
              //If the bullet is in point blank range, the boss is not moving up or down, and the dodge probabilty is a success
              //Then boss will dodge the bullet
              if (point_blank_range && (!currentBoss->m_bStrafeBack && !currentBoss->m_bStrafeFoward) && dodge) {
                  const Vector2 boss = currentBoss->GetPos();
                  const Vector2 bullet = p->GetPos();
                  if (AtWorldEdge(currentBoss)) { //Make sure boss does not leave the screen
                      float width, hieght;
                      m_pRenderer->GetSize(currentBoss->m_nSpriteIndex, width, hieght);
                      if (boss.x - width / 2 < 0 && !currentBoss->m_bStrafeLeft)
                          currentBoss->StrafeRight();
                      else if (boss.x + width / 2 > m_vWorldSize.x && !currentBoss->m_bStrafeRight)
                          currentBoss->StrafeLeft();
                  }
                  else {
                      if (bullet.x < boss.x && !currentBoss->m_bStrafeLeft)
                          currentBoss->StrafeRight();
                      else if (bullet.x > boss.x && !currentBoss->m_bStrafeRight)
                          currentBoss->StrafeLeft();
                      else {
                          if (boss.x <= m_vWinCenter.x && !currentBoss->m_bStrafeLeft)
                              currentBoss->StrafeRight();
                          else
                              currentBoss->StrafeLeft();
                      }
                  }

              }
          }
          // kill bullet at worldedge
          if (p->m_vPos.y >= m_vWorldSize.y)
          {
              p->kill();
          }
      }
      break;

      // enemy red bullet
      case RED_BULLET:
        if (AtWorldEdge(p)) {
           p->kill();
           //m_pAudio->play(RICOCHET_SOUND);
          } //if
       break;

       // enemy blue bullet
      case BLUE_BULLET:
          if (AtWorldEdge(p)) {
              p->kill();
              //m_pAudio->play(RICOCHET_SOUND);
          } //if
          break;

      case REDFIRE: //Hotshots fire trap will only lasts 3 seconds
          if (m_pStepTimer->GetTotalSeconds() > p->m_fGunTimer + 3)
              p->kill();
          break;

      case LILBOMB: {
          Vector2 pos = p->GetPos();
          if (pos.y <= 200.0f) { //Littleboys bomb is triggered
              p->kill();
              CObject* exp = new CObject(BIG_EXPLOSION, pos);
              m_stdObjectList.push_back(exp);
          }
          
      }
          break;

      case BLACK_HOLE: { //black hole lasts for only 5 seconds
          if (m_pStepTimer->GetTotalSeconds() > p->m_fGunTimer + 5) {
              p->kill();
              m_pAudio->stop(BH_SOUND);
              currentBoss->m_fBlackHoleTimer = m_pStepTimer->GetTotalSeconds();
              currentBoss->bh_on = false;
          }
      }
         break;

      case CARD: 
          if (AtWorldEdge(p)) { //card leaves screen
              p->kill();
          } //if
       break;
      case QUEEN: //Queen card leaves screen
          if (AtWorldEdge(p)) {
              p->kill();
          }//if
       break;
      case JACK:
          if (AtWorldEdge(p)) { //Jack card leaves screen
              p->kill();
          } //if
       break;
      case BIG_EXPLOSION: //Explosion stays on screen for less than one second
          if (m_pStepTimer->GetTotalSeconds() >  p->m_fGunTimer + 0.85) {
              p->kill();
            }
          break;
      
      case RED_LINE:
          if (AtWorldEdge(p))
          {
              p->kill();
          }
          break;

      case BLUE_LINE:
          if (AtWorldEdge(p))
          {
              p->kill();
          }
          break;

      default:
          break;
    } //switch
  } //for

  //now do object-object collision detection and response and
  //remove any dead objects from the object list.

  BroadPhase(); //broad phase collision detection and response
  CullDeadObjects(); //remove dead objects from object list
  SpawnBoss(); //Check and see if level is ready to spawn the boss
} //move

/// Create a bullet object and a flash particle effect.
/// It is assumed that the object is round and that the bullet
/// appears at the edge of the object in the direction
/// that it is facing and continues moving in that direction.
/// \param pObj Pointer to an object.
/// \param bullet Sprite type of bullet.

void CObjectManager::FireGun(CObject* pObj, eSpriteType bullet){
  m_pAudio->play(GUN_SOUND);

  const Vector2 view = pObj->GetViewVector();
  Vector2 pos = pObj->GetPos() + 
    0.5f*m_pRenderer->GetWidth(pObj->m_nSpriteIndex)*view;

  //create bullet object

  CObject* pBullet = m_pObjectManager->create(bullet, pos); //create bullet

  const Vector2 norm(view.y, -view.x); //normal to direction
  const float m = 2.0f*m_pRandom->randf() - 1.0f;
  const Vector2 deflection = 0.01f*m*norm;

  pBullet->SetVelocity(m_pPlayer->GetVelocity() + 500.0f*(view + deflection));
  pBullet->SetOrientation(m_pPlayer->GetOrientation()); 

  //particle effect for gun fire
  
  CParticleDesc2D d;

  d.m_nSpriteIndex = SPARK_SPRITE;
  d.m_vPos = pos;
  d.m_vVel = pObj->GetSpeed()*view;
  d.m_fLifeSpan = 0.5f;
  d.m_fScaleInFrac = 1.0f;
  d.m_fFadeOutFrac = 1.0f;
  d.m_fMaxScale = 0.5f;
  
  m_pParticleEngine->create(d);
} //FireGun

/// This is a "bring out yer dead" Monty Python type of thing.
/// Iterate through the objects and check whether their "is dead"
/// flag has been set. If so, then delete its pointer from
/// the object list and destruct the object.

void CObjectManager::CullDeadObjects(){
  for(auto i=m_stdObjectList.begin(); i!=m_stdObjectList.end();){
      //Regular enemy died
       if ((*i)->IsDead() && ((*i)->m_nSpriteIndex == RED_LIGHT_ENEMY || (*i)->m_nSpriteIndex == BLUE_LIGHT_ENEMY || (*i)->m_nSpriteIndex == BLUE_HEAVY_ENEMY
          || (*i)->m_nSpriteIndex == RED_HEAVY_ENEMY))
      {
          enemyCount--;
             delete* i; //delete object-
          i = m_stdObjectList.erase(i); //remove from object list and advance to next object
      }
      //Boss is dead
      else if ((*i)->IsDead() && ((*i)->m_nSpriteIndex == HOTSHOT || (*i)->m_nSpriteIndex == LILBOY || (*i)->m_nSpriteIndex == BLACK_JACK))
      {
          m_pAudio->play(DEATH_SOUND);
          CObject* exp = new CObject(BIG_EXPLOSION, (*i)->GetPos());
          m_stdObjectList.push_back(exp);
          bossCount--;
          delete* i; //delete object-
          i = m_stdObjectList.erase(i); //remove from object list and advance to next object
      }
      //Littleboys bomb explodes
      else if ((*i)->IsDead() && (*i)->m_nSpriteIndex == LILBOMB) {
          m_pAudio->play(DEATH_SOUND);
          CObject* exp = new CObject(BIG_EXPLOSION, (*i)->GetPos());
          m_stdObjectList.push_back(exp);
          delete* i;
          i = m_stdObjectList.erase(i);
      }
      else if ((*i)->m_nSpriteIndex == FORCE_FIELD && (*i)->IsDead()) { //blackjacks Force field is destroyed
           currentBoss->ff_on = false;
           currentBoss->m_fForceFieldTimer = m_pStepTimer->GetTotalSeconds();
           delete* i;
           i = m_stdObjectList.erase(i);
           currentBoss->force_field = nullptr;
       }
      //anything else that is dead, bullets, player etc.
      else if (((*i)->IsDead()) || ((*i)->m_nSpriteIndex == SMALL_EXPLOSION && (*i)->explosionTooOld())) { //"He's dead, Dave." --- Holly, Red Dwarf
      delete *i; //delete object-
      i = m_stdObjectList.erase(i); //remove from object list and advance to next object
    } //if

    else i++; //advance to next object
  } //for
} //CullDeadObjects

/// Perform collision detection and response for all pairs
/// of objects in the object list, making sure that each
/// pair is processed only once.

void CObjectManager::BroadPhase(){
  for(auto i=m_stdObjectList.begin(); i!=m_stdObjectList.end(); i++){
      for (auto j = next(i); j != m_stdObjectList.end(); j++)
          NarrowPhase(*i, *j);
  } //for
} //BroadPhase

/// Perform collision detection and response for a pair of objects.
/// We are talking about bullets hitting the player and the
/// turrets here. When a collision is detected the response
/// is to delete the bullet (i.e. mark its "is dead" flag)
/// play a particle effect at the point of contact, and play one 
/// sound for the player and another for the turrets.
/// \param p0 Pointer to the first object.
/// \param p1 Pointer to the second object.

void CObjectManager::NarrowPhase(CObject* p0, CObject* p1){
  eSpriteType t0 = (eSpriteType)p0->m_nSpriteIndex;
  eSpriteType t1 = (eSpriteType)p1->m_nSpriteIndex;

  if(p0->m_Sphere.Intersects(p1->m_Sphere)){ //bounding spheres intersect

   //Contact between the light red enemy and the player
      if (t0 == BLUE_SHIP && t1 == RED_LIGHT_ENEMY) //player hits enemy
      {
          p0->hit(); // player is hit
          p0->CollisionResponse(); // collision on player
      }

      else if (t1 == BLUE_SHIP && t0 == RED_LIGHT_ENEMY) //enemy hit by player
      {
          p1->hit(); // player is hit
          p1->CollisionResponse(); // collision on player
      }

      if (t0 == RED_SHIP && t1 == RED_LIGHT_ENEMY) //player hits turret
      {
          p0->hit(); // player is hit
          p0->CollisionResponse(); // collision on player
      }

      else if (t1 == RED_SHIP && t0 == RED_LIGHT_ENEMY) //enemy hit by player
      {
          p1->hit(); // player is hit
          p1->CollisionResponse(); // collision on player
      }

      //Contact between the heavy red enemy and the player
      if (t0 == BLUE_SHIP && t1 == RED_HEAVY_ENEMY) //player hits enemy
      {
          p0->hit(); // player is hit
          p0->CollisionResponse(); // collision on player
      }

      else if (t1 == BLUE_SHIP && t0 == RED_HEAVY_ENEMY) //enemy hit by player
      {
          p1->hit(); // player is hit
          p1->CollisionResponse(); // collision on player
      }

      if (t0 == RED_SHIP && t1 == RED_HEAVY_ENEMY) //player hits turret
      {
          p0->hit(); // player is hit
          p0->CollisionResponse(); // collision on player
      }

      else if (t1 == RED_SHIP && t0 == RED_HEAVY_ENEMY) //enemy hit by player
      {
          p1->hit(); // player is hit
          p1->CollisionResponse(); // collision on player
      }
   
  /// Contact between the light blue enemy and the player
      if (t0 == BLUE_SHIP && t1 == BLUE_LIGHT_ENEMY) //player hits enemy
      {
          p0->hit(); // player is hit
          p0->CollisionResponse(); // collision on player
      }

      else if (t1 == BLUE_SHIP && t0 == BLUE_LIGHT_ENEMY) //enemy hit by player
      {
          p1->hit(); // player is hit
          p1->CollisionResponse(); // collision on player
      }

      if (t0 == RED_SHIP && t1 == BLUE_LIGHT_ENEMY) //player hits enemy
      {
          p0->hit(); // player is hit
          p0->CollisionResponse(); // collision on player
      }

      else if (t1 == RED_SHIP && t0 == BLUE_LIGHT_ENEMY) //enemy hit by player
      {
          p1->hit(); // player is hit
          p1->CollisionResponse(); // collision on player
      }

      /// Contact between the heavy blue enemy and the player
      if (t0 == BLUE_SHIP && t1 == BLUE_HEAVY_ENEMY) //player hits enemy
      {
          p0->hit(); // player is hit
          p0->CollisionResponse(); // collision on player
      }

      else if (t1 == BLUE_SHIP && t0 == BLUE_HEAVY_ENEMY) //enemy hit by player
      {
          p1->hit(); // player is hit
          p1->CollisionResponse(); // collision on player
      }

      if (t0 == RED_SHIP && t1 == BLUE_HEAVY_ENEMY) //player hits enemy
      {
          p0->hit(); // player is hit
          p0->CollisionResponse(); // collision on player
      }

      else if (t1 == RED_SHIP && t0 == BLUE_HEAVY_ENEMY) //enemy hit by player
      {
          p1->hit(); // player is hit
          p1->CollisionResponse(); // collision on player
      }

//Contact between the light enemy and the players bullet
    else if(t0 == BULLET_SPRITE && t1 == RED_LIGHT_ENEMY){ //bullet hits turret
        p0->kill(); // destroy bullet
      m_pAudio->play(CLANG_SOUND);
    } //if

    else if(t1 == BULLET_SPRITE && t0 == RED_LIGHT_ENEMY){ //enemy hit by bullet
      p1->kill(); // destroy bullet
      p0->enemyHit();   // turret is hit
      m_pAudio->play(CLANG_SOUND);
    } //else if
    else if (t0 == BULLET_SPRITE && t1 == BLUE_LIGHT_ENEMY) { //bullet hits enemy
        p0->kill(); // destroy bullet
        m_pAudio->play(CLANG_SOUND);
    } //if

    else if (t1 == BULLET_SPRITE && t0 == BLUE_LIGHT_ENEMY) { //enemy hit by bullet
        p1->kill(); // destroy bullet
        p0->enemyHit(); // turret is hit
        m_pAudio->play(CLANG_SOUND);
    } //else if


      //Contact between the heavy enemy and the players bullet
    else if (t0 == BULLET_SPRITE && t1 == RED_HEAVY_ENEMY) { //bullet hits turret
          p0->kill(); // destroy bullet
          m_pAudio->play(CLANG_SOUND);
      } //if

    else if (t1 == BULLET_SPRITE && t0 == RED_HEAVY_ENEMY) { //enemy hit by bullet
          p1->kill(); // destroy bullet
          p0->enemyHit();   // turret is hit
          m_pAudio->play(CLANG_SOUND);
      } //else if
    else if (t0 == BULLET_SPRITE && t1 == BLUE_HEAVY_ENEMY) { //bullet hits enemy
          p0->kill(); // destroy bullet
          m_pAudio->play(CLANG_SOUND);
      } //if

    else if (t1 == BULLET_SPRITE && t0 == BLUE_HEAVY_ENEMY) { //enemy hit by bullet
          p1->kill(); // destroy bullet
          p0->enemyHit(); // turret is hit
          m_pAudio->play(CLANG_SOUND);
      } //else if

//Contact between the regular enemy's bullet and the player
     else if(t0 == RED_BULLET && t1 == BLUE_SHIP){ // blue player hit by red bullet
      p0->kill(); // destroy bullet
    } //else if

     else if(t1 == RED_BULLET && t0 == BLUE_SHIP){ // red bullet hits blue player
      p1->kill(); // destroy bullet
      p0->hit(); // player is hit
    } //else if
     else if (t0 == BLUE_BULLET && t1 == RED_SHIP) { // red player hit by blue bullet
        p0->kill(); // destroy bullet
    } //else if

     else if (t1 == BLUE_BULLET && t0 == RED_SHIP) { // blue bullet hits red player
        p1->kill(); // destroy bullet
        p0->hit(); // player is hit
    } //else if


     // same color bullet and ship collision
     else if (t0 == BLUE_BULLET && t1 == BLUE_SHIP) { // blue player hit by blue bullet
          p0->kill(); // destroy bullet
          m_nScore += 10;
          m_pAudio->play(ABSORB_SOUND);
      } //else if

     else if (t1 == BLUE_BULLET && t0 == BLUE_SHIP) { // blue bullet hits blue player
          p1->kill(); // destroy bullet
          m_nScore += 10;
          m_pAudio->play(ABSORB_SOUND);
      } //else if
      
     else if (t0 == RED_BULLET && t1 == RED_SHIP) { // red player hit by red bullet
          p0->kill(); // destroy bullet
          m_nScore += 10;
          m_pAudio->play(ABSORB_SOUND);
      } //else if

     else if (t1 == RED_BULLET && t0 == RED_SHIP) { // red bullet hits red player
          p1->kill(); // destroy bullet
          m_nScore += 10;
          m_pAudio->play(ABSORB_SOUND);
      } //else if


//Contact between the hotshot's fireball and the player
     else if (t0 == FIREBALL && t1 == BLUE_SHIP) { //player hit by bullet
        p0->kill(); // destroy bullet
    } //else if

     else if (t1 == FIREBALL && t0 == BLUE_SHIP) { //bullet hits player
        p1->kill(); // destroy bullet
        p0->hit(); // player is hit
    } //else if
     else if (t0 == FIREBALL && t1 == RED_SHIP) { //player hit by bullet
        p0->kill(); // destroy bullet
    } //else if

     else if (t1 == FIREBALL && t0 == RED_SHIP) { //bullet hits player
        p1->kill(); // destroy bullet
        p0->hit(); // player is hit
    } //else if


//Contact between the player bullet and hotshot
     else if (t0 == BULLET_SPRITE && t1 == HOTSHOT) { //hotshot hit by bullet
        p0->kill(); // destroy bullet
    } //else if

     else if (t1 == BULLET_SPRITE && t0 == HOTSHOT) { //bullet hits hotshot
        p1->kill(); // destroy bullet
        p0->enemyHit(); // hotshot is hit
    } //else if

     else if (t0 == BULLET_SPRITE && t1 == HOTSHOT) { //hotshot hit by bullet
        p0->kill(); // destroy bullet
    } //else if

     else if (t1 == BULLET_SPRITE && t0 == HOTSHOT) { //bullet hits hotshot
        p1->kill(); // destroy bullet
        p0->enemyHit(); // hotshot is hit
    } //else if

//Contact between hotshot's RED firetrap and the player
     else if (t0 == REDFIRE && t1 == BLUE_SHIP) { //player runs into fire
        p0->kill();
    } //else if

     else if (t1 == REDFIRE && t0 == BLUE_SHIP) { //fire hits player
        p1->kill();
        p0->hit(); // player is hit
    } //else if

     
    //Contact between blackJacks black hole and the player
    //If the player is in the center of the black hole, the player automatically dies
     else if (t0 == BLACK_HOLE && t1 == BLUE_SHIP) { 
         p1->hit();
    } //else if
     else if (t1 == BLACK_HOLE && t0 == BLUE_SHIP) { 
         p0->hit();
    } //else if
     else if (t0 == BLACK_HOLE && t1 == RED_SHIP) { 
         p1->hit();
    } //else if
     else if (t1 == BLACK_HOLE && t0 == RED_SHIP) { 
         p0->hit();
    } //else if

//Contact between the players bullet and blackjacks cards
//If the card is hit, it will reveal what type of card it is: queen or jack
     else if (t0 == CARD && t1 == BULLET_SPRITE) {
     p1->kill();
     p0->m_nSpriteIndex = p0->reveal;
    } //else if
     else if (t1 == CARD && t0 == BULLET_SPRITE) {
     p0->kill();
     p1->m_nSpriteIndex = p1->reveal;
    } //else if


//Contact between Players bullet and Littleboy
     else if (t1 == BULLET_SPRITE && t0 == LILBOY) {
        p1->kill();
        p0->enemyHit(); //Littleboy is hit
     }
     else if (t0 == BULLET_SPRITE && t1 == LILBOY) {
        p0->kill();
        p1->enemyHit(); //Littleboy is hit
     }
//Contact between players bullet and BlackJack
     else if (t1 == BULLET_SPRITE && t0 == BLACK_JACK) {
     p1->kill();
     p0->enemyHit(); //Blackjack is hit
     }
     else if (t0 == BULLET_SPRITE && t1 == BLACK_JACK) {
     p0->kill();
     p1->enemyHit(); //Blackjack is hit -
     }

//Contact between Blackjack and the player
     else if (t0 == BLACK_JACK && t1 == BLUE_SHIP) {
     p1->hit();
    } //else if
     else if (t1 == BLACK_JACK && t0 == BLUE_SHIP) {
     p0->hit();
    } //else if
     else if (t0 == BLACK_JACK && t1 == RED_SHIP) {
     p1->hit();
    } //else if
     else if (t1 == BLACK_JACK && t0 == RED_SHIP) {
     p0->hit();
    } //else if

     

//Contact between players bullet and the enemies forcefield
     else if (t0 == BULLET_SPRITE && t1 == FORCE_FIELD) {
        p1->enemyHit();
        p0->kill();
     }
     else if (t0 == FORCE_FIELD && t1 == BULLET_SPRITE) {
        p0->enemyHit();
        p1->kill();
     }
//Contact between the player and the queen or jack card
//If the player is touched by a queen card, the player will gain +1 health
//If player touches the jack card, the player will take regular damage
     else if (t0 == QUEEN && (t1 == RED_SHIP || t1 == BLUE_SHIP)) {
     p0->kill();
     p1->heal();
    } //else if
     else if (t1 == QUEEN && (t0 == RED_SHIP || t0 == BLUE_SHIP)) {
     p1->kill();
     p0->heal();
    } //else if
     else if (t0 == JACK && (t1 == RED_SHIP || t1 == BLUE_SHIP)) {
     p0->kill();
     p1->hit();
    } //else if
     else if (t1 == JACK && (t0 == RED_SHIP || t0 == BLUE_SHIP)) {
     p1->kill();
     p0->hit();
    } //else if
//Players bullet hits the cards
     else if (t0 == QUEEN && t1 == BULLET_SPRITE) {
     p0->kill();
     p1->heal();
    } //else if
     else if (t1 == QUEEN && t0 == BULLET_SPRITE) {
     p1->kill();
     p0->heal();
    } //else if
     else if (t0 == JACK && t1 == BULLET_SPRITE) {
     p0->kill();
    } //else if
     else if (t1 == JACK && t0 == BULLET_SPRITE) {
     p1->kill();
    } //else if


//Contact between the player bullet and lilboy
     else if (t0 == BULLET_SPRITE && t1 == LILBOY) { //lilboy hit by bullet
     p0->kill(); // destroy bullet
    } //else if

     else if (t1 == BULLET_SPRITE && t0 == LILBOY) { //bullet hits lilboy
     p1->kill(); // destroy bullet
     p0->enemyHit(); // hotshot is hit
    } //else if

// Contact between Little Boy's bomb and player
     else if (t0 == LILBOMB && t1 == BLUE_SHIP) { // blue player hit by bomb
     p0->kill(); // destroy bullet
    } //else if

     else if (t1 == LILBOMB && t0 == BLUE_SHIP) { // bomb hits blue player
     p1->kill(); // destroy bomb
    } //else if
     else if (t0 == LILBOMB && t1 == RED_SHIP) { // red player hit by bomb
     p0->kill(); // destroy bomb
    } //else if

     else if (t1 == LILBOMB && t0 == RED_SHIP) { // bomb hits red player
     p1->kill(); // destroy bomb
    } //else if


// Player is caught in explosion
     else if (t0 == BIG_EXPLOSION && t1 == BLUE_SHIP && getBossCount() != 0) { 
     p1->hit();
    } //else if
     else if (t1 == BIG_EXPLOSION && t0 == BLUE_SHIP && getBossCount() != 0) { 
     p0->hit(); // player is hit
    } //else if
     else if (t0 == BIG_EXPLOSION && t1 == RED_SHIP && getBossCount() != 0) { 
     p1->hit();
    } //else if
     else if (t1 == BIG_EXPLOSION && t0 == RED_SHIP && getBossCount() != 0) { 
     p0->hit(); // player is hit
    } //else if

    /*
    // Contact between red line and player
     else if (t0 == RED_LINE && t1 == BLUE_SHIP) { // blue player hit by bomb
     p1->hit();
    } //else if

     else if (t1 == RED_LINE && t0 == BLUE_SHIP) { // bomb hits blue player
     p0->hit(); // player is hit
    } //else if
    
     
    // Contact between blue line and player
     else if (t0 == BLUE_LINE && t1 == RED_SHIP) { // red player hit by bomb
     p1->hit(); // player is hit
    } //else if

     else if (t1 == BLUE_LINE && t0 == RED_SHIP) { // bomb hits red player
     p0->hit(); // player is hit
    } //else if
    */
    

  } //if
} //NarrowPhase

// get player score
int CObjectManager::GetScore()
{
    return m_nScore;
} //GetScoreCount

// increase score by 100 when enemy is killed
void CObjectManager::EnemyKilledScore()
{
    m_nScore += 100;
}

// reset score when gameover
void CObjectManager::ResetScore()
{
    m_nScore = 0;
}

CObject* CObjectManager::createHotShot(const Vector2& v) //Create Hotshot Boss
{
    CObject * h = new HotShot( v );
    m_stdObjectList.push_back(h);
    //bossCount++;
    return h;
}


CObject* CObjectManager::createLittleBoy(const Vector2& v) //Create LittleBoy Boss
{
    CObject * l = new LittleBoy(v);
    currentBoss = l;
    m_stdObjectList.push_back(l);
    //bossCount++;
    return l;
}

CObject* CObjectManager::createBlackJack(const Vector2& v) //Create BlackJack Boss
{
    CObject* bj = new BlackJack(v);
    currentBoss = bj;
    m_stdObjectList.push_back(bj);
    //bossCount++;
    return bj;
}

CObject* CObjectManager::createEnemy(const Vector2& v, char c, int p ) //Create Normal Enemies
{
    CObject* e = new CEnemyObject( v, c, p );
    m_stdObjectList.push_back(e);

    // do not count RED_LINE and BLUE_LINE as enemies
    if (e->m_nSpriteIndex != RED_LINE && e->m_nSpriteIndex != BLUE_LINE)
        enemyCount++;

    return e;
}

void CObjectManager::add( CObject* obj ) //Adds a CObject to the CObjectManager
{
    m_stdObjectList.push_back(obj);
}

CObject* CObjectManager::PlayerShoots() //The player is shooting their gun
{
    CObject* player_bullet = m_pPlayer->FireGun();
    m_stdObjectList.push_back(player_bullet);
    return player_bullet;
}


int CObjectManager::enemyCountFunc() //Count how many enemies that are alive
{
    int count = 0;
    for (auto i = m_stdObjectList.begin(); i != m_stdObjectList.end(); i++)
    {
        if ((*i)->m_nSpriteIndex == RED_LIGHT_ENEMY || (*i)->m_nSpriteIndex == BLUE_LIGHT_ENEMY)
            count++;
    }
    return count;
}

void CObjectManager::SpawnBoss() //Spawns boss according to the level
{
    if (!boss_present && getEnemyCount() == 0)  //If all regular enemies are defeated, then spawn the boss
    {
        switch (level)
        {
        case 3: { //If level 3, spawn hotshot boss
            createHotShot(Vector2(512, 1100));
            boss_present = true;
            setPlayerHealth(3);
            }
            break;
        case 6: { //If level 6, spawn littleboy boss
            createLittleBoy(Vector2(512, 1100));
            boss_present = true;
            setPlayerHealth(3);
        }
            break;
        case 9: { //If level 9, spawn blackjack
            createBlackJack(Vector2(512, 600));
            boss_present = true;
            setPlayerHealth(3);
        }
            break;
        default:
            break;
        }
    }
}

//Update level so object manager knows what level it is 
void CObjectManager::updateLevel(int x)
{
    level = x;
}

// get number of enemies
int CObjectManager::getEnemyCount()
{
    return enemyCount;
}

// get number of bosses
int CObjectManager::getBossCount()
{
    return bossCount;
}

// set number of bosses
void CObjectManager::setBossCount(int b)
{
    bossCount = b;
}

// return status of level
bool CObjectManager::getLevelCleared()
{
    return levelCleared;
}

// set status of level
void CObjectManager::setLevelCleared(bool l)
{
    levelCleared = l;
}

//Player loses health
void CObjectManager::decrementPlayerHealth()
{
    playerHealth--;
}

//Getting player health
int CObjectManager::getPlayerHealth()
{
    return playerHealth;
}

//Setting player health
void CObjectManager::setPlayerHealth(int h)
{
    playerHealth = h;
}

//Set enemy count
void CObjectManager::setEnemyCount(int e)
{
    enemyCount = e;
}

//Is the the boss alive?
void CObjectManager::setBossPresent(bool b)
{
    boss_present = b;
    if (!boss_present)
        currentBoss = nullptr; //make sure pointer is not holding any memory while there is no boss
}

//Set score payer had when they died
void CObjectManager::SetScore(int x) 
{
    m_nScore = x;
}
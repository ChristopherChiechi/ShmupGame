/// \file Object.cpp
/// \brief Code for the game object class CObject.

#include "Object.h"
#include "ComponentIncludes.h"
#include "GameDefines.h"
#include "Particle.h"
#include "ParticleEngine.h"
#include "ObjectManager.h"  // using this for enemyHit function


// default constructor
CObject::CObject() :
    CObject((eSpriteType)0, Vector2::Zero) {
} //CObject

/// Create and initialize an object given its sprite type
/// and initial position.
/// \param t Type of sprite.
/// \param p Initial position of object.

CObject::CObject(eSpriteType t, const Vector2& p){ 
  m_nSpriteIndex = t;
  m_vPos = p;
  explosionBirthTime = m_pStepTimer->GetTotalSeconds(); // gets when explosion is created

  m_pRenderer->GetSize(t, m_vRadius.x, m_vRadius.y);
  m_vRadius *= 0.5f;
  
  const Vector2 topleft(-m_vRadius.x, m_vRadius.y);
  const Vector2 bottomrt(m_vRadius.x, -m_vRadius.y);

  m_Sphere.Radius = max(m_vRadius.x, m_vRadius.y);
  m_Sphere.Center = (Vector3)m_vPos;
  
  m_fGunTimer = m_pStepTimer->GetTotalSeconds();

  if (m_nSpriteIndex == FORCE_FIELD) //force field has 10 health
      m_fHealth = 10;
  if (m_nSpriteIndex == CARD) { //If card randomly choose which type it will be: queen or jack
      m_fHealth = 3;
      int choose = rand() % 100;
      if (choose <= 50)
          reveal = QUEEN;
      else
          reveal = JACK;
  }

} //constructor

CObject::~CObject() {
} //destructor

/// Move and update all bounding shapes.
/// The player object gets moved by the controller, everything
/// else moves an amount that depends on its velocity and the
/// frame time.

void CObject::move(){
  m_vOldPos = m_vPos;
  
  const float t = m_pStepTimer->GetElapsedSeconds();

//Player movement
  if(m_nSpriteIndex == BLUE_SHIP || m_nSpriteIndex == RED_SHIP){ //Move player object
    const Vector2 viewvec = GetViewVector();
    m_vPos += m_fSpeed*t*viewvec;
    //m_fRoll += m_fRotSpeed*t;
    
    Vector2 norm(viewvec.y, -viewvec.x); 
    const float delta = 300.0f*t;

    if(m_bStrafeRight)
      m_vPos += delta*norm;

    else if(m_bStrafeLeft)
      m_vPos -= delta*norm;

    else if(m_bStrafeBack)
      m_vPos -= delta*viewvec;

    m_bStrafeLeft = m_bStrafeRight = m_bStrafeBack = false;
  } //if


  else m_vPos += m_vVelocity*t;

  m_Sphere.Center = (Vector3)m_vPos; //update bounding sphere

  // Object animation. From Ned's Turkey Farm
  const size_t nFrameCount = m_pRenderer->GetNumFrames(m_nSpriteIndex); // nFrameCount = number of sprite's frames
  const float dt = 1000.0f * m_fFrameInterval / (1500.0f + fabsf(m_vVel.x));    // calculates animation speed

  // calculates current frame
  if (nFrameCount > 1 && m_pStepTimer->GetTotalSeconds() > m_fFrameTimer + dt) {
      m_fFrameTimer = m_pStepTimer->GetTotalSeconds();
      m_nCurrentFrame = (m_nCurrentFrame + 1) % nFrameCount;
  }

} //move


void CObject::CollisionResponse() {
    Vector2 newPos;
    float width, height, x;
    m_pRenderer->GetSize(m_nSpriteIndex, width, height);
    x = (float)(rand() % 700) + 200; //Choose random location to place enemy
    if (m_vPos.x + width < 0) { //Out of bounds on the left side of the screen
        newPos = Vector2(x, 500.0f);
    }
    else if (m_vPos.x - width > m_vWorldSize.x) { //Out of bounds on the right side of the screen
        newPos = Vector2(x, 500.0f);
    }
    else if (m_vPos.y + height < 0) { //Out of bounds on the bottom of the screen
        newPos = Vector2(x, 750.0f);
    }

    if (m_nSpriteIndex == BLACK_JACK || m_nSpriteIndex == HOTSHOT) { //Respawn bosses with bigger portal sprite
        m_pAudio->play(RESPAWN_SOUND);
        CParticleDesc2D spawn;
        spawn.m_nSpriteIndex = LARGE_RESPAWN;
        spawn.m_vPos = newPos;
        spawn.m_fLifeSpan = 0.8f;
        spawn.m_fMaxScale = 0.8f;
        spawn.m_fScaleInFrac = 0.2f;
        spawn.m_fFadeOutFrac = 0.8f;
        spawn.m_fFadeInFrac = 0.8f;
        spawn.m_fScaleOutFrac = 0.2f;
        m_vPos = newPos;
        UpdatePos();
        m_pParticleEngine->create(spawn);
        if (force_field) {
            force_field->m_vPos = newPos;
            force_field->UpdatePos();
        }
    } //smaller portal sprite
    else if (m_nSpriteIndex == BLUE_HEAVY_ENEMY || m_nSpriteIndex == BLUE_LIGHT_ENEMY || m_nSpriteIndex == RED_HEAVY_ENEMY || m_nSpriteIndex == RED_LIGHT_ENEMY) {
        m_pAudio->play(RESPAWN_SOUND);
        CParticleDesc2D spawn;
        spawn.m_nSpriteIndex = SMALL_RESPAWN;
        spawn.m_vPos = newPos;
        spawn.m_fLifeSpan = 0.8f;
        spawn.m_fMaxScale = 0.8f;
        spawn.m_fScaleInFrac = 0.2f;
        spawn.m_fFadeOutFrac = 0.8f;
        spawn.m_fFadeInFrac = 0.8f;
        spawn.m_fScaleOutFrac = 0.2f;
        m_vPos = newPos;
        m_vOldPos = m_vPos;
        m_pParticleEngine->create(spawn);
    }
    else {
        m_vPos = m_vOldPos;
    }
} //CollisionResponse

/// Set the strafe left flag.

void CObject::StrafeLeft(){
  m_bStrafeLeft = true;
} //StrafeLeft

/// Set the strafe right flag.

void CObject::StrafeRight(){
  m_bStrafeRight = true;
} //StrafeRight

/// Set the strafe back flag.

void CObject::StrafeBack(){
  m_bStrafeBack = true;
} //StrafeBack

//Set the Strafe foward Flag
void CObject::StrafeForward() {
    m_bStrafeFoward = true;
}


void CObject::Charge() { //Charging
    charging = true;
    m_bStrafeBack = true;
}


/// Perform a death particle effect to mark the death of an object.

void CObject::DeathFX(){
  m_pAudio->play(DEATH_SOUND);
  m_pObjectManager->create(SMALL_EXPLOSION, m_vPos);

  /*
  CParticleDesc2D explosion;
  explosion.m_nSpriteIndex = SMALL_EXPLOSION;
  explosion.m_vPos = GetPos();
  explosion.m_fLifeSpan = 0.9f;
  explosion.m_fMaxScale = 0.5f;
  explosion.m_fScaleInFrac = 0.4f;
  explosion.m_fFadeOutFrac = 0.8f;
  explosion.m_fFadeInFrac = 0.5f;
  explosion.m_fScaleOutFrac = 0.8f;
  
  m_pParticleEngine->create(explosion);


  CParticleDesc2D d;
  d.m_nSpriteIndex = SMALL_SMOKE;
  d.m_vPos = m_vPos;
      
  d.m_fLifeSpan = 0.6f;
  d.m_fMaxScale = 0.5f;
  d.m_fScaleInFrac = 0.2f;
  d.m_fFadeOutFrac = 0.8f;
  d.m_fScaleOutFrac = d.m_fFadeOutFrac;

  m_pParticleEngine->create(d);
  */
} //DeathFX

/// Kill an object by marking its "is dead" flag. The object
/// will get deleted later at the appropriate time.

void CObject::kill(){
  m_bDead = true;
  if (m_nSpriteIndex == BLACK_JACK) {
      if (force_field)
          force_field->kill();
  }

  //DeathFX();
} //kill

/// Reader function for the "is dead" flag.
/// \return true if marked as being dead, that is, ready for disposal.

bool CObject::IsDead(){
  return m_bDead;
} //IsDead

/// Set the object's speed, assuming that the object
/// moves according to its speed and view vector.
/// \param speed Speed.

void CObject::SetSpeed(float speed){
  m_fSpeed = speed;
} //SetVelocity

/// Set the object's rotational speed in revolutions per second.
/// \param speed Rotational speed in RPS.

void CObject::SetRotSpeed(float speed){
  m_fRotSpeed = speed;
} //Rotate

// set health
void CObject::SetHealth(int h)
{
    m_fHealth = h;
}

// get health
int CObject::GetHealth()
{
    return m_fHealth;
}

// player is hit
void CObject::hit()
{
    // calculate the difference in time since the player was last hit and the current time
    float CurrentHitTime = m_pStepTimer->GetTotalSeconds();
    float difference = CurrentHitTime - PreviousHitTime;
    //HitFX();

    // if the difference is greater than 2 and the level is not completed, decrement health.
    // this gives the player 2 seconds of invincibility after getting hit.
    if (difference >= 3.0f && m_pObjectManager->getLevelCleared() == false)
    {
        HitFX();        // hit effects
       // m_fHealth--;    // decrement health
        m_pObjectManager->decrementPlayerHealth();  // decrement player health
        PreviousHitTime = m_pStepTimer->GetTotalSeconds();  // get new previous hit time
        m_pController->Vibrate(100, 100);
        m_pAudio->play(PLAYERHIT_SOUND);
    }
    // if health <= 0, kill object
    //if (m_fHealth <= 0)
    if (m_pObjectManager->getPlayerHealth() <= 0)
    {
        DeathFX();  // death effects
        kill();     // kill object
    }
}

// enemy is hit
void CObject::enemyHit()
{
    HitFX();        // hit effects
    m_fHealth--;    // decrement health

    // if health <= 0, increment score by 100, and kill object
    if (m_fHealth <= 0)
    {
        m_pObjectManager->EnemyKilledScore();   // +100 score
        kill(); // kill object
        DeathFX();  // death effects
    }
}

/// Reader function for the bounding sphere.
/// \return The bounding sphere.

const BoundingSphere& CObject::GetBoundingSphere(){
  return m_Sphere;
} //GetBoundingSphere

/// Reader function for position.
/// \return Position.

const Vector2& CObject::GetPos(){
  return m_vPos;
} //GetPos

/// Reader function for speed.
/// \return Speed.

float CObject::GetSpeed(){
  return m_fSpeed;
} //GetSpeed

/// Get the view vector.
/// \return The view vector.

Vector2 CObject::GetViewVector(){
  return Vector2(-sinf(m_fRoll), cosf(m_fRoll));
} //GetViewVector

/// Reader function for the orientation. A 2D object's
/// orientation is its roll amount in 3D space.
/// \return The view vector.

float CObject::GetOrientation(){
  return m_fRoll;
} //GetOrientation

/// Writer function for the orientation. A 2D object's
/// orientation is its roll amount in 3D space.
/// \param angle The new orientation.

void CObject::SetOrientation(float angle){
  m_fRoll = angle;
} //GetOrientation

/// Reader function for velocity.
/// \return Velocity.

const Vector2& CObject::GetVelocity(){
  return m_vVelocity;
} //GetVel

/// Writer function for velocity.
/// \param v Velocity.

void CObject::SetVelocity(const Vector2& v){
  m_vVelocity = v;
} //SetVel

void CObject::ChangeColor() //Player changes their color to either red or blue
{
    if (m_nSpriteIndex == BLUE_SHIP)
        m_nSpriteIndex = RED_SHIP;
    else if (m_nSpriteIndex == RED_SHIP)
        m_nSpriteIndex = BLUE_SHIP;

    m_pRenderer->GetSize(m_nSpriteIndex, m_vRadius.x, m_vRadius.y);
    m_vRadius *= 0.5f;

    const Vector2 topleft(-m_vRadius.x, m_vRadius.y);
    const Vector2 bottomrt(m_vRadius.x, -m_vRadius.y);

    m_Sphere.Radius = max(m_vRadius.x, m_vRadius.y);
    m_Sphere.Center = (Vector3)m_vPos;

    m_fGunTimer = m_pStepTimer->GetTotalSeconds();
}


CObject* CObject::Attack1( const Vector2& v) //Players signature move
{
    CObject* o = new CObject(TURRET_SPRITE, v);
    return o;
}


CObject* CObject::FireGun() //Players gun is shot by default
{
    m_pAudio->play(PLAYERGUN_SOUND);

    const Vector2 view = GetViewVector();
    Vector2 pos = GetPos() +
        0.5f * m_pRenderer->GetWidth(m_nSpriteIndex) * view;

    //create bullet object

    CObject* pBullet = new CObject(BULLET_SPRITE, pos); //create bullet

    const Vector2 norm(view.y, -view.x); //normal to direction
    const float m = 2.0f * m_pRandom->randf() - 1.0f;
    const Vector2 deflection = 0.01f * m * norm;
    pBullet->SetVelocity(m_pPlayer->GetVelocity() + 500.0f * (view + deflection));
    pBullet->SetOrientation(135);

    //particle effect for gun fire

    CParticleDesc2D d;

    d.m_nSpriteIndex = SPARK_SPRITE;
    d.m_vPos = pos;
    d.m_vVel = GetSpeed() * view;
    d.m_fLifeSpan = 0.25f;
    d.m_fScaleInFrac = 0.4f;
    d.m_fFadeOutFrac = 0.5f;
    d.m_fMaxScale = 0.5f;
    d.m_f4Tint = XMFLOAT4(Colors::Yellow);

    m_pParticleEngine->create(d);

    return pBullet;
} 

void CObject::HitFX() 
{
    m_pAudio->play(DAMAGE_SOUND);
    CParticleDesc2D damage_effect;
    damage_effect.m_nSpriteIndex = DAMAGE_SPRITE;
    damage_effect.m_vPos = m_vPos - (0.5f * m_pRenderer->GetWidth(m_nSpriteIndex) * GetViewVector());
    damage_effect.m_fLifeSpan = .60f;
    damage_effect.m_fScaleInFrac = 0.4f;
    damage_effect.m_fFadeOutFrac = 0.5f;
    damage_effect.m_fMaxScale = 0.5f;

    m_pParticleEngine->create(damage_effect);
}

void CObject::React(const Vector2& v) 
{

}

// returns if explosion animation's lifespan is over
bool CObject::explosionTooOld()
{
    return m_pStepTimer->GetTotalSeconds() - explosionBirthTime >= explosionLifeTime;
}

void CObject::ActivateForceField(CObject* ff) { //set force field for object for keep up with
    force_field = ff;
    ff_on = true;
}

void CObject::UpdatePos() { //keep up with position of the object
    m_Sphere.Center = (Vector3)m_vPos;
}

void CObject::heal() { //heals the player
        m_pObjectManager->setPlayerHealth(m_pObjectManager->getPlayerHealth() + 1);
        CParticleDesc2D heart_effect; //heart sprite appears when player is healed
        heart_effect.m_nSpriteIndex = HEART1_SPRITE;
        heart_effect.m_vPos = m_vPos + (0.5f * m_pRenderer->GetWidth(m_nSpriteIndex) * GetViewVector());
        heart_effect.m_fLifeSpan = .60f;
        heart_effect.m_fScaleInFrac = 0.4f;
        heart_effect.m_fFadeOutFrac = 0.5f;
        heart_effect.m_fMaxScale = 0.5f;
        m_pParticleEngine->create(heart_effect);
}
/// \file Object.h
/// \brief Interface for the game object class CObject.

#pragma once

#include "GameDefines.h"
#include "Renderer.h"
#include "Common.h"
#include "Component.h"
#include "SpriteDesc.h"
#include "ComponentIncludes.h"
#include "Particle.h"
#include "ParticleEngine.h"
#include "Helpers.h"

/// \brief The game object. 
///
/// CObject is the abstract representation of an object.

class CObject:
  public CCommon,
  public CComponent,
  public CSpriteDesc2D
{
  friend class CObjectManager;

  protected:
    BoundingSphere m_Sphere; ///< Bounding sphere.
    Vector3 m_vVel = Vector3::Zero; // Velocity. Needed for player animation.

    float m_fSpeed = 0; ///< Speed.
    float m_fRotSpeed = 0; ///< Rotational speed.
    float orig_speed = 0;
    int m_fHealth = 3; // set health = 3
    int m_pPlayerScore = 0;
    Vector2 m_vOldPos; ///< Last position.
    Vector2 m_vVelocity; ///< Velocity.
    bool m_bDead = false; ///< Is dead or not.
    bool charging = false; ///<If object is charging or not
    bool ff_on = false; ///<Force field is activated
    bool bh_on = false; ///A black hole is activated by this object

    bool m_bStrafeLeft = false; ///< Strafe left.
    bool m_bStrafeRight = false; ///< Strafe right.
    bool m_bStrafeBack = false; ///< Strafe back.
    bool m_bStrafeFoward = false; ///< Strafe Foward


    Vector2 m_vRadius; ///< Half width and height of object sprite.

    float m_fGunTimer = 0.0f; ///< Gun fire timer.
    float m_fChangeColorTimer; //timer for changing color
    float m_fForceFieldTimer; //timer for force field
    float m_fBlackHoleTimer; //timer for black hole
    float m_fCardTimer; //timer for cards

    // responsible for animation
    float m_fFrameTimer = 0.0f; ///< Last time the frame was changed.
    float m_fFrameInterval = 0.1f; ///< Interval between frames.

    float PreviousHitTime = 0.0f;  // Time since player was last hit

    // responsible for explosion animation lifespan
    float explosionBirthTime = 0.0f;
    float explosionLifeTime = 0.5f;

    //Force field of Object
    CObject* force_field = nullptr;
    //black hole of the object
    CObject* black_hole = nullptr;
    eSpriteType reveal; //queen or jack card
  public:
    CObject(); // default constructor
    CObject(eSpriteType t, const Vector2& p); ///< Constructor.
    virtual ~CObject();

    virtual void move(); ///< Move object.

    // functions for health
    void SetHealth(int h);  // set health
    int GetHealth();    // get health
    void hit(); // player is hit
    void enemyHit();    // enemy is hit

    void kill(); ///< Kill me.
    bool IsDead(); ///< Query whether dead.
    virtual void DeathFX(); ///< Death special effects.
    void HitFX(); ///< Hit by bullet
    void CollisionResponse(); ///< Collision response.
    
    void SetSpeed(float speed); ///< Set speed.
    float GetSpeed(); ///< Set speed.
    void SetRotSpeed(float speed); ///< Set rotational velocity.

    Vector2 GetViewVector(); //Get view vector.

    float GetOrientation(); ///< Get orientation.
    void SetOrientation(float angle); ///< Set orientation.
    
    const Vector2& GetVelocity(); ///< Get orientation.
    void SetVelocity(const Vector2& v); ///< Set orientation.
    
    void StrafeLeft(); ///< Strafe left.
    void StrafeRight(); ///< Strafe right.
    void StrafeBack(); ///< Strafe back.
    void StrafeForward(); ///< Strafe Forward
    void Charge(); ///< Charging
    
    const BoundingSphere& GetBoundingSphere(); ///< Get bounding sphere.
    const Vector2& GetPos(); ///< Get position.

    void ChangeColor();
    virtual CObject* FireGun(); //Object will shoot its own gun
    virtual CObject* Attack1( const Vector2& v ); //signature move of object
    virtual void React(const Vector2& v); //React to enemy bullet
    void ActivateForceField(CObject* ff); //Object keeps copy of forcefield
    bool explosionTooOld(); // returns if explosion animation's lifespan is over
    void UpdatePos(); //Updates the position of the CObject
    void heal(); //Heals the Player
}; //CObject



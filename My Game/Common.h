/// \file Common.h
/// \brief Interface for the class CCommon.

#pragma once

#include "SndList.h"

//forward declarations to make the compiler less stroppy

class CObjectManager; 
class CRenderer;
class CParticleEngine2D;
class CObject;

/// \brief The common variables class.
///
/// CCommon is a singleton class that encapsulates 
/// things that are common to different game components,
/// including game state variables.
/// Making it a singleton class means
/// that we can avoid passing its member variables
/// around as parameters, which makes the code
/// minisculely faster, and more importantly, reduces
/// function clutter.

class CCommon{
  protected:  
    static CRenderer* m_pRenderer; ///< Pointer to the renderer.
    static CObjectManager* m_pObjectManager; ///< Pointer to the object manager.
    static CParticleEngine2D* m_pParticleEngine; ///< Pointer to particle engine.    

    static Vector2 m_vWorldSize; ///< World height and width.
    static CObject* m_pPlayer; ///< Pointer to player character.
}; //CCommon

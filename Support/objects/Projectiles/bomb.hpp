//==============================================================================
//
//  Bomb.hpp
//
//==============================================================================

#ifndef BOMB_HPP
#define BOMB_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#include "Arcing.hpp"
#include "Particles/ParticleEffect.hpp"

//==============================================================================
//  TYPES
//==============================================================================

class bomb : public arcing
{

//------------------------------------------------------------------------------
//  Public Functions

public:

        void            Render          ( void );
        
        void            OnAcceptInit    ( const bitstream& BitStream );

        void            Initialize      ( const vector3&   Position,
                                                object::id OriginID,
                                                object::id ExcludeID,
                                                u32        TeamBits );

//------------------------------------------------------------------------------
//  Private Functions

protected:

        xbool           Impact          ( const collider::collision& Collision ) ;
        
        update_code     AdvanceLogic    ( f32 DeltaTime );
        void            Setup           ( void );

//------------------------------------------------------------------------------
//  Private Data

protected:
    
        f32             m_MaxAge;
        xbool           m_HasExploded;
};

//==============================================================================
//  FUNCTIONS
//==============================================================================

object*     BombCreator( void );

//==============================================================================
#endif // BOMB_HPP
//==============================================================================

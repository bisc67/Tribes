//========================================================================================================================================================
//
//  Campaign 08 Logic
//
//========================================================================================================================================================

#ifndef C08_LOGIC_HPP
#define C08_LOGIC_HPP

//========================================================================================================================================================
//  INCLUDES
//========================================================================================================================================================

#include "Campaign_Logic.hpp"

//========================================================================================================================================================
//  DEFINES
//========================================================================================================================================================

//========================================================================================================================================================
//  TYPES
//========================================================================================================================================================

class c08_logic : public campaign_logic
{

//--------------------------------------------------------------------------------------------------------------------------------------------------------
//  Public Functions
//--------------------------------------------------------------------------------------------------------------------------------------------------------

public:

virtual void    BeginGame       ( void );
virtual void    EndGame         ( void );
virtual void    AdvanceTime     ( f32 DeltaTime );
virtual void    RegisterItem    ( object::id ItemID, const char* pTag );
virtual void    ItemDestroyed   ( object::id ItemID, object::id OriginID );

//--------------------------------------------------------------------------------------------------------------------------------------------------------
//  Private Types
//--------------------------------------------------------------------------------------------------------------------------------------------------------

protected:

    enum game_status
    {
        STATE_IDLE,
        STATE_TRANSIT,
        STATE_BOMBARDMENT,
    };
    
//--------------------------------------------------------------------------------------------------------------------------------------------------------
//  Private Functions
//--------------------------------------------------------------------------------------------------------------------------------------------------------

protected:

        void        Transit         ( void );
        void        Bombardment     ( void );

//--------------------------------------------------------------------------------------------------------------------------------------------------------
//  Private Data
//--------------------------------------------------------------------------------------------------------------------------------------------------------

protected:

    //
    // Objects
    //

    object::id      m_InventoryStation;
    object::id      m_Destination;
    object::id      m_EnemyBase;
    object::id      m_Switch;
    object::id      m_Turrets[5];
    
    //
    // Flags
    //

    xbool           m_bMoveOut;
    xbool           m_bTransitWarning1;
    xbool           m_bTransitWarning2;
    xbool           m_bTurret;
    xbool           m_bDeathmatch;
    xbool           m_bHitSwitch;
    xbool           m_bGetEnemies;
    xbool           m_bGetSwitchBack;
    xbool           m_bTurretsFriendly;
    xbool           m_bTurretsDestroyed[5];

    //
    // Variables
    //
    
    s32             m_nTurretsDestroyed;
    f32             m_TargetTimer;
};

//========================================================================================================================================================
//  GLOBAL VARIABLES
//========================================================================================================================================================

extern c08_logic C08_Logic;

//========================================================================================================================================================
#endif
//========================================================================================================================================================


//========================================================================================================================================================
//
//  Campaign 06 Logic
//
//========================================================================================================================================================

#ifndef C06_LOGIC_HPP
#define C06_LOGIC_HPP

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

class c06_logic : public campaign_logic
{

//--------------------------------------------------------------------------------------------------------------------------------------------------------
//  Public Functions
//--------------------------------------------------------------------------------------------------------------------------------------------------------

public:

virtual void    BeginGame       ( void );
virtual void    EndGame         ( void );
virtual void    AdvanceTime     ( f32 DeltaTime );
virtual void    RegisterItem    ( object::id ItemID, const char* pTag );
virtual void    ItemRepaired    ( object::id ItemID, object::id OriginID, xbool Score );
virtual void    ItemRepairing   ( object::id ItemID, object::id OriginID );
virtual void    ItemDeployed    ( object::id ItemID, object::id OriginID );

//--------------------------------------------------------------------------------------------------------------------------------------------------------
//  Private Types
//--------------------------------------------------------------------------------------------------------------------------------------------------------

protected:

    enum game_status
    {
        STATE_IDLE,
        STATE_GET_READY,
        STATE_TRAVEL,
        STATE_SIEGE,
    };
    
//--------------------------------------------------------------------------------------------------------------------------------------------------------
//  Private Functions
//--------------------------------------------------------------------------------------------------------------------------------------------------------

protected:
        
        void        GetReady        ( void );
        void        Travel          ( void );
        void        Siege           ( void );

//--------------------------------------------------------------------------------------------------------------------------------------------------------
//  Private Data
//--------------------------------------------------------------------------------------------------------------------------------------------------------

protected:

    //
    // Objects
    //
    
    object::id      m_InventoryStation2;
    object::id      m_InventoryStation3;
    object::id      m_InventoryStation4;
    object::id      m_VehicleStation;
    object::id      m_DestinationBase;
    object::id      m_Generator;
    object::id      m_Turret1;
    object::id      m_Turret2;

    //
    // Flags
    //
    
    xbool           m_bInventoryStation1;
    xbool           m_bVehicleStation;
    xbool           m_bGetReadyWarning;
    xbool           m_bTravelWarning;
    xbool           m_bSiegeWave1;
    xbool           m_bSiegeWave2;
    xbool           m_bSiegeWave3;
    xbool           m_bTurret1Repaired;
    xbool           m_bTurret2Repaired;
    xbool           m_bTurretsRepaired;
    xbool           m_bGeneratorHelp;
    xbool           m_bTurretHelp;
    xbool           m_bBarrelsDeployed;
    
    //
    // Variables
    //
    
    f32             m_Time2;
    s32             m_nBarrelsDeployed;
};

//========================================================================================================================================================
//  GLOBAL VARIABLES
//========================================================================================================================================================

extern c06_logic C06_Logic;

//========================================================================================================================================================
#endif
//========================================================================================================================================================


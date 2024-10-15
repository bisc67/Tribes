//==============================================================================
//
//  GameMgr.cpp
//
//==============================================================================

#define NAME(i)     (const char*)xstring( m_Score.Player[i].Name )

//==============================================================================
//  INCLUDES
//==============================================================================

#include "GameMgr.hpp"
#include "CTF_Logic.hpp"
#include "CNH_Logic.hpp"
#include "TDM_Logic.hpp"
#include "DM_Logic.hpp"
#include "HUNT_Logic.hpp"
#include "Campaign_Logic.hpp"
#include "C01_Logic.hpp"
#include "C02_Logic.hpp"
#include "C03_Logic.hpp"
#include "C04_Logic.hpp"
#include "C05_Logic.hpp"
#include "Demo1\Globals.hpp"
#include "Demo1\fe_globals.hpp"
#include "Demo1\GameServer.hpp"
#include "Objects\Bot\BotObject.hpp"
#include "Objects\Player\PlayerObject.hpp"
#include "Objects\Player\DefaultLoadouts.hpp"
#include "Objects\Projectiles\Flag.hpp"
#include "Objects\Projectiles\Sensor.hpp"
#include "Objects\Projectiles\Beacon.hpp"
#include "Objects\Projectiles\FlipFlop.hpp"
#include "Objects\Projectiles\Bounds.hpp"
#include "Objects\Vehicles\Vehicle.hpp"
#include "LabelSets/Tribes2Types.hpp"
#include "Demo1\sm_common.hpp"
#include "Demo1\Data\UI\ui_Strings.h"
#include "Demo1\Data\UI\Messages.h"
#include "StringMgr/StringMgr.hpp"
#include "MsgMgr.hpp"

//==============================================================================
//  STORAGE
//==============================================================================

game_mgr    GameMgr;
game_logic* pGameLogic              = NULL;
xbool       game_mgr::m_Initialized = FALSE;
extern      bot_object::bot_debug g_BotDebug;

//==============================================================================

const char* BotNames[] = 
{
//  "1234567890123",    "1234567890123",    "1234567890123",    "1234567890123",
    "Stress",           "Figment",          "Deliverance",      "Toe Nail",
    "Spyder",           "Seker",            "Blind Luck",       "Lazy Bones",
    "Zero Hour",        "Matrix",           "Jibba",            "Corsair",
    "Teknofreek",       "Duelie",           "Biscuit",          "Bone Grinder",
    "FreakShow",        "Leviathan",        "Gopher",           "The Truck",
    "Flashback",        "Odd Man Out",      "Dark Hawk",        "Sound Geek",
    "Widowmaker",       "Dude Fortune",     "Final Word",       "Arsenic",
    "Rough Stuff",      "Japes",            "Loopy",            "Lexion",
    "Gwydion",          "Hamish",           "Epincion",         "8-Ball",
    "Thresher",         "Trigger Happy",    "Loudmouth",        "DigitalWraith",
    "Ricochet",         "Rock Dragon",      "Wild Fyre",        "Tough Cat",
    "Ekru",             "Revenant",         "Stumble",          "ByTor",
    "Inevitable",       "Bad Candy",        "Dying Light",      "Bommer",
    "Horse Fly",        "Hathor",           "Pepe",             "Renegade",
    "Extra Crispy",     
};

const char* RecruitNames[] = 
{
//  "1234567890123",    "1234567890123",    "1234567890123",    "1234567890123",
    "S.Ansari",         "J.Stubbington",    "C.Galley",         "B.Salinas",
    "A.Cheng",          "R.Brannon",        "M.Traub",          "D.Ancira",
    "M.Fong",           "C.Lupher",         "D.Michael",        "L.Talamo",         
    "P.Haskins",        "T.Arce",           "G.Omelchuck",      "L.Gustafson",      
    "J.Cossigny",       "M.L'Heureux",      "R.Brown",          "S.Hird",           
    "C.Lum",            "R.Higgs",          "J.Versluis",       "M.Warchola",       
    "J.Nagle",          "L.DeBruyn",        "B.Spears",         "C.Spears",         
    "J.Slate",          "A.Wong",           "B.Bickerton",      "T.Heimann",        
    "B.Marques",        "J.Watson",         "M.Coven",          "M.Reed",
    "R.Byrd",           "J.Richardson",     "D.Michael",        "P.Franco",         
    "C.Yarbrough",      "D.Russell",        "A.Rogers",         "S.Broumley",
    "M.Schaefgen",      "J.Franklin",       "P.Masters",        "A.Thyssen",        
    "M.Traub",          "B.Watson",         "C.Cavitt",         "T.Raffray",
    "M.Weisbrod",       "R.Shelley",        "J.Grieves",        "S.Tarrant",        
    "M.McClelland",     "J.Howa",           "N.Macron",         "M.Kupka",
//  "1234567890123",
};

const char* DermNames[] = 
{
//  "1234567890123",    "1234567890123",    "1234567890123",    "1234567890123",
    "Lialith",          "Shralan",          "Rhendaloth",       "Solosk",
    "Elikharn",         "Nasholod",         "Seronser",         "Ushessil",
    "Praloth",          "Qkarrin",          "Nalekk",           "Amarsean",
    "Greqrel",          "Grukorth",         "Ashinmir",         "Thidach",
    "Sarashor",         "Tamoth",           "Rashnak",          "Braonnor",
    "Ahmones",          "Shimosh",          "Lareiard",         "Tumlahr",
    "Eksor",            "Thuqal",           "Gleorrant",        "Grelibar",
    "Siralg",           "Ethohadric",       "Vorhain",          "Abirack",
    "Leigorid",         "Mavauk",           "Privagar",         "Blidohk",
    "Simashnar",        "Chralas",          "Rheonyc",          "Muqhos",
    "Jurklav",          "Sharynosk",        "Protlan",          "Shorrondor",
    "Lohawyr",          "Simoresh",         "Shorari",          "Doxoth",
    "Dapoch",           "Varagh",           "Gourkon",          "Baurk",
    "Eralgoj",          "Esyrsham",         "Bareldan",         "Gequor",
    "Gyoth",            "Taiphak",          "Sovark",           "Mahkoi",
    "Pulogor",          "Sinoshil",         "Doshaxik",         "Atugon",
    "Tavyth",           "Thukhalan",        "Makdohk",          "Cerenst",
    "Astigohn",         "Larekhiv",         "Carag",            "Thosvan",
    "Naihath",          "Tirhent",          "Abyrheth",         "Uledrisarn",
    "Eralrurg",         "Calewin",          "Shihoch",          "Gahkan",
    "Shekynsh",         "Sharakhir",        "Shaidox",          "Talezar",
    "Gwedrius",         "Retheoc",          "Gleraad",          "Gadahr",
    "Talokon",          "Tharaq",           "Chanisen",         "Gruhnak",
    "Rhaelorth",        "Tughos",           "Thuvak",           "Gaigokin",
};

//==============================================================================

static vector3 s_VolumePts[16];
static s32     s_nVerts      = 0;
xbool          g_ShowVolumes = FALSE;

//==============================================================================

const f32 TurretSize1  = 0.4f;
const f32 TurretSize2  = 0.7f;
const f32 TurretHeight = 1.5f;

static vector3 s_TurretVolume[8]=
{
    vector3( -TurretSize1,         0.1f,  TurretSize1 ),
    vector3(  TurretSize1,         0.1f,  TurretSize1 ),
    vector3(  TurretSize1,         0.1f, -TurretSize1 ),
    vector3( -TurretSize1,         0.1f, -TurretSize1 ),
    
    vector3( -TurretSize2, TurretHeight,  TurretSize2 ),
    vector3(  TurretSize2, TurretHeight,  TurretSize2 ),
    vector3(  TurretSize2, TurretHeight, -TurretSize2 ),
    vector3( -TurretSize2, TurretHeight, -TurretSize2 ),
};

//==============================================================================

const f32 SensorSize1  = 0.5f;
const f32 SensorHeight = 0.9f;

static vector3 s_SensorVolume[8]=
{
    vector3( -SensorSize1,         0.1f,  SensorSize1 + 0.2f ),
    vector3(  SensorSize1,         0.1f,  SensorSize1 + 0.2f ),
    vector3(  SensorSize1,         0.1f, -SensorSize1 + 0.2f ),
    vector3( -SensorSize1,         0.1f, -SensorSize1 + 0.2f ),
    
    vector3( -SensorSize1, SensorHeight,  SensorSize1 + 0.2f ),
    vector3(  SensorSize1, SensorHeight,  SensorSize1 + 0.2f ),
    vector3(  SensorSize1, SensorHeight, -SensorSize1 + 0.2f ),
    vector3( -SensorSize1, SensorHeight, -SensorSize1 + 0.2f ),
};

//==============================================================================

const f32 InvenSize1  = 0.9f;
const f32 InvenSize2  = 0.5f;
const f32 InvenHeight = 1.9f;

static vector3 s_InvenVolume[7]=
{
    vector3(        0.0f,        0.1f,        0.0f ),
    vector3( -InvenSize1,        0.5f,        0.0f ),
    vector3(  InvenSize1,        0.5f,        0.0f ),
    vector3( -InvenSize1, InvenHeight,        0.0f ),
    vector3(  InvenSize1, InvenHeight,        0.0f ),
    vector3(        0.0f,        1.5f,       -0.3f ),
    vector3(        0.0f, InvenHeight,  InvenSize2 ),
};

//==============================================================================
//  GAME MANAGER MEMBER FUNCTIONS
//==============================================================================

game_mgr::game_mgr( void )
{
    s32 i;

    ASSERT( !m_Initialized );
    m_Initialized = TRUE;

    m_DebugRender = FALSE;

    m_NPlayers = 0;
    m_NHumans  = 0;
    m_NBots    = 0;

    for( i = 0; i < 32; i++ )
    {
        m_Score.Player[i].IsConnected = FALSE;
        m_Score.Player[i].Name[0]     = '\0';
    }

    m_Score.IsGameOver  = FALSE;    // No previous game to be "over".
    m_Score.IsTeamBased = FALSE;    // Players are NOT on 2 teams.
    for( i = 0; i < 2; i++ )
    {
        m_Score.Team[i].Name[ 0] = '\0';
        m_Score.Team[i].Name[15] = '\0';
        m_Score.Team[i].Size     = 0;
        m_Score.Team[i].Humans   = 0;
        m_Score.Team[i].Bots     = 0;
    }

    // Clear all dirty bits.
    m_DirtyBits  = 0x00;
    m_PlayerBits = 0x00;

    // Default values.
    m_GameType         = GAME_NONE;
    m_MaxPlayers       = 32;
    m_MinPlayers       =  8;
    m_TeamDamageFactor =  0.50f;
    m_TimeRemaining    =  5.00f;
    m_KickPlayer       = -1;        // No kick player vote in progress.
    m_ChangeMap        = -1;        // No change map  vote in progress.
    m_TimeLimit        =  0;        // Unlimited time.
    m_MaxScore         =  0;        // Unlimited score.
    m_DefaultLoadOut   =  0;        // Dunno.
    m_VehicleLimit[0]  =  4;        // GravCycle
    m_VehicleLimit[1]  = -1;        // Tank
    m_VehicleLimit[2]  = -1;        // MPB
    m_VehicleLimit[3]  =  4;        // Shrike
    m_VehicleLimit[4]  =  2;        // Bomber
    m_VehicleLimit[5]  =  2;        // Transport
    m_Bounds( vector3(0,-10000,0), vector3(1024,10000,1024) );
}

//==============================================================================

game_mgr::~game_mgr( void )
{
    ASSERT( m_Initialized );
    m_Initialized = FALSE;
}

//==============================================================================

void game_mgr::SetGameType( game_type GameType, s32 CampaignMission )
{
    // Set the default loadout for the vehicles.
    SetVehicleLimits( 4, 4, 2, 2 );

    x_wstrcpy( m_Score.Team[0].Name, StringMgr( "ui", IDS_STORM   ) );
    x_wstrcpy( m_Score.Team[1].Name, StringMgr( "ui", IDS_INFERNO ) );

    ASSERT( m_Initialized );

    m_MaxScore        =  0;        // Unlimited score.

    m_Campaign        = FALSE;
    m_GameType        = GameType;
    m_CampaignMission = -1;

    // Use 'GameType' (not 'm_GameType') for this switch because 'm_GameType'
    // may be altered within the switch statement.

    switch( GameType )
    {
    case GAME_CTF:
        pGameLogic = &CTF_Logic;
        break;

    case GAME_CNH:
        pGameLogic = &CNH_Logic;
        break;

    case GAME_TDM:
        pGameLogic = &TDM_Logic;
        break;

    case GAME_DM:
        pGameLogic = &DM_Logic;
        break;

    case GAME_HUNTER:
        pGameLogic = &HUNT_Logic;
        break;

    case GAME_CAMPAIGN:
        m_CampaignMission  = CampaignMission;
        m_Campaign         = TRUE;
        m_TeamDamageFactor = 0.75f;
        
        if( (CampaignMission < 6) && (m_NCampaignMissions) )
        {
            switch( CampaignMission )
            {                                       
            case 1:     pGameLogic = &C01_Logic;  m_GameType = GAME_DM;     break;
            case 2:     pGameLogic = &C02_Logic;  m_GameType = GAME_CTF;    break;
            case 3:     pGameLogic = &C03_Logic;  m_GameType = GAME_HUNTER; break;
            case 4:     pGameLogic = &C04_Logic;  m_GameType = GAME_CNH;    break;
            case 5:     pGameLogic = &C05_Logic;  m_GameType = GAME_CTF;    break;
            default:    ASSERT( FALSE );
            }
        }
        else
        {
            m_BotsEnabled = FALSE;
            m_MaxPlayers  = 32;
            m_MinPlayers  =  0;
            
            if( CampaignMission > m_NCampaignMissions )
                pGameLogic = campaign_logic::GetTrainingLogic( CampaignMission - m_NCampaignMissions );
            else
                pGameLogic = campaign_logic::GetCampaignLogic( CampaignMission );
        }
        break;

    default:
        ASSERT( FALSE );
        break;
    }

    // Use 'm_GameType' for the next switch (not 'GameType').

    // Team based?
    switch( m_GameType )
    {
        case GAME_CTF:
        case GAME_CNH:
        case GAME_TDM:
        case GAME_CAMPAIGN:                 // Note: only applies to campaign missions 6-13
            m_Score.IsTeamBased = TRUE;
            break;

        case GAME_DM:
        case GAME_HUNTER:
            m_Score.IsTeamBased = FALSE;
            break;
    }

    // By default, the bounds are not fatal.  This may be overridden by a 
    // particular game logic (namely DM and TDM).
    bounds_SetFatal( FALSE );

    // Start 'er up.
    m_Score.GameType = m_GameType;
    pGameLogic->Initialize();

    if( !tgl.ServerPresent )
        return;

    // Set dirty bits.
    m_DirtyBits  = DIRTY_ALL;
    m_PlayerBits = 0xFFFFFFFF;
}

//==============================================================================

game_type game_mgr::GetGameType( void )
{
    return( m_GameType );
}

//==============================================================================

s32 game_mgr::GetCampaignMission( void )
{
    return( m_CampaignMission );
}

//==============================================================================

void game_mgr::SetNCampaignMissions( s32 NMissions )
{
    m_NCampaignMissions = NMissions;
}

//==============================================================================

s32 game_mgr::GetNCampaignMissions( void )
{
    return( m_NCampaignMissions );
}

//==============================================================================

xbool game_mgr::IsCampaignMission( void )
{
    return( m_Campaign );
}

//==============================================================================

xbool game_mgr::IsTeamBasedGame( void )
{
    return( m_Score.IsTeamBased );
}

//==============================================================================

void game_mgr::SetPlayerLimits( s32 MaxPlayers, s32 MinPlayers, xbool BotsEnabled )
{
    m_MaxPlayers  = MaxPlayers; 
    m_MinPlayers  = BotsEnabled ? MinPlayers : 0; 
    m_BotsEnabled = TRUE;
}

//==============================================================================

void game_mgr::SetVehicleLimits( s32 MaxCycles,  s32 MaxShrikes, 
                                 s32 MaxBombers, s32 MaxTransports )
{
    m_VehicleLimit[0] = MaxCycles;
    m_VehicleLimit[3] = MaxShrikes;
    m_VehicleLimit[4] = MaxBombers;
    m_VehicleLimit[5] = MaxTransports;
}

//==============================================================================

void game_mgr::BeginGame( void )
{
    s32  i;

    // We are now LIVE.
    m_Score.IsGameOver = FALSE;
    m_GameInProgress   = TRUE;
    tgl.GameRunning    = TRUE;

    MsgMgr.Reset();

    if( !tgl.ServerPresent )
        return;

    // Time is money!
    m_TimeRemaining = m_TimeLimit * 60.0f;

    // Randomize the bot name base.
    m_BotNameBase   = x_rand();
    m_BioDermOffset = 0;

    // Set dirty bits.
    m_DirtyBits  = DIRTY_ALL;
    m_PlayerBits = 0xFFFFFFFF;
    m_ScoreBits  = 0xFFFFFFFF;

    // Clear all player scores.
    for( i = 0; i < 32; i++ )
    {
        m_Score.Player[i].Score  = 0;
        m_Score.Player[i].Kills  = 0;
        m_Score.Player[i].Deaths = 0;
    }

    // Clear all team scores.
    for( i = 0; i < 2; i++ )
    {
        m_Score.Team[i].Score = 0;
    }

    // Create all the players.
    for( i = 0; i < 32; i++ )
    {
        if( m_Score.Player[i].IsInGame )
            CreatePlayer( i );
    }

    // Prepare the voting system.
    m_VoteInProgress = FALSE;
    m_KickPlayer     = -1;
    m_ChangeMap      = -1;

    // Flags for countdown sounds.
    m_Said60 = FALSE;
    m_Said30 = FALSE;
    m_Said10 = FALSE;

    // Clear any ingame campaign objective strings
    campaign_logic::SetCampaignObjectives( -1 );

    // Let the specific game logic do its thing.
    ASSERT( pGameLogic ); 
    pGameLogic->BeginGame();
}

//==============================================================================

void game_mgr::EndGame( void )
{
    s32 i;

    // Halt the game from the GameMgr's point of view.
    m_Score.IsGameOver = TRUE;
    m_GameInProgress   = FALSE;

    // And flag the end of game for the outside world.
    tgl.GameRunning     = FALSE;
    g_SM.MissionRunning = FALSE;

    if( m_Campaign )    g_SM.CampaignMissionEnded = TRUE;
    else                g_SM.ServerMissionEnded   = TRUE;

    // Quiet!
    audio_StopAll();

    // Preserve the final score.
    m_FinalScore = m_Score;

    // Different logic now for clients and servers.
    if( tgl.ServerPresent )
    {
        // The polls (if any) are closed.
        ShutDownVote();

        // Nuke all bots.
        for( i = 0; i < 32; i++ )
        {
            if( m_Options[i].Bot )
            {
                if( m_Score.Player[i].IsInGame )
                    ExitGame( i );

                if( m_Score.Player[i].IsConnected )
                    Disconnect( i );
            }
        }

        // Let the specific game logic do its thing.
        ASSERT( pGameLogic );
        pGameLogic->EndGame();   
    }
    else
    {
        // CLIENT CODE
        for( i = 0; i < 32; i++ )
        {
            m_Score.Player[i].IsConnected = FALSE;
            m_Score.Player[i].IsInGame    = FALSE;
            m_Score.Player[i].IsBot       = FALSE;
            m_Score.Player[i].Name[0]     = '\0';
            m_Score.Player[i].Score       = 0;
            m_Score.Player[i].Team        = 0;
        }

        m_Score.Team[0].Size   = 0;
        m_Score.Team[0].Bots   = 0;
        m_Score.Team[0].Humans = 0;
        m_Score.Team[0].Score  = 0;
        
        m_Score.Team[1].Size   = 0;
        m_Score.Team[1].Bots   = 0;
        m_Score.Team[1].Humans = 0;
        m_Score.Team[1].Score  = 0;
    }
/*
    // Announce the results.
    if( !m_Campaign )
    {
        if( m_Score.IsTeamBased )
        {
            if( m_FinalScore.Team[0].Score > m_FinalScore.Team[1].Score )
                SendAudio( SFX_VOICE_ANN_TEAM_STORM_WINS );

            if( m_FinalScore.Team[0].Score < m_FinalScore.Team[1].Score )
                SendAudio( SFX_VOICE_ANN_TEAM_INFERNO_WINS );

            if( m_FinalScore.Team[0].Score == m_FinalScore.Team[1].Score )
                SendAudio( SFX_VOICE_ANN_GAMEOVER );
        }
        else
        {
            SendAudio( SFX_VOICE_ANN_GAMEOVER );
        }
    }
*/
}

//==============================================================================

void game_mgr::SetTeamDamage( f32 TeamDamage )
{
    m_TeamDamageFactor = TeamDamage;
}

//==============================================================================

f32 game_mgr::GetTeamDamage( void )
{
    return( m_TeamDamageFactor );
}

//==============================================================================

u32 game_mgr::GetTeamBits( s32 PlayerIndex )
{
    ASSERT( m_Initialized );
    ASSERT( PlayerIndex >=  0 );
    ASSERT( PlayerIndex <  32 );

    if( m_Score.Player[PlayerIndex].IsConnected && 
        m_Score.Player[PlayerIndex].IsInGame )
    {
        return( 1 << m_Score.Player[PlayerIndex].Team );
    }
    return( 0x00 );
}

//==============================================================================

void game_mgr::ChangeTeam( s32 PlayerIndex, xbool ByAdmin )
{
    s32 OldTeam;
    s32 NewTeam;

    // Only allowed on the server.
    if( !tgl.ServerPresent )
        return;

    // Only allowed in team based games.
    if( !m_Score.IsTeamBased )
        return;

    // Only allowed on connected players.
    if( !m_Score.Player[PlayerIndex].IsConnected )
        return;

    // Only allowed on player which are participating.
    if( !m_Score.Player[PlayerIndex].IsInGame )
        return;

    // Only allowed on humans (not bots).
    if( m_Score.Player[PlayerIndex].IsBot )
        return;

    // Gather some basic information.
    OldTeam = m_Score.Player[PlayerIndex].Team;
    NewTeam = 1 - OldTeam;

    // Only allowed if the new team will not be too big.
    if( m_Score.Team[NewTeam].Humans >= 16 )
        return;

    // Look's like we're clear.  Go ahead and DO IT!

    // Let each game type have a crack at it.
    pGameLogic->ChangeTeam( PlayerIndex );

    // Disconnect deployed turrets and mines from this player.
    ObjMgr.UnbindOriginID( m_PlayerID[PlayerIndex] );

    // Update the score structure.
    m_Score.Team[OldTeam].Size--;
    m_Score.Team[OldTeam].Humans--; 
    m_Score.Team[NewTeam].Size++;
    m_Score.Team[NewTeam].Humans++; 
    m_Score.Player[PlayerIndex].Team = NewTeam;

    // Set dirty bits.
    m_PlayerBits |= (1 << PlayerIndex);
    m_DirtyBits  |= DIRTY_SPECIFIC_DATA;

    // Go ahead and inform the player.
    player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromSlot( PlayerIndex );
    ASSERT( pPlayer );
    pPlayer->ChangeTeam( 1 << NewTeam );

    // Message.
    if( ByAdmin )
        MsgMgr.Message( MSG_ADMIN_CHANGED_TEAM,   PlayerIndex, ARG_WARRIOR, PlayerIndex, ARG_TEAM, NewTeam );
    else
        MsgMgr.Message( MSG_PLAYER_SWITCHED_TEAM, PlayerIndex, ARG_WARRIOR, PlayerIndex, ARG_TEAM, NewTeam );

    UpdateBotCount();
}

//==============================================================================

void game_mgr::BuildVoteMessage( void )
{
    m_VoteMessage[0] = '\0';
    if( m_VoteInProgress )
    {
        if( m_KickPlayer != -1 )
        {
            xwstring Message( "Vote to kick player " );
            xwstring Period ( "." );
            x_wstrcpy( m_VoteMessage, (const xwchar*)Message );
            x_wstrcat( m_VoteMessage, m_Score.Player[m_KickPlayer].Name );
            x_wstrcat( m_VoteMessage, (const xwchar*)Period );
        }
        if( m_ChangeMap != -1 )
        {
            xwstring Message( "Vote to change map to " );
            xwstring Period ( "." );
            x_wstrcpy( m_VoteMessage, (const xwchar*)Message );
            x_wstrcat( m_VoteMessage, StringMgr( "MissionName", fe_Missions[m_ChangeMap].DisplayNameString ) );
            x_wstrcat( m_VoteMessage, (const xwchar*)Period );
        }
    }
}

//==============================================================================

void game_mgr::ExecuteVote( void )
{
    s32 Kick = m_KickPlayer;    // Make backup copy.
    s32 Map  = m_ChangeMap;     // Make backup copy.

    ShutDownVote();             // This clears m_KickPlayer and m_ChangeMap.

    if( m_VoteHasPassed )
    {
        if( Kick != -1 )    KickPlayer( Kick, FALSE );
        if( Map  != -1 )    ChangeMap ( Map,  FALSE );

        m_VoteHasPassed = FALSE;
    }
}

//==============================================================================

void game_mgr::AnnounceVotePasses( void )
{
    //x_DebugMsg( "GAMEMGR: Vote passes\n" );

    if( m_KickPlayer != -1 )
    {
        s32 i;

        for( i = 0; i < 32; i++ )
        {
            if( (m_Score.Player[i].IsInGame) && (m_VotePlayers & (1<<i)) )
            {
                MsgMgr.Message( MSG_VOTE_KICK_PASSES, i, ARG_WARRIOR, m_KickPlayer );

                //x_DebugMsg( "GAMEMGR: Message to %d(%s) \"vote to kick %d(%s) passes\"\n", 
                //            i, NAME(i), m_KickPlayer, NAME(m_KickPlayer) );
            }
        }
    }

    if( m_ChangeMap != -1 )
    {
        MsgMgr.Message( MSG_VOTE_MAP_PASSES, 0 );

        //x_DebugMsg( "GAMEMGR: Message to all \"map change vote passes\"\n" );
    }

    m_VoteHasPassed = TRUE;
    EndVoting();
}

//==============================================================================

void game_mgr::AnnounceVoteFails( void )
{
    //x_DebugMsg( "GAMEMGR: Vote passes\n" );

    if( m_KickPlayer != -1 )
    {
        s32 i;

        for( i = 0; i < 32; i++ )
        {
            if( (m_Score.Player[i].IsInGame) && (m_VotePlayers & (1<<i)) )
            {
                MsgMgr.Message( MSG_VOTE_KICK_FAILS, i, ARG_WARRIOR, m_KickPlayer );

                //x_DebugMsg( "GAMEMGR: Message to %d(%s) \"vote to kick %d(%s) fails\"\n", 
                //            i, NAME(i), m_KickPlayer, NAME(m_KickPlayer) );
            }
        }
    }

    if( m_ChangeMap != -1 )
    {
        MsgMgr.Message( MSG_VOTE_MAP_FAILS, 0 );

        //x_DebugMsg( "GAMEMGR: Message to all \"map change vote fails\"\n" );
    }

    EndVoting();
}

//==============================================================================

void game_mgr::EndVoting( void )
{
    // Make it so nobody can vote, but the vote is technically still in 
    // progress.  This gives a "sink in" time for the vote results.

    if( m_VoteInProgress )
    {
        s32            i;
        player_object* pPlayer;

        for( i = 0; i < 32; i++ )
        {
            if( (m_Score.Player[i].IsInGame) && (!m_Score.Player[i].IsBot) )
            {
                pPlayer = (player_object*)ObjMgr.GetObjectFromSlot( i );
                if( pPlayer )
                {
                    pPlayer->SetVoteCanCast ( FALSE );
                    pPlayer->SetVoteCanStart( FALSE );
                }
            }
        }

        m_VoteTimer = 5.0f;
    }
}

//==============================================================================

void game_mgr::ShutDownVote( void )
{
    //x_DebugMsg( "GAMEMGR: Vote closed\n" );

    if( m_VoteInProgress )
    {
        s32            i;
        player_object* pPlayer;

        m_DirtyBits     |= DIRTY_GENERIC_VOTE_NEW;
        m_KickPlayer     = -1;
        m_ChangeMap      = -1;
        m_VoteInProgress = FALSE;
        m_VotePlayers    = 0;

        for( i = 0; i < 32; i++ )
        {
            if( (m_Score.Player[i].IsInGame) && (!m_Score.Player[i].IsBot) )
            {
                pPlayer = (player_object*)ObjMgr.GetObjectFromSlot( i );
                if( pPlayer )
                {
                    pPlayer->SetVoteCanCast ( FALSE );
                    pPlayer->SetVoteCanStart( m_TimeRemaining > 60.0f );
                }
            }
        }
    }

    // This will build an empty vote message.
    BuildVoteMessage();
}

//==============================================================================

void game_mgr::InitiateKickVote( s32 PlayerIndex )
{
    s32            i;
    player_object* pPlayer;

    // Cannot kick players on the host machine.
    if( tgl.ServerPresent )
    {
        if( PlayerIndex < fegl.nLocalPlayers )
            return;
    }

    if( (m_GameInProgress) &&                           // Game in progress
        (m_KickPlayer == -1) &&                         // No Kick vote in progress
        (m_ChangeMap  == -1) &&                         // No Map  vote in progress
        (m_TimeRemaining > 60.0f) &&                    // At least 1 minute to go
        (m_Score.Player[PlayerIndex].IsInGame) &&       // Player is actually here
        (m_Score.Player[PlayerIndex].IsBot == FALSE) && // Player is not a bot
        (m_CampaignMission == -1) )                     // Not a campaign mission
    {
        m_VotesFor       =  0;
        m_VotesAgainst   =  0;
        m_VotesMissing   =  0;    
        m_VoteBits       = 0x00000000;

        if( m_Score.IsTeamBased )
        {
            for( i = 0; i < 32; i++ )
            {
                if( (m_Score.Player[i].IsInGame) && (!m_Score.Player[i].IsBot) ) 
                {
                    pPlayer = (player_object*)ObjMgr.GetObjectFromSlot( i );
                    if( pPlayer )
                    {
                        if( (m_Score.Player[i].Team == m_Score.Player[PlayerIndex].Team) &&
                            (i != PlayerIndex) )
                        {
                            m_VotesMissing++;
                            m_VoteBits |= (1<<i);
                        }
                    }
                }
            }
        }
        else
        {
            for( i = 0; i < 32; i++ )
            {
                if( (m_Score.Player[i].IsInGame) && (!m_Score.Player[i].IsBot) ) 
                {
                    pPlayer = (player_object*)ObjMgr.GetObjectFromSlot( i );
                    if( pPlayer )
                    {
                        if( i != PlayerIndex )
                        {
                            m_VotesMissing++;
                            m_VoteBits |= (1<<i);
                        }
                    }
                }
            }
        }

        // No participants?  No vote!
        if( m_VotesMissing == 0 )
            return;

        // We have enough participants.  Start sending messages and settings states!
        for( i = 0; i < 32; i++ )
        {
            pPlayer = (player_object*)ObjMgr.GetObjectFromSlot( i );
            if( pPlayer )
            {
                pPlayer->SetVoteCanStart( FALSE );

                if( m_VoteBits & (1<<i) )
                {
                    pPlayer->SetVoteCanCast( TRUE );

                    // Special case.  Solves message order problem.
                    if( m_VotesMissing > 1 )
                        MsgMgr.Message( MSG_VOTE_KICK, i, ARG_WARRIOR, PlayerIndex );
                }
                else
                {
                    pPlayer->SetVoteCanCast( FALSE );
                }
            }
        }

        // Warning!
        // Special case.  Solves message order problem.
        if( m_VotesMissing > 1 )
            MsgMgr.Message( MSG_VOTE_KICK_WARNING, PlayerIndex );

        m_VotePlayers    = m_VoteBits;
        m_KickPlayer     = PlayerIndex;
        m_VotePercent    = fegl.ServerSettings.VotePass;
        m_DirtyBits     |= DIRTY_GENERIC_VOTE_NEW;
        m_DirtyBits     |= DIRTY_GENERIC_VOTE_UPDATE;
        m_VoteInProgress = TRUE;
        m_VoteHasPassed  = FALSE;
        m_VoteTimer      = 55.0f;

        if( m_KickPlayer != -1 )
            m_VotePlayers |= (1 << m_KickPlayer);   // Kick target can see progress.

        BuildVoteMessage();
    }    
}

//==============================================================================

void game_mgr::InitiateMapVote( s32 MapIndex )
{
    s32            i;
    player_object* pPlayer;

    //x_DebugMsg( "GAMEMGR: Initiate change map to %d vote\n", MapIndex );                 

    if( (m_GameInProgress) &&                           // Game in progress
        (m_KickPlayer == -1) &&                         // No Kick vote in progress
        (m_ChangeMap  == -1) &&                         // No Map  vote in progress
        (m_TimeRemaining > 60.0f) &&                    // At least 1 minute to go
        (m_CampaignMission == -1) )                     // Not a campaign mission
    {
        m_VotesFor     = 0;
        m_VotesAgainst = 0;
        m_VotesMissing = 0;    
        m_VoteBits     = 0x00000000;

        for( i = 0; i < 32; i++ )
        {
            if( (m_Score.Player[i].IsInGame) && (!m_Score.Player[i].IsBot) )
            {
                pPlayer = (player_object*)ObjMgr.GetObjectFromSlot( i );
                if( pPlayer )
                {
                    m_VotesMissing++;
                    m_VoteBits |= (1<<i);
                }
            }
        }

        // No participants?  No vote!
        if( m_VotesMissing == 0 )
            return;

        // We have enough participants.  Start settings states!
        for( i = 0; i < 32; i++ )
        {
            pPlayer = (player_object*)ObjMgr.GetObjectFromSlot( i );
            if( pPlayer )
            {
                pPlayer->SetVoteCanStart( FALSE );

                if( m_VoteBits & (1<<i) )
                {
                    pPlayer->SetVoteCanCast( TRUE );
                }
                else
                {
                    pPlayer->SetVoteCanCast( FALSE );
                }
            }
        }

        m_VotePlayers    = m_VoteBits;
        m_ChangeMap      = MapIndex;
        m_VotePercent    = fegl.ServerSettings.VotePass;
        m_DirtyBits     |= DIRTY_GENERIC_VOTE_NEW;
        m_DirtyBits     |= DIRTY_GENERIC_VOTE_UPDATE;
        m_VoteInProgress = TRUE;
        m_VoteHasPassed  = FALSE;
        m_VoteTimer      = 55.0f;

        BuildVoteMessage();

        // Special case.  Solves message order problem.
        if( m_VotesMissing > 1 )
            MsgMgr.Message( MSG_VOTE_CHANGE_MAP, 0 );
    }
}

//==============================================================================

void game_mgr::CastVote( s32 PlayerIndex, xbool Vote )
{
    ASSERT( tgl.ServerPresent );

    //x_DebugMsg( "GAMEMGR: Vote %s from %d(%s)\n", 
    //            Vote ? "YES" : "NO", PlayerIndex, NAME(PlayerIndex) );

    if( m_VoteInProgress && (m_VoteBits & (1<<PlayerIndex)) )
    {
        m_DirtyBits |= DIRTY_GENERIC_VOTE_UPDATE;

        player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromSlot( PlayerIndex );
        if( pPlayer )
            pPlayer->SetVoteCanCast( FALSE );

        if( Vote )  m_VotesFor++;
        else        m_VotesAgainst++;
        m_VotesMissing--;
        m_VoteBits &= ~(1<<PlayerIndex);

        s32 TotalVotes = m_VotesFor + m_VotesAgainst + m_VotesMissing;

        //x_DebugMsg( "GAMEMGR: Yes=%d(%g%%)  No=%d(%g%%)  Missing=%d(%g%%)\n", 
        //            m_VotesFor,     ((m_VotesFor     * 100) / (f32)TotalVotes),
        //            m_VotesAgainst, ((m_VotesAgainst * 100) / (f32)TotalVotes),
        //            m_VotesMissing, ((m_VotesMissing * 100) / (f32)TotalVotes) );

        // See if we have enough votes to pass.
        if( ((m_VotesFor * 100) / TotalVotes) >= m_VotePercent )
        {
            AnnounceVotePasses();
        }
        else
        // See if we have enough votes to fail.
        if( ((m_VotesAgainst * 100) / TotalVotes) > (100 - m_VotePercent) )
        {
            AnnounceVoteFails();
        }
    }
}

//==============================================================================

xbool game_mgr::IsPlayerInVote( s32 PlayerIndex )
{
    return( m_VoteInProgress && (m_VotePlayers & (1<<PlayerIndex)) );
}

//==============================================================================

xbool game_mgr::GetVoteData( const xwchar*& pMessage, 
                                   s32&     For, 
                                   s32&     Against, 
                                   s32&     Missing, 
                                   s32&     PercentNeeded )
{
    if( m_VoteInProgress )
    {
        pMessage      = m_VoteMessage;
        For           = m_VotesFor;
        Against       = m_VotesAgainst;
        Missing       = m_VotesMissing;
        PercentNeeded = m_VotePercent;
        return( TRUE );
    }
    else
    {
        return( FALSE );
    }
}

//==============================================================================

void game_mgr::KickPlayer( s32 PlayerIndex, xbool ByAdmin )
{
    if( !m_GameInProgress )
        return;

    if( m_Score.Player[PlayerIndex].IsBot )
        return;

    //x_DebugMsg( "GAMEMGR: Kick player %d(%s) by %s\n", 
    //            PlayerIndex, NAME(PlayerIndex),
    //            ByAdmin ? "ADMIN" : "VOTE" );
     
    tgl.pServer->KickPlayer( PlayerIndex );

    // Message: Player kicked by admin.
    if( ByAdmin )
        MsgMgr.Message( MSG_ADMIN_PLAYER_KICKED_BY_ADMIN,
                        0, ARG_INLINE, PlayerIndex );
}

//==============================================================================

void game_mgr::ChangeMap( s32 MapIndex, xbool ByAdmin )
{
    (void)ByAdmin;

    //x_DebugMsg( "GAMEMGR: Change map to %d by %s\n", 
    //            MapIndex, ByAdmin ? "ADMIN" : "VOTE" );

    fegl.pNextMissionDef = &fe_Missions[MapIndex];
    EndGame();
}

//==============================================================================

void game_mgr::SetBounds( const bbox& Bounds )
{
    m_Bounds = Bounds;

    m_DeathBounds = Bounds;
    m_DeathBounds.Min.X -= 5120.0f;
    m_DeathBounds.Min.Y -= 5120.0f;
    m_DeathBounds.Min.Z -= 5120.0f;
    m_DeathBounds.Max.X += 5120.0f;
    m_DeathBounds.Max.Y += 5120.0f;
    m_DeathBounds.Max.Z += 5120.0f;
}

//==============================================================================

xbool game_mgr::GameInProgress( void )
{
    return( m_GameInProgress );
}

//==============================================================================

void game_mgr::GetDirtyBits( u32& DirtyBits, u32& PlayerBits, u32& ScoreBits )
{
    DirtyBits  = m_DirtyBits;
    PlayerBits = m_PlayerBits;
    ScoreBits  = m_ScoreBits;
}

//==============================================================================

void game_mgr::ClearDirtyBits( void )
{
    m_DirtyBits  = 0;
    m_PlayerBits = 0;
    m_ScoreBits  = 0;
}

//==============================================================================

void game_mgr::AcceptUpdate( const bitstream& BitStream, f32 AvgPing )
{
    s32 i;

    ASSERT( tgl.ClientPresent );
    ASSERT( m_Initialized );

    //
    // There is one bit which indicates if there is any data at all.
    //
    if( !BitStream.ReadFlag() )
        return;

//  BitStream.ReadMarker();

    //
    // Try to read the GENERIC_INIT data.
    //
    if( BitStream.ReadFlag() )
    {
        BitStream.ReadRangedS32( (s32&)m_GameType, GAME_CTF, GAME_HUNTER );
        BitStream.ReadWString( m_Score.MissionName );
        BitStream.ReadRangedS32( m_TimeLimit, 0, 60 );
    }

//  BitStream.ReadMarker();

    //
    // Try to read the GENERIC_SWITCH data.
    //
    if( BitStream.ReadFlag() )
    {
        for( i = 0; i < 16; i++ )
            BitStream.ReadTeamBits( m_SwitchBits[i] );
    }

//  BitStream.ReadMarker();

    //
    // Try to read the GENERIC_POWER data.
    //
    if( BitStream.ReadFlag() )
    {
        s32 i;
        for( i = 0; i < 16; i++ )
            m_PowerCount[i] = BitStream.ReadFlag();
    }

//  BitStream.ReadMarker();

    //
    // Try to read the GENERIC_TIME data.
    //
    if( BitStream.ReadFlag() )
    {
        if( BitStream.ReadFlag() )
        {
            BitStream.ReadRangedF32( m_TimeRemaining, 16, 0.0f, 4096.0f );
            m_TimeRemaining -= AvgPing;
        }
        else
        {
            m_TimeRemaining = 0.0f;
        }
    }

//  BitStream.ReadMarker();

    //
    // Try to read the GENERIC_VOTE_NEW data.
    //
    if( BitStream.ReadFlag() )
    {
        m_VoteInProgress = FALSE;
        m_KickPlayer     = -1;
        m_ChangeMap      = -1;
        m_VotePlayers    =  0;
        m_VotePercent    =  0;

        if( BitStream.ReadFlag() )
        {
            // There is a kick player vote in progress.
            BitStream.ReadRangedS32( m_KickPlayer,  0,  31 );
            BitStream.ReadU32      ( m_VotePlayers         );
            BitStream.ReadRangedS32( m_VotePercent, 0, 100 );
            m_ChangeMap      = -1;
            m_VoteInProgress = TRUE;
        }

        if( BitStream.ReadFlag() )
        {
            // There is a change map vote in progress.
            BitStream.ReadRangedS32( m_ChangeMap,   0, 127 );
            BitStream.ReadU32      ( m_VotePlayers         );
            BitStream.ReadRangedS32( m_VotePercent, 0, 100 );
            m_KickPlayer     = -1;
            m_VoteInProgress = TRUE;
        }

        BuildVoteMessage();
    }

//  BitStream.ReadMarker();

    //
    // Try to read the GENERIC_VOTE_UPDATE data.
    //
    if( BitStream.ReadFlag() )
    {
        if( BitStream.ReadFlag() )
        {
            // There is a kick player vote in progress.
            BitStream.ReadRangedS32( m_VotesFor,     0, 32 );
            BitStream.ReadRangedS32( m_VotesAgainst, 0, 32 );
            BitStream.ReadRangedS32( m_VotesMissing, 0, 32 );
            m_ChangeMap = -1;
        }

        if( BitStream.ReadFlag() )
        {
            // There is a change map vote in progress.
            BitStream.ReadRangedS32( m_VotesFor,     0, 32 );
            BitStream.ReadRangedS32( m_VotesAgainst, 0, 32 );
            BitStream.ReadRangedS32( m_VotesMissing, 0, 32 );
            m_KickPlayer = -1;
        }
    }

//  BitStream.ReadMarker();

    //
    // Try to read data for each player which was marked as dirty.
    //
    if( BitStream.ReadFlag() )
    {
        for( i = 0; i < 32; i++ )
        {
            if( BitStream.ReadFlag() )
            {
                if( (m_Score.Player[i].IsConnected = BitStream.ReadFlag()) )
                {
                    BitStream.ReadWString  ( m_Score.Player[i].Name );
                    BitStream.ReadRangedS32( m_Score.Player[i].Machine, 0, 31 );

                    if( (m_Score.Player[i].IsInGame = BitStream.ReadFlag()) )
                    {
                        BitStream.ReadObjectID ( m_PlayerID[i] );
                        BitStream.ReadRangedS32( m_Score.Player[i].Team, 0, 31 );
                        m_Score.Player[i].IsBot  = BitStream.ReadFlag();
                        m_Score.Player[i].IsMale = BitStream.ReadFlag();
                    }
                }
                else
                {
                    m_Score.Player[i].IsInGame = FALSE;
                }
            }
        }
    }

//  BitStream.ReadMarker();

    //
    // Try to read scores for each player which had dirty scores.
    //
    if( BitStream.ReadFlag() )
    {
        for( i = 0; i < 32; i++ )
        {
            s32 Score;
            s32 Kills;
            s32 Deaths;

            if( BitStream.ReadFlag() )
            {
                if     ( BitStream.ReadFlag() )  BitStream.ReadRangedS32( Score, -255,  255 );
                else if( BitStream.ReadFlag() )  BitStream.ReadRangedS32( Score,  256, 5000 );
                else                             BitStream.ReadS32      ( Score );
                                                                                         
                if     ( BitStream.ReadFlag() )  BitStream.ReadRangedS32( Kills,    0,   63 );
                else if( BitStream.ReadFlag() )  BitStream.ReadRangedS32( Kills,   64, 1000 );
                else                             BitStream.ReadS32      ( Kills );
                                                                                         
                if     ( BitStream.ReadFlag() )  BitStream.ReadRangedS32( Deaths,   0,   63 );
                else if( BitStream.ReadFlag() )  BitStream.ReadRangedS32( Deaths,  64, 1000 );
                else                             BitStream.ReadS32      ( Deaths );

                m_Score.Player[i].Score  = Score; 
                m_Score.Player[i].Kills  = Kills; 
                m_Score.Player[i].Deaths = Deaths;
            }
        }
    }

//  BitStream.ReadMarker();

    //
    // Let the specific game logic read its data.
    //
    ASSERT( pGameLogic );
    pGameLogic->AcceptUpdate( BitStream );

//  BitStream.ReadMarker();
}

//==============================================================================

void game_mgr::ProvideUpdate( bitstream& BS, u32& DirtyBits, u32& PlayerBits, u32& ScoreBits )
{
    s32 i;

    ASSERT( tgl.ServerPresent );
    ASSERT( m_Initialized );

    // Send a single bit indicating the presence of ANY data.
    if( !BS.WriteFlag( DirtyBits | PlayerBits | ScoreBits ) )
        return;

    // See if all bits are set.  This indicates the "first time" update 
    // situation, and all possible data should be sent.
    if( (DirtyBits & PlayerBits & ScoreBits) == 0xFFFFFFFF )
    {
        DirtyBits = DIRTY_ALL;
    }

//  BS.WriteMarker();

    //
    // Try to pack up the GENERIC_INIT data.
    //
    BS.OpenSection();
    if( BS.WriteFlag( (DirtyBits & DIRTY_GENERIC_INIT) ) )
    {
        BS.WriteRangedS32( m_GameType, GAME_CTF, GAME_HUNTER );
        BS.WriteWString( m_Score.MissionName );
        BS.WriteRangedS32( m_TimeLimit, 0, 60 );
    }
    if( BS.CloseSection() )  DirtyBits &= ~DIRTY_GENERIC_INIT;
    else                            BS.WriteFlag( FALSE );

//  BS.WriteMarker();

    //
    // Try to pack up the GENERIC_SWITCH data.
    //
    BS.OpenSection();
    if( BS.WriteFlag( (DirtyBits & DIRTY_GENERIC_SWITCH) ) )
    {
        for( i = 0; i < 16; i++ )
            BS.WriteTeamBits( m_SwitchBits[i] );
    }
    if( BS.CloseSection() )  DirtyBits &= ~DIRTY_GENERIC_SWITCH;
    else                            BS.WriteFlag( FALSE );

//  BS.WriteMarker();

    //
    // Try to pack up the GENERIC_POWER data.
    //
    BS.OpenSection();
    if( BS.WriteFlag( (DirtyBits & DIRTY_GENERIC_POWER) ) )
    {
        for( i = 0; i < 16; i++ )
            BS.WriteFlag( m_PowerCount[i] > 0 );
    }
    if( BS.CloseSection() )  DirtyBits &= ~DIRTY_GENERIC_POWER;
    else                            BS.WriteFlag( FALSE );

//  BS.WriteMarker();

    //
    // Try to pack up the GENERIC_TIME data.
    //
    BS.OpenSection();
    if( BS.WriteFlag( (DirtyBits & DIRTY_GENERIC_TIME) ) )
    {
        if( BS.WriteFlag( m_TimeLimit > 0 ) )
            BS.WriteRangedF32( m_TimeRemaining, 16, 0.0f, 4096.0f );
    }
    if( BS.CloseSection() )  DirtyBits &= ~DIRTY_GENERIC_TIME;
    else                            BS.WriteFlag( FALSE );

//  BS.WriteMarker();

    //
    // Try to pack up the GENERIC_VOTE_NEW data.
    //
    BS.OpenSection();
    if( BS.WriteFlag( (DirtyBits & DIRTY_GENERIC_VOTE_NEW) ) )
    {
        if( BS.WriteFlag( m_KickPlayer != -1 ) )
        {
            // There is a kick player vote in progress.
            BS.WriteRangedS32( m_KickPlayer,  0,  31 );
            BS.WriteU32      ( m_VotePlayers         );
            BS.WriteRangedS32( m_VotePercent, 0, 100 );
        }
        if( BS.WriteFlag( m_ChangeMap != -1 ) )
        {
            // There is a change map vote in progress.
            BS.WriteRangedS32( m_ChangeMap,   0, 127 );
            BS.WriteU32      ( m_VotePlayers         );
            BS.WriteRangedS32( m_VotePercent, 0, 100 );
        }
    }
    if( BS.CloseSection() )  DirtyBits &= ~DIRTY_GENERIC_VOTE_NEW;
    else                            BS.WriteFlag( FALSE );

//  BS.WriteMarker();

    //
    // Try to pack up the GENERIC_VOTE_UPDATE data.
    //
    BS.OpenSection();
    if( BS.WriteFlag( (DirtyBits & DIRTY_GENERIC_VOTE_UPDATE) ) )
    {
        if( BS.WriteFlag( m_KickPlayer != -1 ) )
        {
            // There is a kick player vote in progress.
            BS.WriteRangedS32( m_VotesFor,     0, 32 );
            BS.WriteRangedS32( m_VotesAgainst, 0, 32 );
            BS.WriteRangedS32( m_VotesMissing, 0, 32 );
        }
        if( BS.WriteFlag( m_ChangeMap != -1 ) )
        {
            // There is a change map vote in progress.
            BS.WriteRangedS32( m_VotesFor,     0, 32 );
            BS.WriteRangedS32( m_VotesAgainst, 0, 32 );
            BS.WriteRangedS32( m_VotesMissing, 0, 32 );
        }
    }
    if( BS.CloseSection() )  DirtyBits &= ~DIRTY_GENERIC_VOTE_UPDATE;
    else                            BS.WriteFlag( FALSE );

//  BS.WriteMarker();

    //
    // Try to pack up data for each player which is marked as dirty.
    //
    if( BS.WriteFlag( PlayerBits ) )
    {
        for( i = 0; i < 32; i++ )
        {
            BS.OpenSection();
            if( BS.WriteFlag( PlayerBits & (1 << i) ) )
            {
                if( BS.WriteFlag( m_Score.Player[i].IsConnected ) )
                {
                    BS.WriteWString  ( m_Score.Player[i].Name );
                    BS.WriteRangedS32( m_Score.Player[i].Machine, 0, 31 );

                    if( BS.WriteFlag( m_Score.Player[i].IsInGame ) )
                    {
                        BS.WriteObjectID ( m_PlayerID[i] );
                        BS.WriteRangedS32( m_Score.Player[i].Team, 0, 31 );
                        BS.WriteFlag( m_Score.Player[i].IsBot );
                        BS.WriteFlag( m_Score.Player[i].IsMale );
                    }
                }
            }
            if( BS.CloseSection() )  PlayerBits &= ~(1 << i);
            else                            BS.WriteFlag( FALSE );
        }
    }

//  BS.WriteMarker();

    //
    // Try to pack up scores for each player with dirty scores.
    //
    if( BS.WriteFlag( ScoreBits ) )
    {
        for( i = 0; i < 32; i++ )
        {
            s32 Score  = m_Score.Player[i].Score;
            s32 Kills  = m_Score.Player[i].Kills;
            s32 Deaths = m_Score.Player[i].Deaths;

            BS.OpenSection();
            if( BS.WriteFlag( (ScoreBits & (1 << i)) &&
                              (m_Score.Player[i].IsConnected) &&
                              (m_Score.Player[i].IsInGame) ) )
            {
                if     ( BS.WriteFlag( IN_RANGE( -255, Score ,  255 ) ) )  BS.WriteRangedS32( Score,  -255,  255 );
                else if( BS.WriteFlag( IN_RANGE(  256, Score , 5000 ) ) )  BS.WriteRangedS32( Score,   256, 5000 );
                else                                                       BS.WriteS32      ( Score );

                if     ( BS.WriteFlag( IN_RANGE(    0, Kills ,   63 ) ) )  BS.WriteRangedS32( Kills,     0,   63 );
                else if( BS.WriteFlag( IN_RANGE(   64, Kills , 1000 ) ) )  BS.WriteRangedS32( Kills,    64, 1000 );
                else                                                       BS.WriteS32      ( Kills );

                if     ( BS.WriteFlag( IN_RANGE(    0, Deaths,   63 ) ) )  BS.WriteRangedS32( Deaths,    0,   63 );
                else if( BS.WriteFlag( IN_RANGE(   64, Deaths, 1000 ) ) )  BS.WriteRangedS32( Deaths,   64, 1000 );
                else                                                       BS.WriteS32      ( Deaths );
            }
            if( BS.CloseSection() )  ScoreBits &= ~(1 << i);
            else                     BS.WriteFlag( FALSE );
        }
    }

//  BS.WriteMarker();

    //
    // Let the specific game logic send its data.
    // But ONLY if the generic initialization has been sent.
    //
    ASSERT( pGameLogic );
    if( !(DirtyBits & DIRTY_GENERIC_INIT) )
        pGameLogic->ProvideUpdate( BS, DirtyBits );

//  BS.WriteMarker();
}

//==============================================================================

void game_mgr::Connect( s32&                          PlayerIndex,
                        s32                           Machine,
                        const xwchar*                 pName,
                        f32                           TVRefreshRate,
                        player_object::character_type Character,
                        player_object::skin_type      Skin,
                        player_object::voice_type     Voice,
                        xbool                         Bot )
{
    PlayerIndex = -1;

    if( !tgl.ServerPresent )
        return;

    ASSERT( m_Initialized );

    // Can we let this player this player in?

    // Bot and already have maximum players?
    if( Bot && (m_NPlayers == m_MaxPlayers) )
    {
        ASSERT( FALSE );
        return;
    }
     
    // Human and already maximum human players?
    if( !Bot && (m_NHumans == m_MaxPlayers) )
    {
        ASSERT( FALSE );
        return;
    }

    // Human and already maximum players (with at least some bots)?
    if( !Bot && (m_NPlayers == m_MaxPlayers) && (m_NBots > 0) )
    {
        RemoveBot();
    }

    // Find available position for the new player.
    for( PlayerIndex = 0; m_Score.Player[PlayerIndex].IsConnected; PlayerIndex++ )
        ;
    ASSERT( PlayerIndex <= 31 );

    // Name game.
    if( Bot )
    {
        // What kind of game are we playing?
        if( m_CampaignMission == -1 )
        {
            // Ordinary game (online or botmatch).
            s32      Name = (m_BotNameBase + PlayerIndex) % ((sizeof(BotNames) / 4) - 1 );
            xwstring Wide = "\025";
            Wide += BotNames[Name];
            RegisterName( PlayerIndex, Wide );        
        }
        else
        {
            // Campaign mission.

            if( IN_RANGE(  1, m_CampaignMission,  5 ) )
            {
                // Game type "training" missions.
                // Use recruit names.
                s32      Name = (m_BotNameBase + PlayerIndex) % ((sizeof(RecruitNames) / 4) - 1 );
                xwstring Wide = RecruitNames[Name];
                RegisterName( PlayerIndex, Wide );        
            }

            if( IN_RANGE(  6, m_CampaignMission, 11 ) )
            {
                // "Story mode" missions.
                // Use recruit names and bioderm names.
                if( Character == player_object::CHARACTER_TYPE_BIO )
                {
                    // Derm name.
                    s32      Name = (m_BotNameBase + m_BioDermOffset++) % ((sizeof(DermNames) / 4) - 1 );
                    xwstring Wide = DermNames[Name];
                    RegisterName( PlayerIndex, Wide );        
                }
                else
                {
                    // Recruit name.
                    s32      Name = (m_BotNameBase + PlayerIndex) % ((sizeof(RecruitNames) / 4) - 1 );
                    xwstring Wide = RecruitNames[Name];
                    RegisterName( PlayerIndex, Wide );        
                }
            }

            if( IN_RANGE( 12, m_CampaignMission, 16 ) )
            {
                // Training missions.  Use "Training Bot".
                xwstring Wide = "Training Bot";
                RegisterName( PlayerIndex, Wide );        
            }
        }
    }
    else
    {
        ASSERT( pName );
        RegisterName( PlayerIndex, pName );
    }

    m_Score.Player[PlayerIndex].IsConnected = TRUE;
    m_Score.Player[PlayerIndex].IsInGame    = FALSE;
    m_Score.Player[PlayerIndex].IsBot       = Bot;
    m_Score.Player[PlayerIndex].IsMale      = (Character != player_object::CHARACTER_TYPE_FEMALE);
    m_Score.Player[PlayerIndex].Team        = 0;
    m_Score.Player[PlayerIndex].Machine     = Machine;

    m_Options[PlayerIndex].TVRefreshRate    = TVRefreshRate ;
    m_Options[PlayerIndex].Character        = Character;
    m_Options[PlayerIndex].Skin             = Skin;
    m_Options[PlayerIndex].Voice            = Voice;
    m_Options[PlayerIndex].Bot              = Bot;

    m_NPlayers++;
    if( Bot )  m_NBots++;
    else       m_NHumans++;

    // Let the specific game logic do its thing.  
    // ATTENTION:  This function call MUST set m_Score.Player[?].Team and take
    //             care of m_Score.Team[?].Size.
    ASSERT( pGameLogic );
    pGameLogic->Connect( PlayerIndex );

    // Announcement for the players on the server.
    if( !m_Score.Player[PlayerIndex].IsBot )
        MsgMgr.Message( MSG_PLAYER_CONNECTED, 0, ARG_INLINE, PlayerIndex );

    // Set dirty bits.
    m_PlayerBits |= (1 << PlayerIndex);
    m_ScoreBits  |= (1 << PlayerIndex);
}

//==============================================================================

void game_mgr::RegisterName( s32 PlayerIndex, const xwchar* pName )
{
    s32      i;
    s32      Version = 0;
    xwstring Name    = pName;
    xwstring Ext     = "123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    if( Name.GetLength() > 15 )
        Name = Name.Left( 15 );

    // Look for a name collision.
    for( i = 0; i < 32; i++ )
    {
        if( (m_Score.Player[i].IsConnected) &&
            (Name == m_Score.Player[i].Name) )
            break;
    }

    // Well?
    if( i < 32 )
    {
        // We had a name collision.  Crap.  "Adjust" the name.

        if( Name.GetLength() > 13 )
            Name = Name.Left( 13 );

        Name += ".?";

        s32 Edit = Name.GetLength() - 1;

        i = 0;

        do
        {
            Name[Edit] = Ext[Version];
            if( (m_Score.Player[i].IsConnected) &&
                (Name == m_Score.Player[i].Name) )
            {
                Version++;
                i = 0;
            }
            else
            {
                i++;
            }
        } while( i < 32 );
    }

    // Ship it!
    x_wstrcpy( m_Score.Player[PlayerIndex].Name, (const xwchar*)Name );
}

//==============================================================================

void game_mgr::Disconnect( s32 PlayerIndex )
{
    if( !tgl.ServerPresent )
        return;

    ASSERT( m_Initialized );
    ASSERT( PlayerIndex >=  0 );
    ASSERT( PlayerIndex <  32 );
    ASSERT(  m_Score.Player[PlayerIndex].IsConnected );
    ASSERT( !m_Score.Player[PlayerIndex].IsInGame );

    // Let the specific game logic do its thing.  
    // ATTENTION:  This function call MUST set m_Score.Player[?].Team and take
    //             care of m_Score.Team[?].Size.
    ASSERT( pGameLogic );
    pGameLogic->Disconnect( PlayerIndex );

    // Announcement for the players on the server.
    if( !m_Score.Player[PlayerIndex].IsBot )
        MsgMgr.Message( MSG_PLAYER_DISCONNECTED, 0, ARG_INLINE, PlayerIndex );

    m_Score.Player[PlayerIndex].Name[0]     = '\0';
    m_Score.Player[PlayerIndex].IsConnected = FALSE;
    m_Score.Player[PlayerIndex].IsBot       = FALSE;
    m_Score.Player[PlayerIndex].IsMale      = FALSE;
    m_Score.Player[PlayerIndex].Machine     = -1;
    m_Score.Player[PlayerIndex].Team        = -1;

    m_NPlayers--;
    if( m_Options[PlayerIndex].Bot )   m_NBots--;
    else                               m_NHumans--;

    m_Options[PlayerIndex].Bot = FALSE;

    // Clear the player object ID from the internal list.
    m_PlayerID[PlayerIndex].SetAsS32( -1 );

    // Set dirty bits.
    m_PlayerBits |= (1 << PlayerIndex);
    m_ScoreBits  |= (1 << PlayerIndex);
}

//==============================================================================

void game_mgr::CreatePlayer( s32 PlayerIndex )
{
    ASSERT( m_GameInProgress );
    ASSERT( m_GameType != GAME_NONE );

    // Lookup correct loadout for game
    s32 DefaultLoadout = -1;
    
    if( m_Campaign )
    {
        if( m_CampaignMission > m_NCampaignMissions )
        {
            s32 Mission = m_CampaignMission - m_NCampaignMissions;

            // Determine training mission loadout
            switch( Mission )
            {
                case  1 : DefaultLoadout = default_loadouts::STANDARD_EMPTY; break;
                case  2 : DefaultLoadout = default_loadouts::STANDARD_EMPTY; break;
                case  3 : DefaultLoadout = default_loadouts::STANDARD_HEAVY; break;
                case  4 : DefaultLoadout = default_loadouts::STANDARD_HEAVY; break;
                case  5 : DefaultLoadout = default_loadouts::STANDARD_CTF;   break;
                default : ASSERT( FALSE );
            }
        }
        else
        {
            // Determine any special loadouts for the campaign missions
            switch( m_CampaignMission )
            {
                case  3 : DefaultLoadout = default_loadouts::STANDARD_HUNTER;    break;
                case  6 : DefaultLoadout = default_loadouts::STANDARD_CAMPAIGN2; break;
                case  8 : DefaultLoadout = default_loadouts::STANDARD_HEAVY2;    break;
                default : DefaultLoadout = default_loadouts::STANDARD_CTF;       break;
            }
        }
    }
    else
    {
        switch(m_GameType)
        {
            case GAME_CTF:          DefaultLoadout = default_loadouts::STANDARD_CTF;       break;
            case GAME_CNH:          DefaultLoadout = default_loadouts::STANDARD_CNH;       break;
            case GAME_TDM:          DefaultLoadout = default_loadouts::STANDARD_TDM;       break;
            case GAME_DM:           DefaultLoadout = default_loadouts::STANDARD_DM;        break;
            case GAME_HUNTER:       DefaultLoadout = default_loadouts::STANDARD_HUNTER;    break;
            default:                ASSERT( FALSE );
        }
    }

    // Init bot/player?
    if( m_Options[PlayerIndex].Bot )
    {
        bot_object* pBot;
        pBot = (bot_object*)ObjMgr.CreateObject( object::TYPE_BOT );
        pBot->Initialize( vector3(0,0,0),
                          m_Options[PlayerIndex].TVRefreshRate,
                          m_Options[PlayerIndex].Character,
                          m_Options[PlayerIndex].Skin,
                          m_Options[PlayerIndex].Voice,
                          DefaultLoadout,
                          m_Score.Player[PlayerIndex].Name,
                          FALSE );
        pBot->SetTeamBits( 1 << m_Score.Player[PlayerIndex].Team );
        pBot->SetPlayerIndex( PlayerIndex );
        ObjMgr.AddObject( pBot, PlayerIndex );
        m_PlayerID[PlayerIndex] = pBot->GetObjectID();
        pBot->ServerInitialize(-1);
    }
    else
    {
        player_object* pPlayer;
        pPlayer = (player_object*)ObjMgr.CreateObject( object::TYPE_PLAYER );
        pPlayer->Initialize( m_Options[PlayerIndex].TVRefreshRate,
                             m_Options[PlayerIndex].Character,
                             m_Options[PlayerIndex].Skin,
                             m_Options[PlayerIndex].Voice,
                             DefaultLoadout,
                             m_Score.Player[PlayerIndex].Name );
        pPlayer->SetTeamBits( 1 << m_Score.Player[PlayerIndex].Team );
        pPlayer->SetPlayerIndex( PlayerIndex );
        ObjMgr.AddObject( pPlayer, PlayerIndex );
        m_PlayerID[PlayerIndex] = pPlayer->GetObjectID();

        if( PlayerIndex < tgl.NRequestedPlayers )
            pPlayer->ServerInitialize( PlayerIndex );
        else
            pPlayer->ServerInitialize(-1);

        pPlayer->SetVoteCanCast ( FALSE );
        pPlayer->SetVoteCanStart( (m_GameInProgress) && 
                                  (!m_VoteInProgress) && 
                                  (m_TimeRemaining > 60.0f) );
    }
}

//==============================================================================

object::id game_mgr::EnterGame( s32 PlayerIndex )
{
    if( !tgl.ServerPresent )
        return( obj_mgr::NullID );

    ASSERT( m_Initialized );
    ASSERT( PlayerIndex >=  0 );
    ASSERT( PlayerIndex <  32 );
    ASSERT(  m_Score.Player[PlayerIndex].IsConnected );
    ASSERT( !m_Score.Player[PlayerIndex].IsInGame );

    m_Score.Player[PlayerIndex].Score  = 0;
    m_Score.Player[PlayerIndex].Kills  = 0;
    m_Score.Player[PlayerIndex].Deaths = 0;

    // Let the specific game logic do its thing.  
    ASSERT( pGameLogic );
    pGameLogic->EnterGame( PlayerIndex );

    m_Score.Player[PlayerIndex].IsInGame = TRUE;

    // If a game is already running, create the player or bot object.
    if( m_GameInProgress )
    {
        CreatePlayer( PlayerIndex );
        
        // Announcement for the players on the server.
        if( !m_Score.Player[PlayerIndex].IsBot )
        {
            if( m_Score.IsTeamBased )
                MsgMgr.Message( MSG_PLAYER_JOINED_TEAM, 0, ARG_INLINE, PlayerIndex, 
                                ARG_TEAM, m_Score.Player[PlayerIndex].Team );
            else
                MsgMgr.Message( MSG_PLAYER_JOINED_GAME, 0, ARG_INLINE, PlayerIndex );
        }
    }
    else
    {
        m_PlayerID[PlayerIndex] = obj_mgr::NullID;
    }

    // Set dirty bits.
    m_PlayerBits |= (1 << PlayerIndex);
    m_ScoreBits  |= (1 << PlayerIndex);

    // Return the object ID of the new player (or bot).
    return( m_PlayerID[PlayerIndex] );
}

//==============================================================================

void game_mgr::ExitGame( s32 PlayerIndex )
{
    if( !tgl.ServerPresent )
        return;

    ASSERT( m_Initialized );
    ASSERT( PlayerIndex >=  0 );
    ASSERT( PlayerIndex <  32 );
    ASSERT( m_Score.Player[PlayerIndex].IsConnected );

    if( m_Score.Player[PlayerIndex].IsInGame )
    {
        // Let the specific game logic do its thing.
        pGameLogic->ExitGame( PlayerIndex );

        m_Score.Player[PlayerIndex].IsInGame = FALSE;
        m_Score.Player[PlayerIndex].Score    =  0;
        m_Score.Player[PlayerIndex].Kills    =  0;
        m_Score.Player[PlayerIndex].Deaths   =  0;

        ObjMgr.UnbindOriginID( m_PlayerID[PlayerIndex] );

        // Set dirty bits.
        m_PlayerBits |= (1 << PlayerIndex);
        m_ScoreBits  |= (1 << PlayerIndex);

        // Destroy the player object if it exists.
        if( m_GameInProgress )
        {
            ObjMgr.DestroyObject( m_PlayerID[PlayerIndex] );
            m_PlayerID[PlayerIndex] = obj_mgr::NullID;
        }

        // Cancel kick vote if this player was the object of the vote.
        if( m_KickPlayer == PlayerIndex )
            ShutDownVote();
    }
}

//==============================================================================

const game_score& game_mgr::GetScore( void )
{
    if( m_Score.IsGameOver )
        return( m_FinalScore );
    else
        return( m_Score );
}

//==============================================================================

void game_mgr::ScoreForPlayer( object::id PlayerID, s32 Score, s32 Message )
{
    if( IN_RANGE( 0, PlayerID.Slot, 31 ) )
    {
        s32 PlayerIndex = PlayerID.Slot;
    
        if( m_Score.Player[ PlayerIndex ].IsInGame )
        {
            player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( PlayerID );

            if( pPlayer )
            {
                m_Score.Player[ PlayerIndex ].Score += Score;
                m_ScoreBits |= (1 << PlayerIndex);
                if( (Message != -1) && (m_GameType != GAME_CAMPAIGN) )
                    MsgMgr.Message( Message, PlayerIndex, ARG_N, ABS(Score) );
            }
        }
    }
}

//==============================================================================

void game_mgr::GetPlayerCounts( s32& Humans, s32& Bots, s32& Maximum )
{
    const game_score& Score = GetScore();

    s32 i;

    Humans = 0;
    Bots   = 0;

    for( i = 0; i < 32; i++ )
    {
        if( Score.Player[i].IsConnected )
        {
            if( Score.Player[i].IsBot )   Bots++;
            else                          Humans++;
        }        
    }

    Maximum = m_MaxPlayers;
}

//==============================================================================

void game_mgr::AdvanceTime( f32 DeltaTime )
{
    s32 i;

#ifdef T2_DESIGNER_BUILD
    return;
#endif

    if( !g_BotDebug.DisableTimer)
    if( m_TimeLimit > 0 )
    {
        m_TimeRemaining   -= DeltaTime;
        m_TimeUpdateTimer -= DeltaTime;
    }

    if( m_TimeRemaining < 0.0f )
        m_TimeRemaining = 0.0f;

    if( !tgl.ServerPresent )
        return;

/*
    if( m_TimeLimit > 0 )
    {
        x_printfxy( 0, 1, "%2d:%02d %d%%",
                          ((s32)m_TimeRemaining / 60),
                          ((s32)m_TimeRemaining % 60),
                          (s32)((GetGameProgress() * 100.0f) + 0.5f) );
    }
    else
        x_printfxy( 0, 1, "%d%%",
                          (s32)((GetGameProgress() * 100.0f) + 0.5f) );
*/

    // Move in the 4th dimension.
    pGameLogic->AdvanceTime( DeltaTime );
    if( m_Score.IsGameOver )
        return;

    // Out of time?
    if( (m_TimeLimit > 0) && (m_TimeRemaining <= 0.0f) )
    {
        EndGame();
        return;
    }

    // Verbal warnings.
    if( (m_TimeLimit > 0) && (m_TimeRemaining <= 62.0f) && (!m_Said60) )
    {
        m_Said60 = TRUE;
        MsgMgr.Message( MSG_TIME_60, 0 );
    }
    if( (m_TimeLimit > 0) && (m_TimeRemaining <= 32.0f) && (!m_Said30) )
    {
        m_Said30 = TRUE;
        MsgMgr.Message( MSG_TIME_30, 0 );
    }
    if( (m_TimeLimit > 0) && (m_TimeRemaining <= 12.0f) && (!m_Said10) )
    {
        m_Said10 = TRUE;
        MsgMgr.Message( MSG_TIME_10, 0 );
    }

    // No negative time values allowed.
    if( m_TimeRemaining < 0.0f )
        m_TimeRemaining = 0.0f;

    // Time for a generic update?
    if( m_TimeUpdateTimer < 0.0f )
    {
        m_TimeUpdateTimer = 30.0f;
        m_DirtyBits |= DIRTY_GENERIC_TIME;
    }

    // Has the score limit been reached?
    if( m_MaxScore > 0 )
    {
        if( m_Score.IsTeamBased )
        {
            // Some team has reached maximum score?
            for( i = 0; i < 2; i++ )
            {
                if( m_Score.Team[i].Score >= m_MaxScore )
                {
                    EndGame();
                    return;
                }
            }
        }
        else
        {
            // Some player has reached maximum score?
            for( i = 0; i < 32; i++ )
            {
                if( m_Score.Player[i].IsInGame && (m_Score.Player[i].Score >= m_MaxScore) )
                {
                    EndGame();
                    return;
                }
            }
        }
    }

    // Advance vote.
    if( m_VoteInProgress )
    {
        m_VoteTimer -= DeltaTime;

        if( (m_VoteTimer <= 10.0f) && ((m_VoteTimer + DeltaTime) > 10.0f) )
        {
            //x_DebugMsg( "GAMEMGR: Vote time expired\n" );

            // Time is up, check the results.
            s32 Votes = m_VotesFor + m_VotesAgainst;
            if( (Votes != 0) && (((m_VotesFor * 100) / (Votes)) >= m_VotePercent) )
                AnnounceVotePasses();
            else
                AnnounceVoteFails();
        }

        if( m_VoteTimer <= 0.0f )
        {
            ExecuteVote();
        }
    }

    // Update up the evil bot army.
    if( m_GameType != GAME_CAMPAIGN )
    {
        UpdateBotCount();
    }

    // Iron fist.
    EnforceBounds( DeltaTime );
}

//==============================================================================

void game_mgr::UpdateSensorNet( void )
{
    s32            i;
    object*        pObject;
    player_object* pPlayer;
    vehicle*       pVehicle;

    object::type   SensorType[] = 
    {
        object::TYPE_SENSOR_LARGE,
        object::TYPE_SENSOR_MEDIUM,
        object::TYPE_SENSOR_REMOTE,
        object::TYPE_TURRET_FIXED,
        object::TYPE_TURRET_SENTRY,
        object::TYPE_TURRET_CLAMP,
        object::TYPE_PLAYER,
        object::TYPE_BOT,
    };

    //
    // Clear sensed bits for all players (bots) and vehicles.
    //

    for( i = 0; i < 32; i++ )
    {
        if( m_Score.Player[i].IsInGame )
        {              
            pPlayer = (player_object*)ObjMgr.GetObjectFromSlot( i );
            if( pPlayer )
                pPlayer->ClearSensedBits();
        }
    }

    for( i = object::TYPE_VEHICLE_WILDCAT; 
         i < object::TYPE_VEHICLE_HAVOC; 
         i++ )
    {
        ObjMgr.StartTypeLoop( (object::type)i );
        while( (pVehicle = (vehicle*)ObjMgr.GetNextInType()) )
            pVehicle->ClearSensedBits();
        ObjMgr.EndTypeLoop();
    }

    //
    // Use each sensor type to attempt to sense stuff.
    //

    for( i = 0; i < 8; i++ )
    {
        object* pSensor;

        ObjMgr.StartTypeLoop( SensorType[i] );
        while( (pSensor = ObjMgr.GetNextInType()) )
        {
            f32     SenseRadius = pSensor->GetSenseRadius();

            if( SenseRadius < 1.0f )
                continue;

            xbool   AATurret    = FALSE;
            vector3 From        = pSensor->GetBBox().GetCenter();
            u32     TeamBits    = pSensor->GetTeamBits();
            f32     RSquared    = SenseRadius * SenseRadius;

            // For players (and bots) move the sense sphere forward relative to
            // the player.
            if( pSensor->GetAttrBits() & object::ATTR_PLAYER )
            {
                player_object* pPlayer = (player_object*)pSensor;
                vector3 Offset( 0.0f, 0.0f, SenseRadius );
                Offset.Rotate( pPlayer->GetRotation() );
                From += Offset;
            }

            if( SensorType[i] == object::TYPE_TURRET_FIXED )
            {
                turret* pTurret = (turret*)pSensor;
                if( (pTurret->GetKind() == turret::AA) && 
                    (pTurret->GetEnabled()) )
                {
                    vector3 Point = pTurret->GetPosition();
                    bbox    BBox( Point, pSensor->GetSenseRadius() );
                    Point.Y += 350.0f;
                    BBox += Point;

                    ObjMgr.Select( object::ATTR_SENSED, BBox );
                    AATurret = TRUE;
                }
            }

            if( !AATurret )
                ObjMgr.Select( object::ATTR_SENSED, From, SenseRadius );

            while( (pObject = ObjMgr.GetNextSelected()) )
            {
                // Don't sense self.
                if( pObject == pSensor )
                    continue;

                u32 SenseBits = pObject->GetSensedBits();

                // Don't bother sensing again if already sensed.
                if( SenseBits & TeamBits )
                    continue;

                // Make sure range is good.
                vector3 To   = pObject->GetBBox().GetCenter();
                if( AATurret )  
                    To.Y = From.Y;
                f32 DSquared = (From - To).LengthSquared();
                if( DSquared < RSquared )
                {
                    // We got it all.  Its in the radius, and it hasn't been 
                    // sensed by this team before now.
                    pObject->AddSensedBits( TeamBits );
                }
            }
            ObjMgr.ClearSelection();
        }
        ObjMgr.EndTypeLoop();
    }
}

//==============================================================================

void game_mgr::EnforceBounds( f32 DeltaTime )
{
    s32             Type;
    s32             i;
    player_object*  pPlayer;

    // Make sure no vehicles go above the flight ceiling.
    for( Type = object::TYPE_VEHICLE_SHRIKE; Type <= object::TYPE_VEHICLE_JERICHO2; Type++ )
    {
        vehicle* pVehicle;
        ObjMgr.StartTypeLoop( (object::type)Type );
        while( (pVehicle = (vehicle*)ObjMgr.GetNextInType()) )
        {
            // Tweak the velocity to reduce flight ceiling violations.
            vector3 Position = pVehicle->GetPosition();
            vector3 Velocity = pVehicle->GetVel();
            if( (Position.Y > m_Bounds.Max.Y) && (Velocity.Y > -1.0f) )
            {
                Velocity.Y -= DeltaTime * 25.0f;
                pVehicle->SetVel( Velocity );
            }
        }
        ObjMgr.EndTypeLoop();
    }

    // Make sure no players (or bots) go above the flight ceiling.
    for( i = 0; i < 32; i++ )
    {
        if( m_Score.Player[i].IsInGame )
        {              
            pPlayer = (player_object*)ObjMgr.GetObjectFromSlot( i );
            if( pPlayer )
            {
                // Tweak the velocity to reduce flight ceiling violations.
                vector3 Position = pPlayer->GetPosition();
                vector3 Velocity = pPlayer->GetVel();
                if( (Position.Y > m_Bounds.Max.Y) && (Velocity.Y > -1.0f) )
                {
                    Velocity.Y -= DeltaTime * 25.0f;
                    pPlayer->SetVel( Velocity );
                }
            }
        }
    }

    // Make sure nothing damageable is outside of extended DeathBounds.
    {
        object* pObject;
        vector3 Position;
        ObjMgr.SelectOutOfRep0( object::ATTR_DAMAGEABLE );
        while( (pObject = ObjMgr.GetNextSelected()) )
        {
            Position = pObject->GetPosition();
            if( !m_DeathBounds.Intersect( Position ) )
            {
                ObjMgr.InflictPain( pain::MISC_BOUNDS_DEATH, 
                                    pObject,
                                    Position,
                                    ObjMgr.NullID,
                                    pObject->GetObjectID() );
            }
        }
        ObjMgr.ClearSelection();
    }

    // Each game type may need to enforce bounds in its own way.
    pGameLogic->EnforceBounds( DeltaTime );
}

//==============================================================================

void game_mgr::SetTimeLimit( s32 Minutes )
{
    m_TimeLimit     = Minutes;
    m_TimeRemaining = Minutes * 60.0f;
}

//==============================================================================

void game_mgr::SetScoreLimit( s32 MaxScore )
{
    m_MaxScore = MaxScore;
}

//==============================================================================

s32 game_mgr::GetTimeLimit( void )
{
    return( m_TimeLimit );
}

//==============================================================================

f32 game_mgr::GetTimeRemaining( void )
{
    if( m_TimeLimit > 0 )
        return( m_TimeRemaining );
    else
        return( -1.0f );
}

//==============================================================================

f32 game_mgr::GetGameProgress( void )
{
    s32 i;
    f32 Progress = 0.0f;

    if( m_MaxScore > 0 )
    {
        if( m_Score.IsTeamBased )
        {
            for( i = 0; i < 2; i++ )
            {
                Progress = MAX( Progress, m_Score.Team[i].Score / m_MaxScore );
            }
        }
        else
        {
            for( i = 0; i < 32; i++ )
            {
                if( m_Score.Player[i].IsInGame )
                    Progress = MAX( Progress, m_Score.Player[i].Score / m_MaxScore );
            }
        }
    }

    if( m_TimeLimit > 0 )
    {
        Progress = MAX( Progress, 1.0f - (m_TimeRemaining / (m_TimeLimit * 60.0f)) );
    }

    return( Progress );
}

//==============================================================================

void game_mgr::PowerLoss( s32 PowerCircuit )
{
    if( !tgl.ServerPresent )
        return;

    m_PowerCount[ PowerCircuit ]--;
    m_DirtyBits |= DIRTY_GENERIC_POWER;
}

//==============================================================================

void game_mgr::PowerGain( s32 PowerCircuit )
{
    if( !tgl.ServerPresent )
        return;

    m_PowerCount[ PowerCircuit ]++;
    m_DirtyBits |= DIRTY_GENERIC_POWER;
}

//==============================================================================

void game_mgr::SwitchTouched( object::id SwitchID, object::id PlayerID )
{
    if( !tgl.ServerPresent )
        return;

    flipflop* pFlipFlop  = (flipflop*)ObjMgr.GetObjectFromID( SwitchID );
    object*   pPlayerObj =            ObjMgr.GetObjectFromID( PlayerID );

    u32 SwitchBits = m_SwitchBits[ pFlipFlop->GetSwitch() ];
    u32 PlayerBits = pPlayerObj->GetTeamBits();

    if( SwitchBits != PlayerBits )
    {
        m_SwitchBits[ pFlipFlop->GetSwitch() ] = PlayerBits;
        m_DirtyBits |= DIRTY_GENERIC_SWITCH;
        pGameLogic->SwitchTouched( SwitchID, PlayerID );

        SendAudio( SFX_MISC_SWITCH_GOOD, PlayerBits, SFX_MISC_SWITCH_BAD );
    }
}

//==============================================================================

u32 game_mgr::GetSwitchBits( s32 SwitchCircuit )
{
    return( m_SwitchBits[SwitchCircuit] );
}

//==============================================================================

void game_mgr::SetStartupPower( void )
{
    for( s32 i = 0; i < 16; i++ )
        m_PowerCount[i] = 1;
}

//==============================================================================

void game_mgr::InitPower( s32* pPowerCount )
{
    for( s32 i = 0; i < 16; i++ )
        m_PowerCount[i] = pPowerCount[i];
}

//==============================================================================

void game_mgr::InitSwitch( u32* pSwitchBits )
{
    for( s32 i = 0; i < 16; i++ )
        m_SwitchBits[i] = pSwitchBits[i];
}

//==============================================================================

xbool game_mgr::GetPower( s32 PowerCircuit )
{
#ifdef T2_DESIGNER_BUILD
    return( FALSE );
#endif
    return( m_PowerCount[PowerCircuit] > 0 );
}

//==============================================================================

s32 game_mgr::RandomTeamMember( s32 Team )
{
    if( m_Score.Team[Team].Size == 0 )
        return( -1 );

    s32 i;
    s32 j = x_irand( 0, m_Score.Team[Team].Size - 1 );

    for( i = 0; i < 32; i++ )
    {   
        if( (m_Score.Player[i].IsInGame) && 
            (m_Score.Player[i].Team == Team) )
        {
            if( j == 0 )
                return( i );
            else
                j--;
        }
    }

    // Should NEVER get here.
    return( -1 );
}

//==============================================================================

const xwchar* game_mgr::GetTeamName( u32 TeamBits )
{
    if( TeamBits == 0x01 )    return( m_Score.Team[0].Name );
    if( TeamBits == 0x02 )    return( m_Score.Team[1].Name );
    return( NULL );
}

//==============================================================================

bbox game_mgr::GetBounds( void )
{
    return( m_Bounds );
}

//==============================================================================

xbool game_mgr::RoomForPlayers( s32 Players )
{
    return( (m_NHumans + Players) <= m_MaxPlayers );
}

//==============================================================================

void game_mgr::SendAudio( s32 Sound, const vector3& Position )
{
    (void)Position;

    if( tgl.ServerPresent )
    {
        tgl.pServer->SendAudio( Sound );
    }
}

//==============================================================================

void game_mgr::SendAudio( s32 Sound, s32 PlayerIndex )
{
    if( tgl.ServerPresent )
    {
        tgl.pServer->SendAudio( Sound, PlayerIndex );
    }
}

//==============================================================================

void game_mgr::SendAudio( s32 Sound, u32 TeamBits )
{
    if( tgl.ServerPresent )
    {
        tgl.pServer->SendAudio( Sound, TeamBits );
    }
}

//==============================================================================

void game_mgr::SendAudio( s32 Sound1, u32 TeamBits1, s32 Sound2 )
{
    if( tgl.ServerPresent )
    {
        tgl.pServer->SendAudio( Sound1,  TeamBits1 );
        tgl.pServer->SendAudio( Sound2, ~TeamBits1 );
    }
}

//==============================================================================

void game_mgr::UpdateBotCount( void )
{
    if( !m_BotsEnabled )
        return;

    // If there are too many total players and there are bots, remove a bot.
    if( m_Score.IsTeamBased )
    {
        if( ((m_Score.Team[0].Size > m_MinPlayers) && (m_Score.Team[0].Bots > 0)) ||
            ((m_Score.Team[1].Size > m_MinPlayers) && (m_Score.Team[1].Bots > 0)) )
            RemoveBot();
    }
    else
    {
        if( (m_NPlayers > m_MinPlayers) && (m_NBots > 0) )
            RemoveBot();
    }

    // If there are not enough total players (less than min players), add a bot.
    // Check this per team for team based games.
    if( m_Score.IsTeamBased )
    {                                              
        if( (m_NPlayers < m_MaxPlayers) &&
            ((m_Score.Team[0].Size < m_MinPlayers) || 
             (m_Score.Team[1].Size < m_MinPlayers)) )
            AddBot();
    }
    else
    {
        if( m_NPlayers < m_MinPlayers )
            AddBot();
    }
}

//==============================================================================

void game_mgr::AddBot( void )
{
    s32  Name;
    s32  Index;

    player_object::character_type  TypeMap[4] = 
    {
        player_object::CHARACTER_TYPE_MALE,
        player_object::CHARACTER_TYPE_MALE,
        player_object::CHARACTER_TYPE_FEMALE,
        player_object::CHARACTER_TYPE_BIO,
    };

    player_object::character_type   Type;
    player_object::skin_type        Skin;
    player_object::voice_type       Voice = (player_object::voice_type)0;

    Name = (m_BotNameBase + m_NBots) % ((sizeof(BotNames) / 4) - 1 );

    Type = TypeMap[ x_irand( 0, 3 ) ];           

    if( m_Campaign && (Type == player_object::CHARACTER_TYPE_BIO) )
        Type = player_object::CHARACTER_TYPE_MALE;

    switch( Type )
    {
    case player_object::CHARACTER_TYPE_FEMALE:
        Voice = (player_object::voice_type)x_irand( player_object::VOICE_TYPE_FEMALE_HEROINE, 
                                                    player_object::VOICE_TYPE_FEMALE_AMAZON );
        break;

    case player_object::CHARACTER_TYPE_MALE:
        Voice = (player_object::voice_type)x_irand( player_object::VOICE_TYPE_MALE_HERO, 
                                                    player_object::VOICE_TYPE_MALE_PSYCHO );
        break;

    case player_object::CHARACTER_TYPE_BIO:
        Voice = (player_object::voice_type)x_irand( player_object::VOICE_TYPE_BIO_WARRIOR, 
                                                    player_object::VOICE_TYPE_BIO_PREDATOR );
        break;

    default:
        ASSERT( FALSE );
        break;
    }

    Skin = (player_object::skin_type)x_irand( player_object::SKIN_TYPE_START,  
                                              player_object::SKIN_TYPE_END );

    Connect( Index, 0, NULL, eng_GetTVRefreshRate(), Type, Skin, Voice, TRUE );
    EnterGame( Index );
}

//==============================================================================

void game_mgr::RemoveBot( void )
{
    s32 Index = -1;

    // TO DO: Find bot we can live without.  
    //        Doesn't have flag, on team with more players, etc.

    if( m_Score.IsTeamBased )
    {
        s32 Team = -1;

        // If a team has too many players, and at least one bot, then that team
        // loses a bot.
        if( (m_Score.Team[0].Size > m_MinPlayers) && (m_Score.Team[0].Bots > 0) )   Team = 0;
        if( (m_Score.Team[1].Size > m_MinPlayers) && (m_Score.Team[1].Bots > 0) )   Team = 1;

        // If both teams are properly sized (and we need to remove a bot, 
        // otherwise we wouldn't be in this function), then the team with more
        // bots loses a bot.
        if( (Team == -1) &&
            (m_Score.Team[0].Size == m_MinPlayers) && 
            (m_Score.Team[1].Size == m_MinPlayers) )
        {
            if( m_Score.Team[0].Bots > m_Score.Team[1].Bots )   Team = 0;
            else                                                Team = 1;
        }

        ASSERT( Team != -1 );

        // Find bot on given Team.
        for( Index = 0; Index < 32; Index++ )
        {
            if( (m_Options[Index].Bot) && (m_Score.Player[Index].Team == Team) )
                break;
        }
    }
    else
    {
        // Any old bot will do.
        for( Index = 0; Index < 32; Index++ )
        {
            if( m_Options[Index].Bot )
                break;
        }
    }

    ASSERT( Index != 32 );

    ExitGame  ( Index );
    Disconnect( Index );
}

//==============================================================================

void game_mgr::GetDeployStats( object::type Type, 
                               u32          TeamBits,
                               s32&         CurrentCount,
                               s32&         MaximumCount )
{
    CurrentCount = 0;
    MaximumCount = 0;

    object* pObject;

    ObjMgr.StartTypeLoop( Type );
    while( (pObject = ObjMgr.GetNextInType()) )
    {
        if( pObject->GetTeamBits() == TeamBits )
            CurrentCount++;
    }
    ObjMgr.EndTypeLoop();

    if( m_Score.IsTeamBased )
    {
        switch( Type )
        {
        case object::TYPE_STATION_DEPLOYED:     MaximumCount =  5;      break;
        case object::TYPE_SENSOR_REMOTE:        MaximumCount = 15;      break;
        case object::TYPE_BEACON:               MaximumCount = 30;      break;

        case object::TYPE_VEHICLE_WILDCAT:     
        case object::TYPE_VEHICLE_BEOWULF:     
        case object::TYPE_VEHICLE_JERICHO2:    
        case object::TYPE_VEHICLE_SHRIKE:      
        case object::TYPE_VEHICLE_THUNDERSWORD:
        case object::TYPE_VEHICLE_HAVOC:       
            MaximumCount = m_VehicleLimit[ Type - object::TYPE_VEHICLE_WILDCAT ];
            break;

        case object::TYPE_TURRET_CLAMP:         
            MaximumCount = MAX( 8, MAX( m_Score.Team[0].Size, 
                                        m_Score.Team[1].Size ) + 4 );
            break;

        default:  
            MaximumCount =  0;      
            break;
        }
    }
    else
    {
        switch( Type )
        {
        case object::TYPE_STATION_DEPLOYED:     MaximumCount =  5;      break;
        case object::TYPE_BEACON:               MaximumCount = 10;      break;
        default:                                MaximumCount =  0;      break;
        }
    } 
}

//==============================================================================

xbool IsVolumeIntersected( vector3* pVert, s32 nVerts )
{
    s32      i, j;
    collider Collider;

    for( i=0; i<nVerts; i++ )
    {
        for( j=0; j<nVerts; j++ )
        {
            if( i != j )
            {
                Collider.RaySetup( NULL, pVert[i], pVert[j] );
                ObjMgr.Collide( object::ATTR_SOLID, Collider );

                if( Collider.HasCollided() )
                {
                    return( TRUE );
                }
            }
        }
    }

    return( FALSE );
}

//==============================================================================

xbool IsInsideForceField( const vector3& Position, f32 Radius, vector3* pVert, s32 nVerts )
{
    object* pObject;

    ObjMgr.Select( object::ATTR_FORCE_FIELD, Position, Radius );
    while( (pObject = ObjMgr.GetNextSelected()) )
    {
        const bbox& BBox = pObject->GetBBox();
    
        for( s32 i=0; i<nVerts; i++ )
        {
            if( BBox.Intersect( pVert[i] ) )
            {
                break;
            }
        }
    }
    ObjMgr.ClearSelection();

    return( pObject != NULL );
}

//==============================================================================

xbool IsEnoughClearance( object::type Type, const vector3& Position, const vector3& Normal, radian Yaw = R_0 )
{
    radian3  Rot;
    matrix4  L2W;
    vector3* pVolume = NULL;
    s32      nVerts  = 0;

    Normal.GetPitchYaw( Rot.Pitch, Rot.Yaw );
    Rot.Roll = R_0;

    switch( Type )
    {
        case object::TYPE_TURRET_CLAMP :
            Rot.Pitch += R_90;
            pVolume    = s_TurretVolume;
            nVerts     = 8;
            break;
        
        case object::TYPE_SENSOR_REMOTE :
            Rot.Pitch += R_90;
            pVolume    = s_SensorVolume;
            nVerts     = 8;
            break;
        
        case object::TYPE_STATION_DEPLOYED :
            Rot.Yaw    = Yaw;
            Rot.Pitch  = R_0;
            pVolume    = s_InvenVolume;
            nVerts     = 7;
            break;
    
        default :
            DEMAND( FALSE );
            break;
    }

    s_nVerts = nVerts;

    L2W.Setup( vector3( 1, 1, 1 ), Rot, Position );
    L2W.Transform( s_VolumePts, pVolume, s_nVerts );

    if( IsVolumeIntersected( s_VolumePts, s_nVerts ))
    {
        return( FALSE );
    }

    if( IsInsideForceField( Position, 5.0f, s_VolumePts, s_nVerts ))
    {
        return( FALSE );
    }

    return( TRUE );
}

//==============================================================================

void game_mgr::RenderVolume( void )
{
    if( g_ShowVolumes )
    {
        for( s32 i=0; i<s_nVerts; i++ )
        {
            for( s32 j=0; j<s_nVerts; j++ )
            {
                if( i != j )
                {
                    draw_Line( s_VolumePts[i], s_VolumePts[j], XCOLOR_GREEN );
                }
            }
        }
        
        for( s32 n=0; n<s_nVerts; n++ )
        {
            draw_Point( s_VolumePts[n], XCOLOR_RED );
        }
    }
}

//==============================================================================

const f32 InvenRadius = 2.0f;

xbool IsNearDeployedInven( const vector3& Point )
{
    object* pObject;

    ObjMgr.Select( object::ATTR_ASSET, Point, InvenRadius );
    while( (pObject = ObjMgr.GetNextSelected()) )
    {
        if( pObject->GetType() == object::TYPE_STATION_DEPLOYED )
        {
            // Point is too near a deployed inventory station
            break;
        }
    }
    ObjMgr.ClearSelection();

    return( pObject != NULL );
}

//==============================================================================

xbool game_mgr::AttemptDeployTurret(       object::id    PlayerID,   
                                     const vector3&      Point,      
                                     const vector3&      Normal,     
                                           object::type  SurfaceType,
                                           xbool         TestOnly )  
{
    s32             Current;
    s32             Count;
    s32             Maximum;
    u32             TeamBits;
    player_object*  pPlayer;
    object*         pObject;

    if( !tgl.ServerPresent )
        return( FALSE );

    pPlayer = (player_object*)ObjMgr.GetObjectFromID( PlayerID );
    if( !pPlayer )
        return( FALSE );

    TeamBits = pPlayer->GetTeamBits();

    //
    // See if surface is an acceptable type.
    //
    if( (SurfaceType != object::TYPE_TERRAIN) && 
        (SurfaceType != object::TYPE_BUILDING) && 
        (SurfaceType != object::TYPE_SCENIC) )
    {
        if( !TestOnly )
            MsgMgr.Message( MSG_TURRET_DEPLOY_BAD_SURFACE, PlayerID.Slot );
        return( FALSE );
    }

    //
    // See if deployment is blocked by maximum count.
    //
    GetDeployStats( object::TYPE_TURRET_CLAMP, TeamBits, Current, Maximum );
#ifndef T2_DESIGNER_BUILD
    if( Current >= Maximum )
    {
        if( !TestOnly )
            MsgMgr.Message( MSG_TURRET_DEPLOY_LIMIT_REACHED, PlayerID.Slot );
        return( FALSE );
    }
    //
    // "Overload" and radius check.
    //
    Count = 0;
    ObjMgr.Select( object::ATTR_ASSET, Point, 50.0f );
    while( (pObject = ObjMgr.GetNextSelected()) )
    {
        if( pObject->GetType() == object::TYPE_TURRET_CLAMP )
        {
            Count++;
            if( Count > 4 )
            {
                if( !TestOnly )
                    MsgMgr.Message( MSG_TURRET_DEPLOY_TOO_MANY_NEAR, PlayerID.Slot );
                break;
            }

            vector3 Span = pObject->GetPosition() - Point;
            if( Span.LengthSquared() < (20*20) )
            {
                if( !TestOnly )
                    MsgMgr.Message( MSG_TURRET_DEPLOY_TOO_CLOSE, PlayerID.Slot );
                break;
            }
        }
    }
    ObjMgr.ClearSelection();

    if( pObject )
        return( FALSE );
#else
    Maximum = 50;
#endif

    //
    // Check for enough clearance to deploy turret.
    //
#ifndef T2_DESIGNER_BUILD
    if( !TestOnly )
    {
#else
		// Designer build:
		// Test the ground still exists.
		{
			collider Ray;
			vector3 A = Normal;
			A.Scale(0.01f);
			vector3 B = Point - A;
			A = Point + A;
			Ray.RaySetup( NULL, A, B);
			ObjMgr.Collide( object::ATTR_SOLID_STATIC, Ray );
			if( !Ray.HasCollided() )
				return (FALSE);
		}
#endif
        if( !IsEnoughClearance( object::TYPE_TURRET_CLAMP, Point, Normal ))
        {
            if( !TestOnly )
                MsgMgr.Message( MSG_TURRET_DEPLOY_NO_ROOM, PlayerID.Slot );
            //x_DebugMsg( "Not enough room to deploy turret\n" );
            return( FALSE );
        }
#ifndef T2_DESIGNER_BUILD
    }
#endif

    //
    // Make sure we don't block deployed inventory stations.
    //
    if( IsNearDeployedInven( Point ) == TRUE )
    {
        if( !TestOnly )
            MsgMgr.Message( MSG_DEPLOY_NEAR_REMOTE_STATION, PlayerID.Slot );
        //x_DebugMsg( "Point is too close to deployed inven\n" );
        return( FALSE );
    }

    //
    // If we made it here, then we can go ahead and make the turret.
    //
    if( !TestOnly )
    {
        turret* pTurret = (turret*)ObjMgr.CreateObject( object::TYPE_TURRET_CLAMP );
        pTurret->Initialize( Point, Normal, PlayerID, TeamBits );
        ObjMgr.AddObject( pTurret, obj_mgr::AVAILABLE_SERVER_SLOT );

        if( m_GameType != GAME_CAMPAIGN )
        {
            if( m_Score.IsTeamBased )
            {
                ScoreForPlayer( PlayerID, SCORE_DEPLOY_TEAM_DEP_TURRET );
                MsgMgr.Message( MSG_TURRET_DEPLOYED, PlayerID.Slot, ARG_N, Current+1, ARG_M, Maximum );
            }
            else
            {
                MsgMgr.Message( MSG_N_OF_M_TURRETS_DEPLOYED, PlayerID.Slot, ARG_N, Current+1, ARG_M, Maximum );
            }
        }
    }
    return( TRUE );
}

//==============================================================================

xbool game_mgr::AttemptDeployStation(       object::id    PlayerID,   
                                      const vector3&      Point,      
                                      const vector3&      Normal,     
                                            object::type  SurfaceType,
                                            xbool         TestOnly )  
{
    s32             Current;
    s32             Maximum;
    u32             TeamBits;
    radian          Yaw;
    player_object*  pPlayer;
    object*         pObject;

    if( !tgl.ServerPresent )
        return( FALSE );

    pPlayer = (player_object*)ObjMgr.GetObjectFromID( PlayerID );
    if( !pPlayer )
        return( FALSE );

    TeamBits = pPlayer->GetTeamBits();
    Yaw      = pPlayer->GetDrawRot().Yaw;

    //
    // See if surface is an acceptable type.
    //
    if( (SurfaceType != object::TYPE_TERRAIN) && 
        (SurfaceType != object::TYPE_BUILDING) && 
        (SurfaceType != object::TYPE_SCENIC) )
    {
        if( !TestOnly )
            MsgMgr.Message( MSG_STATION_DEPLOY_BAD_SURFACE, PlayerID.Slot );
        return( FALSE );
    }

    //
    // See if surface normal is within 30 degrees of vertical.
    //
    if( Normal.Y < 0.8660f )
    {
        if( !TestOnly )
            MsgMgr.Message( MSG_STATION_DEPLOY_TOO_STEEP, PlayerID.Slot );
        return( FALSE );
    }

#ifndef T2_DESIGNER_BUILD
    //
    // See if deployment is blocked by maximum count.
    //
    GetDeployStats( object::TYPE_STATION_DEPLOYED, TeamBits, Current, Maximum );
    if( Current >= Maximum )
    {
        if( !TestOnly )
            MsgMgr.Message( MSG_STATION_DEPLOY_LIMIT_REACHED, PlayerID.Slot );
        return( FALSE );
    }

    //
    // Radius check.
    //
    ObjMgr.Select( object::ATTR_ASSET, Point, 20.0f );
    while( (pObject = ObjMgr.GetNextSelected()) )
    {
        if( pObject->GetType() == object::TYPE_STATION_DEPLOYED )
        {
            if( !TestOnly )
                MsgMgr.Message( MSG_STATION_DEPLOY_TOO_CLOSE, PlayerID.Slot );
            break;
        }
    }
    ObjMgr.ClearSelection();

    if( pObject )
        return( FALSE );
#endif

    //
    // Check for enough clearance to deploy station.
    //
#ifndef T2_DESIGNER_BUILD
    if( !TestOnly )
    {
#else
		// Designer build:
		// Test the ground still exists.
		{
			collider Ray;
			vector3 A = Normal;
			A.Scale(0.01f);
			vector3 B = Point - A;
			A = Point + A;
			Ray.RaySetup( NULL, A, B);
			ObjMgr.Collide( object::ATTR_SOLID_STATIC, Ray );
			if( !Ray.HasCollided() )
				return (FALSE);
		}
#endif
        if( !IsEnoughClearance( object::TYPE_STATION_DEPLOYED, Point, Normal, Yaw ))
        {
            if( !TestOnly )
                MsgMgr.Message( MSG_STATION_DEPLOY_NO_ROOM, PlayerID.Slot );
            //x_DebugMsg( "Not enough room to deploy station\n" );
            return( FALSE );
        }
#ifndef T2_DESIGNER_BUILD
    }
#endif

    //
    // If we made it here, then we can go ahead and make the station.
    //
    if( !TestOnly )
    {
        // Place a beacon.

        vector3  A;
        vector3  B;
        vector3  C( 0.0f, 1.0f, x_frand( 0.5f, 1.0f ) );

        C.RotateY( x_frand( R_0, R_360 ) );
        A    = Point + C;
        B    = A;
        B.Y -= 2.0f;
        
        collider Ray;
        Ray.RaySetup( NULL, A, B );
        ObjMgr.Collide( object::ATTR_SOLID_STATIC, Ray );
        if( !Ray.HasCollided() )
        {
            // Rotate randomly, and try again.
            C.RotateY( x_frand( R_90, R_270 ) );
            A    = Point + C;
            B    = A;
            B.Y -= 2.0f;
            Ray.RaySetup( NULL, A, B );
            ObjMgr.Collide( object::ATTR_SOLID_STATIC, Ray );
        }
        if( Ray.HasCollided() )
        {
            collider::collision Collision;
            Ray.GetCollision( Collision );
            beacon* pBeacon = (beacon*)ObjMgr.CreateObject( object::TYPE_BEACON );
            pBeacon->Initialize( Collision.Point, Collision.Plane.Normal, TeamBits );
            ObjMgr.AddObject( pBeacon, obj_mgr::AVAILABLE_SERVER_SLOT );
        }

        // Place the station.

        station* pStation = (station*)ObjMgr.CreateObject( object::TYPE_STATION_DEPLOYED );
        pStation->Initialize( Point, Yaw, PlayerID, TeamBits );
        ObjMgr.AddObject( pStation, obj_mgr::AVAILABLE_SERVER_SLOT );

        if( m_GameType != GAME_CAMPAIGN )
        {
            if( m_Score.IsTeamBased )
            {
                ScoreForPlayer( PlayerID, SCORE_DEPLOY_TEAM_DEP_INVEN );
                MsgMgr.Message( MSG_STATION_DEPLOYED, PlayerID.Slot, ARG_N, Current+1, ARG_M, Maximum );
            }
            else
            {
                MsgMgr.Message( MSG_N_OF_M_STATIONS_DEPLOYED, PlayerID.Slot, ARG_N, Current+1, ARG_M, Maximum );
            }
        }
    }

    return( TRUE );
}

//==============================================================================

xbool game_mgr::AttemptDeploySensor(       object::id    PlayerID,   
                                     const vector3&      Point,      
                                     const vector3&      Normal,     
                                           object::type  SurfaceType,
                                           xbool         TestOnly )  
{
    s32             Current;
    s32             Maximum;
    u32             TeamBits;
    player_object*  pPlayer;
    object*         pObject;

    if( !tgl.ServerPresent )
        return( FALSE );

    pPlayer = (player_object*)ObjMgr.GetObjectFromID( PlayerID );
    if( !pPlayer )
        return( FALSE );

    TeamBits = pPlayer->GetTeamBits();

    //
    // See if surface is an acceptable type.
    //
    if( (SurfaceType != object::TYPE_TERRAIN) && 
        (SurfaceType != object::TYPE_BUILDING) && 
        (SurfaceType != object::TYPE_SCENIC) )
    {
        if( !TestOnly )
            MsgMgr.Message( MSG_SENSOR_DEPLOY_BAD_SURFACE, PlayerID.Slot );
        return( FALSE );
    }

    //
    // See if surface normal is within 30 degrees of vertical.
    //
    if( Normal.Y < 0.8660f )
    {
        if( !TestOnly )
            MsgMgr.Message( MSG_SENSOR_DEPLOY_TOO_STEEP, PlayerID.Slot );
        return( FALSE );
    }

#ifndef T2_DESIGNER_BUILD

    //
    // See if deployment is blocked by maximum count.
    //
    GetDeployStats( object::TYPE_SENSOR_REMOTE, TeamBits, Current, Maximum );
    if( Current >= Maximum )
    {
        if( !TestOnly )
            MsgMgr.Message( MSG_SENSOR_DEPLOY_LIMIT_REACHED, PlayerID.Slot );
        return( FALSE );
    }

    //
    // Radius check.
    //
    ObjMgr.Select( object::ATTR_ASSET, Point, 0.25f );    // TO DO: Get radius for this.
    while( (pObject = ObjMgr.GetNextSelected()) )
    {
        if( pObject->GetType() == object::TYPE_SENSOR_REMOTE )
        {
            if( !TestOnly )
                MsgMgr.Message( MSG_SENSOR_DEPLOY_TOO_CLOSE, PlayerID.Slot );
            break;
        }
    }
    ObjMgr.ClearSelection();

    if( pObject )
        return( FALSE );
#endif

    //
    // Check for enough clearance to deploy sensor.
    //
#ifndef T2_DESIGNER_BUILD
    if( !TestOnly )
    {
#else
		// Designer build:
		// Test the ground still exists.
		{
			collider Ray;
			vector3 A = Normal;
			A.Scale(0.01f);
			vector3 B = Point - A;
			A = Point + A;
			Ray.RaySetup( NULL, A, B);
			ObjMgr.Collide( object::ATTR_SOLID_STATIC, Ray );
			if( !Ray.HasCollided() )
				return (FALSE);
		}
#endif
        if( !IsEnoughClearance( object::TYPE_SENSOR_REMOTE, Point, Normal ))
        {
            if( !TestOnly )
                MsgMgr.Message( MSG_SENSOR_DEPLOY_NO_ROOM, PlayerID.Slot );
            //x_DebugMsg( "Not enough room to deploy sensor\n" );
            return( FALSE );
        }
#ifndef T2_DESIGNER_BUILD
    }
#endif

    //
    // Make sure we don't block deployed inventory stations.
    //
    if( IsNearDeployedInven( Point ) == TRUE )
    {
        if( !TestOnly )
            MsgMgr.Message( MSG_DEPLOY_NEAR_REMOTE_STATION, PlayerID.Slot );
        //x_DebugMsg( "Point is too close to deployed inven\n" );
        return( FALSE );
    }

    //
    // If we made it here, then we can go ahead and make the sensor.
    //
    if( !TestOnly )
    {
        sensor* pSensor = (sensor*)ObjMgr.CreateObject( object::TYPE_SENSOR_REMOTE );
        pSensor->Initialize( Point, Normal, TeamBits );
        ObjMgr.AddObject( pSensor, obj_mgr::AVAILABLE_SERVER_SLOT );

        if( m_GameType != GAME_CAMPAIGN )
        {
            if( m_Score.IsTeamBased )
            {
                ScoreForPlayer( PlayerID, SCORE_DEPLOY_TEAM_DEP_SENSOR );
                MsgMgr.Message( MSG_SENSOR_DEPLOYED, PlayerID.Slot, ARG_N, Current+1, ARG_M, Maximum );
            }
            else
            {
                MsgMgr.Message( MSG_N_OF_M_SENSORS_DEPLOYED, PlayerID.Slot, ARG_N, Current+1, ARG_M, Maximum );
            }
        }
    }
    return( TRUE );
}

//==============================================================================

xbool game_mgr::AttemptDeployBeacon(       object::id    PlayerID,   
                                     const vector3&      Point,      
                                     const vector3&      Normal,     
                                           object::type  SurfaceType,
                                           xbool         TestOnly )  
{
    s32             Current;
    s32             Maximum;
    u32             TeamBits;
    player_object*  pPlayer;
    object*         pObject;

    if( !tgl.ServerPresent )
        return( FALSE );

    pPlayer = (player_object*)ObjMgr.GetObjectFromID( PlayerID );
    if( !pPlayer )
        return( FALSE );

    TeamBits = pPlayer->GetTeamBits();

    // 
    // Special case: Toggle beacon mode on existing beacon on same team.
    //
    if( SurfaceType == object::TYPE_BEACON )
    {
        ObjMgr.Select( object::ATTR_BEACON, Point, 0.25f );
        pObject = ObjMgr.GetNextSelected();
        ObjMgr.ClearSelection();
        if( pObject )
        {
            ASSERT( pObject->GetType() == object::TYPE_BEACON );

            beacon* pBeacon = (beacon*)pObject;

            if( TeamBits == pBeacon->GetTeamBits() )
            {
                pBeacon->ToggleMode();
            }
            else
            {
                // TO DO: Message: Cannot toggle mode of enemy beacon.
            }
            return( FALSE );
        }
    }

    //
    // See if surface is an acceptable type.
    //
    if( (SurfaceType != object::TYPE_TERRAIN) && 
        (SurfaceType != object::TYPE_BUILDING) && 
        (SurfaceType != object::TYPE_SCENIC) )
    {
        // TO DO: Message: Invalid surface.
        return( FALSE );
    }

    //
    // See if deployment is blocked by maximum count.
    //
    GetDeployStats( object::TYPE_BEACON, TeamBits, Current, Maximum );
    if( Current >= Maximum )
    {
        // TO DO: Message: Capacity reached.
        return( FALSE );
    }

    //
    // Radius check.
    //
    ObjMgr.Select( object::ATTR_BEACON, Point, 0.5f );
    pObject = ObjMgr.GetNextSelected();
    ObjMgr.ClearSelection();
    if( pObject )
    {
        ASSERT( pObject->GetType() == object::TYPE_BEACON );
        // TO DO: Message: Too close to another beacon.
        return( FALSE );
    }

    //
    // TO DO: Need to do clearance check.
    //          Make sure the "pain point" is exposed.
    //      

    //
    // If we made it here, then we can go ahead and make the beacon.
    //
    if( !TestOnly )
    {
        beacon* pBeacon = (beacon*)ObjMgr.CreateObject( object::TYPE_BEACON );
        pBeacon->Initialize( Point, Normal, TeamBits );
        ObjMgr.AddObject( pBeacon, obj_mgr::AVAILABLE_SERVER_SLOT );
    }
    return( TRUE );
}

//==============================================================================

xbool game_mgr::AttemptDeployBarrel(       object::id    PlayerID,   
                                     const vector3&      Point,      
                                     const vector3&      Ray,     
                                           turret::kind  Kind )
{
    collider            Collider;
    collider::collision Collision;
    u32                 TeamBits;
    player_object*      pPlayer;
    object*             pObject;

    if( !tgl.ServerPresent )
        return( FALSE );

    pPlayer = (player_object*)ObjMgr.GetObjectFromID( PlayerID );
    if( !pPlayer )
        return( FALSE );

    TeamBits = pPlayer->GetTeamBits();

    //
    // See if there is a fixed turret there.
    //
    Collider.RaySetup( NULL, Point, Point + Ray, PlayerID.GetAsS32() );
    ObjMgr.Collide( object::ATTR_SOLID, Collider );
    if( !Collider.HasCollided() )
    {
        MsgMgr.Message( MSG_BARREL_DEPLOY_NO_TURRET, PlayerID.Slot );
        return( FALSE );
    }
    Collider.GetCollision( Collision );
    pObject = (object*)Collision.pObjectHit;
    if( pObject->GetType() != object::TYPE_TURRET_FIXED )
    {
        MsgMgr.Message( MSG_BARREL_DEPLOY_NO_TURRET, PlayerID.Slot );
        return( FALSE );
    }

    //
    // See if the turret is on the same team.
    //
    if( (pObject->GetTeamBits() & TeamBits) == 0x00 )
    {
        MsgMgr.Message( MSG_BARREL_DEPLOY_WRONG_TEAM, PlayerID.Slot );
        return( FALSE );
    }

    //
    // Attempt to change the barrel.
    //
    if( ((turret*)pObject)->ChangeBarrel( Kind ) )
    {
        pGameLogic->ItemDeployed( pObject->GetObjectID(), PlayerID );
        MsgMgr.Message( MSG_BARREL_DEPLOYED, PlayerID.Slot );
        return( TRUE );
    }
    else
    {
        MsgMgr.Message( MSG_BARREL_DEPLOY_TURRET_DOWN, PlayerID.Slot );
        return( FALSE );
    }
}

//==============================================================================

void game_mgr::DebugRender( void )
{
    if( !m_DebugRender )
        return;

    if( m_VoteInProgress )
    {
        xstring Message( m_VoteMessage );
        x_printfxy( 0, 0, "%s", (const char*)Message );
        x_printfxy( 0, 1, "%02d:For",     m_VotesFor     );
        x_printfxy( 0, 2, "%02d:Against", m_VotesAgainst );
        x_printfxy( 0, 3, "%02d:Pending", m_VotesMissing );
        x_printfxy( 0, 4, "%02d:%Needed", m_VotePercent  );
        x_printfxy( 0, 5, "%02d:Time",    (s32)m_VoteTimer );
    }
}

//==============================================================================

void game_mgr::ProvideFinalScore( bitstream& BitStream )
{
    s32 i;

    for( i = 0; i < 32; i++ )
    {
        if( BitStream.WriteFlag( (m_FinalScore.Player[i].IsConnected) &&
                                 (m_FinalScore.Player[i].IsInGame) ) )
        {
            BitStream.WriteS32( m_FinalScore.Player[i].Score  );
            BitStream.WriteS32( m_FinalScore.Player[i].Kills  );
            BitStream.WriteS32( m_FinalScore.Player[i].Deaths );
        }
    }

    if( BitStream.WriteFlag( m_FinalScore.IsTeamBased ) )
    {
        BitStream.WriteS32( m_FinalScore.Team[0].Score );
        BitStream.WriteS32( m_FinalScore.Team[1].Score );
    }

    ASSERT( !BitStream.IsFull() );
}

//==============================================================================

void game_mgr::AcceptFinalScore( bitstream& BitStream )
{
    s32 i;

    for( i = 0; i < 32; i++ )
    {
        if( BitStream.ReadFlag() )
        {
            BitStream.ReadS32( m_FinalScore.Player[i].Score  );
            BitStream.ReadS32( m_FinalScore.Player[i].Kills  );
            BitStream.ReadS32( m_FinalScore.Player[i].Deaths );
        }
    }

    if( BitStream.ReadFlag() )
    {
        BitStream.ReadS32( m_FinalScore.Team[0].Score );
        BitStream.ReadS32( m_FinalScore.Team[1].Score );
    }
}

//==============================================================================


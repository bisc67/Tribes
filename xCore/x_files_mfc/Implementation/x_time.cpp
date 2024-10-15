//==============================================================================
//  
//  x_time.cpp
//
//  TODO - Make nice and inline happy for performance.
//
//==============================================================================

#ifndef X_PLUS_HPP
#include "..\x_time.hpp"
#endif

#ifndef X_DEBUG_HPP
#include "..\x_debug.hpp"
#endif

#ifdef TARGET_PS2
#include "eeregs.h"
#include "eekernel.h"
#endif

//==============================================================================
//  PLATFORM SPECIFIC CODE
//==============================================================================
//
//  Must provide the following:
//    - Functions x_TimeInit(), x_TimeKill(), and x_GetTime().
//    - Definition for XTICKS_PER_MS which is one millisecond in xticks.
//  
//==============================================================================
//******************************************************************************
#ifdef TARGET_PS2
//==============================================================================
// This is defined as BUSCLK / 256 / 1000 (BUSCLK == 147.456Mhz)
#define XTICKS_PER_MS   576

static volatile xtick s_TimerWrapCount;
static s32 s_HandlerId;

//==============================================================================
static s32 s_TimerWrapCallback(s32)
{
    // Overflow flag MUST have been set
    ASSERT(DGET_T0_MODE() & (1<<11) );
    // Make sure timer will fire off an interrupt when it wraps again
    DPUT_T0_MODE( DGET_T0_MODE());
    s_TimerWrapCount++;
    ExitHandler();
    return 0;
}

//==============================================================================
void x_TimeInit( void )
{
    s_HandlerId = AddIntcHandler(INTC_TIM0,s_TimerWrapCallback,0);
    ASSERT(s_HandlerId >= 0);
    s_TimerWrapCount=0;
    DPUT_T0_COUNT(0);
    DPUT_T0_MODE(   (2<<0)|         // Clock is BUSCLK/256
                    (1<<7)|         // Continue counting
                    (1<<9)          // Overflow will generate an interrupt
                    );
    EnableIntc(INTC_TIM0);
}

//==============================================================================
void x_TimeKill( void )
{    
    DisableIntc(INTC_TIM0);
    RemoveIntcHandler(INTC_TIM0,s_HandlerId);
}

//==============================================================================
xtick x_GetTime( void )
{
    xtick Ret;
    xtick start;
    u16   time;

    
    start = s_TimerWrapCount;

    time = *T0_COUNT;
    // If an interrupt occured between the wrap read and the hw timer read,
    // then we reload everything.
    if (start!= s_TimerWrapCount)
    {
        start = s_TimerWrapCount;
        time = *T0_COUNT;
    }

    Ret = (start << 16) | time;

    return( Ret );
}

//==============================================================================
#endif // TARGET_PS2
//******************************************************************************

//******************************************************************************
#ifdef TARGET_PC
//==============================================================================

#include <windows.h>

#define XTICKS_PER_MS   s_PCFrequency

static f64 s_PCFrequency;

//==============================================================================

void x_TimeInit( void )
{    
    LARGE_INTEGER F;
    QueryPerformanceFrequency( &F );
    s_PCFrequency = (f64)F.QuadPart / 1000.0;
}

//==============================================================================

void x_TimeKill( void )
{    
}

//==============================================================================

xtick x_GetTime( void )
{
    static xtick LastTicks = 0;
    
    xtick Ticks;
    LARGE_INTEGER C;

    QueryPerformanceCounter( &C );
    Ticks = (s64)C.QuadPart;

    if( Ticks < LastTicks )     
        Ticks = LastTicks + 1;

    LastTicks = Ticks;

    return( Ticks );
}

//==============================================================================

//==============================================================================
#endif // TARGET_PC
//******************************************************************************


//******************************************************************************
#ifdef TARGET_GCN
//==============================================================================

#include <dolphin/os.h>

#define XTICKS_PER_MS   (OS_TIMER_CLOCK / 1000)

//==============================================================================

void x_TimeInit( void )
{    

}

//==============================================================================

void x_TimeKill( void )
{    
}

//==============================================================================

xtick x_GetTime( void )
{
    static xtick LastTicks = 0;
    
    xtick Ticks;

    OSTime  T = OSGetTime();
    
    Ticks = (s64)OSTicksToMilliseconds( T );

    if( Ticks < LastTicks )     
        Ticks = LastTicks + 1;

    LastTicks = Ticks;

    return( Ticks );
}

//==============================================================================

//==============================================================================
#endif // TARGET_GCN
//******************************************************************************



//==============================================================================
//  PLATFORM INDEPENDENT CODE
//==============================================================================

#define ONE_DAY  ((s64)XTICKS_PER_MS * 1000 * 60 * 60 * 24)

//==============================================================================

s64 x_GetTicksPerMs ( void )
{
    return( (s64)XTICKS_PER_MS );
}

//==============================================================================

f32 x_TicksToMs( xtick Ticks )
{
    ASSERT( Ticks < ONE_DAY );
    /*
    ** This really helps on the PS2.  See if we still need it.
    **
    if( Ticks < 0x7FFFFFFF )
        return( (f32)((s32)Ticks) / (f32)XTICKS_PER_MS );
    else
    */
    //
    // We do the multiple casting here to try and preserve as much accuracy as possible
    //
        return( (f32)(     Ticks) / (f32)XTICKS_PER_MS );
}

//==============================================================================

f64 x_TicksToSec( xtick Ticks )
{
    return( ((f64)Ticks) / ((f64)(XTICKS_PER_MS * 1000)) );
}

//==============================================================================

f64 x_GetTimeSec( void )
{
    return( x_TicksToSec( x_GetTime() ) );
}

//==============================================================================

xtimer::xtimer( void )
{
    m_Running   = FALSE;
    m_StartTime = 0;
    m_TotalTime = 0;
    m_NSamples  = 0;
}

//==============================================================================

void xtimer::Start( void )
{
    if( !m_Running )
    {
        m_StartTime = x_GetTime();
        m_Running   = TRUE;        
        m_NSamples++;
    }
}

//==============================================================================

void xtimer::Reset( void )
{
    m_Running   = FALSE;
    m_StartTime = 0;
    m_TotalTime = 0;
    m_NSamples  = 0;
}

//==============================================================================

xtick xtimer::Stop( void )
{
    if( m_Running )
    {
        m_TotalTime += x_GetTime() - m_StartTime;
        m_Running    = FALSE;
    }
    return( m_TotalTime );
}

//==============================================================================

f32 xtimer::StopMs( void )
{
    if( m_Running )
    {
        m_TotalTime += x_GetTime() - m_StartTime;
        m_Running    = FALSE;
    }
    ASSERT( m_TotalTime < ONE_DAY );
    return( (f32)(m_TotalTime) / (f32)XTICKS_PER_MS );
}

//==============================================================================

f32 xtimer::StopSec( void )
{
    if( m_Running )
    {
        m_TotalTime += x_GetTime() - m_StartTime;
        m_Running    = FALSE;
    }
    ASSERT( m_TotalTime < ONE_DAY );
    return( (f32)(m_TotalTime) / ((f32)XTICKS_PER_MS * 1000.0f) );
}

//==============================================================================

xtick xtimer::Read( void ) const
{
    if( m_Running )  return( m_TotalTime + (x_GetTime() - m_StartTime) );
    else             return( m_TotalTime );
}

//==============================================================================

f32 xtimer::ReadMs( void ) const
{
    xtick Ticks;
    if( m_Running )  Ticks = m_TotalTime + (x_GetTime() - m_StartTime);
    else             Ticks = m_TotalTime;
    ASSERT( Ticks < ONE_DAY );
    f32 MS = (f32)(Ticks) / (f32)XTICKS_PER_MS ;
    return( MS ) ;
}

//==============================================================================

f32 xtimer::ReadSec( void ) const
{
    xtick Ticks;
    if( m_Running )  Ticks = m_TotalTime + (x_GetTime() - m_StartTime);
    else             Ticks = m_TotalTime;
    ASSERT( Ticks < ONE_DAY );
    f32 S = (f32)(Ticks) / ((f32)XTICKS_PER_MS * 1000.0f) ;
    return( S );
}

//==============================================================================

xtick xtimer::Trip( void )
{
    xtick Ticks = 0;
    if( m_Running )
    {
        xtick Now = x_GetTime();
        Ticks = m_TotalTime + (Now - m_StartTime);
        m_TotalTime = 0;
        m_StartTime = Now;
        m_NSamples++;
    }
    else
    {
        Start();
    }
    return( Ticks );
}

//==============================================================================

f32 xtimer::TripMs( void )
{
    xtick Ticks = 0;
    if( m_Running )
    {
        xtick Now = x_GetTime();
        Ticks = m_TotalTime + (Now - m_StartTime);
        m_TotalTime = 0;
        m_StartTime = Now;
        m_NSamples++;
    }
    else
    {
        Start();
    }
    ASSERT( Ticks < ONE_DAY );
    return( (f32)(Ticks) / (f32)XTICKS_PER_MS );
}

//==============================================================================

f32 xtimer::TripSec( void )
{
    xtick Ticks = 0;
    if( m_Running )
    {
        xtick Now = x_GetTime();
        Ticks = m_TotalTime + (Now - m_StartTime);
        m_TotalTime = 0;
        m_StartTime = Now;
        m_NSamples++;
    }
    else
    {
        Start();
    }
    ASSERT( Ticks < ONE_DAY );
    return( (f32)(Ticks) / ((f32)XTICKS_PER_MS * 1000.0f) );
}

//==============================================================================

s32 xtimer::GetNSamples( void ) const
{
    return( m_NSamples );
}

//==============================================================================

f32 xtimer::GetAverageMs( void ) const
{
    if( m_NSamples <= 0 ) 
        return( 0 );

    return( ReadMs() / m_NSamples );
}

//==============================================================================

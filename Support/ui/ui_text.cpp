//=========================================================================
//
//  ui_text.cpp
//
//=========================================================================

#include "entropy.hpp"
#include "ui_text.hpp"
#include "ui_manager.hpp"
#include "ui_font.hpp"

//=========================================================================
//  Defines
//=========================================================================

//=========================================================================
//  Structs
//=========================================================================

//=========================================================================
//  Data
//=========================================================================

//=========================================================================
//  Factory function
//=========================================================================

ui_win* ui_text_factory( s32 UserID, ui_manager* pManager, const irect& Position, ui_win* pParent, s32 Flags )
{
    ui_text* pButton = new ui_text;
    pButton->Create( UserID, pManager, Position, pParent, Flags );

    return (ui_win*)pButton;
}

//=========================================================================
//  ui_text
//=========================================================================

ui_text::ui_text( void )
{
    m_LabelColor = XCOLOR_WHITE;
}

//=========================================================================

ui_text::~ui_text( void )
{
    Destroy();
}

//=========================================================================

xbool ui_text::Create( s32 UserID, ui_manager* pManager, const irect& Position, ui_win* pParent, s32 Flags )
{
    xbool   Success;

    Success = ui_control::Create( UserID, pManager, Position, pParent, Flags );

    // Initialize Data

    return Success;
}

//=========================================================================

void ui_text::Render( s32 ox, s32 oy )
{
    s32     State = ui_manager::CS_NORMAL;

    // Only render is visible
    if( m_Flags & WF_VISIBLE )
    {
        xcolor  TextColor1 = XCOLOR_WHITE;
        xcolor  TextColor2 = XCOLOR_BLACK;

        // Calculate rectangle
        irect    r;
        r.Set( (m_Position.l+ox), (m_Position.t+oy), (m_Position.r+ox), (m_Position.b+oy) );

        // Render appropriate state
        if( m_Flags & WF_DISABLED )
        {
            State = ui_manager::CS_DISABLED;
            TextColor1 = XCOLOR_GREY;
            TextColor2 = xcolor(0,0,0,0);
        }
        else if( (m_Flags & (WF_HIGHLIGHT|WF_SELECTED)) == WF_HIGHLIGHT )
        {
            State = ui_manager::CS_HIGHLIGHT;
            TextColor1 = GetLabelColor();
            TextColor2 = XCOLOR_BLACK;
        }
        else if( (m_Flags & (WF_HIGHLIGHT|WF_SELECTED)) == WF_SELECTED )
        {
            State = ui_manager::CS_SELECTED;
            TextColor1 = GetLabelColor();
            TextColor2 = XCOLOR_BLACK;
        }
        else if( (m_Flags & (WF_HIGHLIGHT|WF_SELECTED)) == (WF_HIGHLIGHT|WF_SELECTED) )
        {
            State = ui_manager::CS_HIGHLIGHT_SELECTED;
            TextColor1 = GetLabelColor();
            TextColor2 = XCOLOR_BLACK;
        }
        else
        {
            State = ui_manager::CS_NORMAL;
            TextColor1 = GetLabelColor();
            TextColor2 = XCOLOR_BLACK;
        }

        // Render Text
        r.Translate( 1, -1 );
        m_pManager->RenderText( m_Font, r, m_LabelFlags, TextColor2, m_Label );
        r.Translate( -1, -1 );
        m_pManager->RenderText( m_Font, r, m_LabelFlags, TextColor1, m_Label );

        // Render children
        for( s32 i=0 ; i<m_Children.GetCount() ; i++ )
        {
            m_Children[i]->Render( m_Position.l+ox, m_Position.t+oy );
        }
    }
}

//=========================================================================

void ui_text::OnUpdate( f32 DeltaTime )
{
    (void)DeltaTime;
}

//=========================================================================

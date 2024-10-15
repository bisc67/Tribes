//=========================================================================
//
//  ui_dialog.cpp
//
//=========================================================================

#include "entropy.hpp"
#include "ui_dialog.hpp"
#include "ui_manager.hpp"
#include "ui_control.hpp"
#include "ui_edit.hpp"
#include "ui_font.hpp"
#include "..\StringMgr\StringMgr.hpp"

#include "..\..\demo1\fe_colors.hpp"

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

ui_win* ui_dialog_factory( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData )
{
    ui_dialog* pDialog = new ui_dialog;
    pDialog->Create( UserID, pManager, pDialogTem, Position, pParent, Flags, pUserData );

    return (ui_win*)pDialog;
}

//=========================================================================
//  ui_dialog
//=========================================================================

ui_dialog::ui_dialog( void )
{
}

//=========================================================================

ui_dialog::~ui_dialog( void )
{
    Destroy();
}

//=========================================================================

xbool ui_dialog::Create( s32                        UserID,
                         ui_manager*                pManager,
                         ui_manager::dialog_tem*    pDialogTem,
                         const irect&               Position,
                         ui_win*                    pParent,
                         s32                        Flags,
                         void*                      pUserData)
{
    xbool   Success = FALSE;
    s32     i;
    s32     x, y;

    ASSERT( pManager );

    // Get pointer to user
    ui_manager::user* pUser = pManager->GetUser( UserID );
    ASSERT( pUser );

    // Do window creation
    Success = ui_win::Create( UserID, pManager, Position, pParent, Flags );

    // Setup Dialog specific stuff
    m_iElement          = m_pManager->FindElement( "frame" );
    ASSERT( m_iElement != -1 );
    m_pDialogTem        = pDialogTem;
    m_NavW              = pDialogTem ? pDialogTem->NavW : 0;
    m_NavH              = pDialogTem ? pDialogTem->NavH : 0;
    m_NavX              = 0;
    m_NavY              = 0;
//    m_BackgroundColor   = xcolor(40,80,80,224);
//    m_BackgroundColor   = xcolor(20,40,40,224);
//    m_BackgroundColor   = xcolor(20,30,40,224);
    m_BackgroundColor   = FECOL_DIALOG;
    m_pUserData         = pUserData;

    if( pDialogTem )
        m_Label         = StringMgr( "ui", pDialogTem->StringID );
    m_OldCursorX        = pUser->CursorX;
    m_OldCursorY        = pUser->CursorY;

    // Setup Navgraph size
    m_NavGraph.SetCapacity( m_NavW * m_NavH );
    for( i=0 ; i<(m_NavW*m_NavH) ; i++ )
        m_NavGraph.Append() = 0;

    // Save Template Pointer
    m_pDialogTem = pDialogTem;

    // Create controls and add to navigation graph if template exists
    if( pDialogTem )
    {
        for( i=0 ; i<pDialogTem->nControls ; i++ )
        {
            ui_manager::control_tem*    pControlTem = &pDialogTem->pControls[i];

            // For now create each control
            irect Pos;
            Pos.Set( pControlTem->x, pControlTem->y, pControlTem->x + pControlTem->w, pControlTem->y + pControlTem->h );
            ui_control* pControl = (ui_control*)pManager->CreateWin( UserID, pControlTem->pClass, Pos, this, pControlTem->Flags );
            ASSERT( pControl );

            // Assign control ID
            pControl->SetControlID( pControlTem->ControlID );

            // Configure the control
            pControl->SetLabel( StringMgr( "ui", pControlTem->StringID ) );
            if( x_strcmp( pControlTem->pClass, "edit" ) == 0 )
            {
                ui_edit*    pEdit = (ui_edit*)pControl;
                pEdit->SetVirtualKeyboardTitle( StringMgr( "ui", pControlTem->StringID ) );
            }

            // Add the control to the navigation graph
            ASSERT( pControlTem->nx >= 0 );
            ASSERT( pControlTem->ny >= 0 );
            ASSERT( (pControlTem->nx + pControlTem->nw) <= m_NavW );
            ASSERT( (pControlTem->ny + pControlTem->nh) <= m_NavH );

            // Save navgrid position
            pControl->SetNavPos( irect( pControlTem->nx, pControlTem->ny, pControlTem->nx+pControlTem->nw, pControlTem->ny+pControlTem->nh ) );

            // Add control into navgrid
            for( y=pControlTem->ny ; y<(pControlTem->ny+pControlTem->nh) ; y++ )
            {
                for( x=pControlTem->nx ; x<(pControlTem->nx+pControlTem->nw) ; x++ )
                {
                    m_NavGraph[x+y*m_NavW] = pControl;
                }
            }
        }
    }

    // Return success code
    return Success;
}

//=========================================================================

void ui_dialog::Render( s32 ox, s32 oy )
{
#ifdef TARGET_PC
    // If this is not a TAB dialog page
/*    if( !(GetFlags() & ui_win::WF_TAB) )
    {
        // Adjust the position of the dialogs according to the resolution.
        s32 XRes, YRes;
        eng_GetRes( XRes, YRes );
        s32 midX = XRes>>1;
        s32 midY = YRes>>1;

        s32 dx = midX - 256;
        s32 dy = midY - 256;
        ox = dx;
        oy = dy;
    }
*/
#endif

    // Only render is visible
    if( m_Flags & WF_VISIBLE )
    {
        // Get window rectangle
        irect   r;
        r.Set( m_Position.l+ox, m_Position.t+oy, m_Position.r+ox, m_Position.b+oy );
        irect   rb = r;
        rb.Deflate( 1, 1 );

        // Render frame
        if( m_Flags & WF_BORDER )
        {
            // Render background color
            if( m_BackgroundColor.A > 0 )
            {
                m_pManager->RenderRect( rb, m_BackgroundColor, FALSE );
            }

            // Render Title Bar Gradient
            rb.SetHeight( 40 );
            xcolor c1 = FECOL_TITLE1;
            xcolor c2 = FECOL_TITLE2;
            m_pManager->RenderGouraudRect( rb, c1, c1, c2, c2, FALSE );

            // Render the Frame
            m_pManager->RenderElement( m_iElement, r, 0 );

            // Render Title
            rb.Deflate( 16, 0 );
            m_pManager->RenderText( 2, rb, ui_font::v_center, XCOLOR_BLACK, m_Label );
            rb.Translate( -1, -1 );
            m_pManager->RenderText( 2, rb, ui_font::v_center, XCOLOR_WHITE, m_Label );
        }

        // Render children
        for( s32 i=0 ; i<m_Children.GetCount() ; i++ )
        {
            m_Children[i]->Render( m_Position.l+ox, m_Position.t+oy );
        }
    }
}

//=========================================================================

const irect& ui_dialog::GetCreatePosition( void ) const
{
    return m_CreatePosition;
}

//=========================================================================
//=========================================================================
//  Message Handler Functions
//=========================================================================
//=========================================================================

void ui_dialog::OnPadNavigate( ui_win* pWin, s32 Code, s32 Presses, s32 Repeats )
{
    (void)pWin;
    (void)Presses;
    (void)Repeats;

    ui_manager::user*   pUser   = m_pManager->GetUser( m_UserID );
    s32                 x       = m_NavX;
    s32                 y       = m_NavY;
    s32                 dx      = 0;
    s32                 dy      = 0;

    // Which way are we moving
    switch( Code )
    {
    case ui_manager::NAV_UP:
        dy = -1;
        break;
    case ui_manager::NAV_DOWN:
        dy = 1;
        break;
    case ui_manager::NAV_LEFT:
        dx = -1;
        break;
    case ui_manager::NAV_RIGHT:
        dx = 1;
        break;
    }

/*
    x += dx;
    y += dy;
    if( x <       0 ) x = m_NavW-1;
    if( x >= m_NavW ) x = 0;
    if( y <       0 ) y = m_NavH-1;
    if( y >= m_NavH ) y = 0;
*/

    // Scan until the control to move to is found
    ui_win* pCurrentWin = pUser->pLastWindowUnderCursor;
//    while( ((x != m_NavX) && (dx != 0)) || ((y != m_NavY) && (dy != 0)) )
    while( ((x < m_NavW) && (x >= 0)) && ((y < m_NavH) && (y >= 0)) )
    {
        ui_win* pWin = m_NavGraph[x+y*m_NavW];
        if( pWin && (pWin != pCurrentWin) && !(pWin->GetFlags() & ui_win::WF_DISABLED) )
        {
            // We have found a control to move, set Mouse Cursor to center of that control
            // and update the navigation xy position
//            xstring String = pWin->GetText();
            irect r = pWin->GetPosition();
            s32 cx = (r.r - r.l)/2;
            s32 cy = (r.b - r.t)/2;
            pWin->LocalToScreen( cx, cy );
            m_pManager->SetCursorPos( m_UserID, cx, cy );
            m_NavX = x;
            m_NavY = y;
            break;
        }

        // Advance to next position
        x += dx;
        y += dy;
/*
        if( x <       0 ) x = m_NavW-1;
        if( x >= m_NavW ) x = 0;
        if( y <       0 ) y = m_NavH-1;
        if( y >= m_NavH ) y = 0;
*/
    }
}

//=========================================================================

void ui_dialog::SetBackgroundColor( xcolor Color )
{
    m_BackgroundColor = Color;
}

//=========================================================================

xcolor ui_dialog::GetBackgroundColor( void ) const
{
    return m_BackgroundColor;
}

//=========================================================================

xbool ui_dialog::GotoControl( s32 iControl )
{
    xbool   Success = FALSE;

    ASSERT( (iControl >= 0) && (iControl < m_Children.GetCount()) );

    ui_control* pChild = (ui_control*)m_Children[iControl];

    // Only goto non-static & enabled controls
    if( (!(pChild->GetFlags() & ui_win::WF_STATIC  )) &&
        (!(pChild->GetFlags() & ui_win::WF_DISABLED)) )
    {
        s32 x = pChild->GetWidth() / 2;
        s32 y = pChild->GetHeight() / 2;

        // Position cursor over Child
        pChild->LocalToScreen( x, y );
        m_pManager->SetCursorPos( m_UserID, x, y );

        // Set Navigation cursor to center of the control
        const irect& r = pChild->GetNavPos( );
        m_NavX = r.l + r.GetWidth() / 2;
        m_NavY = r.t + r.GetHeight() / 2;

        Success = TRUE;
    }
    return Success;
}

//=========================================================================

xbool ui_dialog::GotoControl( ui_control* pControl )
{
    xbool   Success = FALSE;

    ui_control* pChild = NULL;

    // Locate the control
    for( s32 i=0 ; i<m_Children.GetCount() ; i++ )
    {
        if( m_Children[i] == pControl )
            pChild = (ui_control*)m_Children[i];
    }
    ASSERT( pChild );

    // Only goto non-static & enabled controls
    if( (!(pChild->GetFlags() & ui_win::WF_STATIC  )) &&
        (!(pChild->GetFlags() & ui_win::WF_DISABLED)) )
    {
        s32 x = pChild->GetWidth() / 2;
        s32 y = pChild->GetHeight() / 2;

        // Position cursor over Child
        pChild->LocalToScreen( x, y );
        m_pManager->SetCursorPos( m_UserID, x, y );

        // Set Navigation cursor to center of the control
        const irect& r = pChild->GetNavPos( );
        m_NavX = r.l + r.GetWidth() / 2;
        m_NavY = r.t + r.GetHeight() / 2;

        Success = TRUE;
    }

    return Success;
}

//=========================================================================

s32 ui_dialog::GetNumControls( void ) const
{
    return m_Children.GetCount();
}

//=========================================================================

ui_manager::dialog_tem* ui_dialog::GetTemplate( void )
{
    return m_pDialogTem;
}

//=========================================================================

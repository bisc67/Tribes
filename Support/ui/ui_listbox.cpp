//=========================================================================
//
//  ui_listbox.cpp
//
//=========================================================================

#include "entropy.hpp"
#include "..\AudioMgr\audio.hpp"
#include "..\LabelSets\Tribes2Types.hpp"

#include "ui_listbox.hpp"
#include "ui_manager.hpp"
#include "ui_font.hpp"
//#include "ui_dlg_combolist.hpp"

//=========================================================================
//  Defines
//=========================================================================

#define SPACE_TOP       4
#define SPACE_BOTTOM    4
#define LINE_HEIGHT     16
#define TEXT_OFFSET     -2

//=========================================================================
//  Structs
//=========================================================================

//=========================================================================
//  Data
//=========================================================================

//=========================================================================
//  Factory function
//=========================================================================

ui_win* ui_listbox_factory( s32 UserID, ui_manager* pManager, const irect& Position, ui_win* pParent, s32 Flags )
{
    ui_listbox* pcombo = new ui_listbox;
    pcombo->Create( UserID, pManager, Position, pParent, Flags );

    return (ui_win*)pcombo;
}

//=========================================================================
//  ui_listbox
//=========================================================================

ui_listbox::ui_listbox( void )
{
}

//=========================================================================

ui_listbox::~ui_listbox( void )
{
    Destroy();
}

//=========================================================================

xbool ui_listbox::Create( s32 UserID, ui_manager* pManager, const irect& Position, ui_win* pParent, s32 Flags )
{
    xbool   Success;

    Success = ui_control::Create( UserID, pManager, Position, pParent, Flags );

    // Set default text flags
    m_LabelFlags = ui_font::h_center|ui_font::v_center|ui_font::clip_ellipsis|ui_font::clip_l_justify;
    m_BackgroundColor = xcolor(0,0,0,0);

    // Initialize data
    m_iElementFrame         = m_pManager->FindElement( "frame" );
    m_iElement_sb_arrowdown = m_pManager->FindElement( "sb_arrowdown" );
    m_iElement_sb_arrowup   = m_pManager->FindElement( "sb_arrowup" );
    m_iElement_sb_container = m_pManager->FindElement( "sb_container" );
    m_iElement_sb_thumb     = m_pManager->FindElement( "sb_thumb" );
    ASSERT( m_iElementFrame != -1 );
    ASSERT( m_iElement_sb_arrowdown != -1 );
    ASSERT( m_iElement_sb_arrowup   != -1 );
    ASSERT( m_iElement_sb_container != -1 );
    ASSERT( m_iElement_sb_thumb     != -1 );

    m_ExitOnSelect      = TRUE;
    m_iSelection        = -1;
    m_iSelectionBackup  = -1;
    m_iFirstVisibleItem = 0;
    m_ShowBorders       = TRUE;
    m_nVisibleItems     = (m_Position.GetHeight()-SPACE_TOP-SPACE_BOTTOM) / LINE_HEIGHT;

#ifdef TARGET_PC
    m_MouseDown = FALSE;
    m_ScrollDown = FALSE;
    m_ScrollTime = 0.0f;
#endif

    return Success;
}

//=========================================================================

void ui_listbox::Render( s32 ox, s32 oy )
{
    s32     i;

    // Only render is visible
    if( m_Flags & WF_VISIBLE )
    {
        xcolor  TextColor1  = XCOLOR_WHITE;
        xcolor  TextColor2  = XCOLOR_BLACK;
        s32     State       = ui_manager::CS_NORMAL;

        // Calculate rectangle
        irect   br;
        irect   r;
        irect   r2;
        br.Set( (m_Position.l+ox), (m_Position.t+oy), (m_Position.r+ox), (m_Position.b+oy) );
        r = br;
        r2 = r;
        r.r -= 19;
        r2.l = r.r;

        // Render appropriate state
        if( m_Flags & WF_DISABLED )
        {
            State = ui_manager::CS_DISABLED;
            TextColor1  = XCOLOR_GREY;
            TextColor2  = xcolor(0,0,0,0);
        }
        else if( (m_Flags & (WF_HIGHLIGHT|WF_SELECTED)) == WF_HIGHLIGHT )
        {
            State = ui_manager::CS_HIGHLIGHT;
            TextColor1  = XCOLOR_WHITE;
            TextColor2  = XCOLOR_BLACK;
        }
        else if( (m_Flags & (WF_HIGHLIGHT|WF_SELECTED)) == WF_SELECTED )
        {
            State = ui_manager::CS_SELECTED;
            TextColor1  = XCOLOR_WHITE;
            TextColor2  = XCOLOR_BLACK;
        }
        else if( (m_Flags & (WF_HIGHLIGHT|WF_SELECTED)) == (WF_HIGHLIGHT|WF_SELECTED) )
        {
            State = ui_manager::CS_HIGHLIGHT_SELECTED;
            TextColor1  = XCOLOR_WHITE;
            TextColor2  = XCOLOR_BLACK;
        }
        else
        {
            State = ui_manager::CS_NORMAL;
            TextColor1  = XCOLOR_WHITE;
            TextColor2  = XCOLOR_BLACK;
        }

        // Add Highlight to list
        if( m_Flags & WF_HIGHLIGHT )
            m_pManager->AddHighlight( m_UserID, br, !(m_Flags & WF_SELECTED) );

        // Render background color
        irect   rb = r;
        rb.Deflate( 1, 1 );
        m_pManager->RenderRect( rb, m_BackgroundColor, FALSE );

        // Render Text & Selection Marker
        irect rl = r;
        rl.SetHeight( LINE_HEIGHT );
        rl.Deflate( 2, 0 );
        rl.r -= 2;
        rl.Translate( 0, SPACE_TOP );

        m_pManager->PushClipWindow( r );

        for( i=0 ; i<m_nVisibleItems ; i++ )
        {
            s32 iItem = m_iFirstVisibleItem + i;

            if( (iItem >= 0) && (iItem < m_Items.GetCount()) )
            {
                // Render Selection Rectangle
                if( (iItem == m_iSelection)  && m_ShowBorders )
                {
                    if( m_Flags & (WF_SELECTED) )
                    {
                        m_pManager->RenderRect( rl, xcolor(0,100,160,192), FALSE );
                        if( m_Flags & WF_HIGHLIGHT )
                            m_pManager->AddHighlight( m_UserID, rl );
                    }
                    else
                        m_pManager->RenderRect( rl, xcolor(0,60,100,192), FALSE );
                }
#ifdef TARGET_PC
                // Let the hight light track the mouse cursor.
                if( iItem == m_TrackHighLight )
                {
                    m_pManager->AddHighlight( m_UserID, rl );
                }
#endif

                // Render Text
                xcolor c1 = m_Items[iItem].Color;
                xcolor c2 = TextColor2;
                if( !m_Items[iItem].Enabled )
                {
                    c1 = XCOLOR_GREY;
                    c2 = xcolor(0,0,0,0);
                }
                irect rl2 = rl;

/*
                // Darken text if listbox not selected
                if( !(m_Flags & WF_SELECTED) )
                {
                    c1.R = (u8)(c1.R * 0.85f);
                    c1.G = (u8)(c1.G * 0.85f);
                    c1.B = (u8)(c1.B * 0.85f);
                    c1.A = (u8)(c1.A * 0.85f);
                    c2.R = (u8)(c2.R * 0.85f);
                    c2.G = (u8)(c2.G * 0.85f);
                    c2.B = (u8)(c2.B * 0.85f);
                    c2.A = (u8)(c2.A * 0.85f);
                }
*/

//				m_pManager->PushClipWindow( rl2 );

                RenderItem( rl2, m_Items[iItem], c1, c2 );

				// Clear the clip window
//				m_pManager->PopClipWindow();

            }
            rl.Translate( 0, LINE_HEIGHT );
        }

        m_pManager->PopClipWindow();

        if (m_ShowBorders)
        {
            // Render Frame
            m_pManager->RenderElement( m_iElementFrame, r, 0 );
            irect r3 = r2;
            irect r4 = r2;
            r3.b = r3.t + 22;
            r4.t = r4.b - 22;
            r2.t = r3.b;
            r2.b = r4.t;

#ifdef TARGET_PC
            m_UpArrow = r3;
            m_DownArrow = r4;
#endif
            m_pManager->RenderElement( m_iElement_sb_container, r2, State );
            m_pManager->RenderElement( m_iElement_sb_arrowup,   r3, State );
            m_pManager->RenderElement( m_iElement_sb_arrowdown, r4, State );

            // Render Thumb
            r2.Deflate( 2, 2 );
			s32 itemcount;

			itemcount = 0;
			for (s32 i=0;i<m_Items.GetCount();i++)
			{
				if (m_Items[i].Enabled)
					itemcount++;
			}

            if( itemcount > m_nVisibleItems )
            {
#if 1 //CJG - New Thumb Code
                s32 ThumbSize = (s32)(r2.GetHeight() * ((f32)m_nVisibleItems / itemcount));
                if( ThumbSize < 16 )
                    ThumbSize = 16;

                s32 ThumbPos  = (s32)((r2.GetHeight()-ThumbSize) * ((f32)m_iFirstVisibleItem / (itemcount - m_nVisibleItems)));

                r2.Set( r2.l, r2.t + ThumbPos, r2.r, r2.t + ThumbPos + ThumbSize );
#else
                f32 t = (f32)m_iFirstVisibleItem / itemcount;
                f32 b = (f32)(m_iFirstVisibleItem + m_nVisibleItems) / itemcount;
                if( t < 0.0f ) t = 0.0f;
                if( b > 1.0f ) b = 1.0f;
                r2.Set( r2.l, r2.t + (s32)(r2.GetHeight() * t), r2.r, r2.t + (s32)(r2.GetHeight() * b) );
#endif
            }
//            if( r2.GetHeight() > 16 )
                m_pManager->RenderElement( m_iElement_sb_thumb,     r2, State );
#ifdef TARGET_PC
            m_ScrollBar = r2;
#endif
        }

        // Render children
        for( s32 i=0 ; i<m_Children.GetCount() ; i++ )
        {
            m_Children[i]->Render( m_Position.l+ox, m_Position.t+oy );
        }
    }
}

//=========================================================================

void ui_listbox::RenderItem( irect r, const item& Item, const xcolor& c1, const xcolor& c2 )
{
    r.Deflate( 4, 0 );
    r.Translate( 1, -2 );
    m_pManager->RenderText( m_Font, r, m_LabelFlags, c2, Item.Label );
    r.Translate( -1, -1 );
    m_pManager->RenderText( m_Font, r, m_LabelFlags, c1, Item.Label );
}

//=========================================================================

void ui_listbox::SetPosition( const irect& Position )
{
    m_Position      = Position;
    m_nVisibleItems = (m_Position.GetHeight()-SPACE_TOP-SPACE_BOTTOM) / LINE_HEIGHT;
}

//=========================================================================

void ui_listbox::OnPadNavigate( ui_win* pWin, s32 Code, s32 Presses, s32 Repeats )
{
    xbool       Processed = FALSE;
    s32         dy = 0;

    // Determine movement required
    switch( Code )
    {
    case ui_manager::NAV_UP:
        dy = -1;
        break;
    case ui_manager::NAV_DOWN:
        dy =  1;
    }

    // Apply movement
    if( m_Flags & WF_SELECTED )
    {
        if( dy != 0 )
        {
            s32 OldSelection = m_iSelection;

            s32 iItem = m_iSelection + dy;
            while( (iItem >= 0) && (iItem < m_Items.GetCount()) && (!m_Items[iItem].Enabled) )
            {
                iItem += dy;
            }
            if( iItem <= -1 )
                iItem = -1;
            if( iItem >= m_Items.GetCount() )
                iItem = -1;

            if( iItem != -1 )
                m_iSelection = iItem;
            else
            {
                if( Presses > 0 )
                    audio_Play( SFX_FRONTEND_ERROR,AUDFLAG_CHANNELSAVER );
            }
            
            EnsureVisible( m_iSelection );

            if( (m_iSelection != OldSelection) && m_pParent )
            {
                m_pParent->OnNotify( m_pParent, this, WN_LIST_SELCHANGE, (void*)m_iSelection );
                audio_Play( SFX_FRONTEND_CURSOR_MOVE_02, AUDFLAG_CHANNELSAVER );
            }
        }

        Processed = TRUE;
    }

    // Pass up chain if not processed
    if( !Processed )
    {
        if( m_pParent )
            m_pParent->OnPadNavigate( pWin, Code, Presses, Repeats );
    }
}

void ui_listbox::OnPadShoulder( ui_win* pWin, s32 Direction )
{
    (void)pWin;

    if( Direction != 0 )
    {
        if( m_iSelection != -1 )
        {
            m_iSelection += (m_nVisibleItems-1) * Direction;
            if( m_iSelection > (m_Items.GetCount()-1) )
                m_iSelection = (m_Items.GetCount()-1);
            if( m_iSelection < 0 )
                m_iSelection = 0;
            EnsureVisible( m_iSelection );
        }
/*
        s32 iFirstVisible = m_iFirstVisibleItem;
        s32 iSelection    = m_iSelection;
        s32 iSelOffset    = iSelection - iFirstVisible;

        iFirstVisible += m_nVisibleItems * Direction;

        if( iFirstVisible > (m_Items.GetCount()-m_nVisibleItems) )
            iFirstVisible = (m_Items.GetCount()-m_nVisibleItems);
        if( iFirstVisible < 0 )
            iFirstVisible = 0;

        iSelection = iFirstVisible + iSelOffset;

        if( iSelection > (m_Items.GetCount()-1) )
            iSelection = (m_Items.GetCount()-1);
        if( iSelection < 0 )
            iSelection = 0;

        m_iFirstVisibleItem = iFirstVisible;
        if( m_iSelection != -1 )
            m_iSelection = iSelection;
*/
    }
}

void ui_listbox::OnPadShoulder2( ui_win* pWin, s32 Direction )
{
    (void)pWin;

    if( Direction != 0 )
    {
        if( m_iSelection != -1 )
        {
            m_iSelection += (m_Items.GetCount()-1) * Direction;
            if( m_iSelection > (m_Items.GetCount()-1) )
                m_iSelection = (m_Items.GetCount()-1);
            if( m_iSelection < 0 )
                m_iSelection = 0;
            EnsureVisible( m_iSelection );
        }
/*
        s32 iFirstVisible = m_iFirstVisibleItem;
        s32 iSelection    = m_iSelection;

        iFirstVisible += m_Items.GetCount() * Direction;

        if( iFirstVisible > (m_Items.GetCount()-m_nVisibleItems) )
            iFirstVisible = (m_Items.GetCount()-m_nVisibleItems);
        if( iFirstVisible < 0 )
            iFirstVisible = 0;

        iSelection += m_Items.GetCount() * Direction;

        if( iSelection > (m_Items.GetCount()-1) )
            iSelection = (m_Items.GetCount()-1);
        if( iSelection < 0 )
            iSelection = 0;

        m_iFirstVisibleItem = iFirstVisible;
        if( m_iSelection != -1 )
            m_iSelection = iSelection;
*/
    }
}

//=========================================================================

void ui_listbox::OnPadSelect( ui_win* pWin )
{
    (void)pWin;

    // Check if Exit on Select is disabled
    if( !m_ExitOnSelect && (m_Flags & WF_SELECTED) )
    {
        if( m_pParent )
            m_pParent->OnNotify( m_pParent, this, WN_LIST_ACCEPTED, (void*)m_iSelection );
    }
    else
    {
        if( (m_Flags & WF_SELECTED) || (GetNumEnabledItems() > 0) )
        {
            // Toggle Selected
            m_Flags ^= WF_SELECTED;

            if( m_Flags & WF_SELECTED )
            {
                audio_Play( SFX_FRONTEND_SELECT_02,AUDFLAG_CHANNELSAVER );
                m_iSelectionBackup = m_iSelection;
//                if( m_pParent )
//                    m_pParent->OnNotify( m_pParent, this, WN_LIST_CANCELLED, (void*)m_iSelection );
            }
            else
            {
                audio_Play( SFX_FRONTEND_CANCEL_02,AUDFLAG_CHANNELSAVER );
                if( m_pParent )
                    m_pParent->OnNotify( m_pParent, this, WN_LIST_ACCEPTED, (void*)m_iSelection );
            }
        }
        else
        {
            audio_Play( SFX_FRONTEND_ERROR,AUDFLAG_CHANNELSAVER );
        }
    }
}

//=========================================================================

void ui_listbox::OnPadBack( ui_win* pWin )
{
    (void)pWin;

    if( m_Flags & WF_SELECTED )
    {
        // Clear selected
        m_Flags &= ~WF_SELECTED;
        audio_Play( SFX_FRONTEND_CANCEL_02,AUDFLAG_CHANNELSAVER );

        // BW 3/28 - I was getting an assert if all the entries within a listbox disappeared
        // between being in the listbox and then hitting back. This was happening because
        // m_iSelectionBackup exceeded the range for this listbox.
        if (m_iSelectionBackup > GetItemCount() )
        {
            m_iSelectionBackup = GetItemCount()-1;
        }
        SetSelection( m_iSelectionBackup );

        if( m_pParent )
            m_pParent->OnNotify( m_pParent, this, WN_LIST_CANCELLED, (void*)m_iSelection );
    }
    else
    {
        if( m_pParent )
            m_pParent->OnPadBack( pWin );
    }
}

//=========================================================================

void ui_listbox::SetExitOnSelect( xbool State )
{
    m_ExitOnSelect = State;
}

//=========================================================================

s32 ui_listbox::AddItem( const xwstring& Label, s32 Data )
{
    item& Item = m_Items.Append();
    Item.Enabled = TRUE;
    Item.Label   = Label;
    Item.Data    = Data;
    Item.Color   = XCOLOR_WHITE;
    return m_Items.GetCount()-1;
}

//=========================================================================

s32 ui_listbox::AddItem( const xwchar* Label, s32 Data )
{
    item& Item = m_Items.Append();
    Item.Enabled = TRUE;
    Item.Label   = Label;
    Item.Data    = Data;
    Item.Color   = XCOLOR_WHITE;
    return m_Items.GetCount()-1;
}

//=========================================================================

void ui_listbox::DeleteAllItems( void )
{
    m_iSelection        = -1;
    m_iFirstVisibleItem = 0;

    m_Items.Delete( 0, m_Items.GetCount() );

    if( m_pParent )
        m_pParent->OnNotify( m_pParent, this, WN_LIST_SELCHANGE, (void*)m_iSelection );
}

//=========================================================================

void ui_listbox::DeleteItem( s32 iItem )
{
    ASSERT( (iItem >= 0) && (iItem < m_Items.GetCount()) );

    s32 OldSelection = m_iSelection;

    m_Items.Delete( iItem );

    if( iItem < m_iSelection ) m_iSelection--;
    if( m_iSelection < 0 ) m_iSelection = 0;
    if( m_iSelection >= m_Items.GetCount() ) m_iSelection = m_Items.GetCount() - 1;
    if( m_Items.GetCount() == 0 ) m_iSelection = -1;

    EnsureVisible( m_iSelection );

    if( (m_iSelection != OldSelection) && m_pParent )
        m_pParent->OnNotify( m_pParent, this, WN_LIST_SELCHANGE, (void*)m_iSelection );
}

//=========================================================================

void ui_listbox::EnableItem( s32 iItem, xbool State )
{
    ASSERT( (iItem >= 0) && (iItem < m_Items.GetCount()) );

    s32 OldSelection = m_iSelection;
    m_Items[iItem].Enabled = State;

    // If the selected item was just disabled then search for new item to select
    if( (iItem == m_iSelection) && !State )
    {
        s32 iFound  = -1;
        s32 i1      = iItem-1;
        s32 i2      = iItem+1;

        while( (i1 >= 0) && (i2 < m_Items.GetCount()) )
        {
            if( i1 >= 0 )
            {
                if( m_Items[i1].Enabled )
                {
                    iFound = i1;
                    break;
                }
                i1--;
            }
            if( i2 < m_Items.GetCount() )
            {
                if( m_Items[i2].Enabled )
                {
                    iFound = i2;
                    break;
                }
                i2++;
            }
        }
        m_iSelection = iFound;
        EnsureVisible( m_iSelection );

        if( (m_iSelection != OldSelection) && m_pParent )
        m_pParent->OnNotify( m_pParent, this, WN_LIST_SELCHANGE, (void*)m_iSelection );
    }
}

//=========================================================================

s32 ui_listbox::GetItemCount( void ) const
{
    return m_Items.GetCount();
}

//=========================================================================

const xwstring& ui_listbox::GetItemLabel( s32 iItem ) const
{
    ASSERT( (iItem >= 0) && (iItem < m_Items.GetCount()) );

    return m_Items[iItem].Label;
}

//=========================================================================

void ui_listbox::SetItemLabel( s32 iItem, const xwstring& Label )
{
    ASSERT( (iItem >= 0) && (iItem < m_Items.GetCount()) );

    m_Items[iItem].Label = Label;
}

//=========================================================================

s32 ui_listbox::GetItemData( s32 iItem ) const
{
    ASSERT( (iItem >= 0) && (iItem < m_Items.GetCount()) );

    return m_Items[iItem].Data;
}

//=========================================================================

const xwstring& ui_listbox::GetSelectedItemLabel( void ) const
{
    ASSERT( (m_iSelection >= 0) && (m_iSelection < m_Items.GetCount()) );

    return m_Items[m_iSelection].Label;
}

//=========================================================================

s32 ui_listbox::GetSelectedItemData( void ) const
{
    ASSERT( (m_iSelection >= 0) && (m_iSelection < m_Items.GetCount()) );

    return m_Items[m_iSelection].Data;
}

//=========================================================================

void ui_listbox::SetItemColor( s32 iItem, const xcolor& Color )
{
    ASSERT( (iItem >= 0) && (iItem < m_Items.GetCount()) );

    m_Items[iItem].Color = Color;
}

//=========================================================================

xcolor ui_listbox::GetItemColor( s32 iItem ) const
{
    ASSERT( (iItem >= 0) && (iItem < m_Items.GetCount()) );

    return m_Items[iItem].Color;
}

//=========================================================================

s32 ui_listbox::FindItemByLabel( const xwstring& Label )
{
    s32     i;
    s32     iFound = -1;

    for( i=0 ; i<m_Items.GetCount() ; i++ )
    {
        if( m_Items[i].Label == Label )
        {
            iFound = i;
            break;
        }
    }

    return iFound;
}

//=========================================================================

s32 ui_listbox::FindItemByData( s32 Data )
{
    s32     i;
    s32     iFound = -1;

    for( i=0 ; i<m_Items.GetCount() ; i++ )
    {
        if( m_Items[i].Data == Data )
        {
            iFound = i;
            break;
        }
    }

    return iFound;
}

//=========================================================================

s32 ui_listbox::GetSelection( void ) const
{
    return m_iSelection;
}

//=========================================================================

void ui_listbox::SetSelection( s32 iSelection )
{
    ASSERT( (iSelection >= -1) && (iSelection < m_Items.GetCount()) );

    if( iSelection < -1 )
        iSelection = -1;
    if( iSelection > (m_Items.GetCount()-1) )
        iSelection = m_Items.GetCount()-1;

    if( (iSelection == -1) || m_Items[iSelection].Enabled )
    {
        m_iSelection = iSelection;
        EnsureVisible( m_iSelection );

        if( m_pParent )
            m_pParent->OnNotify( m_pParent, this, WN_LIST_SELCHANGE, (void*)m_iSelection );
    }
}

//=========================================================================

void ui_listbox::ClearSelection( void )
{
    m_iSelection = -1;
}

//=========================================================================

void ui_listbox::EnsureVisible( s32 iItem )
{
    ASSERT( (iItem >= -1) && (iItem < m_Items.GetCount()) );

    if( iItem != -1 )
    {
        if( iItem < m_iFirstVisibleItem )
        {
            m_iFirstVisibleItem = iItem;
        }
        if( iItem >= (m_iFirstVisibleItem+m_nVisibleItems) )
        {
            m_iFirstVisibleItem = iItem - (m_nVisibleItems-1);
        }
    }
}

//=========================================================================

s32 ui_listbox::GetNumEnabledItems( void )
{
    s32 i;
    s32 Count = 0;
    for( i=0 ; i<m_Items.GetCount() ; i++ )
    {
        if( m_Items[i].Enabled )
            Count++;
    }
    return Count;
}

//=========================================================================

s32 ui_listbox::GetCursorOffset( void )
{
    s32 Offset;

    Offset = m_iSelection - m_iFirstVisibleItem;
    if( Offset >= m_nVisibleItems )
        Offset = m_nVisibleItems-1;
    if( Offset < 0 )
        Offset = 0;

    return Offset;
}

//=========================================================================

void ui_listbox::SetSelectionWithOffset( s32 iSelection, s32 Offset )
{
    ASSERT( (iSelection >= -1) && (iSelection < m_Items.GetCount()) );

    if( (iSelection == -1) || m_Items[iSelection].Enabled )
    {
        m_iSelection = iSelection;

        if( m_iSelection != -1 )
        {
            m_iFirstVisibleItem = m_iSelection - Offset;

            if( m_iFirstVisibleItem >= (m_Items.GetCount() - m_nVisibleItems) )
                m_iFirstVisibleItem = m_Items.GetCount() - (m_nVisibleItems);

            if( m_iFirstVisibleItem < 0 )
                m_iFirstVisibleItem = 0;
        }

        if( m_pParent )
            m_pParent->OnNotify( m_pParent, this, WN_LIST_SELCHANGE, (void*)m_iSelection );
    }
}

//=========================================================================

void ui_listbox::SetBackgroundColor( xcolor Color )
{
    m_BackgroundColor = Color;
}

//=========================================================================

xcolor ui_listbox::GetBackgroundColor( void ) const
{
    return m_BackgroundColor;
}

//=========================================================================

void ui_listbox::OnCursorMove( ui_win* pWin, s32 x, s32 y )
{   
    (void)pWin;
    (void)x;
    (void)y;

#ifndef TARGET_PC
    return;
#else
    
    if( m_ScrollDown )
    {    
        if( m_ScrollBar.PointInRect( m_CursorX, m_CursorY ) )
        {
            s32 FirstVisible = m_iFirstVisibleItem;
            s32 diff = (y - m_CursorY);

            if( diff > 0 )
            {
                diff = (s32)((f32)diff/4);
                FirstVisible += diff;
    
                if( FirstVisible > (m_Items.GetCount()-m_nVisibleItems) )
                    FirstVisible = (m_Items.GetCount()-m_nVisibleItems);

                // Set new position back into first visible
                if( (FirstVisible != m_iFirstVisibleItem) && m_pParent )
                {
                    if( m_iSelection < FirstVisible )
                        m_iSelection = FirstVisible;

                    audio_Play( SFX_FRONTEND_CURSOR_MOVE_02,AUDFLAG_CHANNELSAVER );
                    m_iFirstVisibleItem = FirstVisible;
                    m_pParent->OnNotify( m_pParent, this, WN_LIST_SELCHANGE, (void*)m_iSelection );
                }
            }
            else if( diff < 0 )
            {
                diff = (s32)((f32)diff/4);
                FirstVisible += diff;

                if( FirstVisible < 0 )
                    FirstVisible = 0;

                // Set new position back into last visible
                if( (FirstVisible != m_iFirstVisibleItem) && m_pParent )
                {
                    if( m_iSelection > (FirstVisible + m_nVisibleItems)-1 )
                        m_iSelection = (FirstVisible + m_nVisibleItems)-1;

                    audio_Play( SFX_FRONTEND_CURSOR_MOVE_02,AUDFLAG_CHANNELSAVER );
                    m_iFirstVisibleItem = FirstVisible;
                    m_pParent->OnNotify( m_pParent, this, WN_LIST_SELCHANGE, (void*)m_iSelection );
                }
            }
        }
    }

    xbool Processed = FALSE;
    s32 dy = 0;
    m_CursorX = x;
    m_CursorY = y;
    ScreenToLocal( x, y );
    irect scroll( m_UpArrow.l, m_UpArrow.t, m_DownArrow.r, m_DownArrow.b );
    ScreenToLocal( scroll );

    // If the cursor is in the scroll bar then don't do anything.
    if( scroll.PointInRect( x, y ) )
    {

        if( m_MouseDown )
        {
            // Just move the selected item down one.
            if( m_DownArrow.PointInRect( m_CursorX, m_CursorY ) )
            {
                m_MouseDown = TRUE;
            }
            // Just move the selected item up one.
            else if( m_UpArrow.PointInRect( m_CursorX, m_CursorY ) )
            {
                m_MouseDown = TRUE;
            }    
            else
            {
                m_MouseDown = FALSE;
            }        
        }

        if( m_pParent )
            m_pParent->OnCursorMove( pWin, x, y );
        return;
    }

    // Find out which item is the cursor on.
    dy = y - SPACE_TOP;
    dy = dy/LINE_HEIGHT;
    dy += m_iFirstVisibleItem;

    if( dy <= -1 )
        dy = -1;
    else if( dy >= m_Items.GetCount() )
        dy = -1;
    
    // Apply movement
    if( m_Flags & WF_SELECTED )
    {
        // Chech if we moved.
        s32 OldSelection = m_iSelection;
        s32 iItem = dy;
        
        // The new item to move to.
        if( iItem != -1 )
        {
            m_iSelection = iItem;
            m_TrackHighLight = iItem;

            EnsureVisible( m_iSelection );

            if( (m_iSelection != OldSelection) && m_pParent )
            {
                m_pParent->OnNotify( m_pParent, this, WN_LIST_SELCHANGE, (void*)m_iSelection );
                audio_Play( SFX_FRONTEND_CURSOR_MOVE_02,AUDFLAG_CHANNELSAVER );
            }
        }
            
            
        Processed = TRUE;
    }
    // Track the mouse cursor.
    else
    {
        if( dy != -1 )
            m_TrackHighLight = dy;
    }


    // Pass up the chain, if not processed.
    if( !Processed )
    {
        if( m_pParent )
            m_pParent->OnCursorMove( pWin, x, y );
    }   
#endif
}

//=========================================================================

void ui_listbox::OnLBDown( ui_win* pWin )
{
    (void)pWin;

#ifdef TARGET_PC
    // Check if Exit on Select is disabled
    if( !m_ExitOnSelect && (m_Flags & WF_SELECTED) )
    {
        if( m_pParent )
            m_pParent->OnNotify( m_pParent, this, WN_LIST_ACCEPTED, (void*)m_iSelection );
    }
    else
    {
        irect scroll( m_UpArrow.l, m_UpArrow.t, m_DownArrow.r, m_DownArrow.b );
        if( (m_Flags & WF_SELECTED) || (GetNumEnabledItems() > 0) )
        {

            // Don't select the listbox if the cursor is on the scroll bar portion.
            if( scroll.PointInRect( m_CursorX, m_CursorY ) )
            {
                
                s32 OldSelection = m_iSelection;
                // Just move the selected item down one.
                if( m_DownArrow.PointInRect( m_CursorX, m_CursorY ) )
                {
                    m_iSelection++;
                    if( m_iSelection >= m_Items.GetCount() )
                        m_iSelection = m_Items.GetCount()-1;

                    // Track the highlight.
                    m_TrackHighLight = m_iSelection;
                    m_MouseDown = TRUE;
                }

                // Just move the selected item up one.
                if( m_UpArrow.PointInRect( m_CursorX, m_CursorY ) )
                {
                    m_iSelection--;
                    if( m_iSelection < 0 )
                        m_iSelection = 0;

                    // Track the highlight.
                    m_TrackHighLight = m_iSelection;
                    m_MouseDown = TRUE;
                }            
                // Did the mouse click on the scroll bar.    
                else if( m_ScrollBar.PointInRect( m_CursorX, m_CursorY ) )
                    m_ScrollDown = TRUE;
            
                EnsureVisible( m_iSelection );

                if( m_MouseDown )
                {
                    if( (m_iSelection != OldSelection) && m_pParent )
                    {
                        m_pParent->OnNotify( m_pParent, this, WN_LIST_SELCHANGE, (void*)m_iSelection );
                        audio_Play( SFX_FRONTEND_CURSOR_MOVE_02,AUDFLAG_CHANNELSAVER );
                    }
                }

                return;
            }

            // Toggle Selected
            m_Flags ^= WF_SELECTED;
            m_iSelection = m_TrackHighLight;            

            if( m_Flags & WF_SELECTED )
            {
                audio_Play( SFX_FRONTEND_SELECT_02,AUDFLAG_CHANNELSAVER );
                m_iSelectionBackup = m_iSelection;
//                if( m_pParent )
//                    m_pParent->OnNotify( m_pParent, this, WN_LIST_CANCELLED, (void*)m_iSelection );
            }
            else
            {
                audio_Play( SFX_FRONTEND_CANCEL_02,AUDFLAG_CHANNELSAVER );
                if( m_pParent )
                    m_pParent->OnNotify( m_pParent, this, WN_LIST_ACCEPTED, (void*)m_iSelection );
            }
        }
        else
        {
            audio_Play( SFX_FRONTEND_ERROR,AUDFLAG_CHANNELSAVER );
        }
    }
#endif

}

//=========================================================================

void ui_listbox::OnUpdate ( ui_win* pWin, f32 DeltaTime )
{
    (void)pWin;
    (void)DeltaTime;
    
#ifdef TARGET_PC
    return;
    xbool Processed = FALSE;
    m_ScrollTime += DeltaTime;
    
    // Set a delay to scroll.
    if( m_ScrollTime < 0.3f )
        return;
    else
        m_ScrollTime = 0.0f;
    
    if( m_MouseDown )
    {
        s32 OldSelection = m_iSelection;

        // Just move the selected item down one.
        if( m_DownArrow.PointInRect( m_CursorX, m_CursorY ) )
        {
            m_iSelection++;
            
            // Don't go over the number of items.
            if( m_iSelection >= m_Items.GetCount() )
                m_iSelection = m_Items.GetCount()-1;
            
            // Track the highlight.
            m_TrackHighLight = m_iSelection;
            Processed = TRUE;
        }

        // Just move the selected item up one.
        if( m_UpArrow.PointInRect( m_CursorX, m_CursorY ) )
        {
            m_iSelection--;
            if( m_iSelection < 0 )
                m_iSelection = 0;
            
            // Track the highlight.
            m_TrackHighLight = m_iSelection;
            Processed = TRUE;
        }
        
        EnsureVisible( m_iSelection );

        if( Processed )
        {
            if( (m_iSelection != OldSelection) && m_pParent )
            {
                m_pParent->OnNotify( m_pParent, this, WN_LIST_SELCHANGE, (void*)m_iSelection );
                audio_Play( SFX_FRONTEND_CURSOR_MOVE_02,AUDFLAG_CHANNELSAVER );
            }
        }
    }
    
    if( !Processed )
    {    
        if( m_pParent )
            m_pParent->OnUpdate( pWin, DeltaTime );

    }
#endif
}

//=========================================================================

void ui_listbox::OnLBUp ( ui_win* pWin )
{
    (void)pWin;

#ifdef TARGET_PC
    m_MouseDown = FALSE;
    m_ScrollDown = FALSE;
#endif
}

//=========================================================================

void ui_listbox::OnCursorExit ( ui_win* pWin )
{
    (void) pWin;

#ifdef TARGET_PC
    // If the cursor leaves the list box then place the highlight with the selection.
    m_TrackHighLight = m_iSelection;
    m_MouseDown = FALSE;
    m_ScrollDown = FALSE;
#endif

    // Turn off the high light.
    ui_win::OnCursorExit( pWin );
        
    if( m_pParent )
        m_pParent->OnCursorExit( pWin );
}

//=========================================================================

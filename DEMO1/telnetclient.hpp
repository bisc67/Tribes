#ifndef __TELNET_CLIENT_HPP
#define __TELNET_CLIENT_HPP

#include "telnetdisplay.hpp"
#include "x_threads.hpp"
#include "support/gamemgr/gamemgr.hpp"
#include "gameserver.hpp"

#define TELNET_LOG_HISTORY 20


class telnet_client
{
public:
	void			Init			(const net_address& Local);
	void			Kill			(void);
	void			Main			(void);
	xbool			Update			(f32 DeltaTime);
	void			Refresh			(xbool FullRefresh);
	void			AddLogText		(const f32 time, const char* pString);
private:

	enum disp_mode
	{
		DISP_MODE_PLAYERS_1=0,
		DISP_MODE_PLAYERS_2,
		DISP_MODE_CLIENTS_1,
		DISP_MODE_CLIENTS_2,
		DISP_MODE_SERVER,

		DISP_MODE_LAST,
	};

	net_address		m_Local;
	net_address		m_Remote;
	xthread*		m_pThread;

	// Authentication
	xbool			m_Authenticated;
	s32				m_Retries;
	char			m_Username[TELNET_LINE_LENGTH];
	char			m_Password[TELNET_LINE_LENGTH];

	xbool			m_Exit;

	// Display
	f32				m_UpdateInterval;
	f32				m_RefreshInterval;
	xtimer			m_RefreshTimer;
	telnet_display	m_Display;
	s32				m_DisplayMode;
	s32				m_LastDisplayMode;
	telnet_display::window* m_pInfoWindow;
	telnet_display::window* m_pInputWindow;
	telnet_display::window* m_pLogWindow;

	s32				m_LastBots;
	s32				m_LastHumans;
	s32				m_LastMax;

	s32				m_ClientCount;
	s32				m_LastClientCount;
	game_score		m_LastGameScore;
	s32				m_LastPlayerCount;
	s32				m_LastLeftLine;
	s32				m_LastRightLine;
	s32				m_LastBytesReceived;
	s32				m_LastBytesSent;
	s32				m_LastPacketsReceived;
	s32				m_LastPacketsSent;


	s32				m_LastBytesIn[MAX_CLIENTS];
	s32				m_LastBytesOut[MAX_CLIENTS];

	char			m_LogText[TELNET_LOG_HISTORY][80];
	f32				m_LogTextTime[TELNET_LOG_HISTORY];
	s32				m_LogTextIndex;
	s32				m_LastLogTextIndex;

	// Control
	char			WaitForCommand	(const char* Label, 
									 const char* Commands,
									 const char** Help,
									 const telnet_color foreground=WHITE,
									 const telnet_color background=BLACK);
	void			Stop			(void);
	// Command processing functions
	void			KickPlayer		(void);
	void			SwitchTeam		(void);
	void			EnableProperty	(void);
	void			DisableProperty	(void);
	void			ServerAdmin		(void);
	xbool			RefreshPlayers	(s32 PlayerIndex,xbool ForceRefresh);
	xbool			RefreshClients	(s32 ClientIndex,xbool ForceRefresh);
	void			RefreshServer	(xbool ForceRefresh);
	void			SampleGameState	(void);
	void			RefreshLogText	(xbool ForceRefresh);


friend telnet_display;
};



#endif
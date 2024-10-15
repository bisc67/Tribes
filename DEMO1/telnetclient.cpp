#include "x_files.hpp"
#include "telnetclient.hpp"

enum
{
	HM_MAIN=0,
	HM_ADMIN,
	HM_ENABLE,
	HM_DISABLE,
	HM_TOTAL,
};
//
// Game specific files needed so that we can gather
// stats about what's going on
//
#include "fe_globals.hpp"
#include "support/gamemgr/gamemgr.hpp"
#include "masterserver/masterserver.hpp"
#include "gameserver.hpp"

extern server_manager*  pSvrMgr;

#if !defined(TARGET_PC)

#error This is a PC only file. Please exclude it from the build rules

#endif

struct telnet_usernames
{
	const char* Name;
	const char* Password;
};

telnet_usernames s_Usernames[]=
{
	{"inevitable","peekaboo"},
	{"guest",""},
	{NULL,NULL},
};

extern const char** s_HelpMessage[];

static telnet_client* s_pClient = NULL;

void	s_StartTelnetClient(void)
{
	telnet_client *pThis;

	ASSERT(s_pClient);
	pThis = s_pClient;
	s_pClient = NULL;
	pThis->Main();
	ASSERT(FALSE);
}

//******************************************************************
// Startup/shutdown routines
//******************************************************************
void telnet_client::Init(const net_address& Address)
{
	m_Retries = 3;
	m_Authenticated = false;
	m_Username[0]=0x0;
	m_Password[0]=0x0;
	m_Exit = FALSE;
	s_pClient = this;
	m_UpdateInterval = 0.0f;
	m_DisplayMode = DISP_MODE_PLAYERS_1;
	m_LastDisplayMode = DISP_MODE_LAST;
	x_memset(m_LastBytesIn,0,sizeof(m_LastBytesIn));
	x_memset(m_LastBytesOut,0,sizeof(m_LastBytesOut));
	m_LogTextIndex=0;
	x_memset(m_LogText,0,sizeof(m_LogText));
	x_memset(m_LogTextTime,0,sizeof(m_LogTextTime));

	m_Display.Init(this,Address,80,24);
	m_pInfoWindow = new telnet_display::window(0,4,80,m_Display.GetHeight()-11,WIN_FLAG_BORDER);
	m_pInputWindow = new telnet_display::window(0,m_Display.GetHeight() - 1,m_Display.GetWidth()+2,3,WIN_FLAG_BORDER);
	m_pLogWindow = new telnet_display::window(0,m_Display.GetHeight() - 8,m_Display.GetWidth()+2,8,WIN_FLAG_BORDER);


	m_pThread = new xthread(s_StartTelnetClient,"Telnet Client",8192,0);
	// Wait until the client starts up
	while (s_pClient)
	{
		x_DelayThread(1);
	}
}

//******************************************************************
void telnet_client::Kill(void)
{
	delete m_pInfoWindow;
	delete m_pInputWindow;
	delete m_pLogWindow;
	m_Display.Kill();
	delete m_pThread;
	shutdown(m_Remote.Socket,2);
	closesocket(m_Remote.Socket);
}

//******************************************************************
void telnet_client::Stop(void)
{
	m_Display.Flush();
	m_Exit = TRUE;
	while(1)
	{
		x_DelayThread(100);
	}
}

//******************************************************************
void telnet_client::AddLogText(const f32 time, const char* pString)
{
	x_strncpy(m_LogText[m_LogTextIndex],pString,sizeof(m_LogText[m_LogTextIndex]));
	m_LogText[m_LogTextIndex][79]=0x0;
	m_LogTextTime[m_LogTextIndex]=time;
	m_LogTextIndex++;
	if (m_LogTextIndex >= TELNET_LOG_HISTORY)
	{
		m_LogTextIndex=0;
	}
}

//******************************************************************
xbool telnet_client::Update(f32 DeltaTime)
{
	m_UpdateInterval = DeltaTime;
	return m_Exit;
}

//******************************************************************
// Re-render the entire display
//******************************************************************
void telnet_client::Refresh(xbool ForceRefresh)
{
	s32 Humans,Bots,Max;
	s32 y;
	const game_score& Score = GameMgr.GetScore();


	y = 0;

	m_RefreshTimer.Stop();
	m_RefreshInterval = m_RefreshTimer.ReadSec();
	if (m_RefreshInterval < 0.10f)
	{
		m_RefreshInterval = 0.1f;
	}
	m_RefreshTimer.Reset();
	m_RefreshTimer.Start();
	m_Display.SetWindow(NULL);
	if (ForceRefresh)
	{
		m_Display.Clear();
	}
	s64 time=(s64)x_GetTimeSec();

	s32 days,hours,minutes,seconds;

	seconds = (s32)(time % 60);	time = time / 60;
	minutes = (s32)(time % 60);	time = time / 60;
	hours	= (s32)(time % 24);	time = time / 24;
	days    = (s32)time;

	if (ForceRefresh)
	{
		m_Display.PrintXY(0,y,    "SERVER NAME: %ls \n",fegl.ServerSettings.ServerName);
	}

	m_Display.PrintXY(26,y,	"DELTA: %2.2fms   \n",m_UpdateInterval*1000.0f);
	m_Display.PrintXY(45,y++,	"UPTIME: %d:%d:%02d:%02d   ",days,hours,minutes,seconds);

	GameMgr.GetPlayerCounts(Humans,Bots,Max);
	if (tgl.pMasterServer && tgl.pServer)
	{							
		m_Display.PrintXY(0,y,"   AVG PING: %2.2fms ",tgl.pServer->GetAveragePing());
	}

	if (ForceRefresh || (m_LastHumans!=Humans) || (m_LastBots != Bots) || (m_LastMax != Max) )
	{
		m_Display.PrintXY(20,y,	"     HUMANS: %d   ",Humans);
		m_Display.PrintXY(40,y,	"       BOTS: %d   ",Bots);
		m_Display.PrintXY(60,y,	"    MAXIMUM: %d\n",Max);
		m_LastHumans	= Humans;
		m_LastBots		= Bots;
		m_LastMax		= Max;
	}
	y++;

	if (tgl.pServer)
	{

		seconds = (s32)GameMgr.GetTimeRemaining();
		minutes = seconds / 60; 
		seconds = seconds % 60;
		m_Display.PrintXY(0,y,	"TIME REMAIN: %2d:%02d   ",minutes,seconds);
		m_Display.PrintXY(20,y,	"GAME LENGTH: %d:00   ",GameMgr.GetTimeLimit());
		m_Display.PrintXY(40,y,	"    CLIENTS: %d  ",tgl.pServer->GetClientCount());
		m_Display.PrintXY(60,y,	"MAX CLIENTS: %d  \n",fegl.ServerSettings.MaxClients);
	}
	y++;


	m_Display.SetWindow(m_pInfoWindow);
	if (tgl.pServer)
	{
		s32 count=0;
		s32 index=0;

		if ( ForceRefresh || (m_DisplayMode != m_LastDisplayMode) )
		{
			m_Display.Clear();
			ForceRefresh = TRUE;
		}
		switch (m_DisplayMode)
		{
		case DISP_MODE_PLAYERS_1:
			RefreshPlayers(0, ForceRefresh);
			break;
		case DISP_MODE_PLAYERS_2:
			if (RefreshPlayers(m_Display.GetHeight()*2,ForceRefresh))
			{
				m_DisplayMode++;
				Refresh(TRUE);
			}
			break;
		case DISP_MODE_CLIENTS_1:
			if (RefreshClients(0,ForceRefresh))
			{
				m_DisplayMode++;
				Refresh(TRUE);
			}
			break;
		case DISP_MODE_CLIENTS_2:
			if (RefreshClients(16,ForceRefresh))
			{
				m_DisplayMode++;
				Refresh(TRUE);
			}
			break;

		case DISP_MODE_SERVER:
			RefreshServer(ForceRefresh);
			break;
		default:
			ASSERT(FALSE);
		}
		if (m_DisplayMode >= DISP_MODE_LAST)
		{
			m_DisplayMode = DISP_MODE_PLAYERS_1;
		}

		m_LastDisplayMode = m_DisplayMode;
	}
	else
	{
		m_LastDisplayMode = DISP_MODE_LAST;
		if (ForceRefresh)
		{
			m_Display.Clear();
		}
	}
	m_Display.SetWindow(m_pLogWindow);
	RefreshLogText(ForceRefresh);

	m_Display.SetWindow(NULL);
}

char telnet_client::WaitForCommand(const char* Label, 
								   const char* Commands, 
								   const char** Help, 
								   const telnet_color foreground,
								   const telnet_color background)
{
	const char* pCommands;
	char ch;

	m_Display.SetColor(WHITE,BLACK);
	m_Display.PrintXY(0,m_Display.GetHeight()-1,"%s: ",Label);

	while(1)
	{
		m_Display.SetColor(WHITE,BLACK);
		m_Display.SetWindow(m_pInputWindow);
		m_Display.Clear();
		m_Display.PrintXY(0,m_Display.GetHeight()-1,"%s: ",Label);
		ch = x_toupper(m_Display.GetChar());

		if (ch=='\t')
		{
			m_DisplayMode++;
			if (m_DisplayMode >= DISP_MODE_LAST)
			{
				m_DisplayMode = DISP_MODE_PLAYERS_1;
			}
			Refresh(TRUE);
		}
		if (ch=='H')
		{
			m_Display.SetWindow(m_pInfoWindow);
			m_Display.Clear();
			const char** pText;

			pText = Help;
			while (*pText)
			{
				m_Display.Print(*pText);
				pText++;
			}
			m_Display.GetChar(FALSE);
			m_Display.SetWindow(NULL);
			Refresh(TRUE);
		}
		else
		{
			pCommands = Commands;

			while (*pCommands)
			{
				if (x_toupper(*pCommands)==ch)
				{
					return ch;
				}
				pCommands++;
			}
		}
	}
}
//******************************************************************
// Command mode processors
//******************************************************************
void telnet_client::KickPlayer		(void)
{
}

//-------------------------------------------------------------------
void telnet_client::SwitchTeam		(void)
{
}

//-------------------------------------------------------------------
void telnet_client::EnableProperty	(void)
{
	char ch;

	ch = WaitForCommand("Property to Enable", 
						"vkcsab",	
						s_HelpMessage[HM_ENABLE],YELLOW,RED);
	switch(ch)
	{
	case 0:
	default:
		break;
	}
}

//-------------------------------------------------------------------
void telnet_client::DisableProperty	(void)
{
	char ch;

	ch = WaitForCommand("Property to Disable", 
						"vkcsab",	
						s_HelpMessage[HM_DISABLE],YELLOW,RED);
	switch(ch)
	{
	case 0:
	default:
		break;
	}
}

//-------------------------------------------------------------------
void telnet_client::ServerAdmin		(void)
{
}



//******************************************************************
// Main loop, thread body
//******************************************************************

void telnet_client::Main(void)
{
	s32  i;
	char ch;

	m_Display.Clear();
	m_Display.SetColor(YELLOW);
	m_Display.Print("Welcome to the control console for Tribes : Aerial Assault\n");
	m_Display.Print("Version 1.00 compiled %s at %s\n",__DATE__,__TIME__);
	m_Display.SetColor();

	m_Authenticated = TRUE;
	while (!m_Authenticated)
	{
		if (m_Retries == 0)
		{
			Stop();
		}
		m_Display.Print("\nUsername: ");
		x_strcpy(m_Username,m_Display.GetLine());
		if (m_Username[0])
		{
			m_Display.Print("\nPassword: ");
			x_strcpy(m_Password,m_Display.GetLine(TRUE));

			i = 0;
			while(s_Usernames[i].Name != NULL)
			{
				if ( (x_strcmp(s_Usernames[i].Name,m_Username)==0) &&
					 (x_strcmp(s_Usernames[i].Password,m_Password)==0) )
				{
					m_Authenticated = TRUE;
					break;
				}
				i++;
			}

			if (!m_Authenticated)
			{
				m_Display.Print("\nInvalid Username or Password\n");
				m_Retries--;
			}
		}
	}

	if (!m_Authenticated)
	{
		m_Display.Print("Error: Disconnected due to invalid username or password.\n");
		Stop();
	}
	else
	{
		m_Display.Clear();
		SampleGameState();
		Refresh(TRUE);
	}


	while( !m_Exit )
	{
		ch = WaitForCommand("Command", 
							"ksedqapm",	
							s_HelpMessage[HM_MAIN],
							YELLOW,
							BLUE);
		switch(ch)
		{
		case 'K':
			KickPlayer();
			break;
		case 'S':
			SwitchTeam();
			break;
		case 'E':
			EnableProperty();
			break;
		case 'D':
			DisableProperty();
			break;
		case 'A':
			ServerAdmin();
			break;
		case 'Q':
			m_Display.Clear();
			m_Display.Print("Game over man! Game Over!\nGoodbye and thank you for hosting Tribes:AA\n");
			m_Exit = TRUE;
			break;
		}
	}
	Stop();
}

//-------------------------------------------------------------------
// Main update functions
//-------------------------------------------------------------------
void  telnet_client::RefreshLogText(xbool ForceRefresh)
{
	if (ForceRefresh)
	{
		m_Display.SetColor();
		m_Display.Clear();
		m_LastLogTextIndex=-1;
	}

	if (m_LastLogTextIndex != m_LogTextIndex)
	{
		s32 index = m_LogTextIndex;
		s32 i;

		for (i=m_Display.GetHeight()-1;i>=0;i--)
		{
			index--;
			if (index<0)
			{
				index = TELNET_LOG_HISTORY-1;
			}
			m_LogText[index][m_Display.GetWidth()]=0x0;
			m_Display.ClearLine(i);
			if(m_LogText[index][0])
			{
				m_Display.PrintXY(0,i,"%2.2f: %s",m_LogTextTime[index],m_LogText[index]);
			}

		}
	}

	m_LastLogTextIndex = m_LogTextIndex;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
xbool telnet_client::RefreshPlayers(s32 PlayerIndex, xbool ForceRefresh)
{
	s32 LineCount;
	s32 i;
	const game_score& Score = GameMgr.GetScore();
	client_stats Stats;
	s32 PlayerCount=0;

	m_Display.SetColor(YELLOW,BLUE);
	if (ForceRefresh)
	{
		m_Display.Clear();
		// Draw title bar
		m_Display.SetColor(YELLOW,BLUE);
		m_Display.SetJustify(CENTER);
		m_Display.PrintXY( 0+(14/2),-1,"NAME");
		m_Display.PrintXY(14+(6/2),-1,"SCORE");
		m_Display.PrintXY(20+(6/2),-1,"KILLS");
		m_Display.PrintXY(26+(6/2),-1,"VOTES");
		m_Display.PrintXY(32+(6/2),-1,"CONN");

		m_Display.PrintXY(39+ 0+(14/2),-1,"NAME");
		m_Display.PrintXY(39+14+(6/2),-1,"SCORE");
		m_Display.PrintXY(39+20+(6/2),-1,"KILLS");
		m_Display.PrintXY(39+26+(6/2),-1,"VOTES");
		m_Display.PrintXY(39+32+(6/2),-1,"CONN");
		m_Display.SetJustify();
		// Force a refresh on the data
		x_memset(&m_LastGameScore,0,sizeof(m_LastGameScore));
		m_LastPlayerCount = -1;
	}

	LineCount = m_Display.GetHeight();

	s32 LeftLine;
	s32 RightLine;
	s32 xpos,ypos;

	LeftLine = 0;
	RightLine = 0;

	if (Score.IsTeamBased)
	{
		s32 x,y;
		m_Display.SetColor(GREEN,RED);
		m_Display.PrintXY(0,0,"STORM - SCORE : %d",Score.Team[0].Score);
		m_Display.GetXY(x,y);
		m_Display.ClearChars(39-x);
		m_Display.SetColor(BLUE,GREEN);
		m_Display.PrintXY(39,0,"INFERNO - SCORE : %d",Score.Team[1].Score);
		m_Display.GetXY(x,y);
		m_Display.ClearChars(m_Display.GetWidth()-x);
		LeftLine++;
		RightLine++;
	}
	m_Display.SetColor(YELLOW,BLUE);
	for (i=PlayerIndex;i<32;i++)
	{
		if (Score.Player[i].IsConnected)
		{
			PlayerCount++;
			// Decide where on the display this player is going to be.
			if ( (Score.Player[i].Team==0) || (!Score.IsTeamBased && (LeftLine<=RightLine)))
			{
				xpos = 0;
				ypos = LeftLine;
				LeftLine++;
				if (LeftLine > m_Display.GetHeight())
				{
					LeftLine = m_Display.GetHeight();
					continue;
				}
			}
			else
			{
				xpos = 39;
				ypos = RightLine;
				RightLine++;
				if (RightLine > m_Display.GetHeight())
				{
					RightLine = m_Display.GetHeight();
					continue;
				}
			}

			xbool Changed = x_memcmp(&Score.Player[i],&m_LastGameScore.Player[i],sizeof(Score.Player[i]))!=0;
			// Only draw the player information if this one has been changed
			if ( Changed )
			{
				m_Display.GotoXY(xpos,ypos);
				m_Display.ClearChars(39);
				m_Display.SetJustify(CENTER);
				m_Display.PrintXY(xpos+ 0+(14/2),ypos,"%ls",Score.Player[i].Name);
				m_Display.PrintXY(xpos+14+(6/2),ypos,"%d",Score.Player[i].Score);
				m_Display.PrintXY(xpos+20+(6/2),ypos,"%d",Score.Player[i].Kills);
				m_Display.PrintXY(xpos+26+(6/2),ypos,"%d",0);
				// Now go through the client list to find the client the player is connected to
				if (Score.Player[i].Machine)
				{
					char ipstr[64];

					net_IPToStr(Stats.Address.IP,ipstr);
					m_Display.PrintXY(xpos+32+(6/2),ypos,"%d",Score.Player[i].Machine-1);
				}
				else
				{
					m_Display.PrintXY(xpos+32+(6/2),ypos,"%s",Score.Player[i].IsBot?"BOT":"LOCAL");
				}
				m_Display.SetJustify();
			}
		}
	}


	if (ForceRefresh || (LeftLine != m_LastLeftLine) )
	{
		m_LastLeftLine = LeftLine;
		while (LeftLine < m_Display.GetHeight())
		{
			m_Display.GotoXY(0,LeftLine);
			m_Display.ClearChars(39);
			LeftLine++;
		}
	}

	if (ForceRefresh || (RightLine != m_LastRightLine))
	{
		m_LastRightLine = RightLine;
		while (RightLine < m_Display.GetHeight())
		{
			m_Display.GotoXY(39,RightLine);
			m_Display.ClearChars(39);
			RightLine++;
		}
	}

	m_LastGameScore = Score;
	return (PlayerCount==0);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
xbool telnet_client::RefreshClients(s32 ClientIndex,xbool ForceRefresh)
{
	s32 LineCount;
	s32 i;
	client_stats Stats;
	s32 ClientCount=0;

	if (m_ClientCount < ClientIndex)
		return TRUE;

	m_Display.SetColor(WHITE,RED);
	if (ForceRefresh)
	{
		m_Display.Clear();
		m_Display.SetColor(WHITE,RED);
		// Draw title bar
		m_Display.SetJustify(CENTER);
		m_Display.PrintXY( 0+(4/2),-1,"CONN");
		m_Display.PrintXY( 4+(22/2),-1,"ADDRESS");
		m_Display.PrintXY(26+(7/2),-1,"INT.");
		m_Display.PrintXY(33+(7/2),-1,"IN");
		m_Display.PrintXY(40+(7/2),-1,"OUT");
		m_Display.PrintXY(47+(7/2),-1,"PING");
		m_Display.PrintXY(54+(8/2),-1,"DURATION");
		m_Display.PrintXY(63+(16/2),-1,"PLAYER NAME");

		m_Display.SetJustify();
		m_LastLeftLine = -1;
		m_LastRightLine = -1;
	}

	LineCount = m_Display.GetHeight();

	s32 xpos,ypos;

	xpos = 0;
	ypos = 0;
	for (i=ClientIndex;i<MAX_CLIENTS;i++)
	{
		s32 ConnId = MAX_CLIENTS-1-i;
		xbool IsPresent;
		IsPresent = tgl.pServer->GetClientStats(ConnId,Stats);

		if (IsPresent)
		{
			ClientCount++;
			char ipstring[64];
			char Player1Name[32];
			char Player2Name[32];
			x_strcpy(Player1Name,"Dummy Name");
			x_strcpy(Player2Name,"Dummy Name");

			net_IPToStr(Stats.Address.IP,ipstring);
			s32 time;
			s32 hours,minutes,seconds;

			time = (s32)Stats.ConnectTime;

			seconds = (s32)(time % 60);	time = time / 60;
			minutes = (s32)(time % 60);	time = time / 60;
			hours	= (s32)time;

			m_Display.ClearLine(ypos);
			m_Display.SetJustify(CENTER);
			m_Display.PrintXY(xpos+ 0+(4/2),ypos, "%d",ConnId);
			m_Display.PrintXY(xpos+ 4+(22/2),ypos,"%s:%d",ipstring,Stats.Address.Port);
			m_Display.PrintXY(xpos+26+(7/2),ypos,"%2.02f",Stats.ShipInterval);
			m_Display.PrintXY(xpos+33+(7/2),ypos,"%d",(s32)((Stats.BytesIn-m_LastBytesIn[i])/m_RefreshInterval));
			m_Display.PrintXY(xpos+40+(7/2),ypos,"%d",(s32)((Stats.BytesOut-m_LastBytesOut[i])/m_RefreshInterval));
			m_Display.PrintXY(xpos+47+(7/2),ypos,"%d",(s32)Stats.Ping);
			m_Display.PrintXY(xpos+54+(9/2),ypos,"%d:%02d:%02d",hours,minutes,seconds);
			m_Display.PrintXY(xpos+63+(16/2),ypos,"%s",Player1Name);

			m_Display.SetJustify();
			ypos++;
		}
	}

	while (ypos<m_Display.GetHeight())
	{
		m_Display.ClearLine(ypos);
		ypos++;
	}

	m_Display.SetColor();
	return (ClientCount==0);
}


//-------------------------------------------------------------------
//-------------------------------------------------------------------
extern s32 g_ClientsDropped;
extern s32 g_ClientsConnected;
void telnet_client::RefreshServer(xbool ForceRefresh)
{
	client_stats Stats;
	s32 ClientCount=0;
	s32 ClientIndex = 0;
	s32 ypos=0;

	static char* GameTypes[]=
	{
		"None",
		"CTF",
		"CNH",
		"TDM",
		"DM",
		"HUNT",
		"CAMP",
	};

	s32 PacketsSent,BytesSent,PacketsReceived;
	s32 BytesReceived,AddressesBound;
	f32 SendTime,ReceiveTime;

	net_GetStats(PacketsSent,BytesSent,PacketsReceived,BytesReceived,
				 AddressesBound,SendTime,ReceiveTime);
	m_Display.SetColor(BLUE,GREEN);
	if (ForceRefresh)
	{
		m_Display.Clear();
		m_Display.SetColor(BLUE,GREEN);
	}

	ypos = 0;
	m_Display.ClearLine(ypos);
	m_Display.PrintXY(0,ypos,	"  GAME TYPE: %s",GameTypes[GameMgr.GetGameType()]);;
	m_Display.PrintXY(26,ypos,  "   MAP NAME: %s",tgl.MissionName);
	m_Display.PrintXY(52,ypos,  "      GAMES: %d",0);

	ypos++;
	m_Display.ClearLine(ypos);
	m_Display.PrintXY(0,ypos,   "   PROGRESS: %d%%",(s32)(GameMgr.GetGameProgress()*100.2f));
	m_Display.PrintXY(26,ypos,	" GAME STATE: %s",sm_GetState());

	ypos++;
	m_Display.ClearLine(ypos);
	m_Display.PrintXY(0,ypos,	"  CONNECTED: %d",g_ClientsConnected);
	m_Display.PrintXY(26,ypos,	"   BYTES IN: %d",BytesReceived);
	m_Display.PrintXY(52,ypos,	"  BYTES OUT: %d",BytesSent);

	ypos++;
	m_Display.ClearLine(ypos);
	m_Display.PrintXY(0,ypos,	"    DROPPED: %d",g_ClientsDropped);
	m_Display.PrintXY(26,ypos,	"     IN/SEC: %d",(s32)((BytesReceived-m_LastBytesReceived)/m_RefreshInterval));
	m_Display.PrintXY(52,ypos,	"    OUT/SEC: %d",(s32)((BytesSent-m_LastBytesSent)/m_RefreshInterval));

	ypos++;
	m_Display.ClearLine(ypos);
//	m_Display.PrintXY(0,ypos,	"     LOGINS: %d",0);
	m_Display.PrintXY(26,ypos,  " PACKETS IN: %d",PacketsReceived);
	m_Display.PrintXY(52,ypos,  "PACKETS OUT: %d",PacketsSent);

	ypos++;
	m_Display.ClearLine(ypos);
//	m_Display.PrintXY(0,ypos,	"     FAILED: %d",0);
	m_Display.PrintXY(26,ypos,  " PKT IN/SEC: %d",(s32)((PacketsReceived-m_LastPacketsReceived)/m_RefreshInterval));
	m_Display.PrintXY(52,ypos,  "PKT OUT/SEC: %d",(s32)((PacketsSent-m_LastPacketsSent)/m_RefreshInterval));

	ypos++;
	m_Display.ClearLine(ypos);
	m_Display.PrintXY(0,ypos,	"    LOOKUPS: %d",pSvrMgr->GetLookupRequests());
	m_Display.PrintXY(26,ypos,  "     UNIQUE: %d",pSvrMgr->GetUniqueCount());

	m_Display.SetColor();
}


//-------------------------------------------------------------------
// Updates all internal logging of the game state to get the deltas
// between updates of data change.
void telnet_client::SampleGameState(void)
{
	s32 i;
	// Update client tracking
	client_stats Stats;
	m_ClientCount = 0;
	for (i=0;i<MAX_CLIENTS;i++)
	{
		s32 ConnId = MAX_CLIENTS-1-i;

		if (tgl.pServer && tgl.pServer->GetClientStats(ConnId,Stats))
		{
			m_LastBytesIn[i]=Stats.BytesIn;
			m_LastBytesOut[i]=Stats.BytesOut;
			m_ClientCount ++;
		}
		else
		{
			m_LastBytesIn[i]=0x0;
			m_LastBytesOut[i]=0x0;
		}
	}

	
	// Update server tracking
	s32 AddressesBound;
	f32 SendTime,ReceiveTime;

	net_GetStats(m_LastPacketsSent,m_LastBytesSent,
				 m_LastPacketsReceived,m_LastBytesReceived,
				 AddressesBound,SendTime,ReceiveTime);
}


const char* s_MainMenuHelp[]=
{
	" Main Menu Help\n\n",
	" K - Kick Player   S - Switch Teams  E - Enable Feature D - Disable Feature\n",
	" Q - Quit          A - Server Admin\n",
	" 1 - Players 1 to 16 compact mode    2 - Players 17 to 32 compact mode\n",
	" 3 - Details for clients 1 to 16     4 - Details for clients 17 to 32\n",
	" 5 - Server Game Statistics          6 - Extended log\n",
	" <TAB> will cycle through these in order.\n",
	NULL
};

const char* s_AdminMenuHelp[]=
{
	//----
	" Server Administration Help\n\n",
	" P - Set maximum player count             C - Set Maximum client count\n",
	" B - Set maximum number of bots           D - Set game duration\n",
	" R - Master server registration interval  T - Throttle threshold\n",
	" I - Maximum packet interval\n",
	NULL
};

const char* s_EnableMenuHelp[]=
{
	//----
	" Property Enable or Disable Help\n\n"
	" V - Map change vote K - Kick Request    C - Voice Chat      A - Auto Lock\n"
	" B - Bots            P - Password        J - Ability to join\n"
	" R - Unregister with Master Server when full.",
	NULL
};

const char** s_HelpMessage[HM_TOTAL]=
{
	s_MainMenuHelp,
	s_AdminMenuHelp,
	s_EnableMenuHelp,
};


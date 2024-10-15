#include "x_files.hpp"
#include "iopaud_main.hpp"
#include "sifrpc.h"
#include "sifdev.h"
#include "stdlib.h"
#include "stdio.h"
#include "libgraph.h"

#define AUDIO_REMOTE_MEM_SIZE	(5*1024*1024)

char* SearchPath[]=
{
	"host:"XCORE_PATH"/3rdparty/ps2/sn/iop/modules",
#ifdef X_DEBUG
	"host:ps2-iop-debug",
#else
	"host:ps2-iop-release",
#endif
	"host:"XCORE_PATH"/entropy/ps2/modules",
	NULL
};

char *KernelError(int status)
{
    switch (status)
    {
    case -200:
        return "KE_LINKERR";
        break;
    case -201:
        return "KE_ILLEGAL_OBJECT";
        break;
    case -202:
        return "KE_UNKNOWN_MODULE";
        break;
    case -203:
        return "KE_NOFILE";
        break;
    case -204:
        return "KE_FILEERR";
        break;
    case -205:
        return "KE_MEMINUSE";
        break;
	case -400:
		return "KE_NO_MEMORY";
		break;
    default:
        break;
    }
    return "<unknown>";
}

void LoadModule(const char* pFilename)
{
	char**	pSearch;
	char	path[256];
	int		retcode;

	pSearch = SearchPath;

	while(*pSearch)
	{
		sprintf(path,"%s/%s",*pSearch,pFilename);
		retcode = sceSifLoadModule(path,0,NULL);
		if (retcode != -203)
			break;
		pSearch++;
	}
	if (retcode < 0)
	{
		scePrintf("LoadModule(%s): sceSifLoadModule returned error %d(%s)\n",pFilename,retcode,KernelError(retcode));
		asm("break 0x001");
	}
}

static sceSifClientData    s_ClientData	   PS2_ALIGNMENT(64);
static u32				   s_OutBuffer[512/sizeof(u32)] PS2_ALIGNMENT(64);
static u32				   s_InBuffer [512/sizeof(u32)] PS2_ALIGNMENT(64);

void audio_Init(void* pBase, s32 Length)
{
	s32 result;

	ASSERT(pBase);
	ASSERT(Length);

	// We first bind the sifrpc call then
	// we send an init request
    while (1) 
    {
	    result = sceSifBindRpc (&s_ClientData, AUDIO_RPC_DEV, 0);
        ASSERTS(result>=0,"error: sceSifBindRpc failed");

	    if (s_ClientData.serve != 0) break;
    }
	s_OutBuffer[0]=(u32)pBase;
	s_OutBuffer[1]=Length;
	FlushCache(0);
    sceSifCallRpc (&s_ClientData,  
                    AUDCMD_INIT,0,
                    s_OutBuffer,sizeof(s_OutBuffer),
                    s_InBuffer,sizeof(s_InBuffer),
                    NULL,NULL);
	x_DebugMsg("EE side audio init complete");
	x_strcpy((char*)s_OutBuffer,"host:testfile.vag");
    sceSifCallRpc (&s_ClientData,  
                    AUDCMD_LOAD,0,
                    s_OutBuffer,sizeof(s_OutBuffer),
                    s_InBuffer,sizeof(s_InBuffer),
                    NULL,NULL);

	s_OutBuffer[0]=s_InBuffer[0];
    sceSifCallRpc (&s_ClientData,  
                    AUDCMD_PLAY,0,
                    s_OutBuffer,sizeof(s_OutBuffer),
                    s_InBuffer,sizeof(s_InBuffer),
                    NULL,NULL);

}

int main(void)
{

	x_Init();
	sceSifInitRpc(0);
	LoadModule("sndbgext.irx");
	LoadModule("libsd.irx");
	LoadModule("iopframe.irx");
	audio_Init(malloc(AUDIO_REMOTE_MEM_SIZE),AUDIO_REMOTE_MEM_SIZE);
	while(1)
	{
		sceGsSyncV(0);
	}
	x_Kill();
}
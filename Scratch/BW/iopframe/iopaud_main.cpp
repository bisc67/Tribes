#include "x_files.hpp"
#include "x_threads.hpp"
#include "iopaud_main.hpp"
#include "virtualmap.hpp"
#include <sys/file.h>
#include <stdio.h>
extern "C"
{
	#include "kernel.h"
	#include "sysmem.h"
};


audio_mgr g_Audio;

//*****************************************************************************
// Sets up basic initializing
void audio_mgr::Init(void)
{
	ASSERT(!m_Initialized);
	InitThreads();
	m_Initialized = TRUE;
}

//*****************************************************************************
// Sets up extended init (mem tables etc)
void audio_mgr::Init(void* BufferBase, s32 Length)
{
	s32				RemoteLength;
	s32				LocalLength;
	s32				AramLength;

	ASSERT(m_Initialized);

	m_TransferLock = 0;	// This will eventually be a semaphore
	// 1MB from local memory
	LocalLength = AUDIO_LOCAL_SIZE;
	AramLength  = AUDIO_RAM_END-AUDIO_RAM_START;
	RemoteLength= Length;
	// We assume 8M wanted for audio
	m_pLocalMemory		 = (byte*)x_malloc(AUDIO_LOCAL_SIZE);
	ASSERT(m_pLocalMemory);

	m_Mapper.Init(RemoteLength+AramLength+LocalLength,AUDIO_PAGE_SIZE);
	m_Mapper.Donate((void*)AUDIO_RAM_START,AramLength,MEMTYPE_AUDIO);
	m_Mapper.Donate(m_pLocalMemory,LocalLength,MEMTYPE_LOCAL);
	m_Mapper.Donate((void*)BufferBase,RemoteLength,MEMTYPE_REMOTE);

	InitTransferAgent();
	x_DebugMsg("audio_mgr::Init() System initialized. %dKB audio memory available.\n",m_Mapper.GetMemSize()/1024);
	x_DebugMsg("audio_mgr::Init() %dKB of system memory available.\n",QueryTotalFreeMemSize()/1024);
}

//*****************************************************************************
void audio_mgr::Kill(void)
{
	ASSERT(m_Initialized);
	KillThreads();
	m_Initialized = FALSE;
	x_free(m_pLocalMemory);
}

//*****************************************************************************
s32 audio_mgr::Load(const char* pFilename)
{
	u8* vaddr = NULL;
	s32 fp;
	u32 Length;
	u32 FileSize;


	fp = open(pFilename,O_RDONLY);
	if (fp<0)
	{
		ASSERT(FALSE);
		return -1;
	}

	Length = lseek(fp,0,SEEK_END);
	FileSize = Length;
	lseek(fp,0,SEEK_SET);

	BREAK;
	LockTransferBuffer();

	xtimer xfer;
	xfer.Reset();

	while(Length)
	{
		u32 Size;

		Size = sizeof(m_TransferBuffer);
		if (Size > Length)
		{
			Size = Length;
		}
		read(fp,m_TransferBuffer,Size);
		xfer.Start();
		TransferFromBuffer(vaddr);
		xfer.Stop();
		Length-=Size;
		//vaddr+=Size;
	}
	UnlockTransferBuffer();
	xtick t;
	s32 time;
	t= xfer.Read();
	time = (s32)(t/x_GetTicksPerMs());
	x_DebugMsg("Took %d.%d sec to transfer %d bytes\n",time/1000,time%1000,FileSize);
		BREAK;

	(void)pFilename;
	return 0;
}

//*****************************************************************************
void audio_mgr::Play(s32 Base)
{
	(void)Base;
}


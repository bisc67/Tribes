#include "x_files.hpp"
#include "x_threads.hpp"
#include "iopaud_main.hpp"

extern "C"
{
	#include "sifrpc.h"
};

sceSifQueueData s_RpcQueueData	PS2_ALIGNMENT(16);
sceSifServeData s_RpcServerData PS2_ALIGNMENT(16);
u8				s_RpcBuffer[1024] PS2_ALIGNMENT(16);


//*****************************************************************************
void* s_CommandDispatch(u32 Command, void* pData, s32 Size)
{
	return g_Audio.ProcessCommand(Command, pData, Size);
}

//*****************************************************************************
void s_CommandThread(void)
{
    sceSifInitRpc (0);
    sceSifSetRpcQueue (&s_RpcQueueData, x_GetCurrentThread()->GetSystemId());
    sceSifRegisterRpc (&s_RpcServerData, AUDIO_RPC_DEV, s_CommandDispatch, s_RpcBuffer, NULL, NULL, &s_RpcQueueData);
    sceSifRpcLoop (&s_RpcQueueData);
}

//*****************************************************************************
void s_PeriodicUpdateThread(void)
{
	while(1)
	{
		g_Audio.PeriodicUpdate();
	}
}

void audio_mgr::PeriodicUpdate(void)
{
	page_entry* pPage;

	while(1)
	{
		x_BeginAtomic();
		// see if there are any pending transfers from iop->spu, or iop->ee
		if ( (pPage = m_Mapper.LRU_Remove(MEMTYPE_LOCAL,VM_QUEUE_DIRTY)) )
		{
		}
		else if ( (pPage = m_Mapper.LRU_Remove(MEMTYPE_AUDIO,VM_QUEUE_DIRTY)) )
		{
		}
		else if ( (pPage = m_Mapper.LRU_Remove(MEMTYPE_REMOTE,VM_QUEUE_DIRTY)) )
		{
		}
		// see if there are any pending transfers from spu->iop
		// see if there are any pending transfers from ee->iop
		x_EndAtomic();
		// Wait 10ms for the next transfers to start
		x_DelayThread(10);
	}
}


//*****************************************************************************
void audio_mgr::InitThreads(void)
{
	m_pCommandHandler = new xthread(s_CommandThread,"Audio Command Handler",8192,1);
	m_pPeriodicUpdate = new xthread(s_PeriodicUpdateThread,"Audio Periodic Updater",8192,2);
//	m_pTransferHandler= new xthread(s_TransferThread,"EE/IOP/SPU Transfer thread",4096,3);
}

//*****************************************************************************
void audio_mgr::KillThreads(void)
{
	delete m_pCommandHandler;
	delete m_pPeriodicUpdate;

	m_pCommandHandler = NULL;
	m_pPeriodicUpdate = NULL;
}

//*****************************************************************************
void* audio_mgr::ProcessCommand(u32 Command, void* pData, s32 Length)
{
	s32* pData32=(s32*)pData;

	(void)Command;
	(void)pData;
	(void)Length;

	switch(Command)
	{
	//-----------------------------------------------------
	case AUDCMD_INIT:
		Init((void*)pData32[0],pData32[1]);
		break;
	//-----------------------------------------------------
	case AUDCMD_KILL:
		Kill();
		break;
	//-----------------------------------------------------
	case AUDCMD_LOAD:
		pData32[0]=Load((char*)pData);
		break;
	//-----------------------------------------------------
	case AUDCMD_PLAY:
		Play(pData32[0]);
		break;
	//-----------------------------------------------------
	default:
		ASSERT(FALSE);
		break;
	}
	return pData;
}


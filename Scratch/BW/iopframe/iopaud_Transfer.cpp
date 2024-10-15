#include "x_files.hpp"
#include "x_threads.hpp"
#include "iopaud_main.hpp"

extern "C"
{
	#include "kernel.h"
	#include "libsd.h"
	#include "sif.h"
	#include "sifrpc.h"
};
//*****************************************************************************
//*****************************************************************************
// This is a prototype of the audio management layer just to check the proof
// of concept. If this comment is still in the final version of this code, I'll
// be pissed! There are no overlapped DMA transfers and no background DMA
// transfer thread to deal with it. Like I say, very inefficient but the base
// mechanism should be sound.
//*****************************************************************************
//*****************************************************************************
#define TRANS_DIR_READ		0
#define TRANS_DIR_WRITE		1
extern void aram_Transfer(void* pLocal, void* pRemote, s32 Length, s32 Direction);
extern void remote_Transfer(void* pLocal, void* pRemote, s32 Length, s32 Direction);

s32 s_SpuDmaSemaphore;
s32 s_eeDmaSemaphore;

//---------------------------------------------------------
void		s_SpuDmaComplete(void)
{
	iSignalSema(s_SpuDmaSemaphore);
}

//---------------------------------------------------------
void s_eeDmaComplete(void)
{
	iSignalSema(s_eeDmaSemaphore);
}

//*****************************************************************************
void			audio_mgr::InitTransferAgent(void)
{
	// Initialize spu dma and ee dma transfer managers
	// THIS IS ALL VERY TEMPORARY
    struct SemaParam sema;
	
	sema.initCount = 0;
    sema.maxCount = 1;
    sema.attr = AT_THFIFO;

	s_SpuDmaSemaphore = CreateSema(&sema);
	s_eeDmaSemaphore = CreateSema(&sema);

    // Initialize the sound hardware.
    sceSdInit(0);
    sceSdSetCoreAttr(SD_C_SPDIF_MODE,(SD_SPDIF_MEDIA_DVD | SD_SPDIF_OUT_PCM | SD_SPDIF_COPY_NORMAL));
    sceSdSetTransIntrHandler (0, (sceSdSpu2IntrHandler) s_SpuDmaComplete, NULL);
}

//*****************************************************************************
s32				audio_mgr::TransferToBuffer(void* pSource)
{
	page_entry* pPhys;

	pPhys = m_Mapper.VirtualToPhysical(pSource);
	if (!pPhys)
		return 0;

	switch(pPhys->Type)
	{
	case MEMTYPE_LOCAL:
		x_memcpy(m_TransferBuffer,pPhys->Address,sizeof(m_TransferBuffer));
		break;

	case MEMTYPE_AUDIO:
		aram_Transfer(m_TransferBuffer,pPhys->Address,sizeof(m_TransferBuffer),TRANS_DIR_READ);
		break;

	case MEMTYPE_REMOTE:
		remote_Transfer(m_TransferBuffer,pPhys->Address,sizeof(m_TransferBuffer),TRANS_DIR_READ);
		break;
	default:
		ASSERT(FALSE);
	}

	return (s32)sizeof(m_TransferBuffer);

}

//*****************************************************************************
s32				audio_mgr::TransferFromBuffer(void* pDest)
{
	page_entry* pPhys;

	pPhys = m_Mapper.VirtualToPhysical(pDest);
	if (!pPhys)
		return 0;

	switch(pPhys->Type)
	{
	case MEMTYPE_LOCAL:
		x_memcpy(pPhys->Address,m_TransferBuffer,sizeof(m_TransferBuffer));
		break;

	case MEMTYPE_AUDIO:
		aram_Transfer(m_TransferBuffer,pPhys->Address,sizeof(m_TransferBuffer),TRANS_DIR_WRITE);
		break;

	case MEMTYPE_REMOTE:
		remote_Transfer(m_TransferBuffer,pPhys->Address,sizeof(m_TransferBuffer),TRANS_DIR_WRITE);
		break;
	default:
		ASSERT(FALSE);
	}


	return (s32)sizeof(m_TransferBuffer);

}

//*****************************************************************************
s32				audio_mgr::Transfer(void* pSource, void* pDest, s32 Length)
{
	page_entry* pSrcPhys;
	page_entry* pDstPhys;
	s32 Transferred;
	s32 BlockSize;

	Transferred = 0;

	while(Length)
	{
		BlockSize = MIN(Length,AUDIO_PAGE_SIZE);

		pSrcPhys = m_Mapper.VirtualToPhysical(pSource);
		pDstPhys = m_Mapper.VirtualToPhysical(pDest);
		if (!pSrcPhys || !pDstPhys)
		{
			return Transferred;
		}

		switch (pSrcPhys->Type)
		{
		//---------------------------------------------------------------------
		case MEMTYPE_LOCAL:
			switch(pDstPhys->Type)
			{
			//-----------------------------------------------------------------
			case MEMTYPE_LOCAL:							// Local to local just requires a remap
				break;
			//-----------------------------------------------------------------
			case MEMTYPE_AUDIO:							// Local to ARAM doesn't require a transfer buffer
				aram_Transfer(pSrcPhys->Address,pDstPhys->Address,BlockSize,TRANS_DIR_WRITE);
				break;
			//-----------------------------------------------------------------
			case MEMTYPE_REMOTE:						// Local to Remote doesn't require a transfer buffer
				remote_Transfer(pSrcPhys->Address,pDstPhys->Address,BlockSize,TRANS_DIR_WRITE);
				break;
			//-----------------------------------------------------------------
			default:
				ASSERT(FALSE);
			}
			break;

		//---------------------------------------------------------------------
		case MEMTYPE_AUDIO:
			switch(pDstPhys->Type)
			{
			//-----------------------------------------------------------------
			case MEMTYPE_LOCAL:							// ARAM to local requires no buffer
				aram_Transfer(pSrcPhys->Address,pDstPhys->Address,BlockSize,TRANS_DIR_READ);
				break;
			//-----------------------------------------------------------------
			case MEMTYPE_AUDIO:							// ARAM to ARAM just requires a remap
				break;
			//-----------------------------------------------------------------
			case MEMTYPE_REMOTE:						// ARAM to Remote does require a buffer
				LockTransferBuffer();
				aram_Transfer(pSrcPhys->Address,m_TransferBuffer,BlockSize,TRANS_DIR_READ);
				remote_Transfer(m_TransferBuffer,pDstPhys->Address,BlockSize,TRANS_DIR_WRITE);
				UnlockTransferBuffer();
				break;
			//-----------------------------------------------------------------
			default:
				ASSERT(FALSE);
			}
			break;

		//---------------------------------------------------------------------
		case MEMTYPE_REMOTE:
			switch(pDstPhys->Type)
			{
			//-----------------------------------------------------------------
			case MEMTYPE_LOCAL:							// Remote to local requires no buffer
				remote_Transfer(pSrcPhys->Address,pDstPhys->Address,BlockSize,TRANS_DIR_WRITE);
				break;
			//-----------------------------------------------------------------
			case MEMTYPE_AUDIO:							// Remote to ARAM does require a buffer
				LockTransferBuffer();
				remote_Transfer(pSrcPhys->Address,m_TransferBuffer,BlockSize,TRANS_DIR_READ);
				aram_Transfer(m_TransferBuffer,pDstPhys->Address,BlockSize,TRANS_DIR_WRITE);
				UnlockTransferBuffer();
				break;
			//-----------------------------------------------------------------
			case MEMTYPE_REMOTE:						// Remote to Remote just requires a remap
				break;
			//-----------------------------------------------------------------
			default:
				ASSERT(FALSE);
			}
			break;
		//---------------------------------------------------------------------
		default:
			ASSERT(FALSE);
			break;
		}
		m_Mapper.SwapMapping(pSource,pDest);

		pSource = (void*)( ((u32)pSource)+BlockSize);
		pDest   = (void*)( ((u32)pDest)+BlockSize);

		Length-=BlockSize;
		Transferred += BlockSize;
	}
	return Transferred;
}

//*****************************************************************************
void			audio_mgr::LockTransferBuffer(void)
{
	ASSERT(!m_TransferLock);
	m_TransferLock = TRUE;
}

//*****************************************************************************
void			audio_mgr::UnlockTransferBuffer(void)
{
	ASSERT(m_TransferLock);
	m_TransferLock = FALSE;
}

//*****************************************************************************
void aram_Transfer(void* pLocal, void* pRemote, s32 Length, s32 Direction)
{
    s32 status;
    s32 BlockSize;

    FlushDcache();
    Length = (Length + 15) & ~15;
    ASSERT( ((s32)pLocal & 15) == 0);
    ASSERT( ((s32)pRemote & 15) == 0);

	if (Direction == TRANS_DIR_WRITE)
	{
		Direction = SD_TRANS_MODE_WRITE | SD_TRANS_BY_DMA;
	}
	else
	{
		Direction = SD_TRANS_MODE_READ | SD_TRANS_BY_DMA;
	}

    while (Length)
    {
        BlockSize = MIN(Length,4*1024);
   
        status = sceSdVoiceTrans(0,
                    Direction,
                    (u8*)pLocal,
                    (u32)pRemote,
                    BlockSize);
		WaitSema(s_SpuDmaSemaphore);
        Length -= BlockSize;
        pLocal = (void*)( ((s32)pLocal) + BlockSize);
        pRemote = (void*)( ((s32)pRemote) + BlockSize);
    }
}

//*****************************************************************************
void remote_Transfer(void* pLocal, void* pRemote, s32 Length, s32 Direction)
{
	sceSifDmaData fdma;
	int id;
	s32 oldstat;

	// Of course, Sony in their infinite wisdom created two seperate routines
	// to transfer from the EE to IOP. IOP to SPU just needed one. I just
	// don't get it :). And to top it all, it's synchronous!
	//
	// This won't matter in the final state of these routines. If we have a situation
	// where memory needs to be swapped between EE and SPU, then we can overlap the reads
	// and writes.
	if (Direction == TRANS_DIR_WRITE)
	{
		Length = (Length+15)&~15;
		fdma.data = (s32)pLocal;
		fdma.addr = (s32)pRemote;
		fdma.size = Length;
		fdma.mode = 0;
		FlushDcache();
		CpuSuspendIntr(&oldstat);
		id = sceSifSetDma(&fdma, 1);
		CpuResumeIntr(oldstat);
		while(0 <= sceSifDmaStat(id));
	}
	else
	{
		sceSifReceiveData bd;

		sceSifGetOtherData(&bd,pRemote,pLocal,Length,0);

	}
}

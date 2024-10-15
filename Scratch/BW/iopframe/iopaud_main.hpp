#ifndef __IOPAUD_MAIN_HPP
#define __IOPAUD_MAIN_HPP

#include "x_files.hpp"
#include "x_threads.hpp"
#include "virtualmap.hpp"


#define AUDIO_RPC_DEV	0x1234abcd
#define AUDIO_PAGE_SIZE	32768
#define AUDIO_RAM_START	(24*1024)
#define AUDIO_RAM_END	(2048*1024)
#define AUDIO_LOCAL_SIZE	(1024*1024)

enum
{
	AUDCMD_INIT		= 0x11110000,
	AUDCMD_KILL,
	AUDCMD_LOAD,
	AUDCMD_UNLOAD,
	AUDCMD_PLAY,
};

void*	s_CommandDispatch(u32 Command, void* pData, s32 Size);

class audio_mgr
{
public:
						audio_mgr(void)
						{
							m_Initialized = FALSE;
						};
						~audio_mgr(void)
						{
							ASSERT(!m_Initialized);
						};

		void			Init(void);
		void			Kill(void);
		// Try and lock a page. It can only be locked if it is in audio memory and
		// calling this function will initiate a transfer to make sure it is in
		// audio ram. Returns false if the lock failed, true if it succeeded.
		xbool			Lock(u32 VirtualAddress, s32 Length);
		// Try to unlock a page. If the pages are locked by more than one call,
		// then it cannot be released yet and the function will return false.
		// If the page is now unlocked and able to be moved, then it returns true.
		xbool			Unlock(u32 VirtualAddress, s32 Length);
private:
		void			Init					(void* BufferBase, s32 Length);
		// Transfer buffer needs to be first because, for some reason, the alignment
		// directive isn't working properly
		byte			m_TransferBuffer[AUDIO_PAGE_SIZE] PS2_ALIGNMENT(16);
		xbool			m_Initialized;
		xthread*		m_pPeriodicUpdate;
		xthread*		m_pCommandHandler;
		virtual_map		m_Mapper;

		void*			m_pLocalMemory;			// Ptr to local IOP memory for buffering (to be released)
		
		s32				m_TransferLock;

		void			InitThreads				(void);
		void			KillThreads				(void);
		void*			ProcessCommand			(u32 Command, void* pData, s32 Length);

		void			LockTransferBuffer		(void);
		void			UnlockTransferBuffer	(void);
		s32				TransferToBuffer		(void* pSource);
		s32				TransferFromBuffer		(void* pDest);
		s32				Transfer				(void* pSource, void* pDest, s32 Length);
		void			InitTransferAgent		(void);
		void			PeriodicUpdate			(void);

		// Just test commands for now
		s32				Load					(const char* pFilename);
		void			Play					(s32 Base);

		// END

		friend	void*	s_CommandDispatch(u32 Command, void* pData, s32 Size);
		friend	void	s_PeriodicUpdateThread(void);
};

extern audio_mgr g_Audio;
 
void	iopaud_Init(void);
void	iopaud_Kill(void);
#endif